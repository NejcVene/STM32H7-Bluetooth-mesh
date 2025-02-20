/*
 * node_config.c
 *
 *  Created on: Sep 8, 2024
 *      Author: Nejc
 */

#include "node_config.h"
#include "device_settings.h"
#include "hash_table.h"
#include "cmsis_os2.h"
#include "freertos_os2.h"
#include <string.h>
#include <inttypes.h>

#ifdef _DEBUG
#include "lib_utils.h"
#endif

/*
 * What could be updated here:
 * 	- upgrade to hash tables instead of arrays,
 * 	- add saving of provisioned nodes to long term storage (flash, NAND, NOR ...)
 */

/*
 * When a unprovisioned node is found, it is assigned a temporary address.
 * When a node gets provisioned, it gets assigned its proper network address.
 * This is why two addresses are used.
 */

// macros to clear values
#define CLEAR_NODE_ADDRESSES(nodeAddressValue) (Node_NetworkAddress_t) 				\
											   { .nodeAddress = (nodeAddressValue), \
												 .uuid = "",						\
												 .nodeModels = 0,					\
												 .nodeFeatures = 0					\
											   }
#define CLEAR_NODE_STATES() (Node_ModelState_t)				\
							{.genericOnOffStatus = 0,		\
							 .genericPowerOnOffStatus = 0,	\
							 .levelSetValue = 0				\
							}
#define CLEAR_NODE_SENSORS() (Node_Sensor_t)			\
							{.numOfSensors = 0,			\
							 .sensorArray = NULL		\
							}
#define CLEAR_NODE_CONFIG() (Node_Config_t) 								\
							{.subscriptions = 0, 							\
							.nodeName = "",									\
							.address = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL),	\
							.states = CLEAR_NODE_STATES(),					\
							.sensors = CLEAR_NODE_SENSORS()					\
							}

void NC_ReportItems(uint8_t uuid, NC_MaskedFeatures *items, uint16_t *report);
uint8_t NC_HexCharToInt(char c);

// supported models (always ends with NULL, 0, 0)
NC_MaskedFeatures models[] = {
		{.name = "O",	.bitmask = NC_GENERIC_ON_OFF_MODEL,			.value = SF_GEN_ON_OFF_SERVER_MODEL_ID},
		{.name = "E",	.bitmask = NC_GENERIC_LEVEL_MODEL,			.value = SF_GEN_LEVEL_SERVER_MODEL_ID},
		{.name = "S",	.bitmask = NC_SENSOR_MODEL,					.value = SF_SENSOR_SERVER_MODEL_ID},
		{.name = "S",	.bitmask = NC_SENSOR_SETUP_MODEL,			.value = SF_SENSOR_SETUP_SERVER_MODEL_ID},
		{.name = "L",	.bitmask = NC_LIGH_LIGHTNESS_MODEL,			.value = SF_LIGHT_LIGHTNESS_SERVER_MODEL_ID},
		{.name = "L",	.bitmask = NC_LIGH_LIGHTNESS_SETUP_MODEL,	.value = SF_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL_ID},
		{.name = "P",	.bitmask = NC_GENERIC_POWER_ON_OFF_MODEL,	.value = SF_GEN_POWER_ON_OFF_SERVER_MODEL_ID},
		{.name = "P",	.bitmask = NC_GENERIC_POWER_SETUP_MODEL,	.value = SF_GEN_POWER_ON_OFF_SETUP_SERVER_MODEL_ID},
		{.name = "V",	.bitmask = NC_VENDOR_MODEL_1,				.value = SF_MAYBE_VENDOR_MODEL_ID},
		{.name = NULL,	.bitmask = 0,								.value = 0}
};
// supported features (always ends with NULL, 0, 0)
NC_MaskedFeatures features[] = {
		{.name = "R",	.bitmask = NC_RELAY_FEATURE,				.value = 0},
		{.name = "P", 	.bitmask = NC_PROXY_FEATURE,				.value = 0},
		{.name = "F", 	.bitmask = NC_FRIEND_FEATURE,				.value = 0},
		{.name = "E", 	.bitmask = NC_EMBEDDED_PROVISIONER_FEATURE,	.value = 0},
		{.name = NULL,	.bitmask = 0}
};
// defined group addresses (always ends with NULL, 0, 0)
NC_MaskedFeatures groupAddress[] = {
		{.name = "Default",  	.bitmask = GROUP_ADDRESS_DEFAULT_BIT,		.value = GROUP_ADDRESS_DEFAULT},
		{.name = "Kitchen",		.bitmask = GROUP_ADDRESS_KITCHEN_BIT,		.value = GROUP_ADDRESS_KITCHEN},
		{.name = "Living room",	.bitmask = GROUP_ADDRESS_LIVING_ROOM_BIT,	.value = GROUP_ADDRESS_LIVING_ROOM},
		{.name = "Bedroom",		.bitmask = GROUP_ADDRESS_BEDROOM_BIT,		.value = GROUP_ADDRESS_BEDROOM},
		{.name = "Bathroom",	.bitmask = GROUP_ADDRESS_BATHROOM_BIT,		.value = GROUP_ADDRESS_BATHROOM},
		{.name = NULL,			.bitmask = 0,								.value = 0}
};

Node_NetworkAddress_t nodeAddresses[5];
Node_Config_t nodeConfigs[MAX_CONF_NODES];
HT_HashTable_t *modelsData;
static uint32_t numOfConfiguredNodes;
static int deviceConfiguredFlag;
char nodeModelFeatureStr[10];

/**
  * @brief  Initialize required arrays for node configuration.
  * @note	For debug purposes, can simulate nodes in a network.
  * @retval	None
  */
void NC_Init(void) {

//	modelsData = HT_Create(7, 11);
	for (int i = 0; i<5; i++) {
		nodeAddresses[i] = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL);
		nodeConfigs[i] = CLEAR_NODE_CONFIG();
	}
#ifdef _SIMULATE_CONFIG_NODE
	nodeConfigs[0] = (Node_Config_t) {
		.subscriptions = 3,
		.nodeName = "My node",
		.address = {
			.nodeAddress = 43,
			.nodeModels = 83,
			.nodeFeatures = 0,
			.uuid =  "0753"
		},
		.states = {
			.genericOnOffStatus = 0,
			.genericPowerOnOffStatus = 0
		}
	};
	nodeConfigs[1] = (Node_Config_t) {
			.subscriptions = 5,
			.nodeName = "My node2",
			.address = {
				.nodeAddress = 74,
				.nodeModels = 1,
				.nodeFeatures = 0,
				.uuid =  "0756"
			},
			.states = {
				.genericOnOffStatus = 0,
				.genericPowerOnOffStatus = 0
			}
	};
	NC_IncrementNumOfConfNodes();
	NC_IncrementNumOfConfNodes();
#endif
//	for (int i = 0; models[i].name != NULL; i++) {
//		HT_Insert(modelsData, (int) models[i].name, &models[i]);
//	}

}

/**
  * @brief  Reset timer for timeout.
  * @param  param Pointer to node parameters (node address and UUID).
  * @retval	None
  */
void NC_ReportFoundNodes(char *param) {

	int i = 0;
	uint32_t index;
	char uuid[UUID_SAVE_LEN + 1];
	char *token;
	char *rest = param;
	Node_NetworkAddress_t tmp;

	// parse input param with strtok_r
	// which is separated by ';'
	while ((token = strtok_r(rest, ";", &rest))) {
		tmp = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL);
		sscanf(token, "%" PRIu32 "-%s", &index, uuid);
		tmp.nodeAddress = index;
		strncpy(tmp.uuid, uuid, UUID_SAVE_LEN);
		tmp.uuid[UUID_SAVE_LEN] = '\0';
		nodeAddresses[i++] = tmp;
	}

}

/**
  * @brief  Check for enabled models and features for found nodes.
  * @note	This will run for all found nodes in nodeAdresses array.
  * @retval	None
  */
void NC_CheckEnabledModelsFeatures() {

	for (int i = 0; i<5; i++) {
		if (nodeAddresses[i].nodeAddress != NODE_DEF_VAL) {
			NC_ReportItems((NC_HexCharToInt(nodeAddresses[i].uuid[NODE_FEATURE_BYTE]) << 4) | NC_HexCharToInt(nodeAddresses[i].uuid[NODE_FEATURE_BYTE + 1]), features, &nodeAddresses[i].nodeFeatures);
			NC_ReportItems((NC_HexCharToInt(nodeAddresses[i].uuid[NODE_MODELS_BYTE]) << 4) | NC_HexCharToInt(nodeAddresses[i].uuid[NODE_MODELS_BYTE + 1]), models, &nodeAddresses[i].nodeModels);
		}
	}

}

/**
  * @brief  For the correct UUID and bitmask bit, add bit to report.
  * @note	Used for adding correct bits to nodeFeatures and nodeModels.
  * @param	uuid	UUID of a node.
  * @param	items	Pointer to either features or models array, which contains
  * 				features/models definitions.
  * @param	report	Where to save constructed bitmask. Either nodeFeatures or nodeModels.
  * @retval	None
  */
void NC_ReportItems(uint8_t uuid, NC_MaskedFeatures *items, uint16_t *report) {

	for (int i = 0; items[i].name != NULL; i++) {
		if (uuid & (items[i].bitmask)) {
			*report |= items[i].bitmask;
		}
	}

}

/**
  * @brief	For inputed node, add correct subscription (address) bitmask.
  * @note	Can print debug information (address subscribed too).
  * @param	node	Pointer to a node.
  * @param	bitmask	bitmask value.
  * @retval	None
  */
void NC_AddSubscription(Node_Config_t *node, uint16_t bitmask) {

#ifdef _DEBUG
	char buffer[100];
	sprintf(buffer, "Sub add: %d", bitmask);
	debugMessage(buffer, strlen(buffer), PRINT_CHAR);
#endif
	for (int i = 0; groupAddress[i].name != NULL; i++) {
		if (groupAddress[i].bitmask == bitmask) {
			node->subscriptions |= groupAddress[i].bitmask;
			return;
		}
	}

}

/**
  * @brief	For inputed node, remove subscription (address) bitmask.
  * @note	Can print debug information (address removed).
  * @param	node	Pointer to a node.
  * @param	bitmask	bitmask value.
  * @retval	None
  */
void NC_RemoveSubscription(Node_Config_t *node, uint16_t bitmask) {

#ifdef _DEBUG
	char buffer[100];
	sprintf(buffer, "Sub remove: %d", bitmask);
	debugMessage(buffer, strlen(buffer), PRINT_CHAR);
#endif
	for (int i = 0; groupAddress[i].name != NULL; i++) {
		if (groupAddress[i].bitmask == bitmask) {
			node->subscriptions &= ~groupAddress[i].bitmask;
			return;
		}
	}

}

/**
  * @brief	Convert HEX value, which is a character to decimal.
  * @param	c	HEX value represented as a character.
  * @retval	uint8_t decimal value.
  */
uint8_t NC_HexCharToInt(char c) {

    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }

    return 0;
}

/**
  * @brief	Return node Node_NetworkAddress_t with inputed address.
  * @note	Returns only node information from Node_NetworkAddress_t struct.
  * @param	address	Node address.
  * @retval	Node_NetworkAddress_t pointer.
  */
Node_NetworkAddress_t *NC_GetNodeFromAddress(uint32_t address) {

	for (int i = 0; i<5; i++) {
		if (nodeAddresses[i].nodeAddress == address) {
			return &nodeAddresses[i];
		}
	}

	return NULL;

}

/**
  * @brief	Return node Node_Config_t with inputed address (provisioned node).
  * @note	Should be used with provisioned nodes, as node addressed differ before
  * 		and after provisioning.
  * @param	address	Node address.
  * @retval	Node_Config_t pointer.
  */
Node_Config_t *NC_GetConfigNodeFromAddress(uint32_t nodeAddress) {

	for (int i = 0; i<5; i++) {
		if (nodeConfigs[i].address.nodeAddress == nodeAddress) {
			return &nodeConfigs[i];
		}
	}

	return NULL;

}

/**
  * @brief	Return node Node_NetworkAddress_t from index.
  * @param	index Node index.
  * @retval	Node_NetworkAddress_t pointer.
  */
Node_NetworkAddress_t *NC_GetNodeNetworkAddress(int index) {

	return &nodeAddresses[index];

}

/**
  * @brief	Return pointer to Node_NetworkAddress_t array.
  * @retval	Node_NetworkAddress_t pointer.
  */
Node_NetworkAddress_t *NC_GetNodeNetworkAddressArray(void) {

	return nodeAddresses;

}

/**
  * @brief	Return pointer to Node_Config_t array.
  * @retval	Node_Config_t pointer.
  */
Node_Config_t *NC_GetNodeConfigArray(void) {

	return nodeConfigs;

}

/**
  * @brief	Get the number of provisioned nodes.
  * @retval	uint32_t value.
  */
uint32_t NC_GetNumOfConfNodes(void) {

	return numOfConfiguredNodes;

}

/**
  * @brief	Increment the number of provisioned nodes.
  * @retval	None.
  */
void NC_IncrementNumOfConfNodes(void) {

	numOfConfiguredNodes++;

}

/**
  * @brief	Decrement the number of provisioned nodes.
  * @retval	None.
  */
void NC_DecrementNumOfConfNodes(void) {

	numOfConfiguredNodes--;

}

/**
  * @brief	Get the number of defined models in models array.
  * @retval	int value.
  */
int NC_GetNumOfModels(void) {

	int count = 0;
	while (models[count].name != NULL) {
		count++;
	}

	return count;

}

/**
  * @brief	Return pointer to NC_MaskedFeatures array of models.
  * @retval	NC_MaskedFeatures pointer.
  */
NC_MaskedFeatures *NC_GetAllModels(void) {

	return models;

}

/**
  * @brief	Return pointer to NC_MaskedFeatures array of features.
  * @retval	NC_MaskedFeatures pointer.
  */
NC_MaskedFeatures *NC_GetAllFeatures(void) {

	return features;

}

/**
  * @brief	Return pointer to NC_MaskedFeatures array of group addresses.
  * @retval	NC_MaskedFeatures pointer.
  */
NC_MaskedFeatures *NC_GetAllGroupAddresses(void) {

	return groupAddress;

}

/**
  * @brief	Get the number of bits set to 1 in a number (population count).
  * @note	Uses built in operation for popcount, which should execute as one
  * 		instruction.
  * @param	bitmask	Value to get popcount from.
  * @retval	int value.
  */
int NC_GetPopCount(uint16_t bitmask) {

	return __builtin_popcount(bitmask);

}

/**
  * @brief	Get node features or models as NC_MaskedFeatures pointer.
  * @param	maskedFeatures Pointer to NC_MaskedFeatures.
  * @param	featureBitmask Bitmask from node (nodeModels or nodeFeatures)
  * @retval	NC_MaskedFeatures pointer.
  */
NC_MaskedFeatures *NC_GetNodeFeature(NC_MaskedFeatures *maskedFeatures, uint16_t featureBitmask) {

	NC_MaskedFeatures *tmp = NULL;
	uint16_t count = 0;
	int numOfModels;
	int index = 0;

	count = NC_GetPopCount(featureBitmask);
	if (count > 0) {
		if (!(tmp = (NC_MaskedFeatures *) pvPortMalloc(count * sizeof(NC_MaskedFeatures)))) {
			// raise error
		}
		numOfModels = NC_GetNumOfModels();
		for (int i = 0; i<numOfModels; i++) {
			if (maskedFeatures[i].bitmask & featureBitmask) {
				memcpy(&tmp[index], &maskedFeatures[i], sizeof(NC_MaskedFeatures));
				index++;
			}
		}
	}

	return tmp;

}

/**
  * @brief	Get model name and features from bitmask.
  * @note	Names of all models and features supported by a node whose bitmask was passed in.
  * @param	maskedFeatures	NC_MaskedFeatures pointer to all models or all features.
  * @param	nodeInfo		Models or features of a node
  * @retval	character pointer.
  */
char *NC_GetNodeModelsFeaturesString(NC_MaskedFeatures *maskedFeatures, uint16_t nodeInfo) {

	memset(nodeModelFeatureStr, 0, 10);
	for (int i = 0; maskedFeatures[i].name != NULL; i++) {
		if (maskedFeatures[i].bitmask & nodeInfo) {
			strcat(nodeModelFeatureStr, maskedFeatures[i].name);
		}
	}

	return nodeModelFeatureStr;

}

/**
  * @brief	Add model bitmask to node.
  * @param	node			Node_NetworkAddress_t pointer for a node.
  * @param	modelBitmask	Bitmask for a model.
  * @retval	None.
  */
void NC_AddModel(Node_NetworkAddress_t *node, uint32_t modelBitmask) {

	node->nodeModels |= modelBitmask;

}

/**
  * @brief	Add missing models.
  * @note	A node supports one model, but does not specify others.
  * 		Some models, like Sensor model, have with it tied other models
  * 		like, Sensor setup model. UUID only specifies one of them.
  * 		Because of this, we have to fill in for the missing models.
  * @param	node Node_NetworkAddress_t pointer for a node.
  * @retval	None.
  */
void NC_FillMissingNodeModels(Node_NetworkAddress_t *node) {

	if (node->nodeModels & NC_SENSOR_MODEL) {
		NC_AddModel(node, NC_SENSOR_SETUP_MODEL);
	}
	if (node->nodeModels & NC_LIGH_LIGHTNESS_MODEL) {
		NC_AddModel(node, NC_LIGH_LIGHTNESS_SETUP_MODEL);
	}
	if (node->nodeModels & NC_GENERIC_POWER_ON_OFF_MODEL) {
		NC_AddModel(node, NC_GENERIC_POWER_SETUP_MODEL);
	}


}

/**
  * @brief	Delete a provisioned node.
  * @note	Deletes a node by returning all of its values to default.
  * 		Also frees all sensor data and decrements the number
  * 		of provisioned nodes.
  * 		To be used in conjunction with unprovisioning process.
  * @param	nodeAddress Address to a node.
  * @retval	None.
  */
void NC_DeleteConfiguredNode(uint32_t nodeAddress) {

	if (NC_GetNumOfConfNodes() > 0) {
		for (int i = 0; i<5; i++) {
			if (nodeConfigs[i].address.nodeAddress == nodeAddress) {
				SN_FreeSensorInfo(&nodeConfigs[i].sensors);
				nodeConfigs[i] = CLEAR_NODE_CONFIG();
				NC_DecrementNumOfConfNodes();
				return;
			}
		}
	}

}

/**
  * @brief	Provision a unprovisioned node.
  * @note	Assigns new node address, adds data from Node_NetworkAddress_t,
  * 		adds default subscription, changes node name, fills missing models (if any),
  * 		increments the number of provisioned nodes and clears Node_NetworkAddress_t
  * 		array. To be used in conjunction with provisioning process.
  * 		Fails if the number of supported models is exceeded.
  * @param	nodeAddress Old address of a node.
  * @param	assignedNodeAddress	New address of the node, which is now provisioned
  * 		in the network.
  * @retval	int index of provisioned node in Node_Config_t array.
  */
int NC_ProvisionNode(uint32_t nodeAddress, uint32_t assignedNodeAddress) {

	Node_NetworkAddress_t *prvnNode = NC_GetNodeFromAddress(nodeAddress);

	if (NC_GetNumOfConfNodes() < 5) {
		for (int i = 0; i<5; i++) {
			if (nodeConfigs[i].address.nodeAddress == NODE_DEF_VAL) {
				nodeConfigs[i].address = *prvnNode;
				nodeConfigs[i].address.nodeAddress = assignedNodeAddress;
				NC_AddSubscription(&nodeConfigs[i], GROUP_ADDRESS_DEFAULT_BIT);
				NC_ChangeNodeName(&nodeConfigs[i], DEF_NODE_NAME);
				NC_FillMissingNodeModels(&nodeConfigs[i].address);
				NC_IncrementNumOfConfNodes();
				NC_ClearNodeNetworkAddressArray();
				return i;
			}
		}
	}

	return -1;

}

/**
  * @brief	Get value of a bitmask.
  * @param	maskedFeatures Pointer to NC_MaskedFeatures.
  * @param	bitmask	Bitmask value.
  * @retval	uint32_t value.
  */
uint32_t NC_GetValueFromBitmask(NC_MaskedFeatures *maskedFeatures, uint16_t bitmask) {

	for (int i = 0; maskedFeatures[i].name != NULL; i++) {
		if (maskedFeatures[i].bitmask == bitmask) {
			return maskedFeatures[i].value;
		}
	}

	return 0;

}

/**
  * @brief	Change the name of a node.
  * @param	node Pointer to Node_Config_t.
  * @param	newNodeName	Pointer to the new node name.
  * @retval	None.
  */
void NC_ChangeNodeName(Node_Config_t *node, const char *newNodeName) {

	if (!node || !newNodeName) {
		return;
	}

	memset(node->nodeName, 0, MAX_NODE_NAME);
	strncpy(node->nodeName, newNodeName, MAX_NODE_NAME - 1);
	node->nodeName[MAX_NODE_NAME - 1] = '\0';

}

/**
  * @brief	Set nodeAddresses (Node_NetworkAddress_t) array to default values.
  * @retval	None.
  */
void NC_ClearNodeNetworkAddressArray(void) {

	for (int i = 0; i<5; i++) {
		nodeAddresses[i] = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL);
	}

}

/**
  * @brief	Reset WB55 and H7.
  * @note	Using NRST and NVIC_SystemReset.
  * @retval	None.
  */
void NC_ResetDevice(void) {

	HAL_GPIO_WritePin(COMM_NRST_PORT_MASTER, COMM_NRST_PIN_MASTER, GPIO_PIN_RESET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(COMM_NRST_PORT_MASTER, COMM_NRST_PIN_MASTER, GPIO_PIN_SET);
	HAL_Delay(100);
	NVIC_SystemReset();

}

/**
  * @brief	Return device configured flag.
  * @retval	int value.
  */
int NC_IsDeviceConfigured(void) {

	return deviceConfiguredFlag;

}

/**
  * @brief	Set device configured flag.
  * @retval	None.
  */
void NC_SetDeviceConfiguredFlag(int val) {

	deviceConfiguredFlag = val;

}

//void NC_SaveNodeToNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address) {
//
//	HAL_NOR_Program(hnor, address, (uint16_t *) nodeConfig);
//
//}
//
//void NC_ReadNodeFromNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address) {
//
//	HAL_NOR_Read(hnor, address, (uint16_t *) nodeConfig);
//
//}

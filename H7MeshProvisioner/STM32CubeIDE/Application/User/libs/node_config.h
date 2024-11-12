/*
 * node_config.h
 *
 *  Created on: Sep 7, 2024
 *      Author: Nejc
 */

#ifndef INC_NODE_CONFIG_H_
#define INC_NODE_CONFIG_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MODELS_PER_NODE				5
#define MAX_NODE_NAME					10
#define NODE_DEF_VAL					100
#define UUID_SAVE_LEN					4
#define DEF_NODE_NAME					"New node"

#define NODE_FEATURE_BYTE				0
#define NODE_MODELS_BYTE				2

// predefined group addresses for rooms
#define GROUP_ADDRESS_DEFAULT			0xC000
#define GROUP_ADDRESS_KITCHEN			0xC001
#define GROUP_ADDRESS_LIVING_ROOM		0xC002
#define GROUP_ADDRESS_BEDROOM			0xC003
#define GROUP_ADDRESS_BATHROOM			0xC004
#define GROUP_ADDRESS_DEFAULT_BIT			(1 << 0)
#define GROUP_ADDRESS_KITCHEN_BIT			(1 << 1)
#define GROUP_ADDRESS_LIVING_ROOM_BIT		(1 << 2)
#define GROUP_ADDRESS_BEDROOM_BIT			(1 << 3)
#define GROUP_ADDRESS_BATHROOM_BIT  		(1 << 4)

// Bluetooth SIG model identifiers
// here are listed only the models which our application
// will support
#define SF_GEN_ON_OFF_SERVER_MODEL_ID				0x1000
#define SF_GEN_LEVEL_SERVER_MODEL_ID				0x1002
#define SF_GEN_POWER_ON_OFF_SERVER_MODEL_ID			0x1006
#define SF_GEN_POWER_ON_OFF_SETUP_SERVER_MODEL_ID	0x1007
#define SF_LIGHT_LIGHTNESS_SERVER_MODEL_ID			0x1300
#define SF_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL_ID	0x1301
#define SF_SENSOR_SERVER_MODEL_ID					0x1100
#define SF_SENSOR_SETUP_SERVER_MODEL_ID				0x1101
#define SF_MAYBE_VENDOR_MODEL_ID					0x10030

typedef struct {
	uint32_t nodeAddress;
	uint16_t nodeModels;
	uint16_t nodeFeatures;
	char uuid[UUID_SAVE_LEN + 1];
} Node_NetworkAddress_t;

typedef struct {
	uint8_t genericOnOffStatus : 1,
			genericPowerOnOffStatus : 1;
} Node_ModelState_t;

typedef struct {
	uint8_t subscriptions;
	char nodeName[MAX_NODE_NAME];
	Node_NetworkAddress_t address;
	Node_ModelState_t states;
} Node_Config_t;

typedef struct {
	const char *name;
	uint16_t bitmask;
	uint32_t value;
} NC_MaskedFeatures;

typedef struct {
	uint8_t subbTo; // flag to tell if the address is to be subsribed (1) or unsubscribed (0)
	uint32_t groupAddress;
} Node_SubbedAddress_t;

typedef struct {
	uint8_t numOfSubs;
	uint32_t nodeAddress;
	Node_SubbedAddress_t subbedAddresses[5];
} Node_SubscriptionParam_t;

void NC_Init(void);
void NC_ReportFoundNodes(char *param);
void NC_CheckEnabledModelsFeatures(void);
Node_NetworkAddress_t *NC_GetNodeFromAddress(uint32_t nodeAddress);
Node_NetworkAddress_t *NC_GetNodeNetworkAddressArray(void);
Node_NetworkAddress_t *NC_GetNodeNetworkAddress(int index);
Node_Config_t *NC_GetNodeConfigArray(void);
uint32_t NC_GetNumOfConfNodes(void);
void NC_IncrementNumOfConfNodes(void);
void NC_DecrementNumOfConfNodes(void);
void NC_AddSubscription(Node_Config_t *node, uint16_t bitmask);
void NC_RemoveSubscription(Node_Config_t *node, uint16_t bitmask);
int NC_GetNumOfModels(void);
NC_MaskedFeatures *NC_GetAllModels(void);
NC_MaskedFeatures *NC_GetAllFeatures(void);
NC_MaskedFeatures *NC_GetAllGroupAddresses(void);
NC_MaskedFeatures *NC_GetNodeFeature(NC_MaskedFeatures *maskedFeatures, uint16_t featureBitmask);
int NC_GetPopCount(uint16_t bitmask);
char *NC_GetNodeModelString(uint16_t nodeModels);
char *NC_GetNodeFeatureString(uint16_t nodeFeatures);
void NC_FillMissingNodeModels(Node_NetworkAddress_t *node);
void NC_AddModel(Node_NetworkAddress_t *node, uint32_t modelBitmask);
Node_Config_t *NC_GetConfigNodeFromAddress(uint32_t nodeAddress);
void NC_DeleteConfiguredNode(uint32_t nodeAddress);
int NC_ProvisionNode(uint32_t nodeAddress, uint32_t assignedNodeAddress);
uint32_t NC_GetValueFromBitmask(NC_MaskedFeatures *maskedFeatures, uint16_t bitmask);
void NC_ChangeNodeName(Node_Config_t *node, const char *newNodeName);
void NC_ClearNodeNetworkAddressArray(void);
//void NC_SaveNodeToNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address);
//void NC_ReadNodeFromNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address);

#ifdef __cplusplus
}
#endif

#endif /* INC_NODE_CONFIG_H_ */

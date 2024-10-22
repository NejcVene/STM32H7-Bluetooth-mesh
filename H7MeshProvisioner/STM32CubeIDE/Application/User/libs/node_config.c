/*
 * node_config.c
 *
 *  Created on: Sep 8, 2024
 *      Author: Nejc
 */

#include "node_config.h"
#include "device_settings.h"
#include "hash_table.h"
#include <string.h>
#include <inttypes.h>

#define CLEAR_NODE_ADDRESSES(nodeAddressValue) (Node_NetworkAddress_t) 				\
											   { .nodeAddress = (nodeAddressValue), \
												 .uuid = "",						\
												 .nodeModels = "",					\
												 .nodeFeatures = ""					\
											   }
#define CLEAR_NODE_CONFIG() (Node_Config_t) 								\
							{.subscriptions = 0, 							\
							.nodeName = "",									\
							.address = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL)	\
							}

void NC_ReportItems(uint8_t uuid, NC_MaskedFeatures *items, char *report);
uint8_t NC_HexCharToInt(char c);

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
NC_MaskedFeatures features[] = {
		{.name = "R",	.bitmask = NC_RELAY_FEATURE,				.value = 0},
		{.name = "P", 	.bitmask = NC_PROXY_FEATURE,				.value = 0},
		{.name = "F", 	.bitmask = NC_FRIEND_FEATURE,				.value = 0},
		{.name = "E", 	.bitmask = NC_EMBEDDED_PROVISIONER_FEATURE,	.value = 0},
		{.name = NULL,	.bitmask = 0}
};
NC_MaskedFeatures groupAddress[] = {
		{.name = "Default",  	.bitmask = GROUP_ADDRESS_DEFAULT_BIT,		.value = GROUP_ADDRESS_DEFAULT},
		{.name = "Kitchen",		.bitmask = GROUP_ADDRESS_KITCHEN_BIT,		.value = GROUP_ADDRESS_KITCHEN},
		{.name = "Living room",	.bitmask = GROUP_ADDRESS_LIVING_ROOM_BIT,	.value = GROUP_ADDRESS_LIVING_ROOM},
		{.name = "Bedroom",		.bitmask = GROUP_ADDRESS_BEDROOM_BIT,		.value = GROUP_ADDRESS_BEDROOM},
		{.name = "Bathroom",	.bitmask = GROUP_ADDRESS_BATHROOM_BIT,		.value = GROUP_ADDRESS_BATHROOM},
		{.name = NULL,			.bitmask = 0,								.value = 0}
};

Node_NetworkAddress_t nodeAddresses[5];
Node_Config_t nodeConfigs[5];
HT_HashTable_t *modelsData;
static uint32_t numOfConfiguredNodes;

void NC_Init(void) {

	modelsData = HT_Create(7, 11);
	for (int i = 0; i<5; i++) {
		nodeAddresses[i] = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL);
		nodeConfigs[i] = CLEAR_NODE_CONFIG();
	}
//	for (int i = 0; models[i].name != NULL; i++) {
//		HT_Insert(modelsData, (int) models[i].name, &models[i]);
//	}

}

void NC_ReportFoundNodes(char *param) {

	int i = 0;
	uint32_t index;
	char uuid[UUID_SAVE_LEN + 1];
	char *token;
	char *rest = param;
	Node_NetworkAddress_t tmp;

	while ((token = strtok_r(rest, ";", &rest))) {
		tmp = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL);
		sscanf(token, "%" PRIu32 "-%s", &index, uuid);
		tmp.nodeAddress = index;
		strncpy(tmp.uuid, uuid, UUID_SAVE_LEN);
		tmp.uuid[UUID_SAVE_LEN] = '\0';
		nodeAddresses[i++] = tmp;
	}

}

void NC_CheckEnabledModelsFeatures() {

	for (int i = 0; i<5; i++) {
		if (nodeAddresses[i].nodeAddress != NODE_DEF_VAL) {
			NC_ReportItems((NC_HexCharToInt(nodeAddresses[i].uuid[NODE_FEATURE_BYTE]) << 4) | NC_HexCharToInt(nodeAddresses[i].uuid[NODE_FEATURE_BYTE + 1]), features, nodeAddresses[i].nodeFeatures);
			NC_ReportItems((NC_HexCharToInt(nodeAddresses[i].uuid[NODE_MODELS_BYTE]) << 4) | NC_HexCharToInt(nodeAddresses[i].uuid[NODE_MODELS_BYTE + 1]), models, nodeAddresses[i].nodeModels);
		}
	}

}

void NC_ReportItems(uint8_t uuid, NC_MaskedFeatures *items, char *report) {

	for (int i = 0; items[i].name != NULL; i++) {
		if (uuid & (items[i].bitmask)) {
			strcat(report, items[i].name);
		}
	}

}

void NC_AddSubscription(Node_Config_t *node, uint32_t address) {

	for (int i = 0; groupAddress[i].name != NULL; i++) {
		if (groupAddress[i].value == address) {
			node->subscriptions |= groupAddress[i].bitmask;
		}
	}

}

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

Node_NetworkAddress_t *NC_GetNodeFromAddress(uint32_t address) {

	for (int i = 0; i<5; i++) {
		if (nodeAddresses[i].nodeAddress == address) {
			return &nodeAddresses[i];
		}
	}

	return NULL;

}

Node_NetworkAddress_t *NC_GetNodeNetworkAddress(int index) {

	return &nodeAddresses[index];

}

Node_NetworkAddress_t *NC_GetNodeNetworkAddressArray(void) {

	return nodeAddresses;

}

Node_Config_t *NC_GetNodeConfigArray(void) {

	return nodeConfigs;

}

uint32_t NC_GetNumOfConfModels(void) {

	return numOfConfiguredNodes;

}

void NC_IncrementNumOfConfModels(void) {

	numOfConfiguredNodes++;

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

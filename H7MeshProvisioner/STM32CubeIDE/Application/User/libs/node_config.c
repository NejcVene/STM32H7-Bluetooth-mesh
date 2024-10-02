/*
 * node_config.c
 *
 *  Created on: Sep 8, 2024
 *      Author: Nejc
 */

#include "node_config.h"
#include "device_settings.h"
#include <string.h>
#include <inttypes.h>

#define CLEAR_NODE_ADDRESSES(nodeAddressValue) (Node_NetworkAddress_t) 				\
											   { .nodeAddress = (nodeAddressValue), \
												 .uuid = "",						\
												 .nodeModels = "",					\
												 .nodeFeatures = ""					\
											   }

typedef struct {
	const char *name;
	uint8_t bitmask;
} NC_MaskedModelFeatures;

void NC_ReportItems(uint8_t uuid, NC_MaskedModelFeatures *items, char *report);
uint8_t NC_HexCharToInt(char c);

NC_MaskedModelFeatures models[] = {
		{.name = "O", .bitmask = NC_GENERIC_ON_OFF_MODEL},
		{.name = "E", .bitmask = NC_GENERIC_LEVEL_MODEL},
		{.name = "S", .bitmask = NC_SENSOR_MODEL},
		{.name = "L", .bitmask = NC_LIGH_LIGHTNESS_MODEL},
		{.name = NULL, .bitmask = 0}
};
NC_MaskedModelFeatures features[] = {
		{.name = "R", .bitmask = NC_RELAY_FEATURE},
		{.name = "P", .bitmask = NC_PROXY_FEATURE},
		{.name = "F", .bitmask = NC_FRIEND_FEATURE},
		{.name = "E", .bitmask = NC_EMBEDDED_PROVISIONER_FEATURE},
		{.name = NULL, .bitmask = 0}
};


Node_NetworkAddress_t nodeAddresses[5];
Node_Config_t nodeConfigs[5];

void NC_Init(void) {

	for (int i = 0; i<5; i++) {
		nodeAddresses[i] = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL);
	}

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

void NC_ReportItems(uint8_t uuid, NC_MaskedModelFeatures *items, char *report) {

	for (int i = 0; items[i].name != NULL; i++) {
		if (uuid & (items[i].bitmask)) {
			strcat(report, items[i].name);
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

Node_NetworkAddress_t *NC_GetNodeNetworkAddress(int index) {

	return &nodeAddresses[index];

}

Node_NetworkAddress_t *NC_GetNodeNetworkAddressArray(void) {

	return nodeAddresses;

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

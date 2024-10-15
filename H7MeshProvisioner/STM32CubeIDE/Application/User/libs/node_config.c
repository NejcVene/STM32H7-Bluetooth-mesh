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

typedef struct {
	const char *name;
	uint8_t bitmask;
	uint16_t identifier;
} NC_MaskedModelFeatures;

void NC_ReportItems(uint8_t uuid, NC_MaskedModelFeatures *items, char *report);
uint8_t NC_HexCharToInt(char c);

NC_MaskedModelFeatures models[] = {
		{.name = "O", .bitmask = NC_GENERIC_ON_OFF_MODEL, .identifier = 0},
		{.name = "E", .bitmask = NC_GENERIC_LEVEL_MODEL, .identifier = 0},
		{.name = "S", .bitmask = NC_SENSOR_MODEL, .identifier = 0},
		{.name = "L", .bitmask = NC_LIGH_LIGHTNESS_MODEL, .identifier = 0},
		{.name = NULL, .bitmask = 0, .identifier = 0}
};
NC_MaskedModelFeatures features[] = {
		{.name = "R", .bitmask = NC_RELAY_FEATURE, .identifier = 0},
		{.name = "P", .bitmask = NC_PROXY_FEATURE, .identifier = 0},
		{.name = "F", .bitmask = NC_FRIEND_FEATURE, .identifier = 0},
		{.name = "E", .bitmask = NC_EMBEDDED_PROVISIONER_FEATURE, .identifier = 0},
		{.name = NULL, .bitmask = 0, .identifier = 0}
};
Node_GroupAddress_t groupAddress[] = {
		{.addressName = "Kitchen", .address = GROUP_ADDRESS_KITCHEN},
		{.addressName = "Living room", .address = GROUP_ADDRESS_LIVING_ROOM},
		{.addressName = "Bedroom", .address = GROUP_ADDRESS_BEDROOM},
		{.addressName = "Bathroom", .address = GROUP_ADDRESS_BATHROOM}
};


Node_NetworkAddress_t nodeAddresses[5];
Node_Config_t nodeConfigs[5];
HT_HashTable_t *modelsData;

void NC_Init(void) {

	modelsData = HT_Create(7, 11);
	for (int i = 0; i<5; i++) {
		nodeAddresses[i] = CLEAR_NODE_ADDRESSES(NODE_DEF_VAL);
	}
	for (int i = 0; models[i].name != NULL; i++) {
		HT_Insert(modelsData, (int) models[i].name, &models[i]);
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

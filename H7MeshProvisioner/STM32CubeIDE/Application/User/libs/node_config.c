/*
 * node_config.c
 *
 *  Created on: Sep 8, 2024
 *      Author: Nejc
 */

#include "node_config.h"
#include <string.h>

#include <inttypes.h>

Node_NetworkAddress_t nodeAddresses[5];
Node_Config_t nodeConfigs[5];

void NC_ReportFoundNodes(char *param) {

	int i = 0;
	uint32_t index;
	char uuid[40];
	char *token = strtok(param, ";");
	Node_NetworkAddress_t tmp;

	while (token != NULL) {
		sscanf(token, "%" PRIu32 "-%s", &index, uuid);
		tmp.nodeAddress = index;
		strcpy(tmp.uuid, uuid);
		nodeAddresses[i++] = tmp;
		token = strtok(NULL, ";");
	}

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

/*
 * node_config.h
 *
 *  Created on: Sep 7, 2024
 *      Author: Nejc
 */

#ifndef INC_NODE_CONFIG_H_
#define INC_NODE_CONFIG_H_

#include "main.h"

/*
 * Each model is defined by exactly one bit:
 * 	- 0 0 0 0 0 0 0 0 -> no models defined
 * 	- 0 0 0 0 0 0 0 1 -> generic on/off model
 * 	- 0 0 0 0 0 0 1 0 -> generic level model
 * 	- 0 0 0 0 0 1 0 0 -> sensor model
 * 	- 0 0 0 0 1 0 0 0 -> light lightness model
 * 	- 0 0 0 1 0 0 0 0 -> light lightness setup model,
 * 	- 0 0 1 0 0 0 0 0 -> undefined
 * 	- 0 1 0 0 0 0 0 0 -> undefined
 * 	- 1 0 0 0 0 0 0 0 -> undefined
 * 	As per limitation of the STM BLE Mesh library, the maximum
 * 	number of models on a node, which are active at the same time
 * 	is 5.
 */

#define MAX_NODE_NAME					10
#define NODE_DEF_VAL					100
#define UUID_SAVE_LEN					4

#define NODE_FEATURE_BYTE				0
#define NODE_MODELS_BYTE				2

// predefined group addresses for rooms
#define GROUP_ADDRESS_KITCHEN			0xC001
#define GROUP_ADDRESS_LIVING_ROOM		0xC002
#define GROUP_ADDRESS_BEDROOM			0xC003
#define GROUP_ADDRESS_BATHROOM			0xC004

typedef struct {
	uint32_t nodeAddress;
	char nodeModels[5];
	char nodeFeatures[5];
	char uuid[UUID_SAVE_LEN + 1];
} Node_NetworkAddress_t;

typedef struct {
	uint8_t subscriptions;
	char nodeName[MAX_NODE_NAME];
	Node_NetworkAddress_t address;
} Node_Config_t;

typedef struct {
	char addressName[20];
	uint16_t address;
} Node_GroupAddress_t;

typedef struct {
	;
} Node_ModelIdentifier_t;

void NC_Init(void);
void NC_ReportFoundNodes(char *param);
void NC_CheckEnabledModelsFeatures(void);
Node_NetworkAddress_t *NC_GetNodeFromAddress(uint32_t nodeAddress);
Node_NetworkAddress_t *NC_GetNodeNetworkAddressArray(void);
Node_NetworkAddress_t *NC_GetNodeNetworkAddress(int index);
//void NC_SaveNodeToNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address);
//void NC_ReadNodeFromNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address);

#endif /* INC_NODE_CONFIG_H_ */

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

#define MAX_NODE_NAME				10
#define NODE_DEF_VAL				100
#define UUID_SAVE_LEN				4

#define GENERIC_ON_OFF_MODEL		(1 << 0)   // bit 0
#define GENERIC_LEVEL_MODEL			(1 << 1)   // bit 1
#define SENROR_MODEL				(1 << 2)   // bit 2
#define LIGH_LIGHTNESS_MODEL		(1 << 3)   // bit 3
#define LIGH_LIGHTNESS_SETUP_MODEL	(1 << 4)   // bit 4
#define UNDEFINED_MODEL_1			(1 << 5)   // bit 5
#define UNDEFINED_MODEL_2			(1 << 6)   // bit 6
#define UNDEFINED_MODEL_3			(1 << 7)   // bit 7

typedef struct {
	uint32_t nodeAddress;
	char uuid[UUID_SAVE_LEN + 1];
} Node_NetworkAddress_t;

typedef struct {
	char nodeName[MAX_NODE_NAME];
	uint8_t models;
	Node_NetworkAddress_t address;
} Node_Config_t;

void NC_Init(void);
Node_NetworkAddress_t *NC_GetNodeNetworkAddressArray(void);
Node_NetworkAddress_t *NC_GetNodeNetworkAddress(int index);
void NC_ReportFoundNodes(char *param);
//void NC_SaveNodeToNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address);
//void NC_ReadNodeFromNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address);

#endif /* INC_NODE_CONFIG_H_ */

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

#define NODE_FEATURE_BYTE				0
#define NODE_MODELS_BYTE				2

// predefined group addresses for rooms
#define GROUP_ADDRESS_DEFAULT			0xC000
#define GROUP_ADDRESS_KITCHEN			0xC001
#define GROUP_ADDRESS_LIVING_ROOM		0xC002
#define GROUP_ADDRESS_BEDROOM			0xC003
#define GROUP_ADDRESS_BATHROOM			0xC004

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
	char nodeModels[6];
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
	uint16_t identifier;
} Node_ModelIdentifier_t;

extern Node_GroupAddress_t groupAddress[];

void NC_Init(void);
void NC_ReportFoundNodes(char *param);
void NC_CheckEnabledModelsFeatures(void);
Node_NetworkAddress_t *NC_GetNodeFromAddress(uint32_t nodeAddress);
Node_NetworkAddress_t *NC_GetNodeNetworkAddressArray(void);
Node_NetworkAddress_t *NC_GetNodeNetworkAddress(int index);
Node_ModelIdentifier_t *NC_GetModelIdentifiers(void);
//void NC_SaveNodeToNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address);
//void NC_ReadNodeFromNOR(NOR_HandleTypeDef *hnor, Node_Config_t *nodeConfig, uint32_t address);

#ifdef __cplusplus
}
#endif

#endif /* INC_NODE_CONFIG_H_ */

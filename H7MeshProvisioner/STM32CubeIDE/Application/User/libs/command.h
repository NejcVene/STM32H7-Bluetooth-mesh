/*
 * command.h
 *
 *  Created on: Aug 9, 2024
 *      Author: Nejc
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include "communication_ex.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_MESH_COMMAND_LENGHT	30

typedef enum {
	PARAM_INT = 0,
	PARAM_CHAR,
	PARAM_INT_ARR,
	PARAM_VOID
} PARAMETER_TYPE;

typedef enum {
	CMD_MESH_ATEP_ROOT = 0,
	CMD_MESH_ATEP_SCAN,
	CMD_MESH_ATEP_PRVN,
	CMD_MESH_ATEP_SCAN_RANGE,
	CMD_MESH_ATEP_PRVN_RANGE,
	CMD_MESH_ATCL_GENERIC_ON_OFF_ACK,
	CMD_MESH_ATCL_GENERIC_ON_OFF_GET,
	CMD_MESH_ATCL_UNPROV,
	CMD_MESH_ATCL_SENSOR_GET,
	CMD_FUN_PROV_EM_PROV,
	CMD_FUN_IS_EM_PROV_PROV,
	CMD_FUN_PUB_SET_SUB_ADD,
	CMD_FUN_GET_LIB_VER,
	CMD_FUN_PRO_TEST,
	CMD_MESH_NUM_OF_CMD
} CMD_INDEX;

typedef struct {
	PARAMETER_TYPE type;
	union {
		int i;
		char *str;
		int *intArr;
		void *voidPtr;
	} value;
	int arrayLength;
	size_t elementSize;
} CMD_Parameter_t;

typedef struct {
	int commandIndex;
	int numOfParams;
	CMD_Parameter_t *param;
} CMD_CommandGet_t;

typedef struct {
	char command[CMD_MESH_COMMAND_LENGHT];
	PROTOCOL_MSG_TYPE commandType;
	PROTOCOL_DATATYPE dataType;
	void (*CMD_Setup)(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd);
	CMD_CommandGet_t *(*CMD_Execute)(void *buffer, CMD_CommandGet_t *guiCmd);
} CMD_MeshCommand_t;

// actual mesh commands
extern CMD_MeshCommand_t defineRootNetworkNode;
extern CMD_MeshCommand_t scanForUnprovisionedNetworkDevices;
extern CMD_MeshCommand_t provisionNetworkDevice;
extern CMD_MeshCommand_t scanForUnprovisionedNetworkDevicesOutOfRangePvrn;
extern CMD_MeshCommand_t provisionNetworkDeviceOutOfRangePvrn;
extern CMD_MeshCommand_t genericOnOffSetAck;
extern CMD_MeshCommand_t genericOnOffGet;
extern CMD_MeshCommand_t unprovisionNetworkDevice;
extern CMD_MeshCommand_t sensorGet;

// commands which are not part of mesh, but directly call functions
// on the embedded provisioner
extern CMD_MeshCommand_t unprovisionEmbeddedProv;
extern CMD_MeshCommand_t isEmbeddedProvProvisioned;
extern CMD_MeshCommand_t pubSetSubAdd;
extern CMD_MeshCommand_t getLibInfo;
extern CMD_MeshCommand_t protocolStructTest;

CMD_CommandGet_t *CMD_CreateCommandGet(CMD_INDEX cmdIndex, PARAMETER_TYPE types[], void *paramValues[], int numOfParams, int arrayLengths[], size_t *elementSizes);
void CMD_FreeCommandGet(CMD_CommandGet_t *cmd);

#ifdef __cplusplus
}
#endif

#endif /* INC_COMMAND_H_ */

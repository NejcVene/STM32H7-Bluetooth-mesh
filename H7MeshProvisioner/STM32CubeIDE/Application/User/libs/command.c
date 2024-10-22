/*
 * command.c
 *
 *  Created on: Aug 14, 2024
 *      Author: Nejc
 */

#include "command.h"
#include "node_config.h"
#include "cmsis_os2.h"
#include "freertos_os2.h"
#include <string.h>

void CMD_GenericFormatCommand(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd);
void CMD_SetupSubsAdd(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_NofitfyProvision(char *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_NotifyScan(char *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_SubsAdd(char *buffer, CMD_CommandGet_t *guiCmd);

CMD_MeshCommand_t defineRootNetworkNode = {
		.command = "ATEP ROOT",
		.commandType = PRO_MSG_TYPE_UNACK,
		.CMD_Setup = NULL,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t scanForUnprovisionedNetworkDevices = {
		.command = "ATEP SCAN",
		.commandType = PRO_MSG_TYPE_ACK,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_NotifyScan
};

CMD_MeshCommand_t provisionNetworkDevice = {
		.command = "ATEP PRVN-%d",
		.commandType = PRO_MSG_TYPE_ACK,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NofitfyProvision
};

CMD_MeshCommand_t scanForUnprovisionedNetworkDevicesOutOfRangePvrn = {
		.command = "ATEP NDSCAN",
		.commandType = PRO_MSG_TYPE_ACK,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_NotifyScan
};

CMD_MeshCommand_t provisionNetworkDeviceOutOfRangePvrn = {
		.command = "ATEP NDPRVN-%d",
		.commandType = PRO_MSG_TYPE_ACK,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t genericOnOffSetAck = { // currently hard-coded
		.command = "ATCL %d 8202 01 00",
		.commandType = PRO_MSG_TYPE_UNACK,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t genericOnOffSetAckOff = { // currently hard-coded
		.command = "ATCL %d 8202 00 00", // c000 for group address
		.commandType = PRO_MSG_TYPE_UNACK,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t genericOnOffGet = {
		.command = "ATCL 0002 8201",
		.commandType = PRO_MSG_TYPE_UNACK,
		.CMD_Setup = NULL,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t unprovisionEmbeddedProv = {
		.command = "BLEMesh_Unprovision",
		.commandType = PRO_MSG_TYPE_OTHER,
		.CMD_Setup = NULL,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t isEmbeddedProvProvisioned = {
		.command = "BLEMesh_IsUnprovisioned",
		.commandType = PRO_MSG_TYPE_OTHER,
		.CMD_Setup = NULL,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t subscriptionAdd = {
		.command = "BLEMesh_SubsAdd %d %d %d",
		.commandType = PRO_MSG_TYPE_OTHER,
		.CMD_Setup = CMD_SetupSubsAdd,
		.CMD_Execute = CMD_SubsAdd
};

CMD_MeshCommand_t publicationSet = {
		.command = "BLEMesh_ModelSet %d %d %d",
		.commandType = PRO_MSG_TYPE_OTHER,
		.CMD_Setup = NULL,
		.CMD_Execute = NULL
};

CMD_CommandGet_t *CMD_CreateCommandGet(CMD_INDEX cmdIndex, PARAMETER_TYPE types[], void *paramValues[], int numOfParams, int arrayLengths[], size_t *elementSizes) {

	CMD_CommandGet_t *cmd = NULL;
	int len;

	if (!(cmd = (CMD_CommandGet_t *) pvPortMalloc(sizeof(CMD_CommandGet_t)))) {
		// raise error
	}
	cmd->commandIndex = cmdIndex;
	cmd->numOfParams = numOfParams;
	if (!(cmd->param = (CMD_Parameter_t *) pvPortMalloc(numOfParams * sizeof(CMD_Parameter_t)))) {
		// raise error
	}
	for (int i = 0; i<numOfParams; i++) {
		cmd->param[i].type = types[i];
		cmd->param[i].arrayLength = 0;
		cmd->param[i].elementSize = 0;
		switch (types[i]) {
			case PARAM_INT:
				cmd->param[i].value.i = *((int *) paramValues[i]);
				break;
			case PARAM_CHAR:
				len = strlen((char *) paramValues[i]);
				cmd->param[i].value.str = (char *) pvPortMalloc((len + 1) * sizeof(char));
				strcpy(cmd->param[i].value.str, (char *) paramValues[i]);
				break;
			case PARAM_INT_ARR:
				cmd->param[i].value.intArr = (int *) pvPortMalloc(arrayLengths[i] * sizeof(int));
				memcpy(cmd->param[i].value.intArr, (int *) paramValues, arrayLengths[i] * sizeof(int));
				cmd->param[i].arrayLength = arrayLengths[i];
				break;
			case PARAM_VOID:
				cmd->param[i].elementSize = elementSizes[i];
				cmd->param[i].value.voidPtr = pvPortMalloc(arrayLengths[i] * elementSizes[i]);
				memcpy(cmd->param[i].value.voidPtr, paramValues[i], arrayLengths[i] * elementSizes[i]);
				break;
			default:
				break;
		}
	}

	return cmd;

}

void CMD_FreeCommandGet(CMD_CommandGet_t *cmd) {

	if (!cmd) return;
	for (int i = 0; i<cmd->numOfParams; i++) {
		switch (cmd->param[i].type) {
			case PARAM_INT:
				// nothing to do here as its directly
				// assigned and not allocated
				break;
			case PARAM_CHAR:
				vPortFree(cmd->param[i].value.str);
				break;
			case PARAM_INT_ARR:
				vPortFree(cmd->param[i].value.intArr);
				break;
			case PARAM_VOID:
				vPortFree(cmd->param[i].value.voidPtr);
				break;
			default:
				break;
		}
	}
	vPortFree(cmd->param);
	vPortFree(cmd);

}

void CMD_GenericFormatCommand(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd) {

	char *output = buffer;
	const char *t = cmdTemplate;

	for (int i = 0; i<guiCmd->numOfParams; i++) {
		while (*t && *t != '%') {
			*output++ = *t++;
		}
		if (*t == '%' && *(t + 1) == 'd') {
			if (guiCmd->param[i].type == PARAM_INT) {
				output += sprintf(output, "%d", guiCmd->param[i].value.i);
			} else if (guiCmd->param[i].type == PARAM_INT_ARR) {
				int len = guiCmd->param[i].arrayLength;
				int *arr = guiCmd->param[i].value.intArr;
				for (int j = 0; j<len; j++) {
					if (j > 0) {
						*output++ = ' ';
					}
					output += sprintf(output, "%d", arr[j]);
				}
			}
			t += 2;
		} else if (*t == '%' && *(t + 1)  == 's' && guiCmd->param->type == PARAM_CHAR) {
			output += sprintf(output, "%s", guiCmd->param[i].value.str);
			t += 2;
		}
	}
	*output = '\0';

}

void CMD_SetupSubsAdd(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd) {

	char *output = buffer;
	const char *t = cmdTemplate;
	static int innerIndex = 0;
	int counter = 0;

	for (int i = 0; i<guiCmd->numOfParams; i++) {
		while (*t && *t != '%') {
			*output++ = *t++;
		}
		if (*t == '%' && *(t + 1) == 'd' && guiCmd->param[i].type == PARAM_INT_ARR) {
			int len = guiCmd->param[i].arrayLength;
			int *arr = guiCmd->param[i].value.intArr;
			if (innerIndex < len) {
				for (; counter++<3; innerIndex++) {
					if (innerIndex > 0) {
						*output++ = ' ';
					}
					output += sprintf(output, "%d", arr[innerIndex]);
				}
			} else {
				innerIndex = 0;
			}
			t += 2;
		}
	}
	*output = '\0';

}

CMD_CommandGet_t *CMD_NofitfyProvision(char *buffer, CMD_CommandGet_t *guiCmd) {

	int index;
	PARAMETER_TYPE type = PARAM_VOID;
	void *paramValue[1];
	int arrayLength[] = {1};
	size_t sizes[] = {sizeof(Node_Config_t)};
	CMD_CommandGet_t *cmdRes = NULL;
	Node_NetworkAddress_t *prvnNode = NC_GetNodeFromAddress(guiCmd->param[0].value.i);
	Node_Config_t *configNodes = NC_GetNodeConfigArray();

	NC_IncrementNumOfConfModels();
	index = NC_GetNumOfConfModels() - 1;
	configNodes[index].address = *prvnNode;
	NC_AddSubscription(&configNodes[index], GROUP_ADDRESS_DEFAULT);
	paramValue[0] = (void *) &configNodes[index];
	cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
										&type,
										paramValue,
										1,
										arrayLength,
										sizes);

	return cmdRes;

}

CMD_CommandGet_t *CMD_NotifyScan(char *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_VOID;
	void *paramValue[1];
	int arrayLength[] = {5};
	size_t sizes[] = {sizeof(Node_NetworkAddress_t)};

	if (!strcmp(buffer, "NONE") && guiCmd->commandIndex == CMD_MESH_ATEP_SCAN) {
		guiCmd->commandIndex = CMD_MESH_ATEP_SCAN_RANGE;
	} else if (!strcmp(buffer, "NONE") && guiCmd->commandIndex == CMD_MESH_ATEP_SCAN_RANGE) {
		paramValue[0] = (void *) NC_GetNodeNetworkAddressArray();
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									&type,
									paramValue,
									1,
									arrayLength,
									sizes);
	} else {
		NC_ReportFoundNodes(buffer);
		NC_CheckEnabledModelsFeatures();
		paramValue[0] = (void *) NC_GetNodeNetworkAddressArray();
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									&type,
									paramValue,
									1,
									arrayLength,
									sizes);
	}

	return cmdRes;

}

CMD_CommandGet_t *CMD_SubsAdd(char *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;

	return cmdRes;

}

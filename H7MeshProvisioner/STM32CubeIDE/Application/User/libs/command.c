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
#include "device_settings.h"
#include "sensors.h"
#include <string.h>

void CMD_GenericFormatCommand(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd);
void CMD_SetupConfig(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_NofitfyProvision(void *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_NotifyScan(void *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_SubsAdd(void *buffer, CMD_CommandGet_t *guiCmd);
//CMD_CommandGet_t *CMD_GenericOnOff(char *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_NotifyUnprovision(void *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_NotifyLibVersion(void *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_NotifySensorUpdate(void *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_ProtocolStructTest(void *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_SensorDescriptorGet(void *buffer, CMD_CommandGet_t *guiCmd);

CMD_MeshCommand_t defineRootNetworkNode = {
		.command = "ATEP ROOT",
		.rxTimeout = CMD_MESH_RX_UACK,
		.commandType = PRO_MSG_TYPE_UNACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t scanForUnprovisionedNetworkDevices = {
		.command = "ATEP SCAN",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_NotifyScan
};

CMD_MeshCommand_t provisionNetworkDevice = {
		.command = "ATEP PRVN-%d",
		.rxTimeout = 120,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NofitfyProvision
};

CMD_MeshCommand_t scanForUnprovisionedNetworkDevicesOutOfRangePvrn = {
		.command = "ATEP NDSCAN",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_NotifyScan
};

CMD_MeshCommand_t provisionNetworkDeviceOutOfRangePvrn = {
		.command = "ATEP NDPRVN-%d",
		.rxTimeout = 120,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NofitfyProvision
};

CMD_MeshCommand_t unprovisionNetworkDevice = {
		.command = "ATEP UNPV %d",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NotifyUnprovision
};

CMD_MeshCommand_t genericOnOffSetAck = { // currently hard-coded
		.command = "ATCL %s 8202 %s 00",
		.rxTimeout = CMD_MESH_RX_UACK,
		.commandType = PRO_MSG_TYPE_UNACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = NULL
};

// broken ???
CMD_MeshCommand_t genericOnOffGet = {
		.command = "ATCL %s 8201",
		.rxTimeout = CMD_MESH_RX_UACK,
		.commandType = PRO_MSG_TYPE_UNACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = NULL
};

// not used
CMD_MeshCommand_t unprovisionEmbeddedProv = {
		.command = "BLEMesh_Unprovision",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = NULL
};

// not used
CMD_MeshCommand_t isEmbeddedProvProvisioned = {
		.command = "BLEMesh_IsUnprovisioned",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = NULL
};

CMD_MeshCommand_t pubSetSubAdd = {
		.command = "BLEMesh_PubSub %d %d %d %d",
		.rxTimeout = 120,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = CMD_SetupConfig,
		.CMD_Execute = CMD_SubsAdd
};

CMD_MeshCommand_t getLibInfo = {
		.command = "BLEMesh_LibVer",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NotifyLibVersion
};

CMD_MeshCommand_t sensorGet = {
		.command = "ATCL %s 8231 %s",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRUCT_APC1,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NotifySensorUpdate
};

CMD_MeshCommand_t sensorDescriptorGet = {
		.command = "ATCL %s 8230",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRUCT_DESC_GET,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_SensorDescriptorGet
};

CMD_MeshCommand_t protocolStructTest = {
		.command = "BLEMesh_Protest",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_STRUCT_TEST,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_ProtocolStructTest
};

static NC_MaskedFeatures *allModels;

extern UART_HandleTypeDef huart3;

#ifdef _DEBUG
static inline void debugMessage(char *message) {

	HAL_UART_Transmit(&huart3, (uint8_t *) message, strlen(message), 6000);

}
#endif

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
	// copy remaining characters (if there are any)
	while (*t) {
		*output++ = *t++;
	}
	*output = '\0';

}

void CMD_SetupConfig(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd) {

	static int isAllocated = 0;
	Node_SubscriptionParam_t *toSubb = (Node_SubscriptionParam_t *) guiCmd->param[0].value.voidPtr;
	Node_Config_t *node = NC_GetConfigNodeFromAddress(guiCmd->param[1].value.i);
	NC_MaskedFeatures *groupAddress = NC_GetAllGroupAddresses();
	char *output = buffer;
	const char *t = cmdTemplate;
	int numOfModels = NC_GetPopCount(node->address.nodeModels);
	static int i = 0;
	static int j = 0;

	if (!isAllocated) {
		allModels = NC_GetNodeFeature(NC_GetAllModels(), node->address.nodeModels);
		isAllocated = 1;
	}
	if (i < toSubb->numOfSubs) {
		if (j < numOfModels) {
			sprintf(output, t, toSubb->subbedAddresses[i].subbTo,
							   node->address.nodeAddress,
							   NC_GetValueFromBitmask(groupAddress, toSubb->subbedAddresses[i].groupAddress),
							   allModels[j].value);
			j++;
		}
		if (j == numOfModels) {
			j = 0;
			i++;
		}
	}
	if (i == toSubb->numOfSubs) {
		i = 0;
		j = 0;
		if (isAllocated) {
			vPortFree(allModels);
			isAllocated = 0;
		}
	}

}

CMD_CommandGet_t *CMD_NofitfyProvision(void *buffer, CMD_CommandGet_t *guiCmd) {

	int index;
	PARAMETER_TYPE types[] = {PARAM_VOID, PARAM_VOID};
	void *paramValue[2];
	int arrayLength[] = {1, 6};
	size_t sizes[] = {sizeof(Node_Config_t), sizeof(NC_MaskedFeatures)};
	CMD_CommandGet_t *cmdRes = NULL;
	Node_Config_t *configNodes = NC_GetNodeConfigArray();
	uint32_t assignedNodeAddress = (uint32_t) *((uint16_t *) buffer);

	if ((index = NC_ProvisionNode(guiCmd->param[0].value.i, assignedNodeAddress)) >= 0) {
		paramValue[0] = (void *) &configNodes[index];
		paramValue[1] = (void *) NC_GetAllGroupAddresses();
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									types,
									paramValue,
									2,
									arrayLength,
									sizes);
	}

	return cmdRes;

}

CMD_CommandGet_t *CMD_NotifyScan(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_VOID;
	void *paramValue[1];
	int arrayLength[] = {5};
	size_t sizes[] = {sizeof(Node_NetworkAddress_t)};
	char *inputBuffer = (char *) buffer;

	if (!strcmp(inputBuffer, "NONE") && guiCmd->commandIndex == CMD_MESH_ATEP_SCAN) {
		guiCmd->commandIndex = CMD_MESH_ATEP_SCAN_RANGE;
	} else if (!strcmp(inputBuffer, "NONE") && guiCmd->commandIndex == CMD_MESH_ATEP_SCAN_RANGE) {
		paramValue[0] = (void *) NC_GetNodeNetworkAddressArray();
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									&type,
									paramValue,
									1,
									arrayLength,
									sizes);
	} else {
		NC_ReportFoundNodes(inputBuffer);
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

CMD_CommandGet_t *CMD_SubsAdd(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	Node_SubscriptionParam_t *toSubb = (Node_SubscriptionParam_t *) guiCmd->param[0].value.voidPtr;
	Node_Config_t *node = NC_GetConfigNodeFromAddress(guiCmd->param[1].value.i);
	static int runCounter = 0;
	PARAMETER_TYPE type = PARAM_INT;
	int ok = 1;
	void *paramValue[] = {(void *) &ok};
	uint8_t i = 0;
	uint16_t inputBuffer = *((uint16_t *) buffer);

	if (inputBuffer) {

	} else if (!inputBuffer) {
		runCounter++;
		if (runCounter >= (NC_GetPopCount(node->address.nodeModels) * toSubb->numOfSubs)) {
			// for some reason, the compiler here puts add instruction before cmp
			// making the for loop run less then it should (ex. upper bound is 1
			// the loop doesn't even run).
			// Now using a while loop where I can dictate where to increment
			while (i<toSubb->numOfSubs) {
				if (toSubb->subbedAddresses[i].subbTo) {
					NC_AddSubscription(node, toSubb->subbedAddresses[i].groupAddress);
				} else {
					NC_RemoveSubscription(node, toSubb->subbedAddresses[i].groupAddress);
				}
				i++;
			}
			runCounter = 0;
			cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
										&type,
										paramValue,
										1,
										NULL,
										NULL);
		}
	}

	return cmdRes;

}

//CMD_CommandGet_t *CMD_GenericOnOff(char *buffer, CMD_CommandGet_t *guiCmd) {
//
//	return NULL;
//
//}

CMD_CommandGet_t *CMD_NotifyUnprovision(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	int nodeAddress = guiCmd->param[0].value.i;
	PARAMETER_TYPE type = PARAM_INT;
	int ok = 1;
	void *paramValue[] = {(void *) &ok};
	uint16_t inputBuffer = *((uint16_t *) buffer);

	if (inputBuffer) {

	} else if (!inputBuffer) {
		// clear node config array at said index
		// decrement the number of configured devices
		// change ui screen
		NC_DeleteConfiguredNode(nodeAddress);
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									&type,
									paramValue,
									1,
									NULL,
									NULL);
	}

	return cmdRes;

}

CMD_CommandGet_t *CMD_NotifyLibVersion(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	void *paramValues[3];
	char libVer[20] = {0};
	char libSubVer[20] = {0};
	char commExVer[10] = COMM_EX_VER;
	PARAMETER_TYPE types[] = {PARAM_CHAR, PARAM_CHAR, PARAM_CHAR};
	char *inputBuffer = (char *) buffer;

	if (!strcmp(inputBuffer, "NONE")) {

	} else {
		sscanf(inputBuffer, "%[^;];%s", libVer, libSubVer);
		paramValues[0] = (void *) libVer;
		paramValues[1] = (void *) libSubVer;
		paramValues[2] = (void *) commExVer;
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									types,
									paramValues,
									3,
									NULL,
									NULL);
	}


	return cmdRes;

}

CMD_CommandGet_t *CMD_NotifySensorUpdate(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_VOID;
	int arrayLength[] = {1};
	size_t sizes[] = {sizeof(Sensor_MeasuredData_t)};
	void *paramValues[1];
	unsigned int inputPID;
//	APC1_SelectedData_t *inputBuffer = (APC1_SelectedData_t *) buffer;
//	char message[256];

//	sprintf(message, "APC1 PM1.0: %d\r\n", inputBuffer->pm1_0);
//	debugMessage(message);
	sscanf(guiCmd->param[1].value.str, "%X", &inputPID);
	paramValues[0] = (void *) SN_GetMeasuredSensorData((SENSOR_PID ) inputPID, buffer);
	cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
								&type,
								paramValues,
								1,
								arrayLength,
								sizes);

	return cmdRes;

}

CMD_CommandGet_t *CMD_SensorDescriptorGet(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	Node_Config_t *node;
	PARAMETER_TYPE type = PARAM_INT;
	int ok = 1;
	void *paramValue[] = {(void *) &ok};
	unsigned int nodeAddress;

	sscanf(guiCmd->param[0].value.str, "%X", &nodeAddress);
	node = NC_GetConfigNodeFromAddress(nodeAddress);
	node->sensors = SN_RegisterSensor((SN_SensorDescriptorGet_t *) buffer);

	cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
								&type,
								paramValue,
								1,
								NULL,
								NULL);

	return cmdRes;

}

CMD_CommandGet_t *CMD_ProtocolStructTest(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_INT;
	int ok = 1;
	void *paramValue[] = {(void *) &ok};
	char message[256];
	SF_TestProtocol_t *inputBuffer = (SF_TestProtocol_t *) buffer;

	// note: it does not like printing float variables, as sprintf crashes
	// even with the correct flags to the linker
	sprintf(message, "uint16_t: %d\r\nuint8_t: %d\r\nint: %d\r\n",
				inputBuffer->val1,
				inputBuffer->val2,
				inputBuffer->val4);
	debugMessage(message);

	// something has to be returned to avoid looping
	// the returned value is ignored
	cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
								&type,
								paramValue,
								1,
								NULL,
								NULL);

	return cmdRes;

}

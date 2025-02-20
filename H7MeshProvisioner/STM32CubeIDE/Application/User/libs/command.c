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

#ifdef _DEBUG
#include "lib_utils.h"
#endif

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
CMD_CommandGet_t *CMD_IsEmbeddedProvisioned(void *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_UnprovEmProvisioner(void *buffer, CMD_CommandGet_t *guiCmd);
CMD_CommandGet_t *CMD_NotifyAtepRoot(void *buffer, CMD_CommandGet_t *guiCmd);

/*
 * Use: for provisioning the embedded provisioner (WB55)
 * Timeout: 60 seconds
 * Command type: acknowledge (requires response from WB55)
 * Setup function: none (no parameters)
 * Execute function: CMD_NotifyAtepRoot
 */
CMD_MeshCommand_t defineRootNetworkNode = {
		.command = "ATEP ROOT",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_NotifyAtepRoot
};

/*
 * Use: scanning for unprovisioned devices on WB55
 * Timeout: 60 seconds
 * Command type: acknowledge (requires response from WB55)
 * Response: string where cut UUID of nodes are separated by ';'
 * Setup function: none (no parameters)
 * Execute function: CMD_NotifyScan
 */
CMD_MeshCommand_t scanForUnprovisionedNetworkDevices = {
		.command = "ATEP SCAN",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_NotifyScan
};

/*
 * Use: provision node (add to network)
 * Timeout: 120 seconds
 * Command type: acknowledge (requires response from WB55)
 * Response: uint16_t value which is assigned node address
 * Setup function: CMD_GenericFormatCommand
 * Execute function: CMD_NofitfyProvision
 * Params (in order): node ID
 */
CMD_MeshCommand_t provisionNetworkDevice = {
		.command = "ATEP PRVN-%d",
		.rxTimeout = 120,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NofitfyProvision
};

/*
 * Use: scan for unprovisioned devices by using already provisioned nodes
 * 		(this should find nodes which are out of reach of the embedded provisioner)
 * Timeout: 60 seconds
 * Command type: acknowledge (requires response from WB55)
 * Response: string where cut UUID of nodes are separated by ';'
 * Setup function: none (no parameters)
 * Execute function: CMD_NotifyScan
 */
CMD_MeshCommand_t scanForUnprovisionedNetworkDevicesOutOfRangePvrn = {
		.command = "ATEP NDSCAN",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_NotifyScan
};

/*
 * Use: provision nodes which are out of range of the embedded provisioner and
 * 		found by ATEP NDSCAN
 * Timeout: 120 seconds
 * Command type: acknowledge (requires response from WB55)
 * Response: uint16_t value which is assigned node address
 * Setup function: CMD_GenericFormatCommand
 * Execute function: CMD_NofitfyProvision
 * Params (in order): node ID
 */
CMD_MeshCommand_t provisionNetworkDeviceOutOfRangePvrn = {
		.command = "ATEP NDPRVN-%d",
		.rxTimeout = 120,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NofitfyProvision
};

/*
 * Use: unprovision (delete) node from network
 * Timeout: 60 seconds
 * Command type: acknowledge (requires response from WB55)
 * Response: uint16_t value which is 0 or 1 (success of error)
 * Setup function: CMD_GenericFormatCommand
 * Execute function: CMD_NotifyUnprovision
 * Params (in order): node address
 */
CMD_MeshCommand_t unprovisionNetworkDevice = {
		.command = "ATEP UNPV %d",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NotifyUnprovision
};

/*
 * Use: generic on/off command
 * Timeout: 0 seconds
 * Command type: unacknowledged (requires no response from WB55)
 * Response: none
 * Setup function: CMD_GenericFormatCommand
 * Execute function: none
 * Params (in order): node address, on/off
 */
CMD_MeshCommand_t genericOnOffSetAck = {
		.command = "ATCL %s 8202 %s 00",
		.rxTimeout = CMD_MESH_RX_UACK,
		.commandType = PRO_MSG_TYPE_UNACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = NULL
};

/*
 * Use: generic on/off status
 * Timeout: 0 seconds
 * Command type: unacknowledged (requires no response from WB55)
 * Response: none
 * Setup function: CMD_GenericFormatCommand
 * Execute function: none
 * Note: not used
 * Params (in order): node address
 */
CMD_MeshCommand_t genericOnOffGet = {
		.command = "ATCL %s 8201",
		.rxTimeout = CMD_MESH_RX_UACK,
		.commandType = PRO_MSG_TYPE_UNACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = NULL
};

/*
 * Use: unprovision embedded provisioner
 * Timeout: 60 seconds
 * Command type: acknowledged (requires response from WB55)
 * Response: uint16_t value which is 0 or 1 (success of error)
 * Setup function: none
 * Execute function: CMD_UnprovEmProvisioner
 */
CMD_MeshCommand_t unprovisionEmbeddedProv = {
		.command = "BLEMesh_Unprovision",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_UnprovEmProvisioner
};

/*
 * Use: check if embedded provisioner is provisioned
 * Timeout: 60 seconds
 * Command type: acknowledged (requires response from WB55)
 * Response: uint16_t value which is 0 or 1 (success of error)
 * Setup function: none
 * Execute function: CMD_IsEmbeddedProvisioned
 */
CMD_MeshCommand_t isEmbeddedProvProvisioned = {
		.command = "BLEMesh_IsUnprovisioned",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_IsEmbeddedProvisioned
};

/*
 * Use: add/remove node from a subscription
 * Timeout: 120 seconds
 * Command type: acknowledged (requires response from WB55)
 * Response: uint16_t value which is 0 or 1 (success of error)
 * Setup function: CMD_SetupConfig
 * Execute function: CMD_SubsAdd
 * Params (in order): flag, element address, address, model ID
 */
CMD_MeshCommand_t pubSetSubAdd = {
		.command = "BLEMesh_PubSub %d %d %d %d",
		.rxTimeout = 120,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_U16T,
		.CMD_Setup = CMD_SetupConfig,
		.CMD_Execute = CMD_SubsAdd
};

/*
 * Use: get libraries version
 * Timeout: 60 seconds
 * Command type: acknowledged (requires response from WB55)
 * Response: string which has versions seperated by ';'
 * Setup function: none
 * Execute function: CMD_NotifyLibVersion
 */
CMD_MeshCommand_t getLibInfo = {
		.command = "BLEMesh_LibVer",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_NotifyLibVersion
};

/*
 * Use: get sensor measurement identified by PID
 * Timeout: 60 seconds
 * Command type: acknowledged (requires response from WB55)
 * Response: APC1 data structure
 * Setup function: CMD_GenericFormatCommand
 * Execute function: CMD_NotifySensorUpdate
 * Params (in order): node address, sensor PID
 */
CMD_MeshCommand_t sensorGet = {
		.command = "ATCL %s 8231 %s",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRUCT_APC1,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_NotifySensorUpdate
};

/*
 * Use: get sensor PIDs from a node
 * Timeout: 60 seconds
 * Command type: acknowledged (requires response from WB55)
 * Response: sensor descriptor structure
 * Setup function: CMD_GenericFormatCommand
 * Execute function: CMD_SensorDescriptorGet
 * Params (in order): node address
 */
CMD_MeshCommand_t sensorDescriptorGet = {
		.command = "ATCL %s 8230",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_ACK,
		.dataType = PRO_DATATYPE_STRUCT_DESC_GET,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = CMD_SensorDescriptorGet
};

/*
 * Use: test comm_ex if it is working
 * Timeout: 60 seconds
 * Command type: acknowledged (requires response from WB55)
 * Response: comm_ex test structure
 * Setup function: none
 * Execute function: CMD_ProtocolStructTest
 */
CMD_MeshCommand_t protocolStructTest = {
		.command = "BLEMesh_Protest",
		.rxTimeout = 60,
		.commandType = PRO_MSG_TYPE_OTHER,
		.dataType = PRO_DATATYPE_STRUCT_TEST,
		.CMD_Setup = NULL,
		.CMD_Execute = CMD_ProtocolStructTest
};

/*
 * Use: generic level delta set
 * Timeout: 0 seconds
 * Command type: unacknowledged (requires no response from WB55)
 * Response: none
 * Setup function: CMD_GenericFormatCommand
 * Execute function: none
 * Params (in order): node address, delta set
 */
CMD_MeshCommand_t genericLevelDeltaSet = {
		.command = "ATCL %s 8209 %s 00 00 00",
		.rxTimeout = CMD_MESH_RX_UACK,
		.commandType = PRO_MSG_TYPE_UNACK,
		.dataType = PRO_DATATYPE_STRING,
		.CMD_Setup = CMD_GenericFormatCommand,
		.CMD_Execute = NULL
};

static NC_MaskedFeatures *allModels;

/**
  * @brief  Create CMD_CommandGet_t structure which will contain parameters and their values which are
  * 		used in Comm_ex and GUI.
  * @note	Depending on the passed parameter types not all parameters of this function are necessary.
  * @param  cmdIndex		Enum value which represents the used command.
  * @param  types			Enum value/s which are the data types of used parameters.
  * @param  paramValues		Array of void pointers for individual parameter values.
  * @param  numOfParams		The number of used parameters.
  * @param  arrayLengths	The number of parameters of each type.
  * @param  elementSizes	The size of each parameter used.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_CreateCommandGet(CMD_INDEX cmdIndex, PARAMETER_TYPE types[], void *paramValues[], int numOfParams, int arrayLengths[], size_t *elementSizes) {

	CMD_CommandGet_t *cmd = NULL;
	int len;

	if (!(cmd = (CMD_CommandGet_t *) pvPortMalloc(sizeof(CMD_CommandGet_t)))) {
		// raise error
	}
	cmd->commandIndex = cmdIndex;
	cmd->numOfParams = numOfParams;
	// create an array of parameters
	if (!(cmd->param = (CMD_Parameter_t *) pvPortMalloc(numOfParams * sizeof(CMD_Parameter_t)))) {
		// raise error
	}
	// for each parameter, check its type and if needed create a deep copy or assign value
	for (int i = 0; i<numOfParams; i++) {
		cmd->param[i].type = types[i];
		cmd->param[i].arrayLength = 0;
		cmd->param[i].elementSize = 0;
		switch (types[i]) {
			case PARAM_INT:
				// copy value, no memory allocation needed
				cmd->param[i].value.i = *((int *) paramValues[i]);
				break;
			case PARAM_CHAR:
				// allocate memory for string and copy it
				len = strlen((char *) paramValues[i]);
				cmd->param[i].value.str = (char *) pvPortMalloc((len + 1) * sizeof(char));
				strcpy(cmd->param[i].value.str, (char *) paramValues[i]);
				break;
			case PARAM_INT_ARR:
				// passed parameter is an array
				// allocate memory and make a deep copy
				cmd->param[i].value.intArr = (int *) pvPortMalloc(arrayLengths[i] * sizeof(int));
				memcpy(cmd->param[i].value.intArr, (int *) paramValues, arrayLengths[i] * sizeof(int));
				cmd->param[i].arrayLength = arrayLengths[i];
				break;
			case PARAM_VOID:
				// passed parameter can be anything, so
				// allocate memory and deep copy
				cmd->param[i].elementSize = elementSizes[i];
				cmd->param[i].value.voidPtr = pvPortMalloc(arrayLengths[i] * elementSizes[i]);
				memcpy(cmd->param[i].value.voidPtr, paramValues[i], arrayLengths[i] * elementSizes[i]);
				break;
			default:
				break;
		}
	}

	// since we create a deep copy, the original pointers can be "discarded"
	return cmd;

}

/**
  * @brief  Free allocated memory for CMD_CommandGet_t structure.
  * @note	Depending on the passed parameter types not all parameters of this function are necessary.
  * @param  cmd	CMD_CommandGet_t pointer which contains allocated parameters and their values.
  * @retval	None
  */
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

/**
  * @brief  Place values into command parameters.
  * @note	If a command with no specifier is given, then it just copies it into the buffer.
  * @param  buffer		Character pointer into which formatted command will be placed.
  * @param  cmdTemplate	Character pointer to command template.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains allocated parameters and their values.
  * @retval	None
  */
void CMD_GenericFormatCommand(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd) {

	char *output = buffer;
	const char *t = cmdTemplate;

	for (int i = 0; i<guiCmd->numOfParams; i++) {
		// loop over command template until we find a '%' or hit the end
		// (copy command template into buffer)
		while (*t && *t != '%') {
			*output++ = *t++;
		}
		// found specifier might be an integer or an integer array
		if (*t == '%' && *(t + 1) == 'd') {
			// check if its a integer
			if (guiCmd->param[i].type == PARAM_INT) {
				output += sprintf(output, "%d", guiCmd->param[i].value.i);
			} else if (guiCmd->param[i].type == PARAM_INT_ARR) {
				// check if its an array of integers
				int len = guiCmd->param[i].arrayLength;
				int *arr = guiCmd->param[i].value.intArr;
				for (int j = 0; j<len; j++) {
					if (j > 0) {
						// add spaces between array values
						*output++ = ' ';
					}
					output += sprintf(output, "%d", arr[j]);
				}
			}
			// increment pass the specifier
			t += 2;
		} else if (*t == '%' && *(t + 1)  == 's' && guiCmd->param->type == PARAM_CHAR) {
			// specifier is a string
			output += sprintf(output, "%s", guiCmd->param[i].value.str);
			// increment pass
			t += 2;
		}
	}
	// copy remaining characters (if there are any)
	while (*t) {
		*output++ = *t++;
	}
	// null terminate
	*output = '\0';

}

/**
  * @brief  Place values into command parameters specific for pubSetSubAdd.
  * @param  buffer		Character pointer into which formatted command will be placed.
  * @param  cmdTemplate	Character pointer to command template.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains allocated parameters and their values.
  * @retval	None
  */
void CMD_SetupConfig(char *buffer, const char *cmdTemplate, CMD_CommandGet_t *guiCmd) {

	static int isAllocated = 0;
	Node_SubscriptionParam_t *toSubb = (Node_SubscriptionParam_t *) guiCmd->param[0].value.voidPtr;
	Node_Config_t *node = NC_GetConfigNodeFromAddress(guiCmd->param[1].value.i);
	NC_MaskedFeatures *groupAddress = NC_GetAllGroupAddresses();
	char *output = buffer;
	const char *t = cmdTemplate;
	int numOfModels = NC_GetPopCount(node->address.nodeModels); // get the number of model in a node
	static int i = 0;
	static int j = 0;

	if (!isAllocated) {
		// get model information for enabled models on a node
		allModels = NC_GetNodeFeature(NC_GetAllModels(), node->address.nodeModels);
		isAllocated = 1;
	}
	// this is basically a loop, but the looping part comes from the FSM
	// two loops i and j, where j is the inner loop
	// check if the outer loop finished
	if (i < toSubb->numOfSubs) {
		if (j < numOfModels) {
			sprintf(output, t, toSubb->subbedAddresses[i].subbTo,
							   node->address.nodeAddress,
							   NC_GetValueFromBitmask(groupAddress, toSubb->subbedAddresses[i].groupAddress),
							   allModels[j].value);
			j++;
		}
		// check if the inner loop finished
		if (j == numOfModels) {
			j = 0;
			i++;
		}
	}
	// when finished, free allocated memory for node models info
	if (i == toSubb->numOfSubs) {
		i = 0;
		j = 0;
		if (isAllocated) {
			vPortFree(allModels);
			isAllocated = 0;
		}
	}

}

/**
  * @brief  Configure node the newly added node and inform GUI.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_NofitfyProvision(void *buffer, CMD_CommandGet_t *guiCmd) {

	int index;
	PARAMETER_TYPE types[] = {PARAM_VOID, PARAM_VOID};
	void *paramValue[2];
	int arrayLength[] = {1, 6};
	size_t sizes[] = {sizeof(Node_Config_t), sizeof(NC_MaskedFeatures)};
	CMD_CommandGet_t *cmdRes = NULL;
	Node_Config_t *configNodes = NC_GetNodeConfigArray();
	uint32_t assignedNodeAddress = (uint32_t) *((uint16_t *) buffer);

	// the correct assigned node address is provided by WB55 in the buffer
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

/**
  * @brief  Notify GUI that unprov. node/s have been found.
  * @note	Switches command index if necessary.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_NotifyScan(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_VOID;
	void *paramValue[1];
	int arrayLength[] = {5};
	size_t sizes[] = {sizeof(Node_NetworkAddress_t)};
	char *inputBuffer = (char *) buffer;

	// clear any existing found nodes
	NC_ClearNodeNetworkAddressArray();
	// if result from WB55 is none and ordinary scan was used, then no unprov. node
	// was found. Switch to scan range and scan again.
	if (!strcmp(inputBuffer, "NONE") && guiCmd->commandIndex == CMD_MESH_ATEP_SCAN) {
		guiCmd->commandIndex = CMD_MESH_ATEP_SCAN_RANGE;
	} else if (!strcmp(inputBuffer, "NONE") && guiCmd->commandIndex == CMD_MESH_ATEP_SCAN_RANGE) {
		// if result from WB55 is none and scan range was used, then no unprov. node
		// was found; notify GUI about this
		paramValue[0] = (void *) NC_GetNodeNetworkAddressArray();
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									&type,
									paramValue,
									1,
									arrayLength,
									sizes);
	} else {
		// one or multiple unprov. nodes have been found. Notify
		// the GUI about this
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

/**
  * @brief  Add and/or remove subscription from node; when finished notify GUI.
  * @note	Loops FSM if needed.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
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
		// this loops until all models have been used for
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

/**
  * @brief  Delete node; when finished notify GUI.
  * @note	Resets node data to default values.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_NotifyUnprovision(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	int nodeAddress = guiCmd->param[0].value.i;
	PARAMETER_TYPE type = PARAM_INT;
	int ok = 1;
	void *paramValue[] = {(void *) &ok};
	uint16_t inputBuffer = *((uint16_t *) buffer);

	// input buffer has either 0 or 1
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

/**
  * @brief  Show libraries versions on GUI.
  * @note	Shows STBle mesh version, its subversion & Comm_ex version.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_NotifyLibVersion(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	void *paramValues[3];
	char libVer[20] = {0};
	char libSubVer[20] = {0};
	char commExVer[10] = COMM_EX_VER;
	PARAMETER_TYPE types[] = {PARAM_CHAR, PARAM_CHAR, PARAM_CHAR};
	char *inputBuffer = (char *) buffer;

	// input buffer has library versions separated by ';'
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

/**
  * @brief  Figure out which sensors data has been polled then notify GUI.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_NotifySensorUpdate(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_VOID;
	int arrayLength[] = {1};
	size_t sizes[] = {sizeof(Sensor_MeasuredData_t)};
	void *paramValues[1];
	unsigned int inputPID;

	// get sensor PID which was used as input parameter for command
	sscanf(guiCmd->param[1].value.str, "%X", &inputPID);
	// get measured data
	paramValues[0] = (void *) SN_GetMeasuredSensorData((SENSOR_PID) inputPID, buffer);
	cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
								&type,
								paramValues,
								1,
								arrayLength,
								sizes);

	return cmdRes;

}

/**
  * @brief  Register sensor and add it to correct node config.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_SensorDescriptorGet(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	Node_Config_t *node;
	PARAMETER_TYPE type = PARAM_INT;
	int ok = 1;
	void *paramValue[] = {(void *) &ok};
	unsigned int nodeAddress;

	// get used node address which was used as command parameter
	sscanf(guiCmd->param[0].value.str, "%X", &nodeAddress);
	node = NC_GetConfigNodeFromAddress(nodeAddress);
	// register sensor which is tied to PID (we got PID as input from WB55)
	node->sensors = SN_RegisterSensor((SN_SensorDescriptorGet_t *) buffer);

	cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
								&type,
								paramValue,
								1,
								NULL,
								NULL);

	return cmdRes;

}

/**
  * @brief  Used for testing; checks if Comm_ex is working properly.
  * @note	Prints received data on terminal and nothing else. Used for testing.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_ProtocolStructTest(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_INT;
	int ok = 1;
	void *paramValue[] = {(void *) &ok};
	char message[256];
	SF_TestProtocol_t *inputBuffer = (SF_TestProtocol_t *) buffer;

	// note: it does not like printing float variables, as sprintf crashes
	// even with the correct flags to the linker
	sprintf(message, "uint16_t: %d\r\nuint8_t: %d\r\nint: %d",
				inputBuffer->val1,
				inputBuffer->val2,
				inputBuffer->val4);
#ifdef _DEBUG
	debugMessage(message, strlen(message), PRINT_CHAR);
#endif

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

/**
  * @brief  Check if node configures as unprov. node.
  * @note	Changes command index if necessary.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_IsEmbeddedProvisioned(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_INT;
	void *paramValue[1];
	int inputBuffer = (int) *((uint16_t *) buffer);

	// input buffer is either 0 or 1
	if (inputBuffer) {
		// em. prov. is unprovisioned
		// switch command so than next loop provisions
		guiCmd->commandIndex = CMD_MESH_ATEP_ROOT;
	} else {
		// em. prov. is provisioned
		NC_SetDeviceConfiguredFlag(1);
		paramValue[0] = (void *) &inputBuffer;
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									&type,
									paramValue,
									1,
									NULL,
									NULL);
	}


	return cmdRes;


}

/**
  * @brief  Notify GUI that em. prov. is correctly setup.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_NotifyAtepRoot(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_INT;
	void *paramValue[1];
	uint16_t inputBuffer = *((uint16_t *) buffer);

	// input buffer is either 0 or 1
	if (inputBuffer) {

	} else if (!inputBuffer) {
		// save that em. prov. is provisioned
		NC_SetDeviceConfiguredFlag(1);
		// notify GUI
		paramValue[0] = (void *) &inputBuffer;
		cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
									&type,
									paramValue,
									1,
									NULL,
									NULL);
	}

	return cmdRes;

}

/**
  * @brief  Unprovisions the em. prov. if it is provisioned.
  * @note	Used for testing and should not be accessible to the end user.
  * @param  buffer		buffer which contains response data from WB55.
  * @param  guiCmd		CMD_CommandGet_t pointer which contains command, allocated parameters
  * 					and their values for the command that was sent to WB55 for execution.
  * @retval	CMD_CommandGet_t pointer
  */
CMD_CommandGet_t *CMD_UnprovEmProvisioner(void *buffer, CMD_CommandGet_t *guiCmd) {

	CMD_CommandGet_t *cmdRes = NULL;
	PARAMETER_TYPE type = PARAM_INT;
	void *paramValue[1];
	int inputBuffer = (int) *((uint16_t *) buffer);

	// does not do anything, just passed inputBuffer value to GUI
	paramValue[0] = (void *) &inputBuffer;
	cmdRes = CMD_CreateCommandGet(guiCmd->commandIndex,
								&type,
								paramValue,
								1,
								NULL,
								NULL);


	return cmdRes;

}

/*
 * communication_ex.c
 *
 *  Created on: Jul 24, 2024
 *      Author: Nejc
 */

#include <string.h>
#include <ctype.h>
#include "communication_ex.h"

#define	Toggle_Pin(port, pin, state)			{ HAL_GPIO_WritePin(port, pin, state); }
#ifdef _MASTER
#include "cmsis_os2.h"
#include "command.h"
#include "hash_table.h"
#include "node_config.h"
#include "freertos_os2.h"
#include "task.h"
#include "sensors.h"
#define CS_LOW									{ Toggle_Pin(COMM_CS_PORT_MASTER, COMM_CS_PIN_MASTER, GPIO_PIN_RESET) }
#define CS_HIGH									{ Toggle_Pin(COMM_CS_PORT_MASTER, COMM_CS_PIN_MASTER, GPIO_PIN_SET) }
#define C_SIZE_CMD_STRING						256U
#else
#include "stm32_seq.h"
#include "serial_if.h"
#include "mesh_cfg.h"
#endif

// debug function
#ifdef _DEBUG
#ifdef _MASTER
extern UART_HandleTypeDef huart3;
#define UART_DEBUG &huart3
#else
extern UART_HandleTypeDef huart1;
#define UART_DEBUG &huart1
#endif
static void debugMessage(char *message, uint16_t payloadLenght);
#endif

// UART functions
extern int hsTrigger;
extern volatile int timerTrigger;

// protocol functions
static void Protocol_Process_Messsage(void);
static int Protocol_CheckChecksum(PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length, uint16_t received_checksum);
static uint16_t Protocol_CalcChecksum(PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length);
static inline void Protocol_ConvertMessage(uint8_t *payload, uint16_t payload_length);
#ifdef _MASTER
static PROTOCOL_STATUS _SendData(uint8_t *toSend, uint16_t sizeToSend, uint32_t timeout, void (*callback)(void));
static PROTOCOL_STATUS _ReceiveData(uint8_t *toReceive, uint16_t sizeToReceive, uint32_t timeout, void (*callback)(void));
#endif

// state machine functions
void FSM_Idle(void *param);
void FSM_Setup(void *param);
void FSM_Transmit(void *param);
void FSM_Receive(void *param);
void FSM_Execute(void *param);
void FSM_Error(void *param);

// timer functions

// execution functions
void GUI_QueueMessage(osMessageQueueId_t FSM_ResultQueueHandle, CMD_CommandGet_t *exeResult);
void GUI_ReportError(FSM_ErrorReport_t *reportedError);

// UART variables
extern Comm_Settings_t *commSettings;
void (*LPUART_CallbackTx)(void);
void (*LPUART_CallbackRx)(void);

// protocol variables
#ifdef _MASTER
uint8_t CommandString[C_SIZE_CMD_STRING];
#endif
static uint8_t PAC_START_BYTE = 0x4E;
static uint8_t PAC_END_BYTE = 0x56;
static uint8_t startByteRx, cmdTypeRx, endByteRx;
static uint8_t payloadRx[PAC_MAX_PAYLOAD];
static uint16_t checksumRx, lengthRx, payloadLengthRx;
static PROTOCOL_MSG_TYPE cmdTypeConverted;
static int errorCounter;

// timer variables

// state machine variables
extern Queue *eventQueue;
//static FSM_ErrorReport_t error;
#ifdef _MASTER
//FSM_CommandExecutionResult_t cmdResult;
extern osMessageQueueId_t FSM_ResultQueueHandle;
extern HT_HashTable_t *cmdHashTable;
static FSM_TransitionState_t stateTransitionTable[MAIN_FSM_NUM_OF_STATES][MAIN_FSM_NUM_OF_EVENTS] = {
		[MAIN_FSM_IDLE][MAIN_FSM_EVENT_USER] 						= 	{MAIN_FSM_SETUP, FSM_Setup},
		[MAIN_FSM_SETUP][MAIN_FSM_EVENT_SETUP_COMPLETE] 			= 	{MAIN_FSM_TRANSMIT, FSM_Transmit},
//		[MAIN_FSM_TRANSMIT][MAIN_FSM_EVENT_TRANSMIT_COMPLETE] 		= 	{MAIN_FSM_TRANSMIT_COMPLETE, NULL},
		[MAIN_FSM_TRANSMIT][MAIN_FSM_EVENT_AKC] 					= 	{MAIN_FSM_RECEIVE, FSM_Receive},
		[MAIN_FSM_TRANSMIT][MAIN_FSM_EVENT_UNACK] 					= 	{MAIN_FSM_IDLE, FSM_Idle},
		[MAIN_FSM_RECEIVE][MAIN_FSM_EVENT_RECEIVE_COMPLETE] 		= 	{MAIN_FSM_EXECUTE_COMMAND, FSM_Execute},
		[MAIN_FSM_EXECUTE_COMMAND][MAIN_FSM_EVENT_EXE_COMPLETE] 	=	{MAIN_FSM_IDLE, FSM_Idle},
		[MAIN_FSM_EXECUTE_COMMAND][MAIN_FSM_EVENT_LOOP]				=	{MAIN_FSM_TRANSMIT, FSM_Transmit},
		[MAIN_FSM_TRANSMIT][MAIN_FSM_EVENT_ERROR]					=	{MAIN_FSM_ERROR, FSM_Error},
		[MAIN_FSM_RECEIVE][MAIN_FSM_EVENT_ERROR]					=	{MAIN_FSM_ERROR, FSM_Error},
		[MAIN_FSM_ERROR][MAIN_FSM_EVENT_USER]						=	{MAIN_FSM_SETUP, FSM_Setup}
};
#else
static FSM_TransitionState_t stateTransitionTable[MAIN_FSM_NUM_OF_STATES][MAIN_FSM_NUM_OF_EVENTS] = {
		[MAIN_FSM_IDLE][MAIN_FSM_EVENT_INTERRUPT] 				= {MAIN_FSM_SETUP, FSM_Setup},
		[MAIN_FSM_SETUP][MAIN_FSM_EVENT_SETUP_COMPLETE] 		= {MAIN_FSM_RECEIVE, FSM_Receive},
		[MAIN_FSM_RECEIVE][MAIN_FSM_EVENT_RECEIVE_COMPLETE]		= {MAIN_FSM_EXECUTE_COMMAND, FSM_Execute},
		[MAIN_FSM_EXECUTE_COMMAND][MAIN_FSM_EVENT_AKC] 			= {MAIN_FSM_TRANSMIT, FSM_Transmit},
		[MAIN_FSM_EXECUTE_COMMAND][MAIN_FSM_EVENT_UNACK] 		= {MAIN_FSM_IDLE, FSM_Idle},
		[MAIN_FSM_TRANSMIT][MAIN_FSM_EVENT_TRANSMIT_COMPLETE] 	= {MAIN_FSM_IDLE, FSM_Idle},
		[MAIN_FSM_TRANSMIT][MAIN_FSM_EVENT_ERROR] 				= {MAIN_FSM_ERROR, FSM_Error}
};
#endif

#ifdef _DEBUG
static inline void debugMessage(char *message, uint16_t payloadLength) {

    char tmp[5];
    char output[PAC_MAX_PAYLOAD];
    int index = 0;
    int isBinary = 0;

    for (int i = 0; i < payloadLength; i++) {
        if (!isBinary) {
            if (message[i] == ':') {
                // add ':' to the output and switch to binary mode
                if (index < sizeof(output) - 1) {
                    output[index++] = ':';
                }
                isBinary = 1; // switch to binary processing
            } else if (isprint((uint8_t)message[i])) {
                // append printable character before ':' in string mode
                if (index < sizeof(output) - 1) {
                    output[index++] = message[i];
                }
            } else {
                // non-printable character before ':' should be escaped
            	// this should never happen, right?
                snprintf(tmp, sizeof(tmp), "\\x%02X", (uint8_t)message[i]);
                size_t len = strlen(tmp);
                if (index + len < sizeof(output)) {
                    strcpy(&output[index], tmp);
                    index += len;
                }
            }
        } else {
            // binary mode; always output as hex
            snprintf(tmp, sizeof(tmp), "\\x%02X", (uint8_t)message[i]);
            size_t len = strlen(tmp);
            if (index + len < sizeof(output)) {
                strcpy(&output[index], tmp);
                index += len;
            } else {
                break; // prevent overflow
            }
        }
    }
    // always append \r\n to the output buffer at the end
    if (index + 2 < sizeof(output)) {
        output[index++] = '\r';
        output[index++] = '\n';
    }
    // terminate the output buffer
    if (index < sizeof(output)) {
        output[index] = '\0';
    } else {
        output[sizeof(output) - 1] = '\0';
    }
    HAL_UART_Transmit(UART_DEBUG, (uint8_t *)output, index, 3000);

}
#endif

static inline void Protocol_ConvertMessage(uint8_t *payload, uint16_t payload_lenght) {

	for (int i = 0; i<payload_lenght; i++) {
		if ((payload[i] >= 'a') && (payload[i] <= 'z')) {
			payload[i] += 'A' - 'a';
		}
	}

}

static uint16_t Protocol_CalcChecksum(PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length) {

	uint16_t sum = type + payload_length + PAC_START_BYTE+ + PAC_END_BYTE;
	for (int i = 0; i<payload_length; i++) {
		sum += payload[i];
	}

	return sum;

}

static int Protocol_CheckChecksum(PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length, uint16_t received_checksum) {

	return Protocol_CalcChecksum(type, payload, payload_length) == received_checksum ? 1 : 0;

}

static PROTOCOL_STATUS _SendData(uint8_t *toSend, uint16_t sizeToSend, uint32_t timeout, void (*callback)(void)) {

	PROTOCOL_STATUS status;
	LPUART_CallbackTx = callback;

	vTaskSuspendAll();
	if ((status = Comm_LPUART_Send_IT(commSettings, toSend, sizeToSend, sizeToSend, timeout)) == PRO_OK) {
		Protocol_WaitForTX();
	}
	xTaskResumeAll();

	return status;

}

static PROTOCOL_STATUS _ReceiveData(uint8_t *toReceive, uint16_t sizeToReceive, uint32_t timeout, void (*callback)(void)) {

	PROTOCOL_STATUS status;
	LPUART_CallbackRx = callback;

	vTaskSuspendAll();
	if ((status = Comm_LPUART_Receive_IT(commSettings, toReceive, sizeToReceive, sizeToReceive, timeout)) == PRO_OK) {
		Protocol_WaitForRX();
	}
	xTaskResumeAll();

	return status;

}

PROTOCOL_STATUS Protocol_Send(PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length, uint32_t timeout, void (*callback)(void)) {

	PROTOCOL_STATUS status;
	uint16_t packetLength = payload_length + PAC_SIZE;
	Protocol_ConvertMessage(payload, payload_length);
	uint16_t checksum = Protocol_CalcChecksum(type, payload, payload_length);

#ifdef _MASTER
	CS_LOW
	HAL_Delay(100);
	CS_HIGH
#endif
	if ((status = _SendData(&PAC_START_BYTE, 1, timeout, callback)) != PRO_OK) return status;
	if ((status = _SendData((uint8_t *) &type, 1, timeout, callback)) != PRO_OK) return status;
	if ((status = _SendData((uint8_t *) &packetLength, 2, timeout, callback)) != PRO_OK) return status;
	if ((status = _SendData(payload, payload_length, timeout, callback)) != PRO_OK) return status;
	if ((status = _SendData((uint8_t *) &checksum, 2, timeout, callback)) != PRO_OK) return status;
	if ((status = _SendData(&PAC_END_BYTE, 1, timeout, callback)) != PRO_OK) return status;

	return PRO_OK;

}

PROTOCOL_STATUS Protocol_Receive(uint32_t timeout, void (*callback)(void)) {

	PROTOCOL_STATUS status;

	if ((status = _ReceiveData(&startByteRx, 1, timeout, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData(&cmdTypeRx, 1, timeout, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData((uint8_t *) &lengthRx, 2, timeout, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData(payloadRx, payloadLengthRx, timeout, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData((uint8_t *) &checksumRx, 2, timeout, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData(&endByteRx, 1, timeout, callback)) != PRO_OK) return status;

	if (callback) callback();

	return PRO_OK;

}

void FSM_HandleEvent(FSM_State_t *fsm, FSM_QueuedEvent_t *event) {

	FSM_TransitionState_t transition;
	if (event) {
		transition = stateTransitionTable[fsm->currentState][event->event];
		if (transition.stateFunction) {
			transition.stateFunction(event->param);
		}
		fsm->currentState = transition.nextState;
	}

}


void FSM_RegisterEvent(Queue *queue, MAIN_FSM_EVENT event, void *param, size_t paramSize) {

	FSM_QueuedEvent_t *e;
	if (!(e = (FSM_QueuedEvent_t *) pvPortMalloc(sizeof(FSM_QueuedEvent_t)))) {
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, GPIO_PIN_SET);
		return;
	}
	e->event = event;
	if (param && paramSize > 0) {
		if (!(e->param = pvPortMalloc(paramSize))) {
			// raise error and free e
			vPortFree(e);
		} else {
			memcpy(e->param, param, paramSize);
		}
	} else {
		e->param = NULL;
	}
	enqueue(queue, e, sizeof(FSM_QueuedEvent_t));
	vPortFree(e);

}

FSM_QueuedEvent_t *FSM_GetEvent(Queue *queue) {

	void *data;
	if (!(data = dequeue(queue))) {
		// raise error
	}
	return (FSM_QueuedEvent_t *) data;

}

void FSM_FreeEvent(FSM_QueuedEvent_t *event) {

	if (event) {
		vPortFree(event->param);
		vPortFree(event);
	}

}

void FSM_FreeEventsDeleteQueue(Queue *queue) {

	if (!queue) {
		// raise error
	}
	while (!isEmpty(queue)) {
		FSM_FreeEvent(FSM_GetEvent(queue));
	}
	deleteQueue(queue);

}

void FSM_Idle(void *param) {

#ifdef _DEBUG
	debugMessage("IDLE State", strlen("IDLE State"));
#endif
	errorCounter = 0;
}

void FSM_Setup(void *param) {

#ifdef _DEBUG
	debugMessage("SETUP State", strlen("SETUP State"));
#endif
	// error.commandIndex = *((int *) param);
	startByteRx = 0;
	cmdTypeRx = 0;
	memset(payloadRx, 0, PAC_MAX_PAYLOAD);
	endByteRx = 0;
	checksumRx = 0;
	lengthRx = 0;
	payloadLengthRx = 0;
#ifdef _MASTER
	memset(CommandString, 0, C_SIZE_CMD_STRING);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_SETUP_COMPLETE, param, sizeof(param));
#else
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_SETUP_COMPLETE, NULL, 0);
#endif

}

void FSM_Transmit(void *param) {

#ifdef _DEBUG
	debugMessage("TRANSMIT State", strlen("TRANSMIT State"));
#endif
#ifdef _MASTER
	char cmdToSend[CMD_MESH_COMMAND_LENGHT] = {0};
	PROTOCOL_STATUS status;
	FSM_ErrorReport_t error;
	CMD_MeshCommand_t *meshCommand;
	CMD_CommandGet_t *guiCmd = *((CMD_CommandGet_t **) param);

	if ((meshCommand = (CMD_MeshCommand_t *) HT_Search(cmdHashTable, guiCmd->commandIndex))) {
		if (meshCommand->CMD_Setup) {
			meshCommand->CMD_Setup(cmdToSend, meshCommand->command, guiCmd);
		} else {
			memcpy(cmdToSend, meshCommand->command, strlen(meshCommand->command));
		}
		HAL_Delay(1000);
		if ((status = Protocol_Send(meshCommand->commandType, (uint8_t *) cmdToSend, strlen(cmdToSend), DEFAULT_TX_TIMEOUT, NULL)) == PRO_OK) {
			if (meshCommand->commandType != PRO_MSG_TYPE_UNACK) {
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, param, sizeof(param));
				// don't yet free send command, as it (might) is needed later
			} else {
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_UNACK, NULL, 0);
				CMD_FreeCommandGet(guiCmd); // free send command, as we don't need it anymore
			}
		} else {
			error.status = status;
			error.param = param;
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_ERROR, (void *) &error, sizeof(FSM_ErrorReport_t));
		}
	}
#else
	Protocol_Send(PRO_MSG_TYPE_OTHER, "hello", strlen("hello"), NULL);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_TRANSMIT_COMPLETE, NULL, 0);
#endif

}

void FSM_Receive(void *param) {

#ifdef _DEBUG
	debugMessage("RECEIVE State", strlen("RECEIVE State"));
#endif
	PROTOCOL_STATUS status;
	FSM_ErrorReport_t error;
	CMD_MeshCommand_t *meshCommand;
	CMD_CommandGet_t *guiCmd = *((CMD_CommandGet_t **) param);

	if ((meshCommand = (CMD_MeshCommand_t *) HT_Search(cmdHashTable, guiCmd->commandIndex))) {
		if ((status = Protocol_Receive(meshCommand->rxTimeout, Protocol_Process_Messsage)) == PRO_OK) {
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_RECEIVE_COMPLETE, param, sizeof(param));
		} else {
			error.status = status;
			error.param = param;
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_ERROR, (void *) &error, sizeof(FSM_ErrorReport_t));
		}
	}

}

void FSM_Execute(void *param) {

#ifdef _DEBUG
	debugMessage("EXECUTE State -> RECEIVED: ", strlen("EXECUTE State -> RECEIVED: "));
	debugMessage((char *) CommandString, payloadLengthRx);
#endif
#ifdef _MASTER
	size_t len = 0;
	CMD_CommandGet_t *guiCmd = *((CMD_CommandGet_t **) param);
	CMD_MeshCommand_t *meshCommand = (CMD_MeshCommand_t *) HT_Search(cmdHashTable, guiCmd->commandIndex);
	CMD_CommandGet_t *exeResult;
	FSM_DecodedPayload_t *payload;
	char cutOriginal[CMD_MESH_COMMAND_LENGHT] = {0};

	payload = FSM_DecodePayload(CommandString, meshCommand->dataType);
	sscanf(meshCommand->command, "%[^%]", cutOriginal);
	Protocol_ConvertMessage((uint8_t *) cutOriginal, strlen(cutOriginal));

	len = strlen(cutOriginal);
	while (len > 0 && (cutOriginal[len - 1] == ' ' || cutOriginal[len - 1] == '-')) {
		cutOriginal[--len] = '\0';
	}
	if (!strcmp(payload->command, cutOriginal)) {
		if (meshCommand->CMD_Execute) {
			if ((exeResult = meshCommand->CMD_Execute(payload->data, guiCmd))) {
				GUI_QueueMessage(FSM_ResultQueueHandle, exeResult);
				CMD_FreeCommandGet(guiCmd);
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_EXE_COMPLETE, NULL, 0);
			} else {
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, &guiCmd, sizeof(guiCmd));
			}
			FSM_FreeDecodedPayload(payload);
		}
	}

#else
	// set task for serial interface process, which will execute received command
	// UTIL_SEQ_SetTask(1 << CFG_TASK_MESH_SPI_REQ_ID, CFG_SCH_PRIO_0);
	// Serial_InterfaceProcess();
	// BLEMesh_GetNeighborState(pNeighborTable, pNoOfNeighborPresent); could be called directly for scanning devices
	UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_SERIAL_REQ_ID, CFG_SCH_PRIO_0);
	if (cmdTypeConverted != PRO_MSG_TYPE_UNACK) {
		FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, NULL, 0);
	} else {
		FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_UNACK, NULL, 0);
	}
#endif

}

/*
 * What to do when different errors are encountered:
 * 	- PRO_ERROR_HAL, PRO_ERROR_TIMEOUT, PRO_ERROR_CHECKSUM
 * 	  PRO_ERROR_START_BYTE, PRO_ERROR_END_BYTE:
 * 	  	These errors should be mitigated by sending the command
 * 	  	again and executing it from the beginning.
 * 	  	If any of the above errors occur more than the defined
 * 	  	error threshold reset both systems.
 * 	- PRO_ERROR_PARAM, PRO_ERROR_UN_CMD:
 * 		These two are critical errors; basically they should never
 * 		happen, as commands and their parameters are defined in advance.
 * 		If they do occur though, reset both systems.
 */
void FSM_Error(void *param) {

	FSM_ErrorReport_t *error = (FSM_ErrorReport_t *) param;
#ifdef _DEBUG
	char errString[40];
	sprintf(errString, "ERROR State [%d](%d)", error->status, errorCounter);
	debugMessage(errString, strlen(errString));
#endif
	if (errorCounter++ >= ERROR_THRESHOLD
		|| error->status == PRO_ERROR_UN_CMD
		|| error->status == PRO_ERROR_PARAM) {
		GUI_ReportError(error);
	} else {
		switch (error->status) {
			case PRO_ERROR_START_BYTE:
			case PRO_ERROR_END_BYTE:
			case PRO_ERROR_CHECKSUM:
			case PRO_ERROR_HAL:
			case PRO_ERROR_TIMEOUT:
				HAL_GPIO_WritePin(COMM_ERROR_PORT_MASTER, COMM_ERROR_PIN_MASTER, GPIO_PIN_RESET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(COMM_ERROR_PORT_MASTER, COMM_ERROR_PIN_MASTER, GPIO_PIN_SET);
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_USER, error->param, sizeof(error->param));
				break;
			default:
				break;
		}
	}

}

void GUI_ReportError(FSM_ErrorReport_t *reportedError) {

	CMD_CommandGet_t *cmdError;
	PARAMETER_TYPE type = PARAM_CHAR;
	void *paramValues[1];
	char errorDesc[40];

	sprintf(errorDesc, "CRITICAL ERROR: %d - RESET DEVICE", reportedError->status);
	paramValues[0] = (void *) errorDesc;
	cmdError = CMD_CreateCommandGet(CMD_ERROR,
									&type,
									paramValues,
									1,
									NULL,
									NULL);
	GUI_QueueMessage(FSM_ResultQueueHandle, cmdError);
//	CMD_FreeCommandGet((CMD_CommandGet_t *) reportedError->param);

}

static void Protocol_Process_Messsage(void) {

	static PROCESS_MESSAGE_STATE procState = CHECK_START_BYTE;

	switch (procState) {
		case CHECK_START_BYTE:
			// check for correct start byte of packet
			if (PAC_START_BYTE == startByteRx) {
				procState = CHECK_PACKET_TYPE;
			} else {
				// raise error
			}
			break;
		case CHECK_PACKET_TYPE:
			// this could be filled out at a later date
			// to check for correct packet types
			cmdTypeConverted = (PROTOCOL_MSG_TYPE) cmdTypeRx;
			procState = CHECK_LENGTH;
			break;
		case CHECK_LENGTH:
			// this could be filled out at a later date
			// to check for strange length values
			payloadLengthRx = lengthRx - PAC_SIZE;
			procState = CHECK_BYTE;
			break;
		case CHECK_BYTE:
			memcpy((void *) CommandString, (void *) payloadRx, payloadLengthRx);
			procState = CHECK_CHECKSUM;
			break;
		case CHECK_CHECKSUM:
			// check for correct checksum
			if (Protocol_CheckChecksum(cmdTypeConverted, CommandString, payloadLengthRx, checksumRx)) {
				procState = CHECK_END_BYTE;
			} else {
				// raise error
			}
			break;
		case CHECK_END_BYTE:
			// check for correct end byte of packet
			if (PAC_END_BYTE == endByteRx) {
				// received message is correct
				procState = CHECK_OK;
			} else {
				// raise error
			}
			break;
		case CHECK_OK:
			procState = CHECK_START_BYTE;
#ifdef _MASTER
#else
			indexReceiveChar = payloadLengthRx;
			CommandString[indexReceiveChar] = 0;
#endif
			break;
		default:
			break;
	}

}

void GUI_QueueMessage(osMessageQueueId_t FSM_ResultQueueHandle, CMD_CommandGet_t *exeResult) {

	if (osMessageQueueGetSpace(FSM_ResultQueueHandle) > 0) {
		if (osMessageQueuePut(FSM_ResultQueueHandle, &exeResult, 0, 0) != osOK) {
			// raise error
		}
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

#ifdef _MASTER
	if (GPIO_Pin == COMM_HS_PIN_MASTER) {
		TIM_Stop(commSettings);
		hsTrigger = 1;
	}
#else
	if (GPIO_Pin == COMM_CS_PIN_SLAVE) {
		FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_INTERRUPT, NULL, 0);
	}
#endif

}

void FSM_EncodePayload(char *buffer, const char *command, void *data, size_t dataSize, PROTOCOL_DATATYPE type) {

	size_t commandLength = strlen(command);

	strncpy(buffer, command, commandLength);
	buffer[commandLength] = ':';
	if (type == PRO_DATATYPE_STRING) {
		strncpy(&buffer[commandLength + 1], (char *) data, PAC_MAX_PAYLOAD - commandLength - 1);
	} else {
		memcpy(&buffer[commandLength + 1], data, dataSize);
	}

}

FSM_DecodedPayload_t *FSM_DecodePayload(uint8_t *buffer, PROTOCOL_DATATYPE type) {

	char *delimiter;
	void *data;
	FSM_DecodedPayload_t *outputData = NULL;
	size_t commandLenght = 0;
	size_t len = 0;

	if (!(outputData = (FSM_DecodedPayload_t *) pvPortMalloc(sizeof(FSM_DecodedPayload_t)))) {
		return NULL;
	}
	if (!(delimiter = strchr((char *) buffer, ':'))) {
		return NULL;
	}
	commandLenght = delimiter - (char *) buffer;
	strncpy(outputData->command, (char *) buffer, commandLenght);
	outputData->command[commandLenght] = '\0';
	data = delimiter + 1;
	switch (type) {
		case PRO_DATATYPE_STRUCT_APC1:
			if ((outputData->data = pvPortMalloc(sizeof(APC1_SelectedData_t)))) {
				memcpy(outputData->data, data, sizeof(APC1_SelectedData_t));
			}
			break;
		case PRO_DATATYPE_STRING:
			len = strlen((char *) data) + 1;
			if ((outputData->data = pvPortMalloc(len * sizeof(char)))) {
				strncpy((char *) outputData->data, data, len);
			}
			break;
		case PRO_DATATYPE_U16T:
			if ((outputData->data = pvPortMalloc(sizeof(uint16_t)))) {
				memcpy(outputData->data, data, sizeof(uint16_t));
			}
			break;
		case PRO_DATATYPE_STRUCT_TEST:
			if ((outputData->data = pvPortMalloc(sizeof(SF_TestProtocol_t)))) {
				memcpy(outputData->data, data, sizeof(SF_TestProtocol_t));
			}
			break;
		case PRO_DATATYPE_STRUCT_DESC_GET:
			if ((outputData->data = pvPortMalloc(sizeof(SN_SensorDescriptorGet_t)))) {
				memcpy(outputData->data, data, sizeof(SN_SensorDescriptorGet_t));
			}
		default:
			break;
	}

	return outputData;

}

void FSM_FreeDecodedPayload(FSM_DecodedPayload_t *payload) {

	if (!payload) {
		return;
	}
	vPortFree(payload->data);
	vPortFree(payload);

}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//
//	if (htim == commSettings->timer) {
//		timerTrigger = 1;
//	}
//
//}


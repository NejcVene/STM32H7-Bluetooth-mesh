/*
 * communication_ex.c
 *
 *  Created on: Jul 24, 2024
 *      Author: Nejc
 */

#include <string.h>
#include "communication_ex.h"

#define	Toggle_Pin(port, pin, state)			{ HAL_GPIO_WritePin(port, pin, state); }
#ifdef _MASTER
#include "cmsis_os2.h"
#include "command.h"
#include "hash_table.h"
#include "node_config.h"
#include "freertos_os2.h"
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
static inline void debugMessage(char *message);
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
static PROTOCOL_STATUS _SendData(uint8_t *toSend, uint16_t sizeToSend, void (*callback)(void));
static PROTOCOL_STATUS _ReceiveData(uint8_t *toReceive, uint16_t sizeToReceive, void (*callback)(void));
#endif

// state machine functions
void FSM_Idle(void *param);
void FSM_Setup(void *param);
void FSM_Transmit(void *param);
void FSM_Receive(void *param);
void FSM_Execute(void *param);
void FSM_Error(void *param);

// timer functions

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
static FSM_ErrorReport_t error;
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
		[MAIN_FSM_EXECUTE_COMMAND][MAIN_FSM_EVENT_EMPTY_RESPONSE]	=	{MAIN_FSM_TRANSMIT, FSM_Transmit},
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
static inline void debugMessage(char *message) {

	HAL_UART_Transmit(UART_DEBUG, (uint8_t *) message, strlen(message), 3000);

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

static PROTOCOL_STATUS _SendData(uint8_t *toSend, uint16_t sizeToSend, void (*callback)(void)) {

	LPUART_CallbackTx = callback;
	if ((error.status = Comm_LPUART_Send_IT(commSettings, toSend, sizeToSend, sizeToSend)) != PRO_OK) {
		FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_ERROR, &error, sizeof(FSM_ErrorReport_t));
		return error.status;
	}
	Protocol_WaitForTX();

	return PRO_OK;

}

static PROTOCOL_STATUS _ReceiveData(uint8_t *toReceive, uint16_t sizeToReceive, void (*callback)(void)) {

	LPUART_CallbackRx = callback;
	if ((error.status = Comm_LPUART_Receive_IT(commSettings, toReceive, sizeToReceive, sizeToReceive)) != PRO_OK) {
		FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_ERROR, &error, sizeof(FSM_ErrorReport_t));
		return error.status;
	}
	Protocol_WaitForRX();

	return PRO_OK;

}

PROTOCOL_STATUS Protocol_Send(PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length, void (*callback)(void)) {

	PROTOCOL_STATUS status;
	uint16_t packetLength = payload_length + PAC_SIZE;
	Protocol_ConvertMessage(payload, payload_length);
	uint16_t checksum = Protocol_CalcChecksum(type, payload, payload_length);

#ifdef _MASTER
	CS_LOW
	HAL_Delay(100);
	CS_HIGH
#endif
	if ((status = _SendData(&PAC_START_BYTE, 1, callback)) != PRO_OK) return status;
	if ((status = _SendData((uint8_t *) &type, 1, callback)) != PRO_OK) return status;
	if ((status = _SendData((uint8_t *) &packetLength, 2, callback)) != PRO_OK) return status;
	if ((status = _SendData(payload, payload_length, callback)) != PRO_OK) return status;
	if ((status = _SendData((uint8_t *) &checksum, 2, callback)) != PRO_OK) return status;
	if ((status = _SendData(&PAC_END_BYTE, 1, callback)) != PRO_OK) return status;

	return PRO_OK;

}

PROTOCOL_STATUS Protocol_Receive(void (*callback)(void)) {

	PROTOCOL_STATUS status;

	if ((status = _ReceiveData(&startByteRx, 1, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData(&cmdTypeRx, 1, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData((uint8_t *) &lengthRx, 2, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData(payloadRx, payloadLengthRx, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData((uint8_t *) &checksumRx, 2, callback)) != PRO_OK) return status;
	if ((status = _ReceiveData(&endByteRx, 1, callback)) != PRO_OK) return status;

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
	debugMessage("IDLE State\r\n");
#endif
	errorCounter = 0;
}

void FSM_Setup(void *param) {

#ifdef _DEBUG
	debugMessage("SETUP State\r\n");
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
	debugMessage("TRANSMIT State\r\n");
#endif
#ifdef _MASTER
	char cmdToSend[CMD_MESH_COMMAND_LENGHT] = {0};
	CMD_MeshCommand_t *meshCommand;
	FSM_CommandGet_t *guiCmd = *((FSM_CommandGet_t **) param);
	if ((meshCommand = (CMD_MeshCommand_t *) HT_Search(cmdHashTable, guiCmd->commandIndex))) {
		sprintf(cmdToSend, meshCommand->command, guiCmd->commandParameters ? *((int *) guiCmd->commandParameters) : 0);
		if (Protocol_Send(meshCommand->commandType, (uint8_t *) cmdToSend, strlen(cmdToSend), NULL) == PRO_OK) {
			if (meshCommand->commandType != PRO_MSG_TYPE_UNACK) {
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, param, sizeof(param));
				// don't yet free send command, as it (might) is needed later
			} else {
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_UNACK, NULL, 0);
				vPortFree(guiCmd); // free send command, as we don't need it anymore
			}
		}
	}
#else
	Protocol_Send(PRO_MSG_TYPE_OTHER, "hello", strlen("hello"), NULL);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_TRANSMIT_COMPLETE, NULL, 0);
#endif

}

void FSM_Receive(void *param) {

#ifdef _DEBUG
	debugMessage("RECEIVE State\r\n");
#endif
	Protocol_Receive(Protocol_Process_Messsage);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_RECEIVE_COMPLETE, param, sizeof(param));

}

void FSM_Execute(void *param) {

#ifdef _DEBUG
//	debugMessage("EXECUTE State -> RECEIVED: ");
//	debugMessage((char *) CommandString);
//	debugMessage("\r\n");
#endif
#ifdef _MASTER
	// TODO: master execute command
	// TODO: provision further (work in progress)
	//			- ModelIdentifier struct
	//			- Bind to view
	//			- Switch views from prov. to config
	//			- Expand struct for view to hw for multiple params
	//			-
	// TODO: NDSCAN return value to master
	FSM_CommandGet_t *guiCmd = *((FSM_CommandGet_t **) param);
	char responseCommand[CMD_MESH_COMMAND_LENGHT];
	char responseParameters[PAC_MAX_PAYLOAD]; // = "0-F81D4FAE7DEC4B53A154819B27E180C0";
	FSM_CommandExecutionResult_t *cmdResult;
	if ((cmdResult = (FSM_CommandExecutionResult_t*) pvPortMalloc(sizeof(FSM_CommandExecutionResult_t)))) {
		sscanf((char *) CommandString, "%[^:]: %s", responseCommand, responseParameters);
		if (!strcmp(responseCommand, ((CMD_MeshCommand_t *) HT_Search(cmdHashTable, guiCmd->commandIndex))->command)) {
			if (strcmp(responseParameters, "NONE") != 0) {
				NC_ReportFoundNodes(responseParameters);
				NC_CheckEnabledModelsFeatures();
				cmdResult->result = (void *) NC_GetNodeNetworkAddressArray();
				if (osMessageQueueGetSpace(FSM_ResultQueueHandle) > 0) {
					if (osMessageQueuePut(FSM_ResultQueueHandle, &cmdResult, 0, 0) != osOK) {
						// raise error
					}
					vPortFree(guiCmd);
				}
			} else {
				// if no node was found at the vicinity of the provisioner, check for new nodes by using already provisioned nodes
				guiCmd->commandIndex = CMD_MESH_ATEP_SCAN_RANGE;
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_EMPTY_RESPONSE, param, sizeof(param));
			}
		} else {
			// send and executed commands do not match!
		}
	}
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_EXE_COMPLETE, NULL, 0);
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

void FSM_Error(void *param) {

	FSM_ErrorReport_t *e = (FSM_ErrorReport_t *) param;
#ifdef _DEBUG
	char errString[100];
	sprintf(errString, "ERROR State [%d]\r\n", e->status);
	debugMessage(errString);
#endif
	if (errorCounter == ERROR_TRESHOLD) {
		// if we get here, then we have a critical error
		// which we have to report to the user;
		// the boards will probably have to be reset
	} else {
		errorCounter++;
		switch (e->status) {
			case PRO_ERROR:
			case PRO_ERROR_START_BYTE:
			case PRO_ERROR_END_BYTE:
			case PRO_ERROR_CHECKSUM:
			case PRO_ERROR_UN_CMD:
			case PRO_ERROR_PARAM:
			case PRO_ERROR_HAL:
			case PRO_ERROR_TIMEOUT:
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_USER, &e->commandIndex, sizeof(e->commandIndex));
				break;
			default:
				break;
		}
	}

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
			strncpy((char *) CommandString, (char *) payloadRx, payloadLengthRx);
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

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//
//	if (htim == commSettings->timer) {
//		timerTrigger = 1;
//	}
//
//}


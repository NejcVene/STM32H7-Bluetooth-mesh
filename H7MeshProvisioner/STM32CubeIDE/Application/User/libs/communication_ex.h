/*
 * communication_ex.h
 *
 *  Created on: Jul 24, 2024
 *      Author: Nejc
 */

#ifndef APPLICATION_CORE_LIBS_COMMUNICATION_EX_H_
#define APPLICATION_CORE_LIBS_COMMUNICATION_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "fsm_queue.h"
#include "main.h"
#include "comm_uart.h"
#include "device_settings.h"

#define PAC_SIZE				7
#define PAC_MAX_PAYLOAD 		256
#define CMD_INDEX_SIZE			10
#define CMD_MESH_MAX_PARAMS		10
#define ERROR_TRESHOLD			3

typedef enum {
	PRO_MSG_TYPE_OTHER = 0,				// for custom messages
	PRO_MSG_TYPE_ACK,					// requires answer
	PRO_MSG_TYPE_UNACK,					// does not require answer
	PRO_MSG_TYPE_GET,					// to get status of device
	PRO_MSG_TYPE_STATUS,				// response to get command
	PRO_MSG_TYPE_ERROR,					// for reporting regular errors
	PRO_MSH_TYPE_FUN
} PROTOCOL_MSG_TYPE;

typedef enum {
	MAIN_FSM_IDLE = 0,					// FSM is waiting for an user event
	MAIN_FSM_SETUP,						// FSM sets up required variables
	MAIN_FSM_RECEIVE,					// FSM start receiving data
	MAIN_FSM_EXECUTE_COMMAND,			// FSM executed received command or updates GUI with received data
	MAIN_FSM_TRANSMIT,					// FSM starts transmitting data
	MAIN_FSM_TRANSMIT_COMPLETE,			// FSM transmit is complete,
	MAIN_FSM_ERROR,						// FSM error was detected in transmission/reception
	MAIN_FSM_NUM_OF_STATES
} MAIN_FSM_STATE;

typedef enum {
	MAIN_FSM_EVENT_USER = 0,			// user by pressing a button starts communication
	MAIN_FSM_EVENT_SETUP_COMPLETE,		// once setup is complete
	MAIN_FSM_EVENT_TRANSMIT_COMPLETE,	// once transmit is complete without errors
	MAIN_FSM_EVENT_RECEIVE_COMPLETE,	// once receive is complete without errors
	MAIN_FSM_EVENT_AKC,					// message requires an answer
	MAIN_FSM_EVENT_UNACK,				// message does not require an answer
	MAIN_FSM_EVENT_ERROR,				// an error occurred during transmission/reception
	MAIN_FSM_EVENT_EXE_COMPLETE,		// execution of command/update is complete
	MAIN_FSM_EVENT_LOOP,				// when in execution, it is possible that slave node sends empty response (ex. no nodes found)
										// we must then send to the slave to execute another command which might yield correct results or
										// a command must be sent again with different parameters (ex. subscribe add/publish set)
#ifdef _SLAVE
	MAIN_FSM_EVENT_INTERRUPT,
#endif
	MAIN_FSM_NUM_OF_EVENTS
} MAIN_FSM_EVENT;

typedef enum {
	CHECK_START_BYTE = 0,				// FSM checks received start byte
	CHECK_PACKET_TYPE,					// FSM checks received packet type
	CHECK_LENGTH,						// FSM checks message length
	CHECK_BYTE,							// FSM checks individual byte of the pay-load
	CHECK_CHECKSUM,						// FSM checks for checksum
	CHECK_END_BYTE,						// FSM checks received end byte
	CHECK_OK							// everything is OK and received data can be processed further
} PROCESS_MESSAGE_STATE;

typedef struct {
	int commandIndex;
	void *result;
} FSM_CommandExecutionResult_t;

typedef struct {
	int commandIndex;
	int arrayLength;
	void *commandParameters; // [CMD_MESH_MAX_PARAMS]; could be an array for future use, if a command needs more than one parameter
} FSM_CommandGet_t;

typedef struct {
	MAIN_FSM_STATE nextState;
	void (*stateFunction)(void *param);
} FSM_TransitionState_t;

typedef struct {
	MAIN_FSM_STATE currentState;
} FSM_State_t;

typedef struct {
	MAIN_FSM_EVENT event;
	void *param;
} FSM_QueuedEvent_t;

typedef struct {
	PROTOCOL_STATUS status;
	int commandIndex;
} FSM_ErrorReport_t;

extern Comm_IT_Responses_t itResponses;
extern void (*LPUART_CallbackTx)(void);
extern void (*LPUART_CallbackRx)(void);

void FSM_HandleEvent(FSM_State_t *fsm, FSM_QueuedEvent_t *event);
void FSM_RegisterEvent(Queue *queue, MAIN_FSM_EVENT event, void *param, size_t paramSize);
FSM_QueuedEvent_t *FSM_GetEvent(Queue *queue);
void FSM_FreeEvent(FSM_QueuedEvent_t *event);
void FSM_FreeEventsDeleteQueue(Queue *queue);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_CORE_LIBS_COMMUNICATION_EX_H_ */

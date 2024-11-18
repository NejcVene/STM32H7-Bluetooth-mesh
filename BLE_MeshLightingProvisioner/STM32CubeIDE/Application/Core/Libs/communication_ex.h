/*
 * communication_ex.h
 *
 *  Created on: Jul 24, 2024
 *      Author: Nejc
 */

#ifndef APPLICATION_CORE_LIBS_COMMUNICATION_EX_H_
#define APPLICATION_CORE_LIBS_COMMUNICATION_EX_H_

#include "main.h"
#include "queue.h"
#include "comm_uart.h"
#include "device_settings.h"

#define PAC_SIZE				7
#define PAC_MAX_PAYLOAD 		256
#define CMD_INDEX_SIZE			10
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
	PRO_DATATYPE_8T = 0,
	PRO_DATATYPE_U8T,
	PRO_DATATYPE_16T,
	PRO_DATATYPE_U16T,
	PRO_DATATYPE_32T,
	PRO_DATATYPE_U32T,
	PRO_DATATYPE_DOUBLE,
	PRO_DATATYPE_STRING,
	PRO_DATATYPE_STRUCT_APC1,
	PRO_DATATYPE_STRUCT_TEST
} PROTOCOL_DATATYPE;

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
	MAIN_FSM_EVENT_ERROR,				// an error occurred during transmission/reception,
	MAIN_FSM_EVENT_EXE_COMPLETE,		// execution of command/update is complete,
#ifdef SLAVE
	MAIN_FSM_EVENT_INTERRUPT,
	MAIN_FSM_EVENT_LOOP,
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

typedef struct {
	PROTOCOL_DATATYPE datatype;
	uint8_t *payload;
} FSM_SlaveDataSend_t;

typedef struct {
	char command[30];
	void *data;
} FSM_DecodedPayload_t;

extern Comm_IT_Responses_t itResponses;
extern void (*LPUART_CallbackTx)(void);
extern void (*LPUART_CallbackRx)(void);
extern PROTOCOL_MSG_TYPE cmdTypeConverted;

void FSM_HandleEvent(FSM_State_t *fsm, FSM_QueuedEvent_t *event);
void FSM_RegisterEvent(Queue *queue, MAIN_FSM_EVENT event, void *param, size_t paramSize);
FSM_QueuedEvent_t *FSM_GetEvent(Queue *queue);
void FSM_FreeEvent(FSM_QueuedEvent_t *event);
void FSM_FreeEventsDeleteQueue(Queue *queue);

void FSM_SetNdpvrn(void);
void FSM_UnsetNdpvrn(void);
int FSM_GetNdprn(void);
void FSM_EncodePayload(uint8_t *buffer, const char *command, void *data, size_t dataSize, PROTOCOL_DATATYPE type);
//FSM_DecodedPayload_t *FSM_DecodePayload(char *buffer, PROTOCOL_DATATYPE type);
#endif /* APPLICATION_CORE_LIBS_COMMUNICATION_EX_H_ */

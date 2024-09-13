#include "hw_spi.h"

extern UART_HandleTypeDef hlpuart1;
Comm_Settings_t *commSettings;
Queue *eventQueue;
FSM_State_t sm = {.currentState = MAIN_FSM_IDLE};

void CommEx_Protocol_Init(void) {

	eventQueue = createQueue();
	commSettings = Comm_Init(&hlpuart1, NULL);
	UTIL_SEQ_RegTask(1 << CFG_TASK_MESH_COMM_EX_REQ_ID, UTIL_SEQ_RFU, CommEx_Protocol_Task);
	UTIL_SEQ_SetTask(1 << CFG_TASK_MESH_COMM_EX_REQ_ID, CFG_SCH_PRIO_0);

}

void CommEx_Protocol_Task(void) {

	FSM_QueuedEvent_t *event;
	if (!powerup) {
		UTIL_SEQ_SetTask(1 << CFG_TASK_MESH_COMM_EX_REQ_ID, CFG_SCH_PRIO_0);
	}

	if (!isEmpty(eventQueue)) {
		if ((event = FSM_GetEvent(eventQueue))) {
			FSM_HandleEvent(&sm, event);
			FSM_FreeEvent(event);
		}
	}
	UTIL_SEQ_SetTask(1 << CFG_TASK_MESH_COMM_EX_REQ_ID, CFG_SCH_PRIO_0);

}

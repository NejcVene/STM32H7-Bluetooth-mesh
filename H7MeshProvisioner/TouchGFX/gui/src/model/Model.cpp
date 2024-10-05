#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>


#include "cmsis_os2.h"
#include "freertos_os2.h"

extern "C" {

	extern osMessageQueueId_t FSM_CommandQueueHandle;
	extern osMessageQueueId_t FSM_ResultQueueHandle;
	extern void Error_Handler();

}

Model::Model() : modelListener(0)
{

}

void Model::tick() {

	static FSM_CommandExecutionResult_t *fsmResult;
	static Node_NetworkAddress_t *newFoundDevices;
	if (osMessageQueueGetCount(FSM_ResultQueueHandle) > 0) {
		if (osMessageQueueGet(FSM_ResultQueueHandle, &fsmResult, 0, 0) == osOK) {
			newFoundDevices = (Node_NetworkAddress_t *) fsmResult->result;
			foundDevices = newFoundDevices;
			modelListener->GUI_DevicesFound();
			vPortFree(fsmResult);
		}
	}

}

void Model::GUI_SendCommand(uint16_t cmdIndex, void *cmdParam) {

	static FSM_CommandGet_t *sendCommand;
	if ((sendCommand = (FSM_CommandGet_t *) pvPortMalloc(sizeof(FSM_CommandGet_t)))) {
		sendCommand->commandIndex = cmdIndex;
		sendCommand->commandParameters = cmdParam;
		if (osMessageQueueGetSpace(FSM_CommandQueueHandle) > 0) {
			if (osMessageQueuePut(FSM_CommandQueueHandle, &sendCommand, 0, 0) != osOK) {
				Error_Handler();
			}
		}
	}

}

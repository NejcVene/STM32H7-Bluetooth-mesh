#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "cmsis_os2.h"

extern "C" {

	extern osMessageQueueId_t FSM_CommandQueueHandle;
	extern void Error_Handler();

}

Model::Model() : modelListener(0)
{

}

void Model::tick()
{

}

void Model::GUI_SendCommand(uint16_t cmdIndex) {

	if (osMessageQueueGetSpace(FSM_CommandQueueHandle) > 0) {
		if (osMessageQueuePut(FSM_CommandQueueHandle, &cmdIndex, 0, 0) != osOK) {
			Error_Handler();
		}
	}

}

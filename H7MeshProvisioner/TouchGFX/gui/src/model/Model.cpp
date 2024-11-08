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

	CMD_CommandGet_t *cmdResult;
	if (osMessageQueueGetCount(FSM_ResultQueueHandle) > 0) {
		if (osMessageQueueGet(FSM_ResultQueueHandle, &cmdResult, 0, 0) == osOK) {
			switch (cmdResult->commandIndex) {
				case CMD_MESH_ATEP_PRVN:
				case CMD_MESH_ATEP_PRVN_RANGE:
					deviceToConfigure = (Node_Config_t *) cmdResult->param[0].value.voidPtr;
					allModels = (NC_MaskedFeatures *) cmdResult->param[1].value.voidPtr;
					modelListener->GUI_ChangeScreen();
					// modelListener->GUI_ConfigureDevice();
					break;
				case CMD_MESH_ATEP_SCAN:
				case CMD_MESH_ATEP_SCAN_RANGE:
					foundDevices = (Node_NetworkAddress_t *) cmdResult->param[0].value.voidPtr;
					modelListener->GUI_DevicesFound();
					break;
				case CMD_FUN_PUB_SET_SUB_ADD:
					if (cmdResult->param[0].value.i) {
						modelListener->GUI_ConfShowPopup();
					}
					break;
				case CMD_MESH_ATCL_UNPROV:
					if (cmdResult->param[0].value.i) {
						modelListener->GUI_ChangeScreenAfterUnprov();
					}
					break;
				default:
					break;
			}
			CMD_FreeCommandGet(cmdResult);
		}
	}

}

void Model::GUI_SendCommand(CMD_CommandGet_t *cmd) {

	if (cmd) {
		if (osMessageQueueGetSpace(FSM_CommandQueueHandle) > 0) {
			if (osMessageQueuePut(FSM_CommandQueueHandle, &cmd, 0, 0) != osOK) {
				Error_Handler();
			}
		}
	}

}

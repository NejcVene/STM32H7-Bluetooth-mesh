#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>


#include "cmsis_os2.h"
#include "freertos_os2.h"

extern "C" {

	extern osMessageQueueId_t FSM_CommandQueueHandle;
	extern osMessageQueueId_t FSM_ResultQueueHandle;
	extern RTC_HandleTypeDef hrtc;
	extern RTC_TimeTypeDef sTime;
	extern RTC_DateTypeDef sDate;
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
					usedCommandIndex = cmdResult->commandIndex;
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
				case CMD_FUN_GET_LIB_VER:
					strcpy(libVer, cmdResult->param[0].value.str);
					strcpy(libSubVer, cmdResult->param[1].value.str);
					strcpy(commExVer, cmdResult->param[2].value.str);
					modelListener->GUI_ChangeScreenDevInfo();
					break;
				case CMD_MESH_ATCL_SENSOR_GET:
					sensor = (Sensor_MeasuredData_t *) cmdResult->param[0].value.voidPtr;
					modelListener->GUI_UpdateSensorValues();
					break;
				case CMD_MESH_ATCL_SENSOR_DESC_GET:
					modelListener->GUI_ConfirmDescGet();
					break;
				case CMD_ERROR:
					strcpy(errorString, cmdResult->param[0].value.str);
					modelListener->GUI_NotifyError();
					break;
				case CMD_MESH_ATEP_ROOT:
				case CMD_FUN_IS_EM_PROV_PROV:
					modelListener->GUI_NodeIsConfigured();
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

void Model::DigitalClock() {

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); // this is needed to unlock the values in the higher-order calendar shadow registers
	seconds = sTime.Seconds;
	minutes = sTime.Minutes;
	hours = sTime.Hours;

}

void Model::GUI_SetDigialClock(uint8_t hours, uint8_t minutes) {

	sTime.Hours = hours;
	sTime.Minutes = minutes;
	sTime.Seconds = 0;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

}

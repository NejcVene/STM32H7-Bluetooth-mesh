/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_sensors_client.c
  * @author  MCD Application Team
  * @brief   Application interface for Generic Mesh Models 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "types.h"
#include "appli_sensor.h"
#include "appli_light.h"
#include "common.h"
#include "mesh_cfg_usr.h"
#include "appli_nvm.h"
#include "appli_mesh.h"
#include "sensors_client.h"
#include "appli_sensors_client.h"
#include "appli_light_lc.h"
#include "communication_ex.h"

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
typedef struct __attribute__((packed)) {
	uint16_t pm1_0;
	double tComp;
} APC1_SelectedData_t;

typedef struct __attribute__((packed)) {
	uint16_t descCount;
	uint16_t descriptors[3]; // a node can only have three sensors
} SN_SensorDescriptorGet_t;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern Queue *eventQueue;
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/**
* @brief  Appli_SensorsClient_API: This function is a Wrapper to call Sensors Client API depending on opcode received in input
* @param  elementIndex: Index of the element
* @param  msg_opcode: opcode of the desired API
* @param  msg_params: buffer containing the desired API parameters
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_SensorsClient_API(MOBLEUINT8 elementIndex, MOBLEUINT16 msg_opcode, MOBLEUINT8 *msg_params) 
{ 
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  switch(msg_opcode){
/******************************************************************************/
#ifdef ENABLE_SENSOR_MODEL_CLIENT 
/******************************************************************************/
  case SENSOR_DESCRIPTOR_GET:
    {
      result = SensorsClient_Descriptor_Get(elementIndex, msg_params);
      break;
    }
  case SENSOR_CADENCE_GET:
    {
      result = SensorsClient_Cadence_Get(elementIndex, msg_params);
      break;
    }
  case SENSOR_CADENCE_SET:
    {
      result = SensorsClient_Cadence_Set(elementIndex,
                              msg_params, 
                              10); //(sizeof(sensor_CadenceCbParam_t)-3)); 
      break;
    }
  case SENSOR_CADENCE_SET_UNACK:
    {  
      result = SensorsClient_Cadence_Set_Unack(elementIndex,
                              msg_params, 
                              10); //(sizeof(sensor_CadenceCbParam_t)-3));
      break;
    }
  case SENSOR_SETTINGS_GET:
    {
      result = SensorsClient_Settings_Get(elementIndex, msg_params);
      break;
    }
    
      case SENSOR_SETTING_GET:
    {
      result = SensorsClient_Setting_Get(elementIndex, msg_params, sizeof(sensor_SettingsCbParams_t));
      break;
    }
  case SENSOR_SETTING_SET:
    {
      
      result = SensorsClient_Setting_Set(elementIndex,
                              msg_params, 
                              6);
      break;
    }
  case SENSOR_SETTING_SET_UNACK:
    {  
      
      result = SensorsClient_Setting_Set_Unack(elementIndex,
                              msg_params, 
                              6);
      break;
    }  
  case SENSOR_GET:
    {  
      result = SensorsClient_Get(elementIndex, msg_params);
      break;
    }
  case SENSOR_COLUMN_GET:
    {  
      result = SensorsClient_Column_Get(elementIndex, msg_params, 4);
      break;
    }
  case SENSOR_SERIES_GET:
    {  
      result = SensorsClient_Series_Get(elementIndex, msg_params, 4);
      break;
    }
/******************************************************************************/    
#endif /* #ifdef ENABLE_SENSOR_MODEL_CLIENT */
/******************************************************************************/   
    

  default:
    {
      TRACE_M(TF_SENSOR, "OpCode value invalid %d \r\n", msg_opcode);
      result = MOBLE_RESULT_FAIL;
    }
  }
  return result;
}

/**
  * @brief  Callback corresponding to Appli_Sensor_Cadence_Status
  * @param  Descriptor parameters
  * @param  
* @retval None
*/ 
void Appli_Sensor_Cadence_Status(const MOBLEUINT8 *pCadence,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex)
{
  MOBLEUINT8 i;
  
  TRACE_M(TF_SENSOR,"Appli_Sensor_Cadence_Status callback received \r\n");
  
  TRACE_M(TF_SERIAL_CTRL,"#%d! for element %d \r\n", 
          SENSOR_CADENCE_STATUS,
          elementIndex);
  for(i = 0; i < length; i++)
  {
    TRACE_M(TF_SERIAL_CTRL,"Cadence value: %d\n\r", pCadence[i]);
  }
}
      
      
/**
  * @brief  Callback corresponding to Appli_Sensor_Settings_Status
  * @param  Descriptor parameters
  * @param  
* @retval None
*/ 
void Appli_Sensor_Settings_Status(const MOBLEUINT8 *pSettings,
                                  MOBLEUINT32 length,
                                  MOBLE_ADDRESS dstPeer,
                                  MOBLEUINT8 elementIndex)
{
  MOBLEUINT8 i;
  
  TRACE_M(TF_SENSOR,"Appli_Sensor_Settings_Status callback received \r\n");
  
  TRACE_M(TF_SERIAL_CTRL,"#%d! for element %d \r\n", 
          SENSOR_SETTINGS_STATUS,
          elementIndex);
  for(i = 0; i < length; i++)
  {
    TRACE_M(TF_SERIAL_CTRL,"Settings value: %d\n\r", pSettings[i]);
  }
}
      
      
/**
  * @brief  Callback corresponding to Appli_Sensor_Setting_Status
  * @param  Descriptor parameters
  * @param  
* @retval None
*/ 
void Appli_Sensor_Setting_Status(const MOBLEUINT8 *pSetting,
                                  MOBLEUINT32 length,
                                  MOBLE_ADDRESS dstPeer,
                                  MOBLEUINT8 elementIndex)
{
  MOBLEUINT8 i;
  
  TRACE_M(TF_SENSOR,"Appli_Sensor_Setting_Status callback received \r\n");
  
  TRACE_M(TF_SERIAL_CTRL,"#%d! for element %d \r\n", 
          SENSOR_SETTING_STATUS,
          elementIndex);
  for(i = 0; i < length; i++)
  {
    TRACE_M(TF_SERIAL_CTRL,"Setting value: %d\n\r", pSetting[i]);
  }
}
      
      
/**
  * @brief  Callback corresponding to Appli_Sensor_Descriptor_Status
  * @param  Descriptor parameters
  * @param  
* @retval None
*/ 
void Appli_Sensor_Descriptor_Status(const MOBLEUINT8 *pDescriptor,
                                    MOBLEUINT32 length,
                                    MOBLE_ADDRESS dstPeer,
                                    MOBLEUINT8 elementIndex) {

	MOBLEUINT8 i;
	int index = 0;
	SN_SensorDescriptorGet_t sensorsDesc;
	uint8_t sendingBuffer[PAC_MAX_PAYLOAD] = {0};

	TRACE_M(TF_SENSOR,"Appli_Sensor_Descriptor_Status callback received \r\n");
	TRACE_M(TF_SERIAL_CTRL,"#%d! for element %d \r\n", SENSOR_DESCRIPTOR_STATUS, elementIndex);

	for(i = 0; i < length; i++) {
		TRACE_M(TF_SERIAL_CTRL,"Descriptor value: %d\n\r", pDescriptor[i]);
		if (pDescriptor[i] != 0) {
			sensorsDesc.descriptors[index++] = pDescriptor[i];
		}
	}
	sensorsDesc.descCount = index;
	FSM_EncodePayload(sendingBuffer,
					"ATCL",
					(void *) &sensorsDesc,
					sizeof(SN_SensorDescriptorGet_t),
					PRO_DATATYPE_STRUCT_DESC_GET);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, sendingBuffer, sizeof(sendingBuffer));


}
      
      
/**
  * @brief  Callback corresponding to Appli_Sensor_Status
  * @param  Descriptor parameters
  * @param  
* @retval None
*/ 
void Appli_Sensor_Status(const MOBLEUINT8 *pStatus,
                         MOBLEUINT32 length,
                         MOBLE_ADDRESS dstPeer,
                         MOBLEUINT8 elementIndex) {

	MOBLEUINT8 i;
	MOBLEUINT8 sensorStatusFormat = 0xFF;
	MOBLEUINT32 receivedLenght = 0;
	MOBLEUINT16 receivedPID = 0;
	MOBLEUINT32 offset = 2;
	uint8_t sendingBuffer[PAC_MAX_PAYLOAD] = {0};
	APC1_SelectedData_t sensorData;

	TRACE_M(TF_SENSOR,"Appli_Sensor_Status callback received \r\n");
	TRACE_M(TF_SERIAL_CTRL,"#%d! for element %d \r\n", SENSOR_STATUS, elementIndex);
	for(i = 0; i < length; i++) {
		TRACE_M(TF_SERIAL_CTRL,"Status value: %d\n\r", pStatus[i]);
	}
	// decode data marshalling buffer
	if((pStatus[0] & 0x01) == 0x01){
		sensorStatusFormat = 0x01;
		receivedLenght = (MOBLEUINT32) (((pStatus[0] & 0xFE)>>1) + 1);
	    receivedPID = (MOBLEUINT16) ((pStatus[2] << 8) | (pStatus[1]));
	} else{
		sensorStatusFormat = 0x00;
	    receivedLenght = (MOBLEUINT32) (((pStatus[0] & 0x1E) >> 1) + 1);
	    receivedPID = (MOBLEUINT16) (((pStatus[0] & 0xE0) >> 5) | (pStatus[1] << 3));
	}
	offset = length - receivedLenght;
	TRACE_M(TF_SENSOR,"Offset: %ld\r\n", offset);
	switch (receivedPID) {
		case PRESENT_AMBIENT_TEMPERATURE_PID:
			memcpy((void *) &sensorData, pStatus + offset, sizeof(APC1_SelectedData_t));
			TRACE_M(TF_SENSOR, "APC1 PM1.0: %d\r\n", sensorData.pm1_0);
			TRACE_M(TF_SENSOR, "APC1 Temp: %f °C\r\n", sensorData.tComp);
			break;
	}
	FSM_EncodePayload(sendingBuffer,
					"ATCL",
					(void *) &sensorData,
					sizeof(APC1_SelectedData_t),
					PRO_DATATYPE_STRUCT_APC1);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, sendingBuffer, sizeof(sendingBuffer));
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC   
	Appli_Light_LC_SensorPropertyUpdate(elementIndex,
                                      PRESENCE_DETECTED_PID,
                                      (MOBLEUINT32) pStatus[length-1]);
#endif

}
      
      
/**
  * @brief  Callback corresponding to Appli_Sensor_Column_Status
  * @param  Descriptor parameters
  * @param  
* @retval None
*/ 
void Appli_Sensor_Column_Status(const MOBLEUINT8 *pColumn,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex)
{
  MOBLEUINT8 i;
  
  TRACE_M(TF_SENSOR,"Appli_Sensor_Column_Status callback received \r\n");
  
  TRACE_M(TF_SERIAL_CTRL,"#%d! for element %d \r\n", 
          SENSOR_COLUMN_STATUS,
          elementIndex);
  for(i = 0; i < length; i++)
  {
    TRACE_M(TF_SERIAL_CTRL,"Column Status value: %d\n\r", pColumn[i]);
  }
}
      
      
/**
  * @brief  Callback corresponding to Appli_Sensor_Series_Status
  * @param  Descriptor parameters
  * @param  
* @retval None
*/ 
void Appli_Sensor_Series_Status(const MOBLEUINT8 *pSeries,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex)
{
  MOBLEUINT8 i;
  
  TRACE_M(TF_SENSOR,"Appli_Sensor_Series_Status callback received \r\n");
  
  TRACE_M(TF_SERIAL_CTRL,"#%d! for element %d \r\n", 
          SENSOR_SERIES_STATUS,
          elementIndex);
  for(i = 0; i < length; i++)
  {
    TRACE_M(TF_SERIAL_CTRL,"Series Status value: %d\n\r", pSeries[i]);
  }
}


/**
* @}
*/

/**
* @}
*/



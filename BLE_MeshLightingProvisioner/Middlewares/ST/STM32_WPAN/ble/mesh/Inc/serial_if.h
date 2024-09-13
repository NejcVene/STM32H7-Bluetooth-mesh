/**
******************************************************************************
* @file    serial_if.h
* @author  BLE Mesh Team
* @brief   Header file for the serial interface file 
******************************************************************************
* @attention
*
* Copyright (c) 2018-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERIAL_IF_H
#define __SERIAL_IF_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#define RX_BUFFER_SIZE                                                        8U
#define C_SIZE_CMD_STRING                                                   256U
extern uint8_t aRxBuffer[RX_BUFFER_SIZE];
extern uint8_t CommandString[C_SIZE_CMD_STRING];
extern uint16_t indexReceiveChar;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */
/* Exported Functions Prototypes ---------------------------------------------*/
void Serial_Init(void);
void Serial_RxCpltCallback( void );
void Serial_PrintStringCb(const char *message);
void Serial_InterfaceProcess(void);
MOBLEUINT8 Serial_CharToHexConvert(char addr);
void Serial_Uart_Rx_Task( void );


#endif /* __SERIAL_IF_H */




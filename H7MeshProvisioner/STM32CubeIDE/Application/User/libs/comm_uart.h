/*
 * comm_uart.h
 *
 *  Created on: Sep 1, 2024
 *      Author: Nejc
 */

#ifndef INC_COMM_UART_H_
#define INC_COMM_UART_H_

#include "main.h"
#include "device_settings.h"

#define TIM2_BASE_PRESCALER	(19999 + 1)
#define TIM2_BASE_CLOCK		200 // in MHz
#define DEFAULT_TX_TIMEOUT	10

typedef enum {
	COMM_OK = 0,
	COMM_ERROR,
	COMM_ERROR_NULL_POINTER
} COMM_STATUS;

typedef enum {
	PRO_OK = 0,							// message is OK
	PRO_ERROR,							// general error
	PRO_ERROR_START_BYTE,				// message has wrong start byte
	PRO_ERROR_END_BYTE,					// message has wrong end byte
	PRO_ERROR_CHECKSUM,					// message has wrong checksum
	PRO_ERROR_UN_CMD, 					// message contains unknown command
	PRO_ERROR_TIMEOUT,					// timeout has occurred waiting for HS pin
	PRO_ERROR_PARAM,
	PRO_ERROR_HAL
} PROTOCOL_STATUS;

typedef struct {
	UART_HandleTypeDef *commLpUart;
	TIM_HandleTypeDef *timer;
} Comm_Settings_t;

typedef struct {
	volatile uint8_t lpUartRxFlag,
					 lpUartTxFlag;
} Comm_IT_Responses_t;

Comm_Settings_t *Comm_Init(UART_HandleTypeDef *lpuart, TIM_HandleTypeDef *timer);
PROTOCOL_STATUS Comm_LPUART_Send_IT(Comm_Settings_t *setting, uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, uint32_t timeout);
PROTOCOL_STATUS Comm_LPUART_Receive_IT(Comm_Settings_t *setting, uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, uint32_t timeout);
void Comm_Free(Comm_Settings_t *setting);
void Protocol_WaitForTX(void);
void Protocol_WaitForRX(void);
#ifdef _MASTER
void Protocol_WaitForHS(void);
void TIM_Start(Comm_Settings_t *setting);
void TIM_Stop(Comm_Settings_t *setting);
void TIM_Reset(Comm_Settings_t *setting);
int TIM_GetTimerTrigger();
void TIM_SetTimerTrigger(int val);
void TIM_SetTimeout(Comm_Settings_t *setting, uint32_t timeout);
#endif

#endif /* INC_COMM_UART_H_ */

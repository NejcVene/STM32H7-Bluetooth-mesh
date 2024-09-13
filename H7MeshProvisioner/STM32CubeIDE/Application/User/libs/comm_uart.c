/*
 * comm_uart.c
 *
 *  Created on: Sep 1, 2024
 *      Author: Nejc
 */

#include "comm_uart.h"
#include "freertos_os2.h"

#ifdef _SLAVE
#define	Toggle_Pin(port, pin, state)			{ HAL_GPIO_WritePin(port, pin, state); }
#define HS_LOW									{ Toggle_Pin(COMM_HS_PORT_SLAVE, COMM_HS_PIN_SLAVE, GPIO_PIN_RESET); }
#define HS_HIGH									{ Toggle_Pin(COMM_HS_PORT_SLAVE, COMM_HS_PIN_SLAVE, GPIO_PIN_SET); }
#endif

volatile int timerTrigger;
Comm_IT_Responses_t itResponses;
volatile int hsTrigger;

void Protocol_WaitForTX(void) {

	while (!itResponses.lpUartTxFlag);

}

void Protocol_WaitForRX(void) {

	while (!itResponses.lpUartRxFlag);

}

void Protocol_WaitForHS(void) {

	while (!hsTrigger && !timerTrigger);
	hsTrigger = 0;

}

Comm_Settings_t *Comm_Init(UART_HandleTypeDef *lpuart, TIM_HandleTypeDef *timer) {

	Comm_Settings_t *tmp;
	if (!lpuart || !timer || !(tmp = (Comm_Settings_t *) pvPortMalloc(sizeof(Comm_Settings_t)))) {
		// raise error
		return NULL;
	}
	tmp->commLpUart = lpuart;
	tmp->timer = timer;

	return tmp;

}

COMM_STATUS Comm_CheckParameters(uint8_t *buffer, uint16_t size, uint16_t bufferSize) {

	if (!buffer ||
		bufferSize == 0 ||
		size == 0 ||
		size > bufferSize) {
		return COMM_ERROR;
	}

	return COMM_OK;

}

PROTOCOL_STATUS Comm_LPUART_Send_IT(Comm_Settings_t *setting, uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize) {

	if (Comm_CheckParameters(buffer, sizeToSend, bufferSize) != COMM_OK) {
		return PRO_ERROR_PARAM;
	}
	itResponses.lpUartTxFlag = 0;

#ifdef _MASTER
	TIM_Start(setting);
	Protocol_WaitForHS();
	if (timerTrigger) {
		return PRO_ERROR_TIMEOUT;
	}
#endif

	if (HAL_UART_Transmit_IT(setting->commLpUart, buffer, sizeToSend) != HAL_OK) {
		return PRO_ERROR_HAL;
	}
#ifdef _SLAVE
	else {
		HS_HIGH
		HAL_Delay(100);
		HS_LOW
	}
#endif

	return PRO_OK;

}

PROTOCOL_STATUS Comm_LPUART_Receive_IT(Comm_Settings_t *setting, uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize) {

	HAL_StatusTypeDef status;
	if (Comm_CheckParameters(buffer, sizeToReceive, bufferSize)) {
		return PRO_ERROR_PARAM;
	}
	itResponses.lpUartRxFlag = 0;

#ifdef _MASTER
	TIM_Start(setting);
	Protocol_WaitForHS();
	if (timerTrigger) {
		return PRO_ERROR_TIMEOUT;
	}
#endif

	if ((status = HAL_UART_Receive_IT(setting->commLpUart, buffer, sizeToReceive)) != HAL_OK) {
		return PRO_ERROR_HAL;
	}
#ifdef _SLAVE
	else {
		HS_HIGH
		HAL_Delay(100);
		HS_LOW
	}
#endif

	return PRO_OK;

}

void Comm_Free(Comm_Settings_t *setting) {

	vPortFree(setting);

}

#ifdef _MASTER
void TIM_Start(Comm_Settings_t *setting) {

	TIM_Reset(setting);
	HAL_TIM_Base_Start_IT(setting->timer);

}

void TIM_Stop(Comm_Settings_t *setting) {

	HAL_TIM_Base_Stop_IT(setting->timer);

}

void TIM_Reset(Comm_Settings_t *setting) {

	timerTrigger = 0;
	HAL_TIM_Base_Stop_IT(setting->timer);
	__HAL_TIM_SET_COUNTER(setting->timer, 0);

}
#endif

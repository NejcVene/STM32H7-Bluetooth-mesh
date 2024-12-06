/*
 * comm_uart.c
 *
 *  Created on: Sep 1, 2024
 *      Author: Nejc
 */

#include "comm_uart.h"
#include "freertos_os2.h"
#include "task.h"
#include <math.h>
#include <limits.h>

#ifdef _SLAVE
#define	Toggle_Pin(port, pin, state)			{ HAL_GPIO_WritePin(port, pin, state); }
#define HS_LOW									{ Toggle_Pin(COMM_HS_PORT_SLAVE, COMM_HS_PIN_SLAVE, GPIO_PIN_RESET); }
#define HS_HIGH									{ Toggle_Pin(COMM_HS_PORT_SLAVE, COMM_HS_PIN_SLAVE, GPIO_PIN_SET); }
#endif

static volatile int timerTrigger;
Comm_IT_Responses_t itResponses;
volatile int hsTrigger;

#ifdef _DEBUG
extern UART_HandleTypeDef huart3;
static inline void debugMessage(char *message) {

	HAL_UART_Transmit(&huart3, (uint8_t *) message, strlen(message), 3000);

}
#endif

/**
  * @brief  Wait for transmission on LPUART to complete.
  * @param  None
  * @retval	None
  */
void Protocol_WaitForTX(void) {

	while (!itResponses.lpUartTxFlag);

}

/**
  * @brief  Wait for reception on LPUART to complete.
  * @param  None
  * @retval	None
  */
void Protocol_WaitForRX(void) {

	while (!itResponses.lpUartRxFlag);

}

/**
  * @brief  Wait for either handshake pin or timeout (whichever comes first).
  * @param  None
  * @retval	None
  */
void Protocol_WaitForHS(void) {

	while (!hsTrigger && !timerTrigger);
	hsTrigger = 0;

}

/**
  * @brief  Initializes the Communication interface.
  * @param  lpuart	Pointer to a UART_HandleTypeDef structure that contains
  *               	the configuration information for the specified UART module.
  * @param	timer	Pointer to a TIM_HandleTypeDef structure that contains the
  * 				configuration information for the specified timer.
  * @retval	Comm_Settings_t	pointer
  */
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

/**
  * @brief  Check for correct communication parameters before use on LPUART.
  * @param  buffer		Pointer to a uint8_t buffer that contains data to be sent,
  * 					or received.
  * @param	size		A uint16_t value which specified the amount of data to be
  * 					sent or received.
  * @param	bufferSize	A uint16_t value which specifies size of the used buffer.
  * @retval	COMM_STATUS	status
  */
COMM_STATUS Comm_CheckParameters(uint8_t *buffer, uint16_t size, uint16_t bufferSize) {

	if (!buffer ||
		bufferSize == 0 ||
		size == 0 ||
		size > bufferSize) {
		return COMM_ERROR;
	}

	return COMM_OK;

}

/**
  * @brief  Send data using LPUART.
  * @param  setting		Pointer to a Comm_Settings_t structure that contains initialization
  * 					data for the communication interface.
  * @param	buffer		A uint8_t pointer to a buffer which contains data to be sent.
  * @param	sizeToSend	A uint16_t value which specifies the amount of data to sent.
  * @param	bufferSize	A uint16_t value which specifies size of the used buffer.
  * @param	timeout		A uint32_t value which when timeout should occur.
  * @retval	PROTOCOL_STATUS	status
  */
PROTOCOL_STATUS Comm_LPUART_Send_IT(Comm_Settings_t *setting, uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, uint32_t timeout) {

	if (Comm_CheckParameters(buffer, sizeToSend, bufferSize) != COMM_OK) {
		return PRO_ERROR_PARAM;
	}
	itResponses.lpUartTxFlag = 0;

#ifdef _MASTER
	TIM_SetTimeout(setting, timeout);
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

/**
  * @brief  Receive data using LPUART.
  * @param  setting			Pointer to a Comm_Settings_t structure that contains initialization
  * 						data for the communication interface.
  * @param	buffer			A uint8_t pointer to a buffer which will contain received data.
  * @param	sizeToReceive	A uint16_t value which specifies the amount of data to receive.
  * @param	bufferSize		A uint16_t value which specifies size of the used buffer.
  * @param	timeout			A uint32_t value which when timeout should occur.
  * @retval	PROTOCOL_STATUS	status
  */
PROTOCOL_STATUS Comm_LPUART_Receive_IT(Comm_Settings_t *setting, uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, uint32_t timeout) {

	HAL_StatusTypeDef status;
	if (Comm_CheckParameters(buffer, sizeToReceive, bufferSize)) {
		return PRO_ERROR_PARAM;
	}
	itResponses.lpUartRxFlag = 0;

#ifdef _MASTER
	TIM_SetTimeout(setting, timeout);
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

/**
  * @brief  Free allocated memory for communication interface.
  * @param  setting			Pointer to a Comm_Settings_t structure that contains initialization
  * 						data for the communication interface.
  * @retval	None
  */
void Comm_Free(Comm_Settings_t *setting) {

	vPortFree(setting);

}

#ifdef _MASTER

/**
  * @brief  Get timerTrigger value.
  * @param  None
  * @retval	int
  */
int TIM_GetTimerTrigger() {

	return timerTrigger;

}

/**
  * @brief  Set timerTrigger value.
  * @param  A int value to which timerTrigger will be set
  * @retval	None
  */
void TIM_SetTimerTrigger(int val) {

	timerTrigger = val;

}

/**
  * @brief  Start timer for timeout.
  * @note	Will also reset the timer.
  * @param  setting	Pointer to a Comm_Settings_t structure that contains initialization
  * 				data for the communication interface.
  * @retval	None
  */
void TIM_Start(Comm_Settings_t *setting) {

	TIM_Reset(setting);
	HAL_TIM_Base_Start_IT(setting->timer);

}

/**
  * @brief  Stop timer for timeout.
  * @param  setting	Pointer to a Comm_Settings_t structure that contains initialization
  * 				data for the communication interface.
  * @retval	None
  */
void TIM_Stop(Comm_Settings_t *setting) {

	HAL_TIM_Base_Stop_IT(setting->timer);

}

/**
  * @brief  Reset timer for timeout.
  * @note	Will stop the timer, reset its counter and clear flags.
  * @param  setting	Pointer to a Comm_Settings_t structure that contains initialization
  * 				data for the communication interface.
  * @retval	None
  */
void TIM_Reset(Comm_Settings_t *setting) {

	timerTrigger = 0;
	HAL_TIM_Base_Stop_IT(setting->timer);
	__HAL_TIM_SET_COUNTER(setting->timer, 0);
	__HAL_TIM_CLEAR_FLAG(setting->timer, TIM_FLAG_UPDATE);

}

/**
  * @brief  Set timeout for timer in seconds.
  * @note	Will also reset the timer.
  * @param  setting	Pointer to a Comm_Settings_t structure that contains initialization
  * 				data for the communication interface.
  * @param  timeout	A uint32_t value which represents the set timeout in seconds.
  * @retval	None
  */
void TIM_SetTimeout(Comm_Settings_t *setting, uint32_t timeout) {

	// float will give us enough precision
	float Tcounter = 0;
	uint32_t newARR = 0;

	Tcounter = (float) TIM2_BASE_PRESCALER / (float) (TIM2_BASE_CLOCK * 1e6);
	newARR = (uint32_t) (timeout / Tcounter);
	__HAL_TIM_SET_AUTORELOAD(setting->timer, newARR);

#ifdef _DEBUG
	char message[30];
	float updateEvent = ((float) (TIM2_BASE_CLOCK * 1e6)) / ((float) TIM2_BASE_PRESCALER * newARR);
	sprintf(message, "Calc. delay: %d (%ld)\r\n", (int) (1 / updateEvent), timeout);
	debugMessage(message);
#endif

	TIM_Reset(setting);

}

#endif

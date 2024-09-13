/*
 * communication.c
 *
 *  Created on: Jun 19, 2024
 *      Author: Nejc
 */


#include "communication.h"

#if defined(MASTER) && !defined(SLAVE)
#define _MASTER
#elif !defined(MASTER) && defined(SLAVE)
#define _SLAVE
#else
#error Conflicting master slave SPI setting
#endif

#define INTERRUPT_MODE							1
#define BLOCKING_MODE							0
#define Check_Params(buffer, size, bufferSize)	{ if (Check_Parameters(buffer, size, bufferSize) != COMM_OK) return COMM_ERROR; }
#define	Toggle_Pin(pin, state)					{ HAL_GPIO_WritePin(pin, state); }
#ifdef _MASTER
#define Check_HS_Pin()							{ while (HAL_GPIO_ReadPin(COMM_HS_MASTER) == 0); }
#define CS_LOW									{ Toggle_Pin(COMM_CS_MASTER, GPIO_PIN_RESET) }
#define CS_HIGH									{ Toggle_Pin(COMM_CS_MASTER, GPIO_PIN_SET) }
#else
#define Check_CS_Pin()							{ while (HAL_GPIO_ReadPin(COMM_CS_SLAVE)); }
#define HS_LOW									{ Toggle_Pin(COMM_HS_SLAVE, GPIO_PIN_RESET); }
#define HS_HIGH									{ Toggle_Pin(COMM_HS_SLAVE, GPIO_PIN_SET); }
#endif

struct Comm_Settings comm_settings;
struct Comm_IT_Responses it_responses;
void (*SPI_Rx_Cb)(void);
void (*SPI_Tx_Cb)(void);
void (*UART_Rx_Cb)(void);
void (*UART_Tx_Cb)(void);

enum Comm_Status Check_Parameters(uint8_t *buffer, uint16_t size, uint16_t bufferSize);
enum Comm_Status _SPI_Send(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, uint8_t mode);
enum Comm_Status _SPI_Receive(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, uint8_t mode);
enum Comm_Status _UART_Send(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, uint8_t mode);
enum Comm_Status _UART_Receive(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, uint8_t mode);

/**
  * @brief  Initialize the library.
  * @param  *uart: pointer to a UART_HandleTypeDef structure that contains
  *         the configuration information for UART module.
  * @param  *spi: pointer to a SPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module.
  * @retval Comm status
  */
enum Comm_Status Comm_Init(UART_HandleTypeDef *uart, SPI_HandleTypeDef *spi) {

	if (!uart && !spi) {
		return COMM_ERROR_NULL_POINTER;
	}

	comm_settings.comm_spi = spi;
	comm_settings.comm_uart = uart;

	return COMM_OK;

}

/**
  * @brief  Check parameters for errors before using them for send/receive.
  * @note	Check for null pointer on buffer, if size and/or bufferSize is zero, or
  * 		size of data to read/write is bigger then the buffer itself.
  * @param  *buffer: pointer to a buffer from which data will be read or written.
  * @param  size: size of data that will be read or written.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Check_Parameters(uint8_t *buffer, uint16_t size, uint16_t bufferSize) {

	if (!buffer ||
		bufferSize == 0 ||
		size == 0 ||
		size > bufferSize) {
		return COMM_ERROR;
	}

	return COMM_OK;

}

/**
  * @brief  Send data with SPI.
  * @note	Internal function, not to be called by the user.
  * @param  *buffer: pointer to a buffer from which data will be read to send.
  * @param  sizeToSend: size of data that will be sent.
  * @param  bufferSize: size of buffer.
  * @param	mode: blocking or interrupt mode
  * @retval Comm status
  */
enum Comm_Status _SPI_Send(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, uint8_t mode) {

	Check_Params(buffer, sizeToSend, bufferSize)

#ifdef _MASTER
	CS_LOW
	Check_HS_Pin()
#else
	HS_HIGH
	Check_CS_Pin()
#endif

	if (mode) { // SPI interrupt mode
		it_responses.spi_tx_it_flag = 0;
		if (HAL_SPI_Transmit_IT(comm_settings.comm_spi, buffer, sizeToSend) != HAL_OK) {
	#ifdef _MASTER
			CS_HIGH
	#else
			HS_LOW
	#endif
			return COMM_ERROR;
		}
	} else { // SPI blocking mode
		if (HAL_SPI_Transmit(comm_settings.comm_spi, buffer, sizeToSend, COMM_TIMEOUT) != HAL_OK) {
#ifdef _MASTER
			CS_HIGH
#else
			HS_LOW
#endif
			return COMM_ERROR;
		}
	}

	return COMM_OK;

}

/**
  * @brief  Receive data with SPI.
  * @note	Internal function, not to be called by the user.
  * @param  *buffer: pointer to a buffer from which data will be written to.
  * @param  sizeToReceive: size of data that will be receive.
  * @param  bufferSize: size of buffer.
  * @param	mode: blocking or interrupt mode
  * @retval Comm status
  */
enum Comm_Status _SPI_Receive(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, uint8_t mode) {

	Check_Params(buffer, sizeToReceive, bufferSize)

#ifdef _MASTER
	Check_HS_Pin()
#else
	Check_CS_Pin()
#endif

	if (mode) { // SPI interrupt mode
		it_responses.spi_rx_it_flag = 0;
		if (HAL_SPI_Receive_IT(comm_settings.comm_spi, buffer, sizeToReceive) != HAL_OK) {
#ifdef _MASTER
			CS_HIGH
#else
			HS_HIGH
#endif
			return COMM_ERROR;
		}

#ifdef _MASTER
		CS_LOW
#else
		HS_HIGH
#endif

	} else { // SPI blocking mode
		if (HAL_SPI_Receive(comm_settings.comm_spi, buffer, sizeToReceive, COMM_TIMEOUT) != HAL_OK) {
#ifdef _MASTER
			CS_HIGH
#else
			HS_LOW
#endif
			return COMM_ERROR;
		}
	}

	return COMM_OK;

}

/**
  * @brief  Send data with UART.
  * @note	Internal function, not to be called by the user.
  * @param  *buffer: pointer to a buffer from which data will be read to send.
  * @param  sizeToSend: size of data that will be sent.
  * @param  bufferSize: size of buffer.
  * @param	mode: blocking or interrupt mode
  * @retval Comm status
  */
enum Comm_Status _UART_Send(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, uint8_t mode) {

	Check_Params(buffer, sizeToSend, bufferSize)

#ifdef _MASTER
	Check_HS_Pin()
#else
	HS_HIGH
#endif

	if (mode) {
		it_responses.uart_tx_it_flag = 0;
		if (HAL_UART_Transmit_IT(comm_settings.comm_uart, buffer, sizeToSend) != HAL_OK) {
#ifdef _SLAVE
			HS_LOW
#endif
			return COMM_ERROR;
		}

	} else {
		if (HAL_UART_Transmit(comm_settings.comm_uart, buffer, sizeToSend, COMM_TIMEOUT) != HAL_OK) {
#ifdef _SLAVE
			HS_LOW
#endif
			return COMM_ERROR;
		}
	}

#ifdef _SLAVE
	HS_LOW
#endif

	return COMM_OK;

}

/**
  * @brief  Receive data with UART.
  * @note	Internal function, not to be called by the user.
  * @param  *buffer: pointer to a buffer from which data will be written to.
  * @param  sizeToReceive: size of data that will be received.
  * @param  bufferSize: size of buffer.
  * @param	mode: blocking or interrupt mode
  * @retval Comm status
  */
enum Comm_Status _UART_Receive(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, uint8_t mode) {

	Check_Params(buffer, sizeToReceive, bufferSize)

#ifdef _MASTER
	Check_HS_Pin()
#else
	HS_HIGH
#endif

	if (mode) {
		it_responses.uart_rx_it_flag = 0;
		if (HAL_UART_Receive_IT(comm_settings.comm_uart, buffer, sizeToReceive) != HAL_OK) {
#ifdef _SLAVE
			HS_LOW
#endif
			return COMM_ERROR;
		}

	} else {
		if (HAL_UART_Receive(comm_settings.comm_uart, buffer, sizeToReceive, COMM_TIMEOUT) != HAL_OK) {
#ifdef _SLAVE
			HS_LOW
#endif
			return COMM_ERROR;
		}
	}


#ifdef _SLAVE
	HS_LOW
#endif

	return COMM_OK;

}

// UART Blocking mode

/**
  * @brief  Send sizeToSend amount of data with UART using BLOCKING mode.
  * @param  *buffer: pointer to a buffer from which data will be read to send.
  * @param  sizeToSend: size of data that will be sent.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Comm_UART_Send(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize) {

	return _UART_Send(buffer, sizeToSend, bufferSize, BLOCKING_MODE);

}

/**
  * @brief  Receive sizeToSend amount of data with UART using BLOCKING mode.
  * @param  *buffer: pointer to a buffer from which data will be written to.
  * @param  sizeToReceive: size of data that will be received.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Comm_UART_Receive(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize) {

	return _UART_Receive(buffer, sizeToReceive, bufferSize, BLOCKING_MODE);

}

// UART Interrupt mode

/**
  * @brief  Send sizeToSend amount of data with UART using INTERRUPT mode.
  * @param  *buffer: pointer to a buffer from which data will be read to send.
  * @param  sizeToSend: size of data that will be sent.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Comm_UART_Send_IT(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, void (*callback)(void)) {

	if (callback){
		UART_Tx_Cb = callback;
	}

	return _UART_Send(buffer, sizeToSend, bufferSize, INTERRUPT_MODE);

}

/**
  * @brief  Receive sizeToSend amount of data with UART using INTERRUPT mode.
  * @param  *buffer: pointer to a buffer from which data will be written to.
  * @param  sizeToReceive: size of data that will be received.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Comm_UART_Receive_IT(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, void(*callback)(void)) {

	if (callback) {
		UART_Rx_Cb = callback;
	}

	return _UART_Receive(buffer, sizeToReceive, bufferSize, INTERRUPT_MODE);

}

// SPI Blocking mode

/**
  * @brief  Send sizeToSend amount of data with SPI using BLOCKING mode.
  * @param  *buffer: pointer to a buffer from which data will be read to send.
  * @param  sizeToSend: size of data that will be sent.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Comm_SPI_Send(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize) {

	return _SPI_Send(buffer, sizeToSend, bufferSize, BLOCKING_MODE);

}

/**
  * @brief  Receive sizeToSend amount of data with SPI using BLOCKING mode.
  * @param  *buffer: pointer to a buffer from which data will be written to.
  * @param  sizeToReceive: size of data that will be received.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Comm_SPI_Receive(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize) {

	return _SPI_Receive(buffer, sizeToReceive, bufferSize, BLOCKING_MODE);

}

// SPI Interrupt mode

/**
  * @brief  Send sizeToSend amount of data with SPI using INTERRUPT mode.
  * @param  *buffer: pointer to a buffer from which data will be read to send.
  * @param  sizeToSend: size of data that will be sent.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Comm_SPI_Send_IT(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, void (*callback)(void)) {

	if (callback) {
		SPI_Tx_Cb = callback;
	}

	return _SPI_Send(buffer, sizeToSend, bufferSize, INTERRUPT_MODE);

}

/**
  * @brief  Receive sizeToSend amount of data with SPI using INTERRUPT mode.
  * @param  *buffer: pointer to a buffer from which data will be written to.
  * @param  sizeToReceive: size of data that will be received.
  * @param  bufferSize: size of buffer.
  * @retval Comm status
  */
enum Comm_Status Comm_SPI_Receive_IT(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, void (*callback)(void)) {

	if (callback) {
		SPI_Rx_Cb = callback;
	}

	return _SPI_Receive(buffer, sizeToReceive, bufferSize, INTERRUPT_MODE);

}

// interrupt handlers
#ifndef UART_ISR_DEFINED_ELSEWHERE
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if (huart == comm_settings.comm_uart) {
		it_responses.uart_rx_it_flag = 1;
		if (UART_Rx_Cb) {
			UART_Rx_Cb();
		}
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

	if (huart == comm_settings.comm_uart) {
		it_responses.uart_tx_it_flag = 1;
		if (UART_Tx_Cb) {
			UART_Tx_Cb();
		}
	}

}
#endif

#ifndef SPI_ISR_DEFINED_ELSEWHERE
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {

	if (hspi == comm_settings.comm_spi) {
#ifdef _MASTER
		CS_HIGH
#else
		HS_LOW
#endif
		it_responses.spi_rx_it_flag = 1;
		if (SPI_Rx_Cb) {
			SPI_Rx_Cb();
		}
	}

}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {

	if (hspi == comm_settings.comm_spi) {
#ifdef _MASTER
		CS_HIGH
#else
		HS_LOW
#endif
		it_responses.spi_tx_it_flag = 1;
		if (SPI_Tx_Cb) {
			SPI_Tx_Cb();
		}
	}

}
#endif

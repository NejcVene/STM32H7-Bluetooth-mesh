/*
 * communication.h
 *
 *  Created on: Jun 19, 2024
 *      Author: Nejc
 */

#ifndef INC_COMMUNICATION_H_
#define INC_COMMUNICATION_H_

#include "main.h"

// NOTICE: UART Blocking and interrupt is untested. Same goes for SPI Blocking mode.

// set device as master or slave (cannot be both)
// #define MASTER
#define SLAVE

// if some ISR are defined elsewhere
#define UART_ISR_DEFINED_ELSEWHERE
// #define SPI_ISR_DEFINED_ELSEWHERE

#define COMM_TIMEOUT		10000				// timeout used in blocking mode
#ifdef MASTER
// edit these only if master device
#define COMM_CS_MASTER		GPIOE, GPIO_PIN_3	// port and pin used for output chip select (NSS) line in SPI (master)
#define COMM_HS_MASTER		GPIOH, GPIO_PIN_15	// port and pin used for handshake pin (input)
#else
// edit these only if slave device
#define COMM_CS_SLAVE		GPIOA, GPIO_PIN_5	// port and pin used for input chip select (NSS) line in SPI (slave)
#define COMM_HS_SLAVE		GPIOC, GPIO_PIN_0	// port and pin used for handshake pin (output)
#endif

// DO NOT EDIT BELOW THIS LINE

enum Comm_Status {
	COMM_OK = 0,
	COMM_ERROR,
	COMM_ERROR_NULL_POINTER
};

struct Comm_Settings {
	UART_HandleTypeDef *comm_uart;
	SPI_HandleTypeDef *comm_spi;
};

struct Comm_IT_Responses {
	volatile uint8_t uart_rx_it_flag,
					 uart_tx_it_flag,
					 spi_rx_it_flag,
					 spi_tx_it_flag;
};

// functions to be used by the user
// initialization function
enum Comm_Status Comm_Init(UART_HandleTypeDef *uart, SPI_HandleTypeDef *spi);

// UART Blocking mode
enum Comm_Status Comm_UART_Send(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize);
enum Comm_Status Comm_UART_Receive(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize);

// UART Interrupt mode
enum Comm_Status Comm_UART_Send_IT(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, void (*callback)(void));
enum Comm_Status Comm_UART_Receive_IT(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, void (*callback)(void));

// SPI Blocking mode
enum Comm_Status Comm_SPI_Send(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize);
enum Comm_Status Comm_SPI_Receive(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize);

// SPI Interrupt mode
enum Comm_Status Comm_SPI_Send_IT(uint8_t *buffer, uint16_t sizeToSend, uint16_t bufferSize, void (*callback)(void));
enum Comm_Status Comm_SPI_Receive_IT(uint8_t *buffer, uint16_t sizeToReceive, uint16_t bufferSize, void (*callback)(void));

extern struct Comm_IT_Responses it_responses;

#endif /* INC_COMMUNICATION_H_ */

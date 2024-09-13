/*
 * handshake_spi.h
 *
 *  Created on: Jul 19, 2024
 *      Author: Nejc
 */

#ifndef INC_HANDSHAKE_SPI_H_
#define INC_HANDSHAKE_SPI_H_

#include "communication.h"
#include <string.h>

#define USE_EXTERNAL_ERROR_HANDLER
// #define H7
#define WB55

#define NUMBER_OF_TYPES	6
#define ERROR_LINE		GPIOA, GPIO_PIN_0 // change

enum HANDSHAKE_Status {
	HANDSHAKE_OK = 0,
	HANDSHAKE_ERROR,
	HANDSHAKE_ERROR_START_BYTE,
	HANDSHAKE_ERROR_END_BYTE,
	HANDSHAKE_ERROR_CHECKSUM,
	HANDSHAKE_ERROR_UN_CMD // unknown command
};

enum HANDSHAKE_MSG_TYPE {
	OTHER = 0,
	CMD_SET_ACK,
	CMD_SET_UNACK,
	CMD_GET,
	RES_STATUS,
	RES_ERROR,
};

enum HANDSHAKE_Status Handshake_Send_Message(enum HANDSHAKE_MSG_TYPE type, uint8_t *payload, uint16_t payload_length);
enum HANDSHAKE_Status Handshake_Receive_Message(void (*callback)(void));
void Handshake_Process_Message(void);
void Handshake_Report_Error(enum HANDSHAKE_MSG_TYPE type, enum HANDSHAKE_Status payload);

#endif /* INC_HANDSHAKE_SPI_H_ */

/*
 * nv_protocol.h
 *
 *  Created on: Jul 24, 2024
 *      Author: Nejc
 */

#ifndef APPLICATION_CORE_LIBS_NV_PROTOCOL_H_
#define APPLICATION_CORE_LIBS_NV_PROTOCOL_H_

#include "communication_ex.h"

#define PAC_SIZE	7
#define PAC_MAX_PAYLOAD 256

enum NV_PROTOCOL_Status {
	HANDSHAKE_OK = 0,
	HANDSHAKE_ERROR,
	HANDSHAKE_ERROR_START_BYTE,
	HANDSHAKE_ERROR_END_BYTE,
	HANDSHAKE_ERROR_CHECKSUM,
	HANDSHAKE_ERROR_UN_CMD // unknown command
};

enum NV_PROTOCOL_MSG_TYPE {
	OTHER = 0,
	CMD_SET_ACK,
	CMD_SET_UNACK,
	CMD_GET,
	RES_STATUS,
	RES_ERROR,
};

enum NV_PROTOCOL_Status NV_Protocol_Send(enum NV_PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length);
enum NV_PROTOCOL_Status NV_Protocol_Receive(void (*callback)(void));

#endif /* APPLICATION_CORE_LIBS_NV_PROTOCOL_H_ */

/*
 * nv_protocol.c
 *
 *  Created on: Jul 24, 2024
 *      Author: Nejc
 */

#include "nv_protocol.h"

enum Process_Message_States {
	CHECK_START_BYTE = 0,
	CHECK_PACKET_TYPE,
	CHECK_LENGTH,
	CHECK_BYTE,
	CHECK_CHECKSUM,
	CHECK_END_BYTE,
	CHECK_OK
};

static uint8_t PAC_START_BYTE = 0x4E;
static uint8_t PAC_END_BYTE = 0x56;
static uint8_t start_byteRx, end_byteRx, cmd_type;
static uint8_t payloadRx[PAC_MAX_PAYLOAD];

static uint16_t checksumRx, lengthRx, payload_lengthRx;

static enum Process_Message_States proc_state = CHECK_START_BYTE;

static enum NV_PROTOCOL_MSG_TYPE cmd_typeRx;

static uint16_t NV_Protocol_Calc_Checksum(enum NV_PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length);
static int NV_Protocol_Check_Checksum(enum NV_PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length, uint16_t received_checksum);
static void NV_Protocol_Process_Message(void);
static inline void NV_Protocol_Convert_Message(uint8_t *payload, uint16_t payload_length);
static inline void NV_Protocol_Wait_For_TX(void);
static inline void NV_Protocol_Wait_For_RX(void);

static inline void NV_Protocol_Convert_Message(uint8_t *payload, uint16_t payload_lenght) {

	for (int i = 0; i<payload_lenght; i++) {
		if ((payload[i] >= 'a') && (payload[i] <= 'z')) {
			payload[i] += 'A' - 'a';
		}
	}

}

static uint16_t NV_Protocol_Calc_Checksum(enum NV_PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length) {

	uint16_t sum = type + payload_length + PAC_START_BYTE+ + PAC_END_BYTE;
	for (int i = 0; i<payload_length; i++) {
		sum += payload[i];
	}

	return sum;

}

static int NV_Protocol_Check_Checksum(enum NV_PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length, uint16_t received_checksum) {

	return NV_Protocol_Calc_Checksum(type, payload, payload_length) == received_checksum ? 1 : 0;

}

static inline void NV_Protocol_Wait_For_TX(void) {

	while (!it_responses.spi_tx_it_flag);

}

static inline void NV_Protocol_Wait_For_RX(void) {

	while (!it_responses.spi_rx_it_flag);

}

enum NV_PROTOCOL_Status NV_Protocol_Send(enum NV_PROTOCOL_MSG_TYPE type, uint8_t *payload, uint16_t payload_length) {

	uint16_t packet_length = payload_length + PAC_SIZE;
	NV_Protocol_Convert_Message(payload, payload_length);
	uint16_t checksum = NV_Protocol_Calc_Checksum(type, payload, payload_length);

	Comm_SPI_Send_IT(&PAC_START_BYTE, 1, 1, NULL);
	NV_Protocol_Wait_For_TX();

	Comm_SPI_Send_IT((uint8_t *) &type, 1, 1, NULL);
	NV_Protocol_Wait_For_TX();

	Comm_SPI_Send_IT((uint8_t *) &packet_length, 2, 2, NULL);
	NV_Protocol_Wait_For_TX();

	for (int i = 0; i<payload_length; i++) {
		Comm_SPI_Send_IT(&payload[i], 1, 1, NULL);
		NV_Protocol_Wait_For_TX();
	}

	Comm_SPI_Send_IT((uint8_t *) &checksum, 2, 2, NULL);
	NV_Protocol_Wait_For_TX();

	Comm_SPI_Send_IT(&PAC_END_BYTE, 1, 1, NULL);
	NV_Protocol_Wait_For_TX();

}

enum NV_PROTOCOL_Status NV_Protocol_Receive(void (*callback)(void)) {

	Comm_SPI_Receive_IT(&start_byteRx, 1, 1, callback);
	NV_Protocol_Wait_For_RX();

	Comm_SPI_Receive_IT(&cmd_type, 1, 1, callback);
	NV_Protocol_Wait_For_RX();

	Comm_SPI_Receive_IT((uint8_t *) &lengthRx, 2, 2, callback);
	NV_Protocol_Wait_For_RX();

	for (int i = 0; i<payload_lengthRx; i++) {
		Comm_SPI_Receive_IT(payloadRx, 1, 1, callback);
		NV_Protocol_Wait_For_RX();
	}

	Comm_SPI_Receive_IT((uint8_t *) &checksumRx, 2, 2, callback);
	NV_Protocol_Wait_For_RX();

	Comm_SPI_Receive_IT(&end_byteRx, 1, 1, callback);
	NV_Protocol_Wait_For_RX();

	callback(); // can be changed to its own function and not tied to the FSM states.

	return HANDSHAKE_OK;

}

static void NV_Protocol_Process_Message(void) {

	static int counter = 0;
	int p = 0;

	switch (proc_state) {
		case CHECK_START_BYTE:
			// check for correct start byte of packet
			if (PAC_START_BYTE != start_byteRx) {
				proc_state = CHECK_PACKET_TYPE;
			} else {
				// raise error
			}
			break;
		case CHECK_PACKET_TYPE:
			// this could be filled out at a later date
			// to check for correct packet types
			cmd_typeRx = (enum NV_PROTOCOL_MSG_TYPE) cmd_type;
			proc_state = CHECK_LENGTH;
			break;
		case CHECK_LENGTH:
			// this could be filled out at a later date
			// to check for strange length values
			payload_lengthRx = lengthRx - PAC_SIZE;
			proc_state = CHECK_BYTE;
			break;
		case CHECK_BYTE:
			CommandString[counter++] = payloadRx[0];
			if (counter == payload_lengthRx) {
				counter = 0;
				proc_state = CHECK_CHECKSUM;
			}
			break;
		case CHECK_CHECKSUM:
			// check for correct checksum
			if (Handshake_Check_Checksum(cmd_typeRx, CommandString, payload_lengthRx, checksumRx)) {
				status = CHECK_END_BYTE;
			} else {
				// raise error
			}
			break;
		case CHECK_END_BYTE:
			// check for correct end byte of packet
			if (PAC_END_BYTE == end_byteRx) {
				// received message is correct
				proc_state = CHECK_OK;
			} else {
				// raise error
			}
			break;
		case CHECK_OK:
			proc_state = CHECK_START_BYTE;
#ifdef _H7
			p = 1; // remove later
#else
			indexReceiveChar = payload_lengthRx;
			CommandString[indexReceiveChar] = 0; // make last char NULL for string comp
			TRACE_I(TF_SERIAL_PRINTS, "received %s\n", CommandString);
			// set task for serial interface process, which will execute received command
			UTIL_SEQ_SetTask(1 << CFG_TASK_MESH_SPI_REQ_ID, CFG_SCH_PRIO_0);
#endif
			break;
		default:
			break;
	}

}

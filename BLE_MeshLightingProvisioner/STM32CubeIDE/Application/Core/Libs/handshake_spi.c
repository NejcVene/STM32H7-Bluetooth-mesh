/*
 * handshake_SPI.c
 *
 *  Created on: Jul 19, 2024
 *      Author: Nejc
 */

#include "handshake_spi.h"

#if defined(H7) && !defined(WB55)
#define _H7
#elif !defined(H7) && defined(WB55)
#define _WB55
#include "stm32_seq.h"
#include "serial_if.h"
#include "mesh_cfg.h"
#else
#error Conflicting handshake setting
#endif

#ifdef _H7
uint8_t CommandString[256U];
#endif

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

static uint8_t start_byteRx, end_byteRx;
static enum HANDSHAKE_MSG_TYPE cmd_typeRx;
static enum HANDSHAKE_Status status;
static uint16_t checksumRx, lengthRx, payload_lengthRx;
static uint8_t payloadRx[100];

static uint8_t cmd_type; // remove later

static volatile int error_found;

static enum Process_Message_States proc_state = CHECK_START_BYTE;

static uint16_t Handshake_Calc_Checksum(enum HANDSHAKE_MSG_TYPE type, uint8_t *payload, uint16_t payload_length);
static int Handshake_Check_Checksum(enum HANDSHAKE_MSG_TYPE type, uint8_t *payload, uint16_t payload_length, uint16_t received_checksum);
static uint8_t Handshake_To_Upper_Case(uint8_t character);
static inline int Handshake_Check_Error_Line(void);
static inline void Handshake_Wait_For_TX(void);
static inline void Handshake_Wait_For_RX(void);
static inline void Handshake_Convert_Message(uint8_t *payload, uint16_t payload_length);

static inline void Handshake_Wait_For_TX(void) {

	while (!it_responses.spi_tx_it_flag);

}

static inline void Handshake_Wait_For_RX(void) {

	while (!it_responses.spi_rx_it_flag);

}

static uint8_t Handshake_To_Upper_Case(uint8_t character) {

	return ((character >= 'a') && (character <= 'z')) ? character += 'A' - 'a' : character;

}

static inline void Handshake_Convert_Message(uint8_t *payload, uint16_t payload_lenght) {

	for (int i = 0; i<payload_lenght; i++) {
		payload[i] = Handshake_To_Upper_Case(payload[i]);
	}

}

static inline int Handshake_Check_Error_Line(void) {

	return HAL_GPIO_ReadPin(ERROR_LINE) ? 1 : 0;

}

void Handshake_Report_Error(enum HANDSHAKE_MSG_TYPE type, enum HANDSHAKE_Status payload) {

	HAL_GPIO_WritePin(ERROR_LINE, GPIO_PIN_SET);
	Handshake_Send_Message(type, (uint8_t *) &payload, 1);

}

enum HANDSHAKE_Status Handshake_Send_Message(enum HANDSHAKE_MSG_TYPE type, uint8_t *payload, uint16_t payload_length) {

	uint16_t packet_length = payload_length + 7;
	Handshake_Convert_Message(payload, payload_length);
	uint16_t checksum = Handshake_Calc_Checksum(type, payload, payload_length);

	if (Comm_SPI_Send_IT(&PAC_START_BYTE, 1, 1, NULL)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_TX();
	/*
	HAL_Delay(10000);
	if (Handshake_Check_Error_Line()) {
		return HANDSHAKE_ERROR_START_BYTE;
	}
	*/

	// send packet type
	if (Comm_SPI_Send_IT((uint8_t *) &type, 1, 1, NULL)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_TX();

	// send packet length
	if (Comm_SPI_Send_IT((uint8_t *) &packet_length, 2, 2, NULL)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_TX();

	// send payload
	for (int i = 0; i<payload_length; i++) {
		if (Comm_SPI_Send_IT(&payload[i], 1, 1, NULL)) {
			return HANDSHAKE_ERROR;
		}
		Handshake_Wait_For_TX();
	}

	// send checksum
	if (Comm_SPI_Send_IT((uint8_t *) &checksum, 2, 2, NULL)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_TX();

	// send end byte
	if (Comm_SPI_Send_IT(&PAC_END_BYTE, 1, 1, NULL)) {
		return HANDSHAKE_ERROR;
	}

	return HANDSHAKE_OK;

}

enum HANDSHAKE_Status Handshake_Receive_Message(void (*callback)(void)) {

	if (Comm_SPI_Receive_IT(&start_byteRx, 1, 1, callback)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_RX();
	if (status) return status;

	if (Comm_SPI_Receive_IT(&cmd_type, 1, 1, callback)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_RX();

	if (Comm_SPI_Receive_IT((uint8_t *) &lengthRx, 2, 2, callback)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_RX();

	for (int i = 0; i<payload_lengthRx; i++) {
		if (Comm_SPI_Receive_IT(payloadRx, 1, 1, callback)) {
			return HANDSHAKE_ERROR;
		}
		Handshake_Wait_For_RX();
	}

	if (Comm_SPI_Receive_IT((uint8_t *) &checksumRx, 2, 2, callback)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_RX();

	if (Comm_SPI_Receive_IT(&end_byteRx, 1, 1, callback)) {
		return HANDSHAKE_ERROR;
	}

	Handshake_Wait_For_RX();

	callback(); // can be changed to its own function and not tied to the FSM states.

	return HANDSHAKE_OK;

}

static uint16_t Handshake_Calc_Checksum(enum HANDSHAKE_MSG_TYPE type, uint8_t *payload, uint16_t payload_length) {

	uint16_t sum = type + payload_length + PAC_START_BYTE+ + PAC_END_BYTE;
	for (int i = 0; i<payload_length; i++) {
		sum += payload[i];
	}

	return sum;

}

static int Handshake_Check_Checksum(enum HANDSHAKE_MSG_TYPE type, uint8_t *payload, uint16_t payload_length, uint16_t received_checksum) {

	return Handshake_Calc_Checksum(type, payload, payload_length) == received_checksum ? 1 : 0;

}

void Handshake_Process_Message(void) {

	static int counter = 0;
	int p = 0;

	switch (proc_state) {
		case CHECK_START_BYTE:
			// check for correct start byte of packet
			if (PAC_START_BYTE != start_byteRx) {
				status = HANDSHAKE_ERROR_START_BYTE;
			} else {
				proc_state = CHECK_PACKET_TYPE;
			}
			break;
		case CHECK_PACKET_TYPE:
			// this could be filled out at a later date
			// to check for correct packet types
			cmd_typeRx = (enum HANDSHAKE_MSG_TYPE) cmd_type;
			proc_state = CHECK_LENGTH;
			break;
		case CHECK_LENGTH:
			// this could be filled out at a later date
			// to check for strange length values
			payload_lengthRx = lengthRx - 7;
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
				status = HANDSHAKE_ERROR_CHECKSUM;
			} else {
				// proc_state = CHECK_ERROR;
			}
			break;
		case CHECK_END_BYTE:
			// check for correct end byte of packet
			if (PAC_END_BYTE != end_byteRx) {
				status = HANDSHAKE_ERROR_END_BYTE;
			} else {
				// received message is correct
				proc_state = CHECK_OK;
			}
			break;
		case CHECK_OK:
			proc_state = CHECK_START_BYTE;
			// no errors found
			// TODO
			// - Change WB55 transmission from USART to SPI
			// - Implement a counter for to many packet errors
			//	 what to do in this case is not yet known, maybe force a reset
			// - WB55 will only receive commands, so not much has to be done for reception of data
			// - H7 will check its reception of payload and act accordingly to what it sent and received
			//   ex. if it sent an set ack it will look for an ack response in the payload and then
			//   change GUI status
			// - If H7 gets unknown command as response then it will try to send the command again
			// - If H7 gets packet error then it will try to send again
			// - If H7 gets packet error to many times it means something is wrong whit its SPI sending
			//   same goes for WB55
			// - TEST (master to slave seems to be working at tested speed of 1s)
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

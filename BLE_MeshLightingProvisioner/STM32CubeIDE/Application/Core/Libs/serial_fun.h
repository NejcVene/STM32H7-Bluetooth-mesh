/*
 * serial_fun.h
 *
 *  Created on: Oct 14, 2024
 *      Author: Nejc
 */

#ifndef APPLICATION_CORE_LIBS_SERIAL_FUN_H_
#define APPLICATION_CORE_LIBS_SERIAL_FUN_H_

#include <stdint.h>

#define FUN_INDENTIFIER_LEN								7
#define FUN_INDENTIFIER									((char *) "BLEMESH")

#define SF_DEFAULT_GROUP_ADDR							0xC000
#define SF_DEFAULT_PUBLISH_ADDR							0xC000
#define SF_DEFAULT_NETKEY_INDEX							0x0000
#define SF_DEFAULT_APPKEY_INDEX							0x0000
#define SF_DEFAULT_CREDENTIAL_FLAG						0x00
#define SF_DEFAULT_PUBLISH_TTL							0x08
#define SF_DEFAULT_PUBLISH_PERIOD						0x00
#define SF_DEFAULT_PUBLISH_RETRANSMIT_COUNT				0x00
#define SF_DEFAULT_PUBLISH_RETRANSMIT_INTERVAL_STEPS	0x00

void SF_Process(char *receiveBuffer, uint16_t receiveSize);

#endif /* APPLICATION_CORE_LIBS_SERIAL_FUN_H_ */

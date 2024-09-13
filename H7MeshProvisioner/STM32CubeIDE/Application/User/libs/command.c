/*
 * command.c
 *
 *  Created on: Aug 14, 2024
 *      Author: Nejc
 */

#include "command.h"

CMD_MeshCommand_t defineRootNetworkNode = {
		.command = "atep root",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t scanForUnprovisionedNetworkDevices = {
		.command = "atep scan",
		.commandType = PRO_MSG_TYPE_ACK
};

CMD_MeshCommand_t provisionNetworkDevice = { // currently hard-coded
		.command = "atep prvn-%d",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t scanForUnprovisionedNetworkDevicesOutOfRangePvrn = {
		.command = "atep ndscan",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t provisionNetworkDeviceOutOfRangePvrn = {
		.command = "atep ndprvn-%d",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t genericOnOffSetAck = { // currently hard-coded
		.command = "atcl c000 8202 01 00",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t genericOnOffSetAckOff = { // currently hard-coded
		.command = "atcl c000 8202 00 00", // c000 for group address
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t genericOnOffGet = {
		.command = "atcl 0002 8201",
		.commandType = PRO_MSG_TYPE_UNACK
};

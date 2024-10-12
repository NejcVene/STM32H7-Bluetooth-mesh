/*
 * command.c
 *
 *  Created on: Aug 14, 2024
 *      Author: Nejc
 */

#include "command.h"

CMD_MeshCommand_t defineRootNetworkNode = {
		.command = "ATEP ROOT",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t scanForUnprovisionedNetworkDevices = {
		.command = "ATEP SCAN",
		.commandType = PRO_MSG_TYPE_ACK
};

CMD_MeshCommand_t provisionNetworkDevice = {
		.command = "ATEP PRVN-%d",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t scanForUnprovisionedNetworkDevicesOutOfRangePvrn = {
		.command = "ATEP NDSCAN",
		.commandType = PRO_MSG_TYPE_ACK
};

CMD_MeshCommand_t provisionNetworkDeviceOutOfRangePvrn = {
		.command = "ATEP NDPRVN-%d",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t genericOnOffSetAck = { // currently hard-coded
		.command = "ATCL c000 8202 01 00",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t genericOnOffSetAckOff = { // currently hard-coded
		.command = "ATCL c000 8202 00 00", // c000 for group address
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t genericOnOffGet = {
		.command = "ATCL 0002 8201",
		.commandType = PRO_MSG_TYPE_UNACK
};

CMD_MeshCommand_t unprovisionEmbeddedProv = {
		.command = "BLEMesh_Unprovision",
		.commandType = PRO_MSG_TYPE_OTHER
};

CMD_MeshCommand_t isEmbeddedProvProvisioned = {
		.command = "BLEMesh_IsUnprovisioned",
		.commandType = PRO_MSG_TYPE_OTHER
};

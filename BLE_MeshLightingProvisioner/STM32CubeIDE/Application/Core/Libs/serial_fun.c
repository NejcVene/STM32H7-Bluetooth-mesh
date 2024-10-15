/*
 * serial_fun.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Nejc
 */

#include "serial_fun.h"
#include "appli_config_client.h"
#include "mesh_cfg.h"
#include <string.h>

void SF_UnprovisionEmbedded(char *resultBuffer);
void SF_IsEmbeddedProvisioned(char *resultBuffer);
void SF_SubscriptionAdd(char *receiveBuffer, char *resultBuffer);
void SF_PublicationSet(char *receiveBuffer, char *resultBuffer);


void SF_Process(char *receiveBuffer, uint16_t receiveSize, char *resultBuffer) {

	if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "Unprovision", strlen("Unprovision"))) {
		SF_UnprovisionEmbedded(resultBuffer);
	} else if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "IsUnprovisioned", strlen("IsUnprovisioned"))) {
		SF_IsEmbeddedProvisioned(resultBuffer);
	} else if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "SubsAdd", strlen("SubsAdd"))) {
		SF_SubscriptionAdd(receiveBuffer, resultBuffer);
	} else if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "ModelSet", strlen("ModelSet"))) {
		SF_PublicationSet(receiveBuffer, resultBuffer);
	} else {
		strncat(resultBuffer, receiveBuffer, receiveSize);
		strcat(resultBuffer, ": NONE");
	}

}

void SF_UnprovisionEmbedded(char *resultBuffer) {

	char tmp[2];
	strcat(resultBuffer, "Unprovision: ");
	tmp[0] = BLEMesh_Unprovision() + '0';
	tmp[1] = '\n';
	strcat(resultBuffer, tmp);

}

void SF_IsEmbeddedProvisioned(char *resultBuffer) {

	char tmp[2];
	strcat(resultBuffer, "IsUnprovisioned: ");
	tmp[0] = BLEMesh_IsUnprovisioned() + '0';
	tmp[1] = '\n';
	strcat(resultBuffer, tmp);

}

void SF_SubscriptionAdd(char *receiveBuffer, char *resultBuffer) {

	char tmp[2];
	int elementAddress, address, modelIndentifier;
	sscanf(receiveBuffer, "%*s %d %d %d", &elementAddress, &address, &modelIndentifier);
	strcat(resultBuffer, "SubsAdd: ");
	tmp[0] = ConfigClient_SubscriptionAdd(elementAddress, address, modelIndentifier) + '0';
	tmp[1] = '\n';
	strcat(resultBuffer, tmp);

}

void SF_PublicationSet(char *receiveBuffer, char *resultBuffer) {

	char tmp[2];
	int elementAddress, address, modelIndentifier;
	sscanf(receiveBuffer, "%*s %d %d", &elementAddress, &modelIndentifier);
	strcat(resultBuffer, "ModelSet: ");
	tmp[0] = ConfigClient_PublicationSet(
				elementAddress,
				SF_DEFAULT_PUBLISH_ADDR,
				SF_DEFAULT_APPKEY_INDEX,
				SF_DEFAULT_CREDENTIAL_FLAG,
				SF_DEFAULT_PUBLISH_TTL,
				SF_DEFAULT_PUBLISH_PERIOD,
				SF_DEFAULT_PUBLISH_RETRANSMIT_COUNT,
				SF_DEFAULT_PUBLISH_RETRANSMIT_INTERVAL_STEPS,
				modelIndentifier) + '0';
	tmp[1] = '\n';
	strcat(resultBuffer, tmp);

}


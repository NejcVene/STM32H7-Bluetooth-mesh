/*
 * serial_fun.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Nejc
 */

#include "serial_fun.h"
#include "communication_ex.h"
#include "appli_config_client.h"
#include "appli_config_client_ex.h"
#include "mesh_cfg.h"
#include <string.h>

void SF_UnprovisionEmbedded(char *resultBuffer);
void SF_IsEmbeddedProvisioned(char *resultBuffer);
void SF_SubscriptionAdd(char *receiveBuffer, char *resultBuffer);
void SF_PublicationSet(char *receiveBuffer, char *resultBuffer);
void SF_PublishSubscribe(char *receiveBuffer, char *resultBuffer);
static inline int SF_WaitForProcess();
MOBLE_RESULT _SubscriptionAdd(uint16_t elementAddress, uint16_t address, uint32_t modelIndentifier);
MOBLE_RESULT _PublicationSet(uint16_t elementAddress, uint16_t publisAddress, uint32_t modelIndentifier);

extern Queue *eventQueue;

static inline int SF_WaitForProcess() {

	return Appli_SFGetOpStatus();

}

MOBLE_RESULT _SubscriptionAdd(uint16_t elementAddress, uint16_t address, uint32_t modelIndentifier) {

	MOBLE_RESULT status;

	status = ConfigClient_SubscriptionAdd(elementAddress, address, modelIndentifier);

	return status;

}

MOBLE_RESULT _PublicationSet(uint16_t elementAddress, uint16_t publisAddress, uint32_t modelIndentifier) {

	MOBLE_RESULT status;

	status = ConfigClient_PublicationSet(
			elementAddress,
			publisAddress,
			SF_DEFAULT_APPKEY_INDEX,
			SF_DEFAULT_CREDENTIAL_FLAG,
			SF_DEFAULT_PUBLISH_TTL,
			SF_DEFAULT_PUBLISH_PERIOD,
			SF_DEFAULT_PUBLISH_RETRANSMIT_COUNT,
			SF_DEFAULT_PUBLISH_RETRANSMIT_INTERVAL_STEPS,
			modelIndentifier);

	return status;

}

void SF_Process(char *receiveBuffer, uint16_t receiveSize) {

	char resultBuffer[PAC_MAX_PAYLOAD] = {0};
	if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "Unprovision", strlen("Unprovision"))) {
		SF_UnprovisionEmbedded(resultBuffer);
	} else if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "IsUnprovisioned", strlen("IsUnprovisioned"))) {
		SF_IsEmbeddedProvisioned(resultBuffer);
	} else if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "SubsAdd", strlen("SubsAdd"))) {
		SF_SubscriptionAdd(receiveBuffer, resultBuffer);
	} else if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "ModelSet", strlen("ModelSet"))) {
		SF_PublicationSet(receiveBuffer, resultBuffer);
	} else if (!strncmp(receiveBuffer + FUN_INDENTIFIER_LEN + 1, "PUBSUB", strlen("PUBSUB"))) {
		SF_PublishSubscribe(receiveBuffer, resultBuffer);
	} else {
		strncat(resultBuffer, receiveBuffer, receiveSize);
		strcat(resultBuffer, ": NONE");
	}

}

void SF_UnprovisionEmbedded(char *resultBuffer) {

	char tmp[2] = {'0', '\0'};
	strcat(resultBuffer, "Unprovision: ");
	tmp[0] = BLEMesh_Unprovision() + '0';
	strcat(resultBuffer, tmp);

}

void SF_IsEmbeddedProvisioned(char *resultBuffer) {

	char tmp[2] = {'0', '\0'};
	strcat(resultBuffer, "IsUnprovisioned: ");
	tmp[0] = BLEMesh_IsUnprovisioned() + '0';
	strcat(resultBuffer, tmp);

}

void SF_SubscriptionAdd(char *receiveBuffer, char *resultBuffer) {

	int elementAddress, address, modelIndentifier;
	MOBLE_RESULT status;

	sscanf(receiveBuffer, "%*s %d %d %d", &elementAddress, &address, &modelIndentifier);
	status = _SubscriptionAdd(elementAddress, address, modelIndentifier);
	sprintf(resultBuffer, "BLEMesh_SubsAdd: %d", status);

}

void SF_PublicationSet(char *receiveBuffer, char *resultBuffer) {

	int elementAddress, address, modelIndentifier;
	MOBLE_RESULT status;

	sscanf(receiveBuffer, "%*s %d %d %d", &elementAddress, &address, &modelIndentifier);
	status = _PublicationSet(elementAddress, address, modelIndentifier);
	sprintf(resultBuffer, "BLEMesh_ModelSet: %d", status);

}

void SF_PublishSubscribe(char *receiveBuffer, char *resultBuffer) {

	static int elementAddress;
	static int address;
	static int modelIndentifier;
	static MOBLE_RESULT status;

	switch (Appli_SFGetOpStatus()) {
		case SF_CALLBACK_IDLE:
			Appli_SFSetStatus(SF_CALLBACK_IN_PROGRESS);
			Appli_SFSetAccess(SF_ENABLE_ACCESS);
			sscanf(receiveBuffer, "%*s %d %d %d", &elementAddress, &address, &modelIndentifier);
			if ((status = _SubscriptionAdd(elementAddress, address, modelIndentifier)) == MOBLE_RESULT_SUCCESS) {
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			}
			break;
		case SF_CALLBACK_SUBSCRIBE_OK:
			Appli_SFSetStatus(SF_CALLBACK_IN_PROGRESS);
			if ((status = _PublicationSet(elementAddress, address, modelIndentifier)) == MOBLE_RESULT_SUCCESS) {
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			}
			break;
		case SF_CALLBACK_IN_PROGRESS:
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			break;
		case SF_CALLBACK_PUBLISH_OK:
			Appli_SFSetStatus(SF_CALLBACK_IDLE);
			Appli_SFSetAccess(SF_DISABLE_ACCESS); // BLEMesh_PubSub %d %d %d
			sprintf(resultBuffer, "BLEMesh_PubSub: %d", status);
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, resultBuffer, strlen(resultBuffer) + 1);
			break;
		default:
			break;
	}

}


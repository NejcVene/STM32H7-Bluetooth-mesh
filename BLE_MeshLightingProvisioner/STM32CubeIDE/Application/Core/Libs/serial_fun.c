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

typedef struct {
	uint8_t numOfTx;
	uint32_t messageSentTime;
	SF_OPERATION_STATUS currMsg;
} SF_MessageInfo;

void SF_UnprovisionEmbedded(char *resultBuffer);
void SF_IsEmbeddedProvisioned(char *resultBuffer);
void SF_PublishSubscribe(char *receiveBuffer, char *resultBuffer);
void SF_SubscriptionRemove(char *receiveBuffer, char *resultBuffer);
SF_OPERATION_STATUS SF_CheckTimeout(SF_MessageInfo *msgInfo);
MOBLE_RESULT _SubscriptionAdd(uint16_t elementAddress, uint16_t address, uint32_t modelIndentifier);
MOBLE_RESULT _PublicationSet(uint16_t elementAddress, uint16_t publisAddress, uint32_t modelIndentifier);

extern Queue *eventQueue;

SF_OPERATION_STATUS SF_CheckTimeout(SF_MessageInfo *msgInfo) {

	SF_OPERATION_STATUS status = SF_CALLBACK_IN_PROGRESS;
	uint32_t currentTime = HAL_GetTick();

	if ((currentTime - msgInfo->messageSentTime) >= CONFIGCLIENT_RESPONSE_TIMEOUT) {
		msgInfo->numOfTx++;
		// here we could check if the number of transmissions exceeds a certain value
		// to prevent a infinite loop, but for now we are not doing that
		status = msgInfo->currMsg;
	}

	return status;

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

void SF_PublishSubscribe(char *receiveBuffer, char *resultBuffer) {

	static int elementAddress;
	static int address;
	static int modelIndentifier;
	static SF_MessageInfo msgInfo;
	static MOBLE_RESULT status;

	switch (Appli_SFGetOpStatus()) {
		case SF_CALLBACK_IDLE:
			Appli_SFSetStatus(SF_SUBSCRIBE_ADD);
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			break;
		case SF_SUBSCRIBE_ADD:
			Appli_SFSetStatus(SF_CALLBACK_IN_PROGRESS);
			Appli_SFSetAccess(SF_ENABLE_ACCESS);
			sscanf(receiveBuffer, "%*s %d %d %d", &elementAddress, &address, &modelIndentifier);
			if ((status = _SubscriptionAdd(elementAddress, address, modelIndentifier)) == MOBLE_RESULT_SUCCESS) {
				msgInfo.numOfTx = 0;
				msgInfo.messageSentTime = HAL_GetTick();
				msgInfo.currMsg = SF_SUBSCRIBE_ADD;
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			}
			break;
		case SF_PUBLICATION_SET:
			Appli_SFSetStatus(SF_CALLBACK_IN_PROGRESS);
			if ((status = _PublicationSet(elementAddress, address, modelIndentifier)) == MOBLE_RESULT_SUCCESS) {
				msgInfo.numOfTx = 0;
				msgInfo.messageSentTime = HAL_GetTick();
				msgInfo.currMsg = SF_PUBLICATION_SET;
				FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			}
			break;
		case SF_CALLBACK_SUBSCRIBE_OK:
			Appli_SFSetStatus(SF_PUBLICATION_SET);
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			break;
		case SF_CALLBACK_IN_PROGRESS:
			Appli_SFSetStatus(SF_CheckTimeout(&msgInfo));
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			break;
		case SF_CALLBACK_PUBLISH_OK:
			Appli_SFSetStatus(SF_CALLBACK_IDLE);
			Appli_SFSetAccess(SF_DISABLE_ACCESS);
			sprintf(resultBuffer, "BLEMesh_PubSub: %d", status);
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, resultBuffer, strlen(resultBuffer) + 1);
			break;
		default:
			break;
	}

}

void SF_SubscriptionRemove(char *receiveBuffer, char *resultBuffer) {

	// this will pose a bit of a problem because, at the time of
	// writing this, the mesh API for the embedded provisioner does
	// not support said feature - for the time being this will be
	// spoofed by H7 in the gui

}


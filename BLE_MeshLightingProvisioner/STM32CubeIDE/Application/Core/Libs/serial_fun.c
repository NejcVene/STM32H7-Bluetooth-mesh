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

typedef struct __attribute__((packed)) {
	uint16_t val1;
	uint8_t val2;
	double val3;
	int val4;
} SF_TestProtocol_t;

void SF_UnprovisionEmbedded(uint8_t *receiveBuffer, uint8_t *resultBuffer);
void SF_IsEmbeddedProvisioned(uint8_t *receiveBuffer, uint8_t *resultBuffer);
void SF_PublishSubscribe(uint8_t *receiveBuffer, uint8_t *resultBuffer);
void SF_SubscriptionRemove(uint8_t *receiveBuffer, uint8_t *resultBuffer);
void SF_GetLibVersion(uint8_t *receiveBuffer, uint8_t *resultBuffer);
void SF_TestProtocol(uint8_t *receiveBuffer, uint8_t *resultBuffer);
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

void SF_Process(uint8_t *receiveBuffer, uint16_t receiveSize) {

	int flag;
	uint8_t resultBuffer[PAC_MAX_PAYLOAD] = {0};

	if (!strncmp((char *) receiveBuffer + FUN_INDENTIFIER_LEN + 1, "UNPROVISION", strlen("UNPROVISION"))) {
		SF_UnprovisionEmbedded(receiveBuffer, resultBuffer);
	} else if (!strncmp((char *) receiveBuffer + FUN_INDENTIFIER_LEN + 1, "ISUNPROVISIONED", strlen("ISUNPROVISIONED"))) {
		SF_IsEmbeddedProvisioned(receiveBuffer, resultBuffer);
	} else if (!strncmp((char *) receiveBuffer + FUN_INDENTIFIER_LEN + 1, "PUBSUB", strlen("PUBSUB"))) {
		sscanf((char *) receiveBuffer, "%*s %d", &flag);
		if (flag) {
			SF_PublishSubscribe(receiveBuffer, resultBuffer);
		} else {
			SF_SubscriptionRemove(receiveBuffer, resultBuffer);
		}
	} else if (!strncmp((char *) receiveBuffer + FUN_INDENTIFIER_LEN + 1, "LIBVER", strlen("LIBVER"))) {
		SF_GetLibVersion(receiveBuffer, resultBuffer);
	} else if (!strncmp((char *) receiveBuffer + FUN_INDENTIFIER_LEN + 1, "PROTEST", strlen("PROTEST"))) {
		SF_TestProtocol(receiveBuffer, resultBuffer);
	} else {
		strncat((char *) resultBuffer, (char *) receiveBuffer, receiveSize);
		strcat((char *) resultBuffer, ": NONE");
	}

}

void SF_UnprovisionEmbedded(uint8_t *receiveBuffer, uint8_t *resultBuffer) {

	uint16_t convertedStatus = 0;

	convertedStatus = (uint16_t) BLEMesh_Unprovision();
	FSM_EncodePayload(resultBuffer, "BLEMesh_Unprovision", (void *) &convertedStatus, sizeof(uint16_t), PRO_DATATYPE_U16T);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, resultBuffer, PAC_MAX_PAYLOAD);

}

void SF_IsEmbeddedProvisioned(uint8_t *receiveBuffer, uint8_t *resultBuffer) {

	uint16_t convertedStatus = 0;

	convertedStatus = (uint16_t) BLEMesh_IsUnprovisioned();
	FSM_EncodePayload(resultBuffer, "BLEMesh_IsUnprovisioned", (void *) &convertedStatus, sizeof(uint16_t), PRO_DATATYPE_U16T);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, resultBuffer, PAC_MAX_PAYLOAD);

}

void SF_PublishSubscribe(uint8_t *receiveBuffer, uint8_t *resultBuffer) {

	int flag;
	static int elementAddress;
	static int address;
	static int modelIndentifier;
	static SF_MessageInfo msgInfo;
	static MOBLE_RESULT status;
	uint16_t convertedStatus = 0;

	switch (Appli_SFGetOpStatus()) {
		case SF_CALLBACK_IDLE:
			Appli_SFSetStatus(SF_SUBSCRIBE_ADD);
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_LOOP, receiveBuffer, sizeof(receiveBuffer));
			break;
		case SF_SUBSCRIBE_ADD:
			Appli_SFSetStatus(SF_CALLBACK_IN_PROGRESS);
			Appli_SFSetAccess(SF_ENABLE_ACCESS);
			sscanf((char *) receiveBuffer, "%*s %d %d %d %d", &flag, &elementAddress, &address, &modelIndentifier);
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
			convertedStatus = (uint16_t) status;
			FSM_EncodePayload(resultBuffer, "BLEMesh_PubSub", (void *) &convertedStatus, sizeof(uint16_t), PRO_DATATYPE_U16T);
			FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, resultBuffer, PAC_MAX_PAYLOAD);
			break;
		default:
			break;
	}

}

void SF_SubscriptionRemove(uint8_t *receiveBuffer, uint8_t *resultBuffer) {

	// this will pose a bit of a problem because, at the time of
	// writing this, the mesh API for the embedded provisioner does
	// not support said feature - for the time being this will be
	// spoofed by H7 in the gui

	MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
	uint16_t convertedStatus = 0;

	convertedStatus = (uint16_t) status;
	FSM_EncodePayload(resultBuffer, "BLEMesh_PubSub", (void *) &convertedStatus, sizeof(uint16_t), PRO_DATATYPE_U16T);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, resultBuffer, PAC_MAX_PAYLOAD);

}

void SF_GetLibVersion(uint8_t *receiveBuffer, uint8_t *resultBuffer) {

	uint8_t buffer[PAC_MAX_PAYLOAD] = {0};
	char *libVersion = BLEMesh_GetLibraryVersion();
	char *libSubVersion = BLEMesh_GetLibrarySubVersion();

	sprintf((char *) buffer, "%s;%s", libVersion, libSubVersion);
	FSM_EncodePayload(resultBuffer, "BLEMesh_LibVer", buffer, 0, PRO_DATATYPE_STRING);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, resultBuffer, PAC_MAX_PAYLOAD);

}

void SF_TestProtocol(uint8_t *receiveBuffer, uint8_t *resultBuffer) {

	SF_TestProtocol_t test = {.val1 = 10,
							.val2 = 128,
							.val3 = 24.5,
							.val4 = 420};

	FSM_EncodePayload(resultBuffer,
					"BLEMesh_Protest",
					(void *) &test,
					sizeof(SF_TestProtocol_t),
					PRO_DATATYPE_STRUCT_TEST);
	FSM_RegisterEvent(eventQueue, MAIN_FSM_EVENT_AKC, resultBuffer, PAC_MAX_PAYLOAD);

}


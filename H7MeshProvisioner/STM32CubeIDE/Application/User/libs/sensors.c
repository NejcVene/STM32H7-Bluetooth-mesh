/*
 * sensors.c
 *
 *  Created on: Nov 19, 2024
 *      Author: Nejc
 */

#include "sensors.h"
#include "cmsis_os2.h"
#include "freertos_os2.h"
#include <string.h>

void SN_RegisterAPC1Sensor(Node_SensorInfo_t *sensor);
Node_ButtonInterface_t *SN_CreateButtons(const char **buttonNames, uint8_t buttonCount);
void SN_AllocateAndCopy(Sensor_MeasuredData_t *mea, void *buffer, size_t dataSize);

Node_Sensor_t SN_RegisterSensor(SN_SensorDescriptorGet_t *descriptors) {

	Node_Sensor_t sensors;

	sensors.numOfSensors = descriptors->descCount;
	if ((sensors.sensorArray = (Node_SensorInfo_t *) pvPortMalloc(descriptors->descCount * sizeof(Node_SensorInfo_t)))) {
		for (int i = 0; i<descriptors->descCount; i++) {
			switch ((SENSOR_PID) descriptors->descriptors[i]) {
				case PRESENT_AMBIENT_TEMPERATURE_PID:
					SN_RegisterAPC1Sensor(&sensors.sensorArray[i]);
					break;
				default:
					break;
			}
		}
	}

	return sensors;

}

Sensor_MeasuredData_t *SN_GetMeasuredSensorData(SENSOR_PID inputPID, void *buffer) {

	Sensor_MeasuredData_t *mea = NULL;

	if (buffer && (mea = (Sensor_MeasuredData_t *) pvPortMalloc(sizeof(Sensor_MeasuredData_t)))) {
		mea->PID = inputPID;
		switch (inputPID) {
			case PRESENT_AMBIENT_TEMPERATURE_PID:
				SN_AllocateAndCopy(mea, buffer, sizeof(APC1_SelectedData_t));
				break;
			default:
				break;
		}
	}

	return mea;

}

void SN_AllocateAndCopy(Sensor_MeasuredData_t *mea, void *buffer, size_t dataSize) {

	if ((mea->measuredData = pvPortMalloc(dataSize))) {
		memcpy(mea->measuredData, buffer, dataSize);
	}

}

void SN_FreeMeasuredSensorData(Sensor_MeasuredData_t *mea) {

	if (!mea) {
		return;
	}
	vPortFree(mea->measuredData);

}

void SN_FreeSensorInfo(Node_Sensor_t *sensors) {

	if (!sensors) {
		return;
	}
	for (int i = 0; i<sensors->numOfSensors; i++) {
		if (sensors->sensorArray[i].buttons) {
			vPortFree(sensors->sensorArray[i].buttons);
			sensors->sensorArray[i].buttons = NULL;
		}
	}
	vPortFree(sensors->sensorArray);
	sensors->sensorArray = NULL;

}

Node_ButtonInterface_t *SN_CreateButtons(const char **buttonNames, uint8_t buttonCount) {

	Node_ButtonInterface_t *buttons = NULL;

	if ((buttons = (Node_ButtonInterface_t *) pvPortMalloc(buttonCount * sizeof(Node_ButtonInterface_t)))) {
		for (int i = 0; i<buttonCount; i++) {
			strncpy(buttons[i].name, buttonNames[i], MAX_BUTTON_NAME);
		}
	}

	return buttons;

}

void SN_RegisterAPC1Sensor(Node_SensorInfo_t *sensor) {

	const char *buttonNames[] = APC1_BUTTON_NAMES();
	size_t maxLen = MAX_BUTTON_NAME - 1;

	sensor->PID = PRESENT_AMBIENT_TEMPERATURE_PID;
	sensor->UIcount = APC1_SENSOR_UI_COUNT;
	strncpy(sensor->name, APC1_SENSOR_NAME, maxLen);
	sensor->name[maxLen] = '\0';
	sensor->buttons = SN_CreateButtons(buttonNames, sensor->UIcount);


}

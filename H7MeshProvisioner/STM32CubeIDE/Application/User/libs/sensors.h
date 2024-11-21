/*
 * sensors.h
 *
 *  Created on: Nov 18, 2024
 *      Author: Nejc
 */

#ifndef APPLICATION_USER_LIBS_SENSORS_H_
#define APPLICATION_USER_LIBS_SENSORS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// defines for APC1 sensor
#define APC1_SENSOR_NAME								"APC1"
#define	APC1_SENSOR_UI_COUNT							2
#define APC1_BUTTON_TEMPERATURE							"Temp"
#define APC1_BUTTON_PM1_0								"PM1.0"
#define APC1_BUTTON_NAMES() \
{							\
	APC1_BUTTON_PM1_0,		\
	APC1_BUTTON_TEMPERATURE\
}
#define APC1_BUTTON_PM1_0_INDEX							0
#define APC1_BUTTON_TEMPERATURE_INDEX					1

#define MAX_NODE_NAME	10
#define MAX_BUTTON_NAME	MAX_NODE_NAME

typedef enum {
	PRESENT_AMBIENT_TEMPERATURE_PID = 0x004F
} SENSOR_PID;

typedef struct __attribute__((packed)) {
	uint16_t pm1_0;
	double tComp;
} APC1_SelectedData_t;

typedef struct __attribute__((packed)) {
	uint16_t descCount;
	uint16_t descriptors[3]; // a node can only have three sensors
} SN_SensorDescriptorGet_t;

typedef struct {
	char name[MAX_BUTTON_NAME];
} Node_ButtonInterface_t;

typedef struct {
	char name[MAX_NODE_NAME];
	SENSOR_PID PID;
	int UIcount;
	Node_ButtonInterface_t *buttons;
} Node_SensorInfo_t;

typedef struct {
	uint16_t numOfSensors;
	Node_SensorInfo_t *sensorArray;
} Node_Sensor_t;

typedef struct {
	SENSOR_PID PID;
	void *measuredData;
} Sensor_MeasuredData_t;

Node_Sensor_t SN_RegisterSensor(SN_SensorDescriptorGet_t *descriptors);
Sensor_MeasuredData_t *SN_GetMeasuredSensorData(SENSOR_PID inputPID, void *buffer);
void SN_FreeMeasuredSensorData(Sensor_MeasuredData_t *mea);
void SN_FreeSensorInfo(Node_Sensor_t *sensors);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_LIBS_SENSORS_H_ */

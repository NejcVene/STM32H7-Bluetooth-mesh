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
#define	APC1_SENSOR_UI_COUNT							16
#define APC1_BUTTON_PM1_0								"PM1.0"
#define APC1_BUTTON_PM2_5								"PM2.5"
#define APC1_BUTTON_PM10								"PM10"
#define APC1_BUTTON_PM_1_0_AIR							"PM1.0 air"
#define APC1_BUTTON_PM_2_5_AIR							"PM2.5 air"
#define APC1_BUTTON_PM_10_AIR							"PM10 air"
#define APC1_BUTTON_PARTICLES_0_3						"num > 0.3"
#define APC1_BUTTON_PARTICLES_0_5						"num > 0.5"
#define APC1_BUTTON_PARTICLES_1_0						"num > 1.0"
#define APC1_BUTTON_PARTICLES_2_5						"num > 2.5"
#define APC1_BUTTON_PARTICLES_5_0						"num > 5.0"
#define APC1_BUTTON_PARTICLES_10						"num > 10"
#define APC1_BUTTON_TVOC								"TVOC"
#define APC1_BUTTON_EC02								"eC02"
#define APC1_BUTTON_RH_COMP								"RH"
#define APC1_BUTTON_TEMPERATURE							"Temp"
#define APC1_BUTTON_NAMES() 	\
{								\
	APC1_BUTTON_PM1_0,			\
	APC1_BUTTON_PM2_5,			\
	APC1_BUTTON_PM10,			\
	APC1_BUTTON_PM_1_0_AIR,		\
	APC1_BUTTON_PM_2_5_AIR,		\
	APC1_BUTTON_PM_10_AIR,		\
	APC1_BUTTON_PARTICLES_0_3,	\
	APC1_BUTTON_PARTICLES_0_5,	\
	APC1_BUTTON_PARTICLES_1_0,	\
	APC1_BUTTON_PARTICLES_2_5,	\
	APC1_BUTTON_PARTICLES_5_0,	\
	APC1_BUTTON_PARTICLES_10,	\
	APC1_BUTTON_TVOC,			\
	APC1_BUTTON_EC02,			\
	APC1_BUTTON_RH_COMP,		\
	APC1_BUTTON_TEMPERATURE		\
}
#define APC1_BUTTON_PM1_0_INDEX							0
#define APC1_BUTTON_PM2_5_INDEX							1
#define APC1_BUTTON_PM10_INDEX							2
#define APC1_BUTTON_PM_1_0_AIR_INDEX					3
#define APC1_BUTTON_PM_2_5_AIR_INDEX					4
#define APC1_BUTTON_PM_10_AIR_INDEX						5
#define APC1_BUTTON_PARTICLES_0_3_INDEX					6
#define APC1_BUTTON_PARTICLES_0_5_INDEX					7
#define APC1_BUTTON_PARTICLES_1_0_INDEX					8
#define APC1_BUTTON_PARTICLES_2_5_INDEX					9
#define APC1_BUTTON_PARTICLES_5_0_INDEX					10
#define APC1_BUTTON_PARTICLES_10_INDEX					11
#define APC1_BUTTON_TVOC_INDEX							12
#define APC1_BUTTON_EC02_INDEX							13
#define APC1_BUTTON_RH_COMP_INDEX						14
#define APC1_BUTTON_TEMPERATURE_INDEX					15

#define MAX_NODE_NAME	10
#define MAX_BUTTON_NAME	MAX_NODE_NAME

typedef enum {
	PRESENT_AMBIENT_TEMPERATURE_PID = 0x004F
} SENSOR_PID;

typedef struct __attribute__((packed)) {
	uint16_t pm1_0, pm2_5, pm10,
			 pm1_0_air, pm2_5_air, pm10_air,
			 particles_0_3, particles_0_5, particles_1_0,
			 particles_2_5, particles_5_0, particles_10,
			 TVOC, eCO2;
	double rh_comp, tComp;
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

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

void SN_SaveSensor(Node_SensorInfo_t *sensor, SENSOR_PID pid, int UIcount, const char **buttonNames, char *sensorName);
Node_ButtonInterface_t *SN_CreateButtons(const char **buttonNames, uint8_t buttonCount);
void SN_AllocateAndCopy(Sensor_MeasuredData_t *mea, void *buffer, size_t dataSize);

/*
 * How to add a sensor?
 * 		1.	Inside SN_RegisterSensor function, add a new case with the sensor
 * 			PID as its value. Inside this case call the SN_SaveSensor function you created
 * 			into which pass the required sensor parameters.
 * 		2. 	Create a new structure in sensors.h which will represent
 * 			the actual measurement from the sensor.
 * 		3.	Inside SN_GetMeasuredSensorData function, add a new case with
 * 			the sensor PID as its value. Inside this case call SN_AllocateAndCopy
 * 			function who's parameters are:
 * 				- allocated mea (already done),
 * 				- buffer (already passed in),
 * 				- sizeof the structure which represents the actual measurement
 * 				  from the sensor (from step 4).
 * 		4.	In sensors.h as defines add all buttons names and their indexes.
 * 		5.	In sesnors.h create a define which will represent all buttons names
 * 			as an array.
 * 		Appendix A: basically, do everything like how the APC1 sensor is defined.
 * 		Appendix B: don't forget to add it to NodeSensorScreenView.cpp inside GUI_UpdateDeviceButtonValues().
 * 		Appendix C: the maximum number of buttons is limited to 20 because of the GUI. Inside SN_SaveSensor, we check
 * 					for this limit. If it is exceeded the value 20 is used.
 */

/**
  * @brief  Register a new sensor specified by its descriptor.
  * @param  descriptors	Pointer to a SN_SensorDescriptorGet_t structure that contains
  * 					information about found sensors on a node.
  * @retval	Node_Sensor_t structure
  */
Node_Sensor_t SN_RegisterSensor(SN_SensorDescriptorGet_t *descriptors) {

	Node_Sensor_t sensors;

	sensors.numOfSensors = descriptors->descCount;
	if ((sensors.sensorArray = (Node_SensorInfo_t *) pvPortMalloc(descriptors->descCount * sizeof(Node_SensorInfo_t)))) {
		for (int i = 0; i<descriptors->descCount; i++) {
			switch ((SENSOR_PID) descriptors->descriptors[i]) {
				case PRESENT_AMBIENT_TEMPERATURE_PID:
					SN_SaveSensor(&sensors.sensorArray[i],
								  PRESENT_AMBIENT_TEMPERATURE_PID,
								  APC1_SENSOR_UI_COUNT,
								  APC1_BUTTON_NAMES,
								  APC1_SENSOR_NAME);
					break;
				default:
					break;
			}
		}
	}

	return sensors;

}

/**
  * @brief  Interpret received sensor measurement.
  * @param  inputPID	PID (descriptor) of sensor who took the measurement.
  * @param	buffer		void pointer to a buffer which contains measured data.
  * @retval	Sensor_MeasuredData_t pointer
  */
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

/**
  * @brief  Copies data from buffer into mea for the correct sensor.
  * @param  mea			Sensor_MeasuredData_t pointer with will have received
  * 					sensor data.
  * @param	buffer		Void pointer to a buffer which contains measured data.
  * @param	dataSize	Size of the structure to which the data belongs.
  * @retval	None
  */
void SN_AllocateAndCopy(Sensor_MeasuredData_t *mea, void *buffer, size_t dataSize) {

	if ((mea->measuredData = pvPortMalloc(dataSize))) {
		memcpy(mea->measuredData, buffer, dataSize);
	}

}

/**
  * @brief  Free allocated memory for sensor measurement data.
  * @param  mea			Sensor_MeasuredData_t pointer which has received
  * 					sensor data.
  * @retval	None
  */
void SN_FreeMeasuredSensorData(Sensor_MeasuredData_t *mea) {

	if (!mea) {
		return;
	}
	vPortFree(mea->measuredData);

}

/**
  * @brief  Free allocated memory for sensors on a node.
  * @param  sensors		Node_Sensor_t pointer which has all the sensors
  * 					registered on a node.
  * @retval	None
  */
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

/**
  * @brief  Create buttonCount number of buttons for a sensor which are used by
  * 		GUI.
  * @param  buttonNames		An array of strings which represent button names.
  * 						registered on a node.
  * @param	buttonCount		The number of buttons to be created.
  * @retval	Node_ButtonInterface_t pointer
  */
Node_ButtonInterface_t *SN_CreateButtons(const char **buttonNames, uint8_t buttonCount) {

	Node_ButtonInterface_t *buttons = NULL;

	if ((buttons = (Node_ButtonInterface_t *) pvPortMalloc(buttonCount * sizeof(Node_ButtonInterface_t)))) {
		for (int i = 0; i<buttonCount; i++) {
			strncpy(buttons[i].name, buttonNames[i], MAX_BUTTON_NAME);
		}
	}

	return buttons;

}

/**
  * @brief  Save sensor settings.
  * @param  sensor		Node_SensorInfo_t pointer into which the sensor is registered.
  * @param	pid			PID by which the sensor is identified.
  * @param	UIcount		Number of UI buttons the sensor uses to show measurements.
  * @param	buttonNames	Names for all of the UI buttons for measurements.
  * @param	sensorName	Name of the sensor which will be displayed.
  * @retval	None
  */
void SN_SaveSensor(Node_SensorInfo_t *sensor, SENSOR_PID pid, int UIcount, const char **buttonNames, char *sensorName) {

	size_t maxLen = MAX_BUTTON_NAME - 1;


	sensor->PID = pid;
	sensor->UIcount = UIcount < MAX_SENSOR_BUTTONS ? UIcount : MAX_SENSOR_BUTTONS;
	strncpy(sensor->name, sensorName, maxLen);
	sensor->name[maxLen] = '\0';
	sensor->buttons = SN_CreateButtons(buttonNames, sensor->UIcount);

}


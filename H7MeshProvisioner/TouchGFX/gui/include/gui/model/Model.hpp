#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>
#include "communication_ex.h"
#include "command.h"
#include "node_config.h"
#include "sensors.h"

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    Node_NetworkAddress_t *GUI_GetFoundDevices() {

    	return foundDevices;

    }

    Node_Config_t *GUI_GetDeviceToConfigure() {

    	return deviceToConfigure;

    }

    void GUI_SetDeviceToConfigure(Node_Config_t *node) {

    	this->deviceToConfigure = node;

    }

    NC_MaskedFeatures *GUI_GetAllModels() {

    	return allModels;

    }

    void GUI_SetAllGroupAddress(NC_MaskedFeatures *allGroupAddress) {

    	this->allModels = allGroupAddress;

    }

    uint8_t GUI_GetRoomBitmask() {

    	return roomBitmask;

    }

    void GUI_SetRoomBitmask(uint8_t bitmask) {

    	roomBitmask = bitmask;

    }

    const char *GUI_GetRoomName(void) {

    	return roomName;

    }

    void GUI_SetRoomName(const char *roomName) {

    	strcpy(this->roomName, roomName);

    }

    void GUI_SetNodeAddress(int nodeAddress) {

    	this->nodeAddress = nodeAddress;

    }

    uint32_t GUI_GetNodeAddress() {

    	return this->nodeAddress;

    }

    int GUI_GetUsedCommandIndex() {

    	return this->usedCommandIndex;

    }

    void GUI_SetUsedCommandIndex(int usedCommandIndex) {

    	this->usedCommandIndex = usedCommandIndex;

    }

    uint8_t GUI_GetSeconds() {

    	return this->seconds;

    }

    uint8_t GUI_GetMinutes() {


    	return this->minutes;

    }

    uint8_t GUI_GetHours() {

    	return this->hours;

    }

    const char *GUI_GetLibVer() {

    	return libVer;

    }

    const char *GUI_GetLibSubVer() {

    	return libSubVer;

    }

    const char *GUI_GetCommExVer() {

    	return commExVer;

    }

    void GUI_SetSensorIndex(int sensorIndex) {

    	this->sensorIndex = sensorIndex;

    }

    int GUI_GetSensorIndex() {

    	return sensorIndex;

    }

    Sensor_MeasuredData_t *GUI_GetSensorMeasuredData() {

    	return this->sensor;

    }

    void GUI_SendCommand(CMD_CommandGet_t *cmd);
    void DigitalClock();
    void GUI_SetDigialClock(uint8_t hours, uint8_t minutes);

protected:
    ModelListener* modelListener;
    Node_NetworkAddress_t *foundDevices;
    Node_Config_t *deviceToConfigure;
    NC_MaskedFeatures *allModels;
    uint8_t roomBitmask;
    uint32_t nodeAddress;
    int usedCommandIndex;
    char roomName[30] = {0};
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    char libVer[20] = {0};
    char libSubVer[20] = {0};
    char commExVer[10] = {0};
    int sensorIndex;
    Sensor_MeasuredData_t *sensor;
};

#endif // MODEL_HPP

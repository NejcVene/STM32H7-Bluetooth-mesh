#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>
#include "communication_ex.h"
#include "command.h"
#include "node_config.h"

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



    void GUI_SendCommand(CMD_CommandGet_t *cmd);
protected:
    ModelListener* modelListener;
    Node_NetworkAddress_t *foundDevices;
    Node_Config_t *deviceToConfigure;
    NC_MaskedFeatures *allModels;
    uint8_t roomBitmask;
    uint32_t nodeAddress;
    char roomName[30];
};

#endif // MODEL_HPP

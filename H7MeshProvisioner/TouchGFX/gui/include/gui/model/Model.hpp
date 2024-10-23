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

    NC_MaskedFeatures *GUI_GetAllModels() {

    	return allModels;

    }

    void GUI_SendCommand(CMD_CommandGet_t *cmd);
protected:
    ModelListener* modelListener;
    Node_NetworkAddress_t *foundDevices;
    Node_Config_t *deviceToConfigure;
    NC_MaskedFeatures *allModels;
};

#endif // MODEL_HPP

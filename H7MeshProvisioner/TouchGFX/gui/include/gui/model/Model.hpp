#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>
#include "communication_ex.h"
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

    void GUI_SendCommand(uint16_t cmdIndex, void *cmdParam);
protected:
    ModelListener* modelListener;
    Node_NetworkAddress_t *foundDevices;
};

#endif // MODEL_HPP

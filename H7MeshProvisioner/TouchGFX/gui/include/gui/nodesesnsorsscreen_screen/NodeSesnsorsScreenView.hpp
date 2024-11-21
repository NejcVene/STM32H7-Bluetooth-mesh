#ifndef NODESESNSORSSCREENVIEW_HPP
#define NODESESNSORSSCREENVIEW_HPP

#include <gui_generated/nodesesnsorsscreen_screen/NodeSesnsorsScreenViewBase.hpp>
#include <gui/nodesesnsorsscreen_screen/NodeSesnsorsScreenPresenter.hpp>
#include <gui/containers/DeviceButton.hpp>
#include "sensors.h"

class NodeSesnsorsScreenView : public NodeSesnsorsScreenViewBase
{
public:

    NodeSesnsorsScreenView();
    virtual ~NodeSesnsorsScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    virtual void GUI_UpdateSensorValues();

    void GUI_SetupSensorButtons();
    void GUI_UpdateDeviceButtonValues();

protected:

    CMD_CommandGet_t *cmd;
    Node_Config_t *node;
    Node_SensorInfo_t *sensor;
    Sensor_MeasuredData_t *mea;
    static const int maxButtons = 10;
    int buttonCount = 0;
    DeviceButton sensorButtons[maxButtons];
    int sensorTickCounter = 0;
    int sensorDigitalSeconds;
    int sensorIndex;

};

#endif // NODESESNSORSSCREENVIEW_HPP

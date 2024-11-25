#ifndef NODEPROPERSCREENVIEW_HPP
#define NODEPROPERSCREENVIEW_HPP

#include <gui_generated/nodeproperscreen_screen/NodeProperScreenViewBase.hpp>
#include <gui/nodeproperscreen_screen/NodeProperScreenPresenter.hpp>
#include <gui/containers/DeviceButton.hpp>

class NodeProperScreenView : public NodeProperScreenViewBase
{
public:
    NodeProperScreenView();
    virtual ~NodeProperScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    virtual void GUI_ConfirmDescGet();

    int getBtnIndexFromInstaceID(int instanceID);
    int generateInstanceID(int guiIndex, DeviceButton::ICON_TYPE type);

    void setupGenericOnOff(int *guiIndex, int numToCreate);
    void setupGenericLevel(int *guiIndex, int numToCreate);
    void setupSensor(int *guiIndex, int numToCreate);
    void setupLightLightness(int *guiIndex, int numToCreate);
    void setupGenericPower(int *guiIndex, int numToCreate);
    void setupRename(int *guiIndex);
    void setupConfig(int *guiIndex);
    void setupUnprov(int *guiIndex);

    void handleGenericOnOffClicked(int instanceID);
    void handleGenericLevelClicked(int instanceID);
    void handleSensor(int instanceID);
    void handleLightClicked(int instanceID);
    void handleGenericPowerClicked(int instanceID);
    void handleRenameClicked(int instanceID);
    void handleConfigureClicked(int instanceID);
    void handleDeleteClicked(int instanceID);

    void checkForSensors();
    void setupDeviceButtons();

    void changeScreenAfterUnprov();
    void GUI_ErrorOccured();

//    void GUI_SetTextNodeName(const char *text) {
//
//    	touchgfx::Unicode::strncpy(nodeNameBuffer, text, NODENAME_SIZE);
//    	nodeName.resizeToCurrentText();
//    	nodeName.invalidate();
//
//    }

protected:
    touchgfx::Callback<NodeProperScreenView, int> buttonClickCallback;
    void handleButtonClicked(int instanceID);
    Node_Config_t *node;
    uint16_t nodeModel;
    DeviceButton genOnOffBtn;
    static const int maxDeviceFunctions = 10;
    static const int MULTIPLIER = 1000;
    DeviceButton deviceFunctions[maxDeviceFunctions];
    CMD_CommandGet_t *cmd;
    NC_MaskedFeatures *allModels;

};

#endif // NODEPROPERSCREENVIEW_HPP

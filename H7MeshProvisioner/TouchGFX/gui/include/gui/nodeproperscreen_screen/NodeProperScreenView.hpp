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

    virtual int getBtnIndexFromInstaceID(int instanceID);

    virtual void setupGenericOnOff(int guiIndex);
    virtual void setupGenericLevel(int guiIndex);
    virtual void setupSensor(int guiIndex);
    virtual void setupLightLightness(int guiIndex);
    virtual void setupGenericPower(int guiIndex);
    virtual void setupConfig(int guiIndex);
    virtual void setupUnprov(int guiIndex);

    virtual void handleGenericOnOffClicked(int instanceID);
    virtual void handleGenericLevelClicked(int instanceID);
    virtual void handleSensor(int instanceID);
    virtual void handleLightClicked(int instanceID);
    virtual void handleGenericPowerClicked(int instanceID);
    virtual void handleConfigureClicked(int instanceID);
    virtual void handleDeleteClicked(int instanceID);

    virtual void changeScreenAfterUnprov();

    void GUI_SetTextNodeName(const char *text) {

    	touchgfx::Unicode::strncpy(nodeNameBuffer, text, NODENAME_SIZE);
    	nodeName.resizeToCurrentText();
    	nodeName.invalidate();

    }

protected:
    touchgfx::Callback<NodeProperScreenView, int> buttonClickCallback;
    void handleButtonClicked(int instanceID);
    Node_Config_t *node;
    uint16_t nodeModel;
    DeviceButton genOnOffBtn;
    static const int maxDeviceFunctions = 7;
    DeviceButton deviceFunctions[maxDeviceFunctions];
    CMD_CommandGet_t *cmd;
    NC_MaskedFeatures *allModels;
};

#endif // NODEPROPERSCREENVIEW_HPP

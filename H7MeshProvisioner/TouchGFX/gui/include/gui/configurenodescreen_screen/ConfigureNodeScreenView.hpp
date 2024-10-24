#ifndef CONFIGURENODESCREENVIEW_HPP
#define CONFIGURENODESCREENVIEW_HPP

#include <gui_generated/configurenodescreen_screen/ConfigureNodeScreenViewBase.hpp>
#include <gui/configurenodescreen_screen/ConfigureNodeScreenPresenter.hpp>
#include <gui/containers/DeviceSubs.hpp>

class ConfigureNodeScreenView : public ConfigureNodeScreenViewBase
{
public:

    ConfigureNodeScreenView();
    virtual ~ConfigureNodeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void GUI_SetDeviceToConfigure(Node_Config_t *configureDevice);
    virtual void GUI_SaveConfNode();
    virtual void GUI_ConfBtnClicked();
    virtual void GUI_CancelBtnClicked();
    virtual void GUI_ShowPopup();

    void setTextNodeUUID(const char *text) {
    	touchgfx::Unicode::strncpy(nodeUUIDBuffer, text, NODEUUID_SIZE);
    	nodeUUID.resizeToCurrentText();
    	nodeUUID.invalidate();
    }

    void setTextNodeName(const char *text) {
    	touchgfx::Unicode::strncpy(nodeNameBuffer, text, NODENAME_SIZE);
    	nodeName.resizeToCurrentText();
    	nodeName.invalidate();
    }

    void setTextVendor(const char *text) {
    	touchgfx::Unicode::strncpy(nodeVendorBuffer, text, NODEVENDOR_SIZE);
    	nodeVendor.resizeToCurrentText();
    	nodeVendor.invalidate();
    }

protected:

    static const int maxSubsPerNode = 5;
    DeviceSubs nodeSubscriptions[maxSubsPerNode];

private:

    touchgfx::Callback<ConfigureNodeScreenView, int> buttonClickCallback;
    void handleButtonClicked(int instanceID);
    Node_Config_t *configNode;
    static Node_SubscriptionParam_t toSubb;
    static CMD_CommandGet_t *cmd;
    NC_MaskedFeatures *allGroupAddress;

};

#endif // CONFIGURENODESCREENVIEW_HPP

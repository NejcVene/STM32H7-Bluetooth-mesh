#ifndef ADDDEVICESCREENVIEW_HPP
#define ADDDEVICESCREENVIEW_HPP

#include <gui_generated/adddevicescreen_screen/AddDeviceScreenViewBase.hpp>
#include <gui/adddevicescreen_screen/AddDeviceScreenPresenter.hpp>
#include <gui/containers/DeviceAdd.hpp>

class AddDeviceScreenView : public AddDeviceScreenViewBase
{
public:

    AddDeviceScreenView();
    virtual ~AddDeviceScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void GUI_SetDevicesFound(Node_NetworkAddress_t *foundDevices);
    virtual void GUI_SwitchScreenAfterProv();
    void GUI_ErrorOccured();

    void GUI_NotificationSetText(const char *text) {

    	touchgfx::Unicode::strncpy(noNodesFoundTextBuffer, text, NONODESFOUNDTEXT_SIZE);
    	noNodesFoundText.invalidate();
    	noNodesFoundMsg.setVisible(true);

    }

protected:

    static const int maxButtons = 5;
    DeviceAdd deviceAdds[maxButtons];

private:

    static CMD_CommandGet_t *cmd;
    touchgfx::Callback<AddDeviceScreenView, int> buttonClickCallback;
    void handleButtonClicked(int instanceID);
    int usedCommandIndex;

};

#endif // ADDDEVICESCREENVIEW_HPP

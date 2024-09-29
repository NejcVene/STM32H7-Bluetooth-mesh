#ifndef ADDDEVICESCREENVIEW_HPP
#define ADDDEVICESCREENVIEW_HPP

#include <gui_generated/adddevicescreen_screen/AddDeviceScreenViewBase.hpp>
#include <gui/adddevicescreen_screen/AddDeviceScreenPresenter.hpp>

class AddDeviceScreenView : public AddDeviceScreenViewBase
{
public:
    AddDeviceScreenView();
    virtual ~AddDeviceScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void GUI_SetDevicesFound(Node_NetworkAddress_t *foundDevices);
protected:
    bool increase = true;
    bool disable = false;
    static const int maxButtons = 5;
    touchgfx::WildcardTextButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger > > deviceFoundBtn[maxButtons]; // Array of 5 buttons
};

#endif // ADDDEVICESCREENVIEW_HPP

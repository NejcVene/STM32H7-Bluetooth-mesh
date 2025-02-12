#ifndef DEVICETOGGLE_HPP
#define DEVICETOGGLE_HPP

#include <gui_generated/containers/DeviceToggleBase.hpp>
#include <touchgfx/Callback.hpp>

class DeviceToggle : public DeviceToggleBase
{
public:

    DeviceToggle();
    virtual ~DeviceToggle() {}

    virtual void initialize();

    void GUI_SetTextDeviceName(const char *text) {
    	touchgfx::Unicode::strncpy(deviceNameTextBuffer, text, DEVICENAMETEXT_SIZE);
    	deviceNameText.resizeToCurrentText();
    	deviceNameText.invalidate();
    }

    void setButtonAction(touchgfx::GenericCallback<int>& callback, int instanceID) {
    	buttonClickedCallback = &callback;
    	this->instanceID = instanceID;
    }

    virtual void onButtonClicked(const touchgfx::AbstractButton& button);

protected:

    virtual void emitSomethingHappenedCallback() {
    	if (buttonClickedCallback && buttonClickedCallback->isValid()) {
    		buttonClickedCallback->execute(instanceID);
    	}
    }

private:

    touchgfx::GenericCallback<int>* buttonClickedCallback;
    int instanceID;
    touchgfx::Callback<DeviceToggle, const touchgfx::AbstractButton&> buttonCallback;

};

#endif // DEVICETOGGLE_HPP

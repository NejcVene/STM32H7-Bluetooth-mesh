#ifndef DEVICESUBS_HPP
#define DEVICESUBS_HPP

#include <gui_generated/containers/DeviceSubsBase.hpp>
#include <touchgfx/Callback.hpp>

class DeviceSubs : public DeviceSubsBase
{
public:

    DeviceSubs();
    virtual ~DeviceSubs() {}

    virtual void initialize();

    void setButtonAction(touchgfx::GenericCallback<int>& callback, int instanceID) {
    	buttonClickedCallback = &callback;
    	this->instanceID = instanceID;
    }

    void setTextAddressName(const char *text) {
    	touchgfx::Unicode::strncpy(addressLabelBuffer, text, ADDRESSLABEL_SIZE);
    	addressLabel.resizeToCurrentText();
    	addressLabel.invalidate();
    }

    void setButtonEnable(bool state) {
    	subscribeBtn.forceState(state);
    	subscribeBtn.setTouchable(state);
    	grayBox.setVisible(!state);
    	subscribeBtn.invalidate();
    }

    int getInstanceID(void) {

    	return this->instanceID;

    }

    bool isButtonOn(void) {

    	return subscribeBtn.getPressedState();

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
    touchgfx::Callback<DeviceSubs, const touchgfx::AbstractButton&> buttonCallback;

};

#endif // DEVICESUBS_HPP

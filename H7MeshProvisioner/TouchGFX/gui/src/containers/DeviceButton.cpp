#include <gui/containers/DeviceButton.hpp>

DeviceButton::DeviceButton() : buttonClickedCallback(nullptr),
instanceID(0),
buttonCallback(this, &DeviceButton::onButtonClicked)
{

}

void DeviceButton::initialize()
{
    DeviceButtonBase::initialize();
    interactBtn.setAction(buttonCallback);
}

void DeviceButton::onButtonClicked(const touchgfx::AbstractButton& button) {

	DeviceButton::emitSomethingHappenedCallback();

}

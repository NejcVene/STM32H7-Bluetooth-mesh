#include <gui/containers/DeviceToggle.hpp>

DeviceToggle::DeviceToggle() : buttonClickedCallback(nullptr),
instanceID(0),
buttonCallback(this, &DeviceToggle::onButtonClicked)
{

}

void DeviceToggle::initialize()
{
    DeviceToggleBase::initialize();
    deviceToggleBtn.setAction(buttonCallback);
}

void DeviceToggle::onButtonClicked(const touchgfx::AbstractButton& button){

	DeviceToggle::emitSomethingHappenedCallback();

}

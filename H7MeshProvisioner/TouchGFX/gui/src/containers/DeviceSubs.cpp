#include <gui/containers/DeviceSubs.hpp>

DeviceSubs::DeviceSubs() : buttonClickedCallback(nullptr),
instanceID(0),
buttonCallback(this, &DeviceSubs::onButtonClicked)
{

}

void DeviceSubs::initialize()
{
    DeviceSubsBase::initialize();
    subscribeBtn.setAction(buttonCallback);
}

void DeviceSubs::onButtonClicked(const touchgfx::AbstractButton& button){

	DeviceSubs::emitSomethingHappenedCallback();

}

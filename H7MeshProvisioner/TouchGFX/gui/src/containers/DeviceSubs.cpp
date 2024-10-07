#include <gui/containers/DeviceSubs.hpp>

DeviceSubs::DeviceSubs() : buttonClickedCallback(nullptr),
instanceID(0),
buttonCallback(this, &DeviceSubs::onButtonClicked)
{

}

void DeviceSubs::initialize()
{
    DeviceSubsBase::initialize();
}

void DeviceSubs::onButtonClicked(const touchgfx::AbstractButton& button){

	DeviceSubs::emitSomethingHappenedCallback();

}

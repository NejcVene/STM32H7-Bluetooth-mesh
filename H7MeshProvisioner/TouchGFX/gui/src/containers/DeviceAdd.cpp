#include <gui/containers/DeviceAdd.hpp>

DeviceAdd::DeviceAdd() : buttonClickedCallback(nullptr),
instanceID(0),
buttonCallback(this, &DeviceAdd::onButtonClicked)
{
}

void DeviceAdd::initialize()
{
    DeviceAddBase::initialize();
    provBtn.setAction(buttonCallback);

}

void DeviceAdd::onButtonClicked(const touchgfx::AbstractButton& button) {

	DeviceAdd::emitSomethingHappenedCallback();

}


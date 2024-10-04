#include <gui/containers/DeviceAdd.hpp>

DeviceAdd::DeviceAdd()
    : buttonClickedCallback(nullptr),
      instanceID(0),
      buttonCallback(this, &DeviceAdd::onButtonClicked)  // Initialize in the correct order
{
}

void DeviceAdd::initialize()
{
    DeviceAddBase::initialize();

    // Bind the FlexButton's action to the member variable callback
    deviceAddListBtn.setAction(buttonCallback);
}

void DeviceAdd::onButtonClicked(const touchgfx::AbstractButtonContainer& button) {
    // Call the callback when the button is clicked
    emitSomethingHappenedCallback(button);  // Pass the actual button reference
}

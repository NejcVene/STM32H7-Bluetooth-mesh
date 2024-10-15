#include <gui/adddevicescreen_screen/AddDeviceScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "node_config.h"
#include "command.h"

int nay2 = 0;

AddDeviceScreenView::AddDeviceScreenView() : buttonClickCallback(this, &AddDeviceScreenView::handleButtonClicked)
{

}

void AddDeviceScreenView::setupScreen()
{
    AddDeviceScreenViewBase::setupScreen();
    foudNodeBtn.setVisible(false);
    for (int i = 0; i<maxButtons; i++) {
    	deviceAdds[i].initialize();
    	deviceAdds[i].setPosition(10, 70 * i, 460, 60);
    	deviceAdds[i].setVisible(false);
    	scrollableContainer1.add(deviceAdds[i]);
    }
    modalWindow1.show();
    presenter->GUI_SendCommand(CMD_MESH_ATEP_SCAN, NULL);
}

void AddDeviceScreenView::tearDownScreen()
{
    AddDeviceScreenViewBase::tearDownScreen();
}

void AddDeviceScreenView::handleTickEvent() {

	if (disable) {
		modalWindow1.hide();
		return;
	}

	int nextValue;
	int currentValue = circleProgress1.getValue();
	int16_t max = 100;
	int16_t min = 0;

	if (increase) {
		nextValue = currentValue + 1;
		if (nextValue == max) {
			increase = false;
		}
	} else if (!increase) {
		nextValue = currentValue - 1;
		if (nextValue == min) {
			increase = true;
		}
	}
	circleProgress1.setValue(nextValue);

}

void AddDeviceScreenView::GUI_SetDevicesFound(Node_NetworkAddress_t *foundDevices) {

	disable = true;
	for (int i = 0; i<5; i++) {
		if (foundDevices[i].nodeAddress != NODE_DEF_VAL) {
			deviceAdds[i].setButtonAction(buttonClickCallback, foundDevices->nodeAddress);
			deviceAdds[i].setTextUUID(foundDevices->uuid);
			deviceAdds[i].setTextModels(foundDevices->nodeModels);
			deviceAdds[i].setTextFeatures(foundDevices->nodeFeatures);
			deviceAdds[i].setVisible(true);
		}
	}
	scrollableContainer1.invalidateContent();

}

void AddDeviceScreenView::handleButtonClicked(int instanceID) {

	static int cachedInstanceID = instanceID;
	presenter->GUI_SendCommand(CMD_MESH_ATEP_PRVN, (void *) &cachedInstanceID);

}

void AddDeviceScreenView::GUI_SwitchScreenAfterProv() {

	goToDeviceConfigScreen();

}


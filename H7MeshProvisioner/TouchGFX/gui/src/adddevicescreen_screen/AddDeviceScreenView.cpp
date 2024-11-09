#include <gui/adddevicescreen_screen/AddDeviceScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "node_config.h"
#include "command.h"

CMD_CommandGet_t *AddDeviceScreenView::cmd = nullptr;

AddDeviceScreenView::AddDeviceScreenView() : buttonClickCallback(this, &AddDeviceScreenView::handleButtonClicked)
{

}

void AddDeviceScreenView::setupScreen()
{
    AddDeviceScreenViewBase::setupScreen();
    screenTitleBar1.GUI_SetTextTitleBar("Menu", "add device");
    noNodesFoundMsg.setVisible(false);
    for (int i = 0; i<maxButtons; i++) {
    	deviceAdds[i].initialize();
    	deviceAdds[i].setPosition(10, 70 * i, 460, 60);
    	deviceAdds[i].setVisible(false);
    	scrollableContainer1.add(deviceAdds[i]);
    }
    modalWindow1.show();
    this->cmd = CMD_CreateCommandGet(CMD_MESH_ATEP_SCAN,
    								NULL,
									NULL,
									0,
									NULL,
									NULL);
    presenter->GUI_SendCommand(this->cmd);
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
	int nodeFound = 0;
	for (int i = 0; i<5; i++) {
		if (foundDevices[i].nodeAddress != NODE_DEF_VAL) {
			nodeFound = 1;
			deviceAdds[i].setButtonAction(buttonClickCallback, foundDevices->nodeAddress);
			deviceAdds[i].setTextUUID(foundDevices->uuid);
			deviceAdds[i].setTextModels(NC_GetNodeModelString(foundDevices->nodeModels));
			deviceAdds[i].setTextFeatures(NC_GetNodeFeatureString(foundDevices->nodeFeatures));
			deviceAdds[i].setVisible(true);
		}
	}
	if (!nodeFound) {
		noNodesFoundMsg.setVisible(true);
	}
	scrollableContainer1.invalidateContent();

}

void AddDeviceScreenView::handleButtonClicked(int instanceID) {

	PARAMETER_TYPE type = PARAM_INT;
	void *values[] = {&instanceID};
	int arrayLengths[] = {0};
	size_t sizes[] = {0};
	this->usedCommandIndex = presenter->GUI_GetUsedCommandIndex();
	CMD_INDEX cmdToPrvn;

	if ((CMD_INDEX) this->usedCommandIndex == CMD_MESH_ATEP_SCAN) {
		cmdToPrvn = CMD_MESH_ATEP_PRVN;
	} else if ((CMD_INDEX) this->usedCommandIndex == CMD_MESH_ATEP_SCAN_RANGE) {
		cmdToPrvn = CMD_MESH_ATEP_PRVN_RANGE;
	}
	this->cmd = CMD_CreateCommandGet(cmdToPrvn,
									&type,
									values,
									1,
									arrayLengths,
									sizes);
	presenter->GUI_SendCommand(this->cmd);

}

void AddDeviceScreenView::GUI_SwitchScreenAfterProv() {

	goToDeviceConfigScreen();

}


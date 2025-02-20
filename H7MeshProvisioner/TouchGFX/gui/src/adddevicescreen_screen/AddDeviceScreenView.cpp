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

    if (NC_GetNumOfConfNodes() == MAX_CONF_NODES) {
    	// show message that no more nodes can be provisioned
    	GUI_NotificationSetText("Maximum number of nodes reached");
    } else {
    	modalWindow1.show();
		loader1.GUI_SetLoaderText("Searching");
		loader1.GUI_SetDisable(false);
		this->cmd = CMD_CreateCommandGet(CMD_MESH_ATEP_SCAN,
										NULL,
										NULL,
										0,
										NULL,
										NULL);
		presenter->GUI_SendCommand(this->cmd);
    }

}

void AddDeviceScreenView::tearDownScreen()
{
    AddDeviceScreenViewBase::tearDownScreen();
}

void AddDeviceScreenView::handleTickEvent() {

	loader1.GUI_ProgressLoader();

}

void AddDeviceScreenView::GUI_SetDevicesFound(Node_NetworkAddress_t *foundDevices) {

//	disable = true;
	modalWindow1.hide();
	loader1.GUI_SetDisable(true);
	loader1.GUI_ResetProgressValue();
	int nodeFound = 0;
	for (int i = 0; i<5; i++) {
		if (foundDevices[i].nodeAddress != NODE_DEF_VAL) {
			nodeFound = 1;
			deviceAdds[i].setButtonAction(buttonClickCallback, foundDevices[i].nodeAddress);
			deviceAdds[i].setTextUUID(foundDevices[i].uuid);
			deviceAdds[i].setTextModels(NC_GetNodeModelsFeaturesString(
										NC_GetAllModels(),
										foundDevices[i].nodeModels));
			deviceAdds[i].setTextFeatures(NC_GetNodeModelsFeaturesString(
										NC_GetAllFeatures(),
										foundDevices[i].nodeFeatures));
			deviceAdds[i].setVisible(true);
		}
	}
	if (!nodeFound) {
		GUI_NotificationSetText("No nodes found");
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
	modalWindow1.show();
	loader1.GUI_SetLoaderText("Provisioning node");
	loader1.GUI_SetDisable(false);
	this->cmd = CMD_CreateCommandGet(cmdToPrvn,
									&type,
									values,
									1,
									arrayLengths,
									sizes);
	presenter->GUI_SendCommand(this->cmd);

}

void AddDeviceScreenView::GUI_SwitchScreenAfterProv() {

	modalWindow1.hide();
	loader1.GUI_SetDisable(true);
	loader1.GUI_ResetProgressValue();
	goToDeviceConfigScreen();

}

void AddDeviceScreenView::GUI_ErrorOccured() {

	goToErrorScreen();

}


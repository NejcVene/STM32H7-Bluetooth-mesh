#include <gui/nodeproperscreen_screen/NodeProperScreenView.hpp>
#include "command.h"

NodeProperScreenView::NodeProperScreenView() : buttonClickCallback(this, &NodeProperScreenView::handleButtonClicked)
{

}

void NodeProperScreenView::setupScreen()
{
    NodeProperScreenViewBase::setupScreen();
    loaderModal.hide();
    loader1.GUI_SetDisable(true);
    this->node = NC_GetConfigNodeFromAddress(presenter->GUI_GetNodeAddress());
    this->nodeModel = node->address.nodeModels;
    this->allModels = NC_GetAllModels();
    int yPos = 0;
    int xPos = 10;
    int guiIndex = 0;
//    DeviceButton::ICON_TYPE types[] = {
//    		DeviceButton::GENERIC_ON_OFF,
//			DeviceButton::GENERIC_LEVEL,
//			DeviceButton::SENSOR,
//			DeviceButton::LIGHT_LIGHTNESS,
//			DeviceButton::GENERIC_POWER,
//			DeviceButton::CONFIGURE,
//			DeviceButton::UNPROVISION
//    };


//    GUI_SetTextNodeName(node->nodeName);
    screenTitleBar1.GUI_SetTextTitleBar("Device", node->nodeName);
    for (int i = 0; i<maxDeviceFunctions; i++) {
    	deviceFunctions[i].initialize();
    	deviceFunctions[i].setPosition(xPos, yPos, 112, 112);
    	deviceFunctions[i].setVisible(false);
    	nodeOptions.add(deviceFunctions[i]);
    	if (xPos >= 358) {
    		yPos += (112 + 10);
    		xPos = 10;
    	} else {
    		xPos += (112 + 62);
    	}
    }
    if (this->nodeModel & NC_GENERIC_ON_OFF_MODEL) {
    	setupGenericOnOff(guiIndex);
    	guiIndex++;
    }
    if (this->nodeModel & NC_GENERIC_LEVEL_MODEL) {
    	setupGenericLevel(guiIndex);
    	guiIndex++;
    }
    if (this->nodeModel & NC_SENSOR_MODEL) {
    	setupSensor(guiIndex);
    	guiIndex++;
    }
    if (this->nodeModel & NC_LIGH_LIGHTNESS_MODEL) {
    	setupLightLightness(guiIndex);
    	guiIndex++;
    }
    if (this->nodeModel & NC_GENERIC_POWER_ON_OFF_MODEL) {
    	setupGenericPower(guiIndex);
    	guiIndex++;
    }
    setupConfig(guiIndex);
    guiIndex++;
    setupUnprov(guiIndex);
    guiIndex++;

}

void NodeProperScreenView::tearDownScreen()
{
    NodeProperScreenViewBase::tearDownScreen();
}

void NodeProperScreenView::handleTickEvent() {

	loader1.GUI_ProgressLoader();

}

void NodeProperScreenView::setupGenericOnOff(int guiIndex) {

	deviceFunctions[guiIndex].setVisible(true);
	deviceFunctions[guiIndex].GUI_SetTextType("Switch");
	deviceFunctions[guiIndex].GUI_SetIcon(DeviceButton::GENERIC_ON_OFF);
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::GENERIC_ON_OFF);
	deviceFunctions[guiIndex].setButtonAction(buttonClickCallback, (int) DeviceButton::GENERIC_ON_OFF);

}

void NodeProperScreenView::setupGenericLevel(int guiIndex) {

	deviceFunctions[guiIndex].setVisible(true);
	deviceFunctions[guiIndex].GUI_SetTextType("Level");
	deviceFunctions[guiIndex].GUI_SetIcon(DeviceButton::GENERIC_LEVEL);
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::GENERIC_LEVEL);
	deviceFunctions[guiIndex].setButtonAction(buttonClickCallback, (int) DeviceButton::GENERIC_LEVEL);

}

void NodeProperScreenView::setupSensor(int guiIndex) {

	deviceFunctions[guiIndex].setVisible(true);
	deviceFunctions[guiIndex].GUI_SetTextType("Sensor");
	deviceFunctions[guiIndex].GUI_SetIcon(DeviceButton::SENSOR);
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_ON, DeviceButton::SENSOR);
	deviceFunctions[guiIndex].setButtonAction(buttonClickCallback, (int) DeviceButton::SENSOR);

}

void NodeProperScreenView::setupLightLightness(int guiIndex) {

	deviceFunctions[guiIndex].setVisible(true);
	deviceFunctions[guiIndex].GUI_SetTextType("Light");
	deviceFunctions[guiIndex].GUI_SetIcon(DeviceButton::LIGHT_LIGHTNESS);
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::LIGHT_LIGHTNESS);
	deviceFunctions[guiIndex].setButtonAction(buttonClickCallback, (int) DeviceButton::LIGHT_LIGHTNESS);

}

void NodeProperScreenView::setupGenericPower(int guiIndex) {

	deviceFunctions[guiIndex].setVisible(true);
	deviceFunctions[guiIndex].GUI_SetTextType("Power");
	deviceFunctions[guiIndex].GUI_SetIcon(DeviceButton::GENERIC_POWER);
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::GENERIC_POWER);
	deviceFunctions[guiIndex].setButtonAction(buttonClickCallback, (int) DeviceButton::GENERIC_POWER);

}

void NodeProperScreenView::setupConfig(int guiIndex) {

	deviceFunctions[guiIndex].setVisible(true);
	deviceFunctions[guiIndex].GUI_SetTextType("Configure");
	deviceFunctions[guiIndex].GUI_SetTextStatus(node->nodeName);
	deviceFunctions[guiIndex].GUI_SetIcon(DeviceButton::CONFIGURE);
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::CONFIGURE);
	deviceFunctions[guiIndex].setButtonAction(buttonClickCallback, (int) DeviceButton::CONFIGURE);

}

void NodeProperScreenView::setupUnprov(int guiIndex) {

	deviceFunctions[guiIndex].setVisible(true);
	deviceFunctions[guiIndex].GUI_SetTextType("Delete");
	deviceFunctions[guiIndex].GUI_SetTextStatus(node->nodeName);
	deviceFunctions[guiIndex].GUI_SetIcon(DeviceButton::UNPROVISION);
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::UNPROVISION);
	deviceFunctions[guiIndex].setButtonAction(buttonClickCallback, (int) DeviceButton::UNPROVISION);

}

void NodeProperScreenView::handleButtonClicked(int instanceID) {

	// instanceID will be used to tell which button was pressed
	// sensor will be polled every minute by using tick counter (similar as the clock on main view)
	switch ((DeviceButton::ICON_TYPE) instanceID) {
		case DeviceButton::GENERIC_ON_OFF:
			handleGenericOnOffClicked(instanceID);
			break;
		case DeviceButton::GENERIC_LEVEL:
			handleGenericLevelClicked(instanceID);
			break;
		case DeviceButton::LIGHT_LIGHTNESS:
			handleLightClicked(instanceID);
			break;
		case DeviceButton::GENERIC_POWER:
			handleGenericPowerClicked(instanceID);
			break;
		case DeviceButton::SENSOR:
			break;
		case DeviceButton::CONFIGURE:
			handleConfigureClicked(instanceID);
			break;
		case DeviceButton::UNPROVISION:
			handleDeleteClicked(instanceID);
			break;
		default:
			break;
	}

}

void NodeProperScreenView::handleGenericOnOffClicked(int instanceID) {

	PARAMETER_TYPE types[] = {PARAM_CHAR, PARAM_CHAR};
	void *paramValues[2];
	char stringState[3];
	char stringNodeAddress[10];
	int guiIndex = getBtnIndexFromInstaceID(instanceID);

	DeviceButton::STATE newState = deviceFunctions[guiIndex].GUI_GetButtonState();
	if (newState) {
		newState = DeviceButton::BUTTON_OFF;
	} else {
		newState = DeviceButton::BUTTON_ON;
	}
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(newState, DeviceButton::GENERIC_ON_OFF);
	sprintf(stringNodeAddress, "%04X", (unsigned int) this->node->address.nodeAddress);
	sprintf(stringState, "%02d", (int) newState);
	paramValues[0] = (void *) stringNodeAddress;
	paramValues[1] = (void *) stringState;
	this->cmd = CMD_CreateCommandGet(CMD_MESH_ATCL_GENERIC_ON_OFF_ACK,
									types,
									paramValues,
									2,
									NULL,
									NULL);
	presenter->GUI_SendCommand(this->cmd);

}

void NodeProperScreenView::handleGenericLevelClicked(int instanceID) {}

void NodeProperScreenView::handleSensor(int instanceID) {}

void NodeProperScreenView::handleLightClicked(int instanceID) {}

void NodeProperScreenView::handleGenericPowerClicked(int instanceID) {}

void NodeProperScreenView::handleConfigureClicked(int instanceID) {

	presenter->GUI_SetAllGroupAddress(NC_GetAllGroupAddresses());
	presenter->GUI_SetDeviceToConfigure(NC_GetConfigNodeFromAddress(this->node->address.nodeAddress));
	goToConfigureScreen();

}

void NodeProperScreenView::handleDeleteClicked(int instanceID) {

	PARAMETER_TYPE type = PARAM_INT;
	void *paramValue[] = {(void *) &this->node->address.nodeAddress};

	this->cmd = CMD_CreateCommandGet(CMD_MESH_ATCL_UNPROV,
									&type,
									paramValue,
									1,
									NULL,
									NULL);
	loaderModal.show();
	loader1.GUI_SetLoaderText("Deleting node");
	loader1.GUI_SetDisable(false);
	presenter->GUI_SendCommand(this->cmd);

}

int NodeProperScreenView::getBtnIndexFromInstaceID(int instanceID) {

	for (int i = 0; i<maxDeviceFunctions; i++) {
		if (deviceFunctions[i].GUI_GetInstanceID() == instanceID) {
			return i;
		}
	}

	return 0;

}

void NodeProperScreenView::changeScreenAfterUnprov() {

	loaderModal.hide();
	loader1.GUI_SetDisable(true);
	loader1.GUI_ResetProgressValue();
	goToScreen1();

}

#include <gui/nodeproperscreen_screen/NodeProperScreenView.hpp>
#include "command.h"

NodeProperScreenView::NodeProperScreenView() : buttonClickCallback(this, &NodeProperScreenView::handleButtonClicked)
{

}

void NodeProperScreenView::setupScreen()
{
    NodeProperScreenViewBase::setupScreen();
    this->node = NC_GetConfigNodeFromAddress(presenter->GUI_GetNodeAddress());
    this->nodeModel = node->address.nodeModels;
    this->allModels = NC_GetAllModels();

    loaderModal.hide();
    loader1.GUI_SetDisable(true);
    screenTitleBar1.GUI_SetTextTitleBar("Device", node->nodeName);
    if (this->nodeModel & NC_SENSOR_MODEL && this->node->sensors.sensorArray == NULL) {
    	checkForSensors();
    } else {
    	setupDeviceButtons();
    }

}

void NodeProperScreenView::tearDownScreen()
{
    NodeProperScreenViewBase::tearDownScreen();
}

void NodeProperScreenView::handleTickEvent() {

	loader1.GUI_ProgressLoader();

}

void NodeProperScreenView::setupGenericOnOff(int *guiIndex, int numToCreate) {

	for (int i = 0; i<numToCreate; i++) {
		deviceFunctions[*guiIndex].setVisible(true);
		deviceFunctions[*guiIndex].GUI_SetTextType("Switch");
		deviceFunctions[*guiIndex].GUI_SetIcon(DeviceButton::GENERIC_ON_OFF);
		deviceFunctions[*guiIndex].GUI_ButtonSetOffState((DeviceButton::STATE) this->node->states.genericOnOffStatus, DeviceButton::GENERIC_ON_OFF);
		deviceFunctions[*guiIndex].setButtonAction(buttonClickCallback, generateInstanceID(i, DeviceButton::GENERIC_ON_OFF));
		(*guiIndex)++;
	}

}

void NodeProperScreenView::setupGenericLevel(int *guiIndex, int numToCreate) {

	for (int i = 0; i<numToCreate; i++) {
		deviceFunctions[*guiIndex].setVisible(true);
		deviceFunctions[*guiIndex].GUI_SetTextType("Level");
		deviceFunctions[*guiIndex].GUI_SetIcon(DeviceButton::GENERIC_LEVEL);
		deviceFunctions[*guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::GENERIC_LEVEL);
		deviceFunctions[*guiIndex].setButtonAction(buttonClickCallback, generateInstanceID(i, DeviceButton::GENERIC_LEVEL));
		(*guiIndex)++;
	}

}

void NodeProperScreenView::setupSensor(int *guiIndex, int numToCreate) {

	for (int i = 0; i<numToCreate; i++) {
		deviceFunctions[*guiIndex].setVisible(true);
		deviceFunctions[*guiIndex].GUI_SetTextType("Sensor");
		deviceFunctions[*guiIndex].GUI_SetTextStatus(this->node->sensors.sensorArray[i].name);
		deviceFunctions[*guiIndex].GUI_SetIcon(DeviceButton::SENSOR);
		deviceFunctions[*guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::SENSOR);
		deviceFunctions[*guiIndex].setButtonAction(buttonClickCallback, generateInstanceID(i, DeviceButton::SENSOR));
		(*guiIndex)++;
	}

}

void NodeProperScreenView::setupLightLightness(int *guiIndex, int numToCreate) {

	for (int i = 0; i<numToCreate; i++) {
		deviceFunctions[*guiIndex].setVisible(true);
		deviceFunctions[*guiIndex].GUI_SetTextType("Light");
		deviceFunctions[*guiIndex].GUI_SetIcon(DeviceButton::LIGHT_LIGHTNESS);
		deviceFunctions[*guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::LIGHT_LIGHTNESS);
		deviceFunctions[*guiIndex].setButtonAction(buttonClickCallback, generateInstanceID(i, DeviceButton::LIGHT_LIGHTNESS));
		(*guiIndex)++;
	}

}

void NodeProperScreenView::setupGenericPower(int *guiIndex, int numToCreate) {

	for (int i = 0; i<numToCreate; i++) {
		deviceFunctions[*guiIndex].setVisible(true);
		deviceFunctions[*guiIndex].GUI_SetTextType("Power");
		deviceFunctions[*guiIndex].GUI_SetIcon(DeviceButton::GENERIC_POWER);
		deviceFunctions[*guiIndex].GUI_ButtonSetOffState((DeviceButton::STATE) this->node->states.genericPowerOnOffStatus, DeviceButton::GENERIC_POWER);
		deviceFunctions[*guiIndex].setButtonAction(buttonClickCallback, generateInstanceID(i, DeviceButton::GENERIC_POWER));
		(*guiIndex)++;
	}

}

void NodeProperScreenView::setupConfig(int *guiIndex) {

	deviceFunctions[*guiIndex].setVisible(true);
	deviceFunctions[*guiIndex].GUI_SetTextType("Configure");
	deviceFunctions[*guiIndex].GUI_SetTextStatus(node->nodeName);
	deviceFunctions[*guiIndex].GUI_SetIcon(DeviceButton::CONFIGURE);
	deviceFunctions[*guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::CONFIGURE);
	deviceFunctions[*guiIndex].setButtonAction(buttonClickCallback, generateInstanceID(0, DeviceButton::CONFIGURE));
	(*guiIndex)++;

}

void NodeProperScreenView::setupUnprov(int *guiIndex) {

	deviceFunctions[*guiIndex].setVisible(true);
	deviceFunctions[*guiIndex].GUI_SetTextType("Delete");
	deviceFunctions[*guiIndex].GUI_SetTextStatus(node->nodeName);
	deviceFunctions[*guiIndex].GUI_SetIcon(DeviceButton::UNPROVISION);
	deviceFunctions[*guiIndex].GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::UNPROVISION);
	deviceFunctions[*guiIndex].setButtonAction(buttonClickCallback, generateInstanceID(0, DeviceButton::UNPROVISION));
	(*guiIndex)++;

}

void NodeProperScreenView::handleButtonClicked(int instanceID) {

	// instanceID will be used to tell which button was pressed
	// sensor will be polled every minute by using tick counter (similar as the clock on main view)
	switch ((DeviceButton::ICON_TYPE) (instanceID / MULTIPLIER)) {
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
			handleSensor(instanceID);
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
	newState = (newState == DeviceButton::STATE::BUTTON_OFF) ? DeviceButton::STATE::BUTTON_ON : DeviceButton::STATE::BUTTON_OFF;
	this->node->states.genericOnOffStatus = (int) newState;
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

void NodeProperScreenView::handleSensor(int instanceID) {

	// % 10 will give us the first number of instanceID
	// which is coincidentally also the correct index of the sensor
	presenter->GUI_SetSensorIndex(instanceID % 10);
	goToNodeSensorsScreen();

}

void NodeProperScreenView::handleLightClicked(int instanceID) {}

void NodeProperScreenView::handleGenericPowerClicked(int instanceID) {

	int guiIndex = getBtnIndexFromInstaceID(instanceID);

	DeviceButton::STATE newState = deviceFunctions[guiIndex].GUI_GetButtonState();
	newState = (newState == DeviceButton::STATE::BUTTON_OFF) ? DeviceButton::STATE::BUTTON_ON : DeviceButton::STATE::BUTTON_OFF;
	this->node->states.genericPowerOnOffStatus = (int) newState;
	deviceFunctions[guiIndex].GUI_ButtonSetOffState(newState, DeviceButton::GENERIC_POWER);

}

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

int NodeProperScreenView::generateInstanceID(int guiIndex, DeviceButton::ICON_TYPE type) {

	return (int) type * MULTIPLIER + guiIndex;

}

void NodeProperScreenView::checkForSensors() {

	char stringNodeAddress[10];
	PARAMETER_TYPE type = PARAM_CHAR;
	void *paramValue[1];

	loaderModal.show();
	loader1.GUI_SetLoaderText("Looking for sensors");
	loader1.GUI_SetDisable(false);
	sprintf(stringNodeAddress, "%04X", (unsigned int) this->node->address.nodeAddress);
	paramValue[0] = (void *) stringNodeAddress;
	this->cmd = CMD_CreateCommandGet(CMD_MESH_ATCL_SENSOR_DESC_GET,
									&type,
									paramValue,
									1,
									NULL,
									NULL);
	presenter->GUI_SendCommand(this->cmd);

}

void NodeProperScreenView::setupDeviceButtons() {

	int yPos = 0;
	int xPos = 10;
	int guiIndex = 0;

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
		setupGenericOnOff(&guiIndex, 1);
	}
	if (this->nodeModel & NC_GENERIC_LEVEL_MODEL) {
		setupGenericLevel(&guiIndex, 1);
	}
	if (this->nodeModel & NC_SENSOR_MODEL) {
		setupSensor(&guiIndex, this->node->sensors.numOfSensors);
	}
	if (this->nodeModel & NC_LIGH_LIGHTNESS_MODEL) {
		setupLightLightness(&guiIndex, 1);
	}
	if (this->nodeModel & NC_GENERIC_POWER_ON_OFF_MODEL) {
		setupGenericPower(&guiIndex, 1);
	}
	setupConfig(&guiIndex);
	setupUnprov(&guiIndex);

}

void NodeProperScreenView::GUI_ConfirmDescGet() {

	loaderModal.hide();
	loader1.GUI_SetDisable(true);
	loader1.GUI_ResetProgressValue();
	setupDeviceButtons();

}

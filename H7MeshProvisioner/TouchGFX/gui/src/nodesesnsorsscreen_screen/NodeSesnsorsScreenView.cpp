#include <gui/nodesesnsorsscreen_screen/NodeSesnsorsScreenView.hpp>

NodeSesnsorsScreenView::NodeSesnsorsScreenView()
{

}

void NodeSesnsorsScreenView::setupScreen()
{
    NodeSesnsorsScreenViewBase::setupScreen();
    this->sensorIndex = presenter->GUI_GetSensorIndex();
    this->node = NC_GetConfigNodeFromAddress(presenter->GUI_GetNodeAddress());
    this->sensor = &this->node->sensors.sensorArray[this->sensorIndex];

    loaderModal.hide();
    loader1.GUI_SetDisable(true);
    screenTitleBar1.GUI_SetTextTitleBar("Sensor", this->sensor->name);

    GUI_SetupSensorButtons();
    GUI_RequestNewMeasurement();

}

void NodeSesnsorsScreenView::tearDownScreen()
{
    NodeSesnsorsScreenViewBase::tearDownScreen();
}

void NodeSesnsorsScreenView::GUI_RequestNewMeasurement() {

	PARAMETER_TYPE types[] = {PARAM_CHAR, PARAM_CHAR};
	void *paramValues[2];
	char stringPID[5];
	char stringNodeAddress[10];

	if (this->isTakingMeasurement) {
		return;
	}

	this->isTakingMeasurement = 1;
	loaderModal.show();
	loader1.GUI_SetDisable(false);
	loader1.GUI_SetLoaderText("Taking measurement");
	this->sensorDigitalSeconds = 0;
	sprintf(stringNodeAddress, "%04X", (unsigned int) this->node->address.nodeAddress);
	sprintf(stringPID, "%04X", this->sensor->PID);
	paramValues[0] = (void *) stringNodeAddress;
	paramValues[1] = (void *) stringPID;
	this->cmd = CMD_CreateCommandGet(CMD_MESH_ATCL_SENSOR_GET,
									types,
									paramValues,
									2,
									NULL,
									NULL);
	presenter->GUI_SendCommand(this->cmd);

}

void NodeSesnsorsScreenView::handleTickEvent() {

	this->sensorTickCounter++;
	loader1.GUI_ProgressLoader();
	if (this->sensorTickCounter % 60 == 0) {
		if (++this->sensorDigitalSeconds >= 60) {
			// a minute has passed; refresh sensor data
			GUI_RequestNewMeasurement();
		}
	}

}

void NodeSesnsorsScreenView::GUI_SetupSensorButtons() {

	int yPos = 0;
	int xPos = 10;

	this->buttonCount = this->sensor->UIcount <= maxButtons ? this->sensor->UIcount : maxButtons;
	for (int i = 0; i<this->buttonCount; i++) {
		sensorButtons[i].initialize();
		sensorButtons[i].setPosition(xPos, yPos, 112, 112);
		sensorButtons[i].setVisible(true);
		sensorButtons[i].GUI_SetTextType(this->sensor->buttons[i].name);
		sensorButtons[i].GUI_SetIcon(DeviceButton::SENSOR);
		sensorButtons[i].GUI_ButtonSetOffState(DeviceButton::BUTTON_ON, DeviceButton::SENSOR);
		allNodeSensors.add(sensorButtons[i]);
		if (xPos >= 358) {
			yPos += (112 + 10);
			xPos = 10;
		} else {
			xPos += (112 + 62);
		}

	}

}

void NodeSesnsorsScreenView::GUI_UpdateDeviceButtonValues() {

	switch (this->mea->PID) {
		case PRESENT_AMBIENT_TEMPERATURE_PID:
			{
				APC1_SelectedData_t *apcData = (APC1_SelectedData_t *) this->mea->measuredData;
				sensorButtons[APC1_BUTTON_PM1_0_INDEX].GUI_SetIntigerStatus(apcData->pm1_0);
				sensorButtons[APC1_BUTTON_PM2_5_INDEX].GUI_SetIntigerStatus(apcData->pm2_5);
				sensorButtons[APC1_BUTTON_PM10_INDEX].GUI_SetIntigerStatus(apcData->pm10);
				sensorButtons[APC1_BUTTON_PM_1_0_AIR_INDEX].GUI_SetIntigerStatus(apcData->pm1_0_air);
				sensorButtons[APC1_BUTTON_PM_2_5_AIR_INDEX].GUI_SetIntigerStatus(apcData->pm2_5_air);
				sensorButtons[APC1_BUTTON_PM_10_AIR_INDEX].GUI_SetIntigerStatus(apcData->pm10_air);
				sensorButtons[APC1_BUTTON_PARTICLES_0_3_INDEX].GUI_SetIntigerStatus(apcData->particles_0_3);
				sensorButtons[APC1_BUTTON_PARTICLES_0_5_INDEX].GUI_SetIntigerStatus(apcData->particles_0_5);
				sensorButtons[APC1_BUTTON_PARTICLES_1_0_INDEX].GUI_SetIntigerStatus(apcData->particles_1_0);
				sensorButtons[APC1_BUTTON_PARTICLES_2_5_INDEX].GUI_SetIntigerStatus(apcData->particles_2_5);
				sensorButtons[APC1_BUTTON_PARTICLES_5_0_INDEX].GUI_SetIntigerStatus(apcData->particles_5_0);
				sensorButtons[APC1_BUTTON_PARTICLES_10_INDEX].GUI_SetIntigerStatus(apcData->particles_10);
				sensorButtons[APC1_BUTTON_TVOC_INDEX].GUI_SetIntigerStatus(apcData->TVOC);
				sensorButtons[APC1_BUTTON_EC02_INDEX].GUI_SetIntigerStatus(apcData->eCO2);
				sensorButtons[APC1_BUTTON_RH_COMP_INDEX].GUI_SetDoubleStatus(apcData->rh_comp);
				sensorButtons[APC1_BUTTON_TEMPERATURE_INDEX].GUI_SetDoubleStatus(apcData->tComp);
				sensorButtons[APC1_BUTTON_AQI_INDEX].GUI_SetTextStatus(apcData->aqiString);
			}
			break;
		default:
			break;
	}

}

void NodeSesnsorsScreenView::GUI_UpdateSensorValues() {

	loaderModal.hide();
	loader1.GUI_SetDisable(true);
	loader1.GUI_ResetProgressValue();

	this->isTakingMeasurement = 0;
	this->mea = presenter->GUI_GetSensorMeasuredData();
	GUI_UpdateDeviceButtonValues();

	SN_FreeMeasuredSensorData(this->mea);

}

void NodeSesnsorsScreenView::GUI_ErrorOccured() {

	goToErrorScreen();

}

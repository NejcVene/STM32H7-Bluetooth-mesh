#include <gui/configurenodescreen_screen/ConfigureNodeScreenView.hpp>
#include "node_config.h"
#include "command.h"

Node_SubscriptionParam_t ConfigureNodeScreenView::toSubb;
CMD_CommandGet_t *ConfigureNodeScreenView::cmd = nullptr;
// NC_MaskedFeatures *ConfigureNodeScreenView::allGroupAddress = nullptr;

ConfigureNodeScreenView::ConfigureNodeScreenView() : buttonClickCallback(this, &ConfigureNodeScreenView::handleButtonClicked)
{

}

void ConfigureNodeScreenView::setupScreen()
{
    ConfigureNodeScreenViewBase::setupScreen();
    GUI_CongPopup.setVisible(false);
    this->configNode = presenter->GUI_GetDeviceToConfigureFromModel();
    this->allGroupAddress = presenter->GUI_GetAllMaskedModels();
    touchgfx::Unicode::strncpy(nodeUUIDBuffer, this->configNode->address.uuid, NODEUUID_SIZE);
    touchgfx::Unicode::strncpy(nodeNameBuffer, "New node", NODENAME_SIZE);
    touchgfx::Unicode::strncpy(nodeVendorBuffer, "ST", NODEVENDOR_SIZE);
    nodeName.invalidateContent();
    nodeUUID.invalidateContent();
	nodeVendor.invalidateContent();
    for (int i = 0; this->allGroupAddress[i].bitmask != 0; i++) {
    	nodeSubscriptions[i].initialize();
    	nodeSubscriptions[i].setPosition(10, 10 + (i * 70), 310, 60);
    	nodeSubscriptions[i].setTextAddressName(allGroupAddress[i].name);
    	nodeSubscriptions[i].setButtonAction(buttonClickCallback, allGroupAddress[i].value);
    	nodeSubscriptions[i].setVisible(true);
    	allNodeSubsCont.add(nodeSubscriptions[i]);
    }
    allNodeSubsCont.invalidateContent();
    this->toSubb.nodeAddress = this->configNode->address.nodeAddress;
    this->toSubb.numOfSubs = 0;
}

void ConfigureNodeScreenView::tearDownScreen()
{
    ConfigureNodeScreenViewBase::tearDownScreen();
}

void ConfigureNodeScreenView::handleButtonClicked(int instanceID) {

	if (this->toSubb.numOfSubs < 5) {
		this->toSubb.subbedAddresses[this->toSubb.numOfSubs++] = instanceID;
	}

}

void ConfigureNodeScreenView::GUI_SaveConfNode() {

	PARAMETER_TYPE type = PARAM_VOID;
	void *paramValue[] = {(void *) &this->toSubb};
	int arrayLength[] = {1};
	size_t elementSize[] = {sizeof(Node_SubscriptionParam_t)};
	this->cmd = CMD_CreateCommandGet(CMD_FUN_SUBS_ADD,
									&type,
									paramValue,
									1,
									arrayLength,
									elementSize);

}

void ConfigureNodeScreenView::GUI_ConfBtnClicked() {

	GUI_CongPopup.setVisible(false);
	goToHomeScreen();

}

void ConfigureNodeScreenView::GUI_CancelBtnClicked() {

	goToHomeScreen();

}

void ConfigureNodeScreenView::GUI_ShowPopup() {

	GUI_CongPopup.setVisible(true);

}

void ConfigureNodeScreenView::GUI_SetDeviceToConfigure(Node_Config_t *configureDevice) {

	;

}

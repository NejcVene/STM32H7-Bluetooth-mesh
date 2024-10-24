#include <gui/configurenodescreen_screen/ConfigureNodeScreenView.hpp>
#include "node_config.h"
#include "command.h"

Node_SubscriptionParam_t ConfigureNodeScreenView::toSubb;
CMD_CommandGet_t *ConfigureNodeScreenView::cmd = nullptr;

int nay66;

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
    setTextNodeUUID(this->configNode->address.uuid);
    setTextNodeName("New node");
    setTextVendor("ST micro");
    for (int i = 0; this->allGroupAddress[i].bitmask != 0; i++) {
    	nodeSubscriptions[i].initialize();
    	nodeSubscriptions[i].setPosition(10, 10 + (i * 70), 310, 60);
    	nodeSubscriptions[i].setTextAddressName(allGroupAddress[i].name);
    	nodeSubscriptions[i].setButtonAction(buttonClickCallback, allGroupAddress[i].value);
    	if (i == 0) {
    		nodeSubscriptions[i].setButtonEnable(false);
    	} else {
    		nodeSubscriptions[i].setButtonEnable(true);
    	}
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

	for (int i = 0; i<maxSubsPerNode; i++) {
		if (nodeSubscriptions[i].getInstanceID() == instanceID) {
			// check if button is pressed or not
			if (nodeSubscriptions[i].isButtonOn()) {
				// if the button is now on, the user wishes to subscribe
				if (this->toSubb.numOfSubs < 5) {
					this->toSubb.numOfSubs++;
					for (int j = 0; j<maxSubsPerNode; j++) {
						if (this->toSubb.subbedAddresses[j] == 0) {
							this->toSubb.subbedAddresses[j] = instanceID;
							break;
						}
					}
				}
			} else {
				// if the button is now off, the user wishes to un-subscribe
				if (this->toSubb.numOfSubs > 0) {
					for (int j = 0; j<maxSubsPerNode; j++) {
						if (this->toSubb.subbedAddresses[j] == (uint32_t) instanceID) {
							this->toSubb.subbedAddresses[j] = 0;
							this->toSubb.numOfSubs--;
							break;
						}
					}
				}
			}
		}
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

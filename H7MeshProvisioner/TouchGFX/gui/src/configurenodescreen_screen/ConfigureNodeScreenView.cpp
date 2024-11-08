#include <gui/configurenodescreen_screen/ConfigureNodeScreenView.hpp>
#include "node_config.h"
#include "command.h"

//Node_SubscriptionParam_t ConfigureNodeScreenView::toSubb;
CMD_CommandGet_t *ConfigureNodeScreenView::cmd = nullptr;

// NC_MaskedFeatures *ConfigureNodeScreenView::allGroupAddress = nullptr;

ConfigureNodeScreenView::ConfigureNodeScreenView() : buttonClickCallback(this, &ConfigureNodeScreenView::handleButtonClicked)
{

}

void ConfigureNodeScreenView::setupScreen()
{
    ConfigureNodeScreenViewBase::setupScreen();
    GUI_CongPopup.hide();
    this->setupEnd = 0;
    this->configNode = presenter->GUI_GetDeviceToConfigureFromModel();
    this->allGroupAddress = presenter->GUI_GetAllMaskedModels();
    setTextNodeUUID(this->configNode->address.uuid);
    setTextNodeName(this->configNode->nodeName);
    setTextVendor("ST micro");
    for (int i = 0; this->allGroupAddress[i].bitmask != 0; i++) {
    	nodeSubscriptions[i].initialize();
    	nodeSubscriptions[i].setPosition(10, 10 + (i * 70), 310, 60);
    	nodeSubscriptions[i].setTextAddressName(allGroupAddress[i].name);
    	nodeSubscriptions[i].setButtonAction(buttonClickCallback, allGroupAddress[i].bitmask);
    	if (i == 0) {
    		nodeSubscriptions[i].setButtonEnable(false);
    	} else {
    		nodeSubscriptions[i].setButtonEnable(true);
			if (this->configNode->subscriptions & allGroupAddress[i].bitmask) {
				nodeSubscriptions[i].setButtonState(false);
			}
    	}
    	nodeSubscriptions[i].setVisible(true);
    	allNodeSubsCont.add(nodeSubscriptions[i]);
    }
    allNodeSubsCont.invalidateContent();
    this->toSubb.nodeAddress = this->configNode->address.nodeAddress;
    this->toSubb.numOfSubs = 0;
    this->setupEnd = 1;
}

void ConfigureNodeScreenView::tearDownScreen()
{
    ConfigureNodeScreenViewBase::tearDownScreen();
}

/*
 * This function is a bit odd, as it has to handle these cases:
 * 	1. Not subscribed, but want to subscribe -> add to list
	2. Subscribed, but want to un-subscribe -> add to list
	3. Not subscribed, but accidentally pushed to subscribe, so after on it goes back to off -> remove form list
	4. Subscribed, but accidentally pushed to un-subscribe, so after off it goes back to on -> remove from list
 	Will be cleaned up at a later date
 */
void ConfigureNodeScreenView::handleButtonClicked(int instanceID) {

	if (setupEnd) {
		for (int i = 0; i<maxSubsPerNode; i++) {
			if (nodeSubscriptions[i].getInstanceID() == instanceID) {
				// check if button is pressed or not
				if (!nodeSubscriptions[i].isButtonOn()) {
					// if the button is now on
					if (this->toSubb.numOfSubs < 5) {
						for (int j = 0; j<maxSubsPerNode; j++) {
							if (instanceID & this->configNode->subscriptions) {
								if (this->toSubb.subbedAddresses[j].groupAddress == (uint32_t) instanceID) {
									// accidental subscribe
									this->toSubb.numOfSubs--;
									this->toSubb.subbedAddresses[j].groupAddress = 0;
									this->toSubb.subbedAddresses[j].subbTo = 0;
									return;
								}
							} else if (this->toSubb.subbedAddresses[j].groupAddress == 0) {
								// the user wishes to subscribe
								this->toSubb.numOfSubs++;
								this->toSubb.subbedAddresses[j].groupAddress = instanceID;
								this->toSubb.subbedAddresses[j].subbTo = 1;
								return;
							}
						}
					}
				} else {
					// if the button is now off
					if (this->toSubb.numOfSubs >= 0) {
						for (int j = 0; j<maxSubsPerNode; j++) {
							if (instanceID & this->configNode->subscriptions) {
								if (this->toSubb.subbedAddresses[j].groupAddress == 0) {
									// the user wishes to un-subscribe
									this->toSubb.numOfSubs++;
									this->toSubb.subbedAddresses[j].groupAddress = instanceID;
									this->toSubb.subbedAddresses[j].subbTo = 0;
									return;
								}
							} else if (this->toSubb.subbedAddresses[j].groupAddress == (uint32_t) instanceID) {
								// accidental un-subscribe
								this->toSubb.numOfSubs--;
								this->toSubb.subbedAddresses[j].groupAddress = 0;
								this->toSubb.subbedAddresses[j].subbTo = 0;
								return;
							}
						}
					}
				}
			}
		}
	}

}

void ConfigureNodeScreenView::GUI_SaveConfNode() {

	PARAMETER_TYPE types[] = {PARAM_VOID, PARAM_INT};
	void *paramValue[] = {(void *) &this->toSubb, (void *) &this->configNode->address.nodeAddress};
	int arrayLength[] = {1};
	size_t elementSize[] = {sizeof(Node_SubscriptionParam_t)};
	this->cmd = CMD_CreateCommandGet(CMD_FUN_PUB_SET_SUB_ADD,
									types,
									paramValue,
									2,
									arrayLength,
									elementSize);
	presenter->GUI_SendCommand(this->cmd);

}

void ConfigureNodeScreenView::GUI_ConfBtnClicked() {

	GUI_CongPopup.hide();
	goToHomeScreen();

}

void ConfigureNodeScreenView::GUI_CancelBtnClicked() {

	goToHomeScreen();

}

void ConfigureNodeScreenView::GUI_ShowPopup() {

	GUI_CongPopup.show();

}

void ConfigureNodeScreenView::GUI_SetDeviceToConfigure(Node_Config_t *configureDevice) {

	;

}

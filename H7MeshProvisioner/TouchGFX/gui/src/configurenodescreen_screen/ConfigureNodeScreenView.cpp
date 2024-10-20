#include <gui/configurenodescreen_screen/ConfigureNodeScreenView.hpp>
#include "node_config.h"
#include "command.h"

int nay4 = 0;

ConfigureNodeScreenView::ConfigureNodeScreenView() : buttonClickCallback(this, &ConfigureNodeScreenView::handleButtonClicked)
{

}

void ConfigureNodeScreenView::setupScreen()
{
    ConfigureNodeScreenViewBase::setupScreen();
    this->configNode = presenter->GUI_GetDeviceToConfigureFromModel();
    touchgfx::Unicode::strncpy(nodeUUIDBuffer, this->configNode->uuid, NODEUUID_SIZE);
    touchgfx::Unicode::strncpy(nodeNameBuffer, "New node", NODENAME_SIZE);
    touchgfx::Unicode::strncpy(nodeVendorBuffer, "ST", NODEVENDOR_SIZE);
    nodeName.invalidateContent();
    nodeUUID.invalidateContent();
	nodeVendor.invalidateContent();
    for (int i = 0; i<maxSubsPerNode; i++) {
    	nodeSubscriptions[i].initialize();
    	nodeSubscriptions[i].setPosition(10, 10 + (i * 70), 310, 60);
    	nodeSubscriptions[i].setTextAddressName(groupAddress[i].addressName);
    	nodeSubscriptions[i].setButtonAction(buttonClickCallback, groupAddress[i].address);
    	nodeSubscriptions[i].setVisible(true);
    	allNodeSubsCont.add(nodeSubscriptions[i]);
    }
    allNodeSubsCont.invalidateContent();
}

void ConfigureNodeScreenView::tearDownScreen()
{
    ConfigureNodeScreenViewBase::tearDownScreen();
}

void ConfigureNodeScreenView::handleButtonClicked(int instanceID) {



}

void ConfigureNodeScreenView::GUI_SetDeviceToConfigure(Node_NetworkAddress_t *configureDevice) {

	nay4 = 1;

}

#include <gui/nodesscreen_screen/NodesScreenView.hpp>

NodesScreenView::NodesScreenView() : buttonClickCallback(this, &NodesScreenView::handleButtonClicked)
{

}

void NodesScreenView::setupScreen()
{
    NodesScreenViewBase::setupScreen();
    int hasDevices = 0;
    this->roomName = presenter->GUI_GetRoomName();
    this->roomBitmask = presenter->GUI_GetRoomBitmask();
    this->nodes = NC_GetNodeConfigArray();
    screenTitleBar1.GUI_SetTextTitleBar("Room", this->roomName);
    for (int i = 0; i<this->maxDevices; i++) {
    	if (roomBitmask & this->nodes[i].subscriptions) {
    		// roomNodesContainer
    		hasDevices = 1;
    		this->devices[i].initialize();
    		this->devices[i].setPosition(0, 70 * i, 460, 60);
    		this->devices[i].setButtonAction(buttonClickCallback, this->nodes[i].address.nodeAddress);
    		this->devices[i].GUI_SetTextDeviceName(this->nodes[i].nodeName);
    		this->devices[i].setVisible(true);
    		roomNodesContainer.add(this->devices[i]);
    	}
    }
    if (!hasDevices) {
    	noNodesInRoom.setVisible(true);
    }


}

void NodesScreenView::tearDownScreen()
{
    NodesScreenViewBase::tearDownScreen();
}

void NodesScreenView::handleButtonClicked(int instanceID) {

	presenter->GUI_SaveNodeAddress(instanceID);
	goToNodeProperScreen();

}

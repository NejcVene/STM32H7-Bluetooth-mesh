#include <gui/nodeproperscreen_screen/NodeProperScreenView.hpp>

NodeProperScreenView::NodeProperScreenView() : buttonClickCallback(this, &NodeProperScreenView::handleButtonClicked)
{

}

void NodeProperScreenView::setupScreen()
{
    NodeProperScreenViewBase::setupScreen();
    this->node = NC_GetConfigNodeFromAddress(presenter->GUI_GetNodeAddress());
    this->nodeModel = node->address.nodeModels;

    GUI_SetTextNodeName(node->nodeName);
    if (this->nodeModel & NC_GENERIC_ON_OFF_MODEL) {
    	setupGenericOnOff();
    }

}

void NodeProperScreenView::tearDownScreen()
{
    NodeProperScreenViewBase::tearDownScreen();
}

void NodeProperScreenView::setupGenericOnOff() {

	this->genOnOffBtn.initialize();
	this->genOnOffBtn.setPosition(10, 10, 112, 112);
	this->genOnOffBtn.setVisible(true);
	this->genOnOffBtn.GUI_SetTextType("Switch");
	this->genOnOffBtn.GUI_SetIcon(DeviceButton::GENERIC_ON_OFF);
	this->genOnOffBtn.GUI_ButtonSetOffState(DeviceButton::BUTTON_OFF, DeviceButton::GENERIC_ON_OFF);
	this->genOnOffBtn.setButtonAction(buttonClickCallback, 1);
	nodeOptions.add(this->genOnOffBtn);

}

void NodeProperScreenView::handleButtonClicked(int instanceID) {

	int a = instanceID;
	DeviceButton::STATE newState = this->genOnOffBtn.GUI_GetButtonState();
	if (newState) {
		newState = DeviceButton::BUTTON_OFF;
	} else {
		newState = DeviceButton::BUTTON_ON;
	}
	this->genOnOffBtn.GUI_ButtonSetOffState(newState, DeviceButton::GENERIC_ON_OFF);

}

#include <gui/screen1_screen/Screen1View.hpp>
#include "command.h"

Screen1View::Screen1View() : buttonClickCallback(this, &Screen1View::handleButtonClicked)
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    this->nodes = NC_GetNodeConfigArray();
//    deviceToggle1.setButtonAction(buttonClickCallback, 1);
//    deviceToggle2.setButtonAction(buttonClickCallback, 2);
//    deviceToggle1.initialize();
//    deviceToggle2.initialize();
}

void Screen1View::GUI_ShowAllDevices() {

}

void Screen1View::GUI_ShowKitchenDevices() {

	presenter->GUI_SaveRoomBitmask(GROUP_ADDRESS_KITCHEN_BIT);

}

void Screen1View::GUI_ShowLivingRoomDevices() {

	presenter->GUI_SaveRoomBitmask(0);

}

void Screen1View::GUI_ShowBedroomDevices() {

	presenter->GUI_SaveRoomBitmask(0);

}

void Screen1View::GUI_ShowBathroomDevices() {

	presenter->GUI_SaveRoomBitmask(0);

}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}


void Screen1View::handleButtonClicked(int instanceID) {

}


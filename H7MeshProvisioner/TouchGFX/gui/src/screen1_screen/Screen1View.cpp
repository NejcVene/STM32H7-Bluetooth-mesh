#include <gui/screen1_screen/Screen1View.hpp>
#include "command.h"

int nay = 0;

Screen1View::Screen1View() : buttonClickCallback(this, &Screen1View::handleButtonClicked)
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    deviceToggle1.setButtonAction(buttonClickCallback, 1);
    deviceToggle2.setButtonAction(buttonClickCallback, 2);
    deviceToggle1.initialize();
    deviceToggle2.initialize();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::GUI_OnBtn() {

	presenter->GUI_SendCommand(CMD_MESH_ATCL_GENERIC_ON_OFF_ACK);

}

void Screen1View::GUI_OffBtn() {

	presenter->GUI_SendCommand(CMD_MESH_ATCL_GENECI_ON_OFF_ACK_OFF);

}

void Screen1View::handleButtonClicked(int instanceID) {

	nay = instanceID;

}


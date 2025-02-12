#include <gui/settingsscreen_screen/SettingsScreenView.hpp>

SettingsScreenView::SettingsScreenView()
{

}

void SettingsScreenView::setupScreen()
{
    SettingsScreenViewBase::setupScreen();
    screenTitleBar1.GUI_SetTextTitleBar("Menu", "settings");
}

void SettingsScreenView::tearDownScreen()
{
    SettingsScreenViewBase::tearDownScreen();
}

void SettingsScreenView::GUI_GetDevInfo() {

	this->cmd = CMD_CreateCommandGet(CMD_FUN_GET_LIB_VER,
									 NULL,
									 NULL,
									 0,
									 NULL,
									 NULL);
	presenter->GUI_SendCommand(this->cmd);

}

void SettingsScreenView::GUI_ChangeScreen() {

	goToDevInfo();

}

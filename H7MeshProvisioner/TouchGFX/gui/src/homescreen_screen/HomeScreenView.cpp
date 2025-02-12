#include <gui/homescreen_screen/HomeScreenView.hpp>

HomeScreenView::HomeScreenView()
{

}

void HomeScreenView::setupScreen()
{
    HomeScreenViewBase::setupScreen();
    loaderModal.hide();
    loader1.GUI_SetDisable(true);

	screenTitleBar1.GUI_SetTextTitleBar("Menu", "home");
    if (NC_IsDeviceConfigured()) {
		GUI_UpdateDigitalClock();
    } else {
    	GUI_CheckConfig();
    }

}

void HomeScreenView::tearDownScreen()
{
    HomeScreenViewBase::tearDownScreen();
}

void HomeScreenView::handleTickEvent() {

	GUI_UpdateDigitalClock();
	loader1.GUI_ProgressLoader();

}

void HomeScreenView::GUI_UpdateDigitalClock() {

	presenter->GUI_DigitalClock();
	mainDigitalClock.setTime24Hour(presenter->GUI_GetHours(),
								   presenter->GUI_GetMinutes(),
								   presenter->GUI_GetSeconds());

}

void HomeScreenView::GUI_CheckConfig() {

	loader1.GUI_SetDisable(false);
	loader1.GUI_SetLoaderText("Setting things up");
	loaderModal.show();
	// if em. prov. is not provisioned and both H7 and WB55
	// are turned on at about the same time, there is a race
	// condition, by which WB55 wont respond correctly to
	// ATEP ROOT command. Therefore, a delay will be employed
	// to mitigate this. Ideally, we would find a way to send a
	// signal, but I don't have the patience to setup another
	// GPIO line, oh well ...
	HAL_Delay(6000);
	this->cmd = CMD_CreateCommandGet(CMD_FUN_IS_EM_PROV_PROV,
									NULL,
									NULL,
									0,
									NULL,
									NULL);
	presenter->GUI_SendCommand(this->cmd);

}

void HomeScreenView::GUI_NodeIsConfigured() {

	loaderModal.hide();
	loader1.GUI_SetDisable(true);
	loader1.GUI_ResetProgressValue();

}

void HomeScreenView::GUI_ErrorOccured() {

	goToErrorScreen();

}


#include <gui/homescreen_screen/HomeScreenView.hpp>

HomeScreenView::HomeScreenView()
{

}

void HomeScreenView::setupScreen()
{
    HomeScreenViewBase::setupScreen();
    screenTitleBar1.GUI_SetTextTitleBar("Menu", "home");
    GUI_UpdateDigitalClock();
    // mainDigitalClock
//    digitalHours = mainDigitalClock.getCurrentHour();
//    digitalMinutes = mainDigitalClock.getCurrentMinute();
//    digitalSeconds = mainDigitalClock.getCurrentSecond();
}

void HomeScreenView::tearDownScreen()
{
    HomeScreenViewBase::tearDownScreen();
}

void HomeScreenView::handleTickEvent() {

	GUI_UpdateDigitalClock();

}

void HomeScreenView::GUI_UpdateDigitalClock() {

	presenter->GUI_DigitalClock();
	mainDigitalClock.setTime24Hour(presenter->GUI_GetHours(),
								   presenter->GUI_GetMinutes(),
								   presenter->GUI_GetSeconds());

}

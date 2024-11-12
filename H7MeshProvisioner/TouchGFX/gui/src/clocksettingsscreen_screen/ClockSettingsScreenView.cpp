#include <gui/clocksettingsscreen_screen/ClockSettingsScreenView.hpp>

ClockSettingsScreenView::ClockSettingsScreenView()
{

}

void ClockSettingsScreenView::setupScreen()
{
    ClockSettingsScreenViewBase::setupScreen();
    // call digital clock once to get the current time
    // which will be used to update the textfields
    screenTitleBar1.GUI_SetTextTitleBar("Menu", "set clock");
    presenter->GUI_DigitalClock();
    GUI_SetHourCounter(presenter->GUI_GetHours());
    GUI_SetMinuteCounter(presenter->GUI_GetMinutes());
    GUI_UpdateHourText();
    GUI_UpdateMinuteText();
}

void ClockSettingsScreenView::tearDownScreen()
{
    ClockSettingsScreenViewBase::tearDownScreen();
}

void ClockSettingsScreenView::GUI_SaveClock() {

	presenter->GUI_SaveClockSettings(GUI_GetHourCounter(), GUI_GetMinuteCounter());

}

void ClockSettingsScreenView::GUI_HourUpBtnClicked() {

	GUI_SetHour(true);

}

void ClockSettingsScreenView::GUI_HourDownBtnClicked() {

	GUI_SetHour(false);

}

void ClockSettingsScreenView::GUI_MinuteUpBtnClicked() {

	GUI_SetMinute(true);

}

void ClockSettingsScreenView::GUI_MinuteDownBtnClicked() {

	GUI_SetMinute(false);

}

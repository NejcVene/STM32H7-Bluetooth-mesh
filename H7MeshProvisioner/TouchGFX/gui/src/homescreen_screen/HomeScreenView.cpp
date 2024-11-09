#include <gui/homescreen_screen/HomeScreenView.hpp>

HomeScreenView::HomeScreenView()
{

}

void HomeScreenView::setupScreen()
{
    HomeScreenViewBase::setupScreen();
    screenTitleBar1.GUI_SetTextTitleBar("Menu", "home");
    // mainDigitalClock
    digitalHours = mainDigitalClock.getCurrentHour();
    digitalMinutes = mainDigitalClock.getCurrentMinute();
    digitalSeconds = mainDigitalClock.getCurrentSecond();
}

void HomeScreenView::tearDownScreen()
{
    HomeScreenViewBase::tearDownScreen();
}

void HomeScreenView::handleTickEvent() {

	tickCounter++;

	if (tickCounter % 60 == 0) {
		if (++digitalSeconds >= 60) {
			digitalSeconds = 0;
			if (++digitalMinutes >= 60) {
				digitalMinutes = 0;
				if (++digitalHours >= 24) {
					digitalHours = 0;
				}
			}
		}
		// update the clock
		mainDigitalClock.setTime24Hour(digitalHours, digitalMinutes, digitalSeconds);
	}

}

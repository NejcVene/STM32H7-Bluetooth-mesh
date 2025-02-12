#ifndef CLOCKSETTINGSSCREENVIEW_HPP
#define CLOCKSETTINGSSCREENVIEW_HPP

#include <gui_generated/clocksettingsscreen_screen/ClockSettingsScreenViewBase.hpp>
#include <gui/clocksettingsscreen_screen/ClockSettingsScreenPresenter.hpp>

class ClockSettingsScreenView : public ClockSettingsScreenViewBase
{
public:
    ClockSettingsScreenView();
    virtual ~ClockSettingsScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void GUI_SaveClock();
    virtual void GUI_HourUpBtnClicked();
    virtual void GUI_HourDownBtnClicked();
    virtual void GUI_MinuteUpBtnClicked();
    virtual void GUI_MinuteDownBtnClicked();

    void GUI_SetHour(bool increment) {

    	this->clockCounter += (increment ? 1 : -1);
    	this->clockCounter = (this->clockCounter + 24) % 24;
    	GUI_UpdateHourText();

    }

    void GUI_SetMinute(bool increment) {

    	this->minuteCounter += (increment ? 1 : -1);
    	this->minuteCounter = (this->minuteCounter + 60) % 60;
    	GUI_UpdateMinuteText();

    }

    void GUI_UpdateHourText() {

    	Unicode::snprintf(hourSetTextBuffer, HOURSETTEXT_SIZE, "%02u", this->clockCounter);
    	hourSetText.resizeToCurrentText();
    	hourSetText.invalidate();

    }

    void GUI_UpdateMinuteText() {

    	Unicode::snprintf(minuteSetTextBuffer, MINUTESETTEXT_SIZE, "%02u", this->minuteCounter);
    	minuteSetText.resizeToCurrentText();
    	minuteSetText.invalidate();

    }

    void GUI_SetMinuteCounter(uint8_t minutes) {

    	this->minuteCounter = (int8_t) minutes;

    }

    void GUI_SetHourCounter(uint8_t hours) {

    	this->clockCounter = (int8_t) hours;

    }

    uint8_t GUI_GetMinuteCounter() {

    	return (uint8_t) this->minuteCounter;

    }

    uint8_t GUI_GetHourCounter() {

    	return (uint8_t) this->clockCounter;

    }

protected:
private:
    int8_t minuteCounter = 0;
    int8_t clockCounter = 0;
};

#endif // CLOCKSETTINGSSCREENVIEW_HPP

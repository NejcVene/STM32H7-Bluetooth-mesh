#ifndef CLOCKSETTINGSSCREENPRESENTER_HPP
#define CLOCKSETTINGSSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ClockSettingsScreenView;

class ClockSettingsScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ClockSettingsScreenPresenter(ClockSettingsScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ClockSettingsScreenPresenter() {}

    void GUI_SaveClockSettings(uint8_t hours, uint8_t minutes) {

    	model->GUI_SetDigialClock(hours, minutes);

    }

    uint8_t GUI_GetMinutes() {

    	return model->GUI_GetMinutes();

    }

    uint8_t GUI_GetHours() {

    	return model->GUI_GetHours();

    }

    void GUI_DigitalClock() {

    	model->DigitalClock();

    }

private:
    ClockSettingsScreenPresenter();

    ClockSettingsScreenView& view;
};

#endif // CLOCKSETTINGSSCREENPRESENTER_HPP

#ifndef HOMESCREENVIEW_HPP
#define HOMESCREENVIEW_HPP

#include <gui_generated/homescreen_screen/HomeScreenViewBase.hpp>
#include <gui/homescreen_screen/HomeScreenPresenter.hpp>

class HomeScreenView : public HomeScreenViewBase
{
public:
    HomeScreenView();
    virtual ~HomeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    void GUI_UpdateDigitalClock();
    void GUI_CheckConfig();
    void GUI_NodeIsConfigured();
    void GUI_ErrorOccured();

protected:
    int tickCounter;
    int digitalHours;
    int digitalMinutes;
    int digitalSeconds;
    CMD_CommandGet_t *cmd;
};

#endif // HOMESCREENVIEW_HPP

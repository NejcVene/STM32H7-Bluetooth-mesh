#ifndef ADDDEVICESCREENVIEW_HPP
#define ADDDEVICESCREENVIEW_HPP

#include <gui_generated/adddevicescreen_screen/AddDeviceScreenViewBase.hpp>
#include <gui/adddevicescreen_screen/AddDeviceScreenPresenter.hpp>

class AddDeviceScreenView : public AddDeviceScreenViewBase
{
public:
    AddDeviceScreenView();
    virtual ~AddDeviceScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    bool increase = true;
};

#endif // ADDDEVICESCREENVIEW_HPP

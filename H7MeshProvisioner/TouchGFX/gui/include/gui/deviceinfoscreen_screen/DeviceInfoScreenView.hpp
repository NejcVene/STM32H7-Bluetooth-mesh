#ifndef DEVICEINFOSCREENVIEW_HPP
#define DEVICEINFOSCREENVIEW_HPP

#include <gui_generated/deviceinfoscreen_screen/DeviceInfoScreenViewBase.hpp>
#include <gui/deviceinfoscreen_screen/DeviceInfoScreenPresenter.hpp>

class DeviceInfoScreenView : public DeviceInfoScreenViewBase
{
public:
    DeviceInfoScreenView();
    virtual ~DeviceInfoScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void GUI_SetTextLibVer(const char *text) {

    	touchgfx::Unicode::strncpy(libVerTxtBuffer, text, LIBVERTXT_SIZE);
    	libVerTxt.resizeToCurrentText();
    	libVerTxt.invalidate();

    }

    void GUI_SetTextLibSubVer(const char *text) {

    	touchgfx::Unicode::strncpy(libSubVerTxtBuffer, text, LIBSUBVERTXT_SIZE);
    	libSubVerTxt.resizeToCurrentText();
    	libSubVerTxt.invalidate();

    }

    void GUI_SetTextCommExVer(const char *text) {

    	touchgfx::Unicode::strncpy(commExVerTxtBuffer, text, COMMEXVERTXT_SIZE);
    	commExVerTxt.resizeToCurrentText();
    	commExVerTxt.invalidate();

    }
protected:
};

#endif // DEVICEINFOSCREENVIEW_HPP

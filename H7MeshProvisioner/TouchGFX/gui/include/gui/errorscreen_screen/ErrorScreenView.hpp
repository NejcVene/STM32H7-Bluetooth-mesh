#ifndef ERRORSCREENVIEW_HPP
#define ERRORSCREENVIEW_HPP

#include <gui_generated/errorscreen_screen/ErrorScreenViewBase.hpp>
#include <gui/errorscreen_screen/ErrorScreenPresenter.hpp>

class ErrorScreenView : public ErrorScreenViewBase
{
public:
    ErrorScreenView();
    virtual ~ErrorScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void GUI_ResetDevice();

    void GUI_SetTextErrorDesc(const char *text) {

    	touchgfx::Unicode::strncpy(errorDescBuffer, text, ERRORDESC_SIZE);
    	errorDesc.invalidate();

    }

protected:
};

#endif // ERRORSCREENVIEW_HPP

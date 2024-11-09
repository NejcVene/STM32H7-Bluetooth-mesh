#ifndef SCREENTITLEBAR_HPP
#define SCREENTITLEBAR_HPP

#include <gui_generated/containers/ScreenTitleBarBase.hpp>

class ScreenTitleBar : public ScreenTitleBarBase
{
public:
    ScreenTitleBar();
    virtual ~ScreenTitleBar() {}

    virtual void initialize();

    void GUI_SetTextTitleBar(const char *prefix, const char *text) {

    	touchgfx::Unicode::strncpy(titleBarTxtBuffer1, prefix, TITLEBARTXTBUFFER1_SIZE);
    	touchgfx::Unicode::strncpy(titleBarTxtBuffer2, text, TITLEBARTXTBUFFER2_SIZE);
    	titleBarTxt.resizeToCurrentText();
    	titleBarTxt.invalidate();

    }

protected:
};

#endif // SCREENTITLEBAR_HPP

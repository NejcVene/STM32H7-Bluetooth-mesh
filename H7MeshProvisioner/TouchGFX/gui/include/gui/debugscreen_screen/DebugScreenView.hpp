#ifndef DEBUGSCREENVIEW_HPP
#define DEBUGSCREENVIEW_HPP

#include <gui_generated/debugscreen_screen/DebugScreenViewBase.hpp>
#include <gui/debugscreen_screen/DebugScreenPresenter.hpp>

class DebugScreenView : public DebugScreenViewBase
{
public:
    DebugScreenView();
    virtual ~DebugScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void GUI_DebugAtepRoot();
    virtual void GUI_UnprovEm();
    virtual void GUI_DebugKitGrpAddr();
    virtual void GUI_DebugLivRoomGrpAddr();
    virtual void GUI_DebugTestProStruct();

protected:

private:
    static CMD_CommandGet_t *cmd;
};

#endif // DEBUGSCREENVIEW_HPP

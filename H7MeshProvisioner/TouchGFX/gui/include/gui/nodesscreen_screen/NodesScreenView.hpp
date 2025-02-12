#ifndef NODESSCREENVIEW_HPP
#define NODESSCREENVIEW_HPP

#include <gui_generated/nodesscreen_screen/NodesScreenViewBase.hpp>
#include <gui/nodesscreen_screen/NodesScreenPresenter.hpp>
#include <gui/containers/DeviceToggle.hpp>

class NodesScreenView : public NodesScreenViewBase
{
public:
    NodesScreenView();
    virtual ~NodesScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

//    void GUI_SetTextRoomTitle(const char *text) {
//
//    	touchgfx::Unicode::strncpy(menuNameBuffer, text, MENUNAME_SIZE);
//    	menuName.resizeToCurrentText();
//    	menuName.invalidate();
//
//    }

protected:
    const char *roomName;
    uint8_t roomBitmask;
    Node_Config_t *nodes;
    static const int maxDevices = 5;
    DeviceToggle devices[maxDevices];

private:
    touchgfx::Callback<NodesScreenView, int> buttonClickCallback;
    void handleButtonClicked(int instanceID);
};

#endif // NODESSCREENVIEW_HPP

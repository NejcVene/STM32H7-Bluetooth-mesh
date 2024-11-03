#ifndef NODEPROPERSCREENVIEW_HPP
#define NODEPROPERSCREENVIEW_HPP

#include <gui_generated/nodeproperscreen_screen/NodeProperScreenViewBase.hpp>
#include <gui/nodeproperscreen_screen/NodeProperScreenPresenter.hpp>
#include <gui/containers/DeviceButton.hpp>

class NodeProperScreenView : public NodeProperScreenViewBase
{
public:
    NodeProperScreenView();
    virtual ~NodeProperScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void setupGenericOnOff();

    void GUI_SetTextNodeName(const char *text) {

    	touchgfx::Unicode::strncpy(nodeNameBuffer, text, NODENAME_SIZE);
    	nodeName.resizeToCurrentText();
    	nodeName.invalidate();

    }

protected:
    touchgfx::Callback<NodeProperScreenView, int> buttonClickCallback;
    void handleButtonClicked(int instanceID);
    Node_Config_t *node;
    uint16_t nodeModel;
    DeviceButton genOnOffBtn;
};

#endif // NODEPROPERSCREENVIEW_HPP

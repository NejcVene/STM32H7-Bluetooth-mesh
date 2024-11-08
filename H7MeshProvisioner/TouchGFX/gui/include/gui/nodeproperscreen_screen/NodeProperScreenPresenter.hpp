#ifndef NODEPROPERSCREENPRESENTER_HPP
#define NODEPROPERSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class NodeProperScreenView;

class NodeProperScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    NodeProperScreenPresenter(NodeProperScreenView& v);

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

    virtual ~NodeProperScreenPresenter() {}

    uint32_t GUI_GetNodeAddress() {

    	return model->GUI_GetNodeAddress();

    }

    void GUI_SendCommand(CMD_CommandGet_t *cmd) {

    	model->GUI_SendCommand(cmd);

    }

    void GUI_SetDeviceToConfigure(Node_Config_t *node) {

    	model->GUI_SetDeviceToConfigure(node);

    }

    void GUI_SetAllGroupAddress(NC_MaskedFeatures *allGroupAddress) {

    	model->GUI_SetAllGroupAddress(allGroupAddress);

    }

    virtual void GUI_ChangeScreenAfterUnprov();

private:
    NodeProperScreenPresenter();

    NodeProperScreenView& view;
};

#endif // NODEPROPERSCREENPRESENTER_HPP

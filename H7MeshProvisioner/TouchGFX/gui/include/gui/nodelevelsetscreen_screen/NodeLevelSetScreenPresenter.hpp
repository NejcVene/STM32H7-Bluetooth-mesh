#ifndef NODELEVELSETSCREENPRESENTER_HPP
#define NODELEVELSETSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class NodeLevelSetScreenView;

class NodeLevelSetScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    NodeLevelSetScreenPresenter(NodeLevelSetScreenView& v);

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

    virtual ~NodeLevelSetScreenPresenter() {}

    uint32_t GUI_GetNodeAddress() {

    	return model->GUI_GetNodeAddress();

    }

    void GUI_SendCommand(CMD_CommandGet_t *cmd) {

    	model->GUI_SendCommand(cmd);

    }

private:
    NodeLevelSetScreenPresenter();

    NodeLevelSetScreenView& view;
};

#endif // NODELEVELSETSCREENPRESENTER_HPP

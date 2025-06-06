#ifndef NODESSCREENPRESENTER_HPP
#define NODESSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class NodesScreenView;

class NodesScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    NodesScreenPresenter(NodesScreenView& v);

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

    virtual ~NodesScreenPresenter() {}

    const char *GUI_GetRoomName() {

    	return model->GUI_GetRoomName();

    }

    uint8_t GUI_GetRoomBitmask() {

    	return model->GUI_GetRoomBitmask();

    }

    void GUI_SaveNodeAddress(int nodeAddress) {

    	model->GUI_SetNodeAddress(nodeAddress);

    }

private:
    NodesScreenPresenter();

    NodesScreenView& view;
};

#endif // NODESSCREENPRESENTER_HPP

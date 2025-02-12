#ifndef DEBUGSCREENPRESENTER_HPP
#define DEBUGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DebugScreenView;

class DebugScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DebugScreenPresenter(DebugScreenView& v);

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

    virtual ~DebugScreenPresenter() {}

    virtual void GUI_SendCommand(CMD_CommandGet_t *cmd);

private:
    DebugScreenPresenter();

    DebugScreenView& view;
};

#endif // DEBUGSCREENPRESENTER_HPP

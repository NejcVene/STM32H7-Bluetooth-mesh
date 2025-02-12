#ifndef RENAMESCREENPRESENTER_HPP
#define RENAMESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class RenameScreenView;

class RenameScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    RenameScreenPresenter(RenameScreenView& v);

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

    virtual ~RenameScreenPresenter() {}

    Node_Config_t *GUI_GetDeviceToConfigureFromModel() {

    	return model->GUI_GetDeviceToConfigure();

    }

private:
    RenameScreenPresenter();

    RenameScreenView& view;
};

#endif // RENAMESCREENPRESENTER_HPP

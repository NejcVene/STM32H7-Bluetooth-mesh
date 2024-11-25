#ifndef ADDDEVICESCREENPRESENTER_HPP
#define ADDDEVICESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class AddDeviceScreenView;

class AddDeviceScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    AddDeviceScreenPresenter(AddDeviceScreenView& v);

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

    virtual ~AddDeviceScreenPresenter() {}

    virtual void GUI_SendCommand(CMD_CommandGet_t *cmd);
    virtual void GUI_DevicesFound();
    virtual void GUI_ChangeScreen();
    virtual void GUI_NotifyError();

    int GUI_GetUsedCommandIndex() {

    	return model->GUI_GetUsedCommandIndex();

    }

private:
    AddDeviceScreenPresenter();

    AddDeviceScreenView& view;
};

#endif // ADDDEVICESCREENPRESENTER_HPP

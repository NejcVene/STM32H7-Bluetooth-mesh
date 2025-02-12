#ifndef DEVICEINFOSCREENPRESENTER_HPP
#define DEVICEINFOSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DeviceInfoScreenView;

class DeviceInfoScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DeviceInfoScreenPresenter(DeviceInfoScreenView& v);

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

    virtual ~DeviceInfoScreenPresenter() {}

    const char *GUI_GetLibVer() {

    	return model->GUI_GetLibVer();

    }

    const char *GUI_GetLibSubVer() {

    	return model->GUI_GetLibSubVer();

    }

    const char *GUI_GetCommExVer() {

    	return model->GUI_GetCommExVer();

    }

private:
    DeviceInfoScreenPresenter();

    DeviceInfoScreenView& view;
};

#endif // DEVICEINFOSCREENPRESENTER_HPP

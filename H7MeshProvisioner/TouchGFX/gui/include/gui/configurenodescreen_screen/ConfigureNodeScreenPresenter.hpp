#ifndef CONFIGURENODESCREENPRESENTER_HPP
#define CONFIGURENODESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ConfigureNodeScreenView;

class ConfigureNodeScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ConfigureNodeScreenPresenter(ConfigureNodeScreenView& v);

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

    virtual ~ConfigureNodeScreenPresenter() {}

    virtual void GUI_ConfigureDevice();

private:
    ConfigureNodeScreenPresenter();

    ConfigureNodeScreenView& view;
};

#endif // CONFIGURENODESCREENPRESENTER_HPP

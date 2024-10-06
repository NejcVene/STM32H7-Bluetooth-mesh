#ifndef CONFIGURENODESCREENVIEW_HPP
#define CONFIGURENODESCREENVIEW_HPP

#include <gui_generated/configurenodescreen_screen/ConfigureNodeScreenViewBase.hpp>
#include <gui/configurenodescreen_screen/ConfigureNodeScreenPresenter.hpp>

class ConfigureNodeScreenView : public ConfigureNodeScreenViewBase
{
public:
    ConfigureNodeScreenView();
    virtual ~ConfigureNodeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CONFIGURENODESCREENVIEW_HPP

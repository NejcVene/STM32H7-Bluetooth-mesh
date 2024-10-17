#include <gui/configurenodescreen_screen/ConfigureNodeScreenView.hpp>
#include <gui/configurenodescreen_screen/ConfigureNodeScreenPresenter.hpp>

ConfigureNodeScreenPresenter::ConfigureNodeScreenPresenter(ConfigureNodeScreenView& v)
    : view(v)
{

}

void ConfigureNodeScreenPresenter::activate()
{

}

void ConfigureNodeScreenPresenter::deactivate()
{

}

void ConfigureNodeScreenPresenter::GUI_ConfigureDevice() {

	view.GUI_SetDeviceToConfigure(model->GUI_GetDeviceToConfigure());

}

void ConfigureNodeScreenPresenter::GUI_SendCommand(uint16_t cmdIndex, void *cmdParam) {

	model->GUI_SendCommand(cmdIndex, cmdParam);

}

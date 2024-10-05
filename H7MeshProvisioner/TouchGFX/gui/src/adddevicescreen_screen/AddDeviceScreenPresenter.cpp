#include <gui/adddevicescreen_screen/AddDeviceScreenView.hpp>
#include <gui/adddevicescreen_screen/AddDeviceScreenPresenter.hpp>

AddDeviceScreenPresenter::AddDeviceScreenPresenter(AddDeviceScreenView& v)
    : view(v)
{

}

void AddDeviceScreenPresenter::activate()
{

}

void AddDeviceScreenPresenter::deactivate()
{

}

void AddDeviceScreenPresenter::GUI_SendCommand(uint16_t cmdIndex, void *cmdParam) {

	model->GUI_SendCommand(cmdIndex, cmdParam);

}

void AddDeviceScreenPresenter::GUI_DevicesFound() {

	view.GUI_SetDevicesFound(model->GUI_GetFoundDevices());

}

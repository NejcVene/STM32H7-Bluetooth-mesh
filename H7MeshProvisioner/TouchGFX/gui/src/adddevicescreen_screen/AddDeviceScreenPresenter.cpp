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

void AddDeviceScreenPresenter::GUI_SendCommand(CMD_CommandGet_t *cmd) {

	model->GUI_SendCommand(cmd);

}

void AddDeviceScreenPresenter::GUI_DevicesFound() {

	view.GUI_SetDevicesFound(model->GUI_GetFoundDevices());

}

void AddDeviceScreenPresenter::GUI_ChangeScreen() {

	view.GUI_SwitchScreenAfterProv();

}

#include <gui/settingsscreen_screen/SettingsScreenView.hpp>
#include <gui/settingsscreen_screen/SettingsScreenPresenter.hpp>

SettingsScreenPresenter::SettingsScreenPresenter(SettingsScreenView& v)
    : view(v)
{

}

void SettingsScreenPresenter::activate()
{

}

void SettingsScreenPresenter::deactivate()
{

}

void SettingsScreenPresenter::GUI_ChangeScreenDevInfo() {

	view.GUI_ChangeScreen();

}

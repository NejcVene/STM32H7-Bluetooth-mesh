#include <gui/settingsscreen_screen/SettingsScreenView.hpp>

SettingsScreenView::SettingsScreenView()
{

}

void SettingsScreenView::setupScreen()
{
    SettingsScreenViewBase::setupScreen();
    screenTitleBar1.GUI_SetTextTitleBar("Menu", "settings");
}

void SettingsScreenView::tearDownScreen()
{
    SettingsScreenViewBase::tearDownScreen();
}

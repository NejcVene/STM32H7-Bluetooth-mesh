#include <gui/deviceinfoscreen_screen/DeviceInfoScreenView.hpp>

DeviceInfoScreenView::DeviceInfoScreenView()
{

}

void DeviceInfoScreenView::setupScreen()
{
    DeviceInfoScreenViewBase::setupScreen();
    screenTitleBar1.GUI_SetTextTitleBar("Menu", "device information");
    GUI_SetTextLibVer(presenter->GUI_GetLibVer());
    GUI_SetTextLibSubVer(presenter->GUI_GetLibSubVer());
    GUI_SetTextCommExVer(presenter->GUI_GetCommExVer());
}

void DeviceInfoScreenView::tearDownScreen()
{
    DeviceInfoScreenViewBase::tearDownScreen();
}

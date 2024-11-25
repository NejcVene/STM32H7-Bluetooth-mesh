#include <gui/errorscreen_screen/ErrorScreenView.hpp>

ErrorScreenView::ErrorScreenView()
{

}
// GUI_GetErrorString
void ErrorScreenView::setupScreen()
{
    ErrorScreenViewBase::setupScreen();
    screenTitleBar1.GUI_SetTextTitleBar("System", "error");
    GUI_SetTextErrorDesc(presenter->GUI_GetErrorString());
}

void ErrorScreenView::tearDownScreen()
{
    ErrorScreenViewBase::tearDownScreen();
}

void ErrorScreenView::GUI_ResetDevice() {

	NC_ResetDevice();

}

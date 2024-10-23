#include <gui/debugscreen_screen/DebugScreenView.hpp>
#include "command.h"

CMD_CommandGet_t *DebugScreenView::cmd = nullptr;

DebugScreenView::DebugScreenView()
{

}

void DebugScreenView::setupScreen()
{
    DebugScreenViewBase::setupScreen();
}

void DebugScreenView::tearDownScreen()
{
    DebugScreenViewBase::tearDownScreen();
}

void DebugScreenView::GUI_DebugAtepRoot() {

	this->cmd = CMD_CreateCommandGet(CMD_MESH_ATEP_ROOT,
	    							NULL,
									NULL,
									0,
									NULL,
									NULL);
	presenter->GUI_SendCommand(this->cmd);

}

void DebugScreenView::GUI_UnprovEm() {

}

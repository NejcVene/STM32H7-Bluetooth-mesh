#include <gui/debugscreen_screen/DebugScreenView.hpp>
#include "command.h"

CMD_CommandGet_t *DebugScreenView::cmd = nullptr;

DebugScreenView::DebugScreenView()
{

}

void DebugScreenView::setupScreen()
{
    DebugScreenViewBase::setupScreen();
    screenTitleBar1.GUI_SetTextTitleBar("Menu", "debug");
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

void DebugScreenView::GUI_DebugKitGrpAddr() {

	this->cmd = CMD_CreateCommandGet(CMD_MESH_ATCL_GENERIC_ON_OFF_ACK,
		    						NULL,
									NULL,
									0,
									NULL,
									NULL);
	presenter->GUI_SendCommand(this->cmd);

}

void DebugScreenView::GUI_DebugLivRoomGrpAddr() {

//	this->cmd = CMD_CreateCommandGet(CMD_MESH_ATCL_GENECI_ON_OFF_ACK_OFF,
//			    					NULL,
//									NULL,
//									0,
//									NULL,
//									NULL);
//	presenter->GUI_SendCommand(this->cmd);

}

void DebugScreenView::GUI_DebugTestProStruct() {

	this->cmd = CMD_CreateCommandGet(CMD_FUN_PRO_TEST,
									NULL,
									NULL,
									0,
									NULL,
									NULL);
	presenter->GUI_SendCommand(this->cmd);

}




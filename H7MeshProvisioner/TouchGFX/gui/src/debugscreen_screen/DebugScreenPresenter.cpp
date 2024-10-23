#include <gui/debugscreen_screen/DebugScreenView.hpp>
#include <gui/debugscreen_screen/DebugScreenPresenter.hpp>

DebugScreenPresenter::DebugScreenPresenter(DebugScreenView& v)
    : view(v)
{

}

void DebugScreenPresenter::activate()
{

}

void DebugScreenPresenter::deactivate()
{

}

void DebugScreenPresenter::GUI_SendCommand(CMD_CommandGet_t *cmd) {

	model->GUI_SendCommand(cmd);

}

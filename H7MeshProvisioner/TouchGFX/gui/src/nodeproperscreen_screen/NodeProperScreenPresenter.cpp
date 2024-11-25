#include <gui/nodeproperscreen_screen/NodeProperScreenView.hpp>
#include <gui/nodeproperscreen_screen/NodeProperScreenPresenter.hpp>

NodeProperScreenPresenter::NodeProperScreenPresenter(NodeProperScreenView& v)
    : view(v)
{

}

void NodeProperScreenPresenter::activate()
{

}

void NodeProperScreenPresenter::deactivate()
{

}

void NodeProperScreenPresenter::GUI_ChangeScreenAfterUnprov() {

	view.changeScreenAfterUnprov();

}

void NodeProperScreenPresenter::GUI_ConfirmDescGet() {

	view.GUI_ConfirmDescGet();

}

void NodeProperScreenPresenter::GUI_NotifyError() {

	view.GUI_ErrorOccured();

}

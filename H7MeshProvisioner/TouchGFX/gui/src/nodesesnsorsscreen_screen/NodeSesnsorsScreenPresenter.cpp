#include <gui/nodesesnsorsscreen_screen/NodeSesnsorsScreenView.hpp>
#include <gui/nodesesnsorsscreen_screen/NodeSesnsorsScreenPresenter.hpp>

NodeSesnsorsScreenPresenter::NodeSesnsorsScreenPresenter(NodeSesnsorsScreenView& v)
    : view(v)
{

}

void NodeSesnsorsScreenPresenter::activate()
{

}

void NodeSesnsorsScreenPresenter::deactivate()
{

}

void NodeSesnsorsScreenPresenter::GUI_UpdateSensorValues() {

	view.GUI_UpdateSensorValues();

}

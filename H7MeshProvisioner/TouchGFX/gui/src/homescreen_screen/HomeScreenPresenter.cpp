#include <gui/homescreen_screen/HomeScreenView.hpp>
#include <gui/homescreen_screen/HomeScreenPresenter.hpp>

HomeScreenPresenter::HomeScreenPresenter(HomeScreenView& v)
    : view(v)
{

}

void HomeScreenPresenter::activate()
{

}

void HomeScreenPresenter::deactivate()
{

}

void HomeScreenPresenter::GUI_NodeIsConfigured() {

	view.GUI_NodeIsConfigured();

}

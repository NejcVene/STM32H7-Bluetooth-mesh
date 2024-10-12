#include <gui/configurenodescreen_screen/ConfigureNodeScreenView.hpp>

ConfigureNodeScreenView::ConfigureNodeScreenView() : buttonClickCallback(this, &ConfigureNodeScreenView::handleButtonClicked)
{

}

void ConfigureNodeScreenView::setupScreen()
{
    ConfigureNodeScreenViewBase::setupScreen();
    for (int i = 0; i<maxSubsPerNode; i++) {
    	nodeSubscriptions[i].initialize();
    	nodeSubscriptions[i].setPosition(10, 10 + (i * 70), 310, 60);
    	nodeSubscriptions[i].setVisible(true);
    	allNodeSubsCont.add(nodeSubscriptions[i]);
    }
    allNodeSubsCont.invalidateContent();
}

void ConfigureNodeScreenView::tearDownScreen()
{
    ConfigureNodeScreenViewBase::tearDownScreen();
}

void ConfigureNodeScreenView::handleButtonClicked(int instanceID) {

	;

}

#include <gui/nodelevelsetscreen_screen/NodeLevelSetScreenView.hpp>

NodeLevelSetScreenView::NodeLevelSetScreenView()
{

}

void NodeLevelSetScreenView::setupScreen()
{
    NodeLevelSetScreenViewBase::setupScreen();
    this->node = NC_GetConfigNodeFromAddress(presenter->GUI_GetNodeAddress());

    screenTitleBar1.GUI_SetTextTitleBar("Level set", this->node->nodeName);
    GUI_SetSliderValue(this->node->states.levelSetValue);
    GUI_SetTextSliderValue(this->node->states.levelSetValue);

}

void NodeLevelSetScreenView::tearDownScreen()
{
    NodeLevelSetScreenViewBase::tearDownScreen();
}

void NodeLevelSetScreenView::GUI_ChangeLevelValueText(int value) {

	GUI_SetTextSliderValue(value);

}

void NodeLevelSetScreenView::GUI_LevelSet() {

	PARAMETER_TYPE types[] = {PARAM_CHAR, PARAM_CHAR};
	int16_t levelValue = 0;
	void *paramValues[2];
	char stringLevelSet[5];
	char stringNodeAddress[10];

	levelValue = GUI_GetSliderValue();
	this->node->states.levelSetValue = levelValue;
	sprintf(stringNodeAddress, "%04X", (unsigned int) this->node->address.nodeAddress);
	sprintf(stringLevelSet, "%04X", levelValue);
	paramValues[0] = (void *) stringNodeAddress;
	paramValues[1] = (void *) stringLevelSet;
	this->cmd = CMD_CreateCommandGet(CMD_MESH_ATCL_GENERIC_LEVEL_DELTA_SET,
									types,
									paramValues,
									2,
									NULL,
									NULL);
	presenter->GUI_SendCommand(this->cmd);

}

int16_t NodeLevelSetScreenView::MapSliderToGenericLevelRange(int sliderValue) {

	return (int16_t) ((65535 * sliderValue) / 100) - 32768;

}

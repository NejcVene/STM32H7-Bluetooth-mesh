#include <gui/renamescreen_screen/RenameScreenView.hpp>

RenameScreenView::RenameScreenView()
{

}

void RenameScreenView::setupScreen()
{
    RenameScreenViewBase::setupScreen();

    this->node = presenter->GUI_GetDeviceToConfigureFromModel();

    screenTitleBar1.GUI_SetTextTitleBar("Rename", node->nodeName);
    cKeyboard.setPosition(10, 32, 320, 240);
    add(cKeyboard);

}

void RenameScreenView::tearDownScreen()
{
    RenameScreenViewBase::tearDownScreen();
}

void RenameScreenView::GUI_SaveRename() {

	Unicode::UnicodeChar *pBuffer = cKeyboard.getBuffer();
	uint16_t pos = cKeyboard.getBufferPosition();
	uint8_t converted[20] = {0};

	if (pos > 0) {
		Unicode::toUTF8(pBuffer, converted, sizeof(converted));
	} else {
		strncpy((char *) converted, DEF_NODE_NAME, MAX_NODE_NAME - 1);
	}
	NC_ChangeNodeName(this->node, (char *) converted);
	goToScreen1();

}

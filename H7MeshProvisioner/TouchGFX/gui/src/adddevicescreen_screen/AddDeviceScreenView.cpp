#include <gui/adddevicescreen_screen/AddDeviceScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "node_config.h"
#include "command.h"

int nay2 = 0;

AddDeviceScreenView::AddDeviceScreenView()
{

}

void AddDeviceScreenView::setupScreen()
{
    AddDeviceScreenViewBase::setupScreen();
    foudNodeBtn.setVisible(false);
    for (int i = 0; i<maxButtons; i++) {
    	deviceFoundBtn[i].setBitmaps(Bitmap(BITMAP_H7LINEMEDIUM_ID), Bitmap(BITMAP_H7LINEMEDIUM_ID));
    	deviceFoundBtn[i].setBitmapXY(0, 0);
//    	deviceFoundBtn[i].setWildcardText(TypedText(T___SINGLEUSE_MVY0));
//    	deviceFoundBtn[i].setWildcardTextBuffer(TypedText(T___SINGLEUSE_JF9F).getText());
    	deviceFoundBtn[i].setWildcardTextPosition(0, 16, 460, 60);
    	deviceFoundBtn[i].setWildcardTextColors(touchgfx::Color::getColorFromRGB(10, 10, 10), touchgfx::Color::getColorFromRGB(10, 10, 10));
    	// deviceFoundBtn[i].setAction(flexButtonCallback);
    	deviceFoundBtn[i].setPosition(10, 20 + (60 * i), 460, 60);
    	deviceFoundBtn[i].setVisible(false);
    	scrollableContainer1.add(deviceFoundBtn[i]);
    }
//    foudNodeBtn.setVisible(false);
    for (int i = 0; i<maxButtons; i++) {
    	deviceAdds[i].initialize();
    	deviceAdds[i].setPosition(10, 0 + (70 * i), 460, 60);
    	touchgfx::Callback<AddDeviceScreenView, const touchgfx::AbstractButtonContainer&> callback(this, &AddDeviceScreenView::onDeviceAddButtonClicked);
    	deviceAdds[i].setButtonAction(1, callback); // Example ID of 1
//    	touchgfx::GenericCallback<const touchgfx::AbstractButtonContainer&, int> buttonCallback(this, &AddDeviceScreenView::onDeviceButtonClicked);
//    	deviceAdds[i].setButtonAction(i, buttonCallback);  // Pass the index as instance ID
    	deviceAdds[i].setTextUUID("Testing");
    	scrollableContainer1.add(deviceAdds[i]);
    }
    // modalWindow1.show();
    modalWindow1.hide();
    scrollableContainer1.invalidateContent();
    // presenter->GUI_SendCommand(CMD_MESH_ATEP_SCAN);
}

void AddDeviceScreenView::tearDownScreen()
{
    AddDeviceScreenViewBase::tearDownScreen();
}

void AddDeviceScreenView::handleTickEvent() {

	if (disable) {
		modalWindow1.hide();
		return;
	}

	int nextValue;
	int currentValue = circleProgress1.getValue();
	int16_t max = 100;
	int16_t min = 0;

	if (increase) {
		nextValue = currentValue + 1;
		if (nextValue == max) {
			increase = false;
		}
	} else if (!increase) {
		nextValue = currentValue - 1;
		if (nextValue == min) {
			increase = true;
		}
	}
	circleProgress1.setValue(nextValue);

}

void AddDeviceScreenView::GUI_SetDevicesFound(Node_NetworkAddress_t *foundDevices) {

	// TODO: add callbacks for buttons
	disable = true;
	for (int i = 0; i<5; i++) {
		if (foundDevices[i].nodeAddress != NODE_DEF_VAL) {
//			touchgfx::Unicode::strncpy(textArea2Buffer, foundDevices[i].uuid, TEXTAREA2_SIZE);
//			touchgfx::Unicode::strncpy(modelsBuffer, foundDevices[i].nodeModels, MODELS_SIZE);
//			touchgfx::Unicode::strncpy(featuresBuffer, foundDevices[i].nodeFeatures, FEATURES_SIZE);
			deviceFoundBtn[i].setVisible(true);
		}
	}
	scrollableContainer1.invalidateContent();

}

void AddDeviceScreenView::onDeviceAddButtonClicked(const touchgfx::AbstractButtonContainer& button)
{
    // Handle button click for the DeviceAdd instance
    nay2 = !nay2;
    // Add further logic based on the button clicked
}

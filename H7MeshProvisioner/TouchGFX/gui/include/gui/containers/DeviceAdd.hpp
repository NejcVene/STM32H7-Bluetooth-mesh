#ifndef DEVICEADD_HPP
#define DEVICEADD_HPP

#include <gui_generated/containers/DeviceAddBase.hpp>
#include <touchgfx/Callback.hpp>

class DeviceAdd : public DeviceAddBase
{
public:
    DeviceAdd();
    virtual ~DeviceAdd() {}

    virtual void initialize();

    // Set button action
    void setButtonAction(int id, touchgfx::GenericCallback<const touchgfx::AbstractButtonContainer&>& callback) {
        instanceID = id;  // Store instance ID
        buttonClickedCallback = &callback;  // Store the reference to the callback
    }

    // Event handler for button clicks
    virtual void onButtonClicked(const touchgfx::AbstractButtonContainer& button);

    void setTextUUID(const char* text) {
    	touchgfx::Unicode::strncpy(nodeUUIDBuffer, text, NODEUUID_SIZE);  // Update the first text area
    }

    void setTextModels(const char* text) {
    	touchgfx::Unicode::strncpy(nodeModelsBuffer, text, NODEMODELS_SIZE);  // Update the first text area
    }

    void setTextFeatures(const char* text) {
    	touchgfx::Unicode::strncpy(nodeFeaturesBuffer, text, NODEFEATURES_SIZE);  // Update the first text area
    }

protected:
    virtual void emitSomethingHappenedCallback(const touchgfx::AbstractButtonContainer& button) {
        if (buttonClickedCallback && buttonClickedCallback->isValid()) {
            // Execute the callback and pass the actual button reference
            buttonClickedCallback->execute(button);
        }
    }

private:
    touchgfx::GenericCallback<const touchgfx::AbstractButtonContainer&>* buttonClickedCallback;  // Pointer to the FlexButton callback
    int instanceID;  // Unique ID for each instance
    touchgfx::Callback<DeviceAdd, const touchgfx::AbstractButtonContainer&> buttonCallback; // Member variable for the button action
};


#endif // DEVICEADD_HPP

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

    void setTextUUID(const char* text) {
    	touchgfx::Unicode::strncpy(nodeUUIDBuffer, text, NODEUUID_SIZE);  // Update the first text area
    }

    void setTextModels(const char* text) {
    	touchgfx::Unicode::strncpy(nodeModelsBuffer, text, NODEMODELS_SIZE);  // Update the first text area
    }

    void setTextFeatures(const char* text) {
    	touchgfx::Unicode::strncpy(nodeFeaturesBuffer, text, NODEFEATURES_SIZE);  // Update the first text area
    }

    void setButtonAction(touchgfx::GenericCallback<int>& callback, int instanceID) {
    	buttonClickedCallback = &callback;
    	this->instanceID = instanceID;
    }

    virtual void onButtonClicked(const touchgfx::AbstractButton& button);

protected:

    virtual void emitSomethingHappenedCallback() {

    	if (buttonClickedCallback && buttonClickedCallback->isValid()) {
        		buttonClickedCallback->execute(instanceID);
    	}
    }


private:

    touchgfx::GenericCallback<int>* buttonClickedCallback;
    int instanceID;
    touchgfx::Callback<DeviceAdd, const touchgfx::AbstractButton&> buttonCallback;
};


#endif // DEVICEADD_HPP

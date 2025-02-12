#ifndef LOADER_HPP
#define LOADER_HPP

#include <gui_generated/containers/LoaderBase.hpp>

class Loader : public LoaderBase
{
public:
    Loader();
    virtual ~Loader() {}

    virtual void initialize();

    void GUI_ProgressLoader() {

    	if (this->disable) {
    		return;
    	}

    	int nextValue;
    	int currentValue = loaderProgress.getValue();
    	uint16_t max = 100;
    	uint16_t min = 0;

    	if (this->increase) {
    		nextValue = currentValue + 1;
    		if (nextValue == max) {
    			this->increase = false;
    		}
    	} else if (!this->increase) {
    		nextValue = currentValue - 1;
    		if (nextValue == min) {
    			this->increase = true;
    		}
    	}
    	loaderProgress.setValue(nextValue);

    }

    void GUI_SetLoaderText(const char *text) {

    	touchgfx::Unicode::strncpy(loaderTextBuffer, text, LOADERTEXT_SIZE);
    	loaderText.invalidate();

    }

    void GUI_SetDisable(bool state) {

    	this->disable = state;

    }

    bool GUI_IsDisabled() {

    	return this->disable;

    }

    void GUI_ResetProgressValue() {

    	loaderProgress.setValue(0);

    }

protected:
private:
    bool increase = true;
    bool disable = false;

};

#endif // LOADER_HPP

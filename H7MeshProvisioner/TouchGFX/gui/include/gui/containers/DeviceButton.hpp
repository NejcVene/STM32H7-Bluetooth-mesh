#ifndef DEVICEBUTTON_HPP
#define DEVICEBUTTON_HPP

#include <gui_generated/containers/DeviceButtonBase.hpp>
#include <images/BitmapDatabase.hpp>
#include <touchgfx/Callback.hpp>

class DeviceButton : public DeviceButtonBase
{
public:
    DeviceButton();
    virtual ~DeviceButton() {}

    virtual void initialize();

    enum ICON_TYPE {
    	GENERIC_ON_OFF,
		GENERIC_LEVEL,
		SENSOR,
		LIGHT_LIGHTNESS,
		GENERIC_POWER,
		RENAME,
		CONFIGURE,
		UNPROVISION
    };

    enum STATE {
		BUTTON_OFF = 0,
    	BUTTON_ON
    };

    void GUI_SetTextStatus(const char *text) {

    	touchgfx::Unicode::strncpy(statusTextBuffer, text, STATUSTEXT_SIZE);
    	statusText.resizeToCurrentText();
    	statusText.invalidate();

    }

    void GUI_SetIntigerStatus(int number) {

    	touchgfx::Unicode::snprintf(statusTextBuffer, STATUSTEXT_SIZE, "%d", number);
    	statusText.resizeToCurrentText();
    	statusText.invalidate();

    }

    void GUI_SetDoubleStatus(double number) {

    	touchgfx::Unicode::snprintfFloat(statusTextBuffer, STATUSTEXT_SIZE, "%.1f", number);
    	statusText.resizeToCurrentText();
    	statusText.invalidate();

    }

    void GUI_SetTextType(const char *text) {

    	touchgfx::Unicode::strncpy(typeTextBuffer, text, TYPETEXT_SIZE);
    	typeText.resizeToCurrentText();
    	typeText.invalidate();

    }

    void GUI_SetIcon(ICON_TYPE type) {

    	switch (type) {
    		case GENERIC_ON_OFF: // BITMAP_ICON_THEME_IMAGES_TOGGLE_TOGGLE_OFF_50_50_FFFFFE_SVG_ID
				icon.setIconBitmaps(
						Bitmap(BITMAP_ICON_THEME_IMAGES_TOGGLE_TOGGLE_OFF_50_50_FFFFFE_SVG_ID),
						Bitmap(BITMAP_ICON_THEME_IMAGES_TOGGLE_TOGGLE_OFF_50_50_FFFFFE_SVG_ID));
    			break;
    		case GENERIC_LEVEL: // BITMAP_ICON_THEME_IMAGES_COMMUNICATION_IMPORT_EXPORT_50_50_FFFFFE_SVG_ID
    			icon.setIconBitmaps(
    					Bitmap(BITMAP_ICON_THEME_IMAGES_COMMUNICATION_IMPORT_EXPORT_50_50_FFFFFE_SVG_ID),
						Bitmap(BITMAP_ICON_THEME_IMAGES_COMMUNICATION_IMPORT_EXPORT_50_50_FFFFFE_SVG_ID));
    			break;
    		case SENSOR: // BITMAP_ICON_THEME_IMAGES_DEVICE_DATA_USAGE_50_50_FFFFFE_SVG_ID
    			icon.setIconBitmaps(
    					Bitmap(BITMAP_ICON_THEME_IMAGES_DEVICE_DATA_USAGE_50_50_FFFFFE_SVG_ID),
						Bitmap(BITMAP_ICON_THEME_IMAGES_DEVICE_DATA_USAGE_50_50_FFFFFE_SVG_ID));
    			break;
    		case GENERIC_POWER:
    			icon.setIconBitmaps(
    					Bitmap(BITMAP_ICON_THEME_IMAGES_NOTIFICATION_POWER_OFF_50_50_FFFFFE_SVG_ID),
						Bitmap(BITMAP_ICON_THEME_IMAGES_NOTIFICATION_POWER_OFF_50_50_FFFFFE_SVG_ID));
    			break;
    		case CONFIGURE: // BITMAP_ICON_THEME_IMAGES_ACTION_SETTINGS_APPLICATIONS_50_50_FFFFFE_SVG_ID
    			icon.setIconBitmaps(
    					Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_SETTINGS_APPLICATIONS_50_50_FFFFFE_SVG_ID),
						Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_SETTINGS_APPLICATIONS_50_50_FFFFFE_SVG_ID));
    			break;
    		case UNPROVISION: // BITMAP_ICON_THEME_IMAGES_ACTION_DELETE_OUTLINE_50_50_FFFFFE_SVG_ID
    			icon.setIconBitmaps(
    					Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_DELETE_OUTLINE_50_50_FFFFFE_SVG_ID),
						Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_DELETE_OUTLINE_50_50_FFFFFE_SVG_ID));
    			break;
    		case RENAME:
    			icon.setIconBitmaps(
    					Bitmap(BITMAP_ICON_THEME_IMAGES_FILE_DRIVE_FILE_RENAME_OUTLINE_50_50_FFFFFE_SVG_ID),
						Bitmap(BITMAP_ICON_THEME_IMAGES_FILE_DRIVE_FILE_RENAME_OUTLINE_50_50_FFFFFE_SVG_ID));
    			break;
    		default:
    			break;
    	}
    	icon.invalidate();

    }

    void GUI_ButtonSetOffState(STATE state, ICON_TYPE type) {

    	this->state = state;
    	switch (type) {
    		case GENERIC_ON_OFF:
    			if (state) {
					GUI_SetTextStatus("On");
					icon.setIconBitmaps(
							Bitmap(BITMAP_ICON_THEME_IMAGES_TOGGLE_TOGGLE_ON_50_50_FFFFFE_SVG_ID),
							Bitmap(BITMAP_ICON_THEME_IMAGES_TOGGLE_TOGGLE_ON_50_50_FFFFFE_SVG_ID));
					interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID));
    			} else {
					GUI_SetTextStatus("Off");
					icon.setIconBitmaps(
							Bitmap(BITMAP_ICON_THEME_IMAGES_TOGGLE_TOGGLE_OFF_50_50_FFFFFE_SVG_ID),
							Bitmap(BITMAP_ICON_THEME_IMAGES_TOGGLE_TOGGLE_OFF_50_50_FFFFFE_SVG_ID));
					interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID));
    			}
    			break;
			case GENERIC_POWER:
				if (state) {
					GUI_SetTextStatus("On");
					icon.setIconBitmaps(
							Bitmap(BITMAP_ICON_THEME_IMAGES_NOTIFICATION_POWER_50_50_FFFFFE_SVG_ID),
							Bitmap(BITMAP_ICON_THEME_IMAGES_NOTIFICATION_POWER_50_50_FFFFFE_SVG_ID));
					interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID));
				} else {
					GUI_SetTextStatus("Off");
					icon.setIconBitmaps(
							Bitmap(BITMAP_ICON_THEME_IMAGES_NOTIFICATION_POWER_OFF_50_50_FFFFFE_SVG_ID),
							Bitmap(BITMAP_ICON_THEME_IMAGES_NOTIFICATION_POWER_OFF_50_50_FFFFFE_SVG_ID));
					interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID));
				}
    			break;
    		case GENERIC_LEVEL:
    		case SENSOR:
    			if (state) {
    				interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID));
    			} else {
    				interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID));
    			}
    			break;
    		case LIGHT_LIGHTNESS:
    			if (state) {
    				interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID));
    			} else {
    				interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID));
    			}
    		case CONFIGURE:
    			break;
    		case UNPROVISION:
    			break;
    		case RENAME:
    			break;
    		default:
    			break;
    	}
    	interactBtn.invalidate();

    }

    STATE GUI_GetButtonState() {

    	return this->state;

    }

    int GUI_GetInstanceID() {

    	return this->instanceID;

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

    STATE state;
    touchgfx::GenericCallback<int>* buttonClickedCallback;
    int instanceID;
    touchgfx::Callback<DeviceButton, const touchgfx::AbstractButton&> buttonCallback;

};

#endif // DEVICEBUTTON_HPP

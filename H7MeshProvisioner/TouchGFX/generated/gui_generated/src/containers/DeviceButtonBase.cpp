/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/DeviceButtonBase.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

DeviceButtonBase::DeviceButtonBase()
{
    setWidth(112);
    setHeight(112);
    interactBtn.setXY(0, 0);
    interactBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSED_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUND_ID));
    add(interactBtn);

    icon.setIconBitmaps(Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_LIGHTBULB_OUTLINE_50_50_FFFFFE_SVG_ID), Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_DONE_50_50_FFFFFE_SVG_ID));
    icon.setIconXY(0, 0);
    icon.setPosition(3, 3, 50, 50);
    add(icon);

    typeText.setXY(13, 56);
    typeText.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    typeText.setLinespacing(0);
    Unicode::snprintf(typeTextBuffer, TYPETEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_CUTS).getText());
    typeText.setWildcard(typeTextBuffer);
    typeText.resizeToCurrentText();
    typeText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_N2HL));
    add(typeText);

    statusText.setXY(13, 78);
    statusText.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    statusText.setLinespacing(0);
    Unicode::snprintf(statusTextBuffer, STATUSTEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_6FPD).getText());
    statusText.setWildcard(statusTextBuffer);
    statusText.resizeToCurrentText();
    statusText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_KOKE));
    add(statusText);
}

DeviceButtonBase::~DeviceButtonBase()
{

}

void DeviceButtonBase::initialize()
{

}

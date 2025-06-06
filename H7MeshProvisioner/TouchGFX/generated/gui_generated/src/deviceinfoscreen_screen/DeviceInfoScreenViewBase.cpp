/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/deviceinfoscreen_screen/DeviceInfoScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

DeviceInfoScreenViewBase::DeviceInfoScreenViewBase()
{
    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    tiledImage1.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_BACKGROUNDS_480X272_RAYS_ID));
    tiledImage1.setPosition(0, 0, 480, 272);
    tiledImage1.setOffset(0, 0);
    add(tiledImage1);

    screenTitleBar1.setXY(0, 0);
    add(screenTitleBar1);

    bottomMenu1.setXY(0, 222);
    add(bottomMenu1);

    elements.setPosition(10, 30, 460, 182);
    elements.setScrollbarsColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    elements.setScrollbarsAlpha(255);
    container1_2.setPosition(0, 100, 460, 40);
    box1_2.setPosition(0, 0, 460, 40);
    box1_2.setColor(touchgfx::Color::getColorFromRGB(73, 86, 101));
    container1_2.add(box1_2);

    commExVerTxt.setXY(10, 11);
    commExVerTxt.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    commExVerTxt.setLinespacing(0);
    Unicode::snprintf(commExVerTxtBuffer, COMMEXVERTXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_MFFZ).getText());
    commExVerTxt.setWildcard(commExVerTxtBuffer);
    commExVerTxt.resizeToCurrentText();
    commExVerTxt.setTypedText(touchgfx::TypedText(T___SINGLEUSE_4AS9));
    container1_2.add(commExVerTxt);

    elements.add(container1_2);

    container1_1.setPosition(0, 50, 460, 40);
    box1_1.setPosition(0, 0, 460, 40);
    box1_1.setColor(touchgfx::Color::getColorFromRGB(73, 86, 101));
    container1_1.add(box1_1);

    libSubVerTxt.setXY(10, 11);
    libSubVerTxt.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    libSubVerTxt.setLinespacing(0);
    Unicode::snprintf(libSubVerTxtBuffer, LIBSUBVERTXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_3GFH).getText());
    libSubVerTxt.setWildcard(libSubVerTxtBuffer);
    libSubVerTxt.resizeToCurrentText();
    libSubVerTxt.setTypedText(touchgfx::TypedText(T___SINGLEUSE_RZQR));
    container1_1.add(libSubVerTxt);

    elements.add(container1_1);

    container1.setPosition(0, 0, 460, 40);
    box1.setPosition(0, 0, 460, 40);
    box1.setColor(touchgfx::Color::getColorFromRGB(73, 86, 101));
    container1.add(box1);

    libVerTxt.setXY(10, 11);
    libVerTxt.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    libVerTxt.setLinespacing(0);
    Unicode::snprintf(libVerTxtBuffer, LIBVERTXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_0RUS).getText());
    libVerTxt.setWildcard(libVerTxtBuffer);
    libVerTxt.resizeToCurrentText();
    libVerTxt.setTypedText(touchgfx::TypedText(T___SINGLEUSE_J0YL));
    container1.add(libVerTxt);

    elements.add(container1);

    add(elements);
}

DeviceInfoScreenViewBase::~DeviceInfoScreenViewBase()
{

}

void DeviceInfoScreenViewBase::setupScreen()
{
    screenTitleBar1.initialize();
    bottomMenu1.initialize();
}

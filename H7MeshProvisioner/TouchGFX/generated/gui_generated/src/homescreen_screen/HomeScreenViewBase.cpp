/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/homescreen_screen/HomeScreenViewBase.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

HomeScreenViewBase::HomeScreenViewBase()
{
    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
    
    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    tiledImage1.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_BACKGROUNDS_480X272_RAYS_ID));
    tiledImage1.setPosition(0, 0, 480, 272);
    tiledImage1.setOffset(0, 0);
    add(tiledImage1);

    container1.setPosition(141, 30, 199, 182);
    box1.setPosition(0, 0, 199, 182);
    box1.setColor(touchgfx::Color::getColorFromRGB(73, 86, 101));
    container1.add(box1);

    textArea1.setXY(60, 131);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_2JW3));
    container1.add(textArea1);

    mainDigitalClock.setPosition(0, 30, 199, 51);
    mainDigitalClock.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    mainDigitalClock.setTypedText(touchgfx::TypedText(T___SINGLEUSE_PGKI));
    mainDigitalClock.displayLeadingZeroForHourIndicator(true);
    mainDigitalClock.setDisplayMode(touchgfx::DigitalClock::DISPLAY_24_HOUR);
    mainDigitalClock.setTime24Hour(0, 0, 0);
    container1.add(mainDigitalClock);

    add(container1);

    bottomMenu1.setXY(0, 222);
    add(bottomMenu1);

    screenTitleBar1.setXY(0, 0);
    add(screenTitleBar1);

    loaderModal.setBackground(touchgfx::BitmapId(BITMAP_H7MODALBACKGROUNDSMALL_ID), 133, 61);
    loaderModal.setShadeColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    loaderModal.setShadeAlpha(100);
    loader1.setXY(7, 0);
    loaderModal.add(loader1);

    add(loaderModal);
}

HomeScreenViewBase::~HomeScreenViewBase()
{
    touchgfx::CanvasWidgetRenderer::resetBuffer();
}

void HomeScreenViewBase::setupScreen()
{
    bottomMenu1.initialize();
    screenTitleBar1.initialize();
    loader1.initialize();
}

void HomeScreenViewBase::goToErrorScreen()
{
    //GUI_SwitchToErrorScreen
    //When goToErrorScreen is called change screen to ErrorScreen
    //Go to ErrorScreen with screen transition towards East
    application().gotoErrorScreenScreenSlideTransitionEast();
}

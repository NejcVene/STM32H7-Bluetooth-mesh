/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/adddevicescreen_screen/AddDeviceScreenViewBase.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

AddDeviceScreenViewBase::AddDeviceScreenViewBase()
{
    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
    
    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    tiledImage1.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_BACKGROUNDS_480X272_RAYS_ID));
    tiledImage1.setPosition(0, 0, 480, 272);
    tiledImage1.setOffset(0, 0);
    add(tiledImage1);

    bottomMenu1.setXY(0, 222);
    add(bottomMenu1);

    scrollableContainer1.setPosition(0, 30, 480, 182);
    scrollableContainer1.setScrollbarsColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    scrollableContainer1.setScrollbarsAlpha(255);
    noNodesFoundMsg.setPosition(10, 10, 460, 60);
    box1.setPosition(0, 0, 460, 60);
    box1.setColor(touchgfx::Color::getColorFromRGB(73, 86, 101));
    noNodesFoundMsg.add(box1);

    noNodesFoundText.setPosition(0, 15, 460, 30);
    noNodesFoundText.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    noNodesFoundText.setLinespacing(0);
    Unicode::snprintf(noNodesFoundTextBuffer, NONODESFOUNDTEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_4EUC).getText());
    noNodesFoundText.setWildcard(noNodesFoundTextBuffer);
    noNodesFoundText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_M1AF));
    noNodesFoundMsg.add(noNodesFoundText);

    scrollableContainer1.add(noNodesFoundMsg);

    add(scrollableContainer1);

    screenTitleBar1.setXY(0, 0);
    add(screenTitleBar1);

    modalWindow1.setBackground(touchgfx::BitmapId(BITMAP_H7MODALBACKGROUNDSMALL_ID), 133, 61);
    modalWindow1.setShadeColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    modalWindow1.setShadeAlpha(100);
    modalWindow1.hide();
    loader1.setXY(7, 0);
    modalWindow1.add(loader1);

    add(modalWindow1);
}

AddDeviceScreenViewBase::~AddDeviceScreenViewBase()
{
    touchgfx::CanvasWidgetRenderer::resetBuffer();
}

void AddDeviceScreenViewBase::setupScreen()
{
    bottomMenu1.initialize();
    screenTitleBar1.initialize();
    loader1.initialize();
}

void AddDeviceScreenViewBase::goToDeviceConfigScreen()
{
    //GUI_SwitchScreenToDeviceConfig
    //When goToDeviceConfigScreen is called change screen to ConfigureNodeScreen
    //Go to ConfigureNodeScreen with screen transition towards East
    application().gotoConfigureNodeScreenScreenSlideTransitionEast();
}

void AddDeviceScreenViewBase::goToErrorScreen()
{
    //GUI_SwitchToErrorScreen
    //When goToErrorScreen is called change screen to ErrorScreen
    //Go to ErrorScreen with screen transition towards East
    application().gotoErrorScreenScreenSlideTransitionEast();
}

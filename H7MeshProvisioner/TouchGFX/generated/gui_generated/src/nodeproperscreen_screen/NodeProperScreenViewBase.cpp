/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/nodeproperscreen_screen/NodeProperScreenViewBase.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>

NodeProperScreenViewBase::NodeProperScreenViewBase()
{
    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
    
    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    tiledImage1.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_BACKGROUNDS_480X272_RAYS_ID));
    tiledImage1.setPosition(0, 0, 480, 272);
    tiledImage1.setOffset(0, 0);
    add(tiledImage1);

    nodeOptions.setPosition(0, 30, 480, 182);
    nodeOptions.setScrollbarsColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    nodeOptions.setScrollbarsAlpha(255);
    add(nodeOptions);

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

NodeProperScreenViewBase::~NodeProperScreenViewBase()
{
    touchgfx::CanvasWidgetRenderer::resetBuffer();
}

void NodeProperScreenViewBase::setupScreen()
{
    bottomMenu1.initialize();
    screenTitleBar1.initialize();
    loader1.initialize();
}

void NodeProperScreenViewBase::goToConfigureScreen()
{
    //GUI_SwitchToNodeConfigScreen
    //When goToConfigureScreen is called change screen to ConfigureNodeScreen
    //Go to ConfigureNodeScreen with screen transition towards East
    application().gotoConfigureNodeScreenScreenSlideTransitionEast();
}

void NodeProperScreenViewBase::goToScreen1()
{
    //GUI_SwitchToScreen1
    //When goToScreen1 is called change screen to Screen1
    //Go to Screen1 with screen transition towards East
    application().gotoScreen1ScreenSlideTransitionEast();
}

void NodeProperScreenViewBase::goToNodeSensorsScreen()
{
    //GUI_SwitchToNodeSensorsScreen
    //When goToNodeSensorsScreen is called change screen to NodeSesnsorsScreen
    //Go to NodeSesnsorsScreen with screen transition towards East
    application().gotoNodeSesnsorsScreenScreenSlideTransitionEast();
}

void NodeProperScreenViewBase::goToRenameScreen()
{
    //GUI_SwitchToRenameScreen
    //When goToRenameScreen is called change screen to RenameScreen
    //Go to RenameScreen with screen transition towards East
    application().gotoRenameScreenScreenSlideTransitionEast();
}

void NodeProperScreenViewBase::goToErrorScreen()
{
    //GUI_SwitchToErrorScreen
    //When goToErrorScreen is called change screen to ErrorScreen
    //Go to ErrorScreen with screen transition towards East
    application().gotoErrorScreenScreenSlideTransitionEast();
}

void NodeProperScreenViewBase::goToLevelSetScreen()
{
    //GUI_SwitchToLevelSetScreen
    //When goToLevelSetScreen is called change screen to NodeLevelSetScreen
    //Go to NodeLevelSetScreen with screen transition towards East
    application().gotoNodeLevelSetScreenScreenSlideTransitionEast();
}

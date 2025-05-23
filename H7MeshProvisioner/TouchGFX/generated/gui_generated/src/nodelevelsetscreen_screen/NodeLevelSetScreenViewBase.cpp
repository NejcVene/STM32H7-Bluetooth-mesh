/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/nodelevelsetscreen_screen/NodeLevelSetScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

NodeLevelSetScreenViewBase::NodeLevelSetScreenViewBase() :
    buttonCallback(this, &NodeLevelSetScreenViewBase::buttonCallbackHandler),
    sliderValueChangedCallback(this, &NodeLevelSetScreenViewBase::sliderValueChangedCallbackHandler)
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

    levelSetSlider.setXY(74, 120);
    levelSetSlider.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_SLIDER_HORIZONTAL_THICK_TRACK_MEDIUM_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_SLIDER_HORIZONTAL_THICK_FILLER_MEDIUM_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_SLIDER_HORIZONTAL_THICK_ROUNDED_DARK_ID));
    levelSetSlider.setupHorizontalSlider(16, 11, 0, 0, 300);
    levelSetSlider.setValueRange(0, 100);
    levelSetSlider.setValue(0);
    levelSetSlider.setNewValueCallback(sliderValueChangedCallback);
    add(levelSetSlider);

    box1.setPosition(191, 33, 99, 80);
    box1.setColor(touchgfx::Color::getColorFromRGB(73, 86, 101));
    add(box1);

    valueSetText.setPosition(191, 62, 99, 51);
    valueSetText.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    valueSetText.setLinespacing(0);
    Unicode::snprintf(valueSetTextBuffer, VALUESETTEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_CRF2).getText());
    valueSetText.setWildcard(valueSetTextBuffer);
    valueSetText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_RH3W));
    add(valueSetText);

    valueText.setXY(215, 42);
    valueText.setColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    valueText.setLinespacing(0);
    valueText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_FIW9));
    add(valueText);

    levelSetBtn.setXY(190, 162);
    levelSetBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDPRESSEDTINY_ID), touchgfx::Bitmap(BITMAP_BUTTONBACKGROUNDTINY_ID));
    levelSetBtn.setLabelText(touchgfx::TypedText(T___SINGLEUSE_5E0R));
    levelSetBtn.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 254));
    levelSetBtn.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    levelSetBtn.setAction(buttonCallback);
    add(levelSetBtn);
}

NodeLevelSetScreenViewBase::~NodeLevelSetScreenViewBase()
{

}

void NodeLevelSetScreenViewBase::setupScreen()
{
    screenTitleBar1.initialize();
    bottomMenu1.initialize();
}

void NodeLevelSetScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &levelSetBtn)
    {
        //GUI_LevelSet
        //When levelSetBtn clicked call virtual function
        //Call GUI_LevelSet
        GUI_LevelSet();
    }
}

void NodeLevelSetScreenViewBase::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value)
{
    if (&src == &levelSetSlider)
    {
        //GUI_ChangeLevelValueText
        //When levelSetSlider value changed call virtual function
        //Call GUI_ChangeLevelValueText
        GUI_ChangeLevelValueText(value);
    }
}

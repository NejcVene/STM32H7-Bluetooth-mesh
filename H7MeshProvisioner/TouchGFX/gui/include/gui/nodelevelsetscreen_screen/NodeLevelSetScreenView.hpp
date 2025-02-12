#ifndef NODELEVELSETSCREENVIEW_HPP
#define NODELEVELSETSCREENVIEW_HPP

#include <gui_generated/nodelevelsetscreen_screen/NodeLevelSetScreenViewBase.hpp>
#include <gui/nodelevelsetscreen_screen/NodeLevelSetScreenPresenter.hpp>

class NodeLevelSetScreenView : public NodeLevelSetScreenViewBase
{
public:
    NodeLevelSetScreenView();
    virtual ~NodeLevelSetScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void GUI_LevelSet();
    virtual void GUI_ChangeLevelValueText(int value);

    int16_t MapSliderToGenericLevelRange(int sliderValue);

    void GUI_SetTextSliderValue(int value) {

    	Unicode::snprintf(valueSetTextBuffer, VALUESETTEXT_SIZE, "%i", value);
    	valueSetText.invalidate();

    }

    void GUI_SetSliderValue(int16_t value) {

    	levelSetSlider.setValue(value);

    }

    int GUI_GetSliderValue() {

    	return levelSetSlider.getValue();

    }

protected:
    Node_Config_t *node;
    CMD_CommandGet_t *cmd;
};

#endif // NODELEVELSETSCREENVIEW_HPP

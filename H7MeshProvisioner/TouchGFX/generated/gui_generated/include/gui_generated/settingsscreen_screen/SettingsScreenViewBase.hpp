/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SETTINGSSCREENVIEWBASE_HPP
#define SETTINGSSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/settingsscreen_screen/SettingsScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TiledImage.hpp>
#include <gui/containers/ScreenTitleBar.hpp>
#include <gui/containers/BottomMenu.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>

class SettingsScreenViewBase : public touchgfx::View<SettingsScreenPresenter>
{
public:
    SettingsScreenViewBase();
    virtual ~SettingsScreenViewBase();
    virtual void setupScreen();

    /*
     * Custom Actions
     */
    virtual void goToDevInfo();

    /*
     * Virtual Action Handlers
     */
    virtual void GUI_GetDevInfo()
    {
        // Override and implement this function in SettingsScreen
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::TiledImage tiledImage1;
    ScreenTitleBar screenTitleBar1;
    BottomMenu bottomMenu1;
    touchgfx::ScrollableContainer settingsContainer;
    touchgfx::TextButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger >  >  deviceInfoBtn;
    touchgfx::TextButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger >  >  debugBtn;
    touchgfx::TextButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger >  >  setClockBtn;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<SettingsScreenViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // SETTINGSSCREENVIEWBASE_HPP

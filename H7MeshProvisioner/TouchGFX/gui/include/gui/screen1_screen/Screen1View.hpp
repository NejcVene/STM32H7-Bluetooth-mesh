#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void GUI_ShowAllDevices();
    virtual void GUI_ShowKitchenDevices();
    virtual void GUI_ShowLivingRoomDevices();
    virtual void GUI_ShowBedroomDevices();
    virtual void GUI_ShowBathroomDevices();

protected:
private:
    touchgfx::Callback<Screen1View, int> buttonClickCallback;
    void handleButtonClicked(int instanceID);
//    Node_Config_t *nodes;
};

#endif // SCREEN1VIEW_HPP

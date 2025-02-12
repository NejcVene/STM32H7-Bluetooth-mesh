#ifndef RENAMESCREENVIEW_HPP
#define RENAMESCREENVIEW_HPP

#include <gui_generated/renamescreen_screen/RenameScreenViewBase.hpp>
#include <gui/renamescreen_screen/RenameScreenPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>

class RenameScreenView : public RenameScreenViewBase
{
public:
    RenameScreenView();
    virtual ~RenameScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void GUI_SaveRename();

protected:

    Node_Config_t *node;
    CustomKeyboard cKeyboard;

};

#endif // RENAMESCREENVIEW_HPP

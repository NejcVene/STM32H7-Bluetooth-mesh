#ifndef BOTTOMMENU_HPP
#define BOTTOMMENU_HPP

#include <gui_generated/containers/BottomMenuBase.hpp>

class BottomMenu : public BottomMenuBase
{
public:
    BottomMenu();
    virtual ~BottomMenu() {}

    virtual void initialize();
protected:
};

#endif // BOTTOMMENU_HPP

#ifndef NODEPROPERSCREENVIEW_HPP
#define NODEPROPERSCREENVIEW_HPP

#include <gui_generated/nodeproperscreen_screen/NodeProperScreenViewBase.hpp>
#include <gui/nodeproperscreen_screen/NodeProperScreenPresenter.hpp>

class NodeProperScreenView : public NodeProperScreenViewBase
{
public:
    NodeProperScreenView();
    virtual ~NodeProperScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // NODEPROPERSCREENVIEW_HPP

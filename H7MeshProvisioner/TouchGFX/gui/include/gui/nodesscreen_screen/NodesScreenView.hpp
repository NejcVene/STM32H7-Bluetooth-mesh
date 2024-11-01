#ifndef NODESSCREENVIEW_HPP
#define NODESSCREENVIEW_HPP

#include <gui_generated/nodesscreen_screen/NodesScreenViewBase.hpp>
#include <gui/nodesscreen_screen/NodesScreenPresenter.hpp>

class NodesScreenView : public NodesScreenViewBase
{
public:
    NodesScreenView();
    virtual ~NodesScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // NODESSCREENVIEW_HPP

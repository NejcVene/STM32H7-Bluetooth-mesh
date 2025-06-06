/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef FRONTENDHEAPBASE_HPP
#define FRONTENDHEAPBASE_HPP

#include <common/Meta.hpp>
#include <common/Partition.hpp>
#include <mvp/MVPHeap.hpp>

#include <touchgfx/transitions/NoTransition.hpp>
#include <touchgfx/transitions/SlideTransition.hpp>

#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>

#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <gui/homescreen_screen/HomeScreenView.hpp>
#include <gui/homescreen_screen/HomeScreenPresenter.hpp>
#include <gui/adddevicescreen_screen/AddDeviceScreenView.hpp>
#include <gui/adddevicescreen_screen/AddDeviceScreenPresenter.hpp>
#include <gui/configurenodescreen_screen/ConfigureNodeScreenView.hpp>
#include <gui/configurenodescreen_screen/ConfigureNodeScreenPresenter.hpp>
#include <gui/debugscreen_screen/DebugScreenView.hpp>
#include <gui/debugscreen_screen/DebugScreenPresenter.hpp>
#include <gui/nodesscreen_screen/NodesScreenView.hpp>
#include <gui/nodesscreen_screen/NodesScreenPresenter.hpp>
#include <gui/nodeproperscreen_screen/NodeProperScreenView.hpp>
#include <gui/nodeproperscreen_screen/NodeProperScreenPresenter.hpp>
#include <gui/settingsscreen_screen/SettingsScreenView.hpp>
#include <gui/settingsscreen_screen/SettingsScreenPresenter.hpp>
#include <gui/clocksettingsscreen_screen/ClockSettingsScreenView.hpp>
#include <gui/clocksettingsscreen_screen/ClockSettingsScreenPresenter.hpp>
#include <gui/deviceinfoscreen_screen/DeviceInfoScreenView.hpp>
#include <gui/deviceinfoscreen_screen/DeviceInfoScreenPresenter.hpp>
#include <gui/nodesesnsorsscreen_screen/NodeSesnsorsScreenView.hpp>
#include <gui/nodesesnsorsscreen_screen/NodeSesnsorsScreenPresenter.hpp>
#include <gui/renamescreen_screen/RenameScreenView.hpp>
#include <gui/renamescreen_screen/RenameScreenPresenter.hpp>
#include <gui/errorscreen_screen/ErrorScreenView.hpp>
#include <gui/errorscreen_screen/ErrorScreenPresenter.hpp>
#include <gui/nodelevelsetscreen_screen/NodeLevelSetScreenView.hpp>
#include <gui/nodelevelsetscreen_screen/NodeLevelSetScreenPresenter.hpp>


/**
 * This class provides the memory that shall be used for memory allocations
 * in the frontend. A single instance of the FrontendHeap is allocated once (in heap
 * memory), and all other frontend objects such as views, presenters and data model are
 * allocated within the scope of this FrontendHeap. As such, the RAM usage of the entire
 * user interface is sizeof(FrontendHeap).
 *
 * @note The FrontendHeap reserves memory for the most memory-consuming presenter and
 * view only. The largest of these classes are determined at compile-time using template
 * magic. As such, it is important to add all presenters, views and transitions to the
 * type lists in this class.
 *
 */
class FrontendHeapBase : public touchgfx::MVPHeap
{
public:
    /**
     * A list of all view types. Must end with meta::Nil.
     * @note All view types used in the application MUST be added to this list!
     */
    typedef touchgfx::meta::TypeList< Screen1View,
            touchgfx::meta::TypeList< HomeScreenView,
            touchgfx::meta::TypeList< AddDeviceScreenView,
            touchgfx::meta::TypeList< ConfigureNodeScreenView,
            touchgfx::meta::TypeList< DebugScreenView,
            touchgfx::meta::TypeList< NodesScreenView,
            touchgfx::meta::TypeList< NodeProperScreenView,
            touchgfx::meta::TypeList< SettingsScreenView,
            touchgfx::meta::TypeList< ClockSettingsScreenView,
            touchgfx::meta::TypeList< DeviceInfoScreenView,
            touchgfx::meta::TypeList< NodeSesnsorsScreenView,
            touchgfx::meta::TypeList< RenameScreenView,
            touchgfx::meta::TypeList< ErrorScreenView,
            touchgfx::meta::TypeList< NodeLevelSetScreenView,
            touchgfx::meta::Nil > > > > > > > > > > > > >
            > GeneratedViewTypes;

    /**
     * Determine (compile time) the View type of largest size.
     */
    typedef touchgfx::meta::select_type_maxsize< GeneratedViewTypes >::type MaxGeneratedViewType;

    /**
     * A list of all presenter types. Must end with meta::Nil.
     * @note All presenter types used in the application MUST be added to this list!
     */
    typedef touchgfx::meta::TypeList< Screen1Presenter,
            touchgfx::meta::TypeList< HomeScreenPresenter,
            touchgfx::meta::TypeList< AddDeviceScreenPresenter,
            touchgfx::meta::TypeList< ConfigureNodeScreenPresenter,
            touchgfx::meta::TypeList< DebugScreenPresenter,
            touchgfx::meta::TypeList< NodesScreenPresenter,
            touchgfx::meta::TypeList< NodeProperScreenPresenter,
            touchgfx::meta::TypeList< SettingsScreenPresenter,
            touchgfx::meta::TypeList< ClockSettingsScreenPresenter,
            touchgfx::meta::TypeList< DeviceInfoScreenPresenter,
            touchgfx::meta::TypeList< NodeSesnsorsScreenPresenter,
            touchgfx::meta::TypeList< RenameScreenPresenter,
            touchgfx::meta::TypeList< ErrorScreenPresenter,
            touchgfx::meta::TypeList< NodeLevelSetScreenPresenter,
            touchgfx::meta::Nil > > > > > > > > > > > > >
            > GeneratedPresenterTypes;

    /**
     * Determine (compile time) the Presenter type of largest size.
     */
    typedef touchgfx::meta::select_type_maxsize< GeneratedPresenterTypes >::type MaxGeneratedPresenterType;

    /**
     * A list of all transition types. Must end with meta::Nil.
     * @note All transition types used in the application MUST be added to this list!
     */
    typedef touchgfx::meta::TypeList< touchgfx::NoTransition,
            touchgfx::meta::TypeList< SlideTransition<EAST>,
            touchgfx::meta::Nil >
            > GeneratedTransitionTypes;

    /**
     * Determine (compile time) the Transition type of largest size.
     */
    typedef touchgfx::meta::select_type_maxsize< GeneratedTransitionTypes >::type MaxGeneratedTransitionType;

    virtual void gotoStartScreen(FrontendApplication& app)
    {
        app.gotoHomeScreenScreenNoTransition();
    }
protected:
    FrontendHeapBase(touchgfx::AbstractPartition& presenters, touchgfx::AbstractPartition& views, touchgfx::AbstractPartition& transitions, FrontendApplication& app)
        : MVPHeap(presenters, views, transitions, app)
    {

    }

};

#endif // FRONTENDHEAPBASE_HPP

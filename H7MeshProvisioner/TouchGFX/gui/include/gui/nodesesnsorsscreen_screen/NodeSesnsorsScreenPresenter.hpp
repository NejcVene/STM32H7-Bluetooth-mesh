#ifndef NODESESNSORSSCREENPRESENTER_HPP
#define NODESESNSORSSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class NodeSesnsorsScreenView;

class NodeSesnsorsScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    NodeSesnsorsScreenPresenter(NodeSesnsorsScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~NodeSesnsorsScreenPresenter() {}

    virtual void GUI_UpdateSensorValues();

    void GUI_SendCommand(CMD_CommandGet_t *cmd) {

    	model->GUI_SendCommand(cmd);

    }

    uint32_t GUI_GetNodeAddress() {

    	return model->GUI_GetNodeAddress();

    }

    int GUI_GetSensorIndex() {

    	return model->GUI_GetSensorIndex();

    }

    Sensor_MeasuredData_t *GUI_GetSensorMeasuredData() {

    	return model->GUI_GetSensorMeasuredData();

    }

private:
    NodeSesnsorsScreenPresenter();

    NodeSesnsorsScreenView& view;
};

#endif // NODESESNSORSSCREENPRESENTER_HPP

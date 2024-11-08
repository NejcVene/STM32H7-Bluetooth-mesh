#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void GUI_DevicesFound() {};
    virtual void GUI_ChangeScreen() {};
    virtual void GUI_ConfigureDevice() {};
    virtual void GUI_ConfShowPopup() {};
    virtual void GUI_ChangeScreenAfterUnprov() {};

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP

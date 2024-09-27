#ifndef DEVICETOGGLE_HPP
#define DEVICETOGGLE_HPP

#include <gui_generated/containers/DeviceToggleBase.hpp>

class DeviceToggle : public DeviceToggleBase
{
public:
    DeviceToggle();
    virtual ~DeviceToggle() {}

    virtual void initialize();
protected:
};

#endif // DEVICETOGGLE_HPP

#ifndef XENOMODEBASE_H
#define XENOMODEBASE_H

class XenoCore;
class XenoVehicleBase;

class XenoModeBase {
public:
    XenoModeBase(XenoCore& core, XenoVehicleBase& vehicle)
        : m_core(core), m_vehicle(vehicle) {
    }
    virtual ~XenoModeBase() {}

    virtual void SendCtrl() = 0;

protected:
    XenoCore& m_core;
    XenoVehicleBase& m_vehicle;
};

#endif
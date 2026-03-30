#ifndef XENOMODEBASE_H
#define XENOMODEBASE_H

class XenoCore;
class XenoVehicleFixedWing;

class XenoModeBase {
public:
    XenoModeBase(XenoCore& core, XenoVehicleFixedWing& vehicle)
        : m_core(core), m_vehicle(vehicle) {
    }
    virtual ~XenoModeBase() {}

    virtual void SendCtrl() = 0;

protected:
    XenoCore& m_core;
    XenoVehicleFixedWing& m_vehicle;
};

#endif
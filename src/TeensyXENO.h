#ifndef TEENSYXENO_H
#define TEENSYXENO_H

#include "core/XenoTypes.h"
#include "core/XenoCore.h"
#include "vehicle/XenoVehicleBase.h"
#include "vehicle/FixedWing/XenoVehicleFixedWing.h"
#include "vehicle/RcCar/XenoVehicleRcCar.h"
#include "mode/HILS/XenoModeHILS.h"

class TeensyXENO {
public:
    TeensyXENO(int nTeensyVer, XenoVehicleType vehicleType);
    ~TeensyXENO();

    void GetSBUS();
    void SendSBUS();
    void SendPWM();
    void GetXenoController();
    void SendXenoController();
    void HILS_SendCtrl();

    XenoVehicleType GetVehicleType() const;

    XenoVehicleFixedWing* GetFixedWing();
    XenoVehicleRcCar* GetRcCar();

    XenoCore m_core;

private:
    XenoVehicleBase* m_vehicle;
    XenoModeHILS* m_mode;
    XenoVehicleType m_vehicleType;
};

#endif
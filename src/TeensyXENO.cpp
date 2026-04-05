#include "TeensyXENO.h"

TeensyXENO::TeensyXENO(int nTeensyVer, XenoVehicleType vehicleType)
    : m_core(nTeensyVer),
    m_vehicle(nullptr),
    m_mode(nullptr),
    m_vehicleType(vehicleType)
{
    if (vehicleType == XENO_VEHICLE_FIXEDWING) {
        m_vehicle = new XenoVehicleFixedWing(m_core);
    }
    else if (vehicleType == XENO_VEHICLE_RCCAR) {
        m_vehicle = new XenoVehicleRcCar(m_core);
    }
    else {
        m_vehicle = new XenoVehicleFixedWing(m_core);
        m_vehicleType = XENO_VEHICLE_FIXEDWING;
    }

    m_vehicle->InitVehicleValue();
    m_mode = new XenoModeHILS(m_core, *m_vehicle);
}

TeensyXENO::~TeensyXENO()
{
    delete m_mode;
    delete m_vehicle;
}

void TeensyXENO::GetSBUS()
{
    m_core.GetSBUS();
}

void TeensyXENO::SendSBUS()
{
    m_core.SendSBUS();
}

void TeensyXENO::SendPWM()
{
    m_core.SendPWM();
}

void TeensyXENO::GetXenoController()
{
    if (m_vehicle) {
        m_vehicle->GetXenoController();
    }
}

void TeensyXENO::SendXenoController()
{
    if (m_vehicle) {
        m_vehicle->SendXenoController();
    }
}

void TeensyXENO::HILS_SendCtrl()
{
    if (m_mode) {
        m_mode->SendCtrl();
    }
}

XenoVehicleType TeensyXENO::GetVehicleType() const
{
    return m_vehicleType;
}

XenoVehicleFixedWing* TeensyXENO::GetFixedWing()
{
    if (m_vehicleType == XENO_VEHICLE_FIXEDWING) {
        return static_cast<XenoVehicleFixedWing*>(m_vehicle);
    }
    return nullptr;
}

XenoVehicleRcCar* TeensyXENO::GetRcCar()
{
    if (m_vehicleType == XENO_VEHICLE_RCCAR) {
        return static_cast<XenoVehicleRcCar*>(m_vehicle);
    }
    return nullptr;
}
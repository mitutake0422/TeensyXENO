#include "TeensyXENO.h"

TeensyXENO::TeensyXENO(int nTeensyVer)
    : m_core(nTeensyVer),
    m_vehicle(m_core),
    m_mode(m_core, m_vehicle),

    m_stSensor(m_core.m_stSensor),
    m_stNavi(m_core.m_stNavi),
    m_stSbusRcv(m_core.m_stSbusRcv),
    m_stSbusCmd(m_core.m_stSbusCmd),
    m_stDIP(m_core.m_stDIP),
    m_nCtrl(m_core.m_nCtrl),

    m_stCommand(m_vehicle.m_stCommand),
    m_stFltPlan(m_vehicle.m_stFltPlan),
    m_stParam(m_vehicle.m_stParam),
    m_stControl(m_vehicle.m_stControl)
{
    m_vehicle.InitVehicleValue();
}

TeensyXENO::~TeensyXENO()
{
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
    m_vehicle.GetXenoController();
}

void TeensyXENO::SendXenoController()
{
    m_vehicle.SendXenoController();
}

void TeensyXENO::HILS_SendCtrl()
{
    m_mode.SendCtrl();
}
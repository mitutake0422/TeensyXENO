#ifndef TEENSYXENO_H
#define TEENSYXENO_H

#include "XenoTypes.h"
#include "XenoCore.h"
#include "XenoVehicleFixedWing.h"
#include "XenoModeHILS.h"

class TeensyXENO {
public:
    explicit TeensyXENO(int nTeensyVer);
    ~TeensyXENO();

    void GetSBUS();
    void SendSBUS();
    void SendPWM();
    void GetXenoController();
    void SendXenoController();
    void HILS_SendCtrl();

    XenoCore m_core;
    XenoVehicleFixedWing m_vehicle;
    XenoModeHILS m_mode;

    // å›ä∑ópéQè∆
    _stSensor& m_stSensor;
    _stNavi& m_stNavi;
    _stSBUS& m_stSbusRcv;
    _stSBUS& m_stSbusCmd;
    _stDIP& m_stDIP;
    int& m_nCtrl;

    _stCommand& m_stCommand;
    _stFltPlan& m_stFltPlan;
    _stParam& m_stParam;
    _stControl& m_stControl;
};

#endif
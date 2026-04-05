#ifndef XENOVEHICLEFIXEDWING_H
#define XENOVEHICLEFIXEDWING_H

#include "FixedWingTypes.h"
#include "../XenoVehicleBase.h"

class XenoVehicleFixedWing : public XenoVehicleBase {
public:
    explicit XenoVehicleFixedWing(XenoCore& core);

    void InitVehicleValue() override;
    void GetXenoController() override;
    void SendXenoController() override;
    XenoVehicleType GetVehicleType() const override;

    _stSensor_FixedWing  m_stSensor;
    _stNavi_FixedWing    m_stNavi;
    _stCommand_FixedWing m_stCommand;
    _stParam_FixedWing   m_stParam;
    _stControl_FixedWing m_stControl;
};

#endif
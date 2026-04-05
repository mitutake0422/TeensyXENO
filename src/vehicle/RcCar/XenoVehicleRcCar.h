#ifndef XENOVEHICLERCCAR_H
#define XENOVEHICLERCCAR_H

#include "RcCarTypes.h"
#include "../XenoVehicleBase.h"

class XenoVehicleRcCar : public XenoVehicleBase {
public:
    explicit XenoVehicleRcCar(XenoCore& core);

    void InitVehicleValue() override;
    void GetXenoController() override;
    void SendXenoController() override;
    XenoVehicleType GetVehicleType() const override;

    _stSensor_RcCar  m_stSensor;
    _stNavi_RcCar    m_stNavi;
    _stCommand_RcCar m_stCommand;
    _stParam_RcCar   m_stParam;
    _stControl_RcCar m_stControl;
};

#endif
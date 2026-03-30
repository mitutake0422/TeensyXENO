#ifndef XENOVEHICLEFIXEDWING_H
#define XENOVEHICLEFIXEDWING_H

#include "XenoTypes.h"
#include "XenoVehicleBase.h"

class XenoVehicleFixedWing : public XenoVehicleBase {
public:
    explicit XenoVehicleFixedWing(XenoCore& core);

    void InitVehicleValue() override;
    void GetXenoController() override;
    void SendXenoController() override;

    _stCommand m_stCommand;
    _stFltPlan m_stFltPlan;
    _stParam   m_stParam;
    _stControl m_stControl;
};

#endif
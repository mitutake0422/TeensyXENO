#ifndef XENOVEHICLEBASE_H
#define XENOVEHICLEBASE_H

class XenoCore;

class XenoVehicleBase {
public:
    explicit XenoVehicleBase(XenoCore& core) : m_core(core) {}
    virtual ~XenoVehicleBase() {}

    virtual void InitVehicleValue() = 0;
    virtual void GetXenoController() = 0;
    virtual void SendXenoController() = 0;

protected:
    XenoCore& m_core;
};

#endif
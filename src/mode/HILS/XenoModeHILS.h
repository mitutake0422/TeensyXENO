#ifndef XENOMODEHILS_H
#define XENOMODEHILS_H

#include "../XenoModeBase.h"

class XenoCore;
class XenoVehicleBase;

class XenoModeHILS : public XenoModeBase {
public:
    XenoModeHILS(XenoCore& core, XenoVehicleBase& vehicle);

    void SendCtrl() override;
};

#endif
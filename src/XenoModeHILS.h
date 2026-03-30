#ifndef XENOMODEHILS_H
#define XENOMODEHILS_H

#include "XenoModeBase.h"

class XenoModeHILS : public XenoModeBase {
public:
    XenoModeHILS(XenoCore& core, XenoVehicleFixedWing& vehicle);

    void SendCtrl() override;
};

#endif
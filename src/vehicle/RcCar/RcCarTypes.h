#ifndef RCCARTYPES_H
#define RCCARTYPES_H

struct _stSensor_RcCar
{
    double dLat;
    double dLon;
    double dVel;
    unsigned char uchQuality;
    unsigned char uchHour;
    unsigned char uchMin;
    unsigned char uchSec;
    unsigned char uchNSV;
    double dPsi;
    double dRotZ;
};

struct _stNavi_RcCar
{
    double dXTE;
    double dClosestLat;
    double dClosestLon;
    double dT;
    double dDistPerLat;
    double dDistPerLon;
};

struct _stCommand_RcCar
{
    double dPsiCmd;
    double dVelCmd;
};

struct _stParam_RcCar
{
    unsigned short ushStrKp;
    unsigned short ushStrKi;
    unsigned short ushAccKp;
    unsigned short ushAccKi;

    double dStrKp;
    double dStrKi;
    double dAccKp;
    double dAccKi;
};

struct _stControl_RcCar
{
    double dStrIntg;
    double dAccIntg;
    double dStrTrim;
    double dAccTrim;
};

#endif
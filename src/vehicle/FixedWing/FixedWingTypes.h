#ifndef FIXEDWINGTYPES_H
#define FIXEDWINGTYPES_H

struct _stSensor_FixedWing
{
    double dAccX;
    double dAccY;
    double dAccZ;
    double dRotX;
    double dRotY;
    double dRotZ;
    double dPhi;
    double dTht;
    double dPsi;
    double dLat;
    double dLon;
    double dAlt;
    double dGS;
    double dCOG;
    unsigned char uchHour;
    unsigned char uchMin;
    unsigned char uchSec;
    unsigned char uchNSV;
    unsigned char uchQuality;
    double dPs;
    double dPd;
    double dVS;
};

struct _stNavi_FixedWing
{
    double dAltP;
    double dIAS;

    double dXTE;
    double dClosestLat;
    double dClosestLon;
    double dT;
    double dDistPerLat;
    double dDistPerLon;

    unsigned short ushTcAltP;
    unsigned short ushAltG;
    unsigned short ushPressG;
    unsigned short ushTemprG;

    double dTcAltP;
    double dAltG;
    double dPressG;
    double dtemprG;
};

struct _stCommand_FixedWing
{
    double dPhiCmd;
    double dThtCmd;
    double dPsiCmd;
    double dVelCmd;
    double dAltCmd;
    double dVsCmd;
};

struct _stParam_FixedWing
{
    unsigned short ushElvKp;
    unsigned short ushElvKi;
    unsigned short ushElvKd;
    unsigned short ushElvLimit;
    unsigned short ushThtCmd;

    unsigned short ushAilKp;
    unsigned short ushAilKi;
    unsigned short ushAilKd;
    unsigned short ushAilLimit;
    unsigned short ushPhiCmd;

    unsigned short ushAltKp;  // ALT err Å®VS cmd

    unsigned short ushThtKp;  // VScmd Å® THT cmd
    unsigned short ushThtKi;
    unsigned short ushThtULimit;
    unsigned short ushThtLLimit;
    unsigned short ushAltCmd;

    unsigned short ushPhiKp;
    unsigned short ushPhiKi;
    unsigned short ushPhiLimit;
    unsigned short ushLookAheadDist;
    unsigned short ushPsiCmd;

    double dElvKp;
    double dElvKi;
    double dElvKd;
    double dElvLimit;
    double dThtCmd;

    double dAilKp;
    double dAilKi;
    double dAilKd;
    double dAilLimit;
    double dPhiCmd;

    double dAltKp;
    double dThtKp;
    double dThtKi;
    double dThtULimit;
    double dThtLLimit;
    double dAltCmd;

    double dPhiKp;
    double dPhiKi;
    double dPhiLimit;
    double dLookAheadDist;
    double dPsiCmd;
};

struct _stControl_FixedWing
{
    double dThtIntg;
    double dVsIntg;
    double dElvTrim;
    double dPhiIntg;
    double dAilTrim;
    double dThtTrim;
    double dPsiIntg;
};

#endif
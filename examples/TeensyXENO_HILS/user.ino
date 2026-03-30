#include <math.h>

#define DTIME		0.02	// [sec]	計算時間間隔

/*
#define MANUAL    0
#define AUTO      1

struct _stSensor
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

struct _stNavi
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

struct _stSBUS
{
    unsigned short ushCH1;
    unsigned short ushCH2;
    unsigned short ushCH3;
    unsigned short ushCH4;
    unsigned short ushCH5;
    unsigned short ushCH6;
    unsigned short ushCH7;
    unsigned short ushCH8;
    unsigned short ushCH9;
    unsigned short ushCH10;
    unsigned short ushCH11;
    unsigned short ushCH12;
    unsigned short ushCH13;
    unsigned short ushCH14;
    unsigned short ushCH15;
    unsigned short ushCH16;
    unsigned char uchD1;
    unsigned char uchD2;
    unsigned char uchFrameLost;
    unsigned char uchFailSafe;

    unsigned char uchSBUS23;
};

struct _stCommand
{
    double dPhiCmd;
    double dThtCmd;
    double dPsiCmd;
    double dVelCmd;
    double dAltCmd;
    double dVsCmd;
};

struct _stWP
{
    double dWpLat;
    double dWpLon;
    double dWpAlt;
    double dWpPass;
    unsigned char uchFlg;
};

struct _stFltPlan
{
    _stWP stWP[NUM_WP];
    int nWpTo;
    int nWpNum;
};

struct _stParam
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

    unsigned short ushAltKp;  // ALT err →VS cmd

    unsigned short ushThtKp;  // VScmd → THT cmd
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

struct _stDIP
{
    unsigned char uchDIP1;
    unsigned char uchDIP2;
    unsigned char uchDIP3;
    unsigned char uchDIP4;
};

struct _stControl
{
    double dThtIntg;
    double dVsIntg;
    double dElvTrim;
    double dPhiIntg;
    double dAilTrim;
    double dThtTrim;
};
*/

void userfunc() {

    if (g_clTX.m_nCtrl == MANUAL) {
        // manual
                                    
        g_clTX.m_stSbusCmd.ushCH1 = g_clTX.m_stSbusRcv.ushCH1;
        g_clTX.m_stSbusCmd.ushCH2 = g_clTX.m_stSbusRcv.ushCH2;
        g_clTX.m_stSbusCmd.ushCH3 = g_clTX.m_stSbusRcv.ushCH3;
        g_clTX.m_stSbusCmd.ushCH4 = g_clTX.m_stSbusRcv.ushCH4;
        g_clTX.m_stSbusCmd.ushCH5 = g_clTX.m_stSbusRcv.ushCH5;
        g_clTX.m_stSbusCmd.ushCH6 = g_clTX.m_stSbusRcv.ushCH6;
        g_clTX.m_stSbusCmd.ushCH7 = g_clTX.m_stSbusRcv.ushCH7;
        g_clTX.m_stSbusCmd.ushCH8 = g_clTX.m_stSbusRcv.ushCH8;
        g_clTX.m_stSbusCmd.ushCH9 = g_clTX.m_stSbusRcv.ushCH9;
        g_clTX.m_stSbusCmd.ushCH10 = g_clTX.m_stSbusRcv.ushCH10;
        g_clTX.m_stSbusCmd.ushCH11 = g_clTX.m_stSbusRcv.ushCH11;
        g_clTX.m_stSbusCmd.ushCH12 = g_clTX.m_stSbusRcv.ushCH12;
        g_clTX.m_stSbusCmd.ushCH13 = g_clTX.m_stSbusRcv.ushCH13;
        g_clTX.m_stSbusCmd.ushCH14 = g_clTX.m_stSbusRcv.ushCH14;
        g_clTX.m_stSbusCmd.ushCH15 = g_clTX.m_stSbusRcv.ushCH15;
        g_clTX.m_stSbusCmd.ushCH16 = g_clTX.m_stSbusRcv.ushCH16;

    } else {
        // Auto

        //-----------------------------------------------
        // write here for your autopilot control
        //-----------------------------------------------

        //-----------------------------------------------

        g_clTX.m_stSbusCmd.ushCH1 = 1023;   // THR
        g_clTX.m_stSbusCmd.ushCH2 = 1023;   // AIL_R
        g_clTX.m_stSbusCmd.ushCH3 = 1023;   // ELV
        g_clTX.m_stSbusCmd.ushCH4 = 1023;   // RUD
        g_clTX.m_stSbusCmd.ushCH5 = 1023;   // AIL_L
        g_clTX.m_stSbusCmd.ushCH6 = 1023;
        g_clTX.m_stSbusCmd.ushCH7 = 1023;
        g_clTX.m_stSbusCmd.ushCH8 = 1023;
        g_clTX.m_stSbusCmd.ushCH9 = 1023;
        g_clTX.m_stSbusCmd.ushCH10 = 1023;
        g_clTX.m_stSbusCmd.ushCH11 = 1023;
        g_clTX.m_stSbusCmd.ushCH12 = 1023;
        g_clTX.m_stSbusCmd.ushCH13 = 1023;
        g_clTX.m_stSbusCmd.ushCH14 = 1023;
        g_clTX.m_stSbusCmd.ushCH15 = 1023;
        g_clTX.m_stSbusCmd.ushCH16 = 1023;

    }
}

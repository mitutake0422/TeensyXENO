#ifndef XENOTYPES_H
#define XENOTYPES_H

#include <Arduino.h>

#define TEENSY4_0   40
#define TEENSY4_1   41

#define NUM_WP		256
#define FIFOBUF		100

#define CRTD		57.2957732	          // rad -> deg
#define CDTR		0.01745329	          // deg -> rad
#define GRAV		9.80665		            // 重力加速度
#define EARTH_R		6378178				// 地球半径[m] WGS-84
#define EARTH_E		0.08181919			// 地球離心率[-] WGS-84

// Teensy4.0
#define DIP1_40   9
#define DIP2_40   10
#define DIP3_40   11
#define DIP4_40   12

#define LED1_40   17
#define LED2_40   16
#define LED3_40   15
#define LED4_40   14
#define LED5_40   13

#define PWM1_40   0
#define PWM2_40   1
#define PWM3_40   2
#define PWM4_40   3
#define PWM5_40   4
#define PWM6_40   5
#define PWM7_40   6
#define PWM8_40   7

// Teensy4.1
#define DIP1_41   27
#define DIP2_41   29
#define DIP3_41   30
#define DIP4_41   31

#define LED1_41   40
#define LED2_41   39
#define LED3_41   38
#define LED4_41   37
#define LED5_41   36
#define LED6_41   35
#define LED7_41   34
#define LED8_41   33

#define PWM1_41   0
#define PWM2_41   1
#define PWM3_41   2
#define PWM4_41   3
#define PWM5_41   4
#define PWM6_41   5
#define PWM7_41   6
#define PWM8_41   7
#define PWM9_41   8
#define PWM10_41  9
#define PWM11_41  10
#define PWM12_41  11
#define PWM13_41  12
#define PWM14_41  24
#define PWM15_41  25
#define PWM16_41  28

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
    unsigned char uchID;
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
#endif
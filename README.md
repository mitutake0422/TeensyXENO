\# TeensyXENO



TeensyXENO用ライブラリ（固定翼機・HILS）



\## Overview

This is a Teensy-based XENO controller library for HILS and fixed-wing applications.



\---



\## 概要

このライブラリは、Teensy上で動作するXENOコントローラ用ライブラリです。



\## 機能

SBUS入出力

PWM出力

XenoController通信

HILS対応



\## 動作環境

Teensy 4.0/4.1\*\*

Arduino IDE

Teensyduino



\*\* 公開時、TeensyXENO基板はTeensy4.0にのみ対応



\## 使用方法

TeensyXENO基板にTeensy4.0をのせ、PCとはTeensyとはUSBケーブルで接続してください。



\## サンプルプログラム

examples/TeenyXENO\_HILS を参照してください。

userfunc() 内に誘導制御ロジックを記述します。





\### TeensyXENOクラスのオブジェクト

TeensyXENO g\_clTX(TEENSY4\_0);



g\_clTXを使って関数呼び出し、メンバ変数へアクセスします。





\### メンバ変数

\_stSensor\& m\_stSensor;		// センサー値

\_stNavi\& m\_stNavi;		// 航法計算

\_stSBUS\& m\_stSbusRcv;		// SBUS入力

\_stSBUS\& m\_stSbusCmd;		// SBUS出力

\_stDIP\& m\_stDIP;		// DIP SW

\_stFltPlan\& m\_stFltPlan;	// 飛行計画（XenoControllerから送信）

\_stParam\& m\_stParam;		// 誘導制御パラメータ（XenoControllerから送信）

int\& m\_nCtrl;			// 自動操縦ON/OFF



\_stCommand\& m\_stCommand;	// 指令値（出力用）

\_stControl\& m\_stControl;	// 制御計算（出力用）





\### 定数・構造体定義

\#define MANUAL    0

\#define AUTO      1



struct \_stSensor

{

&#x20;   double dAccX;

&#x20;   double dAccY;

&#x20;   double dAccZ;

&#x20;   double dRotX;

&#x20;   double dRotY;

&#x20;   double dRotZ;

&#x20;   double dPhi;

&#x20;   double dTht;

&#x20;   double dPsi;

&#x20;   double dLat;

&#x20;   double dLon;

&#x20;   double dAlt;

&#x20;   double dGS;

&#x20;   double dCOG;

&#x20;   unsigned char uchHour;

&#x20;   unsigned char uchMin;

&#x20;   unsigned char uchSec;

&#x20;   unsigned char uchNSV;

&#x20;   unsigned char uchQuality;

&#x20;   double dPs;

&#x20;   double dPd;

&#x20;   double dVS;

};



struct \_stNavi

{

&#x20;   double dAltP;

&#x20;   double dIAS;



&#x20;   double dXTE;

&#x20;   double dClosestLat;

&#x20;   double dClosestLon;

&#x20;   double dT;

&#x20;   double dDistPerLat;

&#x20;   double dDistPerLon;



&#x20;   unsigned short ushTcAltP;

&#x20;   unsigned short ushAltG;

&#x20;   unsigned short ushPressG;

&#x20;   unsigned short ushTemprG;



&#x20;   double dTcAltP;

&#x20;   double dAltG;

&#x20;   double dPressG;

&#x20;   double dtemprG;

};



struct \_stSBUS

{

&#x20;   unsigned short ushCH1;

&#x20;   unsigned short ushCH2;

&#x20;   unsigned short ushCH3;

&#x20;   unsigned short ushCH4;

&#x20;   unsigned short ushCH5;

&#x20;   unsigned short ushCH6;

&#x20;   unsigned short ushCH7;

&#x20;   unsigned short ushCH8;

&#x20;   unsigned short ushCH9;

&#x20;   unsigned short ushCH10;

&#x20;   unsigned short ushCH11;

&#x20;   unsigned short ushCH12;

&#x20;   unsigned short ushCH13;

&#x20;   unsigned short ushCH14;

&#x20;   unsigned short ushCH15;

&#x20;   unsigned short ushCH16;

&#x20;   unsigned char uchD1;

&#x20;   unsigned char uchD2;

&#x20;   unsigned char uchFrameLost;

&#x20;   unsigned char uchFailSafe;



&#x20;   unsigned char uchSBUS23;

};



struct \_stCommand

{

&#x20;   double dPhiCmd;

&#x20;   double dThtCmd;

&#x20;   double dPsiCmd;

&#x20;   double dVelCmd;

&#x20;   double dAltCmd;

&#x20;   double dVsCmd;

};



struct \_stWP

{

&#x20;   double dWpLat;

&#x20;   double dWpLon;

&#x20;   double dWpAlt;

&#x20;   double dWpPass;

&#x20;   unsigned char uchFlg;

};



struct \_stFltPlan

{

&#x20;   \_stWP stWP\[NUM\_WP];

&#x20;   int nWpTo;

&#x20;   int nWpNum;

};



struct \_stParam

{

&#x20;   unsigned short ushElvKp;

&#x20;   unsigned short ushElvKi;

&#x20;   unsigned short ushElvKd;

&#x20;   unsigned short ushElvLimit;

&#x20;   unsigned short ushThtCmd;



&#x20;   unsigned short ushAilKp;

&#x20;   unsigned short ushAilKi;

&#x20;   unsigned short ushAilKd;

&#x20;   unsigned short ushAilLimit;

&#x20;   unsigned short ushPhiCmd;



&#x20;   unsigned short ushAltKp;  // ALT err →VS cmd



&#x20;   unsigned short ushThtKp;  // VScmd → THT cmd

&#x20;   unsigned short ushThtKi;

&#x20;   unsigned short ushThtULimit;

&#x20;   unsigned short ushThtLLimit;

&#x20;   unsigned short ushAltCmd;



&#x20;   unsigned short ushPhiKp;

&#x20;   unsigned short ushPhiKi;

&#x20;   unsigned short ushPhiLimit;

&#x20;   unsigned short ushLookAheadDist;

&#x20;   unsigned short ushPsiCmd;



&#x20;   double dElvKp;

&#x20;   double dElvKi;

&#x20;   double dElvKd;

&#x20;   double dElvLimit;

&#x20;   double dThtCmd;



&#x20;   double dAilKp;

&#x20;   double dAilKi;

&#x20;   double dAilKd;

&#x20;   double dAilLimit;

&#x20;   double dPhiCmd;



&#x20;   double dAltKp;

&#x20;   double dThtKp;

&#x20;   double dThtKi;

&#x20;   double dThtULimit;

&#x20;   double dThtLLimit;

&#x20;   double dAltCmd;



&#x20;   double dPhiKp;

&#x20;   double dPhiKi;

&#x20;   double dPhiLimit;

&#x20;   double dLookAheadDist;

&#x20;   double dPsiCmd;

};



struct \_stDIP

{

&#x20;   unsigned char uchDIP1;

&#x20;   unsigned char uchDIP2;

&#x20;   unsigned char uchDIP3;

&#x20;   unsigned char uchDIP4;

};



struct \_stControl

{

&#x20;   double dThtIntg;

&#x20;   double dVsIntg;

&#x20;   double dElvTrim;

&#x20;   double dPhiIntg;

&#x20;   double dAilTrim;

&#x20;   double dThtTrim;

};



\### 記述する場所

user.ino の 関数userfunc



\### 変数の使い方

例1：加速度X

g\_clTX.m\_stSensor.dAccX



例2：飛行計画の目標WP

g\_clTX.m\_stFltPlan.nWpTo



例3：飛行計画のindex = 2のWP緯度

g\_clTX.m\_stFltPlan.stWP\[2].dWpLat



例4：高度指令（ここに代入するとXenoControllerに表示されます）

g\_clTX.m\_stCommand.dAltCmd







\---


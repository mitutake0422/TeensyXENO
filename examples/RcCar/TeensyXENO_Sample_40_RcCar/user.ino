#include <math.h>

#define DTIME		0.02	// [sec]	計算時間間隔

void userfunc()
{

	//XenoVehicleRcCar* rc = g_clTX.GetRcCar();
	
	if (g_clTX.m_core.m_nCtrl == MANUAL) {
		// マニュアル操縦
									
		// 出力
		g_clTX.m_core.m_stSbusCmd.ushCH1 = g_clTX.m_core.m_stSbusRcv.ushCH1;
		g_clTX.m_core.m_stSbusCmd.ushCH2 = g_clTX.m_core.m_stSbusRcv.ushCH2;
		g_clTX.m_core.m_stSbusCmd.ushCH3 = g_clTX.m_core.m_stSbusRcv.ushCH3;
		g_clTX.m_core.m_stSbusCmd.ushCH4 = g_clTX.m_core.m_stSbusRcv.ushCH4;
		g_clTX.m_core.m_stSbusCmd.ushCH5 = g_clTX.m_core.m_stSbusRcv.ushCH5;
		g_clTX.m_core.m_stSbusCmd.ushCH6 = g_clTX.m_core.m_stSbusRcv.ushCH6;
		g_clTX.m_core.m_stSbusCmd.ushCH7 = g_clTX.m_core.m_stSbusRcv.ushCH7;
		g_clTX.m_core.m_stSbusCmd.ushCH8 = g_clTX.m_core.m_stSbusRcv.ushCH8;
		g_clTX.m_core.m_stSbusCmd.ushCH9 = g_clTX.m_core.m_stSbusRcv.ushCH9;
		g_clTX.m_core.m_stSbusCmd.ushCH10 = g_clTX.m_core.m_stSbusRcv.ushCH10;
		g_clTX.m_core.m_stSbusCmd.ushCH11 = g_clTX.m_core.m_stSbusRcv.ushCH11;
		g_clTX.m_core.m_stSbusCmd.ushCH12 = g_clTX.m_core.m_stSbusRcv.ushCH12;
		g_clTX.m_core.m_stSbusCmd.ushCH13 = g_clTX.m_core.m_stSbusRcv.ushCH13;
		g_clTX.m_core.m_stSbusCmd.ushCH14 = g_clTX.m_core.m_stSbusRcv.ushCH14;
		g_clTX.m_core.m_stSbusCmd.ushCH15 = g_clTX.m_core.m_stSbusRcv.ushCH15;
		g_clTX.m_core.m_stSbusCmd.ushCH16 = g_clTX.m_core.m_stSbusRcv.ushCH16;

	} else {
		// 自動操縦
		g_clTX.m_core.m_stSbusCmd.ushCH1 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH2 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH3 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH4 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH5 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH6 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH7 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH8 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH9 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH10 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH11 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH12 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH13 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH14 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH15 = 1023;
		g_clTX.m_core.m_stSbusCmd.ushCH16 = 1023;
  }
}
#include <TeensyXENO.h>

TeensyXENO g_clTX(TEENSY4_0, XENO_VEHICLE_RCCAR);

IntervalTimer g_Timer50hz;

void setup() {
  // put your setup code here, to run once:
	g_Timer50hz.begin(freq50hz, 20000);  // 20msec 50Hz

}

void loop() {
  // put your main code here, to run repeatedly:

	// SBUS入力
	g_clTX.GetSBUS();
	
	// 地上局入力
	g_clTX.GetXenoController();
}

void freq50hz()
{
	static int s_nCnt = 0;

	// 制御計算
	userfunc();

	// PWM出力
	g_clTX.SendPWM();

	//----------------------------------------------
	// HILS
	//----------------------------------------------
	// 制御出力
  if (s_nCnt % 5 == 0) {
	  g_clTX.HILS_SendCtrl();
	}

	// XenoController
	if (s_nCnt % 5 == 2) {
		// 10Hz
		g_clTX.SendXenoController();
	}

	if (++s_nCnt == 50) {
		s_nCnt = 0;
	}
}
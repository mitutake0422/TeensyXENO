#include <TeensyXENO.h>

TeensyXENO g_clTX(TEENSY4_0, XENO_VEHICLE_FIXEDWING);

IntervalTimer g_Timer50hz;
IntervalTimer g_Timer14ms;

void setup() {
  // put your setup code here, to run once:

	// Timer
	g_Timer50hz.begin(freq50hz, 20000);  // 20msec 50Hz
	g_Timer14ms.begin(freq14ms, 14000);  // 14msec SBUS
}

void loop() {
  // put your main code here, to run repeatedly:
	
	// SBUS入力
	g_clTX.GetSBUS();
	
	// 地上局入力
	g_clTX.GetXenoController();
}

// SBUS
void freq14ms()
{
	// SBUS出力
	g_clTX.SendSBUS();
}

// 50Hz
void freq50hz()
{
	static int s_nCnt = 0;

	// 制御計算
	userfunc();

	// PWM出力
	g_clTX.SendPWM();

	// XenoController出力
	if (s_nCnt % 5 == 2) {
		g_clTX.SendXenoController();
	}

	//----------------------------------------------
	// HILS
	//----------------------------------------------
	// 制御出力
  if (s_nCnt % 5 == 0) {
    g_clTX.HILS_SendCtrl();
  }

	if (++s_nCnt == 50) {
		s_nCnt = 0;
	}
}
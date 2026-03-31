#include "XenoVehicleFixedWing.h"
#include "XenoCore.h"

XenoVehicleFixedWing::XenoVehicleFixedWing(XenoCore& core)
    : XenoVehicleBase(core)
{
}

void XenoVehicleFixedWing::InitVehicleValue()
{
    int i;

    m_stCommand.dPhiCmd = 0;
    m_stCommand.dThtCmd = 0;
    m_stCommand.dPsiCmd = 0;
    m_stCommand.dVelCmd = 0;
    m_stCommand.dAltCmd = 0;
    m_stCommand.dVsCmd = 0;

    for (i = 0; i < NUM_WP; i++) {
		m_stFltPlan.stWP[i].uchID = 0;
        m_stFltPlan.stWP[i].dWpLat = 0;
        m_stFltPlan.stWP[i].dWpLon = 0;
        m_stFltPlan.stWP[i].dWpAlt = 0;
        m_stFltPlan.stWP[i].dWpPass = 0;
        m_stFltPlan.stWP[i].uchFlg = 0;
    }
    m_stFltPlan.nWpTo = 255;
    m_stFltPlan.nWpNum = 0;

    m_stParam.ushElvKp = 800;
    m_stParam.ushElvKi = 990;
    m_stParam.ushElvKd = 1000;
    m_stParam.ushElvLimit = 12000;
    m_stParam.ushThtCmd = 0;

    m_stParam.ushAilKp = 1200;
    m_stParam.ushAilKi = 1000;
    m_stParam.ushAilKd = 1000;
    m_stParam.ushAilLimit = 12000;
    m_stParam.ushPhiCmd = 9000;

    m_stParam.ushAltKp = 1000;

    m_stParam.ushThtKp = 1200;
    m_stParam.ushThtKi = 1000;
    m_stParam.ushThtULimit = 19000;
    m_stParam.ushThtLLimit = 11000;
    m_stParam.ushAltCmd = 1000;

    m_stParam.ushPhiKp = 1200;
    m_stParam.ushPhiKi = 1000;
    m_stParam.ushPhiLimit = 12000;
    m_stParam.ushLookAheadDist = 11000;
    m_stParam.ushPsiCmd = 9000;

    m_stParam.dElvKp = (double)m_stParam.ushElvKp / 100.0 - 10.0;
    m_stParam.dElvKi = (double)m_stParam.ushElvKi / 100.0 - 10.0;
    m_stParam.dElvKd = (double)m_stParam.ushElvKd / 100.0 - 10.0;
    m_stParam.dElvLimit = (double)m_stParam.ushElvLimit / 100.0 - 90.0;
    m_stParam.dThtCmd = (double)m_stParam.ushThtCmd / 100.0 - 10.0;

    m_stParam.dAilKp = (double)m_stParam.ushAilKp / 100.0 - 10.0;
    m_stParam.dAilKi = (double)m_stParam.ushAilKi / 100.0 - 10.0;
    m_stParam.dAilKd = (double)m_stParam.ushAilKd / 100.0 - 10.0;
    m_stParam.dAilLimit = (double)m_stParam.ushAilLimit / 100.0 - 90.0;
    m_stParam.dPhiCmd = (double)m_stParam.ushPhiCmd / 100.0 - 90.0;

    m_stParam.dAltKp = (double)m_stParam.ushAltKp / 100.0 - 10.0;
    m_stParam.dThtKp = (double)m_stParam.ushThtKp / 100.0 - 10.0;
    m_stParam.dThtKi = (double)m_stParam.ushThtKi / 100.0 - 10.0;
    m_stParam.dThtULimit = (double)m_stParam.ushThtULimit / 100.0 - 90.0;
    m_stParam.dThtLLimit = (double)m_stParam.ushThtLLimit / 100.0 - 90.0;
    m_stParam.dAltCmd = (double)m_stParam.ushAltCmd / 10.0 - 100.0;

    m_stParam.dPhiKp = (double)m_stParam.ushPhiKp / 100.0 - 10.0;
    m_stParam.dPhiKi = (double)m_stParam.ushPhiKi / 100.0 - 10.0;
    m_stParam.dPhiLimit = (double)m_stParam.ushPhiLimit / 100.0 - 90.0;
    m_stParam.dLookAheadDist = (double)m_stParam.ushLookAheadDist / 10.0 - 1000.0;
    m_stParam.dPsiCmd = (double)m_stParam.ushPsiCmd / 100.0 - 90.0;

    m_stControl.dThtIntg = 0;
    m_stControl.dVsIntg = 0;
    m_stControl.dElvTrim = 0;
    m_stControl.dPhiIntg = 0;
    m_stControl.dAilTrim = 0;
    m_stControl.dThtTrim = 0;
}

void XenoVehicleFixedWing::GetXenoController()
{
	int nRcv;
	int nTmp;
	unsigned char uchIdx = 0;
	unsigned short ushCRC;
	unsigned char uchMID;
	unsigned short ushTmp;
	unsigned long ulTmp;

	static unsigned char s_uchTmp = 0x00;
	static unsigned char s_uchMsg[300];
	static int s_nIdx = -1;
	static int s_nSize = 0;

	for (;;) {

		nRcv = Serial.read();

		if (nRcv < 0) {
			break;
		}

		// ﾒｯｾｰｼﾞ先頭ﾁｪｯｸ
		if (s_nIdx < 0 && s_uchTmp == 0xff && nRcv == 0xff) {
			s_uchMsg[0] = 0xff;
			s_nIdx = 1;
		}

		// ﾒｯｾｰｼﾞ保存
		if (s_nIdx > 0) {

			s_uchMsg[s_nIdx] = (unsigned char)nRcv;

			// ﾒｯｾｰｼﾞｻｲｽﾞ
			if (s_nIdx == 3) {
				s_nSize = (int)s_uchMsg[3];
			}

			if (++s_nIdx == 300) {
				s_nIdx = -1;
			}

			if (s_nIdx == s_nSize + 8) {

				ushCRC = m_core.crc16(0, s_uchMsg, s_nSize + 4);
				ushTmp = ((unsigned short)s_uchMsg[s_nSize + 4] << 8 & 0xff00) | ((unsigned short)s_uchMsg[s_nSize + 5] & 0x00ff);

				if (ushCRC == ushTmp) {
					// ﾒｯｾｰｼﾞ解析

					uchMID = s_uchMsg[2];

					if (uchMID == 0) {
						// MID:0  	目標WP
						nTmp = (int)s_uchMsg[4];

						if (nTmp >= 0 && nTmp < NUM_WP) {
							m_stFltPlan.nWpTo = nTmp;
						}
					}
					else if (uchMID == 1) {
						// MID:1	ﾘｾｯﾄ処理
						m_stFltPlan.nWpNum = 0;
						m_stFltPlan.nWpTo = 255;

						m_core.m_stNavi.dXTE = 0;
						m_core.m_stNavi.dClosestLat = 0;
						m_core.m_stNavi.dClosestLon = 0;
						m_core.m_stNavi.dT = 0;

					}
					else if (uchMID == 30) {
						// MID:30  	気圧高度 時定数
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_core.m_stNavi.ushTcAltP = ushTmp;

						//write_EEPROM(&s_uchMsg[4], EEPROM_TCALTP, 2);

					}
					else if (uchMID == 31) {
						// MID:31  	地上高度
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_core.m_stNavi.ushAltG = ushTmp;

						//write_EEPROM(&s_uchMsg[4], EEPROM_ALTG, 2);

					}
					else if (uchMID == 32) {
						// MID:32  	地上気圧
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_core.m_stNavi.ushPressG = ushTmp;

						//write_EEPROM(&s_uchMsg[4], EEPROM_PRSG, 2);

					}
					else if (uchMID == 33) {
						// MID:33  	地上気温
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_core.m_stNavi.ushTemprG = ushTmp;

						//write_EEPROM(&s_uchMsg[4], EEPROM_TMPG, 2);

					}
					else if (uchMID == 33) {
						// MID:77  	GoHome
						;

					}
					else if (uchMID == 100) {
						// MID:100	ﾋﾟｯﾁ角指令
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushThtCmd = ushTmp;
						if (ushTmp != 0) {
							m_stParam.dThtCmd = (double)ushTmp * 0.01 - 90.0;
						}

					}
					else if (uchMID == 101) {
						// MID:101	ﾋﾟｯﾁ角制御 比例ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushElvKp = ushTmp;
						m_stParam.dElvKp = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 102) {
						// MID:102	ﾋﾟｯﾁ角制御 微分ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushElvKd = ushTmp;
						m_stParam.dElvKd = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 103) {
						// MID:103	ﾋﾟｯﾁ角制御 積分ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushElvKi = ushTmp;
						m_stParam.dElvKi = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 104) {
						// MID:104	ﾋﾟｯﾁ角制御 ｴﾚﾍﾞｰﾀﾘﾐｯﾀ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushElvLimit = ushTmp;
						m_stParam.dElvLimit = (double)ushTmp * 0.01 - 90.0;

					}
					else if (uchMID == 110) {
						// MID:110	ﾊﾞﾝｸ角指令
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushPhiCmd = ushTmp;
						if (ushTmp != 0) {
							m_stParam.dPhiCmd = -90.0 + (double)ushTmp * 0.01;
						}

					}
					else if (uchMID == 111) {
						// MID:111	ﾊﾞﾝｸ角指令 比例ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushAilKp = ushTmp;
						m_stParam.dAilKp = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 112) {
						// MID:112	ﾊﾞﾝｸ角指令 微分ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushAilKd = ushTmp;
						m_stParam.dAilKd = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 113) {
						// MID:113	ﾊﾞﾝｸ角指令 積分ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushAilKi = ushTmp;
						m_stParam.dAilKi = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 114) {
						// MID:113	ﾊﾞﾝｸ角指令 ｴﾙﾛﾝﾘﾐｯﾀ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushAilLimit = ushTmp;
						m_stParam.dAilLimit = (double)ushTmp * 0.01 - 90.0;

					}
					else if (uchMID == 120) {
						// MID:120 高度制御 比例ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushAltKp = ushTmp;
						m_stParam.dAltKp = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 121) {
						// MID:121 高度制御 比例ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushThtKp = ushTmp;
						m_stParam.dThtKp = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 122) {
						// MID:122 高度制御 積分ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushThtKi = ushTmp;
						m_stParam.dThtKi = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 123) {
						// MID:123 高度制御 上限ﾘﾐｯﾄ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushThtULimit = ushTmp;
						m_stParam.dThtULimit = (double)ushTmp * 0.01 - 90.0;

					}
					else if (uchMID == 124) {
						// MID:124 高度制御 下限ﾘﾐｯﾄ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushThtLLimit = ushTmp;
						m_stParam.dThtLLimit = (double)ushTmp * 0.01 - 90.0;

					}
					else if (uchMID == 125) {
						// MID:125 高度制御 高度指令
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushAltCmd = ushTmp;
						if (ushTmp != 0) {
							m_stParam.dAltCmd = (double)(ushTmp - 1) * 100 + 100;
						}

					}
					else if (uchMID == 130) {
						// MID:130 方位制御 比例ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushPhiKp = ushTmp;
						m_stParam.dPhiKp = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 131) {
						// MID:131 方位制御 微分ｹﾞｲﾝ

					}
					else if (uchMID == 132) {
						// MID:132 方位制御 積分ｹﾞｲﾝ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushPhiKi = ushTmp;
						m_stParam.dPhiKi = (double)ushTmp * 0.01 - 10.0;

					}
					else if (uchMID == 133) {
						// MID:133 方位制御 ﾘﾐｯﾄ
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushPhiLimit = ushTmp;
						m_stParam.dPhiLimit = (double)ushTmp * 0.01 - 90.0;

					}
					else if (uchMID == 134) {
						// MID:134 方位制御 前方距離
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushLookAheadDist = ushTmp;
						m_stParam.dLookAheadDist = (double)ushTmp * 500 + 1000;

					}
					else if (uchMID == 135) {
						// MID:135 方位制御 方位指令
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushPsiCmd = ushTmp;
						if (ushTmp != 0) {
							m_stParam.dPsiCmd = (double)ushTmp * 0.01 - 90.0;
						}
					}
					else if (uchMID == 200) {
						// MID:200	ｳｪｲﾎﾟｲﾝﾄ情報
						uchIdx = s_uchMsg[4];

						m_stFltPlan.stWP[uchIdx].uchID = s_uchMsg[5];
						
						ulTmp = (((unsigned long)s_uchMsg[6] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[7] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[8] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[9] << 0) & 0x000000ff);
						m_stFltPlan.stWP[uchIdx].dWpLat = (double)ulTmp / 10000000.0 - 180.0;

						ulTmp = (((unsigned long)s_uchMsg[10] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[11] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[12] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[13] << 0) & 0x000000ff);
						m_stFltPlan.stWP[uchIdx].dWpLon = (double)ulTmp / 10000000.0 - 180.0;

						ulTmp = (((unsigned long)s_uchMsg[14] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[15] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[16] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[17] << 0) & 0x000000ff);
						m_stFltPlan.stWP[uchIdx].dWpAlt = (double)ulTmp / 1000.0 - 1000.0;

						ushTmp = (((unsigned short)s_uchMsg[18] << 8) & 0xff00) | ((unsigned short)s_uchMsg[19] & 0x00ff);
						m_stFltPlan.stWP[uchIdx].dWpPass = (double)ushTmp * 0.1;

						m_stFltPlan.stWP[uchIdx].uchFlg = s_uchMsg[20];

						m_stFltPlan.nWpNum = (int)s_uchMsg[21];
					}
					else if (uchMID == 210) {

						// HILS
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_core.m_stSensor.dAccX = (double)ushTmp * 0.01 - 160.0;

						ushTmp = (((unsigned short)s_uchMsg[6] << 8) & 0xff00) | ((unsigned short)s_uchMsg[7] & 0x00ff);
						m_core.m_stSensor.dAccY = (double)ushTmp * 0.01 - 160.0;

						ushTmp = (((unsigned short)s_uchMsg[8] << 8) & 0xff00) | ((unsigned short)s_uchMsg[9] & 0x00ff);
						m_core.m_stSensor.dAccZ = (double)ushTmp * 0.01 - 160.0;

						ushTmp = (((unsigned short)s_uchMsg[10] << 8) & 0xff00) | ((unsigned short)s_uchMsg[11] & 0x00ff);
						m_core.m_stSensor.dRotX = (double)ushTmp * 0.1 - 2000.0;

						ushTmp = (((unsigned short)s_uchMsg[12] << 8) & 0xff00) | ((unsigned short)s_uchMsg[13] & 0x00ff);
						m_core.m_stSensor.dRotY = (double)ushTmp * 0.1 - 2000.0;

						ushTmp = (((unsigned short)s_uchMsg[14] << 8) & 0xff00) | ((unsigned short)s_uchMsg[15] & 0x00ff);
						m_core.m_stSensor.dRotZ = (double)ushTmp * 0.1 - 2000.0;

						ushTmp = (((unsigned short)s_uchMsg[16] << 8) & 0xff00) | ((unsigned short)s_uchMsg[17] & 0x00ff);
						m_core.m_stSensor.dPhi = (double)ushTmp * 0.01 - 180.0;

						ushTmp = (((unsigned short)s_uchMsg[18] << 8) & 0xff00) | ((unsigned short)s_uchMsg[19] & 0x00ff);
						m_core.m_stSensor.dTht = (double)ushTmp * 0.01 - 180.0;

						ushTmp = (((unsigned short)s_uchMsg[20] << 8) & 0xff00) | ((unsigned short)s_uchMsg[21] & 0x00ff);
						m_core.m_stSensor.dPsi = (double)ushTmp * 0.01 - 180.0;

						ulTmp = (((unsigned long)s_uchMsg[22] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[23] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[24] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[25] << 0) & 0x000000ff);
						m_core.m_stSensor.dLat = (double)ulTmp / 10000000.0 - 180.0;

						ulTmp = (((unsigned long)s_uchMsg[26] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[27] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[28] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[29] << 0) & 0x000000ff);
						m_core.m_stSensor.dLon = (double)ulTmp / 10000000.0 - 180.0;

						ushTmp = (((unsigned short)s_uchMsg[30] << 8) & 0xff00) | ((unsigned short)s_uchMsg[31] & 0x00ff);
						m_core.m_stSensor.dAlt = (double)ushTmp / 10.0 - 100.0;

						ushTmp = (((unsigned short)s_uchMsg[32] << 8) & 0xff00) | ((unsigned short)s_uchMsg[33] & 0x00ff);
						m_core.m_stSensor.dGS = (double)ushTmp * 0.01;

						ushTmp = (((unsigned short)s_uchMsg[34] << 8) & 0xff00) | ((unsigned short)s_uchMsg[35] & 0x00ff);
						m_core.m_stSensor.dCOG = (double)ushTmp * 0.01;

						if (m_core.m_stSensor.dCOG > 180.0) {
							m_core.m_stSensor.dCOG -= 360.0;
						}

						m_core.m_stSensor.uchHour = s_uchMsg[36];
						m_core.m_stSensor.uchMin = s_uchMsg[37];
						m_core.m_stSensor.uchSec = s_uchMsg[38];

						m_core.m_stSensor.uchNSV = s_uchMsg[39] & 0x0f;
						m_core.m_stSensor.uchQuality = (s_uchMsg[39] >> 4) & 0x0f;

						ushTmp = (((unsigned short)s_uchMsg[40] << 8) & 0xff00) | ((unsigned short)s_uchMsg[41] & 0x00ff);
						m_core.m_stSensor.dPs = (double)ushTmp / 10.0;	// [hPa]

						ushTmp = (((unsigned short)s_uchMsg[42] << 8) & 0xff00) | ((unsigned short)s_uchMsg[43] & 0x00ff);
						m_core.m_stSensor.dPd = (double)ushTmp / 10.0;	// [Pa]

						ushTmp = (((unsigned short)s_uchMsg[44] << 8) & 0xff00) | ((unsigned short)s_uchMsg[45] & 0x00ff);
						m_core.m_stSensor.dVS = (double)ushTmp / 100.0 - 100.0;	// [m/s]
					}

					// Ack返送処理
					if (0 <= uchMID && uchMID <= 200) {

						if (uchMID == 200) {
							// WP
							m_core.SetFIFO(200, uchIdx);
						}
						else if (uchMID == 0) {
							// 目標WP
							m_core.SetFIFO(uchMID, (unsigned char)m_stFltPlan.nWpTo);
						}
						else {
							// その他
							m_core.SetFIFO(uchMID, (unsigned char)1);
						}
					}
				}

				// ｲﾝﾃﾞｯｸｽ初期化
				s_nIdx = -1;
			}
		}
		// 今回値保存
		s_uchTmp = (unsigned char)nRcv;
	}
}

void XenoVehicleFixedWing::SendXenoController()
{
	int i;
	unsigned char uchMsg[200];
	unsigned char uchID, uchRS;
	unsigned short ushTmp;
	short shTmp;
	unsigned long ulTmp;

	static unsigned short s_ushCnt = 0;

	if (1) {
		// 10Hz		for PFD

		uchMsg[0] = 0xff;
		uchMsg[1] = 0xff;
		uchMsg[2] = 0x01;

		ushTmp = s_ushCnt++;
		uchMsg[3] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[4] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned short)((m_core.m_stSensor.dPhi + 180.0) * 100.0);
		uchMsg[5] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[6] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned short)((m_core.m_stSensor.dTht + 180.0) * 100.0);
		uchMsg[7] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[8] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned short)(m_core.m_stNavi.dIAS * 100.0);
		uchMsg[9] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[10] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned short)((m_core.m_stNavi.dAltP + 100.0) * 10.0);
		uchMsg[11] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[12] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned short)((m_core.m_stSensor.dVS + 100.0) * 100.0);
		uchMsg[13] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[14] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned short)((m_stCommand.dPhiCmd + 180.0) * 100.0);
		uchMsg[15] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[16] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned short)((m_stCommand.dThtCmd + 180.0) * 100.0);
		uchMsg[17] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[18] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned short)(m_stCommand.dVelCmd * 100.0);
		uchMsg[19] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[20] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned long)((m_stCommand.dAltCmd + 100.0) * 10.0);
		uchMsg[21] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[22] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = (unsigned long)((m_stCommand.dVsCmd + 100.0) * 100.0);
		uchMsg[23] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[24] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stSbusCmd.ushCH8;
		uchMsg[25] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[26] = (unsigned char)(ushTmp & 0x00ff);

		// CRC
		ushTmp = m_core.crc16(0, uchMsg, 27);
		uchMsg[27] = (unsigned char)((ushTmp >> 8) & 0x00ff);
		uchMsg[28] = (unsigned char)(ushTmp & 0x00ff);

		// ﾌｯﾀｰ
		uchMsg[29] = 0x0d;
		uchMsg[30] = 0x0a;

		// 送信
		for (i = 0; i < 31; i++) {
			Serial.write(uchMsg[i]);
		}
	}

	if (s_ushCnt % 2 == 0) {
		// 5Hz	for map

		uchMsg[0] = 0xff;
		uchMsg[1] = 0xff;
		uchMsg[2] = 0x02;

		ulTmp = (unsigned long)((m_core.m_stSensor.dLat + 180.0) * 10000000.0);
		uchMsg[3] = (unsigned char)(ulTmp >> 24 & 0x000000ff);
		uchMsg[4] = (unsigned char)(ulTmp >> 16 & 0x000000ff);
		uchMsg[5] = (unsigned char)(ulTmp >> 8 & 0x000000ff);
		uchMsg[6] = (unsigned char)(ulTmp & 0x000000ff);

		ulTmp = (unsigned long)((m_core.m_stSensor.dLon + 180.0) * 10000000.0);
		uchMsg[7] = (unsigned char)(ulTmp >> 24 & 0x000000ff);
		uchMsg[8] = (unsigned char)(ulTmp >> 16 & 0x000000ff);
		uchMsg[9] = (unsigned char)(ulTmp >> 8 & 0x000000ff);
		uchMsg[10] = (unsigned char)(ulTmp & 0x000000ff);

		uchMsg[11] = ((m_core.m_stSensor.uchQuality << 4) & 0xf0) | (m_core.m_stSensor.uchNSV & 0x0f);

		ushTmp = (unsigned short)((m_core.m_stSensor.dPsi + 180.0) * 100.0);
		uchMsg[12] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[13] = (unsigned char)(ushTmp & 0x00ff);

		uchMsg[14] = (unsigned char)m_stFltPlan.nWpNum;
		uchMsg[15] = (unsigned char)m_stFltPlan.nWpTo;

		ushTmp = (unsigned short)((m_stCommand.dPsiCmd + 180.0) * 100.0);
		uchMsg[16] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[17] = (unsigned char)(ushTmp & 0x00ff);

		shTmp = (short)m_core.m_stNavi.dXTE;
		uchMsg[18] = (unsigned char)(shTmp >> 8 & 0x00ff);
		uchMsg[19] = (unsigned char)(shTmp & 0x00ff);

		ulTmp = (unsigned long)((m_core.m_stNavi.dClosestLat + 180.0) * 10000000.0);
		uchMsg[20] = (unsigned char)(ulTmp >> 24 & 0x000000ff);
		uchMsg[21] = (unsigned char)(ulTmp >> 16 & 0x000000ff);
		uchMsg[22] = (unsigned char)(ulTmp >> 8 & 0x000000ff);
		uchMsg[23] = (unsigned char)(ulTmp & 0x000000ff);

		ulTmp = (unsigned long)((m_core.m_stNavi.dClosestLon + 180.0) * 10000000.0);
		uchMsg[24] = (unsigned char)(ulTmp >> 24 & 0x000000ff);
		uchMsg[25] = (unsigned char)(ulTmp >> 16 & 0x000000ff);
		uchMsg[26] = (unsigned char)(ulTmp >> 8 & 0x000000ff);
		uchMsg[27] = (unsigned char)(ulTmp & 0x000000ff);

		ushTmp = (unsigned short)((m_core.m_stNavi.dT + 10.0) * 1000.0);
		uchMsg[28] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[29] = (unsigned char)(ushTmp & 0x00ff);

		// CRC
		ushTmp = m_core.crc16(0, uchMsg, 30);
		uchMsg[30] = (unsigned char)((ushTmp >> 8) & 0x00ff);
		uchMsg[31] = (unsigned char)(ushTmp & 0x00ff);

		// ﾌｯﾀｰ
		uchMsg[32] = 0x0d;
		uchMsg[33] = 0x0a;

		// 送信
		for (i = 0; i < 34; i++) {
			Serial.write(uchMsg[i]);
		}
	}

	if (s_ushCnt % 2 == 1) {
		// 5Hz	for Param

		uchMsg[0] = 0xff;
		uchMsg[1] = 0xff;
		uchMsg[2] = 0x03;

		ushTmp = m_core.m_stSbusCmd.ushCH1;
		uchMsg[3] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[4] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stSbusCmd.ushCH2;
		uchMsg[5] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[6] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stSbusCmd.ushCH3;
		uchMsg[7] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[8] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stSbusCmd.ushCH4;
		uchMsg[9] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[10] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stSbusCmd.ushCH5;
		uchMsg[11] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[12] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stSbusCmd.ushCH6;
		uchMsg[13] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[14] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stSbusCmd.ushCH7;
		uchMsg[15] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[16] = (unsigned char)(ushTmp & 0x00ff);

		uchMsg[17] = m_core.m_stSbusRcv.uchSBUS23;

		ushTmp = m_core.m_stNavi.ushTcAltP;
		uchMsg[18] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[19] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stNavi.ushAltG;
		uchMsg[20] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[21] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stNavi.ushPressG;
		uchMsg[22] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[23] = (unsigned char)(ushTmp & 0x00ff);

		ushTmp = m_core.m_stNavi.ushTemprG;
		uchMsg[24] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[25] = (unsigned char)(ushTmp & 0x00ff);

		// ﾋﾟｯﾁ角制御 指令値
		ushTmp = m_stParam.ushThtCmd;
		uchMsg[26] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[27] = (unsigned char)(ushTmp & 0x00ff);

		// ﾋﾟｯﾁ角制御 比例ｹﾞｲﾝ
		ushTmp = m_stParam.ushElvKp;
		uchMsg[28] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[29] = (unsigned char)(ushTmp & 0x00ff);

		// ﾋﾟｯﾁ角制御 微分ｹﾞｲﾝ
		ushTmp = m_stParam.ushElvKd;
		uchMsg[30] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[31] = (unsigned char)(ushTmp & 0x00ff);

		// ﾋﾟｯﾁ角制御 積分ｹﾞｲﾝ
		ushTmp = m_stParam.ushElvKi;
		uchMsg[32] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[33] = (unsigned char)(ushTmp & 0x00ff);

		// ﾋﾟｯﾁ角制御 ｴﾚﾍﾞｰﾀﾘﾐｯﾀ
		ushTmp = m_stParam.ushElvLimit;
		uchMsg[34] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[35] = (unsigned char)(ushTmp & 0x00ff);

		// ﾋﾟｯﾁ角制御 積分値
		ushTmp = (unsigned short)((m_stControl.dThtIntg + 100.0) * 100.0);
		uchMsg[36] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[37] = (unsigned char)(ushTmp & 0x00ff);

		// ﾋﾟｯﾁ角制御 ｴﾚﾍﾞｰﾀﾄﾘﾑ
		ushTmp = (unsigned short)((m_stControl.dElvTrim + 100.0) * 100.0);
		uchMsg[38] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[39] = (unsigned char)(ushTmp & 0x00ff);

		// ﾊﾞﾝｸ角制御 指令値
		ushTmp = m_stParam.ushPhiCmd;
		uchMsg[40] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[41] = (unsigned char)(ushTmp & 0x00ff);

		// ﾊﾞﾝｸ角制御 比例ｹﾞｲﾝ
		ushTmp = m_stParam.ushAilKp;
		uchMsg[42] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[43] = (unsigned char)(ushTmp & 0x00ff);

		// ﾊﾞﾝｸ角制御 微分ｹﾞｲﾝ
		ushTmp = m_stParam.ushAilKd;
		uchMsg[44] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[45] = (unsigned char)(ushTmp & 0x00ff);

		// ﾊﾞﾝｸ角制御 積分ｹﾞｲﾝ
		ushTmp = m_stParam.ushAilKi;
		uchMsg[46] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[47] = (unsigned char)(ushTmp & 0x00ff);

		// ﾊﾞﾝｸ角制御 ｴﾙﾛﾝﾘﾐｯﾀ
		ushTmp = m_stParam.ushAilLimit;
		uchMsg[48] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[49] = (unsigned char)(ushTmp & 0x00ff);

		// ﾊﾞﾝｸ角制御 積分値
		ushTmp = (unsigned short)((m_stControl.dPhiIntg + 100.0) * 100.0);
		uchMsg[50] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[51] = (unsigned char)(ushTmp & 0x00ff);

		// ﾊﾞﾝｸ角制御 ｴﾙﾛﾝﾄﾘﾑ
		ushTmp = (unsigned short)((m_stControl.dAilTrim + 100.0) * 100.0);
		uchMsg[52] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[53] = (unsigned char)(ushTmp & 0x00ff);

		// 高度制御 指令値
		ushTmp = m_stParam.ushAltCmd;
		uchMsg[54] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[55] = (unsigned char)(ushTmp & 0x00ff);

		// 高度制御 比例ｹﾞｲﾝ
		ushTmp = m_stParam.ushAltKp;
		uchMsg[56] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[57] = (unsigned char)(ushTmp & 0x00ff);

		// 高度制御 比例ｹﾞｲﾝ
		ushTmp = m_stParam.ushThtKp;
		uchMsg[58] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[59] = (unsigned char)(ushTmp & 0x00ff);

		// 高度制御 積分ｹﾞｲﾝ
		ushTmp = m_stParam.ushThtKi;
		uchMsg[60] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[61] = (unsigned char)(ushTmp & 0x00ff);

		// 高度制御 上限ﾘﾐｯﾀ
		ushTmp = m_stParam.ushThtULimit;
		uchMsg[62] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[63] = (unsigned char)(ushTmp & 0x00ff);

		// 高度制御 下限ﾘﾐｯﾀ
		ushTmp = m_stParam.ushThtLLimit;
		uchMsg[64] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[65] = (unsigned char)(ushTmp & 0x00ff);

		// 高度制御 積分値
		ushTmp = (unsigned short)((m_stControl.dVsIntg + 100.0) * 100.0);
		uchMsg[66] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[67] = (unsigned char)(ushTmp & 0x00ff);

		// 高度制御 ピッチ角トリム
		ushTmp = (unsigned short)((m_stControl.dThtTrim + 180.0) * 100.0);
		uchMsg[68] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[69] = (unsigned char)(ushTmp & 0x00ff);

		// 方位制御 指令値
		ushTmp = m_stParam.ushPsiCmd;
		uchMsg[70] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[71] = (unsigned char)(ushTmp & 0x00ff);

		// 方位制御 比例ｹﾞｲﾝ
		ushTmp = m_stParam.ushPhiKp;
		uchMsg[72] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[73] = (unsigned char)(ushTmp & 0x00ff);

		// 方位制御 積分ｹﾞｲﾝ
		ushTmp = m_stParam.ushPhiKi;
		uchMsg[74] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[75] = (unsigned char)(ushTmp & 0x00ff);

		// 方位制御 ﾘﾐｯﾀ
		ushTmp = m_stParam.ushPhiLimit;
		uchMsg[76] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[77] = (unsigned char)(ushTmp & 0x00ff);

		// 方位制御 前方距離
		ushTmp = m_stParam.ushLookAheadDist;
		uchMsg[78] = (unsigned char)(ushTmp >> 8 & 0x00ff);
		uchMsg[79] = (unsigned char)(ushTmp & 0x00ff);

		// Ax
		ushTmp = (unsigned short)((m_core.m_stSensor.dAccX + 160.0) * 100.0);
		uchMsg[80] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[81] = (unsigned char)(ushTmp & 0x00ff);

		// Ay
		ushTmp = (unsigned short)((m_core.m_stSensor.dAccY + 160.0) * 100.0);
		uchMsg[82] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[83] = (unsigned char)(ushTmp & 0x00ff);

		// Az
		ushTmp = (unsigned short)((m_core.m_stSensor.dAccZ + 160.0) * 100.0);
		uchMsg[84] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[85] = (unsigned char)(ushTmp & 0x00ff);

		// P
		ushTmp = (unsigned short)((m_core.m_stSensor.dRotX + 2000.0) * 10.0);
		uchMsg[86] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[87] = (unsigned char)(ushTmp & 0x00ff);

		// Q
		ushTmp = (unsigned short)((m_core.m_stSensor.dRotY + 2000.0) * 10.0);
		uchMsg[88] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[89] = (unsigned char)(ushTmp & 0x00ff);

		// R
		ushTmp = (unsigned short)((m_core.m_stSensor.dRotZ + 2000.0) * 10.0);
		uchMsg[90] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
		uchMsg[91] = (unsigned char)(ushTmp & 0x00ff);

		// CRC
		ushTmp = m_core.crc16(0, uchMsg, 92);
		uchMsg[92] = (unsigned char)((ushTmp >> 8) & 0x00ff);
		uchMsg[93] = (unsigned char)(ushTmp & 0x00ff);

		// ﾌｯﾀｰ
		uchMsg[94] = 0x0d;
		uchMsg[95] = 0x0a;

		// 送信
		for (i = 0; i < 96; i++) {
			Serial.write(uchMsg[i]);
		}
	}

	if (s_ushCnt % 10 == 0) {
		// 1Hz	応答

		uchMsg[0] = 0xff;
		uchMsg[1] = 0xff;
		uchMsg[2] = 0x04;

		uchMsg[3] = m_core.m_stSensor.uchHour;
		uchMsg[4] = m_core.m_stSensor.uchMin;
		uchMsg[5] = m_core.m_stSensor.uchSec;

		// 応答1
		m_core.GetFIFO(&uchID, &uchRS);
		uchMsg[6] = uchID;
		uchMsg[7] = uchRS;

		// 応答2
		m_core.GetFIFO(&uchID, &uchRS);
		uchMsg[8] = uchID;
		uchMsg[9] = uchRS;

		// 応答3
		m_core.GetFIFO(&uchID, &uchRS);
		uchMsg[10] = uchID;
		uchMsg[11] = uchRS;

		// 応答4
		m_core.GetFIFO(&uchID, &uchRS);
		uchMsg[12] = uchID;
		uchMsg[13] = uchRS;

		// 応答5
		m_core.GetFIFO(&uchID, &uchRS);
		uchMsg[14] = uchID;
		uchMsg[15] = uchRS;
			
		// CRC
		ushTmp = m_core.crc16(0, uchMsg, 16);
		uchMsg[16] = (unsigned char)((ushTmp >> 8) & 0x00ff);
		uchMsg[17] = (unsigned char)(ushTmp & 0x00ff);

		// ﾌｯﾀｰ
		uchMsg[18] = 0x0d;
		uchMsg[19] = 0x0a;

		// 送信
		for (i = 0; i < 20; i++) {
			Serial.write(uchMsg[i]);
		}
	}
}
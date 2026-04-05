#include "XenoVehicleRcCar.h"
#include "../../core/XenoCore.h"

XenoVehicleRcCar::XenoVehicleRcCar(XenoCore& core)
    : XenoVehicleBase(core)
{
}

void XenoVehicleRcCar::InitVehicleValue()
{
	m_stSensor.dLat = 0;
	m_stSensor.dLon = 0;
	m_stSensor.dVel = 0;
	m_stSensor.uchQuality = 0;
	m_stSensor.uchHour = 0;
	m_stSensor.uchMin = 0;
	m_stSensor.uchSec = 0;
	m_stSensor.dPsi = 0;
	m_stSensor.dRotZ = 0;

	m_stNavi.dXTE = 0;
	m_stNavi.dClosestLat = 0;
	m_stNavi.dClosestLon = 0;
	m_stNavi.dT = 0;
	m_stNavi.dDistPerLat = 0;
	m_stNavi.dDistPerLon = 0;

    m_stCommand.dPsiCmd = 0;
    m_stCommand.dVelCmd = 0;

    m_stParam.ushStrKp = 1000;
	m_stParam.ushStrKi = 1000;
	m_stParam.ushAccKp = 1000;
	m_stParam.ushAccKi = 1000;

    m_stParam.dStrKp = (double)m_stParam.ushStrKp / 100.0 - 10.0;
    m_stParam.dStrKi = (double)m_stParam.ushStrKi / 100.0 - 10.0;
	m_stParam.dAccKp = (double)m_stParam.ushAccKp / 100.0 - 10.0;
	m_stParam.dAccKi = (double)m_stParam.ushAccKi / 100.0 - 10.0;

    m_stControl.dStrIntg = 0;
	m_stControl.dAccIntg = 0;
	m_stControl.dStrTrim = 0;
	m_stControl.dAccTrim = 0;
}

XenoVehicleType XenoVehicleRcCar::GetVehicleType() const
{
	return XENO_VEHICLE_RCCAR;
}

void XenoVehicleRcCar::GetXenoController()
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
							m_core.m_stPlan.nWpTo = nTmp;
						}
					}
					else if (uchMID == 1) {
						// MID:1	ﾘｾｯﾄ処理
						m_core.m_stPlan.nWpNum = 0;
						m_core.m_stPlan.nWpTo = 255;

						m_stNavi.dXTE = 0;
						m_stNavi.dClosestLat = 0;
						m_stNavi.dClosestLon = 0;
						m_stNavi.dT = 0;

					}
#if 0
					else if (uchMID == 30) {
						// MID:30  	気圧高度 時定数
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stNavi.ushTcAltP = ushTmp;

						//write_EEPROM(&s_uchMsg[4], EEPROM_TCALTP, 2);

					}
					else if (uchMID == 31) {
						// MID:31  	地上高度
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stNavi.ushAltG = ushTmp;

						//write_EEPROM(&s_uchMsg[4], EEPROM_ALTG, 2);

					}
					else if (uchMID == 32) {
						// MID:32  	地上気圧
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stNavi.ushPressG = ushTmp;

						//write_EEPROM(&s_uchMsg[4], EEPROM_PRSG, 2);

					}
					else if (uchMID == 33) {
						// MID:33  	地上気温
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stNavi.ushTemprG = ushTmp;

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
						m_stParam.dLookAheadDist = (double)ushTmp * 0.1 - 1000;

					}
					else if (uchMID == 135) {
						// MID:135 方位制御 方位指令
						ushTmp = (((unsigned short)s_uchMsg[4] << 8) & 0xff00) | ((unsigned short)s_uchMsg[5] & 0x00ff);
						m_stParam.ushPsiCmd = ushTmp;
						if (ushTmp != 0) {
							m_stParam.dPsiCmd = (double)ushTmp * 0.01 - 90.0;
						}
					}
#endif
					else if (uchMID == 200) {
						// MID:200	ｳｪｲﾎﾟｲﾝﾄ情報
						uchIdx = s_uchMsg[4];

						m_core.m_stPlan.stWP[uchIdx].uchID = s_uchMsg[5];
						
						ulTmp = (((unsigned long)s_uchMsg[6] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[7] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[8] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[9] << 0) & 0x000000ff);
						m_core.m_stPlan.stWP[uchIdx].dWpLat = (double)ulTmp / 10000000.0 - 180.0;

						ulTmp = (((unsigned long)s_uchMsg[10] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[11] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[12] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[13] << 0) & 0x000000ff);
						m_core.m_stPlan.stWP[uchIdx].dWpLon = (double)ulTmp / 10000000.0 - 180.0;

						ulTmp = (((unsigned long)s_uchMsg[14] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[15] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[16] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[17] << 0) & 0x000000ff);
						m_core.m_stPlan.stWP[uchIdx].dWpAlt = 0.0;// (double)ulTmp / 1000.0 - 1000.0;

						ushTmp = (((unsigned short)s_uchMsg[18] << 8) & 0xff00) | ((unsigned short)s_uchMsg[19] & 0x00ff);
						m_core.m_stPlan.stWP[uchIdx].dWpPass = (double)ushTmp * 0.1;

						m_core.m_stPlan.stWP[uchIdx].uchFlg = s_uchMsg[20];

						m_core.m_stPlan.stWP[uchIdx].uchBackIdx = s_uchMsg[21];
						m_core.m_stPlan.stWP[uchIdx].uchNextIdx = s_uchMsg[22];

						m_core.m_stPlan.nWpNum = (int)s_uchMsg[23];

					}
					else if (uchMID == 210) {

						// HILS
						ulTmp = (((unsigned long)s_uchMsg[4] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[5] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[6] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[7] << 0) & 0x000000ff);
						m_stSensor.dLat = (double)ulTmp / 10000000.0 - 180.0;

						ulTmp = (((unsigned long)s_uchMsg[8] << 24) & 0xff000000) |
							(((unsigned long)s_uchMsg[9] << 16) & 0x00ff0000) |
							(((unsigned long)s_uchMsg[10] << 8) & 0x0000ff00) |
							(((unsigned long)s_uchMsg[11] << 0) & 0x000000ff);
						m_stSensor.dLon = (double)ulTmp / 10000000.0 - 180.0;

						ushTmp = (((unsigned short)s_uchMsg[12] << 8) & 0xff00) | ((unsigned short)s_uchMsg[13] & 0x00ff);
						m_stSensor.dVel = (double)ushTmp * 0.001 - 30.0;

						m_stSensor.uchQuality = s_uchMsg[14];

						m_stSensor.uchHour = s_uchMsg[15];
						m_stSensor.uchMin = s_uchMsg[16];
						m_stSensor.uchSec = s_uchMsg[17];

						ushTmp = (((unsigned short)s_uchMsg[18] << 8) & 0xff00) | ((unsigned short)s_uchMsg[19] & 0x00ff);
						m_stSensor.dPsi = (double)ushTmp * 0.01 - 180.0;

						ushTmp = (((unsigned short)s_uchMsg[20] << 8) & 0xff00) | ((unsigned short)s_uchMsg[21] & 0x00ff);
						m_stSensor.dRotZ = (double)ushTmp * 0.1 - 2000.0;
					}

					// Ack返送処理
					if (0 <= uchMID && uchMID <= 200) {

						if (uchMID == 200) {
							// WP
							m_core.SetFIFO(200, uchIdx);
						}
						else if (uchMID == 0) {
							// 目標WP
							m_core.SetFIFO(uchMID, (unsigned char)m_core.m_stPlan.nWpTo);
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

void XenoVehicleRcCar::SendXenoController()
{
	int i;
	unsigned char uchMsg[200];
	unsigned char uchID, uchRS;
	unsigned short ushTmp;
	unsigned long ulTmp;

	static unsigned short s_ushCnt = 0;

	uchMsg[0] = 0xff;
	uchMsg[1] = 0xff;
	uchMsg[2] = 0x01;

	ushTmp = s_ushCnt++;
	uchMsg[3] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[4] = (unsigned char)(ushTmp & 0x00ff);

	ulTmp = (unsigned long)((m_stSensor.dLat + 180.0) * 10000000.0);
	uchMsg[5] = (unsigned char)(ulTmp >> 24 & 0x000000ff);
	uchMsg[6] = (unsigned char)(ulTmp >> 16 & 0x000000ff);
	uchMsg[7] = (unsigned char)(ulTmp >> 8 & 0x000000ff);
	uchMsg[8] = (unsigned char)(ulTmp & 0x000000ff);

	ulTmp = (unsigned long)((m_stSensor.dLon + 180.0) * 10000000.0);
	uchMsg[9] = (unsigned char)(ulTmp >> 24 & 0x000000ff);
	uchMsg[10] = (unsigned char)(ulTmp >> 16 & 0x000000ff);
	uchMsg[11] = (unsigned char)(ulTmp >> 8 & 0x000000ff);
	uchMsg[12] = (unsigned char)(ulTmp & 0x000000ff);

	ushTmp = (unsigned short)((m_stSensor.dVel +30.0) * 1000.0);
	uchMsg[13] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
	uchMsg[14] = (unsigned char)(ushTmp & 0x00ff);

	uchMsg[15] = m_stSensor.uchQuality;

	uchMsg[16] = m_stSensor.uchHour;
	uchMsg[17] = m_stSensor.uchMin;
	uchMsg[18] = m_stSensor.uchSec;

	ushTmp = (unsigned short)((m_stSensor.dPsi + 180.0) * 100.0);
	uchMsg[19] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
	uchMsg[20] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = (unsigned short)((m_stSensor.dRotZ + 2000.0) * 10.0);
	uchMsg[21] = ((unsigned char)(ushTmp >> 8) & 0x00ff);
	uchMsg[22] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = m_core.m_stSbusCmd.ushCH1;
	uchMsg[23] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[24] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = m_core.m_stSbusCmd.ushCH4;
	uchMsg[25] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[26] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = m_core.m_stSbusCmd.ushCH8;
	uchMsg[27] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[28] = (unsigned char)(ushTmp & 0x00ff);

	uchMsg[29] = m_core.m_stSbusRcv.uchSBUS23;

	uchMsg[30] = (unsigned char)m_core.m_stPlan.nWpNum;
	uchMsg[31] = (unsigned char)m_core.m_stPlan.nWpTo;

	ushTmp = (unsigned short)((m_stCommand.dPsiCmd + 180.0) * 100.0);
	uchMsg[32] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[33] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = m_stParam.ushStrKp;
	uchMsg[34] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[35] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = m_stParam.ushStrKi;
	uchMsg[36] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[37] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = (unsigned short)((m_stControl.dStrIntg + 100.0) * 100.0);
	uchMsg[38] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[39] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = m_stParam.ushAccKp;
	uchMsg[40] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[41] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = m_stParam.ushAccKi;
	uchMsg[42] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[43] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = (unsigned short)((m_stControl.dAccIntg + 100.0) * 100.0);
	uchMsg[44] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[45] = (unsigned char)(ushTmp & 0x00ff);

	ushTmp = (unsigned short)((m_stNavi.dXTE + 300.0) * 100.0);
	uchMsg[46] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[47] = (unsigned char)(ushTmp & 0x00ff);

	ulTmp = (unsigned long)((m_stNavi.dClosestLat + 180.0) * 10000000.0);
	uchMsg[48] = (unsigned char)(ulTmp >> 24 & 0x000000ff);
	uchMsg[49] = (unsigned char)(ulTmp >> 16 & 0x000000ff);
	uchMsg[50] = (unsigned char)(ulTmp >> 8 & 0x000000ff);
	uchMsg[51] = (unsigned char)(ulTmp & 0x000000ff);

	ulTmp = (unsigned long)((m_stNavi.dClosestLon + 180.0) * 10000000.0);
	uchMsg[52] = (unsigned char)(ulTmp >> 24 & 0x000000ff);
	uchMsg[53] = (unsigned char)(ulTmp >> 16 & 0x000000ff);
	uchMsg[54] = (unsigned char)(ulTmp >> 8 & 0x000000ff);
	uchMsg[55] = (unsigned char)(ulTmp & 0x000000ff);

	ushTmp = (unsigned short)((m_stNavi.dT + 10.0) * 1000.0);
	uchMsg[56] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[57] = (unsigned char)(ushTmp & 0x00ff);

	// 応答1
	m_core.GetFIFO(&uchID, &uchRS);
	uchMsg[58] = uchID;
	uchMsg[59] = uchRS;

	// CRC
	ushTmp = m_core.crc16(0, uchMsg, 60);
	uchMsg[60] = (unsigned char)((ushTmp >> 8) & 0x00ff);
	uchMsg[61] = (unsigned char)(ushTmp & 0x00ff);

	// ﾌｯﾀｰ
	uchMsg[62] = 0x0d;
	uchMsg[63] = 0x0a;

	// 送信
	for (i = 0; i < 64; i++) {
		Serial.write(uchMsg[i]);
	}
}
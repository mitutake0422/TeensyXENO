#include "XenoModeHILS.h"
#include "XenoCore.h"
#include "XenoVehicleFixedWing.h"

XenoModeHILS::XenoModeHILS(XenoCore& core, XenoVehicleFixedWing& vehicle)
    : XenoModeBase(core, vehicle)
{
}

void XenoModeHILS::SendCtrl()
{
    // 元の TeensyXENO::HILS_SendCtrl() を
    // ほぼそのまま貼る
    //
    // 置換ルール:
    // 1. m_stSbusCmd -> m_core.m_stSbusCmd
    // 2. crc16(...)  -> m_core.crc16(...)
    // 3. Teensy依存の serial は元コード通り
	unsigned char uchMsg[20];
	unsigned short ushTmp;
	int i;

	uchMsg[0] = 0xff;
	uchMsg[1] = 0xff;
	uchMsg[2] = 0x10;

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

	ushTmp = m_core.crc16(0, uchMsg, 11);
	uchMsg[11] = (unsigned char)(ushTmp >> 8 & 0x00ff);
	uchMsg[12] = (unsigned char)(ushTmp & 0x00ff);

	uchMsg[13] = (unsigned char)0x0d;
	uchMsg[14] = (unsigned char)0x0a;

	for (i = 0; i < 15; i++) {
		Serial.write(uchMsg[i]);
	}
}

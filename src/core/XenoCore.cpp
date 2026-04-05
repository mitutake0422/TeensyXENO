#include "XenoCore.h"

// 右送りCRC-16-IBM (x16+x15+x2+1)
// ビット反転（初期値=0）
static const unsigned short g_ushCRC16[256] = {
    0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
    0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
    0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
    0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
    0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
    0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
    0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
    0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
    0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
    0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
    0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
    0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
    0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
    0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
    0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
    0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
    0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
    0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
    0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
    0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
    0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
    0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
    0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
    0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
    0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
    0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
    0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
    0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
    0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
    0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
    0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
    0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040
};

XenoCore::XenoCore(int teensyVer)
    : m_nCtrl(MANUAL),
    m_nTeensyVer(teensyVer),
    m_nFIFO_write(0),
    m_nFIFO_read(0)
{
    Serial.begin(115200);

    if (m_nTeensyVer == TEENSY4_0) {
        pinMode(DIP1_40, INPUT_PULLUP);
        pinMode(DIP2_40, INPUT_PULLUP);
        pinMode(DIP3_40, INPUT_PULLUP);
        pinMode(DIP4_40, INPUT_PULLUP);

        pinMode(LED1_40, OUTPUT);
        pinMode(LED2_40, OUTPUT);
        pinMode(LED3_40, OUTPUT);
        pinMode(LED4_40, OUTPUT);
        pinMode(LED5_40, OUTPUT);

        analogWriteFrequency(PWM1_40, 50);
        analogWriteFrequency(PWM2_40, 50);
        analogWriteFrequency(PWM3_40, 50);
        analogWriteFrequency(PWM4_40, 50);
        analogWriteFrequency(PWM5_40, 50);
        analogWriteFrequency(PWM6_40, 50);
        analogWriteFrequency(PWM7_40, 50);
        analogWriteFrequency(PWM8_40, 50);
        analogWriteResolution(11);

        Serial5.begin(100000, SERIAL_8E2_RXINV_TXINV);
    }
    else {
        pinMode(DIP1_41, INPUT_PULLUP);
        pinMode(DIP2_41, INPUT_PULLUP);
        pinMode(DIP3_41, INPUT_PULLUP);
        pinMode(DIP4_41, INPUT_PULLUP);

        pinMode(LED1_41, OUTPUT);
        pinMode(LED2_41, OUTPUT);
        pinMode(LED3_41, OUTPUT);
        pinMode(LED4_41, OUTPUT);
        pinMode(LED5_41, OUTPUT);
        pinMode(LED6_41, OUTPUT);
        pinMode(LED7_41, OUTPUT);
        pinMode(LED8_41, OUTPUT);

        analogWriteFrequency(PWM1_41, 50);
        analogWriteFrequency(PWM2_41, 50);
        analogWriteFrequency(PWM3_41, 50);
        analogWriteFrequency(PWM4_41, 50);
        analogWriteFrequency(PWM5_41, 50);
        analogWriteFrequency(PWM6_41, 50);
        analogWriteFrequency(PWM7_41, 50);
        analogWriteFrequency(PWM8_41, 50);
        analogWriteFrequency(PWM9_41, 50);
        analogWriteFrequency(PWM10_41, 50);
        analogWriteFrequency(PWM11_41, 50);
        analogWriteFrequency(PWM12_41, 50);
        analogWriteFrequency(PWM13_41, 50);
        analogWriteFrequency(PWM14_41, 50);
        analogWriteFrequency(PWM15_41, 50);
        analogWriteFrequency(PWM16_41, 50);
        analogWriteResolution(11);

        Serial3.begin(100000, SERIAL_8E2_RXINV_TXINV);
    }

    InitCoreValue();
}

int XenoCore::GetTeensyVer() const
{
    return m_nTeensyVer;
}

void XenoCore::InitCoreValue()
{
    int i;

    m_stSbusRcv.ushCH1 = 1024;  m_stSbusRcv.ushCH2 = 1024;
    m_stSbusRcv.ushCH3 = 1024;  m_stSbusRcv.ushCH4 = 1024;
    m_stSbusRcv.ushCH5 = 1024;  m_stSbusRcv.ushCH6 = 1024;
    m_stSbusRcv.ushCH7 = 1024;  m_stSbusRcv.ushCH8 = 1024;
    m_stSbusRcv.ushCH9 = 1024;  m_stSbusRcv.ushCH10 = 1024;
    m_stSbusRcv.ushCH11 = 1024; m_stSbusRcv.ushCH12 = 1024;
    m_stSbusRcv.ushCH13 = 1024; m_stSbusRcv.ushCH14 = 1024;
    m_stSbusRcv.ushCH15 = 1024; m_stSbusRcv.ushCH16 = 1024;
    m_stSbusRcv.uchD1 = 0;
    m_stSbusRcv.uchD2 = 0;
    m_stSbusRcv.uchFrameLost = 0;
    m_stSbusRcv.uchFailSafe = 0;
    m_stSbusRcv.uchSBUS23 = 0;

    m_stSbusCmd = m_stSbusRcv;

    for (i = 0; i < NUM_WP; i++) {
        m_stPlan.stWP[i].uchID = 0;
        m_stPlan.stWP[i].dWpLat = 0;
        m_stPlan.stWP[i].dWpLon = 0;
        m_stPlan.stWP[i].dWpAlt = 0;
        m_stPlan.stWP[i].dWpPass = 0;
        m_stPlan.stWP[i].uchFlg = 0;
        m_stPlan.stWP[i].uchNextIdx = 255;
        m_stPlan.stWP[i].uchBackIdx = 255;
    }
    m_stPlan.nWpTo = 255;
    m_stPlan.nWpNum = 0;

    if (m_nTeensyVer == TEENSY4_0) {
        m_stDIP.uchDIP1 = digitalRead(DIP1_40);
        m_stDIP.uchDIP2 = digitalRead(DIP2_40);
        m_stDIP.uchDIP3 = digitalRead(DIP3_40);
        m_stDIP.uchDIP4 = digitalRead(DIP4_40);
    }
    else {
        m_stDIP.uchDIP1 = ~digitalRead(DIP1_41) & 0x01;
        m_stDIP.uchDIP2 = ~digitalRead(DIP2_41) & 0x01;
        m_stDIP.uchDIP3 = ~digitalRead(DIP3_41) & 0x01;
        m_stDIP.uchDIP4 = ~digitalRead(DIP4_41) & 0x01;
    }

    m_nCtrl = MANUAL;

    for (i = 0; i < FIFOBUF; ++i) {
        m_uchFIFO_ID[i] = 255;
        m_uchFIFO_RS[i] = 255;
    }
    m_nFIFO_write = 0;
    m_nFIFO_read = 0;

}

void XenoCore::GetSBUS()
{
    int nRcv;
    char chRcv;

    static unsigned char s_uchMsg[25];
    static int s_nIdx = -1;
    static unsigned char s_uchLED = 0;

    for (;;) {
        if (m_nTeensyVer == TEENSY4_0) {
            nRcv = Serial5.read();
        }
        else {
            nRcv = Serial3.read();
        }

        if (nRcv == -1) {
            break;
        }
        chRcv = (char)nRcv;

        if (s_nIdx < 0 && nRcv == 0x0f) {
            s_nIdx = 0;
        }

        if (s_nIdx >= 0) {
            s_uchMsg[s_nIdx] = (unsigned char)chRcv;

            if (++s_nIdx == 25) {
                ParseSBUS(s_uchMsg);

                if (m_stSbusRcv.ushCH8 < 1023) {
                    m_nCtrl = MANUAL;
                    if (m_nTeensyVer == TEENSY4_0) {
                        digitalWrite(LED1_40, 1);
                        digitalWrite(LED2_40, 0);
                    }
                    else {
                        digitalWrite(LED1_41, 1);
                        digitalWrite(LED2_41, 0);
                    }
                }
                else {
                    m_nCtrl = AUTO;
                    if (m_nTeensyVer == TEENSY4_0) {
                        digitalWrite(LED1_40, 0);
                        digitalWrite(LED2_40, 1);
                    }
                    else {
                        digitalWrite(LED1_41, 0);
                        digitalWrite(LED2_41, 1);
                    }
                }

                if (m_nTeensyVer == TEENSY4_0) {
                    digitalWrite(LED3_40, ++s_uchLED & 0x01);
                    digitalWrite(LED4_40, m_stSbusRcv.uchFrameLost | m_stSbusRcv.uchFailSafe);
                }
                else {
                    digitalWrite(LED3_41, ++s_uchLED & 0x01);
                    digitalWrite(LED4_41, m_stSbusRcv.uchFrameLost | m_stSbusRcv.uchFailSafe);
                }

                s_nIdx = -1;
            }
        }
    }
}

void XenoCore::SendSBUS()
{
    int i;
    unsigned char uchMsg[25];

    if (m_nCtrl == AUTO) {
        SetSBUS(m_stSbusCmd, uchMsg);
    }
    else {
        SetSBUS(m_stSbusRcv, uchMsg);
    }

    for (i = 0; i < 25; i++) {
        if (m_nTeensyVer == TEENSY4_0) {
            Serial5.write(uchMsg[i]);
        }
        else {
            Serial3.write(uchMsg[i]);
        }
    }
}

void XenoCore::SetSBUS(_stSBUS pstSBUS, unsigned char* puchSBUS)
{
    puchSBUS[0] = 0x0f;
    puchSBUS[1] = (unsigned char)(pstSBUS.ushCH1 & 0x00ff);
    puchSBUS[2] = (unsigned char)((pstSBUS.ushCH1 >> 8) & 0x0007) | (unsigned char)((pstSBUS.ushCH2 << 3) & 0x00f8);
    puchSBUS[3] = (unsigned char)((pstSBUS.ushCH2 >> 5) & 0x003f) | (unsigned char)((pstSBUS.ushCH3 << 6) & 0x00c0);
    puchSBUS[4] = (unsigned char)((pstSBUS.ushCH3 >> 2) & 0x00ff);
    puchSBUS[5] = (unsigned char)((pstSBUS.ushCH3 >> 10) & 0x0001) | (unsigned char)((pstSBUS.ushCH4 << 1) & 0x00fe);
    puchSBUS[6] = (unsigned char)((pstSBUS.ushCH4 >> 7) & 0x000f) | (unsigned char)((pstSBUS.ushCH5 << 4) & 0x00f0);
    puchSBUS[7] = (unsigned char)((pstSBUS.ushCH5 >> 4) & 0x007f) | (unsigned char)((pstSBUS.ushCH6 << 7) & 0x0080);
    puchSBUS[8] = (unsigned char)((pstSBUS.ushCH6 >> 1) & 0x00ff);
    puchSBUS[9] = (unsigned char)((pstSBUS.ushCH6 >> 9) & 0x0003) | (unsigned char)((pstSBUS.ushCH7 << 2) & 0x00fc);
    puchSBUS[10] = (unsigned char)((pstSBUS.ushCH7 >> 6) & 0x001f) | (unsigned char)((pstSBUS.ushCH8 << 5) & 0x00e0);
    puchSBUS[11] = (unsigned char)((pstSBUS.ushCH8 >> 3) & 0x00ff);

    puchSBUS[12] = (unsigned char)(pstSBUS.ushCH9 & 0x00ff);
    puchSBUS[13] = (unsigned char)((pstSBUS.ushCH9 >> 8) & 0x0007) | (unsigned char)((pstSBUS.ushCH10 << 3) & 0x00f8);
    puchSBUS[14] = (unsigned char)((pstSBUS.ushCH10 >> 5) & 0x003f) | (unsigned char)((pstSBUS.ushCH11 << 6) & 0x00c0);
    puchSBUS[15] = (unsigned char)((pstSBUS.ushCH11 >> 2) & 0x00ff);
    puchSBUS[16] = (unsigned char)((pstSBUS.ushCH11 >> 10) & 0x0001) | (unsigned char)((pstSBUS.ushCH12 << 1) & 0x00fe);
    puchSBUS[17] = (unsigned char)((pstSBUS.ushCH12 >> 7) & 0x000f) | (unsigned char)((pstSBUS.ushCH13 << 4) & 0x00f0);
    puchSBUS[18] = (unsigned char)((pstSBUS.ushCH13 >> 4) & 0x007f) | (unsigned char)((pstSBUS.ushCH14 << 7) & 0x0080);
    puchSBUS[19] = (unsigned char)((pstSBUS.ushCH14 >> 1) & 0x00ff);
    puchSBUS[20] = (unsigned char)((pstSBUS.ushCH14 >> 9) & 0x0003) | (unsigned char)((pstSBUS.ushCH15 << 2) & 0x00fc);
    puchSBUS[21] = (unsigned char)((pstSBUS.ushCH15 >> 6) & 0x001f) | (unsigned char)((pstSBUS.ushCH16 << 5) & 0x00e0);
    puchSBUS[22] = (unsigned char)((pstSBUS.ushCH16 >> 3) & 0x00ff);

    puchSBUS[23] = pstSBUS.uchD1 | (pstSBUS.uchD2 << 1) |
        (pstSBUS.uchFrameLost << 2) | (pstSBUS.uchFailSafe << 3);
    puchSBUS[24] = 0x00;
}

void XenoCore::ParseSBUS(unsigned char* puchSBUS)
{
    unsigned short ushTmp;

    ushTmp = ((unsigned short)puchSBUS[1] & 0x00ff) | (((unsigned short)puchSBUS[2] << 8) & 0x0700);
    m_stSbusRcv.ushCH1 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[2] >> 3) & 0x001f) | (((unsigned short)puchSBUS[3] << 5) & 0x07e0);
    m_stSbusRcv.ushCH2 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[3] >> 6) & 0x0003) | (((unsigned short)puchSBUS[4] << 2) & 0x03fc) | (((unsigned short)puchSBUS[5] << 10) & 0x0400);
    m_stSbusRcv.ushCH3 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[5] >> 1) & 0x007f) | (((unsigned short)puchSBUS[6] << 7) & 0x0780);
    m_stSbusRcv.ushCH4 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[6] >> 4) & 0x000f) | (((unsigned short)puchSBUS[7] << 4) & 0x07f0);
    m_stSbusRcv.ushCH5 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[7] >> 7) & 0x0001) | (((unsigned short)puchSBUS[8] << 1) & 0x01fe) | (((unsigned short)puchSBUS[9] << 9) & 0x0600);
    m_stSbusRcv.ushCH6 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[9] >> 2) & 0x003f) | (((unsigned short)puchSBUS[10] << 6) & 0x07c0);
    m_stSbusRcv.ushCH7 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[10] >> 5) & 0x0007) | (((unsigned short)puchSBUS[11] << 3) & 0x07f8);
    m_stSbusRcv.ushCH8 = ushTmp;

    ushTmp = ((unsigned short)puchSBUS[12] & 0x00ff) | (((unsigned short)puchSBUS[13] << 8) & 0x0700);
    m_stSbusRcv.ushCH9 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[13] >> 3) & 0x001f) | (((unsigned short)puchSBUS[14] << 5) & 0x07e0);
    m_stSbusRcv.ushCH10 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[14] >> 6) & 0x0003) | (((unsigned short)puchSBUS[15] << 2) & 0x03fc) | (((unsigned short)puchSBUS[16] << 10) & 0x0400);
    m_stSbusRcv.ushCH11 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[16] >> 1) & 0x007f) | (((unsigned short)puchSBUS[17] << 7) & 0x0780);
    m_stSbusRcv.ushCH12 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[17] >> 4) & 0x000f) | (((unsigned short)puchSBUS[18] << 4) & 0x07f0);
    m_stSbusRcv.ushCH13 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[18] >> 7) & 0x0001) | (((unsigned short)puchSBUS[19] << 1) & 0x01fe) | (((unsigned short)puchSBUS[20] << 9) & 0x0600);
    m_stSbusRcv.ushCH14 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[20] >> 2) & 0x003f) | (((unsigned short)puchSBUS[21] << 6) & 0x07c0);
    m_stSbusRcv.ushCH15 = ushTmp;

    ushTmp = (((unsigned short)puchSBUS[21] >> 5) & 0x0007) | (((unsigned short)puchSBUS[22] << 3) & 0x07f8);
    m_stSbusRcv.ushCH16 = ushTmp;

    m_stSbusRcv.uchSBUS23 = puchSBUS[23];
    m_stSbusRcv.uchD1 = puchSBUS[23] & 0x01;
    m_stSbusRcv.uchD2 = (puchSBUS[23] >> 1) & 0x01;
    m_stSbusRcv.uchFrameLost = (puchSBUS[23] >> 2) & 0x01;
    m_stSbusRcv.uchFailSafe = (puchSBUS[23] >> 3) & 0x01;
}

void XenoCore::SendPWM()
{
    auto map11bitToDuty = [](unsigned short ch) -> int {
        // 172..1811 付近を 5%..10% 相当の 11bit duty に変換
        // 50Hz / 11bit -> 2047 * 1ms/20ms ≒ 102, 2ms/20ms ≒ 205
        long val = map((long)ch, 172L, 1811L, 102L, 205L);
        if (val < 0) val = 0;
        if (val > 2047) val = 2047;
        return (int)val;
        };

    if (m_nTeensyVer == TEENSY4_0) {
        analogWrite(PWM1_40, map11bitToDuty(m_stSbusCmd.ushCH1));
        analogWrite(PWM2_40, map11bitToDuty(m_stSbusCmd.ushCH2));
        analogWrite(PWM3_40, map11bitToDuty(m_stSbusCmd.ushCH3));
        analogWrite(PWM4_40, map11bitToDuty(m_stSbusCmd.ushCH4));
        analogWrite(PWM5_40, map11bitToDuty(m_stSbusCmd.ushCH5));
        analogWrite(PWM6_40, map11bitToDuty(m_stSbusCmd.ushCH6));
        analogWrite(PWM7_40, map11bitToDuty(m_stSbusCmd.ushCH7));
        analogWrite(PWM8_40, map11bitToDuty(m_stSbusCmd.ushCH8));
    }
    else {
        analogWrite(PWM1_41, map11bitToDuty(m_stSbusCmd.ushCH1));
        analogWrite(PWM2_41, map11bitToDuty(m_stSbusCmd.ushCH2));
        analogWrite(PWM3_41, map11bitToDuty(m_stSbusCmd.ushCH3));
        analogWrite(PWM4_41, map11bitToDuty(m_stSbusCmd.ushCH4));
        analogWrite(PWM5_41, map11bitToDuty(m_stSbusCmd.ushCH5));
        analogWrite(PWM6_41, map11bitToDuty(m_stSbusCmd.ushCH6));
        analogWrite(PWM7_41, map11bitToDuty(m_stSbusCmd.ushCH7));
        analogWrite(PWM8_41, map11bitToDuty(m_stSbusCmd.ushCH8));
    }
}

void XenoCore::SetFIFO(unsigned char uchID, unsigned char uchRS)
{
    m_uchFIFO_ID[m_nFIFO_write] = uchID;
    m_uchFIFO_RS[m_nFIFO_write] = uchRS;

    if (++m_nFIFO_write == FIFOBUF) {
        m_nFIFO_write = 0;
    }
}

void XenoCore::GetFIFO(unsigned char* puchID, unsigned char* puchRS)
{
    if (m_nFIFO_write == m_nFIFO_read) {
        *puchID = 255;
        *puchRS = 255;
    }
    else {
        *puchID = m_uchFIFO_ID[m_nFIFO_read];
        *puchRS = m_uchFIFO_RS[m_nFIFO_read];

        if (++m_nFIFO_read == FIFOBUF) {
            m_nFIFO_read = 0;
        }
    }
}

unsigned short XenoCore::crc16(unsigned short ushCRC, unsigned char* puchBuf, int nLen)
{
    int i;
    unsigned char uchTmp;

    ushCRC = ~ushCRC;

    for (i = 0; i < nLen; i++) {
        uchTmp = *(puchBuf + i);
        ushCRC = (ushCRC >> 8) ^ g_ushCRC16[(ushCRC ^ uchTmp) & 0x00ff];
    }

    return ~ushCRC;
}
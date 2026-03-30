#include "XenoTypes.h"

class XenoCore {
public:
    explicit XenoCore(int teensyVer);

    void GetSBUS();
    void SendSBUS();
    void SendPWM();

    _stSensor  m_stSensor;
    _stNavi    m_stNavi;
    _stSBUS    m_stSbusRcv;
    _stSBUS    m_stSbusCmd;
    _stDIP     m_stDIP;
    int        m_nCtrl;

    int GetTeensyVer() const;

private:
    int m_nTeensyVer;
    unsigned char m_uchFIFO_ID[FIFOBUF];
    unsigned char m_uchFIFO_RS[FIFOBUF];
    int m_nFIFO_Idx;

    void InitCoreValue();
    void SetSBUS(_stSBUS pstSBUS, unsigned char* puchSBUS);
    void ParseSBUS(unsigned char* puchSBUS);
    void SetFIFO(unsigned char uchID, unsigned char uchRS);
    void GetFIFO(unsigned char* puchID, unsigned char* puchRS);
    unsigned short crc16(unsigned short ushCRC, unsigned char* puchBuf, int nLen);

    friend class XenoVehicleFixedWing;
    friend class XenoModeHILS;
};
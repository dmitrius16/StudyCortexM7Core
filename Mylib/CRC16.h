#pragma once
class CRC16
{
    ushort crc;
    static const ushort Crc16Table[];
public:
    CRC16(){crc = 0xFFFF;}
	ushort CountCRC(unsigned char * pcBlock, unsigned short len);
	void ResetCRC(){crc = 0xFFFF;}
	ushort GetCRC(){return crc;}
    void SetPreCalcCRC(ushort val){crc = val;}
};


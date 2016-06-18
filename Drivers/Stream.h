#pragma once
class CBufferedStream
{
public:
	CBufferedStream(){}
    //func avaliable!!!!
    virtual bool Open()=0;
	virtual bool Close()=0;
	virtual void Flush()=0;
    virtual void SetTimeOut(UINT time)=0;
	virtual char ReadCh() = 0;
	virtual BYTE ReadBytes(BYTE *rxBuffer,int len) = 0;
	virtual BYTE Write(BYTE val) = 0;
	virtual BYTE WriteBytes(BYTE *txBuffer,int len) = 0;
	virtual BYTE Available() = 0;
};

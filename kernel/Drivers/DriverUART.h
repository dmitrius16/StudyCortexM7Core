#pragma once
#include "Stream.h"

#define NUM_USED_UARTS 1

class CDriverUART;

class CDriverUART : public CBufferedStream
{
    UINT m_TimeOut;
    int m_StreamBufferSize;
	int m_HandlInd;
	bool m_bOpen;
    BYTE *pRxBuffer;
private:
    void InitTermPin();
    void Turn485Tx();
    void Turn485Rx();
public:
	CDriverUART() : m_TimeOut(0),m_StreamBufferSize(0),m_bOpen(false){}
	bool Init(USART_TypeDef *pUsart);
	bool Init(USART_TypeDef *pUsart, UINT baudrate, UINT wordlen, UINT stopbits, UINT parity);
    virtual bool Open();
	virtual bool Close();		
    virtual void SetTimeOut(UINT time){m_TimeOut = time;}
	virtual void Flush();
	virtual char ReadCh();
	virtual BYTE ReadBytes(BYTE *rxBuffer,int len);
	virtual BYTE Write(BYTE val);
	virtual BYTE WriteBytes(BYTE *txBuffer,int len);
	virtual BYTE Available();
};



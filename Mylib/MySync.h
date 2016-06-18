#pragma once
//#include "cmsis_os.h"
//#include "define.h"
class CSemaphore
{
	osSemaphoreId m_SemId;
public: 
    static int sSemaphoreNum;
public:
	CSemaphore();
	~CSemaphore();
	bool Create(UINT nInitialValue,UINT nMaxCount = 1);
	void Destroy();
	bool WaitSemaphore();
	bool WaitSemaphoreMs(UINT nTime);
	bool PendSemaphore();
	void PostSemaphore();
};


class CMutex
{
    osMutexId 	m_MutexId;
public: 
    static int sMutexNum;
    CMutex();
    ~CMutex();
    bool Create();
    void Destroy();
    void Lock();
    void Unlock();
};

class CScopedCritSec
{
    CMutex *m_pCritSec;
private:
    CScopedCritSec(const CScopedCritSec& obj);
    CScopedCritSec& operator=(CScopedCritSec& obj);
public:
    CScopedCritSec(CMutex &critSec){m_pCritSec = &critSec; m_pCritSec->Lock();}
    ~CScopedCritSec(){m_pCritSec->Unlock();}
};



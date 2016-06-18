#pragma once


extern void RunProcess(void const *argument);

class CParentProcess
{
protected:
    bool m_bWDTRun;
    UINT m_PeriodWDTAccept_ms;
    CCyclesTime m_AcceptWDT;
public:
    CParentProcess() : m_bWDTRun(false){};
    void EnableProcessWDT(UINT period_ms = 4000);
    virtual ~CParentProcess(){}
	virtual void OnTimer() = 0;
	virtual bool OnInitProcess(void *param = NULL) = 0;
    
    void ProcessWDT();
    bool WDTIsRun(){return m_bWDTRun;}
    UINT GetWDTPeriod_ms(){return m_PeriodWDTAccept_ms;}
    UINT GetTimeFromLastAcceptWDT();
};



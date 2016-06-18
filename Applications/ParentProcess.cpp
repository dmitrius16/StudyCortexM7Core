#include "Application.h"
#include "ParentProcess.h"

void RunProcess(void const *argument)
{
    CParentProcess *pProc = (CParentProcess*)argument;
    pProc->OnTimer();
}

void CParentProcess::EnableProcessWDT(UINT period_ms)
{
    m_bWDTRun = true;
    m_PeriodWDTAccept_ms = period_ms;
    m_AcceptWDT.Start();
}

void CParentProcess::ProcessWDT()
{
    m_AcceptWDT.Start();
}

UINT CParentProcess::GetTimeFromLastAcceptWDT()
{
    return m_AcceptWDT.GetTime_ms();
}

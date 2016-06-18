#pragma once
namespace TUEngine
{

//typedef list<CParentProcess> ProcessList;   


class CTUEngineWDT : public CParentProcess
{
//    ProcessList m_ProcessList;
public:
	virtual bool OnInitProcess(void *param);
	virtual void OnTimer();
    
};

}

void RunProcessManager(void const *argument);
extern TUEngine::CTUEngineWDT g_WDT;



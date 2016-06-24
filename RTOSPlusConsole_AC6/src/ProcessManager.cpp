#include "StdAfx.h"
#include "ProcessManager.h"
#include "stm32f7xx_hal.h"
#include "..\Applications\Console.h"
#include <list>


osThreadDef(Console, RunProcess, osPriorityLow, 0, configMINIMAL_STACK_SIZE);

IWDG_HandleTypeDef hIwdg;

typedef struct
{
    CParentProcess *pProc;
    const osThreadDef_t  *pThreadDef;
}PROCESS_INFO;

typedef std::list<PROCESS_INFO> ThreadList;

void AddProcess(CParentProcess *pProc,const osThreadDef_t  *pThreadDef);
void InitIWDT();

ThreadList g_Threads;

volatile const char* pStrHrdwVers;

void RunProcessManager(void const *argument)
{
	pStrHrdwVers = GetHardwareVersion();	// fictive calls

	AddProcess(&g_Console,osThread(Console));
	g_Console.OnInitProcess((void*)1);

	osThreadCreate(osThread(Console),(void*)&g_Console);
	osDelay(100);

#ifdef USE_HARDWARE_WDT    
    InitIWDT();
    HAL_IWDG_Start(&hIwdg);
#endif
    while(1)
    {
        osDelay(500);
#ifdef USE_SOFTWARE_WDT        
        for(ThreadList::iterator it = g_Threads.begin(); it != g_Threads.end();++it)
        {
            if(it->pProc->WDTIsRun())
            {
               if(it->pProc->GetTimeFromLastAcceptWDT() > it->pProc->GetWDTPeriod_ms())
               {
                  
                   my_printf("Process %s is hang!!!\r\nRestart CortexM4\r\n",it->pThreadDef->name);
#ifdef USE_HARDWARE_WDT                   
                   while(1)
                   {
                        __NOP();
                   }
#endif
               }
            }
        }
#endif
#ifdef USE_HARDWARE_WDT
        HAL_IWDG_Refresh(&hIwdg);
#endif
    }
}


void AddProcess(CParentProcess *pProc,const osThreadDef_t  *pThreadDef)
{
    PROCESS_INFO procInf = {pProc,pThreadDef};
    g_Threads.push_back(procInf);
}

void InitIWDT()
{
    hIwdg.Instance = IWDG;
    hIwdg.Init.Prescaler = IWDG_PRESCALER_32; 
    hIwdg.Init.Reload = 0xFFF;
    HAL_IWDG_Init(&hIwdg);
}




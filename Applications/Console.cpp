#include "Application.h"
#include <stdio.h>
#include <string.h>
#include "Console.h"
#include "..\Drivers\DriverUART.h"

#ifndef TU8_APP
#include "..\Applications\TUEngineApp\TUEngineHAL.h"
#endif


CConsole g_Console;

#ifdef TS337A
	const char* CConsole::pPromtStr =  "\r\nTS337 >>>";
#elif defined(TU8_APP)
	const char* CConsole::pPromtStr =  "\r\nTU8 >>>";
#else
	const char* CConsole::pPromtStr =  "\r\nARM >>>";
#endif

const char* CConsole::pLongInput = "\r\nInput very long";


CConsole::CConsole() : m_bInit(false),m_bMakeRepeatCalls(false),m_bMakeClearDisplay(false),m_CmdEmptyInd(0),m_CurCmdInd(-1),m_CurParamInd(0),
					   m_RepetCallCnt(0)
{

}


bool CConsole::OnInitProcess(void *param )
{
    m_ConsLock.Create();
    CDriverUART *pDev = new CDriverUART();
	
    
	// hardware initialization console must be in other file
    
    //define hardware version TUEngine 
#ifndef TU8_APP
	USHORT hdwrVers = g_TUEngineHAL.GetHardwareImplVersion();
	if(hdwrVers == 0)
    {
        if(!pDev->Init(USART1))
			return false;
    }
    else if(hdwrVers == 1)
    {
        if(!pDev->Init(UART4))
			return false;
    }
#else
	//if(!pDev->Init(UART4))
	int valParam = reinterpret_cast<int>(reinterpret_cast<int*>(param));
	
	if(valParam == 1)
    {	
		if(!pDev->Init(USART1))
			return false;
	}
	else
	{
		if(!pDev->Init(UART4))
			return false;
	}
#endif	

	m_pStream = pDev;
    
	m_bInit = true;
    return true;
}

int CConsole::PrintData(const char* format, va_list &arglist)
{
    int size = vsnprintf(TxBuffer, CONSOLE_BUFFER_SIZE-1, format, arglist);
    m_pStream->WriteBytes((BYTE*)TxBuffer,size);
    return size;
}

void CConsole::OnTimer()
{
   my_printf(pPromtStr);
   AddConsoleCommand("?",this);
   
   m_pStream->SetTimeOut(1);
   m_pStream->Open();
	while(1)
	{
		osDelay(1);
		ProcessStream();
	}
}


void CConsole::ProcessStream()
{       
	BYTE cntRxSmbls = m_pStream->Available();
	if(cntRxSmbls != 0)
	{	
		m_bMakeRepeatCalls = m_bMakeClearDisplay = false;

		if((m_curRcvIndex + cntRxSmbls) > (sizeof(RxBuffer)/sizeof(*RxBuffer) - 1))
		{
			m_curRcvIndex = 0;
			my_printf(pLongInput);
			my_printf(pPromtStr);
			m_pStream->Flush();
			return;
			//continue;
		}
		cntRxSmbls = m_pStream->ReadBytes((BYTE*)&RxBuffer[m_curRcvIndex],cntRxSmbls);
		m_pStream->WriteBytes((BYTE*)&RxBuffer[m_curRcvIndex],cntRxSmbls);
		
		m_curRcvIndex += cntRxSmbls;//!!!!
		
		if(RxBuffer[m_curRcvIndex - 1] == '\r')  
		{    
			RxBuffer[m_curRcvIndex - 1] = 0;
			m_curRcvIndex = 0;

			ParseRxCmd();
		
			if(!m_bMakeRepeatCalls)
				my_printf(pPromtStr);
		}	
	}
	else 
		CallCmdMngr();
}

void CConsole::ParseRxCmd()
{
	memset(m_pCmdParams,0,sizeof(m_pCmdParams)/sizeof(m_pCmdParams[0]));

	char *pStrCmd = strtok(RxBuffer," ");
	// find command
	if(pStrCmd != NULL)
	{
		if(FindConsCmd(pStrCmd) != -1)
		{
			m_CurParamInd = 0;
			//~~~ get command parameters
			while(pStrCmd!=NULL)
			{
				if(m_CurParamInd == sizeof(m_pCmdParams)/sizeof(m_pCmdParams[0]))
				{	
					my_printf("Unsupported count parameters\r\n");
					//~~~ not accept command
					break;
				}
				m_pCmdParams[m_CurParamInd++] = pStrCmd;//
				pStrCmd = strtok(NULL," ");
				
				//check for --c param
				
				if(!strcmp(pStrCmd,"--c"))
				{    
					m_bMakeRepeatCalls = true;
					break;
				}
				else if(!strcmp(pStrCmd,"--v"))
				{
					m_bMakeRepeatCalls = true;
					m_bMakeClearDisplay = true;
					break;
				}
			}
			my_printf("\r\n");
			CallCmd();
			
		}			
	}
}

void CConsole::CallCmd()
{
	if(m_ConsoleCmdBuf[m_CurCmdInd].pConsCmd != NULL)
	{	
		if(m_bMakeClearDisplay)
			clear_screen();
		m_ConsoleCmdBuf[m_CurCmdInd].pConsCmd->Command(m_CurParamInd,m_pCmdParams);
	}
}

void CConsole::CallCmdMngr()
{
	if( m_bMakeRepeatCalls )
    {
		if(m_RepetCallCnt++ == 1000)
        {    
			CallCmd();
			m_RepetCallCnt = 0;
		}
    }
}

int CConsole::FindConsCmd(char *strName)
{
	if(m_CmdEmptyInd != 0)
	{
		for(int i = 0; i < m_CmdEmptyInd; i++)
		{
			if(!strcmp(strName,m_ConsoleCmdBuf[i].cmdName))
				m_CurCmdInd = i;
		}
	}
	else 
		m_CurCmdInd = -1;
	return m_CurCmdInd;
}

bool CConsole::AddConsCmd(const char* strName,ConsoleCmd *pCmdExec)
{
	if(m_CmdEmptyInd == sizeof(m_ConsoleCmdBuf)/sizeof(m_ConsoleCmdBuf[0]))
	{	
		my_printf("Can't add cmd %s cmd buffer is full\r\n",strName);
		return false;
	}
	//find if command already exsists
	if(m_CmdEmptyInd != 0)
	{
		for(int i = 0;i < m_CmdEmptyInd; i++)
		{
			if(!strcmp(m_ConsoleCmdBuf[i].cmdName,strName))
			{
				my_printf("Command %s already exists\r\n");
				return false;
			}
		}
	}
	m_ConsoleCmdBuf[m_CmdEmptyInd].cmdName = strName;
	m_ConsoleCmdBuf[m_CmdEmptyInd].pConsCmd = pCmdExec;
	m_CmdEmptyInd++;
	return true;
}


void CConsole::Command(int argc,char* argv[])
{
	/*
	if(!strcmp(argv[1],"Info"))
	{
		my_printf("System CPU: ARM CortexM4\r\n");
		my_printf("Project: TUEngine\r\n");
	}*/
	//
	for(int i = 0; i < m_CmdEmptyInd;i++)
	{
		my_printf("%s\r\n",m_ConsoleCmdBuf[i].cmdName);
	}
	
}
	

bool AddConsoleCommand(const char* strName,ConsoleCmd *pCmdExec)
{
	return g_Console.AddConsCmd(strName,pCmdExec);
}


void my_printf(const char* format, ...)
{
    if(g_Console.IsConsoleInit())
    {
        CScopedCritSec critSec(g_Console.m_ConsLock);
        va_list paramList;
        va_start(paramList, format);
        int iResult = g_Console.PrintData(format, paramList);
        va_end(paramList);   
    }
}

void clear_screen()
{
	if(g_Console.IsConsoleInit())
    {
		CScopedCritSec critSec(g_Console.m_ConsLock);
		g_Console.m_pStream->Write(0xC);
	}
}

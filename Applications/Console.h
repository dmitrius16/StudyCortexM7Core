#pragma once
#include <stdarg.h>
#include "..\Drivers\Stream.h"


void my_printf(const char* format, ...);
void clear_screen();


const int CONSOLE_BUFFER_SIZE = 256;
const int NUM_CONSOLE_COMMANDS = 30;
const int NUM_CONSOLE_PARAMS = 10;

//#define CONSOLE_BUFFER_SIZE 256
//#define NUM_CONSOLE_COMMANDS 10

class ConsoleCmd
{
public:
	virtual void Command(int argc,char* argv[]) = 0;
	virtual ~ConsoleCmd(){};
};

typedef struct
{
	const char *cmdName;
	ConsoleCmd *pConsCmd;
}ConsCmd;

bool AddConsoleCommand(const char* strName,ConsoleCmd *pCmdExec);

class CConsole : public CParentProcess, ConsoleCmd
{
    friend void my_printf(const char* format, ...);
	friend void  clear_screen();
	friend bool AddConsoleCommand(const char* strName,ConsoleCmd *pCmdExec);
		
	short m_curRcvIndex;
    bool m_bInit;
    bool m_bMakeRepeatCalls;
	bool m_bMakeClearDisplay;
	int  m_CmdEmptyInd;
	int  m_CurCmdInd;
	int  m_CurParamInd;
	int  m_RepetCallCnt;
	CBufferedStream *m_pStream;

	char TxBuffer[CONSOLE_BUFFER_SIZE];
	char RxBuffer[CONSOLE_BUFFER_SIZE];
	
	ConsCmd m_ConsoleCmdBuf[NUM_CONSOLE_COMMANDS];
	char *m_pCmdParams[NUM_CONSOLE_PARAMS];

    static const char* pPromtStr;
    static const char* pLongInput;
public:
    CMutex m_ConsLock;
private:
    int  PrintData(const char* format, va_list &arglist);
	void ProcessStream();
	bool AddConsCmd(const char* strName,ConsoleCmd *pCmdExec);
	int  FindConsCmd(char *strName);
	bool ExecuteConsoleCommand();
	virtual void Command(int argc,char* argv[]);
	void CallCmdMngr();
	void CallCmd();
	void ParseRxCmd();
public:
    CConsole();
	virtual bool OnInitProcess(void *param = NULL);
    virtual void OnTimer();
    bool IsConsoleInit(){return m_bInit;}
};

extern CConsole g_Console;





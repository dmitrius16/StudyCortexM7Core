#include "Application.h"
#include "StatisticsCalls.h"
#include "..\Applications\Console.h"
#include <string.h>


void Statistics::OutputResult(char *pStrName, bool us)
{
	float resultBuf[3];
	const char *ftmStrQuery = pStrName ? "%-*s |%-8.2f|%-8.2f|%-8.2f|%-8d\r\n" : "|%8.2f|%-8.2f|%-8.2f|%-8d\r\n"; 	
    UINT samples;
    if(us)
    	samples = GetResults_us(resultBuf[0],resultBuf[1],resultBuf[2]);  	
    else
     	samples = GetResults_ms(resultBuf[0],resultBuf[1],resultBuf[2]);
    if(pStrName)
    	my_printf(ftmStrQuery,strlen(pStrName),pStrName,resultBuf[0],resultBuf[1],resultBuf[2],samples);	
    else
    	my_printf(ftmStrQuery,resultBuf[0],resultBuf[1],resultBuf[2],samples);
}

void Statistics::OutputHeader(BYTE fieldWidth)
{
	const char *pStrName = "Statistics";
	if(fieldWidth > 40)
		fieldWidth = 40;
	BYTE len = strlen(pStrName);
	BYTE width = len >= fieldWidth ? len : fieldWidth;
	my_printf("\r\n%-*s |%-8s|%-8s|%-8s|%-8s\r\n",width,pStrName,"min val","max val","aver val","samples"); 
}

void Statistics::CreateNameStr(char* pStrBuf,size_t lenBuf,const char* pStrPostfix)
{
	const char *pStrName = m_Name.c_str();
	int maxNameSize = (lenBuf - strlen(pStrPostfix) - 1);
		
	int cntBytes = m_Name.length() >= maxNameSize ? maxNameSize : m_Name.length();
	strncpy(pStrBuf,pStrName,cntBytes);
    pStrBuf[cntBytes] = 0;
	strcat(pStrBuf,pStrPostfix);
}

void Statistics::OutputResult_ms(bool bPrintHeader, bool bPrintName)
{
	char nameStr[40];
	const char *pStrPostfix = ",ms";
	char *pNameStr = bPrintName ? nameStr : NULL;
	if(bPrintName)
    {	
		CreateNameStr(nameStr,sizeof(nameStr),pStrPostfix);
		if(bPrintHeader)
			OutputHeader(strlen(nameStr));
    }
    OutputResult(pNameStr,false);
}

void Statistics::OutputResult_us(bool bPrintHeader, bool bPrintName)
{
	char nameStr[40];
	const char *pStrPostfix = ",us";
	char *pNameStr = bPrintName ? nameStr : NULL;
	if(bPrintName)
    {	
		CreateNameStr(nameStr,sizeof(nameStr),pStrPostfix);
		if(bPrintHeader)
			OutputHeader(strlen(nameStr));
    }
    OutputResult(pNameStr,true);   
}

void Statistics::OutOneShotResult_ms()
{
	float resultBuf[3];
	UINT samples = GetResults_ms(resultBuf[0],resultBuf[1],resultBuf[2]);
	my_printf("%-*s = %-8.2f\r\n",m_Name.length(),m_Name.c_str(),resultBuf[1]);
}


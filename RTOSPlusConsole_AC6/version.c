//#include "version.h"
#include "StdAfx.h"

const char* GetFullVersionString(void)
{
	const char* pMagicStr = "#Team-R#$ProgramVersionString$:" PROGRAM_VERSION_TEXT_FULL_VERS "@"PROGRAM_VERSION_DATE"*end.\0";
	return pMagicStr;
}

const char* GetHardwareVersion(void)
{
	const char* pMagicStr_1 =  "#Team-R#$HardwareTypeStrings$:" PROGRAM_HARDWARE_TYPE "\0";
	return pMagicStr_1;
}



const char* GetVersionString()
{
	return PROGRAM_VERSION_TEXT;
}


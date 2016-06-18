#pragma once 
#define PROGRAM_VERSION_A	"0"
#define PROGRAM_VERSION_B	"0"
#define PROGRAM_VERSION_C	"1"

#if !defined(PROGRAM_VERSION_D)
#define PROGRAM_VERSION_D   "F"
#endif



#define PROGRAM_VERSION_DATE __DATE__

// PROGRAM_VERSION_D defined used OS F- FreeRTOS R - RTX
#define PROGRAM_HARDWARE_TYPE "TU8_v1"

#define PROGRAM_VERSION_TEXT PROGRAM_VERSION_A "." PROGRAM_VERSION_B "." PROGRAM_VERSION_C "." PROGRAM_VERSION_D "\0"

#define PROGRAM_VERSION_TEXT_FULL_VERS PROGRAM_VERSION_A "." PROGRAM_VERSION_B "." PROGRAM_VERSION_C PROGRAM_VERSION_D



#ifdef __cplusplus 
	extern "C"{
#endif
const char* GetFullVersionString(void);
const char* GetHardwareVersion(void);
#ifdef __cplusplus 
}
#endif

#pragma once
// ############################### Application Name #################################
#define CORTEX_M7_TEMPLATE_APP


// ############################### Board Name #######################################
#define STM32F746DISCOVERY



// ############################### WDT Configuration ################################

//#define USE_DEBUG_CONSOLE
#undef USE_DEBUG_CONSOLE

//#define USE_HARDWARE_WDT
#undef  USE_HARDWARE_WDT
//#define USE_SOFTWARE_WDT
#undef  USE_SOFTWARE_WDT

// ############################### end WDT Config ####################################


// #################### RTOS primitives synchronization configuration ###################################

#define MAX_COUNT_SEMAPHORES 30
#define MAX_COUNT_MUTEX 30

// ################################ Repair Mode #########################################################
//#define USE_DIAGNOSTIC_MODE
//#undef USE_DIAGNOSTIC_MODE

//#define DIAGNOSTIC_WITHOUT_CHECKS
//#undef DIAGNOSTIC_WITHOUT_CHECKS

#if defined(USE_DIAGNOSTIC_MODE) || defined(DIAGNOSTIC_WITHOUT_CHECKS)
	#define PROGRAM_VERSION_D "TEST"
#else
	#undef PROGRAM_VERSION_D
#endif

// ############################## end RTOS primitives synchronization config ############################




#include "version.h"

#ifdef __cplusplus

//header describe hardware that used in project
#include "HardwareDef.h"

// include types used in project
#include "..\MyLib\define.h"

// Lib
#include "..\MyLib\CRC16.h"
#include "..\MyLib\CyclesTimer.h"

// RTOS
#include "cmsis_os.h"
#include "..\Mylib\MySync.h"
#include "..\Applications\ParentProcess.h"

#endif



#include "Application.h"
//#include "MySync.h"
#include <limits>

os_semaphore_def semdefs[MAX_COUNT_SEMAPHORES]; // pool of semaphores
os_mutex_def mutexDefs[MAX_COUNT_MUTEX];
#ifdef RTE_CMSIS_RTOS_RTX
	uint32_t os_sem_memory[2*MAX_COUNT_SEMAPHORES] = { 0 };	// see cmsis_os.h for details
	uint32_t os_mutex_memory[4*MAX_COUNT_MUTEX] = { 0 }; // // see cmsis_os.h for details
#endif



int CSemaphore::sSemaphoreNum = 0;
int CMutex::sMutexNum = 0;
/*********************************************************************************
*   CSemaphore - constructor
**********************************************************************************/
CSemaphore::CSemaphore()
{
	m_SemId = (osSemaphoreId)std::numeric_limits<UINT>::max();
}

/*********************************************************************************
*
**********************************************************************************/
CSemaphore::~CSemaphore()
{
    Destroy();
}

/********************************************************************************
* Create -member function that create semaphore
*   Input:
*       nInitialValue - init value of semaphore counter
*       nMaxCount - max value for semaphore counter
**********************************************************************************/
bool CSemaphore::Create(UINT nInitialValue,UINT nMaxCount)
{
    if(sSemaphoreNum < MAX_COUNT_SEMAPHORES )
    {
#ifdef RTE_CMSIS_RTOS_RTX
			semdefs[sSemaphoreNum].semaphore = (void*)&os_sem_memory[sSemaphoreNum * 2];
#endif
				m_SemId = osSemaphoreCreate(&semdefs[sSemaphoreNum],nMaxCount);
        if(m_SemId == NULL)
            return false;
        sSemaphoreNum++;
        while(nInitialValue < nMaxCount)
        {
            osSemaphoreWait(m_SemId,osWaitForever);
            nInitialValue++;
        }
        return true;
    }
    return false;
}

/********************************************************************************
*   Destroy - member func that destroyed early created semaphore
*********************************************************************************/
void CSemaphore::Destroy()
{
    //write additional code for maintain semaphores pool
    osSemaphoreDelete(m_SemId);
    m_SemId = (osSemaphoreId)std::numeric_limits<UINT>::max();
}

/*******************************************************************************
*   WaitSemaphore - member func that wait until Semaphore will be free
********************************************************************************/
bool CSemaphore::WaitSemaphore()
{
    return osSemaphoreWait(m_SemId,osWaitForever) == osOK;
}

/*******************************************************************************
*   WaitSemaphore - member func that wait until Semaphore will be free
*   with timeout in ms
*   Input:
*       nTime - timeout in ms
********************************************************************************/
bool CSemaphore::WaitSemaphoreMs(UINT nTime)
{
    return osSemaphoreWait(m_SemId,nTime) == osOK;
}

/******************************************************************************
*   PendSemaphore - member func that ask if semaphore avaliable
*******************************************************************************/
bool CSemaphore::PendSemaphore()
{
    return osSemaphoreWait(m_SemId,0) == osOK;
}

/*****************************************************************************
*   PostSemaphore - member func that post semaphore from ISR or Thread
******************************************************************************/
void CSemaphore::PostSemaphore()
{
    osSemaphoreRelease(m_SemId);
}

/////////////########################## Mutex functions ##############################
CMutex::CMutex()
{
    m_MutexId = (osMutexId)std::numeric_limits<UINT>::max();
}

CMutex::~CMutex()
{
    Destroy();
}

bool CMutex::Create()
{
    if(sMutexNum < MAX_COUNT_MUTEX)
    {
#ifdef RTE_CMSIS_RTOS_RTX
			mutexDefs[sMutexNum].mutex = (void*)&os_mutex_memory[4*sMutexNum];
#endif
			
		m_MutexId = osMutexCreate(&mutexDefs[sMutexNum]);
		if(m_MutexId == NULL)
			return false;
				
        sMutexNum++;
        return true;
    }
    return false;
}



void CMutex::Destroy()
{
    osMutexDelete(m_MutexId);
    m_MutexId = (osMutexId)std::numeric_limits<UINT>::max();
}

void CMutex::Lock()
{
    osMutexWait(m_MutexId,osWaitForever);
}

void CMutex::Unlock()
{
    osMutexRelease(m_MutexId);
}




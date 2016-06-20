#include "Hardwaredef.h"
#include "define.h"
#include "CyclesTimer.h"
#include <limits>

TIM_HandleTypeDef hTim;
UINT g_nCounter = 0;

UINT CCyclesTime::s_ClockFreq;
UINT CCyclesTime::s_Ticks_in_ms;
UINT CCyclesTime::s_Ticks_in_us;

extern "C"{
    void TIMx_IRQHandler(void)
    {
        HAL_TIM_IRQHandler(&hTim);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    g_nCounter++;
}

//*****************************************************************************
//  Init - init cycles counter with freq
//      Input:
//            UINT freq - freq that clocked hardware timer
//*****************************************************************************
void CCyclesTime::Init()
{
	/// setup clock freerun counter
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	s_ClockFreq = HAL_RCC_GetPCLK1Freq();
	uint32_t flLatency;
	HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &flLatency);
	if(RCC_ClkInitStruct.APB1CLKDivider != RCC_HCLK_DIV1)	// reference manual 405 don't have TIMPRE
	{
		s_ClockFreq*=2;
	}
	if(s_ClockFreq >= 1000000)
	{	
		hTim.Init.Prescaler = s_ClockFreq / 1000000;
		s_Ticks_in_ms = 1000;//s_ClockFreq / 1000;
		s_Ticks_in_us = 1;	//s_Ticks_in_ms / 1000;
		
		hTim.Instance = TIMx;
		//hTim.Init.Prescaler = 0;   
		hTim.Init.CounterMode = TIM_COUNTERMODE_UP;
		hTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		hTim.Init.Period = std::numeric_limits<UINT>::max();//s_ClockFreq; 
		hTim.Init.RepetitionCounter = 0;
		HAL_TIM_Base_Init(&hTim); 
	}
}

void CCyclesTime::StartCyclesTimer()
{
    HAL_TIM_Base_Start_IT(&hTim);
}



//***************************************************************************************
//* timerStart - set time point 
//***************************************************************************************
void CCyclesTime::Start()
{  
    __disable_irq();
    m_StartTime = __HAL_TIM_GetCounter(&hTim);
    __enable_irq();
}

//**************************************************************************************
//*timerStop - calculate time difference between start point and current time
//**************************************************************************************
UINT CCyclesTime::Stop()
{
    __disable_irq();
    m_FinalTime = __HAL_TIM_GetCounter(&hTim);
    __enable_irq();
    if(m_FinalTime > m_StartTime)    
    {
        m_FinalTime -= m_StartTime;
    }
    else
    {
        m_FinalTime = std::numeric_limits<UINT>::max() - (m_StartTime - m_FinalTime);
    }
	return m_FinalTime;
}

//****************************************************************************************
//* timerRestart - used when measures time between periodic event
//****************************************************************************************
void CCyclesTime::Restart()
{
    Stop();
    Start();
}



#include "Application.h"
#include "stm32f4xx_hal.h"
#include "Tu8Led.h"
#include "..\..\Common\TUEngineTypedefs.h"
#include "..\Applications\TU8App\TU8HALImpl.h"
//
//	גûחמג c ןונטמהמל 50 לס
//	//fast flash - 100 לס
//	//flash 	 - 200 לס
//	//slow flash - 400 לס
using namespace TUEngine;
void CLed::ProcessLEDs()
{
	m_TimerScaler++;
	if(m_TimerScaler == 50)	//50 ms
	{
        m_TimerScaler = 0;
	    m_TimerCounter++;
		
		switch(m_LedModeWork)
    	{
		case LED_OFF:
			m_LedActiveCode &= ~m_LedCommandCode;
			break;
		case LED_ON:
			m_LedActiveCode |= m_LedCommandCode;
			break;
		case LED_TOGGLE:
			m_LedActiveCode ^= m_LedCommandCode;
			break;
		case LED_FLASH_FAST:
			if(!(m_TimerCounter & 0x1))
			{
				m_LedActiveCode ^= m_LedCommandCode;
			}
			break;
		case LED_FLASH:
			if(!(m_TimerCounter & 0x3))
			{
				m_LedActiveCode ^= m_LedCommandCode;
			}
			break;
		case LED_FLASH_SLOW:
			if(!(m_TimerCounter & 0x7))
			{
				m_LedActiveCode ^= m_LedCommandCode;
			}
			break;
		}
			
		TUENGINE_ELEMENT_STATE green = m_LedActiveCode & LED_COLOR_GREEN ? ON : OFF;
		TUENGINE_ELEMENT_STATE red = m_LedActiveCode & LED_COLOR_RED ? ON : OFF;
		g_TU8HALImpl.SetRunLed(green,red);
	}
}

//****************************************************************
//*  Set mode for led "Run" 
//****************************************************************
//* Input 
//*     nValue - bit mask for work mode
//*     high 4 bits - color, low 4 bits - work mode
//****************************************************************
void CLed::SetMode4RunLed(uint8_t nValue)
{
    m_LedActiveCode = 0;
    m_LedCommandCode = (nValue & 0x30);
    m_LedModeWork = (nValue & 0x0F);
}
	
void CLed::Run(uint8_t nValue)
{
    SetMode4RunLed(nValue);
}

CLed g_LED;

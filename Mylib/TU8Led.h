#pragma once


//Schema TU8 Led
/*
	LED position
	LED0  - RUN (only this LED can be controlled)
	LED1  - STATE (Driven by RX/TX)
	LED3  - RS485
*/
/*
#define LED_MODE_RED    0x1
#define LED_MODE_GREEN  0x2

#define LED_STATE_RED   0x4
#define LED_STATE_GREEN 0x8
*/
#define LED_RUN_RED		0x1
#define LED_RUN_GREEN   0x2

// color
#define LED_COLOR_NONE      0x00
#define LED_COLOR_RED       0x10
#define LED_COLOR_GREEN     0x20
#define LED_COLOR_YELLOW    0x30
#define LED_COLOR_RESET     0x80

// режим работы индикатора
#define LED_OFF 0x0
#define LED_ON  0x1
#define LED_TOGGLE 0x2
#define LED_FLASH_SLOW 0x3
#define LED_FLASH 0x4
#define LED_FLASH_FAST 0x5

class CLed
{
	uint8_t  m_LedActiveCode;
	uint8_t  m_LedCommandCode;
	uint8_t  m_TimerCounter;
	uint8_t	 m_TimerScaler;
	uint16_t m_LedModeWork;
	
private:
	void SetMode4RunLed(uint8_t nValue);
public:
	void Init();
    void ProcessLEDs();
	void Run(uint8_t nValue);
};

extern CLed g_LED;

#pragma once
class CCyclesTime
{
    UINT m_StartTime;
    UINT m_FinalTime;
    UINT m_TimerPeriod;

    static UINT s_ClockFreq;
    static UINT s_Ticks_in_ms;
    static UINT s_Ticks_in_us;
private:
    UINT timerGet_ms(){ return (m_FinalTime / s_Ticks_in_ms); }
    LONGLONG timerGet_us(){ return (m_FinalTime / s_Ticks_in_us); }
public:
    static void Init();
    static void StartCyclesTimer();
	static UINT GetTimerClockFreq(){return s_ClockFreq;}
	static UINT GetTimerTicksInMs(){return s_Ticks_in_ms;}
	static UINT GetTimerTicksInUs(){return s_Ticks_in_us;}
	//static UINT GetTicks

    CCyclesTime() : m_StartTime(0),m_FinalTime(0),m_TimerPeriod(0){};
    virtual ~CCyclesTime(){};
    void Start();   
    UINT Stop();
    void Restart();
    UINT GetTimeBtwnCalls()
    {
        Restart();
        return timerGet_ms();
    }
    UINT GetTime_ms()
    {
        Stop();
        return timerGet_ms();
    }
    void StartTimePeriod_ms(int periodVal_ms){m_TimerPeriod = periodVal_ms * s_Ticks_in_ms;Start();}
    void StartTimePeriod_us(int periodVal_us){m_TimerPeriod = periodVal_us * s_Ticks_in_us;Start();}
    bool IsTimePeriodExpired()
    {
        Stop();
        return m_FinalTime >= m_TimerPeriod;
    }  
};

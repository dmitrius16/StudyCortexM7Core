#pragma once
#include <limits>
#include <string>
class Statistics
{
	UINT m_CountAvrg;
	int m_AccSamples;
	int m_MaxValue;
	int m_MinValue;
	int m_LastVal;
	
	int m_AccSamplesRes;
	int m_MaxValRes;
	int m_MinValRes;
	
	CCyclesTime m_Timer;

	std::string  m_Name;
private:
	void SetSample(int Value)
	{
		m_CountAvrg++;
		m_AccSamples += Value;
		if(Value < m_MinValue)
			m_MinValue = Value;
		if(Value > m_MaxValue)
			m_MaxValue = Value;
		m_LastVal = Value;
	}
	
	UINT GetResults_ms(float &minVal,float &maxVal,float &avrVal)
	{
		UINT cntSamples = ResetStat();
		float coeffMs = CCyclesTime::GetTimerTicksInMs();//(float)GetCCLK() / 1000.;
		minVal = (float)m_MinValRes / coeffMs;
		maxVal = (float)m_MaxValRes / coeffMs;
		avrVal = ((float)m_AccSamplesRes / (float)cntSamples)/ coeffMs;
		return cntSamples;
	}
	
	UINT GetResults_us(float &minVal,float &maxVal,float &avrVal)
	{
			UINT cntSamples = ResetStat();
		float coeffMs = CCyclesTime::GetTimerTicksInUs();//(float)GetCCLK() / 1000000.;
		minVal = (float)m_MinValRes / coeffMs;
		maxVal = (float)m_MaxValRes / coeffMs;
		avrVal = ((float)m_AccSamplesRes / (float)cntSamples) / coeffMs;
		return cntSamples;
	}

	UINT ResetStat()
	{
		__disable_irq();
		UINT tempCnt = m_CountAvrg;
		m_AccSamplesRes = m_AccSamples;
		m_MaxValRes = m_MaxValue;
		m_MinValRes = m_MinValue;
		__enable_irq();
		m_CountAvrg = 0;
		m_AccSamples = 0;
		m_MinValue = std::numeric_limits<int>::max();
		m_MaxValue = std::numeric_limits<int>::min();
		return tempCnt;
	}
	void OutputResult(char *pStrName, bool us);
	void CreateNameStr(char* pStrBuf,size_t lenBuf,const char* pStrPostfix);
	void OutputHeader(BYTE fieldWidth);
public:
	Statistics() : m_AccSamples(0), m_Name("No_name")
	{
		m_CountAvrg = 0;
		m_MinValue = std::numeric_limits<int>::max();
		m_MaxValue = std::numeric_limits<int>::min();
	}
	Statistics(std::string Name) : m_AccSamples(0),m_Name(Name)
	{
		m_CountAvrg = 0;
		m_MinValue = std::numeric_limits<int>::max();
		m_MaxValue = std::numeric_limits<int>::min();
	}
	
	void Start()
	{
		m_Timer.Start();
	}
	void Stop()
	{
		SetSample((int)m_Timer.Stop());
	}
	
	
	void OutputResult_ms(bool bPrintHeader = true, bool bPrintName = true);
	void OutputResult_us(bool bPrintHeader = true, bool bPrintName = true);
	void OutOneShotResult_ms();
	
	int GetLastResult()
	{
		return m_LastVal;
	}
};



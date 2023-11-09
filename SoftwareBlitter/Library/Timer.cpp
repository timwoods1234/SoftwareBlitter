
#pragma comment(lib, "Winmm.lib")

#include "windows.h"
#include "Timer.h"

/*****************************************************************************************/
Timer::Timer(void)
{
}

/*****************************************************************************************/
Timer::~Timer(void)
{
}

/*****************************************************************************************/
unsigned __int64 Timer::GetFrequency() const
{
	unsigned __int64 performanceFrequency = 0;
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &performanceFrequency))
	{
		performanceFrequency = 1000;
	}

	return performanceFrequency;
}

/*****************************************************************************************/
unsigned __int64 Timer::GetTime() const
{
	__int64 timeTicks = 0;

	if (!QueryPerformanceCounter((LARGE_INTEGER *) &timeTicks))
	{
		timeTicks = (__int64) timeGetTime();
	}

	return timeTicks;
}

/*****************************************************************************************/
void Timer::Update()
{
	unsigned __int64 timeNow = GetTime();
	unsigned __int64 timeDiff = timeNow - m_lastTime;
	m_lastTime = timeNow;

	unsigned __int64 frequence = GetFrequency();

	if (timeDiff > frequence)
	{
		timeDiff = frequence;
	}

	float timeDiffFloat = (float)timeDiff / (float)frequence;

	m_elapsedFrameTime = timeDiffFloat;
}

/*****************************************************************************************/
float Timer::GetElapsedFrameTime() const
{
	return m_elapsedFrameTime;
}

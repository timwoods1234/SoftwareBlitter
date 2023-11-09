#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer(void);

	~Timer(void);

	unsigned __int64 GetFrequency() const;

	unsigned __int64 GetTime() const;

	void Update();

	float GetElapsedFrameTime() const;

private:
	unsigned __int64 m_currentTime;

	unsigned __int64 m_lastTime;

	float m_elapsedFrameTime;
};

#endif //__TIMER_H__

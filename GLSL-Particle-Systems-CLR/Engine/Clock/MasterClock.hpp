#pragma once
#ifndef _INCLUDE_MASTERCLOCK_
#define _INCLUDE_MASTERCLOCK_
#include <vector>
#include <map>
#include <string>
#include "windows.h"
#include "Engine/EngineCommon/EngineCommon.hpp"
typedef void(*callback_t) (const std::string& event_name, void *arg);
typedef unsigned long long uint64_t;

class Clock;
class Alarm;
//===================================================================================================
//               class MasterClock                                                                 ==
//===================================================================================================
class MasterClock
{
public:
	MasterClock();
	~MasterClock();

	void	UpdateClock(float deltaSeconds);
	void	AdvanceTime(float deltaSeconds);
	void	AdvanceAlarms(float deltaSeconds);

	void	SetTimeScale(float timeScale)					{ m_timeScale = timeScale; }
	void	SetPaused(bool state)						{ m_isPaused = state; }

	double	GetClockCurrentSeconds() { return g_currentSeconds; }
	float	GetClockDeltaSeconds() { return m_deltaSeconds; }
	float   GetTotalTime() { return m_totalTime; }
	float   GetTimeScale() { return m_timeScale; }
	bool	GetIsPaused() { return m_isPaused; }
	void	TogglePause() { m_isPaused = !m_isPaused; }

	void	AddChild(Clock* newChild);

	void	CreateAlarm(const std::string& eventName, float duration, void* data, callback_t cb);
	Alarm*	GetAlarm(const std::string& alarmName);

	static void		InitializeTimer();
	static double   GetAbsoluteTimeSeconds();
	static uint64_t GetAbsoluteTimeMicroSeconds();
	static uint64_t GetAbsoluteTimeMiliSeconds();

	static MasterClock* GetMasterClock() { return g_masterClock; }

private:
	std::vector<Clock*>				m_childrenClocks;
	std::map<std::string, Alarm* >	m_alarms;
	float							m_deltaSeconds;
	double							m_currentSeconds;
	float							m_totalTime;
	float							m_timeScale;
	bool							m_isPaused;
	static  MasterClock*			g_masterClock;
};

#endif // !_INCLUDE_MASTERCLOCK_
#pragma once
#ifndef _INCLUDE_ALARM_
#define _INCLUDE_ALARM_
#include "MasterClock.hpp"

//===================================================================================================
//               class Alarm		                                                               ==
//===================================================================================================
class Alarm
{
public:
	Alarm(const std::string& eventName, float duration, void* data, callback_t cb);

	void	UpdateAlarm(float deltaSeconds);
	void 	SetAlarmDuration(float duration) { m_alarmDuration = duration; }

	float	GetAlarmDuration() { return m_alarmDuration; }
	float	GetAlarmRemainingTime() { return m_remainingTime; }
	bool	GetHasAlarmExpired() { return m_hasExpired; }
	static void OnAlarm(const std::string& eventName, void *arg);

private:
	std::string m_alarmEventName;
	float		m_alarmDuration;
	float		m_remainingTime;
	void*		m_data;
	callback_t  m_callbackFunction;
	bool		m_hasExpired;
};


#endif // !_INCLUDE_ALARM_

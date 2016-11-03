#include "Alarm.hpp"


//===================================================================================================
//               class Alarm                                                                       ==
//===================================================================================================
Alarm::Alarm(const std::string& eventName, float duration, void* data, callback_t cb)
	: m_alarmEventName(eventName)
	, m_alarmDuration(duration)
	, m_remainingTime(duration)
	, m_data(data)
	, m_callbackFunction(cb)
	, m_hasExpired(false)
{

}


//----------------------------------------------------------------------------------------------
void Alarm::UpdateAlarm(float deltaSeconds)
{
	m_remainingTime -= deltaSeconds;
	if (m_remainingTime <= 0.0f)
	{
		m_callbackFunction(m_alarmEventName, m_data);
		m_hasExpired = true;
	}
}


//----------------------------------------------------------------------------------------------
void Alarm::OnAlarm(const std::string& eventName, void *arg)
{
	eventName;
	arg;
}
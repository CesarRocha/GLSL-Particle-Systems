#include "Clock.hpp"
#include "Alarm.hpp"
MasterClock* MasterClock::g_masterClock = new MasterClock();

//===================================================================================================
//				class MasterCLock																   ==
//===================================================================================================
MasterClock::MasterClock()
	: m_deltaSeconds(0.0f)
	, m_currentSeconds(0.0)
	, m_timeScale(1.0f)
	, m_isPaused(false)
{
	InitializeTimer();
}
MasterClock::~MasterClock()
{

}

void	 MasterClock::UpdateClock(float deltaSeconds)
{
	AdvanceTime(deltaSeconds);
	AdvanceAlarms(deltaSeconds);
}
void	 MasterClock::AddChild(Clock* newChild)
{
	if (newChild != nullptr)
		m_childrenClocks.push_back(newChild);
}
void	 MasterClock::AdvanceTime(float deltaSeconds)
{
	deltaSeconds *= m_timeScale;
	if (m_isPaused)
		return;

	g_currentSeconds += deltaSeconds;
	m_deltaSeconds = deltaSeconds;
	m_totalTime += m_deltaSeconds;

	for (std::vector<Clock*>::iterator iter = m_childrenClocks.begin(); iter < m_childrenClocks.end(); iter++)
	{
		Clock* clock = (*iter);
		clock->UpdateClock(m_deltaSeconds);
	}
}
void	 MasterClock::AdvanceAlarms(float deltaSeconds)
{
	for (std::map<std::string, Alarm* >::iterator iter = m_alarms.begin(); iter != m_alarms.end();)
	{
		iter->second->UpdateAlarm(deltaSeconds);
		if (iter->second->GetHasAlarmExpired() == true)
			iter = m_alarms.erase(iter);
		else
			iter++;
	}
}
void	 MasterClock::CreateAlarm(const std::string& eventName, float duration, void* data, callback_t cb)
{
	Alarm* newAlarm = new Alarm(eventName, duration, data, cb);
	m_alarms.insert(std::pair<std::string, Alarm* >(eventName, newAlarm));
}
Alarm*	 MasterClock::GetAlarm(const std::string& alarmName)
{
	std::map<std::string, Alarm* >::iterator iter = m_alarms.find(alarmName);
	if (iter != m_alarms.end())
		return iter->second;

	return nullptr;
}
double   MasterClock::GetAbsoluteTimeSeconds()
{
	LARGE_INTEGER currentTicks;
	QueryPerformanceCounter(&currentTicks);

	double currentSeconds = currentTicks.QuadPart * g_secondsPerTick;

	return currentSeconds;
}
uint64_t MasterClock::GetAbsoluteTimeMicroSeconds()
{
	LARGE_INTEGER currentTicks;
	QueryPerformanceCounter(&currentTicks);

	uint64_t microSeconds = currentTicks.QuadPart * 1000 * 1000;

	return microSeconds;

}
uint64_t MasterClock::GetAbsoluteTimeMiliSeconds()
{
	LARGE_INTEGER currentTicks;
	QueryPerformanceCounter(&currentTicks);

	uint64_t microSeconds = currentTicks.QuadPart * 1000;

	return microSeconds;
}
void	 MasterClock::InitializeTimer()
{
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency(&ticksPerSecond);

	g_secondsPerTick = 1.0 / ticksPerSecond.QuadPart;


}

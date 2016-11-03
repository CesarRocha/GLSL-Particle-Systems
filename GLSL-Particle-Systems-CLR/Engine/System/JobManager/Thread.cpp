//================================================================
//                     Thread.cpp
//================================================================
#include "Thread.hpp"

JobThread::JobThread( JobManager* manager )
	:m_jobManager(manager)
{
}
void JobThread::Run()
{
	while (m_jobManager->GetIsRunning()) 
	{
		ProcessAll();
		yield();
	}

	ProcessAll();
}
void JobThread::ProcessAll()
{
	while (m_jobManager->RunJob()) 
	{

	}
}

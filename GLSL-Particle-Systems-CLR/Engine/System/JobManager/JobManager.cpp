//================================================================
//                     JobManager.cpp
//================================================================
#include "JobManager.hpp"
JobManager* g_jobManager = new JobManager();

void JobManager::InitializeJobManager( int numthreads )
{
	m_isRunning = true;
	AddThreads(numthreads);
}
void JobManager::AddThreads( int numThreads )
{
	for(int index = 0; index < numThreads; index++)
	{
		JobThread* jobthread = new JobThread(this);
		jobthread->Start("JobThread");

		m_threadsList.push_back(jobthread);
	}
}
void JobManager::AddJob( Job* job, JobComplete_cb cb, void* arg, ePriority priority )
{
	job->m_completeCallback = cb;
	job->m_jobArguments = arg;

	m_queuedJobs[priority].enqueue(job);
}
bool JobManager::RunJob()
{
	Job* job;
	for(int p = ePriority_HIGH; p >= ePriority_LOW; --p)
	{
		if (m_queuedJobs[p].dequeue(&job)) 
		{
			job->Run();
			m_finishedJobs.enqueue(job);
			return true;
		}
	}
	return false;
}
void JobManager::Update()
{
	if (m_threadsList.size() == 0) {
		RunJob();
	} 

	Job* finishedJob;
	while (m_finishedJobs.dequeue(&finishedJob)) 
	{
		if (finishedJob->m_completeCallback != nullptr) 
			finishedJob->m_completeCallback( finishedJob->m_jobArguments );

		delete finishedJob;
	}
}
void JobManager::Shutdown()
{
	m_isRunning = false;
	while (m_threadsList.size() > 0) 
	{
		Thread* thread = m_threadsList.front();
		thread->Join();
		delete thread;

		m_threadsList.pop_front();
	}

	// finish out finished jobs
	Update();
}

//===================================================================================================
unsigned int SystemGetCoreCount()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	return (unsigned int) sysinfo.dwNumberOfProcessors;
}
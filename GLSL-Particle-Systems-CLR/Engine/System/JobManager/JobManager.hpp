//================================================================
//                     JobManager.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_JobManager__
#define _INCLUDE_JobManager__
#include "Engine/System/JobManager/Job.hpp"
#include "Engine/System/JobManager/Thread.hpp"
#include "Engine/System/Threading/Threading.hpp"
#include <list>
typedef void (*JobComplete_cb)(void* arg);

enum ePriority
{
	ePriority_LOW = 0,
	ePriority_MID,
	ePriority_HIGH,
	ePriority_Count
};

class JobManager
{
public:
	void		InitializeJobManager( int numThreads );
	void		AddJob( Job* job, JobComplete_cb onComplete_cb, void* jobArguments, ePriority priority = ePriority_MID);

	void		Shutdown();
	void		Update();
	bool		RunJob();

	inline bool GetIsRunning() const { return m_isRunning; }

private:
	friend class JobThread;
	void		AddThreads( int numThreads );

	bool					m_isRunning;
	std::list<Thread*>		m_threadsList;
	ThreadSafeQueue<Job*>	m_queuedJobs[ePriority_Count];
	ThreadSafeQueue<Job*>	m_finishedJobs;
};
extern JobManager* g_jobManager;


unsigned int SystemGetCoreCount();
#endif
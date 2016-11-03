//================================================================
//                     Thread.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_Thread__
#define _INCLUDE_Thread__
#include "Engine/System/JobManager/JobManager.hpp"
#include "Engine/System/Threading/Threading.hpp"
#include <list>


class JobThread : public Thread
{
public:
	JobThread( JobManager* manager );
	virtual void Run();

private:
	void		ProcessAll();
	JobManager* m_jobManager;
};


#endif
//================================================================
//                     Threading.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_Threading__
#define _INCLUDE_Threading__
#include "Engine/System/Threading/ThreadSafeQueue.hpp"
#include <string>

typedef void (*thread_entry_cb)( void *arg );


class Thread
{
public:
	Thread() { m_threadHandle = NULL; }

	bool Start(const std::string& name);
	void Detach();
	void Join();

	virtual void Run() = 0;

	void yield();
	void Sleep(unsigned int ms);

private:
	friend void ThreadClassEntry( void* arg );
	HANDLE m_threadHandle;
};


//###################################################################################################
//##                                                                                               ##
//###################################################################################################
HANDLE	ThreadCreate( const std::string& name, thread_entry_cb entry, void* arg );
void	ThreadDetach( HANDLE handle );
void	ThreadJoin( HANDLE handle );
void	ThreadYield();
void	ThreadSleep( unsigned int ms );
DWORD	ThreadGetCurrentID();
DWORD	ThreadGetID( HANDLE handle );


#endif
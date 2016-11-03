//================================================================
//                     Threading.cpp
//================================================================
#include "Threading.hpp"
#define MS_VC_EXCEPTION		(0x406d1388)


#pragma pack(push, 8)
struct THREADNAME_INFO
{
	DWORD type;				// must be 0x1000
	const char *name;		// name
	DWORD thread_id;		// -1 for calling thread
	DWORD flags;			// must be 0, reserved for future use
};
#pragma pack(pop)

struct StartThreadArguments
{
	thread_entry_cb m_function;
	void* m_arguments;
	bool m_done;
};

//###################################################################################################
//##                                                                                               ##
//###################################################################################################
static DWORD WINAPI ThreadEntryPoint( void *arg )
{
	StartThreadArguments *args = (StartThreadArguments*) arg;

	thread_entry_cb foo = args->m_function;
	void* volatile parg = args->m_arguments;	

	// Tell the calling thread we're created so it can move on
	args->m_done = true;

	// args is no longer valid beyond this point
	foo( parg );
	return 0;
}


void ThreadClassEntry( void* arg ) 
{
	Thread* thread = (Thread*) arg;
	thread->Run();
}


static void SetThreadName( DWORD id, const std::string& name )
{
	if (0 != id) {
		THREADNAME_INFO info;
		info.type = 0x1000;
		info.name = name.c_str();
		info.thread_id = (DWORD) id;
		info.flags = 0;

		__try 
		{
			RaiseException( MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)(&info) );
		} 
		__except (EXCEPTION_CONTINUE_EXECUTION) 
		{
		}
	}
}


/************************************************************************/
/*                                                                      */
/* EXTERNAL FUNCTIONS                                                   */
/*                                                                      */
/************************************************************************/

// Starting a thread blocks until the starting thread finishes an init step
// [naming itself and calling into the proper function].  Makes starting a thread
// pretty costly.  Threads should only be begun during init steps.... everything
// else should use a job
// 
HANDLE ThreadCreate( const std::string& name, thread_entry_cb entry, void *arg )
{
	// Probably not the best idea, but I'm going to cause this function to block - the goal
	// is that threads aren't created on the fly, but instead a job system is used, so this 
	// is fine for my game.
	//
	// Anyway, the arg passed in will contain all the information, and we only exit once it has been consumed
	StartThreadArguments args;
	args.m_function = entry;
	args.m_arguments = arg;
	args.m_done = false;

	DWORD id;
	HANDLE thread_handle = ::CreateThread( NULL, 
		0, 
		ThreadEntryPoint, &args, 
		0, 
		&id );

	if (NULL != thread_handle) 
	{
		SetThreadName( id, name );
		

		while (!args.m_done) 
		{
			ThreadYield();
		}
	}

	// Return the handle
	return thread_handle;
}

//------------------------------------------------------------------------
// Does not stop the thread, just releases my reference to it so it can 
// clean itself up after it's finished
//
void ThreadDetach( HANDLE thread_handle ) 
{
	if (NULL != thread_handle) {
		::CloseHandle( thread_handle );
	}
}


//------------------------------------------------------------------------
// Blocks calling therad until thread reference by handle is finished,
// and releases my hold on this handle.
//
void ThreadJoin( HANDLE handle )
{
	if (NULL != handle) 
	{
		::WaitForSingleObject( handle, INFINITE );
		::CloseHandle( handle );
	}
}

//------------------------------------------------------------------------
// Yields current thread to other threads
//
void ThreadYield()
{
	::SwitchToThread();
}

//------------------------------------------------------------------------
// Sleep current thread for a number of milliseconds
//
void ThreadSleep( unsigned int ms )
{
	::Sleep( (DWORD) ms );
}

//------------------------------------------------------------------------
// Get the current thread id
//
DWORD ThreadGetCurrentID()
{
	return ::GetCurrentThreadId();
}

//------------------------------------------------------------------------
//
DWORD ThreadGetID( HANDLE handle )
{
	return ::GetThreadId( handle );
}

//------------------------------------------------------------------------
void Thread::Sleep( unsigned int ms ) 
{
	ThreadSleep(ms);
}

//------------------------------------------------------------------------
void Thread::yield() 
{
	ThreadYield();
}

//------------------------------------------------------------------------
bool Thread::Start(const std::string& name) 
{
	if (m_threadHandle == NULL) 
	{
		m_threadHandle = ThreadCreate( name, ThreadClassEntry, this );
		return m_threadHandle != NULL;
	}

	return false;
}

//------------------------------------------------------------------------
void Thread::Detach()
{
	if (m_threadHandle != NULL) 
	{
		ThreadDetach( m_threadHandle );
		m_threadHandle = NULL;
	}
}

//------------------------------------------------------------------------
void Thread::Join()
{
	if (m_threadHandle != NULL) 
	{
		ThreadJoin( m_threadHandle );
		m_threadHandle = NULL;
	}
}
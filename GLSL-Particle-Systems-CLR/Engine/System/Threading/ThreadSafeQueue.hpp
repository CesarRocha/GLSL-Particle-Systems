//================================================================
//                     ThreadSafeQueue.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_ThreadSafeQueue__
#define _INCLUDE_ThreadSafeQueue__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <queue>
class CriticalSection
{
private:
	CRITICAL_SECTION cs;

public: 
	CriticalSection()
	{
		::InitializeCriticalSection( &cs );
	}

	void enter()
	{
		::EnterCriticalSection( &cs );
	}

	void exit()
	{
		::LeaveCriticalSection( &cs );
	}
};




//###################################################################################################
//##                                                                                               ##
//###################################################################################################
template <typename _Tx>
class ThreadSafeQueue 
{
	private:
		mutable CriticalSection cs;
		std::queue<_Tx> queue;

	public:
		void enqueue( _Tx &item ) 
		{
			cs.enter();
			queue.push( item );
			cs.exit();
		}

		bool dequeue( _Tx *out )
		{
			bool success = false;
			cs.enter();
			if (queue.size() > 0) {
				*out = queue.front();
				queue.pop();
				success = true;
			}
			cs.exit();
			return success;
		}

		size_t size() const
		{
			size_t size = 0;
			cs.enter();
			size = queue.size();
			cs.exit();

			return size;
		}
};

#endif
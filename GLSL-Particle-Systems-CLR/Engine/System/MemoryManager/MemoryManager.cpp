//================================================================
//                     MemoryManager.cpp
//================================================================
#include "MemoryManager.hpp"
#include "Engine/EngineCommon/EngineCommon.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Utility/Utility.hpp"
  MemoryManager g_memoryManager = MemoryManager();

MemoryManager::MemoryManager()
	: m_callCount(0)
	, m_byteCount(0)
	, m_largestCall(0)
	, m_averageCall(0)
	, m_hasClosed(false)
	, m_mysteryAllocationsAllocated(0)
	, m_mysteryAllocationsDeleted(0)
{
}


void MemoryManager::SubmitCallRequested(size_t size)
{
	g_memoryManager.m_callCount++;
	g_memoryManager.m_byteCount += size;
	CalculateLargest(size);
	CalculateAverage();
}


void MemoryManager::CalculateAverage()
{
	m_averageCall = ( m_byteCount / m_callCount );
}

void MemoryManager::CalculateLargest(size_t size)
{
	if(size > m_largestCall)
		m_largestCall = size;	
}


void MemoryManager::AddNewAllocation(void* ptr, size_t size, const char* file, size_t line)
{
	if(file != nullptr)
	{
		SizeFileLine fsl = SizeFileLine(size, file, line);
		m_trackedAllocations.insert(std::pair<void*, SizeFileLine> (ptr, fsl));
	}
	else
		m_mysteryAllocationsAllocated++;
}

void MemoryManager::RemoveAllocation(void* ptr)
{
	if(g_appIsRunning)
	{
		std::map<void*, SizeFileLine>::iterator iter = m_trackedAllocations.find(ptr);
		if(iter != m_trackedAllocations.end())
			iter = m_trackedAllocations.erase(iter);
		else
			m_mysteryAllocationsDeleted++;
	}
	else  //The app is closing. Output all memory leaks to file.
	{
		if(m_trackedAllocations.size() > 0 && !m_hasClosed)
		{
			LogPrintf("Memory Leaks detected!\n");
			LogPrintf("Mystery Allocations: %i || Mystery Deletes: %i\n\n", m_mysteryAllocationsAllocated, m_mysteryAllocationsDeleted);
			std::map<void*, SizeFileLine>::iterator iter;
			for(iter = m_trackedAllocations.begin(); iter != m_trackedAllocations.end(); iter++)
			{
				SizeFileLine sfl = iter->second;
				LogPrintf("File: %s\nLine: %i\nSize: %i\n\n", sfl.m_file, sfl.m_line, sfl.m_size);
			}
			
			m_hasClosed = true;
			MemoryLeakCheck();
		}
	}
}



void MemoryManager::MemoryLeakCheck()
{
	if(m_trackedAllocations.size() > 0)
	{
		ConsolePrintf("\n================================= Memory Leaks detected! =================================\n");
		std::map<void*, SizeFileLine>::iterator iter;
		for(iter = m_trackedAllocations.begin(); iter != m_trackedAllocations.end(); iter++)
		{
			SizeFileLine sfl = iter->second;

			std::string formatted = MakeComplexString("                               %s(%i): Memory Leak\n", sfl.m_file, sfl.m_line);
			OutputDebugStringA(formatted.c_str());
		}
		ConsolePrintf("================================= Memory Leaks detected! =================================\n\n");
	}

}

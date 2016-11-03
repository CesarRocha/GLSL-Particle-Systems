//================================================================
//                     MemoryManager.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_MemoryManager__
#define _INCLUDE_MemoryManager__
#include <string>
#include <map>


struct SizeFileLine
{
public:
	SizeFileLine(size_t size, const char* file, size_t line)
		:m_size(size)
		,m_file(file)
		,m_line(line)
	{}
	size_t m_size;
	const char* m_file;
	size_t m_line;
};


class MemoryManager
{
public:
	MemoryManager();
	
	int m_callCount;
	size_t m_byteCount;
	size_t m_largestCall;
	size_t m_averageCall;

	void SubmitCallRequested(size_t size);
	void AddNewAllocation(void* ptr, size_t size, const char* file, size_t line);
	void RemoveAllocation(void* ptr);

	void CalculateAverage();
	void CalculateLargest(size_t size);

	void MemoryLeakCheck();
	std::map< void*, SizeFileLine > m_trackedAllocations;
	int								m_mysteryAllocationsAllocated;
	int								m_mysteryAllocationsDeleted;
private:
	bool m_hasClosed;
};

extern MemoryManager g_memoryManager;
#endif
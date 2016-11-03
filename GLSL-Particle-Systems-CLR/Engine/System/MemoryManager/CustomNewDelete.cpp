// ================================================================
//                     CustomNewDelete.cpp
// ================================================================
#if defined(CUSTOM_NEW_DELETE)
#include "CustomNewDelete.hpp"
#include <malloc.h>
#undef new

  void* operator new  (size_t size)
{
	void* ptr = malloc(size);
	g_memoryManager.AddNewAllocation(ptr, size, nullptr, 0);
	g_memoryManager.SubmitCallRequested(size);
	return ptr;
}


//===================================================================================================
void* operator new  (size_t size, const char* file, size_t line)
{
	void* ptr = malloc(size);
	g_memoryManager.AddNewAllocation(ptr, size, file, line);
	g_memoryManager.SubmitCallRequested(size);
	return ptr;
}


//===================================================================================================
void* operator new[]  (size_t size)
{
	void* ptr = malloc(size);
	g_memoryManager.AddNewAllocation(ptr, size, nullptr, 0);
	g_memoryManager.SubmitCallRequested(size);
	return ptr;
}


//===================================================================================================
void* operator new[]  (size_t size, const char* file, size_t line)
{
	void* ptr = malloc(size);
	g_memoryManager.AddNewAllocation(ptr, size, file, line);
	g_memoryManager.SubmitCallRequested(size);
	return ptr;
}


// ###################################################################################################
// ##                                                                                               ##
// ###################################################################################################
void  operator delete  (void* ptr)
{
	g_memoryManager.RemoveAllocation(ptr);
	free(ptr);
}


//===================================================================================================
void  operator delete  (void* ptr, const char* file, size_t line)
{
	UNUSED(file);
	UNUSED(line);
	g_memoryManager.RemoveAllocation(ptr);
	free(ptr);
}


//===================================================================================================
void  operator delete[]  (void* ptr)
{
	g_memoryManager.RemoveAllocation(ptr);
	free(ptr);
}


//===================================================================================================
void  operator delete[]  (void* ptr, const char* file, size_t line)
{
	UNUSED(file);
	UNUSED(line);
	g_memoryManager.RemoveAllocation(ptr);
	free(ptr);
}
#endif
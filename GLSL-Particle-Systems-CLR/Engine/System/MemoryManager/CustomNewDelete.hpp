// ================================================================
// 	CustomNewDelete.hpp
// ================================================================
#pragma once
#if defined(CUSTOM_NEW_DELETE)
#ifndef _INCLUDE_CustomNewDelete__
#define _INCLUDE_CustomNewDelete__
#include "Engine/System/MemoryManager/SignPostAllocator.hpp"
#include "Engine/System/MemoryManager/MemoryManager.hpp"
#include <string>


void* operator new(size_t size);
void* operator new(size_t size, const char* file, size_t line);
void* operator new[](size_t size);
void* operator new[](size_t size, const char* file, size_t line);

void  operator delete(void* ptr);
void  operator delete(void* ptr, const char* file, size_t line);
void  operator delete[](void* ptr);
void  operator delete[](void* ptr, const char* file, size_t line);

#define new new(__FILE__, __LINE__)

#endif
#endif
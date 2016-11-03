//================================================================
//                     SignPostAllocator.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_SignPostAllocator__
#define _INCLUDE_SignPostAllocator__
#include "Engine/Utility/Utility.hpp"
#include <string>

struct SignPost
{
public: 
	SignPost();
	SignPost*	m_next;
	SignPost*	m_previous;
	size_t		m_size;
	bool		m_isFree;
};

class SignPostAllocator
{
public:
	SignPostAllocator();

	void InitializeSignPostAllocator( size_t initalSize);
	void DeinitializeSignPostAllocator();
	void* CustomAlloc(size_t size);
	void CustomFree(void* ptr);


	SignPost*	m_posts;
	void*		m_buffer;
private:
};
extern SignPostAllocator* g_signPostAllocator;

#endif
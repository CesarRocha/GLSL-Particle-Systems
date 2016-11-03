//================================================================
//                     SignPostAllocator.cpp
//================================================================
#include "SignPostAllocator.hpp"
SignPostAllocator* g_signPostAllocator = new SignPostAllocator;

SignPost::SignPost()
	: m_next(new SignPost())
	, m_previous(new SignPost())
	, m_size(0)
	, m_isFree(true)
{
}

//###################################################################################################
//##                                                                                               ##
//###################################################################################################
SignPostAllocator::SignPostAllocator()
{

}


void SignPostAllocator::InitializeSignPostAllocator(size_t initalSize)
{
	m_buffer = malloc(initalSize);

	SignPost* post = (SignPost*)m_buffer;
	post->m_next = nullptr;
	post->m_previous = nullptr;
	post->m_isFree = true;
	post->m_size = initalSize - sizeof(initalSize);

	m_posts = post;
}

void SignPostAllocator::DeinitializeSignPostAllocator()
{
	//std::string errorMessage = "Not all posts are null";
	//RECOVERABLE_ASSERTION( (m_posts->m_next == NULL) && (m_posts->m_previous == NULL) && (m_posts->NULL), errorMessage );
	free(m_buffer);
}

void* SignPostAllocator::CustomAlloc(size_t size)
{
	// walk list and find first free, split, and return
	size_t alloc_size = size + sizeof(SignPost);
	SignPost* iter = m_posts;

	while (iter != nullptr) 
	{
		if (iter->m_isFree && iter->m_size >= alloc_size) 
			break;

		iter = iter->m_next;
	}

	if (iter == nullptr) 
		return nullptr;


	//[iter,buffer]
	// -->
	// [iter,needed_size]->[next_signpost,remaining_size]
	iter->m_isFree = false;

	char* iter_buffer = (char*)iter + sizeof(SignPost);
	SignPost* nextSignpost = (SignPost*)(iter_buffer + size);
	nextSignpost->m_isFree = true;
	nextSignpost->m_previous = iter;
	nextSignpost->m_next = iter->m_next;
	nextSignpost->m_size = iter->m_size - alloc_size;

	iter->m_size = size;
	iter->m_next = nextSignpost;

	return iter_buffer;
}

void SignPostAllocator::CustomFree(void* ptr)
{
	char* iter_buffer = (char*)ptr;
	SignPost* post = (SignPost*)(iter_buffer - sizeof(SignPost));

	std::string errorMessage = "Post is not free";
	FATAL_ASSERTION(post->m_isFree == false, errorMessage );

	// check previous or next, see if they're free, or both are free, and combine
	if ((post->m_previous != nullptr) && (post->m_previous->m_isFree) && (post->m_next != nullptr) && (post->m_next->m_isFree))
	{
		size_t size = post->m_previous->m_size + post->m_size + post->m_next->m_size + 2 * sizeof(SignPost); // 2, this post and next post, previous is staying

		SignPost* prev = post->m_previous;
		prev->m_size = size;
		prev->m_next = post->m_next->m_next;
	}
	else if ((post->m_previous != nullptr) && (post->m_previous->m_isFree)) 
	{
		size_t size = post->m_previous->m_size + post->m_size + sizeof(SignPost);

		SignPost* prev = post->m_previous;
		prev->m_size = size;
		prev->m_next = post->m_next;
	}
	else if ((post->m_next != nullptr) && (post->m_next->m_isFree)) 
	{
		post->m_size = post->m_size + post->m_next->m_size + sizeof(SignPost);
		post->m_next = post->m_next->m_next;
		post->m_isFree = true;
	}
	else 
	{
		post->m_isFree = true;
	}
}

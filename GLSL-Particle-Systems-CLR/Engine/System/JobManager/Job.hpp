//================================================================
//                     Job.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_Job__
#define _INCLUDE_Job__
#include "Engine/System/Threading/Threading.hpp"
#include <list>
typedef void (*JobComplete_cb)(void* arg);
typedef void (*function_cb)(void* arg);

class Job
{
public:
		virtual void Run() = 0;

private:
	friend class JobManager;
	JobComplete_cb	m_completeCallback;
	void*			m_jobArguments;
};


//###################################################################################################
//##                                                                                               ##
//###################################################################################################
class LoadFileJob : public Job
{
public: 
	LoadFileJob(const std::string& filePath, size_t bufferSize, unsigned char*& buffer);
	virtual void Run();

private:
	std::string		m_filePath;
	size_t			m_bufferSize;
	unsigned char*	m_buffer;
};


//===================================================================================================
class HashBufferJob : public Job
{
public: 
	HashBufferJob(unsigned char* buffer);

	virtual void Run();

private:
	unsigned char* m_buffer;
	unsigned long m_hash;
};


//===================================================================================================
class ReverseBufferJob : public Job
{
public: 
	ReverseBufferJob(const size_t bufferSize, unsigned char* buffer);

	virtual void Run();

private:
	size_t			m_bufferSize;
	unsigned char*	m_buffer;
};


//===================================================================================================
class SaveFileJob : public Job
{
public: 
	SaveFileJob(const std::string& filePath, size_t bufferSize, unsigned char* buffer);

	virtual void Run();
private:
	std::string		m_filePath;
	size_t			m_bufferSize;
	unsigned char*	m_buffer;
};


//===================================================================================================
class TestJob : public Job
{
public:
	TestJob( unsigned int ms )
		:m_waitMS(ms)
	{}

	virtual void Run() 	{
		ThreadSleep(m_waitMS);	
	}

private:
	unsigned int m_waitMS;
};
#endif
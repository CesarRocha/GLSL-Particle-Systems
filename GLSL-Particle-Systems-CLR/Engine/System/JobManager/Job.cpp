//================================================================
//                     Job.cpp
//================================================================
#include "Job.hpp"
#include "Engine/Utility/Parser/BinaryFileParser.hpp"

LoadFileJob::LoadFileJob(const std::string& filePath, size_t bufferSize, unsigned char*& buffer)
	: m_filePath(filePath)
	, m_bufferSize(bufferSize)
	, m_buffer(buffer)
{
}
void LoadFileJob::Run()
{
	bool result = BinaryFileParser::LoadBinaryFileToNewBuffer(m_filePath, m_bufferSize, m_buffer);

	if(result)
		MessageBoxA( NULL, "File Load Success", "Loading File Job", MB_OK );
}

//###################################################################################################
//##                                                                                               ##
//###################################################################################################
HashBufferJob::HashBufferJob(unsigned char* buffer)
	:m_buffer(buffer)
	,m_hash(0)
{
}
void HashBufferJob::Run()
{
// 	int c;
// 	while(c = *m_buffer++)
// 		m_hash = ((m_hash << 5) + m_hash) + c;

	MessageBoxA( NULL, "Hash Job Complete", "Hash Job", MB_OK );
}


//###################################################################################################
//##                                                                                               ##
//###################################################################################################
ReverseBufferJob::ReverseBufferJob(const size_t bufferSize, unsigned char* buffer)
	: m_bufferSize(bufferSize)
	, m_buffer(buffer)
{
}
void ReverseBufferJob::Run()
{	
	int size = (int)m_bufferSize;
	for(int index = 0; index < size; index++)
	{
		unsigned char c = m_buffer[index];
		m_buffer[index] = m_buffer[m_bufferSize - 1 - index];
		m_buffer[m_bufferSize - 1 - index] = c;
	}
	MessageBoxA( NULL, "Reverse Buffer Job Complete", "Reverse Buffer Job", MB_OK );
}


//###################################################################################################
//##                                                                                               ##
//###################################################################################################
SaveFileJob::SaveFileJob(const std::string& filePath, size_t bufferSize, unsigned char* buffer)
	: m_filePath(filePath)
	, m_bufferSize(bufferSize)
	, m_buffer(buffer)
{
}
void SaveFileJob::Run()
{
	bool result = BinaryFileParser::WriteBinaryBufferToFile(m_filePath, m_bufferSize, m_buffer);

	if(result)
		MessageBoxA( NULL, "File Save Success", "Save File Job", MB_OK );
	else
		MessageBoxA( NULL, "File Save Fail", "Save File Job", MB_OK );
}
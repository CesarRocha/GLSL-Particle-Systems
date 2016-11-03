#include "BinaryFileParser.hpp"
#include "Engine/Utility/Utility.hpp"
#include <windows.h>
#define WINDOWS_LEAN_AND_MEAN

//===================================================================================================
//																								   ==
//===================================================================================================
BinaryFileParser::BinaryFileParser()
{
}
BinaryFileParser::BinaryFileParser(const std::string& filePath)
{
	//m_bufferB = LoadBinaryFileToNewBuffer(filePath, m_bufferSize);
	LoadBinaryFileToNewBufferVector(filePath, m_buffer);
	m_bufferSize = m_buffer.size();
	//m_bufferS = LoadTextFileToNewStringBuffer(filePath, m_bufferSize);
}
BinaryFileParser::~BinaryFileParser()
{
	// 	if(m_bufferB != NULL)
	// 		delete m_bufferB;
	// 	if(m_bufferS != NULL)
	// 		delete m_bufferS;
}




//===================================================================================================
unsigned char*	BinaryFileParser::LoadBinaryFileToNewBuffer(const std::string& filePath, size_t& out_bufferSize)
{
	//Open File
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "rb");
	if (!file)
	{
		std::string error = "Bad File Path: " + filePath;
		MessageBoxA(NULL, error.c_str(), "Loading File Error", MB_OK);
	}

	//Get Size of File
	size_t numBytes = GetFileLength(file);

	//Create Buffer
	unsigned char* buffer = new unsigned char[numBytes];


	//Read File
	size_t numBytesRead = fread(buffer, 1, numBytes, file);
	if (numBytesRead != numBytes)
		MessageBoxA(NULL, "Sizes do not match", "Loading File Error", MB_OK);

	//Close File
	fclose(file);

	//Return Buffer
	out_bufferSize = numBytesRead;
	return buffer;
}


//===================================================================================================
bool 			BinaryFileParser::LoadBinaryFileToNewBuffer(const std::string& filePath, size_t& out_bufferSize, unsigned char*& out_buffer)
{
	//Open File
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "rb");
	if (!file)
	{
		std::string error = "Bad File Path: " + filePath;
		MessageBoxA(NULL, error.c_str(), "Loading File Error", MB_OK);
		return false;
	}

	//Get Size of File
	size_t numBytes = GetFileLength(file);

	//Create Buffer
	unsigned char* buffer = new unsigned char[numBytes];


	//Read File
	size_t numBytesRead = fread(buffer, 1, numBytes, file);
	if (numBytesRead != numBytes)
		MessageBoxA(NULL, "Sizes do not match", "Loading File Error", MB_OK);

	//Close File
	fclose(file);

	//Return Buffer
	out_bufferSize = numBytesRead;
	out_buffer = buffer;
	return true;
}


//===================================================================================================
bool			BinaryFileParser::WriteBinaryBufferToFile(std::string& filePath, size_t bufferSize, unsigned char* bufferData)
{
	FILE* file = nullptr;
	errno_t error = fopen_s(&file, filePath.c_str(), "wb");
	if (error)
		return false;

	fwrite(bufferData, 1, bufferSize, file);
	fclose(file);
	return true;
}


//===================================================================================================
void			BinaryFileParser::LoadBinaryFileToNewBufferVector(const std::string& filePath, std::vector<unsigned char>& buffer)
{
	//Open File
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "rb");
	if (!file)
	{
		std::string error = "Bad File Path: " + filePath;
		MessageBoxA(NULL, error.c_str(), "Loading File Error", MB_OK);
	}

	//Get Size of File
	size_t numBytes = GetFileLength(file);

	//Resize Vector
	buffer.resize(numBytes);


	//Read File
	fread((char*)&buffer[0], 1, numBytes, file);

	//Close File
	fclose(file);
}


//===================================================================================================
char*			BinaryFileParser::LoadTextFileToNewStringBuffer(const std::string& filePath, size_t& out_bufferSize)
{
	//Open File
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "rb");
	if (!file)
	{
		std::string error = "Bad File Path: " + filePath;
		MessageBoxA(NULL, error.c_str(), "Loading File Error", MB_OK);
	}

	//Get Size of File
	size_t numBytes = GetFileLength(file);

	//Create Buffer
	char* buffer = new char[numBytes + 1];

	//Read File
	size_t numBytesRead = fread(buffer, 1, numBytes, file);

	if (numBytesRead != numBytes)
		MessageBoxA(NULL, "Sizes do not match", "Loading File Error", MB_OK);

	//Close File
	fclose(file);

	//Return Buffer
	buffer[numBytes] = '\0';
	out_bufferSize = numBytesRead;
	return buffer;
}

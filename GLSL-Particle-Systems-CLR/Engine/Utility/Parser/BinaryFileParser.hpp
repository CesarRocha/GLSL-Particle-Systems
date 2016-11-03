#pragma once
#ifndef _INCLUDE_BINARY_FILE_PARSER
#define _INCLUDE_BINARY_FILE_PARSER
#include "BinaryBufferParser.hpp"
#include <string>


//===================================================================================================
//																								   ==
//===================================================================================================
class BinaryFileParser : public BinaryBufferParser
{
public:
	BinaryFileParser();
	BinaryFileParser(const std::string& filePath);
	~BinaryFileParser();

	

	static unsigned char*  LoadBinaryFileToNewBuffer(const std::string & filePath, size_t & out_bufferSize);
	static bool			LoadBinaryFileToNewBuffer(const std::string& filePath, size_t& out_bufferSize, unsigned char*& out_buffer);

	static bool			WriteBinaryBufferToFile(std::string & filePath, size_t bufferSize, unsigned char * bufferData);

	void			LoadBinaryFileToNewBufferVector(const std::string & filePath, std::vector<unsigned char>& buffer);
	char*			LoadTextFileToNewStringBuffer(const std::string & filePath, size_t & out_bufferSize);






private:
};

#endif // !_INCLUDE_BINARY_FILE_PARSER

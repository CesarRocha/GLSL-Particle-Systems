#pragma once
#ifndef _INCLUDE_UTILITY_
#define _INCLUDE_UTILITY_
#include "Engine/EngineCommon/EngineCommon.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Utility/Parser/xmlParser.h"
#include "AssertionMacros.hpp"
#include <string>
#include <vector>
#include <io.h>

//===================================================================================================
//					Utilities																	   ==
//===================================================================================================
bool	DoesFileExist(const std::string* fileName);


//===================================================================================================
size_t	GetFileLength(FILE* fileHandle);


//===================================================================================================
bool	FindAllFilesOfType(const std::string& directory, const std::string& searchPattern, std::vector<std::string>& out_filesFound);


//===================================================================================================
bool	FindAllFilesOfTypeInFolderAndSubFolder(const std::string& directory, const std::string& searchPattern, const std::string& filter, std::vector<std::string>& out_filesFound);


//===================================================================================================
bool	IsWhitespace(char c);


//===================================================================================================
char*	Trim(char *s);


//===================================================================================================
std::wstring ToWideString(const std::string& str);




#endif
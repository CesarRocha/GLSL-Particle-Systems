#include "Engine/Utility/Utility.hpp"



//===================================================================================================
//																								   ==
//===================================================================================================
bool WriteBinaryBufferToFile(std::string& filePath, size_t bufferSize, unsigned char* bufferData)
{
	FILE* file = nullptr;
	errno_t error = fopen_s( &file, filePath.c_str(), "wb" );
	if( error )
		return false;

	fwrite( bufferData, 1, bufferSize, file );
	fclose( file );
	return true;
}



//===================================================================================================
bool DoesFileExist(const std::string* fileName)
{
	std::ifstream infile(fileName->c_str());
	return infile.good();
}



//===================================================================================================
size_t GetFileLength(FILE * fileHandle)
{
	fseek(fileHandle, 0, SEEK_END);
	size_t size = ftell(fileHandle);
	fseek(fileHandle, 0, SEEK_SET);
	return size;
}



//===================================================================================================
bool FindAllFilesOfType(const std::string& directory, const std::string& searchPattern, std::vector<std::string>& out_filesFound)
{
	//Implementation windows specific
	struct _finddata_t fd;
	std::string searchPath = directory + searchPattern;
	intptr_t searchHandle = _findfirst(searchPath.c_str(), &fd);

	if (searchHandle < 0)
		return false;

	do
	{
		out_filesFound.push_back(directory + fd.name);
	} while (_findnext(searchHandle, &fd) == 0);
	_findclose(searchHandle);


	if (!out_filesFound.empty())
		return true;
	else
		return false;
}



//===================================================================================================
bool FindAllFilesOfTypeInFolderAndSubFolder(const std::string& directory, const std::string& searchPattern, const std::string& filter, std::vector<std::string>& out_filesFound)
{
	//Implementation windows specific
	struct _finddata_t fd;
	std::string searchPath = directory + searchPattern;
	intptr_t searchHandle = _findfirst(searchPath.c_str(), &fd);

	if (searchHandle < 0)
		return false;

	do
	{
		if (fd.name != std::string(".") && fd.name != std::string(".."))
		{
			//Go Deeper
			if ((fd.attrib & _A_SUBDIR) && (fd.name != std::string(".") && fd.name != std::string("..")))
			{
				std::string subDirectory = directory + fd.name + "/";
				FindAllFilesOfTypeInFolderAndSubFolder(subDirectory, searchPattern, filter, out_filesFound);
			}
			else
				out_filesFound.push_back(directory + fd.name);
			//Get IN Current Folder
			//FindAllFilesOfType(directory, filter, out_filesFound);
		}


	} while (_findnext(searchHandle, &fd) == 0);

	_findclose(searchHandle);


	if (!out_filesFound.empty())
		return true;
	else
		return false;
}



//===================================================================================================
bool IsWhitespace(char c)
{
	switch (c) 
	{
	case '\n':
	case '\r':
	case ' ':
	case '\t':
		return true;

	default:
		return false;
	};
}



//===================================================================================================
char* Trim(char* s)
{
	while (IsWhitespace(*s))
		++s;

	char* end = s;
	while (*end != NULL)
		++end;


	if (end > s)
	{
		--end;
		while ((end > s) && IsWhitespace(*end))
		{
			--end;
		}

		++end;
		*end = NULL;
	}

	return s;
}



//===================================================================================================
std::wstring ToWideString(const std::string& str)
{
	int stringLength = MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), 0, 0);
	std::wstring wstr(stringLength, 0);
	MultiByteToWideChar(CP_ACP, 0,  str.data(), str.length(), &wstr[0], stringLength);
	return wstr;
}


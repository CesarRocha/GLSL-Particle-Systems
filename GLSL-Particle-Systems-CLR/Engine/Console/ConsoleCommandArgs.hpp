#pragma once
#ifndef _ConsoleCommandArgs_
#define _ConsoleCommandArgs_
#include <string>


//===================================================================================================
//			struct ConsleCommandsArgs															   ==
//===================================================================================================
class ConsoleCommandArgs
{
public:
	ConsoleCommandArgs(const std::string& argument_string);
	void  PrepareString();

	int   GetArgumentCount();
	int   GetArgumentCountSpaces();
	int   GetArgumentQuotes();
	std::string GetNextStringQuoted();
	std::string GetArgument();

	std::string GetNextString();
	float GetNextFloat();
	float GetNextFloatSpaced();
	char  GetNextChar();
	int   GetNextInt();
	int   GetNextIntSpaces();

	// 	short GetNextShort();
// 	bool  GetNextBoolean();
// 	bool  NextIsString();
// 	bool  NextIsInt();
// 	bool  NextIsFloat();
// 	bool  NextIsBoolean();


	std::string m_arg_string;
private:
};


typedef void(*command_callback) (ConsoleCommandArgs &args);
#endif // !_ConsoleCommandArgs_

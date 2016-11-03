#include "ConsoleCommandArgs.hpp"
#include <algorithm>



//===================================================================================================
//               class ConsoleCommandArgs                                                          ==
//===================================================================================================
ConsoleCommandArgs::ConsoleCommandArgs(const std::string & argument_string)
	: m_arg_string(argument_string)
{
	PrepareString();
}
void        ConsoleCommandArgs::PrepareString()
{
	m_arg_string.erase(0, m_arg_string.find("(") + 1);
}


//===================================================================================================
int         ConsoleCommandArgs::GetArgumentCount()
{
	std::string chop = m_arg_string;
	chop.erase(0, chop.find("(") + 1);

	int args = 0;
	for (unsigned int i = 0; i < chop.length(); i++)
	{
		if (chop[i] == ',')
		{
			++args;
		}
		if (chop[i] == ')')
		{
			++args;
			break;
		}
	}
	return args;
	;
}
int         ConsoleCommandArgs::GetArgumentCountSpaces()
{
	std::string chop = m_arg_string;
	if (chop.length() == 0)
		return 0;

	int argumentsFound = 0;
	for (unsigned int i = 0; i < chop.length(); i++)
	{
		if (chop[i] == ' ' && chop[i+1] != NULL )
		{
			++argumentsFound;
		}
	}

	if (chop.length() != 0 && argumentsFound == 0) //There is data, but no spaces. Always 1
		return 1;
	else
		return argumentsFound + 1;
}
int         ConsoleCommandArgs::GetArgumentQuotes()
{
	std::string chop = m_arg_string;
	chop.erase(0, chop.find(" ") + 1);

	int args = 0;
	for (unsigned int i = 0; i < chop.length(); i++)
	{
		if (chop[i] == '"')
		{
			++args;
		}
	}
	return (int)floor(args * .5);
	;
}
std::string ConsoleCommandArgs::GetNextStringQuoted()
{
	std::string token;
	token = m_arg_string.substr(0, m_arg_string.find_first_of("'"));
	m_arg_string.erase(0, m_arg_string.find("'") + 1);
	return  (std::string)token;
}
std::string ConsoleCommandArgs::GetArgument()
{
	std::string out = m_arg_string;
	//out.erase(std::remove(out.begin(), out.end(), " "), out.end());
	out.erase(std::remove(out.begin(), out.end(), ')'), out.end());
	//ERROR?
	return out;
}


//===================================================================================================
std::string ConsoleCommandArgs::GetNextString()
{
	std::string token;
	token = m_arg_string.substr(0, m_arg_string.find_first_of(" "));
	m_arg_string.erase(0, m_arg_string.find(" ") + 1);
	return  (std::string)token;
}
float       ConsoleCommandArgs::GetNextFloat()
{
	float token;
	token = stof(m_arg_string.substr(0, m_arg_string.find_first_of(",")));
	m_arg_string.erase(0, m_arg_string.find(",") + 1);
	return  (float)token;
}
float       ConsoleCommandArgs::GetNextFloatSpaced()
{
	float token;
	token = stof(m_arg_string.substr(0, m_arg_string.find_first_of(" ")));
	m_arg_string.erase(0, m_arg_string.find(" ") + 1);
	return  (float)token;
}
int         ConsoleCommandArgs::GetNextInt()
{
	int token;
	token = stoi(m_arg_string.substr(0, m_arg_string.find_first_of(",")));
	m_arg_string.erase(0, m_arg_string.find(",") + 1);
	return  (int)token;
}
int         ConsoleCommandArgs::GetNextIntSpaces()
{
	int token;
	token = stoi(m_arg_string.substr(0, m_arg_string.find_first_of(" ")));
	m_arg_string.erase(0, m_arg_string.find(" ") + 1);
	return  (int)token;
}
char        ConsoleCommandArgs::GetNextChar()
{
	int token;
	token = stoi(m_arg_string.substr(0, m_arg_string.find_first_of(" ")));
	m_arg_string.erase(0, m_arg_string.find(" ") + 1);
	return  (char)token;
}
//================================================================
//                     Commandlets.cpp
//================================================================
#include "Commanlets.hpp"
#include "Engine/Utility/Utility.hpp"
#include <map>
#include <fstream>

static char const COMMAND_INDICATOR = '-';
static char const QUOTE = '\"';
static char const ESCAPE = '\\';
static std::map<std::string, command_cb> gCommandLookup;

Commandlets::Commandlets( char* name ) 
{
	// move past the COMMAND_INDICATOR
	++name;
	m_name = Trim(name);
}

bool Commandlets::GetString(int index, std::string& out) const
{
	if ((index < (int)m_arguments.size()) && (index >= 0)) {
		auto iter = m_arguments.begin();
		while (index > 0) {
			--index;
			++iter;
		}

		out = *iter;
		return true;
	} else {
		return false;
	}
}
bool Commandlets::GetInt(int index, int* out) const
{
	std::string s;
	if (GetString(index, s)) {
		*out = atoi(s.c_str());
		return true;
	}

	return false;
}


//###################################################################################################
//##                                                                                               ##
//###################################################################################################
static char		GetNextCharacter( char **stream ) 
{
	char *s = *stream;
	char c = *s;

	if (c == ESCAPE) {
		++s;
		switch (*(s)) {
		case 'n':
			c = '\n';
			break;
		case 'r':
			c = '\r';
			break;
		case '\\':
			c = '\\';
			break;
		case 't':
			c = '\t';
			break;
		case '\'':
			c = '\'';
			break;
		case '\"':
			c = '\"';
			break;
		case NULL:
			c = ESCAPE;
			--s; // go back one
			break;
		default:
			c = *s;
		}
	}

	*stream = s;
	return c;
}
static void		GetNextString( char **out_stream, char **stream ) 
{
	char *s = *stream;
	char *out = *out_stream;

	// nothing to do, doesn't start with a quote;
	if (*s != QUOTE) {
		return;
	}

	++s;
	while ((*s != NULL) && (*s != QUOTE)) {
		char c = GetNextCharacter(&s);
		*out = c;
		++out;
		++s;
	}

	if ((*s) == QUOTE) {
		++s;
	}

	*out_stream = out;
	*stream = s;
}
static char*	GetNextToken( char **stream )
{
	char *s = *stream;
	char *token = nullptr;
	// get up the first real character
	while (IsWhitespace(*s)) {
		++s;
	}

	if (*s != NULL) {
		token = s;
		char *out = s;

		if (*s == QUOTE) {
			GetNextString( &out, &s );
		} else {
			while (!IsWhitespace(*s) && (*s != NULL)) {
				char c = GetNextCharacter(&s);
				*out = c;
				++out;
				++s;
			}
		}

		// move past the possible "last position" we left on
		// since we're NULLing it out for our token
		if (*s != NULL) {
			++s;
		}
		*out = NULL;
	}

	*stream = s;
	return token;
}
static bool		TokenIsNullOrEmpty( char const *token ) 
{
	return (token == nullptr) || (*token == NULL);
}
static bool		TokenIsCommand( char const *token ) 
{
	if (token == nullptr) {
		return false;
	}

	return ((*token == COMMAND_INDICATOR) 
		&& (*(token + 1) != NULL)
		&& (!IsWhitespace(*(token + 1))));
}


//###################################################################################################
//##                                                                                               ##
//###################################################################################################
CommandletsList	ParseCommands( std::string command_line )
{
	size_t len = command_line.size();
	char *stream = new char[len + 1];
	memcpy( stream, command_line.c_str(), len );
	stream[len] = NULL;

	Commandlets *command = nullptr;
	std::list<Commandlets*> commands;

	char *s = stream;
	char *token = GetNextToken( &s );
	while (token != nullptr) {
		if (TokenIsCommand(token)) {
			if (command != nullptr) {
				commands.push_back(command);
			}

			command = new Commandlets(token);
		} else if ((command != nullptr) && !TokenIsNullOrEmpty(token)) {
			command->m_arguments.push_back(token);
		}

		token = GetNextToken(&s);
	}

	delete[] stream;

	if (command != nullptr) {
		commands.push_back(command);
	}

	return commands;
}
void			FreeCommands( CommandletsList commands )
{
	for (auto iter = commands.begin(); iter != commands.end(); ++iter) {
		delete *iter;
	}
}
void			RegisterCommand( std::string name, command_cb cb )
{
	gCommandLookup.insert(std::pair<std::string, command_cb> (name, cb) );
}
void			ProcessCommands( CommandletsList commands ) 
{
	for (auto iter = commands.begin(); iter != commands.end(); ++iter) 
	{
		Commandlets *command = *iter;
		if (gCommandLookup.find( command->m_name ) != gCommandLookup.end()) 
			gCommandLookup[command->m_name](command);
	}
}

//###################################################################################################
//##                                                                                               ##
//###################################################################################################
void			OnWriteFiles( const Commandlets* command ) 
{
	int arg0;
	int arg1;
	if (!command->GetInt(0, &arg0) || !command->GetInt(1, &arg1)) {
		return;
	}
	MessageBoxA( NULL, "OnWriteFiles Succeeded.", "", MB_OK );
}
void			GenerateFiles( const Commandlets* command ) 
{
	int arg0;
	int arg1;
	if (!command->GetInt(0, &arg0) || !command->GetInt(1, &arg1)) {
		return;
	}

	for(int index = 0; index < arg0; index++)
	{
		int size = arg1;
		std::stringstream ss;
		ss << "./files/file" << index << ".txt";
		std::string filename = ss.str(); 
		std::ofstream fout;
		fout.open(filename);

		fout.fill (' ');
		fout.width (size);
		fout << " ";

		fout.close();
	}

	MessageBoxA( NULL, "Generate Files Succeeded.", "", MB_OK );
	g_appIsRunning = false;
}
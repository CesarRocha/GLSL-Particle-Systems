//================================================================
//                     Commandlets.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_Commandlets__
#define _INCLUDE_Commandlets__
#include <list>
#include <string>

class Commandlets 
{
public:
	Commandlets( char* name );


	bool GetString( int index, std::string& out ) const;
	bool GetInt( int index, int* out ) const; 


	std::string m_name;
	std::list<std::string> m_arguments;
};


typedef void (*command_cb)( Commandlets const* );
typedef std::list<Commandlets*> CommandletsList;


CommandletsList	ParseCommands( std::string command_line );

void			FreeCommands( CommandletsList commandList );

void			RegisterCommand( std::string name, command_cb cb );

void			ProcessCommands( CommandletsList commands );

void			OnWriteFiles( const Commandlets* command );
void			GenerateFiles( const Commandlets* command );

#endif
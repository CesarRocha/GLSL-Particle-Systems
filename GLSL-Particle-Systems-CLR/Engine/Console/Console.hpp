#ifndef __INCLUDE_CONSOLE__
#define __INCLUDE_CONSOLE__
#define WIN32_LEAN_AND_MEAN
#include "Engine/Renderer/Font\Font.hpp"
#include "Engine\Renderer\Font\BasicShader\BasicShader.hpp"
#include "Engine/Console/ConsoleCommandArgs.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include <string>
#include <algorithm>
#include <functional>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
class MeshRenderer;
class BasicShader;
//===================================================================================================
//					class DeveloperLog															   ==
//===================================================================================================
class DeveloperLog : public Font
{
public:
	DeveloperLog();
	DeveloperLog(const std::string& fontFileName);
	~DeveloperLog();
	static void			 CreateConsoleLogAndFPS();


	//System
	static void			 UpdateDeveloperLogShader(BasicShader* shader, Font* font, Vector2 position, Rgba color);
	static void			 UpdateInfoTextShader(BasicShader* shader, Font* font, Vector2 position);
	static void			 DetectInputForConsole();
	bool				 ToggleConsoleVisibility();



	//Console Shaders and Fonts
	static DeveloperLog* GetDeveloperLog()				{ return g_developerLog; }
	static DeveloperLog* GetDeveloperConsole()			{ return g_developerConsole; }
	static BasicShader*  GetDeveloperConsoleShader()	{ return g_developerConsoleShader; }
	static BasicShader*  GetDeveloperLogShader()		{ return g_developerLogShader; }
	static Font*		 GetFPSFont()					{ return g_fpsFont; }
	static BasicShader*  GetFPSShader()					{ return g_fpsShader; }
	static Font*		 GetInfoTableFont()				{ return g_infoTableFont; }
	static BasicShader*  GetInfoTableShader()			{ return g_infoTableShader; }


	//Console Commands Workings
	void				InitializeAllCommands();
	static void			RegisterCommand( std::string functionName, command_callback callback );
	void				ProcessCommand( std::string inCommand );
	std::string			FindCommandByNameAndReturnName( const std::string commandToFind );
	command_callback	FindCommandByNameAndReturnFunction( const std::string commandToFind );
	void				ExecuteConsoleCommand( std::string command_name, ConsoleCommandArgs &args );


	//Available Commands
	void ClearLog();


	static	std::vector<std::string>	m_myEntireLog;
	static	std::string					m_myLastEntry;
	static	std::string					m_logToSave;


private:
	static std::map< std::string, command_callback  >	m_commandMap;
	static  DeveloperLog*				g_developerLog;
	static  DeveloperLog*				g_developerConsole;
	static  BasicShader*				g_developerConsoleShader;
	static  BasicShader*				g_developerLogShader;
	static  Font*						g_fpsFont;
	static  BasicShader*				g_fpsShader;
	static  Font*						g_infoTableFont;
	static  BasicShader*				g_infoTableShader;
	static  MeshRenderer*				g_consoleLogMeshRenderer;
};






















void			HelpCommand(ConsoleCommandArgs  &args);
void			ClearCommand( ConsoleCommandArgs  &args );
void			QuitCommand( ConsoleCommandArgs  &args );
void			HelpCommand( ConsoleCommandArgs  &args );
void			InvalidCommand( ConsoleCommandArgs  &args );
void			PrintToConsole( ConsoleCommandArgs &args );
void			LogClearf();
void			LogPrintf(const char* messageFormat, ...);
std::string		ConsolePrintf(const char* messageFormat, ...);
std::string		MakeComplexString(const char* messageFormat, ...);
#endif
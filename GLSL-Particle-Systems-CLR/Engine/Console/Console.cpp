#define Win32_LEAN_AND_MEAN
#include "Console.hpp"
#include "Engine/Renderer/Font/BasicShader/BasicShader.hpp"
#include "Engine/EngineCommon/EngineCommon.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
std::map< std::string, command_callback  >	DeveloperLog::m_commandMap;
DeveloperLog*	DeveloperLog::g_developerLog = nullptr;
DeveloperLog*	DeveloperLog::g_developerConsole = nullptr;

BasicShader*	DeveloperLog::g_developerConsoleShader = nullptr;
BasicShader*	DeveloperLog::g_developerLogShader = nullptr;

Font*			DeveloperLog::g_fpsFont = nullptr;
BasicShader*	DeveloperLog::g_fpsShader = nullptr;

Font*			DeveloperLog::g_infoTableFont;
BasicShader*	DeveloperLog::g_infoTableShader;

std::vector<std::string>  DeveloperLog::m_myEntireLog;
std::string		DeveloperLog::m_myLastEntry;
std::string		DeveloperLog::m_logToSave;
MeshRenderer*	DeveloperLog::g_consoleLogMeshRenderer = nullptr;
#include "Engine/Renderer/Shader/Mesh.hpp"
#include "Engine/Renderer/Shader/MeshRenderer.hpp"
//===================================================================================================
//               class DeveloperLog                                                               ==
//===================================================================================================
DeveloperLog::DeveloperLog()
	: Font(g_masterFont)
{
	InitializeAllCommands();
}
DeveloperLog::DeveloperLog(const std::string& fontFileName)
	: Font(fontFileName)	
{
	InitializeAllCommands();
}
DeveloperLog::~DeveloperLog()
{
}



//===================================================================================================
void DeveloperLog::CreateConsoleLogAndFPS()
{
	//Console
	g_developerConsole = new DeveloperLog(g_masterFont);
	g_developerConsoleShader = new BasicShader(g_masterFontShader, g_masterFontImage.c_str());

	int bufferSize = 0;
	vert_t* verts = g_developerConsole->CreateFontVertsPerChar(g_developerConsole->m_myText, g_developerConsole->m_cursorPosition, Rgba::WHITE, bufferSize);
	g_developerConsoleShader->m_myVerts = verts;
	g_developerConsoleShader->m_buffer = g_developerConsoleShader->CreateBuffer(g_developerConsoleShader->m_myVerts, bufferSize);
	g_developerConsoleShader->m_VAO = g_developerConsoleShader->CreateVAO(g_developerConsoleShader->m_buffer);


	//Log
	g_developerLog = new DeveloperLog(g_masterFont);
	g_developerLogShader = new BasicShader(g_masterFontShader, g_masterFontImage.c_str());
	g_developerLog->m_accendingActiveLines = true;

	bufferSize = 0;
	verts = g_developerLog->CreateFontVertsPerChar(g_developerLog->m_myText, g_developerLog->m_cursorPosition, Rgba::WHITE, bufferSize);
	g_developerLogShader->m_myVerts = verts;
	g_developerLogShader->m_buffer = g_developerLogShader->CreateBuffer(g_developerLogShader->m_myVerts, bufferSize);
	g_developerLogShader->m_VAO = g_developerLogShader->CreateVAO(g_developerLogShader->m_buffer);

	
	//FPS
	g_fpsFont = new Font(g_masterFont);
	g_fpsShader = new BasicShader(g_masterFontShader, g_masterFontImage.c_str());

	bufferSize = 0;
	verts = g_fpsFont->CreateFontVertsPerChar(g_fpsFont->m_myText, g_fpsFont->m_cursorPosition, Rgba::WHITE, bufferSize);
	g_fpsShader->m_myVerts = verts;
	g_fpsShader->m_buffer = g_fpsShader->CreateBuffer(g_fpsShader->m_myVerts, bufferSize);
	g_fpsShader->m_VAO =g_fpsShader->CreateVAO(g_fpsShader->m_buffer);


	//InfoTable
	g_infoTableFont = new Font(g_masterFont);
	g_infoTableShader = new BasicShader(g_masterFontShader, g_masterFontImage.c_str());
	g_infoTableFont->m_activeTextLinesLimit = g_numberInfoTableLines;

	bufferSize = 0;
	verts = g_infoTableFont->CreateFontVertsPerChar(g_infoTableFont->m_myText, g_infoTableFont->m_cursorPosition, Rgba::WHITE, bufferSize);
	g_infoTableShader->m_myVerts = verts;
	g_infoTableShader->m_buffer = g_infoTableShader->CreateBuffer(g_infoTableShader->m_myVerts, bufferSize);
	g_infoTableShader->m_VAO = g_infoTableShader->CreateVAO(g_infoTableShader->m_buffer);

// 	Mesh* particleSystemMesh = new Mesh();
// 
// 	g_consoleLogMeshRenderer = MeshRenderer::CreateOrGetMeshRenderer("Console/LogBG Renderer", particleSystemMesh, "particleBillboard", GEOPOINT);
// 	g_consoleLogMeshRenderer->SetMaterialAmbientPower(1.0f);

	

}
void DeveloperLog::DetectInputForConsole()
{
	if (g_developerConsole->UpdateTextString())
	{
		UpdateDeveloperLogShader(g_developerConsoleShader, g_developerConsole, Vector2(5.0f, 15.0f), Rgba::SUNLIGHTYELLOW);
		UpdateInfoTextShader(g_developerLogShader, g_developerLog, Vector2(5.0f, 15.0f + g_developerLog->m_cursorHeight));
	}

	if (s_theInputSystem->wasKeyReleased(VK_RETURN))
	{
		//Allows for UpKey to get last entered string
		g_developerLog->m_myLastEntry = g_developerConsole->m_myText;

		g_developerLog->m_myEntireLog.push_back(g_developerConsole->m_myText);

		g_developerLog->m_myText += g_developerConsole->m_myText;
		g_developerLog->m_logToSave += g_developerLog->m_myText;

		//Process commands if a command
		g_developerLog->ProcessCommand(g_developerConsole->m_myText);
		g_developerConsole->m_myText.clear();

		//Render
		UpdateDeveloperLogShader(g_developerConsoleShader, g_developerConsole, Vector2(5.0f, 15.0f), Rgba::SUNLIGHTYELLOW);
		UpdateInfoTextShader(g_developerLogShader, g_developerLog, Vector2(5.0f, 15.0f + g_developerLog->m_cursorHeight));
	}

	if (s_theInputSystem->wasKeyReleased(VK_UP))
	{
		g_developerConsole->m_myText = g_developerLog->m_myLastEntry;
		UpdateDeveloperLogShader(g_developerConsoleShader, g_developerConsole, Vector2(5.0f, 15.0f), Rgba::SUNLIGHTYELLOW);
		UpdateInfoTextShader(g_developerLogShader, g_developerLog, Vector2(5.0f, 15.0f + g_developerLog->m_cursorHeight));
	}
}
bool DeveloperLog::ToggleConsoleVisibility()
{
	g_displayConsole = !g_displayConsole;
	if (g_displayConsole == false)
	{
		m_myText.clear();
		return true;
	}
	return false;
}
void DeveloperLog::ClearLog()
{
	m_myText.clear();
	m_activeTextLines.clear();
}




//===================================================================================================
void DeveloperLog::UpdateDeveloperLogShader(BasicShader* shader, Font* font, Vector2 position, Rgba color)
{
	int bufferSize = 0;
	shader->m_myVerts = font->CreateFontVertsPerChar(font->m_myText, position, color, bufferSize);
	shader->CopyToBuffer(shader->m_buffer, shader->m_myVerts, bufferSize);
}
void DeveloperLog::UpdateInfoTextShader(BasicShader* shader, Font* font, Vector2 position)
{
	int bufferSize = 0;
	shader->m_myVerts = font->CreateFontVertsPerCharLines(position, bufferSize);
	shader->CopyToBuffer(shader->m_buffer, shader->m_myVerts, bufferSize);
}



//===================================================================================================
void DeveloperLog::InitializeAllCommands()
{
	RegisterCommand( "/help", HelpCommand );
	RegisterCommand( "/clear", ClearCommand );
	RegisterCommand( "/quit",  QuitCommand );
	RegisterCommand( "/invalid", InvalidCommand );
	RegisterCommand( "/printf", PrintToConsole );
}
void DeveloperLog::RegisterCommand( std::string functionName, command_callback callback )
{
	m_commandMap[functionName] = callback;
}



//================================================================
//                    Running Commands from Console 
//================================================================
std::string		 DeveloperLog::FindCommandByNameAndReturnName( const std::string commandToFind )
{
	for(std::map< std::string, command_callback >::iterator found = m_commandMap.begin(); found != m_commandMap.end(); ++found)
		if(found->first == commandToFind){
			std::string whatWasFound = found->first;
			return whatWasFound;
		}
	return "invalid";
}
command_callback DeveloperLog::FindCommandByNameAndReturnFunction( const std::string commandToFind )
{
	for(std::map< std::string, command_callback >::iterator found = m_commandMap.begin(); found != m_commandMap.end(); ++found)
		if(found->first == commandToFind){
			command_callback whatWasFound = found->second;
			return whatWasFound;
		}
	return InvalidCommand;
}
void			 DeveloperLog::ProcessCommand( std::string inCommand ) 
{
	std::string chopCommand = inCommand;
	std::string command;
	std::string arguments;

	chopCommand.erase(0,chopCommand.find_first_of( "/" ));

	command = chopCommand.substr( 0, chopCommand.find_first_of( " (" ) ) ;
	std::string command_name =	FindCommandByNameAndReturnName( command );

	arguments = chopCommand.erase( 0, chopCommand.find( command_name ) + command_name.length() + 1 );
	ConsoleCommandArgs args = ConsoleCommandArgs( arguments );
	
	ExecuteConsoleCommand( command_name, args );
}
void			 DeveloperLog::ExecuteConsoleCommand( std::string command_name, ConsoleCommandArgs &args )
{
	command_callback functionToExecute = FindCommandByNameAndReturnFunction( command_name );
	if (nullptr != functionToExecute) {
		functionToExecute( args );
	} 
	else 
		this->m_myText += "How did you get on this line of code?";
}



//================================================================
//                    Available Commands
//================================================================
void	HelpCommand( ConsoleCommandArgs  &args )
{
	UNUSED(args);
	DeveloperLog::GetDeveloperLog()->ClearLog();
	DeveloperLog::GetDeveloperLog()->m_myText += "/quit  ::  /clear  ::  /printf";
}
void	ClearCommand( ConsoleCommandArgs  &args ) 
{
	UNUSED(args);
	DeveloperLog::GetDeveloperLog()->ClearLog();
}
void	QuitCommand( ConsoleCommandArgs  &args ) 
{
	UNUSED(args);
	s_theInputSystem->SetSystemQuit();
}
void	InvalidCommand( ConsoleCommandArgs  &args ) 
{
	UNUSED(args);
	DeveloperLog::GetDeveloperLog()->m_myText += ": is an invalid command. || ";
}
void	PrintToConsole( ConsoleCommandArgs  &args )
{
	DeveloperLog::GetDeveloperLog()->ClearLog();
	std::string out = args.GetArgument();
	ConsolePrintf("%s\n", out.c_str());
}




std::string ConsolePrintf(const char* messageFormat, ...)
{
	static const size_t BUFFER_SIZE = 2048;
	static char message[BUFFER_SIZE];
	va_list argumentList;
	va_start(argumentList, messageFormat);
	_vsnprintf_s(message,BUFFER_SIZE, BUFFER_SIZE-1, messageFormat, argumentList);
	va_end(argumentList);

	//Windows Specific
	OutputDebugStringA(message);
	return message;
}

void LogClearf()
{
	std::ofstream outputToFile;
	outputToFile.open("../developerLog.txt", std::fstream::trunc); 

	if (!outputToFile) 
		return;

	outputToFile.close();
}

void LogPrintf(const char* messageFormat, ...)
{
	std::ofstream outputToFile;
	outputToFile.open("../developerLog.txt", std::ios::app); 

	if (!outputToFile) 
		MessageBoxA ( NULL, "File Not Found!", "No file available for output.", MB_OK );

	static const size_t BUFFER_SIZE = 2048;
	static char message[BUFFER_SIZE];
	va_list argumentList;
	va_start(argumentList, messageFormat);
	_vsnprintf_s(message,BUFFER_SIZE, BUFFER_SIZE-1, messageFormat, argumentList);
	va_end(argumentList);

	//Windows Specific
	outputToFile << message;

	outputToFile.close();
}

std::string MakeComplexString(const char* messageFormat, ...)
{
	static const size_t BUFFER_SIZE = 2048;
	static char message[BUFFER_SIZE];
	va_list argumentList;
	va_start(argumentList, messageFormat);
	_vsnprintf_s(message,BUFFER_SIZE, BUFFER_SIZE-1, messageFormat, argumentList);
	va_end(argumentList);

	return message;
}





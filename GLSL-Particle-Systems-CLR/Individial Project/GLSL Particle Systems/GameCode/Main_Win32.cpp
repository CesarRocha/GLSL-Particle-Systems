//================================================================
//                     Main_Win32.cpp
//================================================================
#pragma once 
#include <Windows.h>
#include <cstring>
#include "TheApp.hpp"
#include "GameCommon.hpp"
#include "Engine/EngineCommon/EngineCommon.hpp"
#include "Engine/System/Commanlets/Commanlets.hpp"
#include "Engine/Utility/Utility.hpp"
static void ProcessRawCommandLine(LPSTR lpCmdLine);
#define DISCARD_PARAMETER(p) (void)p
TheApp* TheApp::s_theApp = NULL;



//===================================================================================================
LRESULT __stdcall GameMessageProcessingFunction(HWND windowHandle, UINT messageID, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc( windowHandle, messageID, wParam, lParam);
}



//===================================================================================================
HWND CreateAppWindow( HINSTANCE thisAppInstance, int showCommand )
{   
	WNDCLASSEXW windowDescription ;
	memset( &windowDescription, 0, sizeof( WNDCLASSEXW ));

	windowDescription.hbrBackground ;
	windowDescription.cbSize		=	sizeof( WNDCLASSEXW ) ;
	windowDescription.hInstance		=	thisAppInstance ;
	windowDescription.lpszClassName =	TEXT("GLSL Particle Systems") ;
	windowDescription.lpfnWndProc	=	GameMessageProcessingFunction ;
	windowDescription.style			=	CS_HREDRAW | CS_OWNDC | CS_VREDRAW ;

	ATOM returnValue	=	RegisterClassEx( &windowDescription ) ; 
	DISCARD_PARAMETER (returnValue);

	DWORD	style	= WS_OVERLAPPEDWINDOW ;
	int		x		= CW_USEDEFAULT ;
	int		y		= 0 ;
	int		width	= CW_USEDEFAULT ;
	int		height	= 0;
	

	DISCARD_PARAMETER (style);
	DISCARD_PARAMETER (x		);
	DISCARD_PARAMETER (y		);
	DISCARD_PARAMETER (width    );
	DISCARD_PARAMETER (height   );


	HWND desktopWindow = GetDesktopWindow();
	
	RECT windowRect;
	RECT desktopWindowRect;
	GetClientRect(desktopWindow, &desktopWindowRect);
	

	DWORD windowsStyleFlags = WS_OVERLAPPEDWINDOW;
	DWORD windowsStyleFlagsEx = WS_EX_APPWINDOW;
	DWORD borderCase = WS_OVERLAPPEDWINDOW;
	if (g_borderless)
		borderCase = WS_POPUP;


	AdjustWindowRectEx(&windowRect, windowsStyleFlags, false, windowsStyleFlagsEx);
	if (g_fullScreen)
		GetWindowRect(desktopWindow, &windowRect);
	else
	{
		windowRect.left = 190;
		windowRect.top = 90;
		windowRect.right = 1600;
		windowRect.bottom = 900;
	}
	if(!g_borderless)
		windowRect.bottom -= 47;

	g_projectName = "EnhancedParticleSystem";
	HWND WindowHandle = CreateWindow( 
		TEXT("GLSL Particle Systems"),
		TEXT("GLSL Particle Systems"),
		borderCase,
		windowRect.left, 
		windowRect.top, 
		windowRect.right, 
		windowRect.bottom, 
		NULL, 
		NULL, 
		thisAppInstance, 
		NULL);

	if (WindowHandle)
	{
		ShowWindow( WindowHandle, showCommand);
		UpdateWindow(WindowHandle);
	}



	return WindowHandle ;
}



//===================================================================================================
void ProcessRawCommandLine(LPSTR lpCmdLine)
{
	if (lpCmdLine == nullptr) {
		return;
	}

	// Convert to ASCII if we're using Unicode
	std::string command_line;
	if (sizeof(TCHAR) == sizeof(wchar_t)) 
	{
		std::string temp = (std::string)lpCmdLine;
		command_line.assign( temp.begin(), temp.end() );
	} else 
	{
		// this conversion does nothing if we're in wchar_t mode
		command_line = (const char*)lpCmdLine;
	}

	CommandletsList commands = ParseCommands(command_line);
	ProcessCommands(commands);
	FreeCommands( commands );
}



//===================================================================================================
void SetupEngineSystems()
{
	//Setup System Settings
	std::vector<std::string> listOfXMLFiles;
	FindAllFilesOfType("./", "EngineSettings.ini", listOfXMLFiles); //Defaults ./Run_Win32

	XMLNode gameCommonXMLNode;
	for (std::vector<std::string>::iterator iter = listOfXMLFiles.begin(); iter != listOfXMLFiles.end(); iter++)
	{

		const std::string& xmlFileName = *iter;
		gameCommonXMLNode = gameCommonXMLNode.parseFile(xmlFileName.c_str());

		XMLNode projectNode = gameCommonXMLNode.getChildNode("Project");
		XMLNode settingsNode = projectNode.getChildNode("Settings");

		const char* result = settingsNode.getAttribute("renderWorld");
		if(result)
			g_renderWorld		= (strcmp("true", result) == 0);
		
		result = settingsNode.getAttribute("use3DCamera");
		if(result)
			g_use3DCamera		= (strcmp("true", result) == 0);

		result = settingsNode.getAttribute("displayConsole");
		if(result)
			g_displayConsole	= (strcmp("true", result) == 0);

		result = settingsNode.getAttribute("displayFPS");
		if (result)
			g_displayFPS		= (strcmp("true", result) == 0);

		result = settingsNode.getAttribute("displayInfoTable");
		if (result)
		g_displayInfoTable	= (strcmp("true", result) == 0);

		result = settingsNode.getAttribute("polygonLines");
		if (result)
			g_polygonLines		= (strcmp("true", result) == 0);

		result = settingsNode.getAttribute("drawAxes");
		if (result)
			g_drawAxes			= (strcmp("true", result) == 0);

		result = settingsNode.getAttribute("fullScreen");
		if (result)
			g_fullScreen		= (strcmp("true", result) == 0);
		
		result = settingsNode.getAttribute("borderless");
		if (result)
			g_borderless		= (strcmp("true", result) == 0);

		result = settingsNode.getAttribute("numberOfInfoLines");
		if (result)
		{
			std::string nl = result;
			g_numberInfoTableLines = stoi(nl.substr(0, nl.find_first_of("~")));
		}

		result = settingsNode.getAttribute("clearBufferColor");
		if (result)
		{
			std::string cs = result;
			unsigned char c[3];
			for (int i = 0; i < 3; i++)
			{
				c[i] = stoi(cs.substr(0, cs.find_first_of(",")));
				cs.erase(0, cs.find(",") + 1);
			}
			g_clearBufferColor = Rgba(c[0], c[1], c[2]);
		}



	}
}



//===================================================================================================
int __stdcall WinMain( HINSTANCE thisAppInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	DISCARD_PARAMETER(hPrevInstance);
	DISCARD_PARAMETER(lpCmdLine);

	ProcessRawCommandLine( lpCmdLine );


	SetupEngineSystems();


	HWND myWindowHandle = CreateAppWindow( thisAppInstance, nShowCmd ) ;

		TheApp::s_theApp = new TheApp();
		TheApp::s_theApp->Startup( myWindowHandle );
		TheApp::s_theApp->Run();
		TheApp::s_theApp->Shutdown();
}


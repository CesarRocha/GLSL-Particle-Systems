//================================================================
//                     AssertionMacros.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_AssertionMacros_
#define _INCLUDE_AssertionMacros_
#include "Engine\EngineCommon\EngineCommon.hpp"
#include <stdio.h>
#include <string>
#include <vector>
#include <io.h>
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <sstream>

//###################################################################################################
//##            Warnings / Errors                                                                  ##
//###################################################################################################
static inline void RecoverableError(const std::string& FILE, const std::string& FUNCTION, int LINE, const std::string& errorMessage)
{
	ShowCursor(TRUE);
	std::string header = "Recoverable Error";

	std::stringstream out;
	out << LINE;
	std::string message1 =
		"FILE:     " + FILE + "\n"
		+ "Function: " + FUNCTION + "\n"
		+ "Line:     " + out.str() + "\n\n";

	std::string message2 =
		"Error: " + errorMessage + "\n\n"
		+ "Press OK to Continue || Cancel to exit program";

	std::string message3 = message1 + "\n" + message2;

	int result = MessageBoxA(NULL, message3.c_str(), header.c_str(), MB_OKCANCEL | MB_ICONEXCLAMATION);

	switch (result)
	{
	case IDOK: break;
	case IDCANCEL: g_appIsRunning = false;//EXIT PROGRAM;
	}

	ShowCursor(FALSE);

}

static inline void FatalError(const std::string& FILE, const std::string& FUNCTION, int LINE, const std::string& errorMessage)
{
	ShowCursor(TRUE);
	std::string header = "Fatal Error";

	std::stringstream out;
	out << LINE;
	std::string message1 =
		"FILE:     " + FILE + "\n"
		+ "Function: " + FUNCTION + "\n"
		+ "Line:     " + out.str() + "\n\n";

	std::string message2 =
		"Error: " + errorMessage + "\n\n"
		+ "Exiting Program...";


	std::string message3 = message1 + "\n" + message2;

	MessageBoxA(NULL, message3.c_str(), header.c_str(), MB_OK | MB_ICONERROR);
	ShowCursor(FALSE);
	g_appIsRunning = false;
	//Exit Program
}

static inline void RecoverableAssertion(const std::string& FILE, const std::string& FUNCTION, int LINE, const std::string& errorMessage, const std::string& conditionText)
{
	ShowCursor(TRUE);
	std::string header = "Recoverable Assertion";

	std::stringstream out;
	out << LINE;
	std::string message1 =
		"FILE:     " + FILE + "\n"
		+ "Function: " + FUNCTION + "\n"
		+ "Line:     " + out.str() + "\n\n";

	std::string message2 =
		"Error: " + errorMessage + "\n"
		+ "Condition: " + conditionText + "\n\n" +
		+"Press OK to Continue || Cancel to exit program";

	std::string message3 = message1 + "\n" + message2;

	int result = MessageBoxA(NULL, message3.c_str(), header.c_str(), MB_OKCANCEL | MB_ICONEXCLAMATION);

	switch (result)
	{
	case IDOK: break;
	case IDCANCEL: g_appIsRunning = false; //EXIT PROGRAM;
	}
	ShowCursor(FALSE);
}

static inline void FatalAssertion(const std::string& FILE, const std::string& FUNCTION, int LINE, const std::string& errorMessage, const std::string& conditionText)
{
	ShowCursor(TRUE);
	std::string header = "Fatal Assertion";

	std::stringstream out;
	out << LINE;
	std::string message1 =
		"FILE:     " + FILE + "\n"
		+ "Function: " + FUNCTION + "\n"
		+ "Line:     " + out.str() + "\n\n";

	std::string message2 =
		"Error: " + errorMessage + "\n"
		+ "Condition: " + conditionText;

	std::string message3 = message1 + "\n" + message2;

	MessageBoxA(NULL, message3.c_str(), header.c_str(), MB_OK | MB_ICONERROR);
	ShowCursor(FALSE);
	g_appIsRunning = false;
	//Exit Program
}

//###################################################################################################
//##            Assertion Macros                                                                   ##
//###################################################################################################
//#define ASSERTION_MACRO_SUITE

#if defined(ASSERTION_MACRO_SUITE)	 
#define FATAL_ERROR(errorMessage) {(void);}
#else
#define	FATAL_ERROR(errorMessage)\
{\
		FatalError(__FILE__, __FUNCTION__, __LINE__, errorMessage);\
}
#endif

//===================================================================================================
#if defined(ASSERTION_MACRO_SUITE)
#define RECOVERABLE_ERROR(errorMessage) {(void);}
#else
#define	RECOVERABLE_ERROR(errorMessage)\
{\
	RecoverableError(__FILE__, __FUNCTION__, __LINE__, errorMessage);\
}
#endif

//===================================================================================================
#if defined(ASSERTION_MACRO_SUITE)
#define FATAL_ASSERTION(condition, errorMessage) {(void)(condition);}
#else	
#define	FATAL_ASSERTION(condition, errorMessage)\
{\
	if(!condition)\
	{\
		std::string conditionText = #condition;\
		FatalAssertion(__FILE__, __FUNCTION__, __LINE__, errorMessage, conditionText);\
	}\
}
#endif

//===================================================================================================
#if defined(ASSERTION_MACRO_SUITE)
#define RECOVERABLE_ASSERTION(condition, errorMessage) {(void)(condition);}
#else
#define	RECOVERABLE_ASSERTION(condition, errorMessage)\
{\
	if(!condition)\
	{\
		std::string conditionText = #condition;\
		RecoverableAssertion(__FILE__, __FUNCTION__, __LINE__, errorMessage, conditionText);\
	}\
}
#endif
#endif

//================================================================
//                     Input.cpp
//================================================================
#define WIN32_LEAN_AND_MEAN
#include "InputSystem.hpp"
#include <Windows.h>
InputSystem* s_theInputSystem = NULL;
LRESULT __stdcall windowMessageProcessingFunction(HWND windowHandle, UINT messageID, WPARAM wParam, LPARAM lParam)
{
	if(s_theInputSystem)
	{
		switch ( messageID )
		{
		case WM_KEYDOWN:
			s_theInputSystem->setKeyState((int)wParam, true);			return 0;
		case WM_KEYUP:
			s_theInputSystem->setKeyState((int)wParam, false);			return 0;
		case WM_CLOSE:
			s_theInputSystem->SetSystemQuit();							return 0;


		case WM_LBUTTONDOWN:  //513
			s_theInputSystem->setMouseState((int)wParam, true);			return 0;
		case WM_LBUTTONUP:    //514
			s_theInputSystem->setMouseState((int)wParam +1, false);	return 0;


		case WM_RBUTTONDOWN:  //516
			s_theInputSystem->setMouseState((int)wParam, true);			return 0;
		case WM_RBUTTONUP:    //517
			s_theInputSystem->setMouseState((int)wParam + 2, false);	return 0;





		default: break;
		}
	}
	return DefWindowProc( windowHandle, messageID, wParam, lParam);
}


//===================================================================================================
//               class InputSystem                                                                 ==
//===================================================================================================
InputSystem::InputSystem()
	: m_platformHandle(NULL)
	, m_previousAddress(0L)
	, m_isReadyToQuit(false)
{
	if (!s_theInputSystem)
		s_theInputSystem = this;
}

InputSystem::~InputSystem()
{
}


//----------------------------------------------------------------------------------------------
void InputSystem::Startup(void* platformHandle)
{
	m_platformHandle = platformHandle;
	m_previousAddress = SetWindowLong((HWND) platformHandle, GWL_WNDPROC, (long)& windowMessageProcessingFunction);


	//Keyboard
	for(int i = 0; i < NUMBER_OF_VIRTUAL_KEYS; ++i)
	{
		m_keyStates[i].m_isDown = false;
		m_keyStates[i].m_wasReleased = false;
	}


	//Mouse
	for (unsigned int mouseIndex = 0; mouseIndex < NUMBER_OF_MOUSE_KEYS; mouseIndex++)
	{
		m_mouseStates[mouseIndex].m_isDown = false;
		m_mouseStates[mouseIndex].m_wasReleased = false;
	}
}

void InputSystem::Update()
{
	HWND myWindowHandle = (HWND)(m_platformHandle);
	MSG queuedMessage ;

	for(int key = 0; key < NUMBER_OF_VIRTUAL_KEYS; ++key)
		m_keyStates [ key ].m_wasReleased = false;


	//Mouse
	for (unsigned int i = 0; i < NUMBER_OF_MOUSE_KEYS; i++)
		m_mouseStates[i].m_wasReleased = false;



	for ( ; ; )
	{
		BOOL messageWasFound = PeekMessage( &queuedMessage, myWindowHandle, 0 ,0, PM_REMOVE);
		if (!messageWasFound)
			break;
		
		TranslateMessage( &queuedMessage);
		DispatchMessage ( &queuedMessage);
	}


}

void InputSystem::Shutdown()
{	
	SetWindowLong((HWND) m_platformHandle, GWL_WNDPROC, (long)& m_previousAddress);
}


//Keyboard
//----------------------------------------------------------------------------------------------
void InputSystem::setKeyState(int key, bool isPressedDown)
{
	if ( m_keyStates[key].m_isDown && !isPressedDown)
		m_keyStates[key].m_wasReleased = true;
	

	if (key >= 0 && key < NUMBER_OF_VIRTUAL_KEYS)
		m_keyStates [ key ].m_isDown = isPressedDown;
}

bool InputSystem::wasKeyReleased(int key)
{
	if (key >= 0 && key < NUMBER_OF_VIRTUAL_KEYS)
		return m_keyStates [ key ].m_wasReleased;
	else 
		return false;
}

bool InputSystem::isKeyDown(int key)
{
	if (key >= 0 && key < NUMBER_OF_VIRTUAL_KEYS)
		return m_keyStates [ key ].m_isDown;
	else 
		return false;
	
}


//Mouse
//----------------------------------------------------------------------------------------------
void InputSystem::setMouseState(int mouseButton, bool isPressedDown)
{
	if (m_mouseStates[mouseButton].m_isDown == true && isPressedDown == false)
		m_mouseStates[mouseButton].m_wasReleased = true;
		
	if (mouseButton >= 0 && mouseButton < NUMBER_OF_MOUSE_KEYS)
		m_mouseStates[mouseButton].m_isDown = isPressedDown;
}

bool InputSystem::wasMouseReleased(int mouseButton)
{
	if (mouseButton >= 0 && mouseButton < NUMBER_OF_MOUSE_KEYS)
		return m_mouseStates[mouseButton].m_wasReleased;
	else
		return false;
}

bool InputSystem::isMouseDown(int mouseButton)
{
	if (mouseButton >= 0 && mouseButton < NUMBER_OF_MOUSE_KEYS)
		return m_mouseStates[mouseButton].m_isDown;
	else
		return false;
}





//----------------------------------------------------------------------------------------------
void InputSystem::SetSystemQuit()
{
	m_isReadyToQuit = true;
}

void InputSystem::ShowMouseCursor(bool isVisiable)
{
	if (isVisiable)
		ShowCursor(TRUE);
	else
		ShowCursor(FALSE);
}
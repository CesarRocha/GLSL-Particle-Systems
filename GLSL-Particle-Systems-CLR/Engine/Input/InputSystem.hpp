//================================================================
//                     InputSystem.hpp
//================================================================
#pragma once
#ifndef __Included__InputSystems__
#define __Included__InputSystems__
#include "Engine\Math\Vector2.hpp"

#define NUMBER_OF_VIRTUAL_KEYS 256
#define  NUMBER_OF_MOUSE_KEYS 3

struct KeyState
{
	bool	m_isDown;
	bool	m_wasReleased;
};

struct MouseState
{
	bool	m_isDown;
	bool	m_wasReleased;
};

//===================================================================================================
//               class InputSystem                                                                 ==
//===================================================================================================
class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void		Startup(void* platformHandle);
	void		Update();
	void		Shutdown();
		
	//Keyboard
	void		setKeyState(int key, bool isPressedDown);
	bool		wasKeyReleased(int key);
	bool		isKeyDown(int key);

	//Mouse
	void		setMouseState(int mouseButton, bool isPressedDown);
	bool		isMouseDown(int mouseButton);
	bool		wasMouseReleased(int mouseButton);

	const Vector2& GetMousePosition() { return m_mousePosition; }
	void		SetMousePosition(const Vector2& position) { m_mousePosition = position; }
	void		ShowMouseCursor(bool isVisiable);


	bool		isReadyToQuit() const {return m_isReadyToQuit;}
	void		SetSystemQuit();

private:
	void*		m_platformHandle;
	long		m_previousAddress;
	bool		m_isReadyToQuit;

	KeyState	m_keyStates[NUMBER_OF_VIRTUAL_KEYS];

	MouseState  m_mouseStates[NUMBER_OF_MOUSE_KEYS];
	Vector2		m_mousePosition;
};

extern InputSystem* s_theInputSystem;

#endif
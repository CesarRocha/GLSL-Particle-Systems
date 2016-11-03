#pragma once
#include "GameCommon.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Camera3D.hpp"
#include "Engine/Clock/MasterClock.hpp"
#include "World.hpp"



//===================================================================================================
//					class TheApp																   ==
//===================================================================================================
class TheApp
{
public:
	TheApp();
	~TheApp();
	static TheApp* s_theApp;	
	
	void	Startup(void* windowHandle);
	void	Run();
	void	Shutdown();

	void	ProcessInput();
	void	UpdateGameTime();

	void	UpdateWorld();
	void	RenderWorld();

	double	GetDeltaSeconds() {return m_deltaSeconds;}

private:
	void*					m_windowHandle;
	OpenGLRenderer*			m_renderer;	
	World*					m_world;
	InputSystem*			m_inputSystem;
	double					m_deltaSeconds;
};


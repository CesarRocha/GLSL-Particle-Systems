//================================================================
//                     World.hpp
//================================================================
#pragma once
#ifndef __INCLUDE_WORLD__
#define __INCLUDE_WORLD__
#include "GameCommon.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/ParticleSystem/ParticleSystem.hpp"



//===================================================================================================
//				class World 																	   ==
//===================================================================================================
class World
{
public:
	//Constructors
	World(const Vector2& displaySize);
	~World();

	//Systems
	void	StartUp();
	void	ShutDown();
	void	Update(double deltaSeconds);
	void	Render(OpenGLRenderer* renderer);
	void	GameInput(InputSystem* inputSystem);


private:
	friend class TheApp;
	Vector2	m_displaySize;
	bool	m_paused;
};

#endif
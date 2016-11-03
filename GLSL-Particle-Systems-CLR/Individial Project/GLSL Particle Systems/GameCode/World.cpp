#include "World.hpp"
#include "Engine/ParticleSystem/ParticleSystem.hpp"



//===================================================================================================
//               class World	                                                                   ==
//===================================================================================================
World::World(const Vector2& displaySize)
	: m_displaySize(displaySize)
	, m_paused(false)
{	
	srand(time(0));
}
World::~World()
{
}



//===================================================================================================
void World::StartUp()
{
	ParticleSystem::InitializeParticleSystem();
}
void World::GameInput(InputSystem* inputSystem)
{
	if(inputSystem)
	{
		if (inputSystem->isKeyDown(VK_CONTROL) && inputSystem->wasKeyReleased('P'))
			m_paused = !m_paused;
	}
}


//===================================================================================================
void World::Update(double deltaSeconds)
{
	if(m_paused)
		return;

	g_enhancedParticleSystem->UpdateParticleSystem(deltaSeconds);

}
void World::Render(OpenGLRenderer* renderer)
{
	UNUSED(renderer);
	g_enhancedParticleSystem->RenderParticleSystem();
}



//===================================================================================================
void World::ShutDown()
{
}






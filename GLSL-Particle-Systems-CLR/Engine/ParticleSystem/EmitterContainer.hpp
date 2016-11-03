#pragma once
#ifndef _EmitterContainer_
#define _EmitterContainer_
#include "ParticleEmitter.hpp"
#include <vector>
typedef std::vector<ParticleEmitter*> EmitterFactory;
typedef std::vector<ParticleEmitter*> RunningEmitters;
enum ParticleSystemSearchReports
{
	INVALID_PARTIClE_SYSTEM_ERROR = -1,
	PARTICLE_SYSTEM_CONTAINER_FOUND,
	PARTICLE_SYSTEM_CONTAINER_NOT_FOUND,
	PARTICLE_SYSTEM_EMITTER_FOUND,
	PARTICLE_SYSTEM_EMITTER_NOT_FOUND,
	NUM_PARTICLE_SYSTEM_ERRORS

};



//===================================================================================================
//				class EmitterContainer													   ==
//===================================================================================================
class EmitterContainer
{
public:
	//In Progress
	bool Render(MeshRenderer* meshRenderer, std::vector<vert_t>& verts);


	//System
	void LoadEmittersFromXML(XMLNode containerNode);
	void UpdateEmitterContainer(double deltaSeconds);
	void GetEmitterVERT_SForRendering(std::vector<vert_t>& out_vertsFromParticles);
	bool GetTextureForRendering(Texture*& out_texture);


	//Load from XML
	void CreateParticlesInEmitterPI(std::string emitterName, int particleCount, ParticleInformation particleInformation);
	void CreateParticlesInEmitterXML(std::string emitterName, int particleCount, XMLNode xmlNode);


	//Creation
	ParticleEmitter* CreateNewEmitter(std::string emitterName);
	

	//Spawn From Factory
	std::string SpawnEmitterFromFactory(std::string existingEmitterName);

	
	//Activation
	int  ActivateAllEmitterParticles();
	int  ActivateEmitterParticles(std::string emitterName);
	int	 DeactivateAllEmitterParticles();
	int  DeactivateEmitterParticles(std::string emitterName);


	//Modification
	void ChangeAllFactoryEmitterPositions(Vector3 newPosition);
	void ChangeAllRunningEmitterPositions(Vector3 newPosition);

	
	std::string ChangeEmitterTexture(std::string existingEmitterName, std::string newTextureName, int totalSprites = 1, int spritesX = 1, int spritesY = 1);
	void ChangePositionX(std::string existingEmitterName, Vector2 MinMax);
	void ChangePositionY(std::string existingEmitterName, Vector2 MinMax);
	void ChangePositionZ(std::string existingEmitterName, Vector2 MinMax);

	void ChangeVelocityX(std::string existingEmitterName, Vector2 MinMax);
	void ChangeVelocityY(std::string existingEmitterName, Vector2 MinMax);
	void ChangeVelocityZ(std::string existingEmitterName, Vector2 MinMax);

	void ChangeSpawnRate(std::string existingEmitterName, int particles, float overTime);

	void ExportContainerToFile(XMLNode& newXMLFile, std::string newContainernName);
	void ChangeLifeSpan(std::string existingEmitterName, float newLifeSpan);
	void ChangeColor(std::string existingEmitterName, Rgba newColor);
	std::string						m_containerName;
	Vector3							m_containerPosition;
	Vector3							m_previousContainerPosition;
	AnimatedTexture*				m_containerAnimatedTexture;
	char							m_totalSprites;

private:
	friend class ParticleSystem;
	EmitterFactory					s_emitterFactory;
	RunningEmitters					m_runningEmitters;
	ContainerXMLData				m_xmlContainerDataForExporting;
};
#endif // !_EmitterContainer_

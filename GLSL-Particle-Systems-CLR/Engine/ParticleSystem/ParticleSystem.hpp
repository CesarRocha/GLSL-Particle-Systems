#pragma once
#ifndef _ParticleSystem_
#define _ParticleSystem_
#include "EmitterContainer.hpp"
#include "Engine/Renderer/Shader/Material.hpp"
#include "Engine/Renderer/Shader/Mesh.hpp"
#include <map>
typedef std::map<std::string, EmitterContainer*> ContainerFactory;
typedef std::map<std::string, EmitterContainer*> RunningContainers;



//===================================================================================================
//				class ParticleSystem													   ==
//===================================================================================================
class ParticleSystem
{
public:
	//Initialize/Safety
	static void InitializeParticleSystem();
	void ParticleSystemValidation();

	//System
	void UpdateParticleSystem(double deltaseconds);
	void RenderParticleSystem();


	//Load From XML
	void LoadParticleSystemFromXML();
	void LoadContainersFromXML(XMLNode projectNode);


	//Creation
	EmitterContainer* CreateNewContainer(std::string newContainerName);
	ParticleEmitter*  CreateNewEmitter(std::string existingContainerName, std::string newEmitterName);


	//Spawn From Factory
	std::string SpawnContainerFromFactory(std::string containerName);
	std::string SpawnEmitterFromFactory(std::string existingContainerName, std::string existingEmitterName);
	std::string SpawnAllContainersFromFactory();


	//Activation
	std::string ActivateParticlesInExistingContainer(std::string containerName);
	std::string ActivateParticlesInExistingEmitter(std::string containerName, std::string emitterName);
	std::string ActivateAllParticlesInAllContainers();
	

	//Deactivation
	std::string DeactivateParticlesInExistingContainer(std::string existingContainerName);
	std::string DeactivateParticlesInExistingEmitter(std::string existingContainerName, std::string existingEmitterName);
	std::string DeactivateAllParticlesInAllContainers();


	//Modifications
	std::string ChangeContainerPosition(std::string existingContainerName, Vector3 newPosition);
	
	std::string ChangeContainerTextureStill(std::string existingContainerName, std::string newTextureName);
	std::string ChangeContainerTextureAnimated(std::string existingContainerName, std::string newTextureName, int totalSprites, int spritesX, int spritesY);

	std::string ChangeEmitterTexture(std::string existingContainerName, std::string existingEmitterName, std::string newTextureName, int totalSprites, int spritesX, int spritesY);

	std::string ChangePositionX(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax);
	std::string ChangePositionY(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax);
	std::string ChangePositionZ(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax);
	
	std::string ChangeVelocityX(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax);
	std::string ChangeVelocityY(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax);
	std::string ChangeVelocityZ(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax);

	std::string ChangeSpawnRate(std::string existingContainerName, std::string existingEmitterName, int particles, float overTime);
	std::string ChangeLifeSpan(std::string existingContainerName, std::string existingEmitterName, float newLifeSpan);
	std::string ChangeColor(std::string existingContainerName, std::string existingEmitterName, Rgba newColor);


	void		ListAllFactroryContainersToLog();
	void		ListAllActiveContainersToLog();
	std::string ExportContainerToFile(std::string existingContainerName, std::string newContainerName);


private:
	static ContainerFactory						s_containerFactory;
	static RunningContainers					s_runningContainers;

	ParticleSystem();
	Texture*									m_textureToBind;
	static MeshRenderer*						m_particleSystemMeshRenderer;
	std::vector<vert_t>							m_particleVerts;
};









extern ParticleSystem* g_enhancedParticleSystem;
void ParticleSystemCommandController(ConsoleCommandArgs& args);
void ParticleSystemHelpCommand();

void SpawnFromFactoryCommand(ConsoleCommandArgs& args);

void ActivateContainerCommand(ConsoleCommandArgs& args);
void DeactivateContainerCommand(ConsoleCommandArgs& args);

void ExportContainerToFileCommand(ConsoleCommandArgs& args);
void ListFactoryContainersCommand();
void ListActiveContainersCommand();

void ChangeContainerPositionCommand(ConsoleCommandArgs& args);

//===================================================================================================
void ChangeTextureCommand(ConsoleCommandArgs& args);
void ChangePositionXCommand(ConsoleCommandArgs& args);
void ChangePositionYCommand(ConsoleCommandArgs& args);
void ChangePositionZCommand(ConsoleCommandArgs& args);

void ChangeVelocityXCommand(ConsoleCommandArgs& args);
void ChangeVelocityYCommand(ConsoleCommandArgs& args);
void ChangeVelocityZCommand(ConsoleCommandArgs& args);

void ChangeSpawnRateCommand(ConsoleCommandArgs& args);
void ChangeLifeSpanCommand(ConsoleCommandArgs& args);
void ChangeColorCommand(ConsoleCommandArgs& args);





#endif // !_ParticleSystem_






#include "ParticleSystem.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Camera3D.hpp"
ParticleSystem*								g_enhancedParticleSystem;
ContainerFactory							ParticleSystem::s_containerFactory;
RunningContainers							ParticleSystem::s_runningContainers;
MeshRenderer*								ParticleSystem::m_particleSystemMeshRenderer = nullptr;
vert_t*	vertArrayPS;



//===================================================================================================
//				class ParticleSystem															   ==
//===================================================================================================
ParticleSystem::ParticleSystem()
	:m_textureToBind(nullptr)
{
}



void ParticleSystem::InitializeParticleSystem()
{
	if (!g_enhancedParticleSystem)
	{
		LogPrintf("**************************Particle System Initializing**************************\n");
		g_enhancedParticleSystem = new ParticleSystem();

		Mesh* particleSystemMesh = new Mesh();
		g_enhancedParticleSystem->LoadParticleSystemFromXML();

		ParticleSystem::m_particleSystemMeshRenderer = MeshRenderer::CreateOrGetMeshRenderer("Particle Renderer", particleSystemMesh, "particleBillboard", GEOPOINT);
		ParticleSystem::m_particleSystemMeshRenderer->SetMaterialAmbientPower(1.0f);

		LogPrintf("\n\n**************************Particle System Initialized**************************\n");


		DeveloperLog::RegisterCommand("/ps", ParticleSystemCommandController);
		LogPrintf("\n\n**************************Particle System Console Commands Initialized**************************\n");
		return;
	}

	LogPrintf("Particle System Already Exists\n");
}
void ParticleSystem::ParticleSystemValidation()
{
	if (!g_enhancedParticleSystem)
	{
		LogPrintf("Particle System Not Initialized\n");
		InitializeParticleSystem();
	}
}



//===================================================================================================
void ParticleSystem::UpdateParticleSystem(double deltaSeconds)
{
	ParticleSystemValidation();

	for (RunningContainers::iterator iter = s_runningContainers.begin(); iter != s_runningContainers.end(); iter++)
	{
		EmitterContainer* emitterContainer = iter->second;
		emitterContainer->UpdateEmitterContainer(deltaSeconds);
	}
}
void ParticleSystem::RenderParticleSystem()
{
	ParticleSystemValidation();

	std::vector<vert_t> emitterLocationVerts;
	for (RunningContainers::iterator iter = s_runningContainers.begin(); iter != s_runningContainers.end(); iter++)
	{
		//Render the particles of the emitter
		m_particleVerts.clear();
		emitterLocationVerts.clear();


		EmitterContainer* emitterContainer = iter->second;
		bool didAnyRender = emitterContainer->Render(m_particleSystemMeshRenderer, m_particleVerts);

		//Render an icon displaying the emitter count
		if(!didAnyRender)
		{
			vert_t vert;
			vert.m_position = emitterContainer->m_containerPosition;
			vert.m_voxelSize = 3.0;

			size_t emitterCount = emitterContainer->m_runningEmitters.size();
			emitterLocationVerts.push_back(vert);

			size_t voxelCount = emitterLocationVerts.size();
			if (voxelCount != 0)
			{
				vertArrayPS = &emitterLocationVerts[0];

				std::string fileName = MakeComplexString("./data/images/EngineImages/EmitterIcon%i.png", emitterCount);
				Texture* emitterTexture = Texture::CreateOrGetTexture(fileName);
				m_particleSystemMeshRenderer->SetMaterialDiffuseTexture(emitterTexture);
				m_particleSystemMeshRenderer->UpdateMesh(vertArrayPS, voxelCount);
				m_particleSystemMeshRenderer->Render(g_masterCamera->m_worldPosition);
			}
		}
	}
}



//===================================================================================================
void ParticleSystem::LoadParticleSystemFromXML()
{
	//Load all database particles from XML
	std::vector<std::string> listOfXMLFiles;
	FindAllFilesOfType("data/particles/", "*.xml", listOfXMLFiles); //Defaults ./Run_Win32

	XMLNode particleXMLNode;
	for (std::vector<std::string>::iterator iter = listOfXMLFiles.begin(); iter != listOfXMLFiles.end(); iter++)
	{
		const std::string& xmlFileName = *iter;
		particleXMLNode = particleXMLNode.parseFile(xmlFileName.c_str());

		XMLNode projectNode = particleXMLNode.getChildNode("Project");
		LogPrintf("\n\n-----------------------------(%s)\n", xmlFileName.c_str());

		LoadContainersFromXML(projectNode);


		//Auto Spawn and Run if defined in XML
		const char* loadContainer = projectNode.getAttribute("load");
		if (loadContainer && strcmp(loadContainer, "true") == 0)
		{
			XMLNode containerNode = projectNode.getChildNode();
			SpawnContainerFromFactory(containerNode.getAttribute("name"));

			const char* runContainer = projectNode.getAttribute("run");
			if (runContainer && strcmp(runContainer, "true") == 0)
				ActivateParticlesInExistingContainer(containerNode.getAttribute("name"));
		}
	}
}
void ParticleSystem::LoadContainersFromXML(XMLNode projectNode)
{
	//const char* projectName = projectNode.getAttribute("name");

	//System Loads Containers
	int containerCount = projectNode.nChildNode("Container");
	for (int containerIndex = 0; containerIndex < containerCount; containerIndex++)
	{
		XMLNode containerNode = projectNode.getChildNode(containerIndex);
		const char* containerName = containerNode.getAttribute("name");

		EmitterContainer* newContainer = CreateNewContainer(containerName);

		newContainer->LoadEmittersFromXML(containerNode);

	}
}



//===================================================================================================
EmitterContainer* ParticleSystem::CreateNewContainer(std::string newContainerName)
{
	ParticleSystemValidation();

	EmitterContainer* newContainer = new EmitterContainer();
	newContainer->m_containerName = newContainerName;
	s_containerFactory.insert(std::pair<std::string, EmitterContainer*> (newContainerName, newContainer));

	return newContainer;
}
ParticleEmitter*  ParticleSystem::CreateNewEmitter(std::string existingContainerName, std::string newEmitterName)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_containerFactory.find(existingContainerName);
	if (found != s_containerFactory.end())
	{
		EmitterContainer* foundContainer = found->second;
		ParticleEmitter* foundEmitter = foundContainer->CreateNewEmitter(newEmitterName);
		
		return foundEmitter;
	}
	return nullptr;
}



//===================================================================================================
std::string ParticleSystem::SpawnContainerFromFactory(std::string existingContainerName)
{
	ContainerFactory::iterator found = s_containerFactory.find(existingContainerName);
	if (found != s_containerFactory.end())
	{
		EmitterContainer* newContainer = found->second;
		newContainer->m_runningEmitters = newContainer->s_emitterFactory;



		s_runningContainers.insert(std::pair < std::string, EmitterContainer*>(existingContainerName, newContainer));
		
		return MakeComplexString("Spawned '%s' from ContainerFactory", existingContainerName.c_str());
	}
	else
		return MakeComplexString("'%s' does not exist in ContainerFactory", existingContainerName.c_str());

}
std::string ParticleSystem::SpawnEmitterFromFactory(std::string existingContainerName, std::string existingEmitterName)
{
	//Find if the Container exists first
	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);
	if (found != s_runningContainers.end())
	{
		EmitterContainer* newContainer = found->second;
		newContainer->SpawnEmitterFromFactory(existingEmitterName);
		return "";
	}
	else
		return MakeComplexString("'%s' was not found as a running Container. Cannot add additional Emitter", existingContainerName.c_str());

}
std::string ParticleSystem::SpawnAllContainersFromFactory()
{
	int containerCount = 0;
	for (ContainerFactory::iterator iter = s_containerFactory.begin(); iter != s_containerFactory.end(); iter++)
	{
		EmitterContainer* newContainer = iter->second;
		std::string containerName = newContainer->m_containerName;
		newContainer->m_runningEmitters = newContainer->s_emitterFactory;

		s_runningContainers.insert(std::pair < std::string, EmitterContainer*>(containerName, newContainer));
		containerCount++;
	}
	return MakeComplexString("Spawned %i containers from ContainerFactory", containerCount);
}



//===================================================================================================
std::string ParticleSystem::ActivateParticlesInExistingContainer(std::string existingContainerName)
{
	ParticleSystemValidation();
	int activatedParticleCount = 0;

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);
	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		if (foundContainer->m_containerName == existingContainerName)
		{
			activatedParticleCount += foundContainer->ActivateAllEmitterParticles();

			return MakeComplexString("%i particles activated in '%s'", activatedParticleCount, existingContainerName.c_str());
		}
	}
		
	return  MakeComplexString("The container %s does not exist", existingContainerName.c_str());
}
std::string ParticleSystem::ActivateParticlesInExistingEmitter(std::string existingContainerName, std::string existingEmitterName)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	int activatedParticleCount = 0;
	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;
		activatedParticleCount += foundContainer->ActivateEmitterParticles(existingEmitterName);

		if(activatedParticleCount > 0)
			return MakeComplexString("%i particles activated in '%s/%s'", activatedParticleCount, existingContainerName.c_str(), existingEmitterName.c_str());
		else
			return MakeComplexString("'%s exists, %s' does not", existingContainerName.c_str(), existingEmitterName.c_str());
	
	}

	return MakeComplexString("Container '%s' does not exist", existingContainerName.c_str());
}
std::string ParticleSystem::ActivateAllParticlesInAllContainers()
{
	ParticleSystemValidation();
	int activatedParticleCount = 0;

	for(RunningContainers::iterator iter = s_runningContainers.begin(); iter != s_runningContainers.end(); iter++)
	{
		EmitterContainer* foundContainer = iter->second;

		activatedParticleCount += foundContainer->ActivateAllEmitterParticles();
	}
	return MakeComplexString("%i particles activated in all containers", activatedParticleCount);
}



//===================================================================================================
std::string ParticleSystem::DeactivateParticlesInExistingContainer(std::string existingContainerName)
{
	ParticleSystemValidation();
	int activatedParticleCount = 0;

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);
	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;
		activatedParticleCount += foundContainer->DeactivateAllEmitterParticles();
		return MakeComplexString("%i particles deactivated in '%s'", activatedParticleCount, existingContainerName.c_str());
	}

	return MakeComplexString("Container '%s' does not exist", existingContainerName.c_str());	
}
std::string ParticleSystem::DeactivateParticlesInExistingEmitter(std::string existingContainerName, std::string existingEmitterName)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	int activatedParticleCount = 0;
	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;
		activatedParticleCount += foundContainer->DeactivateEmitterParticles(existingEmitterName);

		if (activatedParticleCount > 0)
			return MakeComplexString("%i particles activated in '%s/%s'", activatedParticleCount, existingContainerName.c_str(), existingEmitterName.c_str());
		else
			return MakeComplexString("'%s exists, %s' does not", existingContainerName.c_str(), existingEmitterName.c_str());

	}

	return MakeComplexString("Container '%s' does not exist", existingContainerName.c_str());
}
std::string ParticleSystem::DeactivateAllParticlesInAllContainers()
{
	ParticleSystemValidation();
	int activatedParticleCount = 0;

	for (RunningContainers::iterator iter = s_runningContainers.begin(); iter != s_runningContainers.end(); iter++)
	{
		EmitterContainer* foundContainer = iter->second;

		activatedParticleCount += foundContainer->DeactivateAllEmitterParticles();
	}
	return MakeComplexString("%i particles deactivated in all containers", activatedParticleCount);
}



//===================================================================================================
std::string ParticleSystem::ChangeContainerPosition(std::string existingContainerName, Vector3 newPosition)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;
		foundContainer->ChangeAllRunningEmitterPositions(newPosition);
		return MakeComplexString("'%s' position updated", existingContainerName.c_str());
	}


	return MakeComplexString("Container %s does not exist", existingContainerName.c_str());

}
std::string ParticleSystem::ChangeContainerTextureStill(std::string existingContainerName, std::string newTextureName)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;
		std::string texturePath = "./data/images/" + newTextureName;
		AnimatedTexture* newTexture = AnimatedTexture::CreateOrGetAnimatedTexture(texturePath, 1, 1, 1);

		foundContainer->m_containerAnimatedTexture = newTexture;
		foundContainer->m_xmlContainerDataForExporting.containerTexturePath = texturePath;
		return MakeComplexString("'%s' texture image updated", existingContainerName.c_str());
	}


	return MakeComplexString("Container %s does not exist", existingContainerName.c_str());
}
std::string ParticleSystem::ChangeContainerTextureAnimated(std::string existingContainerName, std::string newTextureName, int totalSprites, int spritesX, int spritesY)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;
		std::string		 texturePath = "./data/images/" + newTextureName;
		AnimatedTexture* newTexture = AnimatedTexture::CreateOrGetAnimatedTexture(texturePath, totalSprites, spritesX, spritesY);

		foundContainer->m_containerAnimatedTexture = newTexture;
		foundContainer->m_xmlContainerDataForExporting.containerTexturePath = texturePath;
		return MakeComplexString("'%s' texture image updated", existingContainerName.c_str());
	}


	return MakeComplexString("Container %s does not exist", existingContainerName.c_str());
}
std::string ParticleSystem::ChangeEmitterTexture(std::string existingContainerName, std::string existingEmitterName, std::string newTextureName, int totalSprites, int spritesX, int spritesY)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;
		
		std::string result = foundContainer->ChangeEmitterTexture(existingEmitterName, newTextureName, totalSprites, spritesX, spritesY);
		
		return result; // MakeComplexString("'%s''%s' texture image updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}



std::string ParticleSystem::ChangePositionX(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangePositionX(existingEmitterName, MinMax);

		return MakeComplexString("'%s''%s' positionX updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}
std::string ParticleSystem::ChangePositionY(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangePositionY(existingEmitterName, MinMax);

		return MakeComplexString("'%s''%s' positionY updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}
std::string ParticleSystem::ChangePositionZ(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangePositionZ(existingEmitterName, MinMax);

		return MakeComplexString("'%s''%s' positionZ updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}

std::string ParticleSystem::ChangeVelocityX(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangeVelocityX(existingEmitterName, MinMax);

		return MakeComplexString("'%s''%s' velocityX updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}
std::string ParticleSystem::ChangeVelocityY(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangeVelocityY(existingEmitterName, MinMax);

		return MakeComplexString("'%s''%s' velocityY updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}
std::string ParticleSystem::ChangeVelocityZ(std::string existingContainerName, std::string existingEmitterName, Vector2 MinMax)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangeVelocityZ(existingEmitterName, MinMax);

		return MakeComplexString("'%s''%s' velocityZ updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}


std::string ParticleSystem::ChangeSpawnRate(std::string existingContainerName, std::string existingEmitterName, int particles, float overTime)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangeSpawnRate(existingEmitterName, particles, overTime);

		return MakeComplexString("'%s''%s' spawnRate updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}
std::string ParticleSystem::ChangeLifeSpan(std::string existingContainerName, std::string existingEmitterName, float newLifeSpan)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangeLifeSpan(existingEmitterName, newLifeSpan);

		return MakeComplexString("'%s''%s' lifetime updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}
std::string ParticleSystem::ChangeColor(std::string existingContainerName, std::string existingEmitterName, Rgba newColor)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		foundContainer->ChangeColor(existingEmitterName, newColor);

		return MakeComplexString("'%s''%s' color updated", existingContainerName.c_str(), existingEmitterName.c_str());
	}


	return MakeComplexString("Emitter %s does not exist", existingEmitterName.c_str());
}

void ParticleSystem::ListAllFactroryContainersToLog()
{
	if (s_containerFactory.size() == 0)
	{
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText("There are no containers loaded in the factory", Rgba::RED);
		return;
	}

	for (ContainerFactory::iterator iter = s_containerFactory.begin(); iter != s_containerFactory.end(); iter++)
	{
		EmitterContainer* foundContainer = iter->second;
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(foundContainer->m_containerName, Rgba::HEALINGBLUE);
	}
	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText("Factory List", Rgba::SUNLIGHTYELLOW);
}
void ParticleSystem::ListAllActiveContainersToLog()
{
	if (s_runningContainers.size() == 0)
	{
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText("There are no active containers in the world", Rgba::RED);
		return;
	}

	for (RunningContainers::iterator iter = s_runningContainers.begin(); iter != s_runningContainers.end(); iter++)
	{
		EmitterContainer* foundContainer = iter->second;
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(foundContainer->m_containerName, Rgba::HEALINGBLUE);
	}
	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText("Active Containers List", Rgba::SUNLIGHTYELLOW);
}
std::string ParticleSystem::ExportContainerToFile(std::string existingContainerName, std::string newContainerName)
{
	ParticleSystemValidation();

	RunningContainers::iterator found = s_runningContainers.find(existingContainerName);

	if (found != s_runningContainers.end())
	{
		EmitterContainer* foundContainer = found->second;

		XMLNode newXMLFile;
		newXMLFile = XMLNode::createXMLTopNode("Project");
		newXMLFile.addAttribute("name", g_projectName.c_str());

		foundContainer->ExportContainerToFile(newXMLFile, newContainerName);
		return MakeComplexString("Current '%s' data exported as '%s'", existingContainerName.c_str(), newContainerName.c_str());
	}
	return MakeComplexString("No running container of that name found");
}







//===================================================================================================
//				Commands																		   ==
//===================================================================================================
void ParticleSystemCommandController(ConsoleCommandArgs& args)
{
	DeveloperLog::GetDeveloperLog()->ClearLog();
	std::string command = args.GetNextString();

	if (command == "help")
		ParticleSystemHelpCommand();

	if (command == "list_active")
		ListActiveContainersCommand();

	if (command == "list_factory")
		ListFactoryContainersCommand();

	if (command == "spawn" || command == "load")
		SpawnFromFactoryCommand(args);

	if (command == "activate" || command == "run")
		ActivateContainerCommand(args);

	if (command == "deactivate" || command == "end")
		DeactivateContainerCommand(args);
	
	if (command == "containerPosition" || command == "containerLocation" || command == "cloc" || command == "cpos")
		ChangeContainerPositionCommand(args);
	
	if (command == "positionx" || command == "posx")
		ChangePositionXCommand(args);
	if (command == "positiony" || command == "posy")
		ChangePositionYCommand(args);
	if (command == "positionz" || command == "posz")
		ChangePositionZCommand(args);

	if (command == "velocityx" || command == "velx")
		ChangeVelocityXCommand(args);
	if (command == "velocityy" || command == "vely")
		ChangeVelocityYCommand(args);
	if (command == "velocityz" || command == "velz")
		ChangeVelocityZCommand(args);

	if (command == "rate")
		ChangeSpawnRateCommand(args);

	if (command == "image" || command == "texture")
		ChangeTextureCommand(args);

	if (command == "export")
		ExportContainerToFileCommand(args);

	if (command == "life")
		ChangeLifeSpanCommand(args);

	if (command == "color")
		ChangeColorCommand(args);

}
void ParticleSystemHelpCommand()
{
	DeveloperLog::GetDeveloperLog()->ClearLog();

	std::string text[20] =
	{
		"Command Structure:",
		"/ps command  ContainerName EmitterName(optional) SubCommand",
		"",
		"Available Commands:", 
		"spawn", 
		"activate/run", 
		"deactivate/end", 
		"containerPosition/containerLocation/Cloc/Cpos",
		"image", 
		"export", 
		"",
		"Example 1:",
		"/ps spawn Fire",
		"Command the particle system to spawn a 'Fire' container from the ContainerFactory using XML/Default settings",
		"",
		"Example 1:",
		"/ps spawn Water 10 10 0",
		"Command the particle system to spawn a 'Water' container from the ContainerFactory at (10.0, 10.0, 0.0)",
		"",
		"/ps COMMAND help  == For specific command help"
	};
	Rgba		color[20] = 
	{
		Rgba::PINK,
		Rgba::HEALINGBLUE,
		Rgba::BLACK,
		Rgba::PINK,
		Rgba::HEALINGBLUE,
		Rgba::HEALINGBLUE,
		Rgba::HEALINGBLUE,
		Rgba::HEALINGBLUE,
		Rgba::HEALINGBLUE,
		Rgba::HEALINGBLUE,
		Rgba::BLACK,
		Rgba::PINK,
		Rgba::HEALINGBLUE,
		Rgba::HEALINGBLUE,
		Rgba::BLACK,
		Rgba::PINK,
		Rgba::HEALINGBLUE,
		Rgba::HEALINGBLUE,
		Rgba::BLACK,
		Rgba::PINK
	};

	for (char index = 0; index < 20; index++)
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(text[index], color[index]);
	
}



//===================================================================================================
void SpawnFromFactoryCommand(ConsoleCommandArgs& args)
{
	std::string result;
	int count = args.GetArgumentCountSpaces();


	//Spawn Container
	if (count == 1) 
	{
		std::string containerName = args.GetNextString();
		if (containerName == "all" || containerName == "All")
			result = g_enhancedParticleSystem->SpawnAllContainersFromFactory();
		else
			result = g_enhancedParticleSystem->SpawnContainerFromFactory(containerName);
	}


	//Spawn Emitter
	if (count == 2) 
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		result = g_enhancedParticleSystem->SpawnEmitterFromFactory(containerName, emitterName);
	}

	
	//Spawn Container at Location
	if (count == 4) 
	{
		std::string containerName = args.GetNextString();
		result = g_enhancedParticleSystem->SpawnContainerFromFactory(containerName);

	
		float xyz[3];
		for (char index = 0; index < 3; index++)
			xyz[index] = args.GetNextFloatSpaced();

		Vector3 newPosition(xyz[0], xyz[1], xyz[2]);


		result = g_enhancedParticleSystem->ChangeContainerPosition(containerName, newPosition);
	}

	
	//Spawn Emitter in Container at Location
	if (count == 5) 
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		result = g_enhancedParticleSystem->SpawnEmitterFromFactory(containerName, emitterName);


		float xyz[3];
		for (char index = 0; index < 3; index++)
			xyz[index] = args.GetNextFloatSpaced();

		Vector3 newPosition(xyz[0], xyz[1], xyz[2]);


		result = g_enhancedParticleSystem->ChangeContainerPosition(containerName, newPosition);
	}


	if (result == "")
		DeveloperLog::GetDeveloperConsole()->ToggleConsoleVisibility();
	else
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}

void ActivateContainerCommand(ConsoleCommandArgs &args)
{
	int count = args.GetArgumentCountSpaces();

	std::string result;
	if(count == 1)
	{
		std::string containerName= args.GetNextString();
		if (containerName == "all" || containerName == "All")
			result = g_enhancedParticleSystem->ActivateAllParticlesInAllContainers();
		else
			result = g_enhancedParticleSystem->ActivateParticlesInExistingContainer(containerName);
	}

	if (count == 2)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		result = g_enhancedParticleSystem->ActivateParticlesInExistingEmitter(containerName, emitterName);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}
void DeactivateContainerCommand(ConsoleCommandArgs& args)
{
	int count = args.GetArgumentCountSpaces();

	std::string result;
	if (count == 1)
	{
		std::string containerName = args.GetNextString();
		if (containerName == "all" || containerName == "All")
			result = g_enhancedParticleSystem->DeactivateAllParticlesInAllContainers();
		else
			result = g_enhancedParticleSystem->DeactivateParticlesInExistingContainer(containerName);
	}

	if (count == 2)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		result = g_enhancedParticleSystem->DeactivateParticlesInExistingEmitter(containerName, emitterName);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}

void ListActiveContainersCommand()
{
	DeveloperLog::GetDeveloperLog()->ClearLog();
	g_enhancedParticleSystem->ListAllActiveContainersToLog();
}
void ListFactoryContainersCommand()
{
	DeveloperLog::GetDeveloperLog()->ClearLog();
	g_enhancedParticleSystem->ListAllFactroryContainersToLog();
}
void ExportContainerToFileCommand(ConsoleCommandArgs& args)
{
	int count = args.GetArgumentCountSpaces();

	std::string result;
	if (count == 2)
	{
		std::string containerName = args.GetNextString();
		std::string newContainerName = args.GetNextString();
		result = g_enhancedParticleSystem->ExportContainerToFile(containerName, newContainerName);
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result, Rgba::HEALINGBLUE);
	}
	else
	{
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText("/ps export SourceContainerName NewContainerName", Rgba::BRIGHTBLUE);
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText("Not enough arguments to export.", Rgba::BRIGHTBLUE);
	}
}

void ChangeContainerPositionCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();
	if (count == 4)
	{
		std::string containerName = args.GetNextString();
		float xyz[3];
		for (char index = 0; index < 3; index++)
			xyz[index] = args.GetNextFloatSpaced();

		Vector3 newPosition(xyz[0], xyz[1], xyz[2]);


		result = g_enhancedParticleSystem->ChangeContainerPosition(containerName, newPosition);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}



//===================================================================================================
void ChangeTextureCommand(ConsoleCommandArgs& args)
{
	std::string result;
	int count = args.GetArgumentCountSpaces();

	//Change Container Image (Static)
	if (count == 2)
	{
		std::string containerName = args.GetNextString();
		std::string imageName = args.GetNextString();

		result = g_enhancedParticleSystem->ChangeContainerTextureStill(containerName, imageName);
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
	}
	//Change Container Image (Animated)
	if (count == 5)
	{
		std::string containerName = args.GetNextString();
		std::string imageName = args.GetNextString();
		//std::string imageName = args.GetNextString();

		int totalSprites = args.GetNextInt();
		int spritesX = args.GetNextIntSpaces();
		int spritesY = args.GetNextIntSpaces();

		result = g_enhancedParticleSystem->ChangeContainerTextureAnimated(containerName, imageName, totalSprites, spritesX, spritesY);
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
	}


	//Change Emitter Image (Static)
	if (count == 3)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		std::string imageName = args.GetNextString();

		result = g_enhancedParticleSystem->ChangeEmitterTexture(containerName, emitterName, imageName, 1, 1, 1);
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
	}
	//Change Emitter Image (Animated)
	if (count == 6)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		std::string imageName = args.GetNextString();

		int totalSprites = args.GetNextIntSpaces();
		int spritesX = args.GetNextIntSpaces();
		int spritesY = args.GetNextIntSpaces();

		result = g_enhancedParticleSystem->ChangeEmitterTexture(containerName, emitterName, imageName, totalSprites, spritesX, spritesY);
		DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
	}
}

void ChangePositionXCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 4)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		float x = args.GetNextFloatSpaced();
		float y = args.GetNextFloatSpaced();

		Vector2 newPosition(x, y);


		result = g_enhancedParticleSystem->ChangePositionX(containerName, emitterName, newPosition);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}
void ChangePositionYCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 4)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		float x = args.GetNextFloatSpaced();
		float y = args.GetNextFloatSpaced();

		Vector2 newPosition(x, y);


		result = g_enhancedParticleSystem->ChangePositionY(containerName, emitterName, newPosition);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}
void ChangePositionZCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 4)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		float x = args.GetNextFloatSpaced();
		float y = args.GetNextFloatSpaced();

		Vector2 newPosition(x, y);


		result = g_enhancedParticleSystem->ChangePositionZ(containerName, emitterName, newPosition);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}

void ChangeVelocityXCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 4)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		float x = args.GetNextFloatSpaced();
		float y = args.GetNextFloatSpaced();

		Vector2 newVelocity(x, y);


		result = g_enhancedParticleSystem->ChangeVelocityX(containerName, emitterName, newVelocity);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}
void ChangeVelocityYCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 4)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		float x = args.GetNextFloatSpaced();
		float y = args.GetNextFloatSpaced();

		Vector2 newVelocity(x, y);


		result = g_enhancedParticleSystem->ChangeVelocityY(containerName, emitterName, newVelocity);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}
void ChangeVelocityZCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 4)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		float x = args.GetNextFloatSpaced();
		float y = args.GetNextFloatSpaced();

		Vector2 newVelocity(x, y);


		result = g_enhancedParticleSystem->ChangeVelocityZ(containerName, emitterName, newVelocity);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}

void ChangeSpawnRateCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 4)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		int particles = args.GetNextFloatSpaced();
		float overTime = args.GetNextFloatSpaced();

		
		result = g_enhancedParticleSystem->ChangeSpawnRate(containerName, emitterName, particles, overTime);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);

}

void ChangeLifeSpanCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 3)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		float		newLifespan = args.GetNextFloatSpaced();


		result = g_enhancedParticleSystem->ChangeLifeSpan(containerName, emitterName, newLifespan);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}
void ChangeColorCommand(ConsoleCommandArgs& args)
{
	std::string result;

	int count = args.GetArgumentCountSpaces();

	if (count == 5)
	{
		std::string containerName = args.GetNextString();
		std::string emitterName = args.GetNextString();
		char		r = args.GetNextFloatSpaced();
		char		g = args.GetNextFloatSpaced();
		char		b = args.GetNextFloatSpaced();

		Rgba newColor(r, g, b, 255);


		result = g_enhancedParticleSystem->ChangeColor(containerName, emitterName, newColor);
	}


	DeveloperLog::GetDeveloperLog()->AddNewInfoLineText(result);
}

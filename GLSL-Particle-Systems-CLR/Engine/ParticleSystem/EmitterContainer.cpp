#include "EmitterContainer.hpp"
#include "ParticleSystem.hpp"
#include "Engine/Renderer/Camera3D.hpp"
vert_t*	vertArray;


//===================================================================================================
//				class ParticleSystem															   ==
//===================================================================================================
void EmitterContainer::LoadEmittersFromXML(XMLNode containerNode)
{
	m_xmlContainerDataForExporting.containerName = m_containerName; //XML Export Container

	const char* particlePosition = containerNode.getAttribute("position");

	//Load Texture
	const char* texturePath = containerNode.getAttribute("texture");
	if (texturePath)
	{
		std::stringstream ss;
		const char* value;

		int countX = 1;
		value = containerNode.getAttribute("spriteCountX");
		if (value)
			ss << value;
		ss >> countX;
		m_xmlContainerDataForExporting.spriteCountX = countX; //XML Export Container
		ss.clear();


		int countY = 1;
		value = containerNode.getAttribute("spriteCountY");
		if (value)
			ss << value;
		ss >> countY;
		m_xmlContainerDataForExporting.spriteCountY = countY; //XML Export Container
		ss.clear();


		int spriteTotal = 1;
		value = containerNode.getAttribute("spriteCountTotal");
		if (value)
			ss << value;
		ss >> spriteTotal;
		m_xmlContainerDataForExporting.spriteCountTotal = spriteTotal; //XML Export Container
		ss.clear();

		m_containerAnimatedTexture = AnimatedTexture::CreateOrGetAnimatedTexture(texturePath, spriteTotal, countX, countY);
		m_xmlContainerDataForExporting.containerTexturePath = texturePath; //XML Export Container
	}

	//Container Loads Emitters
	int emitterCount = containerNode.nChildNode("Emitter");
	for (int emitterIndex = 0; emitterIndex < emitterCount; emitterIndex++)
	{
		XMLNode emitterNode = containerNode.getChildNode(emitterIndex);
		XMLNode basicDataNode = emitterNode.getChildNode("BasicData");
		const char* emitterName = basicDataNode.getAttribute("name");

		ParticleEmitter* newEmitter = CreateNewEmitter(emitterName);

		EmitterXMLData* mydata = new EmitterXMLData();
		m_xmlContainerDataForExporting.xmlEmitterData.push_back(mydata); //XML Export Container

		newEmitter->m_myEmitterDataFromXML = mydata;
		newEmitter->LoadParticlesFromXML(emitterNode);
	}	
	if (particlePosition)
	{
		float position[3];
		std::string vx = particlePosition;
		position[0] = stof(vx.substr(0, vx.find_first_of(",")));
		vx.erase(0, vx.find(",") + 1);
		position[1] = stof(vx.substr(0, vx.find_first_of(",")));
		vx.erase(0, vx.find(",") + 1);
		position[2] = stof(vx.substr(0, vx.find_first_of(",")));
		m_containerPosition = Vector3(position[0], position[1], position[2]);
		ChangeAllFactoryEmitterPositions(m_containerPosition);
		m_xmlContainerDataForExporting.particlePosition = m_containerPosition; //XML Export Container
	}
}
void EmitterContainer::UpdateEmitterContainer(double deltaSeconds)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* particleEmitter = *iter;
		if(particleEmitter->m_emitterActive)
			particleEmitter->UpdateParticleEmitter(deltaSeconds);
	}
}
bool EmitterContainer::Render(MeshRenderer* meshRenderer, std::vector<vert_t>& verts)
{
	int voxelCount = 0;
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		verts.clear();
		ParticleEmitter* particleEmitter = *iter;
		if (particleEmitter->m_emitterActive)
			particleEmitter->GetParticlesForRendering(verts);

		voxelCount = verts.size();
		if (voxelCount != 0)
			vertArray = &verts[0];

		std::string note = MakeComplexString("%s: %i",particleEmitter->m_emitterName.c_str(), voxelCount);
		DeveloperLog::GetInfoTableFont()->AddNewInfoLineText(note, Rgba::SUNLIGHTYELLOW, true);

		//RETURN RenderingRules
		GLSLRenderingRules* rules = nullptr;
		particleEmitter->GetRenderingRules(rules);
		meshRenderer->SetRenderingRules(rules);

		AnimatedTexture* temp = nullptr;
		particleEmitter->GetAnimatedTexture(temp);
		if(temp != nullptr)
			meshRenderer->SetMaterialDiffuseTexture(temp);
		else
			meshRenderer->SetMaterialDiffuseTexture(m_containerAnimatedTexture);

		meshRenderer->UpdateMesh(vertArray, voxelCount);
		meshRenderer->Render(g_masterCamera->m_worldPosition);



	}
	if (voxelCount > 0)
		return true;

	return false;
}



//===================================================================================================
void EmitterContainer::GetEmitterVERT_SForRendering(std::vector<vert_t>& out_vertsOfParticles)
{
	for (EmitterFactory::iterator iter = s_emitterFactory.begin(); iter != s_emitterFactory.end(); iter++)
	{
		ParticleEmitter* particleEmitter = *iter;
		if (particleEmitter->m_emitterActive)
			particleEmitter->GetParticlesForRendering(out_vertsOfParticles);
	}
}
bool EmitterContainer::GetTextureForRendering(Texture*& out_texture)
{
	if (m_containerAnimatedTexture == nullptr)
		return false;

	if (m_containerAnimatedTexture == out_texture)
		return false;

	out_texture = m_containerAnimatedTexture;
		return true;
}



//===================================================================================================
void EmitterContainer::CreateParticlesInEmitterPI(std::string emitterName, int particleCount, ParticleInformation particleInformation)
{
	for (EmitterFactory::iterator found = s_emitterFactory.begin(); found != s_emitterFactory.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;

		if (foundEmitter->m_emitterName == emitterName)
		{
			for (int index = 0; index < particleCount; index++)
				foundEmitter->CreateParticle(particleInformation);
		}
	}
}
void EmitterContainer::CreateParticlesInEmitterXML(std::string emitterName, int particleCount, XMLNode xmlNode)
{
	for (EmitterFactory::iterator found = s_emitterFactory.begin(); found != s_emitterFactory.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;

		if (foundEmitter->m_emitterName == emitterName)
		{
			for (int index = 0; index < particleCount; index++)
				foundEmitter->CreateParticle(xmlNode);
		}
	}
}



//===================================================================================================
ParticleEmitter* EmitterContainer::CreateNewEmitter(std::string emitterName)
{
	ParticleEmitter* newEmitter = new ParticleEmitter();
	newEmitter->m_emitterName = emitterName;
	newEmitter->m_parentContainer = this;
	s_emitterFactory.push_back(newEmitter);

	return newEmitter;
}
std::string		 EmitterContainer::SpawnEmitterFromFactory(std::string existingEmitterName)
{
	for (EmitterFactory::iterator found = s_emitterFactory.begin(); found != s_emitterFactory.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;
		if (foundEmitter->m_emitterName == existingEmitterName)
		{
			m_runningEmitters.push_back(foundEmitter);
		}
	}


	std::string note = MakeComplexString("'%s' does not exist in EmitterFactory", existingEmitterName.c_str());
	LogPrintf("Attempted spawning '%s' from EmitterFactory failed", existingEmitterName.c_str());
	return note;
}



//===================================================================================================
void EmitterContainer::ChangeAllFactoryEmitterPositions(Vector3 newPosition)
{
	Vector3 offset;
	m_containerPosition = newPosition;
	offset -= m_previousContainerPosition;
	offset += m_containerPosition;
	for (EmitterFactory::iterator found = s_emitterFactory.begin(); found != s_emitterFactory.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;
		foundEmitter->ChangeParticlePositions(offset);
	}
}
void EmitterContainer::ChangeAllRunningEmitterPositions(Vector3 newPosition)
{
	Vector3 offset;
	m_previousContainerPosition = m_containerPosition;
	m_containerPosition = newPosition;
	offset -= m_previousContainerPosition;
	offset += m_containerPosition;
	for (RunningEmitters::iterator found = m_runningEmitters.begin(); found != m_runningEmitters.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;
		foundEmitter->ChangeParticlePositions(offset);
	}
}
std::string EmitterContainer::ChangeEmitterTexture(std::string existingEmitterName, std::string newTextureName, int totalSprites, int spritesX, int spritesY)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
		{
			std::string result = emitter->ChangeEmitterTexture(newTextureName, totalSprites, spritesX, spritesY);
			return result;
		}
	}

	return MakeComplexString("Emitter '%s' Not Found", existingEmitterName.c_str());
}



//===================================================================================================
void EmitterContainer::ExportContainerToFile(XMLNode& newXMLFile, std::string newContainerName)
{
	m_xmlContainerDataForExporting.PopulateXMLWithContainerData(newXMLFile, newContainerName);

	for(std::vector<EmitterXMLData*>::iterator iter = m_xmlContainerDataForExporting.xmlEmitterData.begin(); iter != m_xmlContainerDataForExporting.xmlEmitterData.end(); iter++)
	{

		EmitterXMLData* emitterXMLData = *iter;
		emitterXMLData->PopulateXMLWithEmitterData(newXMLFile);
	}
	std::string path = "./data/particles/";
	std::string fullPath = path + newContainerName + ".xml";
	newXMLFile.writeToFile(fullPath.c_str());
}





void EmitterContainer::ChangePositionX(std::string existingEmitterName, Vector2 MinMax)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangePositionX(MinMax);

	}
}
void EmitterContainer::ChangePositionY(std::string existingEmitterName, Vector2 MinMax)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangePositionY(MinMax);

	}
}
void EmitterContainer::ChangePositionZ(std::string existingEmitterName, Vector2 MinMax)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangePositionZ(MinMax);

	}
}


void EmitterContainer::ChangeVelocityX(std::string existingEmitterName, Vector2 MinMax)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangeVelocityX(MinMax);

	}
}
void EmitterContainer::ChangeVelocityY(std::string existingEmitterName, Vector2 MinMax)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangeVelocityY(MinMax);

	}
}
void EmitterContainer::ChangeVelocityZ(std::string existingEmitterName, Vector2 MinMax)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangeVelocityZ(MinMax);

	}
}


void EmitterContainer::ChangeSpawnRate(std::string existingEmitterName, int particles, float overTime)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangeSpawnRate(particles, overTime);
	}
}
void EmitterContainer::ChangeLifeSpan(std::string existingEmitterName, float newLifeSpan)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangeLifeSpan(newLifeSpan);
	}
}

void EmitterContainer::ChangeColor(std::string existingEmitterName, Rgba newColor)
{
	for (RunningEmitters::iterator iter = m_runningEmitters.begin(); iter != m_runningEmitters.end(); iter++)
	{
		ParticleEmitter* emitter = *iter;

		if (emitter->m_emitterName == existingEmitterName)
			emitter->ChangeColor(newColor);
	}
}

//===================================================================================================
int EmitterContainer::ActivateAllEmitterParticles()
{
	int containerParticleCount = 0;
	for (RunningEmitters::iterator found = m_runningEmitters.begin(); found != m_runningEmitters.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;
		containerParticleCount += foundEmitter->ActivateAllParticles();

	}
	return containerParticleCount;
}
int EmitterContainer::DeactivateAllEmitterParticles()
{
	int containerParticleCount = 0;
	for (RunningEmitters::iterator found = m_runningEmitters.begin(); found != m_runningEmitters.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;

		containerParticleCount += foundEmitter->DeactivateAllParticles();
	}
	return containerParticleCount;
}
int EmitterContainer::ActivateEmitterParticles(std::string emitterName)
{
	int containerParticleCount = 0;
	for (RunningEmitters::iterator found = m_runningEmitters.begin(); found != m_runningEmitters.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;
		
		if (foundEmitter->m_emitterName == emitterName)
			containerParticleCount += foundEmitter->ActivateAllParticles();
	}
	return containerParticleCount;
}
int EmitterContainer::DeactivateEmitterParticles(std::string emitterName)
{
	int containerParticleCount = 0;
	for (RunningEmitters::iterator found = m_runningEmitters.begin(); found != m_runningEmitters.end(); found++)
	{
		ParticleEmitter* foundEmitter = *found;

		if (foundEmitter->m_emitterName == emitterName)
			containerParticleCount += foundEmitter->DeactivateAllParticles();
	}
	return containerParticleCount;
}

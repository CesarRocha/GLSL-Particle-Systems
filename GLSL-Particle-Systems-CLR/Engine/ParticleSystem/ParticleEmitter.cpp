#include "ParticleEmitter.hpp"
#include "EmitterContainer.hpp"
int	  initialCount		= 0;
int	  spawnCount		= 0;
float spawnOverTime		= 0.0f;
bool  initialLoop		= false;
bool  spawnRateLoopb		= false;
bool  normalizeVelocity = false;

float positionX[3]		= { 0.0f };
float positionY[3]		= { 0.0f };
float positionZ[3]		= { 0.0f };

float velocityX[3]		= { 0.0f };
float velocityY[3]		= { 0.0f };
float velocityZ[3]		= { 0.0f };

bool  customOrientation = false;
float orientationX[3]	= { 1.0f, 0.0f, 0.0f };
float orientationY[3]	= { 1.0f, 0.0f, 0.0f };
float orientationZ[3]	= { 1.0f, 0.0f, 0.0f };

float size[3]			= { 0.0f };



//===================================================================================================
//					class ParticleEmitter														   ==
//===================================================================================================
ParticleEmitter::ParticleEmitter()
	:m_parentContainer(nullptr)
	, m_emitterActive(false)
	, m_spriteImageXY(IntVector2::ZERO)
	, m_emitterRenderingRules(new GLSLRenderingRules())
	, m_textureIsAnimated(false)
	, m_emitterAnimatedTexture(nullptr)
	, m_myEmitterDataFromXML(new EmitterXMLData())
{
}







//===================================================================================================
void ParticleEmitter::UpdateParticleEmitter(double deltaSeconds)
{
	m_emitterActive = false;
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		EnhancedParticle* particle = *iter;
		if (particle->m_particleInformation.m_particleActive)
		{
			m_emitterActive = TRUE;
			particle->UpdateParticle(deltaSeconds);
		}
	}
}
void ParticleEmitter::GetParticlesForRendering(std::vector<vert_t>& out_vectorOfParticles)
{
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		EnhancedParticle* particle = *iter;

		vert_t vert;
		if(particle->m_particleInformation.m_render == true)
		{
			vert.m_position		= particle->m_particleInformation.m_particlePosition;
			vert.m_orientation	= particle->m_particleInformation.m_particleOrientation;
			vert.m_color		= particle->m_particleInformation.m_particleColor;
			vert.m_voxelSize	= particle->m_particleInformation.m_size;

			//Use parents animated texture math is you do not have own texture
			if(m_emitterAnimatedTexture == nullptr)
			{
				if (m_textureIsAnimated)
				{
					double percentComplete = m_parentContainer->m_containerAnimatedTexture->CalcPercentCompleteTimeAndDuration(particle->m_particleInformationDEFAULT.m_particleLifespan, particle->m_particleInformation.m_particleLifespan);

					Vector2 min;
					Vector2 max;
					m_parentContainer->m_containerAnimatedTexture->CalcTextureCoordinatesAtPercentComplete(1.0 - percentComplete, min, max);

					vert.m_uv = min,
					vert.m_uvMax = max;
				}
				else
				{
					vert.m_uv = m_parentContainer->m_containerAnimatedTexture->GetSpriteCoordsAtXYIndex(m_spriteImageXY.x, m_spriteImageXY.y);
					vert.m_uvMax = vert.m_uv + m_parentContainer->m_containerAnimatedTexture->GetPercentPerSprite();
				}
				out_vectorOfParticles.push_back(vert);
			}
			else //Use own animated texture math is you DO have own texture
			{
				if (m_textureIsAnimated)
				{
					double percentComplete = m_emitterAnimatedTexture->CalcPercentCompleteTimeAndDuration(particle->m_particleInformationDEFAULT.m_particleLifespan, particle->m_particleInformation.m_particleLifespan);

					Vector2 min;
					Vector2 max;
					m_emitterAnimatedTexture->CalcTextureCoordinatesAtPercentComplete(1.0 - percentComplete, min, max);

					vert.m_uv = min,
						vert.m_uvMax = max;
				}
				else
				{
					vert.m_uv = m_emitterAnimatedTexture->GetSpriteCoordsAtXYIndex(m_spriteImageXY.x, m_spriteImageXY.y);
					vert.m_uvMax = vert.m_uv + m_emitterAnimatedTexture->GetPercentPerSprite();
				}
				out_vectorOfParticles.push_back(vert);
			}
		}
	}
}
void ParticleEmitter::GetAnimatedTexture(AnimatedTexture*& out_fromEnitterAnimatedTexture)
{
	if (m_emitterAnimatedTexture != nullptr)
		out_fromEnitterAnimatedTexture = m_emitterAnimatedTexture;
}



//===================================================================================================
void ParticleEmitter::LoadParticlesFromXML(XMLNode emitterNode)
{

	XMLNode basicDataNode		= emitterNode.getChildNode("BasicData");
	XMLNode vertDataNode		= emitterNode.getChildNode("VertData");
	XMLNode spawnRulesNode		= emitterNode.getChildNode("SpawnRules");
	XMLNode renderingRulesNode	= emitterNode.getChildNode("RenderingRules");


	LoadBasicData(basicDataNode);
	LoadVertData(vertDataNode);
	LoadSpawnRulesData(spawnRulesNode);
	LoadRenderingRulesData(renderingRulesNode);



	if(initialLoop)
		m_particleInformationForDefault.m_loop = TRUE;
	for (int particleIndex = 0; particleIndex < initialCount; particleIndex++)
	{
		//Set Velocity
		velocityX[2] = GetRandomFloatInRange(velocityX[0], velocityX[1]);
		velocityY[2] = GetRandomFloatInRange(velocityY[0], velocityY[1]);
		velocityZ[2] = GetRandomFloatInRange(velocityZ[0], velocityZ[1]);
		m_particleInformationForDefault.m_particleVelocity = Vector3(velocityX[2], velocityY[2], velocityZ[2]);


		//Set Position
		positionX[2] = GetRandomFloatInRange(positionX[0], positionX[1]);
		positionY[2] = GetRandomFloatInRange(positionY[0], positionY[1]);
		positionZ[2] = GetRandomFloatInRange(positionZ[0], positionZ[1]);
		m_particleInformationForDefault.m_particlePosition = Vector3(positionX[2], positionY[2], positionZ[2]);

		//Set Orientation
		if(customOrientation)
		{
			orientationX[2] = GetRandomFloatInRange(orientationX[0], orientationX[1]);
			orientationY[2] = GetRandomFloatInRange(orientationY[0], orientationY[1]);
			orientationZ[2] = GetRandomFloatInRange(orientationZ[0], orientationZ[1]);
			Vector3 orientationVector = Vector3(orientationX[2], orientationY[2], orientationZ[2]);
			orientationVector.Normalize();
			m_particleInformationForDefault.m_particleOrientation = orientationVector;
		}
		CreateParticle(m_particleInformationForDefault);
	}
	m_particleInformationForDefault.m_loop = false;


	//Create Spawn Rate Particles
	if (spawnRateLoopb)
		m_particleInformationForDefault.m_loop = TRUE;
	float timeBetweenSpawns = spawnOverTime / spawnCount;
	float delayIncrementer = 0.0f;
	for (int particleIndex = 0; particleIndex < spawnCount; particleIndex++)
	{
		m_particleInformationForDefault.m_initialDelay = delayIncrementer;
		delayIncrementer += timeBetweenSpawns;

		//Set Velocity
		velocityX[2] = GetRandomFloatInRange(velocityX[0], velocityX[1]);
		velocityY[2] = GetRandomFloatInRange(velocityY[0], velocityY[1]);
		velocityZ[2] = GetRandomFloatInRange(velocityZ[0], velocityZ[1]);
			m_particleInformationForDefault.m_particleVelocity = Vector3(velocityX[2], velocityY[2], velocityZ[2]);


		//Set Position
		positionX[2] = GetRandomFloatInRange(positionX[0], positionX[1]);
		positionY[2] = GetRandomFloatInRange(positionY[0], positionY[1]);
		positionZ[2] = GetRandomFloatInRange(positionZ[0], positionZ[1]);
		m_particleInformationForDefault.m_particlePosition = Vector3(positionX[2], positionY[2], positionZ[2]);

		//Set Size
		size[2] = GetRandomFloatInRange(size[0], size[1]);
		m_particleInformationForDefault.m_size = size[2];
		m_myEmitterDataFromXML->m_vertData.size2 = Vector2(size[0], size[1]);

		//Set Orientation
		if (customOrientation)
		{
			orientationX[2] = GetRandomFloatInRange(orientationX[0], orientationX[1]);
			orientationY[2] = GetRandomFloatInRange(orientationY[0], orientationY[1]);
			orientationZ[2] = GetRandomFloatInRange(orientationZ[0], orientationZ[1]);
			Vector3 orientationVector = Vector3(orientationX[2], orientationY[2], orientationZ[2]);
			orientationVector.Normalize();
			m_particleInformationForDefault.m_particleOrientation = orientationVector;
		}
		CreateParticle(m_particleInformationForDefault);
	}


}
void ParticleEmitter::LoadBasicData(XMLNode basicDataNode)
{
	const char* texturePath		= basicDataNode.getAttribute("texture");
	const char* animated		= basicDataNode.getAttribute("animated");
	const char* spriteLocation	= basicDataNode.getAttribute("spriteLocation");

	m_myEmitterDataFromXML->m_basicData.emitterName = m_emitterName;

	//Load Texture
	if (texturePath)
	{
		std::stringstream ss;
		const char* value;

		int countX = 1;
		value = basicDataNode.getAttribute("spriteCountX");
		if (value)
			ss << value;
		ss >> countX;
		m_myEmitterDataFromXML->m_basicData.spriteCountX = countX; //XML Export Emitter
		ss.clear();


		int countY = 1;
		value = basicDataNode.getAttribute("spriteCountY");
		if (value)
			ss << value;
		ss >> countY;
		m_myEmitterDataFromXML->m_basicData.spriteCountY = countY; //XML Export Emitter
		ss.clear();


		int spriteTotal = 1;
		value = basicDataNode.getAttribute("spriteCountTotal");
		if (value)
			ss << value;
		ss >> spriteTotal;
		m_myEmitterDataFromXML->m_basicData.spriteCountTotal = spriteTotal; //XML Export Emitter
		ss.clear();

		m_emitterAnimatedTexture = AnimatedTexture::CreateOrGetAnimatedTexture(texturePath, spriteTotal, countX, countY);
		m_myEmitterDataFromXML->m_basicData.emitterTexturePath = texturePath; //XML Export Emitter
	}

	//Animated
	if (animated)
	{
		if (strcmp(animated, "TRUE") == 0)
		{
			m_textureIsAnimated = true;
			m_myEmitterDataFromXML->m_basicData.emitterAnimated = true; //XML Export Emitter
		}
	}

	//Sprite Location
	if (spriteLocation)
	{
		std::string sl = spriteLocation;
		unsigned char c[2];
		for (int i = 0; i < 2; i++)
		{
			c[i] = stoi(sl.substr(0, sl.find_first_of(",")));
			sl.erase(0, sl.find(",") + 1);
		}
		m_spriteImageXY = IntVector2(c[0], c[1]);
		m_myEmitterDataFromXML->m_basicData.emitterSpriteLocation = m_spriteImageXY; //XML Export Emitter
	}
}
void ParticleEmitter::LoadVertData(XMLNode vertDataNode)
{
	ResetTempData();
	const char* particleSize		 = vertDataNode.getAttribute("size");
	const char* particleColor		 = vertDataNode.getAttribute("color");

	const char* particlePositionX	 = vertDataNode.getAttribute("positionX");
	const char* particlePositionY	 = vertDataNode.getAttribute("positionY");
	const char* particlePositionZ	 = vertDataNode.getAttribute("positionZ");

	const char* particleVelocityX	 = vertDataNode.getAttribute("velocityX");
	const char* particleVelocityY	 = vertDataNode.getAttribute("velocityY");
	const char* particleVelocityZ	 = vertDataNode.getAttribute("velocityZ");

	const char* particleOrientationX = vertDataNode.getAttribute("orientationX");
	const char* particleOrientationY = vertDataNode.getAttribute("orientationY");
	const char* particleOrientationZ = vertDataNode.getAttribute("orientationZ");


	const char* normalize			= vertDataNode.getAttribute("normalizeVelocity");//

	if (normalize)
	{
		if (strcmp(normalize, "true") == 0)
		{
			normalizeVelocity = true;
			m_myEmitterDataFromXML->m_vertData.normalizeVelocity = true; //XML Export Emitter
		}
	}

	//Size
	if (particleSize)
	{
		std::string s = particleSize;
		for (int i = 0; i < 3; i++)
		{
			size[i] = stof(s.substr(0, s.find_first_of("~")));
			s.erase(0, s.find("~") + 1);
		}
		
		//m_particleInformationForDefault.m_size = atof(particleSize);
		//m_myEmitterDataFromXML->m_vertData.size = m_particleInformationForDefault.m_size; //XML Export Emitter
	}
		
	//Color
	if (particleColor)
	{
		std::string cs = particleColor;
		unsigned char c[3];
		for (int i = 0; i < 3; i++)
		{
			c[i] = stoi(cs.substr(0, cs.find_first_of(",")));
			cs.erase(0, cs.find(",") + 1);
		}
		m_particleInformationForDefault.m_particleColor = Rgba(c[0], c[1], c[2]);
		m_myEmitterDataFromXML->m_vertData.color = m_particleInformationForDefault.m_particleColor; //XML Export Emitter
	}

	//Velocity
	if (particleVelocityX)
	{
		std::string vx = particleVelocityX;
		velocityX[0] = stof(vx.substr(0, vx.find_first_of(",")));
		vx.erase(0, vx.find("~") + 1);
		velocityX[1] = stof(vx.substr(0, vx.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.velocityX = Vector2(velocityX[0], velocityX[1]); //XML Export Emitter
	}
	if (particleVelocityY)
	{
		std::string vy = particleVelocityY;
		velocityY[0] = stof(vy.substr(0, vy.find_first_of(",")));
		vy.erase(0, vy.find("~") + 1);
		velocityY[1] = stof(vy.substr(0, vy.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.velocityY = Vector2(velocityY[0], velocityY[1]); //XML Export Emitter

	}
	if (particleVelocityZ)
	{
		std::string vz = particleVelocityZ;
		velocityZ[0] = stof(vz.substr(0, vz.find_first_of(",")));
		vz.erase(0, vz.find("~") + 1);
		velocityZ[1] = stof(vz.substr(0, vz.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.velocityZ = Vector2(velocityZ[0], velocityZ[1]); //XML Export Emitter
	}
	
	//Position
	if (particlePositionX)
	{
		std::string vx = particlePositionX;
		positionX[0] = stof(vx.substr(0, vx.find_first_of(",")));
		vx.erase(0, vx.find("~") + 1);
		positionX[1] = stof(vx.substr(0, vx.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.positionX = Vector2(positionX[0], positionX[1]); //XML Export Emitter
	}
	if (particlePositionY)
	{
		std::string vy = particlePositionY;
		positionY[0] = stof(vy.substr(0, vy.find_first_of(",")));
		vy.erase(0, vy.find("~") + 1);
		positionY[1] = stof(vy.substr(0, vy.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.positionY = Vector2(positionY[0], positionY[1]); //XML Export Emitter
	}
	if (particlePositionZ)
	{
		std::string vz = particlePositionZ;
		positionZ[0] = stof(vz.substr(0, vz.find_first_of(",")));
		vz.erase(0, vz.find("~") + 1);
		positionZ[1] = stof(vz.substr(0, vz.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.positionZ = Vector2(positionZ[0], positionZ[1]); //XML Export Emitter
	}
	
	//Orientation
	m_myEmitterDataFromXML->m_vertData.orientationX = Vector2( m_particleInformationForDefault.m_particleOrientation.x, m_particleInformationForDefault.m_particleOrientation.x);
	if (particleOrientationX)
	{
		customOrientation = true;
		std::string vx = particleOrientationX;
		orientationX[0] = stof(vx.substr(0, vx.find_first_of(",")));
		vx.erase(0, vx.find("~") + 1);
		orientationX[1] = stof(vx.substr(0, vx.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.orientationX = Vector2(orientationX[0], orientationX[1]); //XML Export Emitter
	}
	if (particleOrientationY)
	{
		customOrientation = true;
		std::string vy = particleOrientationY;
		orientationY[0] = stof(vy.substr(0, vy.find_first_of(",")));
		vy.erase(0, vy.find("~") + 1);
		orientationY[1] = stof(vy.substr(0, vy.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.orientationY = Vector2(orientationY[0], orientationY[1]); //XML Export Emitter
	}
	if (particleOrientationZ)
	{
		customOrientation = true;
		std::string vz = particleOrientationZ;
		orientationZ[0] = stof(vz.substr(0, vz.find_first_of(",")));
		vz.erase(0, vz.find("~") + 1);
		orientationZ[1] = stof(vz.substr(0, vz.find_first_of(",")));
		m_myEmitterDataFromXML->m_vertData.orientationZ = Vector2(orientationZ[0], orientationZ[1]); //XML Export Emitter
	}
}
void ParticleEmitter::LoadSpawnRulesData(XMLNode spawnRulesNode)
{
	const char* particleCount		= spawnRulesNode.getAttribute("initialCount");
	const char* particleLoop		= spawnRulesNode.getAttribute("loop");
	const char* particleRenderDelay = spawnRulesNode.getAttribute("renderDelay");//
	const char* particleLifespan	= spawnRulesNode.getAttribute("lifespan");//
	const char* particleFadeTime	= spawnRulesNode.getAttribute("fadeTime");//
	const char* spawnRate			= spawnRulesNode.getAttribute("spawnRate");//
	const char* spawnRateLoop		= spawnRulesNode.getAttribute("spawnRateLoop");//


	initialCount	= 0;
	spawnCount		= 0;
	spawnOverTime	= 0.0f;

	//Initial Count
	if (particleCount)
	{
		initialCount = atoi(particleCount);
		m_myEmitterDataFromXML->m_spawnRules.initialCount = initialCount; //XML Export Emitter
	}

	//Particle Loops?
	if (particleLoop)
	{
		if (strcmp(particleLoop, "TRUE") == 0)
		{
			initialLoop = true;
			m_myEmitterDataFromXML->m_spawnRules.loop = initialLoop; //XML Export Emitter
		}
	}

	//Particle Delay
	if (particleRenderDelay)
	{
		m_particleInformationForDefault.m_initialDelay = atof(particleRenderDelay);
		m_myEmitterDataFromXML->m_spawnRules.renderDelay = m_particleInformationForDefault.m_initialDelay; //XML Export Emitter
	}


	//Lifespan
	if (particleLifespan)
	{
		m_particleInformationForDefault.m_particleLifespan = atof(particleLifespan);
		m_myEmitterDataFromXML->m_spawnRules.lifespan = m_particleInformationForDefault.m_particleLifespan; //XML Export Emitter
	}


	//FadeTime
	if (particleFadeTime)
	{
		m_particleInformationForDefault.m_particleFadeTime = atof(particleFadeTime);
		m_myEmitterDataFromXML->m_spawnRules.fadeTime = m_particleInformationForDefault.m_particleFadeTime; //XML Export Emitter
	}

	//Load Spawn Rate Particles
	if (spawnRate)
	{
		std::string sr = spawnRate;
		spawnCount = stoi(sr.substr(0, sr.find_first_of("~")));
		sr.erase(0, sr.find("~") + 1);
		spawnOverTime = stof(sr.substr(0, sr.find_first_of(",")));
		m_myEmitterDataFromXML->m_spawnRules.spawnRate = Vector2(spawnCount, spawnOverTime); //XML Export Emitter
	}

	//Emitter Load Spawn Rate Particles
	if (spawnRateLoop)
	{
		if (strcmp(spawnRateLoop, "TRUE") == 0)
		{
			spawnRateLoopb = true;
			m_myEmitterDataFromXML->m_spawnRules.spawnRateLoop = true; //XML Export Emitter
		}
	}
}
void ParticleEmitter::LoadRenderingRulesData(XMLNode renderingRulesNode)
{
	const char* glType			= renderingRulesNode.getAttribute("GLType");

	//GL SETTINGS
	if (glType)
	{
		EmitterOpenGLSettings type = LoadOpenGLSettingsByType(glType);
		m_myEmitterDataFromXML->m_renderingRules.GLType = type; //XML Export Emitter
	}
}



//===================================================================================================
void ParticleEmitter::CreateParticle()
{
	EnhancedParticle* newParticle = new EnhancedParticle();
	newParticle->m_parentEmitter = this;
	m_particles.push_back(newParticle);
}
void ParticleEmitter::CreateParticle(ParticleInformation particleInformation)
{
	EnhancedParticle* newParticle = new EnhancedParticle();
	newParticle->m_parentEmitter = this;
	newParticle->CopyParticleInformation(particleInformation);

	m_particles.push_back(newParticle);
}
void ParticleEmitter::CreateParticle(XMLNode xmlNode)
{
	EnhancedParticle* newParticle = new EnhancedParticle();
	newParticle->m_parentEmitter = this;

	m_particles.push_back(newParticle);
}



//===================================================================================================
int  ParticleEmitter::ActivateAllParticles()
{
	int emitterParticleCount = 0;
	m_emitterActive = true;
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		EnhancedParticle* particle = *iter;
		particle->Activate();
		emitterParticleCount++;
	}
	return emitterParticleCount;
}
int  ParticleEmitter::DeactivateAllParticles()
{
	int emitterParticleCount = 0;
	m_emitterActive = true;
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		EnhancedParticle* particle = *iter;
		particle->Deactivate();
		emitterParticleCount++;
	}
	return emitterParticleCount;
}
EmitterOpenGLSettings ParticleEmitter::LoadOpenGLSettingsByType(const char* settingString)
{
	if(strcmp(settingString, "ADDITIVE") == 0)
	{
		m_emitterRenderingRules->m_glDepthTestDisable	= TRUE;
		m_emitterRenderingRules->m_glBlendMode			= GL_BLEND;
		m_emitterRenderingRules->m_glBlendFuncLeft		= GL_SRC_ALPHA;
		m_emitterRenderingRules->m_glBlendFuncRight		= GL_ONE;
		return ADDITIVE;
	}

	if (strcmp(settingString, "NORMAL") == 0)
	{
		m_emitterRenderingRules->m_glDepthTestDisable	= FALSE;
		m_emitterRenderingRules->m_glBlendMode			= GL_BLEND;
		m_emitterRenderingRules->m_glBlendFuncLeft		= GL_SRC_ALPHA,
		m_emitterRenderingRules->m_glBlendFuncRight		= GL_ONE_MINUS_SRC_ALPHA;
		return NORMAL;
	}

	LogPrintf("%s->%s loading an invalid GLSetting string!", m_parentContainer, m_emitterName);
	return INVALID_SETTING;
}



//===================================================================================================
void ParticleEmitter::ChangeParticlePositions(Vector3 positionOffset)
{
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		EnhancedParticle* particle = *iter;
		particle->m_particleInformationDEFAULT.m_particlePosition	+= positionOffset;
		particle->m_particleInformation.m_particlePosition			+= positionOffset;
	}
}
std::string ParticleEmitter::ChangeEmitterTexture(std::string newTextureName, int totalSprites, int spritesX, int spritesY)
{
	std::string texturePath = "./data/images/" + newTextureName;
	bool textureExists = Texture::DoesTextureFileExist(texturePath);
	if (!textureExists)
		return MakeComplexString("'%s' does not exist, wrong extension, case, or name.", newTextureName.c_str());

	AnimatedTexture* newTexture = AnimatedTexture::CreateOrGetAnimatedTexture(texturePath, totalSprites, spritesX, spritesY);


	m_emitterAnimatedTexture = newTexture;
	m_myEmitterDataFromXML->m_basicData.emitterTexturePath = "./data/images/" + newTextureName;
	m_myEmitterDataFromXML->m_basicData.spriteCountTotal = newTexture->GetTotalNumberOfSprites();
	m_myEmitterDataFromXML->m_basicData.spriteCountX = newTexture->GetNumSpritesX();
	m_myEmitterDataFromXML->m_basicData.spriteCountY = newTexture->GetNumSpritesY();

	if (newTexture->GetNumSpritesX() > 1 || newTexture->GetNumSpritesY() > 1 || newTexture->GetTotalNumberOfSprites() > 1)
	{
		m_myEmitterDataFromXML->m_basicData.emitterAnimated = true;
		m_textureIsAnimated = true;
		return "Texture updated (Animated)";
	}
	return "Texture updated (Static)";
}

void ParticleEmitter::ChangePositionX(Vector2 MinMax)
{
	//Set Position
	m_myEmitterDataFromXML->m_vertData.positionX = MinMax;
	
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{

		EnhancedParticle* particle = *iter;
		float positionXf = GetRandomFloatInRange(MinMax.x, MinMax.y);

		particle->m_particleInformationDEFAULT.m_particlePosition.x = positionXf;
		particle->m_particleInformation.m_particlePosition.x = positionXf;
	}
}
void ParticleEmitter::ChangePositionY(Vector2 MinMax)
{
	//Set Position
	m_myEmitterDataFromXML->m_vertData.positionY = MinMax;
	
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{

		EnhancedParticle* particle = *iter;
		float positionZf = GetRandomFloatInRange(MinMax.x, MinMax.y);

		particle->m_particleInformationDEFAULT.m_particlePosition.y = positionZf;
		particle->m_particleInformation.m_particlePosition.y = positionZf;
	}
}
void ParticleEmitter::ChangePositionZ(Vector2 MinMax)
{
	//Set Position
	m_myEmitterDataFromXML->m_vertData.positionZ = MinMax;
	
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{

		EnhancedParticle* particle = *iter;
		float positionZf = GetRandomFloatInRange(MinMax.x, MinMax.y);

		particle->m_particleInformationDEFAULT.m_particlePosition.z = positionZf;
		particle->m_particleInformation.m_particlePosition.z = positionZf;
	}
}

void ParticleEmitter::ChangeVelocityX(Vector2 MinMax)
{
	//Set Position
	m_myEmitterDataFromXML->m_vertData.velocityX = MinMax;

	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{

		EnhancedParticle* particle = *iter;
		float velocityXf = GetRandomFloatInRange(MinMax.x, MinMax.y);

		particle->m_particleInformationDEFAULT.m_particleVelocity.x = velocityXf;
		particle->m_particleInformation.m_particleVelocity.x = velocityXf;
	}
}
void ParticleEmitter::ChangeVelocityY(Vector2 MinMax)
{
	//Set Position
	m_myEmitterDataFromXML->m_vertData.velocityY = MinMax;

	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{

		EnhancedParticle* particle = *iter;
		float velocityYf = GetRandomFloatInRange(MinMax.x, MinMax.y);

		particle->m_particleInformationDEFAULT.m_particleVelocity.y = velocityYf;
		particle->m_particleInformation.m_particleVelocity.y = velocityYf;
	}
}
void ParticleEmitter::ChangeVelocityZ(Vector2 MinMax)
{
	//Set Position
	m_myEmitterDataFromXML->m_vertData.velocityZ = MinMax;

	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{

		EnhancedParticle* particle = *iter;
		float velocityZf = GetRandomFloatInRange(MinMax.x, MinMax.y);

		particle->m_particleInformationDEFAULT.m_particleVelocity.z = velocityZf;
		particle->m_particleInformation.m_particleVelocity.z = velocityZf;
	}
}

void ParticleEmitter::ChangeSpawnRate(int particles, float overTime)
{
	//Set Position
	m_myEmitterDataFromXML->m_spawnRules.spawnRate = Vector2(particles, overTime);
	
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); )
	{
		EnhancedParticle* particle = *iter;
		if (particle->m_particleInformationDEFAULT.m_initialDelay > 0.0f)
			iter = m_particles.erase(iter);
		else
			iter++;
	}

	float timeBetweenSpawns = overTime / particles;
	float delayIncrementer = 0.0f;
	int index = 0;

	bool emitterActive = m_emitterActive;
	m_particles.clear();
	while(index < particles)
	{
			m_particleInformationForDefault.m_initialDelay = delayIncrementer;
			delayIncrementer += timeBetweenSpawns;

			velocityX[0] = m_myEmitterDataFromXML->m_vertData.velocityX.x;
			velocityX[1] = m_myEmitterDataFromXML->m_vertData.velocityX.y;
			velocityY[0] = m_myEmitterDataFromXML->m_vertData.velocityY.x;
			velocityY[1] = m_myEmitterDataFromXML->m_vertData.velocityY.y;
			velocityZ[0] = m_myEmitterDataFromXML->m_vertData.velocityZ.x;
			velocityZ[1] = m_myEmitterDataFromXML->m_vertData.velocityZ.y;


			//Set Velocity
			velocityX[2] = GetRandomFloatInRange(velocityX[0], velocityX[1]);
			velocityY[2] = GetRandomFloatInRange(velocityY[0], velocityY[1]);
			velocityZ[2] = GetRandomFloatInRange(velocityZ[0], velocityZ[1]);
			m_particleInformationForDefault.m_particleVelocity = Vector3(velocityX[2], velocityY[2], velocityZ[2]);

			positionX[0] = m_myEmitterDataFromXML->m_vertData.positionX.x;
			positionX[1] = m_myEmitterDataFromXML->m_vertData.positionX.y;
			positionY[0] = m_myEmitterDataFromXML->m_vertData.positionY.x;
			positionY[1] = m_myEmitterDataFromXML->m_vertData.positionY.y;
			positionZ[0] = m_myEmitterDataFromXML->m_vertData.positionZ.x;
			positionZ[1] = m_myEmitterDataFromXML->m_vertData.positionZ.y;

			//Set Position
			positionX[2] = GetRandomFloatInRange(positionX[0], positionX[1]);
			positionY[2] = GetRandomFloatInRange(positionY[0], positionY[1]);
			positionZ[2] = GetRandomFloatInRange(positionZ[0], positionZ[1]);
			m_particleInformationForDefault.m_particlePosition = Vector3(positionX[2], positionY[2], positionZ[2]);

			orientationX[0] = m_myEmitterDataFromXML->m_vertData.orientationX.x;
			orientationX[1] = m_myEmitterDataFromXML->m_vertData.orientationX.y;
			orientationY[0] = m_myEmitterDataFromXML->m_vertData.orientationY.x;
			orientationY[1] = m_myEmitterDataFromXML->m_vertData.orientationY.y;
			orientationZ[0] = m_myEmitterDataFromXML->m_vertData.orientationZ.x;
			orientationZ[1] = m_myEmitterDataFromXML->m_vertData.orientationZ.y;

			//Set Orientation
			orientationX[2] = GetRandomFloatInRange(orientationX[0], orientationX[1]);
			orientationY[2] = GetRandomFloatInRange(orientationY[0], orientationY[1]);
			orientationZ[2] = GetRandomFloatInRange(orientationZ[0], orientationZ[1]);
			Vector3 orientationVector = Vector3(orientationX[2], orientationY[2], orientationZ[2]);
			orientationVector.Normalize();
			m_particleInformationForDefault.m_particleOrientation = orientationVector;
			
			m_particleInformationForDefault.m_particleActive = emitterActive;

			CreateParticle(m_particleInformationForDefault);
			index++;		
	}
}
void ParticleEmitter::ChangeLifeSpan(float newLifeSpan)
{
	m_myEmitterDataFromXML->m_spawnRules.lifespan = newLifeSpan;
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		EnhancedParticle* particle = *iter;
		particle->m_particleInformationDEFAULT.m_particleLifespan = newLifeSpan;
	}
}
void ParticleEmitter::ChangeColor(Rgba newColor)
{
	m_myEmitterDataFromXML->m_vertData.color = newColor;
	for (std::vector<EnhancedParticle*>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		EnhancedParticle* particle = *iter;
		particle->m_particleInformationDEFAULT.m_particleColor = newColor;
	}
}

//===================================================================================================
void ParticleEmitter::ResetTempData()
{
	//Position
	positionX[0] = 0.0f;
	positionX[1] = 0.0f;
	positionX[2] = 0.0f;
	positionY[0] = 0.0f;
	positionY[1] = 0.0f;
	positionY[2] = 0.0f;
	positionZ[0] = 0.0f;
	positionZ[1] = 0.0f;
	positionZ[2] = 0.0f;

	//Orientation
	customOrientation = false;
	orientationX[0] = 0.0f;
	orientationX[1] = 0.0f;
	orientationX[2] = 1.0f;
	orientationY[0] = 0.0f;
	orientationY[1] = 0.0f;
	orientationY[2] = 1.0f;
	orientationZ[0] = 0.0f;
	orientationZ[1] = 0.0f;
	orientationZ[2] = 1.0f;

	//Velocity
	velocityX[0] = 0.0f;
	velocityX[1] = 0.0f;
	velocityX[2] = 0.0f;
	velocityY[0] = 0.0f;
	velocityY[1] = 0.0f;
	velocityY[2] = 0.0f;
	velocityZ[0] = 0.0f;
	velocityZ[1] = 0.0f;
	velocityZ[2] = 0.0f;

	size[0] = 1.0f;
	size[1] = 1.0f;
	size[2] = 1.0f;
}

#pragma once
#ifndef _ParticleEmitter_
#define _ParticleEmitter_
#include "EnhancedParticle.hpp"
#include "Engine/Renderer/vert_t.hpp"
#include "Engine/Renderer/Shader/MeshRenderer.hpp"
#include <vector>
typedef std::vector<EnhancedParticle*> ParticlesInEmitter;


//===================================================================================================
//				class ParticleEmitter															   ==
//===================================================================================================
class ParticleEmitter
{
public:
	//System
	void UpdateParticleEmitter(double deltaSeconds);
	void GetParticlesForRendering(std::vector<vert_t>& out_vectorOfParticles);
	void GetRenderingRules(GLSLRenderingRules*& out_renderingRules) { out_renderingRules = m_emitterRenderingRules; }
	void GetAnimatedTexture(AnimatedTexture*& out_fromEnitterAnimatedTexture);
	//XML
	void LoadParticlesFromXML(XMLNode emitterNode);
	void LoadBasicData(XMLNode basicDataNode);
	void LoadVertData(XMLNode vertDataNode);
	void LoadSpawnRulesData(XMLNode spawnRulesNode);
	void LoadRenderingRulesData(XMLNode renderingRulesNode);



	//Creation
	void CreateParticle();
	void CreateParticle(ParticleInformation particleInformation);
	void CreateParticle(XMLNode xmlNode);


	//Commands
	int  ActivateAllParticles();
	int  DeactivateAllParticles();
	EmitterOpenGLSettings LoadOpenGLSettingsByType(const char* settingString);


	//Modification
	void ChangeParticlePositions(Vector3 positionOffset);
	std::string ChangeEmitterTexture(std::string newTextureName, int totalSprites = 1, int spritesX = 1, int spritesY = 1);

	void ChangePositionX(Vector2 MinMax);
	void ChangePositionY(Vector2 MinMax);
	void ChangePositionZ(Vector2 MinMax);

	void ChangeVelocityX(Vector2 MinMax);
	void ChangeVelocityY(Vector2 MinMax);
	void ChangeVelocityZ(Vector2 MinMax);

	void ChangeSpawnRate(int particles, float overTime);

	//Other
	void ResetTempData();

private:
	friend class EmitterContainer;
	ParticleEmitter();
	void ChangeLifeSpan(float newLifeSpan);
	void ChangeColor(Rgba newColor);
	//Basic Info
	std::string						m_emitterName;
	bool							m_emitterActive;
	bool							m_textureIsAnimated;
	AnimatedTexture*				m_emitterAnimatedTexture;



	ParticlesInEmitter				m_particles;
	EmitterContainer*				m_parentContainer;
	IntVector2						m_spriteImageXY;
	

	int								m_particleCount;
	ParticleInformation				m_particleInformationForDefault;


	ParticleInformation				m_particleInformationForDefaultSpawnRate;
	int								m_particleCountSpawnRate;
	float							m_rateTime;

	GLSLRenderingRules*				m_emitterRenderingRules;
	EmitterXMLData*					m_myEmitterDataFromXML;
};
#endif // !ParticleEmitter
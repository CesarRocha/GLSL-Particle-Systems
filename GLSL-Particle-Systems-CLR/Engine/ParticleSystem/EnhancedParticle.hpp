#pragma once
#ifndef _EnhancedParticle_
#define _EnhancedParticle_
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Math/3D/Vector3.hpp"
#include "Engine/Utility/Utility.hpp"
#include "ParticleSystemData.hpp"
#include <vector>

//===================================================================================================
//				class EnhancedParticle															   ==
//===================================================================================================
class EnhancedParticle
{
public:


private:
	friend class ParticleEmitter;
	EnhancedParticle();
	EnhancedParticle(Vector3 position, Vector3 orientation, Vector3 velocity, float lifespan, Rgba color);


	void CopyParticleInformation(ParticleInformation particleInformation );


	void Activate();
	void Deactivate();


	void UpdateParticle(double deltaSeconds);
	

	ParticleInformation m_particleInformationDEFAULT;
	ParticleInformation m_particleInformation;
	ParticleEmitter*	m_parentEmitter;
};





















#endif // !EnhancedParticle
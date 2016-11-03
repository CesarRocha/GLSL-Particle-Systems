#pragma once
#ifndef _ParticleInformation_
#define _ParticleInformation_
#include "Engine\Math\3D\Vector3.hpp"
#include "Engine\Renderer\RGBA.hpp"

//===================================================================================================
//				struct ParticleInformation														   ==
//===================================================================================================
struct ParticleInformation
{
public:
	ParticleInformation()
		: m_particleActive(false)
		, m_loop(false)
		, m_initialDelay(0.0f)
		, m_renderDelay(0.0f)
		, m_render(false)
		, m_particleLifespan(0.0f)
		, m_particleFadeTime(0.0f)
		, m_fadeTimeCutter(0)
		, m_size(1.0f)
		, m_particleOrientation(Vector3::XUP)
	{}

	bool	m_particleActive;
	bool	m_loop;
	float	m_initialDelay;
	float	m_renderDelay;
	bool	m_render;
	Vector3 m_particlePosition;
	Vector3 m_particleOrientation;
	Vector3	m_particleVelocity;
	float	m_particleLifespan;
	float	m_particleFadeTime;
	char	m_fadeTimeCutter;
	float	m_size;
	Rgba	m_particleColor;
};


#endif // !_ParticleInformation_
#include "EnhancedParticle.hpp"



//===================================================================================================
//			class EnhancedParticle																   ==
//===================================================================================================
EnhancedParticle::EnhancedParticle()
	:m_parentEmitter(nullptr)
{
}
EnhancedParticle::EnhancedParticle(Vector3 position, Vector3 orientation, Vector3 velocity, float lifespan, Rgba color)
	:m_parentEmitter(nullptr)
{
	m_particleInformation.m_particlePosition	= position;
	m_particleInformation.m_particleOrientation = orientation;
	m_particleInformation.m_particleVelocity	= velocity;
	m_particleInformation.m_particleLifespan	= lifespan;
	m_particleInformation.m_particleColor		= color;
}



//===================================================================================================
void EnhancedParticle::UpdateParticle(double deltaSeconds)
{
	if (m_particleInformation.m_particleActive)
	{
		if (m_particleInformation.m_initialDelay > 0.0f)
		{
			m_particleInformation.m_initialDelay -= deltaSeconds;
			return;
		}
	

		if(m_particleInformation.m_renderDelay > 0.0f)
		{
			m_particleInformation.m_render = FALSE;
			m_particleInformation.m_renderDelay -= deltaSeconds;
			return;
		}

		m_particleInformation.m_render = TRUE;
		m_particleInformation.m_particleLifespan -= (float)deltaSeconds;
		m_particleInformation.m_particlePosition += m_particleInformation.m_particleVelocity;

		if (m_particleInformation.m_particleLifespan <= m_particleInformationDEFAULT.m_particleFadeTime)
		{
			unsigned char fraction = 255 * m_particleInformation.m_particleLifespan / m_particleInformationDEFAULT.m_particleFadeTime;
			m_particleInformation.m_particleColor.a = fraction;
		}
	}


	//Reset to Default XML Settings if not looping
	if (m_particleInformation.m_particleLifespan <= 0.0f)
	{
		if (m_particleInformation.m_loop == FALSE)
		{
			m_particleInformation.m_particleActive = false;
			m_particleInformation.m_renderDelay = m_particleInformationDEFAULT.m_renderDelay;
			m_particleInformation.m_initialDelay = m_particleInformationDEFAULT.m_initialDelay;
		}

		//m_particleInformation.m_render			 = m_particleInformationDEFAULT.m_render;
		m_particleInformation.m_particleLifespan = m_particleInformationDEFAULT.m_particleLifespan;
		m_particleInformation.m_particleVelocity = m_particleInformationDEFAULT.m_particleVelocity;
		m_particleInformation.m_particlePosition = m_particleInformationDEFAULT.m_particlePosition;
		m_particleInformation.m_particleColor	 = m_particleInformationDEFAULT.m_particleColor;
	}




}



//===================================================================================================
void EnhancedParticle::Activate()
{
	m_particleInformation = m_particleInformationDEFAULT;
	m_particleInformation.m_particleActive = true;
}
void EnhancedParticle::Deactivate()
{
	m_particleInformation = m_particleInformationDEFAULT;
	m_particleInformation.m_particleActive = false;
}



//===================================================================================================
void EnhancedParticle::CopyParticleInformation(ParticleInformation particleInformation)
{
	m_particleInformationDEFAULT = particleInformation;
	m_particleInformation		 = particleInformation;
}





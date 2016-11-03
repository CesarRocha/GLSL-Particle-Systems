#include "Sampler.hpp"
SamplerMap Sampler::g_globalSamplers;


//===================================================================================================
//               class Sampler	                                                                   ==
//===================================================================================================
Sampler::Sampler()
	:m_diffuseMap(nullptr)
	, m_normalMap(nullptr)
	, m_specMap(nullptr)
	, m_emissiveMap(nullptr)
	, m_specularGlossEmisiveMap(nullptr)
{
	CreateSampler(GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT);
};


//===================================================================================================
GLuint Sampler::CreateSampler(GLenum min_filter, GLenum mag_filter, GLenum u_wrap, GLenum v_wrap)
{

	GLuint sampler_id;
	glGenSamplers(1, &sampler_id);
	if (sampler_id == NULL)
		return NULL;

	glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, min_filter);
	glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, mag_filter);
	glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, u_wrap);
	glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, v_wrap);

	m_samplerID = sampler_id;
	return sampler_id;

}


//===================================================================================================
void Sampler::DestroySampler(GLuint sampler_id)
{
	glDeleteSamplers(1, &sampler_id);
}


//===================================================================================================
Sampler* Sampler::CreateOrGetSampler(const std::string samplerName)
{
	SamplerMap::iterator samplerIter = g_globalSamplers.find(samplerName);
	if (samplerIter != g_globalSamplers.end())
		return samplerIter->second;


	Sampler* newSampler = new Sampler();
	g_globalSamplers[samplerName] = newSampler;
	return newSampler;
}



#pragma once
#ifndef _INCLUDED_MATERIAL_
#define _INCLUDED_MATERIAL_
#include "Engine/Renderer/Lighting/LightingProperties.hpp"
#include "MaterialDependants/Shader.hpp"
#include "MaterialDependants/Sampler.hpp"


class Material;
typedef std::map< std::string, Material* >	MaterialMap;
//===================================================================================================
//               class Material	                                                                   ==
//===================================================================================================
class Material
{
public:
	Material();
	Material(Sampler& samplerID);

	static Material* CreateOrGetMaterial(const std::string materialName, Sampler* sampler = nullptr);


	void	SetTexture(const char* name, Texture* texture, GLuint tex_index );
	void	SetMatrix(const char* name, const Matrix4x4 matrix);
	bool	ProgramBindMatrix( GLuint program_ID, const char* name, const Matrix4x4& matrix);
	bool	ProgramBindSamplerIndex( GLuint program_ID, const char* name, GLint index);
	void	GLCheckError(const std::string& file, int line);
	GLuint	ProgramFindAndCheck( GLuint program_ID, const char* name, GLenum type);




	std::string			m_materialName;
	Sampler				m_sampler;

	LightProperties		m_lightArray[16];

	char				m_lightCount;
	Rgba				m_materialAmbientColor;
	float				m_materialAmbientPower;
	float				m_shine;
private:

	static MaterialMap	g_globalMaterials;
};

#endif


//LightProperties	 GetLightPropertyAtIndex(char index){ return m_lightArray[index]; }
//LightProperties  GetLightProperty()					{ return m_light; }
//char	GetLightCount()								{ return m_lightCount; }
//Rgba	GetAmbientLightColor()						{ return m_materialAmbientLightColor;}
//float	GetAmbientLightPower()						{ return m_materialAmbientLightPower; }
//float	GetShine()									{ return m_shine; }
//
//void	SetLightCount(char count)					{ m_lightCount = count; }
//void	SetAmbientLightColor(Rgba ambient)			{ m_materialAmbientLightColor = ambient;} 
//void	SetAmbientLightPower(float power)			{ m_materialAmbientLightPower = power;} 
//void	SetShine(float shine)						{ m_shine = shine; }


//void	IncrementLightCount() { m_lightCount++; }
//void	DecrementLightCount() { m_lightCount--; }
#pragma once
#ifndef _INCLUDE_MESHRENDERER_
#define _INCLUDE_MESHRENDERER_
#include "Engine/Renderer/Shader/Material.hpp"
#include "Engine/Renderer/Shader/Mesh.hpp"
#include <string>
class MeshRenderer;
typedef std::map<std::string, MeshRenderer*> MeshRendererMap; 


struct GLSLRenderingRules
{
	GLSLRenderingRules()
		: m_glDepthTestDisable(false)
		, m_glBlendMode(GL_BLEND) 
		, m_glBlendFuncLeft(GL_SRC_ALPHA)
		, m_glBlendFuncRight(GL_ONE_MINUS_SRC_ALPHA)
	{}


	GLenum							m_glBlendMode;
	GLenum							m_glBlendFuncLeft;
	GLenum							m_glBlendFuncRight;
	bool							m_glDepthTestDisable;
};
//===================================================================================================
//               class MeshRenderer                                                                ==
//===================================================================================================
class MeshRenderer
{
public:
	MeshRenderer();
	static		MeshRenderer* CreateOrGetMeshRenderer(std::string name, Mesh* mesh = nullptr, Material* material = nullptr, Shader* shader = nullptr);
	static		MeshRenderer* CreateOrGetMeshRenderer(std::string meshRendererName, Mesh* mesh, std::string shaderFragVertName, BufferType bufferType, std::string = "");



	void		SetRenderingRules(GLSLRenderingRules* newRules)		{ m_renderingRules = newRules; }
	void		Render(const Matrix4x4& view, const Matrix4x4& projection, const Matrix4x4& model, const Vector3& cameraPosition = Vector3::ZERO, const double& deltaSeconds = 0);
	void		Render(const Vector3& cameraPosition = Vector3::ZERO, const double& deltaSeconds = 0);


	//Primary
	Shader&		GetShader()											{ return m_shader; }
	Mesh&		GetMesh()											{ return m_mesh; }
	Material&	GetMaterial()										{ return m_material; }

	//Fog
	void		SetFogStart(float start)							{ m_fogStartDistance = start; }
	void		SetFogEnd(float end)								{ m_fogEndDistance = end; }
	void		SetFogColorAndIntensity(Rgba fogColorAndIntensity)	{ m_fogColorAndIntensity = fogColorAndIntensity; }
	float		GetFogStart()										{ return m_fogStartDistance; }
	float		GetFogEnd()											{ return m_fogEndDistance; }
	Rgba		GetFogColorAndIntensity()							{ return m_fogColorAndIntensity; }

	//Material
	void		SetMaterialDiffuseTexture(Texture* newDiffuseTexture);
	void		SetMaterialNormalTexture(Texture* newNormalMap)		{ m_material.m_sampler.SetNormal(newNormalMap); }
	void		SetMaterialAmbientColor(Rgba color)					{ m_material.m_materialAmbientColor = color; }
	void		SetMaterialAmbientPower(float power)				{ m_material.m_materialAmbientPower = power; }
	void		SetMaterialShine(float shine);
	Rgba		GetMaterialAmbientColor()							{ return m_material.m_materialAmbientColor; }
	float		GetMaterialAmbientPower()							{ return m_material.m_materialAmbientPower; }
	float		GetMaterialShine()									{ return m_material.m_shine; }


	//Mesh
	void	UpdateMaterial(Texture* newTexture);
	void		UpdateMesh(const void* meshData, size_t size);


	//Lights
	void		SetlightCount(char count) { m_material.m_lightCount = count; }
	void		SetLightPositionAtIndex(char index, Vector3 position);
	Vector3		GetLightPositionAtIndex(char index);


	//Time
	void		ResetTime()											{ m_elapsedTime = 0.0f; }
	void		SetTimeScale(float scale)							{ m_timeScale = scale; }

	
private:
	friend class ModelLoader;


	MeshRenderer(Mesh mesh, Material material, Shader shader);
	GLuint		CreateVAO(GLuint buffer_ID);
	void		DestroyVAO(GLuint vao_ID);
	GLuint		CreateIBO(GLuint buffer_ID);
	void		DestroyIBO(GLuint ibo_ID);

	
	bool		ProgramBindAttribute(GLuint program_ID, const char* inName, GLint count, GLenum shader_type, GLboolean normalize, GLsizei stride, GLsizei offset);
	void		ProgramBind( GLuint program_ID );
	bool		ProgramBindFloat( GLuint program_ID, const char* name, const float& val);
	bool		ProgramBindInt(GLuint program_ID, const char * name, const int& val);
	bool		ProgramBindVec2(GLuint program_ID, const char* name, const Vector2& val);
	bool		ProgramBindVec3( GLuint program_ID, const char* name, const Vector3& val);
	bool		ProgramBindVec4( GLuint program_ID, const char* name, const Vector4& val);
	bool		ProgramBindMatrix( GLuint program_ID, const char* name, const Matrix4x4& matrix);
	bool		ProgramBindSamplerIndex( GLuint program_ID, const char* name, GLint index);
	void		GLCheckError(const std::string& file, int line);
	GLuint		ProgramFindAndCheck( GLuint program_ID, const char* name, GLenum type);
	void		BindLightSources();
	void		BindDiffuseMap();
	void		BindNormalMap();
	void		BindSpecularMap();

	GLSLRenderingRules*			m_renderingRules;

	Shader						m_shader;
	Mesh						m_mesh;
	Material					m_material;
	static 		MeshRendererMap	s_meshRendererMap;
	float						m_fogStartDistance;
	float						m_fogEndDistance;
	Rgba						m_fogColorAndIntensity;
	float						m_elapsedTime;
	float						m_timeScale;

	GLuint						m_VBO;
	GLuint						m_IBO;
};



#endif

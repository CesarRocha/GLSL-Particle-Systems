#include "Engine/Renderer/Shader/MeshRenderer.hpp"
#include "Engine/Renderer/ModelLoader/ModelLoader.hpp"
#include "Engine/Renderer/Camera3D.hpp"
MeshRendererMap	MeshRenderer::s_meshRendererMap;



//===================================================================================================
//               class MeshRenderer                                                                ==
//===================================================================================================
MeshRenderer::MeshRenderer()
	:m_fogStartDistance(100)
	, m_fogEndDistance(200)
	, m_fogColorAndIntensity(Rgba::BLACK)
	, m_VBO(NULL)
	, m_IBO(NULL)
	, m_elapsedTime(0.0)
	, m_timeScale(1.0f)
	, m_renderingRules(nullptr)
{
}
MeshRenderer::MeshRenderer(Mesh mesh, Material material, Shader shader)
	: m_fogStartDistance(10)
	, m_fogEndDistance(20)
	, m_fogColorAndIntensity(Rgba::BLACK)
	, m_VBO(NULL)
	, m_IBO(NULL)
	, m_elapsedTime(0.0)
	, m_timeScale(1.0f)
	, m_renderingRules(nullptr)
{
	m_mesh = mesh;
	GLCheckError(__FILE__, __LINE__ - 1);

	m_material = material;
	GLCheckError(__FILE__, __LINE__ - 1);

	m_shader = shader;
	GLCheckError(__FILE__, __LINE__ - 1);

	m_VBO = CreateVAO(m_mesh.m_vboID);
	if (shader.m_bufferType == IBO)
		m_IBO = CreateIBO(m_mesh.m_vboID);

}



//===================================================================================================
MeshRenderer* MeshRenderer::CreateOrGetMeshRenderer(std::string name, Mesh* mesh, Material* material, Shader* shader)
{
	MeshRendererMap::iterator found = s_meshRendererMap.find(name);
	if (found != s_meshRendererMap.end())
		return found->second;

	if (mesh == nullptr, material == nullptr, shader == nullptr)
	{
		std::string error = name + " >> does not exist and received a nullptr for construction";
		MessageBoxA(NULL, error.c_str(), "Create MeshRenderer Error", MB_OK);
		return nullptr;
	}

	MeshRenderer* newMeshRenderer = new MeshRenderer(*mesh, *material, *shader);
	s_meshRendererMap[name] = newMeshRenderer;
	return newMeshRenderer;
}
MeshRenderer* MeshRenderer::CreateOrGetMeshRenderer(std::string meshRendererName, Mesh* mesh, std::string shaderFragVertName, BufferType bufferType, std::string specifiedGeometryShaderIfNeeded)
{
	MeshRendererMap::iterator found = s_meshRendererMap.find(meshRendererName);
	if (found != s_meshRendererMap.end())
		return found->second;

	std::string samplerName			= meshRendererName + "Sampler";
	std::string shaderFileNames		= "shaders/" + shaderFragVertName;
	std::string shaderNameExtend	= meshRendererName + "Shader";
	std::string materialName		= meshRendererName + "Material";

	Shader* shader = nullptr;

	if (bufferType == GEOPOINT || bufferType == GEOTRIANGLE)
	{

		if (specifiedGeometryShaderIfNeeded != "")
		{
			std::string geometryShaderFileName = "shaders/" + specifiedGeometryShaderIfNeeded;
			shader = Shader::CreateOrGetShader(shaderNameExtend, shaderFileNames, geometryShaderFileName, shaderFileNames, bufferType);
		}
		else
			shader = Shader::CreateOrGetShader(shaderNameExtend, shaderFileNames, shaderFileNames, shaderFileNames, bufferType);
	}
	else
		shader = Shader::CreateOrGetShader(shaderNameExtend, shaderFileNames, shaderFileNames, bufferType);



	Sampler*	sampler		= Sampler::CreateOrGetSampler(samplerName);
	Material*	material	= Material::CreateOrGetMaterial(materialName, sampler);



	MeshRenderer* newMeshRenderer = new MeshRenderer(*mesh, *material, *shader);
	s_meshRendererMap[meshRendererName] = newMeshRenderer;
	return newMeshRenderer;
}



//===================================================================================================
void	MeshRenderer::UpdateMesh(const void* meshData, size_t size)
{
	m_mesh.UpdateMesh(meshData, size);
}



//===================================================================================================
void	MeshRenderer::Render(const Matrix4x4& view, const Matrix4x4& projection, const Matrix4x4& model, const Vector3& cameraPosition, const double& deltaSeconds )
{
	if (m_renderingRules != nullptr)
	{
		if (m_renderingRules->m_glDepthTestDisable)
			glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, m_renderingRules->m_glBlendFuncRight);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	m_renderingRules = nullptr;
	bool bindResult;

	
	ProgramBind(m_shader.m_programID);
	glBindVertexArray(m_VBO);
	if (m_shader.m_bufferType == IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	
	
	//Time
	m_elapsedTime += (float)deltaSeconds * m_timeScale;
	bindResult = ProgramBindFloat(  m_shader.m_programID, "gTime",						m_elapsedTime);
	
	
	//MVP
	bindResult = ProgramBindMatrix( m_shader.m_programID, "gModel",						model);
	bindResult = ProgramBindMatrix( m_shader.m_programID, "gView",						view);
	bindResult = ProgramBindMatrix( m_shader.m_programID, "gProj",						projection);
	bindResult = ProgramBindVec3(   m_shader.m_programID, "gCameraPosition",			cameraPosition);


	//Light
	bindResult = ProgramBindInt(	m_shader.m_programID, "gLightCount",				m_material.m_lightCount);
	BindLightSources();
	bindResult = ProgramBindVec4(   m_shader.m_programID, "gMaterialAmbientColor",		Rgba::RgbaToVector4(m_material.m_materialAmbientColor));
	bindResult = ProgramBindFloat(	m_shader.m_programID, "gMaterialAmbientPower",		m_material.m_materialAmbientPower);
	bindResult = ProgramBindFloat(	m_shader.m_programID, "gMaterialShininess",			m_material.m_shine);
	
	
	//Fog
	bindResult = ProgramBindFloat(  m_shader.m_programID, "gFogStart",					m_fogStartDistance );
	bindResult = ProgramBindFloat(  m_shader.m_programID, "gFogEnd",					m_fogEndDistance );
	bindResult = ProgramBindVec3(	m_shader.m_programID, "gFogColor",					Rgba::RgbaToVector3(m_fogColorAndIntensity));


	//Textures
	BindDiffuseMap();
	BindNormalMap();
	BindSpecularMap();
	
	
	switch (m_shader.m_bufferType)
	{
	case GEOPOINT:
		glDrawArrays(GL_POINTS, 0, m_mesh.m_vboVertCount);
		GLCheckError(__FILE__, __LINE__ - 1);
		break;
	case GEOTRIANGLE:
		glDrawArrays(GL_TRIANGLES, 0, m_mesh.m_vboVertCount);
		GLCheckError(__FILE__, __LINE__ - 1);
		break;
	case VBO:
		glDrawArrays(GL_TRIANGLES, 0, m_mesh.m_vboVertCount);
		GLCheckError(__FILE__, __LINE__ - 1);
		break;
	case IBO:
		glDrawElements(GL_TRIANGLES, m_mesh.m_iboIndexCount, GL_UNSIGNED_INT, nullptr);
		GLCheckError(__FILE__, __LINE__ - 1);
		break;
	}

	glBindVertexArray( NULL );
	glUseProgram(NULL);
	GLCheckError( __FILE__,  __LINE__ -1);

}
void	MeshRenderer::Render(const Vector3& cameraPosition, const double& deltaSeconds)
{
	if (m_renderingRules != nullptr)
	{
		if (m_renderingRules->m_glDepthTestDisable)
			glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, m_renderingRules->m_glBlendFuncRight);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	m_renderingRules = nullptr;


	bool bindResult;

	ProgramBind(m_shader.m_programID);
	glBindVertexArray(m_VBO);
	if (m_shader.m_bufferType == IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);


	//Time
	m_elapsedTime += (float)deltaSeconds * m_timeScale;
	bindResult = ProgramBindFloat( m_shader.m_programID,	"gTime", m_elapsedTime);
	bindResult = ProgramBindInt(   m_shader.m_programID,	"gPolygonMode", g_polygonLines);

	//MVP
	bindResult = ProgramBindMatrix(m_shader.m_programID,	"gModel", Matrix4x4());
	bindResult = ProgramBindMatrix(m_shader.m_programID,	"gView", g_masterCamera->m_viewMatrix);
	bindResult = ProgramBindMatrix(m_shader.m_programID,	"gProj", g_masterCamera->m_projPersMatrix);
	bindResult = ProgramBindVec3(  m_shader.m_programID,	"gCameraPosition", cameraPosition);


	//Light
	BindLightSources();
	bindResult = ProgramBindVec4( m_shader.m_programID,		"gMaterialAmbientColor", Rgba::RgbaToVector4(m_material.m_materialAmbientColor));
	bindResult = ProgramBindFloat(m_shader.m_programID,		"gMaterialAmbientPower", m_material.m_materialAmbientPower);
	bindResult = ProgramBindFloat(m_shader.m_programID,		"gMaterialShininess", m_material.m_shine);


	//Fog
	bindResult = ProgramBindFloat(m_shader.m_programID,		"gFogStart", m_fogStartDistance);
	bindResult = ProgramBindFloat(m_shader.m_programID,		"gFogEnd", m_fogEndDistance);
	bindResult = ProgramBindVec3( m_shader.m_programID,		"gFogColor", Rgba::RgbaToVector3(m_fogColorAndIntensity));


	//Textures
	BindDiffuseMap();
	BindNormalMap();
	BindSpecularMap();


	switch (m_shader.m_bufferType)
	{
	case GEOPOINT:
		glDrawArrays(GL_POINTS, 0, m_mesh.m_vboVertCount);
		GLCheckError(__FILE__, __LINE__ - 1);
		break;
	case GEOTRIANGLE:
		glDrawArrays(GL_TRIANGLES, 0, m_mesh.m_vboVertCount);
		GLCheckError(__FILE__, __LINE__ - 1);
		break;
	case VBO:
		glDrawArrays(GL_TRIANGLES, 0, m_mesh.m_vboVertCount);
		GLCheckError(__FILE__, __LINE__ - 1);
		break;
	case IBO:
		glDrawElements(GL_TRIANGLES, m_mesh.m_iboIndexCount, GL_UNSIGNED_INT, nullptr);
		GLCheckError(__FILE__, __LINE__ - 1);
		break;
	}

	glBindVertexArray(NULL);
	glUseProgram(NULL);
	GLCheckError(__FILE__, __LINE__ - 1);
}




//===================================================================================================
void MeshRenderer::SetMaterialDiffuseTexture(Texture* newDiffuseTexture)
{
	m_material.m_sampler.m_diffuseMap = newDiffuseTexture;
}




//===================================================================================================
GLuint	MeshRenderer::CreateVAO(GLuint buffer_ID)
{
	GLuint vao_ID;
	glGenVertexArrays( 1, &vao_ID );
	if( vao_ID == 0 )
		return 0;

	GLint max_attributes;
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &max_attributes );
	glBindVertexArray( vao_ID );
	glBindBuffer( GL_ARRAY_BUFFER, buffer_ID );
	GLCheckError(__FILE__,  __LINE__ -1);
	
	//Query Here
	GLint query;
	GLint size;
	char buffer[128];
	GLsizei length;
	GLenum type;

	glGetProgramiv(m_shader.m_programID, GL_ACTIVE_ATTRIBUTES, &query);
	for (int index = 0; index < query; index++)
	{
		glGetActiveAttrib(m_shader.m_programID, index, 128, &length, &size, &type, buffer);
	}
			
	bool result;
	result = ProgramBindAttribute( m_shader.m_programID, "inPosition",	3, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_position));
	result = ProgramBindAttribute( m_shader.m_programID, "inOrientation", 3, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_orientation));
	result = ProgramBindAttribute( m_shader.m_programID, "inColor",		4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(vert_t), offsetof(vert_t, m_color));
	result = ProgramBindAttribute( m_shader.m_programID, "inUV0",		2, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_uv));
	result = ProgramBindAttribute(m_shader.m_programID,  "inSize",		1, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_voxelSize));


	result = ProgramBindAttribute( m_shader.m_programID, "inNormal",	3, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_norm));
	result = ProgramBindAttribute( m_shader.m_programID, "inTangent",	3, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_tang));
	result = ProgramBindAttribute( m_shader.m_programID, "inBiTangent", 3, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_bitang));

	result = ProgramBindAttribute( m_shader.m_programID, "inUV0Max",	2, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_uvMax));	
	result = ProgramBindAttribute( m_shader.m_programID, "inUV02",		2, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_uv2));
	result = ProgramBindAttribute( m_shader.m_programID, "inUV03",		2, GL_FLOAT,		 GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_uv3));


	GLCheckError(__FILE__,  __LINE__ -1);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	return vao_ID;
}
void	MeshRenderer::DestroyVAO(GLuint vao_ID)
{
	if( vao_ID != NULL)
		glDeleteVertexArrays( 1, &vao_ID );
}
GLuint	MeshRenderer::CreateIBO(GLuint buffer_ID)
{
	GLuint ibo_ID;
	glGenVertexArrays( 1, &ibo_ID );
	if( ibo_ID == NULL ){
		assert(0);
		return NULL;
	}
	GLCheckError(__FILE__,  __LINE__ -1);

	GLint max_attributes;
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &max_attributes );
	glBindVertexArray( ibo_ID );
	glBindBuffer( GL_ARRAY_BUFFER, buffer_ID );
	GLCheckError(__FILE__,  __LINE__ -1);


	bool result;
	result = ProgramBindAttribute(m_shader.m_programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_position));
	result = ProgramBindAttribute(m_shader.m_programID, "inUV0", 2, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_uv));
	result = ProgramBindAttribute(m_shader.m_programID, "inUV02", 2, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_uv2));
	result = ProgramBindAttribute(m_shader.m_programID, "inUV03", 2, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_uv3));
	result = ProgramBindAttribute(m_shader.m_programID, "inNormal", 3, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_norm));
	result = ProgramBindAttribute(m_shader.m_programID, "inTangent", 3, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_tang));
	result = ProgramBindAttribute(m_shader.m_programID, "inBiTangent", 3, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_bitang));
	result = ProgramBindAttribute(m_shader.m_programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vert_t), offsetof(vert_t, m_color));
	GLCheckError(__FILE__, __LINE__ - 1);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	return ibo_ID;
}
void	MeshRenderer::DestroyIBO(GLuint ibo_ID)
{
	if( ibo_ID != NULL)
		glDeleteVertexArrays( 1, &ibo_ID );
}



//===================================================================================================
bool	MeshRenderer::ProgramBindAttribute(GLuint program_ID, const char* inName, GLint count, GLenum shader_type, GLboolean normalize, GLsizei stride, GLsizei offset)
{
	GLint loc = glGetAttribLocation( program_ID, inName );
	GLCheckError(__FILE__,  __LINE__ -1);

	if( loc < 0 )
		return false;

	glEnableVertexAttribArray( loc );
	GLCheckError(__FILE__,  __LINE__ -1);

	glVertexAttribPointer( loc, count, shader_type, normalize, stride, (GLvoid*) offset );
	GLCheckError(__FILE__,  __LINE__ -1);

	return true;
}
void	MeshRenderer::ProgramBind( GLuint program_ID )
{
	glUseProgram(program_ID);
}
bool	MeshRenderer::ProgramBindFloat( GLuint program_ID, const char* name, const float& val)
{
	GLint loc = ProgramFindAndCheck( program_ID, name, GL_FLOAT);
	if(loc < 0)
		return false;

	glUniform1fv( loc, 1, &val);
	return true;
}
bool	MeshRenderer::ProgramBindInt(GLuint program_ID, const char* name, const int& val)
{
	GLint loc = ProgramFindAndCheck(program_ID, name, GL_INT);
	if (loc < 0)
		return false;

	glUniform1iv(loc, 1, &val);
	return true;
}
bool	MeshRenderer::ProgramBindMatrix( GLuint program_ID, const char* name, const Matrix4x4& matrix)
{
	GLint loc = glGetUniformLocation( program_ID, name );
	if( loc < 0 ) 
		return false;

	GLuint index;
	glGetUniformIndices( program_ID, 1, &name, &index );
	if( index >= 0 )
	{
		GLint count;
		GLenum type;
		glGetActiveUniform( program_ID, index, 0, NULL, &count, &type, NULL);
		assert( type == GL_FLOAT_MAT4 );
	}

	glUniformMatrix4fv( loc, 1, GL_FALSE, (GLfloat*)&matrix);////
	return true;
}
bool	MeshRenderer::ProgramBindSamplerIndex( GLuint program_ID, const char* name, GLint index)
{
	GLint loc = ProgramFindAndCheck( program_ID, name, GL_SAMPLER_2D );
	if(loc < 0 )
		return false;

	glUniform1iv( loc, 1, (GLint*)&index);
	return true;
}
bool	MeshRenderer::ProgramBindVec3( GLuint program_ID, const char* name, const Vector3& val)
{
	GLint loc = glGetUniformLocation( program_ID, name );
	if( loc < 0 ) 
		return false;

	glUniform3f( loc, val.x, val.y, val.z);
	return true;
}
bool	MeshRenderer::ProgramBindVec4( GLuint program_ID, const char* name, const Vector4& val)
{
	GLint loc = glGetUniformLocation( program_ID, name );
	if( loc < 0 ) 
		return false;

	glUniform4f( loc, val.x, val.y, val.z, val.w);
	return true;
}
GLuint	MeshRenderer::ProgramFindAndCheck( GLuint program_ID, const char* name, GLenum type)
{
	GLint loc = glGetUniformLocation( program_ID, name);
	if(loc < 0)
		return loc;

	GLuint index; 
	glGetUniformIndices( program_ID, 1, &name, &index );
	if(index >= 0)
	{
		GLint count;
		GLenum check_type;
		glGetActiveUniform( program_ID, index, 0, NULL, &count, &check_type, NULL);
		assert(type == check_type);
		if(type == check_type)
			return loc;
	}
	GLint i = -1;
	return  i;
}
void	MeshRenderer::GLCheckError(const std::string& file, int line)
{
#if defined(OPENGL_CHECK_ERRORS)
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
		const GLubyte* errString;
		errString = gluErrorString(error);
		std::stringstream out;
		out << line;
		std::string header = "Shader Error";

		std::string message = file + "\n" + "Line: " + out.str() + "\n" + ConsolePrintf("0x%04x \n" , error) + (const char*)errString;

		MessageBoxA ( NULL, message.c_str(), header.c_str(), MB_OK | MB_ICONEXCLAMATION );
		if( error == 1282)
			g_appIsRunning = false;
	}
#endif

}



//===================================================================================================
void	MeshRenderer::BindLightSources()
{
	bool bindResult;
	bindResult = ProgramBindInt(m_shader.m_programID, "gLightCount", m_material.m_lightCount);

	for(int i = 0; i < m_material.m_lightCount; i++)
	{				
		std::stringstream out;
		out << i;
		std::string LightPosition    =  "lights[" + out.str() + "].gLightPosition" ;	
		std::string LightColor		 = 	"lights[" + out.str() + "].gLightColor";	
		std::string LightIntensity	 =  "lights[" + out.str() + "].glightIntensity";
		std::string Direction		 = 	"lights[" + out.str() + "].gLightDirection";		

		std::string LightAmbient     =  "lights[" + out.str() + "].gLightAmbient";     
		std::string LightWeight		 =  "lights[" + out.str() + "].gLightWeight";		 
		std::string LightAngle		 =  "lights[" + out.str() + "].gLightAngle";		 
		std::string LightSpotPower	 =  "lights[" + out.str() + "].gLightSpotPower";	 
		std::string LightMinSf		 =  "lights[" + out.str() + "].gLightMinSf";		 
		std::string LightMinIntensity=  "lights[" + out.str() + "].gLightMinIntensity";
		std::string LightMaxIntensity=  "lights[" + out.str() + "].gLightMaxIntensity";

		bindResult = ProgramBindVec3(	m_shader.m_programID,		LightPosition.c_str(),			m_material.m_lightArray[i].m_lightPosition);
		bindResult = ProgramBindVec4(	m_shader.m_programID,		LightColor.c_str(),				Rgba::RgbaToVector4(m_material.m_lightArray[i].m_lightColor));
		bindResult = ProgramBindFloat(	m_shader.m_programID,		LightIntensity.c_str(),			m_material.m_lightArray[i].m_lightIntensity);
		bindResult = ProgramBindVec3(	m_shader.m_programID,		Direction.c_str(),				m_material.m_lightArray[i].m_lightDirection);

		bindResult = ProgramBindFloat(	m_shader.m_programID,		LightAmbient.c_str(),      		m_material.m_lightArray[i].m_lightAmbientStrength);
		bindResult = ProgramBindFloat(	m_shader.m_programID,		LightWeight.c_str(),		  	m_material.m_lightArray[i].m_lightWeight); 
		bindResult = ProgramBindFloat(	m_shader.m_programID,		LightAngle.c_str(),		  		m_material.m_lightArray[i].m_lightAngle);
		bindResult = ProgramBindFloat(	m_shader.m_programID,		LightSpotPower.c_str(),	  		m_material.m_lightArray[i].m_lightSpotPower);
		bindResult = ProgramBindFloat(	m_shader.m_programID,		LightMinSf.c_str(),		  		m_material.m_lightArray[i].m_lightMinSf);
		bindResult = ProgramBindFloat(	m_shader.m_programID,		LightMinIntensity.c_str(), 		m_material.m_lightArray[i].m_lightMinIntensity );
		bindResult = ProgramBindFloat(	m_shader.m_programID,		LightMaxIntensity.c_str(), 		m_material.m_lightArray[i].m_lightMaxIntensity );
	}
}
void	MeshRenderer::BindDiffuseMap()
{	
	GLint  def_index  = 0;
	//Set Default if missing
	if(m_material.m_sampler.GetDiffuse() == NULL)	
	{
		unsigned char plainWhiteTexel[3] = {255, 255, 255};
		m_material.m_sampler.SetDiffuse(Texture::CreateMissingTexture("DefaultDiffuse", plainWhiteTexel, IntVector2(1,1), 3));
	}

	//Bind Diffuse
	if(m_material.m_sampler.GetDiffuse() != NULL)	
	{
		//Make an array of texture "gblank" in the pixel shader.
		ProgramBindSamplerIndex( m_shader.m_programID, "gTexDiffuse", def_index);
		glActiveTexture( GL_TEXTURE0 + def_index );

		glBindTexture( GL_TEXTURE_2D, m_material.m_sampler.GetDiffuse()->GetPlatformHandle());
		glBindSampler( def_index, m_material.m_sampler.m_samplerID);
		glDisable(GL_TEXTURE_2D);
	}
}
void	MeshRenderer::BindNormalMap()
{
	GLint  norm_index = 1;

	//Set Default if missing
	if(m_material.m_sampler.GetNormal() == NULL)	
	{
		unsigned char lightBlueTexel[3] = {127, 127, 255};
		m_material.m_sampler.SetNormal(Texture::CreateMissingTexture("DefaultNormal", lightBlueTexel, IntVector2(1,1), 3));
	}


	if(m_material.m_sampler.GetNormal() != NULL)	
	{
		ProgramBindSamplerIndex( m_shader.m_programID, "gTexNormal", norm_index);
		glActiveTexture( GL_TEXTURE0 + norm_index );

		glBindTexture( GL_TEXTURE_2D, m_material.m_sampler.GetNormal()->GetPlatformHandle());
		glBindSampler( norm_index, m_material.m_sampler.m_samplerID);
		glDisable(GL_TEXTURE_2D);
	}
}
void	MeshRenderer::BindSpecularMap()
{
	GLint  spec_index = 2;

	//Set Default if missing
	if(m_material.m_sampler.GetSpecular() == NULL)		
	{
		unsigned char darkYellowTexel[3] = {127, 127, 0};
		m_material.m_sampler.SetSpecular(Texture::CreateMissingTexture("DefaultSpecular", darkYellowTexel, IntVector2(1,1), 3));
	}


	if (m_material.m_sampler.GetSpecular() != NULL)
	{
		ProgramBindSamplerIndex(m_shader.m_programID, "gTexSpecular", spec_index);
		glActiveTexture(GL_TEXTURE0 + spec_index);

		glBindTexture(GL_TEXTURE_2D, m_material.m_sampler.GetSpecular()->GetPlatformHandle());
		glBindSampler(spec_index, m_material.m_sampler.m_samplerID);
		glDisable(GL_TEXTURE_2D);
	}
}


//===================================================================================================
void	MeshRenderer::SetLightPositionAtIndex(char index, Vector3 position)
{
	if (index > 16 || index < 0)
		return;

	m_material.m_lightArray[index].m_lightPosition = position;
}
Vector3 MeshRenderer::GetLightPositionAtIndex(char index)
{
	if (index > 16 || index < 0)
		return Vector3::ZERO;

	return m_material.m_lightArray[index].m_lightPosition;
}
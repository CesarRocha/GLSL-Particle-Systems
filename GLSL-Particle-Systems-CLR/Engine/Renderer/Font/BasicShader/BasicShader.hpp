//================================================================
//                     BasicShader.hpp
//================================================================
#pragma once
#ifndef _INCLUDED_BasicShader_
#define _INCLUDED_BasicShader_
#define UNUSED(x) (void)(x)
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Console/Console.hpp"
#include <assert.h>
#include <string>




class BasicShader
{
public:
	BasicShader();
	BasicShader(const std::string& shaderFileName);
	BasicShader(const std::string& shaderFileName, const char* textureFilename);
	~BasicShader();

	GLuint		CreateShader( const std::string& fileName, GLenum shader_type );
	void		LinkShader();
	void		DeleteShader();		

	GLuint		CreateBuffer(const void* buffer, size_t size);
	void		DestroyBuffer( GLuint buffer );
	void		CopyToBuffer( GLuint buffer_ID, const void* data, size_t size);

	GLuint		CreateVAO( GLuint buffer_ID);
	void		DestroyVAO(GLuint vao_ID);

	GLuint		CreateSampler( GLenum min_filter, GLenum mag_filter, GLenum u_wrap, GLenum v_wrap);
	void		DestroySampler( GLuint sampler_id );

	bool		ProgramBindAttribute( GLuint program_ID, const char* inName, GLint count, GLenum shader_type, GLboolean normalize, GLsizei stride, GLsizei offset );
	void		ProgramBind( GLuint program_ID );
	bool		ProgramBindFloat( GLuint program_ID, const char* name, const float& val);
	bool		ProgramBindVec3( GLuint program_ID, const char* name, const Vector3& val);
	bool		ProgramBindVec4( GLuint program_ID, const char* name, const Vector4& val);
	bool		ProgramBindMatrix( GLuint program_ID, const char* name, const Matrix4x4& matrix);
	bool		ProgramBindSamplerIndex( GLuint program_ID, const char* name, GLint index);

	void		RenderBasicShader(const Matrix4x4& view, const Matrix4x4& projection, int vertCount);

	//================================================================================================
	//Error Checking																								  
	//================================================================================================
	void		GLCheckError(const std::string& file, int line);
	bool		CheckCompatability();
	bool		CheckShaderStatus( GLuint shader_id, const std::string& filename);
	bool		CheckProgramStatus( GLuint program_ID, const std::string& fileName );
	void		FormatAndPrintLog(const std::string& filename, const char* log);
	GLuint		CreateVAOForFonts(BasicShader shader, GLuint buffer_ID);
	char* ReadFile(const std::string& fileName);
	GLuint ProgramFindAndCheck( GLuint program_ID, const char* name, GLenum type);



	std::string	m_fileName;
	vert_t*		m_myVerts;
	GLuint		m_programID;
	GLuint		m_buffer;
	GLuint		m_VAO;
	GLuint		m_sampler;

	Texture*	m_texture;


private:
	friend class Font;
	GLuint			m_vertShader;
	GLuint			m_fragShader;
};

#endif

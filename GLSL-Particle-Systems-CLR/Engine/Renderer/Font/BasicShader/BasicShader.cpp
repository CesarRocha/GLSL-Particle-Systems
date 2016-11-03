//================================================================
//                     BasicShader.cpp
//================================================================
#pragma once
#include "Engine/Renderer/Font/BasicShader/BasicShader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

BasicShader::BasicShader()
{

}
BasicShader::BasicShader(const std::string& shaderFileName)
{
	//Part of LoadProgram
	m_programID = glCreateProgram();
	m_vertShader = CreateShader( shaderFileName + "_vertex.glsl", GL_VERTEX_SHADER );
	m_fragShader = CreateShader( shaderFileName + "_fragment.glsl", GL_FRAGMENT_SHADER );
	m_fileName = shaderFileName;


	// 	///Part of Link Program
	// 	glAttachShader(m_programID, m_vertShader);
	// 	glAttachShader(m_programID, m_fragShader);
	// 	bool save_binary = false;
	// 	if( save_binary == true )
	// 		glProgramParameteri( m_programID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
	// 	glLinkProgram(m_programID);
	// 	GLCheckError(m_fileName,  __LINE__ -1);
	// 	glDetachShader( m_programID, m_vertShader );
	// 	GLCheckError(m_fileName,  __LINE__ -1);
	// 	glDetachShader( m_programID, m_fragShader );
	// 	GLCheckError(m_fileName,  __LINE__ -1);
	// 	CheckProgramStatus(m_programID, shaderFileName);
	LinkShader();

}
BasicShader::BasicShader(const std::string& shaderFileName, const char* textureFilename)
{
	CheckCompatability();
	//Part of LoadProgram
	m_fileName = shaderFileName;
	m_programID = glCreateProgram();
	m_vertShader = CreateShader( shaderFileName + "_vertex.glsl", GL_VERTEX_SHADER );
	m_fragShader = CreateShader( shaderFileName + "_fragment.glsl", GL_FRAGMENT_SHADER );
	m_texture = Texture::CreateOrGetTexture(textureFilename);// LoadTextureFromFile(filename, width, height);
	m_sampler = CreateSampler( GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT );

	// 
	// 	///Part of Link Program
	// 	glAttachShader(m_programID, m_vertShader);
	// 	glAttachShader(m_programID, m_fragShader);
	// 	bool save_binary = false;
	// 	if( save_binary == true )
	// 		glProgramParameteri( m_programID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
	// 	glLinkProgram(m_programID);	
	// 	GLCheckError(m_fileName,  __LINE__ -1);
	// 	glDetachShader( m_programID, m_vertShader );
	// 	GLCheckError(m_fileName,  __LINE__ -1);
	// 	glDetachShader( m_programID, m_fragShader );
	// 	GLCheckError(m_fileName,  __LINE__ -1);
	// 	CheckProgramStatus(m_programID, shaderFileName);
	LinkShader();


}

char* BasicShader::ReadFile(const std::string& fileName) 
{
	std::string content;
	std::ifstream fileStream(fileName, std::ios::in);

	if(!fileStream.is_open()) 
	{
		ConsolePrintf("Could not read file %s. File does not exist. \n", fileName);
		return NULL;
	}

	std::string line = "";
	while(!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	/*return content;*/

	char* a = new char[content.size()+1];
	a[content.size()]=0;
	memcpy(a,content.c_str(),content.size());

	return a;
}
GLuint BasicShader::ProgramFindAndCheck( GLuint program_ID, const char* name, GLenum type)
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
//################################################################################################//
//##                                                                                            ##//
//################################################################################################//

GLuint	BasicShader::CreateShader(const std::string& fileName, GLenum shader_type)
{
	size_t source_length = 0;

	const char *source = ReadFile(fileName);

	if( nullptr == source )
	{
		std::string note = MakeComplexString("Bad Shader Source\n[%s]\n\nCheck filepath.", fileName.c_str());
		MessageBoxA ( NULL, note.c_str(), "CreateShader error", MB_OK );
		return NULL;
	}

	source_length = strlen(source);

	GLuint shader_id = glCreateShader(shader_type);

	if(NULL == shader_id)
		return NULL;

	GLint len = (GLint)(source_length);

	glShaderSource(shader_id, 1, &source, &len);
	glCompileShader(shader_id);

	CheckShaderStatus(shader_id, fileName);

	return shader_id;
}
void	BasicShader::LinkShader()
{
	///Part of Link Program
	glAttachShader(m_programID, m_vertShader);
	glAttachShader(m_programID, m_fragShader);
	bool save_binary = false;
	if( save_binary == true )
		glProgramParameteri( m_programID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
	glLinkProgram(m_programID);
	GLCheckError(m_fileName,  __LINE__ -1);
	glDetachShader( m_programID, m_vertShader );
	GLCheckError(m_fileName,  __LINE__ -1);
	glDetachShader( m_programID, m_fragShader );
	GLCheckError(m_fileName,  __LINE__ -1);
	CheckProgramStatus(m_programID, m_fileName);
}
void	BasicShader::DeleteShader()
{
	glDeleteShader(m_vertShader);
	glDeleteShader(m_fragShader);
	glDeleteProgram(m_programID);
}

GLuint	BasicShader::CreateBuffer(const void* buffer, size_t size)
{
	GLuint buffer_ID;
	glGenBuffers( 1, &buffer_ID );
	if(buffer_ID == NULL)
		return NULL;

	if( buffer != nullptr )
		CopyToBuffer( buffer_ID, buffer, size );
	GLCheckError(m_fileName,  __LINE__ -1);

	return buffer_ID;
}
void	BasicShader::DestroyBuffer( GLuint buffer )
{
	glDeleteBuffers( 1, &buffer );
}
void	BasicShader::CopyToBuffer( GLuint buffer_ID, const void* data, size_t size)
{
	glBindBuffer( GL_ARRAY_BUFFER, buffer_ID );
	GLCheckError(m_fileName,  __LINE__ -1);

	glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
	GLCheckError(m_fileName,  __LINE__ -1);
}

GLuint	BasicShader::CreateVAO(GLuint buffer_ID)
{
	GLuint vao_ID;
	glGenVertexArrays( 1, &vao_ID );
	if( vao_ID == NULL )
	{
		assert(0);
		return NULL;
	}

	GLint max_attributes;
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &max_attributes );
	//ConsolePrintf(" Max Attributes: %i ", max_attributes );

	glBindVertexArray( vao_ID );
	glBindBuffer( GL_ARRAY_BUFFER, buffer_ID );

	GLCheckError(m_fileName,  __LINE__ -1);
	ProgramBindAttribute( m_programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_position) );
	ProgramBindAttribute( m_programID, "inUV", 2, GL_FLOAT, GL_FALSE, sizeof(vert_t), offsetof(vert_t, m_uv) );
	ProgramBindAttribute( m_programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vert_t), offsetof(vert_t, m_color) );

	//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indexbuffer);
	return vao_ID;
}
void	BasicShader::DestroyVAO(GLuint vao_ID)
{
	if( vao_ID != NULL)
		glDeleteVertexArrays( 1, &vao_ID );
}

GLuint	BasicShader::CreateSampler( GLenum min_filter, GLenum mag_filter, GLenum u_wrap, GLenum v_wrap)
{
	GLuint sampler_id;
	glGenSamplers( 1, &sampler_id );
	if( sampler_id == NULL )
		return NULL;

	glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, min_filter);
	glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, mag_filter);
	glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, u_wrap);
	glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, v_wrap);

	return sampler_id;
}
void	BasicShader::DestroySampler( GLuint sampler_id )
{
	glDeleteSamplers( 1, &sampler_id );
}

bool	BasicShader::ProgramBindAttribute(GLuint program_ID, const char* inName, GLint count, GLenum shader_type, GLboolean normalize, GLsizei stride, GLsizei offset)
{
	GLint loc = glGetAttribLocation( program_ID, inName );
	GLCheckError(m_fileName,  __LINE__ -1);

	if( loc < 0 )
		return false;

	glEnableVertexAttribArray( loc );
	GLCheckError(m_fileName,  __LINE__ -1);

	glVertexAttribPointer( loc, count, shader_type, normalize, stride, (GLvoid*) offset );
	GLCheckError(m_fileName,  __LINE__ -1);

	return true;
}
void	BasicShader::ProgramBind( GLuint program_ID )
{
	glUseProgram(program_ID);
}
bool	BasicShader::ProgramBindFloat( GLuint program_ID, const char* name, const float& val)
{
	GLint loc = ProgramFindAndCheck( program_ID, name, GL_FLOAT);
	if(loc < 0)
		return false;

	glUniform1fv( loc, 1, &val);
	return true;
}
bool	BasicShader::ProgramBindMatrix( GLuint program_ID, const char* name, const Matrix4x4& matrix)
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
bool	BasicShader::ProgramBindSamplerIndex( GLuint program_ID, const char* name, GLint index)
{
	GLint loc = ProgramFindAndCheck( program_ID, name, GL_SAMPLER_2D );
	if(loc < 0 )
		return false;

	glUniform1iv( loc, 1, (GLint*)&index);

	return true;
}
bool	BasicShader::ProgramBindVec3( GLuint program_ID, const char* name, const Vector3& val)
{
	GLint loc = glGetUniformLocation( program_ID, name );
	if( loc < 0 ) 
		return false;

	glUniform3f( loc, val.x, val.y, val.z);
	return true;
}
bool	BasicShader::ProgramBindVec4( GLuint program_ID, const char* name, const Vector4& val)
{
	GLint loc = glGetUniformLocation( program_ID, name );
	if( loc < 0 ) 
		return false;

	glUniform4f( loc, val.x, val.y, val.z, val.w);
	return true;
}



void	BasicShader::RenderBasicShader(const Matrix4x4& view, const Matrix4x4& projection, int vertCount) 
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint tex_index = 0;
	ProgramBind( m_programID );
	GLCheckError(m_fileName,  __LINE__ -1);

	glBindVertexArray( m_VAO );
	GLCheckError(m_fileName,  __LINE__ -1);



	//================================================================
	//                    
	//================================================================
	ProgramBindMatrix( m_programID, "gView", view);
	ProgramBindMatrix( m_programID, "gProj", projection);
	//================================================================
	//                     
	//================================================================
	ProgramBindSamplerIndex( m_programID, "gTexture", tex_index);
	GLCheckError( __FILE__,  __LINE__ -1);
	glActiveTexture( GL_TEXTURE0 + tex_index );


	glBindTexture( GL_TEXTURE_2D, m_texture->GetPlatformHandle());
	GLCheckError( __FILE__,  __LINE__ -1);

	glBindSampler( tex_index, m_sampler);
	GLCheckError( __FILE__,  __LINE__ -1);


	glDisable(GL_CULL_FACE);
	glDrawArrays( GL_TRIANGLES, 0, vertCount );
	GLCheckError( __FILE__,  __LINE__ -1);

	glBindVertexArray( NULL );
	glUseProgram(NULL);
	GLCheckError( __FILE__,  __LINE__ -1);
	//glEnd();
}
//################################################################################################//
//##                        Error Checking                                                      ##//
//################################################################################################//
void	BasicShader::GLCheckError(const std::string& file, int line)
{
#if defined(OPENGL_CHECK_ERRORS)
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{

  		std::stringstream out;
		out << line;
		std::string header = "Error: " + ConsolePrintf("0x%04x\n" , error) + " at " + out.str();
		std::string message1 = file;
		std::string message2 = m_fileName;
		std::string message3 = message1 + "\n" + message2;
		MessageBoxA ( NULL, message3.c_str(), header.c_str(), MB_OK );
	}
#endif
}
bool	BasicShader::CheckCompatability()
{
	//Get OpenGLVersion and print to console
	char const *gl_version = (char const*) glGetString( GL_VERSION );
	ConsolePrintf("GL_VERSION: %s \n", gl_version);
	GLCheckError(m_fileName,  __LINE__ -1);
	//Not supported by OpenGL1.* or lower, but modern cards will
	GLint major;
	GLint minor;
	glGetIntegerv( GL_MAJOR_VERSION, &major );
	glGetIntegerv  ( GL_MINOR_VERSION, &minor );
	ConsolePrintf("OpenGLVersion (int): %i.%i \n", major, minor);

	char const *shader_version = (char const*) glGetString(	GL_SHADING_LANGUAGE_VERSION )  ;
	ConsolePrintf("GL_SHADING_LANGUAGE_VERSION: %i \n", shader_version);
	GLCheckError(m_fileName,  __LINE__ -1);

	GLboolean compiler_available = GL_FALSE;
	glGetBooleanv( GL_SHADER_COMPILER, &compiler_available);
	assert( compiler_available == GL_TRUE);

	GLint binary_formats;
	glGetIntegerv( GL_NUM_PROGRAM_BINARY_FORMATS, &binary_formats);
	assert( binary_formats > 0);

	return (compiler_available == GL_TRUE);
}
bool	BasicShader::CheckShaderStatus( GLuint shader_id, const std::string& fileName)
{
	GLint status;
	glGetShaderiv( shader_id, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		GLint log_length = 0;
		glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &log_length);

		if(log_length > 0)
		{
			char* log = (char*) malloc(log_length);

			glGetShaderInfoLog( shader_id, log_length, &log_length, log);
			FormatAndPrintLog(fileName, log);

			MessageBoxA(NULL,log, "Compile Error!",MB_OK);
			free(log);
		}
		else
			GLCheckError(m_fileName,  __LINE__ -1);
		return false;
	}
	return true;

}
bool	BasicShader::CheckProgramStatus( GLuint program_ID, const std::string& fileName )
{
	GLint link_status;
	glGetProgramiv( program_ID, GL_LINK_STATUS, &link_status);
	if(link_status == GL_FALSE)
	{
		GLint log_length = 0;
		glGetProgramiv( program_ID, GL_INFO_LOG_LENGTH, &log_length);
		if(log_length > 0)
		{
			char* buffer = (char*) malloc(log_length);
			glGetProgramInfoLog( program_ID, sizeof(buffer), &log_length, (GLchar*) buffer);
			FormatAndPrintLog(fileName, buffer);
			free(buffer);
		}
		else
			GLCheckError(m_fileName,  __LINE__ -1);
		return false;
	}
	return true;
}
void	BasicShader::FormatAndPrintLog(const std::string& fileName, const char* log)
{
	std::cout << fileName;
	ConsolePrintf("Line (%d) || FileName: (%s) || Error: (%s)   ", __LINE__, fileName.c_str(), log );
}

BasicShader::~BasicShader()
{
	delete m_myVerts;
}

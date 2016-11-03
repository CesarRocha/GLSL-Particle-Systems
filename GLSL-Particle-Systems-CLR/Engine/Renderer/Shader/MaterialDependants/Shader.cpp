#include "Engine/Utility/Utility.hpp"
#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
ShaderMap Shader::g_globalShaders;



//===================================================================================================
//               class Shader	                                                                   ==
//===================================================================================================
Shader::Shader()
{
}
Shader::Shader(const std::string& shaderFileName)
{
	m_programID = glCreateProgram();
	GLuint vertShader = CreateShader( shaderFileName + "_vertex.glsl", GL_VERTEX_SHADER );
	GLuint fragShader = CreateShader( shaderFileName + "_fragment.glsl", GL_FRAGMENT_SHADER );

	glAttachShader(m_programID, vertShader);
	glAttachShader(m_programID, fragShader);
	bool save_binary = false;
	if( save_binary == true )
		glProgramParameteri( m_programID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
	glLinkProgram(m_programID);
	GLCheckError(__FILE__,  __LINE__ -1);
	glDetachShader( m_programID, vertShader );
	GLCheckError(__FILE__,  __LINE__ -1);
	glDetachShader( m_programID, fragShader );
	GLCheckError(__FILE__,  __LINE__ -1);
	CheckProgramStatus(m_programID, __FILE__);
}
Shader::Shader(const std::string& vertFileName, const std::string& fragFileName, BufferType bufferType)
{
	m_bufferType = bufferType;
	
	m_programID = glCreateProgram();
	GLuint m_vertShader = CreateShader( vertFileName + "_vertex.glsl", GL_VERTEX_SHADER );
	GLuint m_fragShader = CreateShader( fragFileName + "_fragment.glsl", GL_FRAGMENT_SHADER );

	glAttachShader(m_programID, m_vertShader);
	glAttachShader(m_programID, m_fragShader);

	glLinkProgram(m_programID);

	glDetachShader( m_programID, m_vertShader );
	glDetachShader( m_programID, m_fragShader );
	CheckProgramStatus(m_programID, __FILE__);
}
Shader::Shader(const std::string& vertFileName, const std::string& geoFileName, const std::string& fragFileName, BufferType bufferType)
{
	m_bufferType = bufferType;
	
	m_programID = glCreateProgram();
	GLuint vertShader		= CreateShader( vertFileName + "_vertex.glsl",   GL_VERTEX_SHADER );
	GLuint geometryShader	= CreateShader( geoFileName  + "_geometry.glsl", GL_GEOMETRY_SHADER);
	GLuint fragShader		= CreateShader( fragFileName + "_fragment.glsl", GL_FRAGMENT_SHADER );

	glAttachShader(m_programID, vertShader);
	glAttachShader(m_programID, geometryShader);
	glAttachShader(m_programID, fragShader);
	
	glLinkProgram(m_programID);
	
	glDetachShader( m_programID, vertShader );
	glDetachShader( m_programID, geometryShader );
	glDetachShader( m_programID, fragShader );
	CheckProgramStatus(m_programID, __FILE__);
}




//===================================================================================================
GLuint	Shader::CreateShader(const std::string& fileName, GLenum shader_type)
{
	size_t source_length = 0;

	const char *source = ReadFile(fileName);

	if( nullptr == source ){
		MessageBoxA ( NULL, fileName.c_str(), "Bad shader source", MB_OK );
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
bool	Shader::CheckShaderStatus( GLuint shader_id, const std::string& fileName)
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
bool	Shader::CheckProgramStatus( GLuint program_ID, const std::string& fileName )
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
			glGetProgramInfoLog( program_ID, log_length, &log_length, (GLchar*) buffer);
			FormatAndPrintLog(fileName, buffer);
			free(buffer);
		}
		else
			GLCheckError(m_fileName,  __LINE__ -1);
		return false;
	}
	return true;
}
GLuint	Shader::ProgramFindAndCheck( GLuint program_ID, const char* name, GLenum type)
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
void	Shader::GLCheckError(const std::string& file, int line)
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



//===================================================================================================
char*	Shader::ReadFile(const std::string& fileName) 
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

	char* a = new char[content.size()+1];
	a[content.size()]=0;
	memcpy(a,content.c_str(),content.size());

	return a;
}
void	Shader::FormatAndPrintLog(const std::string& fileName, const char* log)
{
	std::cout << fileName;
	ConsolePrintf("Line (%d) || FileName: (%s) || Error: (%s)   ", __LINE__, fileName.c_str(), log );
}



//===================================================================================================
Shader*	Shader::CreateOrGetShader(const std::string shaderFileName)
{
	ShaderMap::iterator shaderlIter = g_globalShaders.find(shaderFileName);
	if (shaderlIter != g_globalShaders.end())
		return shaderlIter->second;

	Shader* newShader = new Shader(shaderFileName);
	newShader->m_fileName = shaderFileName;
	g_globalShaders[shaderFileName] = newShader;
	return newShader;
}
Shader*	Shader::CreateOrGetShader(const std::string shaderFileName, const std::string& vertFileName, const std::string& fragFileName, BufferType bufferType)
{
	ShaderMap::iterator shaderlIter = g_globalShaders.find(shaderFileName);
	if (shaderlIter != g_globalShaders.end())
		return shaderlIter->second;

	Shader* newShader = new Shader(vertFileName, fragFileName, bufferType);
	newShader->m_fileName = shaderFileName;
	g_globalShaders[shaderFileName] = newShader;
	return newShader;
}
Shader*	Shader::CreateOrGetShader(const std::string shaderFileName, const std::string& vertFileName, const std::string& geoFileName, const std::string& fragFileName, BufferType bufferType)
{
	ShaderMap::iterator shaderlIter = g_globalShaders.find(shaderFileName);
	if (shaderlIter != g_globalShaders.end())
		return shaderlIter->second;

	Shader* newShader = new Shader(vertFileName, geoFileName, fragFileName, bufferType);
	newShader->m_fileName = shaderFileName;
	g_globalShaders[shaderFileName] = newShader;
	return newShader;
}
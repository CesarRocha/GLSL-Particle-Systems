#pragma once
#ifndef _INCLUDE_MESH_
#define	_INCLUDE_MESH_
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Console/Console.hpp"


enum BufferType
{
	VBO,
	IBO,
	GEOPOINT,
	GEOTRIANGLE
};
//===================================================================================================
//               class Mesh		                                                                   ==
//===================================================================================================
class Mesh
{
public:
	Mesh();
	Mesh(const void* vboBuffer, size_t vboSize, const void* iboBuffer = 0, size_t iboSize = 0);

	static const Mesh CUBE();

	void UpdateMesh(const void* meshData, size_t size);
	void	CopyToBufferVBO( GLuint buffer_ID, const void* data, size_t numVerts);
	GLuint		m_vboID;
private:
	friend class MeshRenderer;
	size_t		m_vboVertCount;
	GLuint		m_iboID;
	size_t		m_iboIndexCount;


	GLuint	CreateBuffer(const void* buffer, size_t size, const BufferType&  bufferType);
	void	DestroyBuffer( GLuint buffer );
	void	CopyToBufferIBO( GLuint buffer_ID, const void* data, size_t size);
	bool	ProgramBindAttribute(GLuint program_ID, const char* inName, GLint count, GLenum shader_type, GLboolean normalize, GLsizei stride, GLsizei offset);
	void	GLCheckError(const std::string& file, int line);
};

#endif


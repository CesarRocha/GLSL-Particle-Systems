#pragma once
#ifndef _INCLUDE_BINARY_BUFFER_PARSER_
#define _INCLUDE_BINARY_BUFFER_PARSER_
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/3D/Vector3.hpp"
#include "Engine/Math/3D/Vector4.hpp"
#include "Engine/Math/3D/Matrix4x4.hpp"
#include "Engine/Renderer/vert_t.hpp"
#include "Engine/Renderer/vert_s.hpp"
#include <vector>

//===================================================================================================
//																								   ==
//===================================================================================================
class BinaryBufferParser
{
public:

	BinaryBufferParser();
	BinaryBufferParser(const unsigned char* buffer, size_t size);
	BinaryBufferParser(const std::vector<unsigned char>& bytes);
	~BinaryBufferParser();

	int				ReadNextInt();
	char			ReadNextChar();
	unsigned int	ReadNextUnsignedInt();
	unsigned char	ReadNextUnsignedChar();
	short			ReadNextShort();
	unsigned short	ReadNextUnsignedShort();
	float			ReadNextFloat();
	double			ReadNextDouble();
	bool			ReadNextBool();
	std::string		ReadNextString();
	Vector2			ReadNextVector2();
	Vector3			ReadNextVector3();
	Vector4			ReadNextVector4();
	Matrix4x4		ReadNextMatrix4x4();
	//bool			HadReachedEndOfData();
	vert_t			ReadNextTriangleVertex();
	vert_s			ReadNextSkeletalVertex();
	Rgba			ReadNextRGBA();

	//bool CanReadNextSizeofData(size_t size);

	size_t					m_bufferSize;
	const unsigned char*	m_bufferB;
	const char*				m_bufferS;

	std::vector<unsigned char> m_buffer;

	size_t					m_currentIndex;

private:
};

#endif // _INCLUDE_BINARY_BUFFER_PARSER_

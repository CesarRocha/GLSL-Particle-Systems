#include "BinaryBufferParser.hpp"
#include "Engine/EngineCommon/EngineCommon.hpp"

//===================================================================================================
//																								   ==
//===================================================================================================
BinaryBufferParser::BinaryBufferParser()
	:m_currentIndex(0)
{
}
BinaryBufferParser::BinaryBufferParser(const unsigned char* buffer, size_t size)
	: m_bufferB(buffer)
	, m_bufferSize(size)
	, m_currentIndex(0)
{
}
BinaryBufferParser::BinaryBufferParser(const std::vector<unsigned char>& bytes)
{
	UNUSED(bytes);
}
BinaryBufferParser::~BinaryBufferParser()
{
}



//===================================================================================================
int				BinaryBufferParser::ReadNextInt()
{
	int i = *(int*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(int);
	return i;
}


//===================================================================================================
unsigned int	BinaryBufferParser::ReadNextUnsignedInt()
{
	unsigned int ui = *(unsigned int*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(unsigned int);
	return ui;
}


//===================================================================================================
char			BinaryBufferParser::ReadNextChar()
{
	char c = *(char*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(char);
	return c;
}


//===================================================================================================
unsigned char	BinaryBufferParser::ReadNextUnsignedChar()
{
	unsigned char uc = *(unsigned char*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(unsigned char);
	return uc;
}


//===================================================================================================
short			BinaryBufferParser::ReadNextShort()
{
	short s = *(short*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(short);
	return s;
}


//===================================================================================================
unsigned short	BinaryBufferParser::ReadNextUnsignedShort()
{
	unsigned short us = *(unsigned short*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(unsigned short);
	return us;
}


//===================================================================================================
float			BinaryBufferParser::ReadNextFloat()
{
	float f = *(float*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(float);
	return f;
}


//===================================================================================================
double			BinaryBufferParser::ReadNextDouble()
{
	double d = *(double*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(double);
	return d;
}


//===================================================================================================
bool			BinaryBufferParser::ReadNextBool()
{
	bool b = *(bool*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(bool);
	return b;
}


//===================================================================================================
std::string		BinaryBufferParser::ReadNextString()
{
	std::string nextString = "";
	unsigned char nextChar;
	nextChar = ReadNextUnsignedChar();
	nextString += nextChar;
	while (nextChar != '\0')
	{
		nextChar = ReadNextUnsignedChar();
		nextString += nextChar;
	}
	return nextString;
}


//===================================================================================================
Vector2			BinaryBufferParser::ReadNextVector2()
{
	Vector2 nextVector2;
	nextVector2.x = ReadNextFloat();
	nextVector2.y = ReadNextFloat();
	return nextVector2;
}



//===================================================================================================
Vector3			BinaryBufferParser::ReadNextVector3()
{
	Vector3 nextVector3;
	nextVector3.x = ReadNextFloat();
	nextVector3.y = ReadNextFloat();
	nextVector3.z = ReadNextFloat();
	return nextVector3;
}


//===================================================================================================
Vector4			BinaryBufferParser::ReadNextVector4()
{
	Vector4 nextVector4;
	nextVector4.x = ReadNextFloat();
	nextVector4.y = ReadNextFloat();
	nextVector4.z = ReadNextFloat();
	nextVector4.w = ReadNextFloat();
	return nextVector4;
}


//===================================================================================================

Matrix4x4		BinaryBufferParser::ReadNextMatrix4x4()
{
	Matrix4x4 m4 = *(Matrix4x4*)(&m_buffer[m_currentIndex]);
	m_currentIndex += sizeof(Matrix4x4);
	return m4;
}


//===================================================================================================
Rgba			BinaryBufferParser::ReadNextRGBA()
{
	Rgba color;
	color.r = ReadNextUnsignedChar();
	color.g = ReadNextUnsignedChar();
	color.b = ReadNextUnsignedChar();
	color.a = ReadNextUnsignedChar();
	return color;
}


//===================================================================================================
vert_t			BinaryBufferParser::ReadNextTriangleVertex()
{
	vert_t nextVert;
	//check if valid or not

	nextVert.m_position = ReadNextVector3();
	nextVert.m_color = Rgba::WHITE;
	ReadNextRGBA();
	nextVert.m_uv = ReadNextVector2();
	nextVert.m_norm = ReadNextVector3();
	nextVert.m_tang = ReadNextVector3();
	nextVert.m_bitang = ReadNextVector3();
	return nextVert;
}


//===================================================================================================
vert_s			BinaryBufferParser::ReadNextSkeletalVertex()
{
	vert_s nextVert;
	//check if valid or not

	nextVert.m_position = ReadNextVector3();
	nextVert.m_color = ReadNextRGBA();
	nextVert.m_uv = ReadNextVector2();
	nextVert.m_normal = ReadNextVector3();
	nextVert.m_tangent = ReadNextVector3();
	nextVert.m_bitangent = ReadNextVector3();
	return nextVert;
}



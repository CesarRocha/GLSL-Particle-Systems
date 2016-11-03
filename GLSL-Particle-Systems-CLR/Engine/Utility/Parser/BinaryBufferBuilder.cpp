#include "BinaryBufferBuilder.hpp"
#include "Engine/EngineCommon/EngineCommon.hpp"

//===================================================================================================
//				class BinaryBufferBuilder														   ==
//===================================================================================================
BinaryBufferBuilder::BinaryBufferBuilder()
{

}
BinaryBufferBuilder::BinaryBufferBuilder(const size_t& size)
{
	m_builderBuffer.reserve(size);
}
BinaryBufferBuilder::~BinaryBufferBuilder()
{
}

void BinaryBufferBuilder::AppendInt(const int& i)
{
	AppendBuffer(i);
}
void BinaryBufferBuilder::AppendChar(const char& c)
{
	AppendBuffer(c);
}
void BinaryBufferBuilder::AppendUnsignedChar(const unsigned char& uc)
{
	AppendBuffer(uc);
}
void BinaryBufferBuilder::AppendShort(const short& s)
{
	AppendBuffer(s);
}
void BinaryBufferBuilder::AppendUnsignedShort(const unsigned short& us)
{
	AppendBuffer(us);
}
void BinaryBufferBuilder::AppendFloat(const float& f)
{
	AppendBuffer(f);
}
void BinaryBufferBuilder::AppendDouble(const double& d)
{
	AppendBuffer(d);
}
void BinaryBufferBuilder::AppendBool(const bool& b)
{
	AppendBuffer(b);
}
void BinaryBufferBuilder::AppendString(const std::string& str)
{
	UNUSED(str);
	//AppendBuffer(str);
	//DANGER USE LENGTH instead of size of
}
void BinaryBufferBuilder::AppendVector2(const Vector2& v2)
{
	AppendBuffer(v2);
}
void BinaryBufferBuilder::AppendVector3(const Vector3& v3)
{
	AppendBuffer(v3);
}
void BinaryBufferBuilder::AppendVector4(const Vector4& v4)
{
	AppendBuffer(v4);
}
void BinaryBufferBuilder::AppendMatrix4x4(const Matrix4x4& m4)
{
	AppendBuffer(m4);
}
void BinaryBufferBuilder::AppendBuffer(void* location, size_t numBytes)
{
	unsigned char* loc = (unsigned char*)location;
	for (unsigned char i = 0; i < numBytes; i++)
	{
		m_builderBuffer.push_back(*loc);
		loc++;
	}
}
void BinaryBufferBuilder::WriteToFile(const std::string& filePath)
{
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "wb");
	fwrite(&m_builderBuffer[0], sizeof(char), m_builderBuffer.size(), file);
	fclose(file);
}
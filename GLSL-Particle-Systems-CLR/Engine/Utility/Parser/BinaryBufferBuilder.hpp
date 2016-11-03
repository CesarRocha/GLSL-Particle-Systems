#pragma once
#ifndef _INCLUDE_BINARY_BUFFER_BUILDER_
#define _INCLUDE_BINARY_BUFFER_BUILDER_
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/3D/Vector3.hpp"
#include "Engine/Math/3D/Vector4.hpp"
#include "Engine/Math/3D/Matrix4x4.hpp"
#include <vector>


//===================================================================================================
//				class BinaryBufferBuilder														   ==
//===================================================================================================
class BinaryBufferBuilder
{
public:
	BinaryBufferBuilder();
	BinaryBufferBuilder(const size_t& size);
	~BinaryBufferBuilder();

	void	AppendInt(const int& i);
	void	AppendUnsignedInt(const unsigned int& ui);
	void	AppendChar(const char& c);
	void	AppendUnsignedChar(const unsigned char& uc);
	void	AppendShort(const short& s);
	void	AppendUnsignedShort(const unsigned short& us);
	void	AppendFloat(const float& f);
	void	AppendDouble(const double& d);
	void	AppendBool(const bool& b);
	void	AppendString(const std::string& str);
	void	AppendVector2(const Vector2& v2);
	void	AppendVector3(const Vector3& v3);
	void	AppendVector4(const Vector4& v4);
	void	AppendMatrix4x4(const Matrix4x4& m4);
	void	WriteToFile(const std::string& filePath);

	std::vector<unsigned char> m_builderBuffer;

private:
	template<typename T> void AppendBuffer(const T& location) { AppendBuffer((void*)& location, sizeof(T)); }
	void AppendBuffer(void* location, size_t numBytes);

};

#endif //_INCLUDE_BINARY_BUFFER_BUILDER_

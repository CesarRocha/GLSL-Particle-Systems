#pragma once
#ifndef _INCLUDE_VERTT_
#define _INCLUDE_VERTT_
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/3D/Vector3.hpp"
#include "Engine/Renderer/RGBA.hpp"
//===================================================================================================
//				class vert_t																	   ==
//===================================================================================================
class vert_t
{
public:
	vert_t();
	vert_t(const Vector3& position);
	vert_t(const Vector3& position, const Vector2& uv);
	vert_t(const Vector3& position, const Rgba& color);
	vert_t(const Vector3& position, const Vector2& uv, const Rgba& color);
	vert_t(const Vector3& position, const Vector2& uv, const Vector2& uv2, const Rgba& color);
	vert_t(const Vector3& position, const Vector2& uv, const Vector2& uv2, const Vector2& uv3,const Rgba& color);
	vert_t(const Vector3& position, const Vector2& uv, const Vector3& normal);
	vert_t(const Vector3& position, const Vector2& uv, const Vector3& normal, const Vector3& tangent, const Vector3& biTangent, const Rgba color);

	Vector3		m_position;
	Vector3		m_orientation;
	Vector3		m_velocity;
	Vector2		m_uv;
	Rgba		m_color;
	float		m_voxelSize;



	Vector2		m_uvMax;
	Vector2		m_uv2;  
	Vector2		m_uv3; 


	Vector3		m_norm;
	Vector3		m_tang;
	Vector3		m_bitang;


	static vert_t* SQUARE();
	static vert_t* SOLOPOINT();
};


#endif
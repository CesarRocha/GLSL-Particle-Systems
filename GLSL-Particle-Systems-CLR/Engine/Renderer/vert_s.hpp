#pragma once
#ifndef _INCLUDE_VERTS_
#define _INCLUDE_VERTS_
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/3D/Vector3.hpp"
#include "Engine/Renderer/RGBA.hpp"
//===================================================================================================
//				class vert_s																	   ==
//===================================================================================================
struct vert_s
{
	vert_s() {}
	vert_s(const Vector3& position, const Vector2& uv, const Rgba& color)
		: m_position(position)
		, m_uv(uv)
		, m_color(color)
	{}
	vert_s(const Vector3& position, const Vector2& uv)
		: m_position(position)
		, m_uv(uv)
	{}
	vert_s(const Vector3& position, const Vector2& uv, const Vector3& normal)
		: m_position(position)
		, m_uv(uv)
		, m_normal(normal)
	{}
	vert_s(const Vector3& position, const Vector2& uv, const Vector3& normal, const Vector3& tangent, const Vector3& biTangent, const Rgba color)
		: m_position(position)
		, m_uv(uv)
		, m_normal(normal)
		, m_tangent(tangent)
		, m_bitangent(biTangent)
		, m_color(color)
	{}

	Vector3		m_position;
	Vector2		m_uv;
	Vector3		m_normal;
	Vector3		m_tangent;
	Vector3		m_bitangent;
	Rgba		m_color;



	unsigned char m_boneIndexs[4];
	Vector3		m_boneWeights;
};


#endif
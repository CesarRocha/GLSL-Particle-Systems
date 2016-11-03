#include "vert_t.hpp"



//===================================================================================================
//				class vert_t																	   ==
//===================================================================================================
vert_t::vert_t(const Vector3& position)
	: m_position(position)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(Rgba::WHITE)
	, m_voxelSize(1.0f)
	, m_uv(Vector2::ZERO)
	, m_uvMax(Vector2::ONE)
	, m_uv2(Vector2::ZERO)
	, m_uv3(Vector2::ZERO)
	, m_norm(Vector3::ZUP)
	, m_tang(Vector3::XUP)
	, m_bitang(Vector3::YUP)
{}


vert_t::vert_t()
	: m_position(Vector3::ZERO)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(Rgba::WHITE)
	, m_voxelSize(1.0f)
	, m_uv(Vector2::ZERO)
	, m_uvMax(Vector2::ONE)
	, m_uv2(Vector2::ZERO)
	, m_uv3(Vector2::ZERO)
	, m_norm(Vector3::ZUP)
	, m_tang(Vector3::XUP)
	, m_bitang(Vector3::YUP)
{
}
vert_t::vert_t(const Vector3& position, const Vector2& uv)
	: m_position(position)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(Rgba::WHITE)
	, m_voxelSize(1.0f)
	, m_uv(uv)
	, m_uvMax(Vector2::ONE)
	, m_uv2(uv)
	, m_uv3(uv)
	, m_norm(Vector3::ZUP)
	, m_tang(Vector3::XUP)
	, m_bitang(Vector3::YUP)
{}
vert_t::vert_t(const Vector3& position, const Rgba& color)
	: m_position(position)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(color)
	, m_voxelSize(1.0f)
	, m_uv(Vector2::ZERO)
	, m_uvMax(Vector2::ONE)
	, m_uv2(Vector2::ZERO)
	, m_uv3(Vector2::ZERO)
	, m_norm(Vector3::ZUP)
	, m_tang(Vector3::XUP)
	, m_bitang(Vector3::YUP)
{}
vert_t::vert_t(const Vector3& position, const Vector2& uv, const Rgba& color)
	: m_position(position)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(color)
	, m_voxelSize(1.0f)
	, m_uv(uv)
	, m_uvMax(Vector2::ONE)
	, m_uv2(uv)
	, m_uv3(uv)
	, m_norm(Vector3::ZUP)
	, m_tang(Vector3::XUP)
	, m_bitang(Vector3::YUP)
{}
vert_t::vert_t(const Vector3& position, const Vector2& uv, const Vector2& uv2, const Rgba& color)
	: m_position(position)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(color)
	, m_voxelSize(1.0f)
	, m_uv(uv)
	, m_uvMax(Vector2::ONE)
	, m_uv2(uv2)
	, m_uv3(uv2)
	, m_norm(Vector3::ZUP)
	, m_tang(Vector3::XUP)
	, m_bitang(Vector3::YUP)
{}
vert_t::vert_t(const Vector3& position, const Vector2& uv, const Vector2& uv2, const Vector2& uv3, const Rgba& color)
	: m_position(position)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(color)
	, m_voxelSize(1.0f)
	, m_uv(uv)
	, m_uvMax(Vector2::ONE)
	, m_uv2(uv2)
	, m_uv3(uv3)
	, m_norm(Vector3::ZUP)
	, m_tang(Vector3::XUP)
	, m_bitang(Vector3::YUP)
{}
vert_t::vert_t(const Vector3& position, const Vector2& uv, const Vector3& normal)
	: m_position(position)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(Rgba::WHITE)
	, m_voxelSize(1.0f)
	, m_uv(uv)
	, m_uvMax(Vector2::ONE)
	, m_uv2(uv)
	, m_uv3(uv)
	, m_norm(normal)
	, m_tang(Vector3::XUP)
	, m_bitang(Vector3::YUP)
{}
vert_t::vert_t(const Vector3& position, const Vector2& uv, const Vector3& normal, const Vector3& tangent, const Vector3& biTangent, const Rgba color)
	: m_position(position)
	, m_orientation(Vector3::XUP)
	, m_velocity(Vector3::ZERO)
	, m_color(color)
	, m_voxelSize(1.0f)
	, m_uv(uv)
	, m_uvMax(Vector2::ONE)
	, m_uv2(uv)
	, m_uv3(uv)
	, m_norm(normal)
	, m_tang(tangent)
	, m_bitang(biTangent)
{}



//===================================================================================================
vert_t* vert_t::SQUARE()
{
	vert_t* newSquare = new vert_t();
	newSquare[0] = vert_t(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f));
	newSquare[1] = vert_t(Vector3(-1.0f,  1.0f, 0.0f), Vector2(0.0f, 1.0f));
	newSquare[2] = vert_t(Vector3(01.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f));
	newSquare[3] = vert_t(Vector3(01.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f));
	newSquare[4] = vert_t(Vector3(-1.0f,  1.0f, 0.0f), Vector2(0.0f, 1.0f));
	newSquare[5] = vert_t(Vector3(01.0f,  1.0f, 0.0f), Vector2(1.0f, 1.0f));

	return newSquare;
}

vert_t* vert_t::SOLOPOINT()
{
	vert_t* newSquare = new vert_t();
	newSquare[0] = vert_t();

	return newSquare;
}
//================================================================
//                     Math3D.hpp
//================================================================

#pragma once
#ifndef  __included_Math3D__
#define  __included_Math3D__

#include "Vector3.hpp"
#include "AABB3.hpp"
#include "Sphere3.hpp"
#include "LineSegment3.hpp"

class Math3D
{
public:
	bool DoAABBsOverlap( const AABB3& first, const AABB3& second );
	bool DoSpheresOverlap( const Sphere3& first, const Sphere3& second );
	bool DoShapesOverlap( const AABB3& cube, const Sphere3& sphere );
	bool DoShapesOverlap( const Sphere3& sphere, const AABB3& cube );

};
#endif

//================================================================
//                     Math2D.hpp
//================================================================

#pragma once
#ifndef  __included_Math2D__
#define  __included_Math2D__

#include "Vector2.hpp"
#include "AABB2.hpp"
#include "Disc2.hpp"
#include "LineSegment2.hpp"

class Math2D
{
public:
	bool DoAABBsOverlap( const AABB2& first, const AABB2& second );
	bool DoDiscsOverlap( const Disc2& first, const Disc2& second );
	bool DoShapesOverlap( const AABB2& box, const Disc2& disc );
	bool DoShapesOverlap( const Disc2& disc, const AABB2& box );

};

#endif

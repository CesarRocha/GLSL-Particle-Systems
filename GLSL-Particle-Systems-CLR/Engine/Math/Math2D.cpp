//================================================================
//                     Math2D.cpp
//================================================================

#include "Math2D.hpp"
#include "AABB2.hpp"

bool DoAABBsOverlap( const AABB2& first, const AABB2& second )
{	
	return (
		(first.mins.x  + first.CalcSize().x   >= second.mins.x)  &&  
		(second.mins.x + second.CalcSize().x  >= first.mins.x)   &&
			 
		(first.mins.y  + first.CalcSize().y   >= second.mins.y)  &&  
		(second.mins.y + second.CalcSize().y  >= first.mins.y )  );
}

bool DoDiscsOverlap( const Disc2& first, const Disc2& second )
{
	float sumRadius = first.radius + second.radius;
	float discDistance = 
		sqrt(
		(first.center.x - second.center.x)*(first.center.x - second.center.x) + 
		(first.center.y - second.center.y)*(first.center.y - second.center.y) );
	return (discDistance < sumRadius);

}

bool DoShapesOverlap( const AABB2& box, const Disc2& disc )
{
	     if( disc.center.x + disc.radius < box.mins.x)
			return false; //west
	else if( disc.center.x - disc.radius > box.maxs.x)
			return false; //east
	else if( disc.center.y + disc.radius < box.mins.y)
			return false; //south
	else if( disc.center.x - disc.radius > box.maxs.y)
			return false; //north
	else return true;

	//SW
	//SE
	//NE
	//NW
}

bool DoShapesOverlap( const Disc2& disc, const AABB2& box )
{
	if( disc.center.x + disc.radius < box.mins.x)
		return false; //west
	else if( disc.center.x - disc.radius > box.maxs.x)
		return false; //east
	else if( disc.center.y + disc.radius < box.mins.y)
		return false; //south
	else if( disc.center.x - disc.radius > box.maxs.y)
		return false; //north
	else return true;
	//BL
	//BRd
	//TR
	//TL
}
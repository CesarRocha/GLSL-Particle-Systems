//================================================================
//                     Math3D.cpp
//================================================================

#include "Math3D.hpp"
#include "AABB3.hpp"

bool DoAABBsOverlap( const AABB3& first, const AABB3& second )
{	
	return (
		(first.mins.x  + first.CalcSize().x   >= second.mins.x)  &&  
		(second.mins.x + second.CalcSize().x  >= first.mins.x)   &&
			 
		(first.mins.y  + first.CalcSize().y   >= second.mins.y)  &&  
		(second.mins.y + second.CalcSize().y  >= first.mins.y )  &&
		
		(first.mins.z  + first.CalcSize().z   >= second.mins.z)  &&  
		(second.mins.z + second.CalcSize().z  >= first.mins.z )  );
}

bool DoSpheresOverlap( const Sphere3& first, const Sphere3& second )
{
	float sumRadius = first.radius + second.radius;
	float discDistance = 
		sqrt(
		(first.center.x - second.center.x)*(first.center.x - second.center.x) + 
		(first.center.y - second.center.y)*(first.center.y - second.center.y) +
		(first.center.z - second.center.z)*(first.center.z - second.center.z));
	return (discDistance < sumRadius);

}

bool DoShapesOverlap( const AABB3& cube, const Sphere3& sphere )
{
	     if( sphere.center.x + sphere.radius < cube.mins.x)
			return false; //west
	else if( sphere.center.x - sphere.radius > cube.maxs.x)
			return false; //east
	else if( sphere.center.y + sphere.radius < cube.mins.y)
			return false; //south
	else if( sphere.center.x - sphere.radius > cube.maxs.y)
			return false; //north
	else return true;

	//SW
	//SE
	//NE
	//NW
}

bool DoShapesOverlap( const Sphere3& sphere, const AABB3& cube )
{
	if( sphere.center.x + sphere.radius < cube.mins.x)
		return false; //west
	else if( sphere.center.x - sphere.radius > cube.maxs.x)
		return false; //east
	else if( sphere.center.y + sphere.radius < cube.mins.y)
		return false; //south
	else if( sphere.center.x - sphere.radius > cube.maxs.y)
		return false; //north
	else return true;
	//BL
	//BRd
	//TR
	//TL
}
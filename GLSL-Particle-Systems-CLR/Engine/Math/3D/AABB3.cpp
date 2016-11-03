//================================================================
//                     AABB3.cpp
//================================================================

#include "AABB3.hpp"

//Constructors
AABB3::AABB3() 
{
}

AABB3::~AABB3() 
{
}

AABB3::AABB3( const AABB3& copy )
{
	this->mins = copy.mins;
	this->maxs = copy.maxs;
}

AABB3::AABB3( float initialX, float initialY, float initialZ )
: mins(initialX, initialY, initialZ)
, maxs(initialX, initialY, initialZ)
{
}

AABB3::AABB3( float minX, float minY, float minZ, float maxX, float maxY, float maxZ )
: mins(minX, minY, minZ)
, maxs(maxX, maxY, maxZ)
{
}

AABB3::AABB3( const Vector3& mins, const Vector3& maxs )
: mins(mins)
, maxs(maxs)
{
}

AABB3::AABB3( const Vector3& center, float radiusX, float radiusY, float radiusZ )
{
	mins.x = center.x-radiusX;
	mins.y = center.y-radiusY;
	mins.z = center.z-radiusZ;

	maxs.x = center.x+radiusX;
	maxs.y = center.y+radiusY;
	maxs.z = center.y+radiusZ;
}

//################################################################################################//
//##                                 Mutators                                                   ##//
//################################################################################################//
void AABB3::StretchToIncludePoint( const Vector3& point )
{
	if (mins.x > point.x)
		mins.x = point.x;

	if (mins.y > point.y)
		mins.y = point.y;

	if (mins.z > point.z)
		mins.z = point.z;

	if (maxs.x < point.x)
		maxs.x = point.x;

	if (maxs.y < point.y)
		maxs.y = point.y;

	if (maxs.z < point.z)
		maxs.z = point.z;
}

void AABB3::AddPadding( float xPaddingRadius, float yPaddingRadius )
{
	mins.x -= xPaddingRadius;
	maxs.x += xPaddingRadius;

	mins.y -= yPaddingRadius;
	maxs.y += yPaddingRadius;

	mins.z -= yPaddingRadius;
	maxs.z += yPaddingRadius;
}

void AABB3::Translate( const Vector3& translation )
{
	mins += translation;
	maxs += translation;
}

const AABB3 AABB3::Interpolate( const AABB3& start, const AABB3& end, float fractionFromStartToEnd )
{
	Vector3 ResultingMinsVector;
	Vector3 ResultingMaxsVector;

	return AABB3 ( ResultingMinsVector.Interpolate(start.mins, end.mins, fractionFromStartToEnd) ,
			       ResultingMaxsVector.Interpolate(start.maxs, end.maxs, fractionFromStartToEnd) );
}

//################################################################################################//
//##                                 Accessors                                                  ##//
//################################################################################################//
bool AABB3::IsPointInside( const Vector3& point ) const
{
	return ((point.x > mins.x) && 
		    (point.x < maxs.x) && 
			(point.y > mins.y) &&
			(point.y < maxs.y) &&
			(point.z > mins.z) &&
			(point.z < maxs.z));
	
}

const Vector3 AABB3::CalcSize() const
{
	return Vector3(maxs.x - mins.x, maxs.y - mins.y, maxs.z - mins.z);
}

const Vector3 AABB3::CalcCenter() const
{
	return Vector3((maxs.x + mins.x)*.5f, (maxs.y + mins.y)*.5f, (maxs.z + mins.z)*.5f);
}

// const Vector3 AABB3::GetPointAtNormalizedPositionWithinBox( const Vector3& normalizedPosition ) const
// {
// }

// const Vector3 AABB3::GetNormalizedPositionForPointWithinBox( const Vector3& point ) const
// {
// }


const AABB3 AABB3::operator + ( const Vector3& translation ) const
{
	return AABB3(
		mins.x + translation.x, 
		maxs.x + translation.x, 
		mins.y + translation.y, 
		maxs.y + translation.y,
		mins.z + translation.z, 
		maxs.z + translation.z
		);
}

const AABB3 AABB3::operator - ( const Vector3& antiTranslation ) const
{
	return AABB3(
		mins.x - antiTranslation.x, 
		maxs.x - antiTranslation.x,
		mins.y - antiTranslation.y, 
		maxs.y - antiTranslation.y,
		mins.z - antiTranslation.z, 
		maxs.z - antiTranslation.z);
}

void AABB3::operator += ( const Vector3& translation )
{
	mins.x += translation.x;
	mins.y += translation.y;
	mins.z += translation.z;
	maxs.x += translation.x;
	maxs.y += translation.y;
	maxs.z += translation.z;
}


void AABB3::operator -= ( const Vector3& antiTranslation )
{
	mins.x -= antiTranslation.x;
	mins.y -= antiTranslation.y;
	mins.z -= antiTranslation.z;
	maxs.x -= antiTranslation.x;
	maxs.y -= antiTranslation.y;
	maxs.z -= antiTranslation.z;
}



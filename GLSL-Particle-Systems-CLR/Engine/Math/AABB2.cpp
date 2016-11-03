//================================================================
//                     AABB2.cpp
//================================================================

#include "AABB2.hpp"

//Constructors
AABB2::AABB2() 
{
}

AABB2::~AABB2() 
{
}

AABB2::AABB2( const AABB2& copy )
{
	this->mins = copy.mins;
	this->maxs = copy.maxs;
}

AABB2::AABB2( float initialX, float initialY )
: mins(initialX, initialY)
, maxs(initialX, initialY)
{
}

AABB2::AABB2( float minX, float minY, float maxX, float maxY )
: mins(minX, minY)
, maxs(maxX, maxY)
{
}

AABB2::AABB2( const Vector2& mins, const Vector2& maxs )
: mins(mins)
, maxs(maxs)
{
}

AABB2::AABB2( const Vector2& center, float radiusX, float radiusY )
{
	mins.x = center.x-radiusX;
	mins.y = center.y-radiusY;

	maxs.x = center.x+radiusX;
	maxs.y = center.y+radiusY;
}

//################################################################################################//
//##                                 Mutators                                                   ##//
//################################################################################################//
void AABB2::StretchToIncludePoint( const Vector2& point )
{
	if (mins.x > point.x)
		mins.x = point.x;

	if (mins.y > point.y)
		mins.y = point.y;

	if (maxs.x < point.x)
		maxs.x = point.x;

	if (maxs.y < point.y)
		maxs.y = point.y;
}

void AABB2::AddPadding( float xPaddingRadius, float yPaddingRadius )
{
	mins.x -= xPaddingRadius;
	maxs.x += xPaddingRadius;

	mins.y -= yPaddingRadius;
	maxs.y += yPaddingRadius;
}

void AABB2::Translate( const Vector2& translation )
{
	mins += translation;
	maxs += translation;
}

const AABB2 AABB2::Interpolate( const AABB2& start, const AABB2& end, float fractionFromStartToEnd )
{
	Vector2 ResultingMinsVector;
	Vector2 ResultingMaxsVector;

	return AABB2 ( ResultingMinsVector.Interpolate(start.mins, end.mins, fractionFromStartToEnd) ,
			       ResultingMaxsVector.Interpolate(start.maxs, end.maxs, fractionFromStartToEnd) );
}

//################################################################################################//
//##                                 Accessors                                                  ##//
//################################################################################################//
bool AABB2::IsPointInside( const Vector2& point ) const
{
	return ((point.x > mins.x) && (point.x < maxs.x) && (point.y > mins.y) &&  (point.y < maxs.y)); 
	
}

const Vector2 AABB2::CalcSize() const
{
	return Vector2(maxs.x - mins.x, maxs.y - mins.y);
}

const Vector2 AABB2::CalcCenter() const
{
	return Vector2((maxs.x + mins.x)*.5f, (maxs.y + mins.y)*.5f);
}

// const Vector2 AABB2::GetPointAtNormalizedPositionWithinBox( const Vector2& normalizedPosition ) const
// {
// 
// }

// const Vector2 AABB2::GetNormalizedPositionForPointWithinBox( const Vector2& point ) const
// {
// }


const AABB2 AABB2::operator + ( const Vector2& translation ) const
{
	return AABB2(mins.x + translation.x, mins.y + translation.y, maxs.x + translation.x, maxs.y + translation.y);
}

const AABB2 AABB2::operator - ( const Vector2& antiTranslation ) const
{
	return AABB2(mins.x - antiTranslation.x, mins.y - antiTranslation.y, maxs.x - antiTranslation.x, maxs.y - antiTranslation.y);
}

void AABB2::operator += ( const Vector2& translation )
{
	mins.x += translation.x;
	mins.y += translation.y;
	maxs.x += translation.x;
	maxs.y += translation.y;
}

void AABB2::operator -= ( const Vector2& antiTranslation )
{
	mins.x -= antiTranslation.x;
	mins.y -= antiTranslation.y;
	maxs.x -= antiTranslation.x;
	maxs.y -= antiTranslation.y;
}

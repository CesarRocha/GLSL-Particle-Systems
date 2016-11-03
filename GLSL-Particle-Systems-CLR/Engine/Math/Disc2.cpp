//================================================================
//                     Disc2.cpp
//================================================================

#include "Disc2.hpp"

//Constructors
Disc2::~Disc2()
{
}

Disc2::Disc2()
{
}

Disc2::Disc2( const Disc2& copy )
{
	center = copy.center;
	radius = copy.radius;
}

Disc2::Disc2( float initialX, float initialY, float initialRadius )
: center(initialX, initialY)
, radius(initialRadius)
{
}

Disc2::Disc2( const Vector2& initialCenter, float initialRadius )
: center(initialCenter)
, radius(initialRadius)
{
}


//################################################################################################//
//##                               Mutators                                                     ##//
//################################################################################################//
void Disc2::StretchToIncludePoint( const Vector2& point )
{
	radius += point.CalcLength() - radius;
}

void Disc2::AddPadding( float paddingRadius )
{
	radius += paddingRadius;
}

void Disc2::Translate( const Vector2& translation )
{
	center += translation;	
}

// const Disc2 Disc2::Interpolate( const Disc2& start, const Disc2& end, float fractionFromStartToEnd )
// {
// }

//################################################################################################//
//##                                 Accessors                                                  ##//
//################################################################################################//
bool Disc2::IsPointInside( const Vector2& point ) const
{
	return CalcDistanceSquared(point, center) < radius ;
}

//################################################################################################//
//##                               Operators                                                    ##//
//################################################################################################//
const Disc2 Disc2::operator + ( const Vector2& translation ) const
{
	Disc2 temp;
	temp.radius = radius;
	temp.center.x = center.x + translation.x;
	temp.center.y = center.y + translation.y;
	return temp;
}

const Disc2 Disc2::operator - ( const Vector2& antiTranslation ) const
{
	Disc2 temp;
	temp.radius = radius;
	temp.center.x = center.x - antiTranslation.x;
	temp.center.y = center.y - antiTranslation.y;
	return temp;
}

void Disc2::operator += ( const Vector2& translation )
{
	center.x -= translation.x;
	center.y -= translation.y;
}

void Disc2::operator -= ( const Vector2& antiTranslation )
{
	center.x -= antiTranslation.x;
	center.y -= antiTranslation.y;
}

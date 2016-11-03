//================================================================
//                     Sphere3.cpp
//================================================================

#include "Sphere3.hpp"

//Constructors
Sphere3::~Sphere3()
{
}

Sphere3::Sphere3()
{
}

Sphere3::Sphere3( const Sphere3& copy )
{
	center = copy.center;
	radius = copy.radius;
}

Sphere3::Sphere3( float initialX, float initialY, float initialZ, float initialRadius )
: center(initialX, initialY, initialZ)
, radius(initialRadius)
{
}

Sphere3::Sphere3( const Vector3& initialCenter, float initialRadius )
: center(initialCenter)
, radius(initialRadius)
{
}


//################################################################################################//
//##                               Mutators                                                     ##//
//################################################################################################//
void Sphere3::StretchToIncludePoint( const Vector3& point )
{
	radius += point.CalcLength() - radius;
}

void Sphere3::AddPadding( float paddingRadius )
{
	radius += paddingRadius;
}

void Sphere3::Translate( const Vector3& translation )
{
	center += translation;	
}

// const Sphere3 Sphere3::Interpolate( const Sphere3& start, const Sphere3& end, float fractionFromStartToEnd )
// {
// }

//################################################################################################//
//##                                 Accessors                                                  ##//
//################################################################################################//
bool Sphere3::IsPointInside( const Vector3& point ) const
{
	return CalcDistanceSquared(point, center) < radius*radius ;
}

bool Sphere3::IsPointOutside( const Vector3& point ) const
{
	return CalcDistanceSquared(point, center) > radius*radius ;
}

//################################################################################################//
//##                               Operators                                                    ##//
//################################################################################################//
const Sphere3 Sphere3::operator + ( const Vector3& translation ) const
{
	Sphere3 temp;
	temp.radius = radius;
	temp.center.x = center.x + translation.x;
	temp.center.y = center.y + translation.y;
	temp.center.z = center.z + translation.z;
	return temp;
}

const Sphere3 Sphere3::operator - ( const Vector3& antiTranslation ) const
{
	Sphere3 temp;
	temp.radius = radius;
	temp.center.x = center.x - antiTranslation.x;
	temp.center.y = center.y - antiTranslation.y;
	temp.center.z = center.z - antiTranslation.z;
	return temp;
}

void Sphere3::operator += ( const Vector3& translation )
{
	center.x -= translation.x;
	center.y -= translation.y;
	center.z -= translation.z;
}

void Sphere3::operator -= ( const Vector3& antiTranslation )
{
	center.x -= antiTranslation.x;
	center.y -= antiTranslation.y;
	center.z -= antiTranslation.z;
}

//================================================================
//                     Sphere3.h
//================================================================

#pragma once
#ifndef  __included_Sphere3__
#define  __included_Sphere3__

#include "Vector3.hpp"



class Sphere3
{
public:
	Vector3 center;
	float	radius;

	static const Sphere3 UNIT_CIRCLE;

	//Constructors
	~Sphere3();
	Sphere3();
	Sphere3( const Sphere3& copy );
	explicit Sphere3( float initialX, float initialY, float initialZ, float initialRadius );
	explicit Sphere3( const Vector3& initialCenter, float initialRadius );

	//Mutators:
	void StretchToIncludePoint( const Vector3& point );
	void AddPadding( float paddingRadius );
	void Translate( const Vector3& translation );
	const Sphere3 Interpolate( const Sphere3& start, const Sphere3& end, float fractionFromStartToEnd );

	//Accessors
	bool IsPointInside( const Vector3& point ) const;
	bool IsPointOutside( const Vector3& point ) const;
	//Operators
	const Sphere3 operator + ( const Vector3& translation ) const;
	const Sphere3 operator - ( const Vector3& antiTranslation ) const;
	void operator += ( const Vector3& translation );
	void operator -= ( const Vector3& antiTranslation );

};


#endif
//================================================================
//                     Disc2.h
//================================================================

#pragma once
#ifndef  __included_Disc2__
#define  __included_Disc2__

#include "Vector2.hpp"



class Disc2
{
public:
	Vector2 center;
	float	radius;

	static const Disc2 UNIT_CIRCLE;

	//Constructors
	~Disc2();
	Disc2();
	Disc2( const Disc2& copy );
	explicit Disc2( float initialX, float initialY, float initialRadius );
	explicit Disc2( const Vector2& initialCenter, float initialRadius );

	//Mutators:
	void StretchToIncludePoint( const Vector2& point );
	void AddPadding( float paddingRadius );
	void Translate( const Vector2& translation );
	const Disc2 Interpolate( const Disc2& start, const Disc2& end, float fractionFromStartToEnd );

	//Accessors
	bool IsPointInside( const Vector2& point ) const;

	//Operators
	const Disc2 operator + ( const Vector2& translation ) const;
	const Disc2 operator - ( const Vector2& antiTranslation ) const;
	void operator += ( const Vector2& translation );
	void operator -= ( const Vector2& antiTranslation );
};


#endif
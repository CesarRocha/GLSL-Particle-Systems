//================================================================
//                     LineSegment2.hpp
//================================================================

#include "Vector2.hpp"

#pragma once
#ifndef  __included_LineSegment2__
#define  __included_LineSegment2__

class LineSegment2
{
public:
	Vector2 LineSegmentA;
	Vector2	LineSegmentB;

	//Constructors
	LineSegment2();
	LineSegment2(const Vector2& A, const Vector2& B);
	explicit LineSegment2( float AX, float AY, float BX, float BY );

	//Accessors
	const Vector2 GetPointAtParametricValue( float parametricValue );

	//Mutators
	void Translate( const Vector2& translation );
	const LineSegment2 Interpolate( const LineSegment2& start, const LineSegment2& end, float fractionFromStartToEnd );

	//Operators
	const LineSegment2 operator + ( const Vector2& translation ) const;
	const LineSegment2 operator - ( const Vector2& antiTranslation ) const;
	void  operator += ( const Vector2& translation );
	void  operator -= ( const Vector2& antiTranslation );
};

#endif

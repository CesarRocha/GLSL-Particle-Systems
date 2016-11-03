//================================================================
//                     LineSegment3.hpp
//================================================================

#include "Vector3.hpp"

#pragma once
#ifndef  __included_LineSegment3__
#define  __included_LineSegment3__

class LineSegment3
{
public:
	Vector3 LineSegmentA;
	Vector3	LineSegmentB;

	//Constructors
	LineSegment3();
	LineSegment3(const Vector3& A, const Vector3& B);
	explicit LineSegment3( float AX, float AY, float AZ, float BX, float BY, float BZ );

	//Accessors
	const Vector3 GetPointAtParametricValue( float parametricValue );

	//Mutators
	void Translate( const Vector3& translation );
	const LineSegment3 Interpolate( const LineSegment3& start, const LineSegment3& end, float fractionFromStartToEnd );

	//Operators
	const LineSegment3 operator + ( const Vector3& translation ) const;
	const LineSegment3 operator - ( const Vector3& antiTranslation ) const;
	void  operator += ( const Vector3& translation );
	void  operator -= ( const Vector3& antiTranslation );
};

#endif

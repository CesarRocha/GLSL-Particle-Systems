//================================================================
//                    LineSegment3.cpp
//================================================================

#include "LineSegment3.hpp"

LineSegment3::LineSegment3()
{
}

LineSegment3::LineSegment3(const Vector3& A, const Vector3& B)
	: LineSegmentA(A)
	, LineSegmentB(B)
{
}

LineSegment3::LineSegment3( float AX, float AY, float AZ, float BX, float BY, float BZ )
	: LineSegmentA(AX, AY, AZ)
	, LineSegmentB(BX, BY, BZ)
{
}

//################################################################################################//
//##                               Accessors                                                    ##//
//################################################################################################//
const Vector3 LineSegment3::GetPointAtParametricValue( float parametricValue )
{
	Vector3 DisplacementStartToEnd = LineSegmentB - LineSegmentA;
	Vector3 DisplacementStartToPoint = parametricValue * DisplacementStartToEnd;
	return LineSegmentA + DisplacementStartToPoint;
}

//################################################################################################//
//##                               Mutators                                                     ##//
//################################################################################################//
void LineSegment3::Translate( const Vector3& translation )
{
	LineSegmentA += translation;
	LineSegmentB += translation;
}

const LineSegment3 LineSegment3::Interpolate( const LineSegment3& start, const LineSegment3& end, float fractionFromStartToEnd )
{
	Vector3 FirstVector;
	Vector3 SecondVector;

	return LineSegment3( FirstVector.Interpolate ( start.LineSegmentA, end.LineSegmentA, fractionFromStartToEnd), 
						 SecondVector.Interpolate( start.LineSegmentB, end.LineSegmentB, fractionFromStartToEnd));
}

//################################################################################################//
//##                               Operators                                                    ##//
//################################################################################################//
const LineSegment3 LineSegment3::operator + ( const Vector3& translation ) const
{
	return LineSegment3(LineSegmentA.x + translation.x, LineSegmentA.y + translation.y, LineSegmentA.z + translation.z,
						LineSegmentB.x + translation.x, LineSegmentB.y + translation.y, LineSegmentB.z + translation.z);
}

const LineSegment3 LineSegment3::operator - ( const Vector3& antiTranslation ) const
{
	return LineSegment3(LineSegmentA.x - antiTranslation.x, LineSegmentA.y - antiTranslation.y, LineSegmentA.z - antiTranslation.z,
						LineSegmentB.x - antiTranslation.x, LineSegmentB.y - antiTranslation.y, LineSegmentB.z - antiTranslation.z);
}

void  LineSegment3::operator += ( const Vector3& translation )
{
	LineSegmentA += translation;
	LineSegmentB += translation;
}

void  LineSegment3::operator -= ( const Vector3& antiTranslation )
{
	LineSegmentA -= antiTranslation;
	LineSegmentB -= antiTranslation;
}
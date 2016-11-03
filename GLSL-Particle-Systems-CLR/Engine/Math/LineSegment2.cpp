//================================================================
//                    LineSegment2.cpp
//================================================================

#include "LineSegment2.hpp"

LineSegment2::LineSegment2()
{
}

LineSegment2::LineSegment2(const Vector2& A, const Vector2& B)
	: LineSegmentA(A)
	, LineSegmentB(B)
{
}

LineSegment2::LineSegment2( float AX, float AY, float BX, float BY )
	: LineSegmentA(AX,AY)
	, LineSegmentB(BX,BY)
{
}

//################################################################################################//
//##                               Accessors                                                    ##//
//################################################################################################//
const Vector2 LineSegment2::GetPointAtParametricValue( float parametricValue )
{
	Vector2 DisplacementStartToEnd = LineSegmentB - LineSegmentA;
	Vector2 DisplacementStartToPoint = parametricValue * DisplacementStartToEnd;
	return LineSegmentA + DisplacementStartToPoint;
}

//################################################################################################//
//##                               Mutators                                                     ##//
//################################################################################################//
void LineSegment2::Translate( const Vector2& translation )
{
	LineSegmentA += translation;
	LineSegmentB += translation;
}

const LineSegment2 LineSegment2::Interpolate( const LineSegment2& start, const LineSegment2& end, float fractionFromStartToEnd )
{
	Vector2 FirstVector;
	Vector2 SecondVector;

	return LineSegment2( FirstVector.Interpolate ( start.LineSegmentA, end.LineSegmentA, fractionFromStartToEnd), 
						 SecondVector.Interpolate( start.LineSegmentB, end.LineSegmentB, fractionFromStartToEnd));
}

//################################################################################################//
//##                               Operators                                                    ##//
//################################################################################################//
const LineSegment2 LineSegment2::operator + ( const Vector2& translation ) const
{
	return LineSegment2(LineSegmentA.x + translation.x, LineSegmentA.y + translation.y, 
						LineSegmentB.x + translation.x, LineSegmentB.y + translation.y);
}

const LineSegment2 LineSegment2::operator - ( const Vector2& antiTranslation ) const
{
	return LineSegment2(LineSegmentA.x - antiTranslation.x, LineSegmentA.y - antiTranslation.y, 
						LineSegmentB.x - antiTranslation.x, LineSegmentB.y - antiTranslation.y);
}

void  LineSegment2::operator += ( const Vector2& translation )
{
	LineSegmentA += translation;
	LineSegmentB += translation;
}

void  LineSegment2::operator -= ( const Vector2& antiTranslation )
{
	LineSegmentA -= antiTranslation;
	LineSegmentB -= antiTranslation;
}
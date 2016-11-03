//================================================================
//                     IntVector2.hpp
//================================================================

#pragma once
#ifndef  __included_IntVector2__
#define  __included_IntVector2__
#include "MathUtilities.hpp"
#include "Vector2.hpp"
#include <cmath>

class IntVector2
{
public:
	int x;
	int y;
	static const IntVector2 ZERO;

	//Constructors
	IntVector2();
	~IntVector2();
	IntVector2(const IntVector2& copy);
	explicit IntVector2(int initialX, int initialY);
	

	//Accessors
	void getXY (int& out_x, int& out_y) const;
	const int* GetAsintArray() const;
	int* GetAsintArray();
 	float CalcLength() const;
// 	int CalcLengthSquared() const;
// 	int CalcHeadingDegrees() const;
// 	int CalcHeadingRadians() const;

	//Mutators
	void setXY (int newX, int newY);
	void Rotate90Degrees();
	void RotateNegative90Degrees();
	void RotateDegrees(int degrees);
	void RotateRadians (int radians);
	void Normalize();
	int SetLength (int newLength);
	void ScaleUniform (int scale);
	void ScaleUniform( const IntVector2& perAxisScaleFactors);
	void InverseScaleNonUniform (const IntVector2& perAxisDivisors);
	void SetUnitLengthAndHeadingDegrees (int headingDegrees);
	void SetUnitLengthAndHeadingRadians (int headingRadians);
	void SetLengthAndHeadingDegrees (int newLength, int headingDegrees);
	void SetLengthAndHeadingRadians (int newLength, int headingRadians);
	const IntVector2	Interpolate( const IntVector2& start, const IntVector2& end, int fractionFromStartToEnd );


	//Operators
	bool operator == (const IntVector2& vectorToEqual) const;
	bool operator != (const IntVector2& vectorToNotEqual) const;
	bool operator < (const IntVector2& comp) const;
	const IntVector2 operator+ (const IntVector2& vectorToAdd);
	const IntVector2 operator- (const IntVector2& vectorToSubtract) const;
	const IntVector2 operator* (int scale) const;
	const IntVector2 operator* (const IntVector2& perAxisScaleFactors) const;
	const IntVector2 operator/ (int inverseScale) const;
	
	void operator*= (int scale);
	void operator*= (const IntVector2& perAxisScaleFactors);
	void operator+= (const IntVector2& vectorToAdd);
	void operator-= (const IntVector2& vectorToSubtract);

	//Standalone friend functions
	friend float CalcDistance (const IntVector2& positionA, const IntVector2& positionB);
	friend int CalcDistanceSquared ( const IntVector2& positionA, const IntVector2& positionB);
	friend const IntVector2 operator * (int scale, const IntVector2& vectorToScale);
	friend int DotProduct (const IntVector2& a, const IntVector2& b);
};

#endif
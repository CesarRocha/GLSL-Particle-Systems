//================================================================
//                     Vector3.hpp
//================================================================

#pragma once
#ifndef  __included_Vector3__
#define  __included_Vector3__

#include "Engine/Math/MathUtilities.hpp"
#include <cmath>

class Vector3
{
public:
	float x;
	float y;
	float z;
	static const Vector3 ZERO;
	static const Vector3 XUP;
	static const Vector3 YUP;
	static const Vector3 ZUP;


	//Constructors
	Vector3();
	~Vector3();
	Vector3(const Vector3& copy);
	explicit Vector3(float initialX, float initialY, float initialZ);


	//Accessors
	void getXYZ (float& out_x, float& out_y, float& out_z) const;
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const;
	float CalcLengthSquared() const;
	float CalcLengthSquared2Vectors(const Vector3& a, const Vector3& b)	const;
	float CalcHeadingDegrees() const;
	float CalcHeadingRadians() const;

	//Mutators
	void setXYZ (float newX, float newY, float newZ);
	void Normalize();
	float SetLength (float newLength);
	void ScaleUniform (float scale);
	void ScaleUniform( const Vector3& perAxisScaleFactors);
	void InverseScaleNonUniform (const Vector3& perAxisDivisors);
	void SetUnitLengthAndHeadingDegrees (float headingDegrees);
	void SetUnitLengthAndHeadingRadians (float headingRadians);
	void SetLengthAndHeadingDegrees (float newLength, float headingDegrees);
	void SetLengthAndHeadingRadians (float newLength, float headingRadians);
	const Vector3	Interpolate( const Vector3& start, const Vector3& end, float fractionFromStartToEnd );


	//Operators
	bool operator == (const Vector3& vectorToEqual) const;
	bool operator!= (const Vector3& vectorToNotEqual) const;
	const Vector3 operator+ (const Vector3& vectorToAdd);
	const Vector3 operator- (const Vector3& vectorToSubtract) const;
	const Vector3 operator* (float scale) const;
	const Vector3 operator* (const Vector3& perAxisScaleFactors) const;
	const Vector3 operator/ (float inverseScale) const;
	void operator*= (float scale);
	void operator*= (const Vector3& perAxisScaleFactors);
	void operator/= (const Vector3& scale);
	void operator+= (const Vector3& vectorToAdd);
	void operator-= (const Vector3& vectorToSubtract);

	//Standalone friend functions
	friend float CalcDistance (const Vector3& positionA, const Vector3& positionB);
	friend float CalcDistanceSquared ( const Vector3& positionA, const Vector3& positionB);
	friend const Vector3 operator * (float scale, const Vector3& vectorToScale);
	friend float DotProduct (const Vector3& a, const Vector3& b);

	Vector3 CrossProduct(const Vector3& vector3);
	static void CrossProduct(const Vector3& left, const Vector3& right, Vector3& result);
	void GetRandomPointOnSphere();

	void GetRandomPointOnSphere(Vector3 vectorMin, Vector3 vectorMax);
	static Vector3 GetRandomValuesBetweenVectors(Vector3 a, Vector3 b);
};

#endif
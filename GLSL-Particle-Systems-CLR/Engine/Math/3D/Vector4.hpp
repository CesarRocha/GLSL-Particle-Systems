//================================================================
//                     Vector4.hpp
//================================================================


#pragma once
#ifndef  __included_Vector4__
#define  __included_Vector4__

#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include <cmath>

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;


	//Constructors
	Vector4();
	~Vector4();
	Vector4(const Vector4& copy);
	Vector4(Rgba color);
	explicit Vector4(float initialX, float initialY, float initialZ, float initialW);


	//Accessors
	void getXYZW (float& out_x, float& out_y, float& out_z, float& out_w) const;
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const;
	float CalcLengthSquared() const;

	//Mutators
	void setXYZ (float newX, float newY, float newZ, float newW);
	void Normalize3D();
	void Normalize4D();
	float SetLength (float newLength);
	void ScaleUniform (float scale);
	void ScaleUniform( const Vector4& perAxisScaleFactors);
	void InverseScaleNonUniform (const Vector4& perAxisDivisors);
	void SetUnitLengthAndHeadingDegrees (float headingDegrees);
	void SetUnitLengthAndHeadingRadians (float headingRadians);
	void SetLengthAndHeadingDegrees (float newLength, float headingDegrees);
	void SetLengthAndHeadingRadians (float newLength, float headingRadians);
	const Vector4	Interpolate( const Vector4& start, const Vector4& end, float fractionFromStartToEnd );

	//Operators
	bool operator == (const Vector4& vectorToEqual) const;
	bool operator!= (const Vector4& vectorToNotEqual) const;
	const Vector4 operator+ (const Vector4& vectorToAdd);
	const Vector4 operator- (const Vector4& vectorToSubtract) const;
	const Vector4 operator* (float scale) const;
	const Vector4 operator* (const Vector4& perAxisScaleFactors) const;
	const Vector4 operator/ (float inverseScale) const;
	void operator*= (float scale);
	void operator*= (const Vector4& perAxisScaleFactors);
	void operator+= (const Vector4& vectorToAdd);
	void operator-= (const Vector4& vectorToSubtract);

	//Standalone friend functions
	friend float CalcDistance (const Vector4& positionA, const Vector4& positionB);
	friend float CalcDistanceSquared ( const Vector4& positionA, const Vector4& positionB);
	friend const Vector4 operator * (float scale, const Vector4& vectorToScale);
	friend float DotProduct (const Vector4& a, const Vector4& b);
	
};

#endif
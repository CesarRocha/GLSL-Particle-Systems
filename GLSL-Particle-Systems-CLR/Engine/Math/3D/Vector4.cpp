//================================================================
//                     Vector4.cpp
//================================================================
#include "Vector4.hpp"

Vector4::Vector4()
{
}

Vector4::~Vector4()
{
}

Vector4::Vector4(const Vector4& copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
	w = copy.w;
}

Vector4::Vector4(float initialX, float initialY, float initialZ, float initialW)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
	, w(initialW)
{	
}


Vector4::Vector4(Rgba color)
	: x(color.r)
	, y(color.g)
	, z(color.b)
	, w(color.a)
{
}



//################################################################################################//
//##                                 Accessors                                                  ##//
//################################################################################################//
void	Vector4::getXYZW (float& out_x, float& out_y, float& out_z, float& out_w) const
{
	out_x = x;
	out_y = y; 
	out_z = z;
	out_w = w;
}

float	Vector4::CalcLength()			const
{  
	return sqrt( x*x + y*y + z*z );
}

float	Vector4::CalcLengthSquared()	const
{
	return ( x*x + y*y + z*z);
}

// float	Vector4::CalcHeadingDegrees()	const
// {
// 	return atan2(y,x)*(180.0f/PI);
// }
// 
// float	Vector4::CalcHeadingRadians()	const
// {
// 	return atan2(y,x);
// }


//################################################################################################//
//##                               Mutators                                                     ##//
//################################################################################################//
void Vector4::setXYZ (float newX, float newY, float newZ, float newW)
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

void Vector4::Normalize3D()
{
	float rSquared = Vector4::CalcLengthSquared();

	if (rSquared != 0)
	{
		float length = Vector4::CalcLength();
		float inverseLength = (1.0f / length);
		x = x*inverseLength;
		y = y*inverseLength;
		z = z*inverseLength;
	}
}

void Vector4::Normalize4D()
{
	float rSquared = Vector4::CalcLengthSquared();

	if (rSquared != 0)
	{
		float length = Vector4::CalcLength();
		float inverseLength = (1.0f / length);
		x = x*inverseLength;
		y = y*inverseLength;
		z = z*inverseLength;
		w = w*inverseLength;
	}
}

// float Vector4::SetLength (float newLength)
// {
// }

void Vector4::ScaleUniform (float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

void Vector4::ScaleUniform  (const Vector4& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

void Vector4::InverseScaleNonUniform (const Vector4& perAxisDivisors)
{
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
	z /= perAxisDivisors.y;
	w /= perAxisDivisors.w;
}

void Vector4::SetUnitLengthAndHeadingDegrees (float headingDegrees)
{	
	x = cos(ConvertDegreesToRadians(headingDegrees));
	y = sin(ConvertDegreesToRadians(headingDegrees));
	z = tan(ConvertDegreesToRadians(headingDegrees));
}

void Vector4::SetUnitLengthAndHeadingRadians (float headingRadians)
{
	x = cos(headingRadians);
	y = sin(headingRadians);
	z = tan(headingRadians);
}

void Vector4::SetLengthAndHeadingDegrees (float newLength, float headingDegrees)
{
	x = newLength * cos(ConvertDegreesToRadians(headingDegrees));
	y = newLength * sin(ConvertDegreesToRadians(headingDegrees));
	z = newLength * tan(ConvertDegreesToRadians(headingDegrees));
}

void Vector4::SetLengthAndHeadingRadians (float newLength, float headingRadians)
{
	x = newLength * cos(headingRadians);
	y = newLength * sin(headingRadians);
	z = newLength * tan(headingRadians);
}

const Vector4 Vector4::Interpolate( const Vector4& start, const Vector4& end, float fractionFromStartToEnd )
{	
	return Vector4((1.0f-fractionFromStartToEnd)*start) + (fractionFromStartToEnd*end);
}


//################################################################################################//
//##                               Operators                                                    ##//
//################################################################################################//
bool Vector4::operator == (const Vector4& vectorToEqual) const
{

	return (vectorToEqual.x == vectorToEqual.x && 
		    vectorToEqual.y == vectorToEqual.y && 
		    vectorToEqual.z == vectorToEqual.z &&
			vectorToEqual.w == vectorToEqual.w);
}

bool Vector4::operator != (const Vector4& vectorToNotEqual) const
{
	return (vectorToNotEqual.x != vectorToNotEqual.x && 
		    vectorToNotEqual.y != vectorToNotEqual.y && 
			vectorToNotEqual.z != vectorToNotEqual.z &&
			vectorToNotEqual.w != vectorToNotEqual.w);
}

const Vector4 Vector4::operator + (const Vector4& vectorToAdd)
{
	return Vector4(x + vectorToAdd.x, 
		           y + vectorToAdd.y, 
				   z + vectorToAdd.z,
				   w + vectorToAdd.w);
}

const Vector4 Vector4::operator - (const Vector4& vectorToSubtract) const
{
	return Vector4(x - vectorToSubtract.x, 
		           y - vectorToSubtract.y, 
				   z - vectorToSubtract.z,
				   w - vectorToSubtract.w);
}

const Vector4 Vector4::operator * (float scale) const
{
	return Vector4(x * scale, 
		           y * scale, 
				   z * scale,
				   w * scale);
}

const Vector4 Vector4::operator * (const Vector4& perAxisScaleFactors) const
{
	return Vector4(x * perAxisScaleFactors.x, 
		           y * perAxisScaleFactors.y, 
				   z * perAxisScaleFactors.z,
				   w * perAxisScaleFactors.w);
}

const Vector4 Vector4::operator / (float inverseScale) const
{
	float scaler = 1.0f / inverseScale;
	return Vector4(x * scaler, 
		           y * scaler, 
				   z * scaler,
				   w * scaler);
}

void Vector4::operator *= (float scale)
{
	x *=  scale;
	y *=  scale;
	z *=  scale;
	w *=  scale;
}

void Vector4::operator *= (const Vector4& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.y;
	w *= perAxisScaleFactors.w;
}

void Vector4::operator += (const Vector4& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

void Vector4::operator -= (const Vector4& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
	w -= vectorToSubtract.w;
}

//################################################################################################//
//##                                     Friends                                                ##//
//################################################################################################//
float CalcDistance (const Vector4& positionA, const Vector4& positionB)
{
	float X = positionB.x - positionA.x;
	float Y = positionB.y - positionA.y;
	float Z = positionB.z - positionA.z; 
	return sqrt( X*X + Y*Y + Z*Z);
}

float CalcDistanceSquared ( const Vector4& positionA, const Vector4& positionB)
{
	float X = positionB.x - positionA.x;
	float Y = positionB.y - positionA.y;
	float Z = positionB.z - positionA.z;
	return (  X*X + Y*Y + Z*Z );
}

const Vector4 operator * (float scale, const Vector4& vectorToScale)
{
	return vectorToScale * scale;
}

float DotProduct (const Vector4& a, const Vector4& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) ;
}







//================================================================
//                     Vector3.cpp
//================================================================
#include "Vector3.hpp"

Vector3::Vector3()
	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
{
}

Vector3::~Vector3()
{
}

Vector3::Vector3(const Vector3& copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

Vector3::Vector3(float initialX, float initialY, float initialZ)
: x(initialX)
, y(initialY)
, z(initialZ)
{	
}

const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::XUP(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::YUP(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ZUP(0.0f, 0.0f, 1.0f);
//################################################################################################//
//##                                 Accessors                                                  ##//
//################################################################################################//
void	Vector3::getXYZ (float& out_x, float& out_y, float& out_z) const
{
	out_x = x;
	out_y = y; 
	out_z = z;
}

float	Vector3::CalcLength()			const
{  
	return sqrt( x*x + y*y + z*z );
}

float	Vector3::CalcLengthSquared()	const
{
	return ( x*x + y*y + z*z);
}

float	Vector3::CalcLengthSquared2Vectors(const Vector3& a, const Vector3& b)	const
{
	return ( a.x*b.x + a.y*b.y + a.z*b.z);
}


//################################################################################################//
//##                               Mutators                                                     ##//
//################################################################################################//
void Vector3::setXYZ (float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

void Vector3::Normalize()
{
	float rSquared = Vector3::CalcLengthSquared();

	if (rSquared != 0)
	{
		float length = Vector3::CalcLength();
		float inverseLength = (1.0f / length);
		x = x*inverseLength;
		y = y*inverseLength;
		z = z*inverseLength;
	}
}

// float Vector3::SetLength (float newLength)
// {
// }

void Vector3::ScaleUniform (float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

void Vector3::ScaleUniform  (const Vector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

void Vector3::InverseScaleNonUniform (const Vector3& perAxisDivisors)
{
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
	z /= perAxisDivisors.y;
}

void Vector3::SetUnitLengthAndHeadingDegrees (float headingDegrees)
{	
	x = cos(ConvertDegreesToRadians(headingDegrees));
	y = sin(ConvertDegreesToRadians(headingDegrees));
	z = tan(ConvertDegreesToRadians(headingDegrees));
}

void Vector3::SetUnitLengthAndHeadingRadians (float headingRadians)
{
	x = cos(headingRadians);
	y = sin(headingRadians);
	z = tan(headingRadians);
}

void Vector3::SetLengthAndHeadingDegrees (float newLength, float headingDegrees)
{
	x = newLength * cos(ConvertDegreesToRadians(headingDegrees));
	y = newLength * sin(ConvertDegreesToRadians(headingDegrees));
	z = newLength * tan(ConvertDegreesToRadians(headingDegrees));
}
	
void Vector3::SetLengthAndHeadingRadians (float newLength, float headingRadians)
{
	x = newLength * cos(headingRadians);
	y = newLength * sin(headingRadians);
	z = newLength * tan(headingRadians);
}

const Vector3 Vector3::Interpolate( const Vector3& start, const Vector3& end, float fractionFromStartToEnd )
{	
	return Vector3((1.0f-fractionFromStartToEnd)*start) + (fractionFromStartToEnd*end);
}


//################################################################################################//
//##                               Operators                                                    ##//
//################################################################################################//
bool Vector3::operator == (const Vector3& vectorToEqual) const
{

	return (vectorToEqual.x == vectorToEqual.x && vectorToEqual.y == vectorToEqual.y && vectorToEqual.z == vectorToEqual.z);
}

bool Vector3::operator != (const Vector3& vectorToNotEqual) const
{
	return (vectorToNotEqual.x != vectorToNotEqual.x && vectorToNotEqual.y != vectorToNotEqual.y && vectorToNotEqual.z != vectorToNotEqual.z);
}

const Vector3 Vector3::operator + (const Vector3& vectorToAdd)
{
	return Vector3(x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z);
}


const Vector3 Vector3::operator - (const Vector3& vectorToSubtract) const
{
	return Vector3(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z);
}

const Vector3 Vector3::operator * (float scale) const
{
	return Vector3(x * scale, y * scale, z * scale);
}

const Vector3 Vector3::operator * (const Vector3& perAxisScaleFactors) const
{
	return Vector3(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y, z * perAxisScaleFactors.z);
}

const Vector3 Vector3::operator / (float inverseScale) const
{
	float scaler = 1.0f / inverseScale;
	return Vector3(x * scaler, y * scaler, y * scaler);
}

void Vector3::operator *= (float scale)
{
	x *=  scale;
	y *=  scale;
	z *=  scale;
}

void Vector3::operator *= (const Vector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.y;
}

void Vector3::operator/=(const Vector3& scale)
{
	x /=  scale.x;
	y /=  scale.y;
	z /=  scale.z;
}

void Vector3::operator += (const Vector3& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

void Vector3::operator -= (const Vector3& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}



//################################################################################################//
//##                                     Friends                                                ##//
//################################################################################################//
float CalcDistance (const Vector3& positionA, const Vector3& positionB)
{
	float X = positionB.x - positionA.x;
	float Y = positionB.y - positionA.y;
	float Z = positionB.z - positionA.z; 
	return sqrt( X*X + Y*Y + Z*Z);
}

float CalcDistanceSquared ( const Vector3& positionA, const Vector3& positionB)
{
	float X = positionB.x - positionA.x;
	float Y = positionB.y - positionA.y;
	float Z = positionB.z - positionA.z;
	return (  X*X + Y*Y + Z*Z );
}

const Vector3 operator * (float scale, const Vector3& vectorToScale)
{
	return vectorToScale * scale;
}

float DotProduct (const Vector3& a, const Vector3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) ;
}

void Vector3::CrossProduct(const Vector3& a, const Vector3& b, Vector3& result)
{
	result.x = a.y * b.z - b.y * a.z;
	result.y = a.z * b.x - b.z * a.x;
	result.z = a.x * b.y - b.x * a.y;
	//x =  	y * vec.z - z * vec.y,
	//y = 	z * vec.x - x * vec.z,
	//z = 	x * vec.y - y * vec.x,
}

Vector3 Vector3::CrossProduct(const Vector3& vector3) 
{
	return Vector3(
		y * vector3.z - z * vector3.y,
		z * vector3.x - x * vector3.z,
		x * vector3.y - y * vector3.x);
}

void Vector3::GetRandomPointOnSphere()
{
	do 
	{
		x = GetRandomFloatInRange(-1.0f, 1.0f);
		y = GetRandomFloatInRange(-1.0f, 1.0f);
		z = GetRandomFloatInRange(-1.0f, 1.0f);
	} 
	while(x*x + y*y + z*z > 1.0f);
}
void Vector3::GetRandomPointOnSphere(Vector3 vectorMin, Vector3 vectorMax)
{
	do
	{
		x = GetRandomFloatInRange(vectorMin.x, vectorMax.x);
		y = GetRandomFloatInRange(vectorMin.y, vectorMax.y);
		z = GetRandomFloatInRange(vectorMin.z, vectorMax.z);
	} while (x*x + y*y + z*z > 1.0f);
}

Vector3 Vector3::GetRandomValuesBetweenVectors(Vector3 a, Vector3 b)
{
	float x = GetRandomFloatInRange(a.x, b.x);
	float y = GetRandomFloatInRange(a.y, b.y);
	float z = GetRandomFloatInRange(a.z, b.z);
	return Vector3(x, y, z);
}







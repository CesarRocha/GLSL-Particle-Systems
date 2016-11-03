//================================================================
//                     Vector2.cpp
//================================================================
#include "Vector2.hpp"

Vector2::Vector2()
{
}

Vector2::~Vector2()
{
}

Vector2::Vector2(const Vector2& copy)
{
	x = copy.x;
	y = copy.y;
}

Vector2::Vector2(float initialX, float initialY)
: x(initialX)
, y(initialY)
{	
}

const Vector2 Vector2::ZERO(0.0f, 0.0f);
const Vector2 Vector2::ONE(1.0f, 1.0f);


//################################################################################################//
//##                                 Accessors                                                  ##//
//################################################################################################//
void	Vector2::getXY (float& out_x, float& out_y) const
{
	out_x = x;
	out_y = y; 
}

//const float* GetAsFloatArray() const
//float Vector2::GetAsFloatArray()

float	Vector2::CalcLength()			const //Get R
{  
	return sqrt( x*x + y*y );
}

float	Vector2::CalcLengthSquared()	const  
{
	return ( x*x + y*y );
}

float	Vector2::CalcHeadingDegrees()	const //Get *
{
	return atan2(y,x)*(180/PI);
}

float	Vector2::CalcHeadingRadians()	const
{
	return atan2(y,x);
}


//################################################################################################//
//##                               Mutators                                                     ##//
//################################################################################################//
void Vector2::setXY (float newX, float newY)
{
	x = newX;
	y = newY;
}

void Vector2::Rotate90Degrees()
{
	float temp;
	temp = y;
	y = x;
	x = -temp;
}

void Vector2::RotateNegative90Degrees()
{
	float temp;
	temp = x;
	x = y;
	y = -temp;
}

void Vector2::RotateDegrees(float degrees)
{
	float theta = this->CalcHeadingDegrees();
	float R = this->CalcLength();

	theta += degrees;

	x = R*cos(theta);
	y = R*sin(theta);
}

void Vector2::RotateRadians (float radians)
{

	float theta = this->CalcHeadingDegrees();
	float R = this->CalcLength();
	float degrees;
	degrees = ConvertRadiansToDegrees(radians);
	theta += degrees;

	x = R*cos(theta);
	y = R*sin(theta);
}

void Vector2::Normalize()
{
	float rSquared = Vector2::CalcLengthSquared();

	if (rSquared != 0)
	{
		float length = Vector2::CalcLength();
		float inverseLength = (1.0f / length);
		x = x*inverseLength;
		y = y*inverseLength;
	}
}

// float Vector2::SetLength (float newLength)
// {
// }

void Vector2::ScaleUniform (float scale)
{
	x *= scale;
	y *= scale;
}

void Vector2::ScaleUniform  (const Vector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

void Vector2::InverseScaleNonUniform (const Vector2& perAxisDivisors)
{
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
}

void Vector2::SetUnitLengthAndHeadingDegrees (float headingDegrees)
{	
	x = cos(ConvertDegreesToRadians(headingDegrees));
	y = sin(ConvertDegreesToRadians(headingDegrees));
}

void Vector2::SetUnitLengthAndHeadingRadians (float headingRadians)
{
	x = cos(headingRadians);
	y = sin(headingRadians);
}

void Vector2::SetLengthAndHeadingDegrees (float newLength, float headingDegrees)
{
	x = newLength * cos(ConvertDegreesToRadians(headingDegrees));
	y = newLength * sin(ConvertDegreesToRadians(headingDegrees));
}
	
void Vector2::SetLengthAndHeadingRadians (float newLength, float headingRadians)
{
	x = newLength * cos(headingRadians);
	y = newLength * sin(headingRadians);
}

const Vector2 Vector2::Interpolate( const Vector2& start, const Vector2& end, float fractionFromStartToEnd )
{	
	return Vector2((1.0f-fractionFromStartToEnd)*start) + (fractionFromStartToEnd*end);
	}


//################################################################################################//
//##                               Operators                                                    ##//
//################################################################################################//
bool Vector2::operator == (const Vector2& vectorToEqual) const
{
	return (x == vectorToEqual.x && y == vectorToEqual.y);
}

bool Vector2::operator != (const Vector2& vectorToNotEqual) const
{
	return (vectorToNotEqual.x == vectorToNotEqual.x && vectorToNotEqual.y == vectorToNotEqual.y);
}

const Vector2 Vector2::operator + (const Vector2& vectorToAdd)
{
	return Vector2(x + vectorToAdd.x, y + vectorToAdd.y);
}

const Vector2 Vector2::operator - (const Vector2& vectorToSubtract) const
{
	return Vector2(x - vectorToSubtract.x, y - vectorToSubtract.y);
}

const Vector2 Vector2::operator * (float scale) const
{
	return Vector2(x * scale, y * scale);
}

const Vector2 Vector2::operator * (const Vector2& perAxisScaleFactors) const
{
	return Vector2(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y);
}

const Vector2 Vector2::operator / (float inverseScale) const
{
	return Vector2(x / inverseScale, y / inverseScale);
}

void Vector2::operator *= (float scale)
{
	x *=  scale;
	y *=  scale;
}

void Vector2::operator *= (const Vector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

void Vector2::operator += (const Vector2& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}

void Vector2::operator -= (const Vector2& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}

//################################################################################################//
//##                                     Friends                                                ##//
//################################################################################################//
float CalcDistance (const Vector2& positionA, const Vector2& positionB)
{
	float A = positionB.x - positionA.x;
	float B = positionB.y - positionA.y;
	return sqrt( A*A + B*B );
}

float CalcDistanceSquared ( const Vector2& positionA, const Vector2& positionB)
{
	float A = positionB.x - positionA.x;
	float B = positionB.y - positionA.y;
	return ( A*A + B*B );
}

const Vector2 operator * (float scale, const Vector2& vectorToScale)
{
	return vectorToScale * scale;
}

float DotProduct (const Vector2& a, const Vector2& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

void Vector2::GetRandomPointOnCircle()
{
	do 
	{
		x = GetRandomFloatInRange(-1.0f, 1.0f);
		y = GetRandomFloatInRange(-1.0f, 1.0f);
	} 
	while(x*x + y*y > 1.0f);
}





//================================================================
//                     IntVector2.cpp
//================================================================
#include "IntVector2.hpp"

IntVector2::IntVector2()
{
}

IntVector2::~IntVector2()
{
}

IntVector2::IntVector2(const IntVector2& copy)
{
	x = copy.x;
	y = copy.y;
}

IntVector2::IntVector2(int initialX, int initialY)
	: x(initialX)
	, y(initialY)
{	
}

const IntVector2 IntVector2::ZERO(0, 0);


//################################################################################################//
//##                                 Accessors                                                  ##//
//################################################################################################//
void	IntVector2::getXY (int& out_x, int& out_y) const
{
	out_x = x;
	out_y = y; 
}

//const int* GetAsintArray() const
//int IntVector2::GetAsintArray()
// 
float	IntVector2::CalcLength()			const //Get R
{  
	return sqrt( (float)x*(float)x + (float)y*(float)y );
}
// 
// int	IntVector2::CalcLengthSquared()	const  
// {
// 	return ( x*x + y*y );
// }
// 
// int	IntVector2::CalcHeadingDegrees()	const //Get *
// {
// 	return atan2(y,x)*(180/PI);
// }
// 
// int	IntVector2::CalcHeadingRadians()	const
// {
// 	return atan2(y,x);
// }


//################################################################################################//
//##                               Mutators                                                     ##//
//################################################################################################//
void IntVector2::setXY (int newX, int newY)
{
	x = newX;
	y = newY;
}

// void IntVector2::Rotate90Degrees()
// {
// 	int temp;
// 	temp = y;
// 	y = x;
// 	x = -temp;
// }
// 
// void IntVector2::RotateNegative90Degrees()
// {
// 	int temp;
// 	temp = x;
// 	x = y;
// 	y = -temp;
// }
// 
// void IntVector2::RotateDegrees(int degrees)
// {
// 	int theta = this->CalcHeadingDegrees();
// 	int R = this->CalcLength();
// 
// 	theta += degrees;
// 
// 	x = R*cos(theta);
// 	y = R*sin(theta);
// }
// 
// void IntVector2::RotateRadians (int radians)
// {
// 
// 	int theta = this->CalcHeadingDegrees();
// 	int R = this->CalcLength();
// 	int degrees;
// 	degrees = ConvertRadiansToDegrees(radians);
// 	theta += degrees;
// 
// 	x = R*cos(theta);
// 	y = R*sin(theta);
// }
// 
// void IntVector2::Normalize()
// {
// 	int rSquared = IntVector2::CalcLengthSquared();
// 
// 	if (rSquared != 0)
// 	{
// 		int length = IntVector2::CalcLength();
// 		int inverseLength = (1.0f / length);
// 		x = x*inverseLength;
// 		y = y*inverseLength;
// 	}
// }

// int IntVector2::SetLength (int newLength)
// {
// }

void IntVector2::ScaleUniform (int scale)
{
	x *= scale;
	y *= scale;
}

void IntVector2::ScaleUniform  (const IntVector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

void IntVector2::InverseScaleNonUniform (const IntVector2& perAxisDivisors)
{
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
}

// void IntVector2::SetUnitLengthAndHeadingDegrees (int headingDegrees)
// {	
// 	x = cos(ConvertDegreesToRadians(headingDegrees));
// 	y = sin(ConvertDegreesToRadians(headingDegrees));
// }
// 
// void IntVector2::SetUnitLengthAndHeadingRadians (int headingRadians)
// {
// 	x = cos(headingRadians);
// 	y = sin(headingRadians);
// }
// 
// void IntVector2::SetLengthAndHeadingDegrees (int newLength, int headingDegrees)
// {
// 	x = newLength * cos(ConvertDegreesToRadians(headingDegrees));
// 	y = newLength * sin(ConvertDegreesToRadians(headingDegrees));
// }
// 
// void IntVector2::SetLengthAndHeadingRadians (int newLength, int headingRadians)
// {
// 	x = newLength * cos(headingRadians);
// 	y = newLength * sin(headingRadians);
// }
// 
// const IntVector2 IntVector2::Interpolate( const IntVector2& start, const IntVector2& end, int fractionFromStartToEnd )
// {	
// 	return IntVector2((1.0f-fractionFromStartToEnd)*start) + (fractionFromStartToEnd*end);
// }


//################################################################################################//
//##                               Operators                                                    ##//
//################################################################################################//
bool IntVector2::operator == (const IntVector2& vectorToEqual) const
{
	return (x == vectorToEqual.x && y == vectorToEqual.y);
}

bool IntVector2::operator != (const IntVector2& vectorToNotEqual) const
{
	return (vectorToNotEqual.x == vectorToNotEqual.x && vectorToNotEqual.y == vectorToNotEqual.y);
}

bool IntVector2::operator < (const IntVector2& compare) const
{
	if( y < compare.y )
		return true;
	else if( y > compare.y )
		return false;
	else 
		return( x < compare.x);
}

const IntVector2 IntVector2::operator + (const IntVector2& vectorToAdd)
{
	return IntVector2(x + vectorToAdd.x, y + vectorToAdd.y);
}

const IntVector2 IntVector2::operator - (const IntVector2& vectorToSubtract) const
{
	return IntVector2(x - vectorToSubtract.x, y - vectorToSubtract.y);
}

const IntVector2 IntVector2::operator * (int scale) const
{
	return IntVector2(x * scale, y * scale);
}

const IntVector2 IntVector2::operator * (const IntVector2& perAxisScaleFactors) const
{
	return IntVector2(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y);
}

const IntVector2 IntVector2::operator / (int inverseScale) const
{
	return IntVector2(x / inverseScale, y / inverseScale);
}


void IntVector2::operator *= (int scale)
{
	x *=  scale;
	y *=  scale;
}

void IntVector2::operator *= (const IntVector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

void IntVector2::operator += (const IntVector2& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}

void IntVector2::operator -= (const IntVector2& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}

//################################################################################################//
//##                                     Friends                                                ##//
//################################################################################################//
float CalcDistance (const IntVector2& positionA, const IntVector2& positionB)
{
	float A = (float)positionB.x - (float)positionA.x;
	float B = (float)positionB.y - (float)positionA.y;
	return sqrt( A*A + B*B );
}
// 
// int CalcDistanceSquared ( const IntVector2& positionA, const IntVector2& positionB)
// {
// 	int A = positionB.x - positionA.x;
// 	int B = positionB.y - positionA.y;
// 	return ( A*A + B*B );
// }
// 
// const IntVector2 operator * (int scale, const IntVector2& vectorToScale)
// {
// 	return vectorToScale * scale;
// }
// 
// int DotProduct (const IntVector2& a, const IntVector2& b)
// {
// 	return (a.x * b.x) + (a.y * b.y);
// }







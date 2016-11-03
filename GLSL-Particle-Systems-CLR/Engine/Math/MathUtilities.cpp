#include "MathUtilities.hpp"
#include <stdlib.h>
#include "Engine/Renderer/RGBA.hpp"


//===================================================================================================
float UnsignedCharToFloat(unsigned char unsignedChar)
{
	return FLOAT_PER_UNSIGNEDCHAR * unsignedChar;
}


//===================================================================================================
unsigned char FloatToUnsignedChar(float f)
{
	return (unsigned char)(255.0f * f);
}
//===================================================================================================
float	ConvertRadiansToDegrees (float radians)
{
	return radians*(180.0f/PI);
}


//===================================================================================================
float	ConvertDegreesToRadians (float degrees)
{
	return degrees*(PI/180.0f);
}


//===================================================================================================
int		GetRandomIntLessThan	(int maxValueNotInclusive)
{
	return rand () % maxValueNotInclusive;
}


//===================================================================================================
int		GetRandomIntInRange		(int minValueInclusive, int maxValueInclusive)
{
	return rand() % (maxValueInclusive+1-minValueInclusive) + minValueInclusive;
}


//===================================================================================================
float	GetRandomFloatZeroToOne	()
{
	return ((0-1)*((float)rand()/RAND_MAX))+1;
}


//===================================================================================================
float	GetRandomFloatInRange	(float mininumInclusive, float maximumInclusive)
{
	return ((maximumInclusive-mininumInclusive)*((float)rand()/RAND_MAX))+mininumInclusive;
}


//===================================================================================================
float	CalcShortestAngularDisplacement ( float startAngle, float destinationAngle )
{
	float angularDisplacement = destinationAngle - startAngle;
	while (angularDisplacement > 180.0f)
		angularDisplacement -= 360.0f;
	while (angularDisplacement < -180.0f)
		angularDisplacement += 180.0f;
	return angularDisplacement;
}


//===================================================================================================
bool	IsPowerOfTwo ( int x )
{
	if(x > 1 && (x % 2) == 0)
		return true;
	else return false;
}


//===================================================================================================
// float	RangeMap ( float inValue, float inStart, float inEnd, float outStart, float outEnd )
// {
// }
// 

//===================================================================================================
float	Clamp ( float inValue, float min, float max )
{
	if(min > max)
	{
		float temp = min;
		min = max;
		max = temp;
	}

	if(inValue < min)
		return min;
	else if (inValue > max)
		return max;
	else
		return inValue;
}


//===================================================================================================
float	SmoothStart ( float normalizedValue )
{
	return normalizedValue * normalizedValue;
}


//===================================================================================================
float	SmoothStop ( float normalizedValue )
{
	return 1-((1-normalizedValue)*(1-normalizedValue));
}


//===================================================================================================
float	SmoothStep ( float normalizedValue )
{
	return normalizedValue*normalizedValue*(3 - 2*normalizedValue);
}


//===================================================================================================
float	AsymptoticAdd ( float normalizedValueA, float normalizedValueB )
{
	return normalizedValueA + normalizedValueB;
}


//===================================================================================================
float	Interpolate ( float start, float end, float fractionFromStartToEnd )
{	
	return start + fractionFromStartToEnd * (end - start);
}


//===================================================================================================
char	Interpolate(char start, char end, float fractionFromStartToEnd)
{
	return (char)(start * (1.0f * fractionFromStartToEnd) + (end * fractionFromStartToEnd));
}


//===================================================================================================
unsigned char	Interpolate(unsigned char start, unsigned char end, float fractionFromStartToEnd)
{
	return (unsigned char)(start * (1.0f * fractionFromStartToEnd) + (end * fractionFromStartToEnd));
}



//===================================================================================================
Rgba Interpolate(Rgba start, Rgba end, float fractionFromStartToEnd)
{
	unsigned char r = (unsigned char)(start.r * (1.0f * fractionFromStartToEnd) + (end.r * fractionFromStartToEnd));
	unsigned char g = (unsigned char)(start.g * (1.0f * fractionFromStartToEnd) + (end.g * fractionFromStartToEnd));
	unsigned char b = (unsigned char)(start.b * (1.0f * fractionFromStartToEnd) + (end.b * fractionFromStartToEnd));
	return Rgba(r, g, b);
}

//===================================================================================================
int		RoundDownToInt( float f )
{
	if( f < 0.f )
		return (int)f - 1;
	else 
		return (int)f;
}


//===================================================================================================
int		RoundUpToInt(float f)
{
	f += 1.0;
	return RoundDownToInt(f);
}


//================================================================
//                     MathUtilities.hpp
//================================================================
#pragma once
#ifndef  __included_MathUtilities__
#define  __included_MathUtilities__
#include <ctime>
#include <string>
#include <stdio.h>

const float PI = 3.1415926535897932384626433832795f;
const float TWO_PI = 3.1415926535897932384626433832795f * 2.f;
const float RAD2DEG = 57.295779513082320876798154814105f; // (float)180.0f/PI;
const float DEG2RAD = 0.01745329251994329576923690768489f; // (float)PI/180.0f;
const float RADIANS_PER_DEGREE = 0.01745329251994329576923690768489f;
const float FLOAT_PER_UNSIGNEDCHAR = 1.0f / 255;

float	UnsignedCharToFloat(unsigned char unsignedChar);
unsigned char FloatToUnsignedChar(float f);

float	ConvertRadiansToDegrees (float radians);
float	ConvertDegreesToRadians (float degrees);
int		GetRandomIntLessThan	(int maxValueNotInclusive);
int		GetRandomIntInRange		(int minValueInclusive, int maxValueInclusive);
float	GetRandomFloatZeroToOne	();
float	GetRandomFloatInRange	(float mininumInclusive, float maximumInclusive);

float	CalcShortestAngularDisplacement	( float startAngle, float destinationAngle );
bool	IsPowerOfTwo			( int x );
float	RangeMap				( float inValue, float inStart, float inEnd, float outStart, float outEnd );
float	Clamp					( float inValue, float min, float max );
float	SmoothStart				( float normalizedValue );
float	SmoothStop				( float normalizedValue );
float	SmoothStep				( float normalizedValue );
float	AsymptoticAdd			( float normalizedValueA, float normalizedValueB );
int		RoundDownToInt(float f);
int		RoundUpToInt(float f);

class Rgba;
float			Interpolate(float start, float end, float fractionFromStartToEnd );
char			Interpolate(char start, char end, float fractionFromStartToEnd);
unsigned char	Interpolate(unsigned char start, unsigned char end, float fractionFromStartToEnd);
Rgba			Interpolate(Rgba start, Rgba end, float fractionFromStartToEnd);

	
#endif


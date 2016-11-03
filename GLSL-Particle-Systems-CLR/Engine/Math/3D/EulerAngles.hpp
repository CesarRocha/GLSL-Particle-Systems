//================================================================
//                     EulerAngles.hpp
//================================================================

#pragma once
#ifndef  __included_EulerAngles__
#define  __included_EulerAngles__

class EulerAngles
{
public:	

	EulerAngles() 
		: rollDegreesAboutX(0.0f)
		, pitchDegreesAboutY(0.0f)
		, yawDegreesAboutZ(0.0f)
	{};

	EulerAngles( float initialRoll_X, float initialPitch_Y, float intialYaw_Z) 
		: rollDegreesAboutX(initialRoll_X)
		, pitchDegreesAboutY(initialPitch_Y)
		, yawDegreesAboutZ(intialYaw_Z)
	{};


	float rollDegreesAboutX;
	float pitchDegreesAboutY;
	float yawDegreesAboutZ; 

};

#endif
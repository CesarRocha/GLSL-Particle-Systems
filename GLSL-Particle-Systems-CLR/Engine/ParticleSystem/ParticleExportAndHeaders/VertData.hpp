#pragma once
#ifndef _VertData__
#define _VertData__
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Engine\Utility\Utility.hpp"


//================================================================
//              struct VertData                                == 
//================================================================
struct VertData
{
public:
	VertData();
	void PopulateXMLWithVertData(XMLNode& newVertDataFile);

	Vector2		size2;
	float		size;
	Rgba		color;
	Vector2		positionX;
	Vector2		positionY;
	Vector2		positionZ;
	Vector2		velocityX;
	Vector2		velocityY;
	Vector2		velocityZ;
	Vector2		orientationX;
	Vector2		orientationY;
	Vector2		orientationZ;
	bool		normalizeVelocity;
};


#endif //!_VertData_
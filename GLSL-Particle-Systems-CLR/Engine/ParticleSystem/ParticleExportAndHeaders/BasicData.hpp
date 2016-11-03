#pragma once
#ifndef _BasicData__
#define _BasicData__
#include "Engine/Math/IntVector2.hpp"
#include "Engine\Utility\Utility.hpp"


//================================================================
//              struct BasicData                                == 
//================================================================
struct BasicData
{
public:
	BasicData();
	void PopulateXMLWithBasicData(XMLNode& newBasicDataFile);

	std::string emitterName;
	std::string emitterTexturePath;
	int			spriteCountTotal;
	int			spriteCountX;
	int			spriteCountY;
	bool		emitterAnimated;
	IntVector2	emitterSpriteLocation;
};


#endif //!_BasicData_
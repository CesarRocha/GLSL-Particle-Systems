#pragma once
#ifndef _ContainerXMLData_
#define _ContainerXMLData_
#include "EmitterXMLData.hpp"


//===================================================================================================
//					struct ContainerXMLData														   ==
//===================================================================================================
struct ContainerXMLData
{
public:
	ContainerXMLData();
	void PopulateXMLWithContainerData(XMLNode& xmlNode, std::string newContainerName);
	std::string containerName;
	std::string containerTexturePath;
	Vector3		particlePosition;
	int			spriteCountTotal;
	int			spriteCountX;
	int			spriteCountY;

	//EmitterData
	std::vector<EmitterXMLData*>	xmlEmitterData;
};


#endif // !_ConttainerXMLData_

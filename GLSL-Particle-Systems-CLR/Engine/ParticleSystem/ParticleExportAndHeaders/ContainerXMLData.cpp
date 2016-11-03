#include "ContainerXMLData.hpp"



//===================================================================================================
//					class ContainerXMLData														   ==
//===================================================================================================
ContainerXMLData::ContainerXMLData()
	: containerName("NULL")
	, containerTexturePath("NULL")
	, particlePosition(Vector3::ZERO)
	, spriteCountTotal(0)
	, spriteCountX(0)
	, spriteCountY(0)
{ }



//===================================================================================================
void ContainerXMLData::PopulateXMLWithContainerData(XMLNode& xmlNode, std::string newContainerName)
{
	XMLNode newContainerFile = XMLNode::createXMLTopNode("Container");
	xmlNode.addChild(newContainerFile);

	std::string string;
	newContainerFile.addAttribute("name", newContainerName.c_str());

	if (strcmp("NULL", containerTexturePath.c_str()) != 0)
		newContainerFile.addAttribute("texture", containerTexturePath.c_str());

	string = MakeComplexString("%i", spriteCountTotal);
	newContainerFile.addAttribute("spriteCountTotal", string.c_str());

	string = MakeComplexString("%i", spriteCountX);
	newContainerFile.addAttribute("spriteCountX", string.c_str());

	string = MakeComplexString("%i", spriteCountY);
	newContainerFile.addAttribute("spriteCountY", string.c_str());
}


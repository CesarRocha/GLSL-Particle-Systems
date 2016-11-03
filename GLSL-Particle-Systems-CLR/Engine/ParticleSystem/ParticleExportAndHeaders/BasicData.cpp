#include "BasicData.hpp"


//================================================================
//              class BasicData                                 == 
//================================================================
BasicData::BasicData()
	: emitterName("NULL")
	, emitterTexturePath("NULL")
	, spriteCountTotal(0)
	, spriteCountX(0)
	, spriteCountY(0)
	, emitterAnimated(false)
	, emitterSpriteLocation(IntVector2::ZERO)
{ }



//===================================================================================================
void BasicData::PopulateXMLWithBasicData(XMLNode& newBasicDataFile)
{
	std::string string;
	newBasicDataFile.addAttribute("name", emitterName.c_str());

	if (strcmp("NULL", emitterTexturePath.c_str()) != 0)
	{
		newBasicDataFile.addAttribute("texture", emitterTexturePath.c_str());

		//spriteCountTotal
		string = MakeComplexString("%i", spriteCountTotal);
		newBasicDataFile.addAttribute("spriteCountTotal", string.c_str());

		//spriteCountX
		string = MakeComplexString("%i", spriteCountX);
		newBasicDataFile.addAttribute("spriteCountX", string.c_str());

		//spriteCountY
		string = MakeComplexString("%i", spriteCountY);
		newBasicDataFile.addAttribute("spriteCountY", string.c_str());
	}

	//emitterAnimated
	if (emitterAnimated == true)
		newBasicDataFile.addAttribute("animated", "TRUE");
	else
		newBasicDataFile.addAttribute("animated", "FALSE");

	//spriteLocation
	string = MakeComplexString("%i, %i", emitterSpriteLocation.x, emitterSpriteLocation.y);
	newBasicDataFile.addAttribute("spriteLocation", string.c_str());

}
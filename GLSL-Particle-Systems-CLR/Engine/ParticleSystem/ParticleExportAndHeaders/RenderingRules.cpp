#include "RenderingRules.hpp"


//================================================================
//              class RenderingRules                               == 
//================================================================
RenderingRules::RenderingRules()
	:GLType(NORMAL)
{

}



//===================================================================================================
void RenderingRules::PopulateXMLWithRenderingRules(XMLNode& newRenderingRulesFile)
{
	std::string string;
	string = "NORMAL";

	if (GLType == ADDITIVE)
		string = "ADDITIVE";




	newRenderingRulesFile.addAttribute("GLType", string.c_str());
}



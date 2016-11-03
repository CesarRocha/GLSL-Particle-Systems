#pragma once
#ifndef _RenderingRules__
#define _RenderingRules__
#include "Engine\Utility\Utility.hpp"

enum  EmitterOpenGLSettings
{
	INVALID_SETTING = -1,
	ADDITIVE,
	NORMAL,
	NUM_EMITTER_OPENGL_SETTINGS
};


//================================================================
//              class RenderingRules                            == 
//================================================================
class RenderingRules
{
public:
	RenderingRules();
	void PopulateXMLWithRenderingRules(XMLNode& newRenderingRulesFile);

	EmitterOpenGLSettings GLType;

};


#endif //!_RenderingRules_
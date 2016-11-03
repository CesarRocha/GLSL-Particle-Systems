#pragma once
#ifndef _EmitterXMLData_
#define _EmitterXMLData_
#include "BasicData.hpp"
#include "VertData.hpp"
#include "SpawnRules.hpp"
#include "RenderingRules.hpp"


//===================================================================================================
//				struct EmitterXMLData															   ==
//===================================================================================================
struct EmitterXMLData
{
public:
	EmitterXMLData();
	void PopulateXMLWithEmitterData(XMLNode& xmlNode);

	BasicData		m_basicData;
	VertData		m_vertData;
	SpawnRules		m_spawnRules;
	RenderingRules	m_renderingRules;
};


#endif // !_EmitterXMLData_
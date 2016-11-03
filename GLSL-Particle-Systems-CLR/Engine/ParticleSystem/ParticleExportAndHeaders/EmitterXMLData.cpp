#include "EmitterXMLData.hpp"



//===================================================================================================
//				class EmitterXMLData															   ==
//===================================================================================================
EmitterXMLData::EmitterXMLData()
{

}



//===================================================================================================
void EmitterXMLData::PopulateXMLWithEmitterData(XMLNode& xmlNode)
{
	//Grab the container-level node
	XMLNode newContainerFile = xmlNode.getChildNode("Container");

	//Add an Emitter node to container
	XMLNode newEmitterFile = XMLNode::createXMLTopNode("Emitter");
	newContainerFile.addChild(newEmitterFile);


	//Add a BasicData Node to Emitter node
	XMLNode newBasicDataFile = XMLNode::createXMLTopNode("BasicData");
	newEmitterFile.addChild(newBasicDataFile);
	m_basicData.PopulateXMLWithBasicData(newBasicDataFile);

	//Add a VertData Node to Emitter node
	XMLNode newVertDataFile = XMLNode::createXMLTopNode("VertData");
	newEmitterFile.addChild(newVertDataFile);
	m_vertData.PopulateXMLWithVertData(newVertDataFile);

	//Add a SpawnRules Node to Emitter node
	XMLNode newSpawnRulesFile = XMLNode::createXMLTopNode("SpawnRules");
	newEmitterFile.addChild(newSpawnRulesFile);
	m_spawnRules.PopulateXMLWithSpawnRules(newSpawnRulesFile);

	//Add a RenderingRules Node to Emitter node
	XMLNode renderingRulesFile = XMLNode::createXMLTopNode("RenderingRules");
	newEmitterFile.addChild(renderingRulesFile);
	m_renderingRules.PopulateXMLWithRenderingRules(renderingRulesFile);
}


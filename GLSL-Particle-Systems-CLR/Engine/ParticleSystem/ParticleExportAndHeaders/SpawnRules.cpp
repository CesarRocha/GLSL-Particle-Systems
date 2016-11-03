#include "SpawnRules.hpp"


//================================================================
//              class SpawnRules                               == 
//================================================================
SpawnRules::SpawnRules()
	: initialCount(0)
	, loop(false)
	, renderDelay(0.0f)
	, lifespan(0.0)
	, fadeTime(0.0)
	, spawnRate(Vector2(0.0, 0.0f))
	, spawnRateLoop(false)
{

}



//===================================================================================================
void SpawnRules::PopulateXMLWithSpawnRules(XMLNode& newSpawnRulesFile)
{
	std::string string;
	//initialCount");
	string = MakeComplexString("%i", initialCount);
	newSpawnRulesFile.addAttribute("initialCount", string.c_str());

	//loop");
	if (loop == true)
		newSpawnRulesFile.addAttribute("loop", "TRUE");
	else
		newSpawnRulesFile.addAttribute("loop", "FALSE");

	//renderDelay");//
	string = MakeComplexString("%f", renderDelay);
	newSpawnRulesFile.addAttribute("renderDelay", string.c_str());

	//lifespan");//
	string = MakeComplexString("%f", lifespan);
	newSpawnRulesFile.addAttribute("lifespan", string.c_str());

	//fadeTime");//
	string = MakeComplexString("%f", fadeTime);
	newSpawnRulesFile.addAttribute("fadeTime", string.c_str());

	//spawnRate");//
	string = MakeComplexString("%f ~ %f", spawnRate.x, spawnRate.y);
	newSpawnRulesFile.addAttribute("spawnRate", string.c_str());

	//spawnRateLoop");//
	if (spawnRateLoop == true)
		newSpawnRulesFile.addAttribute("spawnRateLoop", "TRUE");
	else
		newSpawnRulesFile.addAttribute("spawnRateLoop", "FALSE");
}
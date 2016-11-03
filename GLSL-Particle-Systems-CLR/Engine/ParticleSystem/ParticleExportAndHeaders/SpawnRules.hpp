#pragma once
#ifndef _SpawnRules__
#define _SpawnRules__
#include "Engine/Math\Vector2.hpp"
#include "Engine\Utility\Utility.hpp"


//================================================================
//              struct SpawnRules                                == 
//================================================================
struct SpawnRules
{
public:
	SpawnRules();
	void PopulateXMLWithSpawnRules(XMLNode& newSpawnRulesFile);

	int			initialCount;
	bool		loop;
	float		renderDelay;
	float		lifespan;
	float		fadeTime;
	Vector2		spawnRate;
	bool		spawnRateLoop;

};


#endif //!_SpawnRules_
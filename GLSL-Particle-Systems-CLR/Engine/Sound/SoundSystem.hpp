//================================================================
//                     SoundSystem.hpp
//================================================================

#pragma once
#ifndef __included__SoundSystem__
#define __included__SoundSystem__
#include "Engine/Sound/Sound.hpp"
#include <string>
#include <map>

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	void StartUp();
	void Update();
	void Shutdown();
	SoundID LoadStreamingSound( const std::string& soundPath, unsigned int maxNumberOfCopies);
	Sound* PlayStreamingSound(SoundID soundID, int loopCount = 0, float volumeLevel = 1.0f);
	

private:
	std::string m_workingDirectory; 
	bool	m_isValid;
	SoundMap m_soundMap;
	SoundID m_nextSoundID;

};
extern SoundSystem* s_theSoundSystem;

#endif




//================================================================
//                     SoundSystem.cpp
//================================================================

#include "SoundSystem.hpp"
#include "Sound.hpp"
#include "ck/ck.h"
#include <direct.h>

SoundSystem* s_theSoundSystem = NULL;

SoundSystem::SoundSystem()
: m_nextSoundID(1)
{
	if (!s_theSoundSystem)
		s_theSoundSystem = this;
}


SoundSystem::~SoundSystem()
{

}

std::string ConvertBackslashesToForwardSlashes(const std::string& stringWithPossibleBackSlashes)
{
	std::string convertedString = stringWithPossibleBackSlashes;

	size_t charactorindex = 0;

	while(charactorindex != std::string::npos)
	{
		charactorindex = convertedString.find_first_of( '\\', charactorindex);
		if(charactorindex != std::string::npos)
			convertedString.replace( charactorindex, 1, "/");
	}
	return convertedString;
}

void SoundSystem::StartUp()
{
	static CkConfig ckConfig;
	CkInit(&ckConfig);
	m_workingDirectory =_getcwd(NULL,0);
	m_workingDirectory += "/";
	ConvertBackslashesToForwardSlashes(m_workingDirectory);
}

void SoundSystem::Update()
{
	CkUpdate();
				 
	for(SoundMap::iterator soundMapIter = m_soundMap.begin(); soundMapIter != m_soundMap.end(); ++soundMapIter) 
	{
		Sounds& m_soundInstances = soundMapIter->second;
		for(Sounds::iterator soundIter = m_soundInstances.begin(); soundIter != m_soundInstances.end(); ++soundIter )
		{
			Sound* sound = (*soundIter);
			if(sound && sound->NeedsReset() && !sound->isPlaying()) 
				sound->Reset();
		}
	}
}


void SoundSystem::Shutdown()
{
	CkShutdown();
}

SoundID SoundSystem::LoadStreamingSound(const std::string& soundPath, unsigned int maxNumberOfCopies)
{
	SoundID soundID = m_nextSoundID++;
	std::string fullPath = m_workingDirectory; 

	fullPath += soundPath;

	

	Sounds sounds;
	for ( unsigned int i = 0; i != maxNumberOfCopies; ++i)
	{
		Sound* newSound = new Sound(fullPath);
		if(newSound->isValid())
		{	
			while(!newSound->isReady())
				CkUpdate();
			sounds.push_back(newSound);
		}
		else 
		{
			soundID = -1;
			delete newSound;
			break;
		}
	}

	if( soundID != -1)
		m_soundMap[soundID] = sounds;
	
	return soundID;
}

Sound* SoundSystem::PlayStreamingSound(SoundID soundID, int loopCount, float volumeLevel)
{
	Sound* soundPicked = NULL;

	SoundMap::iterator soundIDIter = m_soundMap.find(soundID);
	if (soundIDIter != m_soundMap.end())
	{
		Sounds& sounds = soundIDIter->second;
		for (Sounds::iterator soundIter = sounds.begin(); soundIter != sounds.end(); ++soundIter)
		{
			Sound* sound = (*soundIter);
			if(sound->isPlaying())
				continue;
			if(!sound->isReady())
				continue;

			sound->Play(loopCount, volumeLevel);
			soundPicked = sound;
			break;
		}
	}
	return soundPicked;
}


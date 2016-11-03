//================================================================
//                     Sound.hpp
//================================================================

#pragma once
#ifndef __included__Sound__
#define __included__Sound__

#include <string>
#include <vector>
#include <map>

typedef int SoundID;

struct SoundImpl;

class Sound
{
public:
	Sound(const std::string& soundPath);
	~Sound();

	bool isValid() const {return m_isValid;}
	bool Play(int loopCount, float volumeLevel);
	bool isPlaying();
	bool isReady();
    bool NeedsReset();
	void Reset();
private: 
	bool m_isValid;
	SoundID m_nextSoundID;
	SoundImpl* m_soundImpl;
};
typedef std::vector<Sound*> Sounds;
typedef std::map<SoundID, Sounds> SoundMap;

#endif
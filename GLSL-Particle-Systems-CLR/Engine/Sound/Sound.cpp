//================================================================
//                     Sound.cpp
//================================================================

#include "Sound.hpp"
#include "ck/sound.h"

struct SoundImpl
{
	CkSound* m_sound;
};

Sound::Sound(const std::string& absoluteSoundPath)
	: m_soundImpl(NULL)
	, m_isValid(false)
{
	m_soundImpl = new SoundImpl();
	m_soundImpl->m_sound = CkSound::newStreamSound(absoluteSoundPath.c_str(), kCkPathType_FileSystem);
	if(m_soundImpl->m_sound)
		m_isValid = true;
}

Sound::~Sound()
{

}

bool Sound::Play(int loopCount, float volumeLevel)
{
	if (m_soundImpl && m_soundImpl->m_sound)
	{
		m_soundImpl->m_sound->setLoopCount(loopCount);
		m_soundImpl->m_sound->setVolume(volumeLevel);
		m_soundImpl->m_sound->play();
		return true;
	}
	return false;
}

bool Sound::isPlaying()
{
	if (m_soundImpl && m_soundImpl->m_sound)
		return m_soundImpl->m_sound->isPlaying();
	return false;
}

bool Sound::isReady()
{
	if(m_soundImpl && m_soundImpl->m_sound)
		return m_soundImpl->m_sound->isReady();
	return false;
}

bool Sound::NeedsReset()
{

	return true;
}

void Sound::Reset()
{
	m_soundImpl->m_sound->stop();
	m_soundImpl->m_sound->setPlayPosition(0);
}


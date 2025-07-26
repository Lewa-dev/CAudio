#include "Sound.h"

SoundEngine::Sound::Sound(ALuint bufferID, ALenum format, std::uint32_t samplingRate)
{
	this->bufferID = bufferID;
	this->format = format;
	this->samplingRate = samplingRate;
	
}

ALuint SoundEngine::Sound::getBufferID()
{
	return bufferID;
}

ALenum SoundEngine::Sound::getFormat()
{
	return format;
}

std::uint32_t SoundEngine::Sound::getSamplingRate()
{
	return samplingRate;
}

SoundEngine::Sound::~Sound()
{
	
}
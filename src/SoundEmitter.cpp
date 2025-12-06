#include "ALdebug.h"
#include "SoundEmitter.h"
// #include <chrono>

ALuint SoundEngine::SoundEmitter::getSourceID()
{
    return sourceID;
}

SfxSoundHandle SoundEngine::SoundEmitter::getSoundHandle()
{
    return soundHandle;
}

void SoundEngine::SoundEmitter::play()
{
    alSourcePlay(getSourceID());
}

void SoundEngine::SoundEmitter::stop()
{
    alSourceStop(getSourceID());
}

bool SoundEngine::SoundEmitter::isPlaying()
{
    ALint state;
    alGetSourcei(getSourceID(), AL_SOURCE_STATE, &state);
    if (state != AL_STOPPED)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SoundEngine::SoundEmitter::setPosition(float x, float y, float z)
{
    alSource3f(getSourceID(), AL_POSITION, x, y, z);
}

void SoundEngine::SoundEmitter::setLoop(bool loop)
{
    bool alBool = AL_FALSE;
    if (loop)
    {
        alBool = AL_TRUE;
    }

    alSourcei(getSourceID(), AL_LOOPING, alBool);
}

void SoundEngine::SoundEmitter::setRelative(bool relative)
{
    bool alBool = AL_FALSE;
    if (relative)
    {
        alBool = AL_TRUE;
    }

    /*
    NOTE: Sources/sounds have to be MONO in order to set SOURCE_RELATIVE to false. Otherwise the 3D sound position will not work.
    */

    alSourcei(getSourceID(), AL_SOURCE_RELATIVE, alBool);
}

void SoundEngine::SoundEmitter::setGain(float gain)
{
    alSourcef(getSourceID(), AL_GAIN, ALfloat(gain));
}

void SoundEngine::SoundEmitter::setPitch(float pitch)
{
    alSourcef(getSourceID(), AL_PITCH, ALfloat(pitch));
}

SoundEngine::SoundEmitter::SoundEmitter(SfxSoundHandle soundHandle)
{
    this->soundHandle = soundHandle;
    alGenSources(1, &sourceID);
}

SoundEngine::SoundEmitter::~SoundEmitter()
{
    alDeleteSources(1, &sourceID);
}

#include "SoundCore.h"

#include <cstdio>
#include <iostream>
#include <vector>

#include <locale>
#include <codecvt>

#include <filesystem>
#include "AL/alext.h"
#include "StringExt.h"
#include <cstdint>

bool fileExists(const std::wstring& path)
{
    return std::filesystem::exists(std::filesystem::path(path));
}

namespace SoundEngine
{

    Sound* SoundCore::getSound(SfxSoundHandle hndl)
    {
        return sounds.getElement(hndl);
    }

    SoundEmitter* SoundCore::getEmitter(SfxEmitterHandle hndl)
    {
        return emitters.getElement(hndl);
    }

    SoundStream* SoundCore::getStream(SfxSoundStreamHandle hndl)
    {
        return soundStreams.getElement(hndl);// streamArray[hndl];
    }

    bool SoundCore::emitterIsValid(SfxEmitterHandle hndl)
    {
        return (emitters.hasElement(hndl) && hndl > 0);
    }

    bool SoundCore::soundIsValid(SfxSoundHandle hndl)
    {
        return (sounds.hasElement(hndl) && hndl > 0);
    }

    bool SoundCore::streamIsValid(SfxSoundStreamHandle hndl)
    {
        return (soundStreams.hasElement(hndl) && hndl > 0);
    }

    bool SoundCore::cleanupQueueSounds()
    {

        while (soundsToDelete.empty() == false)
        {
            SfxSoundHandle hndl = soundsToDelete.top();
            if (soundAccessCounter.getCount(unsigned int(hndl)) == 0)
            {
                soundDelete(hndl);
                soundsToDelete.pop();
            }
        }
        return false;
    }

    SoundCore::SoundCore()
    {
        const char* devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

        device = alcOpenDevice(devicename);// NULL
        if (!device)
        {
            // std::cerr << "Error1" << std::endl;
            // system("pause");
        }

        // ALC_STEREO_SOURCES
        // ALC_MONO_SOURCES
        ALCint attrs[] = {
            ALC_HRTF_SOFT, ALC_FALSE, /* disable HRTF */
            0                         /* end of list */
        };
        context = alcCreateContext(device, attrs);
        alcMakeContextCurrent(context);

        if (!context)
        {
            // std::cerr << "Error2" << std::endl;
            // system("pause");
        }
    }

    SoundCore::~SoundCore()
    {

        // first clean the soundsources
        for (unsigned int i = 0; i < emitters.getInternalSize(); i++)
        {
            if (emitters.hasElement(i))
            {
                auto e = emitters.getElement(i);
                emitters.removeElement(i);
                delete e;
            }
        }

        cleanupQueueSounds();

        // then delete the soundarray
        for (int i = 0; i < sounds.getInternalSize(); i++)
        {
            if (sounds.hasElement(i))
            {
                auto e = sounds.getElement(i);
                sounds.removeElement(i);
                delete e;
            }
        }

        // then delete the stream array
        for (unsigned int i = 0; i < soundStreams.getInternalSize(); i++)
        {
            if (soundStreams.hasElement(i))
            {
                streamStop(i);

                auto e = soundStreams.getElement(i);
                soundStreams.removeElement(i);
                delete e;
            }
        }

        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }

    SfxSoundHandle SoundCore::soundCreate(const char* utf8path)
    {
        std::wstring wPath = StringExt::to_wstring(utf8path);

        SfxSoundHandle sID = 0;
        if (fileExists(wPath))
        {

            // load OGG file
            ALenum format;
            ALsizei samplingRate;
            std::uint32_t bitsPerSample;
            std::vector<char> bufferData;

            SoundEngine::DataFetch::LoadOGG(wPath.c_str(), bufferData, format, samplingRate, bitsPerSample);

            // create sound
            ALuint bufferID;
            alGenBuffers(1, &bufferID);

            Sound* snd = new Sound(bufferID, format, samplingRate);

            // Upload sound data to buffer
            alBufferData(snd->getBufferID(), format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), samplingRate);

            // get sound ID
            sID = sounds.addElement(snd);
        }

        return sID;
    }

    int iClamp(int v, int a, int b)
    {
        int r = v;
        if (r < a)
        {
            r = a;
        }
        else
        {
            if (r > b)
            {
                r = b;
            }
        }

        return r;
    }

    void SoundCore::soundInitSetLoopPoints(SfxSoundHandle hndl, float startOffsetTime, float endOffsetTime)
    {
        if (startOffsetTime < endOffsetTime)
        {
            if (soundIsValid(hndl))
            {
                Sound* snd = sounds.getElement(hndl);

                int totalSampleLen = this->soundGetSampleLength(hndl);

                ALint loopPoints[] = {
                    iClamp(snd->getSamplingRate() * startOffsetTime, 0, totalSampleLen),
                    iClamp(snd->getSamplingRate() * endOffsetTime, 0, totalSampleLen)};

                // if 0, then not supported
                // ALint alLoopPointsSoft = alGetEnumValue("AL_LOOP_POINTS_SOFT");
                alBufferiv(snd->getBufferID(), AL_LOOP_POINTS_SOFT, loopPoints);
                ALenum error;
                if ((error = alGetError()) != AL_NO_ERROR)
                {
                    // std::cout << "alGenBuffer Error:" << error << std::endl;
                }
            }
        }
    }

    int SoundCore::soundGetSampleLength(SfxSoundHandle hndl)
    {
        if (soundIsValid(hndl))
        {
            auto e = sounds.getElement(hndl);
            auto bufferID = e->getBufferID();

            ALint size;
            ALint channels;
            ALint bits;

            alGetBufferi(bufferID, AL_SIZE, &size);
            alGetBufferi(bufferID, AL_CHANNELS, &channels);
            alGetBufferi(bufferID, AL_BITS, &bits);

            return (size * 8) / (channels * bits);
        }

        return -1;
    }

    float SoundCore::soundGetDurationSeconds(SfxSoundHandle hndl)
    {
        if (soundIsValid(hndl))
        {
            auto e = sounds.getElement(hndl);
            auto bufferID = e->getBufferID();

            unsigned int sampleLength = soundGetSampleLength(hndl);

            ALint frequency;
            alGetBufferi(bufferID, AL_FREQUENCY, &frequency);

            return static_cast<float>(sampleLength) / static_cast<float>(frequency);
        }

        return -1.0f;
    }

    void SoundCore::soundDelete(SfxSoundHandle hndl)
    {
        if (soundIsValid(hndl))
        {

            // todo: FIX!
            if (soundAccessCounter.getCount(unsigned int(hndl)) == 0)
            {// delete now if no source has this buffer

                auto e = sounds.getElement(hndl);

                ALuint bufferID = e->getBufferID();
                alDeleteBuffers(1, &bufferID);

                sounds.removeElement(hndl);
                delete e;
            }
            else
            {
                // add it to a stack which will be deleted later
                soundsToDelete.push(hndl);
            }
        }
    }

    void SoundCore::emitterDelete(SfxEmitterHandle hndl)
    {
        if (emitterIsValid(hndl))
        {
            // if (emitterArray[hndl] != nullptr) {
            auto e = getEmitter(hndl);
            e->stop();

            soundAccessCounter.decrement(unsigned int(e->getSoundHandle()));

            emitters.removeElement(hndl);

            delete e;
        }
    }

    SfxEmitterHandle SoundCore::emitterCreate(SfxSoundHandle sound)
    {

        SfxEmitterHandle sID = 0;

        // if (freeEmitterIDs.size() > 0) {

        Sound* snd = getSound(sound);

        SoundEmitter* source = new SoundEmitter(sound);

        /*SoundEmitterHandle id = freeEmitterIDs.top();
        freeEmitterIDs.pop();
        emitterArray[id] = source;
        */

        sID = emitters.addElement(source);

        soundAccessCounter.increment(unsigned int(sound));

        // Attach sound buffer to source
        alSourcei(source->getSourceID(), AL_BUFFER, snd->getBufferID());

        SfxEmitterHandle h = sID;
        return h;
    }

    void SoundCore::emitterSetPosition(SfxEmitterHandle hndl, float x, float y, float z)
    {
        if (emitterIsValid(hndl))
        {
            getEmitter(hndl)->setPosition(x, y, z);
        }
    }

    void SoundCore::emitterSetGain(SfxEmitterHandle hndl, float gain)
    {
        if (emitterIsValid(hndl))
        {
            getEmitter(hndl)->setGain(gain);
        }
    }

    void SoundCore::emitterSetPitch(SfxEmitterHandle hndl, float gain)
    {
        if (emitterIsValid(hndl))
        {
            getEmitter(hndl)->setPitch(gain);
        }
    }

    void SoundCore::emitterSetLoop(SfxEmitterHandle hndl, bool loop)
    {
        if (emitterIsValid(hndl))
        {
            getEmitter(hndl)->setLoop(loop);
        }
    }

    void SoundCore::emitterSetRelative(SfxEmitterHandle hndl, bool relative)
    {
        if (emitterIsValid(hndl))
        {
            getEmitter(hndl)->setRelative(relative);
        }
    }

    void SoundCore::listenerSetPosition(float x, float y, float z)
    {
        alListener3f(AL_POSITION, x, y, z);
    }

    void SoundCore::listenerSetOrientation(float xForward, float yForward, float zFoorward, float xUp, float yUp, float zUp)
    {
        ALfloat values[6];

        // forward
        values[0] = xForward;
        values[1] = yForward;
        values[2] = zFoorward;

        // up
        values[3] = xUp;
        values[4] = yUp;
        values[5] = zUp;

        alListenerfv(AL_ORIENTATION, &values[0]);
    }

    void SoundCore::emitterPlay(SfxEmitterHandle hndl)
    {
        if (emitterIsValid(hndl))
        {
            emitters.getElement(hndl)->play();
        }
    }

    void SoundCore::emitterStop(SfxEmitterHandle hndl)
    {
        if (emitterIsValid(hndl))
        {
            emitters.getElement(hndl)->stop();
        }
    }

    bool SoundCore::emitterIsPlaying(SfxEmitterHandle hndl)
    {
        if (emitterIsValid(hndl))
        {
            return emitters.getElement(hndl)->isPlaying();
        }
        else
        {
            return false;
        }
    }

    SfxSoundStreamHandle SoundCore::streamCreate(const char* utf8path)
    {
        std::wstring wPath = StringExt::to_wstring(utf8path);
        SfxSoundStreamHandle sID = 0;
        if (fileExists(wPath))
        {
            SoundStream* stream = new SoundStream(wPath.c_str());

            sID = soundStreams.addElement(stream);
        }
        return sID;
    }

    void SoundCore::streamPlay(SfxSoundStreamHandle hndl)
    {
        if (streamIsValid(hndl))
        {
            soundStreams.getElement(hndl)->play();
        }
    }

    void SoundCore::streamStop(SfxSoundStreamHandle hndl)
    {
        if (streamIsValid(hndl))
        {
            soundStreams.getElement(hndl)->stop();
        }
    }

    void SoundCore::streamResetPlayback(SfxSoundStreamHandle hndl)
    {
        if (streamIsValid(hndl))
        {
            soundStreams.getElement(hndl)->resetPlayback();
        }
    }

    bool SoundCore::streamIsPlaying(SfxSoundStreamHandle hndl)
    {
        if (streamIsValid(hndl))
        {
            return soundStreams.getElement(hndl)->isPlaying();
        }
        else
        {
            return false;
        }
    }

    void SoundCore::streamDelete(SfxSoundStreamHandle hndl)
    {
        // TODO: move deletion of the object to a seperate thread (due to blocking thead join() methods in the destructor)
        // to avoid blocking the game thread

        if (soundStreams.hasElement(hndl))
        {
            if (streamIsValid(hndl))
            {
                SoundStream* stream = soundStreams.getElement(hndl);

                // alSourceStop(getEmitter(hndl.getHandleID())->getSourceID());
                streamStop(hndl);

                soundStreams.removeElement(hndl);
                delete stream;
            }
        }
    }

    void SoundCore::streamLoop(SfxSoundStreamHandle hndl, bool loop)
    {
        if (streamIsValid(hndl))
        {
            soundStreams.getElement(hndl)->setLoop(loop);
        }
    }

    void SoundCore::streamSetGain(SfxSoundStreamHandle hndl, float gain)
    {
        if (streamIsValid(hndl))
        {
            soundStreams.getElement(hndl)->setGain(gain);
        }
    }

    float SoundCore::streamGetGain(SfxSoundStreamHandle hndl)
    {
        if (streamIsValid(hndl))
        {
            return soundStreams.getElement(hndl)->getGain();
        }
        else
        {
            return 0;
        }
    }

    void SoundCore::update()
    {
        // update streams
        /*for (unsigned int i = 0; i < streamArray.size(); i++) {
            if (streamArray[i] != nullptr) {
                streamArray[i]->update();
            }
        }*/

        cleanupQueueSounds();
    }

}

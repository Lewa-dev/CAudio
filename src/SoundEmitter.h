#pragma once
#include "CAudio/SoundHandles.h"
#include "OpenAL.h"
namespace SoundEngine {
	class SoundEmitter {
	private:
		//TODO: don't store soundhandles directly. They should only be exposed for the public interface.
		SfxSoundHandle soundHandle;
		ALuint sourceID;
	public:
		ALuint getSourceID();

		SfxSoundHandle getSoundHandle();

		void play();
		void stop();
		bool isPlaying();

		void setPosition(float x, float y, float z);

		void setLoop(bool loop);
		void setRelative(bool relative);
		void setGain(float gain);
		void setPitch(float pitch);

		SoundEmitter(SfxSoundHandle soundHandle);
		~SoundEmitter();

	};
}
#pragma once
#include "OpenAL.h"
#include <cstdint>
namespace SoundEngine {
	//Sound which will be loaded at once into ram
	class Sound {
	private:
		ALuint bufferID;
		ALenum format;
		ALsizei samplingRate;
	public:
		Sound(ALuint bufferID, ALenum format, std::uint32_t samplingRate);
		ALuint getBufferID();
		ALenum getFormat();
		std::uint32_t getSamplingRate();
		~Sound();
	};
}
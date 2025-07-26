#pragma once
#include "OpenAL.h"
#include "vorbis/vorbisfile.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>


namespace SoundEngine {

	namespace DataFetch {
		class OggFile {
		private:
			FILE *file;
			OggVorbis_File oggFile;
			
			ALenum format;
			
			long bitRate;//in Bits
			long sampleRate;//in bits/second per channel!
			unsigned int channels;//number of channels
		public:
			OggFile(const wchar_t* fileName);
			~OggFile();
			OggVorbis_File* getOggFile();

			ALenum getFormat();
			long getBitRate();
			long getSamplingRate();
			unsigned int getNumChannels();
			unsigned int getBitsPerSample();
		};

		void LoadOGG(const wchar_t *fileName, std::vector<char> &buffer, ALenum &format, ALsizei &samplingRate, std::uint32_t& bitsPerSample);
	}

	
}
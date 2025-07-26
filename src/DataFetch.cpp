#include "DataFetch.h"

#include <Windows.h>

SoundEngine::DataFetch::OggFile::OggFile(const wchar_t * fileName)
{
	//TODO: ADD UTF-8 support

	errno_t err;
	err = _wfopen_s(&file, fileName, L"rb");

	if (err != 0) {
		//error
		system("pause");
	}
	if (file == NULL)
	{
		std::wcerr << "Cannot open " << fileName << " for reading..." << std::endl;
		system("pause");
		exit(-1);
	}

	if (ov_open(file, &oggFile, NULL, 0) != 0)
	{
		std::wcerr << "Error opening " << fileName << " for decoding..." << std::endl;
		system("pause");
		exit(-1);
	}
	// end if
	vorbis_info *pInfo;

	
	// Get some information about the OGG file
	pInfo = ov_info(&oggFile, -1);
	
	long bitRate = 0;
	if (pInfo->bitrate_upper > bitRate) { bitRate = pInfo->bitrate_upper; }
	if (pInfo->bitrate_nominal > bitRate) { bitRate = pInfo->bitrate_nominal; }
	if (pInfo->bitrate_lower > bitRate) { bitRate = pInfo->bitrate_lower; }

	this->bitRate = bitRate;

	this->sampleRate = pInfo->rate;
	this->channels = pInfo->channels;
	//std::cout << sampleRate* pInfo->channels << std::endl;
	//this->bitRate = std::ceil(double(bitRate / 8.0));
	//std::cout << "BYTES: " << this->bitRate << std::endl;
	// Check the number of channels... always use 16-bit samples
	
	if (pInfo->channels == 1)
	{
		format = AL_FORMAT_MONO16;
	}
	else
	{
		format = AL_FORMAT_STEREO16;
	}
}

SoundEngine::DataFetch::OggFile::~OggFile()
{
	ov_clear(&oggFile);
	if (file != NULL) {
		fclose(file);
	}
}

OggVorbis_File * SoundEngine::DataFetch::OggFile::getOggFile()
{
	return &oggFile;
}

ALenum SoundEngine::DataFetch::OggFile::getFormat()
{
	return format;
}

long SoundEngine::DataFetch::OggFile::getBitRate()
{
	return this->bitRate;
}

long SoundEngine::DataFetch::OggFile::getSamplingRate()
{
	return sampleRate;
}

unsigned int SoundEngine::DataFetch::OggFile::getNumChannels()
{
	return channels;
}

unsigned int SoundEngine::DataFetch::OggFile::getBitsPerSample()
{
	return 16;
}

void SoundEngine::DataFetch::LoadOGG(const wchar_t * fileName, std::vector<char>& buffer, ALenum & format, ALsizei & samplingRate,std::uint32_t& bitsPerSample)
{

	int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;

	OggFile* file = new OggFile(fileName);

	format = file->getFormat();
	samplingRate = file->getSamplingRate();
	bitsPerSample = file->getBitsPerSample();

	const unsigned int BUFFER_SIZE = 4096;//32768

	//NOTE: was previously heap allocated
	char array[BUFFER_SIZE];

	do
	{
		// Read up to a buffer's worth of decoded sound data
		bytes = ov_read(file->getOggFile(), array, BUFFER_SIZE, endian, 2, 1, &bitStream);

		if (bytes < 0)
		{
			ov_clear(file->getOggFile());
			std::wcerr << "Error decoding " << fileName << "..." << std::endl;
			exit(-1);
		}

		buffer.insert(buffer.end(), array, array + bytes);
	} while (bytes > 0);

	// Clean up!
	delete file;


}
#include "SoundStream.h"
#include "ALdebug.h"
SoundEngine::SoundStream::SoundStream(const wchar_t * fileName)
{

	data = new InternalData(fileName);

	loadStreamThread = new std::thread(SoundStream::loadStreamTask,&loadData,data);
	playStreamThread = new std::thread(SoundStream::playStreamTask,&playData,data);

	controlStreamThread = new std::thread(SoundStream::controlStreamTask, &loadData, &playData, &controlData, data);

}

SoundEngine::SoundStream::~SoundStream()
{
	playData.run = false;
	loadData.run = false;
	controlData.run = false;
	if (playStreamThread->joinable()) {
		playStreamThread->join();
	}
	
	if (loadStreamThread->joinable()) {
		loadStreamThread->join();
	}

	if (controlStreamThread->joinable()) {
		controlStreamThread->join();
	}
	alSourceStop(data->source);

	//clear buffers (depending on the state, buffers can be either
	//in the emptBuffers queue or in the fullBuffers queue)

	//clear emptyBuffers
	while (data->emptyBuffers.empty() == false) {
		delete data->emptyBuffers.front();
		data->emptyBuffers.pop();
	}

	//clear fullBuffers
	while (data->fullBuffers.empty() == false) {
		delete data->fullBuffers.front();
		data->fullBuffers.pop();
	}

	delete data;

}

void SoundEngine::SoundStream::resetPlayback()
{
	controlData.resetToStart = true;
}

void SoundEngine::SoundStream::stop()
{
	controlData.stopStream = true;
	data->playStream = false;
	//alSourceStop(data->source);
}

void SoundEngine::SoundStream::play()
{
	//data->playStream = true;
	data->playStream = true;
	//controlData.playStream = true;
	//alSourcePlay(data->source); //don't call this as this bugs out streaming.
}

void SoundEngine::SoundStream::setGain(float gain)
{
	alSourcef(data->source, AL_GAIN, gain);
}

float SoundEngine::SoundStream::getGain()
{
	float gain;
	alGetSourcef(data->source, AL_GAIN, &gain);
	return gain;
}

bool SoundEngine::SoundStream::isPlaying()
{
	return data->playStream;// || //loadData.endReached == false || 
}

void SoundEngine::SoundStream::setLoop(bool loop)
{
	if (loop == true) {
		controlData.setLoopTrue = true;
	}
	else {
		controlData.setLoopFalse = true;
	}
}



SoundEngine::SoundStream::Buffer::Buffer(unsigned int bufferSize)
{
	data = new char[bufferSize];//same size as BUFFER_SIZE!!!!
	this->bufferSize = bufferSize;
}

SoundEngine::SoundStream::Buffer::~Buffer()
{
	delete[] data;
}

char * SoundEngine::SoundStream::Buffer::getData()
{
	return data;
}

unsigned int SoundEngine::SoundStream::Buffer::getBufferSize()
{
	return bufferSize;
}

unsigned int SoundEngine::SoundStream::Buffer::getBytesRead()
{
	return bytesRead;
}

void SoundEngine::SoundStream::Buffer::setBytesRead(unsigned int bytesRead)
{
	this->bytesRead = bytesRead;
}

SoundEngine::SoundStream::InternalData::InternalData(const wchar_t * fileName)
{
	alGenBuffers(BUFFER_COUNT, alBuffer);

	alGenSources((ALuint)1, &source);

	file = new DataFetch::OggFile(fileName);

	for (unsigned int i = 0; i < BUFFER_COUNT; i++)
	{

		size_t bytesPerSecond = (file->getBitsPerSample() / 8) * file->getNumChannels() * file->getSamplingRate();

		Buffer* b = new Buffer(bytesPerSecond);//32KB buffer
		emptyBuffers.push(b);
	}
}

SoundEngine::SoundStream::InternalData::~InternalData()
{
	//TODO: Finish

	while (!emptyBuffers.empty()) {
		Buffer* b = emptyBuffers.front();
		emptyBuffers.pop();

		delete b;
	}


	while (!fullBuffers.empty()) {
		Buffer* b = fullBuffers.front();
		fullBuffers.pop();

		delete b;
	}

	Debug::AlerrorAssert();

	//first delete source, then buffer. (otherwise crash occurs)
	alDeleteSources(1, &source);
	Debug::AlerrorAssert();

	alDeleteBuffers(BUFFER_COUNT, alBuffer);//alBuffer[0] is this ok?
	Debug::AlerrorAssert();

}


#include "ALdebug.h"
#include <chrono>
#include "SoundStream.h"

void SoundEngine::SoundStream::controlStreamTask(LoadThreadData* loadData, PlayThreadData* playData, ControlData* controlData, InternalData* data)
{
	while (controlData->run)
	{
		//checks if the stream reached it's end. (thus stopping)
		if (data->playStream == true)
		{
			if (loadData->endReached == true && playData->isPlaying == false) {
				if (data->loop == false)
				{
					data->playStream = false;//stops stream
				}
			}
		}

		if (controlData->startStream == true)
		{
			controlData->startStream = false;
		}

		if (controlData->stopStream == true)
		{
			controlData->stopStream = false;

			//locks threads for critical operation. By far not the most efficient method but it "works".

			playData->threadBlock.lock();
			alSourceStop(data->source);

			//push buffers from Processed state into queue
			ALint buffersProcessed = 0;
			alGetSourcei(data->source, AL_BUFFERS_PROCESSED, &buffersProcessed);

			while (buffersProcessed > 0)
			{
				ALuint alBuffer;
				alSourceUnqueueBuffers(data->source, 1, &alBuffer);
				alSourceQueueBuffers(data->source, 1, &alBuffer);

				buffersProcessed--;

				Debug::AlerrorAssert();
			}

			playData->threadBlock.unlock();
		}

		if (controlData->setLoopTrue == true)
		{
			controlData->setLoopTrue = false;

			data->loop = true;
		}

		if (controlData->setLoopFalse == true)
		{
			controlData->setLoopFalse = false;

			data->loop = false;
		}

		if (controlData->resetToStart == true)
		{
			controlData->resetToStart = false;


			//locks threads for critical operation. By far not the most efficient method but it "works".
			loadData->threadBlock.lock();
			playData->threadBlock.lock();
			loadData->reset = true;

			alSourceStop(data->source);

			ALint buffersProcessed = 0;
			alGetSourcei(data->source, AL_BUFFERS_PROCESSED, &buffersProcessed);
			//unqueue buffers which were processed
			while (buffersProcessed > 0)
			{
				//unqueue buffer
				ALuint alBuffer;
				alSourceUnqueueBuffers(data->source, 1, &alBuffer);
				buffersProcessed--;
			}


			while (!data->fullBuffers.empty())
			{
				Buffer* buffer = data->fullBuffers.front();
				data->fullBuffers.pop();//pop
				buffer->setBytesRead(0);
				data->emptyBuffers.push(buffer);
			}

			playData->alreadyInsertedAlBuffers = 0;

			playData->threadBlock.unlock();
			loadData->threadBlock.unlock();

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void SoundEngine::SoundStream::loadStreamTask(LoadThreadData* loadData, InternalData* data)
{
	int endian = 0;
	int bitStream;

	while (loadData->run)
	{

		loadData->threadBlock.lock();

		if (loadData->reset == true)
		{
			loadData->reset = false;
			ov_time_seek(data->file->getOggFile(), 0);
			loadData->endReached = false;
		}

		if (loadData->endReached == false)
		{
			if (!data->emptyBuffers.empty()) {

				Buffer* buffer = data->emptyBuffers.front();

				int bytesRead = 0;
				bool bufferFilled = false;
				do {
					int bytesToRead = (int)(buffer->getBufferSize() - buffer->getBytesRead());

					//bytesRead = ov_read(data->file->getOggFile(), &buffer->data[bytesRead], bytesToRead, endian, 2, 1, &bitStream);
					bytesRead = ov_read(data->file->getOggFile(), &buffer->getData()[buffer->getBytesRead()], bytesToRead, endian, 2, 1, &bitStream);

					if (bytesRead >= 0) {
						buffer->setBytesRead(buffer->getBytesRead() + bytesRead);
						bufferFilled = true;
					}

				} while (bytesRead > 0 && buffer->getBytesRead() < buffer->getBufferSize());

				if (bufferFilled == true)
				{
					data->emptyBuffers.pop();
					data->fullBuffers.push(buffer);
				}

				if (bytesRead == 0)
				{
					//end of file reached

					if (data->loop == true) {
						loadData->reset = true;
					}
					else {
						loadData->endReached = true;
					}

				}


			}
		}
		loadData->threadBlock.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void SoundEngine::SoundStream::playStreamTask(PlayThreadData* playData, InternalData* data)
{

	while (playData->run)
	{
		playData->threadBlock.lock();
		//------------------- AL Buffer ---------------------------

		bool isPlaying = false;

		if (data->playStream == true)
		{
			if (!data->fullBuffers.empty())
			{
				//unused
				//ALint buffersQueued = 0;
				//alGetSourcei(data->source, AL_BUFFERS_QUEUED, &buffersQueued);

				ALuint alBuffer = 0;//buffer in which we write the Sounddata and push it into OpenAL
				bool alBufferSet = false;//check if the buffer was set by one of the following conditions

				//not all buffers were inserted into the ringBuffer. > Simply take one of the generated buffers
				if (playData->alreadyInsertedAlBuffers < SoundEngine::SoundStream::BUFFER_COUNT)
				{
					alBuffer = data->alBuffer[playData->alreadyInsertedAlBuffers];

					//Source Play has to be called after all new buffers were inserted into the source
					playData->alreadyInsertedAlBuffers++;//is that ok?

					alBufferSet = true;
				}
				else {
					//all generated buffers were at least once in the queue. > They were processed
					//thus reuse them
					ALint buffersProcessed = 0;
					alGetSourcei(data->source, AL_BUFFERS_PROCESSED, &buffersProcessed);

					//unqueue buffers which were processed
					if (buffersProcessed > 0)
					{
						//unqueue buffer
						alSourceUnqueueBuffers(data->source, 1, &alBuffer);

						alBufferSet = true;

						Debug::AlerrorAssert();
					}

				}


				if (alBufferSet == true)
				{
					//get buffer with information loaded from the loadThread
					Buffer* buffer = data->fullBuffers.front();
					data->fullBuffers.pop();//pop

					//copy data from loadBuffer into the AL buffer
					alBufferData(alBuffer, data->file->getFormat(), &buffer->getData()[0], static_cast<ALsizei>(buffer->getBytesRead()), data->file->getSamplingRate());

					//queue AL-buffer
					alSourceQueueBuffers(data->source, 1, &alBuffer);

					Debug::AlerrorAssert();

					//put loaded OGG buffer back as emptybuffer
					buffer->setBytesRead(0);
					data->emptyBuffers.push(buffer);

					isPlaying = true;
				}

				//Fixes issue where a stream stops playing sound because it waits for a queued buffer
				//simply calls alSourcePlay() if the conditional checks determine that the stream isn't over.
				//-----------------------------------------------------------------
				ALint state;
				alGetSourcei(data->source, AL_SOURCE_STATE, &state);

				if (state != AL_PLAYING)
				{
					ALint buffersQueued = 0;
					alGetSourcei(data->source, AL_BUFFERS_QUEUED, &buffersQueued);
					if (buffersQueued > 0) {
						if (data->playStream == true)//check again as another thread might have set it to false
						{
							alSourcePlay(data->source);
						}
					}
				}
				//-----------------------------------------------------------------
			}

			//check if sound has to be stopped
			ALint state;
			alGetSourcei(data->source, AL_SOURCE_STATE, &state);

			if (state == AL_PLAYING)
			{
				isPlaying = true;

				if (data->playStream == false)
				{
					alSourceStop(data->source);
				}
			}
			else
			{
				isPlaying = false;

				/*if (loadData->endReached == true) {
					data->playStream = false;
				}*/
			}

			Debug::AlerrorAssert();
		}

		playData->isPlaying = isPlaying;

		playData->threadBlock.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
#pragma once

//for stream threading

//----------
#include <atomic>
#include "OpenAL.h"
#include "SharedQueue.h"
#include "DataFetch.h"
#include <mutex>

namespace SoundEngine {
		//Music/Sound which will be streamed in realtime from the disk
	class SoundStream {
		static const int BUFFER_COUNT = 3;
	private:
		struct Buffer {
		private:
			char* data = nullptr;
			unsigned int bytesRead = 0;
			unsigned int bufferSize;//also datasize

		public:
			Buffer(unsigned int bufferSize);
			~Buffer();
			char* getData();
			unsigned int getBufferSize();
			unsigned int getBytesRead();
			void setBytesRead(unsigned int bytesRead);
		};

		//various flags which then get processed in the processThread
		struct ControlData {
			std::atomic<bool> run = true;

			std::atomic<bool> resetToStart = false;

			std::atomic<bool> setLoopTrue = false;
			std::atomic<bool> setLoopFalse = false;

			std::atomic<bool> startStream = false;
			std::atomic<bool> stopStream = false;
		};

		//data for loadThread
		struct LoadThreadData {
			std::atomic<bool> run = true;

			std::atomic<bool> endReached = false;
			std::atomic<bool> reset = false;//if we want to reset to the start of the file

			std::mutex threadBlock;
		};

		//data for playThread
		struct PlayThreadData {
			std::atomic<bool> run = true;

			unsigned int alreadyInsertedAlBuffers = 0;
			std::atomic<bool> isPlaying = false;

			std::mutex threadBlock;
		};

		//OpenAL buffers, etc...
		//This data will only be modified from the two internal threads!
		struct InternalData {
			
			//
			std::atomic<bool> loop = false;
			std::atomic<bool> playStream = false;
			//----


			//only accessed in playStreamTask
			ALuint alBuffer[BUFFER_COUNT];//Buffers

			//source is only modified in playStreamTask. 
			ALuint source;
			//std::mutex alSourceModificationMutex;

			//only loaded in loadStreamtask. Playstreamtask only accesses read-only methods (frequency, bitrate values)
			DataFetch::OggFile* file;


			SharedQueue<Buffer*> emptyBuffers;//thread can write into them
			SharedQueue<Buffer*> fullBuffers;//buffers which were written to will be inserted here


			InternalData(const wchar_t* fileName);

			~InternalData();
		};


		std::thread* loadStreamThread;
		std::thread* playStreamThread;
		std::thread* controlStreamThread;

		InternalData* data;
		ControlData controlData;

		//data for threads. (Only threads can access that!)
		PlayThreadData playData;
		LoadThreadData loadData;



		//Streaming functions which are defined in "Streaming.cpp"
		static void controlStreamTask(LoadThreadData* loadData, PlayThreadData* playData, ControlData* controlData, InternalData* data);
		static void loadStreamTask(LoadThreadData* loadData, InternalData* data);
		static void playStreamTask(PlayThreadData* playData,InternalData* data);//will be passed into std::thread

	public:
		SoundStream(const wchar_t* fileName);
		~SoundStream();

		ALuint getSourceID() {
			return data->source;
		}

		void resetPlayback();

		void stop();

		void play();

		void setGain(float gain);

		float getGain();

		bool isPlaying();

		void setLoop(bool loop);

	};
}
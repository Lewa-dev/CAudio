#pragma once
#include"OpenAL.h"
#include "vorbis/vorbisfile.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

#include <thread>


#include "DataFetch.h"
#include "CAudio\SoundHandles.h"

#include "Sound.h"
#include "SoundStream.h"
#include "SoundEmitter.h"

//data
#include <map>
#include <stack>

#include <array>
#include "EntityCollection.h"
#include "AccessCounter.h"


namespace SoundEngine {


	//----------------------

	class SoundCore {
	private:
		static const unsigned int SOUNDARRAYSIZE = 256;
		static const unsigned int soundEmitterArraySize = 256;
		static const unsigned int soundStreamArraySize = 16;

		ALCdevice *device;
		ALCcontext *context;


		EntityCollection<Sound> sounds;
		//std::array<Sound*, SOUNDARRAYSIZE> soundArray;//pointer of pointers
		AccessCounter soundAccessCounter;
		//unsigned int soundAccessCounter[SOUNDARRAYSIZE];//array. If soundSource uses a sound, the counter goes up. > prevents deletion of sounds while a soundsource is still using it
		//std::stack<SoundHandle> freeSoundIDs;
		std::stack<SfxSoundHandle> soundsToDelete;//used if a sound buffer has to be deleted, but is still used by a source. Instead of deleting it upon request it is added to the stack and deleted if the accesscounter falls to 0


	
		EntityCollection<SoundEmitter> emitters;


		EntityCollection<SoundStream> soundStreams;
		//std::array<SoundStream*, soundStreamArraySize> streamArray;//pointer of pointers
		//std::stack<SoundStreamHandle> freeStreamIDs;



		Sound* getSound(SfxSoundHandle hndl);
		SoundEmitter * getEmitter(SfxEmitterHandle hndl);
		SoundStream * getStream(SfxSoundStreamHandle hndl);

		bool emitterIsValid(SfxEmitterHandle hndl);
		bool soundIsValid(SfxSoundHandle hndl);
		bool streamIsValid(SfxSoundStreamHandle hndl);
		//std::unordered_map<std::string, SoundSource*> soundSourceMap;

		bool cleanupQueueSounds();
	public:

		SoundCore();
		~SoundCore();



		//-----------Sound------------------------

		SfxSoundHandle soundCreate(const char* utf8path);

		//call those functions after soundCreate but before the first time playing a sound.
		void soundInitSetLoopPoints(SfxSoundHandle hndl, float startOffsetTime, float endOffsetTime);

		int soundGetSampleLength(SfxSoundHandle hndl);
		float soundGetDurationSeconds(SfxSoundHandle hndl);

		void soundDelete(SfxSoundHandle hndl);


		//-----------Emitter------------------------

		SfxEmitterHandle emitterCreate(SfxSoundHandle sound);

		void emitterDelete(SfxEmitterHandle hndl);

		void emitterSetPosition(SfxEmitterHandle hndl,float x,float y,float z);

		void emitterSetGain(SfxEmitterHandle hndl, float gain);

		void emitterSetPitch(SfxEmitterHandle hndl, float gain);

		void emitterSetLoop(SfxEmitterHandle hndl, bool loop);

		void emitterSetRelative(SfxEmitterHandle hndl, bool loop);

		void emitterPlay(SfxEmitterHandle hndl);

		void emitterStop(SfxEmitterHandle hndl);

		bool emitterIsPlaying(SfxEmitterHandle hndl);


		//-----------Stream------------------------

		SfxSoundStreamHandle streamCreate(const char* utf8path);

		void streamPlay(SfxSoundStreamHandle hndl);

		void streamStop(SfxSoundStreamHandle hndl);
		void streamResetPlayback(SfxSoundStreamHandle hndl);

		bool streamIsPlaying(SfxSoundStreamHandle hndl);

		void streamDelete(SfxSoundStreamHandle hndl);
		
		void streamLoop(SfxSoundStreamHandle hndl,bool loop);

		void streamSetGain(SfxSoundStreamHandle hndl, float gain);
		float streamGetGain(SfxSoundStreamHandle hndl);
		//-----------Listener------------------------
		void listenerSetPosition(float x, float y, float z);
		void listenerSetOrientation(float xForward, float yForward, float zFoorward, float xUp, float yUp, float zUp);


		//------------------------------------------

		void update();//cleanup of sound at runtime,etc..
		
 	};
}
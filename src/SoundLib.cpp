#include "CAudio\SoundLib.h"
#include "SoundCore.h"

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>


SoundEngine::SoundCore* core = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//init
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:

		//delete core;
		//free
		//break;
		break;
	}
	return TRUE;
}





void SfxInit()
{
	core = new SoundEngine::SoundCore();
}

void SfxFree()
{
	if (core != nullptr)
	{
		delete core;
		core = nullptr;
	}
}

void SfxUpdate()
{
	core->update();
}

SfxSoundHandle SfxSoundCreate(const char * utf8path)
{
	return core->soundCreate(utf8path);
}

void SfxSoundInitSetLoopPoints(SfxSoundHandle hndl, float startOffsetTime, float endOffsetTime)
{
	core->soundInitSetLoopPoints(hndl, startOffsetTime, endOffsetTime);
}

void SfxSoundDelete(SfxSoundHandle hndl)
{
	return core->soundDelete(hndl);
}

SfxEmitterHandle SfxEmitterCreate(SfxSoundHandle sound)
{
	return core->emitterCreate(sound);
}

void SfxEmitterDelete(SfxEmitterHandle hndl)
{
	core->emitterDelete(hndl);
}

void SfxEmitterSetPosition(SfxEmitterHandle hndl, CFLOAT x, CFLOAT y, CFLOAT z)
{
	core->emitterSetPosition(hndl, x, y, z);
}

void SfxEmitterSetGain(SfxEmitterHandle hndl, CFLOAT gain)
{
	core->emitterSetGain(hndl, gain);
}

void SfxEmitterSetPitch(SfxEmitterHandle hndl, CFLOAT pitch)
{
	core->emitterSetPitch(hndl, pitch);
}

void SfxEmitterPlay(SfxEmitterHandle hndl)
{
	core->emitterPlay(hndl);
}

DLLEXPc void SfxEmitterSetLoop(SfxEmitterHandle hndl, CBOOL loop)
{
	core->emitterSetLoop(hndl,loop);
}

void SfxEmitterSetRelative(SfxEmitterHandle hndl, CBOOL relative)
{
	core->emitterSetRelative(hndl, relative);
}

void SfxEmitterStop(SfxEmitterHandle hndl)
{
	core->emitterStop(hndl);
}

CBOOL SfxEmitterIsPlaying(SfxEmitterHandle hndl)
{
	return core->emitterIsPlaying(hndl);
}



SfxSoundStreamHandle SfxStreamCreate(char * path)
{
	return core->streamCreate(path);
}

void SfxStreamPlay(SfxSoundStreamHandle handle)
{
	core->streamPlay(handle);
}

CBOOL SfxStreamIsPlaying(SfxSoundStreamHandle handle)
{
	return CBOOL(core->streamIsPlaying(handle));
}

DLLEXPc CBOOL SfxIsEmitterValid(SfxEmitterHandle handle)
{
	return handle != 0;
}

DLLEXPc CBOOL SfxIsSoundValid(SfxSoundHandle handle)
{
	return handle != 0;
}

DLLEXPc CBOOL SfxIsSoundStreamValid(SfxSoundStreamHandle handle)
{
	return handle != 0;
}

void SfxStreamResetPlayback(SfxSoundStreamHandle handle)
{
	core->streamResetPlayback(handle);
}

void SfxStreamStop(SfxSoundStreamHandle handle)
{
	core->streamStop(handle);
}

void SfxStreamDelete(SfxSoundStreamHandle handle)
{
	core->streamDelete(handle);
}

void SfxStreamLoop(SfxSoundStreamHandle handle,CBOOL loop)
{
#if defined(GM_EXPORT)  
	if (loop > 0) {
		core->streamLoop(handle, true);
	}
	else {
		core->streamLoop(handle, false);
	}
#else
	
	core->streamLoop(handle, bool(loop));
#endif

}

DLLEXPc void SfxStreamSetGain(SfxSoundStreamHandle handle, CFLOAT gain)
{
	core->streamSetGain(handle,gain);
}

CFLOAT SfxStreamGetGain(SfxSoundStreamHandle handle)
{
	return core->streamGetGain(handle);
}

DLLEXPc void SfxListenerSetPosition(CFLOAT x, CFLOAT y, CFLOAT z)
{
	core->listenerSetPosition(x, y, z);
}

DLLEXPc void SfxListenerSetOrientation(CFLOAT xForward, CFLOAT yForward, CFLOAT zForward,
	CFLOAT xUp, CFLOAT yUp, CFLOAT zUp)
{
	core->listenerSetOrientation(xForward,yForward,zForward,xUp,yUp,zUp);
}

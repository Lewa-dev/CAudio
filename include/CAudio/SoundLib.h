#pragma once
#include "CAudio/SoundHandles.h"

//public C interface for DLL export
//provides C styled function calls for the SoundCore class

//GM doesn't support bool. 
#if defined(GM_EXPORT)  
typedef double CBOOL;
#else
typedef bool CBOOL;
#endif

#if defined(GM_EXPORT)  
typedef double CFLOAT;
#else
typedef float CFLOAT;
#endif

#define DLLEXPc extern "C" __declspec(dllexport) 

//call once at the beginning to initialize
DLLEXPc void SfxInit();

//call at the end to free resources
DLLEXPc void SfxFree();

//call this every updatecycle/frame in the gameloop
DLLEXPc void SfxUpdate();


//-----------Sound------------------------

DLLEXPc SfxSoundHandle SfxSoundCreate(const char* utf8path);

DLLEXPc void SfxSoundInitSetLoopPoints(SfxSoundHandle hndl, float startOffsetTime, float endOffsetTime);

DLLEXPc void SfxSoundDelete(SfxSoundHandle hndl);


//-----------Emitter------------------------

DLLEXPc SfxEmitterHandle SfxEmitterCreate(SfxSoundHandle sound);

DLLEXPc void SfxEmitterDelete(SfxEmitterHandle hndl);

DLLEXPc void SfxEmitterSetPosition(SfxEmitterHandle hndl, CFLOAT x, CFLOAT y, CFLOAT z);

DLLEXPc void SfxEmitterSetGain(SfxEmitterHandle hndl, CFLOAT gain);
DLLEXPc void SfxEmitterSetPitch(SfxEmitterHandle hndl, CFLOAT pitch);

DLLEXPc void SfxEmitterPlay(SfxEmitterHandle hndl);

DLLEXPc void SfxEmitterSetLoop(SfxEmitterHandle hndl, CBOOL loop);

DLLEXPc void SfxEmitterSetRelative(SfxEmitterHandle hndl, CBOOL relative);

DLLEXPc void SfxEmitterStop(SfxEmitterHandle hndl);

DLLEXPc CBOOL SfxEmitterIsPlaying(SfxEmitterHandle hndl);


//----------------- Stream --------------------

DLLEXPc SfxSoundStreamHandle SfxStreamCreate(char* path);

DLLEXPc void SfxStreamPlay(SfxSoundStreamHandle handle);

DLLEXPc CBOOL SfxStreamIsPlaying(SfxSoundStreamHandle handle);

DLLEXPc CBOOL SfxIsEmitterValid(SfxEmitterHandle handle);

DLLEXPc CBOOL SfxIsSoundValid(SfxSoundHandle handle);

DLLEXPc CBOOL SfxIsSoundStreamValid(SfxSoundStreamHandle handle);

DLLEXPc void SfxStreamResetPlayback(SfxSoundStreamHandle handle);

DLLEXPc void SfxStreamStop(SfxSoundStreamHandle handle);

DLLEXPc void SfxStreamDelete(SfxSoundStreamHandle handle);

DLLEXPc void SfxStreamLoop(SfxSoundStreamHandle handle,CBOOL loop);

DLLEXPc void SfxStreamSetGain(SfxSoundStreamHandle handle, CFLOAT gain);

DLLEXPc CFLOAT SfxStreamGetGain(SfxSoundStreamHandle handle);

DLLEXPc void SfxListenerSetPosition(CFLOAT x, CFLOAT y, CFLOAT z);

DLLEXPc void SfxListenerSetOrientation(CFLOAT xForward, CFLOAT yForward, CFLOAT zForward,
	CFLOAT xUp, CFLOAT yUp, CFLOAT zUp);

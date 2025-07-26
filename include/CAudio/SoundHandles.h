#pragma once


#if defined(GM_EXPORT)  
typedef double SfxSoundHandle;
typedef double SfxSoundStreamHandle;
typedef double SfxEmitterHandle;
#else
typedef unsigned int SfxSoundHandle;
typedef unsigned int SfxSoundStreamHandle;
typedef unsigned int SfxEmitterHandle;
#endif

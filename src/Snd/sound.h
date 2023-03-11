#ifndef SND_SOUND_H_
#define SND_SOUND_H_

#include <stdbool.h>

extern void Snd_Create();
extern void Snd_Destroy();

typedef void* Snd_Source;
extern Snd_Source Snd_CreateSource();
extern void Snd_DestroySource();

typedef void* Snd_AudioClip;
extern Snd_AudioClip Snd_LoadAudioClip();
extern void Snd_UnloadAudioClip();

typedef struct
{
    bool loop;
    float speed;
    float volume;
} Snd_PlaybackParameters;

extern void Snd_PlayAudio(void* source, void* clip, Snd_PlaybackParameters parameters);

#endif
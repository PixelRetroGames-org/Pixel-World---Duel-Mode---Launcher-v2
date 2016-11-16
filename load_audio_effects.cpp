#include "load_audio_effects.h"

Mix_Chunk *DUEL_MODE_hit[3]={NULL,NULL,NULL};
Mix_Chunk *DUEL_MODE_START=NULL;

void Load_Duel_Mode_effects()
{
 DUEL_MODE_hit[0]=Mix_LoadWAV("audio/effects/hit0.wav");
 DUEL_MODE_hit[1]=Mix_LoadWAV("audio/effects/hit1.wav");
 DUEL_MODE_hit[2]=Mix_LoadWAV("audio/effects/hit2.wav");
 DUEL_MODE_START=Mix_LoadWAV("audio/effects/DuelMode.wav");
}

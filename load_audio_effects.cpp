#include "load_audio_effects.h"

Mix_Chunk *DUEL_MODE_hit=NULL;

void Load_Duel_Mode_effects()
{
 DUEL_MODE_hit=Mix_LoadWAV("audio/effects/hit.wav");
}


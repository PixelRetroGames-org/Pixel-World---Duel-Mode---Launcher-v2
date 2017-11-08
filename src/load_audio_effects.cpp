#include "load_audio_effects.h"

Mix_Chunk *DUEL_MODE_hit[3]={NULL,NULL,NULL};
Mix_Music *DUEL_MODE_START=NULL;
bool DUEL_MODE_EFFECTS_LOADED;

Mix_Chunk *CLICK=NULL;
bool CLICK_LOADED;

void Load_Duel_Mode_effects()
{
 if(DUEL_MODE_EFFECTS_LOADED)
    return;
 DUEL_MODE_EFFECTS_LOADED=true;
 DUEL_MODE_hit[0]=Mix_LoadWAV("audio/effects/hit0.wav");
 DUEL_MODE_hit[1]=Mix_LoadWAV("audio/effects/hit1.wav");
 DUEL_MODE_hit[2]=Mix_LoadWAV("audio/effects/hit2.wav");
 DUEL_MODE_START=Mix_LoadMUS("audio/effects/DuelMode.mp3");
}

void Load_Click_effects()
{
 if(CLICK_LOADED)
    return;
 CLICK_LOADED=true;
 CLICK=Mix_LoadWAV("audio/effects/mouse_click.wav");
}

void Clear_Duel_Mode_effects()
{
 for(int i=0;i<3;i++)
     Mix_FreeChunk(DUEL_MODE_hit[i]);
 Mix_FreeMusic(DUEL_MODE_START);
 DUEL_MODE_EFFECTS_LOADED=false;
}

void Clear_Click_effects()
{
 Mix_FreeChunk(CLICK);
 CLICK_LOADED=false;
}

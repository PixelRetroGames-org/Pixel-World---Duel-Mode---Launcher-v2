#ifndef LEVEL_H
#define LEVEL_H

#include "player.h"
#include "map.h"
#include "darkness.h"
#include "non_playable_character.h"
#include "script_interpreter.h"
#include "shop.h"
#include "puzzle.h"
#include "journal.h"
#include "inventory.h"
#include "load_audio_effects.h"
#include "settings.h"
#include "menu.h"

#include <map>
#include <ctime>

#include "SDL/SDL_thread.h"

#ifndef NUMBER_OF_SONGS_MAX_DEFINED
const int NUMBER_OF_SONGS_MAX=20;
#define NUMBER_OF_SONGS_MAX_DEFINED
#endif // NUMBER_OF_SONGS_MAX_DEFINED

const int FRAMES_PER_SECOND=27,ARENA_FRAMES_PER_SECOND=16,PLAYER_MOVES_PER_SECOND=1,RENDER_FRAMES_PER_SECOND=10;

const int MS_PER_UPDATE=25;

const int MUSIC_PAUSE=500;

const int NUMBER_OF_NPC_MAX=20;

extern int number_of_background_music_tracks;
extern Mix_Music* background_music[NUMBER_OF_SONGS_MAX];
extern Timer music_time;
extern bool paused_music;
extern int last_track_played;
extern SDL_Thread* level_music_overseer;
extern bool Oversee_music_quit;
extern SDL_mutex* music_overseer_mutex;

class Level
{
 private:
 int type=0,terrain_type=0,last_terrain_type=0;
 bool done=false;
 int duration=0;
 char name[TEXT_LENGTH_MAX]={NULL};
 char darkness_image_name[TEXT_LENGTH_MAX]={NULL};
 char player_name[3][TEXT_LENGTH_MAX];
 int player_type[3],player_time_blocked[3]={0,0,0},player_time_blocked_attack[3]={0,0,0};
 char arena_name[TEXT_LENGTH_MAX]={NULL},background_music_name[TEXT_LENGTH_MAX]={NULL};
 Player player[3];
 Map arena,effects;
 Map aux;
 std::vector<Map> spell_effects;
 std::map<int,int> spell_effects_ids;
 SDL_Rect arena_size;
 Darkness darkness;
 Timer level_duration;
 SDL_Surface* _screen;
 bool level_changed=false,reset_lag=false;
 int number_of_non_playable_characters=0;
 Non_Playable_Character non_playable_characters[NUMBER_OF_NPC_MAX];
 int non_playable_character_time_blocked[NUMBER_OF_NPC_MAX];
 char non_playable_characters_names[NUMBER_OF_NPC_MAX][TEXT_LENGTH_MAX];
 int winner=0;
 bool skeptic_vision_on=false;
 int skeptic_vision_alpha=0;
 std::pair<int,int> player_map_position[3];

 public:
 Level();
 void Clear(bool terminal=false);
 ///Set
 void Set_arena_size();
 void Set_name(char* _name);
 void Set_player_map_position(int x,int y,int _player);
 void Set_player_mana(int _mana,int _player);
 void Set_player_hp(int _hp,int _player);
 void Set_player_mental_health(int _mental_health,int _player);
 void Set_player_POSX(int _player,int _x);
 void Set_player_LAST_POSX(int _player,int _x);
 void Set_player_velocityX(int _player,int _velocityX);
 void Set_player_velocityY(int _player,int _velocityY);
 ///Get
 int Get_player_map_position_x(int _player);
 int Get_player_map_position_y(int _player);
 char* Get_name();
 ///Load
 void Load();
 void Fast_Reload();
 void Change(char* _level_name);
 ///Update
 void Update_players();
 void Update_all_arena_frames();
 ///Music
 void Start_music();
 void Pause_music();
 void Unpause_music();
 static int Change_music(bool play);
 void Stop_music();
 static int Oversee_music(void* data);
 ///Move
 bool Move_player_X(int _player);
 bool Move_player_Y(int _player);
 bool Move_player(int _player);
 void Move_all_players();
 void Block_all_players();
 void Block_player(int _player);
 void Unblock_all_players();
 void Unblock_player(int _player);
 void Move_NPC();
 ///Attack
 bool Players_can_attack(int _player);
 void Player_basic_attack(int _player);
 ///Time blocked
 void Player_time_blocked_decrease(int _player);
 void Players_time_pass();
 void NPC_time_blocked_decrease();
 void Time_Pass();
 ///Print
 bool Player_is_on_light(int _player);
 bool Non_Playable_Character_is_on_light(int _npc_pos);
 void Print_Map(int x,int y,SDL_Surface* _screen);
 void Print_players_informations(SDL_Surface* _screen);
 void Print_player_information(int _player,SDL_Surface* _screen);
 ///Handle Events
 void Handle_Event(int _player);
 void Handle_Events(SDL_Surface* _screen);
 ///Darkness
 void Darkness_increase();
 void Darkness_decrease();
 ///Buffs
 void Apply_player_buffs(int _player);
 void Apply_all_players_buffs();
 ///Spells
 bool Cast_Spell(int _player,int spell_pos);
 void Decrease_all_Spells_time_blocked(int _player);
 ///Interactions
 void Trigger_player_map(int _player);
 void Trigger_all_players_map();
 void Trigger_around_player_map(int _player);
 void Interact_with_map_textures_around_player(int _player);
 void Interact_with_map_texture(int _player,int x,int y);
 void Interact_with_NPC(int _player,int _npc);
 void Interact_with_NPC_around_player(int _player);
 void Interaction_NPC_player(int _player);
 void Interact_with_clues_around_player(int _player);
 bool Interact_with_clue(int _player,int x,int y);
 ///Artificial Intelligence
 void AI_Make_Move_player(int _player);
 void AI_Block_player(int _player);

 ///Start
 void Set_screen(SDL_Surface* screen);
 void Pause_Menu();
 bool Duel_Mode_Finish_Screen(int _player_winner);
 void Print_Duel_Mode_Finish_Screen(int _player_winner);
 void Setup(char* _level_name);
 void Start(SDL_Surface* _screen,bool cleanup=true);
 void Cleanup();
 void Save_gamemode();
};

int Other_player(int _player);

///Launch
void Launch_Story_Mode(Level* level,SDL_Surface* _screen);
void Launch_Duel_Mode(Level* level,SDL_Surface* _screen);

///Journal
void Open_Journal(std::bitset<NUMBER_OF_MAX_KEYS>* progress,SDL_Surface* _screen);

#endif //LEVEL_H

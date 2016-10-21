#ifndef LEVEL_H
#define LEVEL_H

#include "player.h"
#include "map.h"
#include "darkness.h"
#include<ctime>

#ifndef NUMBER_OF_SONGS_MAX_DEFINED
const int NUMBER_OF_SONGS_MAX=10;
#define NUMBER_OF_SONGS_MAX_DEFINED
#endif // NUMBER_OF_SONGS_MAX_DEFINED

const int FRAMES_PER_SECOND=27,ARENA_FRAMES_PER_SECOND=16,PLAYER_MOVES_PER_SECOND=1,RENDER_FRAMES_PER_SECOND=10;

const int MS_PER_UPDATE=25;

const int MUSIC_PAUSE=5000;

class Level
{
 private:
 int type=0,terrain_type=0;
 bool done=false;
 int duration=0;
 char name[TEXT_LENGHT_MAX]={NULL};
 char darkness_image_name[TEXT_LENGHT_MAX]={NULL};
 char player_name[3][TEXT_LENGHT_MAX];
 int player_type[3],player_time_blocked[3]={0,0,0},player_time_blocked_attack[3]={0,0,0};
 char arena_name[TEXT_LENGHT_MAX]={NULL},background_music_name[TEXT_LENGHT_MAX]={NULL};
 int number_of_background_music_tracks;
 Mix_Music *background_music[NUMBER_OF_SONGS_MAX];
 Player player[3];
 Map arena,effects;
 Map spell_effect;
 SDL_Rect arena_size;
 Darkness darkness;
 Timer level_duration;
 SDL_Surface *_screen;

 public:
 Level();
 void Clear();
 ///Set
 void Set_arena_size();
 void Set_name(char *_name);
 void Set_player_map_position(int x,int y,int _player);
 void Set_player_mana(int _mana,int _player);
 void Set_player_hp(int _hp,int _player);
 void Set_player_mental_health(int _mental_health,int _player);
 void Set_player_POSX(int _player,int _x);
 void Set_player_LAST_POSX(int _player,int _x);
 void Set_player_velocityX(int _player,int _velocityX);
 void Set_player_velocityY(int _player,int _velocityY);
 ///Load
 void Load();
 void Change(char *_level_name);
 ///Update
 void Update_players();
 void Update_all_arena_frames();
 ///Music
 void Start_music();
 void Pause_music();
 void Unpause_music();
 int Change_music(bool play);
 void Stop_music();
 ///Move
 bool Move_player_X(int _player);
 bool Move_player_Y(int _player);
 bool Move_player(int _player);
 void Move_all_players();
 void Block_all_players();
 void Block_player(int _player);
 void Unblock_all_players();
 void Unblock_player(int _player);
 ///Attack
 bool Players_can_attack(int _player);
 void Player_basic_attack(int _player);
 ///Time blocked
 void Player_time_blocked_decrease(int _player);
 void Players_time_pass();
 ///Print
 bool Player_is_on_light(int _player);
 void Print_Map(int x,int y,SDL_Surface *_screen);
 void Print_players_informations(SDL_Surface *_screen);
 void Print_player_information(int _player,SDL_Surface *_screen);
 ///Handle Events
 void Handle_Event(int _player);
 void Handle_Events(SDL_Surface *_screen);
 ///Darkness
 void Darkness_increase();
 void Darkness_decrease();
 ///Buffs
 void Apply_player_buffs(int _player);
 void Apply_all_players_buffs();
 ///Spells
 bool Cast_Spell(int _player,int spell_pos);
 void Decrease_all_Spells_time_blocked(int _player);
 ///Interaction with map
 void Trigger_player_map(int _player);
 void Trigger_all_players_map();
 void Trigger_around_player_map(int _player);

 ///Start
 void Set_screen(SDL_Surface *screen);
 void Pause_Menu();
 void Duel_Mode_Finish_Screen(int _player_winner);
 void Print_Duel_Mode_Finish_Screen(int _player_winner);
 void Setup(char *_level_name);
 void Start(SDL_Surface *_screen);
};

int Other_player(int _player);

#endif //LEVEL_H

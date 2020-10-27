#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <stdbool.h>
#include <android/log.h>

typedef struct Game Game;
struct Game {
  int win_w;
  int win_h;
  float win_scale; //480:768に対する画面サイズ倍率
};

typedef enum {
  SCENE_NONE,
  SCENE_OPENING,
  SCENE_STAGE_SELECT,
  SCENE_PLAY,
  SCENE_CARD_PLAY,
}SceneType;

typedef struct Input Input;
struct Input {
  int up;
  int down;
  int left;
  int right;
  bool enter;
  bool esc;
  bool space;
  bool click;
  struct SDL_Point mouse_pos;
};

typedef enum {
  Dir_L = 0,
  Dir_R = 1,
}DirKind;

typedef enum {
  Act_Stand,
  Act_Stand_Atack,
  Act_Walk,
  Act_Wink,
  Act_See,
  Act_Up,
  Act_Dead,
  Act_Damage,
}ActState;

typedef struct Character Character;
struct Character {
  int x;
  int y;
  int prev_x;
  DirKind dir;
  ActState action;
  bool is_animation;
  int animation_start_tick;
  float animation_duration; // tick
  bool is_damage;
  int dp; // damage point
  int max_hp; // hit point
  int hp; // hit point
};

const static int UNIT_SIZE = 32;

typedef enum {
  E_NONE,
  E_UP,
  E_DOWN,
  E_RESET,
}EnergyStatus;


void scene_opening_initialize(SDL_Renderer *render);
void scene_opening_finalize();
bool scene_opening_update(Input *input, SceneType *next);
void scene_opening_draw(SDL_Renderer *render);

void scene_stage_sel_initialize(SDL_Renderer *render);
void scene_stage_sel_finalize();
bool scene_stage_sel_update(Input *input, SceneType *next);
void scene_stage_sel_draw(SDL_Renderer *render);

void scene_play_initialize(SDL_Renderer *render);
void scene_play_finalize();
bool scene_play_update(Input *input, SceneType *next);
void scene_play_draw(SDL_Renderer *render);
bool scene_play_pos_validation(int x, int y);

void scene_card_play_initialize(SDL_Renderer *render);
void scene_card_play_finalize();
bool scene_card_play_update(Input *input, SceneType *next);
void scene_card_play_draw(SDL_Renderer *render);

void human_initialize(SDL_Renderer *render);
void human_finalize();
bool human_update(Input *input, bool damage, int damagePoint, EnergyStatus energy);
void human_draw(SDL_Renderer *render);

void enemy_initialize(SDL_Renderer *render);
void enemy_finalize();
void enemy_update(Input *input, Character *e, int count);
void enemy_draw(SDL_Renderer *render, Character *e, int count);

void draw_text(SDL_Renderer *render, SDL_Texture *tex, int x, int y);

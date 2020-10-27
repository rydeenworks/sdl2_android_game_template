#include "game.h"

TTF_Font *font_title = NULL;
SDL_Surface *img_title = NULL;
SDL_Texture *tex_title = NULL;
SDL_Surface *img_title2 = NULL;
SDL_Texture *tex_title2 = NULL;



TTF_Font *font_menu = NULL;
SDL_Surface *img_start = NULL;
SDL_Texture *tex_start = NULL;
SDL_Surface *img_end = NULL;
SDL_Texture *tex_end = NULL;


SDL_Surface *img_gold = NULL;
SDL_Texture *tex_gold = NULL;

SDL_Surface *img_wall = NULL;
SDL_Texture *tex_wall = NULL;

extern Game game;

void scene_opening_initialize(SDL_Renderer *render) {
  font_title = TTF_OpenFont("Pixelbroidery-0n0G.ttf", 80);
  img_title = TTF_RenderUTF8_Blended(font_title, "GoldRush", (SDL_Color){255,255,255,255});
  tex_title =  SDL_CreateTextureFromSurface(render, img_title);
  img_title2 = TTF_RenderUTF8_Blended(font_title, "Dungeon", (SDL_Color){255,255,255,255});
  tex_title2 =  SDL_CreateTextureFromSurface(render, img_title2);

  font_menu = TTF_OpenFont("PixelMplus12-Regular.ttf", 24);
  img_start = TTF_RenderUTF8_Blended(font_menu, "ゲーム開始 : Enterキー", (SDL_Color){255,255,255,255});
  tex_start =  SDL_CreateTextureFromSurface(render, img_start);

  img_end = TTF_RenderUTF8_Blended(font_menu, "ゲーム終了 : Escキー", (SDL_Color){255,255,255,255});
  tex_end =  SDL_CreateTextureFromSurface(render, img_end);

  img_gold = IMG_Load("gold.png");
  SDL_SetColorKey( img_gold, SDL_TRUE, SDL_MapRGB(img_gold->format, 71, 108, 108));
  tex_gold = SDL_CreateTextureFromSurface(render, img_gold);

  img_wall = IMG_Load("wall.png");
  SDL_SetColorKey( img_wall, SDL_TRUE, SDL_MapRGB(img_wall->format, 71, 108, 108));
  tex_wall = SDL_CreateTextureFromSurface(render, img_wall);

}
void scene_opening_finalize() {
  SDL_DestroyTexture(tex_wall);
  SDL_FreeSurface(img_wall);

  SDL_DestroyTexture(tex_gold);
  SDL_FreeSurface(img_gold);

  SDL_DestroyTexture(tex_end);
  SDL_FreeSurface(img_end);

  SDL_DestroyTexture(tex_start);
  SDL_FreeSurface(img_start);
  TTF_CloseFont(font_menu);

  SDL_DestroyTexture(tex_title2);
  SDL_FreeSurface(img_title2);
  SDL_DestroyTexture(tex_title);
  SDL_FreeSurface(img_title);

  TTF_CloseFont(font_title);
}
bool scene_opening_update(Input *input, SceneType *next) {
  if (input->enter) {
    *next = SCENE_STAGE_SELECT;
  } else if (input->esc) {
    return false;
  }
  return true;
}
void scene_opening_draw(SDL_Renderer *render) {
  draw_text(render, tex_title, 70, 100);
  draw_text(render, tex_title2, 80, 200);
  draw_text(render, tex_start, 110, 400);
  draw_text(render, tex_end, 110, 450);

  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderDrawLine(render, 0, 0, game.win_w, game.win_h);

  {
    static float UNIT_SCALE = 4 * game.win_scale;
    int w,h;
    SDL_QueryTexture(tex_gold, NULL, NULL, &w, &h);
    SDL_Rect srcRect=(SDL_Rect){0,0,w,h};
    {
      SDL_Rect dstRect=(SDL_Rect){
        (int)(150 * game.win_scale),
        (int)(160 * game.win_scale),
        (int)(w * UNIT_SCALE),
        (int)(h * UNIT_SCALE)};
      SDL_RenderCopy(render, tex_gold, &srcRect, &dstRect);
    }
    {
      SDL_Rect dstRect=(SDL_Rect){
        (int)(200 * game.win_scale),
        (int)(160 * game.win_scale),
        (int)(w * UNIT_SCALE),
        (int)(h * UNIT_SCALE)};
      SDL_RenderCopy(render, tex_gold, &srcRect, &dstRect);
    }
    {
      SDL_Rect dstRect=(SDL_Rect){
        (int)(250 * game.win_scale),
        (int)(160 * game.win_scale),
        (int)(w * UNIT_SCALE),
        (int)(h * UNIT_SCALE)};
      SDL_RenderCopy(render, tex_gold, &srcRect, &dstRect);
    }
  }

}

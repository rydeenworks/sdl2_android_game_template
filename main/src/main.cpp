#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL_ttf.h"

#include "game.h"

static char * icon_xpm[] = {
        "32 23 3 1",
        "     c #FFFFFF",
        ".    c #000000",
        "+    c #FFFF00",
        "                                ",
        "            ........            ",
        "          ..++++++++..          ",
        "         .++++++++++++.         ",
        "        .++++++++++++++.        ",
        "       .++++++++++++++++.       ",
        "      .++++++++++++++++++.      ",
        "      .+++....++++....+++.      ",
        "     .++++.. .++++.. .++++.     ",
        "     .++++....++++....++++.     ",
        "     .++++++++++++++++++++.     ",
        "     .++++++++++++++++++++.     ",
        "     .+++++++++..+++++++++.     ",
        "     .+++++++++..+++++++++.     ",
        "     .++++++++++++++++++++.     ",
        "      .++++++++++++++++++.      ",
        "      .++...++++++++...++.      ",
        "       .++............++.       ",
        "        .++..........++.        ",
        "         .+++......+++.         ",
        "          ..++++++++..          ",
        "            ........            ",
        "                                "};

Game game;

SDL_Window *window;
SDL_Renderer *render;

int WIN_W = 480;
int WIN_H = 768;

bool running = true;
int frameCount, timerFPS, lastFrame, fps;

Input in;

SceneType scene = SCENE_NONE;

void switch_scene(SceneType next) {
  switch(scene) {
  case SCENE_OPENING: scene_opening_finalize(); break;
    //  case SCENE_STAGE_SELECT: scene_stage_sel_finalize(); break;
    //  case SCENE_PLAY: scene_play_finalize(); break;
    //  case SCENE_CARD_PLAY: scene_card_play_finalize(); break;
  default: break;
  }

  scene = next;

  switch(scene) {
  case SCENE_OPENING: scene_opening_initialize(render); break;
    //  case SCENE_STAGE_SELECT: scene_stage_sel_initialize(render); break;
    //  case SCENE_PLAY: scene_play_initialize(render); break;
    //  case SCENE_CARD_PLAY: scene_card_play_initialize(render); break;
  default: break;
  }
}

void draw_text(SDL_Renderer *render, SDL_Texture *tex, int x, int y) {
  int w,h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  SDL_Rect src=(SDL_Rect){0,0,w,h};
  SDL_Rect dst=(SDL_Rect){
    (int)(x * game.win_scale),
    (int)(y * game.win_scale),
    (int)(w * game.win_scale),
    (int)(h * game.win_scale)};
  SDL_RenderCopy(render, tex, &src, &dst);
}

void update(SceneType *next) {
  switch(scene) {
  case SCENE_OPENING: running = scene_opening_update(&in, next); break;
    //  case SCENE_STAGE_SELECT: running = scene_stage_sel_update(&in, next); break;
    //  case SCENE_PLAY: running = scene_play_update(&in, next); break;
    //  case SCENE_CARD_PLAY: running = scene_card_play_update(&in, next); break;
  default: break;
  }
}

void input() {
  in.up = in.down = in.left = in.right = 0;
  in.enter = in.esc = in.space = in.click = false;
  in.mouse_pos.x = in.mouse_pos.y = 0;

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) running = false;
    else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_UP: in.up = 1; break;
      case SDLK_DOWN: in.down = 1; break;
      case SDLK_LEFT: in.left = 1; break;
      case SDLK_RIGHT: in.right = 1; break;
      case SDLK_RETURN: in.enter = true; break;
      case SDLK_ESCAPE: in.esc = true; break;
      case SDLK_SPACE: in.space = true; break;
      }
    }else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
      in.click = true;
      SDL_GetMouseState(&in.mouse_pos.x, &in.mouse_pos.y);
    }
  }
}

void draw() {
  SDL_SetRenderDrawColor(render, 110, 0, 0, 255);
  SDL_RenderClear(render);

  switch(scene) {
  case SCENE_OPENING: scene_opening_draw(render); break;
    //  case SCENE_STAGE_SELECT: scene_stage_sel_draw(render); break;
    //  case SCENE_PLAY: scene_play_draw(render); break;
    //  case SCENE_CARD_PLAY: scene_card_play_draw(render); break;
  default: break;
  }

  SDL_RenderPresent(render);
}

bool initialize() {
  int width, height;
  TTF_Init();
  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow(
                            "GoldRush Dangeon",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WIN_W,
                            WIN_H,
                            SDL_WINDOW_OPENGL
                            );
  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return false;
  } else {
    SDL_GetWindowSize(window, &width, &height);
  }

  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // w:h = 1:1.6の比率でゲーム画面サイズを決定する
  if (width * 1.6 <= height) { //幅を基準に倍率を決める
    game.win_scale = width / (float)WIN_W;
    __android_log_print(ANDROID_LOG_DEBUG,"Game","window(%d, %d) WIN_W=%d w-scale=%f",width, height, WIN_W, game.win_scale);
  } else { // 高さを基準に倍率を決める
    game.win_scale = height / (float)WIN_H;
    __android_log_print(ANDROID_LOG_DEBUG,"Game","window(%d, %d) h-scale=%f",width, height, game.win_scale);
  }
  game.win_w = WIN_W * game.win_scale;
  game.win_h = WIN_H * game.win_scale;

  switch_scene(SCENE_OPENING);

  return true;
}
void finalize() {
  switch_scene(SCENE_NONE);

  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);

  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
}



int SDL_main(int argc, char* argv[]) {

  if (!initialize()) return 1;

  int lastTime = 0;
  SceneType next_scene;
  while (running) {
    lastFrame = SDL_GetTicks();
    if (lastFrame >= lastTime + 1000) {
      lastTime = lastFrame;
      fps = frameCount;
      frameCount = 0;
      printf("fps:%d lastFrame:%d\n", fps, lastFrame);

    }
    next_scene = SCENE_NONE;
    update(&next_scene);
    input();
    draw();

    frameCount++;
    int timerFPS = SDL_GetTicks() - lastFrame;
    if (timerFPS < (1000/60)) SDL_Delay((1000/60) - timerFPS);

  //   if (next_scene != SCENE_NONE) {
  //     switch_scene(next_scene);
  //   }
  }
  finalize();
  return 0;
  /*
    SDL_Window *window;                    // Declare a pointer
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1 ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't open mixer: %s", SDL_GetError());
        return 2;
    }

    Mix_Chunk *sample = Mix_LoadWAV("cuckoo.wav");
    if (sample == NULL) {
        fprintf(stderr, "Unable to load wave file\n");
        return 3;
    }


    // Setup renderer
    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

    surface = IMG_ReadXPMFromArray(icon_xpm);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't load texture: %s", SDL_GetError());
        return 4;
    }

    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 220, 220, 220, 255 );

    // Clear winow
    SDL_RenderClear( renderer );

    // Display image
    SDL_Rect dstrect;


    // Load smiley.png and display it.
    // Location of image files for Android is: app/src/main/assets
    SDL_Surface *loadedSurface = IMG_Load("smiley.png");
    if (!loadedSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't load PNG image: %s", SDL_GetError());
        return 5;
    }

    SDL_Surface *backgroundSurface = IMG_Load("brno-snow.jpg");
    if (!backgroundSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't load JPG image: %s", SDL_GetError());
        return 6;
    }

    // Initialize TTF
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init: %s\n", TTF_GetError());
        return 7;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("blazed.ttf", 32);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Unable to load font: %s\n", TTF_GetError());
        return 8;
    }

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = backgroundSurface->w;
    dstrect.h = backgroundSurface->h;
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &dstrect);

    dstrect.x = 190;
    dstrect.y = 130;
    dstrect.w = loadedSurface->w;
    dstrect.h = loadedSurface->h;
    SDL_Texture *smileyTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_RenderCopy(renderer, smileyTexture, NULL, &dstrect);

    dstrect.x = 20;
    dstrect.y = 120;
    dstrect.w = 128;
    dstrect.h = 128;
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 450;
    dstrect.h = 100;
    SDL_Color textColor = { 255, 240, 0, 255 };
    SDL_Surface* solid = TTF_RenderText_Solid(font, "SDL2 Android Example", textColor);

    SDL_Texture* solidTexture = SDL_CreateTextureFromSurface(renderer, solid);
    SDL_RenderCopy(renderer, solidTexture, NULL, &dstrect);
    SDL_FreeSurface(solid);

    // SDL2_gfx example
    thickLineColor(renderer, 0, 300, 300, 300, 20, 0xFF00FFFF) ;

    // Render to the screen
    SDL_RenderPresent(renderer);

    // Event loop
    bool quit = false;
    SDL_Event event;
    while(!quit && SDL_WaitEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if ((event.key.keysym.sym == SDLK_AC_BACK) || (event.key.keysym.sym == SDLK_ESCAPE)) {
                    quit = true;
                }
                break;

            case SDL_FINGERDOWN: {
                Mix_PlayChannel(-1, sample, 0);
                dstrect.x = event.tfinger.x * loadedSurface->w;
                dstrect.y = event.tfinger.y * loadedSurface->h;
                dstrect.w = 128;
                dstrect.h = 128;
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_RenderPresent(renderer);
                break;
            }
            default:
                break;
        }
    }

    Mix_CloseAudio();

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();

    return 0;
  */
}

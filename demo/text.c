#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "list.h"
#include "polygon.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"
#include "make_util.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rec.h"
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h";

typedef enum {BALL = 1, DUNE = 2, DUNE_PT = 3, BACKROUND = 4, LINE = 5} dune_t;

dune_t *info_init(dune_t b) {
  dune_t *info = malloc(sizeof(dune_t));
  *info = b;
  return info;
}
/*
void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
  if (type == KEY_PRESSED) {
    switch (key) {
    case RIGHT_ARROW:
      break;
    case LEFT_ARROW:
      break;
    case SPACE_BAR:
      state->active_scene = state->game;
      printf("spacebar registered\n");
    }
  }
}
*/
state_t *emscripten_init() {
  srand(time(NULL));
  state_t *state = state_init();
  sdl_init(WINDOW_BOTTOM_LEFT, WINDOW_TOP_RIGHT);
  scene_t *end_menu = scene_init();
  IMG_Init();
  //TTF_Init();
  //TTF_Font* Sans = TTF_OpenFont("assets/Sans.ttf", 24);
  //SDL_Color color = SDL_BLACK;
  /*
  char *str_lose = malloc(sizeof(char)* 45);
  sprintf(str_lose, "You lost on level %i", get_current_scene(state));
  SDL_Surface* lose_surface = TTF_RenderText_Solid(Sans, str_lose, color);
  vector_t lose_centroid = {500, 400};
  make_text_box(end_menu, 150, 75, lose_centroid, WHITE, 0, NULL, lose_surface);
  
  SDL_Surface* points_text = TTF_RenderText_Solid(Sans, "Points: 0", color); 
  SDL_Surface* coins_text = TTF_RenderText_Solid(Sans, "Coins: 0", color); 
  make_text_box(end_menu, POINTS_TEXT_WIDTH, POINTS_TEXT_HEIGHT, POINTS_TEXT_CENTROID, GREEN, BALL_MASS, NULL, points_text);
  make_text_box(end_menu, COINS_TEXT_WIDTH, COINS_TEXT_HEIGHT, COINS_TEXT_CENTROID, GREEN, BALL_MASS, NULL, coins_text);

  char *str_points = malloc(sizeof(char)* 33);
  sprintf(str_points, "You were able to get %i Points", get_points(state));
  SDL_Surface* points_surface = TTF_RenderText_Solid(Sans, str_points, color);
  vector_t points_centroid = {400, 300};
  make_text_box(end_menu, 300, 75, points_centroid, WHITE, 0, NULL, points_surface);

  char *str_coins = malloc(sizeof(char)* 35);
  sprintf(str_coins, "You were able to earn %i coins", get_coins(state));
  SDL_Surface* coins_surface = TTF_RenderText_Solid(Sans, str_coins, color);
  vector_t coins_centroid = {400, 200};
  make_text_box(end_menu, 300, 75, coins_centroid, WHITE, 0, NULL, coins_surface);
  set_start(state, end_menu);
  */
  /*
  // initialise game scene
  scene_t *game_scene = scene_init();
  sdl_init(WINDOW_BOTTOM_LEFT, WINDOW_TOP_RIGHT);
  TTF_Init();
  TTF_Font* Sans = TTF_OpenFont("assets/Sans.ttf", 24);
  SDL_Surface* points_text = TTF_RenderText_Solid(Sans, "Points: ", SDL_BLACK); 
  SDL_Surface* coins_text = TTF_RenderText_Solid(Sans, "Coins: ", SDL_BLACK); 
  SDL_Surface * image = IMG_Load("assets/hopper.png");
  make_text_box(game_scene, 300, 300, HOPPER_CENTROID, NEW_GREEN, BALL_MASS, NULL, image);
  make_text_box(game_scene, POINTS_TEXT_WIDTH, POINTS_TEXT_HEIGHT, POINTS_TEXT_CENTROID, 
                                      NEW_GREEN, BALL_MASS, NULL, points_text);
  make_text_box(game_scene, COINS_TEXT_WIDTH, COINS_TEXT_HEIGHT, COINS_TEXT_CENTROID, 
                                      NEW_GREEN, BALL_MASS, NULL, coins_text);
  state->game = game_scene;

  // initialise start menu scene
  scene_t *start_scene = scene_init();
  sdl_init(WINDOW_BOTTOM_LEFT, WINDOW_TOP_RIGHT);
  TTF_Init();
  // TTF_Font* Sans = TTF_OpenFont("assets/Sans.ttf", 24);
  SDL_Surface* msg = TTF_RenderText_Solid(Sans, "press SPACEBAR to start", SDL_BLACK);
  make_text_box(start_scene, 300, 100, HOPPER_CENTROID, NEW_GREEN, BALL_MASS, NULL, msg);

  state->start = start_scene;
  state->active_scene = state->start;

  sdl_render_scene(state->active_scene);

  sdl_on_key((key_handler_t)on_key);
  */
  sdl_render_scene(end_menu);
  return state;
}

void emscripten_main(state_t *state) {
  sdl_render_scene(get_start(state));
}

void emscripten_free(state_t *state) {
  //scene_free(state->active_scene);
  free(state);
}
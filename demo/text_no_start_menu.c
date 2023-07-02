#include <stdio.h>
#include <stdlib.h>
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
#include <time.h>
#include "rec.h"
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

// constants for Window Sizing
const vector_t WINDOW_BOTTOM_LEFT = {0, 0};
const vector_t WINDOW_TOP_RIGHT = {1000.0, 500.0};

//constants for dune
const double DUNE_MASS = INFINITY;
const vector_t DUNE_CENTROID = {210, 50};
const vector_t DUNE_VELOCITY = {-150, 0};

// constants for ball
const double CIRCLE_POINTS = 50;
const double BALL_MASS = 0.005;
const rgb_color_t BALL_COLOR = {0, 0, 1.0};
const double BALL_RADIUS = 30.0;
const vector_t BALL_START_VELOCITY = {0, -150.0};
const vector_t BALL_CENTROID = {20, 400};

// constants for Elasticities
const double ELASTICITY_WITH_DUNE = 0.05;

// gravity
const double GRAVITY_COEFFICIENT = -100;

// Hills constants
const double START_Y = 140;
const double DX = 1;
const double AMP = 60;
const double X_STRETCH = 0.015;
const double Y_TRANSLATION = 0;

// Backround Constants
const double BACKROUND_MASS = 0;
const rgb_color_t WHITE = {1, 1, 1};
const rgb_color_t NEW_GREEN = {0, 1, 0};

// Points Line
const double POINTS_LINE_LENGTH = 5;
const vector_t POINTS_LINE_CENTROID = {500, 400};
const vector_t IMAGE_CENTROID = {500, 200};

const double POINT_LINE_MASS = 0;

//SDL Colors
const SDL_Color SDL_RED = {255, 0, 0};
const SDL_Color SDL_BLACK = {0, 0, 0};
const SDL_Color SDL_WHITE = {255, 255, 255};

//Constants for Coins and Points Text
const double POINTS_TEXT_WIDTH = 100;
const double POINTS_TEXT_HEIGHT = 50;
const vector_t POINTS_TEXT_CENTROID = {850, 470};
const double COINS_TEXT_WIDTH = 100;
const double COINS_TEXT_HEIGHT = 50;
const vector_t COINS_TEXT_CENTROID = {875, 420};
const vector_t HOPPER_CENTROID = {500, 250};
const double COINS_IMAGE_WIDTH = 50;
const double COINS_IMAGE_HEIGHT = 50;
const vector_t COINS_IMAGE_CENTROID = {800, 420};
const double COINS_IMAGE_MASS = 0;

typedef struct state {
  scene_t *scene;
  double points;
} state_t;

typedef enum {BALL = 1, DUNE = 2, DUNE_PT = 3, BACKROUND = 4, LINE = 5} dune_t;

dune_t *info_init(dune_t b) {
  dune_t *info = malloc(sizeof(dune_t));
  *info = b;
  return info;
}

state_t *emscripten_init() {
  srand(time(NULL));
  state_t *state = malloc(sizeof(state_t));
  scene_t *scene = scene_init();
  sdl_init(WINDOW_BOTTOM_LEFT, WINDOW_TOP_RIGHT);
  TTF_Init();
  TTF_Font* Sans = TTF_OpenFont("assets/Sans.ttf", 24);
  SDL_Surface* points_text = TTF_RenderText_Solid(Sans, "Points: ", SDL_BLACK); 
  SDL_Surface* coins_text = TTF_RenderText_Solid(Sans, "Coins: ", SDL_BLACK); 
  SDL_Surface * hopper_image = IMG_Load("assets/hopper.png");
  SDL_Surface *coin_image = IMG_Load("assets/coin.jpeg");
  make_text_box(scene, 300, 300, HOPPER_CENTROID, NEW_GREEN, BALL_MASS, NULL, hopper_image);
  
  state->scene = scene;
  state->points = 0;
  sdl_render_scene(state->scene);
  return state;
}


check_for_points(state_t *state) {
  body_t *coin_text = scene_get_body(state->scene, 2);
  char *str_points = malloc(sizeof(char)* 20);
  sprintf(str_points, "Points: %d", state->points);
  TTF_Font* Sans = TTF_OpenFont("assets/Sans.ttf", 24);
  SDL_Surface* points_text = TTF_RenderText_Solid(Sans, str_points, SDL_BLACK);
  body_init_image(coin_text, points_text);
}

void emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  scene_tick(state->scene, dt);
  check_for_points(state);
  state->points = state->points + 1;
  sdl_render_scene(state->scene);
}

void emscripten_free(state_t *state) {
  scene_free(state->scene);
  free(state);
}
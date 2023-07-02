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
#include <SDL2/SDL_mixer.h>

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

typedef struct state {
  scene_t *scene;
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
  int res = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
  if (res != 0){
      printf("could not open audio\n");
  }
  Mix_Music *star_wars = Mix_LoadMUS("assets/StarWars60.wav");
  if (!star_wars){
      printf("could not load star wars music file\n");
  }
  int didPlay = Mix_PlayMusic(star_wars, -1);
  if (didPlay != 0){
      printf("could not play music\n");
  }
  IMG_Init(IMG_INIT_JPG);
  SDL_Surface * image = IMG_Load("assets/lvl3.jpeg");
  // make_text_box(scene, 400, 200, IMAGE_CENTROID, NEW_GREEN, BALL_MASS, NULL, image);
  state->scene = scene;
  // body_t *b1 = scene_get_body(state->scene, 0);
  // body_t *b2 = scene_get_body(state->scene, 1);
  // vector_t v1 = {100, 0};
  // vector_t v2 = {50, 0};
  // body_set_velocity(b1, v1);
  // body_set_velocity(b2, v2);
  sdl_render_scene(state->scene);
  return state;
}

void emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  scene_tick(state->scene, dt);
  sdl_render_scene(state->scene);
}

void emscripten_free(state_t *state) {
  scene_free(state->scene);
  free(state);
}
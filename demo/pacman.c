#include "body.h"
#include "color.h"
#include "list.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Window Parameters Constants
const vector_t WINDOW_BOTTOM_LEFT = {0, 0};
const vector_t WINDOW_TOP_RIGHT = {1000.0, 500.0};

// Dimension Constants
const vector_t START = {500, 250};
const double RADIUS = 50.0;

// Pacman Conditions
const double PAC_MASS = 10;
const float PAC_COLOR_R = 1;
const float PAC_COLOR_G = 1;
const float PAC_COLOR_B = 0;
const int POINT_COUNT = 50;
const int FOOD_COUNT = 15;
const double TIME_INTERVAL = 5;
const double ACCELERATION = 100;
const double FOOD_SCALE_FACTOR = 0.1;

typedef struct state {
  scene_t *food;
  double time;
  double total_time;
  int total_food;
} state_t;

list_t *make_pacman_shape(double radius) {
  list_t *points = list_init(POINT_COUNT, free);
  double theta = 0.0;
  vector_t *v = malloc(sizeof(vector_t));
  v->x = radius * sin(theta) + START.x;
  v->y = START.y;
  list_add(points, v);
  for (int i = 0; i < POINT_COUNT - 1; i++) {
    theta = (2 * M_PI - M_PI / 3) / (POINT_COUNT - 1) * i + 2 * M_PI / 3;
    vector_t *v = malloc(sizeof(vector_t));
    v->x = radius * sin(theta) + START.x;
    v->y = radius * cos(theta) + START.y;
    list_add(points, v);
  }
  return points;
}

void make_pellet(scene_t *sc, rgb_color_t color, double radius) {
  list_t *p = list_init(POINT_COUNT, free);
  double theta = 0.0;
  vector_t start;
  start.x = ((double)rand() * (WINDOW_TOP_RIGHT.x - WINDOW_BOTTOM_LEFT.x)) /
                (double)RAND_MAX +
            WINDOW_BOTTOM_LEFT.x;
  start.y = ((double)rand() * (WINDOW_TOP_RIGHT.y - WINDOW_BOTTOM_LEFT.y)) /
                (double)RAND_MAX +
            WINDOW_BOTTOM_LEFT.y;
  for (int i = 0; i < POINT_COUNT - 1; i++) {
    theta = (2 * M_PI) / (POINT_COUNT)*i;
    vector_t *v = malloc(sizeof(vector_t));
    v->x = radius * sin(theta) + start.x;
    v->y = radius * cos(theta) + start.y;
    list_add(p, v);
  }
  body_t *piece = body_init(p, PAC_MASS, color);
  scene_add_body(sc, piece);
}

void make_initial_food(scene_t *sc, int count, rgb_color_t color,
                       double radius) {
  srand(time(NULL));
  for (size_t i = 0; i < count; i++) {
    make_pellet(sc, color, radius);
  }
}

void eat_food(state_t *state) {
  scene_t *curr_scene = state->food;
  body_t *pac = scene_get_body(state->food, 0);
  vector_t pac_centroid = body_get_centroid(pac);
  for (int i = 1; i <= state->total_food; i++) {
    body_t *curr = scene_get_body(curr_scene, i);
    vector_t body_centroid = body_get_centroid(curr);
    if (fabs(pac_centroid.x - body_centroid.x) <= RADIUS / 2 &&
        fabs(pac_centroid.y - body_centroid.y) <= RADIUS) {
      scene_remove_body(curr_scene, i);
      list_remove(scene_get_bodies(state->food), i);
      i--;
      body_free(curr);
      state->total_food--;
    }
  }
}

void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
  double dt = time_since_last_tick();
  state->time = state->time + dt;
  state->total_time = state->total_time + dt;
  body_t *pacman = scene_get_body(state->food, 0);
  vector_t the_velocity = body_get_velocity(pacman);
  if (type == KEY_PRESSED) {
    switch (key) {
    case UP_ARROW:
      the_velocity.x = 0.0;
      the_velocity.y = ACCELERATION * state->time;
      body_set_rotation(pacman, M_PI / 2);
      break;
    case DOWN_ARROW:
      the_velocity.x = 0.0;
      the_velocity.y = -ACCELERATION * state->time;
      body_set_rotation(pacman, 3 * M_PI / 2);
      break;
    case RIGHT_ARROW:
      the_velocity.x = ACCELERATION * state->time;
      the_velocity.y = 0.0;
      body_set_rotation(pacman, 0.0);
      break;
    case LEFT_ARROW:
      the_velocity.x = -ACCELERATION * state->time;
      the_velocity.y = 0.0;
      body_set_rotation(pacman, M_PI);
      break;
    }
  } else if (type == KEY_RELEASED) {
    state->time = 0;
  }
  if (state->total_time > TIME_INTERVAL) {
    rgb_color_t PAC_COLOR = {PAC_COLOR_R, PAC_COLOR_G, PAC_COLOR_B};
    make_pellet(state->food, PAC_COLOR, RADIUS * FOOD_SCALE_FACTOR);
    state->total_food++;
    state->total_time = state->total_time - TIME_INTERVAL;
  }
  body_set_velocity(pacman, the_velocity);
  body_tick(pacman, dt);
}

void check_sides(state_t *state) {
  body_t *pac = scene_get_body(state->food, 0);
  vector_t centroid = body_get_centroid(pac);
  if (centroid.x < WINDOW_BOTTOM_LEFT.x) {
    vector_t v = {WINDOW_TOP_RIGHT.x, centroid.y};
    body_set_centroid(pac, v);
  } else if (centroid.x > WINDOW_TOP_RIGHT.x) {
    vector_t v = {WINDOW_BOTTOM_LEFT.x, centroid.y};
    body_set_centroid(pac, v);
  } else if (centroid.y < WINDOW_BOTTOM_LEFT.y) {
    vector_t v = {centroid.x, WINDOW_TOP_RIGHT.y};
    body_set_centroid(pac, v);
  } else if (centroid.y > WINDOW_TOP_RIGHT.y) {
    vector_t v = {centroid.x, WINDOW_BOTTOM_LEFT.y};
    body_set_centroid(pac, v);
  }
}

state_t *emscripten_init() {
  sdl_init(WINDOW_BOTTOM_LEFT, WINDOW_TOP_RIGHT);
  state_t *state = malloc(sizeof(state_t));
  rgb_color_t PAC_COLOR = {PAC_COLOR_R, PAC_COLOR_G, PAC_COLOR_B};
  list_t *PAC_SHAPE = make_pacman_shape(RADIUS);
  body_t *pac = body_init(PAC_SHAPE, PAC_MASS, PAC_COLOR);
  scene_t *sc = scene_init();
  state->total_food = FOOD_COUNT;
  scene_add_body(sc, pac);
  make_initial_food(sc, state->total_food, PAC_COLOR,
                    RADIUS * FOOD_SCALE_FACTOR);
  state->food = sc;
  sdl_render_scene(state->food);
  sdl_on_key((key_handler_t)on_key);
  return state;
}

void emscripten_main(state_t *state) {
  check_sides(state);
  eat_food(state);
  sdl_render_scene(state->food);
}

void emscripten_free(state_t *state) {
  scene_free(state->food);
  free(state);
}

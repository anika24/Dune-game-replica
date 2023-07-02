#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "list.h"
#include "polygon.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constants for Window Sizing
const vector_t WINDOW_BOTTOM_LEFT = {0, 0};
const vector_t WINDOW_TOP_RIGHT = {600.0, 600.0};

// constants for paddle
const double PADDLE_WIDTH = 70.0;
const double PADDLE_LENGTH = 20.0;
const double PADDLE_MASS = INFINITY;
const rgb_color_t PADDLE_COLOR = {0, 1.0, 0};
const double ACCELERATION = 200;

// constants for blocks
const double BLOCK_WIDTH = 50.0;
const double BLOCK_LENGTH = 18;
const double BLOCK_MASS = INFINITY;
const size_t ROW_BLOCK_COUNT = 10;
const size_t COLUMN_BLOCK_COUNT = 3;
const double WHITE_SPACE = 5;
const rgb_color_t RED = {1.0, 0, 0};
const rgb_color_t ORANGE = {1.0, .5, 0};
const rgb_color_t YELLOW = {1.0, 1.0, 0};
const rgb_color_t LIGHT_GREEN = {.5, 1.0, 0};
const rgb_color_t GREEN = {0, 1.0, 0};
const rgb_color_t DARK_GREEN = {0, 1.0, .5};
const rgb_color_t LIGHT_BLUE = {0, 1.0, 1.0};
const rgb_color_t DARK_BLUE = {0, .5, 1.0};
const rgb_color_t ROYAL_BLUE = {0, 0, 1.0};
const rgb_color_t PURPLE = {.5, 0, 1.0};

// constants for ball
const double CIRCLE_POINTS = 40;
const double BALL_MASS = 1.0;
const rgb_color_t BALL_COLOR = {0, 0, 1.0};
const double BALL_START_Y = 100.0;
const double BALL_RADIUS = 7.0;
const vector_t BALL_START_VELOCITY = {250.0, 250.0};
const vector_t BALL_CENTROID = {300.0, 80.0};

// constants for Elasticities
const double ELASTICITY_WITH_PADDLE = 1.0;
const double ELASTICITY_WITH_BLOCK = 1.0;

// constants for Power-ups
const double PLAYER_SHIFT_CONDITION = .3;
const double SMALLEST_PLAYER_SHIFT = 10;
const double LARGEST_PLAYER_SHIFT = 30;
const double BLOCK_SHIFT_CONDITION = .6;
const double VELOCITY_SHIFT_MIN = .6;
const double VELOCITY_SHIFT_MAX = 1.4;
const double BLOCK_VELOCITY_MUL = 1.5;
const vector_t BLOCK_FORCE = {0, -100};
const vector_t BLOCK_VELOCITY = {0, -2.5};

typedef struct state {
  scene_t *scene;
} state_t;

typedef enum { PADDLE, BLOCK, BALL } breakout_t;

breakout_t *info_init(breakout_t b) {
  breakout_t *info = malloc(sizeof(breakout_t));
  *info = b;
  return info;
}

breakout_t get_type(body_t *body) { return *(breakout_t *)body_get_info(body); }

double rand_double(double min, double max) {
  return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

list_t *create_rainbow_color_list() {
  list_t *rainbow_list = list_init(ROW_BLOCK_COUNT, NULL);
  rgb_color_t *color_p = malloc(sizeof(rgb_color_t));
  *color_p = RED;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = ORANGE;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = YELLOW;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = LIGHT_GREEN;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = GREEN;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = DARK_GREEN;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = LIGHT_BLUE;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = DARK_BLUE;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = ROYAL_BLUE;
  list_add(rainbow_list, color_p);
  color_p = malloc(sizeof(rgb_color_t));
  *color_p = PURPLE;
  list_add(rainbow_list, color_p);
  return rainbow_list;
}

list_t *rect_init(double width, double height) {
  vector_t half_width = {.x = width / 2, .y = 0.0},
           half_height = {.x = 0.0, .y = height / 2};
  list_t *rect = list_init(4, free);
  vector_t *v = malloc(sizeof(*v));
  *v = vec_add(half_width, half_height);
  list_add(rect, v);
  v = malloc(sizeof(*v));
  *v = vec_subtract(half_height, half_width);
  list_add(rect, v);
  v = malloc(sizeof(*v));
  *v = vec_negate(*(vector_t *)list_get(rect, 0));
  list_add(rect, v);
  v = malloc(sizeof(*v));
  *v = vec_subtract(half_width, half_height);
  list_add(rect, v);
  return rect;
}

body_t *make_paddle(double width, double length, vector_t centroid) {
  list_t *paddle_shape = rect_init(width, length);
  body_t *paddle = body_init_with_info(paddle_shape, PADDLE_MASS, RED,
                                       info_init((breakout_t)PADDLE), free);
  body_set_centroid(paddle, centroid);
  return paddle;
}

void make_blocks(scene_t *scene, double length) {
  double width = (WINDOW_TOP_RIGHT.x - WHITE_SPACE * (ROW_BLOCK_COUNT + 1)) /
                 ROW_BLOCK_COUNT;
  vector_t centroid = {WHITE_SPACE + (width / 2.0),
                       WINDOW_TOP_RIGHT.y - (WHITE_SPACE + (length / 2.0))};
  list_t *color_list = create_rainbow_color_list();
  for (size_t i = 0; i < COLUMN_BLOCK_COUNT; i++) {
    for (size_t j = 0; j < ROW_BLOCK_COUNT; j++) {
      rgb_color_t *color = list_get(color_list, j);
      list_t *block_shape = rect_init(width, length);
      body_t *block =
          body_init_with_info(block_shape, BLOCK_MASS, (rgb_color_t)*color,
                              info_init((breakout_t)BLOCK), free);
      body_set_centroid(block, centroid);
      scene_add_body(scene, block);
      centroid.x += WHITE_SPACE + width;
    }
    centroid.x = WHITE_SPACE + (width / 2.0);
    centroid.y -= WHITE_SPACE + length;
  }
}

list_t *circle_init(double radius) {
  list_t *circle = list_init(CIRCLE_POINTS, free);
  double arc_angle = 2.0 * M_PI / CIRCLE_POINTS;
  vector_t point = {.x = radius, .y = 0.0};
  for (size_t i = 0; i < CIRCLE_POINTS; i++) {
    vector_t *v = malloc(sizeof(vector_t));
    *v = point;
    list_add(circle, v);
    point = vec_rotate(point, arc_angle);
  }
  return circle;
}

void make_ball(scene_t *scene, double radius, vector_t velocity,
               vector_t centroid, rgb_color_t color) {
  list_t *ball_shape = circle_init(radius);
  body_t *ball = body_init_with_info(ball_shape, BALL_MASS, color,
                                     info_init((breakout_t)BALL), free);
  body_set_centroid(ball, centroid);
  body_set_velocity(ball, velocity);
  scene_add_body(scene, ball);
}

void add_collision_forces(scene_t *scene) {
  body_t *ball = scene_get_body(scene, scene_bodies(scene) - 1);
  for (size_t i = 0; i < scene_bodies(scene); i++) {
    body_t *body = scene_get_body(scene, i);
    switch (get_type(body)) {
    case PADDLE:
      create_physics_collision(scene, ELASTICITY_WITH_PADDLE, ball, body);
      break;
    case BLOCK:
      create_semi_destructive_collision(scene, ELASTICITY_WITH_BLOCK, ball,
                                        body);
      break;
    case BALL:
      break;
    }
  }
}

void reset_game(state_t *state) {
  for (size_t i = 0; i < scene_bodies(state->scene); i++) {
    body_remove(scene_get_body(state->scene, i));
  }
  vector_t centroid = {WINDOW_TOP_RIGHT.x / 2.0,
                       PADDLE_LENGTH / 2.0 + WHITE_SPACE};
  scene_add_body(state->scene,
                 make_paddle(PADDLE_WIDTH, PADDLE_LENGTH, centroid));
  make_blocks(state->scene, BLOCK_LENGTH);
  make_ball(state->scene, BALL_RADIUS, BALL_START_VELOCITY, BALL_CENTROID, RED);
  add_collision_forces(state->scene);
}

void check_win(state_t *state) {
  if (scene_bodies(state->scene) == 2) {
    exit(0);
  }
}

void check_sides(state_t *state, double dt) {
  int exit_game = 1;
  for (int i = 0; i < scene_bodies(state->scene); i++) {
    body_t *body = scene_get_body(state->scene, i);
    vector_t velocity = body_get_velocity(body);
    if (get_type(body) == BALL) {
      vector_t centroid = body_get_centroid(body);
      vector_t translation = vec_multiply(dt, velocity);
      if (centroid.x + translation.x > WINDOW_TOP_RIGHT.x ||
          centroid.x + translation.x < WINDOW_BOTTOM_LEFT.x) {
        velocity.x *= -1;
      }
      if (centroid.y + translation.y > WINDOW_TOP_RIGHT.y) {
        velocity.y *= -1;
      }
      if (centroid.y + translation.y > WINDOW_BOTTOM_LEFT.y) {
        exit_game = 0;
      }
    }
    body_set_velocity(body, velocity);
  }
  if (exit_game) {
    reset_game(state);
  }
}

void powerup_gen(state_t *state) {
  scene_t *scene = state->scene;
  body_t *paddle = scene_get_body(scene, 0);
  double gamma = rand_double(0.0, 1.0);
  // Shifts player upwards - game harder
  if (gamma < PLAYER_SHIFT_CONDITION) {
    vector_t new_centroid =
        vec_add(body_get_centroid(paddle),
                (vector_t){0, rand_double(SMALLEST_PLAYER_SHIFT,
                                          LARGEST_PLAYER_SHIFT)});
    body_set_centroid(paddle, new_centroid);
  }
  // Makes blocks travel downwards
  else if (gamma < BLOCK_SHIFT_CONDITION) {
    for (int i = 0; i < scene_bodies(scene); i++) {
      if (get_type(scene_get_body(scene, i)) == BLOCK) {
        body_t *block = scene_get_body(scene, i);
        if (body_get_velocity(block).y == 0) {
          body_set_velocity(block, BLOCK_VELOCITY);
        } else {
          body_set_velocity(block, vec_multiply(BLOCK_VELOCITY_MUL,
                                                body_get_velocity(block)));
        }
      }
    }
  }
  // Makes ball faster or slower
  else {
    for (int i = 0; i < scene_bodies(scene); i++) {
      if (get_type(scene_get_body(scene, i)) == BALL) {
        body_t *ball = scene_get_body(scene, i);
        vector_t v = body_get_velocity(ball);
        body_set_velocity(
            ball, vec_multiply(
                      rand_double(VELOCITY_SHIFT_MIN, VELOCITY_SHIFT_MAX), v));
      }
      break;
    }
  }
}

void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
  scene_t *scene = state->scene;
  body_t *paddle = scene_get_body(scene, 0);
  vector_t the_velocity = body_get_velocity(paddle);
  double dt = time_since_last_tick();
  vector_t centroid = body_get_centroid(paddle);
  if (type == KEY_PRESSED) {
    switch (key) {
    case RIGHT_ARROW:
      if (centroid.x + PADDLE_WIDTH / 2.0 >= WINDOW_TOP_RIGHT.x) {
        the_velocity = VEC_ZERO;
        break;
      } else {
        the_velocity.x = ACCELERATION * 2;
        the_velocity.y = 0.0;
        break;
      }
    case LEFT_ARROW:
      if (centroid.x - PADDLE_WIDTH / 2.0 <= WINDOW_BOTTOM_LEFT.x) {
        the_velocity = VEC_ZERO;
        break;
      } else {
        the_velocity.x = -ACCELERATION * 2;
        the_velocity.y = 0.0;
        break;
      }
    case SPACE_BAR:
      powerup_gen(state);
    }
  } else if (type == KEY_RELEASED) {
    the_velocity = VEC_ZERO;
  }
  body_set_velocity(paddle, the_velocity);
  body_tick(paddle, dt);
}

state_t *emscripten_init() {
  srand(time(NULL));
  sdl_init(WINDOW_BOTTOM_LEFT, WINDOW_TOP_RIGHT);
  state_t *state = malloc(sizeof(state_t));
  scene_t *scene = scene_init();
  vector_t centroid = {WINDOW_TOP_RIGHT.x / 2.0,
                       PADDLE_LENGTH / 2.0 + WHITE_SPACE};
  scene_add_body(scene, make_paddle(PADDLE_WIDTH, PADDLE_LENGTH, centroid));
  make_blocks(scene, BLOCK_LENGTH);
  make_ball(scene, BALL_RADIUS, BALL_START_VELOCITY, BALL_CENTROID, RED);
  state->scene = scene;
  add_collision_forces(state->scene);
  sdl_on_key((key_handler_t)on_key);
  sdl_render_scene(state->scene);
  return state;
}

void emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  check_sides(state, dt);
  check_win(state);
  scene_tick(state->scene, dt);
  sdl_render_scene(state->scene);
}

void emscripten_free(state_t *state) {
  scene_free(state->scene);
  free(state);
}

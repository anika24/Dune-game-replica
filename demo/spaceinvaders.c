#include "body.h"
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

const vector_t WINDOW_BOTTOM_LEFT = {0, 0};
const vector_t WINDOW_TOP_RIGHT = {600.0, 600.0};

// constants
const double TWOPI = M_PI * 2;

// constants for invaders
const size_t INVADER_RADIUS = 50;
const size_t INVADER_POINTS = 40;
const size_t INVADER_MASS = 25;
const vector_t INVADER_INIT_VELOCITY = {90, 0};
const double INVADER_SHIFT = 150;
const double HORIZONTAL_TRANSLATE = 10;
const double FIRE_PROBABILITY = 1.5;
const double SHOOTING_RANGE = 1.5;

// constants for player
const size_t PLAYER_RADIUS = 35;
const size_t PLAYER_POINTS = 40;
const size_t PLAYER_MASS = 25;
const double ACCELERATION = 100;
const double PLAYER_SECTOR = 1.0;
const double ELLIPSE_A = 30.0;
const double ELLIPSE_B = 10.0;

// constants for projectiles
const vector_t PROJECTILE_SIZE = {5, 10};
const double PROJECTILE_VEL = 100;
const size_t PROJECTILE_MASS = 20;

// colors
const rgb_color_t GRAY = {.r = 0.5, .g = 0.5, .b = 0.5};
const rgb_color_t GREEN = {.r = 0, .g = 1, .b = 0};

typedef struct state {
  scene_t *scene;
} state_t;

double rand_double(double min, double max) {
  return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

int is_close(double x1, double x2) {
  if (fabs(x2 - x1) < SHOOTING_RANGE) {
    return 1;
  }
  return 0;
}

list_t *make_ellipse(double a, double b, size_t num_points, vector_t centroid) {
  double angle = 0;
  double angle_update = TWOPI / num_points;
  double x;
  double y;
  list_t *vertices = list_init(num_points, free);
  assert(vertices != NULL);
  for (size_t i = 0; i < num_points; i++) {
    x = a * cos(angle) + centroid.x;
    y = b * sin(angle) + centroid.y;
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){x, y};
    list_add(vertices, v);
    angle += angle_update;
  }
  return vertices;
}

list_t *make_invader(double radius, size_t num_points, vector_t centroid) {
  list_t *points = list_init(num_points, free);
  double theta = 0.0;
  vector_t *v = malloc(sizeof(vector_t));
  v->x = radius * sin(theta) + centroid.x;
  v->y = centroid.y;
  list_add(points, v);
  for (int i = 0; i < num_points - 1; i++) {
    theta = (2 * M_PI - M_PI * 1.2) / (num_points - 1) * i + 2 * M_PI / 3;
    vector_t *v = malloc(sizeof(vector_t));
    v->x = radius * sin(theta) + centroid.x;
    v->y = radius * cos(theta) + centroid.y;
    list_add(points, v);
  }
  polygon_rotate(points, M_PI + 0.1, centroid);
  return points;
}

void populate_screen(state_t *state, vector_t invader_centroid) {
  // first body in scene is always the player's oval
  for (size_t i = 2 * INVADER_RADIUS; i <= WINDOW_TOP_RIGHT.x;
       i += 2 * INVADER_RADIUS) {
    list_t *invader_vertices =
        make_invader(INVADER_RADIUS, INVADER_POINTS, invader_centroid);
    char *info = malloc(sizeof(char *));
    info = "invader";
    body_t *invader =
        body_init_with_info(invader_vertices, INVADER_MASS, GRAY, info, free);
    body_set_velocity(invader, INVADER_INIT_VELOCITY);
    scene_add_body(state->scene, invader);
    vector_t v = {.x = 2 * INVADER_RADIUS, .y = 0};
    invader_centroid = vec_add(invader_centroid, v);
  }
}

void translate_invaders(state_t *state) {
  scene_t *scene = state->scene;
  for (int i = 1; i < scene_bodies(scene); i++) {
    body_t *invader = scene_get_body(scene, i);
    vector_t centroid = body_get_centroid(invader);
    if ((centroid.x + INVADER_RADIUS >= WINDOW_TOP_RIGHT.x) ||
        (centroid.x <= INVADER_RADIUS)) {
      vector_t *shift = malloc(sizeof(*shift));
      if (centroid.x <= INVADER_RADIUS) {
        shift->x = HORIZONTAL_TRANSLATE;
        shift->y = 0;
      } else {
        shift->x = -HORIZONTAL_TRANSLATE;
        shift->y = 0;
      }
      vector_t v = {.x = 0, .y = -INVADER_SHIFT};
      v = vec_add(v, *shift);
      list_t *vertices = body_get_shape(invader);
      polygon_translate(vertices, v);
      vector_t new_center = vec_add(body_get_centroid(invader), v);
      body_set_centroid(invader, new_center);
      vector_t vel = {.x = body_get_velocity(invader).x * -1, .y = 0};
      body_set_velocity(invader, vel);
    }
  }
}

void fire_projectile(state_t *state, rgb_color_t color, vector_t velocity,
                     double x, double y, void *target, free_func_t info_freer) {
  list_t *rectangle = list_init(4, free);
  vector_t *v1 = malloc(sizeof(*v1));
  *v1 = (vector_t){x + PROJECTILE_SIZE.x, y};
  list_add(rectangle, v1);
  vector_t *v2 = malloc(sizeof(*v2));
  *v2 = (vector_t){x - PROJECTILE_SIZE.x, y};
  list_add(rectangle, v2);
  vector_t *v3 = malloc(sizeof(*v3));
  *v3 = (vector_t){x - PROJECTILE_SIZE.x, y - PROJECTILE_SIZE.y};
  list_add(rectangle, v3);
  vector_t *v4 = malloc(sizeof(*v4));
  *v4 = (vector_t){x + PROJECTILE_SIZE.x, y - PROJECTILE_SIZE.y};
  list_add(rectangle, v4);
  sdl_draw_polygon(rectangle, color);
  char *info = malloc(sizeof(char *));
  info = "bullet";
  body_t *projectile =
      body_init_with_info(rectangle, PROJECTILE_MASS, color, info, free);
  body_set_velocity(projectile, velocity);
  scene_add_body(state->scene, projectile);
  if (target == "player") {
    create_destructive_collision(state->scene, scene_get_body(state->scene, 0),
                                 projectile);
  } else {
    for (size_t i = 1; i < scene_bodies(state->scene); i++) {
      body_t *shape = scene_get_body(state->scene, i);
      if (body_get_info(shape) == "invader") {
        create_destructive_collision(state->scene, shape, projectile);
      }
    }
  }
}

void fire_invader_projectiles(state_t *state) {
  body_t *player = scene_get_body(state->scene, 0);
  for (int i = 1; i < scene_bodies(state->scene); i++) {
    body_t *invader = scene_get_body(state->scene, i);
    vector_t centroid = body_get_centroid(invader);
    if (body_get_info(invader) == "invader" &&
        is_close(centroid.x, body_get_centroid(player).x) &&
        rand_double(0.0, 10.0) < FIRE_PROBABILITY) {
      char *info = malloc(sizeof(char *));
      info = "player";
      fire_projectile(state, GRAY, (vector_t){0.0, -PROJECTILE_VEL},
                      body_get_centroid(invader).x,
                      body_get_centroid(invader).y, info, free);
    }
  }
}

void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
  scene_t *scene = state->scene;
  body_t *player = scene_get_body(scene, 0);
  vector_t the_velocity = body_get_velocity(player);
  double dt = time_since_last_tick();
  vector_t centroid = body_get_centroid(player);
  if (type == KEY_PRESSED) {
    switch (key) {
    case RIGHT_ARROW:
      if (centroid.x + PLAYER_RADIUS > WINDOW_TOP_RIGHT.x) {
        the_velocity = VEC_ZERO;
        break;
      } else {
        the_velocity.x = ACCELERATION * 2;
        the_velocity.y = 0.0;
        break;
      }
    case LEFT_ARROW:
      if (centroid.x - PLAYER_RADIUS < WINDOW_BOTTOM_LEFT.x) {
        the_velocity = VEC_ZERO;
        break;
      } else {
        the_velocity.x = -ACCELERATION * 2;
        the_velocity.y = 0.0;
        break;
      }
    case SPACE_BAR:
      fire_projectile(state, GREEN, (vector_t){0, PROJECTILE_VEL}, centroid.x,
                      centroid.y + PLAYER_RADIUS - PROJECTILE_SIZE.y, NULL,
                      free);
      break;
    }
  } else if (type == KEY_RELEASED) {
    the_velocity = VEC_ZERO;
  }
  body_set_velocity(player, the_velocity);
  body_tick(player, dt);
}

void check_sides(state_t *state) {
  scene_t *scene = state->scene;
  body_t *player = scene_get_body(scene, 0);
  vector_t centroid = body_get_centroid(player);
  if (centroid.x - PLAYER_RADIUS < WINDOW_BOTTOM_LEFT.x) {
    vector_t v = {centroid.x, centroid.y};
    body_set_centroid(player, v);
    body_set_velocity(player, VEC_ZERO);
  } else if (centroid.x + PLAYER_RADIUS > WINDOW_TOP_RIGHT.x) {
    vector_t v = {centroid.x, centroid.y};
    body_set_centroid(player, v);
    body_set_velocity(player, VEC_ZERO);
  }
}

state_t *emscripten_init() {
  srand(time(NULL));
  sdl_init(WINDOW_BOTTOM_LEFT, WINDOW_TOP_RIGHT);
  state_t *init_state = malloc(sizeof(state_t));
  scene_t *sc = scene_init();
  list_t *player_vertices =
      make_ellipse(ELLIPSE_A, ELLIPSE_B, 40,
                   (vector_t){WINDOW_TOP_RIGHT.x / 2, ELLIPSE_B * 2});
  char *info = malloc(sizeof(char *));
  info = "player";
  body_t *player =
      body_init_with_info(player_vertices, PLAYER_MASS, GREEN, info, free);
  scene_add_body(sc, player);
  init_state->scene = sc;
  for (size_t j = 0; j < 3; j++) {
    vector_t invader_centroid = {.x = INVADER_RADIUS + 7,
                                 .y =
                                     (WINDOW_TOP_RIGHT.y - INVADER_RADIUS - 7) -
                                     ((INVADER_RADIUS + 5) * (j + 1))};
    populate_screen(init_state, invader_centroid);
  }
  sdl_on_key((key_handler_t)on_key);
  return init_state;
}

void emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  check_sides(state);
  translate_invaders(state);
  fire_invader_projectiles(state);
  // if player dies
  if (body_get_info(scene_get_body(state->scene, 0)) != "player") {
    exit(0);
  }
  // if all invaders are eliminated
  if (body_get_info(scene_get_body(state->scene, 1)) != "invader") {
    exit(0);
  }
  // if any invader goes below window
  for (size_t i = 1; i < scene_bodies(state->scene); i++) {
    if (body_get_info(scene_get_body(state->scene, i)) == "invader") {
      if (body_get_centroid(scene_get_body(state->scene, i)).y < 0) {
        exit(0);
      }
    }
  }
  scene_tick(state->scene, dt);
  sdl_render_scene(state->scene);
}

void emscripten_free(state_t *state) {
  scene_free(state->scene);
  free(state);
}

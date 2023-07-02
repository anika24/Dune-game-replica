#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "list.h"
#include "polygon.h"
#include "scene.h"
#include "make_util.h"
#include "sdl_wrapper.h"
#include "state.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include "rec.h"
#include "constants.h"

double rand_double(double min, double max) {
  return min + (float)rand() / ((float)RAND_MAX / (max - min));
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

list_t *slope_init(scene_t *scene, vector_t window, double start_y, double dt, double a, double k, double c, double POINTS, void *info, vector_t velocity_points, double slope_length, double ball_centroid_x) {
  list_t *slope = list_init(POINTS, free);
  vector_t *v = malloc(sizeof(*v));
  *v = window;
  list_add(slope, v);

  double x = dt;
  double y;
  int count = 0;

  while(x < 2*M_PI / k * slope_length){
    vector_t *v = malloc(sizeof(vector_t));
    y = start_y;
    y += a * cos(k*x) + c;
    *v = (vector_t){x + ball_centroid_x, y};
    list_add(slope, v);
    x += dt;
    count++;
  }
  vector_t *t = malloc(sizeof(vector_t));
  *t = (vector_t){x, 0};
  list_add(slope, t);

  return slope;
}

void make_dune(scene_t *scene, vector_t centroid, vector_t window, double circ_points, double mass, void *info, vector_t velocity, double start_y, double dt, double a, double k, double c, void *info2, double slope_length, double ball_centroid_x, rgb_color_t color) {
  list_t *dune_shape = slope_init(scene, window, start_y, dt, a, k, c, circ_points, info2, velocity, slope_length, ball_centroid_x);
  body_t *dune = body_init_with_info(dune_shape, mass, color, info, free);
  body_set_centroid(dune, centroid);
  body_set_velocity(dune, velocity);
  scene_add_body(scene, dune);
}

list_t *circle_init(double radius, double points) {
  list_t *circle = list_init(points, free);
  double arc_angle = 2.0 * M_PI / points;
  vector_t point = {.x = radius, .y = 0.0};
  for (size_t i = 0; i < points; i++) {
    vector_t *v = malloc(sizeof(vector_t));
    *v = point;
    list_add(circle, v);
    point = vec_rotate(point, arc_angle);
  }
  return circle;
}

void make_ball(scene_t *scene, double radius, vector_t velocity, vector_t centroid, rgb_color_t color, double mass, void *info, double points, SDL_Surface *image) {
  list_t *ball_shape = circle_init(radius, points);
  body_t *ball = body_init_with_image(ball_shape, mass, color, info, free, image, NULL);
  body_set_centroid(ball, centroid);
  body_set_velocity(ball, velocity);
  if (image != NULL) {
    body_init_rec(ball, rec_init_circle(body_get_centroid(ball), 2 * radius)); 
  }
  scene_add_body(scene, ball);
}

void make_line(scene_t *scene, double width, double length, vector_t centroid, rgb_color_t color, double mass, void *info, vector_t velocity, int alpha) {
  list_t *ball_shape = rect_init(width, length);
  body_t *ball = body_init_with_alpha(ball_shape, mass, color, info, free, NULL, NULL, alpha);
  body_set_centroid(ball, centroid);
  body_set_velocity(ball, velocity);
  scene_add_body(scene, ball);
}

void make_text_box(scene_t *scene, double width, double length, vector_t centroid, rgb_color_t color, double mass, void *info, SDL_Surface *surface) {
  list_t *text_shape = rect_init(width, length);
  body_t *text = body_init_with_image(text_shape, mass, color, info, free, surface, NULL);
  body_set_centroid(text, centroid);
  body_init_rec(text, rec_init_rectangle(centroid, width, length));
  scene_add_body(scene, text);
}

void make_arrow_up(scene_t *scene, double radius, vector_t velocity, vector_t centroid, rgb_color_t color, double mass, void *info) {
  list_t *arrow_shape = circle_init(radius, 3);
  body_t *arrow = body_init_with_info(arrow_shape, mass, color, info, free);
  body_set_centroid(arrow, centroid);
  body_set_rotation(arrow, M_PI / 2.0);
  body_set_velocity(arrow, velocity);
  scene_add_body(scene, arrow);
}

void make_arrow_down(scene_t *scene, double radius, vector_t velocity, vector_t centroid, rgb_color_t color, double mass, void *info) {
  list_t *arrow_shape = circle_init(radius, 3);
  body_t *arrow = body_init_with_info(arrow_shape, mass, color, info, free);
  body_set_centroid(arrow, centroid);
  body_set_rotation(arrow, 3 * M_PI / 2.0);
  body_set_velocity(arrow, velocity);
  scene_add_body(scene, arrow);
}
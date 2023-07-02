#include "body.h"
#include "assert.h"
#include "polygon.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "rec.h"
#include "rec.h"

const size_t IMPULSE_FORCE_SIZE = 10;

typedef struct body {
  double angle;
  list_t *shape;
  double mass;
  vector_t centroid;
  rgb_color_t color;
  vector_t velocity;
  vector_t impulse;
  vector_t force;
  vector_t acceleration;
  void *info;
  free_func_t info_freer;
  bool removed;
  SDL_Surface *image;
  rec_t *rec;
  int alpha;
} body_t;

body_t *body_init_with_alpha(list_t *shape, double mass, rgb_color_t color, void *info, free_func_t info_freer, SDL_Surface *image, rec_t *rec, int alpha){
  body_t *the_body = body_init_with_image(shape, mass, color, info, info_freer, image, rec);
  the_body->alpha = alpha;
  return the_body;
}

body_t *body_init_with_image(list_t *shape, double mass, rgb_color_t color, void *info, free_func_t info_freer, SDL_Surface *image, rec_t *rec){
  body_t *the_body = malloc(sizeof(body_t));
  the_body->shape = shape;
  the_body->mass = mass;
  the_body->centroid = polygon_centroid(shape);
  the_body->color = color;
  vector_t v = {0.0, 0.0};
  the_body->velocity = v;
  the_body->acceleration = v;
  the_body->impulse = v;
  the_body->force = v;
  the_body->info = info;
  the_body->angle = 0.0;
  the_body->removed = false;
  the_body->info_freer = info_freer;
  the_body->image = image;
  the_body->rec = rec;
  the_body->alpha = 255;
  return the_body;
}

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
  void *info = NULL;
  body_t *body = body_init_with_info(shape, mass, color, info, NULL);
  return body;
}

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t color,
                            void *info, free_func_t info_freer) {
  SDL_Surface *image = NULL;
  rec_t *rec = NULL;
  body_t *body = body_init_with_image(shape, mass, color, info, info_freer, image, rec);
  return body;
}

void body_free(body_t *body) {
  list_free(body->shape);
  if (body->info_freer != NULL) {
    body->info_freer(body->info);
  }
  if (body->image != NULL) {
    SDL_FreeSurface(body->image);
  }
  if (body->rec != NULL) {
    free(body->rec);
  }
  free(body);
}

list_t *body_get_shape(body_t *body) {
  list_t *lst = list_init(list_size(body->shape), free);
  for (int i = 0; i < list_size(body->shape); i++) {
    vector_t *v = malloc(sizeof(vector_t));
    vector_t *old_v = list_get(body->shape, i);
    v->x = old_v->x;
    v->y = old_v->y;
    list_add(lst, v);
  }
  return lst;
}

double body_get_mass(body_t *body) { return body->mass; }

vector_t body_get_centroid(body_t *body) { return body->centroid; }

vector_t body_get_velocity(body_t *body) { return body->velocity; }

rgb_color_t body_get_color(body_t *body) { return body->color; }

void *body_get_info(body_t *body) { return body->info; }

void body_set_centroid(body_t *body, vector_t x) {
  polygon_translate(body->shape, vec_subtract(x, body->centroid));
  body->centroid = x;
}

void body_set_velocity(body_t *body, vector_t v) { body->velocity = v; }

void body_set_rotation(body_t *body, double angle) {
  polygon_rotate(body->shape, angle - (body->angle), body->centroid);
  body->angle = angle;
}

void body_add_force(body_t *body, vector_t force) {
  body->force = vec_add(body->force, force);
}

void body_add_impulse(body_t *body, vector_t impulse) {
  body->impulse = vec_add(body->impulse, impulse);
}

void body_tick(body_t *body, double dt) {
  vector_t old = body->velocity;
  body->velocity = vec_add(old, vec_multiply(1 / body->mass, body->impulse));
  body->velocity =
      vec_add(body->velocity, vec_multiply(dt / body->mass, body->force));
  vector_t avg = vec_multiply(0.5, vec_add(body->velocity, old));
  body_set_centroid(body, vec_add(body->centroid, vec_multiply(dt, avg)));
  body->impulse = (vector_t){0.0, 0.0};
  body->force = (vector_t){0.0, 0.0};
  if (body->rec != NULL) {
    rec_t *old_rec = body->rec;
    body->rec = rec_init_rectangle(body_get_centroid(body), rec_get_width(old_rec), rec_get_height(old_rec));
    free(old_rec);
  }
}

void body_remove(body_t *body) { body->removed = true; }

bool body_is_removed(body_t *body) { return body->removed; }

SDL_Surface *body_get_image(body_t *body){
  return body->image;
}

void body_init_rec(body_t *body, rec_t *rec) {
  body->rec = rec;
}

void body_init_image(body_t *body, SDL_Surface *image) {
  SDL_Surface *old_image = body->image;
  if (old_image != NULL) {
    SDL_FreeSurface(old_image);
  }
  body->image = image;
}

rec_t *body_get_rec(body_t *body) {
  return body->rec;
}

void body_add_to_angle(body_t *body, double add_angle) {
  body->angle = body->angle + add_angle;
}

double body_get_angle(body_t *body) {
  return body->angle;
}

int body_get_alpha(body_t *body) {
  return body->alpha;
}

void body_set_alpha(body_t *body, int alpha){
  body->alpha = alpha;
}

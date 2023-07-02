#include "scene.h"
#include "assert.h"
#include "body.h"
#include "forces.h"
#include "list.h"
#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

const int INITIAL_SIZE_ARR = 50;
const int INITIAL_SIZE_PAIRS = 50;

typedef struct pair {
  force_creator_t force_creator;
  void *aux;
  list_t *bodies;
  free_func_t freer;
} pair_t;

void pair_free(pair_t *p) {
  if (p->freer != NULL) {
    p->freer(p->aux);
  }
  for (size_t i = 0; i < list_size(p->bodies); i++) {
    list_remove(p->bodies, i);
    i--;
  }
  list_free(p->bodies);
  free(p);
}

typedef struct scene {
  list_t *arr;
  list_t *pairs;
} scene_t;

scene_t *scene_init(void) {
  scene_t *s = malloc(sizeof(scene_t));
  s->arr = list_init(INITIAL_SIZE_ARR, (free_func_t)body_free);
  s->pairs = list_init(INITIAL_SIZE_PAIRS, (free_func_t)pair_free);
  return s;
}

void scene_free(scene_t *scene) {
  list_free(scene->arr);
  list_free(scene->pairs);
  free(scene);
}

size_t scene_bodies(scene_t *scene) { return list_size(scene->arr); }

list_t *scene_get_bodies(scene_t *scene) { return scene->arr; }

body_t *scene_get_body(scene_t *scene, size_t index) {
  return list_get(scene->arr, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
  list_add(scene->arr, body);
}

void scene_remove_body(scene_t *scene, size_t index) {
  assert(index >= 0);
  assert(index < scene_bodies(scene));
  body_t *body = scene_get_body(scene, index);
  body_remove(body);
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux,
                             free_func_t freer) {
  list_t *bodies = list_init(0, free);
  scene_add_bodies_force_creator(scene, forcer, aux, bodies, freer);
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer,
                                    void *aux, list_t *bodies,
                                    free_func_t freer) {
  pair_t *new_pair = malloc(sizeof(pair_t));
  new_pair->aux = aux;
  new_pair->force_creator = forcer;
  new_pair->freer = freer;
  new_pair->bodies = bodies;
  list_add(scene->pairs, new_pair);
}

void scene_tick(scene_t *scene, double dt) {
  for (int i = 0; i < list_size(scene->pairs); i++) {
    pair_t *force = list_get(scene->pairs, i);
    force->force_creator(force->aux);
  }

  for (int i = 0; i < list_size(scene->pairs); i++) {
    pair_t *force = list_get(scene->pairs, i);
    list_t *bodies = force->bodies;
    for (size_t j = 0; j < list_size(bodies); j++) {
      body_t *body = list_get(bodies, j);
      if (body_is_removed(body)) {
        pair_free(list_remove(scene->pairs, i));
        i--;
        break;
      }
    }
  }

  for (int i = 0; i < list_size(scene->arr); i++) {
    body_t *body = list_get(scene->arr, i);
    if (body_is_removed(body)) {
      body_free(list_remove(scene->arr, i));
      i--;
    } else {
      body_tick(body, dt);
    }
  }
}
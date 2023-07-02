#include "forces.h"
#include "body.h"
#include "collision.h"
#include "list.h"
#include "math.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct body_pairs {
  body_t *body1;
  body_t *body2;
  double val;
} body_pairs_t;

typedef struct state_pairs {
  body_t *body1;
  body_t *body2;
  state_t *state;
} state_pairs_t;

typedef struct one_body {
  body_t *body1;
  double val;
} one_body_t;

typedef struct two_bodies_collision {
  body_t *body1;
  body_t *body2;
  collision_handler_t collision_fxn;
  bool is_colliding;
  void *h_aux;
  free_func_t freer;
} two_bodies_collision_t;

void free_two_bodies_collision(two_bodies_collision_t *two_body) {
  if (two_body->freer != NULL) {
    two_body->freer(two_body->h_aux);
  }
  free(two_body);
}

void helper_create_newtonian_gravity(void *aux) {
  body_pairs_t *pair = (body_pairs_t *)aux;
  body_t *body1 = pair->body1;
  body_t *body2 = pair->body2;
  double G = pair->val;
  vector_t d = vec_subtract(body_get_centroid(body1), body_get_centroid(body2));
  double r = sqrt(pow(d.x, 2) + pow(d.y, 2));
  if (r < 0.05) {
    return;
  }
  vector_t unit = vec_multiply(1 / r, d);
  double scale =
      -1 * G * body_get_mass(body1) * body_get_mass(body2) / pow(r, 2);
  vector_t force = vec_multiply(scale, unit);
  force.x = 0;
  body_add_force(body1, force);
  body_add_force(body2, vec_negate(force));
}

void create_newtonian_gravity(scene_t *scene, double G, body_t *body1,
                              body_t *body2) {
  body_pairs_t *pair = malloc(sizeof(body_pairs_t));
  pair->body1 = body1;
  pair->body2 = body2;
  pair->val = G;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  scene_add_bodies_force_creator(
      scene, (force_creator_t)helper_create_newtonian_gravity, pair, bodies,
      free);
}

void helper_create_spring(void *aux) {
  body_pairs_t *pair = (body_pairs_t *)aux;
  body_t *body1 = pair->body1;
  body_t *body2 = pair->body2;
  double k = pair->val;
  vector_t body1_force;
  vector_t diff =
      vec_subtract(body_get_centroid(body1), body_get_centroid(body2));
  body1_force.x = -k * diff.x;
  body1_force.y = -k * diff.y;
  body_add_force(body1, body1_force);
  body_add_force(body2, vec_multiply(-1, body1_force));
}

void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2) {
  body_pairs_t *pair = malloc(sizeof(body_pairs_t));
  pair->body1 = body1;
  pair->body2 = body2;
  pair->val = k;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  scene_add_bodies_force_creator(scene, (force_creator_t)helper_create_spring,
                                 pair, bodies, free);
}

void helper_create_drag(void *aux) {
  one_body_t *pair = (one_body_t *)aux;
  body_t *body1 = pair->body1;
  double gamma = pair->val;
  vector_t force;
  force.x = -gamma * body_get_velocity(body1).x;
  force.y = -gamma * body_get_velocity(body1).y;
  body_add_force(body1, force);
}

void create_drag(scene_t *scene, double gamma, body_t *body) {
  one_body_t *pair = malloc(sizeof(one_body_t));
  pair->body1 = body;
  pair->val = gamma;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body);
  scene_add_bodies_force_creator(scene, (force_creator_t)helper_create_drag,
                                 pair, bodies, free);
}

void helper_create_collision(void *aux) {
  two_bodies_collision_t *pair = (two_bodies_collision_t *)aux;
  body_t *body1 = pair->body1;
  body_t *body2 = pair->body2;
  collision_handler_t fxn = pair->collision_fxn;
  list_t *shape1 = body_get_shape(body1);
  list_t *shape2 = body_get_shape(body2);
  collision_info_t col = find_collision(shape1, shape2);
  if (col.collided) {
    if (!pair->is_colliding) {
      fxn(body1, body2, col.axis, pair->h_aux);
      pair->is_colliding = true;
    }
  } else {
    pair->is_colliding = false;
  }
}

void create_collision(scene_t *scene, body_t *body1, body_t *body2,
                      collision_handler_t handler, void *aux,
                      free_func_t freer) {
  two_bodies_collision_t *pair = malloc(sizeof(two_bodies_collision_t));
  pair->body1 = body1;
  pair->body2 = body2;
  pair->collision_fxn = handler;
  pair->is_colliding = false;
  pair->h_aux = aux;
  pair->freer = freer;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  scene_add_bodies_force_creator(
      scene, (force_creator_t) helper_create_collision, pair, bodies,
      (free_func_t)free_two_bodies_collision);
}

void collision_handler_destructive_collision(body_t *body1, body_t *body2,
                                             vector_t axis, void *aux) {
  body_remove(body1);
  body_remove(body2);
  list_free(body_get_shape(body1));
  list_free(body_get_shape(body2));
}

void create_destructive_collision(scene_t *scene, body_t *body1,
                                  body_t *body2) {
  two_bodies_collision_t *pair = malloc(sizeof(two_bodies_collision_t));
  pair->body1 = body1;
  pair->body2 = body2;
  pair->collision_fxn = collision_handler_destructive_collision;
  create_collision(scene, body1, body2,
                   (collision_handler_t)collision_handler_destructive_collision,
                   pair, free);
}

void collision_handler_physics_collision(body_t *body1, body_t *body2,
                                         vector_t axis, void *aux) {
  double elasticity = *(double *)aux;
  double mass1 = body_get_mass(body1);
  double mass2 = body_get_mass(body2);
  double reduced_mass = 0;
  if (mass1 != INFINITY && mass2 != INFINITY) {
    reduced_mass = mass1 * mass2 / (mass1 + mass2);
  } else if (mass1 == INFINITY) {
    reduced_mass = mass2;
  } else if (mass2 == INFINITY) {
    reduced_mass = mass1;
  }
  vector_t vel2 = body_get_velocity(body2);
  vector_t vel1 = body_get_velocity(body1);
  double impulse = reduced_mass * (elasticity + 1) *
                   (vec_dot(vel2, axis) - vec_dot(vel1, axis));
  body_add_impulse(body1, vec_multiply(impulse, axis));
  body_add_impulse(body2, vec_multiply(-1 * impulse, axis));
}

void create_physics_collision(scene_t *scene, double elasticity, body_t *body1,
                              body_t *body2) {
  double *elasticity1 = malloc(sizeof(double));
  *elasticity1 = elasticity;
  create_collision(scene, body1, body2,
                   (collision_handler_t)collision_handler_physics_collision,
                   elasticity1, free);
}

void destructive_bounce(body_t *body1, body_t *body2, vector_t axis,
                        void *aux) {
  collision_handler_physics_collision(body1, body2, axis, aux);
  body_remove(body2);
}

void create_semi_destructive_collision(scene_t *scene, double elasticity,
                                       body_t *body1, body_t *body2) {
  double *elasticity1 = malloc(sizeof(double));
  *elasticity1 = elasticity;
  create_collision(scene, body1, body2, (collision_handler_t)destructive_bounce,
                   elasticity1, free);
}

void collision_handler_one_body_remove(body_t *body1, body_t *body2,
                                             vector_t axis, void *aux) {
  state_pairs_t *pair = (state_pairs_t *)aux;
  set_coins(pair->state, get_coins(pair->state) + 1);
  set_coins_change(pair->state, true);
  body_remove(body2);
  list_free(body_get_shape(body2));
}

void create_one_body_remove(scene_t *scene, state_t *state, body_t *body1,
                                  body_t *body2) {
  state_pairs_t *pair = malloc(sizeof(state_pairs_t));
  pair->body1 = body1;
  pair->body2 = body2;
  pair->state = state;
  create_collision(scene, body1, body2,
                   (collision_handler_t)collision_handler_one_body_remove,
                   pair, free);
}


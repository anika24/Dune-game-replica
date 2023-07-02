#include "collision.h"
#include "assert.h"
#include "body.h"
#include "forces.h"
#include "list.h"
#include "math.h"
#include "polygon.h"
#include "scene.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct extrema_info {
  double max;
  double min;
} extrema_info_t;

typedef struct collision_mag {
  collision_info_t collision;
  double mag;
} collision_mag_t;

vector_t make_unit_vector(list_t *shape1, list_t *shape2, size_t i) {
  vector_t *v1 = (vector_t *)list_get(shape1, i);
  vector_t *v2 = (vector_t *)list_get(shape1, (i + 1) % list_size(shape1));
  vector_t side = vec_subtract(*v1, *v2);
  double mag = sqrt(vec_dot(side, side));
  vector_t unit_side = vec_multiply(1.0 / mag, side);
  vector_t unit = vec_rotate(unit_side, M_PI / 2.0);
  return unit;
}

extrema_info_t collision_get_extrema(vector_t unit, list_t *shape) {
  vector_t *v_10 = (vector_t *)list_get(shape, 0);
  double dot = vec_dot(*v_10, unit);
  double max = dot;
  double min = dot;
  for (size_t j = 1; j < list_size(shape); j++) {
    vector_t *v = (vector_t *)list_get(shape, j);
    double val = vec_dot(*v, unit);
    if (val > max) {
      max = val;
    } else if (min > val) {
      min = val;
    }
  }
  extrema_info_t collision;
  collision.max = max;
  collision.min = min;
  return collision;
}

double max(double a, double b) {
  if (a > b) {
    return a;
  }
  return b;
}

double min(double a, double b) {
  if (a < b) {
    return a;
  }
  return b;
}

collision_mag_t collision_helper(list_t *shape1, list_t *shape2) {
  collision_info_t col;
  collision_mag_t info;
  col.collided = false;
  for (size_t i = 0; i < list_size(shape1); i++) {
    vector_t unit = make_unit_vector(shape1, shape2, i);
    extrema_info_t f = collision_get_extrema(unit, shape1);
    extrema_info_t s = collision_get_extrema(unit, shape2);
    if (s.min > f.max || f.min > s.max) {
      col.collided = false;
      info.collision = col;
      return info;
    } else {
      double new_mag = min(f.max, s.max) - max(f.min, s.min);
      if (!col.collided) {
        col.collided = true;
        col.axis = vec_rotate(unit, M_PI);
        info.mag = new_mag;
      } else if (info.mag > new_mag) {
        col.axis = vec_rotate(unit, M_PI);
        info.mag = new_mag;
      }
    }
  }
  info.collision = col;
  return info;
}

collision_info_t find_collision(list_t *shape1, list_t *shape2) {
  collision_mag_t b1 = collision_helper(shape1, shape2);
  collision_mag_t b2 = collision_helper(shape2, shape1);
  collision_info_t b3;
  if (b1.collision.collided && b2.collision.collided) {
    b3.collided = true;
    if (b1.mag > b2.mag) {
      b3.axis = b2.collision.axis;
    } else {
      b3.axis = b1.collision.axis;
    }
  } else {
    b3.collided = false;
  }
  list_free(shape1);
  list_free(shape2);
  return b3;
}

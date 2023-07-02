#include "vector.h"
#include "math.h"

const vector_t VEC_ZERO = {0.0, 0.0};

vector_t vec_add(vector_t v1, vector_t v2) {
  vector_t added = {v1.x + v2.x, v1.y + v2.y};
  return added;
}

vector_t vec_subtract(vector_t v1, vector_t v2) {
  vector_t subtracted = {v1.x - v2.x, v1.y - v2.y};
  return subtracted;
}

vector_t vec_negate(vector_t v) {
  vector_t negated = {-1 * v.x, -1 * v.y};
  return negated;
}

vector_t vec_multiply(double scalar, vector_t v) {
  vector_t multiplied = {scalar * v.x, scalar * v.y};
  return multiplied;
}

double vec_dot(vector_t v1, vector_t v2) {
  return (v1.x * v2.x) + (v1.y * v2.y);
}

double vec_cross(vector_t v1, vector_t v2) {
  return (v1.x * v2.y) - (v1.y * v2.x);
}

vector_t vec_rotate(vector_t v, double angle) {
  vector_t rotated = {(v.x * cos(angle)) - (v.y * sin(angle)),
                      (v.x * sin(angle)) + (v.y * cos(angle))};
  return rotated;
}

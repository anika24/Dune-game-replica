#include "vec_list.h"
#include "assert.h"
#include "list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct vec_list {
  vector_t **arr;
  size_t size;
  size_t capacity;
} vec_list_t;

vec_list_t *vec_list_init(size_t initial_size) {
  return (vec_list_t *)list_init(initial_size, free);
}

void vec_list_free(vec_list_t *list) { list_free((list_t *)list); }

size_t vec_list_size(vec_list_t *list) { return list_size((list_t *)list); }

vector_t *vec_list_get(vec_list_t *list, size_t index) {
  return (vector_t *)list_get((list_t *)list, index);
}

void vec_list_add(vec_list_t *list, vector_t *value) {
  list_add((list_t *)list, value);
}

vector_t *vec_list_remove(vec_list_t *list) {
  return (vector_t *)list_remove((list_t *)list, list->size - 1);
}

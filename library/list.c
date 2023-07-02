#include "list.h"
#include "assert.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct list {
  void **arr;
  size_t capacity;
  size_t size;
  free_func_t freer;
} list_t;

list_t *list_init(size_t initial_size, free_func_t func) {
  list_t *l = malloc(sizeof(list_t));
  l->arr = malloc(sizeof(void *) * initial_size);
  l->capacity = initial_size;
  l->size = 0;
  if (func != NULL) {
    l->freer = func;
  }
  return l;
}

void list_free(list_t *list) {
  for (size_t i = 0; i < list->size; i++) {
    if (list->freer != NULL) {
      (list->freer)(list->arr[i]);
    }
  }
  free(list->arr);
  free(list);
}

size_t list_size(list_t *list) { return list->size; }

void *list_get(list_t *list, size_t index) {
  assert(index < list->size);
  assert(index >= 0);
  return list->arr[index];
}

void *list_remove(list_t *list, size_t index) {
  assert(index < list->size);
  assert(index >= 0);
  assert(list->size != 0);
  void *v = list->arr[index];
  for (size_t i = index; i < list->size - 1; i++) {
    list->arr[i] = list->arr[i + 1];
  }
  list->size--;
  return v;
}

list_t *resizes(list_t *list) {
  if (list->capacity == 0) {
    list->capacity++;
  }
  void **new_arr = malloc(sizeof(void *) * (list->capacity * 2));
  list->capacity = 2 * list->capacity;
  for (size_t i = 0; i < list->size; i++) {
    new_arr[i] = list->arr[i];
  }
  free(list->arr);
  list->arr = new_arr;
  return list;
}

void list_add(list_t *list, void *value) {
  assert(value != NULL);
  if (list->size >= list->capacity) {
    list = resizes(list);
  }
  list->arr[list->size] = value;
  list->size++;
}

void list_add_front(list_t *list, void *value) {
  assert(value != NULL);
  if (list->size >= list->capacity) {
    list = resizes(list);
  }
  for (size_t i = list->size; i >= 1; i--) {
    list->arr[i] = list->arr[i - 1];
  }
  list->arr[0] = value;
  list->size++;
}

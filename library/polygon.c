#include "polygon.h"
#include "list.h"
#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct polygon {
  list_t *arr;
  size_t size;
  rgb_color_t color;
  vector_t *velocity;
} polygon_t;

polygon_t *polygon_init(list_t *v, float r, float g, float b,
                        double velocity_x) {
  polygon_t *p = malloc(sizeof(polygon_t));
  p->arr = v;
  p->color = (rgb_color_t) {r, g, b};
  p->size = list_size(v);
  vector_t *velo = malloc(sizeof(vector_t));
  velo->x = velocity_x;
  velo->y = 0;
  p->velocity = velo;
  return p;
}

list_t *polygon_get_points(polygon_t *p){
  return p->arr;
}

rgb_color_t polygon_get_color(polygon_t *p){
  return p->color;
}

vector_t *polygon_get_velocity(polygon_t *p) { 
  return p->velocity; 
}

double polygon_area(list_t *polygon) {
  double area = 0.0;
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *v1 = (vector_t *)list_get(polygon, i % list_size(polygon));
    vector_t *v2 = (vector_t *)list_get(polygon, (i + 1) % list_size(polygon));
    area += vec_cross(*v1, *v2);
  }
  return 0.5 * area;
}

vector_t polygon_centroid(list_t *polygon) {
  double Cx = 0.0;
  double Cy = 0.0;
  double area = polygon_area(polygon);
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *v1 = (vector_t *)list_get(polygon, i % list_size(polygon));
    vector_t *v2 = (vector_t *)list_get(polygon, (i + 1) % list_size(polygon));
    Cx += (vec_cross(*v1, *v2) * (v1->x + v2->x));
    Cy += (vec_cross(*v1, *v2) * (v1->y + v2->y));
  }
  Cx = Cx / (6 * area);
  Cy = Cy / (6 * area);
  vector_t centroid = {Cx, Cy};
  return centroid;
}

void polygon_translate(list_t *polygon, vector_t translation) {
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *v1 = (vector_t *)list_get(polygon, i);
    vector_t v3 = vec_add(translation, *v1);
    v1->x = v3.x;
    v1->y = v3.y;
  }
}

void polygon_rotate(list_t *polygon, double angle, vector_t point) {
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *v1 = list_get(polygon, i);
    vector_t v2 = {v1->x - point.x, v1->y - point.y};
    vector_t v3 = vec_rotate(v2, angle);
    v1->x = v3.x + point.x;
    v1->y = v3.y + point.y;
  }
}

void polygon_free(polygon_t *p) {
  list_free(p->arr);
  free(p->velocity);
  free(p);
}

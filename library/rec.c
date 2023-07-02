#include "rec.h"
#include "assert.h"
#include "polygon.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct rec {
    double x;
    double y;
    double width;
    double height;
} rec_t;

rec_t *rec_init_circle(vector_t centroid, double d) {
    return rec_init_rectangle(centroid, d, d);
}

rec_t *rec_init_rectangle(vector_t cntr, double width, double height) {
    rec_t *rec = malloc(sizeof(rec_t));
    rec->x = cntr.x - width/2;
    rec->y = cntr.y + height/2;
    rec->width = width;
    rec->height = height; 
    return rec;
}

double rec_get_height(rec_t *rec) {
  return rec->height;
}

double rec_get_width(rec_t *rec) {
  return rec->width;
}

double rec_get_x(rec_t *rec) {
  return rec->x;
}

double rec_get_y(rec_t *rec) {
 return rec->y;
}




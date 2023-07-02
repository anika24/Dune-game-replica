#ifndef __REC_H__
#define __REC_H__

#include "list.h"
#include "assert.h"
#include "polygon.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Stores the values to get converted to an SDL_Rect.
 */
typedef struct rec rec_t;

/**
 * Initialize the SDL_Rect for a circle for a given body;
 */
rec_t *rec_init_circle(vector_t centroid, double d);

/**
 * Initialize the SDL_Rect for a rectangle for a given body;
 */
rec_t *rec_init_rectangle(vector_t cntr, double width, double length);

/**
 * Return the height of the rec_t.
 */
double rec_get_height(rec_t *rec);

/**
 * Return the width of the rec_t.
 */
double rec_get_width(rec_t *rec);

/**
 * Returns the x of the rec_t.
 */
double rec_get_x(rec_t *rec);

/**
 * Return the y of the rec_t.
 */
double rec_get_y(rec_t *rec);

#endif // #ifndef __REC_H__

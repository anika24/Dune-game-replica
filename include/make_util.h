#ifndef __MAKE_UTIL_H__
#define __MAKE_UTIL_H__

#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "list.h"
#include "polygon.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_image.h>

// color contants
extern const rgb_color_t RED;
extern const rgb_color_t ORANGE;
extern const rgb_color_t YELLOW;
extern const rgb_color_t LIGHT_GREEN;
extern const rgb_color_t GREEN;
extern const rgb_color_t DARK_GREEN;
extern const rgb_color_t LIGHT_BLUE;
extern const rgb_color_t DARK_BLUE;
extern const rgb_color_t ROYAL_BLUE;
extern const rgb_color_t PURPLE;

list_t *create_rainbow_color_list(size_t row_block_count);

double rand_double(double min, double max);

list_t *rect_init(double width, double height);

list_t *slope_init(scene_t *scene, vector_t window, double start_y, double dt, double a, double k, double c, double POINTS, void *info, vector_t velocity_points, double slope_length, double ball_centroid_x);

void make_dune(scene_t *scene, vector_t centroid, vector_t window, double circ_points, double mass, void *info, vector_t velocity, double start_y, double dt, double a, double k, double c, void *info2, double slope_length, double ball_centroid_x, rgb_color_t color);

void make_blocks(scene_t *scene, double length, vector_t window, double whitespace, double row_block_count, double column_block_count, double mass, void *info);

void make_ball(scene_t *scene, double radius, vector_t velocity, vector_t centroid, rgb_color_t color, double mass, void *info, double points, SDL_Surface *image);

list_t *circle_init(double radius, double points);

void make_line(scene_t *scene, double width, double length, vector_t centroid, rgb_color_t color, double mass, void *info, vector_t velocity, int alpha);

void make_text_box(scene_t *scene, double width, double length, vector_t centroid, rgb_color_t color, double mass, void *info, SDL_Surface *surface);

void make_arrow_up(scene_t *scene, double radius, vector_t velocity, vector_t centroid, rgb_color_t color, double mass, void *info);

void make_arrow_down(scene_t *scene, double radius, vector_t velocity, vector_t centroid, rgb_color_t color, double mass, void *info);

#endif
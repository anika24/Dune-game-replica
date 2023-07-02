#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "color.h"
#include "vector.h"
#include "math.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

// Zero vector
extern const vector_t ZERO_VEC;

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
extern const rgb_color_t WHITE;
extern const rgb_color_t BLACK;
extern const rgb_color_t BROWN;
extern const rgb_color_t TAN;
extern const rgb_color_t GREY;
extern const rgb_color_t SALMON;

// constants for Window Sizing
const extern vector_t WINDOW_BOTTOM_LEFT;
const extern vector_t WINDOW_TOP_RIGHT;

// Constant between levels;
const extern double DUNE_MASS;
const extern double THRESHOLD_Y_SHIFT;
const extern double JUMP_FACTOR;
const extern double DERIV_JUMP_CONSTANT;
const extern double COIN_RANGE;
const extern double DERIV_MIN;
const extern double DERIV_MAX;
const extern double THRESHOLD_DIFF;

// constants for ball
const extern double BALL_MASS;
const extern rgb_color_t BALL_COLOR;
const extern double BALL_RADIUS;
const extern vector_t BALL_START_VELOCITY;
const extern double VELOCITY_FACTOR;
const extern double CIRCLE_POINTS;
const extern double GRAVITY_COEFFICIENT;
const extern double DUNE_START;

// Coin constants
const extern double COIN_RADIUS;
const extern double COIN_MASS;
const extern double COIN_SPACING;
const extern double COIN_TIME_INTERVAL;
const extern double COIN_MIN;
const extern double COIN_MAX;

// Line constants
const extern double POINTS_LINE_LENGTH;
const extern double POINT_LINE_MASS;
const extern vector_t POINTS_LINE_CENTROID;
const extern double POINTS_LINE_LENGTH_2;
const extern vector_t WIN_CENTROID;
const extern int FULL_COLOR;
const extern int PART_COLOR;

// Arrow constants
const extern vector_t ARROW_UP_CENTROID;
const extern vector_t ARROW_DOWN_CENTROID;
const extern int ALPHA_1;
const extern int ALPHA_2;

//Sound constants
const extern int FREQUENCY;
const extern int CHANNELS;
const extern int CHUNK_SIZE;

// Level 1
const extern vector_t BALL_CENTROID_1;
const extern vector_t DUNE_CENTROID_1;
const extern vector_t DUNE_VELOCITY_1;
const extern double START_Y_1;
const extern double DX_1;
const extern double AMP_1;
const extern double X_STRETCH_1;
const extern double Y_TRANSLATION_1;
const extern double slope_length_1;
const extern double LEVEL_1_DISTANCE;
const extern vector_t POINTS_LINE_CENTROID_1;

// Level 2
const extern vector_t BALL_CENTROID_2;
const extern vector_t DUNE_CENTROID_2;
const extern vector_t DUNE_VELOCITY_2;
const extern double START_Y_2;
const extern double DX_2;
const extern double AMP_2;
const extern double X_STRETCH_2;
const extern double Y_TRANSLATION_2;
const extern double slope_length_2;
const extern double LEVEL_2_DISTANCE;
const extern vector_t POINTS_LINE_CENTROID_2;

// Level 3
const extern vector_t BALL_CENTROID_3;
const extern vector_t DUNE_CENTROID_3;
const extern vector_t DUNE_VELOCITY_3;
const extern double START_Y_3;
const extern double DX_3;
const extern double AMP_3;
const extern double X_STRETCH_3;
const extern double Y_TRANSLATION_3;
const extern double slope_length_3;
const extern double LEVEL_3_DISTANCE;
const extern vector_t POINTS_LINE_CENTROID_3;

// Level 4
const extern vector_t BALL_CENTROID_4;
const extern vector_t DUNE_CENTROID_4;
const extern vector_t DUNE_VELOCITY_4;
const extern double START_Y_4;
const extern double DX_4;
const extern double AMP_4;
const extern double X_STRETCH_4;
const extern double Y_TRANSLATION_4;
const extern double slope_length_4;
const extern double LEVEL_4_DISTANCE;
const extern vector_t POINTS_LINE_CENTROID_4;

// Level 5
const extern vector_t BALL_CENTROID_5;
const extern vector_t DUNE_CENTROID_5;
const extern vector_t DUNE_VELOCITY_5;
const extern double START_Y_5;
const extern double DX_5;
const extern double AMP_5;
const extern double X_STRETCH_5;
const extern double Y_TRANSLATION_5;
const extern double slope_length_5;
const extern double LEVEL_5_DISTANCE;
const extern vector_t POINTS_LINE_CENTROID_5;

// Backround Constants
const extern double BACKROUND_MASS;
const extern rgb_color_t NEW_GREEN;

//Constants for Coins and Points Text
const extern double POINTS_TEXT_WIDTH;
const extern double POINTS_TEXT_HEIGHT;
const extern vector_t POINTS_TEXT_CENTROID;
const extern double COINS_TEXT_WIDTH;
const extern double COINS_TEXT_HEIGHT;
const extern vector_t COINS_TEXT_CENTROID;


//SDL Colors
const extern SDL_Color SDL_RED;
const extern SDL_Color SDL_BLACK;
const extern SDL_Color SDL_WHITE;

//Angle for ball
const extern double ANGLE_TO_ADD_TO_BALL;

const extern double BORDER_OFFSET;
const extern vector_t HOPPER_CENTROID;

//Font
const extern int FONT_SIZE;

#endif // #ifndef __CONSTANTS_H__

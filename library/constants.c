#include "constants.h"

// Zero vector
const vector_t ZERO_VEC = {0, 0};

// color contants
const rgb_color_t RED = {1, 0, 0};
const rgb_color_t ORANGE = {0.95, .8, 0.5};
const rgb_color_t SALMON = {0.95, .85, 0.8};
const rgb_color_t YELLOW = {1.0, 1.0, 0};
const rgb_color_t LIGHT_GREEN = {.5, 1.0, 0};
const rgb_color_t GREEN = {0, 1.0, 0};
const rgb_color_t DARK_GREEN = {0.1, 0.2, 0};
const rgb_color_t LIGHT_BLUE = {0.92, 0.96, 1.0};
const rgb_color_t DARK_BLUE = {0, .5, 1.0};
const rgb_color_t ROYAL_BLUE = {0, 0, 1.0};
const rgb_color_t PURPLE = {.16, 0.08, 0.4};
const rgb_color_t WHITE = {1.0, 1.0, 1.0};
const rgb_color_t BLACK = {0, 0, 0};
const rgb_color_t BROWN = {0.6, 0.3, 0.2};
const rgb_color_t TAN = {0.9, 0.85, 0.60};
const rgb_color_t GREY = {0.74, 0.75, 0.8};

// constants for Window Sizing
const vector_t WINDOW_BOTTOM_LEFT = {0, 0};
const vector_t WINDOW_TOP_RIGHT = {1000.0, 500.0};

// Constant between levels;
const double DUNE_MASS = INFINITY;
const double THRESHOLD_Y_SHIFT  = 150;
const double JUMP_FACTOR = -1.25;
const double DERIV_JUMP_CONSTANT = 0.3;
const double COIN_RANGE = 80;
const double DERIV_MIN = 0.5;
const double DERIV_MAX = 0.9;
const double THRESHOLD_DIFF = 10;

// constants for ball
const double BALL_MASS = 100;
const rgb_color_t BALL_COLOR = {0, 0, 1.0};
const double BALL_RADIUS = 17.0;
const vector_t BALL_START_VELOCITY = {0, -150};
const double VELOCITY_FACTOR = 50;
const double CIRCLE_POINTS = 50;
const double GRAVITY_COEFFICIENT = -100;
const double DUNE_START = 200;

// Coin constants
const double COIN_RADIUS = 13;
const double COIN_MASS = INFINITY;
const double COIN_SPACING = 30;
const double COIN_TIME_INTERVAL = 3;
const double COIN_MIN = 1;
const double COIN_MAX = 4;

// Line constants
const double POINTS_LINE_LENGTH = 3;
const double POINT_LINE_MASS = 0;
const vector_t POINTS_LINE_CENTROID = {1000, 250};
const double POINTS_LINE_LENGTH_2 = 5;
const vector_t WIN_CENTROID = {1500, 250};
const int FULL_COLOR = 255;
const int PART_COLOR = 80;

// Backround Constants
const double BACKROUND_MASS = 0;
const rgb_color_t NEW_GREEN = {0, 1, 0};

//Constants for Coins and Points Text
const double POINTS_TEXT_WIDTH = 100;
const double POINTS_TEXT_HEIGHT = 50;
const vector_t POINTS_TEXT_CENTROID = {925, 470};
const double COINS_TEXT_WIDTH = 100;
const double COINS_TEXT_HEIGHT = 50;
const vector_t COINS_TEXT_CENTROID = {925, 420};

// Arrow constants
const vector_t ARROW_UP_CENTROID = {480, 470};
const vector_t ARROW_DOWN_CENTROID = {520, 470};
const int ALPHA_1 = 30;
const int ALPHA_2 = 140;

//SDL Colors
const SDL_Color SDL_RED = {255, 0, 0};
const SDL_Color SDL_BLACK = {0, 0, 0};
const SDL_Color SDL_WHITE = {255, 255, 255};

//Angle for ball
const double ANGLE_TO_ADD_TO_BALL = 3;

const double BORDER_OFFSET = 15;
const vector_t HOPPER_CENTROID = {500, 250};

//Sound constants
const int FREQUENCY = 44100;
const int CHANNELS = 2;
const int CHUNK_SIZE = 1024;

// Level 1
const vector_t BALL_CENTROID_1 = {50, 300};
const vector_t DUNE_CENTROID_1 = {15, 67};
const vector_t DUNE_VELOCITY_1 = {-160, 0};
const double START_Y_1 = 150;
const double DX_1 = 1;
const double AMP_1 = 60;
const double X_STRETCH_1 = 0.015;
const double Y_TRANSLATION_1 = 0;
const double slope_length_1 = 40;
const double LEVEL_1_DISTANCE = 6000;
const vector_t POINTS_LINE_CENTROID_1 = {500, 350};

// Level 2
const vector_t BALL_CENTROID_2 = {50, 270};
const vector_t DUNE_CENTROID_2 = {20, 65};
const vector_t DUNE_VELOCITY_2 = {-180, 0};
const double START_Y_2 = 150;
const double DX_2 = 1;
const double AMP_2 = 50;
const double X_STRETCH_2 = 0.018;
const double Y_TRANSLATION_2 = 0;
const double slope_length_2 = 50;
const double LEVEL_2_DISTANCE = 6500;
const vector_t POINTS_LINE_CENTROID_2 = {500, 375};

// Level 3
const vector_t BALL_CENTROID_3 = {50, 240};
const vector_t DUNE_CENTROID_3 = {18, 65};
const vector_t DUNE_VELOCITY_3 = {-185, 0};
const double START_Y_3 = 150;
const double DX_3 = 1;
const double AMP_3 = 65;
const double X_STRETCH_3 = 0.021;
const double Y_TRANSLATION_3 = 0;
const double slope_length_3 = 58;
const double LEVEL_3_DISTANCE = 7500;
const vector_t POINTS_LINE_CENTROID_3 = {500, 390};

// Level 4
const vector_t BALL_CENTROID_4 = {50, 225};
const vector_t DUNE_CENTROID_4 = {18, 65};
const vector_t DUNE_VELOCITY_4 = {-190, 0};
const double START_Y_4 = 150;
const double DX_4 = 1;
const double AMP_4 = 70;
const double X_STRETCH_4 = 0.024;
const double Y_TRANSLATION_4 = 0;
const double slope_length_4 = 68;
const double LEVEL_4_DISTANCE = 8000;
const vector_t POINTS_LINE_CENTROID_4 = {500, 400};

// Level 5
const vector_t BALL_CENTROID_5 = {50, 225};
const vector_t DUNE_CENTROID_5 = {17, 64};
const vector_t DUNE_VELOCITY_5 = {-190, 0};
const double START_Y_5 = 150;
const double DX_5 = 1;
const double AMP_5 = 75;
const double X_STRETCH_5 = 0.026;
const double Y_TRANSLATION_5 = 0;
const double slope_length_5 = 80;
const double LEVEL_5_DISTANCE = 8500;
const vector_t POINTS_LINE_CENTROID_5 = {500, 400};

//Font 
const int FONT_SIZE = 24;
#include "body.h"
#include "collision.h"
#include "constants.h"
#include "color.h"
#include "forces.h"
#include "list.h"
#include "polygon.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"
#include "make_util.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rec.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

typedef enum {BALL = 1, DUNE = 2, DUNE_PT = 3, BACKROUND = 4, LINE = 5, COIN = 6, ARROW = 7} dune_t;

dune_t *info_init(dune_t b) {
  dune_t *info = malloc(sizeof(dune_t));
  *info = b;
  return info;
}

dune_t get_type(body_t *body) { return *(dune_t *)body_get_info(body); }

void clear_scene(state_t *state){
  scene_t *scene = get_scene(state);
  for (int i = 0; i < scene_bodies(scene); i++){
    body_remove(scene_get_body(scene, i));
  }
  scene_free(scene);
  set_velocity(state, ZERO_VEC);
  set_time(state, 0);
  set_coin_time(state, 0);
  set_deriv(state, 0);
  set_coins(state, 0);
  set_above_line_bool(state, false);
  set_twice_above_line_bool(state, false);
  set_points(state, 0);
  set_points_change(state, false);
  set_coins_change(state, false);
  set_drawn_line(state, false);
}

void clear_pause(state_t *state){
  scene_t *scene = get_scene(state);
  for (int i = 0; i < scene_bodies(scene); i++){
    body_remove(scene_get_body(scene, i));
  }
  scene_free(scene);
}

void make_backround(scene_t *scene, vector_t window, double mass, rgb_color_t color, void *info, SDL_Surface *image) {
  list_t *backround_shape = rect_init(window.x, window.y);
  body_t *backround = body_init_with_image(backround_shape, mass, color, info, free, image, NULL);
  vector_t c = {window.x/2, window.y/2};
  body_set_centroid(backround, c);
  scene_add_body(scene, backround);
}

void make_backround_lvl1(scene_t *scene, vector_t window, double mass, rgb_color_t color, void *info) {
  SDL_Surface * image = IMG_Load("assets/lvl1.png");
  make_backround(scene, window, mass, color, info, image);
}

void make_backround_lvl2(scene_t *scene, vector_t window, double mass, rgb_color_t color, void *info) {
  SDL_Surface * image = IMG_Load("assets/lvl2.png");
  make_backround(scene, window, mass, color, info, image);
}

void make_backround_lvl3(scene_t *scene, vector_t window, double mass, rgb_color_t color, void *info) {
  SDL_Surface * image = IMG_Load("assets/lvl3.jpg");
  make_backround(scene, window, mass, color, info, image);
}

void make_backround_lvl4(scene_t *scene, vector_t window, double mass, rgb_color_t color, void *info) {
  SDL_Surface * image = IMG_Load("assets/lvl4.png");
  make_backround(scene, window, mass, color, info, image);
}

void make_backround_lvl5(scene_t *scene, vector_t window, double mass, rgb_color_t color, void *info) {
  SDL_Surface * image = IMG_Load("assets/lvl5.png");
  make_backround(scene, window, mass, color, info, image);
}
 
void make_end_menu_backround_lvl1_win(state_t *state) {
  scene_t *end1 = scene_init();
  SDL_Surface *image = IMG_Load("assets/1win.png");
  make_backround(end1, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level1_win(state, end1);
}

void make_end_menu_backround_lvl2_win(state_t *state) {
  scene_t *end2 = scene_init();
  SDL_Surface *image = IMG_Load("assets/2win.png");
  make_backround(end2, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level2_win(state, end2);
}

void make_end_menu_backround_lvl3_win(state_t *state) {
  scene_t *end3 = scene_init();
  SDL_Surface *image = IMG_Load("assets/3win.png");
  make_backround(end3, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level3_win(state, end3);
}

void make_end_menu_backround_lvl4_win(state_t *state) {
  scene_t *end4 = scene_init();
  SDL_Surface *image = IMG_Load("assets/4win.png");
  make_backround(end4, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level4_win(state, end4);
}

void make_end_menu_backround_lvl5_win(state_t *state) {
  scene_t *end5 = scene_init();
  SDL_Surface *image = IMG_Load("assets/5win.png");
  make_backround(end5, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level5_win(state, end5);
}

void make_end_menu_backround_lvl1_lose(state_t *state) {
  scene_t *end1 = scene_init();
  SDL_Surface *image = IMG_Load("assets/1lose.png");
  make_backround(end1, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level1_lose(state, end1);
}

void make_end_menu_backround_lvl2_lose(state_t *state) {
  scene_t *end2 = scene_init();
  SDL_Surface *image = IMG_Load("assets/2lose.png");
  make_backround(end2, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level2_lose(state, end2);
}

void make_end_menu_backround_lvl3_lose(state_t *state) {
  scene_t *end3 = scene_init();
  SDL_Surface *image = IMG_Load("assets/3lose.png");
  make_backround(end3, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level3_lose(state, end3);
}

void make_end_menu_backround_lvl4_lose(state_t *state) {
  scene_t *end4 = scene_init();
  SDL_Surface *image = IMG_Load("assets/4lose.png");
  make_backround(end4, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level4_lose(state, end4);
}

void make_end_menu_backround_lvl5_lose(state_t *state) {
  scene_t *end5 = scene_init();
  SDL_Surface *image = IMG_Load("assets/5lose.png");
  make_backround(end5, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_lost(state, false);
  set_won(state, false);
  set_end_menu_level5_lose(state, end5);
}

SDL_Color text_color_for_end_menu(state_t *state) {
  int lvl = get_current_scene(state);
  if (lvl == 1 || lvl == 3 || lvl == 5) {
    return SDL_WHITE;
  }
  return SDL_BLACK;
}

void remake_end_scenes_win(state_t *state){
  if (get_current_scene(state) == 1){
    make_end_menu_backround_lvl1_win(state);
  }
  else if (get_current_scene(state) == 2){
    make_end_menu_backround_lvl2_win(state);
  }
  else if (get_current_scene(state) == 3){
    make_end_menu_backround_lvl3_win(state);
  }
  else if (get_current_scene(state) == 4){
    make_end_menu_backround_lvl4_win(state);
  }
  else if (get_current_scene(state) == 5){
    make_end_menu_backround_lvl5_win(state);
  }
}

void remake_end_scenes_lose(state_t *state){
  if (get_current_scene(state) == 1){
    make_end_menu_backround_lvl1_lose(state);
  }
  else if (get_current_scene(state) == 2){
    make_end_menu_backround_lvl2_lose(state);
  }
  else if (get_current_scene(state) == 3){
    make_end_menu_backround_lvl3_lose(state);
  }
  else if (get_current_scene(state) == 4){
    make_end_menu_backround_lvl4_lose(state);
  }
  else if (get_current_scene(state) == 5){
    make_end_menu_backround_lvl5_lose(state);
  }
}

void set_win_end_menu(state_t *state) {
  scene_t *end_menu = get_current_end_menu_win(state);
  set_level_before_end(state, get_current_scene(state));
  clear_scene(state);  
  set_won(state, true);
  load_end_menu(state, end_menu);
}

void set_lose_end_menu(state_t *state) {
  scene_t *end_menu = get_current_end_menu_lose(state);
  clear_scene(state); 
  set_lost(state, true);
  load_end_menu(state, end_menu);
}

double check_win(state_t *state, double total_time, double distance){
  double dist = fabs(get_velocity(state).x * total_time);
  if (dist + WINDOW_TOP_RIGHT.x >= distance && !get_drawn_line(state)){
    make_line(get_scene(state), POINTS_LINE_LENGTH_2, WINDOW_TOP_RIGHT.y, POINTS_LINE_CENTROID, WHITE, BALL_MASS, info_init((dune_t)LINE), get_velocity(state), FULL_COLOR);
    make_line(get_scene(state), WINDOW_TOP_RIGHT.x, WINDOW_TOP_RIGHT.y, WIN_CENTROID, WHITE, BALL_MASS, info_init((dune_t)LINE), get_velocity(state), PART_COLOR);
    set_drawn_line(state, true);
  }
  if (dist > distance){
    remake_end_scenes_win(state);
    set_won(state, true);
    set_win_end_menu(state);
  }
  return dist;
}

void check_lose(state_t *state, double deriv){
  body_t *ball = scene_get_body(get_scene(state), 3);
  if (deriv > 0 && get_first_hit(state) == true && body_get_velocity(ball).y > get_velocity(state).x){
    remake_end_scenes_lose(state);
    set_lost(state, true);
    set_lose_end_menu(state);
  }
}


void check_above_line(state_t *state) {
  scene_t *scene = get_scene(state);
  body_t *ball = scene_get_body(scene, 4);
  body_t *line = scene_get_body(scene, 3);
  vector_t centroid = body_get_centroid(ball);
  vector_t centroid_line = body_get_centroid(line);
  if (centroid.y >= centroid_line.y) {
    if (!get_above_line_bool(state)) {
      set_above_line_bool(state, true);
      set_points(state, get_points(state) + 1);
    }
  } else {
    set_above_line_bool(state, false);
  }
}

void check_twice_above_line(state_t *state) {
  scene_t *scene = get_scene(state);
  body_t *ball = scene_get_body(scene, 4);
  body_t *line = scene_get_body(scene, 3);
  vector_t centroid = body_get_centroid(ball);
  vector_t centroid_line = body_get_centroid(line);
  if (centroid.y >= 1.5 * centroid_line.y) {
    if (!get_twice_above_line_bool(state)) {
      set_twice_above_line_bool(state, true);
      set_points(state, get_points(state) + 4);
    }
  } else {
    set_twice_above_line_bool(state, false);
  }
}

double get_derivative(state_t *state, double amp, double x_stretch){
  double total_time = get_total_time(state);
  double deriv = -amp * x_stretch * sin(x_stretch * (total_time * -1 * get_velocity(state).x));
  set_deriv(state, deriv);
  return deriv;
}

void make_init_text_points_coins(scene_t *scene, SDL_Color color) {
  TTF_Font* Sans = TTF_OpenFont("assets/Sans.ttf", FONT_SIZE);
  SDL_Surface* points_text = TTF_RenderText_Solid(Sans, "Points: 0", color); 
  SDL_Surface* coins_text = TTF_RenderText_Solid(Sans, "Coins: 0", color); 
  make_text_box(scene, POINTS_TEXT_WIDTH, POINTS_TEXT_HEIGHT, POINTS_TEXT_CENTROID, GREEN, BALL_MASS, NULL, points_text);
  make_text_box(scene, COINS_TEXT_WIDTH, COINS_TEXT_HEIGHT, COINS_TEXT_CENTROID, GREEN, BALL_MASS, NULL, coins_text);
  TTF_CloseFont(Sans);
}

void level1_init(state_t *state){
  scene_t *one = scene_init();
  make_backround_lvl1(one, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND));
  make_dune(one, DUNE_CENTROID_1, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_1, START_Y_1, DX_1, AMP_1, X_STRETCH_1, Y_TRANSLATION_1 + BORDER_OFFSET, info_init((dune_t) DUNE_PT), slope_length_1, BALL_CENTROID_1.x, LIGHT_BLUE);
  make_dune(one, DUNE_CENTROID_1, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_1, START_Y_1, DX_1, AMP_1, X_STRETCH_1, Y_TRANSLATION_1, info_init((dune_t) DUNE_PT), slope_length_1, BALL_CENTROID_1.x, GREY);
  make_line(one, WINDOW_TOP_RIGHT.x, POINTS_LINE_LENGTH, POINTS_LINE_CENTROID_1, BLACK, BALL_MASS, info_init((dune_t)LINE), ZERO_VEC, FULL_COLOR);
  make_ball(one, BALL_RADIUS, BALL_START_VELOCITY, BALL_CENTROID_1, RED, BALL_MASS, info_init((dune_t)BALL), CIRCLE_POINTS, IMG_Load("assets/skin5.png"));
  make_init_text_points_coins(one, SDL_BLACK);
  make_arrow_up(one, BALL_RADIUS, ZERO_VEC, ARROW_UP_CENTROID, RED, DUNE_MASS, info_init((dune_t)ARROW));
  make_arrow_down(one, BALL_RADIUS, ZERO_VEC, ARROW_DOWN_CENTROID, RED, DUNE_MASS, info_init((dune_t)ARROW));
  set_level1(state, one);
  set_lost(state, false);
  set_won(state, false);
}

void level2_init(state_t *state){
  scene_t *two = scene_init();
  make_backround_lvl2(two, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND));
  make_dune(two, DUNE_CENTROID_2, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_2, START_Y_2, DX_2, AMP_2, X_STRETCH_2, Y_TRANSLATION_2 + BORDER_OFFSET, info_init((dune_t) DUNE_PT), slope_length_2, BALL_CENTROID_2.x, WHITE);
  make_dune(two, DUNE_CENTROID_2, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_2, START_Y_2, DX_2, AMP_2, X_STRETCH_2, Y_TRANSLATION_2, info_init((dune_t) DUNE_PT), slope_length_2, BALL_CENTROID_2.x, ORANGE);
  make_line(two, WINDOW_TOP_RIGHT.x, POINTS_LINE_LENGTH, POINTS_LINE_CENTROID_2, BLACK, BALL_MASS, info_init((dune_t)LINE), ZERO_VEC, FULL_COLOR);
  make_ball(two, BALL_RADIUS, BALL_START_VELOCITY, BALL_CENTROID_2, RED, BALL_MASS, info_init((dune_t)BALL), CIRCLE_POINTS, IMG_Load("assets/hopper.png"));
  make_init_text_points_coins(two, SDL_BLACK);
  make_arrow_up(two, BALL_RADIUS, ZERO_VEC, ARROW_UP_CENTROID, RED, DUNE_MASS, info_init((dune_t)ARROW));
  make_arrow_down(two, BALL_RADIUS, ZERO_VEC, ARROW_DOWN_CENTROID, RED, DUNE_MASS, info_init((dune_t)ARROW));
  set_level2(state, two);
  set_lost(state, false);
  set_won(state, false);
}

void level3_init(state_t *state){
  scene_t *three = scene_init();
  make_backround_lvl3(three, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND));
  make_dune(three, DUNE_CENTROID_3, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_3, START_Y_3, DX_3, AMP_3, X_STRETCH_3, Y_TRANSLATION_3 + BORDER_OFFSET, info_init((dune_t) DUNE_PT), slope_length_3, BALL_CENTROID_3.x, SALMON);
  make_dune(three, DUNE_CENTROID_3, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_3, START_Y_3, DX_3, AMP_3, X_STRETCH_3, Y_TRANSLATION_3, info_init((dune_t) DUNE_PT), slope_length_3, BALL_CENTROID_3.x, BROWN);
  make_line(three, WINDOW_TOP_RIGHT.x, POINTS_LINE_LENGTH, POINTS_LINE_CENTROID_3, BLACK, BALL_MASS, info_init((dune_t)LINE), ZERO_VEC, FULL_COLOR);
  make_ball(three, BALL_RADIUS, BALL_START_VELOCITY, BALL_CENTROID_3, RED, BALL_MASS, info_init((dune_t)BALL), CIRCLE_POINTS, IMG_Load("assets/skin3.png"));
  make_init_text_points_coins(three, SDL_BLACK);
  make_arrow_up(three, BALL_RADIUS, ZERO_VEC, ARROW_UP_CENTROID, RED, DUNE_MASS, info_init((dune_t)ARROW));
  make_arrow_down(three, BALL_RADIUS, ZERO_VEC, ARROW_DOWN_CENTROID, RED, DUNE_MASS, info_init((dune_t)ARROW));
  set_level3(state, three);
  set_lost(state, false);
  set_won(state, false);
}

void level4_init(state_t *state){
  scene_t *four = scene_init();
  make_backround_lvl4(four, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND));
  make_dune(four, DUNE_CENTROID_4, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_4, START_Y_4, DX_4, AMP_4, X_STRETCH_4, Y_TRANSLATION_4 + BORDER_OFFSET, info_init((dune_t) DUNE_PT), slope_length_4, BALL_CENTROID_4.x, WHITE);
  make_dune(four, DUNE_CENTROID_4, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_4, START_Y_4, DX_4, AMP_4, X_STRETCH_4, Y_TRANSLATION_4, info_init((dune_t) DUNE_PT), slope_length_4, BALL_CENTROID_4.x, BLACK);
  make_line(four, WINDOW_TOP_RIGHT.x, POINTS_LINE_LENGTH, POINTS_LINE_CENTROID_4, BLACK, BALL_MASS, info_init((dune_t)LINE), ZERO_VEC, FULL_COLOR);
  make_ball(four, BALL_RADIUS, BALL_START_VELOCITY, BALL_CENTROID_4, RED, BALL_MASS, info_init((dune_t)BALL), CIRCLE_POINTS, IMG_Load("assets/skin4.png"));
  make_init_text_points_coins(four, SDL_BLACK);
  make_arrow_up(four, BALL_RADIUS, ZERO_VEC, ARROW_UP_CENTROID, LIGHT_GREEN, DUNE_MASS, info_init((dune_t)ARROW));
  make_arrow_down(four, BALL_RADIUS, ZERO_VEC, ARROW_DOWN_CENTROID, LIGHT_GREEN, DUNE_MASS, info_init((dune_t)ARROW));
  set_level4(state, four);
  set_lost(state, false);
  set_won(state, false);
}

void level5_init(state_t *state){
  scene_t *five = scene_init();
  make_backround_lvl5(five, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND));
  make_dune(five, DUNE_CENTROID_5, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_5, START_Y_5, DX_5, AMP_5, X_STRETCH_5, Y_TRANSLATION_5 + BORDER_OFFSET, info_init((dune_t) DUNE_PT), slope_length_5, BALL_CENTROID_5.x, WHITE);
  make_dune(five, DUNE_CENTROID_5, WINDOW_BOTTOM_LEFT, CIRCLE_POINTS, DUNE_MASS, info_init((dune_t) DUNE), DUNE_VELOCITY_5, START_Y_5, DX_5, AMP_5, X_STRETCH_5, Y_TRANSLATION_5, info_init((dune_t) DUNE_PT), slope_length_5, BALL_CENTROID_5.x, DARK_GREEN);
  make_line(five, WINDOW_TOP_RIGHT.x, POINTS_LINE_LENGTH, POINTS_LINE_CENTROID_5, WHITE, BALL_MASS, info_init((dune_t)LINE), ZERO_VEC, FULL_COLOR);
  make_ball(five, BALL_RADIUS, BALL_START_VELOCITY, BALL_CENTROID_5, RED, BALL_MASS, info_init((dune_t)BALL), CIRCLE_POINTS, IMG_Load("assets/skin2.png"));
  make_init_text_points_coins(five, SDL_WHITE);
  make_arrow_up(five, BALL_RADIUS, ZERO_VEC, ARROW_UP_CENTROID, LIGHT_GREEN, DUNE_MASS, info_init((dune_t)ARROW));
  make_arrow_down(five, BALL_RADIUS, ZERO_VEC, ARROW_DOWN_CENTROID, LIGHT_GREEN, DUNE_MASS, info_init((dune_t)ARROW));
  set_level5(state, five); 
  set_lost(state, false);
  set_won(state, false);
}

void start_screen_init(state_t *state){
  scene_t *start_scene = scene_init();
  SDL_Surface *image = IMG_Load("assets/dunestart.png");
  make_backround(start_scene, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_start(state, start_scene);
}

void pause_screen_init(state_t *state){
  scene_t *pause_scene = scene_init();
  SDL_Surface * image = IMG_Load("assets/dunepause.png");
  make_backround(pause_scene, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_pause(state, pause_scene);
}

void insturctions_init(state_t *state){
  scene_t *instructions = scene_init();
  SDL_Surface * image = IMG_Load("assets/instructions.png");
  make_backround(instructions, WINDOW_TOP_RIGHT, BACKROUND_MASS, WHITE, info_init((dune_t) BACKROUND), image);
  set_instructions(state, instructions);
}

void play_start_sound() {
  Mix_Chunk *effect = Mix_LoadWAV("assets/start.wav");
  if (!effect){
      printf("could not load start menu music\n");
  }
  int didPlay = Mix_PlayMusic(effect, -1);
  if (didPlay != 0){
      printf("could not play music\n");
  }
}

void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
  body_t *ball = NULL;
  vector_t the_velocity = ZERO_VEC;
  if (type == KEY_PRESSED) {
    switch (key) {
    case ZERO:
      if (get_current_scene(state) != 0){
        clear_scene(state);
        start_screen_init(state);
        load_start(state);
        set_lost(state, false);
        set_won(state, false);
      }
      break;
    case ONE:
      if (get_current_scene(state) == 0){
        clear_scene(state);
        level1_init(state);
        load_1(state);
      }
      else if (get_current_scene(state) == 8 && get_lost(state) && !get_won(state)){
        clear_scene(state);
        level1_init(state);
        load_1(state);
      }
      break;
    case TWO:
      if (get_current_scene(state) == 0){
        clear_scene(state);
        level2_init(state);
        load_2(state);
      }
      else if (!get_won(state)){
        if (get_current_scene(state) == 8 && get_lost(state)){
          clear_scene(state);
          level2_init(state);
          load_2(state);
        }
      }
      break;
    case THREE:
      if (get_current_scene(state) == 0){
        clear_scene(state);
        level3_init(state);
        load_3(state);
      }
      else if (get_current_scene(state) == 8 && get_lost(state) && !get_won(state)){
        clear_scene(state);
        level3_init(state);
        load_3(state);
      }
      break;
    case FOUR:
      if (get_current_scene(state) == 0){
        clear_scene(state);
        level4_init(state);
        load_4(state);
      }
      else if (get_current_scene(state) == 8 && get_lost(state) && !get_won(state)){
        clear_scene(state);
        level4_init(state);
        load_4(state);
      }
      break;
    case FIVE:
      if (get_current_scene(state) == 0){
        clear_scene(state);
        level5_init(state);
        load_5(state);
      }
      else if (get_current_scene(state) == 8 && get_lost(state) && !get_won(state)){
        clear_scene(state);
        level5_init(state);
        load_5(state);
      }
      break;
    case SPACE_BAR:
      if (get_current_scene(state) != 0 && get_current_scene(state) != 6 && get_current_scene(state) != 7 && get_current_scene(state) != 8){
        ball = scene_get_body(get_scene(state), 4);
        the_velocity = body_get_velocity(ball);
        if (body_get_centroid(ball).y > DUNE_START){
          the_velocity.y = the_velocity.y - VELOCITY_FACTOR;
        }
        else if (body_get_centroid(ball).y < DUNE_START){
          if (get_deriv(state) > DERIV_MIN && get_deriv(state) < DERIV_MAX){
            the_velocity.y = the_velocity.y + VELOCITY_FACTOR;
          }
        }
        body_set_velocity(ball, the_velocity);
      }
      else if (get_current_scene(state) == 6){
        clear_pause(state);
        load_after_pause(state);
      }
      break;
    case KEY_P:
      if (get_current_scene(state) != 0 && get_current_scene(state) != 6 && get_current_scene(state) != 7 && get_current_scene(state) != 8){
        set_before_pause(state, get_current_scene(state));
        pause_screen_init(state);
        load_pause_menu(state);
      }
      break;
    case KEY_I:
      if (get_current_scene(state) == 0){
        clear_scene(state);
        insturctions_init(state);
        load_instructions(state);
      }
      break;
    case RIGHT_ARROW:
      if (get_won(state)){
        if (get_current_scene(state) == 8){
          clear_scene(state);
          if (get_level_before_end(state) == 1){
            level2_init(state);
            load_2(state);
          }
          else if (get_level_before_end(state) == 2){
            level3_init(state);
            load_3(state);
          }
          else if (get_level_before_end(state) == 3){
            level4_init(state);
            load_4(state);
          }
          else if (get_level_before_end(state) == 4){
            level5_init(state);
            load_5(state);
          }
          break;
        }
      }
    }
  }
}

// Gravity
void moving_the_ball(state_t *state, double dt) {
  body_t *ball = scene_get_body(get_scene(state), 4);
  vector_t the_velocity = body_get_velocity(ball);
  vector_t new_velocity;
  if (body_get_centroid(ball).y > (START_Y_1+AMP_1)){
    set_first_hit(state, true);
    new_velocity.y = (&the_velocity)->y + GRAVITY_COEFFICIENT * dt;
    new_velocity.x = 0;
    body_set_velocity(ball, new_velocity);
  }
}

void moving_the_threshold(state_t *state, double amp, double x_stretch) {
  if (!get_won(state) && !get_lost(state)){
    double total_time = get_total_time(state);
    double deriv = get_derivative(state, amp, x_stretch);
    double threshold = amp * cos(x_stretch * (total_time * -1 * get_velocity(state).x)) + THRESHOLD_Y_SHIFT; 
    body_t *ball = scene_get_body(get_scene(state), 4);
    double centroid = body_get_centroid(ball).y;
    if (centroid - THRESHOLD_DIFF <= threshold){
      check_lose(state, deriv);
      if (!get_won(state) && !get_lost(state)){
        if (deriv > DERIV_JUMP_CONSTANT){
          body_set_velocity(ball, (vector_t) {0, JUMP_FACTOR * get_velocity(state).x * deriv});
        }
        else {
          body_set_velocity(ball, (vector_t) {0, get_velocity(state).x * -1 * deriv});
        }
        set_first_hit(state, false);
      }
    }
  }
}

void spawn_coins(state_t *state, double coin_spacing){
  double y_value = rand_double(START_Y_1 + AMP_1 + COIN_RANGE, WINDOW_TOP_RIGHT.y - COIN_RANGE);
  int rows = (int) (rand_double(COIN_MIN, COIN_MAX));
  int cols = (int) (rand_double(COIN_MIN, COIN_MAX));
  double x_value = WINDOW_TOP_RIGHT.x;
  vector_t initial_centroid = {x_value, y_value};
  vector_t centroid = {x_value, y_value};
  body_t *ball = scene_get_body(get_scene(state), 4);
  
  if (get_coin_time(state) > COIN_TIME_INTERVAL){
    for (int i = 0; i < rows; i++){
      for (int j = 0; j < cols; j++){
        centroid.x += coin_spacing;
        SDL_Surface *coin_image = IMG_Load("assets/coin.png");
        make_ball(get_scene(state), COIN_RADIUS, get_velocity(state), centroid, ORANGE, COIN_MASS, info_init((dune_t) COIN), CIRCLE_POINTS, coin_image);
        body_t *coin = scene_get_body(get_scene(state), scene_bodies(get_scene(state))-1);
        create_one_body_remove(get_scene(state), state, ball, coin);
      }
      centroid.x = initial_centroid.x;
      centroid.y += coin_spacing;
    }
    set_coin_time(state, get_coin_time(state) - COIN_TIME_INTERVAL);
  }
}

void remove_off_screen(state_t *state){
    for (int i = 3; i < scene_bodies(get_scene(state))-1; i++){
    body_t *body = scene_get_body(get_scene(state), i);
    vector_t centroid = body_get_centroid(body);
    if (centroid.x <= WINDOW_BOTTOM_LEFT.x){
      body_remove(body);
    }
  }
}

void set_backround_sound(void) {
  int res = Mix_OpenAudio(FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, CHUNK_SIZE);
  if (res != 0){
      printf("could not open audio\n");
  }
  Mix_Music *star_wars = Mix_LoadMUS("assets/lofi-soundtrack.wav");
  if (!star_wars){
      printf("could not load soundtrack music file\n");
  }
  int didPlay = Mix_PlayMusic(star_wars, -1);
  if (didPlay != 0){
      printf("could not play music\n");
  }
}

void update_text_points_coins(state_t *state, SDL_Color color) {
  TTF_Init();
  
  scene_t *scene = get_scene(state);
  TTF_Font* Sans = TTF_OpenFont("assets/Sans.ttf", FONT_SIZE);
  if (get_points_change(state)) {
    body_t *points_text = scene_get_body(scene, 5);
    char *str_points = malloc(sizeof(char)* 20);
    sprintf(str_points, "Points: %i", get_points(state));
    SDL_Surface* points_surface = TTF_RenderText_Solid(Sans, str_points, color); 
    body_init_image(points_text, points_surface);
    set_points_change(state, false);
  }
  if (get_coins_change(state)) {
    body_t *coins_text = scene_get_body(scene, 6);
    char *str_coins = malloc(sizeof(char)* 20);
    sprintf(str_coins, "Coins: %i", get_coins(state));
    SDL_Surface* coins_surface = TTF_RenderText_Solid(Sans, str_coins, color);
    body_init_image(coins_text, coins_surface);
    set_coins_change(state, false);
  }
  
  TTF_Quit();
  TTF_CloseFont(Sans);
}

void update_angle_for_ball(state_t *state) {
  body_t *ball = scene_get_body(get_scene(state), 4);
  body_add_to_angle(ball, ANGLE_TO_ADD_TO_BALL);
}

void show_arrow(state_t *state){
  body_t *ball = scene_get_body(get_scene(state), 4);
  body_t *arrow_up = scene_get_body(get_scene(state), 7);
  body_t *arrow_down = scene_get_body(get_scene(state), 8);
  if (body_get_centroid(ball).y >= WINDOW_TOP_RIGHT.y){
    if (body_get_velocity(ball).y > 0){
      body_set_alpha(arrow_up, FULL_COLOR);
      body_set_alpha(arrow_down, ALPHA_1);
    }
    else if (body_get_velocity(ball).y < 0){
      body_set_alpha(arrow_down, FULL_COLOR);
      body_set_alpha(arrow_up, ALPHA_1);
    }
  }
  else if (body_get_centroid(ball).y <= WINDOW_TOP_RIGHT.y){
    if (body_get_velocity(ball).y > 0){
      body_set_alpha(arrow_up, ALPHA_2);
      body_set_alpha(arrow_down, ALPHA_1);
    } 
    else if (body_get_velocity(ball).y < 0){
      body_set_alpha(arrow_down, ALPHA_2);
      body_set_alpha(arrow_up, ALPHA_1);
    }
  }
}

state_t *emscripten_init() {
  srand(time(NULL));
  sdl_init(WINDOW_BOTTOM_LEFT, WINDOW_TOP_RIGHT);
  state_t *state = state_init();
  IMG_Init(IMG_INIT_JPG);
  TTF_Init();

  start_screen_init(state);
  load_start(state);
  set_backround_sound();
  set_velocity(state, ZERO_VEC);
  set_time(state, 0);
  set_coin_time(state, 0);
  set_deriv(state, 0);
  set_coins(state, 0);
  set_above_line_bool(state, false);
  set_twice_above_line_bool(state, false);
  set_points(state, 0);
  set_points_change(state, false);
  set_coins_change(state, false);
  set_lost(state, false);
  set_won(state, false);
  set_drawn_line(state, false);

  sdl_on_key((key_handler_t)on_key);
  sdl_render_scene(get_start(state));
  
  TTF_Quit();
  return state;
}

void emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  scene_tick(get_scene(state), dt);
  TTF_Init();

  if (get_current_scene(state) != 0 && get_current_scene(state) != 6 && get_current_scene(state) != 7 && get_current_scene(state) != 8){
    // LEVEL 1
    if (get_current_scene(state) == 1){
      if (get_total_time(state) == 0){
        set_velocity(state, DUNE_VELOCITY_1);
      }
      get_derivative(state, AMP_1, X_STRETCH_1);
      moving_the_ball(state, dt);
      spawn_coins(state, COIN_SPACING);
      remove_off_screen(state);
      check_above_line(state);
      check_twice_above_line(state);
      update_text_points_coins(state, SDL_BLACK);
      update_angle_for_ball(state); 
      show_arrow(state);
      moving_the_threshold(state, AMP_1, X_STRETCH_1);
      check_win(state, get_total_time(state), LEVEL_1_DISTANCE);
    }

    // LEVEL 2
    else if (get_current_scene(state) == 2){
      if (get_total_time(state) == 0){
        set_velocity(state, DUNE_VELOCITY_2);
      }
      get_derivative(state, AMP_2, X_STRETCH_2); 
      moving_the_ball(state, dt);
      spawn_coins(state, COIN_SPACING);
      remove_off_screen(state);
      check_above_line(state);
      check_twice_above_line(state);
      update_text_points_coins(state, SDL_BLACK);
      update_angle_for_ball(state);
      show_arrow(state);
      check_win(state, get_total_time(state), LEVEL_2_DISTANCE);
      moving_the_threshold(state, AMP_2, X_STRETCH_2);
    }

    // LEVEL 3
    else if (get_current_scene(state) == 3){
      if (get_total_time(state) == 0){
        set_velocity(state, DUNE_VELOCITY_3);
      }
      get_derivative(state, AMP_3, X_STRETCH_3); 
      moving_the_ball(state, dt);
      spawn_coins(state, COIN_SPACING);
      remove_off_screen(state);
      check_above_line(state);
      check_twice_above_line(state);
      update_text_points_coins(state, SDL_BLACK);
      update_angle_for_ball(state);
      show_arrow(state);
      check_win(state, get_total_time(state), LEVEL_3_DISTANCE);
      moving_the_threshold(state, AMP_3, X_STRETCH_3);
    }

    // LEVEL 4
    else if (get_current_scene(state) == 4){
      if (get_total_time(state) == 0){
        set_velocity(state, DUNE_VELOCITY_4);
      }
      get_derivative(state, AMP_4, X_STRETCH_4); 
      moving_the_ball(state, dt);
      spawn_coins(state, COIN_SPACING);
      remove_off_screen(state);
      check_above_line(state);
      check_twice_above_line(state);
      update_text_points_coins(state, SDL_BLACK);
      update_angle_for_ball(state);
      show_arrow(state);
      check_win(state, get_total_time(state), LEVEL_4_DISTANCE);
      moving_the_threshold(state, AMP_4, X_STRETCH_4);
    }

    // LEVEL 5
    else if (get_current_scene(state) == 5){
      if (get_total_time(state) == 0){
        set_velocity(state, DUNE_VELOCITY_5);
      }
      get_derivative(state, AMP_5, X_STRETCH_5); 
      moving_the_ball(state, dt);
      spawn_coins(state, COIN_SPACING);
      remove_off_screen(state);
      check_above_line(state);
      check_twice_above_line(state);
      update_text_points_coins(state, SDL_WHITE);
      update_angle_for_ball(state);
      show_arrow(state);
      check_win(state, get_total_time(state), LEVEL_5_DISTANCE);
      moving_the_threshold(state, AMP_5, X_STRETCH_5);
    }

    set_time(state, get_total_time(state) + dt);
    set_coin_time(state, get_coin_time(state) + dt);

  } 
  sdl_render_scene(get_scene(state));
}

void emscripten_free(state_t *state) {
  IMG_Quit();
  Mix_CloseAudio();
  scene_free(get_scene(state));
  free(state);
}

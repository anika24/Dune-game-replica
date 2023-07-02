#include "math.h"
#include "scene.h"
#include "vector.h"
#include "sdl_wrapper.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Stores the demo state
 * Use this to store any variable needed every 'tick' of your demo
 */
typedef struct state state_t;

double get_total_time(state_t *state);

state_t *state_init();

scene_t *get_scene(state_t *state);

void load_game(state_t *state);

void load_start_menu(state_t *state);

void load_pause_menu(state_t *state);

void set_time(state_t *state, double time);

double get_coin_time(state_t *state);

double get_deriv(state_t *state);

void set_deriv(state_t *state, double deriv);

void set_coin_time(state_t *state, double time);

int get_coins(state_t *state);

void set_coins(state_t *state, int coins);

vector_t get_velocity(state_t *state);

void set_velocity(state_t *state, vector_t velocity);

bool get_first_hit(state_t *state);

void set_first_hit(state_t *state, bool boolean);

void set_above_line_bool(state_t *state, bool boolean);

bool get_above_line_bool(state_t *state);

int get_points(state_t *state);

void set_points(state_t *state, int points);

void set_points_change(state_t *state, bool boolean);

void set_coins_change(state_t *state, bool boolean);

bool get_points_change(state_t *state);

bool get_coins_change(state_t *state);

void set_pause(state_t *state, scene_t *scene);

scene_t *get_pause(state_t *state);

int get_current_scene(state_t *state);

void set_twice_above_line_bool(state_t *state, bool boolean);

bool get_twice_above_line_bool(state_t *state);

scene_t *get_level1(state_t *state);

scene_t *get_level2(state_t *state);

scene_t *get_level3(state_t *state);

scene_t *get_level4(state_t *state);

scene_t *get_level5(state_t *state);

scene_t *get_start(state_t *state);

void set_level1(state_t *state, scene_t *scene);

void set_level2(state_t *state, scene_t *scene);

void set_level3(state_t *state, scene_t *scene);

void set_level4(state_t *state, scene_t *scene);

void set_level5(state_t *state, scene_t *scene);

void set_start(state_t *state, scene_t *scene);

void load_5(state_t *state);

void load_4(state_t *state);

void load_3(state_t *state);

void load_2(state_t *state);

void load_1(state_t *state);

void load_start(state_t *state);

void set_end_menu_level1_win(state_t *state, scene_t *scene);

scene_t *get_end_menu_level1_win(state_t *state);

void set_end_menu_level2_win(state_t *state, scene_t *scene);

scene_t *get_end_menu_level2_win(state_t *state);

void set_end_menu_level3_win(state_t *state, scene_t *scene);

scene_t *get_end_menu_level3_win(state_t *state);

void set_end_menu_level4_win(state_t *state, scene_t *scene);

scene_t *get_end_menu_level4_win(state_t *state);

void set_end_menu_level5_win(state_t *state, scene_t *scene);

scene_t *get_end_menu_level5_win(state_t *state);

void set_end_menu_level1_lose(state_t *state, scene_t *scene);

scene_t *get_end_menu_level1_lose(state_t *state);

void set_end_menu_level2_lose(state_t *state, scene_t *scene);

scene_t *get_end_menu_level2_lose(state_t *state);

void set_end_menu_level3_lose(state_t *state, scene_t *scene);

scene_t *get_end_menu_level3_lose(state_t *state);

void set_end_menu_level4_lose(state_t *state, scene_t *scene);

scene_t *get_end_menu_level4_lose(state_t *state);

void set_end_menu_level5_lose(state_t *state, scene_t *scene);

scene_t *get_end_menu_level5_lose(state_t *state);

scene_t *get_current_end_menu_win(state_t *state);

scene_t *get_current_end_menu_lose(state_t *state);

void load_end_menu(state_t *state, scene_t *scene);

void set_before_pause(state_t *state, int num);

void load_after_pause(state_t *state);

scene_t *get_instructions(state_t *state);

void set_instructions(state_t *state, scene_t *scene);

void load_instructions(state_t *state);

void set_current_scene(state_t *state, int scene);

bool get_lost(state_t *state);

void set_lost(state_t *state, bool lost);

bool get_won(state_t *state);

void set_won(state_t *state, bool won);

int get_level_before_end(state_t *state);

void set_level_before_end(state_t *state, int level);

bool get_drawn_line(state_t *state);

void set_drawn_line(state_t *state, bool line);
/**
 * Initializes sdl as well as the variables needed
 * Creates and stores all necessary variables for the demo in a created state
 * variable Returns the pointer to this state (This is the state emscripten_main
 * and emscripten_free work with)
 */
state_t *emscripten_init();

/**
 * Called on each tick of the program
 * Updates the state variables and display as necessary, depending on the time
 * that has passed
 */
void emscripten_main(state_t *state);

/**
 * Frees anything allocated in the demo
 * Should free everything in state as well as state itself.
 */
void emscripten_free(state_t *state);

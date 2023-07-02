#include "state.h"

typedef struct state {
  scene_t *scene;
  scene_t *start;
  scene_t *instructions;
  scene_t *level_1;
  scene_t *level_2;
  scene_t *level_3;
  scene_t *level_4;
  scene_t *level_5;
  scene_t *end_level_1_win;
  scene_t *end_level_2_win;
  scene_t *end_level_3_win;
  scene_t *end_level_4_win;
  scene_t *end_level_5_win;
  scene_t *end_level_1_lose;
  scene_t *end_level_2_lose;
  scene_t *end_level_3_lose;
  scene_t *end_level_4_lose;
  scene_t *end_level_5_lose;
  scene_t *pause;
  double total_time;
  int slope_points;
  double coin_time;
  double threshold;
  double deriv;
  vector_t velocity;
  bool first_hit;
  bool above_line_bool;
  bool lost;
  bool won;
  int coins;
  int points;
  bool points_change;
  bool coins_change;
  int curr_scene;
  int before_pause;
  bool above_twice_line_bool;
  int level_before_end;
  bool drawn_line;
} state_t;

state_t *state_init(){
  state_t *state = malloc(sizeof(state_t));
  state->first_hit = true;
  return state;
}

bool get_drawn_line(state_t *state){
  return state->drawn_line;
}

void set_drawn_line(state_t *state, bool line){
  state->drawn_line = line;
}

int get_level_before_end(state_t *state){
  return state->level_before_end;
}

void set_level_before_end(state_t *state, int level){
  state->level_before_end = level;
}

bool get_lost(state_t *state){
  return state->lost;
}

void set_lost(state_t *state, bool lost){
  state->lost = lost;
}

bool get_won(state_t *state){
  return state->won;
}

void set_won(state_t *state, bool won){
  state->won = won;
}

scene_t *get_instructions(state_t *state){
  return state->instructions;
}

void set_instructions(state_t *state, scene_t *scene){
  state->instructions = scene;
}

void load_instructions(state_t *state){
  state->scene = state->instructions;
  state->curr_scene = 7;
}

scene_t *get_start(state_t *state){
  return state->start;
}

scene_t *get_level1(state_t *state){
  return state->level_1;
}

scene_t *get_level2(state_t *state){
  return state->level_2;
}

scene_t *get_level3(state_t *state){
  return state->level_3;
}

scene_t *get_level4(state_t *state){
  return state->level_4;
}

scene_t *get_level5(state_t *state){
  return state->level_5;
}

void set_start(state_t *state, scene_t *scene){
  state->start = scene;
}

void set_level1(state_t *state, scene_t *scene){
  state->level_1 = scene;
}

void set_level2(state_t *state, scene_t *scene){
  state->level_2 = scene;
}

void set_level3(state_t *state, scene_t *scene){
  state->level_3 = scene;
}

void set_level4(state_t *state, scene_t *scene){
  state->level_4 = scene;
}

void set_level5(state_t *state, scene_t *scene){
  state->level_5 = scene;
}

void load_start(state_t *state){
  state->scene = state->start;
  state->curr_scene = 0;
}

void load_1(state_t *state){
  state->scene = state->level_1;
  state->curr_scene = 1;
}

void load_2(state_t *state){
  state->scene = state->level_2;
  state->curr_scene = 2;
}

void load_3(state_t *state){
  state->scene = state->level_3;
  state->curr_scene = 3;
}

void load_4(state_t *state){
  state->scene = state->level_4;
  state->curr_scene = 4;
}

void load_5(state_t *state){
  state->scene = state->level_5;
  state->curr_scene = 5;
}

double get_total_time(state_t *state){
  return state->total_time;
}

void set_deriv(state_t *state, double deriv){
  state->deriv = deriv;
}

double get_deriv(state_t *state){
  return state->deriv;
}

int get_coins(state_t *state){
  return state->coins;
}

void set_coins(state_t *state, int coins){
  state->coins = coins;
}

scene_t *get_scene(state_t *state){
  return state->scene;
}

void set_scene(state_t *state, scene_t *scene){
  state->scene = scene;
}

void set_time(state_t *state, double time){
  state->total_time = time;
}

double get_coin_time(state_t *state){
  return state->coin_time;
}

void set_coin_time(state_t *state, double time){
  state->coin_time = time;
}

vector_t get_velocity(state_t *state){
  return state->velocity;
}

void set_velocity(state_t *state, vector_t velocity){
  state->velocity = velocity;
}

bool get_first_hit(state_t *state){
  return state->first_hit;
}

void set_first_hit(state_t *state, bool boolean){
  state->first_hit = boolean;
}

void set_above_line_bool(state_t *state, bool boolean) {
  state->above_line_bool = boolean;
}

bool get_above_line_bool(state_t *state) {
  return state->above_line_bool;
}

int get_points(state_t *state){
  return state->points;
}

void set_points(state_t *state, int points){
  state->points = points;
  state->points_change = true;
}

void set_points_change(state_t *state, bool boolean){
  state->points_change = boolean;
}

void set_coins_change(state_t *state, bool boolean){
  state->coins_change = boolean;
}

bool get_points_change(state_t *state) {
  return state->points_change;
}

bool get_coins_change(state_t *state) {
  return state->coins_change;
}

void set_pause(state_t *state, scene_t *scene){
  state->pause = scene;
}

scene_t *get_pause(state_t *state){
  return state->pause;
}

void load_pause_menu(state_t *state){
  state->scene = state->pause;
  state->curr_scene = 6;
}

int get_current_scene(state_t *state){
  return state->curr_scene;
}

void set_twice_above_line_bool(state_t *state, bool boolean) {
  state->above_twice_line_bool = boolean;
}

bool get_twice_above_line_bool(state_t *state) {
  return state->above_twice_line_bool;
}

void set_end_menu_level1_win(state_t *state, scene_t *scene){
  state->end_level_1_win = scene;
}

scene_t *get_end_menu_level1_win(state_t *state){
  return state->end_level_1_win;
}

void set_end_menu_level2_win(state_t *state, scene_t *scene){
  state->end_level_2_win = scene;
}

scene_t *get_end_menu_level2_win(state_t *state){
  return state->end_level_2_win;
}

void set_end_menu_level3_win(state_t *state, scene_t *scene){
  state->end_level_3_win = scene;
}

scene_t *get_end_menu_level3_win(state_t *state){
  return state->end_level_3_win;
}

void set_end_menu_level4_win(state_t *state, scene_t *scene){
  state->end_level_4_win = scene;
}

scene_t *get_end_menu_level4_win(state_t *state){
  return state->end_level_4_win;
}

void set_end_menu_level5_win(state_t *state, scene_t *scene){
  state->end_level_5_win = scene;
}

scene_t *get_end_menu_level5_win(state_t *state){
  return state->end_level_5_win;
}

void set_end_menu_level1_lose(state_t *state, scene_t *scene){
  state->end_level_1_lose = scene;
}

scene_t *get_end_menu_level1_lose(state_t *state){
  return state->end_level_1_lose;
}

void set_end_menu_level2_lose(state_t *state, scene_t *scene){
  state->end_level_2_lose = scene;
}

scene_t *get_end_menu_level2_lose(state_t *state){
  return state->end_level_2_lose;
}

void set_end_menu_level3_lose(state_t *state, scene_t *scene){
  state->end_level_3_lose = scene;
}

scene_t *get_end_menu_level3_lose(state_t *state){
  return state->end_level_3_lose;
}

void set_end_menu_level4_lose(state_t *state, scene_t *scene){
  state->end_level_4_lose = scene;
}

scene_t *get_end_menu_level4_lose(state_t *state){
  return state->end_level_4_lose;
}

void set_end_menu_level5_lose(state_t *state, scene_t *scene){
  state->end_level_5_lose = scene;
}

scene_t *get_end_menu_level5_lose(state_t *state){
  return state->end_level_5_lose;
}

scene_t *get_current_end_menu_win(state_t *state) {
  if (state->curr_scene == 1) {
    return state->end_level_1_win;
  } else if (state->curr_scene == 2) {
    return state->end_level_2_win;
  } else if (state->curr_scene == 3) {
    return state->end_level_3_win;
  } else if (state->curr_scene == 4) {
    return state->end_level_4_win;
  } else {
    return state->end_level_5_win;
  }
}

scene_t *get_current_end_menu_lose(state_t *state) {
  if (state->curr_scene == 1) {
    return state->end_level_1_lose;
  } else if (state->curr_scene == 2) {
    return state->end_level_2_lose;
  } else if (state->curr_scene == 3) {
    return state->end_level_3_lose;
  } else if (state->curr_scene == 4) {
    return state->end_level_4_lose;
  } else {
    return state->end_level_5_lose;
  }
}

void load_end_menu(state_t *state, scene_t *scene) {
  state->scene = scene;
  state->curr_scene = 8;
}

void set_before_pause(state_t *state, int num){
  state->before_pause = num;
}

void load_after_pause(state_t *state){
  if (state->before_pause == 1){
    load_1(state);
  }
  else if (state->before_pause == 2){
    load_2(state);
  }
  else if (state->before_pause == 3){
    load_3(state);
  }
  else if (state->before_pause == 4){
    load_4(state);
  }
  else if (state->before_pause == 5){
    load_5(state);
  }
}

void set_current_scene(state_t *state, int scene){
  state->curr_scene = scene;
}



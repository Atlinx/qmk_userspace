#include "snake.h"

#include <stdlib.h>

#include "debug.h"
#include "quantum.h"
#include "rgb_matrix.h"
#include "timer.h"

typedef struct {
  uint8_t x;
  uint8_t y;
} snake_point_t;

static snake_point_t snake_body[SNAKE_MAX_LENGTH];
static snake_point_t food;

static uint8_t snake_length;
static int8_t dir_x;
static int8_t dir_y;
static int8_t pending_x;
static int8_t pending_y;
static bool snake_running;
static bool snake_fast;
static uint16_t last_tick;

static const RGB c_snake_head = C_SNAKE_HEAD;
static const RGB c_snake_body = C_SNAKE_BODY;
static const RGB c_snake_food = C_SNAKE_FOOD;

static bool snake_point_on_body(uint8_t x, uint8_t y) {
  for (uint8_t i = 0; i < snake_length; i++) {
    if (snake_body[i].x == x && snake_body[i].y == y) {
      return true;
    }
  }
  return false;
}

static void snake_place_food(void) {
  // Try a bounded number of random cells so a full board can't hang us.
  for (uint8_t tries = 0; tries < 100; tries++) {
    food.x = rand() % PHYSICAL_COLS;
    food.y = rand() % PHYSICAL_ROWS;
    if (!snake_point_on_body(food.x, food.y)) {
      return;
    }
  }
}

static void snake_reset(void) {
  srand(timer_read32());
  snake_length = SNAKE_START_LENGTH;

  uint8_t start_x = PHYSICAL_COLS / 2;
  uint8_t start_y = PHYSICAL_ROWS / 2;
  for (uint8_t i = 0; i < snake_length; i++) {
    snake_body[i].x = start_x - i;
    snake_body[i].y = start_y;
  }

  dir_x = 1;
  dir_y = 0;
  pending_x = 1;
  pending_y = 0;

  snake_fast = false;

  snake_place_food();
  last_tick = timer_read();
}

static void snake_tick(void) {
  // Apply the queued direction, but never reverse into ourselves.
  if (pending_x != -dir_x || pending_y != -dir_y) {
    dir_x = pending_x;
    dir_y = pending_y;
  }

  int16_t new_x = (int16_t)snake_body[0].x + dir_x;
  int16_t new_y = (int16_t)snake_body[0].y + dir_y;

  // Loop around the edges (toroidal board): wrap into the opposite side
  // instead of dying on a wall.
  new_x = (new_x + PHYSICAL_COLS) % PHYSICAL_COLS;
  new_y = (new_y + PHYSICAL_ROWS) % PHYSICAL_ROWS;

  bool eating = (new_x == (int8_t)food.x && new_y == (int8_t)food.y);

  // Self collision. The tail moves away unless we just ate.
  for (uint8_t i = 0; i < snake_length; i++) {
    if (snake_body[i].x == new_x && snake_body[i].y == new_y) {
      if (i == snake_length - 1 && !eating) {
        continue;
      }
      snake_reset();
      return;
    }
  }

  // Shift the body forward and place the new head.
  for (uint8_t i = snake_length; i > 0; i--) {
    snake_body[i] = snake_body[i - 1];
  }
  snake_body[0].x = new_x;
  snake_body[0].y = new_y;

  if (eating) {
    if (snake_length < SNAKE_MAX_LENGTH) {
      snake_length++;
    }
    snake_place_food();
  }
}

static void snake_draw(void) {
  rgb_matrix_set_color_all(0, 0, 0);

  // Body, drawn tail-first so the head stays on top.
  for (uint8_t i = snake_length; i > 0; i--) {
    rgb_matrix_set_color(xy_to_led(snake_body[i - 1].x, snake_body[i - 1].y),
                         c_snake_body.r, c_snake_body.g, c_snake_body.b);
  }

  rgb_matrix_set_color(xy_to_led(snake_body[0].x, snake_body[0].y),
                       c_snake_head.r, c_snake_head.g, c_snake_head.b);
  rgb_matrix_set_color(xy_to_led(food.x, food.y), c_snake_food.r,
                       c_snake_food.g, c_snake_food.b);
}

// --- Hooks ---

void keyboard_post_init_snake(void) {
  srand(timer_read32());
  snake_running = false;
}

bool process_record_snake(uint16_t keycode, keyrecord_t* record) {
  // SNK_SPD is a hold key: track its state on both press and release.
  switch (keycode) {
    case SNK_FAST:
      snake_fast = record->event.pressed;
      return false;
    default:
      break;
  }

  if (!record->event.pressed) {
    return true;
  }

  switch (keycode) {
    case SNK_TOGG:
      snake_toggle();
      return false;
    case SNK_START:
      snake_start();
      return false;
    case SNK_UP:
      pending_x = 0;
      pending_y = -1;
      return false;
    case SNK_DOWN:
      pending_x = 0;
      pending_y = 1;
      return false;
    case SNK_LEFT:
      pending_x = -1;
      pending_y = 0;
      return false;
    case SNK_RIGHT:
      pending_x = 1;
      pending_y = 0;
      return false;
  }

  return true;
}

void matrix_scan_snake(void) {
  if (!snake_running) {
    return;
  }
  // 2x speed while SNK_SPD is held: halve the effective tick time.
  uint16_t tick_time = snake_fast ? SNAKE_TICK_TIME / 2 : SNAKE_TICK_TIME;
  if (timer_elapsed(last_tick) >= tick_time) {
    last_tick = timer_read();
    snake_tick();
  }
}

bool rgb_matrix_indicators_snake(void) {
  if (!snake_running) {
    return false;
  }
  snake_draw();
  return true;
}

// --- Public helpers ---

void snake_toggle(void) {
  if (snake_running) {
    snake_stop();
  } else {
    snake_start();
  }
}

void snake_start(void) {
  snake_running = true;
  snake_reset();
}

void snake_stop(void) { snake_running = false; }

bool snake_is_enabled(void) { return snake_running; }

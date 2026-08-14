#pragma once
#include "atlinx.h"
#include "quantum.h"

// ------------------------------------------------------------------
// Keyboard-agnostic Snake game rendered on the RGB matrix.
//
// This module exposes `_snake` hooks that a keyboard/userspace calls
// from the corresponding `_user` functions:
//
//   keyboard_post_init_snake()   -> from keyboard_post_init_user()
//   process_record_snake()       -> from process_record_user()
//   matrix_scan_snake()          -> from matrix_scan_user()
//   rgb_matrix_indicators_snake()-> from rgb_matrix_indicators_user()
//
// Rendering assumes a row-major ortholinear LED layout, i.e. LED index
// == (row * PHYSICAL_COLS) + col. Override SNAKE_XY_TO_LED if a board
// uses a different LED ordering.
// ------------------------------------------------------------------

// --- Overridable configuration ---
#ifndef SNAKE_START_LENGTH
#define SNAKE_START_LENGTH 3
#endif
#ifndef SNAKE_TICK_TIME
#define SNAKE_TICK_TIME 300
#endif
#ifndef SNAKE_MAX_LENGTH
#define SNAKE_MAX_LENGTH (PHYSICAL_ROWS * PHYSICAL_COLS)
#endif

#ifndef C_SNAKE_HEAD
#define C_SNAKE_HEAD C_PINK
#endif
#ifndef C_SNAKE_BODY
#define C_SNAKE_BODY C_PINK
#endif
#ifndef C_SNAKE_FOOD
#define C_SNAKE_FOOD C_WHITE
#endif

// --- Custom keycodes ---
enum SnakeKeycode {
  SNK_TOGG = SAFE_RANGE,  // Start / stop the game
  SNK_START,              // (Re)start the game
  SNK_UP,
  SNK_DOWN,
  SNK_LEFT,
  SNK_RIGHT,
  SNK_FAST,  // 2x speed while held
  SNK_END,   // Sentinel: other custom ranges may begin here
};

#undef SAFE_RANGE
#define SAFE_RANGE SNK_END

// --- Hooks ---
void keyboard_post_init_snake(void);
bool process_record_snake(uint16_t keycode, keyrecord_t* record);
void matrix_scan_snake(void);
bool rgb_matrix_indicators_snake(void);

// --- Public helpers ---
void snake_toggle(void);
void snake_start(void);
void snake_stop(void);
bool snake_is_enabled(void);

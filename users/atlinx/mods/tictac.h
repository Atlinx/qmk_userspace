#pragma once
#include "atlinx.h"
#include "quantum.h"

// ------------------------------------------------------------------
// Keyboard-agnostic Tic-Tac-Toe game rendered on the RGB matrix.
//
// This module exposes `_tictac` hooks that a keyboard/userspace calls
// from the corresponding `_user` functions:
//
//   keyboard_post_init_tictac()   -> from keyboard_post_init_user()
//   process_record_tictac()       -> from process_record_user()
//   rgb_matrix_indicators_tictac()-> from rgb_matrix_indicators_user()
//
// Rendering assumes a row-major ortholinear LED layout, i.e. LED index
// == (row * PHYSICAL_COLS) + col. Override TICTAC_XY_TO_LED if a board
// uses a different LED ordering.
//
// Unlike snake, this game defines NO custom keycodes. Each of the 3x3
// board cells is bound to an existing key (TICTAC_CELL_KEYS, default
// KC_1..KC_9), so a player "clicks" a board cell by pressing its key on
// the game layer. These keys are only intercepted while the game is
// running, so normal key usage is unaffected.
// ------------------------------------------------------------------

// --- Overridable configuration ---
// Board geometry. Cell (row, col) is rendered at LED position
// (TICTAC_BOARD_X + col * TICTAC_CELL_SPACING_X,
//  TICTAC_BOARD_Y + row * TICTAC_CELL_SPACING_Y).
#ifndef TICTAC_BOARD_X
#define TICTAC_BOARD_X 4  // column of the top-left board cell
#endif
#ifndef TICTAC_BOARD_Y
#define TICTAC_BOARD_Y 0  // row of the top-left board cell
#endif
#ifndef TICTAC_CELL_SPACING_X
#define TICTAC_CELL_SPACING_X 1  // horizontal distance between cells
#endif
#ifndef TICTAC_CELL_SPACING_Y
#define TICTAC_CELL_SPACING_Y 1  // vertical distance between cells
#endif

// Colors. X (red) and O (blue) players; empty cells use the empty color
// (dark white by default so the board stays visible).
#ifndef C_TICTAC_X
#define C_TICTAC_X C_RED
#endif
#ifndef C_TICTAC_O
#define C_TICTAC_O C_BLUE
#endif
#ifndef C_TICTAC_EMPTY
#define C_TICTAC_EMPTY C_WHITE_DARK
#endif

// --- Score squares ---
// Maximum wins tracked per player. The score square arrays below must
// contain at least this many (x, y) tuples.
#ifndef TICTAC_MAX_SCORE
#define TICTAC_MAX_SCORE 4
#endif
// Positions of the O-player's score squares (left column) as an array
// of 2D (x, y) tuples. The first N entries light up to show the score.
#ifndef TICTAC_SCORE_O_LEDS
#define TICTAC_SCORE_O_LEDS {{3, 0}, {3, 1}, {3, 2}, {3, 3}}
#endif
// Positions of the X-player's score squares (right column).
#ifndef TICTAC_SCORE_X_LEDS
#define TICTAC_SCORE_X_LEDS {{7, 0}, {7, 1}, {7, 2}, {7, 3}}
#endif

// --- Turn indicator ---
// LED index (the planck 2u spacebar is index 41) lit in the current
// player's color to show whose turn it is.
#ifndef TICTAC_TURN_LED
#define TICTAC_TURN_LED 41
#endif

// --- Board cell key ---
// All board cells use the same keycode (KC_1); the game resolves which
// cell was clicked from the key's physical position. Only intercepted
// while the game is running.
#ifndef TICTAC_CELL_KEY
#define TICTAC_CELL_KEY KC_1
#endif

// --- Hooks ---
void keyboard_post_init_tictac(void);
bool process_record_tictac(uint16_t keycode, keyrecord_t* record);
bool rgb_matrix_indicators_tictac(void);

// --- Public helpers ---
void tictac_start(void);
void tictac_stop(void);
bool tictac_is_enabled(void);

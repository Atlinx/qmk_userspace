#pragma once
#include "atlinx.h"
#include "quantum.h"

// ------------------------------------------------------------------
// Keyboard-agnostic Battleship game rendered on the RGB matrix.
//
// This module exposes `_battleship` hooks that a keyboard/userspace
// calls from the corresponding `_user` functions:
//
//   keyboard_post_init_battleship()  -> from keyboard_post_init_user()
//   process_record_battleship()      -> from process_record_user()
//   rgb_matrix_indicators_battleship()-> from rgb_matrix_indicators_user()
//
// Rendering assumes a row-major ortholinear LED layout, i.e. LED index
// == (row * PHYSICAL_COLS) + col (the shared `mat_xy` helper).
//
// The screen is split: P1's board on the left half, P2's board on the
// right half. The game uses NO custom keycodes - it is driven by
// existing keys (arrows move the cursor, a rotate key spins the current
// ship, a place key confirms) which are only intercepted while running.
//
// Game flow:
//   1. PLACE phase - players place their ships one at a time. P1 places
//      on the left board, then P2 places on the right board. The
//      opponent's half is drawn dark white. Arrows move a cursor, the
//      rotate key turns the current ship around its (0,0) center, and
//      the place key finalizes it (clamped so it never leaves the board).
//   2. BATTLE phase - players alternate clicking (placing) cells on the
//      opponent's board. A hit turns the cell green, a miss turns it
//      red. Ships are hidden until the round ends.
//   3. REVEAL phase - after a player's ships are all destroyed the round
//      ends, the winner's score increases, and all ships are shown until
//      a key is pressed. If a player reaches the score limit the scores
//      reset to 0 and a new match begins.
// ------------------------------------------------------------------

// --- Board geometry ---
// Both boards share the same size. Cell (r, c) of player p is rendered
// at (BSHIP_Px_X + c, BSHIP_Px_Y + r).
#ifndef BSHIP_COLS
#define BSHIP_COLS 5  // columns per board
#endif
#ifndef BSHIP_ROWS
#define BSHIP_ROWS 4  // rows per board
#endif
#ifndef BSHIP_P1_X
#define BSHIP_P1_X 0  // top-left column of P1's board (left)
#endif
#ifndef BSHIP_P1_Y
#define BSHIP_P1_Y 0  // top-left row of P1's board
#endif
#ifndef BSHIP_P2_X
#define BSHIP_P2_X 7  // top-left column of P2's board (right)
#endif
#ifndef BSHIP_P2_Y
#define BSHIP_P2_Y 0  // top-left row of P2's board
#endif

// --- Colors ---
#ifndef BSHIP_COLOR_P1
#define BSHIP_COLOR_P1 C_BLUE
#endif
#ifndef BSHIP_COLOR_P2
#define BSHIP_COLOR_P2 C_MAGENTA
#endif
#ifndef BSHIP_COLOR_CURSOR
#define BSHIP_COLOR_CURSOR C_WHITE
#endif
#ifndef BSHIP_COLOR_HIT
#define BSHIP_COLOR_HIT C_GREEN
#endif
#ifndef BSHIP_COLOR_MISS
#define BSHIP_COLOR_MISS C_RED
#endif
#ifndef BSHIP_COLOR_EMPTY
#define BSHIP_COLOR_EMPTY C_WHITE_DARK
#endif

// --- Score ---
// Score limit (first to this many wins takes the match).
#ifndef BSHIP_MAX_SCORE
#define BSHIP_MAX_SCORE 3
#endif
// Score square positions as arrays of 2D (x, y) tuples. The first N
// entries light up to show the current score.
#ifndef BSHIP_SCORE_P1_LEDS
#define BSHIP_SCORE_P1_LEDS {{5, 0}, {5, 1}, {5, 2}}
#endif
#ifndef BSHIP_SCORE_P2_LEDS
#define BSHIP_SCORE_P2_LEDS {{6, 0}, {6, 1}, {6, 2}}
#endif

// --- Turn indicator ---
// LED index (the planck 2u spacebar is index 41) lit in the current
// player's color to show whose turn it is.
#ifndef BSHIP_TURN_LED
#define BSHIP_TURN_LED 41
#endif

// --- Cell key ---
// All board cells use the same keycode (KC_1); the game resolves which
// cell/board was clicked from the key's physical position.
#ifndef BSHIP_CELL_KEY
#define BSHIP_CELL_KEY KC_1
#endif

// --- Ships ---
// Sentinel marking the end of a ship's offset list.
#ifndef BSHIP_SHIP_END
#define BSHIP_SHIP_END {-128, -128}
#endif
#ifndef BSHIP_MAX_SHIP_LEN
#define BSHIP_MAX_SHIP_LEN 8
#endif
// The ships available to each player, stored as an array. Each ship is
// an array of 2D (dx, dy) tuples giving the ship's cells relative to its
// center (0, 0) - the point rotation happens around. Lists are
// terminated by BSHIP_SHIP_END.
#ifndef BSHIP_SHIPS
#define BSHIP_SHIPS                                             \
  {                                                             \
    {{0, 0}, BSHIP_SHIP_END}, {{0, 0}, {1, 0}, BSHIP_SHIP_END}, \
        {{0, 0}, {1, 0}, BSHIP_SHIP_END}, {                     \
      {0, 0}, {1, 0}, {-1, 0}, BSHIP_SHIP_END                   \
    }                                                           \
  }
#endif

// --- Control keys (existing keycodes; no custom keys) ---
#ifndef BSHIP_KEY_UP
#define BSHIP_KEY_UP KC_UP
#endif
#ifndef BSHIP_KEY_DOWN
#define BSHIP_KEY_DOWN KC_DOWN
#endif
#ifndef BSHIP_KEY_LEFT
#define BSHIP_KEY_LEFT KC_LEFT
#endif
#ifndef BSHIP_KEY_RIGHT
#define BSHIP_KEY_RIGHT KC_RIGHT
#endif
#ifndef BSHIP_KEY_ROTATE
#define BSHIP_KEY_ROTATE KC_R
#endif
#ifndef BSHIP_KEY_PLACE
#define BSHIP_KEY_PLACE KC_ENT
#endif

// --- Hooks ---
void keyboard_post_init_battleship(void);
bool process_record_battleship(uint16_t keycode, keyrecord_t* record);
bool rgb_matrix_indicators_battleship(void);

// --- Public helpers ---
void battleship_start(void);
void battleship_stop(void);
bool battleship_is_enabled(void);

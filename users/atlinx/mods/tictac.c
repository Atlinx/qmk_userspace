#include "tictac.h"

#include "quantum.h"
#include "rgb_matrix.h"

typedef struct {
  uint8_t x;
  uint8_t y;
} tictac_point_t;

typedef enum {
  TICTAC_CELL_EMPTY,
  TICTAC_CELL_X,
  TICTAC_CELL_O,
} tictac_cell_t;

// 3x3 game board, indexed [y][x].
static tictac_cell_t tictac_board[3][3];

// Score square positions, configured as arrays of (x, y) tuples.
static const tictac_point_t tictac_score_o[TICTAC_MAX_SCORE] =
    TICTAC_SCORE_O_LEDS;
static const tictac_point_t tictac_score_x[TICTAC_MAX_SCORE] =
    TICTAC_SCORE_X_LEDS;

static uint8_t tictac_score_o_count;
static uint8_t tictac_score_x_count;

static uint8_t current_player;  // 0 = X (red), 1 = O (blue)
static uint8_t first_player;    // who starts each round
static bool game_over;
static bool tictac_running;

// Resolve a physical matrix position to a board cell (bx, by), or return
// false if it isn't one of the 3x3 board cells.
static bool tictac_cell_from_pos(uint8_t x, uint8_t y, uint8_t* bx,
                                 uint8_t* by) {
  uint8_t px, py;
  if (!mat_log_to_phys(x, y, &px, &py)) {
    return false;
  }
  if (py < TICTAC_BOARD_Y || px < TICTAC_BOARD_X) {
    return false;
  }
  uint8_t dy = py - TICTAC_BOARD_Y;
  uint8_t dx = px - TICTAC_BOARD_X;
  if (dy >= 3 || dx >= 3) {
    return false;
  }
  if (dy % TICTAC_CELL_SPACING_Y != 0 || dx % TICTAC_CELL_SPACING_X != 0) {
    return false;
  }
  *by = dy / TICTAC_CELL_SPACING_Y;
  *bx = dx / TICTAC_CELL_SPACING_X;
  return true;
}

static const RGB c_x = C_TICTAC_X;
static const RGB c_o = C_TICTAC_O;
static const RGB c_empty = C_TICTAC_EMPTY;

static bool tictac_has_win(tictac_cell_t player) {
  for (uint8_t i = 0; i < 3; i++) {
    if (tictac_board[i][0] == player && tictac_board[i][1] == player &&
        tictac_board[i][2] == player) {
      return true;
    }
    if (tictac_board[0][i] == player && tictac_board[1][i] == player &&
        tictac_board[2][i] == player) {
      return true;
    }
  }
  if (tictac_board[0][0] == player && tictac_board[1][1] == player &&
      tictac_board[2][2] == player) {
    return true;
  }
  if (tictac_board[0][2] == player && tictac_board[1][1] == player &&
      tictac_board[2][0] == player) {
    return true;
  }
  return false;
}

static bool tictac_board_full(void) {
  for (uint8_t y = 0; y < 3; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      if (tictac_board[y][x] == TICTAC_CELL_EMPTY) {
        return false;
      }
    }
  }
  return true;
}

static void tictac_reset_round(void) {
  for (uint8_t y = 0; y < 3; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      tictac_board[y][x] = TICTAC_CELL_EMPTY;
    }
  }
  game_over = false;
  // Alternate who starts each round.
  first_player = 1 - first_player;
  current_player = first_player;
}

// Full reset: board + scores.
static void tictac_reset(void) {
  tictac_score_o_count = 0;
  tictac_score_x_count = 0;
  first_player = 1;  // toggled to 0 (X first) by tictac_reset_round()
  tictac_reset_round();
}

static void tictac_place(uint8_t x, uint8_t y) {
  // When a round has finished, clicking any cell starts a new one
  // (scores are kept).
  if (game_over) {
    tictac_reset_round();
    return;
  }
  if (tictac_board[y][x] != TICTAC_CELL_EMPTY) {
    return;
  }

  tictac_cell_t player = (current_player == 0) ? TICTAC_CELL_X : TICTAC_CELL_O;
  tictac_board[y][x] = player;

  if (tictac_has_win(player)) {
    if (current_player == 0) {
      tictac_score_x_count++;
      if (tictac_score_x_count >= TICTAC_MAX_SCORE) {
        tictac_score_o_count = 0;
        tictac_score_x_count = 0;
      }
    } else {
      tictac_score_o_count++;
      if (tictac_score_o_count >= TICTAC_MAX_SCORE) {
        tictac_score_o_count = 0;
        tictac_score_x_count = 0;
      }
    }
    game_over = true;
    return;
  }
  if (tictac_board_full()) {
    game_over = true;  // draw
    return;
  }
  current_player = 1 - current_player;
}

static void tictac_draw(void) {
  rgb_matrix_set_color_all(0, 0, 0);

  // Board cells.
  for (uint8_t y = 0; y < 3; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      uint8_t lx = TICTAC_BOARD_X + x * TICTAC_CELL_SPACING_X;
      uint8_t ly = TICTAC_BOARD_Y + y * TICTAC_CELL_SPACING_Y;
      RGB col = c_empty;
      if (tictac_board[y][x] == TICTAC_CELL_X) {
        col = c_x;
      } else if (tictac_board[y][x] == TICTAC_CELL_O) {
        col = c_o;
      }
      rgb_matrix_set_color(xy_to_led(lx, ly), col.r, col.g, col.b);
    }
  }

  // Score squares: left column = O (blue), right column = X (red). Only
  // the lit squares are shown; empty ones stay black.
  for (uint8_t i = 0; i < TICTAC_MAX_SCORE; i++) {
    RGB col = {0, 0, 0};
    if (i < tictac_score_o_count) {
      col = c_o;
    }
    rgb_matrix_set_color(xy_to_led(tictac_score_o[i].x, tictac_score_o[i].y),
                         col.r, col.g, col.b);
  }
  for (uint8_t i = 0; i < TICTAC_MAX_SCORE; i++) {
    RGB col = {0, 0, 0};
    if (i < tictac_score_x_count) {
      col = c_x;
    }
    rgb_matrix_set_color(xy_to_led(tictac_score_x[i].x, tictac_score_x[i].y),
                         col.r, col.g, col.b);
  }

  // Turn indicator: the spacebar shows the current player's color.
  RGB turn = (current_player == 0) ? c_x : c_o;
  rgb_matrix_set_color(TICTAC_TURN_LED, turn.r, turn.g, turn.b);
}

// --- Hooks ---

void keyboard_post_init_tictac(void) { tictac_running = false; }

bool process_record_tictac(uint16_t keycode, keyrecord_t* record) {
  // Only intercept input while the game is running so the cell keys keep
  // their normal meaning everywhere else.
  if (!tictac_running) {
    return true;
  }
  if (!record->event.pressed) {
    return true;
  }

  // A cell key press places the current player's mark in the clicked
  // cell, resolved from the key's physical position.
  if (keycode == TICTAC_CELL_KEY) {
    uint8_t bx, by;
    if (tictac_cell_from_pos(record->event.key.col, record->event.key.row, &bx,
                             &by)) {
      tictac_place(bx, by);
    }
    return false;
  }

  return true;
}

bool rgb_matrix_indicators_tictac(void) {
  if (!tictac_running) {
    return false;
  }
  tictac_draw();
  return true;
}

// --- Public helpers ---

void tictac_start(void) {
  tictac_running = true;
  tictac_reset();
}

void tictac_stop(void) { tictac_running = false; }

bool tictac_is_enabled(void) { return tictac_running; }

#include "battleship.h"

#include "quantum.h"
#include "rgb_matrix.h"

typedef struct {
  int8_t x;
  int8_t y;
} bship_offset_t;

typedef enum {
  BSHIP_CELL_EMPTY,
  BSHIP_CELL_SHIP,
  BSHIP_CELL_HIT,
  BSHIP_CELL_MISS,
} bship_cell_t;

// A ship that has been placed on a board, with its absolute cells.
typedef struct {
  uint8_t len;
  uint8_t remaining;                     // unhit cells
  uint8_t cells[BSHIP_MAX_SHIP_LEN][2];  // [i] = {x, y}
} bship_placed_ship_t;

typedef enum {
  BSHIP_PHASE_PLACE,
  BSHIP_PHASE_BATTLE,
  BSHIP_PHASE_REVEAL,
} bship_phase_t;

// Ships, each an array of (dx, dy) offsets from the ship center (0,0),
// terminated by BSHIP_SHIP_END. Rotation happens around (0,0).
static const bship_offset_t bship_ships[][BSHIP_MAX_SHIP_LEN] = BSHIP_SHIPS;
#define BSHIP_NUM_SHIPS (sizeof(bship_ships) / sizeof(bship_ships[0]))

static bship_cell_t bship_board[2][BSHIP_ROWS][BSHIP_COLS];
static bship_placed_ship_t bship_placed[2][BSHIP_NUM_SHIPS];
static uint8_t bship_placed_count[2];

static uint8_t bship_score[2];

static bship_phase_t bship_phase;
static uint8_t bship_current_player;  // placer / attacker
static uint8_t bship_current_ship;    // ship being placed
static uint8_t bship_rotation;        // 0..3 (CW quarter turns)
static uint8_t bship_cursor_board;    // board the cursor is on
static uint8_t bship_cursor_x;
static uint8_t bship_cursor_y;
static bool bship_running;

static const RGB c_p1 = BSHIP_COLOR_P1;
static const RGB c_p2 = BSHIP_COLOR_P2;
static const RGB c_cursor = BSHIP_COLOR_CURSOR;
static const RGB c_hit = BSHIP_COLOR_HIT;
static const RGB c_miss = BSHIP_COLOR_MISS;
static const RGB c_empty = BSHIP_COLOR_EMPTY;

// Score square positions.
static const bship_offset_t bship_score_p1[BSHIP_MAX_SCORE] =
    BSHIP_SCORE_P1_LEDS;
static const bship_offset_t bship_score_p2[BSHIP_MAX_SCORE] =
    BSHIP_SCORE_P2_LEDS;

static uint8_t bship_board_x(uint8_t player) {
  return player == 0 ? BSHIP_P1_X : BSHIP_P2_X;
}
static uint8_t bship_board_y(uint8_t player) {
  return player == 0 ? BSHIP_P1_Y : BSHIP_P2_Y;
}
static uint16_t bship_cell_led(uint8_t player, uint8_t x, uint8_t y) {
  return xy_to_led(bship_board_x(player) + x, bship_board_y(player) + y);
}

static RGB bship_player_color(uint8_t player) {
  return player == 0 ? c_p1 : c_p2;
}

// Empty cells only show a grid on the board the cursor currently
// interacts with (the placer's board, or the board being fired on).
// Other boards render black so non-interactable keys stay dark.
static RGB bship_board_empty_color(uint8_t board) {
  if (board == bship_cursor_board) {
    return c_empty;
  }
  return (RGB){0, 0, 0};
}

// Resolve a physical matrix position to (board, x, y), or return
// false if it isn't one of the board cells.
static bool bship_cell_from_pos(uint8_t x, uint8_t y, uint8_t* board,
                                uint8_t* bx, uint8_t* by) {
  uint8_t px, py;
  if (!mat_log_to_phys(x, y, &px, &py)) {
    return false;
  }
  if (px >= BSHIP_P1_X && px < BSHIP_P1_X + BSHIP_COLS && py >= BSHIP_P1_Y &&
      py < BSHIP_P1_Y + BSHIP_ROWS) {
    *board = 0;
    *bx = px - BSHIP_P1_X;
    *by = py - BSHIP_P1_Y;
    return true;
  }
  if (px >= BSHIP_P2_X && px < BSHIP_P2_X + BSHIP_COLS && py >= BSHIP_P2_Y &&
      py < BSHIP_P2_Y + BSHIP_ROWS) {
    *board = 1;
    *bx = px - BSHIP_P2_X;
    *by = py - BSHIP_P2_Y;
    return true;
  }
  return false;
}

// Rotate an offset (ox, oy) 90 degrees CW `rot` times.
static void bship_rotated_offset(int8_t ox, int8_t oy, uint8_t rot, int8_t* rx,
                                 int8_t* ry) {
  switch (rot & 3) {
    case 0:
      *rx = ox;
      *ry = oy;
      break;
    case 1:
      *rx = oy;
      *ry = -ox;
      break;
    case 2:
      *rx = -ox;
      *ry = -oy;
      break;
    case 3:
      *rx = -oy;
      *ry = ox;
      break;
  }
}

static uint8_t bship_current_len(void) {
  uint8_t len = 0;
  while (len < BSHIP_MAX_SHIP_LEN) {
    if (bship_ships[bship_current_ship][len].x == -128) {
      break;
    }
    len++;
  }
  return len;
}

// Clamp the cursor so the current ship (at the current rotation) fits
// entirely inside the board it is being placed on.
static void bship_clamp_cursor(void) {
  uint8_t len = bship_current_len();
  int8_t minx = 0, maxx = 0, miny = 0, maxy = 0;
  for (uint8_t i = 0; i < len; i++) {
    int8_t rx, ry;
    bship_rotated_offset(bship_ships[bship_current_ship][i].x,
                         bship_ships[bship_current_ship][i].y, bship_rotation,
                         &rx, &ry);
    if (rx < minx) minx = rx;
    if (rx > maxx) maxx = rx;
    if (ry < miny) miny = ry;
    if (ry > maxy) maxy = ry;
  }
  int8_t xmin = -minx;
  int8_t xmax = (int8_t)BSHIP_COLS - 1 - maxx;
  int8_t ymin = -miny;
  int8_t ymax = (int8_t)BSHIP_ROWS - 1 - maxy;
  if ((int8_t)bship_cursor_x < xmin) bship_cursor_x = (uint8_t)xmin;
  if ((int8_t)bship_cursor_x > xmax) bship_cursor_x = (uint8_t)xmax;
  if ((int8_t)bship_cursor_y < ymin) bship_cursor_y = (uint8_t)ymin;
  if ((int8_t)bship_cursor_y > ymax) bship_cursor_y = (uint8_t)ymax;
}

static void bship_set_cursor(uint8_t board, uint8_t x, uint8_t y) {
  bship_cursor_board = board;
  bship_cursor_x = x;
  bship_cursor_y = y;
}

// Finalize the current ship at the cursor position on the active board.
static bool bship_place_ship(void) {
  uint8_t p = bship_current_player;
  uint8_t len = bship_current_len();

  // Overlap check.
  for (uint8_t i = 0; i < len; i++) {
    int8_t rx, ry;
    bship_rotated_offset(bship_ships[bship_current_ship][i].x,
                         bship_ships[bship_current_ship][i].y, bship_rotation,
                         &rx, &ry);
    uint8_t x = (uint8_t)((int16_t)bship_cursor_x + rx);
    uint8_t y = (uint8_t)((int16_t)bship_cursor_y + ry);
    if (bship_board[p][y][x] == BSHIP_CELL_SHIP) {
      return false;
    }
  }

  bship_placed_ship_t* ps = &bship_placed[p][bship_placed_count[p]];
  ps->len = len;
  ps->remaining = len;
  for (uint8_t i = 0; i < len; i++) {
    int8_t rx, ry;
    bship_rotated_offset(bship_ships[bship_current_ship][i].x,
                         bship_ships[bship_current_ship][i].y, bship_rotation,
                         &rx, &ry);
    uint8_t x = (uint8_t)((int16_t)bship_cursor_x + rx);
    uint8_t y = (uint8_t)((int16_t)bship_cursor_y + ry);
    bship_board[p][y][x] = BSHIP_CELL_SHIP;
    ps->cells[i][0] = x;
    ps->cells[i][1] = y;
  }
  bship_placed_count[p]++;

  bship_current_ship++;
  if (bship_current_ship < BSHIP_NUM_SHIPS) {
    // More ships for this player.
    bship_rotation = 0;
    return true;
  }

  // All ships placed for the current player.
  if (p == 0) {
    // P2's placement, UI flipped to the right board.
    bship_current_player = 1;
    bship_current_ship = 0;
    bship_rotation = 0;
    bship_set_cursor(1, BSHIP_COLS / 2, BSHIP_ROWS / 2);
  } else {
    // Both placed: enter battle. P1 attacks P2's board first.
    bship_phase = BSHIP_PHASE_BATTLE;
    bship_current_player = 0;
    bship_rotation = 0;
    bship_set_cursor(1, BSHIP_COLS / 2, BSHIP_ROWS / 2);
  }
  return true;
}

static void bship_register_hit(uint8_t p, uint8_t x, uint8_t y) {
  for (uint8_t i = 0; i < bship_placed_count[p]; i++) {
    for (uint8_t j = 0; j < bship_placed[p][i].len; j++) {
      if (bship_placed[p][i].cells[j][0] == x &&
          bship_placed[p][i].cells[j][1] == y) {
        if (bship_placed[p][i].remaining > 0) {
          bship_placed[p][i].remaining--;
        }
        return;
      }
    }
  }
}

static bool bship_all_destroyed(uint8_t p) {
  for (uint8_t i = 0; i < bship_placed_count[p]; i++) {
    if (bship_placed[p][i].remaining > 0) {
      return false;
    }
  }
  return true;
}

static void bship_attack(void) {
  if (bship_phase != BSHIP_PHASE_BATTLE) {
    return;
  }
  uint8_t target = 1 - bship_current_player;  // board being attacked
  uint8_t x = bship_cursor_x;
  uint8_t y = bship_cursor_y;
  bship_cell_t cell = bship_board[target][y][x];
  if (cell == BSHIP_CELL_HIT || cell == BSHIP_CELL_MISS) {
    return;  // already attacked
  }

  if (cell == BSHIP_CELL_SHIP) {
    bship_board[target][y][x] = BSHIP_CELL_HIT;
    bship_register_hit(target, x, y);
    if (bship_all_destroyed(target)) {
      bship_score[bship_current_player]++;  // winner gains a point
      bship_phase = BSHIP_PHASE_REVEAL;
      return;
    }
  } else {
    bship_board[target][y][x] = BSHIP_CELL_MISS;
  }

  // Switch turn.
  bship_current_player = 1 - bship_current_player;
  bship_set_cursor(1 - bship_current_player, BSHIP_COLS / 2, BSHIP_ROWS / 2);
}

// Advance past the reveal phase into a new round.
static void bship_start_placement(void);
static void bship_advance(void) {
  if (bship_score[0] >= BSHIP_MAX_SCORE || bship_score[1] >= BSHIP_MAX_SCORE) {
    bship_score[0] = 0;
    bship_score[1] = 0;
  }
  bship_start_placement();
}

// --- Drawing ---

static void bship_draw_placement(void) {
  uint8_t p = bship_current_player;
  uint8_t opp = 1 - p;

  // Opponent's half is inactive (no grid while not being interacted with).
  for (uint8_t y = 0; y < BSHIP_ROWS; y++) {
    for (uint8_t x = 0; x < BSHIP_COLS; x++) {
      RGB col = bship_board_empty_color(opp);
      rgb_matrix_set_color(bship_cell_led(opp, x, y), col.r, col.g, col.b);
    }
  }

  // Active board: placed ships + empty cells.
  for (uint8_t y = 0; y < BSHIP_ROWS; y++) {
    for (uint8_t x = 0; x < BSHIP_COLS; x++) {
      RGB col = bship_board_empty_color(p);
      if (bship_board[p][y][x] == BSHIP_CELL_SHIP) {
        col = bship_player_color(p);
      }
      rgb_matrix_set_color(bship_cell_led(p, x, y), col.r, col.g, col.b);
    }
  }

  // Preview the current (pending) ship at the cursor, drawn entirely in
  // the cursor color so the whole ship is highlighted.
  uint8_t len = bship_current_len();
  for (uint8_t i = 0; i < len; i++) {
    int8_t rx, ry;
    bship_rotated_offset(bship_ships[bship_current_ship][i].x,
                         bship_ships[bship_current_ship][i].y, bship_rotation,
                         &rx, &ry);
    uint8_t x = (uint8_t)((int16_t)bship_cursor_x + rx);
    uint8_t y = (uint8_t)((int16_t)bship_cursor_y + ry);
    rgb_matrix_set_color(bship_cell_led(p, x, y), c_cursor.r, c_cursor.g,
                         c_cursor.b);
  }
}

static void bship_draw_battle(void) {
  // Ships are hidden; only hits (green) and misses (red) are shown.
  for (uint8_t p = 0; p < 2; p++) {
    for (uint8_t y = 0; y < BSHIP_ROWS; y++) {
      for (uint8_t x = 0; x < BSHIP_COLS; x++) {
        RGB col = bship_board_empty_color(p);
        switch (bship_board[p][y][x]) {
          case BSHIP_CELL_HIT:
            col = c_hit;
            break;
          case BSHIP_CELL_MISS:
            col = c_miss;
            break;
          default:
            break;
        }
        rgb_matrix_set_color(bship_cell_led(p, x, y), col.r, col.g, col.b);
      }
    }
  }
}

static void bship_draw_reveal(void) {
  // Reveal all ships in their owner's color, plus hits/misses.
  for (uint8_t p = 0; p < 2; p++) {
    for (uint8_t y = 0; y < BSHIP_ROWS; y++) {
      for (uint8_t x = 0; x < BSHIP_COLS; x++) {
        RGB col = c_empty;
        switch (bship_board[p][y][x]) {
          case BSHIP_CELL_SHIP:
            col = bship_player_color(p);
            break;
          case BSHIP_CELL_HIT:
            col = c_hit;
            break;
          case BSHIP_CELL_MISS:
            col = c_miss;
            break;
          default:
            break;
        }
        rgb_matrix_set_color(bship_cell_led(p, x, y), col.r, col.g, col.b);
      }
    }
  }
}

static void bship_draw_scores(void) {
  for (uint8_t i = 0; i < BSHIP_MAX_SCORE; i++) {
    RGB col = (i < bship_score[0]) ? c_p1 : (RGB){0, 0, 0};
    rgb_matrix_set_color(xy_to_led(bship_score_p1[i].x, bship_score_p1[i].y),
                         col.r, col.g, col.b);
    col = (i < bship_score[1]) ? c_p2 : (RGB){0, 0, 0};
    rgb_matrix_set_color(xy_to_led(bship_score_p2[i].x, bship_score_p2[i].y),
                         col.r, col.g, col.b);
  }
}

static void bship_draw(void) {
  rgb_matrix_set_color_all(0, 0, 0);
  switch (bship_phase) {
    case BSHIP_PHASE_PLACE:
      bship_draw_placement();
      break;
    case BSHIP_PHASE_BATTLE:
      bship_draw_battle();
      break;
    case BSHIP_PHASE_REVEAL:
      bship_draw_reveal();
      break;
  }
  bship_draw_scores();

  // Turn indicator: the spacebar shows the current player's color.
  RGB turn = bship_player_color(bship_current_player);
  rgb_matrix_set_color(BSHIP_TURN_LED, turn.r, turn.g, turn.b);
}

// --- Game setup ---

static void bship_start_placement(void) {
  for (uint8_t p = 0; p < 2; p++) {
    for (uint8_t y = 0; y < BSHIP_ROWS; y++) {
      for (uint8_t x = 0; x < BSHIP_COLS; x++) {
        bship_board[p][y][x] = BSHIP_CELL_EMPTY;
      }
    }
    bship_placed_count[p] = 0;
  }
  bship_current_player = 0;
  bship_current_ship = 0;
  bship_rotation = 0;
  bship_set_cursor(0, BSHIP_COLS / 2, BSHIP_ROWS / 2);
  bship_phase = BSHIP_PHASE_PLACE;
}

static void bship_reset(void) {
  bship_score[0] = 0;
  bship_score[1] = 0;
  bship_start_placement();
}

// --- Hooks ---

void keyboard_post_init_battleship(void) { bship_running = false; }

bool process_record_battleship(uint16_t keycode, keyrecord_t* record) {
  if (!bship_running) {
    return true;
  }
  if (!record->event.pressed) {
    return true;
  }

  // A cell key tap is resolved by its physical position.
  if (keycode == BSHIP_CELL_KEY) {
    if (bship_phase == BSHIP_PHASE_REVEAL) {
      bship_advance();
      return false;
    }
    uint8_t board, bx, by;
    if (bship_cell_from_pos(record->event.key.col, record->event.key.row,
                            &board, &bx, &by)) {
      if (bship_phase == BSHIP_PHASE_PLACE) {
        if (board == bship_current_player) {
          bship_cursor_board = board;
          bship_cursor_x = bx;
          bship_cursor_y = by;
          bship_clamp_cursor();
        }
      } else if (bship_phase == BSHIP_PHASE_BATTLE) {
        uint8_t target = 1 - bship_current_player;
        if (board == target) {
          bship_cursor_board = board;
          bship_cursor_x = bx;
          bship_cursor_y = by;
          bship_attack();
        }
      }
    }
    return false;  // consume the cell key
  }

  switch (bship_phase) {
    case BSHIP_PHASE_REVEAL:
      // Rotate or place advances to the next round.
      if (keycode == BSHIP_KEY_ROTATE || keycode == BSHIP_KEY_PLACE) {
        bship_advance();
        return false;
      }
      return true;

    case BSHIP_PHASE_PLACE:
      switch (keycode) {
        case BSHIP_KEY_ROTATE:
          bship_rotation = (bship_rotation + 1) & 3;
          bship_clamp_cursor();
          return false;
        case BSHIP_KEY_PLACE:
          bship_place_ship();
          return false;
      }
      return true;

    case BSHIP_PHASE_BATTLE:
      // Rotate/place have no meaning during battle; consume them so they
      // don't leak through as normal keys.
      if (keycode == BSHIP_KEY_ROTATE || keycode == BSHIP_KEY_PLACE) {
        return false;
      }
      return true;
  }

  return true;
}

bool rgb_matrix_indicators_battleship(void) {
  if (!bship_running) {
    return false;
  }
  bship_draw();
  return true;
}

// --- Public helpers ---

void battleship_start(void) {
  bship_running = true;
  bship_reset();
}

void battleship_stop(void) { bship_running = false; }

bool battleship_is_enabled(void) { return bship_running; }

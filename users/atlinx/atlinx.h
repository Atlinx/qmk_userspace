#pragma once
#include "quantum.h"

#pragma region TYPES
typedef union {
  uint32_t raw;
  struct {
    uint8_t base_overlay : 8;
    bool base_rgb_enabled : 1;
  };
} user_config_t;

extern user_config_t user_config;

typedef enum {
  AK_CYCLE_BOV = SAFE_RANGE,
  AK_BASE_TOGG,

  AK_END
} atlinx_keycode_t;

#undef SAFE_RANGE
#define SAFE_RANGE AK_END

typedef enum {
  BOV_NONE,
  BOV_TRANS_FLAG,
  BOV_LESBIAN_FLAG,
  BOV_BIG_TRANS_FLAG,
  BOV_BIG_LESBIAN_FLAG,

  BOV_COUNT,
} BaseOverlay;
#pragma endregion

#pragma region MACROS
// Colors
#define C______ {0, 0, 0}       /*RGB*/
#define C_WHITE {255, 255, 255} /*RGB*/
#define C_PINK {255, 60, 157}   /*RGB*/
#define C_RED {255, 0, 0}       /*RGB*/
#define C_ORANGE {234, 116, 0}  /*RGB*/
#define C_YELLOW {255, 255, 0}  /*RGB*/
#define C_GREEN {49, 252, 3}    /*RGB*/
#define C_CYAN {0, 174, 255}    /*RGB*/
#define C_BLUE {0, 0, 255}      /*RGB*/
#define C_PURPLE {105, 7, 252}  /*RGB*/
#define C_MAGENTA {255, 0, 222} /*RGB*/

#define C_RED_ORANGE {255, 50, 0}      /*RGB*/
#define C_PURPLE_LIGHT {163, 101, 255} /*RGB*/

#define C_WHITE_DARK {64, 64, 64}  /*RGB*/
#define C_PINK_DARK {64, 15, 39}   /*RGB*/
#define C_RED_DARK {64, 0, 0}      /*RGB*/
#define C_ORANGE_DARK {59, 29, 0}  /*RGB*/
#define C_YELLOW_DARK {64, 64, 0}  /*RGB*/
#define C_GREEN_DARK {17, 64, 6}   /*RGB*/
#define C_CYAN_DARK {0, 44, 64}    /*RGB*/
#define C_BLUE_DARK {0, 0, 64}     /*RGB*/
#define C_PURPLE_DARK {26, 2, 63}  /*RGB*/
#define C_MAGENTA_DARK {64, 0, 56} /*RGB*/

// Unicode Keycodes
#define UC_ACC_E UC(0xE9)      // é
#define UC_ACC_U UC(0xFA)      // ú
#define UC_ACC_UU UC(0xFC)     // ü
#define UC_ACC_I UC(0xED)      // í
#define UC_ACC_O UC(0xF3)      // ó
#define UC_ACC_A UC(0xE1)      // á
#define UC_ACC_N UC(0xF1)      // ñ
#define UC_ACC_E_UP UC(0xC9)   // É
#define UC_ACC_U_UP UC(0xDA)   // Ú
#define UC_ACC_UU_UP UC(0xDC)  // Ü
#define UC_ACC_I_UP UC(0xCD)   // Í
#define UC_ACC_O_UP UC(0xD3)   // Ó
#define UC_ACC_A_UP UC(0xC1)   // Á
#define UC_ACC_N_UP UC(0xD1)   // Ñ
#define UC_EXCL_OPEN UC(0xA1)  // ¡
#define UC_QUES_OPEN UC(0xBF)  // ¿
#define UC_EM_DASH UC(0x2014)  // —

// LED indicator key feature, used by keyboards like Planck Ez
#ifdef KEYBOARD_zsa_planck_ez_glow
#define C_GREEN_LED C_GREEN
#else
#define LED_LEVEL XXXXXXX
#define C_GREEN_LED C______
#endif

#define X______ KC_NO

#undef PHYSICAL_ROWS
#undef PHYSICAL_COLS
#define PHYSICAL_ROWS 4
#define PHYSICAL_COLS 12

/**
 * @brief Toggles the color of an LED based on a boolean function.
 * @param LED The LED index to toggle.
 * @param TRUE_COLOR The color to use when the boolean function returns true.
 * @param FALSE_COLOR The color to use when the boolean function returns false.
 * @param IS_ON_EXPR The boolean expression to evaluate.
 */
#define LED_TOGGLE(LED, TRUE_COLOR, FALSE_COLOR, IS_ON_EXPR) \
  do {                                                       \
    RGB color = (RGB)TRUE_COLOR;                             \
    if (!IS_ON_EXPR) {                                       \
      color = (RGB)FALSE_COLOR;                              \
    }                                                        \
    rgb_matrix_set_color(LED, color.r, color.g, color.b);    \
  } while (0)

/**
 * @brief Converts a physical matrix position (X, Y) to an index on a
 * contiguous array that represents the physical matrix of the keyboard without
 * any padding (ex. the ledmaps array, which does not have empty cells).
 * @param x The matrix column.
 * @param y The matrix row.
 *
 * Example of unpadded array:
 * ```text
 * X X X X
 * X X X X
 * X X   X
 *     '-- no padding, only includes keys on the keyboard
 *
 * Entire array size is 4 + 4 + 3 = 7
 *
 * ```
 */
uint8_t mat_phys_to_index(uint8_t x, uint8_t y);

/**
 * @brief Alias for `mat_phys_to_index`
 */
static inline uint8_t xy_to_led(uint8_t x, uint8_t y) {
  return mat_phys_to_index(x, y);
}

/**
 * @brief Converts a physical matrix position (X, Y) to an index on a
 * contiguous array that represents the physical matrix of the keyboard with
 * padding (ex. the keymaps array, which uses KC_NO for empty cells).
 * @param x The matrix column.
 * @param y The matrix row.
 *
 * Example of padded array:
 *
 * ```text
 * X X X X
 * X X X X
 * X X _ X
 *     '-- padding for keys that takeup multiple keys
 *         (ex. 2u spacebar on Planck MIT)
 *
 * Entire array size is 4 x 3 = 12
 *
 * ```
 */
uint8_t mat_phys_to_padded_index(uint8_t x, uint8_t y);

/**
 * @brief Converts a logical matrix position (X, Y) to a physical position (X,
 * Y).
 *
 * The default (weak) implementation in atlinx.c assumes the matrix grid
 * matches the physical grid (identity). Keyboards whose hardware matrix
 * differs (e.g. the planck EZ 8x6 matrix) override this in their
 * keymap.c. Returns true and writes the physical (x, y) on success.
 * @param log_x The logical matrix column (x).
 * @param log_y The logical matrix row (y).
 * @param phys_x Output physical column (x).
 * @param phys_y Output physical row (y).
 */
bool mat_log_to_phys(uint8_t log_x, uint8_t log_y, uint8_t* phys_x,
                     uint8_t* phys_y);
#pragma endregion

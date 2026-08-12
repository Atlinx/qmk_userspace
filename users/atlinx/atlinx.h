#pragma once
#include "quantum.h"

#ifdef AUDIO_ENABLE
#    include "process_music.h"
#endif

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

#define C_ORANGE_LIGHT {255, 183, 110}  /*RGB*/
#define C_PURPLE_LIGHT {163, 101, 255}  /*RGB*/

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

// LED indicator key feature, used by keyboards like Planck Ez
#ifdef LED_LEVEL
#    define C_GREEN_LED C_GREEN
#else
#    define LED_LEVEL XXXXXXX
#    define C_GREEN_LED C______
#endif

// clang-format off
#ifndef LAYOUT_planck_mit
#define LAYOUT_planck_mit(k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H, k0I, k0J, k0K, k0L, k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H, k1I, k1J, k1K, k1L, k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H, k2I, k2J, k2K, k2L, k3A, k3B, k3C, k3D, k3E, k3F, k3H, k3I, k3J, k3K, k3L) { \
    { k0A, k0B, k0C, k0D, k0E, k0F, k0G,   k0H, k0I, k0J, k0K, k0L }, \
    { k1A, k1B, k1C, k1D, k1E, k1F, k1G,   k1H, k1I, k1J, k1K, k1L }, \
    { k2A, k2B, k2C, k2D, k2E, k2F, k2G,   k2H, k2I, k2J, k2K, k2L }, \
    { k3A, k3B, k3C, k3D, k3E, k3F, KC_NO, k3H, k3I, k3J, k3K, k3L } \
}
#endif
// clang-format on

#ifndef MATRIX_ROWS
#define MATRIX_ROWS 4
#endif
#ifndef MATRIX_COLS
#define MATRIX_COLS 12
#endif

#define X______ KC_NO

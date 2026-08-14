#pragma once
#include "atlinx.h"

// clang-format off
#ifndef LAYOUT_planck_mit
#define LAYOUT_planck_mit(k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H, k0I, k0J, k0K, k0L, k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H, k1I, k1J, k1K, k1L, k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H, k2I, k2J, k2K, k2L, k3A, k3B, k3C, k3D, k3E, k3F, k3H, k3I, k3J, k3K, k3L) { \
    { k0A, k0B, k0C, k0D, k0E, k0F, k0G,   k0H, k0I, k0J, k0K, k0L }, \
    { k1A, k1B, k1C, k1D, k1E, k1F, k1G,   k1H, k1I, k1J, k1K, k1L }, \
    { k2A, k2B, k2C, k2D, k2E, k2F, k2G,   k2H, k2I, k2J, k2K, k2L }, \
    { k3A, k3B, k3C, k3D, k3E, k3F, KC_NO, k3H, k3I, k3J, k3K, k3L } \
}
#endif
#ifndef LAYOUT_planck_grid
#define LAYOUT_planck_grid(k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H, k0I, k0J, k0K, k0L, k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H, k1I, k1J, k1K, k1L, k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H, k2I, k2J, k2K, k2L, k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H, k3I, k3J, k3K, k3L) { \
    { k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H, k0I, k0J, k0K, k0L }, \
    { k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H, k1I, k1J, k1K, k1L }, \
    { k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H, k2I, k2J, k2K, k2L }, \
    { k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H, k3I, k3J, k3K, k3L } \
}
#endif
// clang-format on

/*
Plancks have a physical 4x12 key grid.

Some keyboards may have MATRIX_COLS and MATRIX_ROWS that are not 4x12, but still
multiply to 4x12=48. This is based on the keyboard's underlying hardware pins,
which can differ from the physical appearance of the key grid.
*/
#undef PHYSICAL_ROWS
#undef PHYSICAL_COLS
#define PHYSICAL_ROWS 4
#define PHYSICAL_COLS 12

#include "atlinx.h"

user_config_t user_config;

uint8_t __attribute__((weak)) mat_xy(uint8_t x, uint8_t y) {
  return (y * PHYSICAL_COLS + x);
}

// Default matrix->physical mapping: assume the hardware matrix matches
// the physical grid (identity). Keyboards with a different matrix layout
// (e.g. the planck EZ 8x6 matrix) override this in their keymap.c.
bool __attribute__((weak)) mat_to_phys(uint8_t mrow, uint8_t mcol,
                                       uint8_t* phys_row, uint8_t* phys_col) {
  *phys_row = mrow;
  *phys_col = mcol;
  return true;
}

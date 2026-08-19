#include "atlinx.h"

user_config_t user_config;

uint8_t __attribute__((weak)) mat_phys_to_index(uint8_t x, uint8_t y) {
  return (y * PHYSICAL_COLS + x);
}

uint8_t __attribute__((weak)) mat_phys_to_padded_index(uint8_t x, uint8_t y) {
  return (y * PHYSICAL_COLS + x);
}

// Default matrix->physical mapping: assume the hardware matrix matches
// the physical grid (identity). Keyboards with a different matrix layout
// (e.g. the planck EZ 8x6 matrix) override this in their keymap.c.
bool __attribute__((weak)) mat_log_to_phys(uint8_t log_x, uint8_t log_y,
                                           uint8_t* phys_x, uint8_t* phys_y) {
  *phys_x = log_x;
  *phys_y = log_y;
  return true;
}

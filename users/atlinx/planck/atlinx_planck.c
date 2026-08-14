#include "atlinx_planck.h"

uint8_t mat_xy(uint8_t x, uint8_t y) {
  if (y == 3) {
    if (x == 6) {
      return (y * PHYSICAL_COLS + 5);
    } else if (x > 6) {
      return (y * PHYSICAL_COLS + x - 1);
    }
  }
  return (y * PHYSICAL_COLS + x);
}

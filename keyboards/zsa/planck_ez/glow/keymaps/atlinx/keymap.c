#include QMK_KEYBOARD_H
#include "atlinx.h"

// Planck EZ: the hardware matrix is 8x6, not the physical 4x12 grid.
// Matrix rows 0/1/2 = left half of physical rows 0/1/2; rows 4/5/6 =
// right half; rows 3/7 = the bottom physical row (has a 2u space).
bool mat_to_phys(uint8_t mrow, uint8_t mcol, uint8_t* phys_row,
                 uint8_t* phys_col) {
  if (mrow <= 2 || (mrow >= 4 && mrow <= 6)) {
    *phys_row = mrow % 4;
    *phys_col = (mrow < 4) ? mcol : (6 + mcol);
    return true;
  }
  if (mrow == 3 || mrow == 7) {
    static const uint8_t row3_left[6] = {0, 1, 2, 10, 11, 6};
    static const uint8_t row3_right[6] = {7, 8, 9, 3, 4, 5};
    *phys_row = 3;
    *phys_col = (mrow == 3) ? row3_left[mcol] : row3_right[mcol];
    return true;
  }
  return false;
}

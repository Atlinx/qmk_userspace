#include QMK_KEYBOARD_H
#include "atlinx.h"

/*
For the Planck EZ, the logical hardware matrix is 8x6 (row, col) and differs
from the physical 4x12 grid of a planck keyboard. The mapping from logical to
physical is depicted below. Contiguous blocks of the same letter are assumed to
preserve the relative order of each element across the mapping.

Planck EZ physical layout:
 x  0  1  2  3  4  5  6  7  8  9  10 11
y
0   L  L  L  L  L  L  R  R  R  R  R  R
1   L  L  L  L  L  L  R  R  R  R  R  R
2   L  L  L  L  L  L  R  R  R  R  R  R
3   L  L  L  S  S  S     F  F  F  M  M

Planck EZ logical layout:
 x 0 1 2 3 4 5
y
0  L L L L L L  <--- Left half of keyboard
1  L L L L L L
2  L L L L L L
3  L L L M M _
4  R R R R R R  <--- Right half of keyboard (flip the layout)
5  R R R R R R
6  R R R R R R
7  F F F S S S
*/
bool mat_log_to_phys(uint8_t log_x, uint8_t log_y, uint8_t* phys_x,
                     uint8_t* phys_y) {
  // Handle logical rows 3 and 7, which are a mishmash of mappings
  if (log_y == 3 || log_y == 7) {
    static const uint8_t bottom_cols[2][6] = {
        {0, 1, 2, 10, 11, 0},  // Logical row 3.
        {7, 8, 9, 3, 4, 5},    // Logical row 7
    };
    *phys_y = 3;
    *phys_x = bottom_cols[log_y == 7][log_x];
    return true;
  }
  // Handle the L and R chunks in the physical layout
  // If we take the y mod 4, we get the proper y value for the L and R chunks
  *phys_y = log_y % 4;
  // If we're in the R chunk, then we offset x by 6
  *phys_x = log_x + (log_y >= 4 ? 6 : 0);
  return true;
}

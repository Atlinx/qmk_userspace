#include "atlinx_planck.h"

uint8_t mat_phys_to_index(uint8_t x, uint8_t y) {
  if (y == 3) {
    // Handle the last row which includes the 2u spacebar.
    if (x == 6) {
      // The second key of the spacebar should map to the same index as
      // the previous key (x = 5)
      return (y * PHYSICAL_COLS + 5);
    } else if (x > 6) {
      // Offset remaining indices by -1 to account for losing a key to the 2u
      // spacebar
      return (y * PHYSICAL_COLS + x - 1);
    }
  }
  return (y * PHYSICAL_COLS + x);
}

#pragma once
#include "planck/atlinx_planck.h"

typedef enum {
  L_BASE,
  L_SNAKE,
  L_TICTAC,
  L_BATTLESHIP,
  L_FNS,
  L_NUMPAD,
  L_MOUSE,
  L_ARROWS,
  L_GAMING,
  L_BLEND,
  L_LNUMPAD,
  L_LNAV,
  L_GGST,
  L_LNUMS,
  L_LFNS,
  L_LOWER,
  L_RAISE,
  L_ADJUST,
  L_ACCENTS,
  L_UC_ACC_C,
  L_LOWSHFT,

  L_COUNT,
} atlinx_layer_t;

bool process_record_local(uint16_t keycode, keyrecord_t* record);
bool rgb_matrix_indicators_local(void);

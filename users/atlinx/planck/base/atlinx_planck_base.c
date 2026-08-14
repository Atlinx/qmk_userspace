#include "atlinx_planck_base.h"

#include "info_config.h"

#pragma region MAPS
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Base
  [L_BASE] = LAYOUT_planck_mit(
    KC_TAB        , KC_Q       , KC_W   , KC_E         , KC_R       , KC_T     , KC_Y        , KC_U        , KC_I       , KC_O        , KC_P        , KC_BSPC,
    KC_ESC        , KC_A       , KC_S   , KC_D         , KC_F       , KC_G     , KC_H        , KC_J        , KC_K       , KC_L        , KC_SCLN     , KC_QUOT,
    KC_LSFT       , KC_Z       , KC_X   , KC_C         , KC_V       , KC_B     , KC_N        , KC_M        , KC_COMM    , KC_DOT      , KC_SLSH     , KC_ENT ,
    KC_LCTL       , KC_LALT    , KC_F24 , KC_LGUI      , MO(L_LOWER), KC_SPC                 , MO(L_RAISE) , KC_LEFT    , KC_DOWN     , KC_UP       , KC_RGHT
  ),

  // Lower
  [L_LOWER] = LAYOUT_planck_mit(
    KC_TILD       , KC_EXLM    , KC_AT  , KC_HASH      , KC_DLR     , KC_PERC  , KC_CIRC     , KC_AMPR     , KC_ASTR    , KC_LPRN     , KC_RPRN     , X______,
    KC_DEL        , MS_WHLD    , X______, MS_WHLU      , X______    , X______  , X______     , KC_UNDS     , KC_PLUS    , KC_LCBR     , KC_RCBR     , KC_PIPE,
    MO(L_LOWSHFT) , MS_BTN1    , MS_BTN3, MS_BTN2      , KC_LSFT    , X______  , X______     , X______     , X______    , KC_HOME     , KC_END      , X______,
    X______       , X______    , X______, X______      , _______    , _______                , _______     , X______    , X______     , X______     , X______
  ),

  // Raise
  [L_RAISE] = LAYOUT_planck_mit(
    KC_GRV        , KC_1       , KC_2   , KC_3         , KC_4       , KC_5     , KC_6        , KC_7        , KC_8       , KC_9        , KC_0        , X______,
    KC_DEL        , KC_F1      , KC_F2  , KC_F3        , KC_F4      , KC_F5    , KC_F6       , KC_MINS     , KC_EQL     , KC_LBRC     , KC_RBRC     , KC_BSLS,
    X______       , KC_F7      , KC_F8  , KC_F9        , KC_F10     , KC_F11   , KC_F12      , X______     , X______    , X______     , X______     , X______,
    X______       , X______    , X______, MO(L_ACCENTS), _______    , _______                , _______     , X______    , X______     , X______     , X______
  ),

  // Adjust
  [L_ADJUST] = LAYOUT_planck_mit(
    X______       , X______    , X______, X______      , X______    , X______  , X______     , X______     , X______    , X______     , X______     , X______,
    TO(L_BASE)    , AU_TOGG    , MU_TOGG, MU_NEXT      , X______    , KC_BRID  , KC_BRIU     , X______     , X______    , RM_VALU     , RM_VALD     , QK_BOOT,
    X______       , X______    , X______, X______      , X______    , LED_LEVEL, AK_CYCLE_BOV, X______     , X______    , AK_BASE_TOGG, RM_NEXT     , EE_CLR ,
    KC_MPRV       , KC_MNXT    , KC_MSTP, KC_MPLY      , _______    , _______                , _______     , KC_MUTE    , KC_VOLD     , KC_VOLU     , KC_MPLY
  ),

  // Accents
  [L_ACCENTS] = LAYOUT_planck_mit(
    X______       , X______    , X______, UC_ACC_E     , X______    , X______  , UC_ACC_U    , UC_ACC_UU   , UC_ACC_I   , UC_ACC_O    , X______     , _______,
    X______       , UC_ACC_A   , X______, X______      , X______    , X______  , X______     , X______     , X______    , X______     , X______     , X______,
    MO(L_UC_ACC_C), X______    , X______, X______      , X______    , X______  , UC_ACC_N    , X______     , X______    , UC_EXCL_OPEN, UC_QUES_OPEN, X______,
    X______       , X______    , X______, X______      , _______    , _______                , _______     , X______    , X______     , X______     , X______
  ),

  // Accents capitalized
  [L_UC_ACC_C] = LAYOUT_planck_mit(
    X______       , X______    , X______, UC_ACC_E_UP  , X______    , X______  , UC_ACC_U_UP , UC_ACC_UU_UP, UC_ACC_I_UP, UC_ACC_O_UP , X______     , _______,
    X______       , UC_ACC_A_UP, X______, X______      , X______    , X______  , X______     , X______     , X______    , X______     , X______     , X______,
    _______       , X______    , X______, X______      , X______    , X______  , UC_ACC_N_UP , X______     , X______    , X______     , X______     , X______,
    X______       , X______    , X______, X______      , _______    , _______                , _______     , X______    , X______     , X______     , X______
  ),

  // Low Shift
  [L_LOWSHFT] = LAYOUT_planck_mit(
    X______       , X______    , X______, X______      , X______    , X______  , X______     , X______     , X______    , X______     , X______     , _______,
    X______       , X______    , X______, X______      , X______    , X______  , X______     , UC_EM_DASH  , X______    , X______     , X______     , X______,
    X______       , X______    , X______, X______      , X______    , X______  , X______     , X______     , X______    , X______     , X______     , X______,
    X______       , X______    , X______, X______      , _______    , _______                , _______     , X______    , X______     , X______     , X______
  ),
};
// clang-format on
#pragma endregion

#pragma region KEYCODES
layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, L_LOWER, L_RAISE, L_ADJUST);
}
#pragma endregion

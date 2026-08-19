#include "atlinx_planck_glow.h"

#include "action.h"
#include "action_layer.h"
#include "atlinx.h"
#include "color.h"
#include "debug.h"
#include "eeconfig.h"
#include "info_config.h"
#include "keycodes.h"
#include "mods/battleship.h"
#include "mods/snake.h"
#include "mods/tictac.h"
#include "quantum.h"
#include "quantum/unicode/unicode.h"
#include "rgb_matrix.h"

#define ldprintf(...) dprintf("[atlinx_planck_glow] " __VA_ARGS__)

#pragma region MAPS
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Base
  [L_BASE] = LAYOUT_planck_mit(
    KC_TAB        , KC_Q        , KC_W       , KC_E         , KC_R       , KC_T     , KC_Y        , KC_U        , KC_I        , KC_O            , KC_P        , KC_BSPC   ,
    KC_ESC        , KC_A        , KC_S       , KC_D         , KC_F       , KC_G     , KC_H        , KC_J        , KC_K        , KC_L            , KC_SCLN     , KC_QUOT   ,
    KC_LSFT       , KC_Z        , KC_X       , KC_C         , KC_V       , KC_B     , KC_N        , KC_M        , KC_COMM     , KC_DOT          , KC_SLSH     , KC_ENT    ,
    KC_LCTL       , KC_LALT     , KC_F24     , KC_LGUI      , MO(L_LOWER), KC_SPC                 , MO(L_RAISE) , KC_LEFT     , KC_DOWN         , KC_UP       , KC_RGHT
  ),

  // Snake
  [L_SNAKE] = LAYOUT_planck_mit(
    X______       , X______     , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    TO(L_BASE)    , X______     , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______  , X______     , X______     , X______     , SNK_UP          , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , SNK_FAST               , X______     , SNK_LEFT    , SNK_DOWN        , SNK_RIGHT   , X______
  ),

  // Tic-Tac-Toe
  [L_TICTAC] = LAYOUT_planck_mit(
    X______       , X______     , X______    , X______      , KC_1       , KC_1     , KC_1        , X______     , X______     , X______         , X______     , X______   ,
    TO(L_BASE)    , X______     , X______    , X______      , KC_1       , KC_1     , KC_1        , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , KC_1       , KC_1     , KC_1        , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______                , X______     , X______     , X______         , X______     , X______
  ),

  // Battleship
  [L_BATTLESHIP] = LAYOUT_planck_mit(
    KC_1          , KC_1        , KC_1       , KC_1         , KC_1       , X______  , X______     , KC_1        , KC_1        , KC_1            , KC_1        , KC_1      ,
    KC_1          , KC_1        , KC_1       , KC_1         , KC_1       , X______  , X______     , KC_1        , KC_1        , KC_1            , KC_1        , KC_1      ,
    KC_1          , KC_1        , KC_1       , KC_1         , KC_1       , KC_R     , KC_ENT      , KC_1        , KC_1        , KC_1            , KC_1        , KC_1      ,
    KC_1          , KC_1        , KC_1       , KC_1         , KC_1       , TO(L_BASE)             , KC_1        , KC_1        , KC_1            , KC_1        , KC_1
  ),

  // Function Keys
  [L_FNS] = LAYOUT_planck_mit(
    X______       , X______     , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    TO(L_BASE)    , KC_F13      , KC_F14     , KC_F15       , KC_F16     , KC_F17   , KC_F18      , X______     , X______     , X______         , X______     , X______   ,
    X______       , KC_F19      , KC_F20     , KC_F21       , KC_F22     , KC_F23   , KC_F24      , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______                , X______     , X______     , X______         , X______     , X______
  ),

  // Numpad
  [L_NUMPAD] = LAYOUT_planck_mit(
    X______       , X______     , KC_INS     , KC_HOME      , KC_PGUP    , X______  , KC_PAUS     , X______     , KC_P7       , KC_P8           , KC_P9       , KC_BSPC   ,
    TO(L_BASE)    , X______     , KC_DEL     , KC_END       , KC_PGDN    , X______  , KC_PSCR     , X______     , KC_P4       , KC_P5           , KC_P6       , KC_NUM    ,
    KC_LSFT       , X______     , KC_NUM     , KC_CAPS      , KC_SCRL    , X______  , KC_SYRQ     , X______     , KC_P1       , KC_P2           , KC_P3       , KC_PENT   ,
    KC_LCTL       , KC_LALT     , X______    , KC_LGUI      , X______    , X______                , X______     , KC_P0       , X______         , KC_PDOT     , X______
  ),

  // Mouse
  [L_MOUSE] = LAYOUT_planck_mit(
    X______       , X______     , MS_BTN4    , MS_WHLU      , MS_BTN5    , X______  , X______     , X______     , MS_UP       , X______         , X______     , X______   ,
    TO(L_BASE)    , X______     , MS_WHLL    , MS_WHLD      , MS_WHLR    , X______  , X______     , MS_LEFT     , MS_DOWN     , MS_RGHT         , X______     , X______   ,
    X______       , X______     , MS_BTN1    , MS_BTN3      , MS_BTN2    , X______  , X______     , MS_ACL0     , MS_ACL1     , MS_ACL2         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______                , X______     , X______     , X______         , X______     , X______
  ),

  // Arrows
  [L_ARROWS] = LAYOUT_planck_mit(
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , TO(L_BASE),
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , KC_UP           , _______     , _______   ,
    _______       , _______     , _______    , MO(L_LFNS)   , MO(L_LNUMS), _______                , _______     , KC_LEFT     , KC_DOWN         , KC_RGHT     , _______
  ),

  // Gaming
  [L_GAMING] = LAYOUT_planck_mit(
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , TO(L_BASE),
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , KC_UP           , _______     , _______   ,
    _______       , _______     , MO(L_LNAV) , MO(L_LFNS)   , MO(L_LNUMS), _______                , _______     , KC_LEFT     , KC_DOWN         , KC_RIGHT    , _______
  ),

  // Blender
  [L_BLEND] = LAYOUT_planck_mit(
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , MO(L_LFNS) , MO(L_LNUMPAD), MO(L_LOWER), _______                , MO(L_RAISE) , _______     , _______         , _______     , _______
  ),

  // Left Numpad (For Blender)
  [L_LNUMPAD] = LAYOUT_planck_mit(
    X______       , KC_P7       , KC_P8      , KC_P9        , KC_PSLS    , KC_PAST  , KC_PCMM     , X______     , X______     , X______         , X______     , X______   ,
    X______       , KC_P4       , KC_P5      , KC_P6        , KC_PDOT    , KC_PMNS  , KC_NUM      , X______     , X______     , X______         , X______     , TO(L_BASE),
    X______       , KC_P1       , KC_P2      , KC_P3        , KC_P0      , KC_PPLS  , KC_PENT     , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______                , X______     , X______     , X______         , X______     , X______
  ),

  // Left Navigation
  [L_LNAV] = LAYOUT_planck_mit(
    X______       , KC_INS      , KC_HOME    , KC_PGUP      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , KC_DEL      , KC_END     , KC_PGDN      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______                , X______     , X______     , X______         , X______     , X______
  ),

  // GGST (Guilty Gear Strive)
  [L_GGST] = LAYOUT_planck_mit(
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , _______    , _______      , _______    , _______  , _______     , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , _______    , _______      , MO(L_LOWER), _______                , MO(L_RAISE) , _______     , _______         , _______     , _______
  ),

  // Left Nums (Left handed numbers)
  [L_LNUMS] = LAYOUT_planck_mit(
    X______       , KC_1        , KC_2       , KC_3         , KC_4       , KC_5     , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , KC_6        , KC_7       , KC_8         , KC_9       , KC_0     , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______                , X______     , X______     , X______         , X______     , X______
  ),

  // Left FNS
  [L_LFNS] = LAYOUT_planck_mit(
    X______       , KC_F1       , KC_F2      , KC_F3        , KC_F4      , KC_F5    , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , KC_F6       , KC_F7      , KC_F8        , KC_F9      , KC_F10   , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , KC_F11      , KC_F12     , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______                , X______     , X______     , X______         , X______     , X______
  ),

  // Lower
  [L_LOWER] = LAYOUT_planck_mit(
    KC_TILD       , KC_EXLM     , KC_AT      , KC_HASH      , KC_DLR     , KC_PERC  , KC_CIRC     , KC_AMPR     , KC_ASTR     , KC_LPRN         , KC_RPRN     , _______   ,
    KC_DEL        , MS_WHLD     , _______    , MS_WHLU      , _______    , _______  , _______     , KC_UNDS     , KC_PLUS     , KC_LCBR         , KC_RCBR     , KC_PIPE   ,
    MO(L_LOWSHFT) , MS_BTN1     , MS_BTN3    , MS_BTN2      , KC_LSFT    , _______  , _______     , _______     , _______     , KC_HOME         , KC_END      , _______   ,
    _______       , _______     , _______    , _______      , _______    , _______                , _______     , _______     , _______         , _______     , _______
  ),

  // Raise
  [L_RAISE] = LAYOUT_planck_mit(
    KC_GRV        , KC_1        , KC_2       , KC_3         , KC_4       , KC_5     , KC_6        , KC_7        , KC_8        , KC_9            , KC_0        , _______   ,
    KC_DEL        , KC_F1       , KC_F2      , KC_F3        , KC_F4      , KC_F5    , KC_F6       , KC_MINS     , KC_EQL      , KC_LBRC         , KC_RBRC     , KC_BSLS   ,
    _______       , KC_F7       , KC_F8      , KC_F9        , KC_F10     , KC_F11   , KC_F12      , _______     , _______     , _______         , _______     , _______   ,
    _______       , _______     , _______    , MO(L_ACCENTS), _______    , _______                , _______     , _______     , _______         , _______     , _______
  ),

  // Adjust
  [L_ADJUST] = LAYOUT_planck_mit(
    X______       , TO(L_NUMPAD), TO(L_MOUSE), TO(L_GAMING) , TO(L_BLEND), TO(L_FNS), TO(L_GGST)  , TO(L_SNAKE) , TO(L_TICTAC), TO(L_BATTLESHIP), X______     , X______   ,
    TO(L_BASE)    , AU_TOGG     , MU_TOGG    , MU_NEXT      , X______    , KC_BRID  , KC_BRIU     , X______     , X______     , RM_VALU         , RM_VALD     , QK_BOOT   ,
    X______       , UC_NEXT     , X______    , X______      , X______    , LED_LEVEL, AK_CYCLE_BOV, X______     , X______     , AK_BASE_TOGG    , RM_NEXT     , EE_CLR    ,
    KC_MPRV       , KC_MNXT     , KC_MSTP    , KC_MPLY      , _______    , _______                , _______     , KC_MUTE     , KC_VOLD         , KC_VOLU     , KC_MPLY
  ),

  // Accents
  [L_ACCENTS] = LAYOUT_planck_mit(
    X______       , X______     , X______    , UC_ACC_E     , X______    , X______  , UC_ACC_U    , UC_ACC_UU   , UC_ACC_I    , UC_ACC_O        , X______     , _______   ,
    X______       , UC_ACC_A    , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    MO(L_UC_ACC_C), X______     , X______    , X______      , X______    , X______  , UC_ACC_N    , X______     , X______     , UC_EXCL_OPEN    , UC_QUES_OPEN, X______   ,
    X______       , X______     , X______    , X______      , _______    , _______                , _______     , X______     , X______         , X______     , X______
  ),

  // Accents capitalized
  [L_UC_ACC_C] = LAYOUT_planck_mit(
    X______       , X______     , X______    , UC_ACC_E_UP  , X______    , X______  , UC_ACC_U_UP , UC_ACC_UU_UP, UC_ACC_I_UP , UC_ACC_O_UP     , X______     , _______   ,
    X______       , UC_ACC_A_UP , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    _______       , X______     , X______    , X______      , X______    , X______  , UC_ACC_N_UP , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , _______    , _______                , _______     , X______     , X______         , X______     , X______
  ),

  // Low Shift
  [L_LOWSHFT] = LAYOUT_planck_mit(
    X______       , X______     , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , _______   ,
    X______       , X______     , X______    , X______      , X______    , X______  , X______     , UC_EM_DASH  , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , X______    , X______  , X______     , X______     , X______     , X______         , X______     , X______   ,
    X______       , X______     , X______    , X______      , _______    , _______                , _______     , X______     , X______         , X______     , X______
  ),
};

const uint8_t PROGMEM base_overlay_ledmaps[][RGB_MATRIX_LED_COUNT][3] = {
  [BOV_TRANS_FLAG] = {
    C______     , C______     , C______ , C______ , C______, C______, C______, C_CYAN      , C_PINK        , C_WHITE       , C_PINK        , C_CYAN  ,
    C______     , C______     , C______ , C______ , C______, C______, C______, C_CYAN      , C_PINK        , C_WHITE       , C_PINK        , C_CYAN  ,
    C______     , C______     , C______ , C______ , C______, C______, C______, C_CYAN      , C_PINK        , C_WHITE       , C_PINK        , C_CYAN  ,
    C______     , C______     , C______ , C______ , C______, C______         , C_CYAN      , C_PINK        , C_WHITE       , C_PINK        , C_CYAN
  },

  [BOV_LESBIAN_FLAG] = {
    C______     , C______     , C______ , C______ , C______, C______, C______, C_RED_ORANGE, C_ORANGE      , C_WHITE       , C_PURPLE_LIGHT, C_PURPLE,
    C______     , C______     , C______ , C______ , C______, C______, C______, C_RED_ORANGE, C_ORANGE      , C_WHITE       , C_PURPLE_LIGHT, C_PURPLE,
    C______     , C______     , C______ , C______ , C______, C______, C______, C_RED_ORANGE, C_ORANGE      , C_WHITE       , C_PURPLE_LIGHT, C_PURPLE,
    C______     , C______     , C______ , C______ , C______, C______         , C_RED_ORANGE, C_ORANGE      , C_WHITE       , C_PURPLE_LIGHT, C_PURPLE
  },

  [BOV_BIG_TRANS_FLAG] = {
    C_CYAN      , C_CYAN      , C_PINK  , C_PINK  , C_WHITE, C_WHITE, C_WHITE, C_WHITE     , C_PINK        , C_PINK        , C_CYAN        , C_CYAN  ,
    C_CYAN      , C_CYAN      , C_PINK  , C_PINK  , C_WHITE, C_WHITE, C_WHITE, C_WHITE     , C_PINK        , C_PINK        , C_CYAN        , C_CYAN  ,
    C_CYAN      , C_CYAN      , C_PINK  , C_PINK  , C_WHITE, C_WHITE, C_WHITE, C_WHITE     , C_PINK        , C_PINK        , C_CYAN        , C_CYAN  ,
    C_CYAN      , C_CYAN      , C_PINK  , C_PINK  , C_WHITE, C_WHITE         , C_WHITE     , C_PINK        , C_PINK        , C_CYAN        , C_CYAN
  },

  [BOV_BIG_LESBIAN_FLAG] = {
    C_RED_ORANGE, C_RED_ORANGE, C_ORANGE, C_ORANGE, C_WHITE, C_WHITE, C_WHITE, C_WHITE     , C_PURPLE_LIGHT, C_PURPLE_LIGHT, C_PURPLE      , C_PURPLE,
    C_RED_ORANGE, C_RED_ORANGE, C_ORANGE, C_ORANGE, C_WHITE, C_WHITE, C_WHITE, C_WHITE     , C_PURPLE_LIGHT, C_PURPLE_LIGHT, C_PURPLE      , C_PURPLE,
    C_RED_ORANGE, C_RED_ORANGE, C_ORANGE, C_ORANGE, C_WHITE, C_WHITE, C_WHITE, C_WHITE     , C_PURPLE_LIGHT, C_PURPLE_LIGHT, C_PURPLE      , C_PURPLE,
    C_RED_ORANGE, C_RED_ORANGE, C_ORANGE, C_ORANGE, C_WHITE, C_WHITE         , C_WHITE     , C_PURPLE_LIGHT, C_PURPLE_LIGHT, C_PURPLE      , C_PURPLE
  },
};

const uint8_t PROGMEM ledmaps[][RGB_MATRIX_LED_COUNT][3] = {
  [L_TICTAC] = {
    C______     , C______     , C______     , C______     , C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______                   , C______     , C______     , C______     , C______     , C______
  },

  [L_BATTLESHIP] = {
    C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C______     , C______     , C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK,
    C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C______     , C______     , C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK,
    C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C______     , C______     , C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK,
    C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK              , C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK, C_WHITE_DARK
  },

  [L_FNS] = {
    C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C_WHITE     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     ,
    C______     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______                   , C______     , C______     , C______     , C______     , C______
  },

  [L_NUMPAD] = {
    C______     , C______     , C_RED       , C_ORANGE    , C_YELLOW    , C______     , C_PURPLE    , C______     , C_PINK      , C_PINK      , C_PINK      , C_PINK      ,
    C_WHITE     , C______     , C_RED       , C_ORANGE    , C_YELLOW    , C______     , C_MAGENTA   , C______     , C_PINK      , C_PINK      , C_PINK      , C______     ,
    C_PINK      , C______     , C______     , C______     , C______     , C______     , C_RED       , C______     , C_PINK      , C_PINK      , C_PINK      , C_PINK      ,
    C_PINK      , C_PINK      , C______     , C_PINK      , C______     , C______                   , C______     , C_PINK      , C______     , C_PINK      , C______
  },

  [L_MOUSE] = {
    C______     , C______     , C_PINK      , C_PURPLE    , C_PINK      , C______     , C______     , C______     , C_PINK      , C______     , C______     , C______     ,
    C_WHITE     , C______     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C______     , C_PINK      , C_PINK      , C_PINK      , C______     , C______     ,
    C______     , C______     , C_PINK      , C_PINK      , C_PINK      , C______     , C______     , C_WHITE     , C_WHITE     , C_WHITE     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______                   , C______     , C______     , C______     , C______     , C______
  },

  [L_ARROWS] = {
    C______     , C______     , C_PINK      , C_PURPLE    , C_PINK      , C______     , C______     , C______     , C_PINK      , C______     , C______     , C______     ,
    C_WHITE     , C______     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C______     , C_PINK      , C_PINK      , C_PINK      , C______     , C______     ,
    C______     , C______     , C_PINK      , C_PINK      , C_PINK      , C______     , C______     , C_WHITE     , C_WHITE     , C_WHITE     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______                   , C______     , C______     , C______     , C______     , C______
  },

  [L_GAMING] = {
    C______     , C_PURPLE    , C_PINK      , C_PURPLE    , C_WHITE     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C_PINK      , C_PINK      , C_PINK      , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C_WHITE     ,
    C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C_PINK      , C______     , C______     ,
    C______     , C______     , C_WHITE     , C_WHITE     , C_WHITE     , C______                   , C______     , C_PINK      , C_PINK      , C_PINK      , C______
  },

  [L_BLEND] = {
    C_ORANGE    , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C_ORANGE    ,
    C_ORANGE    , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C_ORANGE    ,
    C_ORANGE    , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C_ORANGE    ,
    C_ORANGE    , C_ORANGE    , C_WHITE     , C_WHITE     , C_WHITE     , C_ORANGE                  , C_WHITE     , C_ORANGE    , C_ORANGE    , C_ORANGE    , C_ORANGE
  },

  [L_LNUMPAD] = {
    C______     , C_PINK      , C_PINK      , C_PINK      , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     ,
    C______     , C_PINK      , C_PINK      , C_PINK      , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     , C_WHITE     ,
    C______     , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______                   , C______     , C______     , C______     , C______     , C______
  },

  [L_LNAV] = {
    C______     , C_RED       , C_ORANGE    , C_YELLOW    , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C_RED       , C_ORANGE    , C_YELLOW    , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______                   , C______     , C______     , C______     , C______     , C______
  },

  [L_GGST] = {
    C______     , C______     , C_CYAN      , C_YELLOW    , C_RED       , C_GREEN     , C______     , C_MAGENTA   , C_GREEN     , C_ORANGE    , C_WHITE     , C______     ,
    C______     , C_RED       , C_RED       , C_RED       , C______     , C______     , C______     , C_CYAN      , C_RED       , C_PINK      , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C_WHITE     , C_RED                     , C_WHITE     , C______     , C______     , C______     , C______
  },

  [L_LNUMS] = {
    C______     , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______                   , C______     , C______     , C______     , C______     , C______
  },

  [L_LFNS] = {
    C______     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C______                   , C______     , C______     , C______     , C______     , C______
  },

  [L_LOWER] = {
    C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_WHITE     ,
    C_WHITE     , C_PURPLE    , C______     , C_PURPLE    , C______     , C______     , C______     , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      ,
    C_WHITE     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PINK      , C______     , C______     , C______     , C______     , C_WHITE     , C_WHITE     , C______     ,
    C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_WHITE     , C______                   , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK
  },

  [L_RAISE] = {
    C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_WHITE     ,
    C_WHITE     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_PINK      ,
    C______     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C______     , C______     , C______     , C______     ,
    C_PINK      , C_PINK      , C_PINK      , C_WHITE     , C_PINK      , C_PINK                    , C_WHITE     , C_PINK      , C_PINK      , C_PINK      , C_PINK
  },

  [L_ADJUST] = {
    C______     , C_RED       , C_ORANGE    , C_YELLOW    , C_GREEN     , C_CYAN      , C_BLUE      , C_PURPLE    , C_MAGENTA   , C_PINK      , C______     , C______     ,
    C_WHITE     , C______     , C______     , C______     , C______     , C_YELLOW    , C_YELLOW    , C______     , C______     , C_CYAN      , C_CYAN      , C_WHITE     ,
    C______     , C______     , C______     , C______     , C______     , C_GREEN_LED , C_MAGENTA   , C______     , C______     , C_BLUE      , C_BLUE      , C_PINK      ,
    C_PINK      , C_PINK      , C_PINK      , C_PINK      , C_WHITE     , C______                   , C_WHITE     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE
  },

  [L_ACCENTS] = {
    C______     , C______     , C______     , C_PURPLE    , C______     , C______     , C_PURPLE    , C_PURPLE    , C_PURPLE    , C_PURPLE    , C______     , C_WHITE     ,
    C______     , C_PURPLE    , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C_WHITE     , C______     , C______     , C______     , C______     , C______     , C_PURPLE    , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C_WHITE     , C______     , C_WHITE                   , C______     , C______     , C______     , C______     , C______
  },

  [L_UC_ACC_C] = {
    C______     , C______     , C______     , C_PINK      , C______     , C______     , C_PINK      , C_PINK      , C_PINK      , C_PINK      , C______     , C_WHITE     ,
    C______     , C_PINK      , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C_WHITE     , C______     , C______     , C______     , C______     , C______     , C_PINK      , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C_WHITE     , C______     , C_WHITE                   , C______     , C______     , C______     , C______     , C______
  },

  [L_LOWSHFT] = {
    C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C_WHITE     ,
    C______     , C______     , C______     , C______     , C______     , C______     , C______     , C_PINK      , C______     , C______     , C______     , C______     ,
    C_WHITE     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     , C______     ,
    C______     , C______     , C______     , C______     , C______     , C_WHITE                   , C______     , C______     , C______     , C______     , C______
  },
};
// clang-format on

void cycle_base_overlay(void) {
  user_config.base_overlay = (user_config.base_overlay + 1) % BOV_COUNT;
  eeconfig_update_user(user_config.raw);
}

void set_layer_color(const uint8_t (*maps)[RGB_MATRIX_LED_COUNT][3], int layer,
                     bool ignore_black) {
  for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
    RGB rgb = {
        .r = pgm_read_byte(&maps[layer][i][0]),
        .g = pgm_read_byte(&maps[layer][i][1]),
        .b = pgm_read_byte(&maps[layer][i][2]),
    };
    if (rgb.r == 0 && rgb.g == 0 && rgb.b == 0 && ignore_black) {
      continue;
    }
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
  }
}
#pragma endregion

#pragma region HOOKS
user_config_t user_config;

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
  user_config.raw = eeconfig_read_user();
  keyboard_post_init_snake();
  keyboard_post_init_tictac();
  keyboard_post_init_battleship();
  // debug_enable = true;

  ldprintf("keyboard_post_init_user: debug enabled\n");
}

void eeconfig_init_user(void) {
  user_config.raw = 0;
  user_config.base_overlay = BOV_NONE;
  user_config.base_rgb_enabled = true;
  eeconfig_update_user(user_config.raw);
  rgb_matrix_sethsv(HSV_RED);
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_record_snake(keycode, record)) {
    return false;
  }
  if (!process_record_tictac(keycode, record)) {
    return false;
  }
  if (!process_record_battleship(keycode, record)) {
    return false;
  }
  return process_record_local(keycode, record);
}

void matrix_scan_user(void) { matrix_scan_snake(); }

bool rgb_matrix_indicators_user(void) {
  if (rgb_matrix_indicators_snake()) {
    return false;
  }
  if (rgb_matrix_indicators_tictac()) {
    return false;
  }
  if (rgb_matrix_indicators_battleship()) {
    return false;
  }
  return rgb_matrix_indicators_local();
}
#pragma endregion

#pragma region LOCAL HOOKS
#ifdef AUDIO_ENABLE
extern uint8_t music_mode;
#endif

bool rgb_matrix_indicators_local(void) {
  int layer_int = biton32(layer_state);
  if (layer_int == L_BASE) {
    if (!user_config.base_rgb_enabled) {
      rgb_matrix_set_color_all(0, 0, 0);
    }
    if (user_config.base_overlay > BOV_NONE &&
        user_config.base_overlay < BOV_COUNT) {
      set_layer_color(base_overlay_ledmaps, user_config.base_overlay, true);
    }
  } else if (layer_int > L_BASE && layer_int < L_COUNT) {
    set_layer_color(ledmaps, layer_int, false);
    if (layer_int == L_ADJUST) {
#ifdef AUDIO_ENABLE
      LED_TOGGLE(xy_to_led(1, 1), C_RED, C_RED_DARK, audio_is_on());  // AU_TOGG
      LED_TOGGLE(xy_to_led(2, 1), C_ORANGE, C_ORANGE_DARK,
                 is_music_on());       // MU_TOGG
      RGB music_mode_color = C______;  // MU_NEXT
      switch (music_mode) {
        case MUSIC_MODE_CHROMATIC:
          music_mode_color = (RGB)C_ORANGE;
          break;
        case MUSIC_MODE_GUITAR:
          music_mode_color = (RGB)C_YELLOW;
          break;
        case MUSIC_MODE_VIOLIN:
          music_mode_color = (RGB)C_GREEN;
          break;
        case MUSIC_MODE_MAJOR:
          music_mode_color = (RGB)C_CYAN;
          break;
      }
      rgb_matrix_set_color(xy_to_led(3, 1), music_mode_color.r,
                           music_mode_color.g, music_mode_color.b);
#endif
      RGB unicode_mode_color = C______;  // UC_NEXT
      switch (get_unicode_input_mode()) {
        case UNICODE_MODE_WINCOMPOSE:
          unicode_mode_color = (RGB)C_CYAN;
          break;
        case UNICODE_MODE_MACOS:
          unicode_mode_color = (RGB)C_WHITE;
          break;
        case UNICODE_MODE_LINUX:
          unicode_mode_color = (RGB)C_ORANGE;
          break;
        default:
          unicode_mode_color = (RGB)C_PURPLE;
          break;
      }
      rgb_matrix_set_color(xy_to_led(1, 2), unicode_mode_color.r,
                           unicode_mode_color.g, unicode_mode_color.b);
    } else if (layer_int == L_NUMPAD) {
      led_t led_state = host_keyboard_led_state();
      LED_TOGGLE(xy_to_led(11, 1), C_CYAN, C_CYAN_DARK,
                 led_state.num_lock);  // KC_NUM
      LED_TOGGLE(xy_to_led(2, 2), C_CYAN, C_CYAN_DARK,
                 led_state.num_lock);  // KC_NUM
      LED_TOGGLE(xy_to_led(3, 2), C_BLUE, C_BLUE_DARK,
                 led_state.caps_lock);  // KC_CAPS
      LED_TOGGLE(xy_to_led(4, 2), C_PURPLE, C_PURPLE_DARK,
                 led_state.scroll_lock);  // KC_SCRL
    } else if (layer_int == L_LNUMPAD) {
      led_t led_state = host_keyboard_led_state();
      LED_TOGGLE(xy_to_led(6, 1), C_CYAN, C_CYAN_DARK,
                 led_state.num_lock);  // KC_NUM
    }
  }
  return true;
}

bool process_record_local(uint16_t keycode, keyrecord_t* record) {
  ldprintf("process_record_local:  keycode: %d  (x,y): (%d, %d) \n", keycode,
           record->event.key.col, record->event.key.row);
  if (record->event.pressed) {
    if (get_highest_layer(layer_state) == L_SNAKE && keycode == TO(L_BASE)) {
      snake_stop();
    } else if (keycode == TO(L_SNAKE)) {
      snake_start();
    } else if (get_highest_layer(layer_state) == L_TICTAC &&
               keycode == TO(L_BASE)) {
      tictac_stop();
    } else if (keycode == TO(L_TICTAC)) {
      tictac_start();
    } else if (get_highest_layer(layer_state) == L_BATTLESHIP &&
               keycode == TO(L_BASE)) {
      battleship_stop();
    } else if (keycode == TO(L_BATTLESHIP)) {
      battleship_start();
    } else {
      switch (keycode) {
        case AK_BASE_TOGG:
          user_config.base_rgb_enabled = !user_config.base_rgb_enabled;
          eeconfig_update_user(user_config.raw);
          return false;
        case AK_CYCLE_BOV:
          cycle_base_overlay();
          return false;
      }
    }
  }
  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, L_LOWER, L_RAISE, L_ADJUST);
}
#pragma endregion

#include "atlinx.h"
#include "eeconfig.h"
#include "keycodes.h"
#include "rgb_matrix.h"

typedef union {
  uint32_t raw;
  struct {
    uint8_t     base_overlay :8;
  };
} user_config_t;

user_config_t user_config;

typedef enum {
    PKC_CYCLE_BOV = SAFE_RANGE,

    PKC_E_ACC,
    PKC_U_ACC,
    PKC_U_ACC2,
    PKC_I_ACC,
    PKC_O_ACC,
    PKC_A_ACC,
    PKC_N_ACC,
    PKC_U_EXC,
    PKC_U_QUES,
    PKC_E_ACC_C,
    PKC_U_ACC_C,
    PKC_U_ACC2_C,
    PKC_I_ACC_C,
    PKC_O_ACC_C,
    PKC_A_ACC_C,
    PKC_N_ACC_C,
    PKC_EMDASH,

    PKC_COUNT
} PlanckKeycode;

typedef enum {
    L_BASE,
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
    L_ACC_C,
    L_LOWSHFT,

    L_COUNT,
} PlanckLayer;

typedef enum {
    BOV_NONE,
    BOV_TRANS_FLAG,
    BOV_LESBIAN_FLAG,

    BOV_COUNT,
} BaseOverlay;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Base
    [L_BASE] = LAYOUT_planck_mit(
        KC_TAB       , KC_Q        , KC_W       , KC_E         , KC_R       , KC_T     , KC_Y         , KC_U        , KC_I       , KC_O       , KC_P      , KC_BSPC   ,
        KC_ESC       , KC_A        , KC_S       , KC_D         , KC_F       , KC_G     , KC_H         , KC_J        , KC_K       , KC_L       , KC_SCLN   , KC_QUOT   ,
        KC_LSFT      , KC_Z        , KC_X       , KC_C         , KC_V       , KC_B     , KC_N         , KC_M        , KC_COMM    , KC_DOT     , KC_SLSH   , KC_ENT    ,
        KC_LCTL      , KC_LALT     , KC_F24     , KC_LGUI      , MO(L_LOWER), KC_SPC                  , MO(L_RAISE) , KC_LEFT    , KC_DOWN    , KC_UP     , KC_RGHT
    ),

    // Function Keys
    [L_FNS] = LAYOUT_planck_mit(
        X______      , X______     , X______    , X______      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        TO(L_BASE)   , KC_F13      , KC_F14     , KC_F15       , KC_F16     , KC_F17   , KC_F18       , X______     , X______    , X______    , X______   , X______   ,
        X______      , KC_F19      , KC_F20     , KC_F21       , KC_F22     , KC_F23   , KC_F24       , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______                 , X______     , X______    , X______    , X______   , X______
    ),

    // Numpad
    [L_NUMPAD] = LAYOUT_planck_mit(
        X______      , X______     , KC_INS     , KC_HOME      , KC_PGUP    , X______  , KC_PAUS      , X______     , KC_P7      , KC_P8      , KC_P9     , KC_BSPC   ,
        TO(L_BASE)   , X______     , KC_DEL     , KC_END       , KC_PGDN    , X______  , KC_PSCR      , X______     , KC_P4      , KC_P5      , KC_P6     , KC_NUM    ,
        KC_LSFT      , X______     , KC_NUM     , KC_CAPS      , KC_SCRL    , X______  , KC_SYRQ      , X______     , KC_P1      , KC_P2      , KC_P3     , KC_PENT   ,
        KC_LCTL      , KC_LALT     , X______    , KC_LGUI      , X______    , X______                 , X______     , KC_P0      , X______    , KC_PDOT   , X______
    ),

    // Mouse
    [L_MOUSE] = LAYOUT_planck_mit(
        X______      , X______     , MS_BTN4    , MS_WHLU      , MS_BTN5    , X______  , X______      , X______     , MS_UP      , X______    , X______   , X______   ,
        TO(L_BASE)   , X______     , MS_WHLL    , MS_WHLD      , MS_WHLR    , X______  , X______      , MS_LEFT     , MS_DOWN    , MS_RGHT    , X______   , X______   ,
        X______      , X______     , MS_BTN1    , MS_BTN3      , MS_BTN2    , X______  , X______      , MS_ACL0     , MS_ACL1    , MS_ACL2    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______                 , X______     , X______    , X______    , X______   , X______
    ),

    // Arrows
    [L_ARROWS] = LAYOUT_planck_mit(
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , _______   ,
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , TO(L_BASE),
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , KC_UP      , _______   , _______   ,
        _______      , _______     , _______    , MO(L_LFNS)   , MO(L_LNUMS), _______                 , _______     , KC_LEFT    , KC_DOWN    , KC_RGHT   , _______
    ),

    // Gaming
    [L_GAMING] = LAYOUT_planck_mit(
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , _______   ,
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , TO(L_BASE),
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , KC_UP      , _______   , _______   ,
        _______      , _______     , MO(L_LNAV) , MO(L_LFNS)   , MO(L_LNUMS), _______                 , _______     , KC_LEFT    , KC_DOWN    , KC_RIGHT  , _______
    ),

    // Blender
    [L_BLEND] = LAYOUT_planck_mit(
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , _______   ,
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , _______   ,
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , _______   ,
        _______      , _______     , MO(L_LFNS) , MO(L_LNUMPAD), MO(L_LOWER), _______                 , MO(L_RAISE) , _______    , _______    , _______   , _______
    ),

    // Left Numpad (For Blender)
    [L_LNUMPAD] = LAYOUT_planck_mit(
        X______      , KC_P7       , KC_P8      , KC_P9        , KC_PSLS    , KC_PAST  , KC_PCMM      , X______     , X______    , X______    , X______   , X______   ,
        X______      , KC_P4       , KC_P5      , KC_P6        , KC_PDOT    , KC_PMNS  , KC_NUM       , X______     , X______    , X______    , X______   , TO(L_BASE),
        X______      , KC_P1       , KC_P2      , KC_P3        , KC_P0      , KC_PPLS  , KC_PENT      , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______                 , X______     , X______    , X______    , X______   , X______
    ),

    // Left Navigation
    [L_LNAV] = LAYOUT_planck_mit(
        X______      , KC_INS      , KC_HOME    , KC_PGUP      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , KC_DEL      , KC_END     , KC_PGDN      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______                 , X______     , X______    , X______    , X______   , X______
    ),

    // GGST (Guilty Gear Strive)
    [L_GGST] = LAYOUT_planck_mit(
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , _______   ,
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , _______   ,
        _______      , _______     , _______    , _______      , _______    , _______  , _______      , _______     , _______    , _______    , _______   , _______   ,
        _______      , _______     , _______    , _______      , MO(L_LOWER), _______                 , MO(L_RAISE) , _______    , _______    , _______   , _______
    ),

    // Left Nums (Left handed numbers)
    [L_LNUMS] = LAYOUT_planck_mit(
        X______      , KC_1        , KC_2       , KC_3         , KC_4       , KC_5     , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , KC_6        , KC_7       , KC_8         , KC_9       , KC_0     , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______                 , X______     , X______    , X______    , X______   , X______
    ),

    // Left FNS
    [L_LFNS] = LAYOUT_planck_mit(
        X______      , KC_F1       , KC_F2      , KC_F3        , KC_F4      , KC_F5    , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , KC_F6       , KC_F7      , KC_F8        , KC_F9      , KC_F10   , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , KC_F11      , KC_F12     , X______      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______                 , X______     , X______    , X______    , X______   , X______
    ),

    // Lower
    [L_LOWER] = LAYOUT_planck_mit(
        KC_TILD      , KC_EXLM     , KC_AT      , KC_HASH      , KC_DLR     , KC_PERC  , KC_CIRC      , KC_AMPR     , KC_ASTR    , KC_LPRN    , KC_RPRN   , X______   ,
        KC_DEL       , MS_WHLD     , X______    , MS_WHLU      , X______    , X______  , X______      , KC_UNDS     , KC_PLUS    , KC_LCBR    , KC_RCBR   , KC_PIPE   ,
        MO(L_LOWSHFT), MS_BTN1     , MS_BTN3    , MS_BTN2      , KC_LSFT    , X______  , X______      , X______     , X______    , KC_HOME    , KC_END    , X______   ,
        X______      , X______     , X______    , X______      , _______    , _______                 , _______     , X______    , X______    , X______   , X______
    ),

    // Raise
    [L_RAISE] = LAYOUT_planck_mit(
        KC_GRV       , KC_1        , KC_2       , KC_3         , KC_4       , KC_5     , KC_6         , KC_7        , KC_8       , KC_9       , KC_0      , X______   ,
        KC_DEL       , KC_F1       , KC_F2      , KC_F3        , KC_F4      , KC_F5    , KC_F6        , KC_MINS     , KC_EQL     , KC_LBRC    , KC_RBRC   , KC_BSLS   ,
        X______      , KC_F7       , KC_F8      , KC_F9        , KC_F10     , KC_F11   , KC_F12       , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , MO(L_ACCENTS), _______    , _______                 , _______     , X______    , X______    , X______   , X______
    ),

    // Adjust
    [L_ADJUST] = LAYOUT_planck_mit(
        X______      , TO(L_NUMPAD), TO(L_MOUSE), TO(L_GAMING) , TO(L_BLEND), TO(L_FNS), TO(L_GGST)   , X______     , X______    , X______    , X______   , X______   ,
        TO(L_BASE)   , AU_TOGG     , MU_TOGG    , MU_NEXT      , X______    , KC_BRID  , KC_BRIU      , X______     , X______    , RM_VALU    , RM_VALD   , QK_BOOT   ,
        X______      , X______     , X______    , X______      , X______    , LED_LEVEL, PKC_CYCLE_BOV, X______     , X______    , RM_TOGG    , RM_NEXT   , X______   ,
        KC_MPRV      , KC_MNXT     , KC_MSTP    , KC_MPLY      , _______    , _______                 , _______     , KC_MUTE    , KC_VOLD    , KC_VOLU   , KC_MPLY
    ),

    // Accents
    [L_ACCENTS] = LAYOUT_planck_mit(
        X______      , X______     , X______    , PKC_E_ACC    , X______    , X______  , PKC_U_ACC    , PKC_U_ACC2  , PKC_I_ACC  , PKC_O_ACC  , X______   , _______   ,
        X______      , PKC_A_ACC   , X______    , X______      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        MO(L_ACC_C)  , X______     , X______    , X______      , X______    , X______  , PKC_N_ACC    , X______     , X______    , PKC_U_EXC  , PKC_U_QUES, X______   ,
        X______      , X______     , X______    , X______      , _______    , _______                 , _______     , X______    , X______    , X______   , X______
    ),

    // Accents capitalized
    [L_ACC_C] = LAYOUT_planck_mit(
        X______      , X______     , X______    , PKC_E_ACC_C  , X______    , X______  , PKC_U_ACC_C  , PKC_U_ACC2_C, PKC_I_ACC_C, PKC_O_ACC_C, X______   , _______   ,
        X______      , PKC_A_ACC_C , X______    , X______      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        _______      , X______     , X______    , X______      , X______    , X______  , PKC_N_ACC_C  , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , _______    , _______                 , _______     , X______    , X______    , X______   , X______
    ),

    // Low Shift
    [L_LOWSHFT] = LAYOUT_planck_mit(
        X______      , X______     , X______    , X______      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , _______   ,
        X______      , X______     , X______    , X______      , X______    , X______  , X______      , PKC_EMDASH  , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , X______    , X______  , X______      , X______     , X______    , X______    , X______   , X______   ,
        X______      , X______     , X______    , X______      , _______    , _______                 , _______     , X______    , X______    , X______   , X______
    ),
};

const uint8_t PROGMEM base_overlay_ledmaps[][RGB_MATRIX_LED_COUNT][3] = {
    [BOV_TRANS_FLAG] = {
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C_CYAN   , C_PINK        , C_WHITE , C_PINK        , C_CYAN  ,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C_CYAN   , C_PINK        , C_WHITE , C_PINK        , C_CYAN  ,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C_CYAN   , C_PINK        , C_WHITE , C_PINK        , C_CYAN  ,
        C______ , C______ , C______ , C______ , C______ , C______               , C_CYAN   , C_PINK        , C_WHITE , C_PINK        , C_CYAN
    },

    [BOV_LESBIAN_FLAG] = {
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C_ORANGE , C_ORANGE_LIGHT, C_WHITE , C_PURPLE_LIGHT, C_PURPLE,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C_ORANGE , C_ORANGE_LIGHT, C_WHITE , C_PURPLE_LIGHT, C_PURPLE,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C_ORANGE , C_ORANGE_LIGHT, C_WHITE , C_PURPLE_LIGHT, C_PURPLE,
        C______ , C______ , C______ , C______ , C______ , C______               , C_ORANGE , C_ORANGE_LIGHT, C_WHITE , C_PURPLE_LIGHT, C_PURPLE
    },
};

const uint8_t PROGMEM ledmaps[][RGB_MATRIX_LED_COUNT][3] = {
    [L_FNS] = {
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C_WHITE , C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE   , C_PURPLE , C______  , C______ , C______ , C______ , C______ ,
        C______ , C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE   , C_PURPLE , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______               , C______  , C______ , C______ , C______ , C______
    },

    [L_NUMPAD] = {
        C______ , C______ , C_RED   , C_ORANGE, C_YELLOW, C______    , C_PURPLE , C______  , C_PINK  , C_PINK  , C_PINK  , C_PINK  ,
        C_WHITE , C______ , C_RED   , C_ORANGE, C_YELLOW, C______    , C_MAGENTA, C______  , C_PINK  , C_PINK  , C_PINK  , C______ ,
        C_PINK  , C______ , C______ , C______ , C______ , C______    , C_RED    , C______  , C_PINK  , C_PINK  , C_PINK  , C_PINK  ,
        C_PINK  , C_PINK  , C______ , C_PINK  , C______ , C______               , C______  , C_PINK  , C______ , C_PINK  , C______
    },

    [L_MOUSE] = {
        C______ , C______ , C_PINK  , C_PURPLE, C_PINK  , C______    , C______  , C______  , C_PINK  , C______ , C______ , C______ ,
        C_WHITE , C______ , C_PURPLE, C_PURPLE, C_PURPLE, C______    , C______  , C_PINK   , C_PINK  , C_PINK  , C______ , C______ ,
        C______ , C______ , C_PINK  , C_PINK  , C_PINK  , C______    , C______  , C_WHITE  , C_WHITE , C_WHITE , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______               , C______  , C______ , C______ , C______ , C______
    },

    [L_ARROWS] = {
        C______ , C______ , C_PINK  , C_PURPLE, C_PINK  , C______    , C______  , C______  , C_PINK  , C______ , C______ , C______ ,
        C_WHITE , C______ , C_PURPLE, C_PURPLE, C_PURPLE, C______    , C______  , C_PINK   , C_PINK  , C_PINK  , C______ , C______ ,
        C______ , C______ , C_PINK  , C_PINK  , C_PINK  , C______    , C______  , C_WHITE  , C_WHITE , C_WHITE , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______               , C______  , C______ , C______ , C______ , C______
    },

    [L_GAMING] = {
        C______ , C_PURPLE, C_PINK  , C_PURPLE, C_WHITE , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C_PINK  , C_PINK  , C_PINK  , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C_WHITE ,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C_PINK  , C______ , C______ ,
        C______ , C______ , C_WHITE , C_WHITE , C_WHITE , C______               , C______  , C_PINK  , C_PINK  , C_PINK  , C______
    },

    [L_BLEND] = {
        C_ORANGE, C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C_ORANGE,
        C_ORANGE, C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C_ORANGE,
        C_ORANGE, C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C_ORANGE,
        C_ORANGE, C_ORANGE, C_WHITE , C_WHITE , C_WHITE , C_ORANGE              , C_WHITE  , C_ORANGE, C_ORANGE, C_ORANGE, C_ORANGE
    },

    [L_LNUMPAD] = {
        C______ , C_PINK  , C_PINK  , C_PINK  , C_PURPLE, C_PURPLE   , C_PURPLE , C______  , C______ , C______ , C______ , C______ ,
        C______ , C_PINK  , C_PINK  , C_PINK  , C_PURPLE, C_PURPLE   , C______  , C______  , C______ , C______ , C______ , C_WHITE ,
        C______ , C_PINK  , C_PINK  , C_PINK  , C_PINK  , C_PURPLE   , C_PURPLE , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______               , C______  , C______ , C______ , C______ , C______
    },

    [L_LNAV] = {
        C______ , C_RED   , C_ORANGE, C_YELLOW, C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C_RED   , C_ORANGE, C_YELLOW, C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______               , C______  , C______ , C______ , C______ , C______
    },

    [L_GGST] = {
        C______ , C______ , C_CYAN  , C_YELLOW, C_RED   , C_GREEN    , C______  , C_MAGENTA, C_GREEN , C_ORANGE, C_WHITE , C______ ,
        C______ , C_RED   , C_RED   , C_RED   , C______ , C______    , C______  , C_CYAN   , C_RED   , C_PINK  , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C_WHITE , C_RED                 , C_WHITE  , C______ , C______ , C______ , C______
    },

    [L_LNUMS] = {
        C______ , C_PINK  , C_PINK  , C_PINK  , C_PINK  , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C_PINK  , C_PINK  , C_PINK  , C_PINK  , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______               , C______  , C______ , C______ , C______ , C______
    },

    [L_LFNS] = {
        C______ , C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE   , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE   , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C_PURPLE, C_PURPLE, C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C______               , C______  , C______ , C______ , C______ , C______
    },

    [L_LOWER] = {
        C_PINK  , C_PINK  , C_PINK  , C_PINK  , C_PINK  , C_PINK     , C_PINK   , C_PINK   , C_PINK  , C_PINK  , C_PINK  , C_WHITE ,
        C_WHITE , C_PURPLE, C______ , C_PURPLE, C______ , C______    , C______  , C_PINK   , C_PINK  , C_PINK  , C_PINK  , C_PINK  ,
        C_WHITE , C_PURPLE, C_PURPLE, C_PURPLE, C_PINK  , C______    , C______  , C______  , C______ , C_WHITE , C_WHITE , C______ ,
        C_PINK  , C_PINK  , C_PINK  , C_PINK  , C_WHITE , C______               , C_PINK   , C_PINK  , C_PINK  , C_PINK  , C_PINK
    },

    [L_RAISE] = {
        C_PINK  , C_PINK  , C_PINK  , C_PINK  , C_PINK  , C_PINK     , C_PINK   , C_PINK   , C_PINK  , C_PINK  , C_PINK  , C_WHITE ,
        C_WHITE , C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE   , C_PURPLE , C_PINK   , C_PINK  , C_PINK  , C_PINK  , C_PINK  ,
        C______ , C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE   , C_PURPLE , C______  , C______ , C______ , C______ , C______ ,
        C_PINK  , C_PINK  , C_PINK  , C_WHITE , C_PINK  , C_PINK                , C_WHITE  , C_PINK  , C_PINK  , C_PINK  , C_PINK
    },

    [L_ADJUST] = {
        C______ , C_RED   , C_ORANGE, C_YELLOW, C_GREEN , C_CYAN     , C_BLUE   , C______  , C______ , C______ , C______ , C______ ,
        C_WHITE , C______ , C______ , C______ , C______ , C_YELLOW   , C_YELLOW , C______  , C______ , C_CYAN  , C_CYAN  , C_WHITE ,
        C______ , C______ , C______ , C______ , C______ , C_GREEN_LED, C_MAGENTA  , C______  , C______ , C_BLUE  , C_BLUE  , C______ ,
        C_PINK  , C_PINK  , C_PINK  , C_PINK  , C_WHITE , C______               , C_WHITE  , C_PURPLE, C_PURPLE, C_PURPLE, C_PURPLE
    },

    [L_ACCENTS] = {
        C______ , C______ , C______ , C_PURPLE, C______ , C______    , C_PURPLE , C_PURPLE , C_PURPLE, C_PURPLE, C______ , C_WHITE ,
        C______ , C_PURPLE, C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C_WHITE , C______ , C______ , C______ , C______ , C______    , C_PURPLE , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C_WHITE , C______ , C_WHITE               , C______  , C______ , C______ , C______ , C______
    },

    [L_ACC_C] = {
        C______ , C______ , C______ , C_PINK  , C______ , C______    , C_PINK   , C_PINK   , C_PINK  , C_PINK  , C______ , C_WHITE ,
        C______ , C_PINK  , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C_WHITE , C______ , C______ , C______ , C______ , C______    , C_PINK   , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C_WHITE , C______ , C_WHITE               , C______  , C______ , C______ , C______ , C______
    },

    [L_LOWSHFT] = {
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C_WHITE ,
        C______ , C______ , C______ , C______ , C______ , C______    , C______  , C_PINK   , C______ , C______ , C______ , C______ ,
        C_WHITE , C______ , C______ , C______ , C______ , C______    , C______  , C______  , C______ , C______ , C______ , C______ ,
        C______ , C______ , C______ , C______ , C______ , C_WHITE               , C______  , C______ , C______ , C______ , C______
    },
};
// clang-format on

extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
    user_config.raw = eeconfig_read_user();
}

void eeconfig_init_user(void) {
    user_config.raw = 0;
    eeconfig_update_user(user_config.raw);
}

void cycle_base_overlay(void) {
    user_config.base_overlay = (user_config.base_overlay + 1) % BOV_COUNT;
    eeconfig_update_user(user_config.raw);
}

void set_layer_color(const uint8_t (*maps)[RGB_MATRIX_LED_COUNT][3], int layer, bool ignore_black) {
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

/**
 * @brief Toggles the color of an LED based on a boolean function.
 * @param LED The LED index to toggle.
 * @param TRUE_COLOR The color to use when the boolean function returns true.
 * @param FALSE_COLOR The color to use when the boolean function returns false.
 * @param IS_ON_EXPR The boolean expression to evaluate.
 */
#define LED_TOGGLE(LED, TRUE_COLOR, FALSE_COLOR, IS_ON_EXPR)  \
    do {                                                      \
        RGB color = (RGB)TRUE_COLOR;                          \
        if (!IS_ON_EXPR) {                                    \
            color = (RGB)FALSE_COLOR;                         \
        }                                                     \
        rgb_matrix_set_color(LED, color.r, color.g, color.b); \
    } while (0)

extern uint8_t music_mode;

bool rgb_matrix_indicators_user(void) {
    int layer_int = biton32(layer_state);
    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0, 0, 0);
    }
    if (layer_int == L_BASE) {
        if (user_config.base_overlay > BOV_NONE && user_config.base_overlay < BOV_COUNT) {
            set_layer_color(base_overlay_ledmaps, user_config.base_overlay, true);
        }
    } else if (layer_int > L_BASE && layer_int < L_COUNT) {
        set_layer_color(ledmaps, layer_int, false);
        if (layer_int == L_ADJUST) {
#ifdef AUDIO_ENABLE
            LED_TOGGLE(13, C_RED, C_RED_DARK, audio_is_on());
            LED_TOGGLE(14, C_ORANGE, C_ORANGE_DARK, is_music_on());
            RGB mu_mode_color = C______;
            switch (music_mode) {
                case MUSIC_MODE_CHROMATIC:
                    mu_mode_color = (RGB)C_ORANGE;
                    break;
                case MUSIC_MODE_GUITAR:
                    mu_mode_color = (RGB)C_YELLOW;
                    break;
                case MUSIC_MODE_VIOLIN:
                    mu_mode_color = (RGB)C_GREEN;
                    break;
                case MUSIC_MODE_MAJOR:
                    mu_mode_color = (RGB)C_CYAN;
                    break;
            }
            rgb_matrix_set_color(15, mu_mode_color.r, mu_mode_color.g, mu_mode_color.b);
#endif
        } else if (layer_int == L_NUMPAD) {
            led_t led_state = host_keyboard_led_state();
            LED_TOGGLE(23, C_CYAN, C_CYAN_DARK, led_state.num_lock);
            LED_TOGGLE(26, C_CYAN, C_CYAN_DARK, led_state.num_lock);
            LED_TOGGLE(27, C_BLUE, C_BLUE_DARK, led_state.caps_lock);
            LED_TOGGLE(28, C_PURPLE, C_PURPLE_DARK, led_state.scroll_lock);
        } else if (layer_int == L_LNUMPAD) {
            led_t led_state = host_keyboard_led_state();
            LED_TOGGLE(18, C_CYAN, C_CYAN_DARK, led_state.num_lock);
        }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case PKC_CYCLE_BOV:
            if (record->event.pressed) {
                cycle_base_overlay();
            }
            break;
        case PKC_E_ACC:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_3) SS_TAP(X_KP_3)));
            }
            break;
        case PKC_U_ACC:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_5) SS_TAP(X_KP_2)));
            }
            break;
        case PKC_U_ACC2:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_5) SS_TAP(X_KP_0)));
            }
            break;
        case PKC_I_ACC:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_3) SS_TAP(X_KP_7)));
            }
            break;
        case PKC_O_ACC:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_4) SS_TAP(X_KP_3)));
            }
            break;
        case PKC_A_ACC:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_2) SS_TAP(X_KP_5)));
            }
            break;
        case PKC_N_ACC:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_4) SS_TAP(X_KP_1)));
            }
            break;
        case PKC_U_EXC:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_6) SS_TAP(X_KP_1)));
            }
            break;
        case PKC_U_QUES:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_9) SS_TAP(X_KP_1)));
            }
            break;
        case PKC_E_ACC_C:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_0) SS_TAP(X_KP_1)));
            }
            break;
        case PKC_U_ACC_C:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_2) SS_TAP(X_KP_0)));
            }
            break;
        case PKC_U_ACC2_C:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_1) SS_TAP(X_KP_8)));
            }
            break;
        case PKC_I_ACC_C:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_0) SS_TAP(X_KP_5)));
            }
            break;
        case PKC_O_ACC_C:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_1) SS_TAP(X_KP_1)));
            }
            break;
        case PKC_A_ACC_C:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_9) SS_TAP(X_KP_3)));
            }
            break;
        case PKC_N_ACC_C:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_0) SS_TAP(X_KP_9)));
            }
            break;
        case PKC_EMDASH:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_5) SS_TAP(X_KP_1)));
            }
            break;
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, L_LOWER, L_RAISE, L_ADJUST);
}

/* Copyright (C) 2023 jonylee@hfd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "action_layer.h"
#include "quantum.h"
#include QMK_KEYBOARD_H

enum planck_layers {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST,
    _GGST,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base
    * ,-----------------------------------------------------------------------------------.
    * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * | Ctrl | Alt  | F24  | Wind |Lower |    Space    |Raise | Left | Down |  Up  |Right |
    * `-----------------------------------------------------------------------------------'
    */
    [_BASE] = LAYOUT_planck_mit(
        KC_TAB,  KC_Q,    KC_W,   KC_E,    KC_R,     KC_T,    KC_Y,   KC_U,      KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_ESC,  KC_A,    KC_S,   KC_D,    KC_F,     KC_G,    KC_H,   KC_J,      KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,   KC_C,    KC_V,     KC_B,    KC_N,   KC_M,      KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
        KC_LCTL, KC_LALT, KC_F24, KC_LGUI, TL_LOWR,      KC_SPC,      TL_UPPR,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),

    /* Lower
    * ,-----------------------------------------------------------------------------------.
    * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 | TO(B)|      |      |      |      |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |      |      |      |      |      |             |      |      |      |      |      |
    * `-----------------------------------------------------------------------------------'
    */
    [_LOWER] = LAYOUT_planck_mit(
        KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,   KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
        KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS,   KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
        _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  TO(_BASE), _______, _______, _______,  _______,
        _______, _______, _______, _______, _______,     _______,      _______,   _______, _______, _______, _______
    ),

    /* Raise
    * ,-----------------------------------------------------------------------------------.
    * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |  \   |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 | TO(B)|      |      |      |      |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |      |      |      |      |      |             |      |      |      |      |      |
    * `-----------------------------------------------------------------------------------'
    */
    [_RAISE] = LAYOUT_planck_mit(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,      KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS,   KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
        _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  TO(_BASE), _______, _______, _______, _______,
        _______, _______, _______, _______, _______,     _______,      _______,   _______, _______, _______, _______
    ),

    /* Adjust
    * ,-----------------------------------------------------------------------------------.
    * |      |      |      |      |      |      |      |      |      |      |      |      |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * |      |      |      |      |      |      |      |LM_TGG| LM_UP|LM_DWN|      | Reset|
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * |      |      |      |      |      |      |      |      |      |      |      |      |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
    * `-----------------------------------------------------------------------------------'
    */
    [_ADJUST] = LAYOUT_planck_mit(
        _______, _______, _______, _______, _______, _______, _______, TO(_GGST), _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, LM_TOGG,  LM_BRIU, LM_BRID, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______,
        _______, _______, _______, _______, _______,     _______,      _______,  KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
    ),

    /* Guilty Gear Strive
    * ,-----------------------------------------------------------------------------------.
    * |      |      | Blue |  Yel |  Red | Gree |      | Pink | Gree | Orng | White|      |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * |      |  Red |  Red |  Red |      |      |      | Blue | Red  | Pink |      |      |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * |      |      |      |      |      |      |      |      |      |      |      |      |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |      |      |      |      | LOWER|     Red     | RAISE|      |      |      |      |
    * `-----------------------------------------------------------------------------------'
    */
    [_GGST] = LAYOUT_planck_mit(
        _______, _______, _______, _______, _______,    _______, _______, _______,    _______, _______, _______, _______,
        _______, _______, _______, _______, _______,    _______, _______, _______,    _______, _______, _______, _______,
        _______, _______, _______, _______, _______,    _______, _______, _______,    _______, _______, _______, _______,
        _______, _______, _______, _______, MO(_LOWER),      _______,     MO(_RAISE), _______, _______, _______, _______
    ),
};
// clang-format on

void set_layer_key_color(uint8_t layer, uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];

            if (index != NO_LED && keymap_key_to_keycode(layer, (keypos_t){col, row}) > KC_TRNS) {
                rgb_matrix_set_color(index, red, green, blue);
            }
        }
    }
}

void rgb_matrix_set_color_rc(int row, int col, uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_set_color(g_led_config.matrix_co[row][col], red, green, blue);
}

#define RGB_GGST_BLUE 0x00, 0xB7, 0xFF
#define RGB_GGST_YELLOW 0xFF, 0xEA, 0x00
#define RGB_GGST_ORANGE 0xFF, 0x95, 0x00
#define RGB_GGST_GREEN 0x31, 0xFF, 0x00

#define RGB_GGST_P 0xFF, 0x00, 0xEA
#define RGB_GGST_S RGB_GGST_GREEN
#define RGB_GGST_D RGB_GGST_ORANGE
#define RGB_GGST_SPRINT RGB_WHITE
#define RGB_GGST_K RGB_GGST_BLUE
#define RGB_GGST_HS RGB_RED
#define RGB_GGST_RC 0xFF, 0x00, 0x80

void set_ggst_key_color(void) {
    // Movement keys
    rgb_matrix_set_color_rc(1, 1, RGB_RED);
    rgb_matrix_set_color_rc(1, 2, RGB_RED);
    rgb_matrix_set_color_rc(1, 3, RGB_RED);
    rgb_matrix_set_color_rc(3, 5, RGB_RED);

    // Top row
    rgb_matrix_set_color_rc(0, 2, RGB_GGST_BLUE);
    rgb_matrix_set_color_rc(0, 3, RGB_GGST_YELLOW);
    rgb_matrix_set_color_rc(0, 4, RGB_RED);
    rgb_matrix_set_color_rc(0, 5, RGB_GGST_GREEN);

    // Attack buttons
    rgb_matrix_set_color_rc(0, 7, RGB_GGST_P);
    rgb_matrix_set_color_rc(0, 8, RGB_GGST_S);
    rgb_matrix_set_color_rc(0, 9, RGB_GGST_D);
    rgb_matrix_set_color_rc(0, 10, RGB_GGST_SPRINT);

    rgb_matrix_set_color_rc(1, 7, RGB_GGST_K);
    rgb_matrix_set_color_rc(1, 8, RGB_GGST_HS);
    rgb_matrix_set_color_rc(1, 9, RGB_GGST_RC);

    // Exit indicator
    rgb_matrix_set_color_rc(3, 7, RGB_WHITE);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (layer_state_is(_ADJUST)) {
        set_layer_key_color(_ADJUST, RGB_WHITE);
        rgb_matrix_set_color_rc(0, 7, RGB_RED);
        rgb_matrix_set_color_rc(3, 4, RGB_WHITE);
        rgb_matrix_set_color_rc(3, 7, RGB_WHITE);
    } else if (layer_state_is(_LOWER)) {
        set_layer_key_color(_LOWER, RGB_ORANGE);
        rgb_matrix_set_color_rc(2, 7, RGB_WHITE);
        rgb_matrix_set_color_rc(3, 4, RGB_WHITE);
    } else if (layer_state_is(_RAISE)) {
        set_layer_key_color(_RAISE, RGB_ORANGE);
        rgb_matrix_set_color_rc(2, 7, RGB_WHITE);
        rgb_matrix_set_color_rc(3, 7, RGB_WHITE);
    } else {
        switch (get_highest_layer(layer_state)) {
            case _GGST:
                set_ggst_key_color();
                break;
        }
    }
    return false;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state                  = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
    uint16_t highest_layer = get_highest_layer(state);
    if (highest_layer == _BASE) {
        rgb_matrix_mode_noeeprom(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
        rgb_matrix_sethsv_noeeprom(HSV_RED);
    } else {
        rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
        rgb_matrix_sethsv_noeeprom(HSV_BLACK);
    }
    return state;
}

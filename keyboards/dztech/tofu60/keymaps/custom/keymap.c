/* Copyright 2022 DZTECH <moyi4681@Live.cn>
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

#include QMK_KEYBOARD_H

enum custom_keycodes {
	DEBUG = QK_KB_0,
	RESTART_DEBUG,
	STOP_DEBUG,
    KC_TERM
};

enum dactyl_layers {
    _DEFAULTS = 0,
    _WIN = 0,
    _GAME,
    _MAC,
    _RAISE
};

#define LT_SPC_RS LT(_RAISE,KC_SPC) 

#define MT_F_GUI MT(MOD_LGUI,KC_F)   
#define MT_D_ALT MT(MOD_LALT,KC_D) 
#define MT_S_CTL MT(MOD_LCTL,KC_S)
#define MT_A_SFT MT(MOD_LSFT,KC_A)

#define ST_DBG STOP_DEBUG
#define RS_DBG RESTART_DEBUG
#define DB_DBG DEBUG

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_WIN] = LAYOUT_all(
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL, KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_LCTL, KC_A,    MT_S_CTL,   MT_D_ALT,   MT_F_GUI,   KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, TD(0),
        KC_NO, KC_LGUI, KC_LALT,                                     LT_SPC_RS,                    MO(3), MO(3),  KC_RGUI, KC_NO
    ),
    [_GAME] = LAYOUT_all(
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL, KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_TRNS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, TO(0),
        KC_NO, KC_NO, KC_LALT,                                     KC_SPC,                    MO(3), MO(3),  KC_TRNS, KC_NO
    ),
    [_MAC] = LAYOUT_all(
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL, KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_TRNS, KC_A,    MT_S_CTL,   MT_D_ALT,   MT_F_GUI,   KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, TO(0),
        KC_NO, KC_LALT, KC_LGUI,                                     LT_SPC_RS,                    MO(3), MO(3),  KC_TRNS, KC_NO
    ),
     [_RAISE] = LAYOUT_all(
        QK_GESC, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11, KC_F12,  KC_DEL, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_HOME,    KC_UP,    KC_END,    KC_HOME,    KC_UP, KC_END, KC_BSLS,
        KC_LCTL, KC_A,    MT_S_CTL,   MT_D_ALT,   MT_F_GUI,    KC_G,    KC_H,    KC_LEFT,    KC_DOWN,    KC_RGHT,    KC_LEFT, KC_DOWN,          KC_RGHT,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_TRNS,
        KC_NO, KC_TRNS, KC_TRNS,                                     KC_SPC,                    KC_TRNS, KC_TRNS,  KC_TRNS, KC_NO
    ),
};

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT_SPC_RS:
            // Immediately select the hold action when another key is pressed.
            return true;
        case MT_F_GUI:
            return true;
        case MT_D_ALT:
             return true;
        case MT_S_CTL:
             return true;
        case MT_A_SFT:
             return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return true;
    }
}

void keyboard_post_init_user(void) {
    vial_tap_dance_entry_t td0 = { TG(1),
                                  TG(2),
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(0, &td0);
    vial_tap_dance_entry_t td1 = { KC_S,
                                  KC_LCTL,
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(1, &td1);
    vial_tap_dance_entry_t td2 = { KC_D,
                                  KC_LALT,
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(2, &td2);
    vial_tap_dance_entry_t td3 = { KC_F,
                                  KC_LGUI,
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(3, &td3);
}
/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
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
#include "../../trackball.h"
#include "qmk_midi.h"

bool wheel_layers = true;
// static uint8_t volume_level = 64; // Initial volume level

void process_wheel_user(int dir) {
    if (wheel_layers) {
        switch (get_highest_layer(layer_state)) {
            case 0:
                encoder_update_kb(0, dir > 0);
                break;
            case 1:
                encoder_update_kb(0, dir > 0);
                break;
            case 2:
                encoder_update_kb(0, dir > 0);
                break;
            case 3:
                dir > 0 ? tap_code(KC_DOT) : tap_code(KC_COMMA);
                break;
            /* case 4:
                if (dir > 0) {
                    volume_level = (volume_level < 127) ? volume_level + 1 : 127;
                } else {
                    volume_level = (volume_level > 0) ? volume_level - 1 : 0;
                }
                midi_send_cc(&midi_device, 0, 7, volume_level);
                break; */
            case 4:
                if (dir > 0) {
                    midi_send_cc(&midi_device, 0, 7, 1); // Increase volume by 1
                } else {
                    midi_send_cc(&midi_device, 0, 7, -1); // Decrease volume by 1
                }
                break;
            default:
                encoder_update_kb(0, dir > 0);
                break;
        }
    } else {
        encoder_update_kb(0, dir > 0);
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 0:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            register_code(KC_LSFT);
            tap_code(KC_F13);
            unregister_code(KC_LSFT);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 1:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            register_code(KC_LSFT);
            tap_code(KC_F14);
            unregister_code(KC_LSFT);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 2:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            register_code(KC_LSFT);
            tap_code(KC_F15);
            unregister_code(KC_LSFT);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 3:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            register_code(KC_LSFT);
            tap_code(KC_F16);
            unregister_code(KC_LSFT);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 4:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            
            register_code(KC_LSFT);
            tap_code(KC_F17);
            unregister_code(KC_LSFT);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
    }
    return state;
}

void handle_dpi_change(uint8_t dpi_index) {
    switch (dpi_index) {
        case 0:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            register_code(KC_LSFT);
            tap_code(KC_F18);
            unregister_code(KC_LSFT);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 1:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            register_code(KC_LSFT);
            tap_code(KC_F19);
            unregister_code(KC_LSFT);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 2:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            register_code(KC_LSFT);
            tap_code(KC_F20);
            unregister_code(KC_LSFT);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
        KC_BTN1, KC_BTN3, KC_BTN4,
          KC_BTN2, TG(1)
    ),
    [1] = LAYOUT(
        _______, _______, _______,
          DPI_CONFIG, TO(0)
    ),
    [2] = LAYOUT(
        _______, _______, _______,
          _______, TO(0)
    ),
    [3] = LAYOUT(
        _______, _______, _______,
          _______, TO(0)
    ),
    [4] = LAYOUT(
        _______, _______, _______,
          _______, TO(0)
    )
};

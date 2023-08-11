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
bool modwheel_enabled = false;
static uint8_t modwheel_value = 0;

// Logic MIDI Settings for incremental fader movement
// Format: 2's Compliment
// Mode: Relative

// Logic MIDI Settings for direct fader values
// Format: Unsigned
// Mode: Scaled

void mousewheel_updown(int dir) {
    encoder_update_kb(0, dir > 0);
}

void logic_playhead(int dir) {
    dir > 0 ? tap_code(KC_DOT) : tap_code(KC_COMMA);
}

void midi_fader_increment(void) {
    midi_send_cc(&midi_device, 0, 7, 1); // Increase CC7 on channel 0 by 1
}
void midi_fader_decrement(void) {
    midi_send_cc(&midi_device, 0, 7, -1); // Decrease CC7 on channel 0 by 1
}

void midi_set_fader_zero(void) {
    midi_send_cc(&midi_device, 1, 7, 90); // Set CC7 to 64 on channel 1
}

void midi_set_fader_down(void) {
    midi_send_cc(&midi_device, 1, 7, 0); // Set CC7 to 0 on channel 1
}

void midi_modwheel_increment(void) {
    if (modwheel_value < 127) {
        modwheel_value++;
    }
    midi_send_cc(&midi_device, 0, 1, modwheel_value); // Set CC1 on channel 0 to modwheel_value
}

void midi_modwheel_decrement(void) {
    if (modwheel_value > 0) {
        modwheel_value--;
    }
    midi_send_cc(&midi_device, 0, 1, modwheel_value); // Set CC1 on channel 0 to modwheel_value
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case SET_FADER_ZERO:
                midi_set_fader_zero();
                return false;
            case SET_FADER_DOWN:
                midi_set_fader_down();
                return false;
            case FADER_INC:
                midi_fader_increment();
                return false;
            case FADER_DEC:
                midi_fader_decrement();
                return false;
            case MODWHEEL_TOGGLE:
                if (record->event.pressed) {
                    modwheel_enabled = !modwheel_enabled; // Toggle the mod wheel state
                }
                return false;
        }
    }
    return true;
}

void process_wheel_user(int dir) {
    if (modwheel_enabled) {
        if (dir > 0) {
            midi_modwheel_increment();
        } else {
            midi_modwheel_decrement();
        }
        return;
    }
    if (wheel_layers) {
        switch (get_highest_layer(layer_state)) {
            case 0:
                mousewheel_updown(dir);
                break;
            case 1:
                mousewheel_updown(dir);
                break;
            case 2:
                mousewheel_updown(dir);
                break;
            case 3:
                logic_playhead(dir);
                break;
            case 4:
                if (dir > 0) {
                    midi_fader_increment();
                } else {
                    midi_fader_decrement();
                }
                break;
            default:
                mousewheel_updown(dir);
                break;
        }
    } else {
        mousewheel_updown(dir);
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

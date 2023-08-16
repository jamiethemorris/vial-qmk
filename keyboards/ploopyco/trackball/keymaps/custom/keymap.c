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
static int scroll_accumulator = 0;
static int midi_fader_accumulator_value = 0;
static int fader_multiplier = 1;
static bool fader_multiplier_enabled = false;

// Logic MIDI Settings for incremental fader movement
// Format: 2's Compliment
// Mode: Relative

// Logic MIDI Settings for direct fader values
// Format: Unsigned
// Mode: Scaled

void mousewheel_updown(int dir) {
    encoder_update_kb(0, dir > 0);
}

void mousewheel_accumulator(int dir) {
    scroll_accumulator += dir;

    if (abs(scroll_accumulator) >= SCROLL_THRESHOLD) {
        // If the accumulated value reaches the threshold, multiply the effect
        int multiplier = (abs(scroll_accumulator) >= SCROLL_THRESHOLD) ? SCROLL_MULTIPLIER : 1;
        bool scroll_up = scroll_accumulator > 0;
        for (int i = 0; i < multiplier; i++) {
            encoder_update_kb(0, scroll_up);
        }
        scroll_accumulator = 0; // Reset the accumulator after processing
    }
}

void logic_playhead(int dir) {
    dir > 0 ? tap_code(KC_DOT) : tap_code(KC_COMMA);
}

/* void midi_fader_increment(void) {
    midi_send_cc(&midi_device, 0, 7, 1); // Increase CC7 on channel 0 by 1
}
void midi_fader_decrement(void) {
    midi_send_cc(&midi_device, 0, 7, -1); // Decrease CC7 on channel 0 by 1
} */

void midi_fader(int value) {
    midi_send_cc(&midi_device, 0, 7, value); // Change CC7 on channel 0 by value
}

void midi_fader_accumulator(int dir) {
    midi_fader_accumulator_value += dir;

    if (abs(midi_fader_accumulator_value) >= MIDI_FADER_THRESHOLD) {
        int change = (midi_fader_accumulator_value > 0) ? 1 : -1; // Determine the direction of change
        int steps = abs(midi_fader_accumulator_value) / MIDI_FADER_THRESHOLD;
        for (int i = 0; i < steps; i++) {
            midi_send_cc(&midi_device, 0, 7, change); // Send relative change
        }
        midi_fader_accumulator_value = 0; // Reset the accumulator after processing
    }
}

void midi_set_fader_zero(void) {
    midi_send_cc(&midi_device, 1, 7, 90); // Set CC7 to 90 on channel 1 (0dB)
}

void midi_set_fader_down(void) {
    midi_send_cc(&midi_device, 1, 7, 0); // Set CC7 to 0 on channel 1 (-inf dB)
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
            case FADER_MULTIPLIER:
                if (record->event.pressed) {
                    // If the key is pressed, toggle the multiplier status
                    fader_multiplier_enabled = !fader_multiplier_enabled;
                    // Set the multiplier value based on the enabled status
                    fader_multiplier = fader_multiplier_enabled ? 10 : 1;
                } else if (fader_multiplier_enabled) {
                    // If the key is released and the multiplier was enabled by holding, reset it
                    fader_multiplier_enabled = false;
                    fader_multiplier = 1;
                }
                return false;
        }
    }
    return true;
}

void process_wheel_user(int dir) {
    if (wheel_layers) {
        switch (get_highest_layer(layer_state)) {
            case 0:
                mousewheel_accumulator(dir);
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
                    midi_fader_accumulator(fader_multiplier);  // Increment                    
                    // midi_fader(1);
                } else {
                    midi_fader_accumulator(-fader_multiplier); // Decrement
                    // midi_fader(-1);
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

void keyboard_post_init_user(void) {
    vial_tap_dance_entry_t td0 = { DRAG_SCROLL,
                                  MO(1),
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(0, &td0);
    vial_tap_dance_entry_t td1 = { KC_BTN4,
                                  KC_NO,
                                  KC_BTN5,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(1, &td1);
    vial_tap_dance_entry_t td2 = { KC_BTN2,
                                  MO(2),
                                  TG(3),
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(2, &td2);
    vial_tap_dance_entry_t td3 = { DRAG_SCROLL,
                                  KC_NO,
                                  TO(0),
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(3, &td3);
    vial_tap_dance_entry_t td4 = { KC_BTN2,
                                  MO(4),
                                  TG(4),
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(4, &td4);
    vial_tap_dance_entry_t td5 = { KC_BTN2,
                                  KC_NO,
                                  TO(3),
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(5, &td5);
    vial_tap_dance_entry_t td6 = { KC_V,
                                  SET_FADER_DOWN,
                                  KC_NO,
                                  SET_FADER_ZERO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(6, &td6);
    vial_tap_dance_entry_t td7 = { KC_NO,
                                  KC_NO,
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(7, &td7);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
        KC_BTN1, KC_F13, TD(1),
          TD(2), TD(0)
    ),
    [1] = LAYOUT(
        _______, _______, _______,
          DPI_CONFIG, TO(0)
    ),
    [2] = LAYOUT(
        KC_BTN4, _______, KC_BTN5,
          _______, TO(0)
    ),
    [3] = LAYOUT(
        KC_BTN1, _______, KC_V,
          TD(4), TD(3)
    ),
    [4] = LAYOUT(
        KC_BTN1, FADER_MULTIPLIER, TD(6),
          TD(5), TD(3)
    )
};

uint16_t keycode_config(uint16_t keycode) {
    return keycode;
}

uint8_t mod_config(uint8_t mod) {
    return mod;
}

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

/*
layer_state_t layer_state_set_user(layer_state_t state) {
    // Check if layer 1 is active
    if (layer_state_cmp(state, 1)) {
        // Layer 1 is active, enable drag scroll
        is_drag_scroll = true;
    } else {
        // Layer 1 is not active, disable drag scroll
        is_drag_scroll = false;
    }
    return state;
} */

bool wheel_layers = false;
static int scroll_accumulator = 0;

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
                mousewheel_updown(dir);
                break;
            case 4:
                mousewheel_updown(dir);
                break;
            default:
                mousewheel_updown(dir);
                break;
        }
    } else {
        mousewheel_accumulator(dir);
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
        KC_BTN1, KC_BTN3, KC_BTN2,
          KC_BTN4, LT(1, KC_BTN5)
    ),
    [1] = LAYOUT(
        DRAG_SCROLL, _______, _______,
          _______, _______
    ),
    [2] = LAYOUT(
        _______, _______, _______,
          _______, _______
    ),
    [3] = LAYOUT(
        _______, _______, _______,
          _______, _______
    ),
    [4] = LAYOUT(
        _______, _______, _______,
          _______, _______
    )
};
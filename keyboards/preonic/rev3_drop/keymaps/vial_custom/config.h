/* Copyright 2020 Josef Adamcik
 * Modification for VIA support and RGB underglow by Jens Bonk-Wiltfang
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

#pragma once

// Vial Support
#define VIAL_KEYBOARD_UID {0x9D, 0xA9, 0x2D, 0x7B, 0x37, 0x6E, 0x83, 0x40}

#define VIAL_UNLOCK_COMBO_ROWS { 0, 4 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 5 }

#define GRAVE_ESC_ALT_OVERRIDE
#define GRAVE_ESC_CTRL_OVERRIDE

#define MIDI_ADVANCED
#define DYNAMIC_KEYMAP_LAYER_COUNT 8
#define VIAL_TAP_DANCE_ENTRIES 16 

#undef TAPPING_TERM
#undef TAP_CODE_DELAY
#define TAPPING_TERM 200
#define TAP_CODE_DELAY 10
#define QUICK_TAP_TERM 110
#define PERMISSIVE_HOLD_PER_KEY
// #define PERMISSIVE_HOLD
#define RETRO_TAPPING_PER_KEY

#define MOUSEKEY_MOVE_DELTA 8
#define MOUSEKEY_MAX_SPEED 4
#define MOUSEKEY_TIME_TO_MAX 15
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

#define USB_POLLING_INTERVAL_MS 1
#undef DEBOUNCE
#define DEBOUNCE 1
#define FORCE_NKRO
#define QMK_KEYS_PER_SCAN 4

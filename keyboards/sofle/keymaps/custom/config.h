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

/* By default left side is selected as master, 
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options. */

#if defined(KEYBOARD_sofle_rev1)
// Add RGB underglow and top facing lighting
#    define WS2812_DI_PIN D3
#    define RGBLED_NUM 72
#    define RGBLED_SPLIT \
        { 36, 36 }
#    ifdef RGB_MATRIX_ENABLE
#        define RGB_MATRIX_LED_COUNT RGBLED_NUM
#        define RGB_MATRIX_SPLIT RGBLED_SPLIT
#        define SPLIT_TRANSPORT_MIRROR
#        define DRIVER_LED_TOTAL RGBLED_NUM
#        define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120
#        define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#        define RGB_MATRIX_KEYPRESSES
#    else
#        define RGBLIGHT_EFFECT_BREATHING
#        define RGBLIGHT_EFFECT_RAINBOW_MOOD
#        define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#        define RGBLIGHT_EFFECT_SNAKE
#        define RGBLIGHT_EFFECT_KNIGHT
#        define RGBLIGHT_EFFECT_CHRISTMAS
#        define RGBLIGHT_EFFECT_STATIC_GRADIENT
#        define RGBLIGHT_EFFECT_RGB_TEST
#        define RGBLIGHT_EFFECT_ALTERNATING
#        define RGBLIGHT_EFFECT_TWINKLE
#        define RGBLIGHT_LIMIT_VAL 120
#        define RGBLIGHT_HUE_STEP 10
#        define RGBLIGHT_SAT_STEP 17
#        define RGBLIGHT_VAL_STEP 17
#    endif
#endif

// Vial Support
#define VIAL_KEYBOARD_UID { 0x05, 0xCD, 0x9F, 0x8A, 0xF4, 0xDF, 0xDE, 0xB2 }

// The four corners
#define VIAL_UNLOCK_COMBO_ROWS { 0, 5, 3, 8 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 0, 0, 0 }

#define RP2040_BUILD
// #define EE_HANDS
// #define MASTER_LEFT
#define MASTER_RIGHT
#define GRAVE_ESC_ALT_OVERRIDE
#define GRAVE_ESC_CTRL_OVERRIDE

#ifdef RP2040_BUILD
        #define MIDI_ADVANCED
        #define DYNAMIC_KEYMAP_LAYER_COUNT 8
        #define VIAL_TAP_DANCE_ENTRIES 16 
        // #define RGB_MATRIX_ENABLE
        #define SPLIT_POINTING_ENABLE
        #define POINTING_DEVICE_RIGHT
        #define CIRQUE_PINNACLE_TAP_ENABLE
        #define CIRQUE_PINNACLE_SECONDARY_TAP_ENABLE
        #define POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE
        #define POINTING_DEVICE_GESTURES_SCROLL_ENABLE
        #define CIRQUE_PINNACLE_TAPPING_TERM 70
        #define POINTING_DEVICE_RIGHT
        #define POINTING_DEVICE_AUTO_MOUSE_ENABLE
        #define AUTO_MOUSE_DEFAULT_LAYER 3
        #define AUTO_MOUSE_TIME 650
        #define AUTO_MOUSE_DELAY 170
#else
        #define DYNAMIC_KEYMAP_LAYER_COUNT 6
        #define VIAL_TAP_DANCE_ENTRIES 10
#endif

#undef ENCODER_MAP_ENABLE
#undef TAPPING_TERM
#undef TAP_CODE_DELAY
#define TAPPING_TERM 170
#define TAP_CODE_DELAY 10

#define MOUSEKEY_MOVE_DELTA 8
#define MOUSEKEY_MAX_SPEED 4
#define MOUSEKEY_TIME_TO_MAX 15
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

#undef PRODUCT
#define PRODUCT "Sofle Custom"

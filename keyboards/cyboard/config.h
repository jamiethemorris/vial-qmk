/* Copyright 2023 Cyboard LLC (@Cyboard-DigitalTailor)
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

/* Split keyboard settings */
#define SERIAL_USART_TX_PIN GP28
#define SPLIT_HAND_PIN GP0

/* RGB settings */
#ifdef RGB_MATRIX_ENABLE
#    define WS2812_PIO_USE_PIO1 // Force the usage of PIO1 peripheral, by default the WS2812 implementation uses the PIO0 peripheral
#    define SPLIT_TRANSPORT_MIRROR
#    define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_REACTIVE // Sets the default mode, if none has been set
#    define RGB_MATRIX_DEFAULT_HUE 33 // Sets the default hue value, if none has been set
#    define RGB_MATRIX_DEFAULT_SAT 255 // Sets the default saturation value, if none has been set
#    define RGB_MATRIX_DEFAULT_SPD 50
#    define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_MATRIX_SLEEP
#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#endif

/* Double-tap reset button parameters */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

#define USB_VBUS_PIN GP27

/* Vial set up */
#define VIAL_KEYBOARD_UID {0xC4, 0xCC, 0xB4, 0x76, 0x30, 0x16, 0xFC, 0x70}

#define VIAL_UNLOCK_COMBO_ROWS { 10, 10 }
#define VIAL_UNLOCK_COMBO_COLS { 3, 4 }

#define DYNAMIC_KEYMAP_LAYER_COUNT 10

// custom

#define GRAVE_ESC_ALT_OVERRIDE
#define GRAVE_ESC_CTRL_OVERRIDE

// #undef ENCODER_MAP_ENABLE
#undef TAPPING_TERM
#undef TAP_CODE_DELAY
#define TAPPING_TERM 200
#define TAP_CODE_DELAY 10
#define QUICK_TAP_TERM 110

#define MOUSEKEY_MOVE_DELTA 8
#define MOUSEKEY_MAX_SPEED 4
#define MOUSEKEY_TIME_TO_MAX 15
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

#define MIDI_ADVANCED

// #define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
// #define RETRO_TAPPING
#define PERMISSIVE_HOLD_PER_KEY
// #define PERMISSIVE_HOLD
#define RETRO_TAPPING_PER_KEY

#define USB_POLLING_INTERVAL_MS 1
#undef DEBOUNCE
#define DEBOUNCE 5
#define FORCE_NKRO
#define QMK_KEYS_PER_SCAN 4

#define ENCODERS_PAD_A { GP16 }
#define ENCODERS_PAD_B { GP18 }
#define ENCODER_RESOLUTION 4
// #define ENCODER_DEFAULT_POS 0x3
#define ENCODER_DIRECTION_FLIP
#define ENCODER_MAP_KEY_DELAY 10

#define MOUSE_TURBO_CLICK_PERIOD 200

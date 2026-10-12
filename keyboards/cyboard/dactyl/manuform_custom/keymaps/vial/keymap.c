/* Copyright 2023 Cyboard LLC (@Cyboard-DigitalTailor)
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include QMK_KEYBOARD_H
#include "qmk_midi.h"
#include "features/mouse_turbo_click.h"

bool is_ag_swapped = false;
bool is_gui_disabled = false;
static bool alt_gui_swapped = false;
static bool gui_disabled = false;
bool shift_lock = false;

enum dactyl_layers {
    _DEFAULTS = 0,
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _MOUSE,
    _NUMPAD,
    _MIDI,
    _CHORD1,
    _CHORD2,
    _MACMOD
};

enum custom_keycodes {
    MD_WIN = QK_KB_0,
    MD_MAC,
    MD_GAME,
	
	DEBUG,
	RESTART_DEBUG,
	STOP_DEBUG,
    KC_LOCK_SHIFT,
    KC_TERM,

    MT_G_GUI_L1,
    MT_V_CTL_L1,
    TURBO
};

enum midi_keycodes
{
    C_MAJ = SAFE_RANGE,
    D_MIN,
    E_MIN,
    F_MAJ,
    G_MAJ,
    A_MIN,
    B_DIM,
    
    MI_CHOCTU,
    MI_CHOCTD,
    MI_CHTRSU,
    MI_CHTRSD,
    
    MI_MDLT,
    MI_7TH,
    MODWL_UP,
    MODWL_DN,
    KC_FDR_UP,
    KC_FDR_DN,
    KC_PAN_L,
    KC_PAN_R,
};

static int8_t octave = 0;  // starts at 0, can be increased or decreased
static int8_t transpose = 0;  // starts at 0, can be increased or decreased

static bool is_key_8_held = false;
static bool is_key_9_held = false;

uint8_t mod_wheel_val = 0;

typedef struct {
    int8_t octave;
    int8_t transpose;
} ChordData;

ChordData chordData[7];

#define LT_TAB_NP LT(_NUMPAD,KC_TAB)
#define LT_SPC_RS LT(_RAISE,KC_SPC) 
#define LT_ENT_LW LT(_LOWER,KC_ENT)
#define LT_ENT_MS LT(_MOUSE,KC_ENT)
#define MT_SPC_GUI MT(MOD_LGUI,KC_SPC) 

#define MT_F_GUI MT(MOD_LGUI,KC_F)   
#define MT_D_ALT MT(MOD_LALT,KC_D) 
#define MT_S_CTL MT(MOD_LCTL,KC_S)
#define MT_A_SFT MT(MOD_LSFT,KC_A)

#define MT_G_GUI MT(MOD_LGUI,KC_G) 
#define MT_V_CTL MT(MOD_LCTL,KC_V)

#define MT_GRV_SFT MT(MOD_LSFT,KC_GRV)

#define ST_DBG STOP_DEBUG
#define RS_DBG RESTART_DEBUG
#define DB_DBG DEBUG

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
TAP DANCE: 
0: W/Win
1: M/Mac
2: G/Game
3: Del/MIDI Layer
4: Minus/Equal
5: Chord1
6: Chord2
7: Shift Lock
*/

    [_QWERTY] = LAYOUT_manuform_num(
        QK_GESC,    KC_1,     KC_2,    KC_3,    KC_4,    KC_5,                                                          KC_6,    KC_7,        KC_8,    KC_9,    KC_0,    KC_BSPC,
        LT_TAB_NP,  KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,                                                          KC_Y,    KC_U,        KC_I,    KC_O,    KC_P,    TD(4),
        TD(8),      KC_A, MT_S_CTL,MT_D_ALT,MT_F_GUI,TD(9),                                                      KC_H,    KC_J,        KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT,    KC_Z,     KC_X,    KC_C,    TD(10),  KC_B,                                                        KC_N,    KC_M,        KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
        KC_LCTL,   KC_LCTL, KC_LALT, KC_LGUI, MT_SPC_GUI, LT_ENT_LW, KC_DEL,                                   KC_BSPC, LT_ENT_MS, LT_SPC_RS,  KC_LBRC, KC_RBRC, KC_MINS, KC_EQUAL,
                                                                     KC_LSFT,TD(8), _______,   _______, KC_HOME, KC_END
    ),

    [_LOWER] = LAYOUT_manuform_num(
        KC_GRV,     KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,                                                           KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  KC_F11,
        _______,    _______,  _______, _______, _______, KC_P,                                                           _______, _______, _______, _______,  _______, KC_F12,
        _______,    _______,  KC_M,    _______, KC_COMM, KC_DOT,                                                          _______, _______, _______, _______,  _______, _______,
        KC_CAPS,    _______,  KC_BSLS, _______, KC_LCBR, KC_RCBR,                                                         KC_LBRC, KC_RBRC, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______, _______,                                       _______, _______, _______, MD_WIN,  MD_MAC,   MD_GAME, _______,
                                                                  _______, _______, _______,   _______, TURBO, _______   
    ),

    [_RAISE] = LAYOUT_manuform_num(
        _______,    KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,                                                           KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  KC_DEL,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, KC_HOME, KC_UP,   KC_END,   _______, _______,
        _______,    _______,  ST_DBG,  DB_DBG,  RS_DBG,  KC_PGUP,                                                         KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT, KC_END,  _______,
        _______,    _______,  _______, _______, _______, KC_PGDN,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______,  _______, _______,
                                                                  _______, _______, _______,   _______, _______, _______
    ),

    [_MOUSE] = LAYOUT_manuform_num(
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, KC_MS_U, _______,  _______, _______,
        _______,    _______,  _______, KC_MS_BTN2, KC_MS_BTN1, _______,                                                   KC_MS_BTN1,KC_MS_L,KC_MS_D,KC_MS_R,KC_MS_BTN2,_______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______,  _______, _______,
                                                                  _______, TURBO, _______,   _______, _______, _______    
    ),

    [_NUMPAD] = LAYOUT_manuform_num(
        _______,    _______,  _______, _______, _______, _______,                                                         _______, KC_7,    KC_8,    KC_9,     _______, KC_PPLS,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, KC_4,    KC_5,    KC_6,     _______, KC_PMNS,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, KC_1,    KC_2,    KC_3,     _______, KC_PAST,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, KC_0,    KC_0,    KC_PDOT,  _______, KC_PSLS,
        _______,    _______,  _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______,  _______, _______,
                                                                  _______, _______, _______,   _______, _______, _______
    ),

    [_MIDI] = LAYOUT_manuform_num(
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______,  _______, _______,
                                                                  _______, _______, _______,   _______, _______, _______
    ),

    [_CHORD1] = LAYOUT_manuform_num(
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______,  _______, _______,
                                                                  _______, _______, _______,   _______, _______, _______    
                                                                 
    ),

    [_CHORD2] = LAYOUT_manuform_num(
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______,  _______, _______,
                                                                  _______, _______, _______,   _______, _______, _______    
    ),

    [_MACMOD] = LAYOUT_manuform_num(
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  KC_TAB, KC_TAB,  MT_GRV_SFT,_______,                                                       _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, KC_LSFT,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, KC_LSFT, _______, _______,                                       _______, _______, _______, _______, _______,  _______, _______,
                                                                  _______, _______, _______,   _______, _______, _______    
    ), 

    [9] = LAYOUT_manuform_num(
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______,                                                         _______, _______, _______, _______,  _______, _______,
        _______,    _______,  _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______,  _______, _______,
                                                                  _______, _______, _______,   _______, _______, _______
    )
};

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT_SPC_RS:
            // Immediately select the hold action when another key is pressed.
            return true;
        case MT_SPC_GUI:
            return true;
        case MT_F_GUI:
            return true;
        case MT_D_ALT:
             return true;
        case MT_S_CTL:
             return true;
        case MT_A_SFT:
             return true;
        case MT_G_GUI:
             return true;
        case MT_V_CTL:
             return true;
        case MT_G_GUI_L1:
             return true;
        case MT_V_CTL_L1:
            return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT_SPC_GUI:
            return false;
        default:
            return true;
    }
}

/* uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT_F_GUI:
        case MT_D_ALT:
        case MT_S_CTL:
        case MT_A_SFT: 
            return 300;
        default:
            return TAPPING_TERM; // Default tapping term for other keys
    }
} */

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),           ENCODER_CCW_CW(KC_COMM, KC_DOT) }, 
    [1] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) },
    [2] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) },
    [3] = { ENCODER_CCW_CW(KC_WH_D, KC_WH_U),           ENCODER_CCW_CW(KC_WH_D, KC_WH_U) },
    [4] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(RGB_SAD, RGB_SAI) },
    [5] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(MODWL_DN, MODWL_UP) },
    [6] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(MODWL_DN, MODWL_UP) },
    [7] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(MODWL_DN, MODWL_UP) },
    [8] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) },
    [9] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) }
};
#endif

void keyboard_post_init_user(void) {

    vial_tap_dance_entry_t td0 = { KC_NO,
                                  KC_NO,
                                  KC_NO,
                                  MD_WIN,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(0, &td0);
    vial_tap_dance_entry_t td1 = { KC_NO,
                                  KC_NO,
                                  KC_NO,
                                  MD_MAC,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(1, &td1);
    vial_tap_dance_entry_t td2 = { KC_NO,
                                  KC_NO,
                                  KC_NO,
                                  MD_GAME,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(2, &td2);
    vial_tap_dance_entry_t td3 = { KC_DEL,
                                  KC_NO,
                                  KC_NO,
                                  TG(5),
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(3, &td3);
    vial_tap_dance_entry_t td4 = { KC_MINS,
                                  KC_EQL,
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(4, &td4);
    vial_tap_dance_entry_t td5 = { TG(6),
                                  MO(6),
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(5, &td5);
    vial_tap_dance_entry_t td6 = { TG(7),
                                  MO(7),
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(6, &td6);
    vial_tap_dance_entry_t td7 = { KC_LSFT,
                                  KC_LSFT,
                                  KC_LOCK_SHIFT,
                                  KC_LOCK_SHIFT,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(7, &td7);
    vial_tap_dance_entry_t td8 = { OSM(MOD_HYPR),
                                  OSM(MOD_HYPR),
                                  KC_TERM,
                                  OSM(MOD_BIT_LCTRL|MOD_BIT_LALT|MOD_BIT_LGUI),
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(8, &td8);
    vial_tap_dance_entry_t td9 = { KC_G,
                                  MT_G_GUI_L1,
                                  KC_NO,
                                  KC_NO,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(9, &td9);
    vial_tap_dance_entry_t td10 = { KC_V,
                                   MT_V_CTL_L1,
                                   KC_NO,
                                   KC_NO,
                                   TAPPING_TERM };
    dynamic_keymap_set_tap_dance(10, &td10);
}

void swap_keys(void) {
    if (!alt_gui_swapped) {
        alt_gui_swapped = true;
    }
}

void unswap_keys(void) {
    if (alt_gui_swapped) {
        alt_gui_swapped = false;
    }
}

void toggle_gui(void) {
    gui_disabled = !gui_disabled;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_mouse_turbo_click(keycode, record, TURBO)) { return false; }
    switch (keycode) {
        case MT_G_GUI_L1:
            if (record->event.pressed) {
                // Key is pressed
                register_mods(MOD_BIT(KC_LGUI));
                layer_on(_MACMOD);
            } else {
                // Key is released
                unregister_mods(MOD_BIT(KC_LGUI));
                layer_off(_MACMOD);
            }
            return false; // Skip further processing of this key code

        case MT_V_CTL_L1:
            if (record->event.pressed) {
                // Key is pressed
                register_mods(MOD_BIT(KC_LCTL));
                layer_on(_MACMOD); 
            } else {
                // Key is released
                unregister_mods(MOD_BIT(KC_LCTL));
                layer_off(_MACMOD);
            }
            return false; // Skip further processing of this key code

        // Add other custom keycode cases here

        default:
            return true; // Process all other keycodes normally
    }
    if (keycode == KC_LOCK_SHIFT) {
        if (record->event.pressed) {
            shift_lock = !shift_lock;  // Toggle the shift lock state
            if (shift_lock) {
                register_code(KC_LSFT);
            } else {
                unregister_code(KC_LSFT);
            }
        }
        return false;
    }

    if (shift_lock) {
        if (record->event.pressed) {
            if (keycode >= KC_A && keycode <= KC_Z) {
                register_code(KC_LSFT);
                return true;
            } 
            if (keycode == KC_MINUS) {
                register_code(KC_LSFT);
                return true;
            }
        }
    }
    if (alt_gui_swapped) {
        if (record->event.pressed) {
            switch (keycode) {
                case KC_LALT:
                    register_code(KC_LGUI);
                    return false;
                case KC_LGUI:
                    register_code(KC_LALT);
                    return false;
                case KC_RALT:
                    register_code(KC_RGUI);
                    return false;
                case KC_RGUI:
                    register_code(KC_RALT);
                    return false;
            }
        } else {
            switch (keycode) {
                case KC_LALT:
                    unregister_code(KC_LGUI);
                    return false;
                case KC_LGUI:
                    unregister_code(KC_LALT);
                    return false;
                case KC_RALT:
                    unregister_code(KC_RGUI);
                    return false;
                case KC_RGUI:
                    unregister_code(KC_RALT);
                    return false;
            }
        }
    }
    if (gui_disabled) {
        if (keycode == KC_LGUI || keycode == KC_RGUI) {
            return false;  // Block the GUI key
        }
    } else {
        if (keycode == KC_LGUI || keycode == KC_RGUI) {
            return true;  // Unblock the GUI key
        }
    }
    
    switch (keycode) {	         
	// Normal macros
        case MD_WIN:
            if (record->event.pressed) {
                swap_keys();
                gui_disabled = false;  // Re-enable the GUI key
                #ifdef OLED_ENABLE
                is_ag_swapped = alt_gui_swapped;
                is_gui_disabled = gui_disabled;
                #endif
            }
            return false;
        case MD_MAC:
            if (record->event.pressed) {
                unswap_keys();
                gui_disabled = false;  // Re-enable the GUI key
                #ifdef OLED_ENABLE
                is_ag_swapped = alt_gui_swapped;
                is_gui_disabled = gui_disabled;
                #endif
            }
            return false;
        case MD_GAME:
            if (record->event.pressed) {
                toggle_gui();
                #ifdef OLED_ENABLE
                is_gui_disabled = gui_disabled;
                #endif
            }
            return false;
        case DEBUG:
            if (record->event.pressed) {
                tap_code(KC_F5);
            }
            return false;  
        case RESTART_DEBUG:
            if (record->event.pressed) {
                register_code(KC_LGUI);
				register_code(KC_LSFT);
				tap_code(KC_F5);
				unregister_code(KC_LSFT);
				unregister_code(KC_LGUI);
            }
            return false;  
        case STOP_DEBUG:
            if (record->event.pressed) {
                register_code(KC_LSFT);
				tap_code(KC_F5);
				unregister_code(KC_LSFT);
            }
            return false;  
        case KC_TERM:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_LSFT);
                register_code(KC_LALT);
                register_code(KC_LCTL);
                tap_code(KC_T);
                unregister_code(KC_LGUI);
                unregister_code(KC_LSFT);
                unregister_code(KC_LALT);
                unregister_code(KC_LCTL);
            }
            return false;

	// Transposition
		case MI_CHOCTU:
			if (record->event.pressed) {
				octave += 12;  // increase by 12 to represent an octave
			}
			return false;
		case MI_CHOCTD:
			if (record->event.pressed) {
				octave -= 12;  // decrease by 12 to represent an octave
			}
			return false;
		case MI_CHTRSU:
			if (record->event.pressed) {
				transpose += 1;  // transpose up by 1 semitone
			}
			return false;
		case MI_CHTRSD:
			if (record->event.pressed) {
				transpose -= 1;  // transpose down by 1 semitone
			}
			return false;
			
		case MI_7TH:  // add appropriate 7th note to a chord
			if (record->event.pressed) {
				is_key_9_held = true;
			} else {
				is_key_9_held = false;
			}
			return false;

		case MI_MDLT:  // sharpen 3rd half step for minor, flatten 3rd half step for major, and flatten diminished 3rd by half step
			if (record->event.pressed) {
				is_key_8_held = true;
			} else {
				is_key_8_held = false;
			}
			return false;
		
	// C Major scale chords
		case C_MAJ:
			if (record->event.pressed) {
				chordData[0].octave = octave;
				chordData[0].transpose = transpose;

				// Base notes (root and 5th)
				midi_send_noteon(&midi_device, 0, 60 + chordData[0].octave + chordData[0].transpose, 127);  // C note
				midi_send_noteon(&midi_device, 0, 67 + chordData[0].octave + chordData[0].transpose, 127);  // G note

				if (is_key_8_held) {
					// Send the minor 3rd for C minor
					midi_send_noteon(&midi_device, 0, 63 + chordData[0].octave + chordData[0].transpose, 127);  // E flat
				} else {
					// Send the major 3rd for C major
					midi_send_noteon(&midi_device, 0, 64 + chordData[0].octave + chordData[0].transpose, 127);  // E
				}

				if (is_key_9_held) {
					// Send the 7th for C major 7
					midi_send_noteon(&midi_device, 0, 71 + chordData[0].octave + chordData[0].transpose, 127);  // B
				}
			} else {
				// Send note off for all potential notes (this ensures no hanging notes)
				midi_send_noteoff(&midi_device, 0, 60 + chordData[0].octave + chordData[0].transpose, 0);  // C note
				midi_send_noteoff(&midi_device, 0, 63 + chordData[0].octave + chordData[0].transpose, 0);  // E flat
				midi_send_noteoff(&midi_device, 0, 64 + chordData[0].octave + chordData[0].transpose, 0);  // E
				midi_send_noteoff(&midi_device, 0, 67 + chordData[0].octave + chordData[0].transpose, 0);  // G note
				midi_send_noteoff(&midi_device, 0, 71 + chordData[0].octave + chordData[0].transpose, 0);  // B
			}
			return false;
        case D_MIN:
			if (record->event.pressed) {
				chordData[1].octave = octave;
				chordData[1].transpose = transpose;

				// Base notes (root and 5th)
				midi_send_noteon(&midi_device, 0, 62 + chordData[1].octave + chordData[1].transpose, 127);  // D note
				midi_send_noteon(&midi_device, 0, 69 + chordData[1].octave + chordData[1].transpose, 127);  // A note

				if (is_key_8_held) {
					// Send the major 3rd for D major
					midi_send_noteon(&midi_device, 0, 66 + chordData[1].octave + chordData[1].transpose, 127);  // F sharp
				} else {
					// Send the minor 3rd for D minor
					midi_send_noteon(&midi_device, 0, 65 + chordData[1].octave + chordData[1].transpose, 127);  // F
				}

				if (is_key_9_held) {
					// Send the 7th for D minor 7
					midi_send_noteon(&midi_device, 0, 72 + chordData[1].octave + chordData[1].transpose, 127);  // C
				}
			} else {
				// Send note off for all potential notes (this ensures no hanging notes)
				midi_send_noteoff(&midi_device, 0, 62 + chordData[1].octave + chordData[1].transpose, 0);  // D note
				midi_send_noteoff(&midi_device, 0, 65 + chordData[1].octave + chordData[1].transpose, 0);  // F
				midi_send_noteoff(&midi_device, 0, 66 + chordData[1].octave + chordData[1].transpose, 0);  // F sharp
				midi_send_noteoff(&midi_device, 0, 69 + chordData[1].octave + chordData[1].transpose, 0);  // A note
				midi_send_noteoff(&midi_device, 0, 72 + chordData[1].octave + chordData[1].transpose, 0);  // C
			}
			return false;

		case E_MIN:          
			if (record->event.pressed) {
				chordData[2].octave = octave;
				chordData[2].transpose = transpose;

				// Base notes (root and 5th)
				midi_send_noteon(&midi_device, 0, 64 + chordData[2].octave + chordData[2].transpose, 127);  // E note
				midi_send_noteon(&midi_device, 0, 71 + chordData[2].octave + chordData[2].transpose, 127);  // B note

				if (is_key_8_held) {
					// Send the major 3rd for E major
					midi_send_noteon(&midi_device, 0, 68 + chordData[2].octave + chordData[2].transpose, 127);  // G sharp
				} else {
					// Send the minor 3rd for E minor
					midi_send_noteon(&midi_device, 0, 67 + chordData[2].octave + chordData[2].transpose, 127);  // G
				}

				if (is_key_9_held) {
					// Send the 7th for E minor 7
					midi_send_noteon(&midi_device, 0, 74 + chordData[2].octave + chordData[2].transpose, 127);  // D
				}
			} else {
				// Send note off for all potential notes (this ensures no hanging notes)
				midi_send_noteoff(&midi_device, 0, 64 + chordData[2].octave + chordData[2].transpose, 0);  // E note
				midi_send_noteoff(&midi_device, 0, 67 + chordData[2].octave + chordData[2].transpose, 0);  // G
				midi_send_noteoff(&midi_device, 0, 68 + chordData[2].octave + chordData[2].transpose, 0);  // G sharp
				midi_send_noteoff(&midi_device, 0, 71 + chordData[2].octave + chordData[2].transpose, 0);  // B note
				midi_send_noteoff(&midi_device, 0, 74 + chordData[2].octave + chordData[2].transpose, 0);  // D
			}
			return false;
		case F_MAJ:
			if (record->event.pressed) {
				chordData[3].octave = octave;
				chordData[3].transpose = transpose;

				// Base notes (root and 5th)
				midi_send_noteon(&midi_device, 0, 65 + chordData[3].octave + chordData[3].transpose, 127);  // F note
				midi_send_noteon(&midi_device, 0, 72 + chordData[3].octave + chordData[3].transpose, 127);  // C note

				if (is_key_8_held) {
					// Send the minor 3rd for F minor
					midi_send_noteon(&midi_device, 0, 68 + chordData[3].octave + chordData[3].transpose, 127);  // A flat
				} else {
					// Send the major 3rd for F major
					midi_send_noteon(&midi_device, 0, 69 + chordData[3].octave + chordData[3].transpose, 127);  // A
				}

				if (is_key_9_held) {
					// Send the major 7th for F major 7
					midi_send_noteon(&midi_device, 0, 76 + chordData[3].octave + chordData[3].transpose, 127);  // E
				}
			} else {
				// Send note off for all potential notes (this ensures no hanging notes)
				midi_send_noteoff(&midi_device, 0, 65 + chordData[3].octave + chordData[3].transpose, 0);  // F note
				midi_send_noteoff(&midi_device, 0, 68 + chordData[3].octave + chordData[3].transpose, 0);  // A flat
				midi_send_noteoff(&midi_device, 0, 69 + chordData[3].octave + chordData[3].transpose, 0);  // A
				midi_send_noteoff(&midi_device, 0, 72 + chordData[3].octave + chordData[3].transpose, 0);  // C note
				midi_send_noteoff(&midi_device, 0, 76 + chordData[3].octave + chordData[3].transpose, 0);  // E
			}
			return false;

		case G_MAJ:
			if (record->event.pressed) {
				chordData[4].octave = octave;
				chordData[4].transpose = transpose;

				// Base notes (root and 5th)
				midi_send_noteon(&midi_device, 0, 67 + chordData[4].octave + chordData[4].transpose, 127);  // G note
				midi_send_noteon(&midi_device, 0, 74 + chordData[4].octave + chordData[4].transpose, 127);  // D note

				if (is_key_8_held) {
					// Send the minor 3rd for G minor
					midi_send_noteon(&midi_device, 0, 70 + chordData[4].octave + chordData[4].transpose, 127);  // B flat
				} else {
					// Send the major 3rd for G major
					midi_send_noteon(&midi_device, 0, 71 + chordData[4].octave + chordData[4].transpose, 127);  // B
				}

				if (is_key_9_held) {
					// Send the minor 7th for G dominant 7
					midi_send_noteon(&midi_device, 0, 77 + chordData[4].octave + chordData[4].transpose, 127);  // F
				}
			} else {
				// Send note off for all potential notes (this ensures no hanging notes)
				midi_send_noteoff(&midi_device, 0, 67 + chordData[4].octave + chordData[4].transpose, 0);  // G note
				midi_send_noteoff(&midi_device, 0, 70 + chordData[4].octave + chordData[4].transpose, 0);  // B flat
				midi_send_noteoff(&midi_device, 0, 71 + chordData[4].octave + chordData[4].transpose, 0);  // B
				midi_send_noteoff(&midi_device, 0, 74 + chordData[4].octave + chordData[4].transpose, 0);  // D note
				midi_send_noteoff(&midi_device, 0, 77 + chordData[4].octave + chordData[4].transpose, 0);  // F
			}
			return false;
		case A_MIN:
			if (record->event.pressed) {
				chordData[5].octave = octave;
				chordData[5].transpose = transpose;

				midi_send_noteon(&midi_device, 0, 69 + chordData[5].octave + chordData[5].transpose, 127);  // A note
				midi_send_noteon(&midi_device, 0, 76 + chordData[5].octave + chordData[5].transpose, 127);  // E note

				if (is_key_8_held) {
					midi_send_noteon(&midi_device, 0, 73 + chordData[5].octave + chordData[5].transpose, 127);  // C# for A major
				} else {
					midi_send_noteon(&midi_device, 0, 72 + chordData[5].octave + chordData[5].transpose, 127);  // C for A minor
				}

				if (is_key_9_held) {
					midi_send_noteon(&midi_device, 0, 79 + chordData[5].octave + chordData[5].transpose, 127);  // G for A minor 7
				}
			} else {
				midi_send_noteoff(&midi_device, 0, 69 + chordData[5].octave + chordData[5].transpose, 0);
				midi_send_noteoff(&midi_device, 0, 72 + chordData[5].octave + chordData[5].transpose, 0);
				midi_send_noteoff(&midi_device, 0, 73 + chordData[5].octave + chordData[5].transpose, 0);
				midi_send_noteoff(&midi_device, 0, 76 + chordData[5].octave + chordData[5].transpose, 0);
				midi_send_noteoff(&midi_device, 0, 79 + chordData[5].octave + chordData[5].transpose, 0);
			}
			return false;

		case B_DIM:
			if (record->event.pressed) {
				chordData[6].octave = octave;
				chordData[6].transpose = transpose;

				midi_send_noteon(&midi_device, 0, 71 + chordData[6].octave + chordData[6].transpose, 127);  // B note
				midi_send_noteon(&midi_device, 0, 74 + chordData[6].octave + chordData[6].transpose, 127);  // D note
				midi_send_noteon(&midi_device, 0, 77 + chordData[6].octave + chordData[6].transpose, 127);  // F note		

				if (is_key_9_held) {
					midi_send_noteon(&midi_device, 0, 81 + chordData[6].octave + chordData[6].transpose, 127);  // A for B half diminished 7
				}
			} else {
				midi_send_noteoff(&midi_device, 0, 71 + chordData[6].octave + chordData[6].transpose, 0);
				midi_send_noteoff(&midi_device, 0, 74 + chordData[6].octave + chordData[6].transpose, 0);				
				midi_send_noteoff(&midi_device, 0, 77 + chordData[6].octave + chordData[6].transpose, 0);
				midi_send_noteoff(&midi_device, 0, 81 + chordData[6].octave + chordData[6].transpose, 0);
			}
			return false;

        case MODWL_UP:
            if (record->event.pressed) {
                if (mod_wheel_val < 127) mod_wheel_val++;
                midi_send_cc(&midi_device, 0, 0x01, mod_wheel_val);
            }
            return false;
        case MODWL_DN:
            if (record->event.pressed) {
                if (mod_wheel_val > 0) mod_wheel_val--;
                midi_send_cc(&midi_device, 0, 0x01, mod_wheel_val);
            }
            return false;

		default:
            return true;  // Process all other keycodes normally
    }
}

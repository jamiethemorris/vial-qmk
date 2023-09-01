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

#include QMK_KEYBOARD_H
#include "oled.c"
#ifdef RP2040_BUILD
    #include "qmk_midi.h"
#endif

bool is_ag_swapped = false;
bool is_gui_disabled = false;

enum sofle_layers {
    _DEFAULTS = 0,
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _MOUSE,
    _NUMPAD,
#ifdef RP2040_BUILD
    _MIDI,
    _CHORD1,
    _CHORD2,
#else
    _MISC
#endif
};

enum custom_keycodes {
    MD_WIN = QK_KB_0,
    MD_MAC,
    MD_GAME,
    #ifdef RP2040_BUILD
    C_MAJ,
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
    #endif
	
	DEBUG,
	RESTART_DEBUG,
	STOP_DEBUG

};

#ifdef RP2040_BUILD
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
#endif

#define LT_TAB_NP LT(_NUMPAD,KC_TAB)
#define LT_SPC_RS LT(_RAISE,KC_SPC)
#define LT_ENT_LW LT(_LOWER,KC_ENT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | GESC |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |Tab/NP|   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  \   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Caps |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  | Enter|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTR | LAlt | LGUI |LOWER | /Enter  /       \Spc/Rs\  |MOUSE |  -   |  [   |   ]  |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

/*

TAP DANCE: 
0: W/Win
1: M/Mac
2: G/Game
3: Gresc/MIDI Layer
4: Minus/Equal
5: Chord1
6: Chord2
*/

[_QWERTY] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  TD(3), KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  LT_TAB_NP,KC_Q,   TD(0),   KC_E,    KC_R,    KC_T,                       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  KC_HYPR, KC_A,   KC_S,    KC_D,    KC_F,    TD(2),                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------| 
  KC_LSFT, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,   KC_MUTE,    XXXXXXX,KC_N,    TD(1),  KC_COMM,  KC_DOT, KC_SLSH,  KC_ENT,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
                 KC_LCTL,KC_LALT,KC_LGUI, KC_SPC, LT_ENT_LW,     LT(3,KC_SPACE),LT_SPC_RS,TD(4),KC_LBRC,KC_RBRC







                 
  //           \--------+--------+--------+---------+--------|   |--------+--------+--------+---------+-------/
),

[_LOWER] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  KC_GRV,  KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,                      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  _______, KC_1,    KC_2,   KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_F12,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  _______, KC_EXLM,KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
  _______, KC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR,_______,    _______,KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
                 _______, _______, _______, _______,  _______,    _______, _______, _______, _______, _______

  //           \--------+--------+--------+---------+--------|   |--------+--------+--------+---------+-------/
),

[_RAISE] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, KC_DEL,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, _______,                   _______, _______, KC_UP,   _______, _______, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, KC_PGUP,                   _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, KC_PGDN,_______,   _______,_______, _______, _______, _______, _______, _______,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
                 _______, _______, _______, _______,  _______,    _______, _______, _______, _______, _______
  //           \--------+--------+--------+---------+--------|   |--------+--------+--------+---------+-------/
),

[_MOUSE] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______,KC_DEL,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, _______,                   _______, KC_BTN4, KC_MS_U, KC_BTN5, _______, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, KC_WH_U,                   KC_BTN1, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN2, _______,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, KC_WH_D,_______,   _______,_______, _______, _______, _______, _______, _______,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
                 _______, _______, _______, _______,  _______,    _______, _______, _______, _______, _______
  //           \--------+--------+--------+---------+--------|   |--------+--------+--------+---------+-------/
),

[_NUMPAD] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_HOME, KC_NUM, KC_PSLS, KC_PAST, KC_PMNS, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_END,  KC_P7,  KC_P8,   KC_P9,   KC_PPLS, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_PGUP, KC_P4,  KC_P5,   KC_P6,   KC_PPLS, _______,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,_______,   _______,KC_PGDN, KC_P1,  KC_P2,   KC_P3,   KC_PENT, _______,  
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                 _______, _______, _______, _______,  _______,    _______, _______,  KC_P0,   KC_PDOT, KC_PENT
  //           \--------+--------+--------+---------+--------|   |--------+--------+--------+---------+-------/
),

#ifdef RP2040_BUILD
[_MIDI] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  MI_TRSU,  MI_Ds1, MI_E1,   MI_F1,   MI_Fs1,  MI_G1,                     MI_Ds2, MI_E2,   MI_F2,   MI_Fs2,  MI_G2, MI_TRSU,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  MI_TRSD,  MI_As,  MI_B,    MI_C1,  MI_Cs1,   MI_D1,                     MI_As1,  MI_B1,  MI_C2,   MI_Cs2,  MI_D2, MI_TRSD,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  TD(5),    MI_F,   MI_Fs,   MI_G,   MI_Gs,    MI_A,                      MI_F1,   MI_Fs1, MI_G1,   MI_Gs1,  MI_A1, TD(6),
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  XXXXXXX,  MI_C,   MI_Cs,   MI_D,   MI_Ds,    MI_E,  _______,    _______,MI_C1,   MI_Cs1, MI_D1,   MI_Ds1,  MI_E1, _______,  
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                  MI_OCTD, MI_OCTU, MI_BNDD, MI_BNDU, _______,    _______, _______,  KC_P0,   KC_PDOT, TO(0)
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
[_CHORD1] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  MI_CHTRSU,_______,_______, _______, _______, _______,                   D_MIN,   E_MIN,  F_MAJ,   MI_MDLT, MI_7TH, MI_CHTRSU,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  MI_CHTRSD,_______,_______, _______, _______, _______,                   F_MAJ,  G_MAJ,   A_MIN,   B_DIM,   C_MAJ, MI_CHTRSD,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  TG(5),    _______,_______, _______, _______, _______,                   A_MIN,  B_DIM,   C_MAJ,   D_MIN,   E_MIN, TG(5),
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  _______,  _______,_______, _______, _______, _______,_______,   _______,C_MAJ,  D_MIN,   E_MIN,   F_MAJ,   G_MAJ, _______,  
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                  MI_CHOCTD, MI_CHOCTU, MI_BNDD, MI_BNDU, _______,    _______, _______,  KC_P0,   KC_PDOT, TO(0)
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
[_CHORD2] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  MI_CHTRSU,D_MIN,  E_MIN,   F_MAJ,   MI_MDLT, MI_7TH,                     _______,_______, _______, _______, _______, MI_CHTRSU,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  MI_CHTRSD,F_MAJ,  G_MAJ,   A_MIN,   B_DIM,   C_MAJ,                     _______,_______, _______, _______, _______, MI_CHTRSD,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  TG(5),    A_MIN,  B_DIM,   C_MAJ,   D_MIN,   E_MIN,                     _______,_______, _______, _______, _______, TG(5),
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  _______,  C_MAJ,  D_MIN,   E_MIN,   F_MAJ,   G_MAJ, _______,    _______,_______,_______, _______, _______, _______, _______,  
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                  MI_CHOCTD, MI_CHOCTU, MI_BNDD, MI_BNDU, _______,    _______, _______,  KC_P0,   KC_PDOT, TO(0)
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
#else 
[_MISC] = LAYOUT(
//,------------------------------------------------.                    ,---------------------------------------------------.
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
  _______, _______, _______, _______, _______, _______,_______,   _______,_______, _______, _______, _______, _______, _______,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+--------+--------+--------+--------+---------|
                 _______, _______, _______, _______,  _______,    _______, _______, _______, _______, _______
  //           \--------+--------+--------+---------+--------|   |--------+--------+--------+---------+-------/
),
#endif
};

#ifdef ENCODER_MAP_ENABLE

#undef NUM_ENCODERS
#define NUM_ENCODERS 2
static uint8_t encoder_counter[NUM_ENCODERS] = {0};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),           ENCODER_CCW_CW(KC_COMM, KC_DOT) }, 
    [1] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) },
    [2] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) },
    [3] = { ENCODER_CCW_CW(KC_WH_D, KC_WH_U),           ENCODER_CCW_CW(KC_WH_D, KC_WH_U) },
    [4] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(RGB_SAD, RGB_SAI) },
    #ifdef RP2040_BUILD
        [5] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(MODWL_DN, MODWL_UP) },
        [6] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(MODWL_DN, MODWL_UP) },
        [7] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(MODWL_DN, MODWL_UP) },
    #else
        [5] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) },
    #endif
    // [3] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(RGB_SAD, RGB_SAI) },
};

bool encoder_update_kb(uint8_t index, bool clockwise) {
    // Increment the counter for this encoder
    encoder_counter[index]++;
    
    // Only act on every other rotation
    if (encoder_counter[index] % 2 == 0) {
        uint16_t mapped_code = 0;
        
        // Fetch current layer
        uint8_t layer = biton32(layer_state);
        
        // Fetch appropriate keycode from encoder_map based on the layer and direction
        if (clockwise) {
            mapped_code = pgm_read_word(&encoder_map[layer][index][1]);
        } else {
            mapped_code = pgm_read_word(&encoder_map[layer][index][0]);
        }
        
        // If the keycode is not empty or transparent, tap it
        if (mapped_code != KC_TRNS && mapped_code != KC_NO) {
            tap_code16(mapped_code);
        }
    }

    // Call user function
    return true;
}

#endif

void keyboard_post_init_user(void) {

    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    //debug_mouse=true;

    vial_tap_dance_entry_t td0 = { KC_W,
                                  KC_NO,
                                  KC_NO,
                                  MD_WIN,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(0, &td0);
    vial_tap_dance_entry_t td1 = { KC_M,
                                  KC_NO,
                                  KC_NO,
                                  MD_MAC,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(1, &td1);
    vial_tap_dance_entry_t td2 = { KC_G,
                                  KC_NO,
                                  KC_NO,
                                  MD_GAME,
                                  TAPPING_TERM };
    dynamic_keymap_set_tap_dance(2, &td2);
    vial_tap_dance_entry_t td3 = { QK_GESC,
                                  MO(5),
                                  TG(5),
                                  KC_NO,
                                  120 };
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
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
	
	// Normal macros
        case MD_WIN:
            if (record->event.pressed) {
                // Perform AG_SWAP
                tap_code16(AG_SWAP);  // Send AG_SWAP keycode
                tap_code16(GU_ON);
                #ifdef OLED_ENABLE
                is_ag_swapped = true;
                // is_gui_disabled = false;
                #endif
            }
            return false;
        case MD_MAC:
            if (record->event.pressed) {
                // Perform AG_NORM
                tap_code16(AG_NORM);  // Send AG_NORM keycode
                tap_code16(GU_ON);
                #ifdef OLED_ENABLE
                is_ag_swapped = false;
                // is_gui_disabled = true;
                #endif
            }
            return false;
        case MD_GAME:
            if (record->event.pressed) {
                // Toggle GU_TOGG
                tap_code16(AG_SWAP);  // Send AG_SWAP keycode
                tap_code16(GU_TOGG);  // Send GU_TOGG keycode
                #ifdef OLED_ENABLE
                // is_gui_disabled = true;
                is_gui_disabled = !is_gui_disabled;
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
#ifdef RP2040_BUILD

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
#endif

		default:
            return true;  // Process all other keycodes normally
    }
}
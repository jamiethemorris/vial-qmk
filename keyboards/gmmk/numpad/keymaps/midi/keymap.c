/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
Modified 2022 by rustedaperture for qmk_firmware
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "analog.h"
#include "qmk_midi.h"
#include "vial.h"

enum custom_keycodes {
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
	
	MACRO_0,
	MACRO_1
};

static int8_t octave = 0;  // starts at 0, can be increased or decreased
static int8_t transpose = 0;  // starts at 0, can be increased or decreased

static bool is_key_8_held = false;
static bool is_key_9_held = false;

typedef struct {
    int8_t octave;
    int8_t transpose;
} ChordData;

ChordData chordData[7];

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      NUM      /       *       -
//      7        8       9       +
//      4        5       6       ENCODER
//      1        2       3       RET
//      0                        .

  [0] = LAYOUT(
    TG(2),   KC_PSLS,   KC_PAST,   KC_PMNS,
    KC_P7,   KC_P8,     KC_P9,     KC_PPLS,
    KC_P4,   KC_P5,     KC_P6,     KC_CALC,
    KC_P1,   KC_P2,     KC_P3,     LT(3,KC_PENT),
    LT(1,KC_P0),                         LT(2,KC_PDOT)
  ),
  [1] = LAYOUT(
    MO(0),   KC_F10,    KC_F11,   KC_F12,
    KC_F7,     KC_F8,   KC_F9,     KC_PPLS,
    MACRO_1,  KC_F5,     MACRO_0,   KC_CALC,
    KC_F1,     KC_F2,   KC_F3,     KC_PENT,
    RGB_TOG,                         QK_BOOT
  ),
  [2] = LAYOUT(
    MI_A1,   MI_As1,   MI_B1,    MI_OCTD,
    MI_Fs1,  MI_G1,    MI_Gs1,   MI_OCTU,
    MI_Ds1,  MI_E1,    MI_F1,    TO(0),
    MI_C1,   MI_Cs1,   MI_D1,    MI_BNDU,  // Enter is now pitch wheel up
    LT(3,TO(3)),                 MI_BNDD // Dot is now pitch wheel down
  ),
  [3] = LAYOUT(
    MI_B2,   MI_E2,   MI_C2,   MI_CHOCTD,
    B_DIM,   MI_MDLT,   MI_7TH,    MI_CHOCTU,
    F_MAJ,   G_MAJ,     A_MIN,     TO(0),
    C_MAJ,   D_MIN,     E_MIN,     MI_BNDU,
    TO(2),                         MI_BNDD
  ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
	
	// Normal macros
        case MACRO_0:
            if (record->event.pressed) {
                register_code(KC_LGUI);
				register_code(KC_LSFT);
				tap_code(KC_F5);
				unregister_code(KC_LSFT);
				unregister_code(KC_LGUI);
            }
            return false;  // Skip all further processing of this key
        case MACRO_1:
            if (record->event.pressed) {
                register_code(KC_LSFT);
				tap_code(KC_F5);
				unregister_code(KC_LSFT);
            }
            return false;  // Skip all further processing of this key
			
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

		default:
            return true;  // Process all other keycodes normally
    }
}

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_COMM, KC_DOT) },
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [2] = { ENCODER_CCW_CW(MI_TRSD, MI_TRSU) },
    [3] = { ENCODER_CCW_CW(MI_CHTRSD, MI_CHTRSU) }
};

// Potentiometer Slider, MIDI Control

uint8_t divisor = 0;

uint8_t last_read = 0;
 
void slider(void) {
    uint8_t current_read = (analogReadPin(SLIDER_PIN) +last_read)/8; //filter strength
 
    if (current_read != last_read ) {
        midi_send_cc(&midi_device, 1, 0x01, 0x7F + (analogReadPin(SLIDER_PIN) >>3));
 
    last_read = current_read;
    }
 
}
 
void matrix_scan_user(void) {
    slider();
}

/*
void keyboard_post_init_user(void) {
    rgb_matrix_disable_noeeprom();
}
*/

layer_state_t layer_state_set_user(layer_state_t state) {
	if (layer_state_cmp(state, 2) || layer_state_cmp(state, 3)) {
        rgb_matrix_enable_noeeprom();
    } else {
        rgb_matrix_disable_noeeprom();
    }
    switch (get_highest_layer(state)) {
        case 0:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            tap_code(KC_F13);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 1:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            tap_code(KC_F14);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 2:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            tap_code(KC_F15);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
        case 3:
            register_code(KC_LCTL);
            register_code(KC_LGUI);
            register_code(KC_LALT);
            tap_code(KC_F16);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            unregister_code(KC_LCTL);
            break;
    }
    return state;
}

bool rgb_matrix_indicators_user(void) {
    if (layer_state_is(2)) {  // If on layer 2
        for (uint8_t i = 0; i < DRIVER_1_LED_TOTAL; i++) {
            if (matrix_get_row(i / MATRIX_COLS) & (1U << (i % MATRIX_COLS))) {
                rgb_matrix_set_color(i, 255, 255, 255);  // Set the color of the pressed key to white
            }
        }
        return true;  // Indicate that custom lighting is set
    } else if (layer_state_is(3)) {  // If on layer 3
        for (uint8_t i = 0; i < DRIVER_1_LED_TOTAL; i++) {
            if (matrix_get_row(i / MATRIX_COLS) & (1U << (i % MATRIX_COLS))) {
                rgb_matrix_set_color(i, 153, 255, 204);  // Set the color of the pressed key to your desired RGB value
            }
        }
        return true;  // Indicate that custom lighting is set
    }
    return false;  // Indicate that custom lighting is not set, and default RGB Matrix behavior should be used
}



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

//Sets up what the OLED screens display.

#ifdef OLED_ENABLE

// #include <stdio.h>
// #include "music-bars.c" 

#define ANIM_INVERT false
#define ANIM_RENDER_WPM true
// #define FAST_TYPE_WPM

extern bool is_ag_swapped;
extern bool is_gui_disabled;
extern bool shift_lock;
extern uint8_t encoder_mode_left;
extern uint8_t encoder_mode_right;

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
    };

    oled_write_P(qmk_logo, false);
}

const char *encoder_mode_names[] = {
    "Base ", "Wheel", "Mixer"
};

static void print_status_narrow(void) {
    // Print current layer
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_ln_P(PSTR("Base "), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case 2:
            oled_write_ln_P(PSTR("Arrow"), false);
            break;
        case 3:
            oled_write_ln_P(PSTR("Mouse"), false);
            break;
        case 4:
            oled_write_ln_P(PSTR("Num  "), false);
            break;        
#ifdef RP2040_BUILD
        case 5:
            oled_write_ln_P(PSTR("MIDI "), false);
            break;
        case 6:
            oled_write_ln_P(PSTR("ChrdR"), false);
            break;
        case 7:
            oled_write_ln_P(PSTR("ChrdL"), false);
            break;
#else
        case 5:
            oled_write_ln_P(PSTR("MISC "), false);
            break;
#endif
        default:
            oled_write_ln_P(PSTR("Base "), false);
            break;
    }

    // oled_write_P(PSTR("\n\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    if (led_usb_state.caps_lock) {
        oled_write_P(PSTR("\n"), false);
        oled_write_ln_P(PSTR("CpsLk"), false);
    } else if (shift_lock) {
    oled_write_ln_P(PSTR("SftLk"), false);
    } else {
        oled_write_ln_P(PSTR("     "), false);
    }

    oled_write_P(PSTR("\n"), false);
    // Update OLED based on Alt/GUI swap and GUI disable states
    oled_write_ln_P(PSTR("MODE "), false);
    if (is_gui_disabled) {
        oled_write_ln_P(PSTR("Game "), false);
    } else if (is_ag_swapped) {
        oled_write_ln_P(PSTR("Win  "), false);
    } else {
        oled_write_ln_P(PSTR("Mac  "), false);
    }

    oled_write_P(PSTR("\n"), false);

    /* oled_write_P(PSTR("\nENL:"), false);
    oled_write_char(encoder_mode_left + '0', false);
    oled_write_P(PSTR("\nENR:"), false);
    oled_write_char(encoder_mode_right + '0', false); */

    oled_write_P(PSTR("\nENCDR"), false);
    oled_write_P(PSTR("\n"), false);
    oled_write(encoder_mode_names[encoder_mode_left], false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
        // oled_render_anim();
        // print_status_narrow();
    }
    return false;
}

#endif

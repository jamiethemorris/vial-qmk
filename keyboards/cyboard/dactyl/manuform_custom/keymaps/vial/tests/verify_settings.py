"""Host regression check of actual Vial settings code with simulated EEPROM.
Compares the custom callbacks against the pre-change implementation over all
16-bit keycodes. Hardware services are stubbed; this is not a key-scan test.
"""
from pathlib import Path
import argparse
import re
import subprocess
import tempfile

repo = next(parent for parent in Path(__file__).resolve().parents if (parent/'quantum/qmk_settings.c').is_file())
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--baseline', default='9d7f5c08cf', help='Commit with the original per-key callbacks')
args = parser.parse_args()
keymap_path = 'keyboards/cyboard/dactyl/manuform_custom/keymaps/vial/keymap.c'
old = subprocess.check_output(['git', 'show', args.baseline+':'+keymap_path], cwd=repo, text=True)
new = (repo/keymap_path).read_text()

def function(source, name):
    match = re.search(r'^(?:bool|void|uint16_t) '+name+r'\([^\n]*\)\s*\{', source, re.M)
    assert match, name
    start = match.start()
    end = source.index('{', start)
    depth = 1
    while depth:
        end += 1
        depth += (source[end] == '{') - (source[end] == '}')
    return source[start:end+1]+'\n'

prefix = new[new.index('enum dactyl_layers'):new.index('static int8_t octave')]
prefix += '\n'.join(re.findall(r'^#define (?:LT_|MT_)[^\n]+', new, re.M))
actual_config = (repo/'quantum/keycode_config.h').read_text()
config_type = actual_config[actual_config.index('typedef union keymap_config_t'):actual_config.index('STATIC_ASSERT')]
support = r'''
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "modifiers.h"
#include "quantum_keycodes.h"
#include "cyboard_config.h"
#include "quantum/action.h"
typedef struct combo_t combo_t;
#define AUTO_SHIFT_TIMEOUT 175
#define COMBO_TERM 50
#define TAPPING_TOGGLE 5
#define MOUSEKEY_DELAY 10
#define MOUSEKEY_INTERVAL 20
#define MOUSEKEY_WHEEL_DELAY 10
#define MOUSEKEY_WHEEL_INTERVAL 80
#define AUTO_SHIFT_ALPHA KC_A ... KC_Z
#define AUTO_SHIFT_NUMERIC KC_1 ... KC_0
#define AUTO_SHIFT_SPECIAL KC_MINUS ... KC_SLASH
'''+config_type+r'''
extern keymap_config_t keymap_config;
extern uint8_t mk_delay, mk_interval, mk_max_speed, mk_time_to_max;
extern uint8_t mk_wheel_delay, mk_wheel_interval, mk_wheel_max_speed, mk_wheel_time_to_max;
void clear_keyboard(void);
void eeconfig_update_keymap(const keymap_config_t *config);
void set_autoshift_timeout(uint16_t timeout);
uint8_t dynamic_keymap_get_qmk_settings(uint16_t offset);
void dynamic_keymap_set_qmk_settings(uint16_t offset, uint8_t value);
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record);
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record);
bool get_chordal_hold_default(keyrecord_t *a, keyrecord_t *b);
bool is_flow_tap_key(uint16_t keycode);
bool get_chordal_hold(uint16_t a, keyrecord_t *ar, uint16_t b, keyrecord_t *br);
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev);
'''
services = r'''
#include "qmk_settings.h"
static uint8_t saved[sizeof(qmk_settings_t)];
keymap_config_t keymap_config;
uint8_t mk_delay, mk_interval, mk_max_speed, mk_time_to_max;
uint8_t mk_wheel_delay, mk_wheel_interval, mk_wheel_max_speed, mk_wheel_time_to_max;
void clear_keyboard(void) {}
void eeconfig_update_keymap(const keymap_config_t *config) {}
void set_autoshift_timeout(uint16_t timeout) {}
uint8_t dynamic_keymap_get_qmk_settings(uint16_t offset) { assert(offset < sizeof(saved)); return saved[offset]; }
void dynamic_keymap_set_qmk_settings(uint16_t offset, uint8_t value) { assert(offset < sizeof(saved)); saved[offset] = value; }
bool get_chordal_hold_default(keyrecord_t *a, keyrecord_t *b) { return false; }
bool is_flow_tap_key(uint16_t keycode) { return true; }
'''
checks = r'''
#include "qmk_settings.h"
#include <stdio.h>
bool old_permissive(uint16_t keycode, keyrecord_t *record);
bool old_retro(uint16_t keycode, keyrecord_t *record);
static void set8(uint16_t id, uint8_t value) { assert(qmk_settings_set(id, &value, sizeof(value)) == 0); }
static void set16(uint16_t id, uint16_t value) { assert(qmk_settings_set(id, &value, sizeof(value)) == 0); }
static uint16_t get(uint16_t id) { uint16_t value=0; assert(qmk_settings_get(id, &value, sizeof(value)) == 0); return value; }
static void compare(bool permissive, bool retro) {
    for (uint32_t key = 0; key <= UINT16_MAX; ++key) {
        assert(get_permissive_hold(key, NULL) == (permissive && old_permissive(key, NULL)));
        assert(get_retro_tapping(key, NULL) == (retro && old_retro(key, NULL)));
    }
}
int main(void) {
    qmk_settings_reset();
#ifdef CUSTOM_TEST
    compare(true, true);
    assert(get(1) == 3); // Alt/Ctrl Grave Escape exceptions.
    assert(get(7) == 200 && get(25) == 110);
    assert(get(22) == 1 && get(24) == 1);
    assert(get(3) == 0 && get(23) == 0 && get(26) == 0 && get(27) == 0);
    assert(QS.mousekey_move_delta == 8 && QS.mousekey_max_speed == 4 && QS.mousekey_time_to_max == 15);
    assert(QS.mousekey_wheel_max_speed == 8 && QS.mousekey_wheel_time_to_max == 40);
    assert(!get_retro_tapping(MT_SPC_GUI, NULL));
    set8(22, 0); compare(false, true);
    set8(24, 0); compare(false, false);
    set8(22, 1); compare(true, false);
    set8(24, 1); compare(true, true);
    set8(22, 0);
    set16(7, 237); set16(25, 99); set16(27, 150); set8(26, 1);
    assert(get_flow_tap_term(MT_F_GUI, NULL, KC_A) == 150);
    assert(!get_chordal_hold(MT_F_GUI, NULL, KC_A, NULL));
    memset(&QS, 0, sizeof(QS));
    qmk_settings_init(); // Simulate reconnecting; EEPROM contents persist.
    assert(get(7) == 237 && get(25) == 99 && get(27) == 150 && get(26) == 1);
    compare(false, true);
    assert(!get_retro_tapping(MT_SPC_GUI, NULL));
    uint16_t ids[32];
    qmk_settings_query(0, ids, sizeof(ids));
    for (uint16_t wanted=22; wanted<=27; ++wanted) {
        bool found=false;
        for (unsigned i=0; i<32; ++i) found |= ids[i] == wanted;
        assert(found); // Settings can be discovered by the Vial GUI.
    }
    qmk_settings_reset();
    memset(&QS, 0, sizeof(QS)); qmk_settings_init();
    compare(true, true);
    assert(get(1) == 3 && get(7) == 200 && get(25) == 110);
    assert(get(26) == 0 && get(27) == 0);
    puts("PASS: all 65,536 keycodes match original defaults; GUI toggles, Space/GUI exception, persistence, reset, and discovery");
#else
    assert(get(22) == 0 && get(24) == 0 && get(25) == 200 && get(1) == 0);
    set8(22, 1); set8(24, 1);
    for (uint32_t key=0; key<=UINT16_MAX; ++key) {
        assert(get_permissive_hold(key, NULL));
        assert(get_retro_tapping(key, NULL));
    }
    puts("PASS: default Vial hooks retain global behavior for other keyboards");
#endif
}
'''
with tempfile.TemporaryDirectory(prefix='vial-settings-test-') as directory:
    p=Path(directory)
    (p/'quantum').mkdir()
    (p/'quantum/action.h').write_text('#pragma once\n#include <stdbool.h>\ntypedef struct { int unused; } keyrecord_t;\n')
    for name in ['qmk_settings.h','qmk_settings.c']:
        (p/name).write_bytes((repo/'quantum'/name).read_bytes())
    (p/'cyboard_config.h').write_bytes((repo/'keyboards/cyboard/config.h').read_bytes())
    for name in ['dynamic_keymap.h','process_auto_shift.h','mousekey.h','process_combo.h','action_tapping.h','keycode_config.h']:
        (p/name).write_text('#pragma once\n')
    (p/'progmem.h').write_text('#define PROGMEM\n#define pgm_read_byte(p) (*(const uint8_t *)(p))\n#define pgm_read_word(p) (*(const uint16_t *)(p))\n#define pgm_read_ptr(p) (*(p))\n')
    (p/'support.h').write_text(support)
    (p/'keymap_defines.h').write_text(prefix)
    (p/'services.c').write_text(services)
    baseline = function(old,'get_permissive_hold').replace('get_permissive_hold','old_permissive')
    baseline += function(old,'get_retro_tapping').replace('get_retro_tapping','old_retro')
    (p/'baseline.c').write_text(baseline)
    (p/'custom.c').write_text('#include "qmk_settings.h"\n'+''.join(function(new,name) for name in ['qmk_settings_reset_user','qmk_settings_permissive_hold_user','qmk_settings_retro_tapping_user']))
    (p/'checks.c').write_text(checks)
    for custom in [True, False]:
        cmd=['cc','-std=gnu11','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unused-function','-DQMK_SETTINGS','-DMOUSEKEY_ENABLE','-I'+str(p),'-I'+str(repo/'quantum'),'-I'+str(repo/'quantum/keymap_extras'),'-I'+str(repo/'quantum/sequencer'),'-include',str(p/'support.h'),'-include',str(p/'keymap_defines.h')]
        cmd += [str(p/name) for name in ['qmk_settings.c','services.c','baseline.c','checks.c']]
        if custom: cmd += ['-DCUSTOM_TEST',str(p/'custom.c')]
        cmd += ['-o',str(p/'check')]
        subprocess.run(cmd,check=True)
        subprocess.run([str(p/'check')],check=True)

# Custom Cyboard Vial settings

This keymap exposes **QMK Settings** in Vial 0.7.4 or newer. Under
**QMK Settings → Tap-Hold**, change a value and click **Save** to apply it.
Saved settings survive reconnecting. Resetting QMK Settings restores these
custom defaults:

| Setting | Default |
| --- | --- |
| Tapping Term | 200 ms |
| Quick Tap Term | 110 ms |
| Permissive Hold | Enabled for the existing key whitelist |
| Retro Tapping | Enabled, except Space/GUI |
| Hold On Other Key Press | Disabled |
| Chordal Hold | Disabled |
| Flow Tap | 0 ms (disabled) |
| Auto Shift | Disabled |

The Permissive Hold checkbox controls the original whitelist in
`qmk_settings_permissive_hold_user()`. It does not enable Permissive Hold on
other keys. The Retro Tapping checkbox controls all eligible keys except
`MT_SPC_GUI`, which always has Retro Tapping disabled. These filters preserve
the original behavior while allowing the GUI toggles to turn it off.

Flow Tap and Chordal Hold are independent, optional changes to tap/hold
behavior. Enabling them can affect Space/GUI even though its Retro Tapping
exception remains in place.

The original mouse-key defaults and Alt/Control Grave Escape overrides are
also preserved. Tap Dance entries 0–10 remain intentionally hard-coded and
are reapplied at startup. Tap Dance uses its own timing and decision logic;
the global tap/hold controls do not replace it.

The small hooks in `quantum/qmk_settings.c` let this keymap supply per-key
filters and reset defaults. Their default implementations keep Vial's usual
behavior for other keyboards. Defaults are written only when settings are
initialized or reset, not on every startup.

## Building and flashing

With the QMK and ARM tools on PATH, build from the repository root:

```sh
make cyboard/dactyl/manuform_custom:vial -j4 USE_CCACHE=no
```

Export the current layout using Vial's **File → Save current layout** before
flashing. Enabling QMK Settings changes the EEPROM layout, and Vial's build
identifier causes a new build to initialize saved data. Restore desired
dynamic mappings afterward; keep the defaults above unless intentionally
changing them. Updating source or building does not update the connected
keyboard until the UF2 is flashed.

## Regression check

From the repository root, with Python 3 and a host C compiler available:

```sh
python3 keyboards/cyboard/dactyl/manuform_custom/keymaps/vial/tests/verify_settings.py
```

This compiles the actual Vial settings implementation with simulated EEPROM
and compares both custom filters against the pre-change commit across all
65,536 keycodes. It also checks GUI setting discovery, toggles, saved-setting
reload, reset defaults, and unchanged behavior for keymaps without custom
hooks. It does not replace a typing test on the physical keyboard.

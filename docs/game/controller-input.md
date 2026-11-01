# Controller input

`PadState` in `include/rnc/input/pad_state.h` describes the record at
`D_0013C940`. The boot producers are `poll_pad_device_state` (`FUN_002170c8`),
`clear_pad_input` (`FUN_002172c0`) and `process_pad_input` (`FUN_00217328`).

| Offset | Field | Meaning |
|---|---|---|
| `0x100`, `0x140` | `analog`, `analog_prev` | Stick axes and pressure values, then their copy |
| `0x180`, `0x184` | `button_profile`, `vibration_profile` | SDK device profiles |
| `0x1A0`, `0x1A4`, `0x1A8` | `held`, `pressed`, `released` | Button masks including stick directions |
| `0x1B0`–`0x1BC` | `raw_*`, `prev_raw_held` | Button-only masks and previous poll |
| `0x1C0`–`0x1C8` | `*_unmasked` | Masks before mode filtering, with mirror swap undone |
| `0x1E0`, `0x258`, `0x2D0` | `pressed_history`, `angle_history`, `float_history` | Thirty-sample button, angle and magnitude rings |
| `0x348` | `saved_held` | Held mask before mode filtering, used by the next poll |
| `0x34D`–`0x34F` | `repeat_delay`, `repeat_rate`, `repeat_timer` | Button-repeat timing |

`pad_held` (`D_0013CAE0`) and `pad_pressed` (`D_0013CAE4`) name the two
interior words separately because retail code addresses them directly.
Their `_lui` aliases preserve non-small-data access. The controller record
also retains a `_lui` alias; pyrocitor uses the second name to preserve address
reconstruction between button tests.

Signed and unsigned views are intentional. Some menu/debug/movie paths load
two button words together; their `PadStateWords` member views or explicit `u64`/PS2 `long` accesses
must not become two independent loads. The member views also preserve the
compiler's base-plus-field addressing where a scalar pointer cast does not. Pause-menu entry reads the unmasked pressed
word through a volatile signed view. The save-loading menu reads `raw_pressed`,
not `held`.

These names and layouts follow the existing matching producer and consumer C.
Byte equality qualifies this cleanup; it does not establish native gameplay
or new PS2 runtime coverage.

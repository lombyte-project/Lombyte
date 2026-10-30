# Game-side voice allocation

The EE game owns a 30-slot request pool at `D_0013E550`. A slot index is not
a sound-driver handle or hardware voice number.

## Functions

The C names describe what the code does; they are not claimed to be
Insomniac's original names.

| C name | USA symbol | What it does |
|---|---|---|
| `allocate_voice_for_target_entry` | `FUN_0022da68` | Read a moby's class sound definition by index, queue it, record owner and entry index; return a slot or -1 |
| `allocate_voice_slot` | `FUN_0022d7f0` | Find a free game-side slot and initialize a queued start; return a slot or -1 |
| `release_voice_slot` | `FUN_0022d798` | Cancel a queued start or request asynchronous stop; ignore negative indices |
| `sound_update` | `FUN_0022ca50` | Evaluate requests and issue sound-driver commands |
| `voice_start_callback` | `FUN_0022dd90` | Store the start handle; advance state 1 to 2 on success, free on failure |
| `voice_update_callback` | `FUN_0022ddd8` | Store the returned handle; clear owner/context and free when the handle is zero |

## States

Only some states have names so far.

| Value | Name | Evidence |
|---|---|---|
| 0 | `VOICE_SLOT_FREE` | Allocator searches for it; completion callback clears to it |
| 4 | `VOICE_SLOT_STOP_REQUESTED` | Release helper writes it; updater schedules a stop |
| 6 | `VOICE_SLOT_STOP_PENDING` | Updater writes it after sending stop and requesting a completion query |
| 7 | `VOICE_SLOT_START_QUEUED` | Allocator writes it; updater sends play and advances to state 1 |

A start queued in state 7 can be cancelled without contacting the driver.
For a started request, the release helper leaves the slot occupied while
the updater and callback finish stopping it.

The ordinary search covers the first 26 slots. The hero, its first item, and
class `0x472` may search all 30. A definition rejected by the source-state/flag
test, a full pool, or volume below `0x20` returns -1.

The target-entry helper checks only the upper entry bound. Release ignores
negative indices but does not check the upper slot bound.

## Layouts

- Pool (`VoicePool`, `0xE00` bytes): a `0x70`-byte header (listener history,
  group volumes, reverb), thirty `0x70`-byte voices, then at `0xD90` the
  level sound instances (`0x90` each) and at `0xDA0` the listener occlusion
  samples. Definition stride: `0x20`.
- `VoicePoolWindow` is an access view at `pool_base + index * 0x70`.
  Its `voice` begins at `+0x70`; it is not an array element to increment.
- `MobyAudioView` and `MobyClassAudioView` are local partial views.
  The `+0x24` class pointer and class `+0x28` sound definitions agree with
  `include/rnc/mobys/moby.h`. The count at class `+0x0D`
  is inferred from the bounds check.
- Definition `+0x1A` is the sound index and `+0x1C` the bank handle passed by
  `sound_update` to `snd_play_sound_vol_pan_pmpb`.
- Slot `+0x1C` is cleared as owner context, not unused padding.
- Slot `+0x44` holds 36 occlusion-history bytes, then eight bytes not yet read.

`src/check/voice_pool_layout_check.c` checks the EE offsets, sizes and alignment.

## OpenRAC comparison

[OpenRAC's sound.c](https://github.com/OpenRAC/rac1-decomp/blob/05da3217a4046ac01dec3ac74f051a348f063155/src/game/sound.c)
has analogous release and target-entry helpers at `func_0022EAB0` and
`func_0022ED80`; allocation at `func_0022EB08` remains an assembly stub.
They back up how the data is used, not the names, and are not independent
proof: both projects share earlier research.

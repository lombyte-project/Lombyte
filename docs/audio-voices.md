# Game-side voice allocation

Start with `src/audio/voices/allocate_voice_for_target_entry.c`, then
`allocate_voice_slot.c`, `sound_update.c`, and `release_voice_slot.c`.
This module owns requests in the EE game's 30-slot pool at `D_0013E550`.
A returned slot index is not a sound-driver handle or a hardware voice number.

## Module boundaries

| Layer | Sources | Responsibility |
|---|---|---|
| Game-side voice lifecycle | `src/audio/voices/allocate_voice_*.c`, `release_voice_slot.c`, `sound_update.c` | Select definitions, reserve slots, calculate parameters, queue starts and stops |
| Completion callbacks | `src/audio/sound/voice_start_callback.c`, `voice_update_callback.c` | Receive driver handles and finish slot transitions |
| Sound-driver command client | `src/audio/voices/snd_play_sound_vol_pan_pmpb.c`, `snd_stop_sound.c`, `snd_sound_is_still_playing_cb.c` | Submit commands to the sound library |
| Command transport | `src/audio/rpc/`, `src/audio/snd_flush_sound_commands.c` | Buffer and send commands |
| Layout and states | `include/rnc/audio/voice_pool.h`, `voice_state.h` | Shared allocation view and the lifecycle states used by this pilot |

The target-entry allocator belongs beside the bank/group-entry allocators in
`audio/voices/`. Its unit moved there without changing its link position.
The callback and driver files retain their current paths. `audio/voices/`
currently mixes game-side lifecycle and driver-client wrappers: these are
conceptual boundaries, not an assertion that the directory separation is
already complete. The table provides navigation without a directory-wide
migration.

## Functions and names

These C names describe observed behavior; none is claimed as an original
Insomniac function name.

| C name | USA symbol | Contract |
|---|---|---|
| `allocate_voice_for_target_entry` | `FUN_0022da68` | Read a moby's class sound definition by index, queue it, record owner and entry index; return a slot or -1 |
| `allocate_voice_slot` | `FUN_0022d7f0` | Find a free game-side slot and initialize a queued start; return a slot or -1 |
| `release_voice_slot` | `FUN_0022d798` | Cancel a queued start or request asynchronous stop; ignore negative indices |
| `sound_update` | `FUN_0022ca50` | Evaluate requests and issue sound-driver commands |
| `voice_start_callback` | `FUN_0022dd90` | Store the start handle; advance state 1 to 2 on success, free on failure |
| `voice_update_callback` | `FUN_0022ddd8` | Store the returned handle; clear owner/context and free when the handle is zero |

Retain these established public names during the pilot. In particular,
`release_voice_slot` does not promise immediate release. A future name such as
`request_voice_stop` needs a complete caller migration, not a second alias.
Likewise, the target-entry allocator's target is a moby, now explicit in its
parameter and partial type names.

Some callers remain misleading: for example,
`src/overlays/l02/gameplay/hero/0021b698.c` declares
`truncate_float_to_s32_cf` against `FUN_0022da68`. Resolve callers by their
assembly labels, not by these local spellings. This pilot does not normalize
the 135 files found by searching the target symbol and its canonical name at
base `d3765ad26a7d8db44e151be306a817af32d7be3a`.

## State and ownership

The named states are a partial vocabulary, not the complete state machine.

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
class `0x472` may search all 30. The class's gameplay identity is not needed
to describe this rule and is not inferred here. A definition rejected by the
source-state/flag test, a full pool, or volume below `0x20` returns -1.

Preserve retail's preconditions: the target-entry helper checks only the upper
entry bound; release ignores negative indices but does not check the upper
slot bound. Adding validation would change behavior.

## Layouts a reviewer should know

- Pool header: `0x70` bytes. Slot stride: `0x70` bytes. Definition stride: `0x20`.
- `VoicePoolWindow` is an access view at `pool_base + index * 0x70`.
  Its `voice` begins at `+0x70`; it is not an array element to increment.
- `MobyAudioView` and `MobyClassAudioView` are local partial views.
  The `+0x24` class pointer and class `+0x28` sound definitions agree with
  `include/rnc/gameplay/entities/moby.h`. The count at class `+0x0D`
  is inferred from the bounds check.
- Definition `+0x1A` is the sound index and `+0x1C` the bank handle passed by
  `sound_update` to `snd_play_sound_vol_pan_pmpb`.
  The allocator's unsigned 16-bit view is retained; the updater uses signed
  16-bit access. A shared name is not permission to change signedness.
- Slot `+0x1C` is cleared as owner context, not unused padding.
- Storage at slot `+0x44` includes 36 occlusion-history bytes and eight bytes
  not interpreted by this view; it is not a 44-sample history.

`src/check/voice_pool_layout_check.c` checks the EE offsets, sizes and alignment.
The updater still has its own richer views. Consolidating them requires
separate per-unit comparisons, including access width/sign and alignment.
The old-style allocator declaration and integer address arithmetic in the
target-entry helper also remain deliberate matching constraints.

## Research and provenance

1. **Current USA source:** the functions above, `sound_update`'s play/stop
   branches, the callbacks, and `moby.h` establish this pilot's names and
   offsets. Binary matching validates placement and generated bytes;
   it does not establish an original source name.
2. **PAL comparison:** [OpenRAC sound.c at 05da3217](https://github.com/OpenRAC/rac1-decomp/blob/05da3217a4046ac01dec3ac74f051a348f063155/src/game/sound.c)
   contains analogous release and target-entry helpers as `func_0022EAB0`
   and `func_0022ED80`; allocation `func_0022EB08` remains an assembly stub.
   It corroborates the control/data-access pattern, not original function
   names. Its `sound.cpp` note is a module clue. Shared research ancestry
   means agreement alone is not independent proof.
3. **OpenGOAL:** [VagVoice and VoiceManager at 6076aab5](https://github.com/open-goal/jak-project/blob/6076aab5a95955443a9e1ed8f72a25bc0e2fcf9d/game/sound/989snd/vagvoice.h)
   represent tones and playback voices in its 989SND implementation.
   They do not establish the layout or names of R&C's game-side request pool.
   Its [project overview](https://github.com/open-goal/jak-project/blob/6076aab5a95955443a9e1ed8f72a25bc0e2fcf9d/docs/project-overview.md)
   is useful for separating game/runtime and sound-library responsibilities.
   No OpenGOAL code or structures are imported.
4. **Development-build notes:** [engine-source-layout.md](engine-source-layout.md)
   records the `989snd.c` EE client and IOP interface. This supports researching
   the driver boundary, not equating game-side slots with driver voices.

## Review and matching

Keep assembly labels, aliases, compiler routes, expression order, widths and
signedness. Review layout/name changes separately from later prototype and
control-flow changes.

Focused units are `audio/voices/allocate_voice_for_target_entry`,
`audio/voices/allocate_voice_slot`, and `audio/voices/release_voice_slot`.
The shared header also has overlay readers: the final tree needs both
`make elf` and `make overlays`, public checks, cleanup-delta checks and an
independent review. A layout check or a host build alone is not exact-match
evidence.

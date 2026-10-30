# Enum scope and shared definitions

Keep one enum for each state, mode or ID selector. Reuse its existing members,
with one canonical name per numeric value. Put consumer-specific details in
comments.

## Where definitions belong

- Used by several C files: the owning subsystem's header.
- Used by several functions in one C file: file scope in that file.
- Used by one function: inside that function.

These shared definitions live under `include/`:

| Header | Defines |
|---|---|
| `rnc/hero/hero_state.h` | Hero states, vehicle modes and gravity modes |
| `rnc/mobys/moby_class_id.h` | Moby class IDs |
| `rnc/camera/camera_modes.h` | Camera activation and attachment modes |
| `rnc/rendering/pixel_format.h` | GS pixel storage formats |
| `rnc/save/memory_card_state.h` | Card operations and their separate substeps |
| `rnc/ui/text/text_region.h` | Text layout flags |

`hero.h` includes `hero_state.h`.

## Combining enums

Extend the existing enum when constants describe the same selector. Keep
independent selectors separate, even when their values or prefixes overlap.
For example, memory-card operations and operation substeps, camera activation
and attachment modes, and moby class IDs and entity states are separate enums.

Keep numeric values and stored integer types unchanged. Check code changes with
`make elf`, `make overlays`, `make check` and `python3 scripts/check-cleanup.py`;
the retail bytes must still match.

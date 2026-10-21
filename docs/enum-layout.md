# Enum scope and shared definitions

The enum audit covered all 815 definitions in `src/` and `include/`, including
the SDK and runtime code. The consolidation leaves 702 definitions: 687 in C
files and 15 in headers. It changes constant declarations and names, not the
stored types, numeric values, function signatures or game logic.

## Shared headers

| Header | Domains |
|---|---|
| `rnc/gameplay/hero_state.h` | `HeroStateId`, `HeroVehicleMode`, `HeroGravityMode` |
| `rnc/gameplay/entities/moby_class_id.h` | `MobyClassId` for the class ID at `Moby.oclass` |
| `rnc/gameplay/camera/camera_modes.h` | `CameraActivationMode` and `CameraAttachMode` |
| `rnc/rendering/pixel_format.h` | `PixelStorageFormat` for GS image/texture formats |
| `rnc/storage/memory_card/memory_card_state.h` | Card operations and six independent sets of operation substeps |
| `rnc/ui/text/text_region.h` | The existing `TextRegionFlags` definition |

`hero.h` includes `hero_state.h`. The former header macros and function-local
`HERO_STATE_*` declarations now use that shared enum, including the crank state.
Gravity helpers use one set of three names for the same hero gravity-mode
field; vehicle-mode consumers likewise share their mode IDs. The fields remain
their original integer types.

Some legacy hero-state names represented different values. Vehicle states now
distinguish mode 1 from mode 3. Other conflicting aliases carry an `Lxx` level
qualifier; the level 16 entry and transition aliases also distinguish those
contexts. These qualifiers preserve what the existing code proves without
assigning one ambiguous name a universal value.

Other descriptive hero-state aliases remain when the meaning is specific to
one consumer. Equal numbers alone do not prove that two descriptions can be
replaced by a single behavioral name.

## Definitions private to a C file

Repeated constants used by several functions in one translation unit stay in
that file, with one definition for each domain:

| Domain | C file |
|---|---|
| Taxi stops | `overlays/l05/gameplay/entities/electric_gate.c` |
| Shuttle routes | `overlays/l06/gameplay/entities/fred.c` |
| Taxi commands | `overlays/l07/gameplay/entities/taxi_platform.c` |
| Paired gate states | `overlays/l10/gameplay/entities/screamer.c` |
| Resort-owner states and path legs, separately | `overlays/l11/gameplay/entities/bob.c` |
| Asteroid spin axes | `overlays/l13/gameplay/entities/blarg_space_fighter.c` |
| Taxi stops | `overlays/l14/gameplay/entities/jet.c` |
| Robot-dog states and shuttle stops, separately | `overlays/l17/gameplay/entities/door.c` |
| Drek missile states | `overlays/l18/gameplay/entities/drek_fight_missile.c` |
| Bolt-crank states | `overlays/shared/gameplay/entities/novalis_elevator.c` |

The paths above are relative to `src/`. These files sometimes contain several
entity classes; that does not make all their state machines one domain.

## Keep distinct domains distinct

- Memory-card operation IDs are separate from probe, create, delete, read,
  update and full-write substeps. The six step enums describe different uses
  of `MemoryCardState.sub`; their overlapping values do not combine them.
- Camera activation is separate from camera attachment mode and from the
  priority callback's return value.
- Drek's boss state and encounter phase remain separate: they select different
  fields. The walker gait enum already describes one private gait selector.
- A moby class ID is separate from its state and command bytes. The bolt update
  now reuses the shared class IDs while keeping its private state enum.
- Hit responses, entity states and flashing substeps remain separate. Mixed
  declarations in the bombomatic, path-child, fighter and activation-pad
  callbacks have been split along those selector boundaries.
- The hit-shape dispatch constants are combined in their one consuming
  function. Runtime print bases and SDK floating-point classification keep
  their implicit sequences and local declarations.

Most other enums belong to a single callback. They remain local: generic names
such as `INITIALIZE` or `HIT_REACTION_NONE` can describe unrelated state machines
and can have different values. Neither matching prefixes nor matching numbers
justify a common header.

## Extending an enum

Reuse a visible shared constant when both its value and role match. Add another
member to that domain's existing definition rather than creating a second enum
in a caller. Keep a new private selector local until it has another consumer.
Do not merge independent selectors merely because their members share a prefix.

Keep values explicit, preserve the original integer types and check the result
with `make elf`, `make overlays`, `make check` and
`python3 scripts/check-cleanup.py`. Enum relocation is a readability change;
it must preserve the retail bytes.

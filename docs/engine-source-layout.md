# Engine source layout (recovered)

Reference notes on the original engine's source-file and module naming,
recovered from development-build metadata during decompilation research.

> [!NOTE]
> **Provenance.** Recovered from a June 2002 preview build. None of these
> identifiers appear in the retail USA disc image (`SCUS_971.99`), so this is
> reference material only: not a build input, and not to be added to `config/`
> without independent address evidence. No code, addresses, string text, or
> assertion expressions are reproduced here — only identifiers.

## What this is

Development builds carry per-check assertion metadata naming the source file of
each check, and their debug UI lists profiler and module names. That documents
how the engine was organised: which source files make up the game, what is
built per level, and which identifiers belong to the moby, HUD, map and save
subsystems. It is useful context when naming boot-executable functions and for
the long-term PC runtime; it does not name any retail unit by itself.

## Core modules

| Module         | Subsystem (inferred from name and call context)  |
| :------------- | :----------------------------------------------- |
| `db_core.cpp`  | debug core: print buffer and assertion plumbing  |
| `debug.cpp`    | developer debug display / profiler front-end     |
| `framebuf.cpp` | framebuffer and render-target setup              |
| `freeze.cpp`   | game-freeze / save-prompt handling               |
| `hud.cpp`      | HUD bank loading and drawing                     |
| `init.cpp`     | level/system initialisation                      |
| `loaders.cpp`  | asset and level loaders                          |
| `map.cpp`      | world map, collision block cache, occlusion grid |
| `memcard.h`    | memory-card helpers used by save code            |
| `mission.cpp`  | mission/objective state                          |
| `mobyfunc.cpp` | moby (entity) helper functions                   |
| `mobyutil.cpp` | moby utility functions                           |
| `npc.cpp`      | NPC behaviour                                    |
| `pause.cpp`    | pause menu                                       |
| `save.cpp`     | save/load handling                               |
| `vuchain.cpp`  | VU microcode chain management                    |

## Per-level modules

`hero1.cpp` … `hero8.cpp` — hero (player) code, one module per level.

## Per-class moby update modules

`update/mobyNNN.cpp` — per-moby-class update code. Class numbers observed:
`190`, `192`, `217`, `258`, `333`, `340`, `367`, `424`, `563`, `572`, `623`,
`831`, `998`, `1048`, `1066`. Classes 190 and 192 appear in every level build
examined; the rest are level-specific. This is the observed set, not a
guaranteed complete list.

## Recovered identifiers

Constants referenced by engine code include: `MAX_IMOBYS`, `MobyGroupCnt`,
`MobyInstances`, `MobyInstancePermEnd`, `USED_HUD_BANKS`, `PRF_MAX_TIMINGS`,
`PRINT_BUFFER_MAX`, `PRINT_BUFFER_SIZE`, `MIDI_PAGE`, `TOTAL_GADGETS`,
`GADGET_UNDEFINED`, `IT_GADGET`, `MAP_BLOCKS`, `MAP_BLOCK_SIZE`,
`MAP_WORK_SIZE`, `OFFER_TEXT_LEN`, `VSYS_GADGET_COUNT`.

Structure fields referenced by engine code include: `UID`, `group`, `pVar`
(moby), `focus` and `items` (debug focus), `state` (manipulator), `cpuIndex`
and `gsIndex` (profiler). Treat these as hints for naming, not as proof of any
specific unit.

## Render pipeline stages

The debug profiler lists frame stages in order: render setup, sky draw,
pre effects, vu effects, moby effects, part draw, post effects, aa blur,
screen overlays.

## Scope

The matching decompilation targets the retail boot executable only. Per-level
`update/*` and `hero*` code lives in the level code overlays, which are out of
scope until the executable is done (see
[progress-metrics.md](progress-metrics.md#level-overlays)); the module list above
is provided as engine context for naming and for the future runtime, not as a
work queue.

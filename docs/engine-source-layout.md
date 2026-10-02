# Engine source layout (recovered)

Reference notes on the original engine's source-file and module naming,
recovered from development-build metadata during decompilation research.

> [!NOTE]
> **Provenance.** Recovered from 2002 development builds, interactive demo discs,
> and prototypes (including the June 2002 preview, June 25 prototype, and
> September 9 review build). None of these identifiers appear in the retail USA
> disc image (`SCUS_971.99`), so this is reference material only: not a build
> input, and not to be added to `config/` without independent address evidence.

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

## IOP Stash subsystem (`IOPSTASH.IRX`)

Unstripped STABS / `.mdebug` debug symbols preserved in `IOPSTASH.IRX` (recovered
from demo discs and the June 25, 2002 prototype) reveal the original IOP-side
driver for the engine's streaming stash buffer:

- **Source path**: `C:\code\i5\stash/iopstash.c` (author initials `jms` — John M. Spinale).
- **Module descriptor**: `IOP_Stash_Driver`.
- **Buffer size**: 512 KiB static buffer (`stash_ram`, `0x80000` bytes).
- **RPC server ID**: `0x11` (17).

### Stash RPC commands

| Command ID | Enum constant     | Direction | Description                                                    |
| :--------- | :---------------- | :-------- | :------------------------------------------------------------- |
| `0`        | `IOP_STASH_SEND`  | EE -> IOP | Send data block from EE memory to the IOP stash buffer         |
| `1`        | `IOP_STASH_FETCH` | IOP -> EE | Retrieve cached data block from the IOP stash buffer to EE RAM |
| `2`        | `IOP_STASH_INFO`  | IOP -> EE | Query base address and size of the IOP stash buffer            |

### Stash type layouts

Authentic struct definitions from `.mdebug` STABS types:

```c
typedef struct StashInfo {
    int base;
    int size;
    sceSifClientData cd;
    int free;
    int block;
} StashInfo;

typedef struct StashBlock {
    int ram;
    int qwc;
    int comment;
    int pad;
} StashBlock;

typedef struct StashFetch {
    int ram;
    int pad[3];
} StashFetch;

typedef struct StashGetInfo {
    int base;
    int size;
    int pad[2];
} StashGetInfo;
```

These definitions resolve the placeholder types previously used in:
- `src/storage/cd/fun_00232d00.c` (initialisation of `D_001DD1A0` / `StashInfo` and the 64-entry `D_001DD1D8` / `StashBlock` table, calling `IOP_STASH_INFO`).
- `src/world/data/stash_send_data.c` (`stash_send_data`, implementing `IOP_STASH_SEND`).
- `src/storage/stash/stash_receive_data.c` (`stash_receive_data`, implementing `IOP_STASH_FETCH`).

## Sound driver subsystem (`989snd.c`)

The Sony 989 Studios audio driver EE client source path is recovered as
`/usr/local/989snd/ee/989snd.c`. Exported symbols and function prototypes
interfacing with `989SND.IRX` include:

- `snd_BankLoad`
- `snd_BankLoadByLoc`
- `snd_BankLoadFromEE`
- `snd_BankLoadFromEE_CB`
- `snd_BankLoadFromIOP`
- `snd_BankLoadFromIOP_CB`
- `snd_SendIOPCommandNoWait`

## Internal diagnostics and developer attributions

Internal assertion formatting and diagnostic printouts preserve initials of the
original Insomniac Games programming team:

- **`RAR`** (Rich A. Rayl): Collision and camera test pipelines (`Camera_CollPrimTest WARNING! - grid out of bounds! (RAR)`).
- **`TJB`** (Ted J. Baker): Environment sampling and lighting calculations (`TJB - No env sample point found`).
- **`MB_CheckCollPill`**: Moby collision pill (cylinder / capsule) intersection testing.
- **`ParsePermGsRam()`**: Permanent GS RAM layout validation.
- **`IMoby`**: Entity C++ base class referenced in entity validation checks (`WARNING: Moby %d (class %d) thinks it's an IMoby and it's not.`).

## Pre-release milestone observations

Analysis of the September 9, 2002 review build (`SCUS_971.99`) reveals
development milestone markers:

- **Hardware dongle protection**: The review build is guarded by WIBU-SYSTEMS WibuKey USB hardware key authentication (`WIBU.IRX`, `USBD.IRX`, and client RPC `wibu_ee`).
- **Pause menu debug trigger**: Entering `Up`, `Down`, `Up`, `Down`, `Left`, `Right`, `Left`, `Right`, `Square` while paused displays `***cheats enabled***` and binds the in-game Debug Menu to `R3`.
- **Level authoring sequence**: Level 18 (Veldin 2, `level18.wad`) is entirely absent from the disc image, indicating it was the final level authored and integrated prior to the retail master.

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

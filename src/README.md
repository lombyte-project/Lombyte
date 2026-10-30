# Source layout

`src/` is grouped by what the code is in the game:

| Folder | What goes there |
|---|---|
| `game/` | main loop, level start, transitions between levels, missions |
| `mobys/` | the moby (game object) system: create, delete, update, animation |
| `camera/`, `weapons/` | the camera, weapons and gadgets (Ratchet's own code is level code, in `overlays/shared/hero/`) |
| `world/` | level data tables, loaders, collision |
| `streaming/`, `save/` | disc reads, WAD and level streaming; memory card saves |
| `rendering/` | one folder per pass (`tfrags`, `ties`, `shrubs`, `mobys`, `sky`) plus effects, packets, textures, view and frame setup |
| `ui/` | HUD, menus (one folder per screen), map, help, text |
| `audio/`, `movies/`, `input/`, `math/`, `runtime/` | sound and music, the FMV player, pads, math helpers, engine core |
| `sdk/` | Sony and newlib libraries, by library |
| `check/` | struct layout checks (`make check`) |

- A file is named after the function it holds (`ui/map/coverage/save_map_coverage.c`);
  `fun_<address>.c` means the function is not understood yet.
- `assembly/` holds units that build from the retail assembly (the oracle),
  under the same subsystem names. `assembly/textbin/` units come from raw-word
  segments; the oracle tooling finds them by that prefix.
- `overlays/` holds the level code, built by `make overlays` and not linked
  into the boot executable ([docs/build/overlays.md](../docs/build/overlays.md)):
  - `shared/`: code every level carries, grouped like the boot tree plus
    `enemies/`, `pickups/`, `props/`, `vehicles/`, `cutscenes/` and `vendor/`;
  - `lNN/`: one level's own code: `hero/`, `mobys/`, and level-wide files
    (class tables, sky, water) at its root.

Rules:

- Name folders after game concepts, never after tools or vague words.
- Place a file by what its functions do, not by the file next to it.
- No folder with only one file, and at most three folders deep.
- Headers mirror the source path under `include/rnc/` (`include/rnc/overlay/` for level code).

Each file's owner path in `config/us/rnc1.us.yaml` must match its location.
Moving or renaming a file follows [docs/contributing/code-cleanup.md](../docs/contributing/code-cleanup.md).

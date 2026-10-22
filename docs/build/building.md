# Building

## Quick setup

```sh
git clone https://github.com/lombyte-project/lombyte.git && cd lombyte
./setup.sh --iso /path/to/your-ratchet-and-clank-usa.iso
```

This installs the tools, takes the game files from your disc image and
rebuilds the game. It ends with `PASS: reconstructed boot ELF matches retail`.
Next step: [CONTRIBUTING.md](../../CONTRIBUTING.md).

| Platform                          | How                                                                                                 |
| :-------------------------------- | :-------------------------------------------------------------------------------------------------- |
| Linux (Ubuntu 24.04+, Debian 13+) | the two lines above                                                                                 |
| Windows                           | `wsl --install -d Ubuntu` in PowerShell, reboot, open Ubuntu, run the two lines above              |
| macOS, other Linux                | with [Docker](https://docs.docker.com/get-docker/): `./setup.sh --docker --iso ...`, then `./setup.sh --shell` |

Other options:

| Command                          | What it does                                                   |
| :------------------------------- | :------------------------------------------------------------- |
| `./setup.sh --elf SCUS_971.99`   | use the executable instead of the disc image                   |
| `./setup.sh --check`             | show what is installed                                         |
| `./setup.sh --no-build`          | install only, do not build                                     |
| `./setup.sh --help`              | everything else                                                |

## What setup does

- Downloads the public toolchain into `tools/` and checks every file against a
  pinned SHA-256. The layout is in [tools/README.md](../../tools/README.md).
- Builds the game compiler from its GPL source and our
  [patches](../../patches/sce-991111b/README.md).
- Creates `.venv` with the Python packages from `requirements.txt`.
- Copies `SCUS_971.99` (and the level code) from your disc image.
- Installs a few Debian/Ubuntu packages with `sudo`, including Wine.

It writes only inside the checkout (`tools/`, `build/`, `.venv`,
`config/us/`). It does not touch your shell profile or `PATH`. The
repository ships nothing proprietary: tools come from the public mirrors the
PS2 decompilation community uses, and the game only from your own disc.

There are no settings or environment variables: every tool has one fixed
place under `tools/`.

## Make targets

| Command          | What it does                                                                 |
| :--------------- | :--------------------------------------------------------------------------- |
| `make elf`       | rebuild the boot executable and check it byte for byte (the gate)            |
| `make overlays`  | build `src/overlays/` and check every level function ([overlays.md](overlays.md)) |
| `make check`     | the CI checks: tests, script syntax, progress report preview                 |
| `make progress`  | after `make elf`: measure C_FUZZY, preview report and map in `build/progress/` |
| `make iso`       | put the rebuilt executable into a copy of the first `dumps/*.iso`            |
| `make clean-iso` | remove the rebuilt ISO                                                       |

`make elf` builds in `build/baseline`, which is deleted and recreated every
run. The executable lands in `build/baseline/config/us/build/SCUS_971.99`,
the objdiff report in `build/baseline/config/us/report.json`. A good run
ends with:

```text
PASS: reconstructed boot ELF matches retail
baseline build OK
```

Run it before every commit. If it fails, it says so and exits with an error.

`make iso` writes `build/Ratchet & Clank (USA) - rebuilt.iso`. Your original
image is not changed. Keep only one ISO in `dumps/`.

## How the game is compiled

The retail executable was built with two compilers, and so is ours:

- **SDK compiler** (EE-GCC 2.9-ee-991111-01) for the Sony SDK libraries at the
  start of the executable. Its output goes through `sdk-as`, a GNU `as` with
  two fixes so delay slots come out like retail
  ([0059](../../patches/sce-991111b/0059-gas-sdk-delay-slots.patch),
  [0063](../../patches/sce-991111b/0063-gas-sync-no-swap.patch)).
- **Game compiler** (Sony/Cygnus 2.9-ee-991111b, rebuilt from source) for the
  game code. Its output is assembled by `Ps2EeAs`, SN's Windows assembler,
  run through Wine.

A few units still need the SN compiler (EE-GCC 2.95.2). They are listed in
`ROUTE_EXCEPTIONS` at the top of `configure.py`, with every other per-unit
compiler setting. That list only gets shorter.

`Ps2EeAs` is version 1.9.25.758 from ProDG 3.01; the older one in the 2.0
archive pads loops differently. `scripts/patch-ps2eeas.py` changes two things
in it: it turns off the NOPs it puts before a `div` (retail has none of them),
and it gives it a bigger stack so large overlay files do not crash it.

## Manual setup

Only needed if you do not use `./setup.sh`. Put each tool at its path from
[tools/README.md](../../tools/README.md), keeping file permissions. Then:

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt
cp /path/to/SCUS_971.99 config/us/SCUS_971.99   # from the root of your USA disc
make elf
```

The game compiler is built with `python3 scripts/build-game-compiler.py`
(about a minute). It also installs `stdarg.h`, `stddef.h` and `va-mips.h`
into `tools/compilers/game-compiler/include/`; the compiler has no built-in
include folder, so code that uses `<stdarg.h>` needs them.

## What is not in Git

`build/`, `config/us/build.ninja`, `config/us/expected/asm/`, `tools/`,
`.venv` and `dumps/` are made on your machine from your own files. The
repository holds no game data, which is why nothing builds before setup has
copied `SCUS_971.99`.

Repository layout: [src/README.md](../../src/README.md) and
[config/README.md](../../config/README.md).

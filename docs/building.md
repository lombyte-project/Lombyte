# Building

## Quick setup

One command installs the toolchain, builds the game compiler from source,
takes the boot executable from your own disc image, and rebuilds the game
byte-for-byte:

```sh
git clone https://github.com/mateuszklysz/Lombyte.git && cd Lombyte
./setup.sh --iso /path/to/your-ratchet-and-clank-usa.iso
```

It ends with `PASS: reconstructed boot ELF matches retail`; from there,
[CONTRIBUTING.md](../CONTRIBUTING.md) shows how to pick a function and check
your C.

| Platform                          | How                                                                                                                                                                     |
| --------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Linux (Ubuntu 24.04+, Debian 13+) | the two lines above                                                                                                                                                     |
| Windows                           | in PowerShell: `wsl --install -d Ubuntu`, reboot, open Ubuntu and run the two lines above from your Linux home directory (`~`, not `/mnt/c`)                             |
| WSL                               | same as Linux                                                                                                                                                           |
| macOS, other Linux distributions  | with [Docker](https://docs.docker.com/get-docker/): `./setup.sh --docker --iso ...`, then `./setup.sh --shell` for a shell with the toolchain (`make elf`, `check-unit`) |

Already have the executable? `./setup.sh --elf /path/to/SCUS_971.99`.
`./setup.sh --check` lists what is installed, `./setup.sh --no-build` installs
without rebuilding, `./setup.sh --with-patched` also builds the optional
[patched profile](patched-toolchain.md), and `./setup.sh --help` lists the rest.

## What the script does

`./setup.sh` does everything on the rest of this page for you: it downloads and hash-checks the
public toolchain releases into `tools/`, builds the game compiler from source
with `scripts/build-game-compiler.py`, creates `.venv`, takes the boot
executable from your disc image, and runs the gate. The rest of this page is
the manual route and the reference for what the script installs.

The script ships nothing proprietary: it downloads the toolchain from the
public mirrors the PS2 decompilation community uses and checks every file
against a pinned SHA-256 before installing it, builds the game compiler from
its GPL source and this repository's
[patch stack](../patches/sce-991111b/README.md), and only ever reads the game
from the disc image or executable you provide. It writes inside the checkout
only (`tools/`, `build/`, `.venv`, `config/us/SCUS_971.99`); the one system
change it makes is installing the listed Debian/Ubuntu packages with `sudo`
(on plain Linux that includes Wine and the i386 architecture for it). Nothing
is added to your shell profile or `PATH`.

The verified environment is **Linux/WSL**. Compiler versions matter for
matching: on the manual route below, preserve the directory layouts and the
executable permissions of what you install. The game code is assembled by SN's
`Ps2EeAs`, a Windows executable that has to be runnable (Wine on Linux), so a
plain Linux toolchain is not enough by itself.

Everything below is run from the checkout root, and `RNC_GAME_ROOT` in the
sibling tooling repository points at this checkout.

## 1. Build tools

| Tool                                                | Required location                                                                 |
| :-------------------------------------------------- | :-------------------------------------------------------------------------------- |
| Game compiler (Sony/Cygnus EE `2.9-ee-991111b`)     | `tools/compilers/game-compiler/` (with `ee-gcc` and `cc1`)                        |
| SDK compiler (EE-GCC `2.9-ee-991111-01`)            | `tools/compilers/sdk-compiler/` (with `bin/ee-gcc`)                               |
| SN EE-GCC `2.95.2` (its `ee/bin/Ps2EeAs.exe`)      | `tools/compilers/ee-gcc-2.95.2/` (with `bin/ee-gcc.exe` and its supporting tools) |
| R5900 binutils                                      | `tools/binutils-mips-ps2-decompals/` (then `/opt/binutils-mips-ps2-decompals`), or set `BINUTILS_ROOT` |
| [objdiff CLI](https://github.com/encounter/objdiff) | `tools/objdiff/objdiff-cli`                                                       |
| Ninja, Python dependencies                          | installed into `.venv` below                                                      |

The retail executable was built with two compilers, and so is this one. The SDK
libraries are linked as one block ahead of the game code; `configure.py` builds
every C unit below `GAME_TEXT_START` with the SDK compiler and every unit from
there on with the game compiler. The game compiler's `cc1` output is assembled
by `Ps2EeAs`, which pads short loops and `div` instructions itself; the game
code is compiled without `-g`, whose line labels the assembler would take for
branch targets. Pending `INCLUDE_ASM` wrappers are assembled by the game
compiler's GNU `as` instead. The few units that do not reproduce on their
compiler yet are listed in `ROUTE_EXCEPTIONS` with the route that still builds
them; that list only shrinks.

The game compiler is required: `configure.py` stops with an error when it is
missing. Its driver has no builtin include directory, so the three headers it
needs for `<stdarg.h>`
(`stdarg.h`, `stddef.h`, `va-mips.h`, taken from the compiler's own
`gcc/ginclude`) must sit in `tools/compilers/game-compiler/include/`; the
`game-compiler` rule adds that directory with `-I`. Without them a unit that
includes `<stdarg.h>` cannot compile. Build it from the patch stack in
[`patches/sce-991111b/`](../patches/sce-991111b/README.md), which records the
pinned source archive, its SHA-256 and the host recipe that reproduces the
expected `cc1`: `python3 scripts/build-game-compiler.py` downloads the
archive, applies the stack and installs `cc1`, `cpp`, `xgcc`, the patched `as`,
the `ee-gcc` wrapper and the headers into `tools/compilers/game-compiler`
(about a minute; `--archive` reuses a local copy of the source archive). Or
point `GAME_COMPILER_ROOT` at an existing build.

The game code is assembled by the EE assembler of SN ProDG 3.01
(`ps2eeas` 1.9.25.758, installed as `ee/bin/Ps2EeAs.exe` in the SN tree);
the older assembler shipped in the ProDG 2.0 compiler archive pads loops
differently and fails the gate.
`setup.sh` installs it with one change, made by
[`scripts/patch-ps2eeas.py`](../scripts/patch-ps2eeas.py): its divbug
workaround, which puts up to two NOPs in front of a `div`/`div.s` near a branch
or a label and refuses a `div.s` in a `.set nomacro` delay slot, is turned
off. Retail has none of that padding (not one `nop; nop; div` among the
executable's 346 div words or the overlays' 26,539), so the assembler retail
used did not apply it. The patch is 6 bytes; the script checks the SHA-256 of
the download and of the result.

Some units in `ROUTE_EXCEPTIONS` build with a patched EE-GCC profile, built
separately. See [patched-toolchain.md](patched-toolchain.md) for the build and
for how those units are handled when the profile is absent.

## 2. Python environment

```sh
cd ~/Lombyte
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt

# Point this at your installed R5900 binutils.
export BINUTILS_ROOT="$HOME/tools/binutils-mips-ps2-decompals"
```

Use the versions pinned in [requirements.txt](../requirements.txt); `spimdisasm`
is pinned to match the expected disassembly.

## 3. Original game files

Extract `SCUS_971.99` from the root of your USA disc image and place it at
`config/us/SCUS_971.99`, then verify it against the hash in the front page:

```sh
sha256sum config/us/SCUS_971.99
```

For an ISO rebuild, also place your original disc image at `dumps/game.iso`
(create `dumps/` if needed). Both inputs are ignored by Git.

## 4. Build the executable

```sh
make elf
```

The build stages the sources, splits the reference executable, materialises the
assembly-backed units' oracles, compiles and links, generates an objdiff report,
and verifies the reconstructed executable against retail.

The staging directory defaults to `build/baseline` inside the checkout and is
**recreated on every run**. If the checkout lives on a Windows-mounted drive
(`/mnt/c/...`), point `BASELINE_ROOT` at a native Linux directory such as
`$HOME/rnc-baseline` so the 32-bit SDK compiler works on a local filesystem.

| Output                   | Default location                             |
| :----------------------- | :------------------------------------------- |
| Reconstructed boot ELF   | `build/baseline/config/us/build/SCUS_971.99` |
| Object comparison report | `build/baseline/config/us/report.json`       |

A successful run ends with:

```text
PASS: reconstructed boot ELF matches retail
baseline build OK
```

## 5. The gate

```sh
./verify-baseline.sh
```

This is the acceptance check, not a convenience wrapper: it rebuilds the
baseline and compares the SHA-256 of the **built** ELF with the one in the front
page. On failure it prints `baseline build failed with status N; no PASS receipt
produced` and exits non-zero, so a green run is evidence and a red one is not
quietly ignored. Run it before proposing a promotion and before committing.

## 6. Disc image

```sh
make iso
```

Rebuilds the executable, then patches it into a copy of the first `dumps/*.iso`
found — keep exactly one input ISO there so the selection is unambiguous. The
original is preserved, and with a byte-identical boot ELF the rebuilt image
matches your own. The output is
`build/Ratchet & Clank (USA) - rebuilt.iso`; `make clean-iso` removes it.

To patch an already-built ELF with explicit paths:

```sh
python3 rebuild-iso.py \
  --iso dumps/game.iso \
  --elf "$BASELINE_ROOT/config/us/build/SCUS_971.99" \
  --out build/rebuilt.iso
```

## Other targets and overrides

| Target           | Effect                                                          |
| :--------------- | :-------------------------------------------------------------- |
| `make check`     | the public CI checks (tests, script parse) and a preview of the progress report and map in `build/progress/`, without a full build |
| `make progress`  | after `make elf`: measure C_FUZZY and preview the report and map in `build/progress/` |
| `make iso`       | patch the verified ELF into a copy of the first `dumps/*.iso`   |
| `make overlays`  | compile `src/overlays/` (needs the generated overlay asm, see [overlays.md](overlays.md)) |
| `make clean-iso` | remove the rebuilt ISO                                          |

| Variable              | Default / usage                                     |
| :-------------------- | :-------------------------------------------------- |
| `VENV`                | `.venv` in the checkout                             |
| `BASELINE_ROOT`       | `build/baseline`; disposable staging directory      |
| `BINUTILS_ROOT`       | directory holding your `mips-ps2-decompals-*` tools |
| `EE_GCC_PATCHED_ROOT` | patched EE-GCC profile; see patched-toolchain.md    |
| `COMPILER_ROOT`       | `tools/compilers` in the checkout                   |
| `GAME_COMPILER_ROOT`  | overrides `tools/compilers/game-compiler`           |
| `SN_TOOLCHAIN_ROOT`   | `tools/compilers/ee-gcc-2.95.2`                     |

```sh
# Override only when the checkout is on a Windows-mounted drive.
export BASELINE_ROOT="$HOME/rnc-baseline"
export BINUTILS_ROOT="$HOME/tools/binutils-mips-ps2-decompals"
make elf
```

## Generated, not stored

`config/us/build.ninja`, `config/us/expected/asm/` and `build/` are produced
during a build from your own retail executable and are ignored by Git. The
repository stores no oracle and no extracted game data, which is why a build
cannot start before step 3.

The layout of this repository is described in [`src/README.md`](../src/README.md)
and [`config/README.md`](../config/README.md).

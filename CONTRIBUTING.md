# Contributing

Thanks for helping with _Ratchet & Clank_! All game code already compiles from
C to the exact retail bytes: the boot executable (`src/`) and the level code
(`src/overlays/`). What is left is making that C easy to read: names, structs,
data in C and notes on how the game works.

The build is the safety net. It checks the rebuilt game against retail byte
for byte, so a change that would alter the game fails instead of slipping
through.

## 1. Build once

```sh
./setup.sh --iso /path/to/your-ratchet-and-clank-usa.iso
```

This installs the toolchain, extracts the boot executable and level code from
your own disc image and runs the first build (Windows, macOS and manual setup:
[docs/build/building.md](docs/build/building.md)). After that:

| Command         | Ends with                                         | Checks                                  |
| :-------------- | :------------------------------------------------ | :-------------------------------------- |
| `make elf`      | `PASS: reconstructed boot ELF matches retail`     | the boot executable, byte for byte      |
| `make overlays` | `PASS`                                            | every level function and its data       |
| `make check`    | no errors                                         | script tests, struct layouts, progress  |

## 2. Pick something

| Work                     | Where to start                                                                 |
| :----------------------- | :----------------------------------------------------------------------------- |
| Name a function or value | the [Naming Desk](https://lombyte-project.github.io/namingdesk), `fun_<address>.c` files |
| Turn offsets into fields | `*(s32 *)(p + 0x40)` reads in `src/`; [structs](docs/contributing/code-cleanup.md#structs-and-data) |
| Move data to C           | [Data](#data) below                                                            |
| Explain a system         | a page in [docs/game/](docs/game/)                                             |

If you want to claim an area, open an issue or comment on one.

## 3. Change the C

Follow [docs/contributing/code-cleanup.md](docs/contributing/code-cleanup.md):
names by use, one declaration per function in a header, structs instead of
offsets, no comments that only repeat the code. The code is GCC 2.95 era C:
declare variables at the start of blocks and use the project types (`u32`,
`s32`, `f32`, …).

A level function keeps its `__asm__("FUN_L00_...")` label and a boot function
its `FUN_<address>` label on the declaration in its own file; the C name can
be anything the evidence supports.

## 4. Check

```sh
python3 scripts/format.py           # the project clang-format
python3 scripts/check-cleanup.py    # the code-cleanup.md rules your change breaks
make elf && make overlays           # both must end with PASS
```

`python3 scripts/check-unit.py <unit or FUN_L00_name>` checks one unit quickly
while you work. If a change breaks the match, undo it or see
[decompilation-tips.md](docs/contributing/decompilation-tips.md).

## 5. Open a pull request

Commit with the [message standard](docs/contributing/commit-messages.md), for
example `docs: explain the vendor menu`, and open a pull request with the
template. CI runs the script tests and `check-cleanup.py`, and comments the
change in progress. It does not build the game: `make elf` and
`make overlays` are yours to run.

## Data

Data can move to C too; its bytes are checked like code.

1. Find it: `python3 scripts/data-refs.py --catalog` lists the boot data the
   code touches in `build/data-refs/boot/data.yaml`; add
   `--level N --overlays config/us/overlays` for one level.
2. Define it: `python3 scripts/data-refs.py --emit ADDR --into FILE` writes
   the C definition after the reader's functions and prints the entry to add
   to a `*_OVERLAYS` table in `configure.py`. With `--level N` it writes level
   data and adds its `name<TAB>size` row to `config/us/levels/data.tsv`.
   A `D_XXXXXXXX` row is data every level holds at that address, such as
   `src/overlays/shared/world/level_globals.c`.
3. `make elf` (boot) or `make overlays` (levels) must end with `PASS`.

More: [code-cleanup.md](docs/contributing/code-cleanup.md#data),
[overlays.md](docs/build/overlays.md#source-files-and-the-check),
[progress.md](docs/build/progress.md#data).

## House rules

- Keep changes focused: one area per pull request.
- Never commit game data, disc images or compiler binaries. `dumps/`,
  `build/`, `tools/` and `config/us/overlays/` stay out of Git; the tests fail
  if any of it is tracked.
- Do not edit `tools/`; it is the local toolchain.
- Run `python3 scripts/test_public_tools.py` before changing `scripts/`.
- Optional hooks: `pre-commit install` for the file checks, and
  `ln -sf ../../scripts/commit-msg.py .git/hooks/commit-msg` for the commit
  message check.

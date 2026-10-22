# Contributing

Thanks for helping decompile _Ratchet & Clank_! The goal is to turn
assembly-backed code into C that compiles to the exact retail bytes: units of
the boot executable under [`src/assembly/`](src/assembly), and level code
under [`src/overlays/`](src/overlays) ([Level overlays](#level-overlays)).
Writing C, recovering names, improving types, and fixing docs all help.

The build is the safety net: it verifies the reconstructed executable against
retail byte-for-byte, so a contribution that would change the game fails the
build instead of slipping through.

## The contribution loop

### 1. Build the baseline once

```sh
./setup.sh --iso /path/to/your-ratchet-and-clank-usa.iso
```

installs the toolchain, extracts the boot executable from your own disc image
and runs the first `make elf` (Windows, macOS and manual setup:
[docs/build/building.md](docs/build/building.md)). Afterwards, rebuild at any
time with:

```sh
make elf
```

A few units need the optional
[patched compiler](docs/build/patched-compiler.md); without it they are built
from the retail bytes.

The run ends with `PASS: reconstructed boot ELF matches retail` and leaves a
workspace at `build/baseline` holding the retail assembly and per-unit
comparison targets used below.

### 2. Pick a function

```sh
python3 scripts/list-functions.py --score
```

Lists the pending units that already carry a C body, smallest first (25 by
default; `--limit 0` shows all, `--all` adds the units with no C yet). A unit
already under its subsystem directory can still appear here when its C keeps a
piece of inline assembly. `--score` measures each candidate's C body against
retail (about a minute for the full list) and lists the closest first; without
it the list is offline. `--filter` narrows it by owner path.

Grab a unit name, for example `assembly/textbin/runtime/memory/clear_u64_value`. If you
want to claim it, open an issue or comment on an existing one with the unit
name.

### 3. Read the source and the retail assembly

Open the unit's source, e.g. `src/assembly/textbin/runtime/memory/clear_u64_value.c`.
While a unit is pending, it keeps the retail assembly as an _oracle_ and the
readable C body as a fallback:

```c
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/.../FUN_001f99f8.s", FUN_001f99f8);
#else
/* readable C body -- this is what you edit */
#endif /* NON_MATCHING */
```

The oracle is generated locally from your own game executable and keeps the
build byte-exact until the C replaces it. **Do not remove it while you work**:
only `check-unit` compiles the C body.

`check-unit` prints the retail disassembly path under `retail asm`. Read it
next to the C and the surrounding units.

### 4. Write the C

- Rewrite or extend the C body under `#else`. Seeded bodies are starting
  points; some are wrong or incomplete.
- Remove `__attribute__((section(".text.*")))` seed attributes: the retail
  unit object uses plain `.text`, and promoted units never use custom text
  sections.
- Keep the unit's canonical symbol name (the second argument of its
  `INCLUDE_ASM(...)` line). A friendly name is welcome as an alias, e.g.
  `int count_items(int a0) __asm__("FUN_00123456");`.
- Declare variables at the start of blocks and use the project typedefs
  (`u32`, `s32`, `f32`, …) — this is GCC 2.9 era, not modern C.
- Keep it descriptive C: no inline assembly or copied disassembly in the body.
- Follow [docs/contributing/code-cleanup.md](docs/contributing/code-cleanup.md) for names, structs, globals and
  data placement.

### 5. Check your work

```sh
python3 scripts/check-unit.py assembly/textbin/runtime/memory/clear_u64_value
```

This compiles just that unit's C body and compares it with the retail object,
printing the `.text` score, per-function scores, and the first differing
instructions. Iterate until it reports `Object matches (promotable).` — that
means the text matches and the strict pass found every non-`.text` section at
100% as well. `Text matches; data/rodata differ` means the text is byte-equal
but the data sections still differ, and the full gate will fail. If you pulled
changes since your last baseline build, re-run `make elf` first.

When `check-unit.py` reports text matches, the authoritative check is still the
full image:

```sh
make elf
```

### 6. Promote an exact unit

Once the unit matches, make the C the only compiled code:

1. Delete the `#ifndef NON_MATCHING` oracle block and the `#else` / `#endif`
   lines, leaving the C body.
2. Move the file out of the assembly tree:
   `git mv src/assembly/<path>.c src/<path>.c`.
3. In [`config/us/rnc1.us.yaml`](config/us/rnc1.us.yaml), change that unit's
   owner from `assembly/<path>` to `<path>`.
   The file location plus the linker-config owner are the whole promotion: no
   per-file metadata is written.

4. Run `make elf` again; it must end with `PASS`.

There is no progress file to regenerate: CI rebuilds the objdiff report and
the progress map from the tree, and the pull request gets a comment with the
change in C_EXACT, in data in C and the newly matched functions.

### 7. Open a pull request

Commit with the project's message standard (see
[`docs/contributing/commit-messages.md`](docs/contributing/commit-messages.md)), for example
`decomp: promote clear_u64_value (8 B)`, and open a pull request
using the template. Include the unit name, its size, and the `PASS` line from
`make elf`. A maintainer will review and merge.

## Level overlays

Same loop, with a function name instead of a unit path. `./setup.sh --iso`
also extracts the level code from your disc image (gitignored, never
committed); run `python3 scripts/overlay-extract.py --iso <image>` if you
set up without it.

1. Pick: `python3 scripts/list-functions.py --overlays` (smallest first).
2. In the listed file, wrap the function's stub and write C under `#else`:

   ```c
   #ifndef NON_MATCHING
   INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002678b8.s", FUN_L00_002678b8);
   #else
   void FUN_L00_002678b8(void) { ... }
   #endif /* NON_MATCHING */
   ```

   The retail listing is `config/us/overlays/asm/<name>.s`.
3. Check: `python3 scripts/check-unit.py FUN_L00_002678b8` until it prints
   `Object matches (promotable)`. If the game compiler cannot reach it,
   add the file to `OVERLAY_SN_UNITS` at the top of `configure.py` (SN
   compiler) and check again.
4. Promote: delete the `#ifndef`/`INCLUDE_ASM`/`#else` lines and the `#endif`.
   There is no file move and no yaml change.
5. `make overlays` must end with `PASS`. Open a pull request
   (`overlay: FUN_L00_002678b8 (8 B)`).

## Data

Data can move to C too; its bytes are checked like code.

1. Find it: `python3 scripts/data-refs.py --catalog` lists the boot data the
   code touches in `build/data-refs/boot/data.yaml`; add
   `--level N --overlays config/us/overlays` for one level.
2. Define it: `python3 scripts/data-refs.py --emit ADDR --into FILE` writes
   the C definition after the reader's functions and prints the entry to add
   to a `*_OVERLAYS` table in `configure.py`. With `--level N` it writes level
   data and adds its `name<TAB>size` row to `config/overlays/us/data.tsv`.
   A `D_XXXXXXXX` row is data every level holds at that address, such as
   `src/overlays/shared/data/level_globals.c`.
3. `make elf` (boot) or `make overlays` (levels) must end with `PASS`.

More: names, types and placement in
[code-cleanup.md](docs/contributing/code-cleanup.md#data), level rows in
[overlays.md](docs/build/overlays.md#source-files-and-the-check), how data is
counted in [progress.md](docs/build/progress.md#data).

## Continuous integration

Every pull request runs the public `tools` job in
[`.github/workflows/checks.yml`](.github/workflows/checks.yml):

- `python3 scripts/test_public_tools.py -v` — the script regression suite;
- `python3 -m py_compile scripts/*.py` — every public script must parse.

These checks need no game data and never upload build outputs. `make check`
runs them locally, and also writes the progress report and map to
`build/progress/` for a preview. The full `make elf` rebuild stays a local,
contributor-run gate (see above); CI does not run it for you.

The `progress` workflow in
[`.github/workflows/progress.yml`](.github/workflows/progress.yml) needs no
game data for C_EXACT and data in C, which follow from the tree, so on every
run it generates the objdiff report (`scripts/gen_progress_report.py`) and the
progress map (`scripts/generate_treemap.py`), validates the report with objdiff
and uploads it as the `SCUS_971.99_report` artifact that
[recomp.fyi](https://recomp.fyi/game/ratchet-and-clank-ps2) reads. On `main` it also
publishes `report.json`, `decomp_map.svg` and `decomp_map.json` on the
[`progress`](https://github.com/lombyte-project/lombyte/tree/progress) branch,
which the README shows. On a pull request it compares the report with that
branch and [`progress-comment.yml`](.github/workflows/progress-comment.yml)
posts the result as one comment, updated on every push. None of these files is
committed, so they cannot fall behind `main` or conflict between pull requests.

C_FUZZY, the similarity of pending C bodies, needs a build, so CI does not
measure it: the maintainers' tooling repository keeps the measured scores and
the workflow reads them through a repository secret (pull requests from forks
reuse the scores last published for `main`). Locally, `make progress` after
`make elf` measures them for a preview.
The report holds unit names, symbols, addresses, sizes and match percentages
only, never retail bytes.

## Not exact yet? That is still useful

You do not have to reach 100% to contribute. Keep the oracle in place (restore
it with `git checkout -- <file>` if you removed it while experimenting), leave
your improved C body under `#else`, make sure `make elf` still passes, and open
a work-in-progress pull request describing what you tried and where the first
mismatch is.

If you are stuck, open an issue with the unit name, the `check-unit` output,
and the mismatch you cannot classify. More tips:
[docs/contributing/decompilation-tips.md](docs/contributing/decompilation-tips.md).

## House rules

- Keep changes focused; one unit per pull request is perfect for a first
  contribution.
- The check tools only read the checkout and write inside the baseline
  workspace (`build/baseline`); they
  never touch other locations.
- Never commit game data, disc images, or compiler binaries. `dumps/`,
  `build/`, `tools/` and `config/us/overlays/` stay out of Git; the test
  suite fails if any of it is tracked.
- Do not edit `tools/`; it is local toolchain setup.
- Readability work on matching code follows
  [docs/contributing/code-cleanup.md](docs/contributing/code-cleanup.md) and must stay byte-exact.
- Run `python3 scripts/test_public_tools.py` before changing anything under
  `scripts/`.
- The tracked [`.pre-commit-config.yaml`](.pre-commit-config.yaml) runs the
  usual file checks (JSON/YAML validity, large files, shebang modes); install
  it with `pre-commit install` or `prek install` if you use either.
- Enable the commit-message hook once per checkout (optional but handy):

  ```sh
  ln -sf ../../scripts/commit-msg.py .git/hooks/commit-msg
  ```

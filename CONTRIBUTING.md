# Contributing

Thanks for helping decompile _Ratchet & Clank_! The goal is to turn the
assembly-backed units under [`src/assembly/`](src/assembly/) into C that
compiles to the exact retail bytes. Writing C, recovering names, improving
types, and fixing docs all help.

The build is the safety net: it verifies the reconstructed executable against
retail byte-for-byte, so a contribution that would change the game fails the
build instead of slipping through.

## The contribution loop

### 1. Build the baseline once

```sh
./setup.sh --iso /path/to/your-ratchet-and-clank-usa.iso
```

installs the toolchain, extracts the boot executable from your own disc image
and runs the first `make elf` (see [Quick setup](docs/building.md#quick-setup) for
Windows, WSL and macOS, and [docs/building.md](docs/building.md) for the manual
route). Afterwards, rebuild at any time with:

```sh
make elf
```

Some units need an optional patched EE-GCC profile
([docs/patched-toolchain.md](docs/patched-toolchain.md)); without it they fall
back to the retail oracle.

The run ends with `PASS: reconstructed boot ELF matches retail` and leaves a
workspace at `build/baseline` inside the checkout (override with
`BASELINE_ROOT`) holding the retail assembly and per-unit comparison targets
used below.

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
  `int MyName(int a0) __asm__("FUN_00123456");`.
- Declare variables at the start of blocks and use the project typedefs
  (`u32`, `s32`, `f32`, …) — this is GCC 2.9 era, not modern C.
- Keep it descriptive C: no inline assembly or copied disassembly in the body.

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

There is no progress file to regenerate: CI rebuilds the decomp.dev report and
the progress map from the tree, and the pull request gets a comment with the
change in C_EXACT and the newly matched functions.

### 7. Open a pull request

Commit with the project's message standard (see
[`docs/commit-messages.md`](docs/commit-messages.md)), for example
`decomp: promote clear_u64_value (8 B)`, and open a pull request
using the template. Include the unit name, its size, and the `PASS` line from
`make elf`. A maintainer will review and merge.

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
game data for C_EXACT, which follows from the tree, so on every
run it generates the objdiff report (`scripts/gen_progress_report.py`) and the
progress map (`scripts/generate_treemap.py`), validates the report with objdiff
and uploads it as the `SCUS_971.99_report` artifact that
[decomp.dev](https://decomp.dev/mateuszklysz/Lombyte) reads. On `main` it also
publishes `report.json`, `decomp_map.svg` and `decomp_map.json` on the
[`progress`](https://github.com/mateuszklysz/Lombyte/tree/progress) branch,
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
and the mismatch you cannot classify. See
[`docs/decompilation-tips.md`](docs/decompilation-tips.md) for the full
acceptance discipline and the recovery ladder.

## House rules

- Keep changes focused; one unit per pull request is perfect for a first
  contribution.
- The check tools only read the checkout and write inside the baseline
  workspace (`build/baseline`, or `$BASELINE_ROOT` when you set one); they
  never touch other locations.
- Never commit game data, disc images, or compiler binaries. `dumps/`,
  `build/`, and `tools/` stay out of Git.
- Do not edit `tools/`; it is local toolchain setup.
- Do not refactor matching code for style: the generated bytes are the
  deliverable.
- Run `python3 scripts/test_public_tools.py` before changing anything under
  `scripts/`.
- The tracked [`.pre-commit-config.yaml`](.pre-commit-config.yaml) runs the
  usual file checks (JSON/YAML validity, large files, shebang modes); install
  it with `pre-commit install` or `prek install` if you use either.
- Enable the commit-message hook once per checkout (optional but handy):

  ```sh
  ln -sf ../../scripts/commit-msg.py .git/hooks/commit-msg
  ```

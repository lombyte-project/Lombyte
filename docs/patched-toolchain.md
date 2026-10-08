# Patched EE-GCC profile

Some units in `src/` are matched with a patched build of EE-GCC
2.9-ee-991111-01. `make elf` reproduces the retail executable without it,
rebuilding those units from the retail oracle instead of their C; install the
profile to compile and verify their C.

The patch lives in
[`patches/ee-gcc-2.9-991111-01/`](../patches/ee-gcc-2.9-991111-01/): 98
inserted and 12 deleted lines over 5 files, adding `sd`/`ld` GPR saves in
16-byte slots, classic `mult`/`mflo` code generation and volatile absolute
stores in call delay slots through `$at`. It adds no command-line options,
against the public snapshot
[`SSXModding/ps2-ee-toolchain`](https://github.com/SSXModding/ps2-ee-toolchain)
at `b595ded`.

## Build

Needs Linux or WSL with 32-bit host support (`gcc -m32`, e.g.
`gcc-multilib`), plus git and make. The script clones the pinned snapshot,
applies the patch, and builds `cc1`/`cpp`/`xgcc`. It fetches and builds bison
1.28 itself when needed (the 991111 grammar needs it).

```sh
python3 scripts/build-patched-toolchain.py
export EE_GCC_PATCHED_ROOT="$PWD/tools/ee-gcc2.9-991111-01-patched"
make elf
```

`./setup.sh --with-patched` runs the same build as part of the one-command
setup and exports `EE_GCC_PATCHED_ROOT` for its own `make elf`.

Options: `--source-dir DIR` (reuse a checkout, offline), `--bison PATH`,
`--check` (verify the patch only), `--verify-hashes`.

The reference hashes are in the patch README and `provenance.json`. GCC embeds
build paths, so rebuilds elsewhere differ; the script smoke-tests the compiler,
and `make elf` verifies the result byte-for-byte. It also warns when the
installed profile records a different patch revision than
`patches/ee-gcc-2.9-991111-01/patched-ee-gcc.patch`; re-run the build script
after pulling patch updates.

## Without the profile

`configure.py` rebuilds the affected units from the retail oracle, writes
`config/us/oracle-fallback-units.json`, and provides C-level aliases for the
link via `config/us/oracle-aliases.txt`. The image is still byte-exact.
`scripts/check-unit.py` refuses to score those units, so oracle bytes are never
reported as a C match.

## License

Patch and binaries are GPLv2-or-later, like the base; see
[`licenses/GPL-2.0.txt`](../licenses/GPL-2.0.txt). Binaries are not distributed
with this repository.

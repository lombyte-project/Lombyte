# Patched compiler

A few SDK units match only with a patched EE-GCC 2.9-ee-991111-01. It is
optional and lives in `tools/compilers/patched-compiler`.

- **Installed:** `make elf` compiles those units from their C and checks them.
- **Not installed:** `make elf` takes their bytes from the retail executable
  instead (listed in `config/us/oracle-fallback-units.json`). The result is
  still byte-exact, and `check-unit.py` refuses to score those units, so
  retail bytes never count as a C match.

## Build it

Needs Linux or WSL with 32-bit support (`gcc-multilib`), git and make.

```sh
python3 scripts/build-patched-toolchain.py
make elf
```

or `./setup.sh --with-patched`. The script clones the public
[`SSXModding/ps2-ee-toolchain`](https://github.com/SSXModding/ps2-ee-toolchain)
at `b595ded`, applies our patch and builds `cc1`, `cpp` and `xgcc`. It builds
bison 1.28 itself if needed. Options: `--source-dir DIR` (offline),
`--bison PATH`, `--check` (check the patch only), `--verify-hashes`.

`make elf` warns when the installed build comes from an older patch; run the
script again after pulling patch changes.

## The patch

[`patches/ee-gcc-2.9-991111-01/`](../../patches/ee-gcc-2.9-991111-01/), about
100 lines in 5 files. It adds `sd`/`ld` register saves in 16-byte slots,
classic `mult`/`mflo` code, and volatile stores in call delay slots through
`$at`. It adds no command-line options. Reference hashes are in the patch
README; GCC embeds build paths, so your build's hashes will differ, and
`make elf` is what proves it works.

Patch and binaries are GPLv2-or-later
([`licenses/GPL-2.0.txt`](../../licenses/GPL-2.0.txt)). No binaries are
stored in the repository.

# `tools/` — local toolchain (not tracked)

Install the matching toolchain here; the directory contents are ignored by Git
(only this README is tracked).

The expected layout and versions are documented in
[docs/building.md](../docs/building.md): the two compilers of the retail build,
`compilers/game-compiler` (required — `configure.py` refuses to generate a build
without it) and `compilers/sdk-compiler`, the retiring SN EE-GCC 2.95.2 tree,
the R5900 binutils, and the objdiff CLI.

No game data, compiler binaries, or proprietary tools are distributed with
this repository.

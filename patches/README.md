# Compiler patches

Every compiler this project builds comes from public source plus the patches
in this directory. Binary-only compilers (SN ProDG) are used as they are.

1. The source is pinned: a git revision, or an archive with its SHA-256.
2. Patches are plain `git apply` patches at the source root. The folder's
   `README.md` records each patch's SHA-256 and why it exists.
3. `make elf` and `make overlays` check the result byte for byte.

| Folder | Compiler | Source | Used for |
| :--- | :--- | :--- | :--- |
| [`sce-991111b/`](sce-991111b/README.md) | Sony/Cygnus EE-GCC 2.9-ee-991111b | Internet Archive copy of `gnu-ee-binutils-gcc-1.1.tar.gz` | the game compiler and the SDK assembler `sdk-as`; built by `scripts/build-game-compiler.py` |
| [`ee-gcc-2.9-991111-01/`](ee-gcc-2.9-991111-01/README.md) | Sony/Cygnus EE-GCC 2.9-ee-991111-01 | <https://github.com/SSXModding/ps2-ee-toolchain> at `b595ded` | the optional patched profile ([docs/patched-toolchain.md](../docs/patched-toolchain.md)) |

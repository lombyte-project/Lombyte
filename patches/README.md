# Compiler patches

Every compiler this project builds comes from public source plus the patches
in this directory. SN ProDG binaries are used as they are,
except the `cc1` of the level units (`sn-2.95.2/`).

1. The source is pinned: a git revision, or an archive with its SHA-256.
2. Patches are plain `git apply` patches at the source root. The folder's
   `README.md` records each patch's SHA-256 and why it exists.
3. `make elf` and `make overlays` check the result byte for byte.

| Folder | Compiler | Source | Used for |
| :--- | :--- | :--- | :--- |
| [`sce-991111b/`](sce-991111b/README.md) | Sony/Cygnus EE-GCC 2.9-ee-991111b | Internet Archive copy of `gnu-ee-binutils-gcc-1.1.tar.gz` | the game compiler and the SDK assembler `sdk-as`; built by `scripts/build-game-compiler.py` |
| [`sn-2.95.2/`](sn-2.95.2/README.md) | SN EE-GCC 2.95.2 v2.73a `cc1` | `gcc-2.95.2.tar.gz` plus the R5900 back end of the archive above | the level units in `OVERLAY_SN_UNITS`; built by `scripts/build-sn-compiler.py` |

# Compiler patch surfaces

Every compiler patch this project uses is reproducible from this directory.
`make elf` needs only the game compiler, which this directory also rebuilds;
units whose *other* compiler is unavailable are rebuilt from the retail oracle,
so the public build stays green without a patched toolchain.

## Rules

1. Sources must be public and pinned: a git URL + revision, or an archive with
   a SHA-256 recorded in the lineage `README.md`.
2. Patches are plain `git apply` patches at the source root, with the SHA-256
   of the patch file recorded in the lineage `README.md`.
3. The build recipe records the resulting `cc1` hash (`--verify-hashes` in
   `scripts/build-patched-toolchain.py`).
4. Binary-only compilers (the SN ProDG line used by the default route) are not
   patchable here; they are used as-is.

## Lineages

| id | lineage | source | patch | status |
| :--- | :--- | :--- | :--- | :--- |
| `ee-gcc-2.9-991111-01` | Sony/Cygnus EE 2.9-ee-991111-01 | <https://github.com/SSXModding/ps2-ee-toolchain> @ `b595ded` | [`patched-ee-gcc.patch`](ee-gcc-2.9-991111-01/patched-ee-gcc.patch) | built, used by the `patched` route |
| `sce-991111b` | Sony/Cygnus EE 2.9-ee-991111b | recovered archive, see [`sce-991111b/README.md`](sce-991111b/README.md) | production stack in [`sce-991111b/`](sce-991111b/README.md#production-stack-game-compiler) | game compiler, production stack through `0056` (cc1 bytes are host-dependent, see its README); 55/58 joint gate |

## Building

See [`docs/patched-toolchain.md`](../docs/patched-toolchain.md). Offline builds
accept `--source-dir` plus `--bison`; the recipe verifies the source revision,
the patch hash and (with `--verify-hashes`) the produced `cc1` and driver.

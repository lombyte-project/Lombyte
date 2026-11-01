# Hand-written assembly

Units here are hand-written low-level code (SIMD, VU0/MMI and COP2 helpers,
syscall stubs). They are listed as `intentional_asm` in
`config/us/unit_categories.json`, have no C goal and build from the retail
assembly: `INCLUDE_ASM` of a listing that `configure.py --make-asm` generates
from your own game executable (`config/us/expected/asm/`, not in Git). A short
comment above the `INCLUDE_ASM` line says why the unit stays in assembly.

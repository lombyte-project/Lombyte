# SN EE-GCC 2.95.2 v2.73a, rebuilt from source

The level units in `OVERLAY_SN_UNITS` (`configure.py`) were built with SN's
EE-GCC 2.95.2. Its `cc1.exe` is GCC 2.95.2 plus the R5900 back end of the
Cygnus EE tree, so we build it from that source.
[`scripts/build-sn-compiler.py`](../../scripts/build-sn-compiler.py) installs
`tools/compilers/sn-cc1/cc1`. The `overlay-sn` rule runs it on the output of
SN's preprocessor. SN's driver, preprocessor and `Ps2EeAs` stay as they are.

## Source

- `gcc-2.95.2.tar.gz` from <https://ftp.gnu.org/gnu/gcc/>, SHA-256
  `064e1cb06ea5d2f4a07ec46c1c64d771f74d04f404b6a6766bca2477f7d72482`.
- These files, copied whole from `gnu-ee-binutils-gcc-1.1.tar.gz` (pinned in
  [`../sce-991111b/README.md`](../sce-991111b/README.md)): `config.sub` and
  `gcc/config/mips/` `abi64.h`, `elf.h`, `elf5900.h`, `elf64.h`, `mips.c`,
  `mips.h`, `mips.md`, `t-r5900`.

SN built on 32-bit Windows. The host flags in the script copy that: a 64-bit
`HOST_WIDE_INT` printed with `"%d"`, and `.p2align`. SN's `cc1.exe` turns on
`-mgas -msplit-addresses` by default; ours gets them as flags.

## Patches

In this order (`PATCH_ORDER` in the script):

| Patch | Kind | Change | Source or fixtures |
|---|---|---|---|
| [sce-991111b 0001](../sce-991111b/0001-r5900-quad-saves.patch) | retail | R5900 saves and restores every GPR with `sq`/`lq` | same as the game compiler |
| [sce-991111b 0054](../sce-991111b/0054-r5900-assembler-pads-loops.patch) | retail | `cc1` does not pad short loops: `Ps2EeAs` does | same as the game compiler |
| [sce-991111b 0064](../sce-991111b/0064-r5900-timode-reload-one-register.patch) | retail | a TImode reload in general registers takes one register | same as the game compiler |
| [0000](0000-host-obstack.patch) | host | `include/obstack.h` lvalue cast | the hunk of sce-991111b 0000 |
| [0001](0001-r5900-target.patch) | host | the `mips64r5900*-*-elf*` target in `gcc/configure.in` and `gcc/configure`; the `MAX_INTEGER_COMPUTATION_MODE` fixes of the EE tree's `expr.c` | |
| [0002](0002-vu0-registers.patch) | retail | 111 hard registers: `vf0`-`vf31` in a `VU_REGS` class, `TI_REGS` = general + VU, `movti` with a `j` (VU) alternative | the register tables of SN `cc1.exe`; 49 units, e.g. `l11/gameplay/entities/seesaw_platform` |
| [0003](0003-no-modes-in-second-hilo.patch) | retail | no mode fits in the second HI/LO pair | 28 units, e.g. `l10/gameplay/entities/screamer` |
| [0004](0004-quad-block-moves.patch) | retail | 16-byte aligned block moves use `lq`/`sq` | 31 units, e.g. `l07/gameplay/entities/taxi_platform` |
| [0005](0005-aligned-array-typedef.patch) | retail | `aligned` on a typedef of an array type makes a new type instead of changing the shared one | 24 units, e.g. `l09/gameplay/entities/elevator` |
| [0006](0006-msvcrt-float-comments.patch) | retail | float constants printed like msvcrt (`1.#INF`, 17 digits, 3-digit exponents) | 3 units, e.g. `l15/gameplay/entities/conveyor_belt` |
| [sce-991111b 0062](../sce-991111b/0062-reload-reg-unavailable.patch) | upstream | reload never picks a register outside the insn's spill set | gcc-2.95.3, Bernd Schmidt 2000-01-12 (`reload1.c`) |

Kinds as in [`../sce-991111b/README.md`](../sce-991111b/README.md). A fixture
is a unit whose `.s` changes when the patch is left out.

### Proof

Without `0062`, this `cc1` gives the same `.s` as SN's `cc1.exe` for all 180
units in `OVERLAY_SN_UNITS` (same preprocessed input; `.ident` and `.file`
lines aside).

`0062` is not in SN v2.73a, but the retail level code was built with it. The
180 units do not change; only functions still in asm do:
`FUN_L05_0030de90` and `FUN_L10_002dcc58` become exact (in the first, retail
loads the constant `0x90` again where SN v2.73a copies it), the drafts of
`FUN_L08_002f5e58` and `FUN_L00_002ddf60` come closer, and the draft of
`FUN_L00_002b77f0` moves further off.

### SHA-256

```text
0000-host-obstack.patch                           03586b316d9359d3fef35522251479e0037823ca4471711e85e2086803bfbc1f
0001-r5900-target.patch                           6cdc88add0f979fe66785a20b60697e7a9862cc225f78f8415e8791ade67d852
0002-vu0-registers.patch                          31e1952263d9be423063d6048ae333eeab9004294baa1b23fa2081c76f66b9d2
0003-no-modes-in-second-hilo.patch                1eea80f6cfb689ffe0aa5211ccbf3824ee15bdd70e3456f3a8271884509f1a4b
0004-quad-block-moves.patch                       32e060274bd3bcb42c95a3e0b52aadb4f30b554a2565b31e0fe13fa41ed762b0
0005-aligned-array-typedef.patch                  f69306309f0c6ad165e3119c9fb60576815d08c5a79a5311f5cd14cd158a9809
0006-msvcrt-float-comments.patch                  ad2881c8a03da36c35598e2b16ab5c668267b2bb2e5e754df05a9d0846f40a69
```

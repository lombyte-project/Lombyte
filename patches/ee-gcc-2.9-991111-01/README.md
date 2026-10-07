# Patched EE-GCC 2.9-ee-991111-01
Source patch for the optional compiler profile used by some units in `src/`.
`make elf` does not need it: without it those units are rebuilt from the retail
oracle. Build instructions and requirements:
[`docs/patched-toolchain.md`](../../docs/patched-toolchain.md).
- Base: <https://github.com/SSXModding/ps2-ee-toolchain> at `b595ded`
  (public Sony EE-GCC 2.9 snapshot).
- File: [`patched-ee-gcc.patch`](patched-ee-gcc.patch), applied with
  `git apply` at the source root.
- Patch SHA-256:
  `0faa74c638ad4ce2fa3ebf7e9b1a99afc295bb2b61958d177bf883d4fa3eef4e`.
Changes (98 inserted, 12 deleted lines across 5 files):
| File                                   | Change                                                                  |
| :------------------------------------- | :---------------------------------------------------------------------- |
| `ee/gcc/c-parse.in`                    | typed midrule actions, so bison 1.28 parses the grammar on modern hosts |
| `ee/gcc/config/mips/mips.h`            | declares the flag reorg sets while it splits a volatile store           |
| `ee/gcc/config/mips/mips.c`            | GPR callee saves with `sd`/`ld` in 16-byte slots, as the retail SDK code |
| `ee/gcc/config/mips/mips.md`           | `mulsi3` uses classic `mult`/`mflo`; a volatile store to an absolute address in a call delay slot is split into `lui $1` / `sw` through `$1`, bracketed with `.set noat` |
| `ee/gcc/reorg.c`                       | a volatile store just before a call may fill its delay slot             |
The profile has no per-unit options: every unit on it builds with the same
command line.
Reference binaries (a Linux cloud host build; rebuilds elsewhere differ
because GCC embeds build paths):
```text
cc1   c2aaf9dcbd5d72eafcc63b4ae98472d3f8b9f7202bab3a10dfe7b535f10b9213
cpp   1509d06d85068c86c6e2529108597c0e4d3d9ecafcd3e43b6d14607c9bc3b1b7
xgcc  28872669c68b760b3220077ac313f908c479f3b1282e29308988318e2c76b252
```
Patch and binaries are GPLv2-or-later, like the base; see
[`licenses/GPL-2.0.txt`](../../licenses/GPL-2.0.txt). Binaries are not
distributed with this repository.

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
  `cc55e70cba79e24fba4195d494370b850d329a4e039e5b2c72dc42661c2e74fa`.
Changes (151 inserted, 14 deleted lines across 5 files):
| File                                   | Change                                                                  |
| :------------------------------------- | :---------------------------------------------------------------------- |
| `ee/gcc/c-parse.in`                    | typed midrule actions, so bison 1.28 parses the grammar on modern hosts |
| `ee/gcc/config/mips/mips.h`            | opt-in `-mastra-*` target options, inert unless selected                |
| `ee/gcc/config/mips/mips.c`            | GPR callee saves with `sd`/`ld` in 16-byte slots, as the retail SDK code |
| `ee/gcc/config/mips/mips.md`           | `mulsi3` uses classic `mult`/`mflo`; in-place `cvt.w.s` conversion (`-mastra-inplace-cvt`); a volatile store to an absolute address in a call delay slot is split into `lui $1` / `sw` through `$1`, bracketed with `.set noat` |
| `ee/gcc/reorg.c`                       | a volatile store just before a call may fill its delay slot             |
The profile has no per-unit options: every unit on it builds with the same
command line.
Reference binaries (a Linux cloud host build; rebuilds elsewhere differ
because GCC embeds build paths):
```text
cc1   9a46212fe367be7787b8b07e5e498585b8cef8a2bbd29c1a70da4eb29f31dc76
cpp   f2b234687c9b518f5c78c2f42ab6cc67ea6a1b1eb2042889cc6bcfafdc3db341
xgcc  44143d386c89bd63d04ec5ca3e01471511768be8066364fe4ce236feae4a7db0
```
Patch and binaries are GPLv2-or-later, like the base; see
[`licenses/GPL-2.0.txt`](../../licenses/GPL-2.0.txt). Binaries are not
distributed with this repository.

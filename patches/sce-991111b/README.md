# Game compiler: Sony EE-GCC 2.9-ee-991111b

The game compiler (`cc1`, `cpp`, `xgcc` and GNU `as`) and the SDK assembler
`sdk-as` are built from the archive below with the patches in this folder.
[`scripts/build-game-compiler.py`](../../scripts/build-game-compiler.py) does
it: `PATCH_ORDER` is the game compiler stack, `SDK_AS_PATCH_ORDER` the
`sdk-as` stack.

## Source

- Archive: `gnu-ee-binutils-gcc-1.1.tar.gz`, 16,510,927 bytes
- SHA-256: `1f518043e252d6eda726386971d52eda26541ab936ea73a9783d73712b595f92`
- Origin: Internet Archive copy of
  <https://web.archive.org/web/20060518234647id_/http://ps2dev.sourceforge.net:80/downloads/ee/gnu-ee-binutils-gcc-1.1.tar.gz>
  (gzip timestamp 2001-01-29)
- Contents: the full GCC and binutils tree; `gcc/version.c` says
  `2.9-ee-991111b`; target `mips64r5900-sf-elf`.

## Build

Apply the patches with `git apply` at the root of the extracted archive, in
the script's order. Configure with `--target=mips64r5900-sf-elf
--host=i686-linux-gnu --build=i686-linux-gnu --disable-nls
--enable-languages=c --without-headers` and build with
`make LANGUAGES=c 'CC=gcc -m32' 'CFLAGS=-O2 -fno-strict-aliasing -fcommon
-std=gnu89 -D_GNU_SOURCE' cc1` (`make all-gas` for the assemblers).

The host needs `-fno-strict-aliasing`: `real.c` type-puns through `EMUSHORT`
pointers, and with strict aliasing a modern host compiler turns
`REAL_VALUE_NEGATE` into a no-op, so negative float constants land in
`.sdata` instead of `li.s`/`li.d`.

The binaries embed build paths, so their hashes depend on the host and the
work directory. `make elf` and `make overlays` are the check.

## Patches

Each patch is one change against the archive. Numbers are not reused, so a
gap is a patch that was dropped. The kinds:

- **upstream**: the same code as a later GCC or binutils release. The
  source names the release and its ChangeLog entry.
- **as upstream**: the archive has a Cygnus-only pass or default that
  GCC 2.95.x does not have; the patch turns it off.
- **retail**: no upstream change; the patch makes the output match the
  retail game. The fixtures are units that need it.
- **host**: build fix only, no change to the output.

### Game compiler

| Patch | Kind | Change | Source or fixtures |
|---|---|---|---|
| [0000](0000-modern-host-fixes.patch) | host | `include/obstack.h` lvalue cast; the generated `gcc/c-gperf.h` | |
| [0001](0001-r5900-quad-saves.patch) | retail | R5900 saves and restores every GPR with `sq`/`lq` | retail game prologues |
| [0016](0016-no-edge-lcm-default.patch) | as upstream | edge-based LCM in GCSE off by default | gcc-2.95.2 has no edge-based LCM; `textbin/fun_001fe898` |
| [0020](0020-gas-absolute-unknown-symbol.patch) | retail | `as`: a load or store of a symbol whose size is not known yet uses `lui`/`%lo` | `textbin/fun_0023b590`, `textbin/fun_0023aba0` |
| [0022](0022-no-sibcalls.patch) | as upstream | no sibling calls | gcc-2.95.2 has none (they came in gcc 3.0); `textbin/memcard_init` |
| [0027](0027-gas-inline-float-literals.patch) | retail | `as`: `li.s` builds the constant inline, not in `.lit4` | `textbin/fun_00206e18`, `textbin/fun_00213308` |
| [0029](0029-call-clobber-pending.patch) | upstream | the scheduler treats call-clobbered registers as clobbers, not sets | gcc-2.95.2 `haifa-sched.c` (`sched_analyze`, same code) |
| [0037](0037-game-no-strict-aliasing.patch) | upstream | `-O2` does not turn on strict aliasing | gcc-2.95.2, Jeff Law 1999-10-20: "Do not turn on strict aliasing by default" |
| [0044](0044-sda-nosda-attributes.patch) | retail | SN's `sda`/`nosda` variable attributes: placement in `.sdata`/`.data` | SN's GCC 2.95.3 extension |
| [0045](0045-encode-section-info-sda-nosda.patch) | retail | `sda`/`nosda` decide gp-relative or absolute references | SN's GCC 2.95.3 extension |
| [0047](0047-gcc-2.95.2-reload-local-alloc-regclass.patch) | upstream | `reload1.c`, the `local-alloc.c` equivalence fixes and the `regclass.c` `'p'` constraint | gcc-2.95.2 |
| [0048](0048-gcc-2.95.2-cse.patch) | upstream | `cse.c`, plus the helpers the 2.9 tree lacks | gcc-2.95.2; `textbin/fun_00226a70` |
| [0049](0049-sibcall-pass-needs-placeholder.patch) | as upstream | the Cygnus sibcall pass does nothing when there is no `CALL_PLACEHOLDER` | gcc-2.95.2 has no such pass; `textbin/fun_001eb740` |
| [0051](0051-r5900-fpr-hazard-exact.patch) | retail | `as`: an FPR hazard checks the one register, not the even/odd pair | `textbin/fun_001f2070`, `textbin/rendering/view/init_view_context` |
| [0052](0052-r5900-dli-retail-general.patch) | retail | `as`: 64-bit `dli` constants built top-down in 16-bit fields | `textbin/fun_001f4fb8`, `textbin/fun_00200600` |
| [0053](0053-gas-la-absolute-unknown-symbol.patch) | retail | `as`: the 0020 rule for `la` | `textbin/fun_002135f0` |
| [0054](0054-r5900-assembler-pads-loops.patch) | retail | `cc1` does not pad R5900 short loops: `Ps2EeAs` does | `ui/hud/get_icon_frame`, `video/decoder/video_dec_flush` |
| [0055](0055-r5900-no-second-hilo.patch) | retail | the second HI/LO pair stays fixed: no `mult1`/`div1` in game code | `textbin/fun_001fe980`, `textbin/fun_00214260` |
| [0056](0056-sda-extern-before-use.patch) | retail | `.extern` of an `sda` variable at its first use, so `Ps2EeAs` knows its size | `textbin/fun_00206b78`, `textbin/fun_0022e8c8` |
| [0057](0057-reorg-sticky-volatile-set.patch) | upstream | reorg keeps a volatile store's flag when a plain store follows | part of gcc-2.95.2, Mark Mitchell 1999-05-06: "Make volatil monotonically increasing" |
| [0058](0058-reorg-trap-if-sets-volatile.patch) | upstream | reorg moves nothing across a conditional trap | gcc-2.95.2, Mark Mitchell 1999-05-04: "Handle ... TRAP_IF ... just like in mark_referenced_resources" |
| [0060](0060-alias-static-chain-not-a-base.patch) | upstream | the static chain and struct value registers are not alias bases | gcc-2.95.3, Richard Henderson 2000-08-22 (`alias.c`) |
| [0061](0061-copy-rtx-if-shared-virtual-mems.patch) | upstream | a stack MEM is not shared | gcc-2.95.3, Jeff Law 1999-08-30 (`emit-rtl.c`) |
| [0062](0062-reload-reg-unavailable.patch) | upstream | reload never picks a register outside the insn's spill set | gcc-2.95.3, Bernd Schmidt 2000-01-12 (`reload1.c`) |

### SDK assembler

`sdk-as` assembles the SDK units: the archive's GNU `as` with `0000`, `0063`
and `0059`, none of the game assembler patches.

| Patch | Kind | Change | Source or fixtures |
|---|---|---|---|
| [0063](0063-gas-sync-no-swap.patch) | upstream | the `sync` test reads `prev_pinfo & INSN_SYNC`; the archive's `&&` was true for every instruction, so no delay slot was ever filled | binutils 2.10 and later, Ian Lance Taylor 1999-08-03: "Correct INSN_SYNC test.  From Ralf Baechle" |
| [0059](0059-gas-sdk-delay-slots.patch) | retail | an instruction of a macro expansion, and an `mflo`/`mfhi`, stay out of a delay slot | `kputchar`, `ftoi`, `sceCdRead`, `_calloc_r` (`mflo`), `_dtoa_r` (`li.d`) |

No binutils release has `0059`: binutils 2.17 and older have no R5900, and
2.25 counts the R5900 HI/LO as interlocked, so it would move an `mflo` into
the slot. Both rules come from the retail SDK code only.

### SHA-256

```text
0000-modern-host-fixes.patch                      90cec417b339e7afd8407e50c49b28a6ad438aa91e5a994f40a25bba0d1d73ad
0001-r5900-quad-saves.patch                       dad144292eb17a79b6477a94f3c51fecc59e8b201998eabdbb4040e6decd6e84
0016-no-edge-lcm-default.patch                    ab869552e38a3e49246f741a95a817a82e255514ea45ef8ccb41ab29d6e05098
0020-gas-absolute-unknown-symbol.patch            4c354ca93fa17cbe47879d34d34ee87cb760fb13c32c3770444cec3c99d7a0b5
0022-no-sibcalls.patch                            2809fa41e7a6fcc5c84bac00568e42a42df19e57644cc2adb700f343c2e59bc8
0027-gas-inline-float-literals.patch              0272bee08529c415490f3f565f5a38846a98872fb972c81f8f17f07682605682
0029-call-clobber-pending.patch                   c3031b4a0c88a03b19f82f74feb31455c5ee86f06f538ad869fbf9cf734f2af0
0037-game-no-strict-aliasing.patch                5c25409ae438dd09e8720c93a0c81898ac3001c30d3f9fe1c59b1ebbbeeed261
0044-sda-nosda-attributes.patch                   b14f446a4f03387aae3a78a21a245e0d10c5274e592fb95673674d3636c5bd46
0045-encode-section-info-sda-nosda.patch          de44f2608b06689b9e9885d6fa09dcd9f951599ef78d36eee74bce00e7cf7d0f
0047-gcc-2.95.2-reload-local-alloc-regclass.patch 91a335c85007d9ebd82af63dea612fef6dd25196ef4e093f2f25430c42eebdc6
0048-gcc-2.95.2-cse.patch                         75ab8f74bccb1daf022cb4842cd11b1fa4a776994d9de497b658f91b09209bca
0049-sibcall-pass-needs-placeholder.patch         39747f7eb498836603d67916b36076f323c4634a3f62c69ac757bbbdc455a74e
0051-r5900-fpr-hazard-exact.patch                 9cb7b484e639776e2bd8631940f37a0a29f76a37dcfaf7ba00bcfdd8d69082d0
0052-r5900-dli-retail-general.patch               dc1f21eda731afdbf46f1ac5bb831af68621d5dcedf446c9c75153a1d45b09ea
0053-gas-la-absolute-unknown-symbol.patch         bbde5a43cc31e01cab06e19a93f050b297778f3bed107e6ef9f83b29429591d6
0054-r5900-assembler-pads-loops.patch             f9e33444445c41bcf3a366a29e356a47a09b2dc795a38502239fc53c1b35db1e
0055-r5900-no-second-hilo.patch                   f10fc14fa98c178f91244e721ed7fc4e5f1c1c6b1e851b8cf63c012d72f25262
0056-sda-extern-before-use.patch                  0900ebe87a1d8fe160e27e68e2668aa7e57b16b2f30492d0f0ea6212a46b230a
0057-reorg-sticky-volatile-set.patch              6cdde99d4d4e2854d31ee6d2433a638b6eb564fcb70cb3a00d78c4aceae402e2
0058-reorg-trap-if-sets-volatile.patch            1171f8e37de070d8ff418ea9812b8c1f27313eeb19fcbb49fd8b6aef45590346
0059-gas-sdk-delay-slots.patch                    0d1fa03a7bb1ee3d2d0710967e15cb9b046af2449118fc9290241f1579e0d703
0060-alias-static-chain-not-a-base.patch          0bf0ab1423c33e674c7cc34b0286750e75777d42f52d540be9e53957bb7d3789
0061-copy-rtx-if-shared-virtual-mems.patch        4f21bbdd604aa90367775c96d39aea13c14fe21217e4e1f2fc24ee7d9470d928
0062-reload-reg-unavailable.patch                 9bddaf0912802fba0a322dcfaace0ec3626da4a11b9f5c698b60cebab7a7c2c0
0063-gas-sync-no-swap.patch                       e4bf497eb0ab4ab8d3f584fa17fc8cab33733b7f216f2f1be129093312d2bf96
```

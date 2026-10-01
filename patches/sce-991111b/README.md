# Sony EE-GCC 2.9-ee-991111b (recovered source)

Second patch surface for the Sony/Cygnus EE line and the source of the
project's **game compiler**: the production stack below, applied to the pinned
archive, rebuilds the installed game-compiler `cc1` (joint parity gate 55/58,
full-ELF gate PASS). Its bytes depend on the host compiler: through `0049`
the WSL host built `05ff323f6e75accbcec5129b233d7ec16a3045805ea3c2098572985bdcf0a19f`,
and the stack through `0055` builds `66c253915a6fa851…`, the full stack through
`0056` `fc69951c0ec883e1…`, on the Linux cloud host (see the build notes below). Check a rebuilt compiler by its output.

- Archive: `gnu-ee-binutils-gcc-1.1.tar.gz`, 16,510,927 bytes
- SHA-256: `1f518043e252d6eda726386971d52eda26541ab936ea73a9783d73712b595f92`
- Origin: Internet Archive copy of
  <https://web.archive.org/web/20060518234647id_/http://ps2dev.sourceforge.net:80/downloads/ee/gnu-ee-binutils-gcc-1.1.tar.gz>
  (ps2dev.sourceforge.net downloads/ee, gzip timestamp 2001-01-29)
- Contents: complete GCC + binutils tree, 8,139 entries, `gcc/version.c` says
  `2.9-ee-991111b`; target `mips64r5900-sf-elf`.
- Tool dirs used so far: `src/gcc`, `src/binutils`, `src/bfd`, `src/gas`.

Build notes: same 32-bit host recipe as
[`../ee-gcc-2.9-991111-01/`](../ee-gcc-2.9-991111-01/), **plus
`-fno-strict-aliasing` in the host CFLAGS** (`-O2 -fno-strict-aliasing -fcommon
-std=gnu89 -D_GNU_SOURCE`). `real.c`'s float emulator type-puns through
`EMUSHORT` pointers; a modern host compiler with strict aliasing turns
`REAL_VALUE_NEGATE` into a no-op, so negative float/double constants are sent
to the `.sdata` pool instead of `li.s`/`li.d` (the retail SN cc1 emits
`li.s`/`li.d`).
A reference build of
this vintage scores 79.87 on `fun_0022f778`, so it is not the retail compiler
for that unit.

## Production stack (game compiler)

Apply with `git apply` at the root of the extracted archive, in this order:

1. `0000-modern-host-fixes.patch` (host build only: `include/obstack.h`
   lvalue-cast fix and the generated `gcc/c-gperf.h`)
2. `0001`, `0015`, `0016`, `0019`, `0020`, `0021`, `0022`, `0025`, `0026`,
   `0027`, `0028`, `0029`, `0030`, `0031`, `0032`, `0033`, `0034`, `0037`
3. `0036`, `0044`, `0045`, `0046`, `0047`, `0048`, `0049`,
   `0050`, `0051`, `0052`, `0053`, `0054`, `0055`, `0056`

Configure for `--target=mips64r5900-sf-elf --host=i686-linux-gnu
--build=i686-linux-gnu --disable-nls --enable-languages=c --without-headers`
and build `cc1` with `make LANGUAGES=c 'CC=gcc -m32' 'CFLAGS=-O2
-fno-strict-aliasing -fcommon -std=gnu89 -D_GNU_SOURCE' cc1`; the result is
`cc1` `05ff323f6e75accb…` (verified 2026-09-23 from a fresh extraction;
through `0048` it is `4dfa3cf0f0fa8d31…`).
`0038` is published but not part of the production stack; `0035` was never
published (its record below is kept for the stack history).

Patches marked *production dependency* are in that stack but turned no
fixture exact on their own; they are published so the production compiler is
reproducible from this directory. Every other entry names its exact fixture.

The game code is compiled with this `cc1` and assembled by SN's `Ps2EeAs`
(`configure.py`, rule `game-compiler`), as the retail executable was. The
patched GNU `as` of this tree only assembles the pending `INCLUDE_ASM`
wrappers. The `cc1` bytes depend on the host compiler as well as on the
stack: on the Linux cloud host used for `0054`, the stack through `0053`
builds `d372122712b3f995…`, which produces the same object as `05ff323f…` for
every game-compiler unit, the stack through `0054` builds `6f4bb085d51c0633…`,
the stack through `0055` builds `66c253915a6fa851…`, and the full stack
through `0056` builds `fc69951c0ec883e19179d289fd690b3c7f94f15abb71b1ddafad14d7794cdf15`.
Check a rebuilt compiler by its output, not by its hash, when the host
differs. `make` does not track header dependencies: after changing
`mips.h`, remove `toplev.o` (or build from a fresh tree).

## Published patches

- `0056-sda-extern-before-use.patch` SHA-256: `3cddcbd64614d349a827f1bc51678dbe383996575b9e7c27f3a79bb3da0998ad`
  - role: default. A variable declared `__attribute__((sda))` gets its
    `.extern NAME, SIZE` at its first use, ahead of the text of the function
    that uses it, instead of in the end-of-file list. SN's `Ps2EeAs` is
    single-pass: it addresses a symbol gp-relative only when its size is known
    before the reference, which is how retail reaches some externals through
    `$gp` and keeps others of the same size absolute. Unmarked externals keep
    the end-of-file placement, so the patch is inert without the attribute.
  - fixtures: `textbin/fun_00206b78`, `textbin/fun_0022e8c8` byte-identical,
    and thirteen promoted units drop their inline `.extern` asm; full-ELF gate
    PASS, linked ELF hash unchanged

- `0055-r5900-no-second-hilo.patch` SHA-256: `f10fc14fa98c178f91244e721ed7fc4e5f1c1c6b1e851b8cf63c012d72f25262`
  - role: default. The R5900's second HI/LO pair stays fixed, so the register
    allocator never places a product there (`mult1`, `div1`). The retail game
    code has 436 multiplies and none on pipeline 1 (the 8 `mult1` of the
    executable are all in the SDK, which the SDK compiler builds). With the
    pair exposed, the game compiler chose `mult1` for the second of two
    overlapping multiplies. All 494 game-range units that were byte-identical
    stay byte-identical; `-ffixed-hi1` cannot express this, because the
    R5900 register setup un-fixes the pair after the command line is read
  - fixtures: `textbin/fun_001fe980`, `textbin/fun_00214260`,
    `textbin/storage/memory_card/data/memcard_restore_info` (PAL imports)

- `0054-r5900-assembler-pads-loops.patch` SHA-256: `2cec82e691f4c2fbd20fe530960df5fe2476b891561bd6ad65342b31f8fa03eb`
  - role: default. The R5900 short-loop padding and the `div` padding are
    the assembler's: `Ps2EeAs` adds them itself. `cc1` no longer imitates
    it: the Cygnus `mips_r5900_lengthen_loops` pass and the post-dbr
    `mips_r5900_pad_loops` (`0019`, `0046`) are not run, and the `0025`
    dead-slot annulment is opt-in again (it annulled slots retail leaves
    plain). With `Ps2EeAs` and no `-g` (its line labels count as branch
    targets for the `div` padding), 478 of 522 game-range C units build
    byte-identically with the common configuration; full-ELF gate PASS
  - fixtures: `textbin/audio/banks/snd_bank_load_by_loc`,
    `textbin/fun_00226670`, `textbin/video/decoder/vi_buf_stop_dma`,
    `gameplay/animation/find_valid_animation_frame_index`,
    `video/decoder/video_dec_flush`

- `0053-gas-la-absolute-unknown-symbol.patch` SHA-256: `c43dcd5c4f48a7b25b2660f2bace5cd9709364d179c561ef9a705f8f0ea8c93d`
  - role: default (gas). `0020` gave loads and stores the retail rule for a
    symbol whose size is still unknown at the use (no `.extern` seen yet):
    in reorder mode the absolute `lui`/`<op>` pair. The `la` macro kept the
    relaxable gp pair, which the link resolves to one gp-relative `addiu`, so
    an 8-byte symbol that retail addresses as `lui`/`addiu` in one register
    came out gp-relative. The same predicate now applies to `la`
  - fixtures: `textbin/ui/menus/save_data/saving_data_menu` and
    `textbin/fun_002135f0` byte-identical on the game compiler; full-ELF gate
    PASS, linked ELF hash unchanged
- `0052-r5900-dli-retail-general.patch` SHA-256: `fadf5a22a28c07fddc1e0607df186870f0a32e53597b83e0d85a2d43260e21c3`
  - role: default (gas). Replaces the narrow rule of `0050` with the general
    one. The original assembler builds a 64-bit `dli` constant top-down in
    16-bit fields: the first field is aligned to the highest set bit, each
    following field to the highest bit still missing, and the shifts between
    them are merged. Derived from every multi-instruction 64-bit constant in
    the game's expected listings (`0x00053535353106` ->
    `ori 0xa6a6; dsll 16; ori 0xa6a6; dsll 11; ori 0x106`,
    `0x000000ff000000ff` -> `ori 0xff00; dsll 24; ori 0xff`). A plain low mask
    `2^n - 1` keeps `li -1; dsrl`, and values with an all-ones high word keep
    the existing expansion, because the listings show them that way
  - fixtures: `textbin/fun_001f4fb8`, `fun_001ffe18`, `fun_00200600`,
    `fun_00200c80`, `fun_00200e08` byte-identical on the game compiler;
    full-ELF gate PASS, linked ELF hash unchanged
- `0051-r5900-fpr-hazard-exact.patch` SHA-256: `9ff24814d74915e50f3d0297fbf6441881538379897deed00f91525df23584db`
- `0050-r5900-dli-retail-form.patch` SHA-256: `e671ea0cb029a7193d6a54a0ee58d65ae116edcd4674bf88baa3670eacecbe43`
- `0049-sibcall-pass-needs-placeholder.patch` SHA-256: `6b9dae8921c7cdcc9cae3879f92990c969fd098aed7594afa7d98db8c33e714a`
  - cc1 (production stack): `05ff323f6e75accbcec5129b233d7ec16a3045805ea3c2098572985bdcf0a19f`
  - role: default. The Cygnus 2.9 sibcall pass (absent from gcc-2.95.2 and
    from the SN compiler) ran `jump_optimize_minimal` and a CFG rebuild on
    every function before the RTL dump, even with sibling calls off (0022).
    That deleted the jump to the return label after the last `return`, so
    the first jump pass if-converted `return 1; ... return 0;` tails
    (`v0 = 0` hoisted above the first branch, `sltu` into `v0`), which moved
    parameters out of `v0`/`v1`. The pass now returns early when no
    `CALL_PLACEHOLDER` exists; `-mastra-cygnus-cfg` restores the old
    behavior per unit
  - fixture: `textbin/fun_001eb740` -> 100.0 (76.36 before; SN emits the
    same code for the same source)
  - ten promoted sources matched against the old behavior take
    `-mastra-cygnus-cfg` (`configure.py`); full-ELF gate PASS; joint gate
    55/58; without per-unit flags the promoted sweep gains `fun_001fdd10`,
    `fun_00206f50`, `fun_002073b8`, `fun_0020c9e0` (SN-exact sources)
  - published via ASTRA 2026-09-23

- `0047-pathb-reload1-localalloc-regclass-2952.patch` SHA-256: `5817432d64f0835407fe5d68a8364bf628e403aeba35a20fe3472b1bc8fde0fa`
  - role: production dependency. Path B slice 1: the whole gcc-2.95.2
    `reload1.c`, the gcc-2.95.2 `local-alloc.c` equivalence fixes (the hard
    frame pointer is not invariant; `REG_EQUIV` is tested on the
    initialising insn) and the `regclass.c` `'p'` constraint
  - measured: promoted sources 0 lost, pending units +4/-0 (all below 80 %),
    joint gate 55/58, full-ELF gate PASS
  - published via ASTRA 2026-09-23 (production dependency)

- `0045-encode-section-info-sda-nosda.patch` SHA-256: `de44f2608b06689b9e9885d6fa09dcd9f951599ef78d36eee74bce00e7cf7d0f`
  - role: production dependency. `ENCODE_SECTION_INFO`: `DECL_NOSDA` forces
    an absolute reference, `DECL_SDA` a gp-relative one (after SN's
    section/size heuristics); reference half of the SN `sda`/`nosda` port
  - inert without annotations: joint gate 55/58 unchanged, full-ELF PASS
  - published via ASTRA 2026-09-23 (production dependency)

- `0044-sda-nosda-attributes.patch` SHA-256: `b14f446a4f03387aae3a78a21a245e0d10c5274e592fb95673674d3636c5bd46`
  - role: production dependency. SN's `sda`/`nosda` variable attributes
    (`tree.h` decl bits, `c-common.c` handler with SN's mutual exclusion and
    `VAR_DECL`-only rule, `mips_select_section` placement); definition half of
    the port (`.data` <-> `.sdata` proven on an A/B object)
  - inert without annotations: joint gate 55/58 unchanged, full-ELF PASS
  - published via ASTRA 2026-09-23 (production dependency)

- `0000-modern-host-fixes.patch` SHA-256: `90cec417b339e7afd8407e50c49b28a6ad438aa91e5a994f40a25bba0d1d73ad`
  - role: production dependency (host build only, no codegen change):
    `include/obstack.h` lvalue-cast fix for modern host compilers and the
    `gcc/c-gperf.h` that `gperf` generates from `c-parse.gperf`
  - published via ASTRA 2026-09-23 (production dependency)

- `0048-pathb-cse-2952.patch` SHA-256: `8dda70c9c214edd67b1b34febe7b49bb0d3bea5e3bb391c1958e39da46e12e34`
  - cc1 (production stack): `4dfa3cf0f0fa8d31a5efa074d4fede0d31a75c8a3e3d1d5e7f4862968b33aab9`
  - role: default. Path B slice 2: the whole gcc-2.95.2 `cse.c` on the 2.9
    tree, with shims for what the 2.9 tree lacks (`splay_tree_compare_ints`,
    `fnotice`, `do_float_handler`, `shallow_copy_rtx` taken from 2.95.2)
  - fixture: `textbin/fun_00226a70` -> 100.0 (84.26 without it)
  - promoted sources +1 / 0 lost; joint gate 55/58; full-ELF gate PASS with
    `fun_00226a70` promoted
  - published via ASTRA 2026-09-23

- `0046-r5900-pad-unfilled-loops.patch` SHA-256: `a29addcdc9e12bba51192f48e38c628d4ad1d8d4695d0462f31aede6189edf31`
  - cc1 (committed P1..P37 baseline + 0036 + 0044/0045 (installed, unpublished)
    + 0046): `eb7a3497c39e0f73fc4f01bcda7de426c071c8f2f9d4702ffecefb5a6e1c8743`
  - role: default: extend the P19 post-dbr R5900 short-loop padding to loops
    closed by a branch whose delay slot reorg left empty. Such branches (and
    unfilled calls) are emitted in reorder mode and GAS appends the delay NOP,
    so each occupies two words; retail pads the loop to 7 words
    (`jal; nop x4; bnez; nop`). P19 only measured loops closed by a filled
    `SEQUENCE`, so these loops were never padded
  - fixture: `textbin/fun_0012eb20` -> 100.0 (with `-mastra-r5900-extern-buffer`
    and a local `s32 args[4]` argument buffer)
  - wide sweep 282 -> 284 exact (+`runtime/state/read_state_field`,
    +`textbin/fun_0012f208`; 0 regressions across all 557); joint gate 55/58
    unchanged; full-ELF gate PASS with `fun_0012eb20` promoted
  - published via ASTRA 2026-09-22

- `0038-unannul-redundant.patch` SHA-256: `563a644f5a0102ce4150ffb938db6a287d17eb5d47f638a0ccb49091dc1fe767`
  - cc1 (P26..P34 + 0036 + 0037 + 0038 stack): `6b66b17be24bff8c47dafad65432ec1575d32c804cbbb54cfe1b4a1f70bea805`
  - role: default: clear a spuriously-annulled branch (`beqzl` where retail
    keeps plain `beqz`) in the one narrow shape where `reorg.c`'s generic
    delay-slot filler picks the annulled form only because its liveness walk
    (`mark_target_live_regs`/`next_insn_no_annul`) treats the very next
    already-delay-filled branch as opaque and never sees that branch's own
    delay-slot write - a genuine blind spot confirmed present in genuine
    upstream FSF gcc-2.95.2 too, not something this project's own patches
    introduced (reproduced on the bare P1 baseline)
  - root cause found by runtime instrumentation (fprintf traces at every real
    `INSN_ANNULLED_BRANCH_P` assignment site in a rebuilt `cc1`), not static
    source reading - three earlier static theories about this exact mechanism
    were each disproven first; the full trace is kept with the project's
    internal development notes
  - fixture: `textbin/fun_002071c0` -> 100.0. The compiler patch alone only
    narrows this unit's residual (91.111 -> 92.778%, zero movement on every
    other unit in the corpus); reaching exact also required an independent,
    unrelated paired source-shape fix on the same file (an early-return guard
    inverted to the project's established positive-guard pattern)
  - wide sweep 276 -> 277 exact (+1/0 regressions across all 557); joint gate
    55/58 unchanged
  - published via ASTRA 2026-09-20

- `0037-game-no-strict-aliasing.patch` SHA-256: `5c25409ae438dd09e8720c93a0c81898ac3001c30d3f9fe1c59b1ebbbeeed261`
  - cc1 (P26..P34 + 0036 + 0037 stack): `82b332bbd4512c0e7d85c75d8af7d9fef398578b1e68d4b700d2c244aeae4866`
  - role: game-only default: disable strict-aliasing-based optimization at
    `-O2` (matches SN/retail codegen for pointer-aliased loads/stores); scoped
    to the game compiler, not the SDK route
  - fixtures: `assembly/textbin/fun_001f6250`, `textbin/fun_001fe898`,
    `textbin/attach_manipulator`, `textbin/audio_dec_begin_put`,
    `textbin/fun_00233980`, `textbin/fun_0012ee08` -> 100.0
  - game-only route-aware battery: 175/213 -> 178/213 exact (+3/0); P37
    controls 55/58 (not a clean same-stack comparator against the historical
    52/58 receipt)
  - Path-A: all four compiled objects link byte-equal to their retail slices;
    full ELF gate remains blocked by the known missing-expected-object units
    `fun_00221968`/`fun_002267b8` (documented exception, not a compiler gap)
  - published via ASTRA 2026-09-20

- `0036-retire-frame-save-pref.patch` SHA-256: `0c0970ecc7e2ca5d18bc3c1e1609a17d3600b35511a400a11e54ea6d0dee7711`
  - cc1 (P26..P34 + 0036 stack): `5c1bf8e5bcf0c40d03149bc1b50fd470ff260054f192de4844e77e77637940ea`
  - role: retire the P26/P33 frame-store preference. With P29's call-clobber
    graph the preference is no longer needed: all six P26 fixtures stay exact
    without it, and removing it fixes the last blocked `$ra`-placement residual
  - fixture: `textbin/fun_00201128` -> 100.0 (the `$ra` save now follows the
    last argument captures, matching retail)
  - evidence: sched2 ready lists vs the SN oracle are identical at all 38
    cycles only after the removal (with P26/P33, t=6/t=7 differ); the deciding
    pairs (ra save 100 vs capture moves 10/12) are never directly compared by
    qsort, so pairwise predicates could not act
  - parity: wide 272 -> 273 (+1 / zero regressions across all 557); joint gate
    54/58 unchanged; installed production cc1 `5c1bf8e5bcf0c40d`
  - note: 0026/0033 remain in the stack history and are neutralized by 0036;
    `0035-extern-buffer-default` is not part of the production stack (P34 stays
    opt-in)

- `0035-extern-buffer-default.patch` SHA-256: `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855`
  - cc1 (P26..P35 stack): `f920538211f2b03699b15e6f8c97e4cc302e1b556f39bd0d8488e0cf5fa2d476`
  - role: enable the P34 R5900/GAS extern buffering by default
  - fixtures: `textbin/fun_0021f8e8`, `textbin/snd_unk_function_0012eb00` -> 100.0
  - in-scope textbin: +2 / 0 (161/213 exact); the 8 all-557 sweep regressions
    are units owned by other routes (patched profile, padless assembler,
    native EE-GCC 2.9) and are unaffected in the real build
  - joint gate 54/58 unchanged


- `0034-r5900-extern-buffer-optin.patch` SHA-256: `085ac4b30bf79e56c9f51c58013ac22cd77b62456c8f6669e909bca5435088bc`
  - cc1 (P26..P34 stack): `ad58bb494426663dcf73317db34eaea91c33449039723be9bf2e905c2deb09ab`
  - role: opt-in R5900/GAS extern buffering (`-mastra-r5900-extern-buffer`):
    the body is buffered and the `.extern name,size` declarations of
    GP-eligible bare integer references are emitted in front of it, so GAS can
    pick the retail gp-relative form; `TARGET_FILE_SWITCHING` is untouched
  - default-neutral: flag off = 272 exact, zero diffs
  - flag on: +4/-8 on the corpus; the 2 in-scope conversions are
    `textbin/fun_0021f8e8`, `textbin/snd_unk_function_0012eb00`; all 8
    regressions are units owned by other routes (patched profile, padless
    assembler, non-textbin)


- `0033-ra-not-vs-nonframe.patch` SHA-256: `c399a2055c8a4b743b5b39a826bc72c29a1f0b7d552e98f627a34777042a52e1`
  - cc1 (P26..P33 stack): `ac97dd50a166706506047e3ff1beed084cd2c5f070564357884e5bc449517165`
  - role: the P26 frame-save preference no longer applies to the `$ra` save
    against non-frame instructions (its priority/depend-count order is kept,
    matching retail), while `$ra` vs other saves is unchanged
  - fixtures: `textbin/fun_002144d8`, `textbin/fun_00238520` -> 100.0
  - parity: wide 270 -> 272 (+2 / zero regressions); joint gate 54/58 unchanged


- `0032-anchor-all-pads.patch` SHA-256: `da022fae877139914004aefc9e20293b5e7bc2d5fbf6a173602ca2bbf8ffd8dc`
  - cc1 (P26..P32 stack): `bc32cb311c2908fdce2ef582a1741582aec130725c863c9d58800737f4cd6307`
  - role: place every pad NOP before the instruction preceding the branch so
    the delay-slot filler sees that instruction first (retail geometry: two
    pads before the branch, loop increment in the slot)
  - fixture: `textbin/fun_002172c0` -> 100.0
  - parity: wide 268 -> 270 (+2 / zero regressions); joint gate 54/58 unchanged


- `0031-annul-traced-comparison.patch` SHA-256: `f1b0c7b29178733ef9e9a0994efb3704d736f19b4bd322480b5d11d0ad67d60b`
  - cc1 (P26..P31 stack): `11cd3fac9a5bd1dad6edc57215a27f2d6ff028f3b923eea9913b4061b0f17e2f`
  - role: default branch-likely fallback for branches whose tested register was
    produced by an arithmetic comparison (`slt` etc.); uses the branch's own
    zero test, safe because not-taken of `ne dest,0` implies dest is zero
  - fixture: `textbin/fun_00220790` -> 100.0
  - parity: wide 268 -> 269 (+1 / zero regressions); joint gate 54/58 unchanged


- `0030-pad-before-preceding.patch` SHA-256: `69c094e45910293e0d05a11a8fd6818a5a871246b69136bd6fbddcd01b5bf148`
  - cc1 (P26+P27+P28+P29+P30 stack): `3fc3a8e6aa24a0c9a538dae5cc933b1e3ace737dfabd2115c2a08bbd39c7fcd8`
  - role: R5900 short-loop padding placement; a single pad NOP goes before the
    instruction preceding the branch so the delay-slot filler can place a
    useful instruction (movn) in the slot, with the NOP as fallback before the
    branch (retail layout)
  - fixtures: `textbin/fun_00215290`, `textbin/fun_00215300`,
    `textbin/fun_00215348` -> 100.0
  - in-scope textbin parity: +3 / 0 (155/213 exact); all-557 sweep 267 (the
    four regressions are sdk/* units owned by the other compiler);
    joint gate 54/58 unchanged


- `0029-call-clobber-pending.patch` SHA-256: `c3031b4a0c88a03b19f82f74feb31455c5ee86f06f538ad869fbf9cf734f2af0`
  - cc1 (P26+P28+P29 stack): `864f7d4e7fa77d2fba88f84831e804ad29b807a8439ff0d93436514cb3ae4b53`
  - role: port the GCC 2.95.2 call-clobber dependency analysis (pending
    clobbers instead of `reg_pending_sets`; keep `reg_last_uses`)
  - effect: the a0/a1 argument-setup order, the `sq ra`/`sq s0` save order and
    several scheduler tie-break classes now match SN by default
  - wide promoted parity: 237 -> 266 exact (+29 / zero regressions);
    joint gate 52 -> 54/58
  - fixtures: fun_001f6940, fun_001f69d0, fun_001f6a60, fun_001f6af0,
    fun_001f6b88, fun_001f6c20, fun_0021eaf0, fun_0021fd78, fun_00221a88,
    fun_00222f58, fun_0023aa68, fun_0023bf18, fun_0023bb40, fun_0023bbb0


- `0028-annul-ne-zero-default.patch` SHA-256: `037ca6b28fdd05e81a7bf37fcbe642068f633d3aae315b77f72959c0939690e4`
  - cc1 (P26+P27+P28 stack): `d68136c9f12b886e0da0c5d4d7fa36a23b622fa569989b17a50e3ec6e7b530fb`
  - role: default branch-likely for the late fallback; the P25 pass now runs
    without a flag and only for the SN shape (NE zero-test with a const0
    delay-slot write of the tested register)
  - fixtures: `textbin/fun_0023d2d8`, `textbin/vo_buf_get_data` -> 100.0
  - wide promoted parity: 237 -> 239 exact (+2 / zero regressions);
    joint gate 52/58 unchanged
  - counterexamples kept: `fun_002212b8` (EQ zero-clear, retail plain),
    `fun_00215b10` (register-vs-register `bne`, retail plain)


- `0027-gas-inline-float-literals.patch` SHA-256: `0272bee08529c415490f3f565f5a38846a98872fb972c81f8f17f07682605682`
  - as (gas, with P20): `af95ed125045dcc0dd3549b25e2d73f44ed03b283d4129d09979e879eab34fff`
  - role: default assembler policy; synthesize `li.s` constants inline
    (lui/ori/mtc1) instead of pooling them in `.lit4`, matching the retail SN
    R5900 assembler; `li.d`/.lit8 pooling untouched
  - fixtures: `textbin/fun_00206e18`, `textbin/fun_00207300`,
    `textbin/fun_00213308`, `textbin/fun_002133d0` -> 100.0
  - wide promoted parity: 233 -> 237 exact (+4, zero regressions) on the
    production route; joint gate 52/58 unchanged


- `0026-frame-save-first.patch` SHA-256: `9d42deb1f3e92ce6ae8f46ae1672a5c03ca93f9fd80d3f4f664461c2c03a12de`
  - cc1 (P1+P15+P22+P16+P19+P21+P25+P26 stack): `9ea59250181268d93c75de980c04679e3ae529a262db820408836d4c4f35df6f`
  - role: default scheduler order; prefer frame-related stores (prologue saves)
    in `rank_for_schedule`, reproducing retail's save/move interleaving in the
    prologue (the previous chain let the depend-count heuristic rank the
    anti-dependent parameter move ahead of the next save)
  - fixtures: `textbin/fun_001f6530`, `textbin/fun_001f65b0`,
    `textbin/fun_001f6fd0`, `textbin/fun_001f7580`, `textbin/fun_001f75f0`,
    `textbin/fun_00214530` -> 100.0
  - wide promoted parity: 227 -> 233 exact (+6, zero regressions) on the
    production route (ELF as); 231 -> 237 through the ASTRA wrapper route
  - joint gate: 52/58 controls, zero change
  - rejected variants (all measured): remove depend_count 15/28; anti-dep
    class 2 1/7; flip sign 7/47; fewer true deps 9/65; prefer any memory insn
    5/12; prefer all frame-related insns 6/1


- `0001-r5900-quad-saves.patch` SHA-256: `13f54afb0045ea80d98e03b1b31417593f11abfaaf6fc254a8d41d750f75406f`
  - role: R5900 quadword-save parity prerequisite for the cumulative game compiler
- `0015-no-sibcall.patch` SHA-256: `63ff746052f682037413ce7188c3622786ec829674500de36d8bb5d950ceb071`
  - cc1: `5f8c800390c82137e3f25de82ec22b8868ede84c54034cab6d8e8844cf3c4193`
  - fixtures (with `-mastra-no-sibcall`; retail objects have `jal` + epilogue,
    no tail `j`): `assembly/textbin/fun_001f6250`, `textbin/fun_00201f58`,
    `textbin/fun_0020d3b0`, `textbin/fun_00235840`, `textbin/memcard_init`
    -> 100.0
  - counter-examples kept on the default route (retail has a tail `j`):
    `rendering/image/set_image_buffer_flag`, `sdk/dma/sce_mpeg_init`
- `0016-no-edge-lcm-default.patch` SHA-256: `7a383321db39ad13f82703530909b99e34eb5b04aac8988746eaeda1035f0a15`
  - cc1: `f5c4dd418dc3281eecf00faedd59319e8f504d1c3ae79fcbaa3664f3dff28a1a`
  - fixture: `textbin/fun_001fe898` -> 100.0 with the default route

- `0019-r5900-post-dbr-loop-pad.patch` SHA-256: `9be60b68f35cf775bce2f0b677cd0b3ba459d8531649d99b6993b0d83e290562`
  - cc1 (P1+P15+P16+P19 stack): `5db99bf745e1df0c77d649ca2f2d6c50ef5619bbd0e7bd46c6cb938624512b5f`
  - role: reproduce the retail R5900 assembler's short-loop padding in cc1 after
    delay-slot scheduling; backward-branch loops shorter than 7 instructions get
    NOPs immediately before the branch
  - fixtures: `textbin/fun_001ff480`, `textbin/fun_002212b8`,
    `textbin/fun_002242b8`, `textbin/fun_00225660`, `textbin/fun_0022da68` -> 100.0
  - control parity: 58 controls, 21 -> 26 exact, zero regressions

- `0020-gas-absolute-unknown-symbol.patch` SHA-256: `3e961d443cc8e6dedd89ef82c7afa2bfdb791b65a16949a2b22fc5855b0fa0c6`
  - component: gas from the same recovered tree; assembler SHA-256
    `4f76f5b6eca8e66240ab7b98efef8071a6740b8d33e32238427dbcba9f87cfba`
  - role: reproduce the retail R5900 absolute/gp decision: a reference to a
    symbol whose size is not yet known at the use (no `.extern` seen yet)
    becomes `lui %hi` + `op %lo` in reorder and noreorder alike (the noreorder
    case follows the same size-at-use rule), known-small symbols stay
    `%gp_rel`, and inside a noreorder block a known symbol is the single
    gp-relative instruction emitted at the use
  - fixtures: `textbin/fun_0023b590`, `textbin/fun_0023aba0`,
    `runtime/dma/initialize_streaming_state` (with `-mno-split-addresses`), plus
    `textbin/fun_001eda60` and `textbin/fun_00233d90` (default route) -> 100.0
  - control parity: 58 controls, 34 -> 36 exact, zero exact regressions

- `0021-sched-keep-frame-related-order.patch` SHA-256: `5d9feea0d193c4dc7dc376129661084531392dab69735c4b11fcadc2912d9ff5`
  - cc1 (P1+P15+P16+P19+P20+P21 stack): `52099c6af010389c59593b3f05750a3411634e45ed97da6c6a886e1dee5d2cd9`
  - role: opt-in `-mastra-keep-frame-order` keeps prologue saves / epilogue
    restores in `save_restore_insns` emission order under the scheduler
    (default scheduler behavior is unchanged, since retail objects mix both
    orders across units)
  - fixtures: `textbin/fun_0023c610` -> 100.0 with
    `-fno-schedule-insns -mastra-keep-frame-order`; `textbin/fun_0023aa68`
    -> 100.0 with `-mastra-keep-frame-order`
  - parity: joint gate 50 -> 52 with zero exact regressions

- `0022-sibcall-default-off.patch` SHA-256: `b5749a54934b569190571ea5a867f20e6508e9e4ed349333fffc73449a0e2ea0`
  - cc1 (P1+P15+P22+P16+P19+P20+P21 stack): `ac549856a020b23b608052c3a6b66bfb52d47ccf3662d318ca725e5091993c98`
  - role: sibling-call expansion off by default (retail behavior);
    `-mastra-sibcall` re-enables it per unit
  - evidence: 547 measurable promoted sources -> 37 converted, 6 regressions
    (all genuine retail tail calls); wide 557-source sample exactness 196 -> 231
  - fixtures: `textbin/fun_001f6250`, `textbin/fun_00201f58`,
    `textbin/fun_0020d3b0`, `textbin/fun_00235840`, `textbin/memcard_init` on the
    default route; `rendering/image/set_image_buffer_flag` stays exact with `-mastra-sibcall`

- `0025-annul-dead-delay-slots.patch` SHA-256: `7ea7e0ecdb20ed4be7777eaada61299adb5e1a3a5563b2146b6b505fb1d59659`
  - cc1 (stack through P25): `4622bb1bd38a03dece9ee49cd2ccc3069c57aff690b32a8fcfdb66668cf00121`
  - role: opt-in `-mastra-annul-dead-slots`; annuls a branch whose
    delay slot writes the branch-tested register and that register is dead
    on the not-taken path (retail `bnezl v0,L; move v0,zero`)
  - default-neutral (wide sweep byte-identical with the flag off)
  - fixtures: `textbin/fun_0023d2d8`, `textbin/vo_buf_get_data` -> 100.0
  - rejected as a default: +2 / -9 narrowed, +2 / -29 blanket

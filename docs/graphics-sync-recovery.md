# Provisional graphics synchronization recovery

FUN_00120558 at 0x00120558 now has descriptive NON_MATCHING C for its complete
polling algorithm. Keep the 140-byte retail oracle; this is not an exact-C
promotion. Both incoming argument registers are unused. Each pass observes,
in order, 32-bit VIF1 DMA CHCR at 0x10009000, GIF DMA CHCR at 0x1000a000,
VIF1 STAT at 0x10003c00, COP2 control register 29, and GIF STAT at 0x10003020.
The respective busy masks are 0x100, 0x100, 3, 0x100 and 0xc00. Every source
is read even when another indicates busy; repeat while any masked condition is
nonzero and return zero on completion. There is no timeout or hardware write.

include/ee_cop2.h makes the unavoidable COP2 instruction explicit outside the
recovered polling body. Its low-level accessor uses cfc2.ni, the observed
following nop, and a compiler memory clobber to preserve MMIO observation
order. This hardware accessor is not a retail function call or MMIO alias.
Consumers on other architectures must supply an equivalent explicit control
register read contract; returning zero without owning idle hardware state is
not a general implementation. The hardware accessor remains assembly even
though the polling logic is C. Removing this unit from intentional_asm admits
its C goal; it does not establish a pure-C replacement for COP2.

The installed Sony/Cygnus EE SDK compiler builds the fallback at -O2. Its
candidate text is 128 bytes versus the resident's 140; objdiff reports
76.828575 percent text similarity. The first differing region replaces the
retail constant/move/conditional-zero sequence with a shift and bit mask;
register allocation and scheduling also differ. Other sections are not claimed
to match. The generated candidate retains all five observation positions,
the same masks, loop-back condition and zero return.

Private instruction review uses the verified resident SHA256
e050581032e4bb3f20341307da5b69b76f1574910519155380ea771e55c3c0c9.
No resident instructions, object files or game data are committed. SDK
compilation and static review do not establish dynamic PS2 behavior, hardware
timing or a strict byte-matching gate. The public tool suite passes 78 cases.

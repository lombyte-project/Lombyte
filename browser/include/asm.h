#ifndef RCP2P_ASM_H
#define RCP2P_ASM_H

/*
 * Browser replacement for include/asm.h. It shadows the original only for WebAssembly builds,
 * because browser/include comes first on their include path. The matching PS2 build never sees it.
 *
 * In the matching build, INCLUDE_ASM pastes the original MIPS listing in as inline assembly.
 * WebAssembly cannot run MIPS, so here it expands to nothing: the function keeps no body, and the
 * C functions beside it in the same file still compile. A caller of an assembly-only function
 * then links to a missing import, which the spike runner turns into a trap naming that function,
 * until it is ported (browser/level-math.c, browser/spike/ee-helpers.c).
 */
#define INCLUDE_ASM(path, symbol)

#endif /* RCP2P_ASM_H */

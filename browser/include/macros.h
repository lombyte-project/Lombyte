#ifndef RCP2P_MACROS_H
#define RCP2P_MACROS_H

/* Browser replacement for include/macros.h (browser/include comes first on WebAssembly builds'
   include path). Both macros are scheduling hints for the matching build and emit no code;
   REG_LIVE names an EE register, which WebAssembly has no equivalent of, so it is empty here. */
#define BARRIER() __asm__ volatile("")
#define REG_LIVE(reg) do { } while (0)

#endif /* RCP2P_MACROS_H */

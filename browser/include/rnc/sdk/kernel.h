/* Browser build: the recovered kernel header, with the EE pipeline syncs (sync.l / sync.p, inline
   MIPS) as compiler barriers. WebAssembly runs memory operations in order; the syncs only had to
   keep the compiler from moving accesses across them. */
#include_next "rnc/sdk/kernel.h"
#undef CpuSync
#undef CpuSyncP
#define CpuSync() __asm__ __volatile__("" ::: "memory")
#define CpuSyncP() __asm__ __volatile__("" ::: "memory")

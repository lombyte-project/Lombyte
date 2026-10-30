#include "types.h"
extern void sceSifInitRpc(int mode);
void initialize_sif_rpc(void) __asm__("FUN_00232ce0");

void initialize_sif_rpc(void) {
    sceSifInitRpc(0);
}

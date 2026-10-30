/* Mobys: sets the moby state to 1. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "sda.h"

/* Sets the moby state byte to 1. */
void moby_set_state_one(unsigned char *moby) __asm__("FUN_L14_002ffc08");

void moby_set_state_one(unsigned char *moby) {
    moby[0x20] = 0x1;
}

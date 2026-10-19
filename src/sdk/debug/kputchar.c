#include "types.h"
#include "eeregs.h"

s32 kputchar(s32 c) {
    while (*SIO_ISR & 0x8000) {
    }
    *SIO_TXFIFO = c;
    return c;
}

#include "types.h"
#include "rnc/sdk/eeregs.h"

s32 kputchar(s32 character) {
    while (*SIO_ISR & 0x8000) {
    }
    *SIO_TXFIFO = character;
    return character;
}

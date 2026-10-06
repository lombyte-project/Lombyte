#ifndef LOMBYTE_STRUCTS_H
#define LOMBYTE_STRUCTS_H

#include "types.h"

/* State block used by the 0x216088 status-reset helper. */
typedef struct RncCoreState {
    u8 unknown_00[0x40];
    u16 status_40;
    u8 unknown_42[0x1a];
    u16 status_5c;
    u8 unknown_5e[0x1a];
    u16 status_78;
} RncCoreState;

#endif /* LOMBYTE_STRUCTS_H */

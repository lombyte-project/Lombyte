#include "types.h"

struct Pad2StateEntry {
    s32 state;
    s32 request;
    s32 result;
};

extern u32 pad2_initialized[] __asm__("D_00132DD0");
extern u8  D_0015B540[];

s32 scePad2Init(void) {
    struct Pad2StateEntry *pad2_socket_record;

    pad2_initialized[0] = 1;
    pad2_socket_record = (struct Pad2StateEntry *)D_0015B540;
    do {
        pad2_socket_record->state = 0;
        pad2_socket_record->request = 0;
        pad2_socket_record->result = 0;
        pad2_socket_record = (struct Pad2StateEntry *)((u8 *)pad2_socket_record + 0x330);
    } while ((s32)pad2_socket_record < (s32)(D_0015B540 + 0x3300));
    return 1;
}

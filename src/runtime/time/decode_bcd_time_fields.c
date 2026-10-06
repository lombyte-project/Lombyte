#include "types.h"
struct BcdClockTime {
    u8 pad_0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad_4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
};

extern s32 BcdToTime(u8 value);

void decode_bcd_time_fields(struct BcdClockTime *time) __asm__("FUN_0012d3c0");

void decode_bcd_time_fields(struct BcdClockTime *time) {
    time->unk7 = BcdToTime(time->unk7);
    time->unk6 = BcdToTime(time->unk6);
    time->unk5 = BcdToTime(time->unk5);
    time->unk3 = BcdToTime(time->unk3);
    time->unk2 = BcdToTime(time->unk2);
    time->unk1 = BcdToTime(time->unk1);
}

extern void func_0012D3C0(struct BcdClockTime *) __attribute__((alias("FUN_0012d3c0")));

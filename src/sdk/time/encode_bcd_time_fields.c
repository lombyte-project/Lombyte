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

extern s32 TimeToBcd(u8 value);
void encode_bcd_time_fields(struct BcdClockTime *time) __asm__("FUN_0012d428");

void encode_bcd_time_fields(struct BcdClockTime *time) {
    time->unk7 = TimeToBcd(time->unk7);
    time->unk6 = TimeToBcd(time->unk6);
    time->unk5 = TimeToBcd(time->unk5);
    time->unk3 = TimeToBcd(time->unk3);
    time->unk2 = TimeToBcd(time->unk2);
    time->unk1 = TimeToBcd(time->unk1);
}

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
struct MpegDecoder {
    u8 pad_0[0x150];
    s32 unk150;
    u8 pad_154[0x58];
    s32 unk1AC;
    u8 pad_1B0[0x69C];
    s32 unk84C;
    s32 unk850;
    s32 unk854;
};
void UpdateTempTrackData(struct MpegDecoder *arg0, s32 delta) {
    s32 temp_2_30;
    s32 temp_3_21;
    s32 temp_4_32;
    s32 var_4_8;
    s32 var_7_4;
    var_7_4 = 0;
    var_4_8 = 0;
    if ((arg0->unk150 != 3) && (delta != 0)) {
        if (delta < 0) {
            var_7_4 = arg0->unk854 == 0;
        }
        arg0->unk854 = 0;
        var_4_8 = delta;
    }
    temp_3_21 = arg0->unk84C + delta;
    arg0->unk1AC = temp_3_21;
    if ((var_7_4 != 0) && (var_4_8 >= delta)) {
        arg0->unk1AC = (s32)(temp_3_21 + 0x400);
    };
    temp_4_32 = arg0->unk1AC;
    arg0->unk850 = (s32)((arg0->unk850 < temp_4_32) ? (temp_4_32) : (arg0->unk850));
}

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};
struct M2c_temp_2_7 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
};
extern s32 GetRpcPacket();
extern void isceSifSendCmd();
void _request_rdata(struct M2c_arg0 *arg0, s32 arg1) {
    s32 a;
    s32 b;
    struct M2c_temp_2_7 *temp_2_7;
    temp_2_7 = GetRpcPacket(arg1);
    a = (s32)arg0->unk14;
    b = (s32)arg0->unk1C;
    temp_2_7->unk14 = a;
    temp_2_7->unk1C = b;
    temp_2_7->unk20 = 0x8000000C;
    isceSifSendCmd(0x80000008, temp_2_7, 0x40, arg0->unk20, arg0->unk24, arg0->unk28);
}

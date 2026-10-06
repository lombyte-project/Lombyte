typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
struct M2c_D_00130310 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};
extern u32 D_00130308[];
extern struct M2c_D_00130310 D_00130310;
extern s32 InvokeKernelSyscall0083_Rfu();
extern s32 Rfu116SetSyscallMid();
extern void FindKernelAddress();
extern void KernelCopyRdata();
void InitSystemCallTableAddress(void) {
    s32 var_18_32;
    s32 var_19_26;
    s32 var_2_36;
    u32 var_16_33;
    u32 var_17_31;
    Rfu116SetSyscallMid(D_00130310.unk0, D_00130310.unk4);
    Rfu116SetSyscallMid(D_00130310.unk8, D_00130310.unkC);
    var_19_26 = InvokeKernelSyscall0083_Rfu(0x80000000, 0x80080000, &FindKernelAddress);
    var_17_31 = var_19_26 - 0x20C;
    var_18_32 = InvokeKernelSyscall0083_Rfu(0x80000000, 0x80080000, &KernelCopyRdata);
    var_16_33 = var_18_32 - 0x168;
    if (var_17_31 != var_16_33) {
        ;
        do {
            if ((var_17_31 < var_16_33) != 0) {
                var_19_26 =
                    InvokeKernelSyscall0083_Rfu(var_19_26 + 4, 0x80080000, &FindKernelAddress);
                var_17_31 = var_19_26 - 0x20C;
            } else {
                var_18_32 =
                    InvokeKernelSyscall0083_Rfu(var_18_32 + 4, 0x80080000, &KernelCopyRdata);
                var_16_33 = var_18_32 - 0x168;
            }
            var_2_36 = var_17_31 < var_16_33;
        } while (var_17_31 != var_16_33);
    }
    D_00130308[0] = var_17_31;
}

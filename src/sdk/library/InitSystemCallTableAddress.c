typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
struct SyscallMidTable {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};
extern u32 D_00130308[];
extern struct SyscallMidTable D_00130310;
extern s32 InvokeKernelSyscall0083_Rfu();
extern s32 Rfu116SetSyscallMid();
extern void FindKernelAddress();
extern void KernelCopyRdata();
void InitSystemCallTableAddress(void) {
    s32 copy_cursor;
    s32 find_cursor;
    s32 var_2_36;
    u32 copy_addr;
    u32 find_addr;
    Rfu116SetSyscallMid(D_00130310.unk0, D_00130310.unk4);
    Rfu116SetSyscallMid(D_00130310.unk8, D_00130310.unkC);
    find_cursor = InvokeKernelSyscall0083_Rfu(0x80000000, 0x80080000, &FindKernelAddress);
    find_addr = find_cursor - 0x20C;
    copy_cursor = InvokeKernelSyscall0083_Rfu(0x80000000, 0x80080000, &KernelCopyRdata);
    copy_addr = copy_cursor - 0x168;
    if (find_addr != copy_addr) {
        ;
        do {
            if ((find_addr < copy_addr) != 0) {
                find_cursor =
                    InvokeKernelSyscall0083_Rfu(find_cursor + 4, 0x80080000, &FindKernelAddress);
                find_addr = find_cursor - 0x20C;
            } else {
                copy_cursor =
                    InvokeKernelSyscall0083_Rfu(copy_cursor + 4, 0x80080000, &KernelCopyRdata);
                copy_addr = copy_cursor - 0x168;
            }
            var_2_36 = find_addr < copy_addr;
        } while (find_addr != copy_addr);
    }
    D_00130308[0] = find_addr;
}

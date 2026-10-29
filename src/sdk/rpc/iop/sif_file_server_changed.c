typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;
extern u8                  D_0012FBEC[];
extern u32                 server_version_record_address[] __asm__("D_0012FCA8");
extern s32                 memcmp();
extern u8                  FsResetBuffer[] __asm__("D_00157FA8");
s32                        sif_file_server_changed(void) __asm__("FUN_0011bbb8");

s32 sif_file_server_changed(void) {
    s32 all_version_records_differ;
    u8 *new_var;
    all_version_records_differ = 0;
    new_var = D_0012FBEC;
    if ((memcmp(FsResetBuffer, new_var, 4) != 0) &&
        (memcmp(FsResetBuffer, server_version_record_address[0], 4) != 0)) {
        all_version_records_differ = memcmp(new_var, server_version_record_address[0], 4) != 0;
    }
    return all_version_records_differ;
}

extern s32 func_0011BBB8(void) __attribute__((alias("FUN_0011bbb8")));

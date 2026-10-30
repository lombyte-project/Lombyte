typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;
extern u8                  D_0012FBEC[];
extern u32                 sif_load_module_version_reference[] __asm__("D_0012FCB4");
extern u8                  D_00158428[];
extern s32                 memcmp();
s32                        sif_load_module_server_changed(void) __asm__("FUN_0011cae0");

s32 sif_load_module_server_changed(void) {
    u8 *new_var;
    s32 result;
    result = 0;
    new_var = D_0012FBEC;
    if ((memcmp(D_00158428, new_var, 4) != 0) &&
        (memcmp(D_00158428, sif_load_module_version_reference[0], 4) != 0)) {
        result = memcmp(D_0012FBEC, sif_load_module_version_reference[0], 4) != 0;
    }
    return result;
}

extern s32 func_0011CAE0(void) __attribute__((alias("FUN_0011cae0")));

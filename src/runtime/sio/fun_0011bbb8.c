typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
extern u8 D_0012FBEC[];
extern u32 D_0012FCA8[];
extern u8 D_00157FA8[];
extern s32 memcmp();
s32 FUN_0011bbb8(void) {
    s32 result;
    u8 *new_var;
    result = 0;
    new_var = D_0012FBEC;
    if ((memcmp(D_00157FA8, new_var, 4) != 0) && (memcmp(D_00157FA8, D_0012FCA8[0], 4) != 0)) {
        result = memcmp(new_var, D_0012FCA8[0], 4) != 0;
    }
    return result;
}

extern s32 func_0011BBB8(void) __attribute__((alias("FUN_0011bbb8")));

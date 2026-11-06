typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;

/* newlib struct __sFILE */
struct StdioFile {
    s32 _p;
    s32 _r;
    s32 _w;
    u16 _flags;
    u16 _file;
    s32 _bf_base;
    s32 _bf_size;
    s32 _lbfsize;
    s32 _cookie;
    s32 _read;
    s32 _write;
    s32 _seek;
    s32 _close;
    u8  pad_30[0x24];
    s32 _data;
};

extern void __sread();
extern void __sseek();
extern void __swrite();
extern void __sclose() __asm__("func_00116408");

void std(struct StdioFile *fp, s16 flags, s16 fd, s32 reent) {
    int               new_var2;
    struct StdioFile *new_var;
    fp->_data = reent;
    new_var = fp;
    new_var->_flags = (unsigned long)flags;
    new_var->_file = fd;
    fp->_read = &__sread;
    new_var->_write = &__swrite;
    new_var->_seek = &__sseek;
    new_var->_close = &__sclose;
    do {
        new_var2 = 0;
        fp->_p = new_var2;
        fp->_r = new_var2;
        new_var->_w = new_var2;
        new_var->_bf_base = new_var2;
        fp->_lbfsize = new_var2;
    } while (0);
    new_var->_cookie = fp;
}

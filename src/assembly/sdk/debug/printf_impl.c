#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/debug/printf_impl/_printf.s", _printf);
#else
#include "types.h"

extern s32 DIntr(void);
extern s32 EnableInterrupts(void);
extern void printfloat(double);
extern void (*D_0012FC00[])(s32);

#define ARG(T) (ap += 8, *(T *)(ap - 8))

void _printf(const char *fmt, char *ap) {
    char buf[32];
    char *pad;
    char *p;
    s32 mod;
    s32 n;
    unsigned long d;
    u8 ch;
    char second;
    s64 v;
    s32 ie;

    ie = DIntr();
    for (; *fmt != 0; fmt++) {
        pad = 0;
        mod = 0;
        if (*fmt == '%') {
        again:
            fmt++;
            switch ((char)(*fmt - '0')) {
            case 0:
                n = fmt[1] - '0';
                second = fmt[2];
                if ((u8)n < 10) {
                    if ((u32)(second - '0') < 10) {
                        n = n * 10 + (second - '0');
                        fmt += 2;
                        if (n >= 32) {
                            n = 31;
                        }
                    } else {
                        fmt += 1;
                    }
                    pad = &buf[31 - n];
                    while (n > 0) {
                        buf[31 - n] = '0';
                        n--;
                    }
                    goto again;
                }
                goto again;
            case 'l' - '0':
                mod = 'l';
                goto again;
            case 'h' - '0':
                mod = 'h';
                goto again;
            case 'o' - '0':
                if (mod == 'l') {
                    v = ARG(s64);
                } else if (mod == 'h') {
                    v = ARG(u16);
                } else {
                    v = ARG(u32);
                }
                p = &buf[31];
                *p = 0;
                if (v == 0) {
                    *--p = '0';
                } else {
                    do {
                        *--p = (v & 7) + '0';
                        v = (u64)v >> 3;
                    } while (v != 0);
                }
                if (pad != 0 && pad < p) {
                    p = pad;
                }
                while (*p != 0) {
                    D_0012FC00[0](*p++);
                }
                break;
            case 'x' - '0':
                if (mod == 'l') {
                    v = ARG(s64);
                } else if (mod == 'h') {
                    v = ARG(u16);
                } else {
                    v = ARG(u32);
                }
                p = &buf[31];
                *p = 0;
                if (v == 0) {
                    *--p = '0';
                } else {
                    do {
                        d = v & 0xF;
                        ch = (d < 10) ? d + '0' : d + 'a' - 10;
                    --p;
                    *p = ch;
                        v = (u64)v >> 4;
                    } while (v != 0);
                }
                if (pad != 0 && pad < p) {
                    p = pad;
                }
                while (*p != 0) {
                    D_0012FC00[0](*p++);
                }
                break;
            case 'd' - '0':
                if (mod == 'l') {
                    v = ARG(s64);
                } else if (mod == 'h') {
                    v = ARG(s16);
                } else {
                    v = ARG(s32);
                }
                p = &buf[31];
                *p = 0;
                if (v == 0) {
                    *--p = '0';
                } else {
                    if (v < 0) {
                        v = -v;
                        D_0012FC00[0]('-');
                    }
                    while (v != 0) {
                        *--p = v % 10 + '0';
                        v = v / 10;
                    }
                }
                if (pad != 0 && pad < p) {
                    p = pad;
                }
                while (*p != 0) {
                    D_0012FC00[0](*p++);
                }
                break;
            case 'u' - '0':
                if (mod == 'l') {
                    v = ARG(s64);
                } else if (mod == 'h') {
                    v = ARG(u16);
                } else {
                    v = ARG(u32);
                }
                p = &buf[31];
                *p = 0;
                if (v == 0) {
                    *--p = '0';
                } else {
                    do {
                        *--p = (u64)v % 10 + '0';
                        v = (u64)v / 10;
                    } while (v != 0);
                }
                if (pad != 0 && pad < p) {
                    p = pad;
                }
                while (*p != 0) {
                    D_0012FC00[0](*p++);
                }
                break;
            case 'e' - '0':
            case 'f' - '0': {
                f32 f = ARG(f32);
                if (f == 0.0f) {
                    D_0012FC00[0]('0');
                } else {
                    printfloat(f);
                }
            } break;
            case 's' - '0':
                p = ARG(char *);
                if (*p == 0) {
                    D_0012FC00[0]('(');
                    D_0012FC00[0]('n');
                    D_0012FC00[0]('u');
                    D_0012FC00[0]('l');
                    D_0012FC00[0]('l');
                    D_0012FC00[0](')');
                } else {
                    if (*p != 0) {
                        do {
                            D_0012FC00[0](*p++);
                        } while (*p != 0);
                    }
                }
                break;
            case 'c' - '0':
                v = ARG(char);
                D_0012FC00[0](v);
                break;
            default:
                break;
            }
        } else {
            D_0012FC00[0](*fmt);
        }
    }
    if (ie != 0) {
        EnableInterrupts();
    }
}
#endif /* NON_MATCHING */

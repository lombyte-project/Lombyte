#include "types.h"

extern s32 DIntr(void);
extern s32 EnableInterrupts(void);
extern void printfloat(double);
extern void (*D_0012FC00[])(s32);

#define ARG(T) (ap += 8, *(T *)(ap - 8))

void _printf(const char *fmt, char *ap) {
    enum {
        FORMAT_ZERO_PAD_WIDTH = 0
    };
    char buf[32];
    char *pad;
    char *p;
    s32 mod;
    s32 zero_pad_width;
    unsigned long hex_digit;
    u8 ch;
    char second;
    s64 v;
    s32 ie;

    ie = DIntr();
    for (; *fmt != 0; fmt++) {
        p = (char *)fmt;
        pad = 0;
        mod = 0;
        if (*fmt == '%') {
        again:
            fmt++;
            switch ((char)(*fmt - '0')) {
            case FORMAT_ZERO_PAD_WIDTH:
                zero_pad_width = fmt[1] - '0';
                second = fmt[2];
                if ((u8)zero_pad_width < 10) {
                    if ((u32)(second - '0') < 10) {
                        zero_pad_width = zero_pad_width * 10 + (second - '0');
                        fmt += 2;
                        if (zero_pad_width >= 32) {
                            zero_pad_width = 31;
                        }
                    } else {
                        fmt += 1;
                    }
                    pad = &buf[31 - zero_pad_width];
                    while (zero_pad_width > 0) {
                        buf[31 - zero_pad_width] = '0';
                        zero_pad_width--;
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
                        hex_digit = v & 0xF;
                        if (hex_digit < 10) {
                            *--p = hex_digit + '0';
                        } else {
                            *--p = hex_digit + 'a' - 10;
                        }
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
                f32 float_argument = ARG(f32);
                if (float_argument == 0.0f) {
                    D_0012FC00[0]('0');
                } else {
                    printfloat(float_argument);
                }
            } break;
            case 's' - '0': {
                char *string = ARG(char *);
                if (*string == 0) {
                    D_0012FC00[0]('(');
                    D_0012FC00[0]('n');
                    D_0012FC00[0]('u');
                    D_0012FC00[0]('l');
                    D_0012FC00[0]('l');
                    D_0012FC00[0](')');
                } else {
                    p = string;
                    if (*p != 0) {
                        do {
                            D_0012FC00[0](*p++);
                        } while (*p != 0);
                    }
                }
                break;
            }
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

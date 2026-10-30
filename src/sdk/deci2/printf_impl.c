#include "types.h"

extern s32  DIntr(void);
extern s32  EnableInterrupts(void);
extern void printfloat(double);
extern void (*D_0012FC00[])(s32);

#define ARG(T) (ap += 8, *(T *)(ap - 8))

void _printf(const char *fmt, char *ap) {
    enum {
        FORMAT_ZERO_PAD_WIDTH = 0
    };
    char          integer_text_buffer[32];
    char         *pad;
    char         *text_cursor;
    s32           mod;
    s32           zero_pad_width;
    unsigned long hex_digit;
    u8            ch;
    char          width_ones_character;
    s64           integer_value;
    s32           ie;

    ie = DIntr();
    for (; *fmt != 0; fmt++) {
        text_cursor = (char *)fmt;
        pad = 0;
        mod = 0;
        if (*fmt == '%') {
        again:
            fmt++;
            switch ((char)(*fmt - '0')) {
            case FORMAT_ZERO_PAD_WIDTH:
                zero_pad_width = fmt[1] - '0';
                width_ones_character = fmt[2];
                if ((u8)zero_pad_width < 10) {
                    if ((u32)(width_ones_character - '0') < 10) {
                        zero_pad_width = zero_pad_width * 10 + (width_ones_character - '0');
                        fmt += 2;
                        if (zero_pad_width >= 32) {
                            zero_pad_width = 31;
                        }
                    } else {
                        fmt += 1;
                    }
                    pad = &integer_text_buffer[31 - zero_pad_width];
                    while (zero_pad_width > 0) {
                        integer_text_buffer[31 - zero_pad_width] = '0';
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
                    integer_value = ARG(s64);
                } else if (mod == 'h') {
                    integer_value = ARG(u16);
                } else {
                    integer_value = ARG(u32);
                }
                text_cursor = &integer_text_buffer[31];
                *text_cursor = 0;
                if (integer_value == 0) {
                    *--text_cursor = '0';
                } else {
                    do {
                        *--text_cursor = (integer_value & 7) + '0';
                        integer_value = (u64)integer_value >> 3;
                    } while (integer_value != 0);
                }
                if (pad != 0 && pad < text_cursor) {
                    text_cursor = pad;
                }
                while (*text_cursor != 0) {
                    D_0012FC00[0](*text_cursor++);
                }
                break;
            case 'x' - '0':
                if (mod == 'l') {
                    integer_value = ARG(s64);
                } else if (mod == 'h') {
                    integer_value = ARG(u16);
                } else {
                    integer_value = ARG(u32);
                }
                text_cursor = &integer_text_buffer[31];
                *text_cursor = 0;
                if (integer_value == 0) {
                    *--text_cursor = '0';
                } else {
                    do {
                        hex_digit = integer_value & 0xF;
                        if (hex_digit < 10) {
                            *--text_cursor = hex_digit + '0';
                        } else {
                            *--text_cursor = hex_digit + 'a' - 10;
                        }
                        integer_value = (u64)integer_value >> 4;
                    } while (integer_value != 0);
                }
                if (pad != 0 && pad < text_cursor) {
                    text_cursor = pad;
                }
                while (*text_cursor != 0) {
                    D_0012FC00[0](*text_cursor++);
                }
                break;
            case 'd' - '0':
                if (mod == 'l') {
                    integer_value = ARG(s64);
                } else if (mod == 'h') {
                    integer_value = ARG(s16);
                } else {
                    integer_value = ARG(s32);
                }
                text_cursor = &integer_text_buffer[31];
                *text_cursor = 0;
                if (integer_value == 0) {
                    *--text_cursor = '0';
                } else {
                    if (integer_value < 0) {
                        integer_value = -integer_value;
                        D_0012FC00[0]('-');
                    }
                    while (integer_value != 0) {
                        *--text_cursor = integer_value % 10 + '0';
                        integer_value = integer_value / 10;
                    }
                }
                if (pad != 0 && pad < text_cursor) {
                    text_cursor = pad;
                }
                while (*text_cursor != 0) {
                    D_0012FC00[0](*text_cursor++);
                }
                break;
            case 'u' - '0':
                if (mod == 'l') {
                    integer_value = ARG(s64);
                } else if (mod == 'h') {
                    integer_value = ARG(u16);
                } else {
                    integer_value = ARG(u32);
                }
                text_cursor = &integer_text_buffer[31];
                *text_cursor = 0;
                if (integer_value == 0) {
                    *--text_cursor = '0';
                } else {
                    do {
                        *--text_cursor = (u64)integer_value % 10 + '0';
                        integer_value = (u64)integer_value / 10;
                    } while (integer_value != 0);
                }
                if (pad != 0 && pad < text_cursor) {
                    text_cursor = pad;
                }
                while (*text_cursor != 0) {
                    D_0012FC00[0](*text_cursor++);
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
                    text_cursor = string;
                    if (*text_cursor != 0) {
                        do {
                            D_0012FC00[0](*text_cursor++);
                        } while (*text_cursor != 0);
                    }
                }
                break;
            }
            case 'c' - '0':
                integer_value = ARG(char);
                D_0012FC00[0](integer_value);
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

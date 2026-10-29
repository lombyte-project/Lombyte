#include "sda.h"

typedef char *va_list;

extern int              _printf(const char *format, va_list arguments);
extern void             deci2Putchar(int character);
extern void *D_0012FC00 NOT_SDA;

int scePrintf(const char *format, ...) {
    va_list arguments = __builtin_next_arg(format) - 0x38;
    void   *previous_putchar = D_0012FC00;
    int     formatter_return_value;

    D_0012FC00 = deci2Putchar;
    formatter_return_value = _printf(format, arguments);
    D_0012FC00 = previous_putchar;
    return formatter_return_value;
}

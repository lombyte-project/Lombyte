#include "rnc/sdk/library/stdio.h"

typedef char *va_list;

extern int vfprintf(RncFile *file, const char *format, va_list arguments) __asm__("func_00116DA8");

int Printf(const char *format, ...) __asm__("printf");

int Printf(const char *format, ...) {
    va_list   arguments;
    RncReent *reent = _impure_ptr;

    reent->_stdout->_data = reent;
    arguments = __builtin_next_arg(format) - 0x38;
    return vfprintf(reent->_stdout, format, arguments);
}

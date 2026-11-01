#include "rnc/sdk/libc/stdio/sprintf.h"
#include "rnc/sdk/mpeg/errors/_Error.h"

void ErrorWithMessage(void *error_state, const char *format) __asm__("_Error1");

void ErrorWithMessage(void *error_state, const char *format) {
    char message[0x100];
    sprintf(message, format);
    _Error(error_state, message);
}

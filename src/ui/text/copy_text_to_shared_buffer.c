#include "types.h"
struct Str19 {
    char c[19];
};
extern struct Str19 message_too_long_label __asm__("D_001E7AA8");
extern char message_text_buffer[] __asm__("D_0019A440");
extern u32 strlen(const char *);
extern char *strcpy(char *, const char *);
void copy_text_to_shared_buffer(char *s) __asm__("FUN_001ff658");

void copy_text_to_shared_buffer(char *s) {
    if (strlen(s) < 0x50) {
        *(struct Str19 *)message_text_buffer = message_too_long_label;
    }
    strcpy(message_text_buffer, s);
}

extern __typeof__(copy_text_to_shared_buffer) func_001FF658 __attribute__((alias("FUN_001ff658")));

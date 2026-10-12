#include "types.h"

extern volatile s32 movie_display_active __asm__("D_001611E0");

void end_display(void) {
    movie_display_active = 0;
}

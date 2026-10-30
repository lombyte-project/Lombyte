#include "types.h"

extern s32 snd_send_iop_command_and_wait() __asm__("FUN_0012e548");
void       snd_get_movie_nax(void) __asm__("FUN_0012f178");

void snd_get_movie_nax(void) {
    snd_send_iop_command_and_wait(0x5B, 0, 0);
}

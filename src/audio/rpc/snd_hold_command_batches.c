typedef int s32;

extern s32 snd_batches_held __asm__("D_0015ECC4");

void snd_hold_command_batches(void) __asm__("ReadGlobalTableEntry");

void snd_hold_command_batches(void) {
    snd_batches_held = 1;
}

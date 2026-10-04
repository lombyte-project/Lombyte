/* Ported from rac1-decomp, the PAL decompilation (src/game/sound.c, func_0022F090). */
/* 8 bytes of post-endlabel nop padding in retail -- see func_001F6668. */
void store_sound_bank_handle_callback(int handle, long context) __asm__("FUN_0022dd78");

void store_sound_bank_handle_callback(int handle, long context) {
    int *handle_output = (int *)(int)context;
    if (handle_output != 0) {
        *handle_output = handle;
    }
}

extern __typeof__(store_sound_bank_handle_callback) func_0022DD78 __attribute__((alias("FUN_0022dd78")));

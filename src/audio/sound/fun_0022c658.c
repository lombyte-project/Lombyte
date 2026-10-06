/* Ported from rac1-decomp (src/game/sound.c, func_0022D970). */
extern void FUN_001f9a68(void *, void *, float);
extern void FUN_001f9a10(void *, void *, void *);
extern int FUN_001efa68(void *, void *, int, int, int);
extern char D_00187080[];
extern void FUN_001f9a28(void *, void *, void *);
extern void FUN_001f9c90(void *, void *, float);
/* Aim the vector from the frame at D_00187080 toward arg0+0x20, scaled to
   3/4 and 64, and return the line test from arg1 to its end. The int
   return keeps retail's argument order for FUN_001f9a10. */
int test_voice_occlusion(void *voice, void *listener_position) __asm__("FUN_0022c658");

int test_voice_occlusion(void *voice, void *listener_position) {
    float test_position[4];

    FUN_001f9a28(test_position, (char *)voice + 0x20, D_00187080);
    FUN_001f9a68(test_position, test_position, 0.75f);
    FUN_001f9c90(test_position, test_position, 64.0f);
    FUN_001f9a10(test_position, test_position, D_00187080);
    return FUN_001efa68(listener_position, test_position, 0x82, *(int *)((char *)voice + 0x18), 0);
}

extern __typeof__(test_voice_occlusion) func_0022C658 __attribute__((alias("FUN_0022c658")));

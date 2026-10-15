/* Ported from rac1-decomp (src/game/sound.c, func_0022D970). */
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern int FUN_001efa68(void *, void *, int, int, int);
extern char D_00187080[];
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
/* Aim the vector from the frame at D_00187080 toward arg0+0x20, scaled to
   3/4 and 64, and return the line test from arg1 to its end. The int
   return keeps retail's argument order for add_vector_xyz. */
int test_voice_occlusion(void *voice, void *listener_position) __asm__("FUN_0022c658");

int test_voice_occlusion(void *voice, void *listener_position) {
    float test_position[4];

    subtract_vector_xyz(test_position, (char *)voice + 0x20, D_00187080);
    fast_vec_scale(test_position, test_position, 0.75f);
    clamp_vector_length(test_position, test_position, 64.0f);
    add_vector_xyz(test_position, test_position, D_00187080);
    return FUN_001efa68(listener_position, test_position, 0x82, *(int *)((char *)voice + 0x18), 0);
}

extern __typeof__(test_voice_occlusion) func_0022C658 __attribute__((alias("FUN_0022c658")));

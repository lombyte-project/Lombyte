#include "rnc/math/fast_math.h"
#include "rnc/camera/camera_globals.h"

/* Ported from rac1-decomp (src/game/sound.c, func_0022D970). */
extern int  collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
/* Aim the vector from the frame at ((char *)camera_position) toward arg0+0x20, scaled to
   3/4 and 64, and return the line test from arg1 to its end. The int
   return keeps retail's argument order for fast_vec_add. */
int test_voice_occlusion(void *voice, void *listener_position) __asm__("FUN_0022c658");

int test_voice_occlusion(void *voice, void *listener_position) {
    float test_position[4];

    fast_vec_sub(test_position, (char *)voice + 0x20, ((char *)camera_position));
    fast_vec_scale(test_position, test_position, 0.75f);
    clamp_vector_length(test_position, test_position, 64.0f);
    fast_vec_add(test_position, test_position, ((char *)camera_position));
    return collision_line(
        listener_position, test_position, 0x82, *(int *)((char *)voice + 0x18), 0);
}

extern __typeof__(test_voice_occlusion) func_0022C658 __attribute__((alias("FUN_0022c658")));

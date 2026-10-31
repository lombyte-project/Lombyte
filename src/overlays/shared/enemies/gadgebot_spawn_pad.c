/* Shared moby class gadgebot_spawn_pad: the glowing pad under a spawning gadgebot, and the function that spawns it. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern float fast_sin(float);

extern int level_frame_count __asm__("D_L06_0015F5CC");
extern s32 level_frame_count __asm__("D_L06_0015F5CC");

/* Spawns a moby of type 0x12F at the owner's position and links it to the owner. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002D9F68), where it is exact; names translated to the US level program. */
char *spawn_gadgebot_pad(char *owner) __asm__("FUN_L06_002d8b98");

char *spawn_gadgebot_pad(char *gadgebot) {
    char *moby = create_moby(0x12F);
    if (moby != 0) {
        MOBY(moby)->draw_dist = 0x60;
        moby[0x31] = 1;
        ((unsigned char *)moby)[0x30] = 0;
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        MOBY(moby)->rot.z = *(float *)(gadgebot + 0x48);
        MOBY(moby)->spawn_frame = *(long *)(gadgebot + 0x38);
        MOBY(moby)->parent = gadgebot;
        qcopy(moby + 0x10, gadgebot + 0x10);
        update_entity(moby);
    }
    return moby;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002D9FF0), where it is exact; names translated to the US level program. */
void gadgebot_spawn_pad_update(char *moby) __asm__("FUN_L06_002d8c20");

/* Moby class 303 */
void gadgebot_spawn_pad_update(char *moby) {
    char *owner_moby = ((char *)MOBY(moby)->parent);
    if (owner_moby != 0 && (unsigned char)owner_moby[0x20] < 2) {
        int   frame_count_snapshot = *(int *)&level_frame_count;
        float color_pulse_value = (float)(frame_count_snapshot % 150) / 150.0f * 2.0f * 3.14159274f;
        if (color_pulse_value > 3.14159274f)
            color_pulse_value -= 6.28318548f;
        color_pulse_value = (fast_sin(color_pulse_value) + 1.0f) * 0.5f;
        MOBY(moby)->glow_color = fast_tween_color(0x80464646, 0x80828282, color_pulse_value);
    } else {
        MOBY(moby)->glow_color = fast_tween_color(MOBY(moby)->glow_color, 0x80464646, 0.1f);
    }
}

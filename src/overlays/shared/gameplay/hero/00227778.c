/* FUN_L00_00227778 split out of 00221310.c so it builds with the SN
   compiler on its own. */
#include "types.h"
#include "qcopy.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math/vector.h"
extern unsigned char D_0013D4C0[];
extern unsigned int D_0013CAE0 __attribute__((section(".data")));
extern int D_L00_00167014[];
extern f32 frame_time __asm__("D_0015ED6C") __attribute__((sda));
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void queue_animation_update(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern void FUN_L00_00236268(void);
extern void FUN_L00_00236458(void);
extern void FUN_L00_002365a8(void);
extern int ray_hits(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
/* The same function under a void-returning prototype, as an older
   declaration of it would have read; see the 0x33 call below. */
extern void hero_set_state_v(int, int) __asm__("FUN_L00_002223f8");

int FUN_L00_00227778(void) {
    Vec4 start;
    Vec4 end;
    int decrement;
    int state;

    if (hero.state.control_mode == 0x11) {
        if (D_0013D4C0[6] != 0) {
            decrement = 0;
        } else {
            decrement = 10000 / (scale_game_frames(0x3C) * 15);
            queue_animation_update(4, 0x753F, FUN_L00_00236268,
                FUN_L00_00236458, FUN_L00_002365a8, &hero.unk22A0, 10000);
        }
        hero.unk22A0 -= decrement;
        if (hero.unk22A0 < 0)
            hero.unk22A0 = 0;
        if (hero.unk22A0 == 0 &&
            (!(hero.water_surface_z - 2.0f < hero.motion.pos.f[2]) ||
             !(hero.motion.rot.f[1] < -0.87266463f) ||
             !(frame_time < hero.motion.displacement.f[2]))) {
            state = 0x6A;
        } else if (hero.water_surface_z + 0.4f < hero.motion.pos.f[2]) {
            D_L00_00167014[0] = 0;
            state = 6;
        } else {
            goto collision_mode;
        }
change_state:
        hero_set_state(state, 1);
        return 1;
    } else if (hero.state.current != 0x6A && hero.state.current != 0x76) {
        hero.unk22A0 = 10000;
    }
collision_mode:
    if (hero.state.control_mode != 0x12)
        return 0;
    qcopy(&start, &hero.motion.pos);
    start.f[2] = hero.water_surface_z + 0.01f;
    qcopy(&end, &start);
    end.f[2] += 0.3f;
    if (ray_hits(&start, &end, 2, hero.moby, 0)) {
        if (D_0013D4C0[4] != 0 && (D_0013CAE0 & 10) != 0) {
            state = 0x35;
            goto change_state;
        }
        /* Retail keeps this as its own call site. Through the int-returning
           prototype, jump2 cross-jumping merges it into the shared
           change_state call above; the void-returning declaration keeps it
           separate. That the original source declared it this way is a
           hypothesis. */
        hero_set_state_v(0x33, 1);
        return 1;
    }
    return 0;
}

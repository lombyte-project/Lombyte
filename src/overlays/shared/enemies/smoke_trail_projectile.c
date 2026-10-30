/* Smoke trail projectile (class 0x237): burning debris that trails smoke, sticks to the ground and hurts what it hits. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "qcopy.h"

typedef struct {
    float v[2];
    float f8;
    float fC;
    char  pad10[8];
    char  b18;
    char  b19;
    short s1A;
    char  pad1C[0x14];
} Hit_2eedd8;

extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");

extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern char D_L01_00174340_c[] __asm__("D_L01_001742C0");

/* Burning debris: flies along its velocity leaving two smoke trails, sticks to the ground, and hurts what it hits. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002EEDD8), where it is exact; names translated to the US level program. */

void update_smoke_trail_projectile(char *moby) __asm__("FUN_L01_002eda00");

void update_smoke_trail_projectile(char *moby) {
    enum {
        MOVE_UNTIL_COLLISION_OR_TIMEOUT = 1,
        BURST_AND_REMOVE = 2
    };
    char      *moby_vars = ((char *)MOBY(moby)->pvars);
    float      previous_position[4];
    float      z[4];
    Hit_2eedd8 hit_descriptor;
    float      gz;
    switch (((unsigned char *)moby)[0x20]) {
    case MOVE_UNTIL_COLLISION_OR_TIMEOUT:
        qcopy(previous_position, moby + 0x10);
        fast_vec_add(moby + 0x10, moby + 0x10, moby_vars);
        MOBY(moby)->rot.z = fast_arc_tan(*(float *)(moby_vars + 0), *(float *)(moby_vars + 4));
        clear_u64_value(z);
        spawn_effect(moby + 0x10,
                     z,
                     0x6F00AFFF,
                     0xFF,
                     scale_game_frames(random_int_between(0xF, 0x16)),
                     0x28,
                     random_int_between(0x14, 0x23),
                     1);
        spawn_effect(moby + 0x10,
                     z,
                     0x1FFFFFFF,
                     0x4F4F4F,
                     scale_game_frames(random_int_between(0x1E, 0x3C)),
                     0x28,
                     random_int_between(0x32, 0x4B),
                     0);
        gz = MOBY(moby)->pos.z - probe_ground_height(moby + 0x10, 0, 0.5f);
        if (gz < *(float *)(moby_vars + 0x18)) {
            MOBY(moby)->pos.z = MOBY(moby)->pos.z + (*(float *)(moby_vars + 0x18) - gz);
            *(int *)(moby_vars + 8) = 0;
        } else if (MOBY(moby)->pos.z < hero.ground_point.f[2] + *(float *)(moby_vars + 0x18)) {
            *(int *)(moby_vars + 8) = 0;
        } else {
            *(float *)(moby_vars + 8) -= frame_time_sq * 10.8f;
        }
        if (collision_line(moby + 0x10, previous_position, 0, *(int *)(moby_vars + 0x10), 0) != 0) {
            char *collision_result = D_L01_00174340_c;
            if (*(int *)(collision_result + 0x18) != 0) {
                make_hit_descriptor(&hit_descriptor, moby, 0x10001, 1.0f, moby_vars);
                set_vector_length_xy(&hit_descriptor, &hit_descriptor, 1.0f);
                hit_descriptor.f8 = 1.0f;
                hit_descriptor.fC = 5627.9248f;
                hit_descriptor.b19 = 1;
                hit_descriptor.s1A = ((unsigned short)MOBY(moby)->oclass);
                hit_descriptor.b18 = 0;
                record_moby_hit(*(void **)(collision_result + 0x18), &hit_descriptor);
            }
            moby[0x20] = 2;
        }
        if (fast_dec_timer(moby_vars + 0x14) != 0)
            moby[0x20] = 2;
        break;
    case BURST_AND_REMOVE:
        cart_burst(moby, moby + 0x10, -1, 0.25f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}

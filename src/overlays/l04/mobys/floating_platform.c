/* Level 04 moby class floating_platform: a platform with a spinning ring. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"

extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");

/* Floating platform: bobs on a slow wave above its start height (or above the moby it rides), and spins
 * its three rings. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002CF180), where it is exact; names translated to the US level program. */

extern int   D_L04_0015FFD8; /* no foreign declaration */
extern short floating_platform_ring_rotation_rate_degrees __asm__("D_L04_0016195C")
    __attribute__((sda));

void floating_platform_update(char *moby) __asm__("FUN_L04_002cdda0");

void floating_platform_update(char *moby) {
    enum {
        FLOATING_PLATFORM_INITIALIZE = 0,
        FLOATING_PLATFORM_UPDATE_MOTION = 1
    };
    float old[4];
    float delta[4];
    char *data = ((char *)MOBY(moby)->pvars);
    qcopy(old, moby + 0x10);
    if (data == 0) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (((unsigned char *)moby)[0x20]) {
    case FLOATING_PLATFORM_INITIALIZE: {
        int i;
        MOBY(moby)->anim_speed = random_float_between(0.5f, 0.8f);
        moby[0x20] = 1;
        *(float *)(data + 0x220) = MOBY(moby)->pos.z + 15.0f;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(data + 0x3E) = 0xD;
        for (i = 0; i < 3; i++) {
            ((float *)(data + 0x224))[i] = random_angle_radians();
        }
        break;
    }
    case FLOATING_PLATFORM_UPDATE_MOTION: {
        float height_offset = 0.0f;
        float bob = fast_cos(fast_normalize_angle(compute_interpolated_record_value(moby) *
                                                  6.2831855f / 118.0f)) *
                    0.75f;
        int i;
        for (i = 0; i < 3; i++) {
            float *ring_angle_ptr = (float *)(data + 0x224) + i;
            float  ring_angle = fast_add_rotations(
                *ring_angle_ptr,
                *(float *)&floating_platform_ring_rotation_rate_degrees * DEG_TO_RAD * frame_time);
            float joint_animation_frame_scale = frame_scale_sq;
            *ring_angle_ptr = ring_angle;
            *(float *)(data + 0x108 + i * 0x80) = ring_angle;
            update_joint_animation(moby,
                                   data + 0xA0 + i * 0x80,
                                   i,
                                   joint_animation_frame_scale * 0.03f,
                                   joint_animation_frame_scale * 0.3f);
        }
        if (*(int *)(data + 0x230) != -1) {
            char *linked_moby = D_L04_0015FFD8 + (*(int *)(data + 0x230) << 8);
            if (MOBY(linked_moby)->oclass == 0x118) {
                height_offset = *((float *)MOBY(linked_moby)->pvars) * 15.0f;
            }
        } else {
            height_offset = 15.0f;
        }
        MOBY(moby)->pos.z = *(float *)(data + 0x220) + bob - height_offset;
        break;
    }
    }
    fast_vec_sub(delta, moby + 0x10, old);
    update_entity_matrix(data + 0x60, delta, moby + 0x40, moby + 0x40);
}

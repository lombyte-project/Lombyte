/* Shared moby class blarg_gun. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"

extern char D_L05_00174240[];
extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_00319C08), where it is exact; names translated to the US level program. */

void blarg_gun_update(char *moby) __asm__("FUN_L05_003186f8");

void blarg_gun_update(char *moby) {
    enum {
        BLARG_GUN_STATE_INITIALIZE = 0,
        BLARG_GUN_STATE_MOVING = 1
    };
    char *data = ((char *)MOBY(moby)->pvars);
    char  rotation_workspace[0x70];
    char *hs;
    int   joint_index;
    float fade_decrement;

    switch ((unsigned char)moby[0x20]) {
    case BLARG_GUN_STATE_INITIALIZE:
        if (((unsigned short)MOBY(moby)->draw_dist) & 1)
            MOBY(moby)->flags |= 0x8000;
        moby[0x20] = 2;
        break;
    case BLARG_GUN_STATE_MOVING:
        joint_index = *(int *)(data + 4);
        if (joint_index > 0) {
            moby_point_position(moby, joint_index, rotation_workspace + 0x40);
            MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, *(float *)(data + 0x20));
            MOBY(moby)->rot.y = fast_add_rotations(MOBY(moby)->rot.y, *(float *)(data + 0x24));
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, *(float *)(data + 0x28));
            euler_rotation_matrix(rotation_workspace, moby + 0x40);
            copy_matrix3x4(moby + 0xC0, rotation_workspace);
            moby_point_position(moby, *(int *)(data + 4), rotation_workspace + 0x50);
            fast_vec_sub(
                rotation_workspace + 0x60, rotation_workspace + 0x40, rotation_workspace + 0x50);
        } else {
            transform_vector_by_basis(rotation_workspace + 0x40, data + 0x10, moby + 0xC0);
            MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, *(float *)(data + 0x20));
            MOBY(moby)->rot.y = fast_add_rotations(MOBY(moby)->rot.y, *(float *)(data + 0x24));
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, *(float *)(data + 0x28));
            euler_rotation_matrix(rotation_workspace, moby + 0x40);
            if (MOBY(moby)->flags & 0x8000)
                fast_vec_scale(rotation_workspace + 0x10, rotation_workspace + 0x10, -1.0f);
            copy_matrix3x4(moby + 0xC0, rotation_workspace);
            transform_vector_by_basis(rotation_workspace + 0x50, data + 0x10, moby + 0xC0);
            fast_vec_sub(
                rotation_workspace + 0x60, rotation_workspace + 0x40, rotation_workspace + 0x50);
            fast_vec_add(rotation_workspace + 0x50, rotation_workspace + 0x50, moby + 0x10);
        }

        if (coll_sphere(convert_integer_to_float(*(int *)(data + 0xC)) * 0.0009765625f,
                        rotation_workspace + 0x50,
                        0,
                        moby)) {
            hs = D_L05_00174240;
            if (*(int *)(hs + 0x18) == 0 || *(int *)(hs + 0x18) != *(int *)(data + 8)) {
                *(int *)data = 0;
                if (fast_vec_dot(data + 0x30, hs + 0x40) < 0.0f) {
                    reflect_vector(data + 0x30, data + 0x30, hs + 0x40);
                    fast_vec_scale(data + 0x30, data + 0x30, 0.5f);
                }
            }
        }
        if (fast_dec_timer((int *)data)) {
            fade_decrement = 128.0f / (float)scale_game_frames(20);
            *(int *)data = 0;
            if (fade_decrement <= (float)((unsigned char *)moby)[0x23]) {
                ((unsigned char *)moby)[0x23] = ((unsigned char *)moby)[0x23] - (int)fade_decrement;
            } else {
                mark_moby_for_removal(moby);
            }
        }
        *(float *)(data + 0x38) = *(float *)(data + 0x38) - frame_time_sq * 10.8f;
        fast_vec_add(moby + 0x10, moby + 0x10, data + 0x30);
        fast_vec_add(moby + 0x10, moby + 0x10, rotation_workspace + 0x60);
        if (MOBY(moby)->pos.z < 5.0f)
            mark_moby_for_removal(moby);
        break;
    }
}

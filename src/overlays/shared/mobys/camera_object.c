/* Shared moby class camera_object: the moby the camera collides with. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/collision.h"

typedef struct {
    char  p0[0x98];
    float f98;
    char  p1[0x164 - 0x9C];
    float f164;
    char  p2[0x2080 - 0x168];
    char *obj;
    char  p3[0x208C - 0x2084];
    int   w208C;
} G_2a4e48;

typedef struct {
    char  p0[0x158];
    float f158;
    char  p1[0x180 - 0x15C];
    char *c;
} C_2a4e48;

extern LevelCamera l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;

/* Creates a class 0x3EF moby at v and runs update_entity on it. */

unsigned char *create_camera_collision_moby(u128 *spawn_position) __asm__("FUN_L00_002a4de0");

unsigned char *create_camera_collision_moby(u128 *spawn_position) {
    unsigned char *spawned_moby = create_moby(0x3ef);
    if (spawned_moby) {
        spawned_moby[0x30] = 0xff;
        MOBY(spawned_moby)->flags |= 0x41;
        qcopy(spawned_moby + 0x10, spawn_position);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

void camera_object_update(char *o) __asm__("FUN_L00_002a4e48");

/* Moby class 1007 */
void camera_object_update(char *camera_object) {
    OvlQuad      difference_vector[1];
    OvlQuad      scaled_vector[1];
    LevelCamera *C = &l00_camera_lui;
    char        *object_position;
    char        *pos;
    G_2a4e48    *P;
    float        dot_product_or_threshold;

    if (C->unk180 != 0) {
        object_position = camera_object + 0x10;
        qcopy(object_position, (char *)C->unk180 + 0x30);
        pos = (char *)&hero.motion.pos;
        fast_vec_sub(difference_vector, object_position, pos);
        P = (G_2a4e48 *)(pos - 0x80);
        dot_product_or_threshold = fast_vec_dot(difference_vector, P->obj + 0xE0);
        fast_vec_scale(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
        fast_vec_sub(difference_vector, difference_vector, scaled_vector);
        if (absolute_float(dot_product_or_threshold) < 2.5f) {
            dot_product_or_threshold = 0.1f;
            if (fast_vec_length(difference_vector) < dot_product_or_threshold ||
                fast_difference_between_rotations(P->f98, C->yaw) < 2.3561945f ||
                P->f164 < frame_time * dot_product_or_threshold) {
                *(float *)(camera_object + 0x18) = 15.0f;
                *(float *)(camera_object + 0x14) = 15.0f;
                *(float *)(camera_object + 0x10) = 15.0f;
            } else {
                dot_product_or_threshold = fast_vec_dot(object_position, P->obj + 0xE0);
                fast_vec_scale(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
                fast_vec_sub(difference_vector, object_position, scaled_vector);
                dot_product_or_threshold = fast_vec_dot(pos, P->obj + 0xE0);
                if (P->w208C != 0x11) {
                    fast_vec_scale(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
                    fast_vec_add(object_position, difference_vector, scaled_vector);
                }
            }
        }
    } else {
        *(float *)(camera_object + 0x18) = 5.0f;
        *(float *)(camera_object + 0x14) = 5.0f;
        *(float *)(camera_object + 0x10) = 5.0f;
    }
}

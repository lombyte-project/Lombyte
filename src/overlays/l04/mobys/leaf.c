/* Level 04 moby class leaf. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

extern char  D_L04_00166F40[];
extern float scale_time(float) __asm__("FUN_001f96b0");

extern int   collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void  reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern float leaf_motion_parameters[6] __asm__("D_L04_001CB940");
extern char  D_L04_00173FE0[];

/* Three-state motion update. The saved position is restored when the timer expires. */
void leaf_update(struct Moby *moby) __asm__("FUN_L04_002d7e90");

void leaf_update(struct Moby *moby) {
    enum {
        LEAF_INITIALIZE = 0,
        LEAF_WAIT_FOR_ACTIVATION = 1,
        LEAF_UPDATE_MOTION = 2
    };
    Vec4   random;
    Vec4   old;
    Vec4   work;
    Vec4   initialDirection;
    Vec4   direction;
    char  *data = (char *)moby->pvars;
    float  speed;
    float  amount;
    int    timer;
    float *motion;

    old.q = 0;
    old.f[0] = random_float_between(-1.0f, 1.0f);
    old.f[1] = random_float_between(-1.0f, 1.0f);
    old.f[2] = random_float_between(-1.0f, 1.0f);
    random.q = old.q;
    speed = vector_length_xy(data);
    qcopy(&old, &moby->pos);
    switch (moby->state) {
    case LEAF_INITIALIZE:
        qcopy(data + 0x30, &moby->pos);
        *(int *)(data + 0x40) = *(int *)(data + 0x44) = random_int_between(120, 2400);
        *(float *)(data + 0x4c) = moby->scale * random_float_between(0.8f, 1.2f);
        moby->state = 1;
        break;
    case LEAF_WAIT_FOR_ACTIVATION:
        if (!(vector_distance_xy(&moby->pos, D_L04_00166F40) < 32.0f))
            return;
        if (!fast_dec_timer((int *)(data + 0x44)))
            return;
        direction.q = 0;
        direction.f[0] = random_float_between(-1.0f, 1.0f);
        direction.f[1] = random_float_between(-1.0f, 1.0f);
        direction.f[2] = random_float_between(-1.0f, 1.0f);
        moby->update_dist = 0xff;
        moby->draw_dist = 0x40;
        moby->scale = 0.0f;
        motion = (float *)moby->pvars;
        initialDirection.q = direction.q;
        fast_vec_normalize(&initialDirection, &initialDirection, leaf_motion_parameters[5] / 5.0f);
        motion[0] = leaf_motion_parameters[5] * fast_cos(leaf_motion_parameters[1]);
        motion[1] = leaf_motion_parameters[5] * fast_sin(leaf_motion_parameters[1]);
        motion[2] = frame_time * -0.5f;
        fast_vec_normalize(&moby->rot, &initialDirection, 3.1415927f);
        fast_vec_normalize(motion + 8, &initialDirection, 0.098174773f);
        fast_vec_normalize(motion + 4, &initialDirection, leaf_motion_parameters[5] * 0.5f);
        *(int *)(motion + 16) = *(int *)(motion + 17) = random_int_between(720, 1200);
        moby->state = 2;
        return;
    case LEAF_UPDATE_MOTION:
        fast_dec_timer((int *)(data + 0x44));
        fast_vec_normalize(&random, &random, leaf_motion_parameters[5] / 10.0f);
        fast_vec_add(data + 0x10, data + 0x10, &random);
        fast_vec_normalize(&work, data + 0x10, 5.0f);
        fast_vec_normalize(data + 0x10, data + 0x10, leaf_motion_parameters[5] * 0.5f);
        fast_vec_add(&work, &work, &moby->pos);
        if (collision_line(&old, &work, 6, 0, 0) && !random_integer_below(8)) {
            reflect_vector(data + 0x10, data + 0x10, D_L04_00173FE0 + 0x20);
            *(float *)(data + 0x18) += leaf_motion_parameters[5] / 10.0f;
        }
        fast_vec_add(data, data, data + 0x10);
        work.f[0] = leaf_motion_parameters[5] * fast_cos(leaf_motion_parameters[1]);
        work.f[1] = leaf_motion_parameters[5] * fast_sin(leaf_motion_parameters[1]);
        work.f[2] = frame_time * -0.5f;
        fast_vec_scale(&work, &work, 0.4f);
        fast_vec_scale(data, data, 0.6f);
        fast_vec_add(data, &work, data);
        fast_vec_add(&moby->pos, &moby->pos, data);
        if (collision_line(&old, &moby->pos, 6, 0, 0)) {
            qcopy(&moby->pos, D_L04_00173FE0);
            reflect_vector(data, data, D_L04_00173FE0 + 0x20);
            fast_vec_scale(data, data, 0.25f);
            fast_vec_add(&moby->pos, &moby->pos, data);
            *(float *)(data + 8) += leaf_motion_parameters[5] / 5.0f;
        }
        if (leaf_motion_parameters[5] < speed)
            fast_vec_normalize(data, data, leaf_motion_parameters[5]);
        {
            float amount = frame_time * -0.5f;
            if (amount < *(float *)(data + 8))
                *(float *)(data + 8) += amount / 10.0f;
            else
                *(float *)(data + 8) = amount;
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, *(float *)(data + 0x20));
        moby->rot.y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x24));
        moby->rot.z = fast_add_rotations(moby->rot.z, *(float *)(data + 0x28));
        if (*(int *)(data + 0x40) - scale_game_frames(10) < *(int *)(data + 0x44)) {
            amount = convert_integer_to_float(*(int *)(data + 0x44) - *(int *)(data + 0x40));
            moby->scale = (*(float *)(data + 0x4c) * amount) / scale_time(10.0f);
        } else {
            if (*(int *)(data + 0x44) < scale_game_frames(100))
                moby->scale *= 0.99f;
            else
                moby->scale = *(float *)(data + 0x4c);
        }
        if (*(int *)(data + 0x44) > 0)
            return;
        qcopy(&moby->pos, data + 0x30);
        timer = random_int_between(120, 2400);
        /* Preserve the retail ordering of the timer reload stores. */
        *(volatile int *)(data + 0x40) = timer;
        *(volatile int *)(data + 0x44) = timer;
        moby->state = 1;
        break;
    default:
        return;
    }
    moby->scale = 0.0f;
}

/* Level 04 moby class snow_plow. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/vehicles/gunship.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l04/mobys/snow_plow_globals.h"

extern int D_L04_0015FFD8;

char snow_plow_missing_path_warning_format[48] __asm__("D_L04_001DBE10") =
    "WARNING moby class %d instance %d has no path!\n";

extern char  D_L04_00166E00[];
extern float D_L04_0015F580[] MACRO_ADDR;

extern int  snow_plow_hit_count_gp __asm__("D_L04_0015EDF8") __attribute__((sda));
extern void debug_print() __asm__("FUN_001e93b0");
extern void FUN_L00_00258278(void *, float, void *, float, float, float);
extern void attach_manipulator(char *, int, void *) __asm__("FUN_0020cb10");
extern void FUN_L00_001fff28(void *, int, float);
extern void advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void update_entity(void *) __asm__("FUN_L00_00250df8");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void
collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);

/* Class-dependent movement, manipulator rotation, hit effects and visibility. */
void snow_plow_update(struct Moby *moby) __asm__("FUN_L04_002ca420");

/* Moby classes 466, 480, 485, 486, 488, 490, 493-495, 498, 555 */
void snow_plow_update(struct Moby *moby) {
    enum {
        SNOW_PLOW_SCAN_CLIP_VOLUMES = 0,
        SNOW_PLOW_MOVE_TO_CLIP_VOLUME = 1,
        SNOW_PLOW_WAIT_BEFORE_RETURN = 2,
        SNOW_PLOW_RETURN_TO_SAVED_TRANSFORM = 3
    };
    enum {
        SNOW_PLOW_CLASS_WITH_QUARTER_TURN_LINKED_MOBY = 0x1e0,
        SNOW_PLOW_CLASS_WITH_ALIGNED_LINKED_MOBY = 0x1ee
    };
    enum {
        SNOW_PLOW_CLASS_OFFSET_WITH_THREE_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ONE = 0,
        SNOW_PLOW_CLASS_OFFSET_WITH_QUARTER_TURN_LINKED_MOBY = 0xe,
        SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPHERE_QUERY_AND_TARGET_VOICE = 0x13,
        SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPHERE_QUERY_WITHOUT_TARGET_VOICE = 0x14,
        SNOW_PLOW_CLASS_OFFSET_WITH_TWO_ROTATING_MANIPULATORS = 0x16,
        SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPAWNS_647_648_649 = 0x18,
        SNOW_PLOW_CLASS_OFFSET_WITH_TWO_POINT_CART_BURST = 0x1b,
        SNOW_PLOW_CLASS_OFFSET_WITH_ALIGNED_LINKED_MOBY = 0x1c,
        SNOW_PLOW_CLASS_OFFSET_WITH_FOUR_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ONE = 0x1d,
        SNOW_PLOW_CLASS_OFFSET_WITH_THREE_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ZERO = 0x20,
        SNOW_PLOW_CLASS_OFFSET_WITH_TARGET_VOICE_SELECTOR_ZERO = 0x59
    };
    Vec4 update_start_position;
    Vec4 delta, *return_delta_ptr;
    /* The sphere query is its position followed by the record; the hit cases pass one or the other. */
    Vec4 sphere_pos;
    struct {
        struct Moby *moby;
        int          flags;
        int          unused;
        float        scale;
        int          zero;
    } sphere;
    struct {
        Vec4         vector;
        struct Moby *moby;
        int          flags;
        int          unused;
        float        scale;
        int          zero;
    } sphere2;
    char         *snow_plow_vars = (char *)moby->pvars;
    unsigned char state = moby->state;
    int           i, *p, *path;
    float         amount, length, step, speed;
    int           timer;
    qcopy(&update_start_position, &moby->pos);
    if (!snow_plow_vars)
        return;
    path = (int *)(snow_plow_vars + 0x60);
    if (!state) {
        if (path[5] != -1) {
            int *p;
            p = ((int *)l04_paths[path[5]]);
            if (fast_vec_dist(p + 4, p + *p * 4) < 0.5f)
                --*p;
        } else {
            char *message = snow_plow_missing_path_warning_format;
            int   oclass = moby->oclass;
            timer = truncate_float_to_s32((float)(((int)moby - D_L04_0015FFD8) >> 8));
            debug_print(message, oclass, timer);
        }
        moby->draw_dist = 0x7f;
        moby->update_dist = 0xff;
        *(float *)(snow_plow_vars + 0x138) = *(float *)(snow_plow_vars + 0xfc);
        *(int *)(snow_plow_vars + 0x178) = moby->draw_dist;
    }
    amount = 0.0f;
    gunship_update(moby);
    if (moby->oclass != 0x1e4) {
        fast_dec_timer_s16(snow_plow_vars + 0x14a);
        switch (*(unsigned char *)(snow_plow_vars + 0x149)) {
        case SNOW_PLOW_SCAN_CLIP_VOLUMES: {
            float length = *(float *)(snow_plow_vars + 0xfc);
            float target = *(float *)(snow_plow_vars + 0x138);
            if (length < target) {
                *(float *)(snow_plow_vars + 0xfc) = length + frame_time_sq * 20.0f;
                if (target < *(float *)(snow_plow_vars + 0xfc))
                    *(float *)(snow_plow_vars + 0xfc) = target;
            }
            p = (int *)(snow_plow_vars + 0x150);
            i = 0;
            do {
                if (*(short *)(snow_plow_vars + 0x14a) == 0 && p[i] != -1 &&
                    is_point_inside_clip_volume(&moby->pos, p[i])) {
                    qcopy(snow_plow_vars + 0x160, &moby->pos);
                    *(unsigned char *)(snow_plow_vars + 0x148) = *(unsigned char *)&p[i];
                    *(unsigned char *)(snow_plow_vars + 0x149) = 1;
                    *(float *)(snow_plow_vars + 0x14c) = moby->rot.z;
                    *(int *)(snow_plow_vars + 0x7c) = 0;
                    moby->state = 3;
                    *(int *)(snow_plow_vars + 0x11c) = scale_game_frames(36000);
                    break;
                }
                ++i;
            } while (i < 4);
            break;
        }
        case SNOW_PLOW_MOVE_TO_CLIP_VOLUME:
            FUN_L00_00258278(moby,
                             l04_cuboids[*(unsigned char *)(snow_plow_vars + 0x148)].rot.z,
                             snow_plow_vars + 0x7c,
                             frame_time_sq * 1.0471976f,
                             frame_time_sq * 3.1415927f,
                             frame_time * 3.1415927f);
            if (vector_distance_xy(&moby->pos,
                                   &l04_cuboids[*(unsigned char *)(snow_plow_vars + 0x148)].pos) <
                    1.0f &&
                fast_difference_between_rotations(
                    moby->rot.z, l04_cuboids[*(unsigned char *)(snow_plow_vars + 0x148)].rot.z) <
                    0.17453292f) {
                *(unsigned char *)(snow_plow_vars + 0x149) = 2;
                *(short *)(snow_plow_vars + 0x14a) = scale_game_frames(300);
            }
            fast_vec_sub(
                &delta, &l04_cuboids[*(unsigned char *)(snow_plow_vars + 0x148)].pos, &moby->pos);
            fast_vec_normalize(&delta, &delta, frame_time * 3.0f);
            fast_vec_add(&moby->pos, &moby->pos, &delta);
            break;
        case SNOW_PLOW_WAIT_BEFORE_RETURN:
            if (*(short *)(snow_plow_vars + 0x14a) == 0) {
                *(float *)(snow_plow_vars + 0x7c) = amount;
                *(unsigned char *)(snow_plow_vars + 0x149) = 3;
            }
            break;
        case SNOW_PLOW_RETURN_TO_SAVED_TRANSFORM:
            FUN_L00_00258278(moby,
                             *(float *)(snow_plow_vars + 0x14c),
                             snow_plow_vars + 0x7c,
                             frame_time_sq * 1.0471976f,
                             frame_time_sq * 3.1415927f,
                             frame_time * 3.1415927f);
            if (vector_distance_xy(&moby->pos, snow_plow_vars + 0x160) < 1.0f &&
                fast_difference_between_rotations(moby->rot.z, *(float *)(snow_plow_vars + 0x14c)) <
                    0.034906585f) {
                *(unsigned char *)(snow_plow_vars + 0x149) = 0;
                *(short *)(snow_plow_vars + 0x14a) = scale_game_frames(600);
                *(float *)(snow_plow_vars + 0xfc) = amount;
                *(float *)(snow_plow_vars + 0x7c) = amount;
                *(int *)(snow_plow_vars + 0x11c) = 0;
            }
            fast_vec_sub(&delta, snow_plow_vars + 0x160, &moby->pos);
            length = fast_vec_length(&delta);
            step = frame_time * 3.0f;
            return_delta_ptr = &delta;
            if (length < step) {
                speed = length;
            } else {
                length -= step; /* distance left after this step */
                speed = step;
            }
            fast_vec_normalize(return_delta_ptr, return_delta_ptr, speed);
            fast_vec_add(&moby->pos, &moby->pos, &delta);
            break;
        }
    }
    if (!state) {
        switch ((short)(moby->oclass - 0x1d2)) {
        case SNOW_PLOW_CLASS_OFFSET_WITH_THREE_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ONE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_QUARTER_TURN_LINKED_MOBY:
        case SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPHERE_QUERY_AND_TARGET_VOICE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPHERE_QUERY_WITHOUT_TARGET_VOICE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_ALIGNED_LINKED_MOBY:
        case SNOW_PLOW_CLASS_OFFSET_WITH_FOUR_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ONE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_THREE_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ZERO:
        case SNOW_PLOW_CLASS_OFFSET_WITH_TARGET_VOICE_SELECTOR_ZERO:
            attach_manipulator((char *)moby, 0, snow_plow_vars + 0x180);
            *(int *)(snow_plow_vars + 0x140) = 0;
            break;
        case SNOW_PLOW_CLASS_OFFSET_WITH_TWO_ROTATING_MANIPULATORS:
        case SNOW_PLOW_CLASS_OFFSET_WITH_TWO_POINT_CART_BURST:
            attach_manipulator((char *)moby, 0, snow_plow_vars + 0x180);
            *(int *)(snow_plow_vars + 0x140) = 0;
            attach_manipulator((char *)moby, 1, snow_plow_vars + 0x1c0);
            *(int *)(snow_plow_vars + 0x144) = 0;
            break;
        }
    } else {
        switch ((short)(moby->oclass - 0x1d2)) {
        case SNOW_PLOW_CLASS_OFFSET_WITH_THREE_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ONE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_QUARTER_TURN_LINKED_MOBY:
            *(float *)(snow_plow_vars + 0x140) =
                fast_add_rotations(*(float *)(snow_plow_vars + 0x140), frame_time * 12.566371f);
            FUN_L00_001fff28(snow_plow_vars + 0x190, 0, *(float *)(snow_plow_vars + 0x140));
            break;
        case SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPHERE_QUERY_AND_TARGET_VOICE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPHERE_QUERY_WITHOUT_TARGET_VOICE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_ALIGNED_LINKED_MOBY:
        case SNOW_PLOW_CLASS_OFFSET_WITH_FOUR_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ONE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_THREE_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ZERO:
        case SNOW_PLOW_CLASS_OFFSET_WITH_TARGET_VOICE_SELECTOR_ZERO:
            *(float *)(snow_plow_vars + 0x140) =
                fast_add_rotations(*(float *)(snow_plow_vars + 0x140), frame_time * 12.566371f);
            FUN_L00_001fff28(snow_plow_vars + 0x190, 2, *(float *)(snow_plow_vars + 0x140));
            break;
        case SNOW_PLOW_CLASS_OFFSET_WITH_TWO_ROTATING_MANIPULATORS:
        case SNOW_PLOW_CLASS_OFFSET_WITH_TWO_POINT_CART_BURST:
            *(float *)(snow_plow_vars + 0x140) =
                fast_add_rotations(*(float *)(snow_plow_vars + 0x140), frame_time * 12.566371f);
            FUN_L00_001fff28(snow_plow_vars + 0x190, 2, *(float *)(snow_plow_vars + 0x140));
            *(float *)(snow_plow_vars + 0x144) =
                fast_add_rotations(*(float *)(snow_plow_vars + 0x144), frame_time * 12.566371f);
            FUN_L00_001fff28(snow_plow_vars + 0x1d0, 2, *(float *)(snow_plow_vars + 0x144));
            break;
        }
    }
    if (*(int *)(snow_plow_vars + 0x17c) != -1) {
        struct Moby *other =
            (struct Moby *)(D_L04_0015FFD8 + (*(int *)(snow_plow_vars + 0x17c) << 8));
        other->draw_dist = 0x7f;
        switch (moby->oclass) {
        case SNOW_PLOW_CLASS_WITH_QUARTER_TURN_LINKED_MOBY:
            delta.q = 0;
            delta.f[0] = 0.5f;
            delta.f[2] = -5.0f;
            transform_vector_by_basis(&delta, &delta, &moby->rot_row0);
            fast_vec_add(&other->pos, &moby->pos, &delta);
            qcopy(&other->rot, &moby->rot);
            other->rot.y = fast_add_rotations(other->rot.y, 1.5707964f);
            break;
        case SNOW_PLOW_CLASS_WITH_ALIGNED_LINKED_MOBY:
            delta.q = 0;
            delta.f[2] = -2.0f;
            transform_vector_by_basis(&delta, &delta, &moby->rot_row0);
            fast_vec_add(&other->pos, &moby->pos, &delta);
            qcopy(&other->rot, &moby->rot);
            break;
        default:
            goto hit;
        }
        advance_moby_animation(other);
        update_entity(other);
        refresh_moby_spatial_bounds_from_basis(other);
        other->flags |= 6;
    }
hit:
    if (find_moby_hit(moby, 0x210000, 0)) {
        if (++snow_plow_hit_count_gp >= 10) {
            unsigned char *flags = item_unlocked;
            if (flags[6] == 0) {
                flags[6] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53d6, -1);
            }
        }
        switch ((short)(moby->oclass - 0x1d2)) {
        case SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPHERE_QUERY_AND_TARGET_VOICE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPHERE_QUERY_WITHOUT_TARGET_VOICE:
            sphere.flags = 0x810000;
            sphere.scale = 1.0f;
            sphere.moby = moby;
            clear_u64_value(&sphere_pos);
            sphere.zero = 0;
            collect_mobies_in_sphere(&moby->pos, 4.0f, 0x10, moby, &sphere_pos);
        case SNOW_PLOW_CLASS_OFFSET_WITH_THREE_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ONE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_QUARTER_TURN_LINKED_MOBY:
        case SNOW_PLOW_CLASS_OFFSET_WITH_TWO_ROTATING_MANIPULATORS:
        case SNOW_PLOW_CLASS_OFFSET_WITH_HIT_SPAWNS_647_648_649:
        case SNOW_PLOW_CLASS_OFFSET_WITH_ALIGNED_LINKED_MOBY:
        case SNOW_PLOW_CLASS_OFFSET_WITH_FOUR_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ONE:
        case SNOW_PLOW_CLASS_OFFSET_WITH_THREE_HIT_SPAWNS_AND_TARGET_VOICE_SELECTOR_ZERO:
        case SNOW_PLOW_CLASS_OFFSET_WITH_TARGET_VOICE_SELECTOR_ZERO:
            delta.q = 0;
            delta.f[2] = 0.75f;
            clear_u64_value(&sphere_pos);
            fast_vec_add(&delta, &delta, &moby->pos);
            spawn_explosion(moby,
                            &sphere_pos,
                            &delta,
                            0.0f,
                            0.0f,
                            10,
                            3,
                            4,
                            2.0f,
                            1.0f,
                            100000.0f,
                            3.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            if (moby->oclass == 0x1f2 || moby->oclass == 0x22b)
                allocate_voice_for_target_entry(0, 0, moby);
            else if (moby->oclass != 0x1e6)
                allocate_voice_for_target_entry(1, 0, moby);
            *(int *)(snow_plow_vars + 0x170) = scale_game_frames(600);
            *(int *)(snow_plow_vars + 0x174) = 1;
            break;
        case SNOW_PLOW_CLASS_OFFSET_WITH_TWO_POINT_CART_BURST:
            sphere_pos.q = 0;
            sphere_pos.f[2] = 0.75f;
            clear_u64_value(&sphere);
            moby_point_position(moby, 0, &delta);
            fast_vec_add(&delta, &delta, &sphere_pos);
            cart_burst(moby, &delta, -1, 1.0f, 13.0f);
            moby_point_position(moby, 1, &delta);
            fast_vec_add(&delta, &delta, &sphere_pos);
            cart_burst(moby, &delta, -1, 1.0f, 13.0f);
            fast_vec_add(&delta, &sphere_pos, &moby->pos);
            spawn_explosion(moby,
                            &sphere,
                            &delta,
                            0.0f,
                            0.0f,
                            20,
                            5,
                            4,
                            2.0f,
                            1.0f,
                            100000.0f,
                            3.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            allocate_voice_for_target_entry(1, 0, moby);
            sphere2.flags = 0x810000;
            sphere2.scale = 2.0f;
            sphere2.moby = moby;
            clear_u64_value(&sphere2.vector);
            sphere2.zero = 0;
            collect_mobies_in_sphere(&moby->pos, 6.0f, 0x10, moby, &sphere2);
            *(int *)(snow_plow_vars + 0x170) = scale_game_frames(600);
            *(int *)(snow_plow_vars + 0x174) = 1;
            break;
        }
        fast_vec_scale(&delta, &moby->rot_row0, frame_scale * 0.075f);
        delta.f[2] += frame_scale * 0.08f;
        if (moby->oclass == 0x1d2) {
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x63f,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x641,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x642,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
        } else if (moby->oclass == 0x1e0) {
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x642,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x643,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x644,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
        } else if (moby->oclass == 0x1e6) {
            int i;
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x645,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            i = 5;
            do {
                --i;
                timer = scale_game_frames(90);
                FUN_L00_00263fd8(moby,
                                 0x646,
                                 &moby->pos,
                                 &moby->rot,
                                 timer,
                                 0,
                                 frame_time_sq * 12.0f,
                                 &delta,
                                 D_L04_0015F580,
                                 D_L04_0015F580);
            } while (i >= 0);
        } else if (moby->oclass == 0x1ea) {
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x647,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x648,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x649,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
        } else if (moby->oclass == 0x1ee) {
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x64d,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x64e,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
        } else if (moby->oclass == 0x1ef) {
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x64f,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x650,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x651,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x658,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
        } else if (moby->oclass == 0x1f2) {
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x652,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x653,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x654,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
        } else if (moby->oclass == 0x1ed) {
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x793,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            timer = scale_game_frames(90);
            FUN_L00_00263fd8(moby,
                             0x794,
                             &moby->pos,
                             &moby->rot,
                             timer,
                             0,
                             frame_time_sq * 12.0f,
                             &delta,
                             D_L04_0015F580,
                             D_L04_0015F580);
            {
                int i = 2;
                do {
                    --i;
                    timer = scale_game_frames(90);
                    FUN_L00_00263fd8(moby,
                                     0x795,
                                     &moby->pos,
                                     &moby->rot,
                                     timer,
                                     0,
                                     frame_time_sq * 12.0f,
                                     &delta,
                                     D_L04_0015F580,
                                     D_L04_0015F580);
                } while (i >= 0);
            }
            {
                int i = 2;
                do {
                    --i;
                    timer = scale_game_frames(90);
                    FUN_L00_00263fd8(moby,
                                     0x796,
                                     &moby->pos,
                                     &moby->rot,
                                     timer,
                                     0,
                                     frame_time_sq * 12.0f,
                                     &delta,
                                     D_L04_0015F580,
                                     D_L04_0015F580);
                } while (i >= 0);
            }
        }
    }
    moby->hit_slot = 0xff;
    if (*(int *)(snow_plow_vars + 0x174) == 0)
        return;
    if (fast_dec_timer((int *)(snow_plow_vars + 0x170))) {
        char *camera = D_L04_00166E00;
        amount = fast_arc_tan(moby->pos.x - *(float *)(camera + 0x140),
                              moby->pos.y - *(float *)(camera + 0x144));
        if (1.5707964f < fast_difference_between_rotations(*(float *)(camera + 0x158), amount)) {
            moby->collision = moby->pclass->collision;
            moby->flags &= 0xffbe;
            *(int *)(snow_plow_vars + 0x174) = 0;
            if (*(int *)(snow_plow_vars + 0x17c) != -1) {
                struct Moby *other =
                    (struct Moby *)(D_L04_0015FFD8 + (*(int *)(snow_plow_vars + 0x17c) << 8));
                other->flags &= 0xffbe;
            }
            moby->flags |= 0x1000;
            return;
        }
    }
    moby->collision = 0;
    moby->flags |= 0x41;
    if (*(int *)(snow_plow_vars + 0x17c) != -1) {
        struct Moby *other =
            (struct Moby *)(D_L04_0015FFD8 + (*(int *)(snow_plow_vars + 0x17c) << 8));
        other->flags |= 0x41;
    }
    moby->flags &= 0xefff;
}

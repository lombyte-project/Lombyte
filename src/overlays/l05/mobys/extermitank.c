/* Level 05 moby class extermitank. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/game/fun_00213928.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/overlay/l05/mobys/extermitank_globals.h"

extern char  D_0013E533[];
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern void *create_moby() __asm__("FUN_0020c4f8");
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");
extern void  blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float fast_cos(float);
extern float fast_sin(float);
float        absolute_float(float input) __asm__("func_001F99C0");
extern void  FUN_L01_0026d930(char *p);
extern char  D_L05_001671C0[];
extern float fast_vec_dist(void *, void *);
extern void  FUN_L00_0024f7c8(void *, int, void *);
extern void  FUN_L00_0025a120(void *);
extern int   collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
/* Extermitank update: spawns its two parts, follows its path, turns its turret toward the hero and sprays particles while firing. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00305778), where it is exact; names translated to the US level program. */

extern float D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float step_angle_toward_target(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern int   frame_number_gp __asm__("D_L05_0015F5CC") MACRO_ADDR;
extern int   FUN_L00_00259028(char *m, char *p, float *pos, char *v);
extern int   FUN_L00_00259740(void *, void *, int);
extern s32
spawn_spark_c2(void *, void *, void *, s32, s32, s32, s32, s32, s32) __asm__("func_00218888");
extern float D_L05_00161AF8 __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermitank_data/extermitank_update_6.h"

extern float D_L05_00161B18 __attribute__((sda));
extern float D_L05_00161B1C __attribute__((sda));
extern float extermitank_tracked_particle_vector_a_xy_scale_gp __asm__("D_L05_00161B20")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermitank_data/extermitank_update_5.h"

extern float extermitank_tracked_particle_vector_b_xy_scale_gp __asm__("D_L05_00161B28")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermitank_data/extermitank_update_4.h"

extern float extermitank_tracked_particle_vector_b_spread_degrees_gp __asm__("D_L05_00161B38")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermitank_data/extermitank_update_3.h"

extern float D_L05_00161B48 __attribute__((sda));
extern float D_L05_00161B4C __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_xy_scale_gp __asm__("D_L05_00161B50")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermitank_data/extermitank_update_2.h"

extern float extermitank_secondary_particle_vector_b_xy_scale_gp __asm__("D_L05_00161B58")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermitank_data/extermitank_update.h"

extern float extermitank_secondary_particle_vector_b_spread_degrees_gp __asm__("D_L05_00161B68")
    __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_spread_degrees_gp __asm__("D_L05_00161B6C")
    __attribute__((sda));
extern short D_L05_00161B78 __attribute__((sda));
extern float D_L05_00161B7C __attribute__((sda));
extern void  FUN_L00_001fff28(void *, int, float);
extern void  mark_moby_for_removal_c3(void *) __asm__("func_0020C828");
void         handle_extermitank_hits_and_sync_children(char *moby) __asm__("FUN_L05_00305178");
void         process_extermitank_particle_hits(char *m) __asm__("FUN_L05_00305898");
void         attach_manipulator(void *owner, s32 slot_index, void *manip) __asm__("FUN_0020cb10");
extern char  D_0013F350[];

typedef char *(*Spawn_305778)(void *, void *, void *, int, int, long, long, long, int);

void extermitank_update(unsigned char *m) __asm__("FUN_L05_00304320");

void extermitank_update(unsigned char *moby) {
    enum {
        EXTERMITANK_INITIALIZE = 0,
        EXTERMITANK_FOLLOW_PATH = 1,
        EXTERMITANK_FIRE_PARTICLES = 2,
        EXTERMITANK_REMOVE = 3
    };
    char *data = ((char *)MOBY(moby)->pvars);
    float origin_position[4];
    float particle_vector_a[4];
    float particle_vector_b[4];
    float working_position[4];

    handle_extermitank_hits_and_sync_children((char *)moby);
    process_extermitank_particle_hits((char *)moby);
    if (*(unsigned char *)(data + 0x2E) == 2) {
        if (*(char **)(data + 0x184) != 0)
            mark_moby_for_removal_c3(*(char **)(data + 0x184));
        if (*(char **)(data + 0x188) != 0)
            mark_moby_for_removal_c3(*(char **)(data + 0x188));
        mark_moby_for_removal_c3(moby);
        return;
    }
    if (moby[0x31] != 0) {
        if (fast_vec_dist(moby + 0x10, D_L05_001671C0) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby[0x7F] = 0x15;
        }
    }
    switch (moby[0x20]) {
    case EXTERMITANK_INITIALIZE: {
        char *child_moby;
        char *g;
        if (*(char **)(data + 0x184) == 0) {
            *(char **)(data + 0x184) = create_moby(0x368);
            (*(char **)(data + 0x184))[0x30] = 0x40;
            *(short *)(*(char **)(data + 0x184) + 0x32) = 0x40;
            (*(char **)(data + 0x184))[0x31] = 1;
            {
                char *g2 = ((char *)&D_0013F350);
                *(long *)(*(char **)(data + 0x184) + 0x38) =
                    *(long *)(*(char **)(g2 + 0x2080) + 0x38);
            }
            *(short *)(*(char **)(data + 0x184) + 0x34) = 0x4000;
            child_moby = *(char **)(data + 0x184);
            qcopy(child_moby + 0x10, moby + 0x10);
            MOBY(child_moby)->collision = 0;
            update_entity(*(char **)(data + 0x184));
        }
        if (*(char **)(data + 0x188) == 0) {
            *(char **)(data + 0x188) = create_moby(0x369);
            (*(char **)(data + 0x188))[0x30] = 0x40;
            *(short *)(*(char **)(data + 0x188) + 0x32) = 0x40;
            (*(char **)(data + 0x188))[0x31] = 1;
            {
                char *g2 = ((char *)&D_0013F350);
                *(long *)(*(char **)(data + 0x188) + 0x38) =
                    *(long *)(*(char **)(g2 + 0x2080) + 0x38);
            }
            *(short *)(*(char **)(data + 0x188) + 0x34) = 0x4000;
            child_moby = *(char **)(data + 0x188);
            qcopy(child_moby + 0x10, moby + 0x10);
            MOBY(child_moby)->collision = 0;
            update_entity(*(char **)(data + 0x188));
        }
        attach_manipulator((char *)moby, 0, (unsigned char *)(data + 0xF0));
        attach_manipulator((char *)moby, 1, (unsigned char *)(data + 0x130));
        MOBY(moby)->flags |= 0x1000;
        g = ((char *)&D_0013F350);
        MOBY(moby)->spawn_frame = *(long *)(*(char **)(g + 0x2080) + 0x38);
        FUN_L01_0026d930(data + 0x70);
        *(float *)(data + 0x7C) = 2.0f;
        *(float *)(data + 0x78) = 4.0f;
        *(int *)(data + 0x70) = 0x38D;
        *(float *)(data + 0xAC) = 0.01f;
        *(float *)(data + 0xB0) = 0.3f;
        *(float *)(data + 0xB4) = 0.1f;
        *(char **)(data + 0xD0) = ((char *)l05_paths[*(int *)(data + 0xD4)]);
        *(int *)(data + 0xC0) = 1;
        qcopy(moby + 0x10, *(char **)(data + 0xD0) + 0x10);
        moby[0x20] = 1;
        if (moby[0x53] != 0)
            blend_moby_animation(moby, 0, 0, 0);
        *(float *)(data + 0x94) = D_L05_00161AF8 * D_0015ED6C_gp;
        clear_u64_value(data + 0x40);
        *(unsigned short *)(data + 0x3E) |= 2;
        data[0x2E] = 1;
        data[0x29] = 0;
        *(short *)(data + 0x24) = 6;
        data[0x28] = 3;
        *(float *)(data + 0x20) = 6.0f;
        *(int *)(data + 0x180) = scale_game_frames(0x3C);
        *(int *)(data + 0x198) = 0;
        {
            float t = *(volatile float *)&D_L05_00161B78;
            *(float *)(data + 0x194) = *(volatile float *)(moby + 0x48);
            data[0x58] = truncate_float_to_s32(t);
        }
        data[0x5A] = truncate_float_to_s32(D_L05_00161B7C * 8.0f);
        break;
    }
    case EXTERMITANK_FOLLOW_PATH: {
        char       *pp = data + 0xC0;
        char       *path = *(char **)(pp + 0x10);
        int         n = *(int *)path;
        signed char dir = *(signed char *)(pp + 4);
        int         last = 0;
        int         idx;
        char       *h;
        if (dir >= 0)
            last = n - 1;
        idx = (*(int *)(data + 0xC0) + n + dir) % n;
        FUN_L00_00259028(
            (char *)moby, data + 0x70, (float *)(path + (idx * 16 + 0x10)), data + 0x40);
        if (vector_distance_xy(moby + 0x10, *(char **)(pp + 0x10) + (idx * 16 + 0x10)) < 1.0f) {
            if (idx == last) {
                char *q = *(char **)(pp + 0x10);
                if (5.0f < fast_vec_dist(q + *(int *)q * 16, q + 0x10)) {
                    int t = -*(signed char *)(pp + 4);
                    *(signed char *)(pp + 4) = t;
                }
            }
            *(int *)pp = idx;
        }
        h = ((char *)&D_0013F350 + 0x80);
        if (vector_distance_xy(moby + 0x10, h) < *(float *)(data + 0x174)) {
            if (absolute_float(MOBY(moby)->pos.z - *(float *)(h + 8)) < 4.0f) {
                if (*(int *)(data + 0x190) == -1 ||
                    FUN_L00_00259740(h,
                                     ((char *)l05_paths[*(int *)(data + 0x190)]) + 0x10,
                                     *(int *)l05_paths[*(int *)(data + 0x190)]) != 0) {
                    qcopy(origin_position, moby + 0x10);
                    origin_position[2] = origin_position[2] + 3.0f;
                    if (collision_line(origin_position, h + 0x50, 2, moby, 0) == 0) {
                        if (moby[0x53] != 0)
                            blend_moby_animation(moby, 0, 0, 3);
                        moby[0x20] = 2;
                    }
                }
            }
        }
        if (vector_distance_xy(moby + 0x10, ((char *)&D_0013F350 + 0x80)) < 16.0f) {
            char *g = ((char *)&D_0013F350);
            float a = fast_arc_tan(*(float *)(g + 0x80) - MOBY(moby)->pos.x,
                                   *(float *)(g + 0x84) - MOBY(moby)->pos.y);
            step_angle_toward_target((float *)(data + 0x194), 0, a, D_0015ED6C_gp * 1.5707964f);
            step_angle_toward_target(
                (float *)(data + 0x198), 0, 0.08726646f, D_0015ED6C_gp * 1.5707964f);
        } else {
            step_angle_toward_target(
                (float *)(data + 0x194), 0, MOBY(moby)->rot.z, D_0015ED6C_gp * 1.5707964f);
            step_angle_toward_target((float *)(data + 0x198), 0, 0.0f, D_0015ED6C_gp * 1.5707964f);
        }
        FUN_L00_001fff28(data + 0x140, 1, *(float *)(data + 0x198));
        FUN_L00_001fff28(
            data + 0x100, 2, fast_subtract_rotations(*(float *)(data + 0x194), MOBY(moby)->rot.z));
        break;
    }
    case EXTERMITANK_FIRE_PARTICLES: {
        char       *pp = data + 0xC0;
        char       *path = *(char **)(pp + 0x10);
        int         n = *(int *)path;
        signed char dir = *(signed char *)(pp + 4);
        int         last = 0;
        int         idx;
        int         i;
        int         k;
        char       *h;
        if (dir >= 0)
            last = n - 1;
        idx = (*(int *)(data + 0xC0) + n + dir) % n;
        FUN_L00_00259028(
            (char *)moby, data + 0x70, (float *)(path + (idx * 16 + 0x10)), data + 0x40);
        if (vector_distance_xy(moby + 0x10, *(char **)(pp + 0x10) + (idx * 16 + 0x10)) < 1.0f) {
            if (idx == last) {
                char *q = *(char **)(pp + 0x10);
                if (5.0f < fast_vec_dist(q + *(int *)q * 16, q + 0x10)) {
                    int t = -*(signed char *)(pp + 4);
                    *(signed char *)(pp + 4) = t;
                }
            }
            *(int *)pp = idx;
        }
        particle_vector_a[3] = extermitank_tracked_particle_vector_a_w_gp;
        particle_vector_b[3] = extermitank_tracked_particle_vector_b_w_gp;
        FUN_L00_0024f7c8(moby, 2, origin_position);
        for (i = 0; i < 2; i++) {
            int   a, b, e;
            float ang;
            char *r;
            qcopy(working_position, origin_position);
            working_position[0] += random_float_between(-0.065f, 0.065f);
            working_position[1] += random_float_between(-0.065f, 0.065f);
            working_position[2] += random_float_between(-0.065f, 0.065f);
            particle_vector_a[0] =
                fast_cos(*(float *)(data + 0x194)) *
                (extermitank_tracked_particle_vector_a_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_a[1] =
                fast_sin(*(float *)(data + 0x194)) *
                (extermitank_tracked_particle_vector_a_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_a[2] = 0.0f;
            particle_vector_a[2] = random_float_between(
                extermitank_tracked_particle_vector_a_z_scale_gp * D_0015ED6C_gp * 0.9f,
                extermitank_tracked_particle_vector_a_z_scale_gp * D_0015ED6C_gp * 1.1f);
            ang = random_float_between(-extermitank_tracked_particle_vector_b_spread_degrees_gp,
                                       extermitank_tracked_particle_vector_b_spread_degrees_gp) *
                  0.017453292f;
            particle_vector_b[0] =
                fast_cos(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_tracked_particle_vector_b_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_b[1] =
                fast_sin(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_tracked_particle_vector_b_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_b[2] = 0.0f;
            particle_vector_b[2] = random_float_between(
                extermitank_tracked_particle_vector_b_z_scale_gp * D_0015ED6C_gp * 0.9f,
                extermitank_tracked_particle_vector_b_z_scale_gp * D_0015ED6C_gp * 1.1f);
            a = truncate_float_to_s32(scale_time(
                random_float_between(extermitank_tracked_particle_lifetime_base_frames_gp,
                                     extermitank_tracked_particle_lifetime_base_frames_gp +
                                         extermitank_tracked_particle_lifetime_base_frames_gp)));
            b = truncate_float_to_s32(
                scale_time(random_float_between(D_L05_00161B18, D_L05_00161B18 * 1.2f)));
            e = truncate_float_to_s32(
                scale_time(random_float_between(D_L05_00161B1C, D_L05_00161B1C * 1.2f)));
            r = ((Spawn_305778)spawn_spark_c2)(working_position,
                                               particle_vector_a,
                                               particle_vector_b,
                                               extermitank_tracked_particle_color_a_gp,
                                               extermitank_tracked_particle_color_b_gp,
                                               a,
                                               b,
                                               e,
                                               -1);
            if ((frame_number_gp & 7) == 0 && r != 0 && i == 0) {
                int j;
                for (j = 0; j < 8; j++) {
                    int   slot_byte_offset = j * 4;
                    char *particle_slots = data + 0x1A0;
                    if (*(char **)(particle_slots + slot_byte_offset) == 0) {
                        *(char **)(particle_slots + slot_byte_offset) = r;
                        break;
                    }
                }
            }
        }
        particle_vector_a[3] = extermitank_secondary_particle_vector_a_w_gp;
        particle_vector_b[3] = extermitank_secondary_particle_vector_b_w_gp;
        for (k = 1; k >= 0; k--) {
            int   a, b, e;
            float ang;
            qcopy(working_position, origin_position);
            working_position[0] += random_float_between(-0.2f, 0.2f);
            working_position[1] += random_float_between(-0.2f, 0.2f);
            working_position[2] += random_float_between(-0.2f, 0.2f);
            ang = random_float_between(-extermitank_secondary_particle_vector_a_spread_degrees_gp,
                                       extermitank_secondary_particle_vector_a_spread_degrees_gp) *
                  0.017453292f;
            particle_vector_a[0] =
                fast_cos(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_secondary_particle_vector_a_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_a[1] =
                fast_sin(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_secondary_particle_vector_a_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_a[2] = 0.0f;
            particle_vector_a[2] = random_float_between(
                extermitank_secondary_particle_vector_a_z_scale_gp * D_0015ED6C_gp * 0.7f,
                extermitank_secondary_particle_vector_a_z_scale_gp * D_0015ED6C_gp * 1.5f);
            ang = random_float_between(-extermitank_secondary_particle_vector_b_spread_degrees_gp,
                                       extermitank_secondary_particle_vector_b_spread_degrees_gp) *
                  0.017453292f;
            particle_vector_b[0] =
                fast_cos(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_secondary_particle_vector_b_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_b[1] =
                fast_sin(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_secondary_particle_vector_b_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_b[2] = 0.0f;
            particle_vector_b[2] = random_float_between(
                extermitank_secondary_particle_vector_b_z_scale_gp * D_0015ED6C_gp * 0.7f,
                extermitank_secondary_particle_vector_b_z_scale_gp * D_0015ED6C_gp * 1.5f);
            a = truncate_float_to_s32(scale_time(
                random_float_between(extermitank_secondary_particle_lifetime_base_frames_gp,
                                     extermitank_secondary_particle_lifetime_base_frames_gp +
                                         extermitank_secondary_particle_lifetime_base_frames_gp)));
            b = truncate_float_to_s32(
                scale_time(random_float_between(D_L05_00161B48, D_L05_00161B48 * 1.2f)));
            e = truncate_float_to_s32(
                scale_time(random_float_between(D_L05_00161B4C, D_L05_00161B4C * 1.2f)));
            ((Spawn_305778)spawn_spark_c2)(working_position,
                                           particle_vector_a,
                                           particle_vector_b,
                                           extermitank_secondary_particle_color_a_gp,
                                           extermitank_secondary_particle_color_b_gp,
                                           a,
                                           b,
                                           e,
                                           -1);
        }
        qcopy(working_position, moby + 0x10);
        working_position[2] = working_position[2] + 3.0f;
        h = ((char *)&D_0013F350 + 0x80);
        if (*(float *)(data + 0x174) < vector_distance_xy(moby + 0x10, h) ||
            4.0f <= absolute_float(MOBY(moby)->pos.z - *(float *)(h + 8)) ||
            (*(int *)(data + 0x190) != -1 &&
             FUN_L00_00259740(h,
                              ((char *)l05_paths[*(int *)(data + 0x190)]) + 0x10,
                              *(int *)l05_paths[*(int *)(data + 0x190)]) == 0) ||
            collision_line(working_position, D_0013E533 + 0xEED, 2, moby, 0) != 0) {
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, 3);
            moby[0x20] = 1;
        }
        {
            char *g = ((char *)&D_0013F350);
            float a = fast_arc_tan(*(float *)(g + 0x80) - MOBY(moby)->pos.x,
                                   *(float *)(g + 0x84) - MOBY(moby)->pos.y);
            step_angle_toward_target((float *)(data + 0x194), 0, a, D_0015ED6C_gp * 1.5707964f);
            step_angle_toward_target(
                (float *)(data + 0x198), 0, 0.08726646f, D_0015ED6C_gp * 1.5707964f);
        }
        FUN_L00_001fff28(data + 0x140, 1, *(float *)(data + 0x198));
        FUN_L00_001fff28(
            data + 0x100, 2, fast_subtract_rotations(*(float *)(data + 0x194), MOBY(moby)->rot.z));
        break;
    }
    case EXTERMITANK_REMOVE:
        if (*(char **)(data + 0x184) != 0)
            mark_moby_for_removal_c3(*(char **)(data + 0x184));
        if (*(char **)(data + 0x188) != 0)
            mark_moby_for_removal_c3(*(char **)(data + 0x188));
        mark_moby_for_removal_c3(moby);
        break;
    }
}

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char  D_L05_0015F580_macro[] __asm__("D_L05_0015F580") MACRO_ADDR;

extern void *
select_candidate_by_max_float_2c(void *, void *, void *, int) __asm__("FUN_L05_002899f8");
extern void mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
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
extern void
spawn_particle_305178(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern void  update_motion_305178(void *, void *) __asm__("FUN_L00_0025d458");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void  finish_motion_305178(void *, void *) __asm__("FUN_L00_0025d538");

int          D_L05_00161B74 = 5;
extern float extermitank_child_animation_speed_divisor_gp __asm__("D_L05_00161AF0")
    __attribute__((sda));
extern float extermitank_child_animation_speed_offset_scale_gp __asm__("D_L05_00161AF4")
    __attribute__((sda));
typedef struct Hero Hero;

void handle_extermitank_hits_and_sync_children(char *moby) {
    char        *data = ((char *)MOBY(moby)->pvars);
    char        *first;
    char        *second;
    char        *part0;
    char        *part1;
    char        *part2;
    char        *hit;
    char        *owner;
    int          result;
    float        remaining = 0.0f;
    float        position[4] __attribute__((aligned(16)));
    char         effect_data[16] __attribute__((aligned(16)));
    float        speed;
    float        angle;
    struct Hero *base;

    part0 = find_moby_hit(moby, 0x210000, 0);
    first = *(char **)(data + 0x184);
    part1 = first ? find_moby_hit(first, 0x210000, 0) : 0;
    second = *(char **)(data + 0x188);
    part2 = second ? find_moby_hit(second, 0x210000, 0) : 0;
    part0 = select_candidate_by_max_float_2c(part0, part1, part2, 0);
    if (part0 != 0) {
        if (*(char **)(part0 + 0x20) == moby)
            part0 = 0;
        owner = *(char **)(part0 + 0x20);
        if (owner != 0 && (*(short *)(owner + 0xa6) == 0x47 || *(short *)(owner + 0xa6) == 0))
            part0 = 0;
    }
    hit = part0;
    fun_00213928(moby, hit, data + 0x20, 0, &result, &remaining, 0, 4);
    if (result == 1 || (unsigned char)moby[0x20] == 3)
        goto mark_children;
    {
        *(float *)(data + 0x20) -= remaining;
        if (*(float *)(data + 0x20) > 0.0f && remaining > 0.0f) {
            allocate_voice_for_target_entry(2, 0, moby);
            ((unsigned char *)data)[0x67] = 200;
            goto update_motion;
        } else {
            allocate_voice_for_target_entry(3, 0, moby);
            mark_moby_for_removal_cf(moby, 0, -1);
            qcopy(position, moby + 0x10);
            position[2] += 3.0f;
            spawn_explosion(moby,
                            data + 0x40,
                            position,
                            0.0f,
                            0.0f,
                            0x14,
                            8,
                            0x14,
                            4.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            qcopy(position, moby + 0x10);
            position[0] += 2.0f * fast_cos(MOBY(moby)->rot.z);
            position[1] += 2.0f * fast_sin(MOBY(moby)->rot.z);
            spawn_explosion(moby,
                            data + 0x40,
                            position,
                            0.0f,
                            0.0f,
                            0x14,
                            8,
                            0x14,
                            4.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            qcopy(position, moby + 0x10);
            angle = fast_add_rotations(MOBY(moby)->rot.z, 3.14159274f);
            position[0] += 2.0f * fast_cos(angle);
            angle = fast_add_rotations(MOBY(moby)->rot.z, 3.14159274f);
            position[1] += 2.0f * fast_sin(angle);
            spawn_explosion(moby,
                            data + 0x40,
                            position,
                            0.0f,
                            0.0f,
                            0x14,
                            8,
                            0x14,
                            4.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            *(short *)(data + 0x19e) = scale_game_frames(D_L05_00161B74);
            moby[0x20] = 3;
            clear_u64_value(effect_data);
            *(float *)(effect_data + 8) = *(float *)(data + 0x194);
            spawn_particle_305178(moby,
                                  0x6dd,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6df,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6e0,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6fe,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6fe,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6ff,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6ff,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6ff,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            return;
        }
    }
update_motion:
    update_motion_305178(moby, data + 0x60);
mark_children:
    ((unsigned char *)moby)[0xa4] = 0xff;
    if (*(char **)(data + 0x184) != 0)
        ((unsigned char *)*(char **)(data + 0x184))[0xa4] = 0xff;
    if (*(char **)(data + 0x188) != 0)
        ((unsigned char *)*(char **)(data + 0x188))[0xa4] = 0xff;
    if (*(int *)(data + 0x38) != 0 && *(int *)(data + 0x178) == 0) {
        *(int *)(data + 0x178) = scale_game_frames(0xf0);
        *(int *)(data + 0x38) = 0;
        base = &hero;
        angle = fast_arc_tan(base->motion.pos.f[0] - MOBY(moby)->pos.x,
                             base->motion.pos.f[1] - MOBY(moby)->pos.y);
        if (fast_difference_between_rotations(MOBY(moby)->rot.z, angle) > 1.5707964f)
            ((signed char *)data)[0xc4] *= -1;
    }
    if (fast_dec_timer(data + 0x178) != 0)
        *(float *)(data + 0x174) = 20.0f;
    else
        *(float *)(data + 0x174) = 14.0f;
    if (*(char **)(data + 0x184) != 0) {
        char *first;
        float offset_scale;
        speed = fast_vec_length(data + 0x40);
        speed /= extermitank_child_animation_speed_divisor_gp * frame_time;
        offset_scale = extermitank_child_animation_speed_offset_scale_gp;
        *(float *)(*(char **)(data + 0x184) + 0x58) = speed;
        *(float *)(*(char **)(data + 0x184) + 0x58) += *(float *)(data + 0x84) * offset_scale;
        first = *(char **)(data + 0x184);
        qcopy(first + 0x40, moby + 0x40);
        qcopy(first + 0x10, moby + 0x10);
    }
    if (*(char **)(data + 0x188) != 0) {
        char *second;
        float offset_scale;
        speed = fast_vec_length(data + 0x40);
        speed /= extermitank_child_animation_speed_divisor_gp * frame_time;
        offset_scale = extermitank_child_animation_speed_offset_scale_gp;
        *(float *)(*(char **)(data + 0x188) + 0x58) = speed;
        *(float *)(*(char **)(data + 0x188) + 0x58) -= *(float *)(data + 0x84) * offset_scale;
        second = *(char **)(data + 0x188);
        qcopy(second + 0x40, moby + 0x40);
        qcopy(second + 0x10, moby + 0x10);
    }
    finish_motion_305178(moby, data + 0x60);
}
/* Drops stale entries from the moby's 8 slots, then pushes each remaining one away from the hero. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00306CF0), where it is exact; names translated to the US level program. */

typedef struct {
    float         v[4];
    char         *owner;
    int           x14;
    unsigned char x18;
    unsigned char x19;
    short         type;
    float         x1C;
    int           flags;
} Hit_306CF0;

extern void
collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern void fast_vec_sub(void *dst, void *a, void *b) __asm__("FUN_001f9a28");

void process_extermitank_particle_hits(char *moby) {
    char       **slots = (char **)(((char *)MOBY(moby)->pvars) + 0x1A0);
    struct Hero *base;
    Hit_306CF0   hit_info;
    int          i;
    int          j;
    for (i = 0; i < 8; i++) {
        char *tracked_particle = slots[i];
        if (tracked_particle != 0) {
            if (*(unsigned char *)tracked_particle != 2 ||
                *(short *)(tracked_particle + 0x32) == 0) {
                slots[i] = 0;
            }
        }
    }
    base = &hero;
    if (base->items[2].item_id == 6)
        return;
    hit_info.flags |= 1;
    hit_info.owner = moby;
    hit_info.x14 = 1;
    hit_info.x1C = 1.0f;
    fast_vec_sub(hit_info.v, &base->motion.pos, moby + 0x10);
    hit_info.v[2] = 0.0f;
    set_vector_length_xy(hit_info.v, hit_info.v, 1.5f);
    hit_info.v[2] = 1.0f;
    hit_info.v[3] = 5627.9248f;
    hit_info.x19 = 1;
    hit_info.type = MOBY(moby)->oclass;
    hit_info.x18 = 0;
    for (j = 0; j < 8; j++) {
        if (slots[j] != 0) {
            collect_mobies_in_sphere(slots[j] + 0x10, 0, moby, &hit_info, 0.75f);
        }
    }
}

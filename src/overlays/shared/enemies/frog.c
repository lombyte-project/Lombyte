/* Shared moby class frog: hops along a path and chases or flees its target; its suck cannon methods. */

#include "types.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/math/rotations/extract_matrix_rotation_quaternion.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/shared/enemies/frog_data/update_frog_health_respawn_and_target.h"
#include "rnc/overlay/shared/enemies/frog_data/spawn_moby_directional_spark_bursts_2.h"
#include "rnc/overlay/shared/enemies/frog_data/spawn_moby_directional_spark_bursts.h"

struct MobyHit {
    u8           pad0[0x10];
    Vec4f        pos;
    struct Moby *source;
};

struct RespawnVars {
    u8           pad0[0x20];
    f32          health;
    s16          max_health;
    u8           pad26[0xA];
    f32          spin;
    u8           pad34[4];
    int          new_blink;
    u8           pad3C[0x112];
    u8           sparks_pending;
    u8           pad14F[0x31];
    Vec4f        target_pos;
    u8           pad190[0x30];
    struct Moby *target;
    int          target_mode;
    u8           pad1C8[0x14];
    f32          base_range;
    f32          range;
    u8           pad1E4[4];
    s16          blink_timer;
    u8           pad1EA[0xA];
    int          path;
    u8           pad1F8[0xC];
    int          target_id;
    u8           pad208[2];
    s16          frame_slot;
    struct Moby *carried;
    Vec4f        spawn_pos;
    u8           pad220[0x20];
    s16          lives;
    u8           pad242[6];
    f32          glow_phase;
};

typedef struct {
    char *moby;
    f32   score;
} Best;

extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern float FUN_L00_0025abf0(void *, void *, void *, float, float);
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int   FUN_L00_00261b48(int, void *, void *, void *);
extern int   FUN_L00_002dc9f0(void *, void *);
extern void  FUN_L00_00257470(void *, int, int);
extern void  collect_mobies_in_sphere_at_moby_point(
     void *, int, int, float, float, float, int, int, int) __asm__("FUN_L00_002599e8");
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void
blend_frog_rotation_to_facing_frame(char *, float *, float *) __asm__("FUN_L06_002e9998");
extern void probe_frog_local_z_collision(char *) __asm__("FUN_L06_002e9aa8");
extern void FUN_L06_002e9b60_c(void *, void *, int, float, float) __asm__("FUN_L06_002e9b60");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
void        trooper_start_knockback(u8 *a, void *b, int c, int d, int e, float f) __asm__("FUN_L00_0025c558");
void       update_frog_health_respawn_and_target(struct Moby *moby) __asm__("FUN_L06_002e94f0");
void       destroy_frog_child_moby(struct Moby *moby) __asm__("FUN_L06_002e9f30");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void  mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
void         spawn_moby_directional_spark_bursts(struct Moby *moby,
                                                 Vec4 *spark_push_direction) __asm__("FUN_L06_002ea198");
extern void  spawn_moby_death_effect_burst(char *) __asm__("FUN_L06_002ea498");
extern float scale_time(float) __asm__("FUN_001f96b0");
void         select_hero_or_gadgebot_target(char  *moby,
                                            char **current,
                                            f32    range) __asm__("FUN_L06_002f34d8");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int
activate_effect_l08(void *, void *, float, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void
resolve_point_path_segment_xy_clearance(float, int, void *, void *) __asm__("FUN_L00_00261d78");
extern void spawn_crate_debris(void *, void *, void *, void *, int, int, float, float) __asm__("FUN_L00_002af8b0");
extern void  add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern float fast_cos(float);
extern float fast_sin(float);
extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void  random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void  spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern int   process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");
extern int   store_moby_in_suck_cannon(void *) __asm__("FUN_L00_002dbb20");
extern s32   cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");
extern float fast_add_rotations(float, float);
extern float vector_length_xy(void *);
extern int   coll_sphere(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
extern int follow_path(int *, int, int, void *, void *, void *, float) __asm__("FUN_L01_00276fe8");
extern int scale_game_frames(int);

extern float              D_0015ED6C;
extern int                level_frame_count __asm__("D_L06_0015F5CC");
extern char               D_L06_00167540[];
extern s32                level_frame_count __asm__("D_L06_0015F5CC");
extern char               D_L06_00174600[];
extern float              D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float D_0015ED70   MACRO_ADDR;
extern int               *D_L06_001B0C30[];
extern int D_L06_0015F5C4 MACRO_ADDR;
extern int D_L06_0015FDF8 MACRO_ADDR;
extern int                D_L06_00161C10 __attribute__((sda));
extern int                D_L06_00161C14 __attribute__((sda));
extern short              D_L06_00161BF0[1] __attribute__((sda));
extern char               D_0013F3D0[];
extern f32                D_L06_0015F580[] MACRO_ADDR;
extern struct Moby       *D_L06_0015FFD8_e94f0 __asm__("D_L06_0015FFD8");
extern u8                 D_001413F4_e94f0[] __asm__("D_001413F4");
extern struct Moby       *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern float frog_facing_rotation_blend_factor_gp __asm__("D_L06_00161C00") __attribute__((sda));
extern f32   D_0015ED6C;
extern u128 D_L06_001745E0 NOT_SDA;
extern OvlVec4             coll_hit_pos __asm__("D_L06_001745F0") NOT_SDA; /* position after push-out of the last collision query */
extern s32 moby_spark_burst_count_gp __asm__("D_L06_00161C18") __attribute__((sda));
extern s32 moby_sparks_per_burst_gp __asm__("D_L06_00161C1C") __attribute__((sda));
extern f32 moby_spark_burst_position_jitter_bound_gp __asm__("D_L06_00161C20") __attribute__((sda));
extern f32 moby_spark_position_jitter_bound_gp __asm__("D_L06_00161C24") __attribute__((sda));
extern f32 moby_spark_velocity_jitter_bound_gp __asm__("D_L06_00161C28") __attribute__((sda));
extern f32 moby_spark_horizontal_speed_range_start_gp __asm__("D_L06_00161C2C")
    __attribute__((sda));
extern f32 moby_spark_horizontal_speed_range_end_gp __asm__("D_L06_00161C30") __attribute__((sda));
extern f32 moby_spark_vertical_speed_range_start_gp __asm__("D_L06_00161C34") __attribute__((sda));
extern f32 moby_spark_vertical_speed_range_end_gp __asm__("D_L06_00161C38") __attribute__((sda));
extern f32 D_L06_00161C40 __attribute__((sda));
extern f32 D_L06_00161C44 __attribute__((sda));
extern f32 moby_spark_push_scale_range_end_gp __asm__("D_L06_00161C64") __attribute__((sda));
extern float D_0015ED70;
extern char *D_L06_001AC180_2f34d8[] __asm__("D_L06_001AC180");
extern s32   gadgebot_target_cache_frame_gp __asm__("D_L06_00161CE8") __attribute__((sda));
extern s32   cached_gadgebot_target_count_gp __asm__("D_L06_00161CE4") __attribute__((sda));
extern char *cached_gadgebot_target_mobies[] __asm__("D_L06_001DAE60");

s32 D_L10_001D9F20[1] __attribute__((section(".data"))) = {0};

int   D_L10_0015FDF8 = 0;
int   D_L10_00161B00 = 0;
int   D_L10_00161B04 = 0;
short D_L10_00161AE0[1] = {0x404};

float              D_L10_00161AF0 = 0.1f;
s32                D_L10_00161B08 = 0x14;
s32                D_L10_00161B0C = 0xa;
f32                D_L10_00161B10 = 0.25f;
f32                D_L10_00161B14 = 0.125f;
f32                D_L10_00161B18 = 0.5f;
f32                D_L10_00161B1C = 1.0f;
f32                D_L10_00161B20 = 8.0f;
f32                D_L10_00161B24 = 2.0f;
f32                D_L10_00161B28 = 6.0f;
f32                D_L10_00161B30 = 0.125f;
f32                D_L10_00161B34 = 0.065f;
f32                D_L10_00161B54 = 0;
int D_L10_001ABE80 NOT_SDA = 0;
s32                D_L10_00161BD8 = -1;
s32                D_L10_00161BD4 = 0;

/* Frog (class 827) update: hops along a path, wanders near home, and chases or flees its target. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002E9AA8), where it is exact; names translated to the US level program. */
void frog_update(unsigned char *moby) __asm__("FUN_L06_002e8678");

void frog_update(unsigned char *moby) {
    float          A[4] __attribute__((aligned(16)));
    float          B[4] __attribute__((aligned(16)));
    float          C[4] __attribute__((aligned(16)));
    float          D[4] __attribute__((aligned(16)));
    char          *data;
    unsigned char *target;
    int            mode;

    data = ((char *)MOBY(moby)->pvars);
    if (D_L06_00161C10 != 0) {
        return;
    }
    mode = D_L06_0015F5C4;
    if (mode == 2 && ((MOBY(moby)->flags ^ 1) & 1)) {
        *(short *)(data + 0x1EA) = 1;
        MOBY(moby)->flags |= 0x41;
    } else if (mode == 0 && *(short *)(data + 0x1EA) != 0) {
        *(short *)(data + 0x1EA) = 0;
        MOBY(moby)->flags &= 0xFFBE;
    }
    if (moby[0x20] != 0x10) {
        if (*(int *)&D_L06_0015FDF8 != 0) {
            if (*(short *)(data + 0x242) == 0)
                return;
        } else {
            if (*(short *)(data + 0x242) != 0)
                return;
        }
    }
    FUN_L00_00263ac8(2.1f, (int)moby, 1, (unsigned char *)(data + 0x250));
    update_frog_health_respawn_and_target(moby);
    if (moby[0x31] != 0 && fast_vec_dist(moby + 0x10, D_L06_00167540) < 14.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7F] = 0xB;
    }
    target = *(unsigned char **)(data + 0x1C0);
    switch (moby[0x20]) {
    case 0: {
        float wait;
        int   spawn_count = D_L06_00161C14;
        int   divisor = *(short *)(data + 0x240);
        *(short *)(data + 0x20A) = spawn_count % 8;
        D_L06_00161C14 = spawn_count + 1;
        if (divisor != 0) {
            MOBY(moby)->unkB4 = MOBY(moby)->unkB4 / divisor;
        }
        MOBY(moby)->flags |= 0x100;
        data[0x58] = truncate_float_to_s32(8.0f);
        data[0x5A] = truncate_float_to_s32(3.2f);
        qcopy(data + 0x210, moby + 0x10);
        qcopy(data + 0x230, moby + 0x10);
        qcopy(data + 0x220, moby + 0x40);
        *(short *)(data + 0x208) = 0;
        wait = random_float_between(0.5f, 3.0f);
        *(float *)(data + 0x130) = D_0015ED70 * 20.0f;
        *(int *)(data + 0x148) = 0;
        *(int *)(data + 0x134) = 0;
        *(int *)(data + 0x140) = 0x200;
        *(float *)(data + 0x244) = wait;
        *(float *)(data + 0x248) = random_angle_radians();
        *(short **)(data + 0xD0) = D_L06_00161BF0;
        data[0x29] = 1;
        if (*(short *)(data + 0x200) != 0) {
            char *child;
            moby[0x20] = 1;
            child = create_moby(0x1F4);
            *(char **)(data + 0x20C) = child;
            if (child == 0) {
                mark_moby_for_removal(moby);
                return;
            }
            *(long *)(child + 0x38) = ((long)MOBY(moby)->spawn_frame);
            *(unsigned short *)(*(char **)(data + 0x20C) + 0x34) |= 0x4002;
            {
                char *child = *(char **)(data + 0x20C);
                qcopy(child + 0x10, moby + 0x10);
                qcopy(child + 0x40, moby + 0x40);
                *(short *)(child + 0x32) = 0x40;
            }
            (*(char **)(data + 0x20C))[0x31] = 1;
            update_entity(*(char **)(data + 0x20C));
            MOBY(moby)->flags |= 0x41;
        } else if (*(short *)(data + 0x202) != 0) {
            moby[0x20] = 0xF;
            MOBY(moby)->collision = 0;
            MOBY(moby)->flags = (MOBY(moby)->flags | 0x41) & 0xEFFF;
        } else if (*(int *)(data + 0x1F8) >= 0) {
            moby[0x20] = 3;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, 0);
        } else {
            moby[0x20] = 8;
            if (moby[0x53] != 4)
                blend_moby_animation(
                    moby,
                    4,
                    random_integer_below(
                        *(unsigned char *)(*(char **)(((char *)MOBY(moby)->pclass) + 0x58) + 0x10)),
                    0);
        }
        break;
    }
    case 1:
        if (vector_distance_xy(moby + 0x10, target + 0x10) < 4.0f) {
            MOBY(moby)->flags &= 0xFFBE;
            destroy_frog_child_moby(moby);
            moby[0x20] = 8;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(5));
        }
        break;
    case 3:
        fast_vec_sub(A, target + 0x10, moby + 0x10);
        probe_frog_local_z_collision((char *)moby);
        blend_frog_rotation_to_facing_frame((char *)moby, (float *)D_L06_00174600, A);
        if (vector_distance_xy(((char *)&D_0013F3D0), moby + 0x10) < *(float *)(data + 0x24C)) {
            moby[0x20] = 4;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(5));
        }
        break;
    case 4: {
        int *path;
        int  point_count;
        qcopy(B,
              (char *)D_L06_001B0C30[*(int *)(data + 0x1F8)] + *(short *)(data + 0x208) * 16 +
                  0x10);
        fast_vec_sub(A, B, moby + 0x10);
        probe_frog_local_z_collision((char *)moby);
        blend_frog_rotation_to_facing_frame((char *)moby, (float *)D_L06_00174600, A);
        fast_vec_normalize(C, moby + 0xC0, *(float *)(data + 0x1E4) * D_0015ED6C_gp);
        FUN_L06_002e9b60_c(moby, C, 0, 0.3f, 0.3f);
        transform_vector_by_basis(A, A, moby + 0xC0);
        if (vector_length_xy(A) < 0.5f) {
            float return_speed;
            float dist;
            path = D_L06_001B0C30[*(int *)(data + 0x1F8)];
            point_count = *path;
            if (*(short *)(data + 0x208) < point_count - 2) {
                *(short *)(data + 0x208) = *(short *)(data + 0x208) + 1;
                return;
            }
            qcopy(data + 0x210, (char *)path + point_count * 16);
            qcopy(data + 0x230, data + 0x210);
            dist = vector_distance_xy(moby + 0x10, data + 0x210);
            return_speed = dist / (float)scale_game_frames(30);
            *(float *)(data + 0x138) = return_speed;
            *(float *)(data + 0x13C) = FUN_L00_0025abf0(
                moby + 0x10, data + 0x210, 0, return_speed, -*(float *)(data + 0x130));
            data[0x15D] = 3;
            *(int *)(data + 0x144) = 5;
            trooper_start_knockback(moby,
                                    data + 0x120,
                                    2,
                                    1,
                                    0,
                                    fast_arc_tan(*(float *)(data + 0x210) - MOBY(moby)->pos.x,
                                                 *(float *)(data + 0x214) - MOBY(moby)->pos.y));
            moby[0x20] = 5;
        }
        break;
    }
    case 5:
        set_vector_unit_z(A);
        blend_frog_rotation_to_facing_frame((char *)moby, A, (float *)(data + 0x120));
        if (step_moby_motion(moby, data + 0x120) & 1) {
            moby[0x20] = 7;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, 1);
            MOBY(moby)->anim_time = 1.0f;
            MOBY(moby)->pos.z = probe_ground_height(moby + 0x10, 0, 0.5f);
        }
        break;
    case 6: {
        int hit = step_moby_motion(moby, data + 0x120);
        if (hit & 1) {
            moby[0x20] = 7;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, 1);
            MOBY(moby)->anim_time = 1.0f;
            MOBY(moby)->pos.z = probe_ground_height(moby + 0x10, 0, 0.5f);
        } else if (hit & 0x120) {
            FUN_L00_00257470(moby, 0, -1);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case 7:
        if (moby[0x70] & 2) {
            moby[0x20] = 8;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(5));
        }
        break;
    case 8:
        if (moby[0x31] != 0) {
            if (fast_vec_dist(moby + 0x10, data + 0x230) < 0.05f ||
                fast_dec_timer((int *)(data + 0x1FC)) != 0) {
                float angle = random_angle_radians();
                float radius = random_float_between(0.5f, 2.0f);
                A[0] = fast_cos(angle) * radius;
                A[1] = fast_sin(angle) * radius;
                A[2] = 0.0f;
                fast_vec_add(data + 0x230, data + 0x210, A);
                *(int *)(data + 0x1FC) =
                    scale_game_frames(truncate_float_to_s32(random_float_between(45.0f, 75.0f)));
            }
            fast_vec_sub(C, data + 0x230, moby + 0x10);
            probe_frog_local_z_collision((char *)moby);
            blend_frog_rotation_to_facing_frame((char *)moby, (float *)D_L06_00174600, C);
            fast_vec_normalize(B, moby + 0xC0, *(float *)(data + 0x1E4) * D_0015ED6C_gp);
            FUN_L06_002e9b60_c(moby, B, 1, 0.3f, 0.3f);
        }
        if (*(int *)(data + 0x1C4) != 2 && random_integer_below(9) == 0) {
            *(float *)(data + 0x1D4) =
                random_float_between(-*(float *)(data + 0x1D8), *(float *)(data + 0x1D8)) *
                0.017453292f;
            moby[0x20] = 9;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, 0);
        } else if (*(short *)(data + 0x1E8) != 0) {
            *(float *)(data + 0x1D4) =
                random_float_between(-*(float *)(data + 0x1D8), *(float *)(data + 0x1D8)) *
                0.017453292f;
            moby[0x20] = 10;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, 0);
        }
        break;
    case 9: {
        float angle =
            fast_add_rotations(fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                                            *(float *)(target + 0x14) - MOBY(moby)->pos.y),
                               *(float *)(data + 0x1D4));
        B[0] = fast_cos(angle);
        B[1] = fast_sin(angle);
        B[2] = 0.0f;
        probe_frog_local_z_collision((char *)moby);
        blend_frog_rotation_to_facing_frame((char *)moby, (float *)D_L06_00174600, B);
        fast_vec_normalize(A, moby + 0xC0, *(float *)(data + 0x1E4) * D_0015ED6C_gp);
        FUN_L06_002e9b60_c(moby, A, 1, 0.3f, 0.3f);
        if (vector_distance_xy(moby + 0x10, target + 0x10) < 2.0f) {
            moby[0x20] = 0xC;
            if (moby[0x53] != 5)
                blend_moby_animation(moby, 5, 4, scale_game_frames(10));
        } else if (*(int *)(data + 0x1C4) == 2) {
            moby[0x20] = 0xD;
        }
        break;
    }
    case 10: {
        int   near;
        float angle =
            fast_add_rotations(fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                                            *(float *)(target + 0x14) - MOBY(moby)->pos.y),
                               *(float *)(data + 0x1D4));
        qcopy(C, moby + 0x10);
        B[0] = fast_cos(angle);
        B[1] = fast_sin(angle);
        B[2] = 0.0f;
        near = 0;
        probe_frog_local_z_collision((char *)moby);
        blend_frog_rotation_to_facing_frame((char *)moby, (float *)D_L06_00174600, B);
        fast_vec_normalize(A, moby + 0xC0, *(float *)(data + 0x1E4) * D_0015ED6C_gp);
        FUN_L06_002e9b60_c(moby, A, 1, 0.3f, 0.3f);
        if (FUN_L00_00261b48(*(int *)(data + 0x1EC), moby + 0x10, target + 0x10, D) != 0 &&
            fast_vec_dist(moby + 0x10, D) < *(float *)(data + 0x244)) {
            near = 1;
        }
        if (*(short *)(data + 0x1E8) == 0) {
            moby[0x20] = 0xD;
        } else if (near) {
            moby[0x20] = 0xB;
            if (moby[0x53] != 6)
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
        } else if (vector_distance_xy(moby + 0x10, target + 0x10) < 2.0f) {
            moby[0x20] = 0xC;
            if (moby[0x53] != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
        }
        break;
    }
    case 11:
        if (moby[0x70] & 2) {
            moby[0x20] = 0xA;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(20));
        }
        break;
    case 12: {
        float wait;
        B[0] = fast_cos(fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                                     *(float *)(target + 0x14) - MOBY(moby)->pos.y));
        B[1] = fast_sin(fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                                     *(float *)(target + 0x14) - MOBY(moby)->pos.y));
        B[2] = 0.0f;
        blend_frog_rotation_to_facing_frame((char *)moby, (float *)&MOBY(moby)->rot_row2, B);
        wait = compute_interpolated_record_value(moby);
        if (moby[0x52] == moby[0x53] && wait >= 10.0f && wait <= 15.0f &&
            *(int *)(((char *)&camera_secondary_mode)) != 0x51) {
            fast_vec_normalize(A,
                               moby + 0xC0,
                               (*(float *)(data + 0x1E4) + *(float *)(data + 0x1E4)) *
                                   D_0015ED6C_gp);
            FUN_L06_002e9b60_c(moby, A, 1, 0.3f, 0.3f);
            collect_mobies_in_sphere_at_moby_point(moby, 0, 1, 0.333f, 1.0f, 1.0f, 0, 1, 0);
            return;
        }
        if ((moby[0x70] & 2) && vector_distance_xy(moby + 0x10, target + 0x10) > 2.0f) {
            moby[0x20] = 9;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    }
    case 13:
        fast_vec_sub(B, data + 0x210, moby + 0x10);
        probe_frog_local_z_collision((char *)moby);
        blend_frog_rotation_to_facing_frame((char *)moby, (float *)D_L06_00174600, B);
        fast_vec_normalize(A, moby + 0xC0, *(float *)(data + 0x1E4) * D_0015ED6C_gp);
        FUN_L06_002e9b60_c(moby, A, 1, 0.3f, 0.3f);
        if (vector_distance_xy(moby + 0x10, data + 0x210) < 2.0f) {
            moby[0x20] = 8;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        } else if (*(int *)(data + 0x1C4) != 2) {
            moby[0x20] = 9;
        } else if (*(short *)(data + 0x1E8) != 0) {
            moby[0x20] = 0xA;
        }
        break;
    case 16:
        if (FUN_L00_002dc9f0(moby, data + 0x120) != 0) {
            moby[0x20] = 8;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            *(short *)(data + 0xC8) = 0;
        }
        break;
    }
}

void update_frog_health_respawn_and_target(struct Moby *moby) {
    enum {
        HIT_RESULT_LETHAL = 1,
        HIT_RESULT_REMOVE_OR_RESPAWN = 2,
        HIT_RESULT_NO_RESPONSE = 11
    };
    struct RespawnVars *vars;
    struct MobyHit     *hit;
    int                 kind;
    float               damage;
    int                 code;

    vars = (struct RespawnVars *)moby->pvars;
    vars->spin = moby->rot_row2.z * 0.25f;
    if (vars->carried != 0) {
        hit = find_moby_hit(vars->carried, 0x330000, 0);
        if (hit != 0 && (hit->source == 0 || hit->source->oclass != 0xAF)) {
            spawn_moby_directional_spark_bursts(moby, (Vec4 *)D_L06_0015F580);
            destroy_frog_child_moby(moby);
            mark_moby_for_removal_cf(moby, 0, -1);
            goto remove;
        }
    }
    damage = 0.0f;
    hit = find_moby_hit(moby, 0x330000, 0);
    code = resolve_moby_hit_response(moby, hit, &vars->health, 0, &kind, &damage, 0, 4);
    if (kind != 1 && moby->state != 0xE) {
        vars->health = vars->health - damage;
        if (vars->health <= 0.0f) {
            code = 1;
        }
        switch (code) {
        case HIT_RESULT_LETHAL:
        case HIT_RESULT_REMOVE_OR_RESPAWN:
            mark_moby_for_removal_cf(moby, 0, -1);
            spawn_moby_directional_spark_bursts(moby, (Vec4 *)&hit->pos);
            if (vars->sparks_pending == 1) {
                spawn_moby_death_effect_burst((char *)moby);
                vars->sparks_pending = 0;
            }
            allocate_voice_for_target_entry(3, 0, (int)moby);
            if (vars->carried != 0) {
                destroy_frog_child_moby(moby);
            }
            if (vars->lives == 0) {
            remove:
                mark_moby_for_removal(moby);
                return;
            }
            vars->lives = vars->lives - 1;
            vars->health = vars->max_health;
            qcopy(&moby->pos, &vars->spawn_pos);
            moby->state = 0xF;
            moby->collision = 0;
            moby->flags = (moby->flags & 0xEFFF) | 0x41;
            break;
        case 0:
        case 3:
        case HIT_RESULT_NO_RESPONSE:
            break;
        }
    }
    moby->hit_slot = 0xFF;
    vars->glow_phase = fast_add_rotations(
        vars->glow_phase, frog_glow_phase_rate_degrees_gp * 0.017453292f * D_0015ED6C);
    moby->glow_color = fast_tween_color(frog_glow_color_start_gp,
                                        frog_glow_color_end_gp,
                                        (fast_sin(vars->glow_phase) + 1.0f) * 0.5f);
    if (vars->new_blink != 0) {
        vars->blink_timer = truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
        vars->new_blink = 0;
    }
    fast_dec_timer_s16(&vars->blink_timer);
    if (vars->blink_timer != 0) {
        vars->range = vars->base_range + 6.0f;
    } else {
        vars->range = vars->base_range;
    }
    if (vars->target_id != -1) {
        struct Moby *target = &D_L06_0015FFD8_e94f0[vars->target_id];
        vars->target_mode = 1;
        vars->target = target;
        qcopy(&vars->target_pos, &target->pos);
    } else if (D_001413F4_e94f0[0] == 1) {
        if (level_frame_count % 8 == vars->frame_slot) {
            select_hero_or_gadgebot_target((char *)moby, (char **)&vars->target, vars->range);
            if (vars->target == 0) {
                vars->target_mode = 2;
            } else {
                int *path = D_L06_001B0C30[vars->path];
                if (point_in_path_xy(&vars->target->pos, path + 4, *path) == 0 ||
                    (vars->target->oclass == 0x359 &&
                     2.0f < vector_distance_xy(&moby->pos, &vars->target->pos))) {
                    vars->target = 0;
                    vars->target_mode = 2;
                } else {
                    vars->target_mode = 0;
                }
            }
        }
    } else {
        Vec4f *goal = &vars->target_pos;
        int   *path = D_L06_001B0C30[vars->path];
        if (activate_effect_l08(moby, goal, vars->range, 0, 0, path + 4, *path) != 2) {
            if (vars->range < vector_distance_xy(&vars->spawn_pos, goal) ||
                3.0f < absolute_float(moby->pos.z - vars->target_pos.z)) {
                vars->target_mode = 2;
            }
        }
    }
    if (vars->target == 0) {
        vars->target = D_001413D0_lui;
    }
}

/* Build an orthogonal frame from two vectors and apply it to the matrix at moby+0xC0. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002EADC8), where it is exact; names translated to the US level program. */
void blend_frog_rotation_to_facing_frame(char  *moby,
                                         float *reference_axis,

                                         float *facing_direction) {
    float facing_projection;
    float A[4], B[4], C[4], D[4], E[4], F[4], G[4], H[4];
    fast_vec_normalize(C, reference_axis, 1.0f);
    facing_projection = fast_vec_dot(C, facing_direction);
    fast_vec_normalize(E, C, facing_projection);
    fast_vec_sub(D, facing_direction, E);
    fast_vec_normalize(D, D, 1.0f);
    qcopy(H, C);
    qcopy(F, D);
    fast_vec_cross(G, F, H);
    moby += 0xC0;
    extract_matrix_rotation_quaternion(A, moby);
    extract_matrix_rotation_quaternion(B, F);
    quaternion_nlerp(A, A, B, frog_facing_rotation_blend_factor_gp);
    quaternion_to_rotation_basis(A, moby);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002EAED8), where it is exact; names translated to the US level program. */
void probe_frog_local_z_collision(char *moby) {
    float collision_line_end[4];
    float collision_line_start[4];
    float axis_offset[4];
    qcopy(collision_line_start, moby + 0x10);
    fast_vec_normalize(axis_offset, moby + 0xE0, 1.0f);
    fast_vec_add(collision_line_start, collision_line_start, axis_offset);
    qcopy(collision_line_end, moby + 0x10);
    fast_vec_normalize(axis_offset, moby + 0xE0, -1.0f);
    fast_vec_add(collision_line_end, collision_line_end, axis_offset);
    collision_line(collision_line_start, collision_line_end, 2, 0, 0);
}

void move_frog_with_collision_resolution(float  scale,
                                         float  threshold,
                                         char  *moby,
                                         float *out,
                                         int    follow_path) __asm__("FUN_L06_002e9b60");

void move_frog_with_collision_resolution(

    float scale, float threshold, char *moby, float *out, int follow_path) {
    extern int coll_sphere(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
    float      start[4];
    float      end[4];
    float      forward[4];
    float      backward[4];
    int        i;

    fast_vec_normalize(forward, moby + 0xE0, scale);
    fast_vec_normalize(backward, forward, -D_0015ED6C);
    fast_vec_add(out, out, backward);
    qcopy(start, moby + 0x10);
    fast_vec_add(start, start, forward);
    fast_vec_add(moby + 0x10, moby + 0x10, out);
    fast_vec_add(end, moby + 0x10, forward);

    if (threshold < fast_vec_length(out) && collision_line(start, end, 0, moby, 0) != 0) {
        qcopy(end, &D_L06_001745E0);
        fast_vec_sub(moby + 0x10, end, forward);
    }
    for (i = 0; i < 6; i++) {
        if (coll_sphere(end, threshold, 4, moby) != 0) {
            qcopy(end, &coll_hit_pos);
            fast_vec_sub(moby + 0x10, end, forward);
        } else {
            break;
        }
    }
    if (follow_path != 0) {
        resolve_point_path_segment_xy_clearance(
            0.5f, *(int *)(((char *)MOBY(moby)->pvars) + 0x1EC), moby + 0x10, moby + 0x10);
    }
}

/* Bursts 4 then 8 particles up from the moby, then removes the moby stored at vars+0x20C. */
void destroy_frog_child_moby(struct Moby *moby) {
    float origin[4];
    float debris_velocity[4];
    float pos[4];
    float offset[4];
    u8   *vars;
    int   i;
    int   j;

    vars = moby->pvars;
    allocate_voice_for_target_entry(4, 0, (int)moby);
    qcopy(origin, &moby->pos);
    origin[2] = origin[2] + 0.5f;
    for (i = 3; i >= 0; i--) {
        float speed = random_float_between(1.0f, 4.0f) * frame_time;
        *(u128 *)offset = 0;
        offset[0] = random_float_between(-0.625f, 0.625f);
        offset[1] = random_float_between(-0.625f, 0.625f);
        offset[2] = random_float_between(0.0f, 1.25f);
        *(u128 *)pos = *(u128 *)offset;
        fast_vec_normalize(debris_velocity, pos, speed);
        fast_vec_add(pos, pos, origin);
        spawn_crate_debris(
            *(void **)(vars + 0x20C), debris_velocity, pos, D_L06_0015F580, 0x165, 0, 0.3f, 0.6f);
    }
    for (j = 7; j >= 0; j--) {
        float speed = random_float_between(1.0f, 4.0f) * frame_time;
        *(u128 *)offset = 0;
        offset[0] = random_float_between(-0.625f, 0.625f);
        offset[1] = random_float_between(-0.625f, 0.625f);
        offset[2] = random_float_between(0.0f, 1.25f);
        *(u128 *)pos = *(u128 *)offset;
        fast_vec_normalize(debris_velocity, pos, speed);
        fast_vec_add(pos, pos, origin);
        spawn_crate_debris(*(void **)(vars + 0x20C),
                           debris_velocity,
                           pos,
                           D_L06_0015F580,
                           random_integer_below(2) + 0x166,
                           0,
                           0.5f,
                           0.7f);
    }
    mark_moby_for_removal(*(void **)(vars + 0x20C));
    *(int *)(vars + 0x20C) = 0;
}

/* Throws D_L06_00161C18 bursts of sparks from around the moby: each burst gets a random heading, speed and rise on top
   of a push along dir, and spawns D_L06_00161C1C sparks with random sizes, colours and lifetimes. */
void spawn_moby_directional_spark_bursts(struct Moby *moby, Vec4 *spark_push_direction) {
    Vec4 pos;
    Vec4 burst_spark_velocity;
    Vec4 jittered_spark_velocity;
    int  i;
    int  j;

    for (i = 0; i < moby_spark_burst_count_gp; i++) {
        float angle;
        float speed;
        float rise;
        Vec4 *spark_position_destination;
        Vec4 *origin;
        origin = &moby->pos;
        spark_position_destination = &pos;
        qcopy_nc(spark_position_destination, origin);
        add_random_offset_to_vector_xyz(&pos, moby_spark_burst_position_jitter_bound_gp);
        pos.f[2] += 0.25f;
        angle = random_angle_radians();
        speed = random_float_between(moby_spark_horizontal_speed_range_start_gp,
                                     moby_spark_horizontal_speed_range_end_gp);
        rise = random_float_between(moby_spark_vertical_speed_range_start_gp,
                                    moby_spark_vertical_speed_range_end_gp);
        fast_vec_scale(&burst_spark_velocity,
                       spark_push_direction,
                       random_float_between(0.0f, moby_spark_push_scale_range_end_gp) * frame_time);
        burst_spark_velocity.f[0] += fast_cos(angle) * (speed * frame_time);
        burst_spark_velocity.f[1] += fast_sin(angle) * (speed * frame_time);
        burst_spark_velocity.f[2] += rise * frame_time;
        qcopy(&jittered_spark_velocity, &burst_spark_velocity);
        jittered_spark_velocity.f[2] -=
            moby_spark_vertical_velocity_correction_scale_gp * frame_time_sq *
            (float)scale_game_frames(moby_spark_second_frame_count_base_gp);
        for (j = 0; j < moby_sparks_per_burst_gp; j++) {
            float size = random_float_between(0.5f, 1.5f);
            int   color0;
            int   color1;
            int   life;
            int   fade;
            float third_frame_count_base;
            burst_spark_velocity.f[3] = size * D_L06_00161C40;
            jittered_spark_velocity.f[3] = size * D_L06_00161C44;
            add_random_offset_to_vector_xyz(&pos, moby_spark_position_jitter_bound_gp);
            add_random_offset_to_vector_xyz(&jittered_spark_velocity,
                                            moby_spark_velocity_jitter_bound_gp * frame_time);
            color0 = fast_tween_color(moby_spark_first_color_range_start_gp,
                                      moby_spark_first_color_range_end_gp,
                                      random_float_between(0.0f, 1.0f));
            color1 = fast_tween_color(moby_spark_second_color_range_start_gp,
                                      moby_spark_second_color_range_end_gp,
                                      random_float_between(0.0f, 1.0f));
            life = scale_game_frames(moby_spark_first_frame_count_base_gp);
            fade = scale_game_frames(moby_spark_second_frame_count_base_gp);
            third_frame_count_base = (float)moby_spark_third_frame_count_base_gp;
            spawn_spark(&pos,
                        &burst_spark_velocity,
                        &jittered_spark_velocity,
                        color0,
                        color1,
                        life,
                        fade,
                        truncate_float_to_s32(scale_time(random_float_between(
                            third_frame_count_base * 0.5f, third_frame_count_base * 2.5f))),
                        -1);
        }
    }
}

/* burst of 20 sparks around a point with random velocity and colour */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002EB8C8), where it is exact; names translated to the US level program. */
void spawn_moby_death_effect_burst(char *moby) __asm__("FUN_L06_002ea498");

void spawn_moby_death_effect_burst(char *moby) {
    float spherical_offset[4];
    float spawn_position[4];
    int   i;
    for (i = 19; i >= 0; i--) {
        float azimuth_angle = random_angle_radians();
        float elevation_angle = random_angle_radians();
        int   random_frame_count, random_effect_parameter;
        build_spherical_offset(spherical_offset, frame_time * 1.8f, azimuth_angle, elevation_angle);
        spherical_offset[2] += frame_time * 0.9f;
        random_vector(spawn_position, 0.0f, 0.5f);
        fast_vec_add(spawn_position, spawn_position, moby + 0x10);
        spawn_position[2] += 0.5f;
        random_frame_count = scale_game_frames(random_int_between(0x14, 0x23));
        random_effect_parameter = random_int_between(0x28, 0x3C);
        spawn_effect(spawn_position,
                     spherical_offset,
                     0x7000A0FF,
                     0xFF,
                     random_frame_count,
                     0x1E,
                     random_effect_parameter,
                     1);
    }
}

/* Grabbed check: while FUN_L00_002db8f8 holds the moby, park it in state 0x10 (remembering the state it came from)
   unless it is in a state that cannot be grabbed; once released, drop it back to state 8. */
int update_moby_grab_state(struct Moby *moby) __asm__("FUN_L06_002ea5e0");

int update_moby_grab_state(struct Moby *moby) {
    int grab_check_result = process_moby_suck_cannon_suction(moby);
    if (grab_check_result != 0) {
        if (moby->state == 8 || moby->state == 9 || moby->state == 0xC || moby->state == 0xD ||
            moby->state == 0x10) {
            if (moby->state != 0x10) {
                moby->command = moby->state;
                moby->state = 0x10;
            }
        } else {
            grab_check_result = 0;
        }
    } else if (moby->state == 0x10) {
        moby->state = 8;
        if (moby->prev_seq != 0) {
            blend_moby_animation((MobyAnim *)moby, 0, 0, scale_game_frames(10));
        }
    }
    return grab_check_result;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002EBAD8), where it is exact; names translated to the US level program. */
int update_moby_suck_cannon_storage_state(unsigned char *moby) __asm__("FUN_L06_002ea6a8");

int update_moby_suck_cannon_storage_state(unsigned char *moby) {
    int suck_cannon_store_result = store_moby_in_suck_cannon(moby);
    if (suck_cannon_store_result != 0) {
        if (moby[0x20] != 0x10) {
            moby[0xBC] = moby[0x20];
            moby[0x20] = 0x10;
        }
    } else if (moby[0x20] == 0x10) {
        moby[0x20] = 8;
        if (moby[0x53] != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
    }
    return suck_cannon_store_result;
}

s32 cancel_moby_suction_and_save_state(unsigned char *moby) __asm__("FUN_L06_002ea7c8");

s32 cancel_moby_suction_and_save_state(unsigned char *moby) {
    u8  previous_state;
    s32 helper_result;
    helper_result = cancel_suck_cannon_suction(moby);
    previous_state = moby[0x20];
    if (previous_state != 0x10) {
        moby[0xBC] = previous_state;
        moby[0x20] = 0x10;
    }
    return helper_result;
}

/* Scores a candidate target and keeps it if it beats the best so far. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F4818), where it is exact; names translated to the US level program. */
void consider_moby_target_candidate(char  *moby,
                                    char  *current_target,
                                    char  *candidate_moby,
                                    int   *out,
                                    float *best,
                                    float  thresh) __asm__("FUN_L06_002f33e8");

void consider_moby_target_candidate(

    char *moby, char *current_target, char *candidate_moby, int *best_target_out, float *best, float thresh) {
    float candidate_distance = vector_distance_xy(moby + 0x10, candidate_moby + 0x10);
    if (!(thresh < candidate_distance)) {
        float ang = fast_arc_tan(MOBY(candidate_moby)->pos.x - MOBY(moby)->pos.x,
                                 MOBY(candidate_moby)->pos.y - MOBY(moby)->pos.y);
        float candidate_score =
            candidate_distance + fast_difference_between_rotations(MOBY(moby)->rot.z, ang) * 5.0f;
        if (candidate_moby == current_target) {
            candidate_score -= 2.0f;
        }
        if (candidate_score < *best) {
            *best_target_out = (int)candidate_moby;
            *best = candidate_score;
        }
    }
}

/* Picks the best target for moby among this frame's live class-0x359 mobys and the hero
 * (current is the target it has now), and stores it in *current. */
void select_hero_or_gadgebot_target(char *moby, char **current, f32 range) {
    Best  best;
    char *candidate_moby;
    s32   i;
    if (gadgebot_target_cache_frame_gp != level_frame_count) {
        gadgebot_target_cache_frame_gp = level_frame_count;
        cached_gadgebot_target_count_gp = 0;
        for (i = 0; (candidate_moby = D_L06_001AC180_2f34d8[i]) != 0; i++) {
            if (MOBY(candidate_moby)->oclass == 0x359 && (u8)candidate_moby[0x20] >= 2 &&
                (u8)candidate_moby[0x20] != 9) {
                s32 candidate_slot_index = cached_gadgebot_target_count_gp;
                cached_gadgebot_target_count_gp = candidate_slot_index + 1;
                cached_gadgebot_target_mobies[candidate_slot_index] = candidate_moby;
            }
        }
    }
    best.score = 999999.0f;
    best.moby = 0;
    for (i = 0; i < cached_gadgebot_target_count_gp; i++) {
        consider_moby_target_candidate(moby,
                                       *current,
                                       cached_gadgebot_target_mobies[i],
                                       (int *)&best.moby,
                                       &best.score,
                                       range);
    }
    consider_moby_target_candidate(
        moby, *current, (char *)hero.moby, (int *)&best.moby, &best.score, range);
    *current = best.moby;
}

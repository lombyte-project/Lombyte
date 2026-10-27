/* Level 14 moby classes: platform. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/vector.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/audio/voice_pool.h"

/* Finds the path segments nearest to the two target points and sums the segment lengths between them. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002D7668), where it is exact; names translated to the US level program. */

typedef struct {
    float v[4];
    int i;
    float t;
} PathPos_2D7668;

extern int *D_L14_001B0BB0[];
extern int D_L14_0015F70C;
extern int FUN_L00_0025df68(void *, void *, float *, int *, float *, int, float, float, float);
extern float D_L14_00161A38_gp __asm__("D_L14_00161A38") __attribute__((sda));

void initialize_racer_path_tracking(struct Moby *racer_moby) __asm__("FUN_L14_002d6358");

void initialize_racer_path_tracking(struct Moby *racer_moby) {
    char *moby_data = (char *)racer_moby->pvars;
    char *path = *(char **)(D_L14_0015F70C + *(int *)(moby_data + 0x60) * 32 + 0x10);
    char *tg;
    PathPos_2D7668 nearest_path_position;
    int k;
    float z;
    unsigned short second_point_segment;
    *(float *)(moby_data + 0x74) = D_L14_00161A38_gp;
    *(int *)(moby_data + 0x68) = 0;
    *(int *)(moby_data + 0x64) = 0;
    z = *(float *)(moby_data + 0x68);
    tg = (char *)D_L14_001B0BB0[*(int *)(moby_data + 0x88)] + 0x30;
    nearest_path_position.t = z;
    nearest_path_position.i = 0;
    FUN_L00_0025df68(path, tg, nearest_path_position.v, &nearest_path_position.i, &nearest_path_position.t, 0, 20.0f, 5.0f, z);
    *(short *)(moby_data + 0x8C) = nearest_path_position.i;
    nearest_path_position.t = z;
    nearest_path_position.i = 0;
    FUN_L00_0025df68(path, (char *)D_L14_001B0BB0[*(int *)(moby_data + 0x88)] + 0x20, nearest_path_position.v, &nearest_path_position.i, &nearest_path_position.t, 0,
                     20.0f, 5.0f, z);
    second_point_segment = nearest_path_position.i;
    *(short *)(moby_data + 0x8E) = second_point_segment;
    *(float *)(moby_data + 0x78) = z;
    for (k = (short)second_point_segment; k < *(short *)(moby_data + 0x8C); k++) {
        *(float *)(moby_data + 0x78) += *(float *)(path + k * 16 + 0x1C);
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002D77E0), where it is exact; names translated to the US level program. */

extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
extern int *D_L14_001B0BB0[];
extern int D_L14_0015F70C;
extern int tick_countdown_32(void *);
extern void lerp_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern char *D_L14_001B0F30_a[] __asm__("D_L14_001B0BB0");

int interpolate_racer_to_path_start(struct Moby *racer_moby) __asm__("FUN_L14_002d64d0");

int interpolate_racer_to_path_start(struct Moby *racer_moby) {
    char *moby_data = (char *)racer_moby->pvars;
    char *ent = *(char **)((char *)((*(int *)(moby_data + 0x60) << 5) + (int)D_L14_0015F70C) + 0x10);
    int ok;
    float remaining_frames;
    ok = tick_countdown_32(moby_data + 0x6C) != 0;
    remaining_frames = convert_integer_to_float(*(int *)(moby_data + 0x6C));
    lerp_vector(&racer_moby->pos, ent + 0x10, D_L14_001B0F30_a[*(int *)(moby_data + 0x84)] + 0x10,
                 remaining_frames * *(float *)(moby_data + 0x70));
    return ok;
}
/* Lines up the three listed racers on the hero's track at the start point and resets their race state. */
extern unsigned short *D_L14_001ABF40_6570[] __asm__("D_L14_001ABF40");
extern char *D_L14_0015FFD8_6570 __asm__("D_L14_0015FFD8");
extern char *D_L14_0015F70C_6570 __asm__("D_L14_0015F70C");
extern char *D_L14_001B0BB0_6570[] __asm__("D_L14_001B0BB0");
extern float D_L14_00161A38_gp __asm__("D_L14_00161A38") __attribute__((sda));
typedef struct { char p[0x48]; float f48; } M_6570;
typedef struct {
    char p0[0x80];
    float f80;
    float f84;
    char p1[0x560 - 0x88];
    char *f560;
    int f564;
    float f568;
} G_6570;
extern int FUN_L00_0025df68_c(void *, void *, void *, int *, float *, int, float, float, float) __asm__("FUN_L00_0025df68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void place_6570(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void FUN_L00_0025d808_6570(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");

void reset_racer_group_to_hero_path_position(struct Moby *group_anchor_moby) __asm__("FUN_L14_002d6570");

void reset_racer_group_to_hero_path_position(struct Moby *group_anchor_moby) {
    char *list[4];
    float near[4];
    float path_heading_delta[4];
    float hero_path_euler_rotation[4];
    int path_segment_index;
    float nearest_segment_distance;
    unsigned short *moby_index_cursor = D_L14_001ABF40_6570[group_anchor_moby->group];
    char **out;
    int i;
    float path_step_distance;
    if (moby_index_cursor == 0) {
        return;
    }
    out = list;
    do {
        char *o = D_L14_0015FFD8_6570 + ((*moby_index_cursor & 0x7FFF) << 8);
        char *od = *(char **)(o + 0x78);
        char *path = hero.rail_spline;
        if (path == *(char **)(D_L14_0015F70C_6570 + *(int *)(od + 0x60) * 32 + 0x10)) {
            char *tp = D_L14_001B0BB0_6570[*(int *)(od + 0x88)];
            nearest_segment_distance = 0.0f;
            FUN_L00_0025df68_c(path, tp + 0x10, near, &path_segment_index, &nearest_segment_distance, 0, 20.0f, 5.0f, 0.0f);
            subtract_vector_xyz(path_heading_delta, path + (path_segment_index * 16 + 0x40), path + (path_segment_index * 16 + 0x10));
            hero_path_euler_rotation[0] = 0.0f;
            hero_path_euler_rotation[1] = 0.0f;
            hero_path_euler_rotation[2] = fast_add_rotations(angle_atan2(path_heading_delta[0], path_heading_delta[1]), -1.5707964f);
            hero_path_euler_rotation[3] = 0.0f;
            hero.rail_segment = path_segment_index;
            hero.rail_seg_frac = 0.0f;
            place_6570(path + (path_segment_index * 16 + 0x10), hero_path_euler_rotation, 0x28, 0);
        }
        *out++ = o;
    } while ((short)*moby_index_cursor++ >= 0);
    for (i = 0; i < 3; i++) {
        char *o = list[i];
        char *od = *(char **)(o + 0x78);
        char *path = *(char **)(D_L14_0015F70C_6570 + *(int *)(od + 0x60) * 32 + 0x10);
        o[0x20] = 6;
        ((M_6570 *)o)->f48 = angle_atan2(hero.motion.pos.f[0] - group_anchor_moby->pos.x, hero.motion.pos.f[1] - group_anchor_moby->pos.y);
        path_step_distance = -D_L14_00161A38_gp;
        *(int *)(od + 0x64) = hero.rail_segment;
        *(float *)(od + 0x68) = hero.rail_seg_frac;
        FUN_L00_0025d808_6570(path, o + 0x10, od + 0x64, od + 0x68, 0, path_step_distance);
        clear_u64_value(od + 0x90);
        *(int *)(od + 0xF8) = i * 2;
        *(int *)(od + 0xFC) = i * 2 + 1;
        if (*(int *)(od + 0xF0) != -1) {
            release_point_light_slot(*(int *)(od + 0xF0));
            *(int *)(od + 0xF0) = -1;
        }
    }
}
/* Racer, racing: measures how far along its stretch of track it is (sets the catch-up speed), snaps to the
 * track point nearest the hero and runs along the track, springing its position towards it, and faces along the
 * track. While its gun is active (states 2-4) it swings the gun joint to aim at the track ahead, blending in and
 * out over the aim timers, then fires a ray (3) or shows the muzzle flash and its light (2, 4). Last it turns
 * its head towards the hero. */
typedef int u128_67e8 __attribute__((mode(TI)));
typedef struct { char pad00[0x64]; int seg; float frac; } RacerSeg_67e8;
typedef struct { float x, y, z, w; } V_67e8;
extern V_67e8 D_0013F3D0_67e8 __asm__("D_0013F3D0");
typedef struct {
    V_67e8 dir;                 /* 0x00 */
    struct Moby *self;          /* 0x10 */
    int flags;                  /* 0x14 */
    unsigned char b18;
    unsigned char b19;
    short oclass;               /* 0x1A */
    float f1C;
    int i20;
    int pad[3];
} Ray_67e8;
int D_L14_00161A3C = 0x8c;
#include "rnc/overlay/l14/gameplay/entities/platform_data/update_racer_path_and_weapon.h"
extern char D_L14_001809C0[];
extern char D_L14_0015F580_67e8[] __asm__("D_L14_0015F580") __attribute__((section(".sdata")));
extern void track_67e8(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");
extern void spring_67e8(void *, void *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern float atan2_67e8(float, float) __asm__("FUN_001f9e90");
extern float sub_rot_67e8(float, float) __asm__("FUN_001fa5c8");
extern int timer_67e8(void *) __asm__("FUN_001f9740");
extern int frames_67e8(int) __asm__("FUN_001f96f8");
extern float itof_67e8(int) __asm__("FUN_001fa6c0");
extern int ftoi_67e8(float) __asm__("FUN_001fa6d0");
extern void joint_matrix_67e8(struct Moby *, int, void *) __asm__("FUN_0020cca8");
extern void joint_pos_67e8(struct Moby *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void normalize_67e8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_67e8(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void sub_67e8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_67e8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_67e8(void *, void *, float) __asm__("FUN_001f9a68");
extern void invert_67e8(void *, void *) __asm__("FUN_001fa2d8");
extern void matmul_67e8(void *, void *, void *) __asm__("FUN_001fa378");
extern void to_quat_67e8(void *, void *) __asm__("FUN_00214260");
extern void slerp_67e8(void *, void *, void *, float) __asm__("FUN_001fa400");
extern void FUN_L00_001fff28(void *, int, float);
extern void register_beam_slot(struct Moby *, int, int) __asm__("FUN_L00_002e1f28");
extern int set_beam_origin_transform_and_endpoint(struct Moby *, void *, void *) __asm__("FUN_L00_002e2190");
extern int ray_67e8(void *, void *, int, struct Moby *, void *) __asm__("FUN_001efa68");
extern char *FUN_L00_00274948_c(void *, void *, int, struct Moby *) __asm__("FUN_L00_00274948");
extern int FUN_L00_0023e738_c(void *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");
extern float cos_67e8(float) __asm__("FUN_001f9dc8");
extern float sin_67e8(float) __asm__("FUN_001f9de0");

void update_racer_path_and_weapon(struct Moby *racer_moby) __asm__("FUN_L14_002d67e8");

void update_racer_path_and_weapon(struct Moby *racer_moby) {
    char *moby_data = (char *)racer_moby->pvars;
    char *path = *(char **)(D_L14_0015F70C + *(int *)(moby_data + 0x60) * 32 + 0x10);
    V_67e8 near;
    V_67e8 joint_transform_and_aim_vectors[4];
    V_67e8 ma[4];
    V_67e8 mb[4];
    V_67e8 mc[4];
    V_67e8 md[4];
    V_67e8 scl;
    V_67e8 rot;
    V_67e8 one;
    Ray_67e8 ray;
    int ti;
    float tf;
    float z;
    float yaw;
    float blend;
    float target_yaw_offset;

    if (*(int *)(moby_data + 0x64) >= *(short *)(moby_data + 0x8E)) {
        float sum = 0.0f;
        int k;
        for (k = *(short *)(moby_data + 0x8E); k < *(int *)(moby_data + 0x64); k++) {
            sum += *(float *)(path + k * 16 + 0x1C);
        }
        sum += *(float *)(moby_data + 0x68);
        sum /= *(float *)(moby_data + 0x78);
        if (sum > 1.0f) {
            sum = 1.0f;
        }
        *(float *)(moby_data + 0x74) = D_L14_00161A38_gp + (-3.0f - D_L14_00161A38_gp) * sum;
    }
    *(int *)(moby_data + 0x68) = 0;
    *(int *)(moby_data + 0x64) = 0;
    z = *(float *)(moby_data + 0x68);
    FUN_L00_0025df68(path, &D_0013F3D0_67e8, &near.x, (int *)(moby_data + 0x64), (float *)(moby_data + 0x68), 0, 20.0f, 5.0f, z);
    track_67e8(path, &near, moby_data + 0x64, moby_data + 0x68, 0, -*(float *)(moby_data + 0x74));
    spring_67e8(&racer_moby->pos.x, moby_data + 0x90, near.x, 0.01f, 0.2f, z);
    spring_67e8(&racer_moby->pos.y, moby_data + 0x94, near.y, 0.01f, 0.2f, z);
    spring_67e8(&racer_moby->pos.z, moby_data + 0x98, near.z, 0.01f, 0.2f, z);
    {
        int i = *(int *)(moby_data + 0x64);
        char *current_path_record = path + i * 16;
        char *lookahead_path_record = path + (i + 5) * 16;
        racer_moby->rot.z = atan2_67e8(*(float *)(lookahead_path_record + 0x10) - *(float *)(current_path_record + 0x10), *(float *)(lookahead_path_record + 0x14) - *(float *)(current_path_record + 0x14));
    }
    if (racer_moby->command == 5 && timer_67e8(moby_data + 0x1C8) != 0) {
        racer_moby->command = 1;
    }
    if (racer_moby->command == 1) {
        *(int *)(moby_data + 0x1C0) = frames_67e8(D_L14_00161A3C);
        *(int *)(moby_data + 0x1C4) = 0;
        racer_moby->command = 2;
        joint_matrix_67e8(racer_moby, 4, joint_transform_and_aim_vectors);
        joint_matrix_67e8(racer_moby, 5, ma);
        normalize_67e8(&ma[0], &ma[0], 1.0f);
        normalize_67e8(&ma[1], &ma[1], 1.0f);
        normalize_67e8(&ma[2], &ma[2], 1.0f);
        invert_67e8(mb, ma);
        mb[3].w = 1.0f;
        mb[3].x = mb[3].y = mb[3].z = 0.0f;
        matmul_67e8(mc, mb, joint_transform_and_aim_vectors);
        to_quat_67e8(moby_data + 0x1D0, mc);
    }
    yaw = racer_moby->rot.z;
    if ((unsigned char)(racer_moby->command - 2) < 3) {
        float turn;
        float aim_timer_fraction;
        float k;
        joint_matrix_67e8(racer_moby, 0, moby_data + 0xA0);
        blend = 1.0f;
        aim_timer_fraction = itof_67e8(*(int *)(moby_data + 0x1C0)) / itof_67e8(frames_67e8(D_L14_00161A3C));
        k = weapon_aim_final_path_step;
        ti = ((RacerSeg_67e8 *)moby_data)->seg;
        tf = ((RacerSeg_67e8 *)moby_data)->frac;
        track_67e8(path, &joint_transform_and_aim_vectors[0], &ti, &tf, 0, k + (1.0f - k) * aim_timer_fraction);
        qcopy(moby_data + 0x1E0, &joint_transform_and_aim_vectors[0]);
        sub_67e8(&joint_transform_and_aim_vectors[1], &joint_transform_and_aim_vectors[0], &racer_moby->pos);
        yaw = atan2_67e8(joint_transform_and_aim_vectors[1].x, joint_transform_and_aim_vectors[1].y);
        turn = sub_rot_67e8(yaw, racer_moby->rot.z);
        if (turn > 1.2217305f) {
            turn = 1.2217305f;
        } else if (turn < -1.2217305f) {
            turn = -1.2217305f;
        }
        (*(int *)(moby_data + 0x1C4))++;
        if (frames_67e8(30) < *(int *)(moby_data + 0x1C4)) {
            if (racer_moby->command == 4) {
                racer_moby->command = 0;
            } else {
                unsigned char firing_command = 3;
                register_beam_slot(racer_moby, firing_command, 1);
                racer_moby->command = firing_command;
            }
            *(int *)(moby_data + 0x1C4) = frames_67e8(30);
        }
        if (racer_moby->command == 2 || racer_moby->command == 4) {
            blend = itof_67e8(*(int *)(moby_data + 0x1C4)) / itof_67e8(frames_67e8(30));
            if (racer_moby->command == 4) {
                blend = 1.0f - blend;
            }
            turn = (turn - 0.0f) * blend + 0.0f;
        }
        FUN_L00_001fff28(moby_data + 0x150, 2, turn);
        joint_pos_67e8(racer_moby, 4, &joint_transform_and_aim_vectors[2]);
        *(u128_67e8 *)&joint_transform_and_aim_vectors[3] = 0;
        joint_transform_and_aim_vectors[3].z = 1.0f;
        joint_transform_and_aim_vectors[3].w = 1.0f;
        normalize_67e8(&ma[1], &joint_transform_and_aim_vectors[1], 1.0f);
        cross_67e8(&ma[2], &ma[1], &joint_transform_and_aim_vectors[3]);
        normalize_67e8(&ma[2], &ma[2], 1.0f);
        cross_67e8(&ma[0], &ma[2], &ma[1]);
        ma[3].x = ma[3].y = ma[3].z = 0.0f;
        ma[3].w = 1.0f;
        joint_matrix_67e8(racer_moby, 5, mb);
        normalize_67e8(&mb[0], &mb[0], 1.0f);
        normalize_67e8(&mb[1], &mb[1], 1.0f);
        normalize_67e8(&mb[2], &mb[2], 1.0f);
        invert_67e8(mc, mb);
        mc[3].x = mc[3].y = mc[3].z = 0.0f;
        mc[3].w = 1.0f;
        matmul_67e8(md, mc, ma);
        *(u128_67e8 *)&scl = 0;
        scl.w = 1.0f;
        scl.y = 512.0f / racer_moby->scale;
        to_quat_67e8(&rot, md);
        if (racer_moby->command == 2 || racer_moby->command == 4) {
            slerp_67e8(moby_data + 0x190, moby_data + 0x1D0, &rot, blend);
        } else {
            qcopy(moby_data + 0x190, &rot);
        }
        *(char *)(moby_data + 0x183) = 1;
        *(float *)(moby_data + 0x18C) = 1.0f;
        *(u128_67e8 *)&one = 0;
        one.x = 1.0f;
        one.y = 1.0f;
        one.z = 1.0f;
        one.w = 1.0f;
        qcopy(moby_data + 0x1A0, &one);
        qcopy(moby_data + 0x1B0, &scl);
        if (timer_67e8(moby_data + 0x1C0) != 0) {
            racer_moby->command = 4;
            *(int *)(moby_data + 0x1C4) = 0;
        }
        if (racer_moby->command == 3) {
            if (set_beam_origin_transform_and_endpoint(racer_moby, moby_data + 0xA0, moby_data + 0x1E0) == 0) {
                register_beam_slot(racer_moby, 3, 1);
            }
            ray.flags = 0x10001;
            ray.self = racer_moby;
            ray.i20 = 1;
            ray.f1C = 1.0f;
            ray.dir.x = cos_67e8(racer_moby->rot.z);
            ray.dir.y = sin_67e8(racer_moby->rot.z);
            ray.dir.z = 1.0f;
            ray.dir.w = 5627.925f;
            ray.b19 = 1;
            ray.oclass = racer_moby->oclass;
            ray.b18 = 0;
            ray_67e8(moby_data + 0xD0, moby_data + 0x1E0, 9, racer_moby, &ray);
        } else if (racer_moby->command == 2 || racer_moby->command == 4) {
            char *muzzle_flash_particle;
            float muzzle_light_position_w;
            char *muzzle = moby_data + 0xD0;
            scale_67e8(&ray, moby_data + 0xA0, muzzle_flash_joint_axis_offset);
            add_67e8(&ray, &ray, muzzle);
            muzzle_flash_particle = FUN_L00_00274948_c(&ray, D_L14_0015F580_67e8, 0x7F, racer_moby);
            if (muzzle_flash_particle != 0) {
                char *muzzle_flash_particle_state = muzzle_flash_particle + 0x20;
                int lifetime_frames;
                *(float *)(muzzle_flash_particle + 0xC) = (muzzle_flash_particle_full_blend_endpoint - 1000.0f) * blend + 1000.0f;
                *(short *)(muzzle_flash_particle_state + 0x14) = ftoi_67e8(blend * 111.0f + 16.0f);
                *(short *)(muzzle_flash_particle_state + 0x16) = 1;
                lifetime_frames = frames_67e8(4);
                *(short *)(muzzle_flash_particle + 0xA) = lifetime_frames;
                *(float *)(muzzle_flash_particle_state + 0x10) = 1.0f / itof_67e8((short)lifetime_frames);
                *(int *)(muzzle_flash_particle_state + 0x18) = 0x7F7F7F;
            }
            qcopy(&ray.self, muzzle);
            ((float *)&ray.self)[2] -= 0.5f;
            muzzle_light_position_w = (muzzle_light_full_blend_position_w - 0.1f) * blend + 0.1f;
            if (*(int *)(moby_data + 0xF0) == -1) {
                *(int *)(moby_data + 0xF0) = FUN_L00_0023e738_c(&ray.self, muzzle_light_position_w, 0.0f, muzzle_light_color_component_x, muzzle_light_color_component_y, muzzle_light_color_component_z);
            }
            if (*(int *)(moby_data + 0xF0) >= 0) {
                float *muzzle_light_entry = (float *)(D_L14_001809C0 + (*(int *)(moby_data + 0xF0) << 5));
                qcopy(muzzle_light_entry + 4, &ray.self);
                muzzle_light_entry[7] = muzzle_light_position_w;
                muzzle_light_entry[0] = muzzle_light_color_component_x;
                muzzle_light_entry[1] = muzzle_light_color_component_y;
                muzzle_light_entry[2] = muzzle_light_color_component_z;
            }
        } else if (*(int *)(moby_data + 0xF0) != -1) {
            release_point_light_slot(*(int *)(moby_data + 0xF0));
            *(int *)(moby_data + 0xF0) = -1;
        }
    } else if (*(int *)(moby_data + 0xF0) != -1) {
        release_point_light_slot(*(int *)(moby_data + 0xF0));
        *(int *)(moby_data + 0xF0) = -1;
    }
    sub_67e8(&joint_transform_and_aim_vectors[0], &D_0013F3D0_67e8, &racer_moby->pos);
    target_yaw_offset = sub_rot_67e8(atan2_67e8(joint_transform_and_aim_vectors[0].x, joint_transform_and_aim_vectors[0].y), yaw);
    if (target_yaw_offset > 1.2217305f) {
        target_yaw_offset = 1.2217305f;
    } else if (target_yaw_offset < -1.2217305f) {
        target_yaw_offset = -1.2217305f;
    }
    FUN_L00_001fff28(moby_data + 0x110, 2, target_yaw_offset);
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002D84A8), where it is exact; names translated to the US level program. */

extern int D_L14_0015F70C;
extern void sample_path(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");

extern float D_L14_00161A30 __attribute__((sda));
void advance_moby_along_path(struct Moby *moby) __asm__("FUN_L14_002d7198");

void advance_moby_along_path(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int path_index = *(int *)(data + 0x60);
    sample_path(*(int *)((path_index << 5) + D_L14_0015F70C + 0x10), &moby->pos, data + 0x64,
                     data + 0x68, 0, D_L14_00161A30 * frame_time);
}
/* Draws the racer's two glow quads, at its lamp (0xE0) and its tail (0x200), turned to face the camera. */
typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_2d71f0;

float D_L14_001DE540[4][4] = {
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
};
extern void scale_g(void *, void *, float) __asm__("FUN_001f9a68");
extern void sub_g(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_g(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_g(void *, void *, void *) __asm__("FUN_001f9ad8");
extern unsigned long tex_g(s32) __asm__("FUN_001f44b8");
extern void xform_g(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_g(void *, int, int) __asm__("FUN_001f7d30");

void draw_racer_glow_quads(struct Moby *m) __asm__("FUN_L14_002d71f0");

void draw_racer_glow_quads(struct Moby *m) {
    char *d = (char *)m->pvars;
    Pk_2d71f0 pk;
    float glow_quad_transform[4][4];
    int i;
    int j;
    qcopy(glow_quad_transform[3], d + 0xE0);
    glow_quad_transform[3][3] = 1.0f;
    sub_g(glow_quad_transform[0], D_L14_001674C0, glow_quad_transform[3]);
    normalize_g(glow_quad_transform[0], glow_quad_transform[0], 1.0f);
    cross_g(glow_quad_transform[1], glow_quad_transform[0], &hero.gravity_dir);
    normalize_g(glow_quad_transform[1], glow_quad_transform[1], -1.0f);
    cross_g(glow_quad_transform[2], glow_quad_transform[1], glow_quad_transform[0]);
    pk.b = tex_g(0xB);
    pk.d = 0x8000000048ULL;
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 5;
    pk.col[3] = 0x80FFFFFF;
    pk.col[2] = 0x80FFFFFF;
    pk.col[1] = 0x80FFFFFF;
    pk.col[0] = 0x80FFFFFF;
    pk.uv[0] = 0.0f;
    pk.uv[1] = 0.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 1.0f;
    pk.uv[7] = 1.0f;
    for (i = 0; i < 4; i++) {
        scale_g(pk.m[i], D_L14_001DE540[i], 0.4f);
        xform_g(pk.m[i], pk.m[i], glow_quad_transform);
    }
    draw_g(&pk, 0, 0);
    qcopy(glow_quad_transform[3], d + 0x200);
    sub_g(glow_quad_transform[0], D_L14_001674C0, glow_quad_transform[3]);
    normalize_g(glow_quad_transform[0], glow_quad_transform[0], 1.0f);
    cross_g(glow_quad_transform[1], glow_quad_transform[0], &hero.gravity_dir);
    normalize_g(glow_quad_transform[1], glow_quad_transform[1], -1.0f);
    cross_g(glow_quad_transform[2], glow_quad_transform[1], glow_quad_transform[0]);
    pk.col[3] = 0x202020FF;
    pk.col[2] = 0x202020FF;
    pk.col[1] = 0x202020FF;
    pk.col[0] = 0x202020FF;
    for (j = 0; j < 4; j++) {
        scale_g(pk.m[j], D_L14_001DE540[j], 0.2f);
        xform_g(pk.m[j], pk.m[j], glow_quad_transform);
    }
    draw_g(&pk, 0, 0);
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Puffs smoke from the moby's exhaust joint: keeps its light in place while active, then spawns four puffs. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002D87A0), where it is exact; names translated to the US level program. */

extern char *spawn_mode_controlled_rotating_alpha_fade_particle(void *, void *, int, void *) __asm__("FUN_L00_00274948");
extern char D_L14_001809C0[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_L14_0015F580[] __attribute__((section(".sdata")));
extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern int allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
#include "rnc/overlay/l14/gameplay/entities/platform_globals.h"
extern float joint_light_position_w_gp __asm__("D_L14_00161A60") __attribute__((sda));
extern void random_spherical_offset(void *, float, float) __asm__("FUN_00213358");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void draw_racer_glow_quads(struct Moby *) __asm__("FUN_L14_002d71f0");
extern void add_vector_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
void enqueue_callback_list_1(int arg0, int arg1) __asm__("FUN_001f4600");

void update_moby_joint_visual_effects(struct Moby *moby) __asm__("FUN_L14_002d7490");

void update_moby_joint_visual_effects(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    float joint_position[4];
    float glow_position_offset[4];
    float random_particle_vector[4];
    char *particle;
    int i;
    moby_point_position(moby, 1, joint_position);
    subtract_vector_xyz(glow_position_offset, joint_position, D_L14_001674C0);
    normalize_vector_xyz(glow_position_offset, glow_position_offset, -0.5f);
    add_vector_xyz(moby_data + 0xE0, joint_position, glow_position_offset);
    if ((unsigned int)(moby->state - 3) < 3) {
        joint_position[2] += 1.0f;
        if (*(int *)(moby_data + 0xF0) == -1) {
            *(int *)(moby_data + 0xF0) = allocate_colored_effect_slot(joint_position, joint_light_position_w_gp, 0.0f, 0.5f, 0.5f, 1.0f);
        }
        if (*(int *)(moby_data + 0xF0) >= 0) {
            char *e = D_L14_001809C0 + *(int *)(moby_data + 0xF0) * 32;
            qcopy(e + 0x10, joint_position);
            *(float *)(e + 0x1C) = joint_light_position_w_gp;
        }
        joint_position[2] -= 1.0f;
    }
    random_spherical_offset(random_particle_vector, 0.005f, 0.03f);
    particle = spawn_mode_controlled_rotating_alpha_fade_particle(joint_position, random_particle_vector, 0x7F, moby);
    if (particle != 0) {
        *(float *)(particle + 0xC) = random_float_between(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        char *e;
        int lifetime_frames;
        particle = spawn_mode_controlled_rotating_alpha_fade_particle(joint_position, D_L14_0015F580, 0x7F, moby);
        if (particle == 0)
            continue;
        e = particle + 0x20;
        if (i == 2 && random_integer_below(8) == 0) {
            *(float *)(particle + 0xC) = 180000.0f;
        } else {
            *(float *)(particle + 0xC) = random_float_between(80000.0f, 120000.0f);
        }
        lifetime_frames = scale_game_frames(2);
        *(short *)(particle + 0xA) = lifetime_frames;
        *(float *)(e + 0x10) = 1.0f / convert_integer_to_float((short)lifetime_frames);
        *(int *)(e + 0x18) = 0x7F7F7F;
        *(short *)(e + 0x16) = 3;
    }
    moby_point_position(moby, 6, moby_data + 0x200);
    enqueue_callback_list_1(draw_racer_glow_quads, moby);
}
#include "qzero.h"
extern void FUN_L14_002de2b8_call(struct Moby *, f32, f32) __asm__("FUN_L14_002de2b8");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
#include "rnc/overlay/l14/gameplay/entities/platform_data/update_tilting_platform.h"

void update_tilting_platform(struct Moby *moby) __asm__("FUN_L14_002de1f8");

void update_tilting_platform(struct Moby *moby) __asm__("FUN_L14_002de1f8");
void update_tilting_platform(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    void *rotation;
    u128 old_rotation;
    qcopy(&old_rotation, &moby->rot);

    if (moby->state == 0) {
        moby->state = 1;
        moby->command = 0;
        moby->flags |= 0x100;
        moby->pos.z += 0.35f;
        *(int *)(data + 0x5C) = 1;
        *(int *)(data + 0x70) = 0;
        qzero(data + 0x60);
        qzero(&moby->rot);
        rotation = &moby->rot;
    } else {
        rotation = &moby->rot;
    }
    FUN_L14_002de2b8_call(moby, unused_platform_tilt_argument, 8.0f);
    update_entity_matrix(data + 0x20, D_L14_0015F580, &old_rotation, rotation);
}
/* Ported from rac1-decomp src/overlays/l14_oltanis/vendor_002ACCC0.c (func_L14_002DF6B8) */
#include "eetypes.h"


typedef int u128_s07 __attribute__((mode(TI)));

extern f32 fast_arc_sin(f32) __asm__("func_001F9DF8");
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern int allocate_voice_slot(void *, int, int, int, int) __asm__("FUN_0022d7f0");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
#include "rnc/overlay/l14/gameplay/entities/platform_data/update_platform_tilt_basis.h"
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013F3D0[];

/* The caller passes two floats in f12/f13; this routine never reads them. */
void update_platform_tilt_basis(char *moby, f32 unused_arg1, f32 unused_arg2) __asm__("FUN_L14_002de2b8");

void update_platform_tilt_basis(char *moby, f32 unused_arg1, f32 unused_arg2) {
    char *data = *(char **)(moby + 0x78);
    char *hero_state = (char *)((char *)((char *)&hero));
    float target_up_vector[4];
    float up[4];
    float direction_scratch[4];
    float ax[4];
    float direction_length;
    float dt;
    float tilt_angle;
    float max_tilt_angle;

    *(u128_s07 *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;

    if ((*(char **)(hero_state + 0x2FC) == moby && *(short *)(hero_state + 0x30E) == 0) ||
        (*(char **)(hero_state + 0x4F8) == moby && *(int *)(hero_state + 0x208C) == 3)) {
        char *hero_data = (char *)((char *)((char *)&hero));
        *(char *)(moby + 0xBC) = 1;
        if (absolute_float(*(float *)(hero_data + 0x80) - *(float *)(moby + 0x10)) < 0.1f &&
            absolute_float(*(float *)(hero_data + 0x84) - *(float *)(moby + 0x14)) < 0.1f) {
            qcopy(target_up_vector, up);
        } else {
            subtract_vector_xyz(direction_scratch, (char *)((char *)&D_0013F3D0), moby + 0x10);
            direction_scratch[2] = direction_scratch[2] + platform_tilt_direction_z_offset;
            direction_length = vector_length_xyz(direction_scratch);
            if (direction_length == 0.0f) {
                qcopy(target_up_vector, up);
            } else {
                dt = dot_vectors_xyz(up, direction_scratch);
                tilt_angle = 1.5707963705f - fast_arc_sin(dt / direction_length);
                max_tilt_angle = platform_max_tilt_degrees * 0.0174532924f;
                if (max_tilt_angle < tilt_angle) {
                    tilt_angle = max_tilt_angle;
                }
                cross_vectors_xyz(ax, direction_scratch, up);
                build_look_at_matrix(target_up_vector, up, ax, tilt_angle);
                normalize_vector_xyz(target_up_vector, target_up_vector, 1.0f);
            }
        }
    } else {
        qcopy(target_up_vector, up);
        *(char *)(moby + 0xBC) = 0;
    }

    if (fast_dec_timer(data + 0x70)) {
        if (0.1f < absolute_float(*(float *)(moby + 0xE0) - target_up_vector[0]) ||
            0.1f < absolute_float(*(float *)(moby + 0xE4) - target_up_vector[1]) ||
            0.1f < absolute_float(*(float *)(moby + 0xE8) - target_up_vector[2])) {
            *(int *)(data + 0x70) = scale_game_frames_alt(60);
            *(float *)(moby + 0x18) = *(float *)(moby + 0x18) + 2.0f;
            allocate_voice_slot(*(void **)(*(char **)(moby + 0x24) + 0x28), 8, (int)moby, (int)(moby + 0x10), 0x400);
            *(float *)(moby + 0x18) = *(float *)(moby + 0x18) - 2.0f;
        }
    }

    vendor_spring((float *)(moby + 0xE0), (float *)(data + 0x60), target_up_vector[0], 0.005f, 0.2f, 0.0f);
    vendor_spring((float *)(moby + 0xE4), (float *)(data + 0x64), target_up_vector[1], 0.005f, 0.2f, 0.0f);
    vendor_spring((float *)(moby + 0xE8), (float *)(data + 0x68), target_up_vector[2], 0.005f, 0.2f, 0.0f);

    normalize_vector_xyz(moby + 0xE0, moby + 0xE0, 1.0f);
    *(u128_s07 *)direction_scratch = 0;
    direction_scratch[0] = 1.0f;
    direction_scratch[3] = 1.0f;
    cross_vectors_xyz(moby + 0xD0, direction_scratch, moby + 0xE0);
    normalize_vector_xyz(moby + 0xD0, moby + 0xD0, 1.0f);
    cross_vectors_xyz(moby + 0xC0, moby + 0xE0, moby + 0xD0);
    *(float *)(moby + 0xCC) = 0.0f;
    *(float *)(moby + 0xDC) = 0.0f;
    *(float *)(moby + 0xEC) = 0.0f;
}
#include "sda.h"

/* Updates a gem-lock sentry: picks up a sound slot and fires when the player is near. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002E0228), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x10];
    char pos[0x10];
    unsigned char b20;
    char pad21[0xB];
    float f2C;
    unsigned char b30;
    unsigned char b31;
    short s32;
    unsigned short h34;
    char pad36[0x12];
    float f48;
    char pad4C[0x2C];
    Child *child;
    char pad7C[0x14];
    int f90;
} Obj;

extern char D_0013E633[] __asm__("D_0013F3D0");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern int D_L14_0015F7EC __asm__("D_L14_0015F70C");
extern int fast_bsphere_check(char *, float) __asm__("FUN_001fa728");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int trooper_play_sound(int i, int a1, int v, int k) __asm__("FUN_L00_0028dc90");
extern void initialize_ribbon_effect_state(char *) __asm__("FUN_L14_002df080");
extern void animate_ribbon_effect_paths(char *) __asm__("FUN_L14_002df138");
extern void register_hero_hit_in_clip_volume(char *) __asm__("FUN_L14_002dfc58");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void update_ribbon_effect_moby(struct Moby *moby) __asm__("FUN_L14_002dee28");

void update_ribbon_effect_moby(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_UPDATE = 1
    };
    char *data = (char *)moby->pvars;
    struct Hero *x;
    float forward_point[4];
    float bounding_sphere[4];
    int voice_slot_index;
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE:
        if (*(int *)(data + 0x1F4) == -1 || *(int *)(data + 0x1F8) == -1 ||
            *(int *)(data + 0x1FC) == -1 || *(int *)(data + 0x200) < 0) {
            mark_moby_for_removal(moby);
            return;
        }
        initialize_ribbon_effect_state(moby);
        moby->state = 1;
        *(int *)(data + 0x204) = -1;
        break;
    case MOBY_STATE_UPDATE:
        x = &hero;
        if (x->state.control_mode != 0xF ||
            (x->rail_spline !=
                 *(int *)((*(int *)(data + 0x1F4) << 5) + D_L14_0015F7EC + 0x10) &&
             x->rail_spline !=
                 *(int *)((*(int *)(data + 0x1F8) << 5) + D_L14_0015F7EC + 0x10) &&
             x->rail_spline !=
                 *(int *)((*(int *)(data + 0x1FC) << 5) + D_L14_0015F7EC + 0x10))) {
            if (16.0f < absolute_float(x->motion.pos.f[0] - moby->pos.x))
                goto end;
            if (16.0f < absolute_float(x->motion.pos.f[1] - moby->pos.y))
                goto end;
        }
        if (voice_is_playing(moby, *(int *)(data + 0x204)) == 0) {
            *(int *)(data + 0x204) = trooper_play_sound(6, 4, (int)moby, 0xD3);
        }
        register_hero_hit_in_clip_volume(moby);
        forward_point[0] = fast_cos(moby->rot.z) * 3.125f;
        forward_point[1] = fast_sin(moby->rot.z) * 3.125f;
        forward_point[2] = 0.0f;
        add_vector_xyz(forward_point, &moby->pos, forward_point);
        qcopy(bounding_sphere, forward_point);
        bounding_sphere[3] = 4.5f;
        if (fast_bsphere_check((char *)bounding_sphere, 64.0f) >= 0) {
            animate_ribbon_effect_paths(moby);
        }
        return;
    }
end:
    voice_slot_index = *(int *)(data + 0x204);
    if (voice_slot_index >= 0) {
        if (voice_slot_index != -1) {
            char *voice_slot_entry = D_0013E633 - 0xE80 + voice_slot_index * 0x70;
            if (*(char **)(voice_slot_entry + 0x88) == moby && *(unsigned char *)(voice_slot_entry + 0x74) != 0) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(int *)(data + 0x204) = -1;
    }
}

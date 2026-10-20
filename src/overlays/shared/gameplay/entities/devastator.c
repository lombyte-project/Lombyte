/* Shared moby classes: devastator, visibomb, blaster, visibomb_missile, taunter, pyrocitor, tesla_claw. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/gameplay/hero.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/level_labels.h"

/* Devastator (class 157) update: tracks the aimed target, draws the reticle, picks a target and fires,
   keeps its usage statistics and moves the muzzle light. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B33E8), where it is exact; names translated to the US level program. */
#include "rnc/gameplay/entities/moby.h"
#include "qcopy.h"
#include "rnc/gameplay/state/usage_stats.h"
typedef unsigned int Quad_2b2100 __attribute__((mode(TI)));

typedef union { struct { float x, y, z, w; } f; Quad_2b2100 q; } Vec_2b2100 __attribute__((aligned(16)));

typedef struct { Vec_2b2100 row[3]; } Matrix3_2b2100;

typedef struct { Vec_2b2100 row[4]; } Matrix4_2b2100;

typedef struct Class_2b2100 { char pad0[0x46]; short category; } Class_2b2100;

typedef struct Devastator_2b2100 {
    int cooldown;
    char pad4[0xC];
    struct Moby *target;
    int target_timer;
    float target_height;
    short light;
    short glow;
} Devastator_2b2100;

typedef struct Input_2b2100 { char pad0[0x1A0]; int held; } Input_2b2100;

typedef struct ItemVariant_2b2100 { unsigned char pad[0xB]; unsigned char level; } ItemVariant_2b2100;

typedef struct UsageStat_2b2100 { char pad0[0x58]; unsigned short uses; unsigned short longest; int levels; } UsageStat_2b2100;

typedef struct Hit_2b2100 { char pad0[0x18]; struct Moby *moby; char pad1C[4]; Vec_2b2100 pos; } Hit_2b2100;

typedef struct AimInfo_2b2100 { char pad0[0xA]; unsigned char radius; char padB[5]; float height; } AimInfo_2b2100;

typedef struct Light_2b2100 { float x, y, z, b; Vec_2b2100 pos; } Light_2b2100;

extern Input_2b2100 pad_input_2b2100 __asm__("D_0013C940");
extern ItemVariant_2b2100 item_variant_2b2100 __asm__("D_0013E520");
extern char usage_stats_2b2100[] __asm__("D_00141680");
extern char camera_2b2100[] __asm__("D_L00_00166C80");
extern char view_origin_2b2100[] __asm__("D_L00_00166DC0");
extern char collision_hit_2b2100[] __asm__("D_L00_00173E40");
extern float collision_hit_z_2b2100 __asm__("D_L00_00173E68") __attribute__((section(".data")));
extern struct Moby *D_L00_001ABA00_2b2100[] __asm__("D_L00_001ABA00");
extern Light_2b2100 lights_2b2100[] __asm__("D_L00_001802C0");
extern int D_L00_0015F5CC_2b2100 __asm__("D_L00_0015F5CC");
extern float frame_time_2b2100 __asm__("D_0015ED6C");
extern int D_0015EEA4;
extern int level_index_2b2100 __asm__("D_0015ED84");
extern void FUN_L00_002b1af0_2b2100(struct Moby *) __asm__("FUN_L00_002b1af0");
extern void copy_matrix3x4_2b2100(void *, void *) __asm__("FUN_001fa2b8");
extern void FUN_001fa2d8_2b2100(void *, void *) __asm__("FUN_001fa2d8");
extern int tick_countdown_32_2b2100(int *) __asm__("FUN_001f9740");
extern void hero_set_state_2b2100(int, int) __asm__("FUN_L00_002223f8");
extern void transform_vector_2b2100(void *, void *, void *) __asm__("FUN_001f9d20");
extern void transform_vector_by_basis_2b2100(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_vector_xyz_2b2100(void *, void *, void *) __asm__("FUN_001f9a10");
extern int collision_segment_hits_2b2100(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int scale_game_frames_2b2100(int) __asm__("FUN_001f96f8");
extern float convert_integer_to_float_2b2100(int) __asm__("FUN_001fa6c0");
extern float FUN_001f96b0_2b2100(float) __asm__("FUN_001f96b0");
extern float fast_normalize_angle_2b2100(float) __asm__("FUN_001fa610");
extern void FUN_L00_001edf60_2b2100(int, int, float, void *, float, int, int, float, int) __asm__("FUN_L00_001edf60");
extern int FUN_L00_00233e98_2b2100(int) __asm__("FUN_L00_00233e98");
extern float distance_xyz_2b2100(void *, void *) __asm__("FUN_001f9b48");
extern float absolute_float_2b2100(float) __asm__("FUN_001f99c0");
extern void blend_moby_animation_2b2100(void *, int, int, int) __asm__("FUN_00212f90");
extern int FUN_L00_00233db8_2b2100(int, int) __asm__("FUN_L00_00233db8");
extern float atan2_fast_2b2100(float, float) __asm__("FUN_001f9e90");
extern float vector_length_xy_2b2100(void *) __asm__("FUN_001f9b20");
extern void FUN_001f99f8_2b2100(void *) __asm__("FUN_001f99f8");
extern void FUN_L00_0024f7c8_2b2100(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern AimInfo_2b2100 *get_moby_aim_info_2b2100(void *) __asm__("FUN_002141f8");
extern float distance_xy_2b2100(void *, void *) __asm__("FUN_001f9b80");
extern float fast_difference_between_rotations_2b2100(float, float) __asm__("FUN_001fa688");
extern void build_spherical_offset_2b2100(void *, float, float, float) __asm__("FUN_00214db0");
extern float approximate_arcsine_2b2100(float) __asm__("FUN_001f9df8");
extern int FUN_L00_0025f878_2b2100(void *) __asm__("FUN_L00_0025f878");
extern int allocate_voice_for_target_entry_2b2100(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_002b1de0_2b2100(void *) __asm__("FUN_L00_002b1de0");
extern void *FUN_L00_002afc70_2b2100(void *, void *, void *, float, float, float, float, float) __asm__("FUN_L00_002afc70");
extern float FUN_L00_002aff50_2b2100(int) __asm__("FUN_L00_002aff50");
extern void FUN_L00_002323b8_2b2100(int, int, float) __asm__("FUN_L00_002323b8");
extern void FUN_L00_0020e4c8_2b2100(void) __asm__("FUN_L00_0020e4c8");
extern float fast_cos_2b2100(float) __asm__("FUN_001f9dc8");
extern float fast_sin_2b2100(float) __asm__("FUN_001f9de0");
extern int FUN_L00_0023e738_2b2100(void *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void FUN_L00_0023e838_2b2100(int) __asm__("FUN_L00_0023e838");
extern int FUN_001f9770_2b2100(short *) __asm__("FUN_001f9770");

extern float D_L00_0017ACF4_lui __asm__("D_L00_0017ACF4") __attribute__((section(".data")));
void devastator_update(struct Moby *m) __asm__("FUN_L00_002b2100");
void devastator_update(struct Moby *m) {
    Matrix3_2b2100 rotation;
    Matrix4_2b2100 inverse;
    Vec_2b2100 velocity;
    Vec_2b2100 launch;
    Vec_2b2100 point;
    Vec_2b2100 probe;
    Devastator_2b2100 *data = (Devastator_2b2100 *)m->pvars;
    struct Moby *best;
    struct Moby *o;
    AimInfo_2b2100 *r;
    UsageStat_2b2100 *stats;
    Hit_2b2100 *hit;
    float height, limit, yaw, pitch, yaw0, pitch0;
    float size;
    float a, b, c;
    int i;

    FUN_L00_002b1af0_2b2100(m);
    height = 0.75f;
    limit = 10000.0f;
    if ((unsigned char)m->state == 0) {
        m->state = 1;
        data->light = -1;
        data->glow = 0;
    }
    copy_matrix3x4_2b2100(&rotation, camera_2b2100);
    FUN_001fa2d8_2b2100(&inverse, &rotation);
    tick_countdown_32_2b2100(&data->cooldown);
    if (tick_countdown_32_2b2100(&data->target_timer) ||
        (data->target && ((unsigned char)data->target->state & 0x80))) data->target = 0;
    if (hero.state.current == 1) hero_set_state_2b2100(30, 1);
    if (hero.state.current == 30 && (hero.moby->flags & 1) && !hero.unk20AC) {
        velocity.q = 0;
        velocity.f.z = 70.0f;
        transform_vector_2b2100(&velocity, &velocity, &inverse);
        add_vector_xyz_2b2100(&launch, view_origin_2b2100, &velocity);
        if (collision_segment_hits_2b2100(view_origin_2b2100, &launch, 0, (int)m, 0)) {
            hit = (Hit_2b2100 *)collision_hit_2b2100;
            if (hit->moby && (hit->moby->flags & 0x1000)) {
                data->target = hit->moby;
                data->target_timer = scale_game_frames_2b2100(20);
                data->target_height = hit->pos.f.z - data->target->pos.z;
            }
        }
        FUN_L00_001edf60_2b2100((int)m, 0xFF180A65, 1.0f,
            0, fast_normalize_angle_2b2100(convert_integer_to_float_2b2100(D_L00_0015F5CC_2b2100 % scale_game_frames_2b2100(240)) * 360.0f / FUN_001f96b0_2b2100(240.0f) * 0.017453292f),
            0x27, -1, 90.0f, 4);
        if (data->target && !data->cooldown && FUN_L00_00233e98_2b2100(-1)) {
            float d = distance_xyz_2b2100(view_origin_2b2100, &data->target->pos) / 100.0f;
            qcopy(&point, &data->target->pos);
            point.f.z += data->target_height;
            size = absolute_float_2b2100(0.9f - d * 0.57f);
            FUN_L00_001edf60_2b2100((int)m, 0xFF0FFF0F, size,
                &point, fast_normalize_angle_2b2100(convert_integer_to_float_2b2100(D_L00_0015F5CC_2b2100 % scale_game_frames_2b2100(240)) * -360.0f / FUN_001f96b0_2b2100(240.0f) * 0.017453292f),
                0x27, -1, 90.0f, 4);
        }
    }
    if (hero.items[6].unk40) {
        data->target = hero.items[6].lock_target;
        data->target_timer = scale_game_frames_2b2100(20);
        data->target_height = collision_hit_z_2b2100 - data->target->pos.z;
    }
    if (D_L00_0015F5C4 != 2 && !data->cooldown && !hero.unk20AC && m->seq < 2 &&
        (pad_input_2b2100.held & hero.items[0].button_mask) && scale_game_frames_2b2100(22) < hero.items[0].unk20) {
        velocity.q = 0;
        if (m->prev_seq == 0) blend_moby_animation_2b2100(m, 1, 0, scale_game_frames_2b2100(4));
        if (hero.state.current == 30 && (hero.moby->flags & 1)) {
            velocity.f.z = frame_time_2b2100 * 10.0f;
            transform_vector_2b2100(&velocity, &velocity, &inverse);
        } else {
            velocity.f.x = frame_time_2b2100 * 10.0f;
            transform_vector_by_basis_2b2100(&velocity, &velocity, &hero.basis);
        }
        if (FUN_L00_00233db8_2b2100(-1, 1)) {
            best = 0;
            if (hero.items[6].unk40) yaw = hero.items[6].unk4C;
            else yaw = atan2_fast_2b2100(velocity.f.x, velocity.f.y);
            pitch = -atan2_fast_2b2100(vector_length_xy_2b2100(&velocity), velocity.f.z);
            yaw0 = yaw;
            pitch0 = pitch;
            if (hero.state.current == 30 && (hero.moby->flags & 1)) {
                FUN_001f99f8_2b2100(&launch);
                launch.f.x += 0.15f;
                launch.f.z += 0.75f;
                launch.f.y += 0.3f;
                transform_vector_2b2100(&launch, &launch, &inverse);
                add_vector_xyz_2b2100(&launch, &launch, view_origin_2b2100);
                if (data->target) height = data->target_height;
            } else {
                FUN_L00_0024f7c8_2b2100(m, 0, &launch);
                i = 0;
                o = D_L00_001ABA00_2b2100[i];
                while (o) {
                    if ((*(signed char *)&o->state) >= 0) {
                        qcopy(&point, &o->pos);
                        r = get_moby_aim_info_2b2100(o);
                        if (r) point.f.z += r->height;
                        else point.f.z += 0.5f;
                        if ((o->flags & 0x1000) && o && ((Class_2b2100 *)o->pclass) && ((Class_2b2100 *)o->pclass)->category == 5) {
                            float tyaw, tpitch, distance, spread, off;
                            tyaw = atan2_fast_2b2100(point.f.x - launch.f.x, point.f.y - launch.f.y);
                            tpitch = atan2_fast_2b2100(distance_xy_2b2100(&launch, &point), point.f.z - launch.f.z);
                            distance = distance_xyz_2b2100(&launch, &point);
                            if (distance < 2.5f) {
                                struct Moby *cam = hero.moby;
                                if (fast_difference_between_rotations_2b2100(cam->rot.z, atan2_fast_2b2100(o->pos.x - hero.motion.pos.f[0], o->pos.y - hero.motion.pos.f[1])) < 1.0471976f &&
                                    absolute_float_2b2100(tpitch) < 0.7853982f) {
                                    limit = distance;
                                    pitch = -tpitch;
                                    best = o;
                                    yaw = tyaw;
                                    break;
                                }
                            }
                            if (!(limit < distance)) {
                                spread = convert_integer_to_float_2b2100(item_variant_2b2100.level) * 0.6981317f;
                                build_spherical_offset_2b2100(&probe, distance, yaw, pitch + hero.ground_pitch.f * 0.5f);
                                add_vector_xyz_2b2100(&probe, &probe, &launch);
                                off = approximate_arcsine_2b2100(distance_xyz_2b2100(&probe, &point) / (distance + distance));
                                off = off + off;
                                if (spread + 0.17453292f < off) {
                                    if (r) {
                                        float radius = convert_integer_to_float_2b2100(r->radius) * 0.125f;
                                        if (radius < distance) {
                                            off -= approximate_arcsine_2b2100(radius / distance);
                                            if (off < 0.0f) off = 0.0f;
                                        }
                                    }
                                }
                                if (off < spread + 0.17453292f) {
                                    if (!collision_segment_hits_2b2100(view_origin_2b2100, &point, 6, (int)m, 0)) {
                                        best = o;
                                        limit = distance;
                                        pitch = -tpitch;
                                        yaw = tyaw;
                                        if (!FUN_L00_0025f878_2b2100(best)) {
                                            height = 0.5f;
                                            data->target = best;
                                            if (r) height = r->height;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    i++;
                    o = D_L00_001ABA00_2b2100[i];
                }
            }
            allocate_voice_for_target_entry_2b2100(0, 0, (int)m);
            data->glow = scale_game_frames_2b2100(5) + scale_game_frames_2b2100(10);
            if (hero.state.current != 30) FUN_L00_002b1de0_2b2100(m);
            stats = (UsageStat_2b2100 *)usage_stats_2b2100;
            if (stats->uses < 65535) stats->uses++;
            if (scale_game_frames_2b2100(D_0015EEA4) / 600 > stats->longest)
                stats->longest = scale_game_frames_2b2100(D_0015EEA4) / 600;
            stats->levels = (stats->levels | (1 << level_index_2b2100)) | 0x80000000;
            if (hero.unk22C8 == 1 && hero.state.current != 30) {
                pitch = D_L00_0017ACF4_lui;
                yaw = m->rot.z;
                r = get_moby_aim_info_2b2100(data->target);
                if (r) height = r->height;
                FUN_L00_002afc70_2b2100(m, &launch, data->target, yaw, pitch, height, yaw, pitch);
            } else if (limit < 2.0f) {
                r = get_moby_aim_info_2b2100(data->target);
                if (r) height = r->height;
                FUN_L00_002afc70_2b2100(m, &launch, data->target, yaw, pitch, height, yaw, pitch);
            } else {
                if (!data->target) {
                    if (best) {
                        limit = FUN_L00_002aff50_2b2100(6);
                        qcopy(&point, &best->pos);
                        r = get_moby_aim_info_2b2100(best);
                        if (r) point.f.z += r->height;
                        else point.f.z += 0.5f;
                        pitch0 -= hero.ground_pitch.f * 0.5f;
                        build_spherical_offset_2b2100(&probe, limit, yaw0, pitch0);
                        add_vector_xyz_2b2100(&probe, &probe, &launch);
                        yaw = atan2_fast_2b2100(point.f.x - probe.f.x, point.f.y - probe.f.y);
                        pitch = -atan2_fast_2b2100(distance_xy_2b2100(&probe, &point), point.f.z - probe.f.z);
                    } else if (hero.state.current != 30) {
                        pitch -= hero.ground_pitch.f * 0.5f;
                        pitch0 -= hero.ground_pitch.f * 0.5f;
                    }
                }
                FUN_L00_002afc70_2b2100(m, &launch, data->target, yaw, pitch, height, yaw0, pitch0);
            }
            if (hero.state.control_mode == 0 || (hero.state.current == 2 && hero.unk229C < 0.7f)) {
                FUN_L00_002323b8_2b2100(0x36, 1, (float)scale_game_frames_2b2100(10));
            } else {
                FUN_L00_0020e4c8_2b2100();
            }
        } else {
            allocate_voice_for_target_entry_2b2100(1, 0, (int)m);
        }
        data->cooldown = scale_game_frames_2b2100(35);
    }
    if (m->seq != 2 && data->glow > 0) {
        if (data->glow > scale_game_frames_2b2100(10)) {
            a = convert_integer_to_float_2b2100(scale_game_frames_2b2100(5) + scale_game_frames_2b2100(10) - data->glow) / (float)scale_game_frames_2b2100(5);
            b = convert_integer_to_float_2b2100(scale_game_frames_2b2100(5) + scale_game_frames_2b2100(10) - data->glow) * 0.8f / (float)scale_game_frames_2b2100(5);
            c = convert_integer_to_float_2b2100(scale_game_frames_2b2100(5) + scale_game_frames_2b2100(10) - data->glow) * 0.5f / (float)scale_game_frames_2b2100(5);
        } else {
            a = convert_integer_to_float_2b2100(data->glow) / (float)scale_game_frames_2b2100(10);
            b = convert_integer_to_float_2b2100(data->glow) * 0.8f / (float)scale_game_frames_2b2100(10);
            c = convert_integer_to_float_2b2100(data->glow) * 0.5f / (float)scale_game_frames_2b2100(10);
        }
        FUN_L00_0024f7c8_2b2100(m, 0, &velocity);
        velocity.f.x += fast_cos_2b2100(atan2_fast_2b2100(hero.unk670, hero.unk674));
        velocity.f.y += fast_sin_2b2100(atan2_fast_2b2100(hero.unk670, hero.unk674));
        velocity.f.z += 0.3f;
        if (data->light == -1) {
            data->light = FUN_L00_0023e738_2b2100(&velocity, 7.0f, 0.0f, a, b, c);
        } else {
            Light_2b2100 *e = &lights_2b2100[data->light];
            qcopy(&e->pos, &velocity);
            e->pos.f.w = 7.0f;
            e->x = a;
            e->y = b;
            e->z = c;
        }
        FUN_001f9770_2b2100(&data->glow);
    } else if (data->light != -1) {
        FUN_L00_0023e838_2b2100(data->light);
        data->light = -1;
        data->glow = 0;
    }
    if (m->prev_seq == 2) {
        FUN_L00_0023e838_2b2100(data->light);
        data->light = -1;
        data->glow = 0;
    }
}
typedef struct {
    u8 pad0[0x1A4];
    s32 i1A4;
} C_b3058;

typedef struct {
    u8 pad0[0x140];
    f32 v140[4];
    u8 pad150[0x180 - 0x150];
    u8 *p180;
    u8 pad184[0x350 - 0x184];
    f32 f350;
    f32 f354;
    f32 f358;
    u8 pad35C[0x370 - 0x35C];
    f32 v370[4];
} S_b3058;

typedef struct {
    u8 pad0[0x68];
    u16 u68;
    u16 u6A;
    s32 i6C;
} R_b3058;

typedef struct {
    s32 i0;
    s32 i4;
} V_b3058;

typedef struct {
    u8 pad0[0x10];
    f32 v10[4];
    u8 pad20[0x34 - 0x20];
    u16 u34;
    u8 pad36[0x53 - 0x36];
    u8 b53;
    u8 pad54[0x70 - 0x54];
    u8 b70;
    u8 pad71[0x78 - 0x71];
    V_b3058 *v;
    u8 pad7C[0xD0 - 0x7C];
    f32 vD0[2];
    f32 fD8;
} O_b3058;

extern C_b3058 D_0013C940_b3058 __asm__("D_0013C940");
extern S_b3058 D_L00_00166C80_b3058 __asm__("D_L00_00166C80");
extern R_b3058 D_00141680_b3058 __asm__("D_00141680");
extern s32 D_0015EEA4;
extern s32 D_0015ED84_b3058 __asm__("D_0015ED84");

s32 FUN_001f9740_b3058(void *) __asm__("FUN_001f9740");
s32 FUN_L00_00233db8_b3058(s32, s32) __asm__("FUN_L00_00233db8");
f32 FUN_001f9e90_b3058(f32, f32) __asm__("FUN_001f9e90");
f32 FUN_001f9b20_b3058(void *) __asm__("FUN_001f9b20");
void FUN_001f9bf8_b3058(void *, void *, f32) __asm__("FUN_001f9bf8");
void FUN_001f9a10_b3058(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_00214db0_b3058(void *, f32, f32, f32) __asm__("FUN_00214db0");
void FUN_00212f90_b3058(void *, s32, s32, s32) __asm__("FUN_00212f90");
void FUN_L00_00203b18_b3058(void) __asm__("FUN_L00_00203b18");
s32 FUN_L00_002b58d8_b3058(void *, void *, f32, f32) __asm__("FUN_L00_002b58d8");
s32 FUN_001f96f8_b3058(s32) __asm__("FUN_001f96f8");
void FUN_0022da68_b3058(s32, s32, void *) __asm__("FUN_0022da68");

void visibomb_update(O_b3058 *o) __asm__("FUN_L00_002b3058");
void visibomb_update(O_b3058 *o) {
    V_b3058 *v;
    struct Hero *g;
    C_b3058 *c;
    S_b3058 *s;
    R_b3058 *r;
    f32 a, b;
    s32 k;
    f32 t[4] __attribute__((aligned(16)));
    f32 u[4] __attribute__((aligned(16)));

    if (o == 0) {
        return;
    }
    v = o->v;
    FUN_001f9740_b3058(v);
    c = &D_0013C940_b3058;
    g = &hero;
    if (!(c->i1A4 & g->items[0].button_mask))
        goto tail;
    if (v->i0 != 0)
        goto tail;
    if (v->i4 != 0)
        goto tail;
    if (g->unk20AC != 0)
        goto tail;
    k = g->state.control_mode;
    if (k == 7 || k == 2 || k == 3 || k == 4 || k == 5 || k == 6 || k == 0x10 || k == 0xE ||
        k == 9 || k == 0xA || k == 0xB || k >= 0xD)
        goto tail;
    if (g->state.current == 0x72 || g->state.current == 0x65)
        goto tail;
    s = &D_L00_00166C80_b3058;
    if (s->p180 != 0 && *(s16 *)(s->p180 + 0x86) == 5)
        goto tail;
    if (g->grounded_frames == 0)
        goto tail;
    if (((u8 *)g->ground_moby) != 0 && (*(s16 *)(((u8 *)g->ground_moby) + 0xA6) == 0x13E || *(s16 *)(((u8 *)g->ground_moby) + 0xA6) == 0x46F))
        goto tail;
    if (FUN_L00_00233db8_b3058(-1, 1) != 0) {
        b = 0.0f;
        a = g->unk688;
        if (g->state.current == 1 && (*(u16 *)((u8 *)g->moby + 0x34) & 1)) {
            a = FUN_001f9e90_b3058(s->f350, s->f354);
            b = -FUN_001f9e90_b3058(FUN_001f9b20_b3058(&s->f350), s->f358);
            FUN_001f9bf8_b3058(t, &s->f350, 0.3f);
            FUN_001f9bf8_b3058(u, s->v370, -0.35f);
            FUN_001f9a10_b3058(t, t, u);
            FUN_001f9a10_b3058(t, t, s->v140);
        } else if (hero.unk22C8 == 1) {
            b = FUN_001f9e90_b3058(FUN_001f9b20_b3058(o->vD0), o->fD8);
            FUN_00214db0_b3058(t, 0.6f, a, -b);
            FUN_001f9a10_b3058(t, t, o->v10);
        } else {
            FUN_00214db0_b3058(t, 0.6f, a, -b);
            FUN_001f9a10_b3058(t, t, o->v10);
        }
        if (o->b53 != 3) {
            FUN_00212f90_b3058(o, 3, 0, 0);
        }
        FUN_L00_00203b18_b3058();
        v->i4 = FUN_L00_002b58d8_b3058(o, t, a, b);
        r = &D_00141680_b3058;
        if (r->u68 < 0xFFFF) {
            r->u68++;
        }
        if (FUN_001f96f8_b3058(D_0015EEA4) / 600 > r->u6A) {
            r->u6A = FUN_001f96f8_b3058(D_0015EEA4) / 600;
        }
        r->i6C = r->i6C | (1 << D_0015ED84_b3058) | 0x80000000;
        v->i0 = FUN_001f96f8_b3058(0x3C);
    } else if (c->i1A4 & g->items[0].button_mask) {
        FUN_0022da68_b3058(0, 0, o);
    }
tail:
    if ((o->b70 & 2) && o->b53 != 1) {
        FUN_00212f90_b3058(o, 1, 0, 0);
    }
    o->u34 |= 4;
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
typedef struct {
    int w0;
    int w4;
    int w8;
    int wC;
    int idx[4];
    unsigned char *p[4];
} V_2b3458;
int FUN_L00_0025a420_2b3458(void *, int, int) __asm__("FUN_L00_0025a420");
int FUN_001f9740_2b3458(int *) __asm__("FUN_001f9740");
void FUN_0020c828_2b3458(void *) __asm__("FUN_0020c828");
extern int D_L00_00161478 __attribute__((sda));
extern int D_L00_00161480 __attribute__((sda));
void FUN_L00_002b3458(unsigned char *m) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_ACTIVE = 1
    };
    V_2b3458 *v = *(V_2b3458 **)(m + 0x78);
    int r, i;
    switch (m[0x20]) {
    case STATE_INITIALIZE:
        m[0x20] = 1;
        v->w4 = D_L00_00161478;
        v->w8 = 3;
        v->p[0] = D_L00_0015FFD8 + (v->idx[0] << 8);
        v->p[1] = D_L00_0015FFD8 + (v->idx[1] << 8);
        v->p[2] = D_L00_0015FFD8 + (v->idx[2] << 8);
        v->p[3] = D_L00_0015FFD8 + (v->idx[3] << 8);
        break;
    case STATE_ACTIVE:
        r = FUN_L00_0025a420_2b3458(m, 0x10000, 0);
        FUN_001f9740_2b3458(&v->wC);
        if (r && v->wC == 0) {
            m[0xA4] = 0xFF;
            v->wC = D_L00_00161480;
            if (--v->w8 <= 0) {
                FUN_0020c828_2b3458(m);
                return;
            }
        }
        goto test;
    found: {
        unsigned char *q;
        v->p[i][0x20] = 0x51;
        q = v->p[i];
        qcopy(q + 0x10, m + 0x10);
        q[0xA4] = 0xFF;
        return;
    }
    test:
        if (v->p[0][0x20] == 0x50 || v->p[1][0x20] == 0x50 || v->p[2][0x20] == 0x50 ||
            v->p[3][0x20] == 0x50) {
            if (FUN_001f9740_2b3458(&v->w4)) {
                v->w4 = D_L00_00161478;
                for (i = 0; i < 4; i++) {
                    if (v->p[i][0x20] == 0x50)
                        goto found;
                }
            }
        }
        break;
    }
}
#include "rnc/gameplay/entities/moby.h"
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) Vec4A_2b3628;

typedef int Q_2b3628 __attribute__((mode(TI)));
typedef struct { float pos[4][4]; unsigned int col[4]; float uv[4][2]; unsigned long tag[4]; } Quad_2b3628;
typedef struct { int a, b; } Ids_2b3628;
typedef struct { float x, y, z; } P3_2b3628;
typedef struct { float u, v; } UV_2b3628;
typedef struct Child_2b3628 { char pad0[0xA6]; short cls; } Child_2b3628;
typedef struct Data_2b3628 { int f0; int f4; Child_2b3628 *child; char padC[0x14]; float f20; char pad24[8]; float f2C; } Data_2b3628;

extern float D_L00_00166DC0_2b3628[] __asm__("D_L00_00166DC0");
extern void transform_joint_points_2b3628(void *, int, void *, void *) __asm__("FUN_0020cd48");
extern void subtract_vector_xyz_2b3628(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz_2b3628(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_vec_cross_2b3628(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void func_001FA190_2b3628(void *) __asm__("FUN_001f9fc8");
extern void func_001FA540_2b3628(void *, void *, void *) __asm__("FUN_001fa378");
extern long get_effect_texture_2b3628(int) __asm__("FUN_001f44b8");
extern void scale_vector_xyz_2b3628(void *, void *, float) __asm__("FUN_001f9a68");
extern float random_float_between_2b3628(float, float) __asm__("FUN_002132a8");
extern void draw_geometry_quad_2b3628(void *, void *, int) __asm__("FUN_001f7d30");
extern void add_vector_xyz_2b3628(void *, void *, void *) __asm__("FUN_001f9a10");
extern void transform_vector_2b3628(void *, void *, void *) __asm__("FUN_001f9d20");
extern void vu1_add_g_sregister_2b3628(int, long) __asm__("FUN_00233980");
extern void font_queue_vu_state_2b3628(void) __asm__("FUN_001f76a0");
extern void func_L00_001FDE48_2b3628(int, void *, void *, void *, int) __asm__("FUN_L00_001fde98");

extern Ids_2b3628 D_L00_00161488 __attribute__((section(".data")));
extern Ids_2b3628 D_L00_00161490 __attribute__((section(".data")));
#define BEAM_QUAD_YZ { \
    {0.0f, -2.0f, 2.0f, 1.0f}, \
    {0.0f, -2.0f, -2.0f, 1.0f}, \
    {0.0f, 2.0f, 2.0f, 1.0f}, \
    {0.0f, 2.0f, -2.0f, 1.0f} \
}
float D_L00_001DA9E0[4][4] = BEAM_QUAD_YZ;
float D_L01_001DAE60[4][4] = BEAM_QUAD_YZ;
float D_L02_001CF460[4][4] = BEAM_QUAD_YZ;
float D_L03_001CEF60[4][4] = BEAM_QUAD_YZ;
float D_L04_001CEFE0[4][4] = BEAM_QUAD_YZ;
float D_L05_001CF300[4][4] = BEAM_QUAD_YZ;
float D_L06_001CF5E0[4][4] = BEAM_QUAD_YZ;
float D_L07_001CEEE0[4][4] = BEAM_QUAD_YZ;
float D_L08_001CF660[4][4] = BEAM_QUAD_YZ;
float D_L09_001CEFE0[4][4] = BEAM_QUAD_YZ;
float D_L10_001CF2E0[4][4] = BEAM_QUAD_YZ;
float D_L11_001CF860[4][4] = BEAM_QUAD_YZ;
float D_L12_001E1F00[4][4] = BEAM_QUAD_YZ;
float D_L13_001CF770[4][4] = BEAM_QUAD_YZ;
float D_L14_001DBE20[4][4] = BEAM_QUAD_YZ;
float D_L15_001CF8E0[4][4] = BEAM_QUAD_YZ;
float D_L16_001CF2E0[4][4] = BEAM_QUAD_YZ;
float D_L17_001CFB60[4][4] = BEAM_QUAD_YZ;
float D_L18_001CF860[4][4] = BEAM_QUAD_YZ;
#define BEAM_QUAD_UVS { \
    {0.0f, 0.0f}, \
    {0.0f, 1.0f}, \
    {1.0f, 0.0f}, \
    {1.0f, 1.0f} \
}
float D_L00_001DAA60[4][2] = BEAM_QUAD_UVS;
float D_L01_001DAEE0[4][2] = BEAM_QUAD_UVS;
float D_L02_001CF4E0[4][2] = BEAM_QUAD_UVS;
float D_L03_001CEFE0[4][2] = BEAM_QUAD_UVS;
float D_L04_001CF060[4][2] = BEAM_QUAD_UVS;
float D_L05_001CF380[4][2] = BEAM_QUAD_UVS;
float D_L06_001CF660[4][2] = BEAM_QUAD_UVS;
float D_L07_001CEF60[4][2] = BEAM_QUAD_UVS;
float D_L08_001CF6E0[4][2] = BEAM_QUAD_UVS;
float D_L09_001CF060[4][2] = BEAM_QUAD_UVS;
float D_L10_001CF360[4][2] = BEAM_QUAD_UVS;
float D_L11_001CF8E0[4][2] = BEAM_QUAD_UVS;
float D_L12_001E1F80[4][2] = BEAM_QUAD_UVS;
float D_L13_001CF7F0[4][2] = BEAM_QUAD_UVS;
float D_L14_001DBEA0[4][2] = BEAM_QUAD_UVS;
float D_L15_001CF960[4][2] = BEAM_QUAD_UVS;
float D_L16_001CF360[4][2] = BEAM_QUAD_UVS;
float D_L17_001CFBE0[4][2] = BEAM_QUAD_UVS;
float D_L18_001CF8E0[4][2] = BEAM_QUAD_UVS;
#define BEAM_QUAD_XZ { \
    {-2.0f, 0.0f, 2.0f, 1.0f}, \
    {-2.0f, 0.0f, -2.0f, 1.0f}, \
    {2.0f, 0.0f, 2.0f, 1.0f}, \
    {2.0f, 0.0f, -2.0f, 1.0f} \
}
float D_L00_001DAA20[4][4] = BEAM_QUAD_XZ;
float D_L01_001DAEA0[4][4] = BEAM_QUAD_XZ;
float D_L02_001CF4A0[4][4] = BEAM_QUAD_XZ;
float D_L03_001CEFA0[4][4] = BEAM_QUAD_XZ;
float D_L04_001CF020[4][4] = BEAM_QUAD_XZ;
float D_L05_001CF340[4][4] = BEAM_QUAD_XZ;
float D_L06_001CF620[4][4] = BEAM_QUAD_XZ;
float D_L07_001CEF20[4][4] = BEAM_QUAD_XZ;
float D_L08_001CF6A0[4][4] = BEAM_QUAD_XZ;
float D_L09_001CF020[4][4] = BEAM_QUAD_XZ;
float D_L10_001CF320[4][4] = BEAM_QUAD_XZ;
float D_L11_001CF8A0[4][4] = BEAM_QUAD_XZ;
float D_L12_001E1F40[4][4] = BEAM_QUAD_XZ;
float D_L13_001CF7B0[4][4] = BEAM_QUAD_XZ;
float D_L14_001DBE60[4][4] = BEAM_QUAD_XZ;
float D_L15_001CF920[4][4] = BEAM_QUAD_XZ;
float D_L16_001CF320[4][4] = BEAM_QUAD_XZ;
float D_L17_001CFBA0[4][4] = BEAM_QUAD_XZ;
float D_L18_001CF8A0[4][4] = BEAM_QUAD_XZ;
#define RIBBON_UVS { \
    {0.625f, 0.027055f}, \
    {0.654509f, 0.027055f}, \
    {0.5f, 0.0f}, \
    {0.625f, 0.027055f}, \
    {0.5f, 0.0f}, \
    {0.547746f, 0.027055f}, \
    {0.5f, 0.0f}, \
    {0.452254f, 0.027055f}, \
    {0.5f, 0.0f}, \
    {0.375f, 0.027055f}, \
    {0.5f, 0.0f}, \
    {0.345491f, 0.027055f}, \
    {0.5f, 0.0f}, \
    {0.375f, 0.027055f}, \
    {0.5f, 0.0f}, \
    {0.452254f, 0.027055f}, \
    {0.5f, 0.0f}, \
    {0.547746f, 0.027055f}, \
    {0.625f, 0.027055f}, \
    {0.547746f, 0.027055f}, \
    {0.737764f, 0.105573f}, \
    {0.547746f, 0.027055f}, \
    {0.590818f, 0.105573f}, \
    {0.452254f, 0.027055f}, \
    {0.409182f, 0.105573f}, \
    {0.375f, 0.027055f}, \
    {0.262236f, 0.105573f}, \
    {0.345491f, 0.027055f}, \
    {0.206107f, 0.105573f}, \
    {0.375f, 0.027055f}, \
    {0.262236f, 0.105573f}, \
    {0.452254f, 0.027055f}, \
    {0.409182f, 0.105573f}, \
    {0.547746f, 0.027055f}, \
    {0.590818f, 0.105573f}, \
    {0.625f, 0.027055f}, \
    {0.737764f, 0.105573f}, \
    {0.654509f, 0.027055f}, \
    {0.793893f, 0.105573f}, \
    {0.625f, 0.027055f}, \
    {0.793893f, 0.105573f}, \
    {0.737764f, 0.105573f}, \
    {0.904509f, 0.227867f}, \
    {0.737764f, 0.105573f}, \
    {0.827254f, 0.227867f}, \
    {0.590818f, 0.105573f}, \
    {0.625f, 0.227867f}, \
    {0.409182f, 0.105573f}, \
    {0.375f, 0.227867f}, \
    {0.262236f, 0.105573f}, \
    {0.172746f, 0.227867f}, \
    {0.206107f, 0.105573f}, \
    {0.095491f, 0.227867f}, \
    {0.262236f, 0.105573f}, \
    {0.172746f, 0.227867f}, \
    {0.409182f, 0.105573f}, \
    {0.375f, 0.227867f}, \
    {0.590818f, 0.105573f}, \
    {0.625f, 0.227867f}, \
    {0.737764f, 0.105573f}, \
    {0.827254f, 0.227867f}, \
    {0.793893f, 0.105573f}, \
    {0.827254f, 0.227867f}, \
    {0.904509f, 0.227867f}, \
    {0.88471f, 0.381966f}, \
    {0.904509f, 0.227867f}, \
    {0.975528f, 0.381966f}, \
    {0.827254f, 0.227867f}, \
    {0.88471f, 0.381966f}, \
    {0.625f, 0.227867f}, \
    {0.646946f, 0.381966f}, \
    {0.375f, 0.227867f}, \
    {0.353054f, 0.381966f}, \
    {0.172746f, 0.227867f}, \
    {0.11529f, 0.381966f}, \
    {0.095491f, 0.227867f}, \
    {0.024472f, 0.381966f}, \
    {0.172746f, 0.227867f}, \
    {0.11529f, 0.381966f}, \
    {0.375f, 0.227867f}, \
    {0.353054f, 0.381966f}, \
    {0.625f, 0.227867f}, \
    {0.646946f, 0.381966f}, \
    {0.827254f, 0.227867f}, \
    {0.646946f, 0.381966f}, \
    {0.88471f, 0.381966f}, \
    {0.654509f, 0.552786f}, \
    {0.88471f, 0.381966f}, \
    {0.904509f, 0.552786f}, \
    {0.975528f, 0.381966f}, \
    {1.0f, 0.552786f}, \
    {0.88471f, 0.381966f}, \
    {0.904508f, 0.552786f}, \
    {0.646946f, 0.381966f}, \
    {0.654508f, 0.552786f}, \
    {0.353054f, 0.381966f}, \
    {0.345491f, 0.552786f}, \
    {0.11529f, 0.381966f}, \
    {0.095491f, 0.552786f}, \
    {0.024472f, 0.381966f}, \
    {0.0f, 0.552786f}, \
    {0.11529f, 0.381966f}, \
    {0.095491f, 0.552786f}, \
    {0.353054f, 0.381966f}, \
    {0.345491f, 0.552786f}, \
    {0.646946f, 0.381966f}, \
    {0.345491f, 0.552786f}, \
    {0.654509f, 0.552786f}, \
    {0.353054f, 0.723607f}, \
    {0.654509f, 0.552786f}, \
    {0.646946f, 0.723607f}, \
    {0.904509f, 0.552786f}, \
    {0.88471f, 0.723607f}, \
    {1.0f, 0.552786f}, \
    {0.975528f, 0.723607f}, \
    {0.904508f, 0.552786f}, \
    {0.88471f, 0.723607f}, \
    {0.654508f, 0.552786f}, \
    {0.646946f, 0.723607f}, \
    {0.345491f, 0.552786f}, \
    {0.353054f, 0.723607f}, \
    {0.095491f, 0.552786f}, \
    {0.11529f, 0.723607f}, \
    {0.0f, 0.552786f}, \
    {0.024472f, 0.723607f}, \
    {0.095491f, 0.552786f}, \
    {0.11529f, 0.723607f}, \
    {0.345491f, 0.552786f}, \
    {0.11529f, 0.723607f}, \
    {0.353054f, 0.723607f}, \
    {0.172746f, 0.877706f}, \
    {0.375f, 0.877706f}, \
    {0.262236f, 1.0f}, \
    {0.375f, 0.877706f}, \
    {0.409182f, 1.0f}, \
    {0.375f, 0.877706f}, \
    {0.590818f, 1.0f}, \
    {0.375f, 0.877706f}, \
    {0.625f, 0.877706f}, \
    {0.353054f, 0.723607f}, \
    {0.625f, 0.877706f}, \
    {0.646946f, 0.723607f}, \
    {0.625f, 0.877706f}, \
    {0.88471f, 0.723607f}, \
    {0.625f, 0.877706f}, \
    {0.827254f, 0.877706f}, \
    {0.590818f, 1.0f}, \
    {0.827254f, 0.877706f}, \
    {0.737764f, 1.0f}, \
    {0.827254f, 0.877706f}, \
    {0.793893f, 1.0f}, \
    {0.827254f, 0.877706f}, \
    {0.904509f, 0.877706f}, \
    {0.88471f, 0.723607f}, \
    {0.904509f, 0.877706f}, \
    {0.975528f, 0.723607f}, \
    {0.904509f, 0.877706f}, \
    {0.88471f, 0.723607f}, \
    {0.904509f, 0.877706f}, \
    {0.827254f, 0.877706f}, \
    {0.793893f, 1.0f}, \
    {0.827254f, 0.877706f}, \
    {0.737764f, 1.0f}, \
    {0.827254f, 0.877706f}, \
    {0.590818f, 1.0f}, \
    {0.827254f, 0.877706f}, \
    {0.625f, 0.877706f}, \
    {0.88471f, 0.723607f}, \
    {0.625f, 0.877706f}, \
    {0.646946f, 0.723607f}, \
    {0.625f, 0.877706f}, \
    {0.353054f, 0.723607f}, \
    {0.625f, 0.877706f}, \
    {0.375f, 0.877706f}, \
    {0.590818f, 1.0f}, \
    {0.375f, 0.877706f}, \
    {0.409182f, 1.0f}, \
    {0.375f, 0.877706f}, \
    {0.262236f, 1.0f}, \
    {0.375f, 0.877706f}, \
    {0.172746f, 0.877706f}, \
    {0.353054f, 0.723607f}, \
    {0.172746f, 0.877706f}, \
    {0.11529f, 0.723607f}, \
    {0.095491f, 0.877706f}, \
    {0.024472f, 0.723607f}, \
    {0.095491f, 0.877706f}, \
    {0.11529f, 0.723607f}, \
    {0.095491f, 0.877706f}, \
    {0.172746f, 0.877706f}, \
    {0.095491f, 0.877706f}, \
    {0.262236f, 1.0f}, \
    {0.095491f, 0.877706f}, \
    {0.206107f, 1.0f}, \
    {0.172746f, 0.877706f}, \
    {0.262236f, 1.0f} \
}
UV_2b3628 D_L00_001DA0B0[196] = RIBBON_UVS;
UV_2b3628 D_L01_001DA530[196] = RIBBON_UVS;
UV_2b3628 D_L02_001CEB30[196] = RIBBON_UVS;
UV_2b3628 D_L03_001CE630[196] = RIBBON_UVS;
UV_2b3628 D_L04_001CE6B0[196] = RIBBON_UVS;
UV_2b3628 D_L05_001CE9D0[196] = RIBBON_UVS;
UV_2b3628 D_L06_001CECB0[196] = RIBBON_UVS;
UV_2b3628 D_L07_001CE5B0[196] = RIBBON_UVS;
UV_2b3628 D_L08_001CED30[196] = RIBBON_UVS;
UV_2b3628 D_L09_001CE6B0[196] = RIBBON_UVS;
UV_2b3628 D_L10_001CE9B0[196] = RIBBON_UVS;
UV_2b3628 D_L11_001CEF30[196] = RIBBON_UVS;
UV_2b3628 D_L12_001E15D0[196] = RIBBON_UVS;
UV_2b3628 D_L13_001CEE40[196] = RIBBON_UVS;
UV_2b3628 D_L14_001DB4F0[196] = RIBBON_UVS;
UV_2b3628 D_L15_001CEFB0[196] = RIBBON_UVS;
UV_2b3628 D_L16_001CE9B0[196] = RIBBON_UVS;
UV_2b3628 D_L17_001CF230[196] = RIBBON_UVS;
UV_2b3628 D_L18_001CEF30[196] = RIBBON_UVS;
#define RIBBON_POINTS { \
    {-1.389244f, -1.912131f, -0.901867f, 1.0f}, \
    {0.0f, -2.363524f, -0.901867f, 1.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}, \
    {1.389244f, -1.912131f, -0.901867f, 1.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}, \
    {2.247845f, -0.730369f, -0.901867f, 1.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}, \
    {2.247845f, 0.730369f, -0.901867f, 1.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}, \
    {1.389244f, 1.912131f, -0.901867f, 1.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}, \
    {0.0f, 2.363524f, -0.901867f, 1.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}, \
    {-1.389244f, 1.912131f, -0.901867f, 1.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}, \
    {-2.247844f, 0.730369f, -0.901867f, 1.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}, \
    {-2.247845f, -0.730369f, -0.901867f, 1.0f}, \
    {-1.389244f, -1.912131f, -0.901867f, 1.0f}, \
    {-2.247845f, -0.730369f, -0.901867f, 1.0f}, \
    {-2.6425f, -3.637089f, -3.519189f, 1.0f}, \
    {-2.247845f, -0.730369f, -0.901867f, 1.0f}, \
    {-4.275654f, -1.389244f, -3.519189f, 1.0f}, \
    {-2.247844f, 0.730369f, -0.901867f, 1.0f}, \
    {-4.275654f, 1.389245f, -3.519189f, 1.0f}, \
    {-1.389244f, 1.912131f, -0.901867f, 1.0f}, \
    {-2.642499f, 3.637089f, -3.519189f, 1.0f}, \
    {0.0f, 2.363524f, -0.901867f, 1.0f}, \
    {0.0f, 4.495689f, -3.519189f, 1.0f}, \
    {1.389244f, 1.912131f, -0.901867f, 1.0f}, \
    {2.6425f, 3.637089f, -3.519189f, 1.0f}, \
    {2.247845f, 0.730369f, -0.901867f, 1.0f}, \
    {4.275654f, 1.389244f, -3.519189f, 1.0f}, \
    {2.247845f, -0.730369f, -0.901867f, 1.0f}, \
    {4.275654f, -1.389244f, -3.519189f, 1.0f}, \
    {1.389244f, -1.912131f, -0.901867f, 1.0f}, \
    {2.6425f, -3.637089f, -3.519189f, 1.0f}, \
    {0.0f, -2.363524f, -0.901867f, 1.0f}, \
    {0.0f, -4.495689f, -3.519189f, 1.0f}, \
    {-1.389244f, -1.912131f, -0.901867f, 1.0f}, \
    {0.0f, -4.495689f, -3.519189f, 1.0f}, \
    {-2.6425f, -3.637089f, -3.519189f, 1.0f}, \
    {0.0f, -6.187785f, -7.595764f, 1.0f}, \
    {-2.6425f, -3.637089f, -3.519189f, 1.0f}, \
    {-3.637089f, -5.006023f, -7.595764f, 1.0f}, \
    {-4.275654f, -1.389244f, -3.519189f, 1.0f}, \
    {-5.884933f, -1.91213f, -7.595764f, 1.0f}, \
    {-4.275654f, 1.389245f, -3.519189f, 1.0f}, \
    {-5.884933f, 1.912131f, -7.595764f, 1.0f}, \
    {-2.642499f, 3.637089f, -3.519189f, 1.0f}, \
    {-3.637089f, 5.006023f, -7.595764f, 1.0f}, \
    {0.0f, 4.495689f, -3.519189f, 1.0f}, \
    {0.0f, 6.187785f, -7.595764f, 1.0f}, \
    {2.6425f, 3.637089f, -3.519189f, 1.0f}, \
    {3.637089f, 5.006023f, -7.595764f, 1.0f}, \
    {4.275654f, 1.389244f, -3.519189f, 1.0f}, \
    {5.884933f, 1.912131f, -7.595764f, 1.0f}, \
    {4.275654f, -1.389244f, -3.519189f, 1.0f}, \
    {5.884933f, -1.912131f, -7.595764f, 1.0f}, \
    {2.6425f, -3.637089f, -3.519189f, 1.0f}, \
    {3.637089f, -5.006023f, -7.595764f, 1.0f}, \
    {0.0f, -4.495689f, -3.519189f, 1.0f}, \
    {3.637089f, -5.006023f, -7.595764f, 1.0f}, \
    {0.0f, -6.187785f, -7.595764f, 1.0f}, \
    {4.275654f, -5.884933f, -12.732548f, 1.0f}, \
    {0.0f, -6.187785f, -7.595764f, 1.0f}, \
    {0.0f, -7.274178f, -12.732548f, 1.0f}, \
    {-3.637089f, -5.006023f, -7.595764f, 1.0f}, \
    {-4.275655f, -5.884933f, -12.732548f, 1.0f}, \
    {-5.884933f, -1.91213f, -7.595764f, 1.0f}, \
    {-6.918154f, -2.247844f, -12.732548f, 1.0f}, \
    {-5.884933f, 1.912131f, -7.595764f, 1.0f}, \
    {-6.918154f, 2.247845f, -12.732548f, 1.0f}, \
    {-3.637089f, 5.006023f, -7.595764f, 1.0f}, \
    {-4.275654f, 5.884933f, -12.732548f, 1.0f}, \
    {0.0f, 6.187785f, -7.595764f, 1.0f}, \
    {0.0f, 7.274178f, -12.732548f, 1.0f}, \
    {3.637089f, 5.006023f, -7.595764f, 1.0f}, \
    {4.275655f, 5.884933f, -12.732548f, 1.0f}, \
    {5.884933f, 1.912131f, -7.595764f, 1.0f}, \
    {6.918154f, 2.247845f, -12.732548f, 1.0f}, \
    {5.884933f, -1.912131f, -7.595764f, 1.0f}, \
    {6.918154f, -2.247845f, -12.732548f, 1.0f}, \
    {3.637089f, -5.006023f, -7.595764f, 1.0f}, \
    {6.918154f, -2.247845f, -12.732548f, 1.0f}, \
    {4.275654f, -5.884933f, -12.732548f, 1.0f}, \
    {7.274178f, -2.363524f, -18.426716f, 1.0f}, \
    {4.275654f, -5.884933f, -12.732548f, 1.0f}, \
    {4.495689f, -6.187785f, -18.426716f, 1.0f}, \
    {0.0f, -7.274178f, -12.732548f, 1.0f}, \
    {0.0f, -7.648523f, -18.426716f, 1.0f}, \
    {-4.275655f, -5.884933f, -12.732548f, 1.0f}, \
    {-4.495689f, -6.187785f, -18.426716f, 1.0f}, \
    {-6.918154f, -2.247844f, -12.732548f, 1.0f}, \
    {-7.274178f, -2.363523f, -18.426716f, 1.0f}, \
    {-6.918154f, 2.247845f, -12.732548f, 1.0f}, \
    {-7.274178f, 2.363524f, -18.426716f, 1.0f}, \
    {-4.275654f, 5.884933f, -12.732548f, 1.0f}, \
    {-4.495688f, 6.187785f, -18.426716f, 1.0f}, \
    {0.0f, 7.274178f, -12.732548f, 1.0f}, \
    {0.0f, 7.648523f, -18.426716f, 1.0f}, \
    {4.275655f, 5.884933f, -12.732548f, 1.0f}, \
    {4.495689f, 6.187785f, -18.426716f, 1.0f}, \
    {6.918154f, 2.247845f, -12.732548f, 1.0f}, \
    {7.274178f, 2.363524f, -18.426716f, 1.0f}, \
    {6.918154f, -2.247845f, -12.732548f, 1.0f}, \
    {7.274178f, 2.363524f, -18.426716f, 1.0f}, \
    {7.274178f, -2.363524f, -18.426716f, 1.0f}, \
    {6.918154f, 2.247845f, -24.120884f, 1.0f}, \
    {7.274178f, -2.363524f, -18.426716f, 1.0f}, \
    {6.918154f, -2.247845f, -24.120884f, 1.0f}, \
    {4.495689f, -6.187785f, -18.426716f, 1.0f}, \
    {4.275654f, -5.884933f, -24.120884f, 1.0f}, \
    {0.0f, -7.648523f, -18.426716f, 1.0f}, \
    {0.0f, -7.274178f, -24.120884f, 1.0f}, \
    {-4.495689f, -6.187785f, -18.426716f, 1.0f}, \
    {-4.275655f, -5.884933f, -24.120884f, 1.0f}, \
    {-7.274178f, -2.363523f, -18.426716f, 1.0f}, \
    {-6.918154f, -2.247844f, -24.120884f, 1.0f}, \
    {-7.274178f, 2.363524f, -18.426716f, 1.0f}, \
    {-6.918154f, 2.247845f, -24.120884f, 1.0f}, \
    {-4.495688f, 6.187785f, -18.426716f, 1.0f}, \
    {-4.275654f, 5.884933f, -24.120884f, 1.0f}, \
    {0.0f, 7.648523f, -18.426716f, 1.0f}, \
    {0.0f, 7.274178f, -24.120884f, 1.0f}, \
    {4.495689f, 6.187785f, -18.426716f, 1.0f}, \
    {4.275655f, 5.884933f, -24.120884f, 1.0f}, \
    {7.274178f, 2.363524f, -18.426716f, 1.0f}, \
    {4.275655f, 5.884933f, -24.120884f, 1.0f}, \
    {6.918154f, 2.247845f, -24.120884f, 1.0f}, \
    {3.637089f, 5.006023f, -29.257668f, 1.0f}, \
    {5.884933f, 1.912131f, -29.257668f, 1.0f}, \
    {2.6425f, 3.637089f, -33.334244f, 1.0f}, \
    {5.884933f, 1.912131f, -29.257668f, 1.0f}, \
    {4.275654f, 1.389244f, -33.334244f, 1.0f}, \
    {5.884933f, 1.912131f, -29.257668f, 1.0f}, \
    {4.275654f, -1.389244f, -33.334244f, 1.0f}, \
    {5.884933f, 1.912131f, -29.257668f, 1.0f}, \
    {5.884933f, -1.912131f, -29.257668f, 1.0f}, \
    {6.918154f, 2.247845f, -24.120884f, 1.0f}, \
    {5.884933f, -1.912131f, -29.257668f, 1.0f}, \
    {6.918154f, -2.247845f, -24.120884f, 1.0f}, \
    {5.884933f, -1.912131f, -29.257668f, 1.0f}, \
    {4.275654f, -5.884933f, -24.120884f, 1.0f}, \
    {5.884933f, -1.912131f, -29.257668f, 1.0f}, \
    {3.637089f, -5.006023f, -29.257668f, 1.0f}, \
    {4.275654f, -1.389244f, -33.334244f, 1.0f}, \
    {3.637089f, -5.006023f, -29.257668f, 1.0f}, \
    {2.6425f, -3.637089f, -33.334244f, 1.0f}, \
    {3.637089f, -5.006023f, -29.257668f, 1.0f}, \
    {0.0f, -4.495689f, -33.334244f, 1.0f}, \
    {3.637089f, -5.006023f, -29.257668f, 1.0f}, \
    {0.0f, -6.187785f, -29.257668f, 1.0f}, \
    {4.275654f, -5.884933f, -24.120884f, 1.0f}, \
    {0.0f, -6.187785f, -29.257668f, 1.0f}, \
    {0.0f, -7.274178f, -24.120884f, 1.0f}, \
    {0.0f, -6.187785f, -29.257668f, 1.0f}, \
    {-4.275655f, -5.884933f, -24.120884f, 1.0f}, \
    {0.0f, -6.187785f, -29.257668f, 1.0f}, \
    {-3.637089f, -5.006023f, -29.257668f, 1.0f}, \
    {0.0f, -4.495689f, -33.334244f, 1.0f}, \
    {-3.637089f, -5.006023f, -29.257668f, 1.0f}, \
    {-2.6425f, -3.637089f, -33.334244f, 1.0f}, \
    {-3.637089f, -5.006023f, -29.257668f, 1.0f}, \
    {-4.275654f, -1.389244f, -33.334244f, 1.0f}, \
    {-3.637089f, -5.006023f, -29.257668f, 1.0f}, \
    {-5.884933f, -1.91213f, -29.257668f, 1.0f}, \
    {-4.275655f, -5.884933f, -24.120884f, 1.0f}, \
    {-5.884933f, -1.91213f, -29.257668f, 1.0f}, \
    {-6.918154f, -2.247844f, -24.120884f, 1.0f}, \
    {-5.884933f, -1.91213f, -29.257668f, 1.0f}, \
    {-6.918154f, 2.247845f, -24.120884f, 1.0f}, \
    {-5.884933f, -1.91213f, -29.257668f, 1.0f}, \
    {-5.884933f, 1.912131f, -29.257668f, 1.0f}, \
    {-4.275654f, -1.389244f, -33.334244f, 1.0f}, \
    {-5.884933f, 1.912131f, -29.257668f, 1.0f}, \
    {-4.275654f, 1.389245f, -33.334244f, 1.0f}, \
    {-5.884933f, 1.912131f, -29.257668f, 1.0f}, \
    {-2.642499f, 3.637089f, -33.334244f, 1.0f}, \
    {-5.884933f, 1.912131f, -29.257668f, 1.0f}, \
    {-3.637089f, 5.006023f, -29.257668f, 1.0f}, \
    {-6.918154f, 2.247845f, -24.120884f, 1.0f}, \
    {-3.637089f, 5.006023f, -29.257668f, 1.0f}, \
    {-4.275654f, 5.884933f, -24.120884f, 1.0f}, \
    {0.0f, 6.187785f, -29.257668f, 1.0f}, \
    {0.0f, 7.274178f, -24.120884f, 1.0f}, \
    {0.0f, 6.187785f, -29.257668f, 1.0f}, \
    {4.275655f, 5.884933f, -24.120884f, 1.0f}, \
    {0.0f, 6.187785f, -29.257668f, 1.0f}, \
    {3.637089f, 5.006023f, -29.257668f, 1.0f}, \
    {0.0f, 6.187785f, -29.257668f, 1.0f}, \
    {2.6425f, 3.637089f, -33.334244f, 1.0f}, \
    {0.0f, 6.187785f, -29.257668f, 1.0f}, \
    {0.0f, 4.495689f, -33.334244f, 1.0f}, \
    {-3.637089f, 5.006023f, -29.257668f, 1.0f}, \
    {-2.642499f, 3.637089f, -33.334244f, 1.0f} \
}
float D_L00_001D9470[196][4] = RIBBON_POINTS;
float D_L01_001D98F0[196][4] = RIBBON_POINTS;
float D_L02_001CDEF0[196][4] = RIBBON_POINTS;
float D_L03_001CD9F0[196][4] = RIBBON_POINTS;
float D_L04_001CDA70[196][4] = RIBBON_POINTS;
float D_L05_001CDD90[196][4] = RIBBON_POINTS;
float D_L06_001CE070[196][4] = RIBBON_POINTS;
float D_L07_001CD970[196][4] = RIBBON_POINTS;
float D_L08_001CE0F0[196][4] = RIBBON_POINTS;
float D_L09_001CDA70[196][4] = RIBBON_POINTS;
float D_L10_001CDD70[196][4] = RIBBON_POINTS;
float D_L11_001CE2F0[196][4] = RIBBON_POINTS;
float D_L12_001E0990[196][4] = RIBBON_POINTS;
float D_L13_001CE200[196][4] = RIBBON_POINTS;
float D_L14_001DA8B0[196][4] = RIBBON_POINTS;
float D_L15_001CE370[196][4] = RIBBON_POINTS;
float D_L16_001CDD70[196][4] = RIBBON_POINTS;
float D_L17_001CE5F0[196][4] = RIBBON_POINTS;
float D_L18_001CE2F0[196][4] = RIBBON_POINTS;
P3_2b3628 D_L00_001DAA80[196] = {0};
P3_2b3628 D_L01_001DAF00[196] = {0};
P3_2b3628 D_L02_001CF500[196] = {0};
P3_2b3628 D_L03_001CF000[196] = {0};
P3_2b3628 D_L04_001CF080[196] = {0};
P3_2b3628 D_L05_001CF3A0[196] = {0};
P3_2b3628 D_L06_001CF680[196] = {0};
P3_2b3628 D_L07_001CEF80[196] = {0};
P3_2b3628 D_L08_001CF700[196] = {0};
P3_2b3628 D_L09_001CF080[196] = {0};
P3_2b3628 D_L10_001CF380[196] = {0};
P3_2b3628 D_L11_001CF900[196] = {0};
P3_2b3628 D_L12_001E1FA0[196] = {0};
P3_2b3628 D_L13_001CF810[196] = {0};
P3_2b3628 D_L14_001DBEC0[196] = {0};
P3_2b3628 D_L15_001CF980[196] = {0};
P3_2b3628 D_L16_001CF380[196] = {0};
P3_2b3628 D_L17_001CFC00[196] = {0};
P3_2b3628 D_L18_001CF900[196] = {0};
#define RIBBON_COLORS { \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, \
    0x20808080, 0x80808080, 0x80808080, 0x80808080, 0x20808080, 0x80808080, 0x80808080, 0x80808080, \
    0x20808080, 0x80808080, 0x80808080, 0x80808080, 0x20808080, 0x80808080, 0x80808080, 0x80808080, \
    0x20808080, 0x80808080, 0x20808080, 0x80808080, 0x8808080, 0x80808080, 0x20808080, 0x20808080, \
    0x8808080, 0x80808080, 0x20808080, 0x20808080, 0x8808080, 0x80808080, 0x20808080, 0x20808080, \
    0x8808080, 0x80808080, 0x20808080, 0x20808080, 0x8808080, 0x80808080, 0x20808080, 0x20808080, \
    0x20808080, 0x8808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, \
    0x808080, 0x808080, 0x808080, 0x8808080, 0x808080, 0x20808080, 0x808080, 0x8808080, \
    0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, \
    0x808080, 0x8808080, 0x808080, 0x20808080, 0x808080, 0x8808080, 0x808080, 0x808080, \
    0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x8808080, \
    0x808080, 0x20808080, 0x808080, 0x8808080, 0x808080, 0x808080, 0x808080, 0x808080, \
    0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x8808080, 0x808080, 0x20808080, \
    0x808080, 0x8808080, 0x808080, 0x20808080, 0x808080, 0x808080, 0x808080, 0x808080, \
    0x808080, 0x808080, 0x808080, 0x808080 \
}
int D_L00_001DA6D0[196] = RIBBON_COLORS;
int D_L01_001DAB50[196] = RIBBON_COLORS;
int D_L02_001CF150[196] = RIBBON_COLORS;
int D_L03_001CEC50[196] = RIBBON_COLORS;
int D_L04_001CECD0[196] = RIBBON_COLORS;
int D_L05_001CEFF0[196] = RIBBON_COLORS;
int D_L06_001CF2D0[196] = RIBBON_COLORS;
int D_L07_001CEBD0[196] = RIBBON_COLORS;
int D_L08_001CF350[196] = RIBBON_COLORS;
int D_L09_001CECD0[196] = RIBBON_COLORS;
int D_L10_001CEFD0[196] = RIBBON_COLORS;
int D_L11_001CF550[196] = RIBBON_COLORS;
int D_L12_001E1BF0[196] = RIBBON_COLORS;
int D_L13_001CF460[196] = RIBBON_COLORS;
int D_L14_001DBB10[196] = RIBBON_COLORS;
int D_L15_001CF5D0[196] = RIBBON_COLORS;
int D_L16_001CEFD0[196] = RIBBON_COLORS;
int D_L17_001CF850[196] = RIBBON_COLORS;
int D_L18_001CF550[196] = RIBBON_COLORS;
/* Draws the moby's beam: orients the moby along its child's two joints, draws three quads and a ribbon of 196 points. */
void FUN_L00_002b3628(struct Moby *m) {
    Quad_2b3628 pk;
    Vec4A_2b3628 mtx[4];
    Vec4A_2b3628 out[2];
    Ids_2b3628 ids;
    Vec4A_2b3628 up;
    Vec4A_2b3628 res[4];
    Vec4A_2b3628 idm[4];
    Vec4A_2b3628 a;
    Vec4A_2b3628 b;
    Vec4A_2b3628 c;
    Vec4A_2b3628 d;
    Vec4A_2b3628 e;
    Data_2b3628 *data = (Data_2b3628 *)m->pvars;
    Child_2b3628 *child = data->child;
    int i;

    if (child == 0 || child->cls != 0x260) {
        return;
    }
    pk.tag[2] = 0xFF9000000260;
    pk.tag[3] = 0x8000000048;
    pk.tag[0] = 0;
    if (data->f4 != 0) {
        ids = D_L00_00161488;
        transform_joint_points_2b3628(child, 2, &ids, out);
    } else {
        ids = D_L00_00161490;
        transform_joint_points_2b3628(child, 2, &ids, out);
    }
    *(Q_2b3628 *)&up = 0;
    up.y = 1.0f;
    subtract_vector_xyz_2b3628(&mtx[2], &out[0], &out[1]);
    normalize_vector_xyz_2b3628(&mtx[2], &mtx[2], 1.0f);
    mtx[2].w = 0.0f;
    fast_vec_cross_2b3628(&mtx[0], &up, &mtx[2]);
    normalize_vector_xyz_2b3628(&mtx[0], &mtx[0], 1.0f);
    mtx[0].w = 0.0f;
    fast_vec_cross_2b3628(&mtx[1], &mtx[2], &mtx[0]);
    mtx[1].w = 0.0f;
    qcopy(&mtx[3], &out[1]);
    mtx[3].w = 1.0f;
    qcopy(&m->rot_row0, &mtx[0]);
    qcopy(&m->rot_row1, &mtx[1]);
    qcopy(&m->rot_row2, &mtx[2]);
    qcopy(&m->pos, &mtx[3]);
    func_001FA190_2b3628(idm);
    idm[3].w = 1.0f;
    idm[3].z = -0.075f;
    idm[3].x = 0.0f;
    idm[3].y = 0.0f;
    func_001FA540_2b3628(res, mtx, idm);
    pk.tag[1] = get_effect_texture_2b3628(0xC);
    for (i = 0; i < 4; i++) {
        pk.col[i] = 0x80808080;
        if (i == 0 || i == 2) {
            scale_vector_xyz_2b3628(pk.pos[i], D_L00_001DA9E0[i], data->f2C);
        } else {
            float *v = pk.pos[i];
            scale_vector_xyz_2b3628(v, D_L00_001DA9E0[i], data->f2C);
            v[2] -= random_float_between_2b3628(0.0f, 0.1f);
        }
        pk.uv[i][0] = D_L00_001DAA60[i][0];
        pk.uv[i][1] = D_L00_001DAA60[i][1];
    }
    draw_geometry_quad_2b3628(&pk, res, 0);
    for (i = 0; i < 4; i++) {
        if (i == 0 || i == 2) {
            scale_vector_xyz_2b3628(pk.pos[i], D_L00_001DAA20[i], data->f2C);
        } else {
            float *v = pk.pos[i];
            scale_vector_xyz_2b3628(v, D_L00_001DAA20[i], data->f2C);
            v[2] -= random_float_between_2b3628(0.0f, 0.1f);
        }
    }
    draw_geometry_quad_2b3628(&pk, res, 0);
    pk.tag[1] = get_effect_texture_2b3628(0xB);
    scale_vector_xyz_2b3628(&d, &mtx[2], -0.13f);
    add_vector_xyz_2b3628(&d, &d, &mtx[3]);
    subtract_vector_xyz_2b3628(&a, D_L00_00166DC0_2b3628, &d);
    normalize_vector_xyz_2b3628(&a, &a, 1.0f);
    fast_vec_cross_2b3628(&b, &a, &hero.gravity_dir);
    normalize_vector_xyz_2b3628(&b, &b, -1.0f);
    fast_vec_cross_2b3628(&c, &b, &a);
    pk.col[0] = pk.col[1] = pk.col[2] = pk.col[3] = 0x7F2020FF;
    for (i = 0; i < 4; i++) {
        scale_vector_xyz_2b3628(pk.pos[i], D_L00_001DA9E0[i], 0.1f);
        transform_vector_2b3628(pk.pos[i], pk.pos[i], &a);
    }
    draw_geometry_quad_2b3628(&pk, 0, 0);
    vu1_add_g_sregister_2b3628(6, get_effect_texture_2b3628(0xA));
    vu1_add_g_sregister_2b3628(0x42, pk.tag[3]);
    vu1_add_g_sregister_2b3628(8, 0);
    vu1_add_g_sregister_2b3628(0x14, 0xFF9000000260);
    vu1_add_g_sregister_2b3628(0x4A, 0);
    font_queue_vu_state_2b3628();
    for (i = 0; i < 0xC4; i++) {
        scale_vector_xyz_2b3628(&e, D_L00_001D9470[i], data->f20);
        transform_vector_2b3628(&e, &e, mtx);
        D_L00_001DAA80[i].x = e.x;
        D_L00_001DAA80[i].y = e.y;
        D_L00_001DAA80[i].z = e.z;
        D_L00_001DA0B0[i].v -= 0.025f;
        if (D_L00_001DA0B0[i].v < -7.0f) {
            D_L00_001DA0B0[i].v += 7.0f;
        }
    }
    func_L00_001FDE48_2b3628(0x94, D_L00_001DAA80, D_L00_001DA6D0, D_L00_001DA0B0, 1);
    func_L00_001FDE48_2b3628(0x32, &D_L00_001DAA80[0x92], &D_L00_001DA6D0[0x92], &D_L00_001DA0B0[0x92], 1);
}
typedef struct {
    f32 f[4];
} __attribute__((aligned(16))) V4;
typedef struct N {
    s32 f00;
    u8 p04[8];
    s32 f0C;
    V4 v10;
} N;
typedef struct X {
    u8 p00[0x10];
    V4 v10;
    u8 p20[0x58];
    N *f78;
    u8 p7C[0x44];
    V4 vC0;
} X;
typedef struct {
    u8 p0[0x2084];
    s32 i2084;
} G;

extern s32 D_L00_0015F5CC;
extern f32 D_0015ED6C_c __asm__("D_0015ED6C");

extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern void FUN_001f9cf8(V4 *, V4 *, V4 *);
extern s32 FUN_001f96f8_c2(s32) __asm__("FUN_001f96f8");
extern u8 *FUN_L00_0026cbb0(V4 *, V4 *, s32, s32, s32, s32, f32);
extern f32 fast_vec_dist(V4 *, V4 *) __asm__("FUN_001f9b48");
extern void FUN_001f9a28_c(V4 *, V4 *, V4 *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(V4 *, V4 *, f32) __asm__("FUN_001f9bf8");
extern void FUN_001f9a10_c(V4 *, V4 *, V4 *) __asm__("FUN_001f9a10");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern u8 *FUN_L00_0026d000(V4 *, f32, f32, f32, f32, s32, V4 *, u32);

void FUN_L00_002b3c50(X *x) {
    V4 l0;
    V4 a;
    V4 d;
    V4 t;
    N *g;
    f32 s;
    f32 q;
    u32 col;
    s32 i;
    s32 n;
    s32 m;
    u8 *p;

    g = x->f78;
    l0.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    l0.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    l0.f[2] = 0.0f;
    l0.f[2] = -random_float_between(0.01f, 0.03f);
    FUN_001f9cf8(&l0, &l0, &x->vC0);
    qcopy(&a, &x->v10);
    if (hero.state.current == 0x81) {
        if (D_L00_0015F5CC % 3 == 0) {
            FUN_L00_0026cbb0(&a, &l0, 0x4F007FFF, 0x1FFFFFFF, FUN_001f96f8_c2(10), 1, 10000.0f);
        }
    } else {
        FUN_L00_0026cbb0(&a, &l0, 0x4F007FFF, 0x1FFFFFFF, FUN_001f96f8_c2(10), 1, 10000.0f);
    }
    if (g->f0C) {
        s = fast_vec_dist(&g->v10, &a) * 0.5f;
        FUN_001f9a28_c(&d, &g->v10, &a);
        for (i = 1; i < 2; i++) {
            fast_vec_normalize(&t, &d, s * (f32)i);
            FUN_001f9a10_c(&t, &a, &t);
            FUN_L00_0026cbb0(&t, &l0, 0x4F007FFF, 0x1FFFFFFF, FUN_001f96f8_c2(10), 1, 10000.0f);
        }
    }
    g->f00 = random_integer_below(FUN_001f96f8_c2(10));
    q = 1.075f;
    col = 0x808080;
    l0.f[0] = 0.0f;
    l0.f[1] = 0.0f;
    l0.f[2] = -0.3f;
    l0.f[3] = 1.0f;
    FUN_001f9cf8(&l0, &l0, &x->vC0);
    FUN_001f9a10_c(&a, &x->v10, &l0);
    l0.f[3] = 1.0f;
    l0.f[2] = -0.075f;
    l0.f[0] = 0.0f;
    l0.f[1] = 0.0f;
    FUN_001f9cf8(&l0, &l0, &x->vC0);
    l0.f[2] = 0.025f;
    if (hero.state.current == HERO_STATE_LAND) {
        col = 0xA0A0A0;
        q = 1.0165f;
        l0.f[2] -= D_0015ED6C_c * 5.0f;
    }
    if (hero.state.current == 0x81) {
        col = 0xA0A0A0;
        q = 1.037f;
        l0.f[2] -= D_0015ED6C_c * 3.0f;
    }
    p = FUN_L00_0026d000(&a, 0.05f, 1.01f, q, 30000.0f, 6, &l0, col);
    if (p)
        p[3] = 0x44;
    if (g->f0C) {
        s = fast_vec_dist(&g->v10, &a) * 0.16667f;
        FUN_001f9a28_c(&d, &g->v10, &a);
        m = hero.state.current;
        n = 4;
        if (m == 8)
            n = 2;
        if (m == 0x81)
            n = 1;
        for (i = 1; i < n; i++) {
            fast_vec_normalize(&t, &d, s * (f32)i);
            FUN_001f9a10_c(&t, &a, &t);
            p = FUN_L00_0026d000(&t, 0.05f, 1.01f, q, 30000.0f, 6, &l0, col);
            if (p)
                p[3] = 0x44;
        }
    }
    qcopy(&g->v10, &a);
    g->f0C = 1;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B5428), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);

/* Creates a class 0xA7 moby carrying a in its data; sets bytes 0x30/0x31, 0x32 and state 0. */
char *FUN_L00_002b4138(int a) {
    char *m = func_0020D348_m(0xA7);
    if (m != 0) {
        int *p = *(int **)(m + 0x78);
        p[1] = a;
        p[0] = 0;
        p[2] = 0;
        p[3] = 0;
        *(unsigned char *)(m + 0x30) = 0xFF;
        *(short *)(m + 0x32) = 0x7E;
        *(unsigned char *)(m + 0x31) = 0;
        *(unsigned char *)(m + 0x20) = 0;
    }
    return m;
}
typedef union {
    OvlQuad q;
    float f[4];
} V_2b46a8;
extern unsigned char *D_L00_0015FFE4_2b46a8 __asm__("D_L00_0015FFE4");
extern unsigned char D_L00_00166DC0_2b46a8[] __asm__("D_L00_00166DC0")
    __attribute__((section(".data")));
extern unsigned char *FUN_002141f8_2b46a8(void *) __asm__("FUN_002141f8");
extern float FUN_001f9e90_2b46a8(float, float) __asm__("FUN_001f9e90");
extern float FUN_001f9b80_2b46a8(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9b48_2b46a8(void *, void *) __asm__("FUN_001f9b48");
extern float FUN_001fa688_2b46a8(float, float) __asm__("FUN_001fa688");
extern float FUN_001f9df8_2b46a8(float) __asm__("FUN_001f9df8");
extern float FUN_001fa6c0_2b46a8(int) __asm__("FUN_001fa6c0");
extern void FUN_00214db0_2b46a8(void *, float, float, float) __asm__("FUN_00214db0");
extern void FUN_001f9a10_2b46a8(void *, void *, void *) __asm__("FUN_001f9a10");
extern int FUN_001efa68_2b46a8(void *, void *, int, int, int) __asm__("FUN_001efa68");

unsigned char *FUN_L00_002b46a8(int ign, V_2b46a8 *from, float *yaw, float *pitch) {
    V_2b46a8 src;
    V_2b46a8 tp;
    V_2b46a8 w;
    unsigned char *m;
    unsigned char *r;
    unsigned char *best;
    float bestd;
    float a;
    float b;
    float dy;
    float d;
    float e;
    float s;
    float t;
    struct Hero *g;

    best = 0;
    src.q = from->q;
    from = &src;
    bestd = 20.0f;
    for (m = D_L00_0015FFE4_2b46a8; m != 0; m = *(unsigned char **)(m + 0x28)) {
        if ((signed char)m[0x20] < 0) {
            continue;
        }
        qcopy(&tp, m + 0x10);
        r = FUN_002141f8_2b46a8(m);
        if (r != 0) {
            tp.f[2] += *(float *)(r + 0x10);
        } else {
            tp.f[2] += 0.5f;
        }
        if (!(*(unsigned short *)(m + 0x34) & 0x1000)) {
            continue;
        }
        a = FUN_001f9e90_2b46a8(tp.f[0] - from->f[0], tp.f[1] - from->f[1]);
        b = FUN_001f9e90_2b46a8(FUN_001f9b80_2b46a8(from, &tp), tp.f[2] - from->f[2]);
        dy = FUN_001fa688_2b46a8(*yaw, a);
        d = FUN_001f9b48_2b46a8(from, &tp);
        if (bestd < d) {
            continue;
        }
        g = &hero;
        FUN_00214db0_2b46a8(&w, d, *yaw, *pitch + g->ground_pitch.f * 0.5f);
        FUN_001f9a10_2b46a8(&w, &w, from);
        e = FUN_001f9df8_2b46a8(FUN_001f9b48_2b46a8(&w, &tp) / (d + d));
        e = e + e;
        if (0.15707964f < e) {
            r = FUN_002141f8_2b46a8(m);
            if (r != 0) {
                s = FUN_001fa6c0_2b46a8(r[0xA]) * 0.125f;
                t = FUN_001f9df8_2b46a8(s / d);
                if (s < d) {
                    e -= t;
                    if (e < 0.0f) {
                        e = 0.0f;
                    }
                }
            }
        }
        if (e < 0.15707964f) {
            if (FUN_001efa68_2b46a8(D_L00_00166DC0_2b46a8, &tp, 6, ign, 0) ||
                FUN_001efa68_2b46a8(from, &tp, 6, ign, 0)) {
                return best;
            }
            *pitch = -b;
            bestd = d;
            if (dy < 0.17453292f) {
                *yaw = a;
            }
            best = m;
        }
    }
    return best;
}
/* Blaster (class 168) update: aims (reticle in first person, auto-aim otherwise), fires a shot with its sparks,
   keeps the usage statistics, returns to idle and moves the muzzle light. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B5C98), where it is exact; names translated to the US level program. */
/* NEAR: a dozen spots; this compiler hoists the 3.0f spray constant out of the loop and rebuilds the zero target before its conditional move. */

typedef struct Hero Hero;


typedef struct { u8 p0[0x2084]; s32 i2084; } G_B4F40_49a8;

typedef union { struct { float x, y, z, w; } f; u128 q; } BlasterVec __attribute__((aligned(16)));

typedef struct { BlasterVec row[3]; } BlasterMatrix3;

typedef struct { BlasterVec row[4]; } BlasterMatrix4;

typedef struct BlasterVars {
    int cooldown;
    short lock;
    char pad6[0xA];
    int light;
    int glow;
} BlasterVars;

#include "rnc/input/pad_state.h"

typedef struct BlasterUsage { char pad0[0x78]; unsigned short uses; unsigned short longest; int levels; } BlasterUsage;

typedef struct AimTarget { char pad0[0x10]; float height; } AimTarget;

typedef struct PointLight { float red, green, blue, pad_C; BlasterVec pos; } PointLight;

typedef struct ShotSpawn {
    BlasterVec pos;
    struct Moby *owner;
    int flags;
    unsigned char b18;
    unsigned char b19;
    unsigned short cls;
    float f1C;
    int i20;
} ShotSpawn;

typedef union BlasterWork {
    BlasterMatrix3 rotation;
    BlasterVec point;
    ShotSpawn spawn;
    struct { BlasterVec v; BlasterVec rnd; } spray;
} BlasterWork;

char *FUN_L00_0026dd20(void *pos, void *dir, int a, int b, float f);
extern G_B4F40_49a8 D_0013F350;
extern char D_00141680_49a8[] NOT_SDA;
extern char D_L00_00166C80[];
#include "rnc/rendering/debug/debug_menu.h"
extern f32 D_0015ED6C MACRO_ADDR;
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int D_0013E520_49a8 __asm__("D_0013E520"); /* no foreign declaration */
extern int D_0015ED84 MACRO_ADDR;
extern int D_0015EEA4 MACRO_ADDR;
extern int D_L00_0015F5C4; /* no foreign declaration */
extern int D_L00_00173E58; /* no foreign declaration */
extern float D_L00_0017ACF4 __attribute__((section(".data")));
extern unsigned char D_L00_001802C0[];
extern int coll_line_fix(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int transpose_rotation_matrix() __asm__("FUN_001fa2d8");
extern int FUN_L00_00233db8(int, int);
extern int FUN_L00_00233e98();
extern int FUN_L00_00257b90(int, int);
extern int advance_moby_animation(void *) __asm__("func_0020D580");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(int *arg0) __asm__("FUN_001f9740");
extern u32 D_0013C940[];
extern unsigned char *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern void collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_002223f8(int, int);
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_49a8(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void set_moby_animation(void *, s32, s32) __asm__("func_00212ED8");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
float absolute_float(float input) __asm__("func_001F99C0");
int FUN_L00_0023e738(void *v, float a, float b_0023e738, float c, float d, float e);
struct Moby *FUN_L00_002caf70(struct Moby *source, OvlVec4 *position, struct Moby *other, f32 vx, f32 vy, f32 height, f32 speed);
void FUN_L00_0020e698(void);
void FUN_L00_00233660(float *out, float x, float y, float z);
void FUN_L00_0023e838(int);
extern unsigned char item_usage[] __asm__("D_00141680");
extern char player_camera[] __asm__("D_L00_00166C80");
extern s32 D_L00_0015F5C4_macro __asm__("D_L00_0015F5C4") MACRO_ADDR;
extern u8 item_text_variant[] __asm__("D_0013E520");
extern struct Moby *coll_hit_moby __asm__("D_L00_00173E58") __attribute__((section(".data")));
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_L00_001edf60_49a8(int, int, float, void *, float, int, int, float, int) __asm__("FUN_L00_001edf60");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern struct Moby *FUN_L00_002b46a8_49a8(struct Moby *, void *, float *, float *) __asm__("FUN_L00_002b46a8");
extern int get_ammo(int) __asm__("FUN_L00_00233e98");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern int use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_002caf70_49a8(void *, float, float, float, float, void *, void *) __asm__("FUN_L00_002caf70");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");

void blaster_update(struct Moby *moby) __asm__("FUN_L00_002b49a8");
void blaster_update(struct Moby *moby) {
    BlasterVec velocity;
    BlasterVec aim_point;
    BlasterVec launch;
    BlasterWork work;
    BlasterMatrix4 inverse;
    BlasterVec reach;
    BlasterVec end;
    struct Moby *target;
    float yaw;
    float pitch;
    float base_yaw, base_pitch;
    float red, green, blue;
    BlasterVars *data;
    BlasterUsage *stats;
    int i;
    int never;

    velocity.q = 0;
    target = 0;
    never = 0;
    pitch = 0.0f;
    data = (BlasterVars *)moby->pvars;
    if (moby->state == 0) {
        moby->state = 1;
        data->light = -1;
        data->glow = 0;
    }
    fast_dec_timer_s16(&data->lock);
    fast_dec_timer(&data->cooldown);
    if (hero.state.current == 1) hero_set_state(30, 1);
    if (moby->prev_seq == 0 && moby->prev_frame < 3) data->cooldown = scale_game_frames(7);
    if (hero.state.current == 30 && (hero.moby->flags & 1)) {
        copy_matrix3x4(&work.rotation, player_camera);
        transpose_rotation_matrix(&inverse, &work.rotation);
        velocity.f.z = frame_time * 40.0f;
        transform_vector(&velocity, &velocity, &inverse);
        clear_u64_value(&launch);
        launch.f.x += 0.15f;
        launch.f.z += 0.75f;
        launch.f.y += 0.15f;
        transform_vector(&launch, &launch, &inverse);
        fast_vec_add(&launch, &launch, player_camera + 0x140);
        if (hero.state.current == 30 && (hero.moby->flags & 1) && !hero.unk20AC) {
            FUN_L00_001edf60_49a8((int)moby, 0xFF0F0FFF, 1.0f, 0, 0.0f, 0x26, -1, 90.0f, 4);
            fast_vec_normalize(&reach, &velocity, 20.0f);
            fast_vec_add(&end, &reach, &launch);
            reach.q = end.q;
            /* Fakematch: a never-taken set of target on this path keeps gcse from
               folding target's 0 into the hit test below (retail keeps it in $fp). */
            if (never) target = (struct Moby *)moby->pvars;
            if (coll_line_fix(player_camera + 0x140, &reach, 0, (int)moby, 0)) {
                struct Moby *hit = coll_hit_moby;
                if (hit && (hit->flags & 0x1000)) target = hit;
            }
        }
    } else {
        velocity.f.x = frame_time * 40.0f;
        transform_vector_by_basis(&velocity, &velocity, &hero.basis);
        FUN_L00_0024f7c8(moby, 0, &launch);
    }
    if (hero.items[6].unk40) yaw = hero.items[6].unk4C;
    else yaw = fast_arc_tan(velocity.f.x, velocity.f.y);
    pitch = -fast_arc_tan(vector_length_xy(&velocity), velocity.f.z);
    base_yaw = yaw;
    base_pitch = pitch;
    if (hero.state.current != 30) {
        if (hero.unk22C8 == 1) {
            yaw = moby->rot.z;
            base_yaw = yaw;
            pitch = D_L00_0017ACF4;
            base_pitch = pitch;
        }
        target = FUN_L00_002b46a8_49a8(moby, &launch, &yaw, &pitch);
    }
    if (target && data->lock == 0) {
        if (get_ammo(-1)) {
            AimTarget *aim = (AimTarget *)suck_get_aim_height(target);
            float distance = fast_vec_dist(g_debug_camera.position, &target->pos) / 50.0f;
            float height;
            qcopy(&work.point, &target->pos);
            height = 0.5f;
            if (aim) height = aim->height;
            work.point.f.z += height;
            FUN_L00_001edf60_49a8((int)moby, 0xFF0FFF0F, absolute_float(0.9f - distance * 0.65f), &work.point, 0.0f, 0x26, -1, 90.0f, 4);
        }
    }
    if (hero.unk20A8 && hero.state.current != 0x72 && D_L00_0015F5C4_macro != 2) {
        if (scale_game_frames(5 - item_text_variant[15]) < hero.unk1BC && data->cooldown == 0) {
            if (use_ammo(-1, 1) && !hero.unk20AC) {
                data->cooldown = scale_game_frames(6);
                if (target) {
                    float speed = frame_time * 40.0f;
                    AimTarget *aim;
                    qcopy(&aim_point, &target->pos);
                    aim = (AimTarget *)suck_get_aim_height(target);
                    if (aim) aim_point.f.z += aim->height;
                    else aim_point.f.z += 0.5f;
                    pitch = -fast_arc_tan(vector_distance(&launch, &aim_point), aim_point.f.z - launch.f.z);
                    base_pitch -= hero.ground_pitch.f * 0.5f;
                    if (fast_difference_between_rotations(base_pitch, pitch) < 0.17453292f) {
                        base_pitch = pitch;
                    } else if (fast_add_rotations(base_pitch, 0.17453292f) < pitch) {
                        base_pitch = fast_add_rotations(base_pitch, 0.17453292f);
                    } else if (pitch < fast_add_rotations(base_pitch, -0.17453292f)) {
                        base_pitch = fast_add_rotations(base_pitch, -0.17453292f);
                    }
                    build_spherical_offset(&work.point, speed, base_yaw, -base_pitch);
                    fast_vec_add(&work.point, &work.point, &launch);
                    yaw = fast_arc_tan(aim_point.f.x - work.point.f.x, aim_point.f.y - work.point.f.y);
                    pitch = -fast_arc_tan(vector_distance(&work.point, &aim_point), aim_point.f.z - work.point.f.z);
                } else if (hero.state.current != 30) {
                    pitch -= hero.ground_pitch.f * 0.5f;
                    base_pitch -= hero.ground_pitch.f * 0.5f;
                }
                work.spawn.flags = 0x10000;
                work.spawn.owner = moby;
                work.spawn.f1C = 0.25f;
                work.spawn.i20 = 1;
                work.spawn.pos.f.x = fast_cos(fast_arc_tan(hero.unk670, hero.unk674));
                work.spawn.pos.f.y = fast_sin(fast_arc_tan(hero.unk670, hero.unk674));
                work.spawn.pos.f.z = 1.0f;
                work.spawn.pos.f.w = 5627.9248f;
                work.spawn.b18 = 1;
                work.spawn.b19 = 1;
                work.spawn.cls = (*(unsigned short *)&moby->oclass);
                FUN_L00_00233660(&inverse, 0.7f, -0.15f, 0.5f);
                collect_mobies_in_sphere(&inverse, 0.42f, 5, hero.moby, &work.spawn);
                FUN_L00_002caf70_49a8(hero.moby, yaw, pitch, base_yaw, base_pitch, &launch, target);
                data->glow = scale_game_frames(15);
                for (i = 0; i < 5; i++) {
                    work.spray.rnd.q = 0;
                    work.spray.rnd.f.x = random_float_between(-1.0f, 1.0f);
                    work.spray.rnd.f.y = random_float_between(-1.0f, 1.0f);
                    work.spray.rnd.f.z = random_float_between(-1.0f, 1.0f);
                    work.spray.v.q = work.spray.rnd.q;
                    fast_vec_normalize(&work.spray.v, &work.spray.v, fast_vec_length(&velocity) * 0.65f);
                    fast_vec_add(&work.spray.v, &velocity, &work.spray.v);
                    fast_vec_normalize(&work.spray.v, &work.spray.v, random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                    FUN_L00_0026dd20(&launch, &work.spray.v, 0x5F2F4F6F,
                        random_int_between(scale_game_frames(5), scale_game_frames(10)), 10000.0f);
                }
                fast_vec_normalize(&velocity, &velocity, frame_time * 0.5f);
                FUN_L00_0026dd20(&launch, &velocity, 0x2F4F7F7F,
                    random_int_between(scale_game_frames(4), scale_game_frames(7)), 200000.0f);
            }
            if (moby->prev_seq != 4) blend_moby_animation(moby, 4, 0, scale_game_frames(7));
            stats = (BlasterUsage *)item_usage;
            if (stats->uses < 65535) stats->uses++;
            if (scale_game_frames(D_0015EEA4) / 600 > stats->longest)
                stats->longest = scale_game_frames(D_0015EEA4) / 600;
            stats->levels = (stats->levels | (1 << current_level_index)) | 0x80000000;
        }
    }
    if (!(((controller_state.held & hero.items[0].button_mask) && get_ammo(-1)) ||
          (!(scale_game_frames(9) < hero.unk1BC) && hero.unk20A8)) ||
        hero.state.current == 0x72 || D_L00_0015F5C4_macro == 2) {
        if ((moby->prev_seq != 0 || (moby->anim_flags & 2)) && hero.state.current != 0x72 && D_L00_0015F5C4_macro != 2) {
            if (moby->prev_seq != 1) blend_moby_animation(moby, 1, 0, scale_game_frames(7));
        } else if (hero.state.current == 0x72 || D_L00_0015F5C4_macro == 2) {
            set_moby_animation(moby, 1, 0);
            advance_moby_animation(moby);
        }
        FUN_L00_0020e698();
        data->cooldown = 0;
    }
    if (moby->seq != 2 && data->glow > 0) {
        red = convert_integer_to_float(data->glow < scale_game_frames(9) ? data->glow : scale_game_frames(9)) * 0.7f / (float)scale_game_frames(9);
        green = convert_integer_to_float(data->glow < scale_game_frames(9) ? data->glow : scale_game_frames(9)) * 0.7f / (float)scale_game_frames(9);
        blue = convert_integer_to_float(data->glow < scale_game_frames(9) ? data->glow : scale_game_frames(9)) * 0.7f / (float)scale_game_frames(9);
        work.point.f.x = hero.motion.pos.f[0] + fast_cos(fast_arc_tan(hero.unk670, hero.unk674)) * 2.0f;
        work.point.f.y = hero.motion.pos.f[1] + fast_sin(fast_arc_tan(hero.unk670, hero.unk674)) * 2.0f;
        work.point.f.z = hero.motion.pos.f[2] + 1.0f;
        if (data->light == -1) {
            data->light = FUN_L00_0023e738(&work.point, 6.0f, 0.0f, red, green, blue);
        } else {
            PointLight *light = &((PointLight *)D_L00_001802C0)[data->light];
            qcopy(&light->pos, &work.point);
            light->pos.f.w = 6.0f;
            light->red = red;
            light->green = green;
            light->blue = blue;
        }
        fast_dec_timer(&data->glow);
    } else if (data->light != -1) {
        FUN_L00_0023e838(data->light);
        data->light = -1;
        data->glow = 0;
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B69C0), where it is exact; names translated to the US level program. */

extern int D_L00_0015F30C;
extern unsigned char D_L00_0015F444_e[] __asm__("D_L00_0015F444")
    __attribute__((section(".sdata")));
extern float D_L00_0015F448;
extern float D_L00_0015F448_e[] __asm__("D_L00_0015F448") __attribute__((section(".sdata")));
extern int D_L00_00169FF8 __attribute__((section(".data")));
extern void PackDmaTag(s32 arg0, unsigned long arg1, unsigned long arg2);

void FUN_L00_002b56d0(void *m_) {
    char *m = (char *)m_;
    char *p;
    float f0, f1, f2, f3;

    p = *(char **)(m + 0x78);
    f0 = D_L00_0015F448;
    f1 = D_L00_0015F44C;
    f2 = D_L00_0015F450;
    f3 = D_L00_0015F448_e[3];
    D_L00_0015F30C = 1;
    *(float *)(p + 0x30) = f0;
    *(float *)(p + 0x34) = f1;
    *(float *)(p + 0x38) = f2;
    *(float *)(p + 0x3C) = f3;
    p[0x40] = D_L00_0015F444_e[0];
    p[0x41] = D_L00_0015F444_e[1];
    p[0x42] = D_L00_0015F444_e[2];
    D_L00_0015F444_e[0] = 0x40;
    D_L00_0015F448 = 0;
    D_L00_0015F444_e[1] = 0x60;
    D_L00_0015F454 = 0;
    D_L00_0015F444_e[2] = 0x40;
    D_L00_00160FE0 = 144.0f;
    D_L00_0015F44C = 131072.0f;
    D_L00_0015F450 = 255.0f;
    D_L00_00169FF8 = 0;
    D_L00_00160F80 = 147456.0f;
    D_L00_001604A4 = 144.0f;
    D_L00_0015FFF0 = 0x90;
    D_L00_0016017C = 0x50000;
    PackDmaTag(0x40, 0x60, 0x40);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B6AE0), where it is exact; names translated to the US level program. */

extern char D_L00_0016CA40[] __attribute__((section(".data")));
extern unsigned char D_L00_0015F444;
extern float D_L00_0015F448;
extern void set_background_color(int, int, int) __asm__("FUN_001fb280");

void FUN_L00_002b57f0(int m_, int flag) {
    char *m = (char *)m_;
    char *p;
    float f0, f1, f2, f3;
    char *d;

    p = *(char **)(m + 0x78);
    D_L00_0015F30C = 0;
    f0 = *(float *)(p + 0x30);
    f1 = *(float *)(p + 0x34);
    f2 = *(float *)(p + 0x38);
    f3 = *(float *)(p + 0x3C);
    D_L00_0015F448 = f0;
    D_L00_0015F44C = f1;
    D_L00_0015F450 = f2;
    D_L00_0015F448_e[3] = f3;
    D_L00_0015F444 = p[0x40];
    D_L00_0015F445 = p[0x41];
    D_L00_0015F446 = p[0x42];
    D_L00_00169FF8 = 1;
    if (flag != 0) {
        D_L00_00160F80 = 512000.0f;
        D_L00_00160FE0 = 720.0f;
        D_L00_001604A4 = 500.0f;
        D_L00_0015FFF0 = 500;
        D_L00_0016017C = 0x1F4000;
    }
    d = D_L00_0016CA40;
    set_background_color(*(int *)(d + 0x23C), *(int *)(d + 0x240), *(int *)(d + 0x244));
}
typedef union {
    OvlQuad q;
    float f[4];
} V_2b58d8;
extern float D_0015ED6C_2b58d8 __asm__("D_0015ED6C");
extern OvlQuad D_L00_00173E60_2b58d8 __asm__("D_L00_00173E60") __attribute__((section(".data")));
unsigned char *FUN_0020c4f8_2b58d8(int) __asm__("FUN_0020c4f8");
int FUN_L00_002603d0_2b58d8(void *) __asm__("FUN_L00_002603d0");
float FUN_001f9af0_2b58d8(void *) __asm__("FUN_001f9af0");
int FUN_001f96f8_2b58d8(int) __asm__("FUN_001f96f8");
void FUN_L00_002b56d0_2b58d8(void *) __asm__("FUN_L00_002b56d0");
void FUN_L00_00250df8_2b58d8(void *) __asm__("FUN_L00_00250df8");
int FUN_L00_002eb930_2b58d8(void *) __asm__("FUN_L00_002eb930");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
void FUN_001f97e8_2b58d8(void *, int, int) __asm__("FUN_001f97e8");
void FUN_0020cb10_2b58d8(void *, int, void *) __asm__("FUN_0020cb10");
int FUN_L00_002bb360_2b58d8(void *) __asm__("FUN_L00_002bb360");
void FUN_L00_0025d1b8_2b58d8(void *) __asm__("FUN_L00_0025d1b8");
int FUN_0022da68_2b58d8(int, int, void *) __asm__("FUN_0022da68");
int FUN_001efa68_2b58d8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
void FUN_002151d8_2b58d8(int, int) __asm__("FUN_002151d8");

extern int D_L00_0017E508 __attribute__((section(".data")));
extern int D_L00_0015F608;
extern float D_L00_0016149C __attribute__((sda));
unsigned char *FUN_L00_002b58d8(unsigned char *a, V_2b58d8 *pos, float fa, float fb) {
    unsigned char *m = FUN_0020c4f8_2b58d8(0xAC);
    if (m) {
        unsigned char *v = *(unsigned char **)(m + 0x78);
        struct Hero *G = &hero;
        V_2b58d8 t;
        G->unk1FE0 = m;
        D_L00_0017E508 = 1;
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        m[0x20] = 1;
        *(float *)(m + 0x48) = fa;
        *(float *)(m + 0x44) = fb;
        *(unsigned char **)(v + 0x24) = G->unk1F60;
        *(unsigned char **)(v + 0x28) = G->unk1FA0;
        *(unsigned char **)(v + 0x0) = a;
        qcopy(m + 0x10, pos);
        *(int *)(v + 0x8) = 0;
        *(int *)(v + 0xC) = 0;
        if ((void *)G->ground_moby && FUN_L00_002603d0_2b58d8((void *)G->ground_moby))
            *(float *)(v + 0x4) = FUN_001f9af0_2b58d8(&G->motion.displacement);
        else
            *(int *)(v + 0x4) = 0;
        *(int *)(v + 0x10) = FUN_001f96f8_2b58d8(0xBB8);
        *(int *)(v + 0x18) = 0;
        *(int *)(v + 0x1C) = 0;
        *(int *)(v + 0x14) = 0;
        *(float *)(v + 0x44) = 0.4f;
        FUN_L00_002b56d0_2b58d8(m);
        FUN_L00_00250df8_2b58d8(m);
        *(int *)(v + 0x14) = FUN_L00_002eb930_2b58d8(m);
        hero_set_state(0x1D, 1);
        FUN_001f97e8_2b58d8(*(void **)(v + 0x24), 0, 0x40);
        FUN_001f97e8_2b58d8(*(void **)(v + 0x28), 0, 0x40);
        FUN_0020cb10_2b58d8(m, 0, *(void **)(v + 0x24));
        FUN_0020cb10_2b58d8(m, 1, *(void **)(v + 0x28));
        {
            int r = FUN_L00_002bb360_2b58d8(m);
            float k = D_L00_0016149C * D_0015ED6C_2b58d8;
            *(int *)(v + 0x20) = r;
            *(float *)(v + 0x48) = k;
        }
        FUN_L00_0025d1b8_2b58d8(m);
        *(int *)(v + 0x2C) = FUN_0022da68_2b58d8(0, 4, m);
        t.q = *(OvlQuad *)(a + 0x10);
        t.f[2] = pos->f[2];
        if (FUN_001efa68_2b58d8(&t, pos, 0, ({
                struct Hero *g2 = &hero;
                (void *)g2->moby;
            }),
                                0)) {
            m[0xBC] = 1;
            qcopy(m + 0x10, &D_L00_00173E60_2b58d8);
        }
        D_L00_0015F608 = 1;
        FUN_002151d8_2b58d8(6, 0);
    }
    return m;
}
extern int D_00141330[];
void FUN_L00_002eba18();
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
void FUN_L00_002b57f0_u() __asm__("FUN_L00_002b57f0");
void FUN_L00_00203b78();
void mark_moby_for_removal() __asm__("FUN_0020c828");
/* Clears the moby state word 4 and the 0x14 handle, calls release helpers, marks a for removal. */
void FUN_L00_002b5b20(void *a, char **b) {
    char *x = *(char **)(*b + 0x78);
    if (x)
        *(int *)(x + 4) = 0;
    D_00141330[0] = 0;
    FUN_L00_002eba18(*(int *)((char *)b + 0x14));
    *(int *)((char *)b + 0x14) = 0;
    hero_set_state(HERO_STATE_IDLE, 1);
    FUN_L00_002b57f0_u(a, 1);
    FUN_L00_00203b78();
    mark_moby_for_removal(a);
    D_L00_00167014[0] = 0;
}
typedef unsigned int u128_2b5ba0 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2b5ba0 q;
    float f[4];
} V_2b5ba0;
typedef struct {
    u128_2b5ba0 r[4];
} M_2b5ba0;
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void copy_matrix_rotation(void *, void *) __asm__("FUN_001fa298");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");

void FUN_L00_002b5ba0(unsigned char *m, unsigned char *o) {
    M_2b5ba0 a, b, c;
    V_2b5ba0 k, r;
    unsigned char *e = *(unsigned char **)(o + 0x20);
    unsigned char *t;
    if (e == 0)
        return;
    k.q = 0;
    k.f[1] = 0.11f;
    t = *(unsigned char **)(e + 0x78);
    if (0.45f < *(float *)(t + 0xC)) {
        *(float *)(t + 0xC) = *(float *)(t + 0xC) - D_0015ED60_gp * 0.05f;
    } else if (*(float *)(t + 0xC) < 0.45f) {
        *(float *)(t + 0xC) = *(float *)(t + 0xC) + D_0015ED60_gp * 0.05f;
    }
    *(float *)(*(unsigned char **)(o + 0x20) + 0x48) = frame_scale * -1.5707964f;
    euler_rotation_matrix(&b, *(unsigned char **)(o + 0x20) + 0x40);
    copy_matrix_rotation(&c, m + 0xC0);
    multiply_matrix(&a, &c, &b);
    copy_matrix3x4(*(unsigned char **)(o + 0x20) + 0xC0, &a);
    transform_vector(&r, &k, &a);
    fast_vec_add(*(unsigned char **)(o + 0x20) + 0x10, &r, m + 0x10);
    refresh_moby_spatial_bounds_from_basis(*(unsigned char **)(o + 0x20));
}
/* spawns a burst of sparks and glow flashes at a moby, scaled by its distance from the origin marker */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B6FF0), where it is exact; names translated to the US level program. */

typedef struct {
    int v[6];
} V6;

extern V6 D_L00_001E9A58 __attribute__((section(".data")));
extern V6 D_L00_001E9A70 __attribute__((section(".data")));
extern char D_L00_00166DC0[] __attribute__((section(".data")));
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int FUN_L00_00257b90(int, int);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern unsigned char *FUN_L00_0026b790(void *pos, int spin, int col, float range, float f1,
                                       float f2, float f3, float scale);
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_L00_0026a9f0(void *, void *, int, int, int, int, int, int, float, float);
extern void spawn_glow_particle(void *, void *, void *, int, int, int, int, int, float) __asm__("FUN_L00_002ac910");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L00_002b5d00(char *m, int flag) {
    float tmp[4];
    float d[4];
    float n[4];
    V6 A;
    V6 B;
    float len, extra, sc;
    int count, i, j;

    qcopy(tmp, m + 0x10);
    tmp[2] += 0.3f;
    FUN_L00_0026b790(tmp, 1, 0x40808080, 0.3f, 1.01f, 1.075f, 0.05f, 50000.0f);
    for (j = 4; j >= 0; j--) {
        FUN_L00_0026b790(tmp, 1, 0x40808080, 0.3f, 1.01f, 1.07f, 0.05f, 50000.0f);
    }
    if (flag != 0) {
        subtract_vector_xyz(d, D_L00_00166DC0, m + 0x10);
        clear_u64_value(n);
        len = fast_vec_length(d);
        if (len < 8.0f) {
            count = func_001FA898_r(len) + 2;
        } else {
            count = 10;
        }
        extra = 0.0f;
        if (len < 7.0f)
            extra = 7.0f - len;
        if (count > 0) {
            i = count;
            do {
                int *pa, *pb;
                int a, b;
                sc = random_float_between_alt(8.0f, 10.0f) * frame_time - extra * frame_time;
                A = D_L00_001E9A58;
                B = D_L00_001E9A70;
                pa = A.v + random_integer_below(6);
                pb = B.v + random_integer_below(6);
                a = FUN_L00_00257b90(scale_game_frames(15), scale_game_frames(20));
                b = FUN_L00_00257b90(scale_game_frames(25), scale_game_frames(30));
                FUN_L00_0026a9f0(m + 0x10, n, *pa, *pb, a, b, 0, 0, 400000.0f, sc);
            } while (--i);
        }
        if (D_L00_0015F5D0 < 0.95f) {
            spawn_glow_particle(m, m + 0x10, n, scale_game_frames(15), 0x7F, 0x7F, 0x7F, 0x20, 3.0f);
            spawn_glow_particle(m, m + 0x10, n, scale_game_frames(24), 0x7F, 0x20, 0, 0x20, 2.0f);
        }
        spawn_glow_particle(m, m + 0x10, n, scale_game_frames(20), 0x7F, 0x40, 0, 0x30, 3.0f);
        spawn_glow_particle(m, m + 0x10, n, scale_game_frames(27), 0x60, 0x10, 0, 0x40, 2.0f);
        spawn_glow_particle(m, m + 0x10, n, scale_game_frames(29), 0x20, 0, 0, 0x20, 1.0f);
    }
}
#include "rnc/input/pad_state.h"
extern float D_0015ED6C_2b6140 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern char D_L00_00173E40_2b6140[] __asm__("D_L00_00173E40");

typedef int u128_2b6140 __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V_2b6140;
typedef struct Ev_2b6140 {
    V_2b6140 pos;
    struct Moby *moby;
    int flags;
    unsigned char b18;
    unsigned char b19;
    unsigned short h1A;
    float f1C;
    int f20;
} Ev_2b6140;
typedef struct Part_2b6140 { char p0[0x10]; V_2b6140 pos; } Part_2b6140;
typedef struct Tgt_2b6140 { char p0[0x46]; short h46; char p48[0x5E]; short hA6; } Tgt_2b6140;
typedef struct D_2b6140 {
    int f0;
    float f4;
    char p8[8];
    int f10;
    void *f14;
    float f18;
    float f1C;
    char p20[4];
    Part_2b6140 *f24;
    Part_2b6140 *f28;
    int f2C;
    char p30[0x14];
    float f44;
    float f48;
} D_2b6140;
typedef struct Hit_2b6140 {
    char p0[0x18];
    struct Moby *f18;
    int f1C;
    V_2b6140 v20;
    char p30[0x10];
    V_2b6140 v40;
    V_2b6140 v50;
    V_2b6140 v60;
} Hit_2b6140;
typedef struct Hit2_2b6140 { char p0[0x24]; Tgt_2b6140 *f24; } Hit2_2b6140;
typedef struct Row_2b6140 { char p0[0x74]; unsigned char f74; char p1[0x13]; struct Moby *f88; } Row_2b6140;
extern char D_0013E550_2b6140[] __asm__("D_0013E550");
extern Hit2_2b6140 *D_L00_00173E58_2b6140 __asm__("D_L00_00173E58") __attribute__((section(".data")));
extern char D_L00_00177F00_2b6140[] __asm__("D_L00_00177F00");
extern float D_0015ED60_2b6140 __asm__("D_0015ED60") __attribute__((section(".sdata")));
extern void func_L00_00204130_2b6140(void) __asm__("FUN_L00_00203b18");
extern int force_help_message_2b6140(int, int) __asm__("FUN_002151d8");
extern int scale_game_frames_2b6140(int) __asm__("FUN_001f96f8");
extern int func_L00_002ECAF8_2b6140(void *, void *) __asm__("FUN_L00_002eb648");
extern void func_L00_002B6AE0_2b6140(void *, int) __asm__("FUN_L00_002b57f0");
extern void func_L00_002B6E90_2b6140(void *, void *) __asm__("FUN_L00_002b5ba0");
extern void func_L00_002B6E10_2b6140(void *, void *) __asm__("FUN_L00_002b5b20");
extern void func_L00_002B6FF0_2b6140(void *, int) __asm__("FUN_L00_002b5d00");
extern int truncate_float_to_s32_2b6140(float) __asm__("FUN_001fa6d0");
extern int func_L00_0028F210_2b6140(int, int) __asm__("FUN_L00_0028df38");
extern float fast_subtract_rotations_2b6140(float, float) __asm__("FUN_001fa5c8");
extern void func_L00_0025B040_2b6140(void *, float) __asm__("FUN_L00_00259fe8");
extern void build_spherical_offset_2b6140(void *, float, float, float) __asm__("FUN_00214db0");
extern void add_vector_xyz_2b6140(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_2b6140(void *, void *, void *) __asm__("FUN_001f9a28");
extern float probe_ground_height_2b6140(void *, int, float) __asm__("FUN_00213508");
extern float approach_value_2b6140(float *, float, float) __asm__("FUN_00213ed8");
extern float angle_atan2_2b6140(float, float) __asm__("FUN_001f9e90");
extern float absolute_float_2b6140(float) __asm__("FUN_001f99c0");
extern void func_L00_001FFED8_2b6140(void *, int, float) __asm__("FUN_L00_001fff28");
extern void trooper_scale_2b6140(void *, void *, float) __asm__("FUN_001f9c48");
extern int collision_line_2b6140(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void normalize_vector_xyz_2b6140(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_L00_001FF610_2b6140(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void release_voice_slot_2b6140(int) __asm__("FUN_0022d798");
extern void spawn_explosion_2b6140(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int collect_mobies_in_sphere_2b6140(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void send_blast_hits_2b6140(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void func_L00_00260958_2b6140(void *, float) __asm__("FUN_L00_0025f8e0");

extern int D_L00_0015F458 __attribute__((sda));
extern float D_L00_00161498 __attribute__((sda));
extern float D_L00_001614A0 __attribute__((sda));
extern float D_L00_001614A4 __attribute__((sda));
extern float D_L00_001614A8 __attribute__((sda));
extern int D_L00_0015F608_macro __asm__("D_L00_0015F608") __attribute__((section(".sdata")));
/* visibomb_missile update (moby class 172): drift, homing toward the target, arming countdown. */
void visibomb_missile_update(struct Moby *m) __asm__("FUN_L00_002b6140");
void visibomb_missile_update(struct Moby *m) {
    V_2b6140 v20;
    V_2b6140 k30;
    V_2b6140 k40;
    V_2b6140 v50;
    V_2b6140 v60;
    Ev_2b6140 ev;
    V_2b6140 vA0;
    D_2b6140 *d = (D_2b6140 *)m->pvars;
    int direct = 0;

    *(u128_2b6140 *)&k40 = 0;
    k40.z = D_0015ED6C_2b6140 * 8.0f;
    *(u128_2b6140 *)&k30 = *(u128_2b6140 *)&k40;
    *(u128_2b6140 *)&k40 = 0;
    k40.z = 1.0f;
    D_L00_0015F608_macro = 1;
    D_L00_0015F458 = 1;
    func_L00_00204130_2b6140();
    D_L00_0017E508 = 1;
    force_help_message_2b6140(6, 0);
    d->f10--;
    if (!(-scale_game_frames_2b6140(100) < d->f10)
        || hero.state.current == 0x16
        || hero.state.current == 0x3D
        || hero.state.current == 0x72
        || hero.state.control_mode == 2
        || hero.state.control_mode == 4
        || hero.state.control_mode == 3
        || hero.state.control_mode > 12
        || hero.state.control_mode == 7
        || hero.state.control_mode == 10
        || hero.state.control_mode == 16
        || hero.state.control_mode == 5
        || hero.state.current == 0x65
        || hero.grounded_frames == 0
        || (((Tgt_2b6140 *)hero.ground_moby) != 0
            && (((Tgt_2b6140 *)hero.ground_moby)->hA6 == 0x13E || ((Tgt_2b6140 *)hero.ground_moby)->hA6 == 0x46F))) {
        func_L00_002B6E10_2b6140(m, d);
        return;
    }
    if (d->f10 < 0) {
        if (d->f14 != 0) func_L00_002ECAF8_2b6140(d->f14, m);
        func_L00_002B6AE0_2b6140(m, 0);
        func_L00_002B6E90_2b6140(m, d);
        return;
    }
    {
        float t = D_L00_0016149C * D_0015ED6C_2b6140;
        if (controller_state.held & 0x40) t = D_L00_001614A0 * D_0015ED6C_2b6140;
        d->f48 += (t - d->f48) * D_L00_001614A4;
        d->f4 += (d->f48 - d->f4) * D_L00_001614A8;
        if (D_L00_001614A0 * D_0015ED6C_2b6140 < d->f4) d->f4 = D_L00_001614A0 * D_0015ED6C_2b6140;
    }
    func_L00_0028F210_2b6140(d->f2C, truncate_float_to_s32_2b6140((d->f48 - D_L00_0016149C * D_0015ED6C_2b6140
        / (D_L00_001614A0 * D_0015ED6C_2b6140 - D_L00_0016149C * D_0015ED6C_2b6140)) * D_L00_00161498));
    if (d->f10 > 0) {
        if (d->f10 < scale_game_frames_2b6140(0xBB8) - scale_game_frames_2b6140(5)) {
            float ang;
            float dx = hero.stick_x - d->f18;
            float dy = hero.stick_y - d->f1C;
            float ka = D_0015ED60_2b6140 * 0.1f;
            float kb = D_0015ED60_2b6140 * 0.06f;
            float kc = D_0015ED60_2b6140 * 0.0122718466f;
            d->f18 += dx * ka;
            d->f1C += dy * ka;
            m->rot.x += (d->f18 - m->rot.x) * kb;
            ang = fast_subtract_rotations_2b6140(m->rot.y, d->f1C * kc);
            m->rot.y = ang;
            if (1.57079637f <= ang) {
                m->rot.y = 1.56979632f;
            } else if (ang <= -1.57079637f) {
                m->rot.y = -1.56979632f;
            }
            m->rot.z = fast_subtract_rotations_2b6140(m->rot.z, d->f18 * (D_0015ED60_2b6140 * 0.0122718466f));
            func_L00_0025B040_2b6140(m, 0.2f);
        }
    }
    build_spherical_offset_2b6140(&v50, d->f4, m->rot.z, -m->rot.y);
    qcopy(&v20, &m->pos);
    add_vector_xyz_2b6140(&m->pos, &m->pos, &v50);
    if (scale_game_frames_2b6140(0xBB8) - scale_game_frames_2b6140(0x3C) < d->f10) {
        float t = probe_ground_height_2b6140(&m->pos, 0, 0.5f) + d->f44;
        if (m->pos.z < t) {
            if (D_L00_00173E58_2b6140 == 0 || (D_L00_00173E58_2b6140->f24 != 0 && D_L00_00173E58_2b6140->f24->h46 == 5)) {
                float s = angle_atan2_2b6140(d->f4, approach_value_2b6140(&m->pos.z, t, D_0015ED6C_2b6140 * 6.0f));
                if (D_0015ED6C_2b6140 * 1.57079637f < s) s = D_0015ED6C_2b6140 * 1.57079637f;
                m->rot.y -= s;
            }
        }
    }
    if (m->pos.x < 4.5f || 1018.5f < m->pos.x || m->pos.y < 4.5f || 1018.5f < m->pos.y
        || m->pos.z < 4.5f || 1018.5f < m->pos.z) {
        func_L00_002B6E10_2b6140(m, d);
        return;
    }
    func_L00_002B6E90_2b6140(m, d);
    {
        struct Moby *skip = m;
        func_L00_001FFED8_2b6140(&d->f24->pos, 1,
            -(d->f1C * 0.5f + absolute_float_2b6140(d->f18 / 3.0f) + (d->f18 - m->rot.x) * 1.1f) * 0.7f);
        func_L00_001FFED8_2b6140(&d->f28->pos, 1,
            -(d->f1C * 0.5f + absolute_float_2b6140(d->f18 / 3.0f) - (d->f18 - m->rot.x) * 1.1f) * 0.7f);
        ev.moby = m;
        ev.flags = 0x830000;
        ev.f1C = 6.0f;
        ev.f20 = 1;
        *(u128_2b6140 *)&ev.pos = *(u128_2b6140 *)&v50;
        trooper_scale_2b6140(&ev, &ev, 1.0f);
        ev.b18 = 3;
        ev.pos.w = 5627.9248f;
        ev.pos.z = 1.0f;
        ev.b19 = 2;
        ev.h1A = (*(unsigned short *)&m->oclass);
        if (scale_game_frames_2b6140(0xBB8) - 40 < d->f10) skip = hero.moby;
        if (collision_line_2b6140(&v20, &m->pos, 0, skip, &ev)) {
            if (((Hit_2b6140 *)D_L00_00173E40_2b6140)->f18 != 0) {
                direct = 0;
                if (((Hit_2b6140 *)D_L00_00173E40_2b6140)->f18 == hero.moby) direct = 1;
                m->command = 2;
                qcopy(&m->pos, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v20);
                subtract_vector_xyz_2b6140(&v60, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v50, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v60);
                normalize_vector_xyz_2b6140(&v60, &v60, 1.0f);
                func_L00_001FF610_2b6140(&k30, &v50, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v40);
                normalize_vector_xyz_2b6140(&k30, &k30, D_0015ED6C_2b6140 + D_0015ED6C_2b6140);
                normalize_vector_xyz_2b6140(&k40, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v40, 1.0f);
                if (d->f2C != -1) {
                    Row_2b6140 *e = (Row_2b6140 *)(D_0013E550_2b6140 + d->f2C * 0x70);
                    if (e->f88 == m) {
                        if (e->f74 != 0) release_voice_slot_2b6140(d->f2C);
                    }
                }
                d->f2C = -1;
            } else if (((Hit_2b6140 *)D_L00_00173E40_2b6140)->f1C > 0) {
                m->command = 1;
                qcopy(&m->pos, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v20);
                subtract_vector_xyz_2b6140(&v60, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v50, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v60);
                normalize_vector_xyz_2b6140(&v60, &v60, 1.0f);
                func_L00_001FF610_2b6140(&k30, &v50, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v40);
                normalize_vector_xyz_2b6140(&k30, &k30, D_0015ED6C_2b6140 + D_0015ED6C_2b6140);
                normalize_vector_xyz_2b6140(&k40, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v40, 1.0f);
                if (d->f2C != -1) {
                    Row_2b6140 *e = (Row_2b6140 *)(D_0013E550_2b6140 + d->f2C * 0x70);
                    if (e->f88 == m) {
                        if (e->f74 != 0) release_voice_slot_2b6140(d->f2C);
                    }
                }
                d->f2C = -1;
            }
        } else if (d->f10 == 0) {
            func_L00_002B6E10_2b6140(m, d);
            return;
        }
    }
    if (d->f10 < scale_game_frames_2b6140(0xBB8) - scale_game_frames_2b6140(0x3C)) {
        if (controller_state.pressed & 0x20) {
            m->command = 1;
            if (d->f2C != -1) {
                Row_2b6140 *e = (Row_2b6140 *)(D_0013E550_2b6140 + d->f2C * 0x70);
                if (e->f88 == m) {
                    if (e->f74 != 0) release_voice_slot_2b6140(d->f2C);
                }
            }
            d->f2C = -1;
        }
    }
    if (m->command != 0) {
        int k;
        int rr;
        if (direct == 0) {
            spawn_explosion_2b6140(m, &k30, 0, 0.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 0.0f, 1.0f, 1, 20.0f, 1, 1, -1, 0);
            rr = collect_mobies_in_sphere_2b6140(&m->pos, 4.0f, 16, m, 0);
            *(u128_2b6140 *)&vA0 = *(u128_2b6140 *)((V_2b6140 *)&m->pos);
            send_blast_hits_2b6140(m, &vA0, D_L00_00177F00_2b6140, rr, 0, 0x830000, 3, 2, 6.0f, 1.0f, 1.0f);
        }
        m->flags |= 1;
        d->f4 = D_0015ED60_2b6140 * 0.0199999996f;
        d->f18 = d->f18 * 0.00999999978f;
        if (scale_game_frames_2b6140(0xBB8) - scale_game_frames_2b6140(0x1E) < d->f10) {
            d->f10 = -scale_game_frames_2b6140(100);
        } else {
            d->f10 = -1;
        }
        qcopy(&vA0, &m->pos);
        for (k = 4; k >= 0; k--) {
            qcopy(&m->pos, &vA0);
            func_L00_00260958_2b6140(&m->pos, 1.5f);
            func_L00_002B6FF0_2b6140(m, 1);
            qcopy(&m->pos, &vA0);
        }
        v50.x = 0.0f;
        m->command = 0;
    }
    if (d->f14 != 0) func_L00_002ECAF8_2b6140(d->f14, m);
}
typedef struct {
    f32 x;
    f32 y;
    u8 pad[0x18];
} Tab;
typedef struct {
    u8 pad0[0xD];
    u8 count;
    u8 padE[0x1A];
    Tab *tab;
} Cfg;
typedef struct {
    s32 idx;
    s32 anim;
    f32 radius;
    s32 retry;
    s32 frame;
    s32 pad14;
    s32 cnt18;
} Vars;
typedef struct {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[3];
    Cfg *cfg;
    u8 pad28[0x2B];
    u8 type;
    u8 pad54[0x1C];
    u8 flags;
    u8 pad71[7];
    Vars *vars;
} Moby;
typedef struct {
    u8 pad0[0x70];
    u16 cnt;
    u16 last;
    u32 flags;
} Ring;

extern s32 FUN_L00_0028d8c0_u(Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern s32 random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern s32 FUN_001f96f8_c(s32) __asm__("FUN_001f96f8");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern s32 allocate_voice_for_target_entry(s32, s32, Moby *) __asm__("FUN_0022da68");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 FUN_L00_002b6bc8(Moby *, f32);
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern u32 D_0013C940[];
extern u32 D_0013CAE0[];
extern u8 D_0013E550[];
extern Ring D_00141680;
extern s32 D_L00_001614CC;
extern s32 D_L00_001614D4;
extern s32 D_0015EEA4;
extern s32 D_L00_0015F5CC;
extern s32 FUN_L00_00257b50_b6bc8(s32) __asm__("FUN_L00_00257b50");
extern u8 *FUN_L00_0025c338_b6bc8(Moby *) __asm__("FUN_L00_0025c338");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_L00_00259bc8_b6bc8(Moby *, Moby *, s32, void *, void *, f32) __asm__("FUN_L00_00259bc8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern Moby *D_001ABA00_b6bc8[] __asm__("D_L00_001ABA00");
extern s16 D_L00_001B07B0[] __asm__("D_L00_001B07B0");
extern u8 *moby_table __asm__("D_L00_0015FFD8");

s32 FUN_L00_002b6bc8(Moby *m, f32 radius) {
    u128 velocity __attribute__((aligned(16)));
    union { u128 q; f32 f[4]; } pos __attribute__((aligned(16)));
    Moby *other;
    u8 *state;
    Vars *vars;
    struct Hero *player;
    s32 limit;
    s32 count = 0;
    s32 i;
    f32 distance;
    f32 angle;

    pos.q = *(u128 *)((u8 *)m + 0x10);
    vars = m->vars;
    limit = FUN_L00_00257b50_b6bc8(vars->pad14 > 0 ? vars->pad14 : 1);
    vars->pad14 = 0;
    other = D_001ABA00_b6bc8[0];
    if (other != 0) {
        i = 0;
        do {
            /* Fakematch: the extra block keeps retail's scheduling of the moby lookup. */
            do { state = FUN_L00_0025c338_b6bc8(other); } while (0);
            if (state != 0) {
                /* Fakematch: &hero through the motion label keeps retail's &hero + 0x98 load below. */
                player = (struct Hero *)(D_0013F3D0 - 0x80);
                distance = vector_distance(&pos, (u8 *)other + 0x10);
                if (distance < 12.0f &&
                    ((u16)(*(u16 *)((u8 *)other + 0xa6) - 500) < 2 ||
                     *(s16 *)((u8 *)other + 0xa6) == 0x1f9 ||
                     *(s16 *)((u8 *)other + 0xa6) == 0x1ff)) {
                    struct Hero *g = &hero;
                    angle = fast_difference_between_rotations(g->motion.rot.f[2],
                        fast_arc_tan(*(f32 *)((u8 *)other + 0x10) - g->motion.pos.f[0],
                                               *(f32 *)((u8 *)other + 0x14) - g->motion.pos.f[1]));
                    if (angle < 0.9599311f) {
                        if (vars->pad14 == limit) {
                            fast_vec_sub(&velocity, (u8 *)other + 0x10, (u8 *)m + 0x10);
                            fast_vec_normalize(&velocity, &velocity, 1.5f);
                            FUN_L00_00259bc8_b6bc8(other, m, 0x10000, &g->motion.pos, &velocity, 1.0f);
                            vars->frame = scale_game_frames(random_int_between(5, 0x2d));
                        }
                        vars->pad14++;
                    }
                }
                if (distance < radius) {
                    struct Hero *g = &hero;
                    angle = fast_arc_tan(*(f32 *)((u8 *)other + 0x10) - g->motion.pos.f[0],
                                               *(f32 *)((u8 *)other + 0x14) - g->motion.pos.f[1]);
                    angle = fast_difference_between_rotations(player->motion.rot.f[2], angle);
                    if (angle < 0.9599311f) {
                        Moby *current = *(Moby **)(state + 0x18);
                        if (current == 0 || distance < vector_distance((u8 *)current + 0x10, (u8 *)other + 0x10)) {
                            *(Moby **)(state + 0x18) = m;
                            count++;
                        }
                    }
                }
            }
            i++;
            other = D_001ABA00_b6bc8[i];
        } while (other != 0);
    }
    for (i = 1; i <= D_L00_001B07B0[0]; i++) {
        other = (Moby *)(moby_table + D_L00_001B07B0[i] * 0x100);
        if (other != 0 && (s8)other->state >= 0) {
            Vars *vars = other->vars;
            distance = vector_distance(&pos, (u8 *)other + 0x10);
            if (distance < radius) {
                angle = fast_difference_between_rotations(hero.motion.rot.f[2],
                    fast_arc_tan(*(f32 *)((u8 *)other + 0x10) - hero.motion.pos.f[0],
                                           *(f32 *)((u8 *)other + 0x14) - hero.motion.pos.f[1]));
                if (angle < 0.9599311f) {
                    *(Moby **)((u8 *)vars + 0x78) = m;
                    count++;
                }
            }
        }
    }
    return count;
}


void taunter_update(Moby *m) __asm__("FUN_L00_002b6f10");
void taunter_update(Moby *m) {
    Vars *o;
    s32 v;
    s32 a;
    s32 b;
    s32 i;
    s32 r1;
    s32 r2;
    u8 *e;
    s32 play_time_stat_value;

    if (m == 0)
        return;
    o = m->vars;
    if (o == 0)
        return;
    if (o->radius == 0.0f)
        o->radius = 30.0f;
    if (m->type == 2)
        m->state = 3;
    D_L00_001614D4 = 1;
    if (m->state == 0) {
        o->anim = -1;
        m->state++;
        D_L00_001614CC = 1;
    }
    v = m->state;
    if ((u8)v == 1) {
        if (m->flags & 2) {
            m->state = v + 1;
            o->retry = 0;
        }
        goto end;
    }
    if ((u8)v != 2)
        goto end;
    if (m->type == 3 && hero.unk20AC == 0) {
        D_L00_001614D4 = 0;
        o->cnt18++;
    } else {
        o->cnt18 = 0;
    }
    if (FUN_L00_0028d8c0_u(m, o->anim) == 0) {
        b = D_0013C940[0x1A4 / 4] & hero.items[0].button_mask;
        if ((b != 0 || ((D_0013C940[0x1A0 / 4] & hero.items[0].button_mask) != 0 && o->retry >= 0x11)) &&
            hero.unk20AC == 0) {
            if (b != 0) {
                o->frame = FUN_001f96f8_c(random_int_between(0xF, 0x2D));
            }
            if (D_00141680.cnt < 0xFFFF)
                D_00141680.cnt++;
            play_time_stat_value = FUN_001f96f8_c(D_0015EEA4) / 600;
            if (D_00141680.last < play_time_stat_value) {
                D_00141680.last = FUN_001f96f8_c(D_0015EEA4) / 600;
            }
            D_00141680.flags |= 1 << current_level_index;
            D_00141680.flags |= 0x80000000;
            if (m->type != 3)
                blend_moby_animation(m, 3, 0, FUN_001f96f8_c(3));
            o->anim = allocate_voice_for_target_entry(o->idx, 1, m);
            i = o->idx + 1;
            o->idx = i;
            r1 = truncate_float_to_s32(m->cfg->tab[i].x * 10.0f);
            r2 = truncate_float_to_s32(m->cfg->tab[i].y * 10.0f);
            if (!(o->idx < m->cfg->count && r1 == 0x1E && r2 == r1))
                o->idx = 0;
            FUN_L00_002b6bc8(m, o->radius);
            o->retry = 0;
            goto end;
        }
        if (o->anim < 0) {
            if ((D_0013C940[0x1A0 / 4] & hero.items[0].button_mask) != 0 && hero.unk20AC == 0)
                o->retry++;
            else
                o->retry = 0;
            if (m->type != 1)
                blend_moby_animation(m, 1, 0, FUN_001f96f8_c(5));
            goto end;
        }
    } else {
        o->retry++;
        if ((D_0013CAE0[0] & hero.items[0].button_mask) != 0 || FUN_001f96f8_c(0x1E) >= o->cnt18) {
            if (hero.unk20AC == 0)
                goto L7340;
        }
    }
    if (m->type != 1)
        blend_moby_animation(m, 1, 0, FUN_001f96f8_c(5));
    a = o->anim;
    if (a != -1) {
        e = D_0013E550 + a * 0x70;
        if (*(Moby **)(e + 0x88) == m && e[0x74] != 0)
            release_voice_slot(a);
    }
    o->retry = 0;
    o->anim = -1;
    goto end;
L7340:
    if (!(D_L00_0015F5CC & 3))
        FUN_L00_002b6bc8(m, o->radius);
end:
    if ((s32)(hero.items[0].state ^ 3) != 0)
        FUN_L00_002b7398(m);
}

#define NOT_SDA

#define MACRO_ADDR

/* per-frame update of the ten slot table: spawn one, age the rest, then draw */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B8690), where it is exact; names translated to the US level program. */

typedef struct {
    float f;
    short a;
    short b;
    short c;
    short pad;
} E8690;

extern E8690 D_L00_001DB440[];
extern int D_L00_001614D0;
extern float D_L00_001614DC;
extern char D_L00_001614E0[] __attribute__((section(".sdata")));
extern int D_L00_001614B0 __attribute__((sda));
extern float D_L00_001614B4 __attribute__((sda));
extern float D_L00_001614B8 __attribute__((sda));
extern unsigned short D_L00_001614BC __attribute__((sda));
extern int D_L00_001614D8;
extern int D_L00_001614D0_e[] __asm__("D_L00_001614D0") __attribute__((section(".sdata")));
extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32(int *arg0);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
extern int f_to_int(float) __asm__("FUN_001fa6d0");
void FUN_L00_002b7568_c(void) __asm__("FUN_L00_002b7568");

void FUN_L00_002b7398(void *arg) {
    E8690 *p;
    unsigned short av;
    int i;
    int r;
    if (D_L00_001614CC != 0) {
        for (i = 9; i >= 0; i--)
            D_L00_001DB440[i].c = 0;
        D_L00_001614CC = 0;
        D_L00_001614D0 = 0;
        r = f_to_int(D_L00_001614B4 / (D_L00_001614B8 * frame_time));
        D_L00_001614D8 = r;
        D_L00_001614DC = 1.0f / convert_integer_to_float(r);
    }
    if (D_L00_001614D4 == 0) {
        if (tick_countdown_32(&D_L00_001614D0) != 0) {
            i = 0;
            do {
                p = &D_L00_001DB440[i];
                if (p->c == 0) {
                    D_L00_001614D0 = scale_game_frames(D_L00_001614B0);
                    av = *(unsigned short *)&D_L00_001614D0_e[2];
                    p->b = D_L00_001614BC;
                    p->c = 1;
                    p->a = av;
                    p->f = 0;
                    break;
                }
            } while (++i < 10);
        }
    }
    for (i = 0; i < 10; i++) {
        p = &D_L00_001DB440[i];
        if (p->c != 0) {
            if (fast_dec_timer_s16(&p->a) != 0)
                p->c = 0;
            else
                p->f += D_L00_001614B8 * frame_time;
        }
    }
    moby_point_position(arg, 0, D_L00_001614E0);
    enqueue_callback_list_1(FUN_L00_002b7568_c, arg);
}
typedef union {
    OvlQuad q;
    f32 f[4];
} V_2b7568;
typedef struct {
    V_2b7568 corner[4];
    u32 color[4];
    struct {
        f32 u, v;
    } uv[4];
    long unk70, tex, unk80, unk88;
} Q_2b7568;
typedef struct {
    f32 len;
    s16 alpha;
    s16 pad;
    s16 on;
    s16 pad2;
} E_2b7568;
extern E_2b7568 D_L00_001DB440_2b7568[] __asm__("D_L00_001DB440") __attribute__((section(".data")));
extern V_2b7568 D_L00_001614E0_2b7568[] __asm__("D_L00_001614E0")
    __attribute__((section(".sdata")));
extern f32 D_L00_001614DC;
extern s32 D_L00_001614BC_2b7568 __asm__("D_L00_001614BC") __attribute__((sda));
extern V_2b7568 D_0013F5E0_2b7568 __asm__("D_0013F5E0");
extern long tex_2b7568(int) __asm__("FUN_001f44b8");
extern void setlen_2b7568(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_2b7568(void *, void *, f32) __asm__("FUN_001f9a68");
extern void add_2b7568(void *, void *, void *) __asm__("FUN_001f9a10");
extern void cross_2b7568(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void tf_2b7568(void *, void *, void *) __asm__("FUN_001f9d20");
extern f32 itof_2b7568(s32) __asm__("FUN_001fa6c0");
extern s32 ftoi_2b7568(f32) __asm__("FUN_001fa6d0");
extern void draw_2b7568(void *, void *, int) __asm__("FUN_001f7d30");
extern V_2b7568 D_L00_001DB400[] __attribute__((section(".data")));
extern f32 D_L00_001614C8 __attribute__((sda));
extern f32 D_L00_001614C4 __attribute__((sda));
extern u32 D_L00_001614C0 __attribute__((sda));
void FUN_L00_002b7568(u8 *m) {
    Q_2b7568 quad;
    V_2b7568 m90;
    V_2b7568 up;
    V_2b7568 side;
    V_2b7568 pos;
    V_2b7568 corner;
    V_2b7568 dir;
    V_2b7568 p;
    E_2b7568 *e;
    s32 i, j;
    f32 t, sz;
    u32 col;
    quad.tex = tex_2b7568(8);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    quad.uv[0].u = 0.0f;
    quad.uv[0].v = 0.0f;
    quad.uv[1].u = 0.0f;
    quad.uv[1].v = 1.0f;
    quad.uv[2].u = 1.0f;
    quad.uv[2].v = 0.0f;
    quad.uv[3].u = 1.0f;
    quad.uv[3].v = 1.0f;
    setlen_2b7568(&dir, m + 0xD0, -1.0f);
    for (i = 0; i < 10; i++) {
        if (D_L00_001DB440_2b7568[i].on == 0)
            continue;
        scale_2b7568(&p, &dir, D_L00_001DB440_2b7568[i].len);
        add_2b7568(&pos, D_L00_001614E0_2b7568, &p);
        qcopy(&m90, &dir);
        scale_2b7568(&side, &D_0013F5E0_2b7568, -1.0f);
        cross_2b7568(&up, &m90, &side);
        setlen_2b7568(&up, &up, 1.0f);
        cross_2b7568(&side, &up, &m90);
        t = itof_2b7568(D_L00_001DB440_2b7568[i].alpha) * D_L00_001614DC;
        if (1.0f < t)
            t = 1.0f;
        sz = D_L00_001614C8 + (D_L00_001614C4 - D_L00_001614C8) * t;
        col = (D_L00_001614C0 & 0xFFFFFF) |
              (ftoi_2b7568((itof_2b7568(D_L00_001614BC_2b7568) - 0.0f) * t + 0.0f) << 24);
        quad.color[3] = col;
        quad.color[2] = col;
        quad.color[1] = col;
        quad.color[0] = col;
        for (j = 0; j < 4; j++) {
            scale_2b7568(&corner, &D_L00_001DB400[j], sz);
            tf_2b7568(&quad.corner[j], &corner, &m90);
        }
        draw_2b7568(&quad, 0, 0);
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002b77f0.s", FUN_L00_002b77f0);
#else
/* Ported from rac1-decomp (nonmatching/shared/func_L00_002B8AE8.c: func_L00_002B8AE8). */
typedef struct PyrocitorVars_2b77f0 {
    struct Moby *other;
    char pad4[0x40];
    s16 light;
    char pad46[4];
    s16 voice;
    s16 timer;
    s16 spark;
} PyrocitorVars_2b77f0;

typedef struct FlameHit_2b77f0 {
    Vec_2b2100 point;
    struct Moby *moby;
    s32 flags;
    u8 type;
    u8 pad[3];
    f32 scale;
    s32 mode;
} FlameHit_2b77f0;

typedef Vec_2b2100 Vec_2b77f0;
extern Vec_2b77f0 D_L00_0015F580_2b77f0 __asm__("D_L00_0015F580") MACRO_ADDR;
extern f32 D_L00_001614F0 __attribute__((sda));
extern f32 D_L00_00161500 __attribute__((sda));
extern f32 D_L00_00161510 __attribute__((sda));
extern f32 D_L00_00161514 __attribute__((sda));
extern s32 D_L00_00161518 __attribute__((sda));
extern s32 D_L00_0016151C __attribute__((sda));
extern f32 D_L00_00161520 __attribute__((sda));
extern f32 D_L00_00161524 __attribute__((sda));
extern f32 D_L00_00161528 __attribute__((sda));
extern s32 D_L00_00161530 __attribute__((sda));
extern f32 D_L00_001601E8;

void FUN_L00_002b8230(unsigned char *);
void FUN_L00_002b8438(char *);
extern f32 compute_interpolated_record_value(struct Moby *) __asm__("FUN_0020c9e0");
extern struct Moby *FUN_L00_002bb360_2b77f0(struct Moby *) __asm__("FUN_L00_002bb360");
extern void lerp_vector(void *, void *, void *, f32) __asm__("FUN_001f9a40");
extern s16 FUN_L00_0023e5e0_2b77f0(void *, f32, f32, s32) __asm__("FUN_L00_0023e5e0");
extern s16 FUN_0022da68(s32, s32, struct Moby *);
extern s32 FUN_L00_0028d8c0_2b77f0(struct Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern void FUN_L00_00257d78_2b77f0(void *, f32, f32) __asm__("FUN_L00_00257d78");
extern void *FUN_L00_0026b230_2b77f0(void *, void *, s32, f32) __asm__("FUN_L00_0026b230");
extern void FUN_L00_002b8798_2b77f0(struct Moby *, void *) __asm__("FUN_L00_002b8798");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void FUN_L00_0026d598_2b77f0(void *, void *, s32) __asm__("FUN_L00_0026d598");
extern void FUN_001fa030_2b77f0(void *, void *) __asm__("FUN_001fa030");
extern void FUN_001fa328_2b77f0(void *, void *, void *) __asm__("FUN_001fa328");
extern void FUN_L00_00250df8_2b77f0(void) __asm__("FUN_L00_00250df8");

/* Fakematch: a second name for the pad state makes the button tests rebuild its address as retail does. */
extern struct PadState controller_state_lui __asm__("D_0013C940");
void pyrocitor_update(struct Moby *moby) __asm__("FUN_L00_002b77f0");
void pyrocitor_update(struct Moby *moby) {
    PyrocitorVars_2b77f0 *v = (PyrocitorVars_2b77f0 *)moby->pvars;
    Vec_2b2100 pos, dir, scratch, endpoint, particle_pos, random_vec;
    FlameHit_2b77f0 hit;
    f32 pos_out, vel_out;
    f32 speed, length, accel, damping;
    s32 gate, initial_count, extra, count;
    s32 i;
    s32 fl;
    void *effect;
    s32 ready;
    s32 h;

    FUN_L00_002b8230((unsigned char *)moby);
    FUN_L00_0024f7c8_2b2100(moby, 0, &pos);
    switch (moby->state) {
    case 0:
        v->light = -1;
        v->voice = -1;
        moby->state = 1;
        break;
    case 1:
        if (moby->seq == 1)
            moby->state = 2;
        if (moby->seq != 0)
            break;
        if (compute_interpolated_record_value(moby) > 10.0f && (controller_state.held & hero.items[0].button_mask) &&
            moby->prev_seq != 1)
            blend_moby_animation_2b2100(moby, 1, 0, scale_game_frames_2b2100(5));
        break;
    case 2:
        if (v->other == 0) {
            v->other = FUN_L00_002bb360_2b77f0(moby);
            if (v->other != 0) {
                v->other->flags |= 0x100;
                *(f32 *)(v->other->pvars + 0xC) = D_L00_00161510;
            }
        }
        if (v->light != -1) {
            lerp_vector(&lights_2b2100[v->light], &lights_2b2100[v->light], &D_L00_0015F580_2b77f0, 0.1f);
            if (fast_vec_length(&lights_2b2100[v->light]) < 0.001f) {
                FUN_L00_0023e838_2b2100(v->light);
                /* Fakematch: the dead test on moby keeps the s6/s7 assignment of retail. */
                if (moby) v->light = -1; else v->light = -1;
            }
        }
        if ((controller_state_lui.held & hero.items[0].button_mask) && !hero.unk20AC && FUN_L00_00233e98_2b2100(-1)) {
            moby->state = 3;
            FUN_L00_0020e4c8_2b2100();
            v->timer = scale_game_frames_2b2100(10);
            if (v->light == -1)
                v->light = FUN_L00_0023e5e0_2b77f0(&moby->pos, 7.5f, 0.0f, 0);
        } else if ((controller_state_lui.pressed & hero.items[0].button_mask) && !FUN_L00_00233e98_2b2100(-1)) {
            FUN_0022da68(0, 0, moby);
        }
        break;
    case 3:
        if (!FUN_L00_0028d8c0_2b77f0(moby, v->voice))
            v->voice = FUN_0022da68(1, 4, moby);
        if (v->light != -1 && moby->state == 3) {
            dir.q = 0;
            dir.f.x = 2.0f;
            dir.f.z = 2.0f;
            transform_vector_by_basis_2b2100(&dir, &dir, hero.basis);
            add_vector_xyz_2b2100(&lights_2b2100[v->light].pos, &hero.motion.pos, &dir);
            lights_2b2100[v->light].pos.f.w = 7.5f;
            lerp_vector(&scratch, &D_L00_001614F0, &D_L00_00161500,
                fast_sin_2b2100(((f32)(D_L00_0015F5CC % 120) / 120.0f) * 6.28318f - 3.14159f) * 0.5f + 0.5f);
            lerp_vector(&lights_2b2100[v->light], &lights_2b2100[v->light], &scratch, 0.1f);
        }
        pos_out = vel_out = 0.0f;
        speed = convert_integer_to_float_2b2100(item_text_variant[16]) * 4.0f;
        if (!(D_L00_0015F5CC & 3))
            gate = 1;
        else
            gate = 0;
        if (hero.state.current == 1 && (moby->flags & 1)) {
            normalize_vector_xyz(&scratch, camera_2b2100 + 0x370, -1.0f);
            add_vector_xyz_2b2100(&scratch, &scratch, camera_2b2100 + 0x140);
            normalize_vector_xyz(&dir, camera_2b2100 + 0x350, D_L00_00161520 + speed + 0.75f);
            add_vector_xyz_2b2100(&endpoint, &dir, &scratch);
        } else {
            qcopy(&scratch, &pos);
            normalize_vector_xyz(&dir, &hero.unk670, D_L00_00161520 + speed + 0.75f);
            add_vector_xyz_2b2100(&endpoint, &dir, &pos);
        }
        if (collision_segment_hits_2b2100(&hero.motion.unkD0, &scratch, 2, 0, 0)) {
            length = 0.0f;
        } else if (collision_segment_hits_2b2100(&scratch, &endpoint, 2, 0, 0)) {
            length = distance_xyz_2b2100(&scratch, collision_hit_2b2100 + 0x20) - 0.75f;
            if (length < 0.0f)
                length = 0.0f;
        } else {
            length = D_L00_00161520 + speed;
        }
        accel = D_L00_001601E8 * frame_time_sq;
        damping = D_L00_00161514 * frame_time;
        advance_accelerated_scalar(&pos_out, &vel_out, length, 1.0f, accel, damping);
        normalize_vector_xyz(&dir, &dir, vel_out);
        initial_count = D_L00_00161518;
        extra = D_L00_0016151C;
        if (length < 4.0f || (moby->flags & 1))
            initial_count = 1;
        count = extra + initial_count;
        for (i = 0; i < count; i++) {
            FUN_L00_00257d78_2b77f0(&random_vec, 0.0f,
                ((D_L00_00161524 - D_L00_00161528) * (length / (D_L00_00161520 + speed)) + D_L00_00161528) *
                    frame_time);
            add_vector_xyz_2b2100(&random_vec, &random_vec, &dir);
            scale_vector_xyz_2b3628(&particle_pos, &dir, random_float_between_2b3628(0.0f, 1.0f));
            add_vector_xyz_2b2100(&particle_pos, &particle_pos, &scratch);
            if (i < initial_count)
                fl = 1;
            else
                fl = 0;
            effect = FUN_L00_0026b230_2b77f0(&particle_pos, &random_vec, fl | 4, length);
            if (i == 2 && gate)
                FUN_L00_002b8798_2b77f0(moby, effect);
        }
        particle_pos.q = 0;
        particle_pos.f.x = -0.3f;
        transform_vector_by_basis_2b2100(&random_vec, &particle_pos, hero.basis);
        add_vector_xyz_2b2100(&random_vec, &random_vec, &pos);
        hit.moby = moby;
        hit.flags = 0x10000;
        hit.scale = convert_integer_to_float_2b2100(item_text_variant[16]) + 1.0f;
        hit.point.q = dir.q;
        hit.mode = 1;
        set_vector_length_xy(&hit.point, &hit.point, 1.0f);
        hit.point.f.z = 1.0f;
        hit.point.f.w = 5627.9248f;
        hit.mode = 1;
        hit.type = 5;
        collect_mobies_in_sphere(&random_vec, 0.42f, 5, hero.moby, &hit);
        particle_pos.f.x = -0.8f;
        transform_vector_by_basis_2b2100(&random_vec, &particle_pos, hero.basis);
        add_vector_xyz_2b2100(&random_vec, &random_vec, &pos);
        collect_mobies_in_sphere(&random_vec, 0.42f, 5, hero.moby, &hit);
        if (fast_dec_timer_s16(&v->spark)) {
            v->spark = truncate_float_to_s32(FUN_001f96b0_2b2100(random_float_between_2b3628(10.0f, 30.0f)));
            FUN_L00_00257d78_2b77f0(&random_vec, 0.0f, frame_time + frame_time);
            scale_vector_xyz_2b3628(&particle_pos, &dir, random_float_between_2b3628(0.5f, 0.1f));
            add_vector_xyz_2b2100(&particle_pos, &particle_pos, &random_vec);
            FUN_L00_0026d598_2b77f0(&scratch, &particle_pos, item_text_variant[16]);
        }
        if (D_L00_0015F5CC % scale_game_frames_2b2100(D_L00_00161530) == 0)
            FUN_L00_00233db8_2b2100(-1, 1);
        ready = FUN_L00_00233e98_2b2100(-1);
        if (fast_dec_timer_s16(&v->timer)) {
            if ((controller_state.held & hero.items[0].button_mask) && !hero.unk20AC && ready)
                break;
            moby->state = 2;
            FUN_L00_0020e698();
            h = v->voice;
            if (h != -1) {
                u8 *slot = D_0013E550 + h * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74])
                    release_voice_slot(h);
            }
            v->voice = -1;
            if (!ready)
                FUN_0022da68(0, 0, moby);
        }
        break;
    }
    if (v->other) {
        FUN_001fa030_2b77f0(&v->other->rot_row0, &v->other->rot);
        FUN_001fa328_2b77f0(&v->other->rot_row0, &moby->rot_row0, &v->other->rot_row0);
        qcopy(&v->other->pos, &pos);
        FUN_L00_00250df8_2b77f0();
    }
    FUN_L00_002b8438((char *)moby);
}
#endif
extern unsigned char D_0013E550_2b8230[] __asm__("D_0013E550");
extern int D_0015EEA4;
extern int D_0015ED84_2b8230 __asm__("D_0015ED84");
void FUN_0020c828_2b8230(void *) __asm__("FUN_0020c828");
void FUN_L00_0023e838_2b8230(int) __asm__("FUN_L00_0023e838");
void FUN_0022d798_2b8230(int) __asm__("FUN_0022d798");
int FUN_001f96f8_2b8230(int) __asm__("FUN_001f96f8");
void FUN_L00_00203908_2b8230(int, int) __asm__("FUN_L00_00203908");

extern unsigned short D_00141BD8 __attribute__((section(".data")));
void FUN_L00_002b8230(unsigned char *m) {
    unsigned char *v = *(unsigned char **)(m + 0x78);
    struct UsageStats *P;
    unsigned char *voice;
    int h, c;
    if (m[0x53] == 2) {
        m[0x20] = 4;
        if (*(void **)v)
            FUN_0020c828_2b8230(*(void **)v);
        if (*(short *)(v + 0x44) != -1) {
            FUN_L00_0023e838_2b8230(*(short *)(v + 0x44));
            *(short *)(v + 0x44) = -1;
        }
        h = *(short *)(v + 0x4A);
        if (h != -1) {
            voice = D_0013E550_2b8230 + h * 0x70;
            if (*(unsigned char **)(voice + 0x88) == m && voice[0x74])
                FUN_0022d798_2b8230(h);
        }
        *(short *)(v + 0x4A) = -1;
        return;
    }
    if (D_00141BD8)
        return;
    P = &usage_stats;
    if (P->stat[10].count)
        return;
    if (m[0x20] == 3) {
        (*(unsigned short *)(v + 0x50))++;
        return;
    }
    c = *(short *)(v + 0x50);
    if (c == 0)
        return;
    if (FUN_001f96f8_2b8230(0x78) < c) {
        if (P->stat[10].count <= 0xFFFE)
            (P->stat[10].count)++;
        h = FUN_001f96f8_2b8230(D_0015EEA4) / 600;
        if (P->stat[10].unk2 < h)
            P->stat[10].unk2 = FUN_001f96f8_2b8230(D_0015EEA4) / 600;
        P->stat[10].level_mask = P->stat[10].level_mask | (1 << D_0015ED84_2b8230) | 0x80000000;
    } else if (*(short *)(v + 0x50) < FUN_001f96f8_2b8230(0x1E)) {
        hero.unk20B4++;
    }
    *(short *)(v + 0x50) = 0;
    if (hero.unk20B4 >= 3)
        FUN_L00_00203908_2b8230(0x4E24, 0x4E);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B9730), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern char D_0013E520[];
extern f32 convert_integer_to_float_c(s32) __asm__("func_001FA6C0");
extern f32 D_0015ED6C_c2 __asm__("D_0015ED6C");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern unsigned FUN_L00_0025c0e8(unsigned c, int mask);
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_00257d78(float *, float, float);
extern void make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");

extern short D_L00_0016152C __attribute__((sda));
extern short D_L00_00161538 __attribute__((sda));
extern short D_L00_0016153C __attribute__((sda));
extern short D_L00_00161540 __attribute__((sda));
extern short D_L00_00161544 __attribute__((sda));
extern short D_L00_00161548 __attribute__((sda));
extern short D_L00_0016154C __attribute__((sda));
extern short D_L00_00161550 __attribute__((sda));
extern short D_L00_00161554 __attribute__((sda));
extern short D_L00_00161558 __attribute__((sda));
extern short D_L00_0016155C __attribute__((sda));
/* Walks the 15 entry pointers in the moby's data; for each live type-0xC entry builds a hit record and, for sub-type 10,
   spawns a burst of particles. Entries that are invalid or have sub-type < 2 are cleared. */
void FUN_L00_002b8438(char *moby) {
    float vec[4];
    char buf[0x30];
    char qv[16];
    float spark_random_vector[4];
    float spark_position[4];
    char *slots_base = *(char **)(moby + 0x78) + 4;
    unsigned char *q = (unsigned char *)D_0013E520;
    int i;

    for (i = 0; i < 15; i++) {
        char *entry = *(char **)(slots_base - (-(i * 4)));
        if (entry == 0) {
            continue;
        }
        if (*(unsigned char *)entry == 0xC && entry[1] >= 0) {
            float f20 = *(float *)(entry + 0xC) / 210000.0f * 0.5f;
            char *src = entry + 0x20;
            int sub_type;
            set_vector_length_xy(vec, src, 1.0f);
            vec[2] = 1.0f;
            vec[3] = 5627.9248046875f;
            make_hit_descriptor(buf, (int)moby, 0x10000, vec, convert_integer_to_float_c(q[0x10]) + 1.0f);
            *(unsigned char *)(buf + 0x18) = 5;
            *(unsigned char *)(buf + 0x19) = 1;
            *(unsigned short *)(buf + 0x1A) = *(unsigned short *)(moby + 0xA6);
            collect_mobies_in_sphere((entry + 0x10), f20, 0, moby, buf);
            sub_type = *(short *)(entry + 0xA);
            if (sub_type == 10) {
                int j;
                for (j = 0; j < *(int *)&D_L00_0016155C; j++) {
                    int c1;
                    int c2;
                    int c3;
                    int c4;
                    qcopy(qv, src);
                    FUN_L00_00257d78(spark_random_vector, 0.0f, *(float *)&D_L00_00161538 * D_0015ED6C_c2);
                    *(float *)(qv + 0xC) = *(float *)&D_L00_0016153C;
                    spark_random_vector[2] = D_0015ED6C_c2 * 4.0f;
                    spark_random_vector[3] = *(float *)&D_L00_00161540;
                    c1 = fast_tween_color(*(int *)&D_L00_00161544, *(int *)&D_L00_00161548, random_float_between_c(0.0f, 1.0f));
                    FUN_L00_0025c0e8(c1, q[0x10]);
                    c2 = fast_tween_color(*(int *)&D_L00_0016154C, *(int *)&D_L00_00161550, random_float_between_c(0.0f, 1.0f));
                    FUN_L00_0025c0e8(c2, q[0x10]);
                    c3 = func_001FA898_r(multiply_global_factor_ed64(random_float_between_c((float)*(int *)&D_L00_00161554, (float)*(int *)&D_L00_00161558)));
                    c4 = func_001FA898_r(multiply_global_factor_ed64(random_float_between_c((float)*(int *)&D_L00_00161554, (float)*(int *)&D_L00_00161558)));
                    FUN_L00_00257d78(spark_position, 0.0f, f20 + f20);
                    add_vector_xyz(spark_position, spark_position, (entry + 0x10));
                    spawn_spark(spark_position, qv, spark_random_vector, c1, c2, 0xA, c3, c4, *(int *)&D_L00_0016152C);
                }
            } else if (sub_type < 2) {
                *(int *)(slots_base - (-(i * 4))) = 0;
            }
        } else {
            *(int *)(slots_base - (-(i * 4))) = 0;
        }
    }
}
/* 0x78 points at a table of 15 s32 slots; index 0 is skipped, so the first
   free slot from 1 to 15 takes val. */
typedef struct {
    u8 pad0[0x78];
    s32 *slots;
} ListSlots8;

/* Stores val in the first empty of 15 slots after the list head. */
void FUN_L00_002b8798(ListSlots8 *list, s32 val) {
    s32 *p = list->slots;
    s32 i = 0;
    s32 *q;
    s32 n;

    while (i < 15) {
        n = i * 4;
        q = (s32 *)((u8 *)p + 4);
        if (*(s32 *)((u8 *)q + n) == 0) {
            *(s32 *)((u8 *)q + n) = val;
            break;
        }
        i++;
    }
}
#include "rnc/input/pad_state.h"



#include "qcopy.h"

#include "qzero.h"

/* Tesla claw (class 177) update: tracks up to two targets while fire is held, extends the claw tip, runs its
   firing states and statistics, and moves its light. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B9AD8), where it is exact; names translated to the US level program. */




typedef struct { u8 p0[0x2084]; s32 i2084; } G_B4F40;

typedef unsigned int Q_2B9AD8 __attribute__((mode(TI)));

typedef union { struct { float x, y, z, w; } f; Q_2B9AD8 q; } V_2B9AD8 __attribute__((aligned(16)));

typedef struct Data_2B9AD8 {
    V_2B9AD8 pos;
    V_2B9AD8 tip;
    struct Moby *target;
    float reach;
    short hold;
    short hold2;
    int timer;
    int light;
    int glow;
    struct Moby *target2;
    short tick;
    short f3E;
    short lock;
    short lock2;
    short f44;
    short f46;
    int sound;
} Data_2B9AD8;

typedef struct Stats_2B9AD8 { char pad0[0x98]; unsigned short uses; unsigned short longest; int levels; } Stats_2B9AD8;

typedef struct Stats2_2B9AD8 { char pad0[0xD0]; unsigned short uses; unsigned short longest; int levels; } Stats2_2B9AD8;

typedef struct Voice_2B9AD8 { char pad0[4]; unsigned char active; char pad5[0x13]; struct Moby *owner; char pad1C[0x54]; } Voice_2B9AD8;

typedef struct Aim_2B9AD8 { char pad0[4]; short kind; } Aim_2B9AD8;

typedef struct Light_2B9AD8 { float x, y, z, b; V_2B9AD8 pos; } Light_2B9AD8;

extern char D_0013E533[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int D_00141918; /* no foreign declaration */
extern int D_00141D70; /* no foreign declaration */
extern int D_0015EEA4;
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L00_00233db8(int, int);
extern int FUN_L00_00233e98();
extern int FUN_L00_002b94d0();
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_002ba970(char *);
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(int *arg0) __asm__("FUN_001f9740");
extern s32 voice_is_playing(struct Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern u32 D_0013C940[];
extern u8 D_0013E550[];
extern unsigned char *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
int FUN_L00_0023e5e0(u128 *v, float a, float b, int col);
void FUN_L00_00203908(int, int);
void FUN_L00_0020e4c8(void);
void FUN_L00_0020e698(void);
void FUN_L00_0023e838(int);
void FUN_L00_002b9310(char *moby);
extern int D_0015EEA4;
extern int D_0015ED84_2b2100 __asm__("D_0015ED84");
extern char D_0013F350_2b2100[] __asm__("D_0013F350");
extern float D_0015ED60_2b2100 __asm__("D_0015ED60");
extern Voice_2B9AD8 D_0013E550_2b2100[] __asm__("D_0013E550");
extern unsigned char D_0013E533_2b2100[] __asm__("D_0013E533");
extern unsigned short D_00141D70_2b2100[] __asm__("D_00141D70");
extern unsigned short D_00141918_2b2100[] __asm__("D_00141918");
extern int D_L00_00161590 __asm__("D_L00_00161590");
extern Light_2B9AD8 D_L00_001802C0_2b2100[] __asm__("D_L00_001802C0");
extern int FUN_L00_00233e98_2b2100a(int) __asm__("FUN_L00_00233e98");
extern void FUN_L00_00203908_2b2100(int, int) __asm__("FUN_L00_00203908");
extern int FUN_001f9770_2b2100a(short *) __asm__("FUN_001f9770");
extern int FUN_001f9740_2b2100(int *) __asm__("FUN_001f9740");
extern int FUN_L00_00233db8_2b2100a(int, int) __asm__("FUN_L00_00233db8");
extern int FUN_001f96f8_2b2100(int) __asm__("FUN_001f96f8");
extern int FUN_L00_0023e5e0_2b2100(void *, float, float, int) __asm__("FUN_L00_0023e5e0");
extern float FUN_001f9e90_2b2100(float, float) __asm__("FUN_001f9e90");
extern float FUN_001fa688_2b2100(float, float) __asm__("FUN_001fa688");
extern float FUN_001f9b48_2b2100(void *, void *) __asm__("FUN_001f9b48");
extern Aim_2B9AD8 *FUN_002141f8_2b2100(void *) __asm__("FUN_002141f8");
extern float FUN_001fa6c0_2b2100(int) __asm__("FUN_001fa6c0");
extern void FUN_001f9cf8_2b2100(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_001f9a10_2b2100(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_L00_0024f7c8_2b2100a(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_001f9bf8_2b2100(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_002b9310_2b2100(void *) __asm__("FUN_L00_002b9310");
extern void FUN_L00_002b94d0_2b2100(void *) __asm__("FUN_L00_002b94d0");
extern void FUN_001f4600_2b2100(void (*)(void), void *) __asm__("FUN_001f4600");
extern void FUN_0022d798_2b2100(int) __asm__("FUN_0022d798");
extern int FUN_L00_0028d8c0_2b2100(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_0022da68_2b2100(int, int, void *) __asm__("FUN_0022da68");
extern void FUN_L00_0020e4c8_2b2100a(void) __asm__("FUN_L00_0020e4c8");
extern void FUN_L00_0020e698_2b2100(void) __asm__("FUN_L00_0020e698");
extern void FUN_L00_0023e838_2b2100a(int) __asm__("FUN_L00_0023e838");

void tesla_claw_update(struct Moby *m) __asm__("FUN_L00_002b87e0");
void tesla_claw_update(struct Moby *m) {
    V_2B9AD8 t;
    Data_2B9AD8 *s = (Data_2B9AD8 *)m->pvars;
    int active;
    int done = ((struct Hero *)D_0013F350_2b2100)->items[0].state == 3;
    int ready;
    struct Hero *h;

    if ((controller_state.pressed & ((struct Hero *)D_0013F350_2b2100)->items[0].button_mask) &&
        !((struct Hero *)D_0013F350_2b2100)->unk20AC) {
        s->reach = 0.8f;
    }
    D_L00_0017AEBC = 0.9f;
    active = 0;
    ready = FUN_L00_00233e98_2b2100a(-1);
    if (D_00141D70_2b2100[0] == 0 && D_00141918_2b2100[0] == 0 &&
        ((struct Hero *)D_0013F350_2b2100)->unk20B5 >= 3) {
        if ((unsigned int)((struct Hero *)D_0013F350_2b2100)->state.control_mode < 2 || ((struct Hero *)D_0013F350_2b2100)->state.control_mode == 9)
            FUN_L00_00203908_2b2100(0x4E30, 0x81);
    }
    if ((controller_state.held & ((struct Hero *)D_0013F350_2b2100)->items[0].button_mask) && m->state != 4 &&
        m->state != 0 && !((struct Hero *)D_0013F350_2b2100)->unk20AC && ((struct Hero *)D_0013F350_2b2100)->state.current != 0x72) {
        if (s->timer == 0) {
            if (ready > 0) {
                if (FUN_001f9770_2b2100a(&s->tick)) {
                    FUN_001f9770_2b2100a(&s->f44);
                    FUN_001f9770_2b2100a(&s->f46);
                    FUN_L00_00233db8_2b2100a(-1, 1);
                    s->tick = FUN_001f96f8_2b2100(10);
                }
                if (s->light == -1) {
                    s->light = FUN_L00_0023e5e0_2b2100(&m->pos, 7.5f, 0.0f, 0);
                } else if (s->glow < FUN_001f96f8_2b2100(10)) {
                    s->glow++;
                }
                active = 1;
                if (s->target) {
                    if (2.0943f < FUN_001fa688_2b2100(((struct Hero *)D_0013F350_2b2100)->motion.rot.f[2],
                            FUN_001f9e90_2b2100(s->target->pos.x - ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[0],
                                                     s->target->pos.y - ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[1])) ||
                        !s->target->visible ||
                        20.0f < FUN_001f9b48_2b2100(&s->target->pos, &((struct Hero *)D_0013F350_2b2100)->motion.pos)) {
                        s->target = 0;
                        s->lock = 0;
                    }
                }
                if (s->target2) {
                    if (2.0943f < FUN_001fa688_2b2100(((struct Hero *)D_0013F350_2b2100)->motion.rot.f[2],
                            FUN_001f9e90_2b2100(s->target2->pos.x - ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[0],
                                                     s->target2->pos.y - ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[1])) ||
                        !s->target2->visible ||
                        20.0f < FUN_001f9b48_2b2100(&s->target2->pos, &((struct Hero *)D_0013F350_2b2100)->motion.pos)) {
                        s->target2 = 0;
                        s->lock2 = 0;
                    }
                }
                qcopy(&s->pos, &m->pos);
                s->pos.f.z += 0.7f;
                qcopy(&s->tip, &s->pos);
                if (s->target && s->lock) {
                    int lim = 50;
                    Aim_2B9AD8 *a;
                    short n;
                    s->hold++;
                    a = FUN_002141f8_2b2100(s->target);
                    if (a && a->kind == 1) lim = 5;
                    n = s->hold;
                    if (!(n < FUN_001f96f8_2b2100(lim))) {
                        s->target = 0;
                        s->lock = 0;
                    }
                } else {
                    s->hold = 0;
                }
                if (s->target2 && s->lock2) {
                    int lim = 50;
                    Aim_2B9AD8 *a;
                    short n;
                    s->hold2++;
                    a = FUN_002141f8_2b2100(s->target2);
                    if (a && a->kind == 1) lim = 5;
                    n = s->hold2;
                    if (!(n < FUN_001f96f8_2b2100(lim))) {
                        s->target2 = 0;
                        s->lock2 = 0;
                    }
                } else {
                    s->hold2 = 0;
                }
                s->reach += (FUN_001fa6c0_2b2100(D_0013E533_2b2100[0]) * 6.0f + 12.0f - s->reach) * (D_0015ED60_2b2100 * 0.07f);
                if (((struct Hero *)D_0013F350_2b2100)->state.current == 1) {
                    t.f.x = 1.2f;
                    t.f.y = 0.0f;
                    t.f.z = 0.0f;
                    FUN_001f9cf8_2b2100(&t, &t, D_L00_00166FD0);
                    s->pos.f.z = t.f.z + m->pos.z;
                    s->pos.f.x = t.f.x + ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[0];
                    s->pos.f.y = t.f.y + ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[1];
                    t.f.x = s->reach;
                    t.f.y = 0.0f;
                    t.f.z = 0.0f;
                    FUN_001f9cf8_2b2100(&t, &t, D_L00_00166FD0);
                    FUN_001f9a10_2b2100(&s->tip, &s->pos, &t);
                } else if (((struct Hero *)D_0013F350_2b2100)->unk22C8 == 1) {
                    FUN_L00_0024f7c8_2b2100a(m, 0, &s->pos);
                    t.f.x = 0.0f;
                    t.f.y = -s->reach;
                    t.f.z = 0.0f;
                    FUN_001f9cf8_2b2100(&t, &t, &m->rot_row0);
                    FUN_001f9a10_2b2100(&s->tip, &s->pos, &t);
                } else {
                    FUN_L00_0024f7c8_2b2100a(m, 0, &s->pos);
                    FUN_001f9bf8_2b2100(&s->tip, D_0013F350_2b2100 + 0x640, s->reach);
                    FUN_001f9a10_2b2100(&s->tip, &s->pos, &s->tip);
                }
                FUN_L00_002b9310_2b2100(m);
                FUN_L00_002b94d0_2b2100(m);
                FUN_001f4600_2b2100(FUN_L00_002ba970, m);
            } else {
                s->target = 0;
                s->target2 = 0;
                s->lock = 0;
                s->lock2 = 0;
                s->f44 = 0;
                s->f46 = 0;
            }
        }
    } else {
        s->target = 0;
        s->target2 = 0;
        s->lock = 0;
        s->lock2 = 0;
    }
    if (!active) {
        D_L00_00161590 = 1;
        if (s->glow > 0) s->glow--;
        if (s->sound != -1) {
            Voice_2B9AD8 *v = &D_0013E550_2b2100[s->sound];
            if (v[1].owner == m && v[1].active) FUN_0022d798_2b2100(s->sound);
        }
        s->sound = -1;
    } else if (!FUN_L00_0028d8c0_2b2100(m, s->sound)) {
        s->sound = FUN_0022da68_2b2100(4, 4, m);
    }
    if (!s->target || s->target->state == 0xFE || s->target->state == 0xFD || !(s->target->flags & 0x1000)) s->lock = 1;
    if (!s->target2 || s->target2->state == 0xFE || s->target2->state == 0xFD || !(s->target2->flags & 0x1000)) s->lock2 = 1;
    switch (m->state) {
    case 0:
        s->timer = FUN_001f96f8_2b2100(16);
        s->light = FUN_L00_0023e5e0_2b2100(&s->pos, 0.0f, 0.0f, 0);
        D_L00_00161590 = 1;
        m->state = 1;
        s->f3E = 20;
        s->sound = -1;
        s->target2 = 0;
        s->target = 0;
        s->lock = 0;
        s->lock2 = 0;
        s->f46 = 0;
        s->f44 = 0;
        break;
    case 1:
        FUN_001f9740_2b2100(&s->timer);
        if (active) {
            FUN_L00_0020e4c8_2b2100a();
            m->state = 3;
        }
        if ((controller_state.pressed & (h = (struct Hero *)D_0013F350_2b2100)->items[0].button_mask) && !h->unk20AC) {
            if (active) {
                Stats_2B9AD8 *st = (Stats_2B9AD8 *)(D_0014161B + 0x65);
                if (st->uses < 65535) st->uses++;
                if (FUN_001f96f8_2b2100(D_0015EEA4) / 600 > st->longest)
                    st->longest = FUN_001f96f8_2b2100(D_0015EEA4) / 600;
                st->levels = (st->levels | (1 << D_0015ED84_2b2100)) | 0x80000000;
                FUN_L00_0020e4c8_2b2100a();
                m->state = 3;
            } else {
                s->target = 0;
                s->target2 = 0;
                s->lock = 0;
                s->lock2 = 0;
                m->state = 1;
                FUN_0022da68_2b2100(0, 0, m);
                FUN_L00_0020e698_2b2100();
            }
        }
        break;
    case 2:
        break;
    case 3:
        if ((controller_state.released & ((struct Hero *)D_0013F350_2b2100)->items[0].button_mask) || !active ||
            ((struct Hero *)D_0013F350_2b2100)->unk20AC) {
            h = (struct Hero *)D_0013F350_2b2100;
            if (FUN_001f96f8_2b2100(70) < h->unk1BC) {
                if (((Stats2_2B9AD8 *)(D_0014161B + 0x22D))->uses < 65535) ((Stats2_2B9AD8 *)(D_0014161B + 0x22D))->uses++;
            }
            if (FUN_001f96f8_2b2100(D_0015EEA4) / 600 > ((Stats2_2B9AD8 *)(D_0014161B + 0x22D))->longest)
                ((Stats2_2B9AD8 *)(D_0014161B + 0x22D))->longest = FUN_001f96f8_2b2100(D_0015EEA4) / 600;
            ((Stats2_2B9AD8 *)(D_0014161B + 0x22D))->levels = (((Stats2_2B9AD8 *)(D_0014161B + 0x22D))->levels | (1 << D_0015ED84_2b2100)) | 0x80000000;
            if (((struct Hero *)D_0013F350_2b2100)->unk1BC < FUN_001f96f8_2b2100(30))
                ((struct Hero *)D_0013F350_2b2100)->unk20B5++;
            else
                ((struct Hero *)D_0013F350_2b2100)->unk20B5 = 0;
            s->target = 0;
            s->target2 = 0;
            s->lock = 0;
            s->lock2 = 0;
            s->reach = 0.0f;
            FUN_L00_0020e698_2b2100();
            m->state = 1;
        }
        break;
    case 4:
        break;
    case 5:
        break;
    }
    if (s->light != -1) {
        float k;
        qzero(&t);
        t.f.x = 2.0f;
        t.f.z = 2.0f;
        FUN_001f9cf8_2b2100(&t, &t, D_0013F350_2b2100 + 0x640);
        D_L00_001802C0_2b2100[s->light].pos.f.x = ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[0] + t.f.x;
        D_L00_001802C0_2b2100[s->light].pos.f.y = ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[1] + t.f.y;
        D_L00_001802C0_2b2100[s->light].pos.f.z = ((struct Hero *)D_0013F350_2b2100)->motion.pos.f[2] + t.f.z;
        D_L00_001802C0_2b2100[s->light].pos.f.w = 7.5f;
        k = FUN_001fa6c0_2b2100(s->glow) / (float)FUN_001f96f8_2b2100(10);
        D_L00_001802C0_2b2100[s->light].z = k;
        k = k * 0.7f;
        D_L00_001802C0_2b2100[s->light].y = k;
        D_L00_001802C0_2b2100[s->light].x = D_L00_001802C0_2b2100[s->light].y;
        if (done && s->light != -1) {
            FUN_L00_0023e838_2b2100a(s->light);
            s->light = -1;
        }
    }
    if (done && s->light == -1) m->state = 4;
}

#include "rnc/overlay/beam_vector.h"
extern int D_L00_001615AC_gp __asm__("D_L00_001615AC") __attribute__((sda));
extern int D_L00_001615B4_gp __asm__("D_L00_001615B4") __attribute__((sda));
extern int D_L00_001615C8_gp __asm__("D_L00_001615C8") __attribute__((sda));
extern int D_L00_00161590_gp __asm__("D_L00_00161590") __attribute__((sda));
extern int frames_2b9310(int) __asm__("FUN_001f96f8");
extern void vsub_2b9310(void *, void *, void *) __asm__("FUN_001f9a28");
extern void vscale_2b9310(void *, void *, float) __asm__("FUN_001f9bf8");
extern void vadd_2b9310(void *, void *, void *) __asm__("FUN_001f9a10");
extern void vzero_2b9310(void *) __asm__("FUN_001f99f8");

extern int D_L00_0016167C __attribute__((sda));
extern int D_L00_00161624 __attribute__((sda));
extern int D_L00_00161678;
extern float D_L00_00161594 __attribute__((sda));
extern short D_L00_00161658[] __attribute__((section(".sdata")));
extern short D_L00_00161668;
extern short D_L00_0016166A;
extern short D_L00_0016166C;
extern short D_L00_0016166E;
extern short D_L00_00161670;
extern short D_L00_00161672;
extern short D_L00_00161674;
extern short D_L00_00161676;
extern V2b9310 D_L00_001DB5B0[];
extern V2b9310 D_L00_001DB5C0[];
/* Starts the beam when one is pending: lays 19 points from the moby toward its target and resets the segment state. */
void FUN_L00_002b9310(char *moby) {
    V2b9310 d;
    V2b9310 step;
    char *data;
    int i;
    int j;

    data = *(char **)(moby + 0x78);
    if (D_L00_00161590_gp == 0) {
        return;
    }
    D_L00_00161590_gp = 0;
    D_L00_0016167C = 0;
    D_L00_00161678 = frames_2b9310(D_L00_00161624);
    D_L00_001615F8 = 0x7F2020;
    vsub_2b9310(&d, data + 0x10, data);
    vscale_2b9310(&step, &d, D_L00_00161594 / 20.0f);
    qcopy(D_L00_001DB5C0, data);
    for (i = 1; i < 20; i++) {
        vadd_2b9310(&D_L00_001DB5C0[i], &D_L00_001DB5B0[i], &step);
        D_L00_001DB840[i] = 0;
        vzero_2b9310(&D_L00_001DB890[i]);
        D_L00_001DB9D0[i] = 0;
    }
    for (j = 0; j < 4; j++) {
        D_L00_00161658[j] = -1;
    }
    D_L00_00161668 = 8;
    D_L00_0016166C = 8;
    D_L00_00161670 = 4;
    D_L00_0016166A = 4;
    D_L00_00161672 = 2;
    D_L00_00161674 = 4;
    D_L00_0016166E = 4;
    D_L00_00161676 = 2;
    D_L00_001615AC_gp = 0;
    D_L00_001615B4_gp = 0;
    D_L00_001615C8_gp = 0;
}

/* D_0013F350, the shared Level-00 state block; only the offsets the listing
   names are declared. */
/* the per-object block reached through o+0x78: 0x20..0x2C are s32 counters and
   0x30..0x3C are f32 rates (the case-1/3 bodies divide five constants by
   scale_game_frames(2) / scale_game_frames(8) and store the results there). */
typedef struct {
    s32 i0;
    char pad4[4];
    s32 i8;
    s32 iC;
    char pad10[0x10];
    f32 f20;
    f32 f24;
    f32 f28;
    f32 f2C;
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
} V_4198;

typedef struct {
    char pad00[0x18];
    f32 f18;
    char pad1C[4];
    u8 state;
    char pad21[0xF];
    u8 u30;
    char pad31[3];
    u16 u34;
    char pad36[0x42];
    V_4198 *v;
} O_4198;

extern s32 FUN_001f96f8_4198(s32) __asm__("FUN_001f96f8");
extern s32 FUN_001f9740_4198(void *) __asm__("FUN_001f9740");
extern void FUN_001f4600_4198(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L00_002b3628_4198(void) __asm__("FUN_L00_002b3628");
extern s32 FUN_L00_0020d498_4198(s32) __asm__("FUN_L00_0020d498");
extern void FUN_0020c828_4198(void *) __asm__("FUN_0020c828");
extern void FUN_L00_002b3c50_4198(void *) __asm__("FUN_L00_002b3c50");

void FUN_L00_002b4198(void *p) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_WAIT_FOR_BEAM = 1,
        STATE_BEAM_GROW = 2,
        STATE_BEAM_ACTIVE = 3,
        STATE_BEAM_SHRINK = 4
    };
    O_4198 *o = (O_4198 *)p;
    V_4198 *v = o->v;
    s32 flag = 0;
    s32 st;
    s32 lim;

    if (hero.items[3].item_id == 3) {
        if (o->state == 1 || o->state == 3) {
            st = hero.state.current;
            if ((u32)(st - 0xB) < 4 ||
                (st == 0x1C && ((u8 *)((void *)hero.moby))[0x52] == ((u8 *)((void *)hero.moby))[0x53] &&
                 hero.anim_frame >= 6.0f && hero.anim_frame <= 16.0f)) {
                if (!hero.unk41E)
                    flag = 1;
            }
            if (hero.state.current == HERO_STATE_PACK_LONG_JUMP) {
                if (hero.state_timer < FUN_001f96f8_4198(0x2C)) {
                    flag = 1;
                }
            }
            if (hero.state.current == 0x22) {
                if (hero.state_timer < FUN_001f96f8_4198(0xF) ||
                    (FUN_001f96f8_4198(0x21) < hero.state_timer && hero.air_frames.s != 0)) {
                    flag = 1;
                }
            }
            if (hero.state.current == HERO_STATE_LAND || hero.state.current == 0x81) {
                flag = 1;
            }
        }
    }
    switch (o->state) {
    case STATE_INITIALIZE:
        o->f18 += 0.5f;
        o->state = 1;
        o->u30 = 0xFF;
        o->u34 |= 1;
        break;
    case STATE_WAIT_FOR_BEAM:
        if (FUN_L00_0020d498_4198(3) != 3) {
            FUN_0020c828_4198(o);
        } else if (flag != 0) {
            o->state = 2;
            v->i0 = FUN_001f96f8_4198(2);
            v->f30 = 0.0125f / FUN_001f96f8_4198(2);
            v->f34 = 0.2f / FUN_001f96f8_4198(2);
            v->f38 = 0.05f / FUN_001f96f8_4198(2);
            v->f3C = 0.025f / FUN_001f96f8_4198(2);
            v->f20 = 0.0f;
            v->f24 = 0.0f;
            v->f28 = 0.0f;
            v->f2C = 0.0f;
        }
        break;
    case STATE_BEAM_GROW:
        v->f20 += v->f30;
        v->f24 += v->f34;
        v->f28 += v->f38;
        v->f2C += v->f3C;
        v->i8 = ((s32)hero.items[3].moby);
        FUN_001f4600_4198(FUN_L00_002b3628_4198, o);
        if (FUN_001f9740_4198(v) != 0) {
            o->state = 3;
            v->f20 = 0.0125f;
            v->f24 = 0.2f;
            v->f28 = 0.05f;
            v->iC = 0;
            v->f2C = 0.025f;
        }
        break;
    case STATE_BEAM_ACTIVE:
        v->i8 = ((s32)hero.items[3].moby);
        FUN_001f4600_4198(FUN_L00_002b3628_4198, o);
        if (flag == 0) {
            o->state = 4;
            v->i0 = FUN_001f96f8_4198(8);
            v->f30 = 0.0125f / FUN_001f96f8_4198(8);
            v->f34 = 0.2f / FUN_001f96f8_4198(8);
            v->f38 = 0.05f / FUN_001f96f8_4198(8);
            v->f3C = 0.025f / FUN_001f96f8_4198(8);
        } else {
            v->f20 = 0.0125f;
            FUN_L00_002b3c50_4198(o);
        }
        break;
    case STATE_BEAM_SHRINK:
        if (FUN_001f9740_4198(v) != 0) {
            o->state = 1;
        } else {
            v->f20 -= v->f30;
            v->f24 -= v->f34;
            v->f28 -= v->f38;
            v->f2C -= v->f3C;
            v->i8 = ((s32)hero.items[3].moby);
            FUN_001f4600_4198(FUN_L00_002b3628_4198, o);
        }
        break;
    }
}

f32 D_L15_001D034C[1] __attribute__((section(".data"))) = {0};

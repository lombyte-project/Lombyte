/* Shared moby classes: sandmouse. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/state/options.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "qzero.h"
extern s32 truncate_float_to_s32_b618(f32) __asm__("FUN_001fa6d0");
extern f32 distance_xyz_b618(void *, void *) __asm__("FUN_001f9b80");
extern s32 is_point_inside_clip_volume_b618(void *, s32) __asm__("FUN_00214720");
extern s32 is_timer_active_b618(void *) __asm__("FUN_001f9740");
extern s32 random_integer_between_b618(s32, s32) __asm__("FUN_L00_00257b90");
extern f32 itof_s_b618(s32) __asm__("FUN_001fa6c0");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern void interpolate_vector_b618(void *, void *, void *, f32) __asm__("FUN_L00_0025b6b8");
extern s32 allocate_colored_effect_slot(void *, f32, f32, f32, f32, f32) __asm__("FUN_L00_0023e738");
extern void release_point_light_slot(s32) __asm__("FUN_L00_0023e838");
extern void mark_moby_for_removal_b618(void *) __asm__("FUN_0020c828");
extern char D_L01_00167240_b618[] __asm__("D_L01_00167240");
extern char *D_L01_0015FFD8_b618 __asm__("D_L01_0015FFD8");

extern char D_L01_00180740[];
void update_moby_interpolated_point_light(u8 *moby) __asm__("FUN_L01_0030b618");

void update_moby_interpolated_point_light(u8 *moby) {
    enum {
        EFFECT_STATE_INITIALIZE = 0,
        EFFECT_STATE_UPDATE = 1
    };
    s32 state = moby[0x20];
    char *effect_vars = *(char **)(moby + 0x78);
    s32 i;
    s32 current;
    s32 next;
    s32 slot;
    f32 elapsed;
    f32 active;
    f32 phase;
    f32 blend;
    f32 cutoff;
    OvlQuad first;
    OvlQuad second;

    switch (state) {
    case EFFECT_STATE_INITIALIZE:
        if (*(s32 *)(effect_vars + 0x58) < 2) {
            mark_moby_for_removal_b618(moby);
            return;
        }
        {
            s32 i;
            for (i = 0; i < *(s32 *)(effect_vars + 0x58); i++) {
                f32 *point = ((f32 *)effect_vars) + i * 4;
                point[0] *= 0.00390625f;
                point[1] *= 0.00390625f;
                point[2] *= 0.00390625f;
                point[3] *= 0.00390625f;
            }
        }
        *(s32 *)(effect_vars + 0x68) = -1;
        if (moby[0x30] < truncate_float_to_s32_b618(*(f32 *)(effect_vars + 0x5c)) + 4)
            moby[0x30] = truncate_float_to_s32_b618(*(f32 *)(effect_vars + 0x5c)) + 4;
        moby[0x20] = 1;
        return;
    case EFFECT_STATE_UPDATE:
        break;
    default:
        return;
    }

    elapsed = distance_xyz_b618(moby + 0x10, D_L01_00167240_b618);
    active = elapsed < *(f32 *)(effect_vars + 0x5c) ? 1.0f : 0.0f;
    if (active != 0.0f && *(s32 *)(effect_vars + 0x80) != -1) {
        if (!is_point_inside_clip_volume_b618(D_L01_00167240_b618, *(s32 *)(effect_vars + 0x80)))
            active = 0.0f;
    }
    i = *(s32 *)(effect_vars + 0x84);
    if (i != -1 && *(s8 *)((D_L01_0015FFD8_b618 + 0x20) + i * 0x100) < 0) {
        slot = *(s32 *)(effect_vars + 0x68);
        if (slot != -1)
            release_point_light_slot(slot);
        mark_moby_for_removal_b618(moby);
        return;
    }
    if (active != 0.0f) {

        if (is_timer_active_b618(effect_vars + 0x6c)) {
            current = random_integer_between_b618(*(s32 *)(effect_vars + 0x50), *(s32 *)(effect_vars + 0x54));
            *(s32 *)(effect_vars + 0x74) = *(s32 *)(effect_vars + 0x78);
            *(s32 *)(effect_vars + 0x70) = current;
            *(s32 *)(effect_vars + 0x6c) = current;
            next = random_integer_between_b618(1, *(s32 *)(effect_vars + 0x58) - 1);
            i = (*(s32 *)(effect_vars + 0x78) + next) % *(s32 *)(effect_vars + 0x58);
            *(s32 *)(effect_vars + 0x7c) = *(s32 *)(effect_vars + 0x7c) == 0;
            *(s32 *)(effect_vars + 0x78) = i;
        }
        phase = itof_s_b618(*(s32 *)(effect_vars + 0x70) - *(s32 *)(effect_vars + 0x6c));
        phase = phase / itof_s_b618(*(s32 *)(effect_vars + 0x70));
        phase *= 3.1415927f;
        blend = phase * 0.5f;
        if (*(s32 *)(effect_vars + 0x7c) != 0)
            blend += 1.5707964f;
        blend = fast_sin(blend);

        if (*(s32 *)(effect_vars + 0x7c) != 0) {
            first = ((OvlQuad *)effect_vars)[*(s32 *)(effect_vars + 0x78)];
            second = ((OvlQuad *)effect_vars)[*(s32 *)(effect_vars + 0x74)];
            interpolate_vector_b618(effect_vars + 0x40, &first, &second, blend);
        } else {
            first = ((OvlQuad *)effect_vars)[*(s32 *)(effect_vars + 0x74)];
            second = ((OvlQuad *)effect_vars)[*(s32 *)(effect_vars + 0x78)];
            interpolate_vector_b618(effect_vars + 0x40, &first, &second, blend);
        }
        cutoff = *(f32 *)(effect_vars + 0x5c) * 0.75f;
        {
            f32 denominator = *(f32 *)(effect_vars + 0x5c) - cutoff;
            if (cutoff < elapsed) {
                f32 fade = (elapsed - cutoff) / denominator;
                qzero(&first);
                second = *(OvlQuad *)(effect_vars + 0x40);
                interpolate_vector_b618(effect_vars + 0x40, &second, &first, fade);
            }
        }
        slot = *(s32 *)(effect_vars + 0x68);
        if (slot == -1) {
            slot = allocate_colored_effect_slot(moby + 0x10, *(f32 *)(effect_vars + 0x60), *(f32 *)(effect_vars + 0x4c),
                                  *(f32 *)(effect_vars + 0x40), *(f32 *)(effect_vars + 0x44),
                                  *(f32 *)(effect_vars + 0x48));
            *(s32 *)(effect_vars + 0x68) = slot;
            return;
        }
        {
            char *effect_entry = D_L01_00180740 + slot * 0x20;
            qcopy(effect_entry + 0x10, moby + 0x10);
            *(f32 *)(effect_entry + 0x1c) = *(f32 *)(effect_vars + 0x60);
            qcopy(effect_entry, effect_vars + 0x40);
        }
    } else {
        slot = *(s32 *)(effect_vars + 0x68);
        if (slot != -1) {
            release_point_light_slot(slot);
            *(s32 *)(effect_vars + 0x68) = -1;
        }
    }
}
#include "rnc/overlay/quad.h"
typedef union {
    OvlQuad q;
    f32 f[4];
} GlowVec_bfa8;
typedef struct { GlowVec_bfa8 v[4]; } GlowCorners_bfa8;

typedef struct {
    GlowVec_bfa8 corner[4];
    u32 color[4];
    s32 unk50, unk54, unk58;
    f32 unk5C, unk60;
    s32 unk64;
    f32 unk68, unk6C;
    s64 unk70, tex, unk80, unk88;
} GlowQuad_bfa8;

extern GlowVec_bfa8 D_L01_0020B9D0[4];
extern char D_L01_00167240[];
extern GlowVec_bfa8 D_0013F5E0[];
#include "rnc/overlay/shared/gameplay/entities/sandmouse_globals.h"
extern f32 moby_quad_c0_offset_scale_gp __asm__("D_L01_00162064") __attribute__((sda));
extern f32 moby_quad_e0_offset_scale_gp __asm__("D_L01_0016206C") __attribute__((sda));
extern s64 get_effect_texture_bfa8(s32) __asm__("FUN_001f44b8");
extern void add_vector_xyz_bfa8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_bfa8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz_bfa8(void *, void *, f32) __asm__("FUN_001f9a68");
extern void cross_vectors_xyz_bfa8(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void normalize_vector_xyz_bfa8(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void transform_vector_bfa8(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_geometry_quad_bfa8(void *, void *, s32) __asm__("FUN_001f7d30");

void draw_moby_offset_textured_quad(char *moby) __asm__("FUN_L01_0030bfa8");

void draw_moby_offset_textured_quad(char *moby) {
    struct {
        GlowQuad_bfa8 quad;
        GlowVec_bfa8 src_corner[4];
        struct { GlowVec_bfa8 mat0, mat1, mat2, point, partial, offset; } basis;
    } local;
    GlowVec_bfa8 *basis_axis_or_quad_corner;
    GlowVec_bfa8 *basis_axis_or_source_corner;
    GlowVec_bfa8 *basis;
    s32 i;
    f32 one;
    OvlQuad source_corner_0, source_corner_1, source_corner_2, source_corner_3;

    i = 3;
    one = 1.0f;
    source_corner_0 = D_L01_0020B9D0[0].q;
    source_corner_1 = D_L01_0020B9D0[1].q;
    source_corner_2 = D_L01_0020B9D0[2].q;
    source_corner_3 = D_L01_0020B9D0[3].q;
    local.src_corner[0].q = source_corner_0;
    local.src_corner[1].q = source_corner_1;
    local.src_corner[2].q = source_corner_2;
    local.src_corner[3].q = source_corner_3;
    local.quad.tex = get_effect_texture_bfa8(0xB);
    local.quad.unk88 = 0x8000000048L;
    local.quad.unk80 = 0xFF9000000260L;
    local.quad.unk70 = 5;
    local.quad.unk50 = 0;
    local.quad.unk54 = 0;
    local.quad.unk58 = 0;
    local.quad.unk5C = one;
    local.quad.unk60 = one;
    local.quad.unk64 = 0;
    local.quad.unk68 = one;
    local.quad.unk6C = one;

    scale_vector_xyz_bfa8(&local.basis.offset, moby + 0xc0, moby_quad_c0_offset_scale_gp);
    scale_vector_xyz_bfa8(&local.basis.partial, moby + 0xe0, moby_quad_e0_offset_scale_gp);
    add_vector_xyz_bfa8(&local.basis.offset, &local.basis.offset, &local.basis.partial);
    basis_axis_or_source_corner = &local.basis.mat0;
    add_vector_xyz_bfa8(&local.basis.point, &local.basis.offset, moby + 0x10);
    subtract_vector_xyz_bfa8(basis_axis_or_source_corner, D_L01_00167240, &local.basis.point);
    normalize_vector_xyz_bfa8(basis_axis_or_source_corner, basis_axis_or_source_corner, one);
    basis_axis_or_quad_corner = &local.basis.mat1;
    cross_vectors_xyz_bfa8(basis_axis_or_quad_corner, basis_axis_or_source_corner, D_0013F5E0);
    normalize_vector_xyz_bfa8(basis_axis_or_quad_corner, basis_axis_or_quad_corner, -1.0f);
    cross_vectors_xyz_bfa8(&local.basis.mat2, basis_axis_or_quad_corner, basis_axis_or_source_corner);
    basis = basis_axis_or_source_corner;

    local.quad.color[3] = 0x202020FF;
    local.quad.color[2] = 0x202020FF;
    local.quad.color[1] = 0x202020FF;
    local.quad.color[0] = 0x202020FF;
    basis_axis_or_source_corner = local.src_corner;
    basis_axis_or_quad_corner = local.quad.corner;
    do {
        scale_vector_xyz_bfa8(basis_axis_or_quad_corner, basis_axis_or_source_corner, 0.4f);
        basis_axis_or_source_corner++;
        transform_vector_bfa8(basis_axis_or_quad_corner, basis_axis_or_quad_corner, basis);
        basis_axis_or_quad_corner++;
        i--;
    } while (i >= 0);
    draw_geometry_quad_bfa8(&local.quad, 0, 0);
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030DC70), where it is exact; names translated to the US level program. */


extern char *func_0020D348_m(int);
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");

char *spawn_sandmouse_projectile(char *owner, char *spawn_position, float target_yaw, float target_pitch, float initial_yaw, float initial_pitch) __asm__("FUN_L01_0030c898");

char *spawn_sandmouse_projectile(char *owner, char *spawn_position, float target_yaw, float target_pitch, float initial_yaw, float initial_pitch) {
    OvlQuad spawn_position_copy;
    char *cp = (char *)&spawn_position_copy;
    char *spawned_moby;
    spawn_position_copy = *(OvlQuad *)spawn_position;
    spawned_moby = func_0020D348_m(0x661);
    if (spawned_moby) {
        char *moby_vars = *(char **)(spawned_moby + 0x78);
        int lifetime_frames;
        *(char **)(moby_vars + 0x10) = owner;
        ((unsigned char *)spawned_moby)[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0x7F;
        spawned_moby[0x31] = 1;
        *(long *)(spawned_moby + 0x38) = *(long *)(owner + 0x38);
        *(float *)(spawned_moby + 0x44) = initial_pitch;
        *(float *)(spawned_moby + 0x48) = initial_yaw;
        *(int *)(spawned_moby + 0x40) = 0;
        qcopy(spawned_moby + 0x10, cp);
        *(float *)(moby_vars + 8) = target_yaw;
        *(float *)(moby_vars + 0xC) = target_pitch;
        *(int *)moby_vars = 0;
        *(int *)(moby_vars + 4) = 0;
        lifetime_frames = scale_game_frames(0x23);
        *(short *)(moby_vars + 0x14) = lifetime_frames;
        *(short *)(moby_vars + 0x16) = lifetime_frames;
        spawned_moby[0x23] = 0x40;
        *(unsigned short *)(spawned_moby + 0x34) |= 0xA00;
        *(float *)(spawned_moby + 0x2C) = *(float *)(spawned_moby + 0x2C) * 0.5f;
    }
    return spawned_moby;
}
typedef struct {
    OvlQuad dir;
    void *owner;
    s32 flags;
    u8 b28;
    u8 b29;
    u16 id;
    f32 scale;
    s32 count;
} DropProbe_30c9a8;

extern f32 spring_turn(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");
extern void build_spherical_offset_30c9a8(f32 *, f32, f32, f32) __asm__("FUN_00214db0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 distance_xyz_30c9a8(void *, void *) __asm__("FUN_001f9b80");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz_30c9a8(void *, void *, f32) __asm__("FUN_001f9c48");
extern s32 is_timer_active_30c9a8(void *) __asm__("FUN_001f9770");
extern s32 collision_line_30c9a8(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");
extern s32 collision_hit_material(void) __asm__("FUN_001f0b58");
extern void mark_moby_for_removal_30c9a8(void *) __asm__("FUN_0020c828");
extern f32 D_L01_00167240_30c9a8[] __asm__("D_L01_00167240");
extern OvlQuad D_0013F3D0_30c9a8 __asm__("D_0013F3D0");

extern OvlQuad D_L01_001742E0;
void update_sandmouse_projectile(char *moby) __asm__("FUN_L01_0030c9a8");

void update_sandmouse_projectile(char *moby) {
    char *moby_vars = *(char **)(moby + 0x78);
    f32 *moby_position;
    OvlVec4 old_pos;
    OvlVec4 offset;
    DropProbe_30c9a8 probe;
    f32 desired_yaw, desired_pitch;
    f32 pitch_k, pitch_d, pitch_max;
    f32 height = *(f32 *)(*(char **)(moby + 0x24) + 0x24);

    if (*(f32 *)(moby + 0x2c) < height)
        *(f32 *)(moby + 0x2c) += frame_scale * 0.21f * height;

    if (*(s16 *)(moby_vars + 0x14) < scale_game_frames(0x23) - scale_game_frames(1)) {
        desired_yaw = *(f32 *)(moby_vars + 8);
        desired_pitch = *(f32 *)(moby_vars + 0xc);
    } else {
        desired_yaw = *(f32 *)(moby + 0x48);
        desired_pitch = *(f32 *)(moby + 0x44);
    }
    moby_position = (f32 *)(moby + 0x10);
    desired_yaw = spring_turn((f32 *)moby_vars, *(f32 *)(moby + 0x48), desired_yaw,
                                            frame_time_sq * 4.712389f,
                                            frame_time_sq * 3.1415927f,
                                            frame_time * 3.1415927f);
    pitch_d = frame_time_sq * 3.1415927f;
    pitch_k = frame_time_sq * 4.712389f;
    pitch_max = frame_time * 3.1415927f;
    *(f32 *)(moby + 0x48) = desired_yaw;
    {
        f32 pitch_result = spring_turn((f32 *)(moby_vars + 4), *(f32 *)(moby + 0x44), desired_pitch,
                                                pitch_k, pitch_d, pitch_max);
        pitch_max = frame_time * 40.0f;
        *(f32 *)(moby + 0x44) = pitch_result;
        build_spherical_offset_30c9a8(offset.f, pitch_max,
                                       *(f32 *)(moby + 0x48), -pitch_result);
    }
    qcopy(&old_pos, moby_position);
    fast_vec_add(moby_position, moby_position, &offset);
    if (*(f32 *)(moby + 0x10) < 2.0f || *(f32 *)(moby + 0x10) > 1021.0f ||
        *(f32 *)(moby + 0x14) < 2.0f || *(f32 *)(moby + 0x14) > 1021.0f ||
        *(f32 *)(moby + 0x18) < 2.0f || *(f32 *)(moby + 0x18) > 1021.0f ||
        *(f32 *)(moby + 0x10) < 0.0f || *(f32 *)(moby + 0x14) < 0.0f || *(f32 *)(moby + 0x18) < 0.0f ||
        distance_xyz_30c9a8(moby_position, D_L01_00167240_30c9a8) > 64.0f) {
        mark_moby_for_removal_30c9a8(moby);
        return;
    }
    probe.owner = moby;
    probe.flags = 0x10001;
    probe.scale = 0.5f;
    probe.count = 1;
    fast_vec_sub(&probe.dir, moby_position, &D_0013F3D0_30c9a8);
    scale_vector_xyz_30c9a8(&probe.dir, &probe.dir, 1.0f);
    ((f32 *)&probe.dir)[2] = 1.0f;
    ((f32 *)&probe.dir)[3] = 5627.9248f;
    probe.b28 = 1;
    probe.b29 = 1;
    probe.id = *(u16 *)(moby + 0xa6);
    if (is_timer_active_30c9a8(moby_vars + 0x14)) {
        mark_moby_for_removal_30c9a8(moby);
        return;
    }
    {
        if (!collision_line_30c9a8(&old_pos, moby_position, 0, *(void **)(moby_vars + 0x10), &probe))
            return;
        if (!collision_hit_material()) {
            if (*(s16 *)(moby_vars + 0x14) > scale_game_frames(3))
                *(s16 *)(moby_vars + 0x14) = scale_game_frames(3);
            return;
        }
        qcopy(moby_position, &D_L01_001742E0);
    }
    mark_moby_for_removal_30c9a8(moby);
}
#define NOT_SDA

#define MACRO_ADDR

/* offset a position by two camera-relative vectors, then raise z by 1 */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030E638), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void compute_sandmouse_follow_goal(void *unused, float *destination_position) __asm__("FUN_L01_0030d260");

void compute_sandmouse_follow_goal(void *unused, float *destination_position) {
    float normalized_offset[4];
    float unit_offset[4];
    char *hero_bytes = D_0013E533 + 0xE1D;
    fast_vec_normalize(normalized_offset, *(char **)(hero_bytes + 0x2080) + 0xD0, 1.1f);
    fast_vec_normalize(unit_offset, *(char **)(hero_bytes + 0x2080) + 0xE0, 1.0f);
    fast_vec_add(destination_position, normalized_offset, unit_offset);
    fast_vec_add(destination_position, destination_position, hero_bytes + 0x80);
    destination_position[2] = destination_position[2] + 1.0f;
}
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/gameplay/entities/moby.h"

/* The part of a moby class FUN_L01_0030d308 reads */
typedef struct {
    u8 pad00[0x46];
    s16 kind;      /* 0x46: 5 for lock-on targets */
} TargetClass_30d308;

extern f32 distance_xyz_30d308(void *, void *) __asm__("FUN_001f9b48");
extern f32 FUN_001f9988_30d308(f32) __asm__("FUN_001f9988");
extern f32 atan2_30d308(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_difference_between_rotations_30d308(f32, f32) __asm__("FUN_001fa688");
extern f32 *FUN_002141f8_30d308(struct Moby *) __asm__("FUN_002141f8");
extern s32 collision_line_30d308(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");

extern struct Moby *D_L01_001ABE80[];
extern CollisionHit D_L01_001742C0;
/* FUN_L01_002e32a8 over the null-terminated moby table D_L01_001ABE80: the
   best lock-on target for moby seen from `from` along `ang`. */
struct Moby *select_sandmouse_projectile_target(struct Moby *moby, f32 *from, f32 *reference_rotation, f32 yawMax, f32 pitchMax,
                              f32 range, f32 near, f32 nearYaw, f32 nearPitch) __asm__("FUN_L01_0030d308");

struct Moby *select_sandmouse_projectile_target(struct Moby *moby, f32 *from, f32 *reference_rotation, f32 yawMax, f32 pitchMax,
                              f32 range, f32 near, f32 nearYaw, f32 nearPitch) {
    f32 tp[4];
    struct Moby *best = 0;
    f32 bestScore = 1.0e9f;
    s32 i;
    struct Moby *candidate_moby;

    for (i = 0; (candidate_moby = D_L01_001ABE80[i]) != 0; i++) {
        f32 *hp;
        s32 candidate_target_kind;
        f32 candidate_distance, score, pitch_error_squared;
        struct Moby *hit;

        if (candidate_moby->draw_dist == 0) continue;
        hp = FUN_002141f8_30d308(candidate_moby);
        if (candidate_moby == 0 || candidate_moby->pclass == 0) continue;
        candidate_target_kind = ((TargetClass_30d308 *)candidate_moby->pclass)->kind;
        if (candidate_target_kind != 5 || hp == 0 || !(0.0f < *hp)) continue;
        candidate_distance = distance_xyz_30d308(from, &candidate_moby->pos);
        if (!(candidate_distance < range)) continue;
        qcopy(tp, &candidate_moby->pos);
        tp[2] += 0.4f;
        score = fast_difference_between_rotations_30d308(reference_rotation[2], atan2_30d308(tp[0] - from[0], tp[1] - from[1]));
        score = score * score;
        if (!(score < yawMax * yawMax)) {
            if (!(candidate_distance < near) || !(score < nearYaw * nearYaw)) continue;
        }
        pitch_error_squared = fast_difference_between_rotations_30d308(reference_rotation[1], atan2_30d308(candidate_distance, tp[2] - from[2]));
        pitch_error_squared = pitch_error_squared * pitch_error_squared;
        if (!(pitch_error_squared < pitchMax * pitchMax)) {
            if (!(candidate_distance < near) || !(pitch_error_squared < nearPitch * nearPitch)) continue;
        }
        score *= pitch_error_squared;
        score *= FUN_001f9988_30d308(distance_xyz_30d308(from, tp));
        if (!(score < bestScore)) continue;
        if (collision_line_30d308(from, tp, 0, moby, 0)) {
            hit = (struct Moby *)D_L01_001742C0.moby;
            if (hit == 0 || hit == hero.moby || hit == 0 || hit->pclass == 0
                || ((TargetClass_30d308 *)hit->pclass)->kind != candidate_target_kind) {
                continue;
            }
        }
        bestScore = score;
        best = candidate_moby;
    }
    return best;
}
#include "sda.h"


extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern s32 allocate_voice_for_target_entry(s32 entry_index, s32 flags, struct VoiceTarget *target) __asm__("FUN_0022da68");
struct Moby *select_sandmouse_projectile_target(struct Moby *moby, f32 *from, f32 *ang, f32 yawMax, f32 pitchMax,
                              f32 range, f32 near, f32 nearYaw, f32 nearPitch) __asm__("FUN_L01_0030d308");
extern void FUN_L01_0030c898_c(void *, void *, float, float, float,
                               float) __asm__("FUN_L01_0030c898");

extern float sandmouse_projectile_target_range_gp __asm__("D_L01_00162084") __attribute__((sda));
extern float sandmouse_projectile_target_yaw_limit_degrees_gp __asm__("D_L01_00162088") __attribute__((sda));
extern float sandmouse_projectile_target_pitch_limit_degrees_gp __asm__("D_L01_0016208C") __attribute__((sda));
extern float sandmouse_projectile_spawn_offset_distance_gp __asm__("D_L01_00162090") __attribute__((sda));
extern float sandmouse_projectile_fallback_aim_height_gp __asm__("D_L01_00162094") __attribute__((sda));
extern int D_L01_00162080_i __asm__("D_L01_00162080") __attribute__((sda));

typedef union {
    OvlQuad q;
    float f[4];
} V4_w22c;

extern s32 cutscene_mode_gp __asm__("D_L01_0015F5C4") __attribute__((sda));
void try_fire_sandmouse_projectile(char *moby, char *state) __asm__("FUN_L01_0030d5f0");

void try_fire_sandmouse_projectile(char *moby, char *state) {
    V4_w22c spawn_position;
    V4_w22c target_aim_position;
    V4_w22c spawn_offset;
    char *target_moby;
    float target_yaw, target_pitch, initial_yaw, initial_pitch, target_bearing_yaw, spawn_to_target_distance;

    if (fast_dec_timer_s16(state + 0x54) == 0)
        return;
    moby_point_position(moby, (state[0x56] & 1) + 1, &spawn_position);
    state[0x56] = state[0x56] ^ 1;
    target_moby = select_sandmouse_projectile_target(moby, &spawn_position, moby + 0x40, sandmouse_projectile_target_yaw_limit_degrees_gp * DEG_TO_RAD,
                         sandmouse_projectile_target_pitch_limit_degrees_gp * DEG_TO_RAD, sandmouse_projectile_target_range_gp, 2.0f, 3.1415927f,
                         3.1415927f);
    if (target_moby != 0) {
        *(char **)(state + 0x60) = target_moby;
        if (hero.motion.pos.f[2] + 1.1f < *(float *)(moby + 0x18)) {
            target_bearing_yaw = fast_arc_tan(*(float *)(target_moby + 0x10) - *(float *)(moby + 0x10),
                             *(float *)(target_moby + 0x14) - *(float *)(moby + 0x14));
            if (fast_difference_between_rotations(*(float *)(moby + 0x48), target_bearing_yaw) < 0.5235988f) {
                if (cutscene_mode_gp != 2) {
                    char *target_aim_height_data;
                    qcopy(&target_aim_position, target_moby + 0x10);
                    target_aim_height_data = suck_get_aim_height(target_moby);
                    if (target_aim_height_data != 0)
                        target_aim_position.f[2] = target_aim_position.f[2] + *(float *)(target_aim_height_data + 0x10);
                    else
                        target_aim_position.f[2] = target_aim_position.f[2] + sandmouse_projectile_fallback_aim_height_gp;
                    fast_vec_sub(&spawn_offset, &target_aim_position, &spawn_position);
                    fast_vec_normalize(&spawn_offset, &spawn_offset, sandmouse_projectile_spawn_offset_distance_gp);
                    fast_vec_add(&spawn_position, &spawn_position, &spawn_offset);
                    target_yaw = fast_arc_tan(target_aim_position.f[0] - spawn_position.f[0], target_aim_position.f[1] - spawn_position.f[1]);
                    spawn_to_target_distance = vector_distance(&spawn_position, &target_aim_position);
                    target_pitch = -fast_arc_tan(spawn_to_target_distance, target_aim_position.f[2] - spawn_position.f[2]);
                    initial_yaw = fast_arc_tan(target_aim_position.f[0] - spawn_position.f[0], target_aim_position.f[1] - spawn_position.f[1]);
                    spawn_to_target_distance = vector_distance(&spawn_position, &target_aim_position);
                    initial_pitch = -fast_arc_tan(spawn_to_target_distance, target_aim_position.f[2] - spawn_position.f[2]);
                    FUN_L01_0030c898_c(moby, &spawn_position, target_yaw, target_pitch, initial_yaw, initial_pitch);
                    allocate_voice_for_target_entry(0, 0, (struct VoiceTarget *)moby);
                    *(short *)(state + 0x54) = scale_game_frames(D_L01_00162080_i);
                    *(int *)(state + 0x5C) = *(int *)(state + 0x5C) + 1;
                }
            }
        }
    } else {
        *(int *)(state + 0x60) = 0;
    }
}
/* pvars of the hovering moby FUN_L01_0030d880 steers */
struct HoverVars {
    Vec4 goal;            /* 0x00: point it flies back to */
    Vec4 vel;             /* 0x10 */
    u8 pad20[0x10];
    Vec4 unk30;           /* 0x30: point it measures from in state 6 */
    u8 pad40[0x10];
    s16 stuck;            /* 0x50: frames it has been blocked and nearly still */
    u8 pad52[0xE];
    struct Moby *target;  /* 0x60: set by FUN_L01_0030d5f0 */
};

/* steering constants */
struct HoverTuning {
    s32 unk0;             /* passed to FUN_L00_00258ad0 */
    f32 unk4, unk8, unkC, unk10;
    f32 yaw_speed;        /* 0x14: turn speed FUN_L00_00258278 keeps (D_L01_001DEB4C) */
    f32 unk18, unk1C, unk20, unk24, unk28, unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
    f32 unk3C, unk40, unk44;
    s32 unk48;
    s32 unk4C;
    struct Moby *owner; /* 0x50 */
};

extern struct HoverTuning D_L01_001DEB38;
extern f32 D_L01_001DEB4C NOT_SDA;
extern f32 sandmouse_target_yaw_spring_strength_degrees_gp __asm__("D_L01_001620C4") __attribute__((sda));
extern f32 sandmouse_target_yaw_damping_degrees_gp __asm__("D_L01_001620C8") __attribute__((sda));
extern f32 sandmouse_target_yaw_turn_limit_degrees_gp __asm__("D_L01_001620CC") __attribute__((sda));
extern f32 D_L01_001620D0 __attribute__((sda));
extern f32 sandmouse_hover_velocity_to_rotation_y_scale_gp __asm__("D_L01_001620D4") __attribute__((sda));
extern f32 sandmouse_hover_rotation_x_blend_factor_gp __asm__("D_L01_001620D8") __attribute__((sda));
extern f32 sandmouse_hover_rotation_y_blend_factor_gp __asm__("D_L01_001620DC") __attribute__((sda));
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void clamp_vector_length(void *, void *, f32) __asm__("FUN_001f9c90");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern void FUN_L00_00258278(struct Moby *, f32 *, f32, f32, f32, f32);
extern s32 move_moby_with_collision_integer_radius(struct Moby *, Vec4 *, f32, f32, s32, s32) __asm__("FUN_L00_00258ad0");
extern f32 normalize_angle(f32) __asm__("FUN_L00_0025e310");

void update_sandmouse_hover_movement(struct Moby *moby, struct HoverVars *hover_vars) __asm__("FUN_L01_0030d880");

void update_sandmouse_hover_movement(struct Moby *moby, struct HoverVars *hover_vars) {
    enum {
        HOVER_COMMAND_FACE_GOAL = 0,
        HOVER_COMMAND_MOVE_TO_GOAL = 2
    };
    Vec4 steering_correction;
    f32 reference_distance;
    f32 k;
    f32 bearing_yaw;
    f32 velocity_magnitude;
    s32 hit;
    s16 stuck_frames;
    struct HoverTuning *hover_tuning;

    fast_vec_sub(&steering_correction, hover_vars, &moby->pos);
    if (moby->state == 6) {
        reference_distance = vector_distance(&moby->pos, &hover_vars->unk30);
    } else {
        reference_distance = vector_distance(&moby->pos, &hero.motion.pos);
    }
    vector_distance(&moby->pos, hover_vars);
    switch (moby->command) {
    case HOVER_COMMAND_FACE_GOAL:
        bearing_yaw = fast_arc_tan(hover_vars->goal.f[0] - moby->pos.x, hover_vars->goal.f[1] - moby->pos.y);
        FUN_L00_00258278(moby, &D_L01_001DEB4C, bearing_yaw, frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f,
                         frame_time * 11.519173f);
        if (1.3f < reference_distance) {
            moby->command = 2;
        }
        break;
    case HOVER_COMMAND_MOVE_TO_GOAL:
        if (hover_vars->target == 0) {
            bearing_yaw = fast_arc_tan(hover_vars->goal.f[0] - moby->pos.x, hover_vars->goal.f[1] - moby->pos.y);
            FUN_L00_00258278(moby, &D_L01_001DEB4C, bearing_yaw, frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f,
                             frame_time * 11.519173f);
        } else {
            bearing_yaw = fast_arc_tan(hover_vars->target->pos.x - moby->pos.x, hover_vars->target->pos.y - moby->pos.y);
            FUN_L00_00258278(moby, &D_L01_001DEB4C, bearing_yaw,
                             sandmouse_target_yaw_spring_strength_degrees_gp * DEG_TO_RAD * frame_time_sq, sandmouse_target_yaw_damping_degrees_gp * DEG_TO_RAD * frame_time_sq,
                             sandmouse_target_yaw_turn_limit_degrees_gp * DEG_TO_RAD * frame_time);
        }
        fast_vec_scale(&steering_correction, &steering_correction, frame_scale * 0.04f);
        fast_vec_sub(&steering_correction, &steering_correction, &hover_vars->vel);
        if (1.3f < reference_distance) {
            fast_vec_scale(&steering_correction, &steering_correction, frame_scale_sq * 0.13f);
        } else {
            fast_vec_scale(&steering_correction, &steering_correction, frame_scale_sq * 0.25f);
        }
        steering_correction.f[2] *= frame_scale * -0.100000024f + 1.0f;
        fast_vec_add(&hover_vars->vel, &hover_vars->vel, &steering_correction);
        if (moby->state == 6) {
            clamp_vector_length(&hover_vars->vel, &hover_vars->vel, frame_time * 2.7f);
        }
        hover_tuning = &D_L01_001DEB38;
        hit = move_moby_with_collision_integer_radius(moby, &hover_vars->vel, 1.0f, hover_tuning->unk30, hover_tuning->unk0, 0);
        velocity_magnitude = fast_vec_length(&hover_vars->vel);
        if (hit == 0) {
            hover_vars->stuck = 0;
        } else if (velocity_magnitude < 0.005f) {
            stuck_frames = ++hover_vars->stuck;
            if (scale_game_frames(20) < stuck_frames && moby->visible == 0) {
                qcopy(&moby->pos, hover_vars);
                hover_vars->stuck = 0;
            }
        }
        if (reference_distance < 1.1f) {
            k = 1.1f - reference_distance;
            bearing_yaw = fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
            hover_vars->vel.f[0] -= k * fast_cos(bearing_yaw) * (frame_scale_sq * 0.1f);
            hover_vars->vel.f[1] -= k * fast_sin(bearing_yaw) * (frame_scale_sq * 0.1f);
        }
        break;
    default:
        moby->command = 0;
        if (1.3f < reference_distance) {
            moby->command = 2;
        }
        break;
    }
    hover_tuning = &D_L01_001DEB38;
    moby->rot.x += (normalize_angle(hover_tuning->yaw_speed * D_L01_001620D0) - moby->rot.x) * sandmouse_hover_rotation_x_blend_factor_gp;
    moby->rot.y += (normalize_angle(fast_vec_length(&hover_vars->vel) * sandmouse_hover_velocity_to_rotation_y_scale_gp) - moby->rot.y) * sandmouse_hover_rotation_y_blend_factor_gp;
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030F178), where it is exact; names translated to the US level program. */


extern char D_L01_001620E0 __attribute__((sda));
extern char D_L01_001620F0 __attribute__((sda));
extern char D_L01_00162100 __attribute__((sda));
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void spawn_type_74_particle(void *, void *, void *, void *, int) __asm__("FUN_L01_0028a7a8");

void emit_sandmouse_particles(char *source_moby) __asm__("FUN_L01_0030dda0");

void emit_sandmouse_particles(char *source_moby) {
    if (*(unsigned char *)(source_moby + 0x31) != 0) {
        float particle_spawn_position[4];
        float particle_velocity[4];
        float z[4];
        *(OvlQuad *)z = 0;
        z[2] = -(random_float_between_alt(1.0f, 2.0f) * frame_time);
        *(OvlQuad *)particle_velocity = *(OvlQuad *)z;
        moby_point_position(source_moby, 0, particle_spawn_position);
        spawn_type_74_particle(source_moby, particle_spawn_position, particle_velocity, &D_L01_001620E0, 1);
        spawn_type_74_particle(source_moby, particle_spawn_position, particle_velocity, &D_L01_001620F0, 1);
        spawn_type_74_particle(source_moby, particle_spawn_position, particle_velocity, &D_L01_00162100, 1);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Initialize both consecutive effect entries. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030F240), where it is exact; names translated to the US level program. */

extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

void draw_sandmouse_billboard_quads(char *moby) __asm__("FUN_L01_0030de68");

void draw_sandmouse_billboard_quads(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *entry = data + 0x70;
    int i;
    for (i = 1; i >= 0; i--) {
        draw_offset_textured_effect_quad(entry, 0x2528AA28, 0.1f, 0.08f);
        entry += 0x10;
    }
}
/* Attach two effect entries and register their update callback. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030F2A8), where it is exact; names translated to the US level program. */

void draw_sandmouse_billboard_quads(char *moby) __asm__("FUN_L01_0030de68");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void enqueue_callback_list_1_alt(void (*)(char *), char *) __asm__("FUN_001f4600");

void update_sandmouse_billboard_quads(char *moby) __asm__("FUN_L01_0030ded0");

void update_sandmouse_billboard_quads(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *entry = data + 0x70;
    int i;
    for (i = 0; i < 2; i++) {
        moby_point_position(moby, i + 3, entry);
        entry += 0x10;
    }
    enqueue_callback_list_1_alt(draw_sandmouse_billboard_quads, moby);
}
#include "qcopy.h"
#include "qzero.h"
/* Ported from rac1-decomp (func_L01_0030F318). */






typedef u32 VecQuad __attribute__((mode(TI), aligned(16)));


typedef struct Moby Moby;

/* Per-level pickup state; collected[] is indexed by the moby's save id. */
typedef struct { u8 pad0[0x454]; u8 collected[1]; } L01LevelState;

typedef struct {
    Vec4 goal;      /* 0x00 */
    Vec4 vel;       /* 0x10 */
    Vec4 a20;       /* 0x20 */
    Vec4 a30;       /* 0x30 */
    s32 radius;     /* 0x40 */
    u8 pad44[4];
    s32 link;       /* 0x48 */
    u8 pad4C[4];
    s16 stuck;      /* 0x50 */
    s16 timer;      /* 0x52 */
    u8 pad54[4];
    s32 k58;
    s32 k5C;
    s32 pad60;
    s32 slot;       /* 0x64 */
} Pv_30F318;

extern char voice_pool_lui[] __asm__("D_0013E550") __attribute__((section(".data")));
extern struct Moby *D_L01_001742D8 __attribute__((section(".data")));
extern L01LevelState D_L01_001BB6B0 __attribute__((section(".data")));
extern struct HoverTuning D_L01_001DEB38_lui __asm__("D_L01_001DEB38") __attribute__((section(".data")));
extern char *D_L01_0015FFD8;
extern char *D_L01_00167280 __attribute__((section(".data")));
extern char D_0013F350[] __attribute__((section(".data")));
extern char D_L01_0020BA10[] __attribute__((section(".data")));
extern char D_L01_0020BA30[] __attribute__((section(".data")));
extern f32 D_L01_001DEB4C __attribute__((section(".data")));
extern f32 fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern float fast_vec_dist(void *, void *);
extern int debug_print() __asm__("FUN_001e93b0");
extern int FUN_L00_00257b90(int, int);
extern int FUN_L00_00259028(void *, void *, void *, void *);
extern int FUN_L01_0023cf98(int, int);
extern int is_point_inside_clip_volume(void *point, int volume) __asm__("func_00214720");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s32 collision_line(void *, void *, s32, void *, s32);
extern s32 rand(void);
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern short D_L01_00162098 __attribute__((sda));
extern short D_L01_0016209C __attribute__((sda));
extern short D_L01_001620A4 __attribute__((sda));
extern short D_L01_001620A8 __attribute__((sda));
extern short D_L01_001620AC __attribute__((sda));
extern short D_L01_001620B0 __attribute__((sda));
extern short D_L01_001620B4 __attribute__((sda));
extern short D_L01_001620B8 __attribute__((sda));
extern short D_L01_001620BC __attribute__((sda));
extern short D_L01_001620C0 __attribute__((sda));
extern unsigned char D_0013D4C5 __attribute__((section(".data")));
extern unsigned char D_0014C190[] __attribute__((section(".data")));
extern void *FUN_L00_00272060(void *, int, f32, f32, int, int, int, void *, f32);
extern void FUN_L00_002323b8(f32, int, int);
extern void FUN_L00_00258278_df40(void *, f32, void *, f32, f32, f32) __asm__("FUN_L00_00258278");
extern void jet_animate(void *, f32) __asm__("FUN_L00_00259fe8");
extern void add_random_offset_to_vector_xyz(void *, f32) __asm__("FUN_L00_0025f8e0");
extern void FUN_L01_0026d930(void *);
extern void compute_sand_mouse_approach_target(void *, void *) __asm__("FUN_L01_002e30b8");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
void compute_sandmouse_follow_goal(void *unused, float *p) __asm__("FUN_L01_0030d260");
void try_fire_sandmouse_projectile(char *moby, char *state) __asm__("FUN_L01_0030d5f0");
void emit_sandmouse_particles(char *m) __asm__("FUN_L01_0030dda0");
void update_sandmouse_billboard_quads(char *moby) __asm__("FUN_L01_0030ded0");
void attach_manipulator(void *owner, s32 slot_index, void *manip) __asm__("FUN_0020cb10");
void detach_manipulator(void *moby, void *manip) __asm__("FUN_0020cb88");

#define LNK(i) (((Moby *)D_L01_0015FFD8) + (i))
#define VOICE(i) (voice_pool_lui + (i) * 0x70)

void sandmouse_update(Moby *m) __asm__("FUN_L01_0030df40");
void sandmouse_update(Moby *sandmouse_moby) {
    enum {
        SANDMOUSE_STATE_INITIALIZE = 0,
        SANDMOUSE_STATE_WAIT_FOR_ACTIVATION = 1,
        SANDMOUSE_STATE_WAIT_FOR_LINK = 2,
        SANDMOUSE_STATE_MOVE_TO_GOAL = 3,
        SANDMOUSE_STATE_BEGIN_FOLLOW_HERO_AND_FIRE = 4,
        SANDMOUSE_STATE_FOLLOW_HERO_AND_FIRE = 5,
        SANDMOUSE_STATE_FADE_OUT = 6,
        SANDMOUSE_STATE_RESET = 7
    };
    Pv_30F318 *sandmouse_vars;
    f32 goal_bearing_yaw;
    int dv;
    s16 timer_frames;
    if (sandmouse_moby == 0) return;
    sandmouse_vars = (Pv_30F318 *)sandmouse_moby->pvars;
    if (sandmouse_vars == 0) return;
    if (sandmouse_vars->link != -1) {
        if (sandmouse_moby->state >= 5) {
            *(f32 *)LNK(sandmouse_vars->link)->pvars = fast_arc_tan(sandmouse_moby->pos.x - LNK(sandmouse_vars->link)->pos.x, sandmouse_moby->pos.y - LNK(sandmouse_vars->link)->pos.y);
        } else {
            char *hero = D_0013F350;
            *(f32 *)LNK(sandmouse_vars->link)->pvars = fast_arc_tan(*(f32 *)(hero + 0x80) - LNK(sandmouse_vars->link)->pos.x, *(f32 *)(hero + 0x84) - LNK(sandmouse_vars->link)->pos.y);
        }
        if (sandmouse_vars->link != -1) {
            LNK(sandmouse_vars->link)->collision = 0;
        }
    }
    if (sandmouse_moby->state >= 3) {
        if (big_head_cheat_enabled != 0) {
            if (*((u8 *)sandmouse_vars + 0x91) == 0) {
                attach_manipulator(sandmouse_moby, 5, (u8 *)sandmouse_vars + 0x90);
            }
            *(f32 *)((u8 *)sandmouse_vars + 0xB0) = 2.1f;
            *(f32 *)((u8 *)sandmouse_vars + 0xB4) = 2.1f;
            *(f32 *)((u8 *)sandmouse_vars + 0xB8) = 2.1f;
        } else if (*((u8 *)sandmouse_vars + 0x91) != 0) {
            detach_manipulator(sandmouse_moby, (u8 *)sandmouse_vars + 0x90);
        }
        update_sandmouse_billboard_quads(sandmouse_moby);
    }
    switch (sandmouse_moby->state) {
    case SANDMOUSE_STATE_INITIALIZE:
        qzero(sandmouse_vars);
        qzero(&sandmouse_vars->vel);
        sandmouse_vars->stuck = 0;
        sandmouse_vars->timer = 0;
        if (sandmouse_moby->prev_seq != 0) {
            blend_moby_animation(sandmouse_moby, 0, 0, scale_game_frames(10));
        }
        sandmouse_moby->update_dist = 0xFF;
        sandmouse_moby->draw_dist = 0xFF;
        if (sandmouse_vars->link != -1) {
            qcopy(&sandmouse_moby->pos, &LNK(sandmouse_vars->link)->pos);
            sandmouse_moby->pos.z += 0.02f;
            if (D_L01_001BB6B0.collected[(short)sandmouse_moby->save_id] != 0
                || (*(int *)(D_0014C190 + (((short)sandmouse_moby->save_id >> 5) * 4 + (current_level_index << 8))) >> (sandmouse_moby->save_id & 0x1F)) & 1) {
                debug_print(D_L01_0020BA10);
                mark_moby_for_removal(sandmouse_moby);
                return;
            }
        } else {
            debug_print(D_L01_0020BA30);
            mark_moby_for_removal(sandmouse_moby);
            return;
        }
        qcopy(&sandmouse_vars->a20, &sandmouse_moby->pos);
        qcopy(&sandmouse_vars->a30, &sandmouse_moby->pos);
        FUN_L01_0026d930(&D_L01_001DEB38_lui);
        D_L01_001DEB38_lui.unk0 = 500;
        D_L01_001DEB38_lui.unk8 = 0.15f;
        D_L01_001DEB38_lui.unkC = 0.15f;
        D_L01_001DEB38_lui.yaw_speed = frame_scale * 0.4f;
        D_L01_001DEB38_lui.unk10 = frame_scale * 0.01f;
        D_L01_001DEB38_lui.unk24 = frame_time * 2.7f;
        D_L01_001DEB38_lui.unk1C = frame_scale * 0.01f;
        D_L01_001DEB38_lui.unk4 = 0.3f;
        D_L01_001DEB38_lui.unk30 = 0.005f;
        D_L01_001DEB38_lui.unk38 |= 8;
        D_L01_001DEB38_lui.unk3C = frame_time_sq * 6.2831855f;
        D_L01_001DEB38_lui.unk40 = frame_time_sq * 3.1415927f;
        D_L01_001DEB38_lui.unk44 = frame_time * 11.519173f;
        D_L01_001DEB38_lui.unk48 = scale_game_frames(4);
        D_L01_001DEB38_lui.unk28 = 3.1415927f;
        sandmouse_moby->state = 1;
        sandmouse_moby->draw_dist = 0;
        break;
    case SANDMOUSE_STATE_WAIT_FOR_ACTIVATION:
        if (D_0013D4C5 != 0) {
            char *hero_bytes;
            {
                char *hero_bytes = D_0013F350;
                if (*(int *)(hero_bytes + 0x1158) != 5) break;
            }
            sandmouse_vars->k5C = 0;
            sandmouse_vars->k58 = scale_game_frames(*(int *)&D_L01_00162098);
            if (sandmouse_moby->visible != 0) {
                if ((sandmouse_moby->anim_flags & 2) && sandmouse_moby->prev_seq != 2) {
                    blend_moby_animation(sandmouse_moby, 2, 0, scale_game_frames(10));
                }
            }
            if (sandmouse_vars->link != -1) {
                if (is_point_inside_clip_volume(D_0013F350 + 0x80, sandmouse_vars->radius)) {
                    LNK(sandmouse_vars->link)->command = 3;
                } else {
                    LNK(sandmouse_vars->link)->command = 1;
                }
            }
            hero_bytes = D_0013F350;
            if (*(int *)(hero_bytes + 0x1130) != 0) {
                Vec4 linked_probe_start;
                Vec4 hero_probe_end;
                f32 angle_to_hero;
                if (*(int *)(hero_bytes + 0x1158) != 5) break;
                angle_to_hero = fast_arc_tan(*(f32 *)(hero_bytes + 0x80) - sandmouse_moby->pos.x, *(f32 *)(hero_bytes + 0x84) - sandmouse_moby->pos.y);
                FUN_L00_00258278_df40(sandmouse_moby, angle_to_hero, &D_L01_001DEB4C,
                    frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
                qcopy(&linked_probe_start, &LNK(sandmouse_vars->link)->pos);
                linked_probe_start.f[2] += 0.5f;
                qcopy(&hero_probe_end, hero_bytes + 0x80);
                hero_probe_end.f[2] += 0.2f;
                if (is_point_inside_clip_volume(hero_bytes + 0x80, sandmouse_vars->radius) == 0) break;
                if (collision_line(&linked_probe_start, &hero_probe_end, 0, sandmouse_moby, 0)) {
                    if (D_L01_001742D8 != *(Moby **)(hero_bytes + 0x2080)) break;
                }
                compute_sand_mouse_approach_target(sandmouse_moby, sandmouse_vars);
                if (*(Moby **)(hero_bytes + 0x1130) != 0 && (*(Moby **)(hero_bytes + 0x1130))->oclass == 0x1B1
                    && (*(Moby **)(hero_bytes + 0x1130))->state != 0xFE && (*(Moby **)(hero_bytes + 0x1130))->state != 0xFD) {
                    (*(Moby **)(hero_bytes + 0x1130))->state = 1;
                }
                D_L01_001DEB38_lui.owner = sandmouse_moby;
                FUN_L01_0023cf98(0x1F, 0);
                FUN_L00_002323b8((float)scale_game_frames(6), 0, 0);
                if (sandmouse_vars->link != -1) {
                    LNK(sandmouse_vars->link)->command = 5;
                    sandmouse_moby->state = 2;
                } else {
                    sandmouse_moby->draw_dist = 0xFF;
                    if (sandmouse_moby->prev_seq != 0) {
                        blend_moby_animation(sandmouse_moby, 0, 0, scale_game_frames(10));
                    }
                    sandmouse_moby->state = 3;
                }
            }
        }
        break;
    case SANDMOUSE_STATE_WAIT_FOR_LINK:
        if (sandmouse_vars->link != -1) {
            if (LNK(sandmouse_vars->link)->state == 5) {
                if (sandmouse_moby->prev_seq != 0) {
                    blend_moby_animation(sandmouse_moby, 0, 0, scale_game_frames(10));
                }
                sandmouse_moby->state = 3;
                sandmouse_vars->timer = 0;
                sandmouse_moby->draw_dist = 0xFF;
            }
        } else {
            sandmouse_vars->timer = 0;
            sandmouse_moby->draw_dist = 0xFF;
            if (sandmouse_moby->prev_seq != 0) {
                blend_moby_animation(sandmouse_moby, 0, 0, scale_game_frames(20));
            }
            if (sandmouse_vars->slot == -1) {
                sandmouse_vars->slot = allocate_voice_for_target_entry(1, 4, (struct VoiceTarget *)sandmouse_moby);
            }
            sandmouse_moby->state = 3;
        }
        break;
    case SANDMOUSE_STATE_MOVE_TO_GOAL: {
        int movement_result_flags;
        {
            char *v = VOICE(sandmouse_vars->slot);
            if (*(Moby **)(v + 0x88) != sandmouse_moby || *(u8 *)(v + 0x74) == 0) {
                sandmouse_vars->slot = -1;
                sandmouse_vars->slot = allocate_voice_for_target_entry(1, 4, (struct VoiceTarget *)sandmouse_moby);
            }
        }
        if (0.6f < vector_distance(&sandmouse_moby->pos, &sandmouse_vars->a30) && sandmouse_vars->timer == 0) {
            D_L01_001DEB38_lui.unk4 = 0.7f;
            compute_sandmouse_follow_goal(sandmouse_moby, sandmouse_vars);
            update_sandmouse_hover_movement(sandmouse_moby, (struct HoverVars *)sandmouse_vars);
            movement_result_flags = sandmouse_vars->stuck < 0x15;
            movement_result_flags = movement_result_flags ? 0 : 2;
        } else {
            compute_sand_mouse_approach_target(sandmouse_moby, sandmouse_vars);
            goal_bearing_yaw = fast_arc_tan(sandmouse_vars->goal.f[0] - sandmouse_moby->pos.x, sandmouse_vars->goal.f[1] - sandmouse_moby->pos.y);
            FUN_L00_00258278_df40(sandmouse_moby, goal_bearing_yaw, &D_L01_001DEB38_lui.yaw_speed,
                frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
            sandmouse_vars->vel.f[2] = 0;
            movement_result_flags = FUN_L00_00259028(sandmouse_moby, &D_L01_001DEB38_lui, sandmouse_vars, &sandmouse_vars->vel);
        }
        if ((movement_result_flags & 6) != 0 || fast_vec_dist(&sandmouse_moby->pos, sandmouse_vars) < 0.25f) {
            if (sandmouse_moby->prev_seq != 2) {
                blend_moby_animation(sandmouse_moby, 2, 0, scale_game_frames(10));
            }
            qzero(&sandmouse_vars->vel);
            sandmouse_moby->state = 4;
            sandmouse_vars->timer = scale_game_frames(0x78);
        }
        if (sandmouse_moby->anim_flags & 2) {
            if (sandmouse_moby->prev_seq != 1) {
                blend_moby_animation(sandmouse_moby, 1, 0, scale_game_frames(FUN_L00_00257b90(7, 12)));
            }
            sandmouse_moby->anim_speed = random_float_between(0.95f, 1.05f);
        }
        break;
    }
    case SANDMOUSE_STATE_BEGIN_FOLLOW_HERO_AND_FIRE:
        {
            char *v = VOICE(sandmouse_vars->slot);
            if (*(Moby **)(v + 0x88) != sandmouse_moby || *(u8 *)(v + 0x74) == 0) {
                sandmouse_vars->slot = -1;
                sandmouse_vars->slot = allocate_voice_for_target_entry(1, 4, (struct VoiceTarget *)sandmouse_moby);
            }
        }
        FUN_L01_0023cf98(0, 1);
        sandmouse_moby->flags &= 0xFFBE;
        if (sandmouse_vars->link != -1) {
            LNK(sandmouse_vars->link)->flags &= 0xFFBE;
        }
        sandmouse_moby->state = 5;
        break;
    case SANDMOUSE_STATE_FOLLOW_HERO_AND_FIRE:
        {
            char *v = VOICE(sandmouse_vars->slot);
            if (*(Moby **)(v + 0x88) != sandmouse_moby || *(u8 *)(v + 0x74) == 0) {
                sandmouse_vars->slot = -1;
                sandmouse_vars->slot = allocate_voice_for_target_entry(1, 4, (struct VoiceTarget *)sandmouse_moby);
            }
        }
        if (sandmouse_vars->timer == scale_game_frames(20) || *(int *)&D_L01_0016209C - 5 < sandmouse_vars->k5C) {
            if (sandmouse_moby->prev_seq != 2) {
                blend_moby_animation(sandmouse_moby, 2, 0, scale_game_frames(5));
            }
        } else if (timer_frames = sandmouse_vars->timer, scale_game_frames(20) < timer_frames && (sandmouse_moby->anim_flags & 2)) {
            if (sandmouse_moby->prev_seq != 1) {
                blend_moby_animation(sandmouse_moby, 1, 0, scale_game_frames(FUN_L00_00257b90(7, 12)));
            }
            sandmouse_moby->anim_speed = random_float_between(0.95f, 1.05f);
        }
        {
            char *hero_bytes = D_0013F350;
            if (*(int *)(hero_bytes + 0x1130) != 0 && *(int *)(hero_bytes + 0x1158) == 5
                && *(int *)(hero_bytes + 0x208C) != 0x16 && *(int *)(hero_bytes + 0x208C) != 0x14
                && *(u8 *)(hero_bytes + 0x20A4) == 0 && *(s16 *)(D_L01_00167280 + 0x86) != 0x14
                && fast_dec_timer(&sandmouse_vars->k58) == 0 && sandmouse_vars->k5C < *(int *)&D_L01_0016209C) {
                compute_sandmouse_follow_goal(sandmouse_moby, sandmouse_vars);
                update_sandmouse_hover_movement(sandmouse_moby, (struct HoverVars *)sandmouse_vars);
            } else {
                qzero(&sandmouse_vars->vel);
                sandmouse_vars->timer = *(int *)&D_L01_001620A4;
                if (sandmouse_vars->slot != -1) {
                    char *v = VOICE(sandmouse_vars->slot);
                    if (*(Moby **)(v + 0x88) == sandmouse_moby && *(u8 *)(v + 0x74) != 0) {
                        release_voice_slot(sandmouse_vars->slot);
                    }
                }
                sandmouse_vars->slot = -1;
                allocate_voice_for_target_entry(2, 0, (struct VoiceTarget *)sandmouse_moby);
                sandmouse_moby->state = 6;
                if (sandmouse_vars->link != -1) {
                    LNK(sandmouse_vars->link)->command = 5;
                }
            }
        }
        try_fire_sandmouse_projectile(sandmouse_moby, sandmouse_vars);
        break;
    case SANDMOUSE_STATE_FADE_OUT:
        if (sandmouse_vars->slot != -1) {
            char *v = VOICE(sandmouse_vars->slot);
            if (*(Moby **)(v + 0x88) == sandmouse_moby && *(u8 *)(v + 0x74) != 0) {
                release_voice_slot(sandmouse_vars->slot);
            }
        }
        sandmouse_vars->slot = -1;
        if (fast_dec_timer_s16(&sandmouse_vars->timer) != 0) {
            sandmouse_moby->alpha = 0x80;
            qcopy(&sandmouse_moby->pos, &sandmouse_vars->a30);
            qcopy(sandmouse_vars, &sandmouse_vars->a30);
            qzero(&sandmouse_vars->vel);
            sandmouse_moby->state = 7;
            sandmouse_moby->flags &= 0xFDFF;
        } else {
            Vec4 pos;
            Vec4 vel = {0};
            int fade_particle_color_low_byte, fade_particle_color_second_byte, fade_particle_color_third_byte, color, life, spin;
            f32 size;
            qcopy(&pos, &sandmouse_moby->pos);
            add_random_offset_to_vector_xyz(&pos, *(f32 *)&D_L01_001620B0);
            fade_particle_color_low_byte = (rand() + 0x30) & 0x3F;
            fade_particle_color_second_byte = (rand() + 0x20) & 0x3F;
            fade_particle_color_third_byte = rand() & 0x2F;
            color = fade_particle_color_low_byte | (fade_particle_color_second_byte << 8) | (fade_particle_color_third_byte << 16) | (*(int *)&D_L01_001620BC << 24);
            spin = rand() % 2;
            if (sandmouse_vars->timer & 1) spin = -spin;
            life = FUN_L00_00257b90(*(int *)&D_L01_001620A8, *(int *)&D_L01_001620AC);
            size = random_float_between(*(f32 *)&D_L01_001620B4, *(f32 *)&D_L01_001620B8);
            FUN_L00_00272060(&pos, life, size, 0.001f, color,
                2, spin, &vel, *(f32 *)&D_L01_001620C0 * frame_time_sq);
            dv = *(int *)&D_L01_001620A4;
            sandmouse_moby->flags |= 0x200;
            sandmouse_moby->alpha -= 0x80 / dv;
            sandmouse_moby->rot.z = fast_add_rotations(sandmouse_moby->rot.z, frame_time * 25.132742f);
        }
        break;
    case SANDMOUSE_STATE_RESET:
        if (sandmouse_vars->slot != -1) {
            char *v = VOICE(sandmouse_vars->slot);
            if (*(Moby **)(v + 0x88) == sandmouse_moby && *(u8 *)(v + 0x74) != 0) {
                release_voice_slot(sandmouse_vars->slot);
            }
        }
        sandmouse_vars->slot = -1;
        qzero(&sandmouse_vars->vel);
        qcopy(sandmouse_vars, &LNK(sandmouse_vars->link)->pos);
        D_L01_001DEB38_lui.owner = 0;
        goal_bearing_yaw = fast_arc_tan(sandmouse_vars->goal.f[0] - sandmouse_moby->pos.x, sandmouse_vars->goal.f[1] - sandmouse_moby->pos.y);
        FUN_L00_00258278_df40(sandmouse_moby, goal_bearing_yaw, &D_L01_001DEB38_lui.yaw_speed,
            frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
        if (sandmouse_moby->prev_seq != 0) {
            blend_moby_animation(sandmouse_moby, 0, 0, scale_game_frames(6));
        }
        LNK(sandmouse_vars->link)->command = 1;
        sandmouse_moby->state = 1;
        sandmouse_moby->draw_dist = 0;
        break;
    case 8:
        break;
    }
    if (sandmouse_moby->state >= 2) {
        jet_animate(sandmouse_moby, 0.3f);
        ((void (*)(void *, void *))emit_sandmouse_particles)(sandmouse_moby, sandmouse_vars);
    }
    if (sandmouse_vars->link != -1) {
        LNK(sandmouse_vars->link)->collision = LNK(sandmouse_vars->link)->pclass->collision;
    }
}
/* Camera blocks FUN_L01_00314e98 resets (camera + 0x70 and its preset row in D_L01_0015EF50) */
typedef struct {
    Vec4 pos;           /* 0x00 */
    u8 pad10[0x30];
    s32 state;          /* 0x40 */
    u8 pad44[4];
    f32 max_angle;      /* 0x48 */
    f32 dist;           /* 0x4C */
    u8 pad50[0x10];
} CameraFocus;

typedef struct {
    Vec4 vel;           /* 0x00 */
    u8 pad10[0x10];
    f32 unk20;          /* 0x20 */
    f32 unk24;          /* 0x24 */
    f32 unk28;          /* 0x28 */
    s16 unk2C;          /* 0x2C */
    u8 pad2E[0x22];
} CameraView;

#include "rnc/gameplay/camera/camera_spring.h"

typedef struct {
    Vec4 vel;           /* 0x00 */
    u8 pad10[0x10];
    Vec4 accel;         /* 0x20 */
    f32 unk30;          /* 0x30 */
    f32 unk34;          /* 0x34 */
    f32 unk38;          /* 0x38 */
    s32 unk3C;          /* 0x3C */
    f32 unk40;          /* 0x40 */
    u8 pad44[0xC];
} CameraTrack;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} CameraBlend;

typedef struct {
    Vec4 axis;          /* 0x00 */
    f32 unk10;          /* 0x10 */
    f32 unk14;          /* 0x14 */
    s32 unk18;          /* 0x18 */
    s16 mode;           /* 0x1C */
    u8 pad1E[2];
    f32 unk20;          /* 0x20 */
    s32 unk24;          /* 0x24 */
    u8 pad28[8];
    CameraView view;    /* 0x30 */
    CameraFocus focus;  /* 0x80 */
    CameraSpring spring; /* 0xE0 */
    CameraTrack track;  /* 0x100 */
    CameraBlend blend;  /* 0x150 */
} CameraOrbit;

typedef struct {
    u8 padA[0xA];
    s16 mode;           /* 0x7E */
} CameraControl;

typedef struct {
    u8 pad0[0x70];
    CameraOrbit *orbit; /* 0x70 */
    CameraControl control; /* 0x74 */
    u8 pad80[4];
    s16 preset;         /* 0x84: row of D_L01_0015EF50 */
} Camera;

/* A camera path: the point count, then 16-byte points */
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 pts[1];        /* 0x10 */
} CameraPath;

typedef struct {
    u8 pad0[0x20];
    s32 track_path;     /* 0x20: D_L01_001B0930 path the first path is measured along */
    s32 target;         /* 0x24: row of D_L01_0015F70C */
    s32 path;           /* 0x28: D_L01_001B0930 path */
    s32 look_path;      /* 0x2C: D_L01_001B0930 path measured along the target's path */
    s32 kind;           /* 0x30 */
    s16 ready;          /* 0x34: paths prepared */
} CameraPreset;

typedef struct {
    u8 pad0[0x1C];
    CameraPreset *preset; /* 0x1C */
} CameraPresetRow;

typedef struct {
    u8 pad0[0x10];
    CameraPath *path;   /* 0x10 */
    u8 pad14[0xC];
} CameraTarget;

extern CameraPresetRow *D_L01_0015EF50 MACRO_ADDR;
extern CameraTarget *D_L01_0015F70C;
extern CameraPath *D_L01_001B0930[];
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 FUN_L00_0025df68(void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);

/* Resets the camera's orbit, focus, spring, view and track from fixed values; for a kind 2 preset it
   also prepares its two paths once, measuring each point against the path it follows. */
void reset_camera_state_and_prepare_preset_paths(Camera *destination_camera) __asm__("FUN_L01_00314e98");

void reset_camera_state_and_prepare_preset_paths(Camera *destination_camera) {
    Vec4 at;
    s32 seg;
    f32 path_fraction_or_length;
    CameraPreset *preset = D_L01_0015EF50[destination_camera->preset].preset;
    CameraControl *control;
    CameraFocus *focus;
    CameraSpring *spring;
    CameraOrbit *orbit;
    CameraView *view;
    CameraTrack *track;
    CameraBlend *blend;
    CameraPath *preset_path, *track_path, *look_path, *target_path;
    s32 i, j, k, segment_count, previous_path_node_index;

    focus = &destination_camera->orbit->focus;
    focus->dist = 1.5f;
    focus->state = 0;
    if (preset->kind == 3) {
        focus->dist = 0.5f;
    }
    control = &destination_camera->control;
    focus->max_angle = 0.20943952f;
    spring = &destination_camera->orbit->spring;
    spring->dist = 6.0f;
    spring->unk4 = 0.5f;
    spring->height = 2.0f;
    orbit = destination_camera->orbit;
    orbit->unk10 = 0.01f;
    orbit->unk14 = 0.175f;
    orbit->unk18 = 0;
    orbit->mode = 0;
    orbit->unk20 = 0.0005f;
    orbit->unk24 = 0;
    clear_u64_value(orbit);
    view = &destination_camera->orbit->view;
    view->unk20 = 0.01f;
    view->unk24 = 0.3f;
    view->unk28 = 0.2f;
    view->unk2C = 0;
    clear_u64_value(view);
    track = &destination_camera->orbit->track;
    clear_u64_value(track);
    clear_u64_value(&track->accel);
    track->unk40 = 3.0f;
    track->unk30 = 0.003f;
    track->unk34 = 0.1f;
    track->unk38 = 0.2f;
    track->unk3C = 0;
    blend = &destination_camera->orbit->blend;
    blend->unk0 = 0;
    blend->unk4 = 0;
    blend->unk8 = 0;
    blend->unkC = 0;
    blend->unk10 = 1;
    if (preset->kind == 2 && preset->ready == 0) {
        track_path = D_L01_001B0930[preset->track_path];
        target_path = D_L01_0015F70C[preset->target].path;
        preset_path = D_L01_001B0930[preset->path];
        look_path = D_L01_001B0930[preset->look_path];
        preset->ready = 1;
        for (i = 0; i < preset_path->count; i++) {
            FUN_L00_0025df68(track_path, &preset_path->pts[i], &at, &seg, &path_fraction_or_length, 0, 20.0f, 5.0f, 0.0f);
            preset_path->pts[i].f[0] = preset_path->pts[i].f[3];
            preset_path->pts[i].f[3] = seg;
        }
        previous_path_node_index = preset_path->count - 1;
        for (i = 0; i < preset_path->count; i++) {
            j = truncate_float_to_s32(preset_path->pts[previous_path_node_index].f[3]);
            segment_count = truncate_float_to_s32(preset_path->pts[i].f[3]) - j;
            if (segment_count < 0) {
                segment_count += track_path->count;
            }
            path_fraction_or_length = 0.0f;
            for (k = 0; k < segment_count; k++) {
                path_fraction_or_length += track_path->pts[(j + i) % track_path->count].f[3];
            }
            preset_path->pts[i].f[1] = path_fraction_or_length;
            previous_path_node_index = i;
        }
        for (i = 0; i < look_path->count; i++) {
            FUN_L00_0025df68(target_path, &look_path->pts[i], &at, &seg, &path_fraction_or_length, 0, 20.0f, 5.0f, 0.0f);
            look_path->pts[i].f[0] = look_path->pts[i].f[3];
            look_path->pts[i].f[3] = seg;
        }
        previous_path_node_index = look_path->count - 1;
        for (i = 0; i < look_path->count; i++) {
            j = truncate_float_to_s32(look_path->pts[previous_path_node_index].f[3]);
            segment_count = truncate_float_to_s32(look_path->pts[i].f[3]) - j;
            if (segment_count < 0) {
                segment_count += target_path->count;
            }
            path_fraction_or_length = 0.0f;
            for (k = 0; k < segment_count; k++) {
                path_fraction_or_length += target_path->pts[(j + i) % target_path->count].f[3];
            }
            look_path->pts[i].f[1] = path_fraction_or_length;
            previous_path_node_index = i;
        }
    }
    control->mode = 0;
}
#include "rnc/math/vector.h"

/* Update function of camera 3: follows a path beside the hero's and turns toward a look-at point on it.
   The four `= 0` initialisers in the mode 2 block are dead and leave no code: they make up for the insns
   qcopy()'s parameters add, which decide the size of gcse's hash table and so the order of the spill slots. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00316730), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) Node_316730;

typedef struct {
    int n;
    int pad[3];
    Node_316730 nodes[1];
} Path_316730;

typedef struct {
    char p00[0x20];
    float unk20;
    char p24[0x10];
    float unk34;
    char p38[0x18];
} SubA_316730;

typedef struct {
    float pos[4];
    char p10[0x3C];
    float unk4C;
    char p50[0x80];
} SubB_316730;

typedef struct {
    int idx;
    float t;
    int p08[2];
    int unk10;
} SubC_316730;

typedef struct {
    float vel[3];
    float unk0C;
    float unk10;
    char p14[0x1C];
    SubA_316730 a;
    SubB_316730 b;
    SubC_316730 c;
} Data_316730;

typedef struct {
    float unk0;
    char p04[0x1C];
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    short unk34;
    short unk36;
    float unk38;
} Parm_316730;

typedef struct {
    char p00[0x1C];
    Parm_316730 *unk1C;
} Ent50_316730;

typedef struct {
    char p00[0x10];
    Path_316730 *unk10;
    char p14[0xC];
} Ent7EC_316730;

typedef struct {
    float unk0[4];
    float unk10[4];
    float unk20[4];
    float pos[4];
    float unk40[4];
    char p50[0x20];
    Data_316730 *data;
    char p74[0x10];
    short unk84;
} Cam_316730;

extern Ent50_316730 *D_L01_0015EF50_c __asm__("D_L01_0015EF50") __attribute__((section(".sdata")));
extern Ent7EC_316730 *D_L01_0015F70C_c __asm__("D_L01_0015F70C") __attribute__((section(".sdata")));
extern f32 approximate_arcsine(f32) __asm__("func_001F9DF8");
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_L01_001672B0[];
extern float rope_spring_turn(void *, float, float, float, float, float) __asm__("FUN_L00_001eb328");
extern float cam_interp_values(float a, float b, float *p, float c, float d, float e);
extern int sample_path(void *, void *, int *, float *, float, int) __asm__("FUN_L00_0025d808");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern int D_0015EE84_c __asm__("D_0015ED84") __attribute__((section(".sdata")));
extern Path_316730 *D_L01_001B0C30_316730[] __asm__("D_L01_001B0930");

void update_camera_position_and_orientation_from_paths(Cam_316730 *cam) __asm__("FUN_L01_00315358");

void update_camera_position_and_orientation_from_paths(Cam_316730 *camera_to_update) {
    float camera_goal[4];
    float look_offset[4];
    float look_dir[4];
    float path_point[4];
    float look_target[4];
    float delta[4];
    float rail_point[4];
    int segment;
    float fraction;
    Data_316730 *data = camera_to_update->data;
    SubA_316730 *look = &data->a;
    SubB_316730 *follow = &data->b;
    SubC_316730 *progress = &data->c;
    Parm_316730 *params = D_L01_0015EF50_c[camera_to_update->unk84].unk1C;
    float rate = 0.01f;
    int mode = params->unk30;
    int path_index = params->unk20;
    float lead = params->unk0;
    float pitch_error;
    float look_length;
    float pitch_step;

    if (mode == 0) {
        Path_316730 *path;
        float dist;

        segment = ((Hero *)(((char *)((char *)&hero))))->rail_segment;
        fraction = ((Hero *)(((char *)((char *)&hero))))->rail_seg_frac;
        progress->t = fraction;
        progress->idx = segment;
        sample_path(D_L01_001B0C30_316730[path_index], camera_goal, &progress->idx, &progress->t, 1.0f, 0);
        qcopy(follow->pos, camera_goal);
        path = D_L01_001B0C30_316730[path_index];
        dist = path->nodes[segment].w * (fraction / ((Path_316730 *)((Hero *)(((char *)((char *)&hero))))->rail_spline)->nodes[segment].w);
        subtract_vector_xyz(delta, &path->nodes[segment + 1], &path->nodes[segment]);
        normalize_vector_xyz(look_target, delta, dist);
        add_vector_xyz(rail_point, &path->nodes[segment], look_target);
        camera_to_update->pos[0] = cam_interp_values(camera_to_update->pos[0], rail_point[0], &data->vel[0], rate, 0.175f, 0.0f);
        camera_to_update->pos[1] = cam_interp_values(camera_to_update->pos[1], rail_point[1], &data->vel[1], rate, 0.175f, 0.0f);
        camera_to_update->pos[2] = cam_interp_values(camera_to_update->pos[2], rail_point[2], &data->vel[2], rate, 0.175f, 0.0f);
    } else if (mode == 2) {
        Path_316730 *pathA = D_L01_001B0C30_316730[path_index];
        Path_316730 *pathD = D_L01_0015F70C_c[params->unk24].unk10;
        Path_316730 *pathC = D_L01_001B0C30_316730[params->unk28];
        Path_316730 *pathB = D_L01_001B0C30_316730[params->unk2C];
        int i;
        int previous_path_node_index = 0;
        int range_start = 0;
        int range_end = 0;
        int j = 0;
        int count;
        int base;
        int node;
        float node_fraction;
        float sum;
        float hero_length;
        float camera_length;
        float speed_scale;
        float dist;

        previous_path_node_index = pathB->n - 1;
        for (i = 0; i < pathB->n; i++) {
            range_start = func_001FA898_r(pathB->nodes[previous_path_node_index].w);
            range_end = func_001FA898_r(pathB->nodes[i].w);
            if (range_end < range_start) {
                if (((Hero *)(((char *)((char *)&hero))))->rail_segment >= range_start || ((Hero *)(((char *)((char *)&hero))))->rail_segment < range_end) {
                    break;
                }
            } else {
                if (((Hero *)(((char *)((char *)&hero))))->rail_segment >= range_start && ((Hero *)(((char *)((char *)&hero))))->rail_segment < range_end) {
                    break;
                }
            }
            previous_path_node_index = i;
        }
        progress->unk10 = i;
        sum = 0.0f;
        j = (i - 1 + pathB->n) % pathB->n;
        hero_length = pathB->nodes[i].y;
        camera_length = pathC->nodes[i].y;
        if (pathB->nodes[j].x != 0.0f) {
            lead = pathB->nodes[j].x;
        }
        base = func_001FA898_r(pathB->nodes[j].w);
        count = ((Hero *)(((char *)((char *)&hero))))->rail_segment - base;
        if (count < 0) {
            count += pathD->n;
        }
        for (i = 0; i < count; i++) {
            sum += pathD->nodes[(base + i) % pathD->n].w;
        }
        sum += ((Hero *)(((char *)((char *)&hero))))->rail_seg_frac;
        dist = camera_length * (sum / hero_length);
        node = func_001FA898_r(pathC->nodes[j].w);
        node_fraction = 0.0f;
        sample_path(pathA, look_target, &node, &node_fraction, dist, 1);
        speed_scale = pathC->nodes[j].x;
        if (speed_scale <= 0.0f) {
            speed_scale = 0.01f;
        } else {
            rate *= speed_scale;
            speed_scale *= 0.01f;
        }
        if (absolute_float(data->unk10 - speed_scale) < 0.00015f) {
            data->unk10 = speed_scale;
        } else if (data->unk10 < speed_scale) {
            data->unk10 = data->unk10 + 0.00015f;
        } else {
            data->unk10 = data->unk10 - 0.00015f;
        }
        camera_to_update->pos[0] = cam_interp_values(camera_to_update->pos[0], look_target[0], &data->vel[0], data->unk10, 0.175f, 0.0f);
        camera_to_update->pos[1] = cam_interp_values(camera_to_update->pos[1], look_target[1], &data->vel[1], data->unk10, 0.175f, 0.0f);
        camera_to_update->pos[2] = cam_interp_values(camera_to_update->pos[2], look_target[2], &data->vel[2], data->unk10, 0.175f, 0.0f);
    } else {
        Path_316730 *path = D_L01_001B0C30_316730[path_index];

        if (params->unk24 < 0) {
            float lead_offset;

            FUN_L00_0025df68(path, ((Hero *)(((char *)((char *)&hero))))->motion.pos.f, path_point, &segment, &fraction, 0, 20.0f, 1.0f, 0.0f);
            if (params->unk36 == 1) {
                lead_offset = -(params->unk38 * 4.0f);
            } else {
                lead_offset = params->unk38;
            }
            sample_path(path, path_point, &segment, &fraction, lead_offset, 1);
            qcopy(camera_goal, path_point);
        } else {
            int hero_segment;
            float hero_fraction;

            hero_segment = ((Hero *)(((char *)((char *)&hero))))->rail_segment;
            hero_fraction = ((Hero *)(((char *)((char *)&hero))))->rail_seg_frac;
            sample_path((Path_316730 *)((Hero *)(((char *)((char *)&hero))))->rail_spline, path_point, &hero_segment, &hero_fraction, params->unk38, 1);
            FUN_L00_0025df68(path, path_point, camera_goal, &segment, &fraction, 0, 20.0f, 1.0f, 0.0f);
        }
        camera_to_update->pos[0] = cam_interp_values(camera_to_update->pos[0], camera_goal[0], &data->vel[0], rate, 0.175f, 0.0f);
        camera_to_update->pos[1] = cam_interp_values(camera_to_update->pos[1], camera_goal[1], &data->vel[1], rate, 0.175f, 0.0f);
        camera_to_update->pos[2] = cam_interp_values(camera_to_update->pos[2], camera_goal[2], &data->vel[2], rate, 0.175f, 0.0f);
    }
    if (params->unk24 < 0) {
        Path_316730 *path = D_L01_001B0C30_316730[params->unk20];

        FUN_L00_0025df68(path, ((Hero *)(((char *)((char *)&hero))))->motion.pos.f, look_target, &segment, &fraction, 0, 20.0f, 1.0f, 0.0f);
        if (D_0015EE84_c == 0xE && params->unk30 == 0) {
            sample_path(path, delta, &segment, &fraction, lead, 0);
        } else {
            sample_path(path, delta, &segment, &fraction, lead, 1);
        }
        subtract_vector_xyz(delta, delta, look_target);
        add_vector_xyz(look_target, ((Hero *)(((char *)((char *)&hero))))->motion.pos.f, delta);
    } else {
        int hero_segment;
        float hero_fraction;

        hero_segment = ((Hero *)(((char *)((char *)&hero))))->rail_segment;
        hero_fraction = ((Hero *)(((char *)((char *)&hero))))->rail_seg_frac;
        sample_path((Path_316730 *)((Hero *)(((char *)((char *)&hero))))->rail_spline, look_target, &hero_segment, &hero_fraction, lead, 1);
    }
    normalize_vector_xyz(look_offset, D_L01_001672B0, follow->unk4C);
    add_vector_xyz(path_point, look_offset, look_target);
    subtract_vector_xyz(look_dir, path_point, camera_to_update->pos);
    look_length = vector_length_xyz(look_dir);
    if (look_length != 0.0f) {
        scale_vector_xyz(look_dir, look_dir, 1.0f / look_length);
        pitch_error = 1.5707964f - approximate_arcsine(dot_vectors_xyz(look_dir, camera_to_update->unk0));
    } else {
        pitch_error = 0.0f;
    }
    if (absolute_float(look->unk20 - rate) < 0.00015f) {
        look->unk20 = rate;
    } else if (look->unk20 < rate) {
        look->unk20 = look->unk20 + 0.00015f;
    } else {
        look->unk20 = look->unk20 - 0.00015f;
    }
    pitch_step = rope_spring_turn(&look->unk34, 0.0f, pitch_error, look->unk20, 0.175f, 0.0f);
    cross_vectors_xyz(delta, look_dir, camera_to_update->unk40);
    build_look_at_matrix(camera_to_update->unk0, camera_to_update->unk40, delta, pitch_step);
    normalize_vector_xyz(camera_to_update->unk40, camera_to_update->unk0, 1.0f);
    normalize_vector_xyz(camera_to_update->unk0, camera_to_update->unk0, 1.0f);
    cross_vectors_xyz(camera_to_update->unk10, camera_to_update->unk0, D_L01_001672B0);
    normalize_vector_xyz(camera_to_update->unk10, camera_to_update->unk10, 1.0f);
    cross_vectors_xyz(camera_to_update->unk20, camera_to_update->unk10, camera_to_update->unk0);
}
extern Camera *D_L01_00167284 __attribute__((section(".data")));
extern OvlVec4 D_0013F3D0_315de0 __asm__("D_0013F3D0");
extern f32 fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern f32 fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");

void initialize_camera_transform_and_paths(Camera *destination_camera) __asm__("FUN_L01_00315de0");

void initialize_camera_transform_and_paths(Camera *destination_camera) {
    CameraPresetRow *rows = D_L01_0015EF50;
    Camera *source_camera = D_L01_00167284;
    CameraPreset *camera_preset = rows[destination_camera->preset].preset;
    OvlVec4 at, delta, to_camera;
    s32 seg;
    f32 path_fraction;

    qcopy((char *)destination_camera + 0x30, (char *)source_camera + 0x30);
    qcopy(destination_camera, source_camera);
    qcopy((char *)destination_camera + 0x10, (char *)source_camera + 0x10);
    qcopy((char *)destination_camera + 0x20, (char *)source_camera + 0x20);
    qcopy((char *)destination_camera + 0x40, destination_camera);

    if (camera_preset->track_path >= 0) {
        CameraPath *path;
        s32 i;
        s32 next;
        path = D_L01_001B0930[camera_preset->track_path];
        for (i = 0; i < path->count; i++) {
            next = (i + 1) % path->count;
            path->pts[i].f[3] = fast_vec_dist(&path->pts[i], &path->pts[next]);
        }
    }

    *(s16 *)((char *)camera_preset + 0x36) = 0;
    if (camera_preset->kind == 3) {
        CameraPath *path;
        s32 next;
        path = D_L01_001B0930[camera_preset->track_path];
        seg = 0;
        path_fraction = 0.0f;
        if (FUN_L00_0025df68(path, &D_0013F3D0_315de0, &at, &seg, &path_fraction,
                             0, 20.0f, 1.0f, 0.0f)) {
            s32 count = path->count;
            next = seg + 1;
            if (next >= count) next = count - 1;
            fast_vec_sub(&delta, &path->pts[next], &path->pts[seg]);
            if (fast_vec_length(&delta) != 0.0f && next != seg) {
                fast_vec_normalize(&delta, &delta, 1.0f);
                fast_vec_sub(&to_camera, (char *)destination_camera + 0x30, &D_0013F3D0_315de0);
                if (0.0f < fast_vec_dot(&to_camera, &delta))
                    *(s16 *)((char *)camera_preset + 0x36) = 1;
            }
        }
    }
    reset_camera_state_and_prepare_preset_paths(destination_camera);
}
#include "sda.h"

/* Camera data reached through D_L01_0015EF50 */
typedef struct {
    u8 pad00[0x24];
    s32 target;    /* 0x24: index into D_L01_0015F70C, -1 none */
    u8 pad28[8];
    s32 kind;      /* 0x30 */
} CamData_316030;

/* One 32-byte entry of D_L01_0015EF50 */
typedef struct {
    u8 pad00[0x1C];
    CamData_316030 *data; /* 0x1C */
} CamSlot_316030;

/* One 32-byte entry of D_L01_0015F70C */
typedef struct {
    u8 pad00[0x10];
    s32 unk10;     /* 0x10: compared with hero.rail_spline */
    u8 pad14[0xC];
} CamTarget_316030;

/* The object FUN_L01_00316030 updates */
typedef struct {
    u8 pad00[0x7E];
    s16 mode;      /* 0x7E: set to 3 or 5 */
    u8 pad80[4];
    s16 slot;      /* 0x84: index into D_L01_0015EF50 */
    s16 unk86;     /* 0x86: compared with hero.unk2284 */
} CamUser_316030;

extern CamSlot_316030 *D_L01_0015EF50_316030 __asm__("D_L01_0015EF50");
extern CamTarget_316030 *D_L01_0015F70C_316030 __asm__("D_L01_0015F70C");

void update_camera_mode_from_preset(CamUser_316030 *camera_user) __asm__("FUN_L01_00316030");

void update_camera_mode_from_preset(CamUser_316030 *camera_user) {
    CamData_316030 *camera_preset = D_L01_0015EF50_316030[camera_user->slot].data;

    if (hero.unk2284 == camera_user->unk86) {
        if (camera_preset->target < 0) {
            return;
        }
        if (hero.rail_spline == D_L01_0015F70C_316030[camera_preset->target].unk10 && hero.rail_off_frames == 0) {
            return;
        }
    }
    if (camera_preset->kind == 3) {
        camera_user->mode = 5;
    } else {
        camera_user->mode = 3;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00318260), where it is exact; names translated to the US level program. */

extern char *find_camera_record_by_id(int) __asm__("FUN_L00_001eb1f8");

extern char *D_L01_00167280_lui __asm__("D_L01_00167280") __attribute__((section(".data")));
void set_camera_5_track_acceleration_yz(float camera_track_accel_y, float camera_track_accel_z) __asm__("FUN_L01_00316e88");

void set_camera_5_track_acceleration_yz(float camera_track_accel_y, float camera_track_accel_z) {
    char *camera_candidate = D_L01_00167280_lui;
    char *selected_camera;
    char *destination;
    if (*(short *)(camera_candidate + 0x86) != 5)
        selected_camera = find_camera_record_by_id(5);
    else
        selected_camera = camera_candidate;
    destination = *(char **)(selected_camera + 0x70) + 0xE0;
    *(float *)(destination + 0x44) = camera_track_accel_y;
    *(float *)(destination + 0x48) = camera_track_accel_z;
}
extern char *D_L00_0015F5F4;
extern char D_0013E550[];
extern char D_0013E5E0[];
extern char D_L01_00167240[];
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern int calculate_spatial_volume_319928(float, float, float, void *) __asm__("FUN_0022c6f8");
extern int is_timer_active_319928(void *) __asm__("FUN_001f9740");
extern int play_voice_319928(int, int, void *, int) __asm__("FUN_L00_0028ddd8");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern int random_integer_below_319928(int) __asm__("FUN_00213260");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int set_voice_volume_319928(int, int) __asm__("FUN_L01_002a1968");

void update_box_positional_voice(char *moby) __asm__("FUN_L01_00319928");

void update_box_positional_voice(char *moby) {
    OvlVec4 delta, radius, box_size, listener;
    int *sound_data = *(int **)(moby + 8);
    int volume;
    int flags;
    float box_length, listener_length;

    radius.f[2] = radius.f[1] = radius.f[0] = *(float *)(moby + 0xc);
    subtract_vector_xyz(&delta, D_L01_00167240, moby + 0x40);
    delta.f[3] = 0.0f;
    transform_vector_by_basis(&box_size, &delta, moby + 0x50);
    transform_vector_by_basis(&listener, &radius, moby + 0x50);
    listener_length = fast_vec_length(&listener);
    box_length = fast_vec_length(&box_size);
    if (box_length < listener_length) {
        flags = 0x10;
        if (absolute_float(box_size.f[0]) <= 1.0f &&
            absolute_float(box_size.f[1]) <= 1.0f &&
            absolute_float(box_size.f[2]) <= 1.0f) {
            volume = *(int *)(D_L00_0015F5F4 + sound_data[0] * 0x20 + 0xc);
        } else {
            if (box_size.f[0] > 1.0f) box_size.f[0] = 1.0f;
            else if (box_size.f[0] < -1.0f) box_size.f[0] = -1.0f;
            if (box_size.f[1] > 1.0f) box_size.f[1] = 1.0f;
            else if (box_size.f[1] < -1.0f) box_size.f[1] = -1.0f;
            if (box_size.f[2] > 1.0f) box_size.f[2] = 1.0f;
            else if (box_size.f[2] < -1.0f) box_size.f[2] = -1.0f;
            volume = calculate_spatial_volume_319928(box_length, fast_vec_length(&box_size),
                                                     listener_length, D_L00_0015F5F4 + sound_data[0] * 0x20);
        }
        if (*(u8 *)(D_L00_0015F5F4 + sound_data[0] * 0x20 + 0x18)) flags = 0x14;
        {
            char *slot = D_0013E550 + sound_data[4] * 0x70;
            if (*(char **)(slot + 0x8c) != moby || *(u8 *)(slot + 0x74) == 0) {
                if (is_timer_active_319928(&sound_data[3])) {
                    sound_data[4] = play_voice_319928(sound_data[0], flags, moby, volume);
                    if (sound_data[2] > 0) {
                        sound_data[3] = (int)((float)scale_game_frames(sound_data[1] +
                            random_integer_below_319928(sound_data[2] - sound_data[1])) * 60.0f);
                    }
                } else {
                    sound_data[4] = -1;
                }
            }
        }
        if (sound_data[4] != -1) {
            set_voice_volume_319928(sound_data[4], volume);
            transform_vector_by_basis(&delta, &box_size, moby + 0x10);
            add_vector_xyz(&delta, &delta, moby + 0x40);
            qcopy(D_0013E5E0 + sound_data[4] * 0x70, &delta);
        }
    } else {
        if (sound_data[4] != -1) {
            char *slot = D_0013E550 + sound_data[4] * 0x70;
            if (*(char **)(slot + 0x8c) == moby && *(u8 *)(slot + 0x74) != 0)
                release_voice_slot(sound_data[4]);
        }
        sound_data[4] = -1;
    }
}
/* reverb box sound update: starts or stops the sound as the listener enters or leaves the box */
/* Ported from rac1-decomp (src/overlays/shared/vendor_0031AD00.c: func_L01_0031B2F0), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void set_requested_state_and_mark_changes(int a, int b, int c, int d, int e) __asm__("FUN_L01_002a1a90");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
float absolute_float(float input) __asm__("func_001F99C0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_requested_state_from_box_position(char *moby) __asm__("FUN_L01_00319f18");

void update_requested_state_from_box_position(char *moby) {
    float hero_offset_from_box[4];
    float hero_offset_in_box_basis[4];
    unsigned char *data = *(unsigned char **)(moby + 8);
    subtract_vector_xyz(hero_offset_from_box, &hero.motion.pos, moby + 0x40);
    hero_offset_from_box[3] = 0;
    transform_vector_by_basis(hero_offset_in_box_basis, hero_offset_from_box, moby + 0x50);
    if (absolute_float(hero_offset_in_box_basis[0]) <= 1.0f && absolute_float(hero_offset_in_box_basis[1]) <= 1.0f && absolute_float(hero_offset_in_box_basis[2]) <= 1.0f) {
        float box_x_fraction = (hero_offset_in_box_basis[0] + 1.0f) * 0.5f;
        int scaled_state_value = func_001FA898_r(convert_integer_to_float(*(int *)(data + 4)) * box_x_fraction);
        int clamped_state_value = *(int *)(data + 4);
        if (!(clamped_state_value < scaled_state_value))
            clamped_state_value = scaled_state_value;
        set_requested_state_and_mark_changes((int)moby, data[0], clamped_state_value, data[1], data[2]);
        data[3] = 1;
    } else if (data[3] != 0) {
        if (hero_offset_in_box_basis[0] > 0.0f) {
            set_requested_state_and_mark_changes(0, data[0], *(int *)(data + 4), data[1], data[2]);
        } else {
            set_requested_state_and_mark_changes((int)moby, 0, 0, 0, 0);
        }
        data[3] = 0;
    }
}
extern char D_0013E5E0[];
extern char D_L01_00167240[];
extern int play_voice(int, int, int, int) __asm__("FUN_L00_0028ddd8");
extern void release_voice_slot(int) __asm__("FUN_0022d798");

void update_listener_following_voice(int sound_owner) __asm__("FUN_L01_0031a078");

void update_listener_following_voice(int sound_owner) {
    char *sound_data = *(char **)(sound_owner + 8);
    int voice_slot_index = *(int *)(sound_data + 4);
    char *voice_slot;
    int flag;
    qcopy(D_0013E5E0 + voice_slot_index * 0x70, D_L01_00167240);
    voice_slot = D_0013E5E0 - 0x90 + voice_slot_index * 0x70;
    flag = *(int *)(voice_slot + 0x8C) == sound_owner && *(unsigned char *)(voice_slot + 0x74) != 0;
    if (*(int *)(D_L01_00167240 + 0x254) != 0) {
        if (!flag) {
            *(int *)(sound_data + 4) = play_voice(*(int *)sound_data, 0x15, sound_owner, 0x400);
        }
    } else {
        if (flag) {
            release_voice_slot(voice_slot_index);
            *(int *)(sound_data + 4) = -1;
        }
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* sound instance update: fade/stop test against a listener distance */
/* Ported from rac1-decomp (src/overlays/shared/vendor_0031AD00.c: func_L01_0031B500), where it is exact; names translated to the US level program. */

extern float absolute_float(float) __asm__("func_001F99C0");
extern void jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern void subtract_vector_xyz(void *, void *, void *);
extern void transform_vector_by_basis(void *, void *, void *);

void update_music_box_exit_trigger(int *moby) __asm__("FUN_L01_0031a128");

void update_music_box_exit_trigger(int *moby) {
    float hero_offset_vectors[8];
    int *music_trigger_data = (int *)moby[2];
    if (((unsigned *)music_trigger_data)[3] != 0xFFFFFFFF) {
        subtract_vector_xyz(hero_offset_vectors, &hero.motion.pos, (char *)moby + 0x40);
        hero_offset_vectors[3] = 0;
        transform_vector_by_basis(hero_offset_vectors + 4, hero_offset_vectors, (char *)moby + 0x50);
        if (absolute_float(hero_offset_vectors[4]) <= 1.0f && absolute_float(hero_offset_vectors[5]) <= 1.0f &&
            absolute_float(hero_offset_vectors[6]) <= 1.0f) {
            music_trigger_data[2] = 1;
            return;
        }
        if (music_trigger_data[2] != 0) {
            short *music_trigger_halfwords = (short *)music_trigger_data;
            if (music_trigger_halfwords[7] == -1)
                music_trigger_halfwords[7] = music_trigger_halfwords[6];
            if (music_trigger_halfwords[6] == -1)
                music_trigger_halfwords[6] = music_trigger_halfwords[7];
            if (hero_offset_vectors[4] > 0) {
                jet_game_state(music_trigger_data[1], music_trigger_halfwords[7]);
            } else {
                jet_game_state(music_trigger_data[0], music_trigger_halfwords[6]);
            }
            music_trigger_data[2] = 0;
        }
    }
}

s32 D_L03_001D2C10[1] __attribute__((section(".data"))) = {0};
f32 D_L03_001D2C14[1] __attribute__((section(".data"))) = {0};
f32 D_L03_001D2C18[1] __attribute__((section(".data"))) = {0};
f32 D_L03_001D2C1C[1] __attribute__((section(".data"))) = {0};
f32 D_L03_001D2C20[1] __attribute__((section(".data"))) = {0};
f32 D_L03_001D2C34[1] __attribute__((section(".data"))) = {0};
s32 D_L03_001D2C48[1] __attribute__((section(".data"))) = {0};
f32 D_L03_001D2C4C[1] __attribute__((section(".data"))) = {0};
f32 D_L03_001D2C50[1] __attribute__((section(".data"))) = {0};
f32 D_L03_001D2C54[1] __attribute__((section(".data"))) = {0};
s32 D_L05_001D2FB0[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001D2FB4[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001D2FB8[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001D2FBC[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001D2FC0[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001D2FD4[1] __attribute__((section(".data"))) = {0};
s32 D_L05_001D2FE8[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001D2FEC[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001D2FF0[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001D2FF4[1] __attribute__((section(".data"))) = {0};
s32 D_L06_001D48C0[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D48C4[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D48C8[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D48CC[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D48D0[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D48E4[1] __attribute__((section(".data"))) = {0};
s32 D_L06_001D48F8[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D48FC[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D4900[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D4904[1] __attribute__((section(".data"))) = {0};
s32 D_L08_001DB300[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001DB304[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001DB308[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001DB30C[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001DB310[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001DB324[1] __attribute__((section(".data"))) = {0};
s32 D_L08_001DB338[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001DB33C[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001DB340[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001DB344[1] __attribute__((section(".data"))) = {0};
f32 D_L11_001D3574[1] __attribute__((section(".data"))) = {0};
s32 D_L11_001D3588[1] __attribute__((section(".data"))) = {0};
f32 D_L11_001D358C[1] __attribute__((section(".data"))) = {0};
f32 D_L11_001D3590[1] __attribute__((section(".data"))) = {0};
f32 D_L11_001D3594[1] __attribute__((section(".data"))) = {0};
s32 D_L12_001F5B60[1] __attribute__((section(".data"))) = {0};
f32 D_L12_001F5B64[1] __attribute__((section(".data"))) = {0};
f32 D_L12_001F5B68[1] __attribute__((section(".data"))) = {0};
f32 D_L12_001F5B6C[1] __attribute__((section(".data"))) = {0};
f32 D_L12_001F5B70[1] __attribute__((section(".data"))) = {0};
f32 D_L12_001F5B84[1] __attribute__((section(".data"))) = {0};
s32 D_L12_001F5B98[1] __attribute__((section(".data"))) = {0};
f32 D_L12_001F5B9C[1] __attribute__((section(".data"))) = {0};
f32 D_L12_001F5BA0[1] __attribute__((section(".data"))) = {0};
f32 D_L12_001F5BA4[1] __attribute__((section(".data"))) = {0};
s32 D_L14_001DFB50[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001DFB54[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001DFB58[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001DFB5C[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001DFB60[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001DFB74[1] __attribute__((section(".data"))) = {0};
s32 D_L14_001DFB88[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001DFB8C[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001DFB90[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001DFB94[1] __attribute__((section(".data"))) = {0};

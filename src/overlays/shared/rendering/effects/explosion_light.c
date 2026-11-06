/* Shared moby class 0x27F: an animated point light (blast flashes), and the function that spawns it. */
#include "types.h"
#include "rnc/overlay/shared/rendering/point_light.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/vector.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"

typedef struct {
    float x, y, z, w;
} Vx __attribute__((aligned(16)));

/* One colour channel of the light, in percent: it rises toward max and falls toward min. */
typedef struct {
    u8 initial;
    u8 max;
    u8 min;
    u8 rise_rate;
    u8 fall_rate;
} ExplosionLightChannel;

/* The template a caller passes to mine_camera_blast; it is copied into the light's vars. */
typedef struct {
    f32                   offset[4]; /* 0x00: position offset, used when no override is passed */
    u8                    intensity; /* 0x10 */
    ExplosionLightChannel channel[3];
    f32                   radius_initial; /* 0x20 */
    f32                   radius_max;
    f32                   radius_min;
    f32                   radius_rise_rate;
    f32                   radius_fall_rate;
    s32                   lifetime; /* 0x34: frames, -1: forever */
    s32                   flags;
    u8                    draw_dist;          /* 0x3C */
    u8                    hold_frames[3];     /* 0x3D: per colour channel */
    u16                   period[4];          /* 0x40: colour channels, then radius */
    u8                    radius_hold_frames; /* 0x48 */
    u8                    start_delay;        /* 0x49: frames before the light starts */
} ExplosionLightSettings;

typedef struct {
    Vec4                  offset;        /* 0x00: w is the minimum random offset scale */
    f32                   anchor_pos[3]; /* 0x10: spawn position, used without attach_moby */
    u8                    start_delay;   /* 0x1C */
    u8                    pad1D[3];
    struct Moby          *attach_moby; /* 0x20: the light follows this moby */
    u8                    intensity;   /* 0x24 */
    ExplosionLightChannel channel[3];
    f32                   radius_initial; /* 0x34 */
    f32                   radius_max;
    f32                   radius_min;
    f32                   radius_rise_rate;
    f32                   radius_fall_rate;
    s32                   lifetime;          /* 0x48: frames, -1: forever */
    s32                   flags;             /* 0x4C */
    s32                   light_slot;        /* 0x50: point light index, -1: none */
    f32                   color[3];          /* 0x54 */
    f32                   radius;            /* 0x60 */
    s32                   attach_moby_index; /* 0x64: -1: none */
    u8                    hold_frames[4];    /* 0x68: colour channels, then radius */
    u16                   period[4];         /* 0x6C */
    s16                   period_timer[4];   /* 0x74 */
    u8                    hold_timer[4];     /* 0x7C */
} ExplosionLightVars;

extern void mark_moby_for_removal() __asm__("FUN_0020c828");

void *create_moby(int) __asm__("FUN_0020c4f8");
void  upd_a(void *) __asm__("FUN_L00_00250df8");
void  FUN_L00_0025d1b8_2d3838(void *) __asm__("FUN_L00_0025d1b8");

extern int FUN_L00_001fefc8(void *);
extern int allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");

unsigned char *
mine_camera_blast(ExplosionLightSettings *, void *, int, void *) __asm__("FUN_L00_002d3838");

unsigned char *mine_camera_blast(ExplosionLightSettings *blast_settings,
                                 void                   *spawn_position,
                                 int                     position_reference_moby,

                                 void *position_offset_override) {
    unsigned char *spawned_moby = create_moby(0x27F);
    if (spawned_moby) {
        ExplosionLightVars *blast_vars;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->flags |= 1;
        MOBY(spawned_moby)->visible = 1;
        blast_vars = (ExplosionLightVars *)MOBY(spawned_moby)->pvars;
        MOBY(spawned_moby)->state = 0;
        MOBY(spawned_moby)->command = 0;
        qcopy(&MOBY(spawned_moby)->pos, spawn_position);
        if (position_offset_override)
            qcopy(&blast_vars->offset, position_offset_override);
        else
            qcopy(&blast_vars->offset, blast_settings->offset);
        blast_vars->attach_moby = (struct Moby *)position_reference_moby;
        MOBY(spawned_moby)->draw_dist = blast_settings->draw_dist;
        blast_vars->intensity = blast_settings->intensity;
        blast_vars->channel[0].initial = blast_settings->channel[0].initial;
        blast_vars->channel[0].max = blast_settings->channel[0].max;
        blast_vars->channel[0].min = blast_settings->channel[0].min;
        blast_vars->channel[0].rise_rate = blast_settings->channel[0].rise_rate;
        blast_vars->channel[0].fall_rate = blast_settings->channel[0].fall_rate;
        blast_vars->hold_frames[0] = blast_settings->hold_frames[0];
        blast_vars->period[0] = blast_settings->period[0];
        blast_vars->channel[1].initial = blast_settings->channel[1].initial;
        blast_vars->channel[1].max = blast_settings->channel[1].max;
        blast_vars->channel[1].min = blast_settings->channel[1].min;
        blast_vars->channel[1].rise_rate = blast_settings->channel[1].rise_rate;
        blast_vars->channel[1].fall_rate = blast_settings->channel[1].fall_rate;
        blast_vars->hold_frames[1] = blast_settings->hold_frames[1];
        blast_vars->period[1] = blast_settings->period[1];
        blast_vars->channel[2].initial = blast_settings->channel[2].initial;
        blast_vars->channel[2].max = blast_settings->channel[2].max;
        blast_vars->channel[2].min = blast_settings->channel[2].min;
        blast_vars->channel[2].rise_rate = blast_settings->channel[2].rise_rate;
        blast_vars->channel[2].fall_rate = blast_settings->channel[2].fall_rate;
        blast_vars->hold_frames[2] = blast_settings->hold_frames[2];
        blast_vars->period[2] = blast_settings->period[2];
        blast_vars->radius_initial = blast_settings->radius_initial;
        blast_vars->radius_max = blast_settings->radius_max;
        blast_vars->radius_min = blast_settings->radius_min;
        blast_vars->radius_rise_rate = blast_settings->radius_rise_rate;
        blast_vars->radius_fall_rate = blast_settings->radius_fall_rate;
        blast_vars->hold_frames[3] = blast_settings->radius_hold_frames;
        blast_vars->period[3] = blast_settings->period[3];
        blast_vars->lifetime = blast_settings->lifetime;
        blast_vars->flags = blast_settings->flags;
        blast_vars->start_delay = blast_settings->start_delay;
        blast_vars->attach_moby_index = -1;
        upd_a(spawned_moby);
        FUN_L00_0025d1b8_2d3838(spawned_moby);
    }
    return spawned_moby;
}

/* Runs its state machine and eases its tracked values toward their targets */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D4EC0), where it is exact; names translated to the US level program. */

void explosion_light_update(unsigned char *moby) __asm__("FUN_L00_002d3a10");

/* Moby class 639 */
void explosion_light_update(unsigned char *moby) {
    enum {
        EXPLOSION_LIGHT_STATE_UNINITIALIZED = 0,
        EXPLOSION_LIGHT_STATE_INITIALIZED = 1
    };
    ExplosionLightVars *animated_point_light_vars;
    Vx                  tp;
    Vx                  base_position;
    Vx                  position_offset;
    unsigned char      *moby_position;
    void               *base_position_ptr;
    void               *position_offset_ptr;
    int                 radius_reset_this_update;
    int                 first_color_randomization_timer_bypass = 0;
    int                 second_color_randomization_timer_bypass = 0;
    int                 third_color_randomization_timer_bypass = 0;
    int                 sphere_check_result_or_light_slot_index;

    if (moby == 0)
        return;
    animated_point_light_vars = (ExplosionLightVars *)MOBY(moby)->pvars;
    radius_reset_this_update = 0;
    if (animated_point_light_vars == 0)
        return;
    if (MOBY(moby)->state == EXPLOSION_LIGHT_STATE_UNINITIALIZED) {
        unsigned char b1c;
        MOBY(moby)->update_dist = 0xFF;
        b1c = animated_point_light_vars->start_delay;
        MOBY(moby)->command = b1c;
        qcopy(animated_point_light_vars->anchor_pos, &MOBY(moby)->pos);
        animated_point_light_vars->light_slot = -1;
        animated_point_light_vars->start_delay = b1c;
        animated_point_light_vars->offset.i[3] = animated_point_light_vars->flags;
        animated_point_light_vars->period_timer[0] = animated_point_light_vars->period[0];
        animated_point_light_vars->period_timer[1] = animated_point_light_vars->period[1];
        animated_point_light_vars->period_timer[2] = animated_point_light_vars->period[2];
        animated_point_light_vars->period_timer[3] = animated_point_light_vars->period[3];
        animated_point_light_vars->color[0] =
            convert_integer_to_float(animated_point_light_vars->channel[0].initial) / 100.0f;
        animated_point_light_vars->color[1] =
            convert_integer_to_float(animated_point_light_vars->channel[1].initial) / 100.0f;
        {
            float initial_color_channel_value =
                convert_integer_to_float(animated_point_light_vars->channel[2].initial) / 100.0f;
            animated_point_light_vars->radius = animated_point_light_vars->radius_initial;
            animated_point_light_vars->color[2] = initial_color_channel_value;
        }
        if (animated_point_light_vars->attach_moby_index != -1) {
            animated_point_light_vars->attach_moby =
                (struct Moby *)(moby_table + (animated_point_light_vars->attach_moby_index << 8));
        }
        if (animated_point_light_vars->radius_rise_rate < 0.0f)
            animated_point_light_vars->radius_rise_rate =
                -animated_point_light_vars->radius_rise_rate;
        if (animated_point_light_vars->radius_fall_rate < 0.0f)
            animated_point_light_vars->radius_fall_rate =
                -animated_point_light_vars->radius_fall_rate;
        MOBY(moby)->state = EXPLOSION_LIGHT_STATE_INITIALIZED;
    }
    if (MOBY(moby)->command != 0) {
        MOBY(moby)->command = MOBY(moby)->command - 1;
        return;
    }
    if (animated_point_light_vars->lifetime != -1 &&
        fast_dec_timer(&animated_point_light_vars->lifetime)) {
        if (animated_point_light_vars->light_slot != -1) {
            release_point_light_slot(animated_point_light_vars->light_slot);
            animated_point_light_vars->light_slot = -1;
        }
        mark_moby_for_removal(moby);
        return;
    }
    {
        unsigned char *position_or_sphere_ptr = &MOBY(moby)->pos;
        qcopy(&tp, position_or_sphere_ptr);
        tp.w = animated_point_light_vars->radius;
        moby_position = position_or_sphere_ptr;
        position_or_sphere_ptr = (unsigned char *)&tp;
        sphere_check_result_or_light_slot_index =
            fast_bsphere_check(position_or_sphere_ptr, 255.0f);
    }
    if (sphere_check_result_or_light_slot_index != -1) {
        if (animated_point_light_vars->attach_moby != 0) {
            qcopy(&base_position, &animated_point_light_vars->attach_moby->pos);
        } else {
            qcopy(&base_position, animated_point_light_vars->anchor_pos);
        }
        base_position_ptr = &base_position;
        if (!(animated_point_light_vars->flags & 0x800000)) {
            if (animated_point_light_vars->flags & 0x100000) {
                if (animated_point_light_vars->flags & 0x200000) {
                    position_offset.x =
                        random_float_between(-animated_point_light_vars->offset.f[0],
                                             animated_point_light_vars->offset.f[0]);
                    position_offset.y =
                        random_float_between(-animated_point_light_vars->offset.f[1],
                                             animated_point_light_vars->offset.f[1]);
                    position_offset.z =
                        random_float_between(-animated_point_light_vars->offset.f[2],
                                             animated_point_light_vars->offset.f[2]);
                    position_offset_ptr = &position_offset;
                } else {
                    int   c = animated_point_light_vars->offset.i[3];
                    void *p = &position_offset;
                    animated_point_light_vars->offset.i[3] = 0;
                    fast_vec_scale(
                        p,
                        &animated_point_light_vars->offset,
                        random_float_between(animated_point_light_vars->offset.f[3], 1.0f));
                    animated_point_light_vars->offset.i[3] = c;
                    position_offset_ptr = p;
                }
            } else {
                int   c = animated_point_light_vars->offset.i[3];
                void *p = &position_offset;
                animated_point_light_vars->offset.i[3] = 0;
                qcopy(p, &animated_point_light_vars->offset);
                animated_point_light_vars->offset.i[3] = c;
                position_offset_ptr = p;
            }
            if (animated_point_light_vars->flags & 0x400000) {
                if (animated_point_light_vars->attach_moby != 0) {
                    transform_vector_by_basis(position_offset_ptr,
                                              position_offset_ptr,
                                              &animated_point_light_vars->attach_moby->rot_row0);
                }
            }
            fast_vec_add(moby_position, base_position_ptr, position_offset_ptr);
        } else {
            qcopy(moby_position, &base_position);
        }
        if (animated_point_light_vars->period[3] != 0) {
            if (fast_dec_timer_s16(&animated_point_light_vars->period_timer[3])) {
                animated_point_light_vars->period_timer[3] =
                    scale_game_frames(animated_point_light_vars->period[3]);
                if (animated_point_light_vars->flags & 4) {
                    radius_reset_this_update = 1;
                    animated_point_light_vars->radius = animated_point_light_vars->radius_min;
                    animated_point_light_vars->flags &= 0xFFFFFFF7;
                }
            }
        }
        if (!(animated_point_light_vars->flags & 0x80000)) {
            s64 q = *(s64 *)&animated_point_light_vars->lifetime;
            if ((q & (0xC000L << 18)) == (0x8000L << 18))
                animated_point_light_vars->flags ^= 0x2;
            if (!(animated_point_light_vars->flags & 0x2)) {
                if (animated_point_light_vars->flags & 0x8) {
                    float t = animated_point_light_vars->radius -
                              animated_point_light_vars->radius_fall_rate * frame_scale;
                    animated_point_light_vars->radius = t;
                    if (t <= animated_point_light_vars->radius_min) {
                        animated_point_light_vars->radius = animated_point_light_vars->radius_min;
                        if (animated_point_light_vars->flags & 0x4) {
                            if (scale_game_frames(animated_point_light_vars->period[3]) == 0)
                                animated_point_light_vars->flags ^= 0x8;
                        }
                    }
                } else {
                    float t = animated_point_light_vars->radius +
                              animated_point_light_vars->radius_rise_rate * frame_scale;
                    animated_point_light_vars->radius = t;
                    if (animated_point_light_vars->radius_max <= t) {
                        animated_point_light_vars->radius = animated_point_light_vars->radius_max;
                        if (!(animated_point_light_vars->flags & 0x1)) {
                            animated_point_light_vars->flags ^= 0x8;
                        } else {
                            animated_point_light_vars->hold_timer[3] =
                                scale_game_frames(animated_point_light_vars->hold_frames[3]);
                            animated_point_light_vars->flags =
                                (animated_point_light_vars->flags | 0x2) ^ 0x8;
                        }
                    }
                }
            } else {
                if (animated_point_light_vars->hold_frames[3] != 0 &&
                    FUN_L00_001fefc8(&animated_point_light_vars->hold_timer[3]))
                    animated_point_light_vars->flags ^= 0x2;
            }
        } else {
            if (animated_point_light_vars->period[3] == 0 || radius_reset_this_update == 1) {
                animated_point_light_vars->radius = random_float_between(
                    animated_point_light_vars->radius_min, animated_point_light_vars->radius_max);
            }
        }
        if (animated_point_light_vars->period[0] != 0) {
            if (fast_dec_timer_s16(&animated_point_light_vars->period_timer[0])) {
                animated_point_light_vars->period_timer[0] =
                    scale_game_frames(animated_point_light_vars->period[0]);
                animated_point_light_vars->flags &= 0xFFFFFF7F;
                animated_point_light_vars->color[0] =
                    (float)animated_point_light_vars->channel[0].min;
            }
        }
        if (!(animated_point_light_vars->flags & 0x10000)) {
            s64 q = *(s64 *)&animated_point_light_vars->lifetime;
            if ((q & (0xC000L << 22)) == (0x8000L << 22))
                animated_point_light_vars->flags ^= 0x20;
            if (!(animated_point_light_vars->flags & 0x20)) {
                if (animated_point_light_vars->flags & 0x80) {
                    float a = convert_integer_to_float(animated_point_light_vars->channel[0].min) /
                              100.0f;
                    float b =
                        convert_integer_to_float(animated_point_light_vars->channel[0].fall_rate) /
                        100.0f;
                    float t = animated_point_light_vars->color[0] - b * frame_scale;
                    animated_point_light_vars->color[0] = t;
                    if (t <= a) {
                        animated_point_light_vars->color[0] = a;
                        if (animated_point_light_vars->flags & 0x40) {
                            if (animated_point_light_vars->period[0] == 0)
                                animated_point_light_vars->flags ^= 0x80;
                        }
                    }
                } else {
                    float a = convert_integer_to_float(animated_point_light_vars->channel[0].max) /
                              100.0f;
                    float b =
                        convert_integer_to_float(animated_point_light_vars->channel[0].rise_rate) /
                        100.0f;
                    float t = animated_point_light_vars->color[0] + b * frame_scale;
                    animated_point_light_vars->color[0] = t;
                    if (a <= t) {
                        animated_point_light_vars->color[0] = a;
                        if (!(animated_point_light_vars->flags & 0x10)) {
                            animated_point_light_vars->flags ^= 0x80;
                        } else {
                            animated_point_light_vars->hold_timer[0] =
                                scale_game_frames(animated_point_light_vars->hold_frames[0]);
                            animated_point_light_vars->flags =
                                (animated_point_light_vars->flags | 0x20) ^ 0x80;
                        }
                    }
                }
            } else {
                if (animated_point_light_vars->hold_frames[0] != 0 &&
                    FUN_L00_001fefc8(&animated_point_light_vars->hold_timer[0]))
                    animated_point_light_vars->flags ^= 0x20;
            }
        } else {
            if (animated_point_light_vars->period[0] == 0 ||
                first_color_randomization_timer_bypass == 1) {
                float a =
                    (float)truncate_float_to_s32((float)animated_point_light_vars->channel[0].min) /
                    100.0f;
                float b =
                    (float)truncate_float_to_s32((float)animated_point_light_vars->channel[0].max) /
                    100.0f;
                animated_point_light_vars->color[0] = random_float_between(a, b);
            }
        }
        if (animated_point_light_vars->period[1] != 0) {
            if (fast_dec_timer_s16(&animated_point_light_vars->period_timer[1])) {
                animated_point_light_vars->period_timer[1] =
                    scale_game_frames(animated_point_light_vars->period[1]);
                animated_point_light_vars->flags &= 0xFFFFF7FF;
                animated_point_light_vars->color[1] =
                    (float)animated_point_light_vars->channel[1].min;
            }
        }
        if (!(animated_point_light_vars->flags & 0x20000)) {
            s64 q = *(s64 *)&animated_point_light_vars->lifetime;
            if ((q & (0xC000L << 26)) == (0x8000L << 26))
                animated_point_light_vars->flags ^= 0x200;
            if (!(animated_point_light_vars->flags & 0x200)) {
                if (animated_point_light_vars->flags & 0x800) {
                    float a = convert_integer_to_float(animated_point_light_vars->channel[1].min) /
                              100.0f;
                    float b =
                        convert_integer_to_float(animated_point_light_vars->channel[1].fall_rate) /
                        100.0f;
                    float t = animated_point_light_vars->color[1] - b * frame_scale;
                    animated_point_light_vars->color[1] = t;
                    if (t <= a) {
                        animated_point_light_vars->color[1] = a;
                        if (animated_point_light_vars->flags & 0x400) {
                            if (animated_point_light_vars->period[1] == 0)
                                animated_point_light_vars->flags ^= 0x800;
                        }
                    }
                } else {
                    float a = convert_integer_to_float(animated_point_light_vars->channel[1].max) /
                              100.0f;
                    float b =
                        convert_integer_to_float(animated_point_light_vars->channel[1].rise_rate) /
                        100.0f;
                    float t = animated_point_light_vars->color[1] + b * frame_scale;
                    animated_point_light_vars->color[1] = t;
                    if (a <= t) {
                        animated_point_light_vars->color[1] = a;
                        if (!(animated_point_light_vars->flags & 0x100)) {
                            animated_point_light_vars->flags ^= 0x800;
                        } else {
                            animated_point_light_vars->hold_timer[1] =
                                scale_game_frames(animated_point_light_vars->hold_frames[1]);
                            animated_point_light_vars->flags =
                                (animated_point_light_vars->flags | 0x200) ^ 0x800;
                        }
                    }
                }
            } else {
                if (animated_point_light_vars->hold_frames[1] != 0 &&
                    FUN_L00_001fefc8(&animated_point_light_vars->hold_timer[1]))
                    animated_point_light_vars->flags ^= 0x200;
            }
        } else {
            if (animated_point_light_vars->period[1] == 0 ||
                second_color_randomization_timer_bypass == 1) {
                float a =
                    (float)truncate_float_to_s32((float)animated_point_light_vars->channel[1].min) /
                    100.0f;
                float b =
                    (float)truncate_float_to_s32((float)animated_point_light_vars->channel[1].max) /
                    100.0f;
                animated_point_light_vars->color[1] = random_float_between(a, b);
            }
        }
        if (animated_point_light_vars->period[2] != 0) {
            if (fast_dec_timer_s16(&animated_point_light_vars->period_timer[2])) {
                animated_point_light_vars->period_timer[2] =
                    scale_game_frames(animated_point_light_vars->period[2]);
                animated_point_light_vars->flags &= 0xFFFF7FFF;
                animated_point_light_vars->color[2] =
                    (float)animated_point_light_vars->channel[2].min;
            }
        }
        if (!(animated_point_light_vars->flags & 0x40000)) {
            s64 q = *(s64 *)&animated_point_light_vars->lifetime;
            if ((q & (0xC000L << 30)) == (0x8000L << 30))
                animated_point_light_vars->flags ^= 0x2000;
            if (!(animated_point_light_vars->flags & 0x2000)) {
                if (animated_point_light_vars->flags & 0x8000) {
                    float a = convert_integer_to_float(animated_point_light_vars->channel[2].min) /
                              100.0f;
                    float b =
                        convert_integer_to_float(animated_point_light_vars->channel[2].fall_rate) /
                        100.0f;
                    float t = animated_point_light_vars->color[2] - b * frame_scale;
                    animated_point_light_vars->color[2] = t;
                    if (t <= a) {
                        animated_point_light_vars->color[2] = a;
                        if (animated_point_light_vars->flags & 0x4000) {
                            if (animated_point_light_vars->period[2] == 0)
                                animated_point_light_vars->flags ^= 0x8000;
                        }
                    }
                } else {
                    float a = convert_integer_to_float(animated_point_light_vars->channel[2].max) /
                              100.0f;
                    float b =
                        convert_integer_to_float(animated_point_light_vars->channel[2].rise_rate) /
                        100.0f;
                    float t = animated_point_light_vars->color[2] + b * frame_scale;
                    animated_point_light_vars->color[2] = t;
                    if (a <= t) {
                        animated_point_light_vars->color[2] = a;
                        if (!(animated_point_light_vars->flags & 0x1000)) {
                            animated_point_light_vars->flags ^= 0x8000;
                        } else {
                            animated_point_light_vars->hold_timer[2] =
                                scale_game_frames(animated_point_light_vars->hold_frames[2]);
                            animated_point_light_vars->flags =
                                (animated_point_light_vars->flags | 0x2000) ^ 0x8000;
                        }
                    }
                }
            } else {
                if (animated_point_light_vars->hold_frames[2] != 0 &&
                    FUN_L00_001fefc8(&animated_point_light_vars->hold_timer[2]))
                    animated_point_light_vars->flags ^= 0x2000;
            }
        } else {
            if (animated_point_light_vars->period[2] == 0 ||
                third_color_randomization_timer_bypass == 1) {
                float a =
                    (float)truncate_float_to_s32((float)animated_point_light_vars->channel[2].min) /
                    100.0f;
                float b =
                    (float)truncate_float_to_s32((float)animated_point_light_vars->channel[2].max) /
                    100.0f;
                animated_point_light_vars->color[2] = random_float_between(a, b);
            }
        }
        sphere_check_result_or_light_slot_index = animated_point_light_vars->light_slot;
        if (sphere_check_result_or_light_slot_index == -1) {
            float kk = convert_integer_to_float(animated_point_light_vars->intensity) / 100.0f;
            animated_point_light_vars->light_slot =
                allocate_colored_effect_slot((float *)moby_position,
                                             animated_point_light_vars->radius,
                                             kk,
                                             animated_point_light_vars->color[0],
                                             animated_point_light_vars->color[1],
                                             animated_point_light_vars->color[2]);
        } else {
            PointLight *point_light = &point_lights[sphere_check_result_or_light_slot_index];
            qcopy(&point_light->position, moby_position);
            point_light->position.f[3] = animated_point_light_vars->radius;
            point_light->color[0] = animated_point_light_vars->color[0];
            point_light->color[1] = animated_point_light_vars->color[1];
            point_light->color[2] = animated_point_light_vars->color[2];
            point_light->intensity =
                convert_integer_to_float(animated_point_light_vars->intensity) / 100.0f;
        }
    } else {
        if (animated_point_light_vars->light_slot != -1) {
            release_point_light_slot(animated_point_light_vars->light_slot);
            animated_point_light_vars->light_slot = sphere_check_result_or_light_slot_index;
        }
    }
}

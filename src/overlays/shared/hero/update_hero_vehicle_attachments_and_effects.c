/* Hero vehicle attachments and effects, collision push-out, timers and the help value lookup. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/hero/hero_idle_sequence.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/overlay/l06/hero/hero_set_state.h"
#include "rnc/overlay/shared/hero/advance_hero_animation.h"
#include "rnc/overlay/shared/ui/map/reveal_map_around_position.h"
#include "rnc/sdk/library/sceVu0Normalize.h"
#include "rnc/overlay/shared/input/pad_pressed_within.h"
#include "rnc/world/collision/collision_line.h"

/* Returns the current help record's value, 0x54 in a special state, or 0 when no record applies. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/help_00202740.c: func_L17_00202740), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x24];
    int  v;
    char pad2[0x24];
} Rec;

extern Rec D_L06_0017A240[];

int get_hero_help_value(int allow_item_help) __asm__("FUN_L06_0021d0a0");

int get_hero_help_value(int allow_item_help) {
    struct Hero *hero_context = &hero;
    switch (hero_context->vehicle_mode) {
    case HERO_VEHICLE_MODE_CLANK:
        return 0;
    case 3:
        return 0;
    }
    {
        struct Hero *hero_help_context = &hero;
        if (hero_help_context->health.hp == 1) {
            return 0x54;
        }
        if (allow_item_help == 0) {
            return 0;
        }
        if (hero_help_context->item_anim_active == 0) {
            return 0;
        }
        if (hero_help_context->unk20AA == 0) {
            return 0;
        }
        if (hero_help_context->unk22C8 != 0) {
            return 0;
        }
    }
    {
        int i = hero_slot_item_id(0);
        return D_L06_0017A240[i].v;
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

extern char D_0013F350_lui[] __asm__("D_0013F350") NOT_SDA;
/* Separate linker alias keeps GP and macro accesses independent in SN. */
extern f32          D_0015ED60_gp __asm__("D_0015ED60_gp") __attribute__((sda));
extern f32          D_0015EE18_macro __asm__("D_0015EE18") MACRO_ADDR;
extern volatile f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern f32          frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern u8           D_0015EDB3_macro __asm__("D_0015EDB3") MACRO_ADDR;
extern s32          D_L06_0015F5CC_macro __asm__("D_L06_0015F5CC") MACRO_ADDR;
extern f32          D_L06_0017BFDC_lui __asm__("D_L06_0017BFDC") NOT_SDA;
extern void         hero_update(void) __asm__("FUN_L00_00211670");
extern void         apply_hero_ground_material_response(void) __asm__("FUN_L06_00222fb8");
extern void         check_ledge_grab(void) __asm__("FUN_L00_0020bed0");
extern void         update_hero_motion_response_parameters(void) __asm__("FUN_L00_00215b68");
extern void         update_hero_state_machine(void) __asm__("FUN_L06_0023b440");
extern void         update_hero_delayed_voice_requests(void) __asm__("FUN_L00_00216f10");
extern void         update_hero_easing_records(void) __asm__("FUN_L00_002054e8");
extern void         update_hero_vehicle_moby(void) __asm__("FUN_L00_00208a10");
extern void         update_hero_scalar_pulse(void) __asm__("FUN_L00_00207c48");
extern void         apply_hero_and_item_scale_sequence(void) __asm__("FUN_L00_00207a58");
extern void         update_hero_glow_and_extra_manipulators(void) __asm__("FUN_L00_002059d8");
extern void         enqueue_hero_glow_draw_callback_once(void) __asm__("FUN_L00_00207d00");
extern void         update_hero_moby_probed_height_bounds(void) __asm__("FUN_L00_00208b60");
extern void         FUN_L00_00257024(int);
extern void         allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
extern s32          fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern void         approach_value(f32, f32, void *) __asm__("FUN_00213ed8");

void update_hero_vehicle_attachments_and_effects(void) __asm__("FUN_L06_0021e1a0");

void update_hero_vehicle_attachments_and_effects(void) {
    enum {
        ATTACHMENT_PULSE_GREEN = 1,
        ATTACHMENT_PULSE_YELLOW = 2,
        ATTACHMENT_PULSE_RED = 3,
        ATTACHMENT_PULSE_BLUE = 4
    };
    char *g;
    char *moby;
    /* The two transform paths reuse the retail 0xA0-byte scratch region. */
    union {
        OvlQuad rotation[10];
        struct {
            OvlQuad reserved;
            OvlQuad transform[4];
            OvlQuad offset;
            OvlQuad inverse[4];
        } help;
    } scratch;
    f32 phase, scale, wave;
    s32 i, span, red, green, blue, alpha;
    s32 color, color2;

    advance_hero_animation();
    hero_update();
    apply_hero_ground_material_response();
    g = (char *)&hero;
    if (*(u8 *)(g + 0x20a4) == 1) {
        check_ledge_grab();
        update_hero_motion_response_parameters();
    }
    update_hero_state_machine();
    if (*(u8 *)(g + 0x20a4) == 0)
        return;
    update_hero_delayed_voice_requests();
    update_hero_easing_records();
    if (*(u8 *)(g + 0x20a4) == 0)
        return;
    update_hero_vehicle_moby();
    update_hero_scalar_pulse();
    apply_hero_and_item_scale_sequence();
    if (*(u8 *)(g + 0x20a4) == 1) {
        D_L06_0017BFDC_lui = D_0015EE18_macro;
        update_hero_glow_and_extra_manipulators();
        if (*(char **)(g + 0x1624) == 0) {
            *(char **)(g + 0x1624) = create_moby(0x4b4);
            if (*(char **)(g + 0x1624)) {
                char *moby = *(char **)(g + 0x1624);
                MOBY(moby)->glow_color = 0x801432d7;
                *(u32 *)(g + 0x2300) = 0x301432d7;
                moby = *(char **)(g + 0x1624);
                qcopy(moby + 0x10, g + 0x80);
                qcopy(moby + 0x40, g + 0x90);
                MOBY(moby)->draw_dist = 0x40;
                (*(char **)(g + 0x1624))[0x31] = 1;
                (*(char **)(g + 0x1624))[0x30] = 0;
                *(u16 *)(*(char **)(g + 0x1624) + 0x34) = 0;
                *(f32 *)(*(char **)(g + 0x1624) + 0x2c) *= 1.7f;
                *(u16 *)(*(char **)(g + 0x1624) + 0x34) |= 0x10;
                update_entity(*(char **)(g + 0x1624));
            }
        }
        {
            char *g = (char *)&hero;
            if ((*(u16 *)(*(char **)(g + 0x2080) + 0x34) ^ 1) & 1) {
                moby_point_position(*(char **)(g + 0x2080), 7, scratch.rotation);
                qcopy(g + 0x1d80, &scratch.rotation[0]);
                enqueue_hero_glow_draw_callback_once();
                if (*(char **)(g + 0x1624) != 0) {
                    build_moby_bone_transform(*(char **)(g + 0x2080), 7, scratch.help.transform);
                    qcopy_nc(*(char **)(g + 0x1624) + 0x10, &scratch.help.transform[3]);
                    copy_matrix3x4(*(char **)(g + 0x1624) + 0xc0, scratch.help.transform);
                    scratch.help.offset = 0;
                    ((f32 *)&scratch.help.offset)[2] = 0.0257f;
                    transform_vector_by_basis(
                        &scratch.help.offset, &scratch.help.offset, *(char **)(g + 0x1624) + 0xc0);
                    *(f32 *)(*(char **)(g + 0x1624) + 0x18) -= ((f32 *)&scratch.help.offset)[2];
                    update_entity(*(char **)(g + 0x1624));
                    scale = 1.7f;
                    if (D_0015EDB3_macro)
                        scale = 3.1f;
                    if (*(s16 *)(g + 0x22fc))
                        scale *= 1.4f;
                    approach_value(*(f32 *)(*(char **)(*(char **)(g + 0x1624) + 0x24) + 0x24) *
                                       scale,
                                   0.02f,
                                   *(char **)(g + 0x1624) + 0x2c);
                }
            }
        }
        {
            char *g = (char *)&hero;
            color = 0x281414c8;
            color2 = 0x1e00004b;
            {
                char *g = (char *)&hero;
                *(f32 *)(g + 0x1628) =
                    fast_add_rotations(*(f32 *)(g + 0x1628), frame_time_macro * 2.9670596f);
            }
            if (fast_dec_timer_s16(g + 0x22fc) == 0) {
                char *g = (char *)&hero;
                *(f32 *)(g + 0x1628) =
                    fast_add_rotations(*(f32 *)(g + 0x1628), frame_time_macro * 12.217304f);
                switch (*(s16 *)(g + 0x22fe)) {
                case ATTACHMENT_PULSE_GREEN:
                    color = 0x2814c814;
                    color2 = 0x1e004b00;
                    break;
                case ATTACHMENT_PULSE_YELLOW:
                    color = 0x2814c8c8;
                    color2 = 0x1e004b4b;
                    break;
                case ATTACHMENT_PULSE_RED:
                    color = 0x323232fa;
                    color2 = 0x1e000078;
                    break;
                case ATTACHMENT_PULSE_BLUE:
                    color = 0x28f01414;
                    color2 = 0x1e4b0000;
                    break;
                }
            }
            {
                char *g = (char *)&hero;
                s32   tint;
                wave = fast_sin(*(f32 *)(g + 0x1628));
                tint = fast_tween_color(wave * 0.5f + 0.5f, color, color2);
                if (*(char **)(g + 0x1624))
                    *(u32 *)(*(char **)(g + 0x1624) + 0x90) = fast_tween_color(
                        D_0015ED60_gp * 0.1f, *(u32 *)(*(char **)(g + 0x1624) + 0x90), tint);
                *(u32 *)(g + 0x2300) =
                    fast_tween_color(frame_scale_macro * 0.1f, *(u32 *)(g + 0x2300), tint);
                if (*(char **)(g + 0x1620) == 0) {
                    *(char **)(g + 0x1620) = create_moby(0x47a);
                    if (*(char **)(g + 0x1620)) {
                        char *moby = *(char **)(g + 0x1620);
                        qcopy(moby + 0x10, g + 0x80);
                        qcopy(moby + 0x40, g + 0x90);
                        MOBY(moby)->draw_dist = 0x40;
                        (*(char **)(g + 0x1620))[0x31] = 1;
                        (*(char **)(g + 0x1620))[0x30] = 0;
                        *(u16 *)(*(char **)(g + 0x1620) + 0x34) = 0;
                        update_entity(*(char **)(g + 0x1620));
                    }
                }
                if (*(char **)(g + 0x1620)) {
                    build_moby_bone_transform(*(char **)(g + 0x2080), 5, scratch.rotation);
                    {
                        OvlQuad *position = &scratch.rotation[3];
                        char    *moby = *(char **)(g + 0x1620);
                        qcopy_nc(moby + 0x10, position);
                        update_entity(moby);
                    }
                    clear_u64_value((scratch.rotation + 4));
                    ((f32 *)(scratch.rotation + 4))[2] = -*(f32 *)(g + 0x98);
                    euler_rotation_matrix(scratch.help.inverse, (scratch.rotation + 4));
                    multiply_matrix(scratch.rotation, scratch.rotation, scratch.help.inverse);
                    copy_matrix3x4(*(char **)(g + 0x1620) + 0xc0, scratch.rotation);
                    *(u64 *)(*(char **)(g + 0x1620) + 0x38) =
                        *(u64 *)(*(char **)(g + 0x2080) + 0x38);
                    if (*(s32 *)(g + 0x2084) == 0x4f) {
                        if (*(u8 *)(*(char **)(g + 0x1620) + 0x53) != 1)
                            blend_moby_animation(
                                *(char **)(g + 0x1620), 1, 0, scale_game_frames(10));
                    } else if (*(u8 *)(*(char **)(g + 0x1620) + 0x53) != 0) {
                        blend_moby_animation(*(char **)(g + 0x1620), 0, 0, scale_game_frames(0x11));
                    }
                }
            }
        }
        {
            char *g = (char *)&hero;
            if (*(char **)(g + 0x22c0)) {
                allocate_voice_for_target_entry(
                    *(char **)(g + 0x22c0) + 0x10, 0, *(char **)(g + 0x2080));
                /* Retail copies the low halfword of this pending pointer. */
                *(s16 *)(g + 0x22fe) = *(u16 *)(g + 0x22c0);
                *(s16 *)(g + 0x22fc) = scale_game_frames(0x39);
            }
            *(u32 *)(g + 0x22c0) = 0;
        }
    }
    reveal_map_around_position(&hero.motion.pos);
    if (hero.vehicle_mode == 3) {
        s32 color;
        /* D_0015EDB1 is the byte two bytes before the checked D_0015EDB3 base. */
        if (*((u8 *)&D_0015EDB3_macro - 2))
            *(f32 *)((char *)&D_L06_0017BFDC_lui + 0x370) = 1.9f;
        {
            char *g = (char *)&hero;
            alpha = 0x80;
            span = scale_game_frames(0xaa);
            if (*(s16 *)(g + 0x22e0) == 2)
                span = scale_game_frames(0x32);
            else if (*(s16 *)(g + 0x22e0) == 1)
                span = scale_game_frames(0x5a);
            phase = convert_integer_to_float(span);
            phase = (f32)(D_L06_0015F5CC_macro % span) / phase;
            wave = fast_sin((phase + phase) * 3.1415927f + -3.1415927f);
        }
        red = truncate_float_to_s32(wave * 20.0f) + 0x32;
        green = truncate_float_to_s32(wave * 70.0f) + 0xb4;
        blue = truncate_float_to_s32(wave * 10.0f) + 0x14;
        {
            char *g = (char *)&hero;
            {
                s32 span = red < 0x100;
                s32 i = green < 0x100;
                green = i ? green : 0xff;
                *(u16 *)(*(char **)(g + 0x2080) + 0x34) |= 0x10;
                red = span ? red : 0xff;
            }
            if (*(s16 *)(g + 0x22e0) == 2)
                color = ((u32)alpha << 24) | ((u32)blue << 16) | ((u32)(red / 2) << 8) | (u32)green;
            else
                color = ((u32)alpha << 24) | ((u32)blue << 16) | ((u32)green << 8) | (u32)red;
        }
        {
            char *g;
            {
                char *g = (char *)&hero;
                *(u32 *)(*(char **)(g + 0x2080) + 0x90) =
                    fast_tween_color(0.1f, *(u32 *)(*(char **)(g + 0x2080) + 0x90), color);
            }
            g = (char *)&hero;
            for (i = 0; i < 3; i++)
                moby_point_position(hero.moby, i, g + 0x1d50 + i * 0x10);
            g = (char *)&hero;
            moby = *(char **)(g + 0x2080);
            /* Retail snapshots these channels before the alpha helper calls. */
            blue = *(u8 *)(moby + 0x92);
            green = *(u8 *)(moby + 0x91);
            red = *(u8 *)(moby + 0x90);
            alpha = 0x30;
            if (*(s16 *)(g + 0x22de))
                alpha = truncate_float_to_s32(convert_integer_to_float(*(s16 *)(g + 0x22de)) /
                                              convert_integer_to_float(0x12) * (f32)alpha);
            *(u32 *)(g + 0x22e4) = (alpha << 24) | ((u32)blue << 16) | ((u32)green << 8) | (u32)red;
            enqueue_hero_glow_draw_callback_once();
            *(s16 *)(g + 0x22e0) = 0;
            if (*(u8 *)(g + 0x20a4) == 3) {
                if (*(s32 *)(g + 0x20b8) != 0 && *(s32 *)(g + 0x20b8) != 0x1f) {
                    if (*(s16 *)(g + 0x22de) == 0) {
                        allocate_hero_voice_for_entry(0x19, 0);
                        *(s16 *)(g + 0x22de) = 0x12;
                        *(s16 *)(g + 0x22dc) = 1;
                    }
                } else {
                    char *g = (char *)&hero;
                    *(s32 *)(g + 0x20b8) = 0;
                    if (*(u8 *)(g + 0x12e7) && *(s32 *)(g + 0x300) &&
                        *(u8 *)(D_0013F350_lui - 0x1e74) && *(s16 *)(g + 0x22de) == 0) {
                        allocate_hero_voice_for_entry(0x19, 0);
                        *(s16 *)(g + 0x22de) = 0x12;
                        *(s16 *)(g + 0x22dc) = 1;
                    }
                }
            }
        }
    }
    update_hero_moby_probed_height_bounds();
    FUN_L00_00257024((s32)hero.moby);
}

extern void reset_hero_easing_records(void) __asm__("FUN_L00_00205538");
extern void clear_hero_queued_animation(void) __asm__("FUN_L00_00232628");

void restore_hero_from_vehicle_mode(void) __asm__("FUN_L06_00227cf8");

void restore_hero_from_vehicle_mode(void) {
    struct Hero   *hero_context = &hero;
    unsigned char *flag_clear_target;
    if (hero_context->vehicle_mode == 1) {
        hero_context->health.vehicle_mode_hp = hero_context->health.hp;
        hero_context->health.hp = hero_context->health.unk22B0;
        if (hero_context->unk1620) {
            mark_moby_for_removal(hero_context->unk1620);
            hero_context->unk1620 = 0;
        }
        if (hero_context->unk1624) {
            mark_moby_for_removal(hero_context->unk1624);
            hero_context->unk1624 = 0;
        }
    }
    {
        struct Hero *hero_restore_context = &hero;
        reset_hero_easing_records();
        hero_restore_context->vehicle_mode = 0;
        flag_clear_target = hero_restore_context->unkA84;
        if (flag_clear_target) {
            *(u16 *)(flag_clear_target + 0x34) &= ~6;
        }
        hero_restore_context->unkA84 = 0;
        if (hero_restore_context->vehicle_mode3_moby) {
            mark_moby_for_removal(hero_restore_context->vehicle_mode3_moby);
            hero_restore_context->vehicle_mode3_moby = 0;
        }
        release_and_clear_hero_voice_slots();
        {
            unsigned char *restored_hero_moby = hero_restore_context->unkA88;
            hero_restore_context->moby = (struct Moby *)restored_hero_moby;
            qcopy(restored_hero_moby + 0x10, &hero_restore_context->motion.pos);
            MOBY(restored_hero_moby)->collision_disable = 0;
        }
        hero_restore_context->anim_frame_rate = 1.0f;
        clear_hero_queued_animation();
        if (hero_restore_context->state.current != 100 ||
            (D_L06_0015F5C4 != 2 && D_L06_0015F5C4 != 6)) {
            hero_set_state(HERO_STATE_IDLE, 1);
        }
    }
}

typedef struct {
    f32 f[4];
} V_2291d0;

typedef struct {
    u8       p0[0x98];
    f32      f98;
    u8       p1[0xE0 - 0x9C];
    V_2291d0 vE0;
    u8       p2[0x240 - 0xF0];
    s32      i240;
    u8       p3[0x308 - 0x244];
    s16      h308;
    u8       p4[0x2080 - 0x30A];
    void    *p2080;
    u8       p5[0x208C - 0x2084];
    s32      i208C;
    u8       p6[0x20A4 - 0x2090];
    u8       b20A4;
} P_2291d0;

typedef struct {
    u8       p0[0x18];
    s32      i18;
    s32      i1C;
    u8       p1[0x40 - 0x20];
    V_2291d0 v40;
} X_2291d0;

extern P_2291d0 D_0013F350_2291d0 __asm__("D_0013F350") NOT_SDA;
extern X_2291d0 D_L06_001745C0_lui __asm__("D_L06_001745C0") NOT_SDA;
extern f32      gravity_length_xy(void *) __asm__("FUN_L00_002339d0");
#define P D_0013F350_2291d0
#define X D_L06_001745C0_lui

extern s32 D_L06_001745DC NOT_SDA;
extern char               D_0013F3D0[] NOT_SDA;
void adjust_hero_velocity_from_collision_probes(int force) __asm__("FUN_L06_002291d0");

void adjust_hero_velocity_from_collision_probes(int force) {
    V_2291d0  primary_vector_origin, primary_vector_endpoint, b, secondary_vector_endpoint;
    char     *vector_origin_base;
    P_2291d0 *g;
    f32       primary_origin_z_offset, k, secondary_vector_scale, primary_probe_result_angle,
        negative_horizontal_dot;
    int r;

    if (P.h308 != 0)
        return;
    if (gravity_length_xy(&P.vE0) < frame_time * 0.1f)
        return;
    primary_origin_z_offset = 0.3700000047683716f;
    k = 0.5f;
    if ((u32)(P.i208C - 0x11) < 2) {
        primary_origin_z_offset = 0.0f;
        k = 0.7f;
    } else if (P.b20A4 == 1) {
        primary_origin_z_offset = 0.35f;
        k = 0.27000001072883606f;
    }
    vector_origin_base = D_0013F3D0;
    qcopy(&primary_vector_origin, vector_origin_base);
    primary_vector_origin.f[2] += primary_origin_z_offset;
    qcopy(&b, vector_origin_base);
    b.f[2] += primary_origin_z_offset * 0.3f;
    if (force || vector_length_xy(vector_origin_base + 0x60) < 0.01f) {
        g = (P_2291d0 *)(vector_origin_base - 0x80);
        secondary_vector_scale = 1.4f;
        primary_vector_endpoint.f[0] = fast_cos(g->f98) * k;
        primary_vector_endpoint.f[1] = fast_sin(g->f98) * k;
        *(s32 *)&primary_vector_endpoint.f[2] = 0;
        secondary_vector_endpoint.f[0] = fast_cos(g->f98) * k * secondary_vector_scale;
        secondary_vector_endpoint.f[1] = fast_sin(g->f98) * k * secondary_vector_scale;
        *(s32 *)&secondary_vector_endpoint.f[2] = 0;
    } else {
        qcopy(&primary_vector_endpoint, vector_origin_base + 0x60);
        *(s32 *)&primary_vector_endpoint.f[2] = 0;
        fast_vec_scale(&primary_vector_endpoint,
                       &primary_vector_endpoint,
                       k / vector_length_xy(&primary_vector_endpoint));
        qcopy(&secondary_vector_endpoint, &primary_vector_endpoint);
        fast_vec_scale(&secondary_vector_endpoint, &secondary_vector_endpoint, 1.4f);
    }
    fast_vec_add(&primary_vector_endpoint, &primary_vector_endpoint, &primary_vector_origin);
    fast_vec_add(&secondary_vector_endpoint, &secondary_vector_endpoint, &b);
    r = -1;
    if (collision_line(&b, &secondary_vector_endpoint, 4, P.p2080, 0) && D_L06_001745DC > 0)
        r = collision_hit_material();
    primary_probe_result_angle = 0.0f;
    if (collision_line(&primary_vector_origin, &primary_vector_endpoint, 4, P.p2080, 0) &&
        X.i1C > 0)
        primary_probe_result_angle = fast_arc_tan(X.v40.f[2], vector_length_xy(&X.v40));
    if (0.8726646304130554f <= primary_probe_result_angle || r == 8 || r == 12) {
        P.i240 = X.i18;
        *(s32 *)&X.v40.f[2] = 0;
        sceVu0Normalize(&X.v40, &X.v40);
        negative_horizontal_dot = -P.vE0.f[0] * X.v40.f[0] - P.vE0.f[1] * X.v40.f[1];
        if (0.0f < negative_horizontal_dot) {
            fast_vec_scale(&X.v40, &X.v40, negative_horizontal_dot);
            fast_vec_add(&P.vE0, &P.vE0, &X.v40);
        }
    }
}

typedef unsigned int u128_22a1d0 __attribute__((mode(TI), aligned(16)));

typedef struct {
    u8          pad0[0x80];
    u128_22a1d0 pos;
    u8          pad90[0x1CC - 0x90];
    s32         w1CC;
    u8          pad1D0[0x1F8 - 0x1D0];
    s16         h1F8;
    u8          pad1FA[6];
    u128_22a1d0 q200;
    u128_22a1d0 q210;
    f32         f220;
    f32         f224;
    u8          pad228[0xC];
    f32         f234;
    u8          pad238[4];
    s32         w23C;
    u8          pad240[0x17];
    u8          b257;
    u8          pad258[0x2080 - 0x258];
    s32         w2080;
    s32         w2084;
    u8          pad2088[4];
    s32         w208C;
    u8          pad2090[0x20B3 - 0x2090];
    u8          b20B3;
    u8          pad20B4[0x22DA - 0x20B4];
    s16         h22DA;
} G_22a1d0;

typedef union {
    u128_22a1d0 q;
    f32         f[4];
} V_22a1d0;

extern OvlVec4 coll_hit_pos __asm__("D_L06_001745F0") NOT_SDA; /* position after push-out of the last collision query */
extern void f2347c0_22a1d0(void) __asm__("FUN_L00_002347c0");
extern void f233810_22a1d0(void *, void *, f32) __asm__("FUN_L00_00233810");
extern void f2334d0_22a1d0(void *, void *, f32) __asm__("FUN_L00_002334d0");
extern s32  coll_sphere(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern s32  rig_probe(void *, s32, f32, s32, f32) __asm__("FUN_L00_001f19a0");
extern s32  coll3_22a1d0(void *, f32) __asm__("FUN_L00_001f3170");
s32         resolve_hero_position_collisions(s32 mode) __asm__("FUN_L06_0022a1d0");

s32 resolve_hero_position_collisions(s32 mode) {
    V_22a1d0 old;
    V_22a1d0 collision_query_offset;
    V_22a1d0 position_displacement;
    s32      mask;
    s32      i;
    OvlVec4 *collision_result_position;
    f32      query_height_or_displacement_length;
    if (hero.collision_off_frames != 0) {
        return 1;
    }
    qcopy(&old, &hero.motion.pos.q);
    clear_u64_value(&collision_query_offset);
    if (hero.path_constrain_pending != 0) {
        f2347c0_22a1d0();
        hero.path_constrain_pending = 0;
    }
    if (hero.state.control_mode != 0x11) {
        if (hero.gravity_mode != 0 || hero.unk1F8 != 0) {
            if (hero.gravity_mode == 1 || hero.unk1F8 != 0) {
                f233810_22a1d0(&collision_query_offset, &collision_query_offset, 0.6f);
            } else {
                f2334d0_22a1d0(
                    &collision_query_offset, &collision_query_offset, -hero.capsule_bottom);
            }
        } else {
            collision_query_offset.f[2] = hero.capsule_bottom;
        }
    }
    fast_vec_add(&hero.motion.pos.q, &hero.motion.pos.q, &collision_query_offset);
    mask = 0xD24;
    if (hero.state.current == HERO_STATE_SINK)
        mask = 0xD24;
    for (i = 0; i < 8; i++) {
        if (hero.gravity_mode != 0) {
            if (!coll_sphere(&hero.motion.pos.q, mask, hero.moby, frame_scale * 0.4f))
                break;
        } else if (hero.state.control_mode == 0xF) {
            if (!coll_sphere(&hero.motion.pos.q, mask, hero.moby, frame_scale * 0.45f))
                break;
        } else {
            s32 collision_query_result;
            query_height_or_displacement_length = hero.capsule_top - hero.capsule_bottom;
            if (query_height_or_displacement_length < 0.05f) {
                query_height_or_displacement_length = 0.05f;
            }
            collision_query_result = rig_probe(&hero.motion.pos.q,
                                               mask,
                                               hero.capsule_radius,
                                               hero.moby,
                                               query_height_or_displacement_length);
            if (!(collision_query_result | coll3_22a1d0(&hero.motion.pos.q, hero.capsule_radius)))
                break;
        }
        collision_result_position = &coll_hit_pos;
        qcopy(&hero.motion.pos.q, collision_result_position);
        qcopy(&hero.wall_contact_normal.q, collision_result_position + 1);
        qcopy(&hero.wall_contact_point.q, collision_result_position - 1);
        hero.wall_hit = 1;
        hero.coll_hit_moby = ((s32 *)collision_result_position)[-6];
    }
    fast_vec_sub(&hero.motion.pos.q, &hero.motion.pos.q, &collision_query_offset);
    fast_vec_sub(&position_displacement, &hero.motion.pos.q, &old);
    query_height_or_displacement_length = fast_vec_length(&position_displacement);
    if (hero.capsule_radius * 1.5f < query_height_or_displacement_length) {
        if (mode == 0xF) {
            if (512.0f < position_displacement.f[0])
                position_displacement.f[0] = 512.0f;
            else if (position_displacement.f[0] < -512.0f)
                position_displacement.f[0] = -512.0f;
            if (512.0f < position_displacement.f[1])
                position_displacement.f[1] = 512.0f;
            else if (position_displacement.f[1] < -512.0f)
                position_displacement.f[1] = -512.0f;
            if (512.0f < position_displacement.f[2])
                position_displacement.f[2] = 512.0f;
            else if (position_displacement.f[2] < -512.0f)
                position_displacement.f[2] = -512.0f;
            fast_vec_normalize(&position_displacement, &position_displacement, hero.capsule_radius);
            fast_vec_add(&hero.motion.pos.q, &old, &position_displacement);
        }
        return -1;
    }
    return 1;
}

#include "eetypes.h"
#include "rnc/input/pad_state.h"

typedef struct {
    s32 w0;
    u8  pad4[0x70 - 4];
} Entry70;

extern u8 D_0013D4E2_lui __asm__("D_0013D4E2") NOT_SDA;

extern void update_hero_tilt(void) __asm__("FUN_L00_002165b8");
extern void restore_hero_from_vehicle_mode(void) __asm__("FUN_L06_00227cf8");
extern void
enter_hero_vehicle_mode_and_set_moby(s32, s32, struct Moby *) __asm__("FUN_L01_00231348");
extern void update_hero_planar_distance_spring_motion(void) __asm__("FUN_L00_00221b68");
extern void transform_hero_local_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_00233660");

extern Entry70 l06_hero_countdown_records[] __asm__("D_L06_0017A070");
void           update_hero_timers_and_spatial_state(void) __asm__("FUN_L06_002348d0");

void update_hero_timers_and_spatial_state(void) {
    Vec4         sum;
    Vec4         history_delta;
    struct Moby *spawned_moby;
    s32          i;
    s32          history_index_base;
    s32          sample_count;

    if (hero.tilt_disabled == 0) {
        update_hero_tilt();
    }
    hero.state_timer++;
    hero.control_mode_timer++;
    hero.step_frames++;
    hero.anim_seq_frames++;
    fast_dec_timer(&hero.move_lock_timer);
    fast_dec_timer(&hero.invulnerable_frames);
    fast_dec_timer_s16(&hero.unk1B2);
    fast_dec_timer(&hero.long_jump_lockout);
    fast_dec_timer_s16(&hero.jump_lockout_timer);
    fast_dec_timer_s16(&hero.unk1DE);
    fast_dec_timer(&hero.wall_jump_window);
    fast_dec_timer_s16(&hero.unk1F0);
    fast_dec_timer_s16(&hero.ledge_probe_cooldown);
    fast_dec_timer_s16(&hero.unk1C8);
    fast_dec_timer_s16(&hero.rail_catch_grace);
    fast_dec_timer_s16(&hero.rail_grab_cooldown);
    fast_dec_timer_s16(&hero.grapple_lockout_timer);
    fast_dec_timer(&hero.collision_off_frames);
    fast_dec_timer(&hero.unk1D0);
    fast_dec_timer(&hero.jump_buffer_lockout);
    fast_dec_timer_s16(&hero.fidget_cooldown);
    fast_dec_timer_s16(&hero.unk1E0);
    fast_dec_timer_s16(&hero.unk1EE);
    fast_dec_timer_s16(&hero.unk1EA);
    fast_dec_timer_s16(&hero.unk1F4);
    fast_dec_timer_s16(&hero.jump_lock_timer);
    fast_dec_timer_s16(&hero.unk1DC);
    fast_dec_timer_s16(&hero.unk1F6);
    fast_dec_timer_s16(&hero.unk1DA);
    fast_dec_timer_s16(&hero.unk1F8);
    if (!(controller_state.held & 5)) {
        hero.unk1EC = 0;
    }
    if (hero.item_anim_active != 0) {
        hero.item_anim_frames++;
    } else {
        hero.item_anim_frames = 0;
    }
    if (controller_state.no_direction != 0) {
        hero.stick_held_frames = 0;
        hero.stick_idle_frames++;
    } else {
        hero.stick_idle_frames = 0;
        hero.stick_held_frames++;
    }
    if (hero.vehicle_toggle_timer != 0 && fast_dec_timer_s16(&hero.vehicle_toggle_timer) != 0 &&
        hero.health.hp != 0 && D_L06_0015F5C4 == 0) {
        if (hero.vehicle_mode == 3) {
            restore_hero_from_vehicle_mode();
            if (pad_pressed_within(0x80, scale_game_frames(0x12), 0)) {
                hero.force_slot0_item8 = 1;
            }
        } else {
            spawned_moby = create_moby(0x27A);
            hero.vehicle_mode3_moby = spawned_moby;
            if (spawned_moby != 0) {
                qcopy(&spawned_moby->pos, &hero.motion.pos);
                qcopy(&spawned_moby->rot, &hero.motion.rot);
                spawned_moby->draw_dist = 0x40;
                ((struct Moby *)hero.vehicle_mode3_moby)->visible = 1;
                ((struct Moby *)hero.vehicle_mode3_moby)->glow_color = 0;
                ((struct Moby *)hero.vehicle_mode3_moby)->spawn_frame = hero.moby->spawn_frame;
                update_entity(hero.vehicle_mode3_moby);
                enter_hero_vehicle_mode_and_set_moby(3, 0x53, hero.vehicle_mode3_moby);
            }
        }
    }
    for (i = 0; i < 5; i++) {
        fast_dec_timer(&l06_hero_countdown_records[i].w0);
    }
    hero.current_path_dist = 9999.0f;
    if (hero.air_frames.s != 0 && gravity_z(&hero.motion.velocity) < 0.0f) {
        if (hero.pos_history_count != 0x20)
            goto skip;
        clear_u64_value(&sum);
        sample_count = 8;
        for (i = 0; i < sample_count; i++) {
            history_index_base = hero.pos_history_index - i;
            {
                s32 history_sample_index = (history_index_base + 0x1F) % 32;
                s32 adjacent_history_sample_index = (history_index_base + 0x1E) % 32;
                fast_vec_sub(&history_delta,
                             &hero.pos_history[history_sample_index],
                             &hero.pos_history[adjacent_history_sample_index]);
            }
            fast_vec_add(&sum, &sum, &history_delta);
        }
        fast_vec_scale(&sum, &sum, 1.0f / (f32)sample_count);
        if (fast_vec_length(&sum) < frame_time) {
            hero.still_frames++;
            goto skip;
        }
    }
    hero.still_frames = 0;
skip:
    update_hero_planar_distance_spring_motion();
    if (hero.vehicle_mode == 1) {
        transform_hero_local_offset_to_world(&hero.motion.body_center, 0.0f, 0.0f, 0.4f);
        hero.reach_distance = 2.125f;
        hero.reach_height = 1.25f;
    } else {
        transform_hero_local_offset_to_world(&hero.motion.body_center, 0.0f, 0.0f, 0.7f);
        if (D_0013D4E2_lui) {
            hero.reach_distance = 12.0f;
            hero.reach_height = 4.5f;
        } else {
            hero.reach_distance = 3.0f;
            hero.reach_height = 1.75f;
        }
    }
    {
        f32 z = hero.ground_z.f;
        if (hero.motion.pos.f[2] - z < 4.0f) {
            qcopy(&hero.motion.near_ground_point, &hero.motion.pos);
            hero.motion.near_ground_point.f[2] = z + 0.5f;
        } else {
            qcopy(&hero.motion.near_ground_point, &hero.motion.body_center);
        }
    }
    if (hero.motion.pos.f[2] < 1.0f) {
        hero_respawn();
    }
}

/* Ported from rac1-decomp (src/overlays/shared/help_0021D6B8.c: func_L06_00239CD0), where it is exact; names translated to the US level program. */

#include "rnc/data_labels.h"

void enter_hero_idle_state_for_current_vehicle_mode(void) __asm__("FUN_L06_00239528");

void enter_hero_idle_state_for_current_vehicle_mode(void) {
    switch (((unsigned char *)&D_0013E533)[0x2EC1]) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        hero_set_state(HERO_STATE_IDLE, 1);
        break;
    case HERO_VEHICLE_MODE_CLANK:
        hero_set_state(0x43, 1);
        break;
    case 3:
        hero_set_state(0x53, 1);
        break;
    }
}

f32 D_L17_0017C15C NOT_SDA = 1.0f;
s32 D_L17_0017475C NOT_SDA = 0;

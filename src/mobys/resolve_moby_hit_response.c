#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/mobys/hit_response_tables.h"

typedef unsigned int u128_25a478 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_25a478 q;
    f32         f[4];
} V_25a478;

typedef struct {
    u8  pad0[0xA6];
    s16 hA6;
} T_25a478;

typedef struct {
    u8 pad0[0x10];
    union {
        V_25a478 pos;
        struct {
            f32 x, y, z, f1C;
        } s;
    } u;
    T_25a478 *tgt;
    u8        pad24[4];
    u8        state;
    u8        sub;
    u16       kind;
    f32       f2C;
} P_25a478;

typedef struct {
    u8  pad0[8];
    u8  b8;
    u8  pad9[0x14 - 9];
    s16 h14;
    s16 h16;
    u8  pad18[4];
    s16 h1C;
    u8  pad1e[0x2E - 0x1E];
    u8  b2E;
} Q_25a478;

typedef struct {
    u8  pad0[0x10];
    f32 x, y;
} O_25a478;

extern u8        D_0013E533[];
extern f32       hit_damage_override_table[2] __asm__("D_0015FFC8") __attribute__((sda));
extern Q_25a478 *get_moby_hit_state(void *);
extern s32       scale_game_frames(s32) __asm__("func_001F96F8");
extern f32       fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32       fast_cos(f32) __asm__("func_001F9DC8");
extern f32       fast_sin(f32) __asm__("func_001F9DE0");

s32 resolve_moby_hit_response(O_25a478 *obj,
                              P_25a478 *p,
                              Q_25a478 *q,
                              u32       flags,
                              s32      *out,
                              f32      *fp,
                              s32      *kp,
                              s32       idx) __asm__("FUN_00213928");

s32 resolve_moby_hit_response(O_25a478 *hit_recipient_moby,
                              P_25a478 *hit_record,
                              Q_25a478 *q,
                              u32       flags,
                              s32      *hit_status_output,
                              f32      *fp,
                              s32      *kp,

                              s32 hit_vector_scale_row) {
    enum {
        KIND_GATED_HIT_RESPONSE = 0x0,
        DEFAULT_HIT_RESPONSE = 0x1,
        AUXILIARY_FLAG_SETTING_HIT_RESPONSE = 0x4,
        CONDITIONAL_DURATION_HIT_RESPONSE = 0x7,
        DAMAGE_PRESERVING_COOLDOWN_RESPONSE = 0xB
    };
    enum {
        KIND_GATED_HIT_STATE = 0,
        SUBSTATE_GATED_DAMAGE_PRESERVING_HIT_STATE = 1,
        SUBSTATE_GATED_HIT_STATE = 2,
        COOLDOWN_RESETTING_HIT_STATE = 3,
        FIXED_RESPONSE_HIT_STATE = 4,
        SUBSTATE_GATED_AUXILIARY_FLAG_HIT_STATE = 5,
        SUBSTATE_GATED_COOLDOWN_DAMAGE_CLEARING_HIT_STATE = 7
    };
    V_25a478  hit_blend_vector;
    V_25a478  combined_hit_vector;
    s32       hit_response_index;
    s32       kind;
    s32       hit_response_code;
    Q_25a478 *object_state;

    *hit_status_output = 0;
    if (kp)
        *kp = -1;
    if (hit_record && hit_record->tgt &&
        (hit_record->tgt->hA6 == 0x131 || hit_record->tgt->hA6 == 0xA8) && q) {
        hit_record->f2C = hit_damage_override_table[q->b8];
    }
    if (!hit_recipient_moby) {
        if (q) {
            q->h1C = 0;
            q->h14 = -1;
        }
        q->h16 = -1;
        *hit_status_output = 0;
        return 0;
    }
    if (!q) {
        q = get_moby_target_info(hit_recipient_moby);
        if (!q) {
            *hit_status_output = 0;
            return 0;
        }
    }
    if (hit_vector_scale_row == 4)
        hit_vector_scale_row = q->b8;
    if (!hit_record) {
        if (q) {
            fast_dec_timer_s16(&q->h1C);
            q->h16 = -1;
        }
        *hit_status_output = 1;
        if (fp)
            *fp = 0;
        return 0xB;
    }
    if (hit_record->u.s.f1C == 5627.9248f) {
        V_25a478 *hit_vector = &hit_record->u.pos;
        fast_vec_scale(hit_vector,
                       hit_vector,
                       hit_vector_scale_table[hit_vector_scale_row][hit_record->state]);
        kind = hit_record->kind;
        hit_record->u.s.f1C = 5627.9248f;
        if (kind == 0x47) {
            hit_blend_vector.f[0] =
                fast_cos(fast_arc_tan(hit_recipient_moby->x - hero.motion.pos.f[0],
                                      hit_recipient_moby->y - hero.motion.pos.f[1]));
            hit_blend_vector.f[1] =
                fast_sin(fast_arc_tan(hit_recipient_moby->x - hero.motion.pos.f[0],
                                      hit_recipient_moby->y - hero.motion.pos.f[1]));
            hit_blend_vector.f[2] = 1.0f;
            fast_vec_normalize(
                &hit_blend_vector, &hit_blend_vector, fast_vec_length(hit_vector) * 0.7f);
            fast_vec_scale(hit_vector, hit_vector, 0.3f);
            fast_vec_add(&combined_hit_vector, hit_vector, &hit_blend_vector);
            hit_vector->q = combined_hit_vector.q;
            set_vector_length_xy(hit_vector, hit_vector, 1.0f);
        }
    } else {
        if (!hit_record->tgt) {
            hit_record->state = 8;
            kind = 0x47;
        } else {
            kind = hit_record->tgt->hA6;
            hit_record->state = 8;
        }
    }
    switch (hit_record->state) {
    case KIND_GATED_HIT_STATE:
        if (hit_record->kind == 1 || hit_record->kind == 2) {
            *hit_status_output = 2;
            hit_response_index = 0;
            break;
        }
        goto dflt;
    case SUBSTATE_GATED_DAMAGE_PRESERVING_HIT_STATE:
        if (hit_record->sub < 2) {
            *hit_status_output = 2;
            hit_response_index = 0xB;
            break;
        }
        if (hit_record->sub == 2) {
            *hit_status_output = 2;
            hit_response_index = 5;
            break;
        }
        *hit_status_output = 2;
        hit_response_index = 9;
        break;
    case SUBSTATE_GATED_HIT_STATE:
        if (hit_record->sub < 2) {
            *hit_status_output = 2;
            hit_response_index = 8;
            break;
        }
        *hit_status_output = 2;
        hit_response_index = 0xD;
        break;
    case COOLDOWN_RESETTING_HIT_STATE:
        if (hit_record->sub < 2) {
            *hit_status_output = 2;
            hit_response_index = 0xA;
            break;
        }
        if (hit_record->sub < 3) {
            *hit_status_output = 2;
            hit_response_index = 0xF;
            break;
        }
        *hit_status_output = 2;
        hit_response_index = 0x10;
        q->h14 = -1;
        q->h1C = 0;
        break;
    case FIXED_RESPONSE_HIT_STATE:
        *hit_status_output = 2;
        hit_response_index = 0xE;
        break;
    case SUBSTATE_GATED_AUXILIARY_FLAG_HIT_STATE:
        if (hit_record->sub < 2) {
            *hit_status_output = 3;
            hit_response_index = 4;
            break;
        }
        *hit_status_output = 4;
        hit_response_index = 7;
        break;
    case SUBSTATE_GATED_COOLDOWN_DAMAGE_CLEARING_HIT_STATE:
        if (hit_record->sub < 2) {
            *hit_status_output = 2;
            hit_response_index = 0xC;
            break;
        }
        *hit_status_output = 2;
        hit_response_index = 0x11;
        break;
    default:
    dflt:
        *hit_status_output = 2;
        hit_response_index = 1;
        break;
    }
    if (kp)
        *kp = kind;
    hit_response_code = hit_response_code_table[hit_response_index][q->b8];
    if (q->h14 != kind)
        q->h1C = 0;
    if (fast_dec_timer_s16(&q->h1C)) {
        switch (hit_response_index) {
        case KIND_GATED_HIT_RESPONSE:
            q->h1C = scale_game_frames(0x25);
            break;
        case DEFAULT_HIT_RESPONSE:
            q->h1C = scale_game_frames(0xF);
            break;
        case AUXILIARY_FLAG_SETTING_HIT_RESPONSE:
            q->h1C = scale_game_frames(0x3C);
            break;
        case CONDITIONAL_DURATION_HIT_RESPONSE:
            if (D_0013E533[0])
                q->h1C = scale_game_frames(0x2D);
            else
                q->h1C = scale_game_frames(0x3C);
            break;
        case DAMAGE_PRESERVING_COOLDOWN_RESPONSE:
            q->h1C = scale_game_frames(0x3C);
            break;
        default:
            q->h1C = scale_game_frames(0x1E);
            break;
        }
    } else {
        switch (hit_response_index) {
        case KIND_GATED_HIT_RESPONSE:
        case DEFAULT_HIT_RESPONSE:
            if (!(flags & 0x10))
                hit_response_code = 0xB;
            if (!(flags & 0x20))
                *hit_status_output = 1;
            goto z;
        case AUXILIARY_FLAG_SETTING_HIT_RESPONSE:
            if (!(flags & 1))
                hit_response_code = 0xB;
            if (!(flags & 2))
                *hit_status_output = 1;
            goto z;
        case CONDITIONAL_DURATION_HIT_RESPONSE:
            if (!(flags & 4))
                hit_response_code = 0xB;
            if (!(flags & 8))
                *hit_status_output = 1;
        z:
            hit_record->f2C = 0;
            break;
        case DAMAGE_PRESERVING_COOLDOWN_RESPONSE:
            hit_response_code = 0xC;
            break;
        default:
            hit_record->f2C = 0;
            hit_response_code = 0xD;
            break;
        }
    }
    q->h14 = kind;
    q->h16 = kind;
    if (fp)
        *fp = hit_record->f2C;
    if (hit_response_index == 4) {
        object_state = get_moby_hit_state(hit_recipient_moby);
        if (object_state)
            object_state->b2E = 1;
    } else {
        Q_25a478 *object_state_for_flag_clear = get_moby_hit_state(hit_recipient_moby);
        if (object_state_for_flag_clear)
            object_state_for_flag_clear->b2E = 0;
    }
    return hit_response_code;
}

extern __typeof__(resolve_moby_hit_response) func_00213928 __attribute__((alias("FUN_00213928")));

HitResponseCode hit_response_code_table[18][4] = {
    {1, 3, 7,   9},
    {1, 3, 7,   9},
    {1, 3, 7,   9},
    {1, 3, 7,   9},
    {1, 4, 8,   9},
    {1, 5, 3,   9},
    {1, 1, 5,   9},
    {1, 1, 4,   9},
    {1, 1, 5,   8},
    {1, 1, 5,   8},
    {1, 1, 1,   5},
    {2, 6, 6, 0xa},
    {1, 1, 1,   5},
    {1, 1, 1,   5},
    {1, 1, 1,   5},
    {1, 1, 1,   1},
    {1, 1, 1,   1},
    {1, 1, 1,   1},
};

f32 hit_vector_scale_table[4][8] = {
    {1.0f, 0.95f, 1.2f, 1.4f, 1.4f, 1.15f, 1.0f,  2.0f},
    {1.0f, 0.85f, 1.1f, 1.2f, 1.2f,  1.0f, 1.0f,  1.5f},
    {1.0f, 0.75f, 1.0f, 1.0f, 1.1f, 0.85f, 1.0f, 1.25f},
    {1.0f, 0.65f, 0.9f, 1.0f, 1.0f, 0.75f, 1.0f,  1.0f},
};

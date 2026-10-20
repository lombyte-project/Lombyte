#include "types.h"
#include "rnc/gameplay/hero.h"
typedef unsigned int u128_25a478 __attribute__((mode(TI), aligned(16)));
typedef union { u128_25a478 q; f32 f[4]; } V_25a478;
typedef struct { u8 pad0[0xA6]; s16 hA6; } T_25a478;
typedef struct {
    u8 pad0[0x10];
    union { V_25a478 pos; struct { f32 x, y, z, f1C; } s; } u;
    T_25a478 *tgt;
    u8 pad24[4];
    u8 state;
    u8 sub;
    u16 kind;
    f32 f2C;
} P_25a478;
typedef struct {
    u8 pad0[8];
    u8 b8;
    u8 pad9[0x14 - 9];
    s16 h14;
    s16 h16;
    u8 pad18[4];
    s16 h1C;
    u8 pad1e[0x2E - 0x1E];
    u8 b2E;
} Q_25a478;
typedef struct { u8 pad0[0x10]; f32 x, y; } O_25a478;
extern u8 D_0013E533[];
extern f32 D_0015FFC8[2] __attribute__((sda));
extern f32 D_001CC0C8[][8];
extern u8 D_001CC080[][4];
extern Q_25a478 *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern Q_25a478 *ReadStateField(void *);
extern s32 fast_dec_timer_s16(s16 *) __asm__("func_001F9770");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern f32 fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");

s32 fun_00213928(O_25a478 *obj, P_25a478 *p, Q_25a478 *q, u32 flags, s32 *out, f32 *fp, s32 *kp, s32 idx) __asm__("FUN_00213928");

s32 fun_00213928(O_25a478 *obj, P_25a478 *hit_record, Q_25a478 *q, u32 flags, s32 *out, f32 *fp, s32 *kp, s32 idx) {
    enum {
        KIND_GATED_HIT_RESPONSE = 0,
        FALLBACK_HIT_RESPONSE = 1,
        DEFAULT_HIT_RESPONSE = 1,
        AUXILIARY_FLAG_SETTING_HIT_RESPONSE = 4,
        CONDITIONAL_DURATION_HIT_RESPONSE = 7,
        DAMAGE_PRESERVING_COOLDOWN_RESPONSE = 11
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
    V_25a478 v;
    V_25a478 combined_hit_vector;
    s32 r;
    s32 kind;
    s32 res;
    Q_25a478 *o;

    *out = 0;
    if (kp) *kp = -1;
    if (hit_record && hit_record->tgt && (hit_record->tgt->hA6 == 0x131 || hit_record->tgt->hA6 == 0xA8) && q) {
        hit_record->f2C = D_0015FFC8[q->b8];
    }
    if (!obj) {
        if (q) {
            q->h1C = 0;
            q->h14 = -1;
        }
        q->h16 = -1;
        *out = 0;
        return 0;
    }
    if (!q) {
        q = suck_get_aim_height(obj);
        if (!q) {
            *out = 0;
            return 0;
        }
    }
    if (idx == 4) idx = q->b8;
    if (!hit_record) {
        if (q) {
            fast_dec_timer_s16(&q->h1C);
            q->h16 = -1;
        }
        *out = 1;
        if (fp) *fp = 0;
        return 0xB;
    }
    if (hit_record->u.s.f1C == 5627.9248f) {
        V_25a478 *pv = &hit_record->u.pos;
        fast_vec_scale(pv, pv, D_001CC0C8[idx][hit_record->state]);
        kind = hit_record->kind;
        hit_record->u.s.f1C = 5627.9248f;
        if (kind == 0x47) {
            v.f[0] = fast_cos(fast_arc_tan(obj->x - hero.motion.pos.f[0], obj->y - hero.motion.pos.f[1]));
            v.f[1] = fast_sin(fast_arc_tan(obj->x - hero.motion.pos.f[0], obj->y - hero.motion.pos.f[1]));
            v.f[2] = 1.0f;
            fast_vec_normalize(&v, &v, fast_vec_length(pv) * 0.7f);
            fast_vec_scale(pv, pv, 0.3f);
            add_vector_xyz(&combined_hit_vector, pv, &v);
            pv->q = combined_hit_vector.q;
            set_vector_length_xy(pv, pv, 1.0f);
        }
    } else {
        if (!hit_record->tgt) { hit_record->state = 8; kind = 0x47; }
        else { kind = hit_record->tgt->hA6; hit_record->state = 8; }
    }
    switch (hit_record->state) {
    case KIND_GATED_HIT_STATE:
        if (hit_record->kind == 1 || hit_record->kind == 2) { *out = 2; r = 0; break; }
        goto dflt;
    case SUBSTATE_GATED_DAMAGE_PRESERVING_HIT_STATE:
        if (hit_record->sub < 2) { *out = 2; r = 0xB; break; }
        if (hit_record->sub == 2) { *out = 2; r = 5; break; }
        *out = 2; r = 9;
        break;
    case SUBSTATE_GATED_HIT_STATE:
        if (hit_record->sub < 2) { *out = 2; r = 8; break; }
        *out = 2; r = 0xD;
        break;
    case COOLDOWN_RESETTING_HIT_STATE:
        if (hit_record->sub < 2) { *out = 2; r = 0xA; break; }
        if (hit_record->sub < 3) { *out = 2; r = 0xF; break; }
        *out = 2; r = 0x10; q->h14 = -1; q->h1C = 0;
        break;
    case FIXED_RESPONSE_HIT_STATE:
        *out = 2; r = 0xE;
        break;
    case SUBSTATE_GATED_AUXILIARY_FLAG_HIT_STATE:
        if (hit_record->sub < 2) { *out = 3; r = 4; break; }
        *out = 4; r = 7;
        break;
    case SUBSTATE_GATED_COOLDOWN_DAMAGE_CLEARING_HIT_STATE:
        if (hit_record->sub < 2) { *out = 2; r = 0xC; break; }
        *out = 2; r = 0x11;
        break;
    default:
    dflt:
        *out = 2; r = 1;
        break;
    }
    if (kp) *kp = kind;
    res = D_001CC080[r][q->b8];
    if (q->h14 != kind) q->h1C = 0;
    if (fast_dec_timer_s16(&q->h1C)) {
        switch (r) {
        case KIND_GATED_HIT_RESPONSE: q->h1C = scale_game_frames(0x25); break;
        case DEFAULT_HIT_RESPONSE: q->h1C = scale_game_frames(0xF); break;
        case AUXILIARY_FLAG_SETTING_HIT_RESPONSE: q->h1C = scale_game_frames(0x3C); break;
        case CONDITIONAL_DURATION_HIT_RESPONSE:
            if (D_0013E533[0]) q->h1C = scale_game_frames(0x2D);
            else q->h1C = scale_game_frames(0x3C);
            break;
        case DAMAGE_PRESERVING_COOLDOWN_RESPONSE: q->h1C = scale_game_frames(0x3C); break;
        default: q->h1C = scale_game_frames(0x1E); break;
        }
    } else {
        switch (r) {
        case KIND_GATED_HIT_RESPONSE:
        case FALLBACK_HIT_RESPONSE:
            if (!(flags & 0x10)) res = 0xB;
            if (!(flags & 0x20)) *out = 1;
            goto z;
        case AUXILIARY_FLAG_SETTING_HIT_RESPONSE:
            if (!(flags & 1)) res = 0xB;
            if (!(flags & 2)) *out = 1;
            goto z;
        case CONDITIONAL_DURATION_HIT_RESPONSE:
            if (!(flags & 4)) res = 0xB;
            if (!(flags & 8)) *out = 1;
        z:
            hit_record->f2C = 0;
            break;
        case DAMAGE_PRESERVING_COOLDOWN_RESPONSE:
            res = 0xC;
            break;
        default:
            hit_record->f2C = 0;
            res = 0xD;
            break;
        }
    }
    q->h14 = kind;
    q->h16 = kind;
    if (fp) *fp = hit_record->f2C;
    if (r == 4) {
        o = ReadStateField(obj);
        if (o) o->b2E = 1;
    } else {
        Q_25a478 *o2 = ReadStateField(obj);
        if (o2) o2->b2E = 0;
    }
    return res;
}

extern __typeof__(fun_00213928) func_00213928 __attribute__((alias("FUN_00213928")));

/* Level 15 moby classes: gate, construction_arm, helicopter, ultramech_unlimited. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"

#define NOT_SDA

#define MACRO_ADDR

/* Nudge a moby's field 0x48 toward a signed step depending on a linked moby's state. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002E5AA0), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern short D_L15_0015FFD8_d __asm__("D_L15_0015FFD8");

extern short D_L15_00161EC0 __attribute__((sda));
void FUN_L15_002e46b0(struct Moby *moby) {
    int idx = *(int *)moby->pvars;
    if (idx != -1) {
        char *other = (char *)(idx * 256 + *(int *)&D_L15_0015FFD8_d);
        char *d = *(char **)(other + 0x78);
        int st = (unsigned char)other[0x20];
        float step;
        if (st == 2) {
            step = *(float *)&D_L15_00161EC0 * DEG_TO_RAD * frame_time;
            if (*(int *)(d + 0x10) != 0)
                step = -step;
        } else if (st == 4) {
            step = *(float *)&D_L15_00161EC0 * DEG_TO_RAD * frame_time;
            if (*(int *)(d + 0x10) == 0)
                step = -step;
        } else {
            return;
        }
        moby->rot.z = fast_add_rotations(moby->rot.z, step);
    }
}

typedef struct {
    Vec4 home;
    s32 open_volume;
    s32 close_volume;
    f32 travel;
    s32 trigger;
    s32 sound;
    f32 duration;
    s32 lock;
} MovingVars_29aff0;

typedef struct {
    char pad00[0x74];
    u8 active;
    char pad75[0x13];
    struct Moby *owner;
} VoiceSlot_29aff0;

typedef struct {
    char pad0[0x454];
    unsigned char collected[1];
} CollectedFlags_29aff0;

extern CollectedFlags_29aff0 D_L15_001BB830_29aff0 __asm__("D_L15_001BB830");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern u8 D_0013F3D0[];
extern char D_L15_001673C0[];
extern char D_0013E550[];
extern char D_0014C190[];
extern int D_L15_001BAAD0[];
extern s32 current_level_index __asm__("D_0015ED84");
extern f32 frame_time __asm__("D_0015ED6C");

void FUN_L15_0029aff0(struct Moby *m)
{
    enum {
        GATE_INITIALIZE = 0,
        GATE_WAIT_TO_OPEN = 1,
        GATE_OPENING = 2,
        GATE_OPEN = 3,
        GATE_CLOSING = 4
    };
    MovingVars_29aff0 *d = (MovingVars_29aff0 *)m->pvars;
    Vec4 offset;
    Vec4 pos;
    s32 slot;
    VoiceSlot_29aff0 *voice_slot;
    s32 trigger_active;
    u32 mask;
    u32 *bits;

    switch (m->state) {
    case GATE_INITIALIZE:
        d->home.q = *(u128 *)&m->pos;
        m->state = 1;
        return;
    case GATE_WAIT_TO_OPEN:
        trigger_active = 0;
        d->travel = 0;
        if (d->trigger != -1) {
            struct Moby *other = (struct Moby *)((char *)*(s32 *)&D_L15_0015FFD8_d + d->trigger * 0x100);
            if (other->oclass == 0x4b9) {
                if (other->state == 4) {
                    trigger_active = 1;
                    goto check_lock;
                }
            }
            if (other->oclass == 0x596) {
                trigger_active = other->state == 4;
            }
        }
check_lock:
        if (d->lock != 0) {
            if (D_L15_001BB830_29aff0.collected[(s16)m->save_id] != 0 ||
                ((*(s32 *)(D_0014C190 + (current_level_index * 0x100 + ((s16)m->save_id >> 5) * 4)) >> (m->save_id & 31)) & 1) != 0)
                trigger_active = 1;
        }
        if (is_point_inside_clip_volume(D_0013F3D0, d->open_volume)) {
            m->state = 2;
        } else if (trigger_active) {
            m->state = 2;
        }
        return;
    case GATE_OPENING:
        if (((VoiceSlot_29aff0 *)(D_0013E550 + d->sound * 0x70))->owner != m ||
            ((VoiceSlot_29aff0 *)(D_0013E550 + d->sound * 0x70))->active == 0)
            d->sound = allocate_voice_for_target_entry(0, 4, (int)m);
        if (d->duration < (d->travel = d->travel + (d->duration / 0.6f) * frame_time)) {
            slot = d->sound;
            if (slot != -1) {
                if (((VoiceSlot_29aff0 *)(D_0013E550 + slot * 0x70))->owner == m &&
                    ((VoiceSlot_29aff0 *)(D_0013E550 + slot * 0x70))->active != 0)
                    release_voice_slot(slot);
            }
            d->travel = d->duration;
            d->sound = -1;
            if (d->trigger != -1 || d->lock != 0) {
                mask = 1U << (m->save_id & 31);
                bits = (u32 *)(D_0014C190 + (((s16)m->save_id >> 5) * 4 + current_level_index * 0x100));
                *bits |= mask;
                D_L15_001BAAD0[((s16)m->save_id >> 5)] |= 1U << (m->save_id & 31);
                m->state = 5;
            } else {
                m->state = 3;
            }
        }
        offset.f[0] = fast_cos(m->rot.z) * -d->travel;
        offset.f[1] = fast_sin(m->rot.z) * -d->travel;
        offset.f[2] = 0.0f;
        add_vector_xyz(&pos, &d->home, &offset);
        *(u128 *)&m->pos = pos.q;
        return;
    case GATE_OPEN:
        if (is_point_inside_clip_volume(D_0013F3D0, d->close_volume) ||
            is_point_inside_clip_volume(D_L15_001673C0, d->close_volume))
            return;
        m->state = 4;
        return;
    case GATE_CLOSING:
        if (((VoiceSlot_29aff0 *)(D_0013E550 + d->sound * 0x70))->owner != m ||
            ((VoiceSlot_29aff0 *)(D_0013E550 + d->sound * 0x70))->active == 0)
            d->sound = allocate_voice_for_target_entry(0, 4, (int)m);
        if ((d->travel = d->travel - (d->duration / 0.6f) * frame_time) < 0.0f) {
            slot = d->sound;
            if (slot != -1 && ((VoiceSlot_29aff0 *)(D_0013E550 + slot * 0x70))->owner == m &&
                ((VoiceSlot_29aff0 *)(D_0013E550 + slot * 0x70))->active != 0)
                release_voice_slot(slot);
            d->travel = 0;
            d->sound = -1;
            m->state = 1;
        }
        offset.f[0] = fast_cos(m->rot.z) * -d->travel;
        offset.f[1] = fast_sin(m->rot.z) * -d->travel;
        offset.f[2] = 0.0f;
        add_vector_xyz(&pos, &d->home, &offset);
        *(u128 *)&m->pos = pos.q;
        if (is_point_inside_clip_volume(D_0013F3D0, d->open_volume) ||
            is_point_inside_clip_volume(D_L15_001673C0, d->open_volume))
            m->state = 2;
        return;
    default:
        return;
    }
}
/* Ported from rac1-decomp src/overlays/l15_quartu/vendor_0029C1D0.c (func_L15_002A48B0) */
#include "eetypes.h"

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Vec_2A48B0;

typedef struct {
    Vec_2A48B0 home;   /* 0x00: x y z w at open */
    int vol_near;       /* 0x10 */
    int vol_trigger;    /* 0x14 */
    float travel;       /* 0x18 */
    int sound;          /* 0x1C */
    int key;            /* 0x20 */
    int trigger;        /* 0x24 */
    int locked;         /* 0x28 */
} GateVars_2A48B0;

typedef struct {
    char pad00[0x10];
    Vec_2A48B0 pos;    /* 0x10 */
    unsigned char state; /* 0x20 */
    char pad21[0xF];
    unsigned char f30;  /* 0x30 */
    char pad31[0x47];
    GateVars_2A48B0 *vars; /* 0x78 */
} GateMoby_2A48B0;


typedef struct {
    char pad00[0x74];
    unsigned char active; /* 0x74 */
    char pad75[0x13];
    GateMoby_2A48B0 *owner; /* 0x88 */
} SoundSlot_2A48B0;

extern float D_0015ED6C;
extern float D_L15_001614F8 __attribute__((sda));
extern int D_L15_00161AC0;
extern unsigned char D_001413F4[];
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int func_0022ED80_2A48B0(int, int, GateMoby_2A48B0 *) __asm__("FUN_0022da68");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern unsigned char D_0013DD50 __attribute__((section(".data")));
extern unsigned char D_0013F3D0_2A48B0[] __asm__("D_0013F3D0");
extern char D_0013E550[];
extern char D_L15_00167440_2A48B0[] __asm__("D_L15_001673C0");
extern int D_L15_00184B28_2A48B0 __asm__("D_L15_00184AA8") __attribute__((section(".data")));

void FUN_L15_002a36d0(GateMoby_2A48B0 *m)
{
    enum {
        GATE_INITIALIZE = 0,
        GATE_WAIT_TO_OPEN = 1,
        GATE_OPENING = 2,
        GATE_OPEN = 3,
        GATE_CLOSING = 4,
        GATE_POSITION_OVERRIDE = 5
    };
    GateVars_2A48B0 *d = m->vars;
    float step;

    switch (m->state) {
    case GATE_INITIALIZE:
        *(u128 *)&d->home = *(u128 *)&m->pos;
        m->state = 1;
        d->locked = 0;
        if (d->key != -1) {
            m->f30 = 0xFF;
        }
        if (D_0013DD50 != 0 && d->key != -1) {
            m->pos.z = m->pos.z - D_L15_001614F8;
            m->state = 5;
        }
        break;
    case GATE_WAIT_TO_OPEN:
        d->travel = 0;
        if (d->key != -1) {
            D_L15_00184B28_2A48B0 = 0;
        }
        if (d->trigger != -1) {
            struct Moby *p = (struct Moby *)((char *)*(int *)&D_L15_0015FFD8_d + (d->trigger << 8));
            if (p == 0 || p->state == 0xFE || p->state == 0xFD) {
                if (D_L15_00161AC0 == 0) {
                    m->state = 2;
                    d->locked = 1;
                }
            }
        } else {
            if (is_point_inside_clip_volume(D_0013F3D0_2A48B0, d->vol_near)) {
                if (d->key == -1 || D_0013F3D0_2A48B0[0x2024] == 2) {
                    m->state = 2;
                }
            }
        }
        if (D_0013DD50 != 0 && d->key != -1) {
            m->pos.z = d->home.z - D_L15_001614F8;
            m->state = 5;
        }
        break;
    case GATE_OPENING:
        step = (D_L15_001614F8 / 0.6f) * D_0015ED6C;
        if (d->key != -1) {
            step = step * 5.0f;
        }
        if (((SoundSlot_2A48B0 *)(D_0013E550 + d->sound * 0x70))->owner != m ||
            ((SoundSlot_2A48B0 *)(D_0013E550 + d->sound * 0x70))->active == 0) {
            d->sound = func_0022ED80_2A48B0(0, 4, m);
        }
        if (D_L15_001614F8 < (d->travel = d->travel + step)) {
            if (d->sound != -1 &&
                ((SoundSlot_2A48B0 *)(D_0013E550 + d->sound * 0x70))->owner == m &&
                ((SoundSlot_2A48B0 *)(D_0013E550 + d->sound * 0x70))->active != 0) {
                release_voice_slot(d->sound);
            }
            d->sound = -1;
            d->travel = D_L15_001614F8;
            m->state = 3;
        }
        {
            float t = d->travel;
            float h = d->home.z;
            float z;
            if (d->key != -1) {
                t = -t;
                z = h + t;
            } else {
                z = h + t;
            }
            m->pos.z = z;
        }
        break;
    case GATE_OPEN:
        if (d->key != -1) {
            D_L15_00184B28_2A48B0 = 1;
        }
        if (is_point_inside_clip_volume(D_0013F3D0_2A48B0, d->vol_trigger) != 0 ||
            is_point_inside_clip_volume(D_L15_00167440_2A48B0, d->vol_trigger) != 0) {
            if (is_point_inside_clip_volume(D_0013F3D0_2A48B0, d->key) == 0) {
                break;
            }
        }
        if (d->locked == 0) {
            m->state = 4;
        }
        break;
    case GATE_CLOSING:
        step = (D_L15_001614F8 / 0.6f) * D_0015ED6C;
        if (d->key != -1) {
            step = step * 5.0f;
        }
        if (((SoundSlot_2A48B0 *)(D_0013E550 + d->sound * 0x70))->owner != m ||
            ((SoundSlot_2A48B0 *)(D_0013E550 + d->sound * 0x70))->active == 0) {
            d->sound = func_0022ED80_2A48B0(0, 4, m);
        }
        if ((d->travel = d->travel - step) < 0.0f) {
            if (d->sound != -1 &&
                ((SoundSlot_2A48B0 *)(D_0013E550 + d->sound * 0x70))->owner == m &&
                ((SoundSlot_2A48B0 *)(D_0013E550 + d->sound * 0x70))->active != 0) {
                release_voice_slot(d->sound);
            }
            d->travel = 0;
            d->sound = -1;
            m->state = 1;
            if (d->key != -1) {
                m->state = 5;
                D_L15_00184B28_2A48B0 = 0;
            }
        }
        {
            float t = d->travel;
            float h = d->home.z;
            float z;
            if (d->key != -1) {
                t = -t;
                z = h + t;
            } else {
                z = h + t;
            }
            m->pos.z = z;
        }
        if (is_point_inside_clip_volume(D_0013F3D0_2A48B0, d->vol_near) != 0 ||
            is_point_inside_clip_volume(D_L15_00167440_2A48B0, d->vol_near) != 0) {
            m->state = 2;
        }
        break;
    case GATE_POSITION_OVERRIDE:
        if (D_0013DD50 != 0) {
            m->pos.z = d->home.z - D_L15_001614F8;
            D_L15_00184B28_2A48B0 = 1;
        } else {
            if (D_001413F4[0] == 2) {
                break;
            }
            m->pos.z = d->home.z;
            m->state = 1;
            D_L15_00184B28_2A48B0 = 0;
        }
        break;
    }
}
#include "rnc/gameplay/entities/moby.h"
/* pvars of the swinging door FUN_L15_002a3ba8 updates */
typedef struct {
    f32 closed_yaw;   /* 0x00: yaw when shut */
    s32 open_volume;  /* 0x04: someone inside opens it */
    s32 close_volume; /* 0x08: nobody inside lets it close */
    f32 swing;        /* 0x0C: current swing from closed_yaw */
    s32 reverse;      /* 0x10: swings the other way */
    s32 trigger;      /* 0x14: moby index that opens it, -1 for the volumes */
    s32 sound;        /* 0x18 */
} DoorVars;

extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void *suck_get_aim_height(struct Moby *) __asm__("FUN_002141f8");
extern char D_L15_001673C0[];
extern char *D_L15_0015FFE4;
extern struct Moby *D_L15_0015FFD8;
extern f32 D_L15_001614FC __attribute__((sda));
extern f32 D_L15_00161500 __attribute__((sda));

void FUN_L15_002a3ba8(struct Moby *moby) {
    enum {
        SWINGING_DOOR_INITIALIZE = 0,
        SWINGING_DOOR_CLOSED = 1,
        SWINGING_DOOR_OPENING = 2,
        SWINGING_DOOR_OPEN = 3,
        SWINGING_DOOR_CLOSING = 4
    };
    DoorVars *vars = (DoorVars *)moby->pvars;
    s32 open = 0;
    s32 close = 0;
    struct Moby *m;
    f32 step;
    f32 limit;
    f32 yaw;

    if (vars->trigger == -1) {
        if (moby->state != 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, vars->open_volume) ||
                is_point_inside_clip_volume(D_L15_001673C0, vars->open_volume)) {
                open = 1;
            } else if (!is_point_inside_clip_volume(&hero.motion.pos, vars->close_volume) &&
                       !is_point_inside_clip_volume(D_L15_001673C0, vars->close_volume)) {
                close = 1;
            }
            for (m = (struct Moby *)D_L15_0015FFE4; m != 0; m = m->next) {
                if (suck_get_aim_height(m) == 0) {
                    continue;
                }
                if (is_point_inside_clip_volume(&m->pos, vars->open_volume)) {
                    open = 1;
                } else if (!is_point_inside_clip_volume(&m->pos, vars->close_volume)) {
                    close = 1;
                }
            }
        }
    } else {
        if (D_L15_0015FFD8[vars->trigger].oclass == 0x49B) {
            open = D_L15_0015FFD8[vars->trigger].state == 2;
        }
    }
    if (open) {
        close = 0;
    }
    switch (moby->state) {
    case SWINGING_DOOR_INITIALIZE:
        if (vars->reverse) {
            moby->flags |= 0x8000;
        }
        step = D_L15_001614FC * DEG_TO_RAD;
        if (!vars->reverse) {
            step = -step;
        }
        yaw = fast_add_rotations(moby->rot.z, step);
        vars->swing = 0.0f;
        vars->closed_yaw = yaw;
        moby->rot.z = yaw;
        moby->state = 1;
        break;
    case SWINGING_DOOR_CLOSED:
        vars->swing = 0.0f;
        moby->rot.z = vars->closed_yaw;
        if (open) {
            moby->state = 2;
            vars->sound = allocate_voice_for_target_entry(0, 0, (int)moby);
        }
        break;
    case SWINGING_DOOR_OPENING:
        vars->swing = fast_add_rotations(vars->swing, D_L15_001614FC / D_L15_00161500 * DEG_TO_RAD * frame_time);
        limit = D_L15_001614FC * DEG_TO_RAD;
        if (vars->swing < limit) {
            vars->swing = limit;
            moby->state = 3;
        }
        step = vars->swing;
        if (vars->reverse) {
            step = -step;
        }
        moby->rot.z = fast_add_rotations(vars->closed_yaw, step);
        break;
    case SWINGING_DOOR_OPEN:
        if (close) {
            moby->state = 4;
            vars->sound = allocate_voice_for_target_entry(0, 0, (int)moby);
        }
        break;
    case SWINGING_DOOR_CLOSING:
        vars->swing = fast_add_rotations(vars->swing, -(D_L15_001614FC / D_L15_00161500 * DEG_TO_RAD * frame_time));
        if (0.0f < vars->swing) {
            vars->swing = 0.0f;
            moby->state = 1;
        }
        step = vars->swing;
        if (vars->reverse) {
            step = -step;
        }
        moby->rot.z = fast_add_rotations(vars->closed_yaw, step);
        if (open) {
            moby->state = 2;
        }
        break;
    }
}
typedef union { u128 q; float f[4]; } Vec_2a7880;
extern char D_L15_00178700[];
extern char D_L15_00161568[];
extern u64 D_L15_00161570;
typedef struct { u32 words[3]; } Colors_2a7880;
extern Colors_2a7880 D_L15_00161578;
extern u32 D_L15_00161580;
extern u8 D_001413F4[];
extern u8 D_0013DD50;
extern char D_0014C190[];
extern int current_level_index __asm__("D_0015ED84");
extern float frame_time __asm__("D_0015ED6C");
extern void normalize_vector_xyz_7880(float, void *, void *) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_7880(float, void *, void *) __asm__("FUN_001f9a68");
extern float vector_length_xyz_7880(void *) __asm__("FUN_001f9af0");
extern void add_vector_xyz_7880(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_7880(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_full_vector_7880(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void clear_u64_value_7880(void *) __asm__("FUN_001f99f8");
extern float fast_cos_7880(float) __asm__("FUN_001f9dc8");
extern float fast_sin_7880(float) __asm__("FUN_001f9de0");
extern float random_float_between_7880(float, float) __asm__("FUN_002132a8");
extern float random_angle_radians_7880(void) __asm__("FUN_00213308");
extern int random_integer_below_7880(int) __asm__("FUN_00213260");
extern int FUN_L00_00257b90_7880(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L00_002371e0_7880(float, int, int) __asm__("FUN_L15_00249ce8");
extern int scale_game_frames_7880(int) __asm__("FUN_001f96f8");
extern void mark_moby_for_removal_7880(struct Moby *) __asm__("FUN_0020c828");
extern void FUN_L00_00257470_7880(void *, int, long long) __asm__("FUN_L00_00257470");
extern void FUN_L00_0025e450_7880(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void *FUN_L01_002f8530_7880(void *, void *, int, float, int, float, float, float, int) __asm__("FUN_L01_002f8530");
extern void *FUN_L00_0026bed0_7880(void *, void *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");

void FUN_L15_002a7880(struct Moby *moby)
{
    char *slot = 0;
    Vec_2a7880 velocity, first, second, offset, base;
    u32 palette[4];
    u32 colors[4];
    Vec_2a7880 position;
    int i, index, slot_index;
    u32 *palette_entry;
    float radius, angle, size;
    int color, color2, duration;
    void *effect;
    Vec_2a7880 *base_position;

    if (D_001413F4[0] != 2 && moby->state == 1)
        return;

    for (slot_index = 0; slot_index < 64; slot_index++) {
        char *entry = D_L15_00178700 + slot_index * 0x40;
        if (*(struct Moby **)(entry + 0x34) != moby)
            continue;
        if ((*(u32 *)(entry + 0x24) & 0x80000) == 0)
            continue;
        slot = entry;
        break;
    }
    moby->hit_slot = 0xff;
    if (slot == 0)
        goto no_hit;

    __builtin_memcpy(palette, &D_L15_00161568, 16);
    normalize_vector_xyz_7880(frame_time * 10.0f, &first, slot + 0x10);
    scale_vector_xyz_7880(2.0f, &second, &first);
    if (vector_length_xyz_7880(slot) != 0.0f) {
        base.q = *(u128 *)slot;
    } else {
        base.q = *(u128 *)&moby->pos;
        base.f[2] += 6.0f;
    }

    base_position = &base;
    {
    float radius, angle;
    int i = 0x18;
    do {
        int duration;
        radius = random_float_between_7880(0.5f, 1.5f);
        angle = random_angle_radians_7880();
        if (moby->oclass == 0x94)
            radius *= 3.0f;
        offset.f[0] = fast_cos_7880(angle) * radius;
        offset.f[1] = fast_sin_7880(angle) * radius;
        offset.f[2] = 0.0f;
        offset.f[2] = random_float_between_7880(1.0f, 7.0f);
        add_full_vector_7880(colors, &offset, &moby->pos);
        subtract_vector_xyz_7880(colors, base_position, &offset);
        velocity.q = *(u128 *)colors;
        normalize_vector_xyz_7880(frame_time * 5.0f, &velocity, &velocity);
        add_full_vector_7880(colors, &velocity, &second);
        velocity.f[2] += frame_time * 3.0f;
        size = random_float_between_7880(0.75f, 1.25f);
        scale_vector_xyz_7880(size, &velocity, &velocity);
        index = random_integer_below_7880(4);
        palette_entry = &palette[index];
        radius = random_float_between_7880(0.1f, 0.3f);
        duration = FUN_L00_00257b90_7880(60, 100);
        size = random_float_between_7880(3.0f, 7.0f);
        FUN_L01_002f8530_7880(&offset, &velocity, *palette_entry, radius, duration,
                                size, 0.15f, 2.0f, 1);
        i--;
    } while (i >= 0);
    }
    if (moby->oclass == 0x94) {
        offset.q = *(u128 *)&moby->pos;
        offset.f[2] += 10.0f;
        radius = 1.0f;
        subtract_vector_xyz_7880(colors, base_position, &offset);
        velocity.q = *(u128 *)colors;
        normalize_vector_xyz_7880(frame_time * 5.0f, &velocity, &velocity);
        add_full_vector_7880(colors, &velocity, &second);
        velocity.f[2] += frame_time * 3.0f;
        size = random_float_between_7880(0.75f, 1.25f);
        scale_vector_xyz_7880(size, &velocity, &velocity);
        duration = FUN_L00_00257b90_7880(60, 100);
        effect = FUN_L01_002f8530_7880(&offset, &velocity, 0xc9, radius, duration,
                                        5.0f, radius, 2.0f, 1);
        if (effect != 0)
            scale_vector_xyz_7880(0.35f, (char *)effect + 0x40, (char *)effect + 0x40);
    }
    clear_u64_value_7880(&velocity);
    FUN_L00_0025e450_7880(moby, &velocity, base_position, 0.0f, 0.0f, 20,
                            3, 4, 10.0f, 5.0f, 100000.0f,
                            0, 3.0f, 15.0f, 1, 1, -1, 0);

    i = 0x13;
    do {
        float radius, angle;
        int color, color2, duration;
        __builtin_memcpy(colors, &D_L15_00161578, 12);
        radius = random_float_between_7880(0.0f, 1.5f);
        angle = random_angle_radians_7880();
        if (moby->oclass == 0x94)
            radius *= 3.0f;
        offset.f[0] = fast_cos_7880(angle) * radius;
        offset.f[1] = fast_sin_7880(angle) * radius;
        offset.f[2] = 0.0f;
        add_vector_xyz_7880(&offset, &offset, &moby->pos);
        offset.f[2] += random_float_between_7880(1.0f, 5.5f);
        subtract_vector_xyz_7880(&position, base_position, &offset);
        velocity.q = position.q;
        normalize_vector_xyz_7880(frame_time * 5.0f, &velocity, &velocity);
        add_full_vector_7880(&position, &velocity, &first);
        size = random_float_between_7880(0.75f, 1.25f);
        scale_vector_xyz_7880(size, &velocity, &velocity);
        size = random_float_between_7880(0.25f, 1.0f);
        color = FUN_L00_002371e0_7880(size, 0x7f000000, 0x7f182030);
        size = random_float_between_7880(0.5f, 1.0f);
        color2 = FUN_L00_002371e0_7880(size, 0, 0x5f5f5f);
        size = random_float_between_7880(1.0f, 3.0f) * 210000.0f;
        duration = scale_game_frames_7880(180);
        index = random_integer_below_7880(3);
        effect = FUN_L00_0026bed0_7880(&offset, &velocity, color, color2,
                                         size, duration, colors[index]);
        if (effect != 0)
            *(float *)((char *)effect + 0x2c) = moby->pos.z - 0.5f;
        i--;
    } while (i >= 0);
    FUN_L00_00257470_7880(moby, 0, -1);
    mark_moby_for_removal_7880(moby);
    return;

no_hit:
    moby->hit_slot = 0xff;
    if (moby->state != 0)
        return;
    moby->update_dist = 0xff;
    moby->state = 1;
    moby->draw_dist = 0x80;
    moby->scale = moby->scale + moby->scale;
    {
    u8 saved_flag = D_0013DD50;
    u16 save_id = moby->save_id;
    u32 saved_bit = *(u32 *)(D_0014C190 + (((s16)save_id >> 5) * 4) +
                    current_level_index * 0x100) &
                    (1U << (save_id & 31));
    if (saved_flag != 0 && saved_bit != 0)
        mark_moby_for_removal_7880(moby);
    }
}
extern char D_L15_00178700[];
extern u8 D_001413F4[];
extern u8 D_0013DD50;
extern char D_0014C190[];
extern char D_L15_001615B8[];
extern struct { u32 words[3]; } D_L15_001615C8;
extern void fast_vec_normalize(void *out, void *a, f32 length) __asm__("FUN_001f9bf8");
extern void fast_vec_scale(void *out, void *a, f32 scale) __asm__("FUN_001f9a68");
extern f32 fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern void fast_vec_add(void *out, void *a, void *b) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void add_vectors(void *out, void *a, void *b) __asm__("FUN_L00_001ff290");
extern void clear_vector_2aa280(void *) __asm__("FUN_001f99f8");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int blend_color(float t, int from, int to) __asm__("FUN_L15_00249ce8");
extern s32 scale_game_frames_2aa280(s32) __asm__("FUN_001f96f8");
extern void mark_moby_for_removal(struct Moby *) __asm__("FUN_0020c828");
extern void FUN_L00_00257470(void *, int, int);
extern void spawn_explosion(void *m, void *unused, void *pos, float fA, float fB, int n, int k8, int k9,
                             float fC, float fD, float fE, float fF, int k10, float fG, int k11,
                             int s16, int s18, unsigned char flag) __asm__("FUN_L00_0025e450");
extern void *spawn_debris_chunk(void *pos, void *vel, int color, float a, int life, float size, float b,
                                float c, int d) __asm__("FUN_L01_002f8530");
extern void *mine_debris(void *pos, void *vel, int color, int color2, float size, int life, int tint)
    __asm__("FUN_L00_0026bed0");

/* Hit by a shot (a 0x40-byte entry of the hit table that names this moby): sparks fly from the hit
   point, a burst and delta follow, then the moby goes away. Not hit: on its first update it sets its
   distances and is removed when its save bit says it is gone. */
void FUN_L15_002aa280(struct Moby *moby)
{
    char *hit = 0;
    Vec4 velocity, hit_direction, push, offset, center;
    u32 palette[4];
    u32 colors[4];
    Vec4 delta;
    int spark, puff, pick, n;
    u32 *color;
    float size;
    void *debris;

    if (D_001413F4[0] != 2 && moby->state == 1)
        return;

    for (n = 0; n < 64; n++) {
        char *entry = D_L15_00178700 + n * 0x40;
        if (*(struct Moby **)(entry + 0x34) != moby)
            continue;
        if ((*(u32 *)(entry + 0x24) & 0x80000) == 0)
            continue;
        hit = entry;
        break;
    }
    moby->hit_slot = 0xff;
    if (hit != 0) {
        __builtin_memcpy(palette, &D_L15_001615B8, 16);
        fast_vec_normalize(&hit_direction, hit + 0x10, frame_time * 10.0f);
        fast_vec_scale(&push, &hit_direction, 2.0f);
        if (fast_vec_length(hit) != 0.0f) {
            center.q = *(u128 *)hit;
        } else {
            center.q = *(u128 *)&moby->pos;
            center.f[2] += 6.0f;
        }

        for (spark = 0; spark < 25; spark++) {
            float radius, angle;
            int life;
            angle = random_float_between(moby->rot.z, 1.5707963267948966f);
            radius = fast_cos(angle);
            radius *= random_float_between(-3.0f, 3.0f);
            offset.f[0] = radius;
            angle = random_float_between(moby->rot.z, 1.5707963267948966f);
            radius = fast_sin(angle);
            radius *= random_float_between(-3.0f, 3.0f);
            offset.f[1] = radius;
            offset.f[2] = 0.0f;
            offset.f[2] = random_float_between(1.0f, 7.0f);
            add_vectors(colors, &offset, &moby->pos);
            fast_vec_sub(colors, &center, &offset);
            velocity.q = *(u128 *)colors;
            fast_vec_normalize(&velocity, &velocity, frame_time * 5.0f);
            add_vectors(colors, &velocity, &push);
            velocity.f[2] += frame_time * 3.0f;
            size = random_float_between(0.75f, 1.25f);
            fast_vec_scale(&velocity, &velocity, size);
            pick = random_integer_below(4);
            color = &palette[pick];
            radius = random_float_between(0.1f, 0.3f);
            life = random_int_between(100, 160);
            size = random_float_between(3.0f, 7.0f);
            spawn_debris_chunk(&offset, &velocity, *color, radius, life, size, 0.15f, 2.0f, 1);
        }
        clear_vector_2aa280(&velocity);
        spawn_explosion(moby, &velocity, &center, 0.0f, 0.0f, 20, 3, 4, 10.0f, 5.0f, 100000.0f, 3.0f, 0,
                         15.0f, 1, 1, -1, 0);

        for (puff = 0; puff < 20; puff++) {
            float radius, angle;
            int color1, color2, life;
            __builtin_memcpy(colors, &D_L15_001615C8, 12);
            angle = random_float_between(moby->rot.z, 1.5707963267948966f);
            radius = fast_cos(angle);
            radius *= random_float_between(-3.0f, 3.0f);
            offset.f[0] = radius;
            angle = random_float_between(moby->rot.z, 1.5707963267948966f);
            radius = fast_sin(angle);
            radius *= random_float_between(-3.0f, 3.0f);
            offset.f[1] = radius;
            offset.f[2] = 0.0f;
            fast_vec_add(&offset, &offset, &moby->pos);
            offset.f[2] += random_float_between(1.0f, 5.5f);
            fast_vec_sub(&delta, &center, &offset);
            velocity.q = delta.q;
            fast_vec_normalize(&velocity, &velocity, frame_time * 5.0f);
            add_vectors(&delta, &velocity, &hit_direction);
            size = random_float_between(0.75f, 1.25f);
            fast_vec_scale(&velocity, &velocity, size);
            size = random_float_between(0.25f, 1.0f);
            color1 = blend_color(size, 0x7f000000, 0x7f182030);
            size = random_float_between(0.5f, 1.0f);
            color2 = blend_color(size, 0, 0x5f5f5f);
            size = random_float_between(1.0f, 3.0f) * 210000.0f;
            life = scale_game_frames_2aa280(180);
            pick = random_integer_below(3);
            debris = mine_debris(&offset, &velocity, color1, color2, size, life, colors[pick]);
            if (debris != 0)
                *(float *)((char *)debris + 0x2c) = moby->pos.z - 0.5f;
        }
        FUN_L00_00257470(moby, 0, -1);
        mark_moby_for_removal(moby);
    } else {
        moby->hit_slot = 0xff;
        if (moby->state != 0)
            return;
        moby->draw_dist = 0x80;
        moby->update_dist = 0xff;
        moby->state = 1;
        if (D_0013DD50 != 0) {
            u16 save_id = moby->save_id;
            if ((*(s32 *)(D_0014C190 + (((s16)save_id >> 5) * 4) + current_level_index * 0x100) >> (save_id & 31)) & 1)
                mark_moby_for_removal(moby);
        }
    }
}
typedef struct {
    char pad00[0x68];
    float facing;
    char pad6c[0x14];
    short timer;
    short side;
    float yaw;
    float yaw_velocity;
    float target_yaw;
    float direction;
} UpdateVars_2938;
extern int D_L15_001619EC __attribute__((sda));
extern int D_L15_001619F4 __attribute__((sda));
extern int D_L15_001619F8 __attribute__((sda));
extern float D_L15_001619FC __attribute__((sda));
extern float D_L15_00161A00 __attribute__((sda));
extern float D_L15_00161A04 __attribute__((sda));
extern unsigned short *D_L15_001ABE40[];
extern float D_L15_0015F580[] __attribute__((section(".sdata")));
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void FUN_00251fd0(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern float FUN_L15_001f8648(float);
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern float FUN_001f9b48(void *, void *);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int FUN_L00_00257b90(int, int);
extern int random_integer_below(int) __asm__("FUN_00213260");
extern float random_angle_radians_7880(void) __asm__("FUN_00213308");
extern void mark_moby_for_removal_7880(struct Moby *) __asm__("FUN_0020c828");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void cart_burst(float, float, void *, void *, long long) __asm__("FUN_L00_0025f090");
extern void FUN_L01_002787a0(void *);
extern void *FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
extern void FUN_0025c2f0(float, void *, void *, int, int, int, int) __asm__("FUN_L00_0026cbb0");

void construction_arm_update(struct Moby *moby) __asm__("FUN_L15_002c2938");
void construction_arm_update(struct Moby *moby)
{
    enum {
        CONSTRUCTION_ARM_INITIALIZE = 0,
        CONSTRUCTION_ARM_WAIT_TO_TURN = 1,
        CONSTRUCTION_ARM_TURN_TO_TARGET_YAW = 2,
        CONSTRUCTION_ARM_WAIT_FOR_ENTRY_ANIMATION = 3,
        CONSTRUCTION_ARM_WAIT_TO_START_EXIT_ANIMATION = 4,
        CONSTRUCTION_ARM_WAIT_FOR_EXIT_ANIMATION = 5,
        CONSTRUCTION_ARM_DESTROY = 6
    };
    UpdateVars_2938 *d = (UpdateVars_2938 *)moby->pvars;
    int hit = 0;
    char *slot;
    int ready, count;
    unsigned short *list;
    struct Moby *other;
    float minimum, maximum, speed, spread, direction;
    int animation, duration, first;
    Vec4 origin, velocity, offset, sum;

    if (moby->state != 6) {
        slot = find_moby_hit(moby, 0x10000, 0);
        if (slot != 0 && *(float *)(slot + 0x2c) > 0.0f)
            hit = 1;
        if (hit)
            moby->state = 6;
    }
    if (moby->state != 0) {
        d->facing = d->yaw;
        FUN_00251fd0(frame_scale_sq * 0.03f, frame_scale_sq * 0.3f, moby, d, 0);
    }
    switch (moby->state) {
    case CONSTRUCTION_ARM_INITIALIZE:
        d->timer = truncate_float_to_s32(random_float_between(
            FUN_L15_001f8648(D_L15_00161A00 * 60.0f), FUN_L15_001f8648(D_L15_00161A04 * 60.0f)));
        *(int *)&d->yaw_velocity = d->side = *(int *)&d->yaw = 0;
        moby->update_dist = 0xff;
        moby->state = 1;
        break;
    case CONSTRUCTION_ARM_WAIT_TO_TURN:
        if (fast_dec_timer_s16(&d->timer) && FUN_001f9b48(D_L15_001673C0, &moby->pos) < 42.0f) {
            ready = 1;
            if (moby->group != 0xff) {
                slot = (char *)D_L15_001ABE40[moby->group];
                count = 0;
                if (slot == 0) return;
                list = (unsigned short *)slot;
                do {
                    other = (struct Moby *)((*list & 0x7fff) * 0x100 + (int)D_L15_0015FFD8);
                    if ((signed char)other->state >= 0 && other->oclass == moby->oclass && other->state != 1)
                        count++;
                } while ((short)*list++ >= 0);
                if (count != 0) {
                    d->timer = scale_game_frames(60);
                    ready = 0;
                }
            }
            if (ready) {
                d->target_yaw = random_float_between(-0.20943952f, 0.20943952f);
                moby->state = 2;
            }
        }
        break;
    case CONSTRUCTION_ARM_TURN_TO_TARGET_YAW:
        d->side = FUN_L00_00257b90(0, 1);
        if (fast_difference_between_rotations(d->yaw, d->target_yaw) < 0.008726646f) {
            animation = d->side;
            animation *= 2;
            blend_moby_animation(moby, animation + 1, 0, scale_game_frames(6));
            moby->state = 3;
        }
        d->yaw = spring_turn(&d->yaw_velocity, d->yaw, d->target_yaw,
                             frame_time_sq * 1.5707964f, frame_time_sq * 0.7853982f,
                             frame_time * 0.7853982f);
        break;
    case CONSTRUCTION_ARM_WAIT_FOR_ENTRY_ANIMATION:
        if (moby->prev_seq == d->side * 2 + 1 && (moby->anim_flags & 2)) {
            d->timer = scale_game_frames(FUN_L00_00257b90(15, 45));
            animation = d->side;
            animation *= 2;
            blend_moby_animation(moby, animation + 2, 0, scale_game_frames(3));
            d->direction = random_angle_radians_7880();
            moby->state = 4;
        }
        break;
    case CONSTRUCTION_ARM_WAIT_TO_START_EXIT_ANIMATION:
        if (FUN_001f9b48(&moby->pos, D_L15_001673C0) < 20.0f &&
            random_integer_below(2) == 0 && moby->visible != 0) {
            speed = random_float_between(D_L15_001619FC / 5.0f, D_L15_001619FC) * frame_time;
            spread = random_float_between(0.0f, D_L15_001619FC / 5.0f) * frame_time;
            direction = random_angle_radians_7880();
            velocity.f[0] = fast_cos(d->direction) * speed;
            velocity.f[1] = fast_sin(d->direction) * speed;
            velocity.f[2] = 0.0f;
            offset.f[0] = fast_cos(direction) * spread;
            offset.f[1] = fast_sin(direction) * spread;
            offset.f[2] = 0.0f;
            /* Retail computes the sum but passes velocity to the effect helper. */
            FUN_L00_001ff290(&sum, &velocity, &offset);
            moby_point_position(moby, 1, &origin);
            first = scale_game_frames(D_L15_001619F4);
            duration = scale_game_frames(D_L15_001619F8);
            duration = FUN_L00_00257b90(first, duration);
            FUN_0025c2f0(40000.0f, &origin, &velocity, D_L15_001619EC, D_L15_001619EC, duration, 1);
        }
        if (fast_dec_timer_s16(&d->timer)) {
            animation = d->side;
            animation *= 2;
            blend_moby_animation(moby, animation + 3, 0, scale_game_frames(6));
            moby->state = 5;
        }
        break;
    case CONSTRUCTION_ARM_WAIT_FOR_EXIT_ANIMATION:
        if (moby->prev_seq == d->side * 2 + 3 && (moby->anim_flags & 2)) {
            minimum = FUN_L15_001f8648(D_L15_00161A00 * 60.0f);
            maximum = FUN_L15_001f8648(D_L15_00161A04 * 60.0f);
            d->timer = truncate_float_to_s32(random_float_between(minimum, maximum));
            duration = scale_game_frames(6);
            blend_moby_animation(moby, 0, 0, duration);
            moby->state = 1;
        }
        break;
    case CONSTRUCTION_ARM_DESTROY:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        cart_burst(1.5f, 13.0f, moby, &moby->pos, -1);
        FUN_L01_002787a0(moby);
        FUN_L00_00263fd8(moby, 0x74b, &moby->pos, &moby->rot, 0, 0, 0.0f,
                         D_L15_0015F580, D_L15_0015F580, D_L15_0015F580);
        FUN_L00_00263fd8(moby, 0x74c, &moby->pos, &moby->rot, 0, 0, 0.0f,
                         D_L15_0015F580, D_L15_0015F580, D_L15_0015F580);
        FUN_L00_00263fd8(moby, 0x74d, &moby->pos, &moby->rot, 0, 0, 0.0f,
                         D_L15_0015F580, D_L15_0015F580, D_L15_0015F580);
        mark_moby_for_removal_7880(moby);
        break;
    }
}
typedef union { u128 q; float f[4]; } SteeringVec_4f88;
typedef struct {
    char pad[0x150];
    SteeringVec_4f88 delta;
    char pad160[0x10];
    float speed;
    float pitch_velocity;
    float yaw_velocity;
    float facing_velocity;
    char pad180[0x28];
    float height;
} SteeringVars_4f88;

extern float FUN_001f9b48(void *, void *);
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9bf8_4f88(float, void *, void *) __asm__("FUN_001f9bf8");
extern void FUN_001f9a68_4f88(float, void *, void *) __asm__("FUN_001f9a68");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void FUN_L15_00247e90(float, float, struct Moby *, void *, int);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");

void FUN_L15_002c4f88(struct Moby *moby, SteeringVec_4f88 *goal, SteeringVec_4f88 *look) {
    SteeringVec_4f88 target;
    SteeringVec_4f88 look_copy;
    SteeringVec_4f88 scaled;
    SteeringVec_4f88 origin;
    SteeringVec_4f88 delta;
    SteeringVec_4f88 *look_at = &look_copy;
    SteeringVec_4f88 *target_at = &target;
    SteeringVars_4f88 *vars = (SteeringVars_4f88 *)moby->pvars;
    float distance;
    float step;
    float old_speed;
    float limit;
    float facing;
    float angle;
    float c;
    float s;
    float yaw_target;
    float pitch_target;
    float dx;
    float dy;
    float threshold;
    float speed_sq;
    float twice_step;
    float height;
    float pos_x;
    float pos_y;


    target.q = goal->q;
    look_copy.q = look->q;
    origin.q = *(u128 *)&moby->pos;
    distance = FUN_001f9b48(&moby->pos, target_at);
    step = frame_time_sq * 10.0f;
    old_speed = vars->speed;
    speed_sq = old_speed * old_speed;
    twice_step = step + step;
    threshold = speed_sq / twice_step;
    if (distance <= threshold) {
        vars->speed = old_speed - step;
        if (vars->speed < 0.0f) {
            vars->speed = 0.0f;
        }
    } else {
        vars->speed = old_speed + step;
        limit = frame_time * 10.0f;
        if (limit < vars->speed) {
            vars->speed = limit;
        }
    }

    subtract_vector_xyz(&delta, target_at, &moby->pos);
    scaled.q = delta.q;
    FUN_001f9bf8_4f88(vars->speed * 0.1f, &scaled, &scaled);
    FUN_001f9a68_4f88(0.9f, &delta, &vars->delta);
    vars->delta.q = delta.q;
    FUN_L00_001ff290(&delta, &vars->delta, &scaled);
    FUN_L15_00247e90(1.0f, 2.0f, moby, &vars->delta, 0);
    /* Preserve the retail height store and facing input access order. */
    height = *(volatile float *)&vars->height;
    pos_x = *(volatile float *)&moby->pos.x;
    *(volatile float *)&moby->pos.z = height;
    pos_y = *(volatile float *)&moby->pos.y;
    dy = *(volatile float *)&look_at->f[1];
    dx = *(volatile float *)&look_at->f[0];
    dy = dy - pos_y;
    dx = dx - pos_x;
    facing = fast_arc_tan(dx, dy);
    moby->rot.z = spring_turn(&vars->facing_velocity, moby->rot.z, facing,
                                   0.01f, 0.3f, 0.1f);
    subtract_vector_xyz(&delta, &moby->pos, &origin);
    vars->delta.q = delta.q;
    angle = fast_arc_tan(vars->delta.f[0], vars->delta.f[1]);
    angle = fast_subtract_rotations(angle, moby->rot.z);
    c = fast_cos(angle);
    yaw_target = (vars->speed * 0.34906584f * c) / (frame_time * 10.0f);
    s = fast_sin(angle);
    pitch_target = (vars->speed * -0.34906584f * s) / (frame_time * 10.0f);
    moby->rot.y = spring_turn(&vars->yaw_velocity, moby->rot.y,
        yaw_target,
        frame_time_sq * 0.5235988f, frame_time_sq * 1.0471976f,
        frame_time * 0.7853982f);
    moby->rot.x = spring_turn(&vars->pitch_velocity, moby->rot.x,
        pitch_target,
        frame_time_sq * 1.0471976f, frame_time_sq * 2.0943952f,
        frame_time * 1.5707964f);
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* A steering goal: the point to reach first, then the rest of the path state. */
typedef struct {
    union {
        OvlQuad q;
        float f[4];
    } pos;
    OvlQuad rest[4];
} Goal_52b8;

extern char *D_L15_0015FFE4;
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void add_vector_xyz_52b8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_52b8(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");

/* Flocking step toward goal (cf. FUN_L15_002cf110): separates from live
 * same-class mobys within 10 units and keeps ~15 units off the goal while
 * another moby is closer to it (~10 otherwise); writes the steering point to
 * out and returns the heading to follow. */
float FUN_L15_002c52b8(struct Moby *m, float *out, Goal_52b8 goal) {
    float tmp[4];
    float dist;
    float wsum;
    int clear;
    float k;
    char *o;
    dist = vector_distance(&m->pos, &goal);
    clear = 1;
    clear_u64_value(out);
    wsum = 0.0f;
    for (o = D_L15_0015FFE4; o != 0; o = *(char **)(o + 0x28)) {
        if (o == m)
            continue;
        if (*(short *)(o + 0xA6) != m->oclass)
            continue;
        if ((u8)o[0x20] == 1 || (u8)o[0x20] == 0x40)
            continue;
        if (vector_distance(o + 0x10, &m->pos) < 10.0f) {
            wsum += 6.0f;
            subtract_vector_xyz(tmp, &m->pos, o + 0x10);
            scale_vector_xyz_52b8(tmp, tmp, 6.0f);
            add_vector_xyz_52b8(out, out, tmp);
        }
        if (vector_distance(o + 0x10, &goal) < dist)
            clear = 0;
    }
    if (clear == 0) {
        subtract_vector_xyz(tmp, &m->pos, &goal);
        if (dist < 14.5f) {
            k = 15.0f;
        } else if (15.5f < dist) {
            k = -15.0f;
        } else {
            k = 0.0f;
        }
        wsum += 10.0f;
        normalize_vector_xyz(tmp, tmp, k * 10.0f);
        add_vector_xyz_52b8(out, out, tmp);
        scale_vector_xyz_52b8(out, out, 1.0f / wsum);
        add_vector_xyz_52b8(out, out, &m->pos);
    } else {
        subtract_vector_xyz(tmp, &m->pos, &goal);
        if (dist < 9.5f) {
            k = 10.0f;
        } else if (10.5f < dist) {
            k = -10.0f;
        } else {
            k = 0.0f;
        }
        wsum += 10.0f;
        normalize_vector_xyz(tmp, tmp, k * 10.0f);
        add_vector_xyz_52b8(out, out, tmp);
        scale_vector_xyz_52b8(out, out, 1.0f / wsum);
        add_vector_xyz_52b8(out, out, &m->pos);
    }
    if (vector_distance(&m->pos, out) < 1.0f) {
        qcopy(out, &m->pos);
        return angle_atan2(goal.pos.f[0] - m->pos.x, goal.pos.f[1] - m->pos.y);
    }
    return angle_atan2(out[0] - m->pos.x, out[1] - m->pos.y);
}
typedef union {
    OvlQuad q;
    float f[4];
} UVec;

typedef struct {
    int n;
    int pad[3];
    UVec pt[1];
} UPath;

typedef struct {
    char pad0[0x454];
    unsigned char collected[1];
} ULevelState;

/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002C6950). */

typedef struct {
    u8 pad0[0x20];
    f32 health;
    s16 unk24;
    u8 pad26[2];
    u8 unk28;
    u8 pad29[5];
    u8 unk2E;
    u8 pad2F;
    f32 unk30;
    u8 pad34[0x24];
    u8 unk58;
    u8 unk59;
    u8 unk5A;
    u8 pad5B[0x75];
    u8 turret_yaw_joint[0x40];
    u8 turret_pitch_joint[0x50];
    struct Moby *pilot;
    struct Moby *shot;
    s16 fire_timer;
    s16 recoil_timer;
    f32 bob_angle;
    f32 speed;
    f32 pitch_velocity;
    f32 yaw_velocity;
    f32 facing_velocity;
    f32 turret_yaw;
    f32 turret_yaw_velocity;
    f32 turret_pitch;
    f32 turret_pitch_velocity;
    s32 wait_group;
    s32 area_path;
    f32 wake_range;
    f32 chase_range;
    f32 range;
    s32 unk1A4;
    f32 height;
    s32 entry_path;
    s32 trigger_volume;
} HelicopterVars;

/* Filled by the target search: the target's position, then its moby at 0x40. */
typedef struct {
    Vec4 pos;
    u8 pad10[0x30];
    struct Moby *target;
    s32 status;
    u8 pad48[8];
} HelicopterGoal;

extern UPath *D_L15_001B0AB0[];
extern ULevelState D_L15_001BABD0;
extern ULevelState D_L15_001BB830;
extern float D_L15_00161A08 __attribute__((sda));
extern float D_L15_00161A0C __attribute__((sda));

extern struct Moby *FUN_L12_00308670(struct Moby *);
extern void FUN_L12_00308708(void *, struct Moby *);
void FUN_L15_002c6900(struct Moby *m);
extern void FUN_L02_00261c80(void *, float *, float);
extern int FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);
extern int FUN_L01_0026e008(int, int);
extern void FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);
extern struct Moby *FUN_L03_002d5208(void *, void *, struct Moby *, int, float);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_001fff28(void *, int, float);
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern void attach_manipulator(void *owner, s32 slot_index, void *manip) __asm__("FUN_0020cb10");
extern void detach_manipulator(void *moby, void *manip) __asm__("FUN_0020cb88");
extern void advance_moby_animation(void *) __asm__("FUN_0020d580");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern f32 fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");

/* Helicopter (moby class 233): waits, flies its path, hovers near and fires at its target, tilts and bobs,
 * aims its turret, and dies. */
void helicopter_update(struct Moby *moby) __asm__("FUN_L15_002c5630");
void helicopter_update(struct Moby *moby) {
    Vec4 previous_pos;
    HelicopterGoal goal;
    Vec4 move;
    Vec4 point;
    Vec4 aim;
    Vec4 target_pos;
    Vec4 turret_pos;
    Vec4 local_dir;
    Vec4 world_dir;
    int segment;
    float fraction;
    struct Moby *target;
    float distance;
    HelicopterVars *vars;
    float length;
    float angle;
    float spin;

    previous_pos.q = *(u128 *)&moby->pos;
    vars = (HelicopterVars *)moby->pvars;
    if (vars->pilot == 0) {
        vars->pilot = FUN_L12_00308670(moby);
    }
    if (moby->state != 0 && moby->state != 0x40 && vars->unk1A4 != 0 && hero.vehicle_mode != 2) {
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        vars->unk58 = 0;
        if (vars->pilot != 0) vars->pilot->flags |= 0x41;
        moby->state = 1;
        return;
    }
    FUN_L15_002c6900(moby);
    goal.status = 0;
    if (moby->state == 1) {
        vars->range = vars->wake_range;
        goto near;
    }
    if (vars->area_path == -1) {
        vars->range = vars->chase_range;
    near:
        FUN_L02_00261c80(moby, goal.pos.f, vars->range);
    } else {
        UPath *path;
        vars->range = vars->chase_range;
        path = D_L15_001B0AB0[vars->area_path];
        FUN_L00_0025ff38(vars->chase_range, moby, &goal, 0, 0, path->pt, path->n);
    }
    if (vars->range < fast_vec_dist(&moby->pos, &goal)) goal.status = 2;
    if (goal.target == 0) goal.target = hero.moby;
    target = goal.target;
    if (moby->state != 0 && vars->unk2E != 1) {
        if (vars->pilot != 0) mark_moby_for_removal(vars->pilot);
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case 0:
        moby->pos.z += 7.5f;
        moby->shadow_range = 0x20;
        vars->height = moby->pos.z;
        vars->health = 3.0f;
        vars->unk24 = truncate_float_to_s32(3.0f);
        vars->unk28 = 2;
        vars->unk58 = 0x19;
        vars->unk30 = 1.0f;
        vars->unk2E = 1;
        vars->unk59 = 1;
        vars->unk5A = 6;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0x80;
        if (vars->entry_path != -1) {
            UPath *path;
            int i;
            path = D_L15_001B0AB0[vars->entry_path];
            for (i = 0; i < path->n - 2; i++) {
                path->pt[i].f[3] = fast_vec_dist(&path->pt[i], &path->pt[i + 1]);
            }
            *(u128 *)&moby->pos = path->pt[0].q;
        }
        moby->state = 1;
        vars->speed = 0.0f;
        vars->pitch_velocity = 0.0f;
        vars->yaw_velocity = 0.0f;
        vars->facing_velocity = 0.0f;
        vars->chase_range = 60.0f;
        attach_manipulator(moby, 0, vars->turret_yaw_joint);
        attach_manipulator(moby, 1, vars->turret_pitch_joint);
        moby->scale *= 0.5f;
        if (vars->pilot != 0) {
            vars->pilot->update_dist = 0;
            vars->pilot->flags &= 0xFFBE;
            vars->pilot->scale *= 0.5f;
        }
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        vars->unk58 = 0;
        if (vars->pilot != 0) vars->pilot->flags |= 0x41;
        break;
    case 1:
        if (vars->unk1A4 != 0 && hero.vehicle_mode != 2) {
            moby->collision = 0;
            moby->flags = (moby->flags | 0x41) & 0xEFFF;
            vars->unk58 = 0;
            if (vars->pilot != 0) vars->pilot->flags |= 0x41;
            return;
        }
        if (vars->wait_group != -1 ||
            (is_point_inside_clip_volume(&hero.motion.pos, vars->trigger_volume) && goal.status != 2)) {
            if (vars->wait_group != -1 && FUN_L01_0026e008(vars->wait_group, 0x40)) break;
            moby->collision = moby->pclass->collision;
            moby->flags = (moby->flags & 0xFFBE) | 0x1000;
            vars->unk58 = 0x19;
            if (vars->entry_path != -1) moby->state = 2; else moby->state = 3;
        } else {
            if (vars->trigger_volume != -1) break;
            if (goal.status == 2) break;
            moby->collision = moby->pclass->collision;
            moby->flags = (moby->flags & 0xFFBE) | 0x1000;
            vars->unk58 = 0x19;
            if (vars->entry_path != -1) moby->state = 2; else moby->state = 3;
        }
        if (vars->pilot != 0) vars->pilot->flags &= 0xFFBE;
        break;
    case 2: {
        UPath *path;
        path = D_L15_001B0AB0[vars->entry_path];
        FUN_L00_0025df68(path, &moby->pos, &point, &segment, &fraction, 0, 999.0f, 5.0f, 0.0f);
        distance = convert_integer_to_float(segment) * path->pt[0].f[3] + fraction + 2.0f;
        if (fast_vec_dist(&moby->pos, &path->pt[path->n - 1]) <=
            vars->speed * vars->speed / (frame_time_sq * 10.0f + frame_time_sq * 10.0f)) {
            vars->speed -= frame_time_sq * 10.0f;
            if (vars->speed < 0.0f) vars->speed = 0.0f;
        } else {
            vars->speed += frame_time_sq * 10.0f;
            if (frame_time * 10.0f < vars->speed) vars->speed = frame_time * 10.0f;
        }
        segment = truncate_float_to_s32(distance / path->pt[0].f[3]);
        fraction = (distance - convert_integer_to_float(segment) * path->pt[0].f[3]) / path->pt[0].f[3];
        if (segment >= path->n - 1) {
            segment = path->n - 2;
            fraction = 1.0f;
        }
        subtract_vector_xyz(&point, &path->pt[segment + 1], &path->pt[segment]);
        fast_vec_scale(&point, &point, fraction);
        add_vector_xyz(&point, &point, &path->pt[segment]);
        if (vector_distance(&path->pt[path->n - 1], &moby->pos) < 1.0f ||
            point_in_path_xy(&moby->pos, D_L15_001B0AB0[vars->area_path]->pt, D_L15_001B0AB0[vars->area_path]->n)) {
            if (goal.status != 2) {
                moby->state = 5;
                vars->fire_timer = 0;
            } else {
                moby->state = 4;
            }
        }
        if (segment + 1 < path->n - 8) {
            angle = fast_arc_tan(path->pt[segment + 1].f[0] - moby->pos.x, path->pt[segment + 1].f[1] - moby->pos.y);
            moby->rot.z = spring_turn(&vars->facing_velocity, moby->rot.z, angle, 0.01f, 0.3f, 0.1f);
        } else {
            angle = fast_arc_tan(goal.pos.f[0] - moby->pos.x, goal.pos.f[1] - moby->pos.y);
            moby->rot.z = spring_turn(&vars->facing_velocity, moby->rot.z, angle, 0.01f, 0.3f, 0.1f);
        }
        subtract_vector_xyz(&move, &point, &moby->pos);
        length = fast_vec_length(&move);
        if (vars->speed < length) length = vars->speed;
        fast_vec_normalize(&move, &move, length);
        add_vector_xyz(&moby->pos, &moby->pos, &move);
        {
            float heading;
            float tilt_y;
            float tilt_x;
            heading = fast_subtract_rotations(fast_arc_tan(move.f[0], move.f[1]), moby->rot.z);
            tilt_y = vars->speed * 0.34906584f * fast_cos(heading) / (frame_time * 10.0f);
            tilt_x = vars->speed * -0.34906584f * fast_sin(heading) / (frame_time * 10.0f);
            moby->rot.y = spring_turn(&vars->yaw_velocity, moby->rot.y, tilt_y, frame_time_sq * 0.5235988f,
                                      frame_time_sq * 1.0471976f, frame_time * 0.7853982f);
            moby->rot.x = spring_turn(&vars->pitch_velocity, moby->rot.x, tilt_x, frame_time_sq * 1.0471976f,
                                      frame_time_sq * 2.0943952f, frame_time * 1.5707964f);
        }
        break;
    }
    case 3: {
        UPath *path;
        int i;
        float best = 9999.999f;
        path = D_L15_001B0AB0[vars->area_path];
        move.q = *(u128 *)&target->pos;
        i = 0;
        if (i < path->n) {
            UVec *p = path->pt;
            UVec *next = p;
            do {
                if (vector_distance(p, &moby->pos) < best) move.q = next->q;
                i++;
                next++;
                p++;
            } while (i < path->n);
        }
        move.f[2] = target->pos.z + 7.5f;
        point.q = *(u128 *)&target->pos;
        FUN_L15_002c4f88(moby, (SteeringVec_4f88 *)&move, (SteeringVec_4f88 *)&point);
        if (vector_distance(&move, &moby->pos) < 1.0f || point_in_path_xy(&moby->pos, path->pt, path->n)) {
            if (goal.status != 2) {
                moby->state = 5;
                vars->fire_timer = 0;
            } else {
                moby->state = 4;
            }
        }
        break;
    }
    case 4: {
        UPath *path;
        path = D_L15_001B0AB0[vars->area_path];
        move.q = *(u128 *)&moby->pos;
        point.q = *(u128 *)&target->pos;
        FUN_L15_002c4f88(moby, (SteeringVec_4f88 *)&move, (SteeringVec_4f88 *)&point);
        if (goal.status == 2) break;
        if (point_in_path_xy(&moby->pos, path->pt, path->n)) {
            moby->state = 5;
            vars->fire_timer = 0;
        } else {
            moby->state = 3;
        }
        break;
    }
    case 5:
        moby_point_position(moby, 0, &point);
        if (goal.status != 2) {
            FUN_L15_002c52b8(moby, move.f, *(Goal_52b8 *)&goal);
            aim.q = *(u128 *)&target->pos;
            FUN_L15_002c4f88(moby, (SteeringVec_4f88 *)&move, (SteeringVec_4f88 *)&aim);
            if (fast_dec_timer_s16(&vars->fire_timer) && vars->shot == 0) {
                vars->fire_timer = scale_game_frames(0x3C);
                qcopy(&target_pos, &goal);
                subtract_vector_xyz(&aim, &target_pos, &point);
                set_vector_length_xy(&aim, &aim, frame_time * 6.0f);
                aim.i[2] = 0;
                vars->shot = FUN_L03_002d5208(&aim, &point, moby, scale_game_frames(0xF0), goal.pos.f[2]);
            }
        } else {
            aim.q = *(u128 *)&moby->pos;
            target_pos.q = *(u128 *)&target->pos;
            FUN_L15_002c4f88(moby, (SteeringVec_4f88 *)&aim, (SteeringVec_4f88 *)&target_pos);
            moby->state = 4;
        }
        break;
    case 6:
        if (fast_dec_timer_s16(&vars->recoil_timer)) {
            moby->state = moby->command;
            break;
        }
        vars->speed -= frame_time_sq * 4.0f;
        if (vars->speed < 0.0f) vars->speed = 0.0f;
        move.f[0] = fast_cos(moby->rot.z) * -vars->speed;
        move.f[1] = fast_sin(moby->rot.z) * -vars->speed;
        move.f[2] = 0.0f;
        add_vector_xyz(&moby->pos, &moby->pos, &move);
        {
            float heading;
            float tilt_y;
            float tilt_x;
            heading = fast_subtract_rotations(fast_arc_tan(move.f[0], move.f[1]), moby->rot.z);
            tilt_y = vars->speed * 0.34906584f * fast_cos(heading) / (frame_time * 10.0f);
            tilt_x = vars->speed * -0.34906584f * fast_sin(heading) / (frame_time * 10.0f);
            moby->rot.y = spring_turn(&vars->yaw_velocity, moby->rot.y, tilt_y, frame_time_sq * 0.5235988f,
                                      frame_time_sq * 1.0471976f, frame_time * 0.7853982f);
            moby->rot.x = spring_turn(&vars->pitch_velocity, moby->rot.x, tilt_x, frame_time_sq * 1.0471976f,
                                      frame_time_sq * 2.0943952f, frame_time * 1.5707964f);
        }
        break;
    case 7: {
        int i;
        move.q = 0;
        spin = random_angle_radians();
        moby_point_position(moby, 4, &point);
        for (i = 0; i < 4; i++) {
            aim.f[0] = fast_cos(spin) * 1.5f;
            aim.f[1] = fast_sin(spin) * 1.5f;
            aim.i[2] = 0;
            spin = fast_add_rotations(spin, 1.5707964f);
            FUN_L00_001ff290(&target_pos, &aim, &point);
        }
        moby_point_position(moby, 5, &aim);
        if (vars->pilot != 0) {
            FUN_L12_00308708(moby, vars->pilot);
            vars->pilot = 0;
        }
        spawn_explosion(moby, &move, &moby->pos, 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, 3.0f, 2,
                        15.0f, 1, 1, -1, 0);
        FUN_L00_00257470(moby, 0x200, -1);
        detach_manipulator(moby, vars->turret_yaw_joint);
        detach_manipulator(moby, vars->turret_pitch_joint);
        moby->state = 0x40;
        return;
    }
    case 0x40:
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        vars->unk58 = 0;
        if (vars->pilot != 0) vars->pilot->flags |= 0x41;
        D_L15_001BABD0.collected[(short)moby->save_id] = 0;
        D_L15_001BB830.collected[(short)moby->save_id] = 0;
        *(int *)(D_0014C190 + (((short)moby->save_id >> 5) * 4 + (current_level_index << 8))) &=
            ~(1 << (moby->save_id & 0x1F));
        D_L15_001BAAD0[(short)moby->save_id >> 5] &= ~(1 << (moby->save_id & 0x1F));
        if (hero.vehicle_mode != 2) {
            moby->pos.z = vars->height - 7.5f;
            moby->scale *= 2.0f;
            moby->state = 0;
        }
        return;
    }
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L15_001673C0) < 40.0f) {
        FUN_L00_0025a120(moby);
    }
    if (moby->state != 1) {
        float climb = absolute_float(moby->pos.z - previous_pos.f[2]);
        float limit = frame_time * 10.0f;
        if (limit < climb) {
            if (previous_pos.f[2] < moby->pos.z) moby->pos.z = previous_pos.f[2] + limit;
            else moby->pos.z = previous_pos.f[2] - limit;
        }
    }
    {
        float z = moby->pos.z - D_L15_00161A0C * fast_cos(vars->bob_angle);
        float period = D_L15_00161A08 * 60.0f;
        moby->pos.z = z;
        vars->bob_angle = fast_add_rotations(vars->bob_angle, 6.2831855f / scale_time(period));
    }
    moby->pos.z += D_L15_00161A0C * fast_cos(vars->bob_angle);
    if (vars->pilot != 0) {
        *(u128 *)&vars->pilot->pos = *(u128 *)&moby->pos;
        *(u128 *)&vars->pilot->rot = *(u128 *)&moby->rot;
        advance_moby_animation(vars->pilot);
        FUN_L00_00250df8(vars->pilot);
        vars->pilot->flags |= 6;
    }
    if (vars->shot == 0 || vars->shot->oclass != 0x350 || vars->shot->state == 0xFE || vars->shot->state == 0xFD) {
        moby_point_position(moby, 0, &turret_pos);
        subtract_vector_xyz(&world_dir, &goal.pos, &turret_pos);
        local_dir.q = world_dir.q;
        transpose_rotation_matrix(&move, &moby->rot_row0);
        transform_vector(&local_dir, &local_dir, &move);
        angle = fast_arc_tan(local_dir.f[0], local_dir.f[1]);
        vars->turret_yaw = spring_turn(&vars->turret_yaw_velocity, vars->turret_yaw, angle, frame_time_sq * 6.2831855f,
                                       frame_time_sq * 12.566371f, frame_time * 6.2831855f);
        angle = fast_arc_tan(vector_length_xy(&local_dir), local_dir.f[2]);
        vars->turret_pitch = spring_turn(&vars->turret_pitch_velocity, vars->turret_pitch, -angle,
                                         frame_time_sq * 3.1415927f, frame_time_sq * 6.2831855f,
                                         frame_time * 1.5707964f);
        FUN_L00_001fff28(vars->turret_yaw_joint + 0x10, 2, vars->turret_yaw);
        FUN_L00_001fff28(vars->turret_pitch_joint + 0x10, 2, vars->turret_pitch);
        vars->shot = 0;
    } else {
        moby_point_position(moby, 0, &turret_pos);
        subtract_vector_xyz(&world_dir, &vars->shot->pos, &turret_pos);
        local_dir.q = world_dir.q;
        transpose_rotation_matrix(&move, &moby->rot_row0);
        transform_vector(&local_dir, &local_dir, &move);
        angle = fast_arc_tan(local_dir.f[0], local_dir.f[1]);
        vars->turret_yaw = spring_turn(&vars->turret_yaw_velocity, vars->turret_yaw, angle, frame_time_sq * 6.2831855f,
                                       frame_time_sq * 12.566371f, frame_time * 6.2831855f);
        angle = fast_arc_tan(vector_length_xy(&local_dir), local_dir.f[2]);
        vars->turret_pitch = spring_turn(&vars->turret_pitch_velocity, vars->turret_pitch, -angle,
                                         frame_time_sq * 3.1415927f, frame_time_sq * 6.2831855f,
                                         frame_time * 1.5707964f);
        FUN_L00_001fff28(vars->turret_yaw_joint + 0x10, 2, vars->turret_yaw);
        FUN_L00_001fff28(vars->turret_pitch_joint + 0x10, 2, vars->turret_pitch);
    }
}
/* Steps a moby's countdown from a helper's output, then aims it at its target. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002C7C20), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern int hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L15_002c6900(struct Moby *m) {
    int st;
    float fv;
    int res;
    char *d;
    char *r;
    float a, b, p, q;
    d = (char *)m->pvars;
    fv = 0.0f;
    r = find_moby_hit(m, 0x330000, 0);
    res = hit_reaction(m, r, d + 0x20, 0, &st, &fv, 0, 4);
    if (st != 1 && m->state != 7 && m->state != 0x40) {
        float t = *(float *)(d + 0x20) - fv;
        *(float *)(d + 0x20) = t;
        if (res == 1 || t <= 0.0f) {
            m->state = 7;
        } else if (res != 0xB) {
            *(unsigned char *)(d + 0x67) = 0xB4;
            trooper_anim_update(m, (short *)(d + 0x60));
            if (*(float *)(r + 0x1C) == 5627.925f && m->state != 6) {
                a = fast_arc_tan(m->pos.x - *(float *)(r + 0),
                                 m->pos.y - *(float *)(r + 4));
                b = fast_subtract_rotations(a, m->rot.z);
                p = fast_cos(b) * 0.08726646f;
                q = fast_sin(b) * -0.34906584f;
                if (absolute_float(m->rot.x) < absolute_float(q)) {
                    m->rot.x = q;
                    *(float *)(d + 0x174) = 0.0f;
                }
                if (absolute_float(m->rot.y) < absolute_float(p)) {
                    m->rot.y = p;
                    *(float *)(d + 0x178) = 0.0f;
                }
                *(float *)(d + 0x170) = frame_time * 4.0f;
                *(short *)(d + 0x16A) = scale_game_frames(0x3C);
                m->command = m->state;
                m->state = 6;
            }
        }
    }
    m->hit_slot = 0xFF;
    trooper_anim_finish(m, d + 0x60);
}
#include "qcopy.h"

/* Flocking step toward the moby's path target: separates from same-type mobys, keeps ~10 units off the
 * target while others are closer, writes the steering point to out and returns the heading to follow. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002D0500), where it is exact; names translated to the US level program. */

extern char *D_L15_0015FFE4;
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void add_vector_xyz(void *, void *, void *);
extern void clear_vector(void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
int FUN_L00_0025fcb8(char *a, char *o, float best);
extern void func_L00_00260D30_t(void *, float *, float) __asm__("FUN_L00_0025fcb8");

float FUN_L15_002cf110(void *m_, void *out, float r) {
    char *m = m_;
    float tmp[4];
    float tgt[4];
    float pad[4][4]; /* unused, but retail reserves the stack space */
    float dist;
    float wsum = 0.0f;
    int clear = 1;
    float k;
    char *o;
    func_L00_00260D30_t(m, tgt, r);
    dist = vector_distance(m + 0x10, tgt);
    clear_u64_value(out);
    for (o = D_L15_0015FFE4; o != 0; o = *(char **)(o + 0x28)) {
        if (o == m)
            continue;
        if (*(short *)(o + 0xA6) != *(short *)(m + 0xA6))
            continue;
        if (vector_distance(o + 0x10, m + 0x10) < 3.0f) {
            wsum += 6.0f;
            subtract_vector_xyz(tmp, m + 0x10, o + 0x10);
            scale_vector_xyz(tmp, tmp, 6.0f);
            add_vector_xyz(out, out, tmp);
        }
        if (vector_distance(o + 0x10, tgt) < dist)
            clear = 0;
    }
    if (clear == 0) {
        subtract_vector_xyz(tmp, m + 0x10, tgt);
        if (dist < 9.5f) {
            k = 10.0f;
        } else if (10.5f < dist) {
            k = -10.0f;
        } else {
            k = 0.0f;
        }
        wsum += 10.0f;
        normalize_vector_xyz(tmp, tmp, k * 10.0f);
        add_vector_xyz(out, out, tmp);
        scale_vector_xyz(out, out, 1.0f / wsum);
        add_vector_xyz(out, out, m + 0x10);
    } else {
        qcopy(out, tgt);
    }
    if (vector_distance(m + 0x10, out) < 1.0f) {
        qcopy(out, m + 0x10);
        return fast_arc_tan(tgt[0] - *(float *)(m + 0x10), tgt[1] - *(float *)(m + 0x14));
    }
    return fast_arc_tan(((float *)out)[0] - *(float *)(m + 0x10),
                        ((float *)out)[1] - *(float *)(m + 0x14));
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Update function of the ultramech (moby class 491): waits for the player, follows its path, turns, fires and strafes, with jet particles while it moves. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002D0798), where it is exact; names translated to the US level program. */




typedef struct UData UData;

typedef struct UMoby UMoby;

struct UMoby {
    char pad0[0x10];
    UVec pos;
    unsigned char state;
    char pad21[3];
    char *cls;
    char pad28[8];
    unsigned char b30;
    char pad31;
    unsigned short h32;
    unsigned short flags;
    char pad36[0xA];
    UVec rot;
    char pad50[3];
    unsigned char anim;
    char pad54[4];
    float animSpeed;
    char pad5C[0x14];
    unsigned char animFlags;
    char pad71[7];
    UData *data;
    char pad7C[0x18];
    int w94;
    char pad98[0x1A];
    unsigned short id;
    char padB4[0xC];
    char mtx[0x40];
};

struct UData {
    char pad0[0x20];
    float f20;
    short h24;
    char pad26[2];
    unsigned char b28;
    unsigned char b29;
    char pad2A[6];
    float f30;
    char pad34[0xA];
    unsigned short h3E;
    UVec vel;
    char pad50[8];
    unsigned char b58;
    char pad59[0x17];
    char hit[0x78];
    float fE8;
    char padEC[0x74];
    UMoby *target;
    int w164;
    char pad168[8];
    int path;
    float f174;
    char pad178[8];
    int paths[2];
    float f188;
    char pad18C[0x14];
    UVec v1A0;
    int side;
    int point;
    float f1B8;
    float turnVel;
    int timer;
    int shots;
    int w1C8;
    int w1CC;
    int w1D0;
    int w1D4;
    int w1D8;
    int w1DC;
    int snd;
    char pad1E4[0x1C];
    unsigned char v200[0x10];
};


typedef struct {
    int idx[2];
} UPaths;

char *FUN_L10_002cc1b8(float scale, void *vel, void *pos, int a, int b, int c);
extern ULevelState D_L15_001BABD0;
extern ULevelState D_L15_001BB830;
extern UPath *D_L15_001B0AB0[];
extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
extern float FUN_L04_001f1978(float);
extern float FUN_001f9b48(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float FUN_L00_0025abf0(void *a, void *b, void *out, float speed, float g);
extern float approach_angle(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");
extern float FUN_L15_002cf110_u(void *, void *, float) __asm__("FUN_L15_002cf110");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float fast_cos(float);
extern float fast_sin(float);
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float vector_length_xyz(void *);
extern int D_L15_001BAAD0[];
extern int coll_line_fix(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int FUN_L00_00257b90(int, int);
extern int FUN_L00_00258490(void *, void *, void *, int, float, float, float, float);
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int sweep_hit_segment(void *a, void *b, void *c, void *d, void *e, void *f, int n) __asm__("FUN_L00_00259d08");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L01_0026e008(int, int);
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int rand(void);
extern int random_integer_below(int) __asm__("FUN_00213260");
extern int scale_game_frames(int);
extern int tick_countdown_32(int *);
extern int truncate_float_to_s32() __asm__("FUN_001fa6d0");
extern float D_L15_0015F580[] __attribute__((section(".sdata")));
extern short D_L15_00161B14 __attribute__((sda));
extern short D_L15_00161B18 __attribute__((sda));
extern short D_L15_00161B20 __attribute__((sda));
extern short D_L15_00161B24 __attribute__((sda));
extern short D_L15_00161B28 __attribute__((sda));
extern short D_L15_00161B34 __attribute__((sda));
extern short D_L15_00161B38 __attribute__((sda));
extern short D_L15_00161B3C __attribute__((sda));
int D_L15_00161B1C = 180;
extern void *FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void FUN_L00_00263e30(void *, int, int, int, int, int, int);
extern void FUN_L00_0026ced0(void *, void *, int, int, float, int);
extern void FUN_L03_00250ae8(void *, void *, int);
extern void FUN_L15_002d0fa8(void *);
extern void no_op_main_callback(void *, void *) __asm__("func_001E93E8");
extern void add_vector_xyz(void *, void *, void *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *, void *, void *);
extern void transform_vector_by_basis(void *, void *, void *);
extern char D_0013E550[];
extern char D_0014C190[];
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern char *func_L10_002CD578_v(void *vel, float scale, void *pos, int a, int b,
                                 int c) __asm__("FUN_L10_002cc1b8");

void ultramech_unlimited_update(UMoby *m) __asm__("FUN_L15_002cf3a8");
void ultramech_unlimited_update(UMoby *m) {
    enum {
        ULTRAMECH_INITIALIZE = 0,
        ULTRAMECH_WAIT_FOR_ACTIVATION = 1,
        ULTRAMECH_FOLLOW_ENTRY_PATH = 2,
        ULTRAMECH_BALLISTIC_PATH_MOVE = 3,
        ULTRAMECH_AIM_AND_SELECT_ATTACK = 4,
        ULTRAMECH_FIRE_BURST = 5,
        ULTRAMECH_WAIT_BETWEEN_BURSTS = 6,
        ULTRAMECH_APPROACH_TARGET = 7,
        ULTRAMECH_MELEE_ATTACK = 8,
        ULTRAMECH_KNOCKBACK_RECOVERY = 9,
        ULTRAMECH_DEATH_EFFECTS = 10,
        ULTRAMECH_DEAD_WAIT_FOR_RESET = 0x40
    };
    UVec vector_or_hit_descriptor;
    UVec position_or_motion_vector;
    UVec vector_arithmetic_result;
    UVec hit_direction;
    UVec attack_start;
    UVec attack_end;
    int seg;
    float frac;
    int moving;
    UData *d;
    UMoby *tgt;
    int idx;

    moving = 0;
    d = m->data;
    FUN_L15_002d0fa8(m);
    tgt = d->target;
    if (m->state != 0 && m->state != 0x40 && hero.vehicle_mode != 2) {
        m->w94 = 0;
        m->flags |= 0x41;
        idx = d->snd;
        if (idx != -1) {
            char *e = (char *)((char *)&D_0013E550) + idx * 0x70;
            if (*(UMoby **)(e + 0x88) == m) {
                if (*(unsigned char *)(e + 0x74) != 0)
                    release_voice_slot(idx);
            }
        }
        d->snd = -1;
        m->flags &= 0xEFFF;
        d->b58 = 0;
        m->state = 1;
        return;
    }
    FUN_L00_00263ac8(2.5f, (int)m, 6, d->v200);
    switch (m->state) {
    case ULTRAMECH_INITIALIZE:
        d->f20 = 12.0f;
        d->h24 = 0xC;
        d->b28 = 3;
        d->h3E |= 2;
        d->f30 = 6.0f;
        d->b29 = 0;
        {
            int ff = 0xFF;
            m->h32 = ff;
            m->b30 = ff;
        }
        d->snd = -1;
        d->turnVel = 0.0f;
        d->f174 = 90.0f;
        if (rand() & 1)
            m->flags |= 0x8000;
        m->w94 = 0;
        m->flags = (m->flags | 0x41) & 0xEFFF;
        if (d->paths[0] == -1 || d->paths[1] == -1)
            d->w1D4 = 1;
        if (d->path != -1)
            m->pos.q = D_L15_001B0AB0[d->path]->pt[0].q;
        m->state = 1;
        if (m->anim != 0)
            blend_moby_animation(m, 0, 0, 5);
        break;
    case ULTRAMECH_WAIT_FOR_ACTIVATION:
        if (d->w1CC != -1 || is_point_inside_clip_volume(&hero.motion.pos, d->w1DC)) {
            if (d->w1CC != -1 && FUN_L01_0026e008(d->w1CC, 0x40))
                break;
            if (hero.vehicle_mode == 2) {
                m->w94 = *(int *)(m->cls + 0x10);
                m->flags = (m->flags & 0xFFBE) | 0x1000;
            }
        }
        if (d->w164 != 2 && d->path != -1) {
            struct Hero *pl = &hero;
            if (pl->vehicle_mode == 2 && (m->flags & 0x1000)) {
                if (d->w1D0 != -1 && !point_in_path_xy(&pl->motion.pos, D_L15_001B0AB0[d->w1D0]->pt,
                                                       D_L15_001B0AB0[d->w1D0]->n)) {
                    m->w94 = 0;
                    m->flags = (m->flags | 0x41) & 0xEFFF;
                } else {
                    UPath *path;
                    int i;
                    m->state = 2;
                    if (m->anim != 4)
                        blend_moby_animation(m, 4, 0, scale_game_frames(0x14));
                    path = D_L15_001B0AB0[d->path];
                    for (i = 0; i < path->n - 2; i++) {
                        path->pt[i].f[3] = FUN_001f9b48(&path->pt[i], &path->pt[i + 1]);
                    }
                    m->pos.q = path->pt[0].q;
                    subtract_vector_xyz(&d->vel, (UVec *)path + path->n, &m->pos);
                    set_vector_length_xy(&d->vel, &d->vel, *(float *)&D_L15_00161B14 * frame_time);
                    d->vel.f[2] = FUN_L00_0025abf0(&m->pos, (UVec *)path + path->n, 0,
                                                   *(float *)&D_L15_00161B14 * frame_time,
                                                   -(*(float *)&D_L15_00161B18 * frame_time_sq));
                }
            }
        }
        break;
    case ULTRAMECH_FOLLOW_ENTRY_PATH: {
        UPath *path = D_L15_001B0AB0[d->path];
        float speed = frame_time * 12.0f;
        float dist;
        float len;
        FUN_L00_0025df68(path, &m->pos, &position_or_motion_vector, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
        dist = convert_integer_to_float(seg) * path->pt[0].f[3];
        dist += frac;
        dist += 2.0f;
        seg = func_001FA898_r(dist / path->pt[0].f[3]);
        dist -= convert_integer_to_float(seg) * path->pt[0].f[3];
        dist /= path->pt[0].f[3];
        frac = dist;
        if (seg >= path->n - 1) {
            float z = probe_ground_height(&m->pos, 0, 0.5f);
            if (m->pos.f[2] < z)
                m->pos.f[2] = z;
            m->state = 4;
            d->timer = scale_game_frames(0xF);
            break;
        }
        dist = FUN_001f9b48(&m->pos, (UVec *)path + path->n);
        if (dist < speed / FUN_L04_001f1978(26.0f) && m->anim != 5) {
            blend_moby_animation(m, 5, 0, scale_game_frames(6));
        }
        subtract_vector_xyz(&position_or_motion_vector, (UVec *)path + (seg + 2), (UVec *)path + (seg + 1));
        scale_vector_xyz(&position_or_motion_vector, &position_or_motion_vector, frac);
        add_vector_xyz(&position_or_motion_vector, &position_or_motion_vector, (UVec *)path + (seg + 1));
        if (seg + 1 < path->n - 8) {
            float ang = fast_arc_tan(path->pt[seg + 1].f[0] - m->pos.f[0],
                                     path->pt[seg + 1].f[1] - m->pos.f[1]);
            m->rot.f[2] = spring_turn(&d->turnVel, m->rot.f[2], ang, 0.01f, 0.3f, 0.1f);
        }
        subtract_vector_xyz(&vector_or_hit_descriptor, &position_or_motion_vector, &m->pos);
        len = vector_length_xyz(&vector_or_hit_descriptor);
        if (speed < len)
            len = speed;
        moving = 1;
        normalize_vector_xyz(&vector_or_hit_descriptor, &vector_or_hit_descriptor, len);
        add_vector_xyz(&m->pos, &m->pos, &vector_or_hit_descriptor);
        break;
    }
    case ULTRAMECH_BALLISTIC_PATH_MOVE:
        if (m->anim == 3 && (m->animFlags & 2))
            blend_moby_animation(m, 4, 0, scale_game_frames(6));
        add_vector_xyz(&m->pos, &m->pos, &d->vel);
        d->vel.f[2] -= *(float *)&D_L15_00161B18 * frame_time_sq;
        if (d->vel.f[2] < 0.0f) {
            float z;
            qcopy(&vector_or_hit_descriptor, &m->pos);
            vector_or_hit_descriptor.f[2] += 1.0f;
            z = probe_ground_height(&vector_or_hit_descriptor, 0, 0.5f);
            if (m->pos.f[2] - z < d->vel.f[2] * FUN_L04_001f1978(26.0f) +
                                      *(float *)&D_L15_00161B18 * frame_time_sq * 0.5f *
                                          FUN_L04_001f1978(26.0f) * FUN_L04_001f1978(26.0f) &&
                m->anim != 5) {
                blend_moby_animation(m, 5, 0, scale_game_frames(6));
            }
            if (m->pos.f[2] < z) {
                m->pos.f[2] = z;
                m->state = 4;
                d->timer = scale_game_frames(0xF);
            }
        }
        moving = 1;
        break;
    case ULTRAMECH_AIM_AND_SELECT_ATTACK:
        if (m->anim != 0 && (m->animFlags & 2))
            blend_moby_animation(m, 0, 0, scale_game_frames(0x14));
        if (d->w164 != 2) {
            approach_angle(&m->rot.f[2],
                             fast_arc_tan(tgt->pos.f[0] - m->pos.f[0], tgt->pos.f[1] - m->pos.f[1]),
                             &d->turnVel, frame_time_sq * 8.726646f, frame_time_sq * 8.726646f,
                             frame_time * 12.566371f);
            if (tick_countdown_32(&d->timer)) {
                if (d->w1D8 == 0 && vector_distance(&m->pos, &tgt->pos) < *(float *)&D_L15_00161B24) {
                    m->state = 7;
                    if (m->anim != 1)
                        blend_moby_animation(m, 1, 0, scale_game_frames(0xA));
                } else if (d->w1D8 == 0 &&
                           vector_distance(&m->pos, &tgt->pos) < *(float *)&D_L15_00161B28) {
                    m->state = 8;
                    if (m->anim != 2)
                        blend_moby_animation(m, 2, 0, scale_game_frames(0xA));
                } else {
                    m->state = 5;
                    if (m->anim != 6)
                        blend_moby_animation(m, 6, 0, scale_game_frames(0x14));
                    d->shots = 0;
                }
            }
        }
        break;
    case ULTRAMECH_FIRE_BURST:
        if (m->anim == 6 && (m->animFlags & 2)) {
            blend_moby_animation(m, 7, 0, scale_game_frames(5));
            m->animSpeed = *(float *)&D_L15_00161B34;
        }
        if (d->w164 != 2) {
            int fire = -1;
            approach_angle(&m->rot.f[2],
                             fast_arc_tan(tgt->pos.f[0] - m->pos.f[0], tgt->pos.f[1] - m->pos.f[1]),
                             &d->turnVel, frame_time_sq * 8.726646f, frame_time_sq * 8.726646f,
                             frame_time * 12.566371f);
            if (m->anim == 7) {
                if (is_value_within_interpolated_window((char *)m, 0.0f))
                    fire = 1;
            }
            if (fire >= 0) {
                moby_point_position(m, 0, &vector_or_hit_descriptor);
                subtract_vector_xyz(&position_or_motion_vector, &tgt->pos, &vector_or_hit_descriptor);
                position_or_motion_vector.f[2] += 0.5f;
                normalize_vector_xyz(&position_or_motion_vector, &position_or_motion_vector, *(float *)&D_L15_00161B20 * frame_time);
                if (0.0f < position_or_motion_vector.f[2])
                    position_or_motion_vector.f[2] = 0.0f;
                if (position_or_motion_vector.f[2] < 0.0f)
                    position_or_motion_vector.f[2] = 0.0f;
                func_L10_002CD578_v(&position_or_motion_vector, 3.0f, &vector_or_hit_descriptor, (int)m, (int)tgt,
                                    scale_game_frames(D_L15_00161B1C));
                normalize_vector_xyz(&position_or_motion_vector, &position_or_motion_vector, 3.0f);
                FUN_L00_001ff290(&vector_arithmetic_result, &vector_or_hit_descriptor, &position_or_motion_vector);
                FUN_L03_00250ae8(m, &vector_or_hit_descriptor, 0);
                d->shots++;
            } else if ((float)d->shots >= 6.0f && (m->animFlags & 2)) {
                m->state = 6;
                if (m->anim != 0)
                    blend_moby_animation(m, 0, 0, scale_game_frames(0xA));
                d->timer = func_001FA898_r(FUN_L04_001f1978(d->f188 * 60.0f));
            } else if (d->w1D8 == 0 &&
                       vector_distance(&m->pos, &tgt->pos) < *(float *)&D_L15_00161B24) {
                m->state = 7;
                if (m->anim != 1)
                    blend_moby_animation(m, 1, 0, scale_game_frames(0xA));
            } else if (d->w1D8 == 0 &&
                       vector_distance(&m->pos, &tgt->pos) < *(float *)&D_L15_00161B28) {
                m->state = 8;
                if (m->anim != 2)
                    blend_moby_animation(m, 2, 0, scale_game_frames(0xA));
            } else if (d->w1D8 == 0 && vector_distance(&m->pos, &tgt->pos) > 10.0f && d->w1C8 != 0 &&
                       d->w1D4 == 0) {
                m->state = 3;
                d->timer = 0;
                d->side = (d->side + 1) & 1;
                if (m->anim != 3)
                    blend_moby_animation(m, 3, 0, scale_game_frames(0x14));
                subtract_vector_xyz(
                    &d->vel,
                    (UVec *)D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]] +
                        (d->point + 1),
                    &m->pos);
                set_vector_length_xy(&d->vel, &d->vel, *(float *)&D_L15_00161B14 * frame_time);
                d->vel.f[2] = FUN_L00_0025abf0(
                    &m->pos,
                    (UVec *)D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]] +
                        (d->point + 1),
                    0, *(float *)&D_L15_00161B14 * frame_time,
                    -(*(float *)&D_L15_00161B18 * frame_time_sq));
            }
        } else {
            m->state = 4;
            if (m->anim != 0)
                blend_moby_animation(m, 0, 0, scale_game_frames(0x14));
        }
        break;
    case ULTRAMECH_WAIT_BETWEEN_BURSTS:
        if (d->w164 != 2) {
            approach_angle(&m->rot.f[2],
                             fast_arc_tan(tgt->pos.f[0] - m->pos.f[0], tgt->pos.f[1] - m->pos.f[1]),
                             &d->turnVel, frame_time_sq * 8.726646f, frame_time_sq * 8.726646f,
                             frame_time * 12.566371f);
            if (d->w1D8 == 0 && vector_distance(&m->pos, &tgt->pos) < 8.0f && d->w1D4 == 0 &&
                d->point < D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]]->n - 1) {
                m->state = 3;
                d->timer = 0;
                d->point++;
                if (m->anim != 3)
                    blend_moby_animation(m, 3, 0, scale_game_frames(0x14));
                subtract_vector_xyz(
                    &d->vel,
                    (UVec *)D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]] +
                        (d->point + 1),
                    &m->pos);
                set_vector_length_xy(&d->vel, &d->vel, *(float *)&D_L15_00161B14 * frame_time);
                d->vel.f[2] = FUN_L00_0025abf0(
                    &m->pos,
                    (UVec *)D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]] +
                        (d->point + 1),
                    0, *(float *)&D_L15_00161B14 * frame_time,
                    -(*(float *)&D_L15_00161B18 * frame_time_sq));
            } else if (tick_countdown_32(&d->timer)) {
                m->state = 5;
                if (m->anim != 6)
                    blend_moby_animation(m, 6, 0, scale_game_frames(0x14));
                d->shots = 0;
            }
        } else {
            m->state = 4;
            if (m->anim != 0)
                blend_moby_animation(m, 0, 0, scale_game_frames(0x14));
        }
        break;
    case ULTRAMECH_APPROACH_TARGET: {
        float ang = fast_arc_tan(tgt->pos.f[0] - m->pos.f[0], tgt->pos.f[1] - m->pos.f[1]);
        if (d->w164 != 2) {
            ang = FUN_L15_002cf110_u(m, &position_or_motion_vector, d->f1B8);
            if (vector_distance(&m->pos, &tgt->pos) < *(float *)&D_L15_00161B28) {
                m->state = 8;
                if (m->anim != 2)
                    blend_moby_animation(m, 2, 0, scale_game_frames(0xA));
            }
        } else {
            m->state = 4;
            if (m->anim != 0)
                blend_moby_animation(m, 0, 0, scale_game_frames(0x14));
        }
        FUN_L00_00258278((char *)m, &d->turnVel, ang, frame_time_sq * 1.0471976f,
                         frame_time_sq * 3.1415927f, frame_time * 3.1415927f);
        qcopy(&vector_or_hit_descriptor, &m->pos);
        subtract_vector_xyz(&vector_arithmetic_result, &position_or_motion_vector, &m->pos);
        position_or_motion_vector.q = vector_arithmetic_result.q;
        normalize_vector_xyz(&position_or_motion_vector, &position_or_motion_vector, frame_time * 6.5f);
        FUN_L00_001ff290(&vector_arithmetic_result, &position_or_motion_vector, &m->pos);
        if (!FUN_L00_00258490(m, &vector_or_hit_descriptor, &position_or_motion_vector, 1, 2.25f, 1.75f, 0.5f, 0.5235988f) ||
            point_in_path_xy(&vector_or_hit_descriptor, D_L15_001B0AB0[d->side]->pt, D_L15_001B0AB0[d->side]->n)) {
            qcopy(&m->pos, &vector_or_hit_descriptor);
            m->state = 4;
            if (m->anim != 0)
                blend_moby_animation(m, 0, 0, scale_game_frames(0xC));
        } else {
            m->pos.q = vector_or_hit_descriptor.q;
        }
        break;
    }
    case ULTRAMECH_MELEE_ATTACK:
        approach_angle(
            &m->rot.f[2], fast_arc_tan(tgt->pos.f[0] - m->pos.f[0], tgt->pos.f[1] - m->pos.f[1]),
            &d->turnVel, frame_time_sq * 8.726646f, frame_time_sq * 8.726646f, frame_time * 12.566371f);
        if (m->animFlags & 2) {
            if (vector_distance(&m->pos, &tgt->pos) < *(float *)&D_L15_00161B28 && d->w1D4 == 0 &&
                d->point < D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]]->n - 1) {
                m->state = 3;
                d->timer = 0;
                d->point++;
                if (m->anim != 3)
                    blend_moby_animation(m, 3, 0, scale_game_frames(0x14));
                subtract_vector_xyz(
                    &d->vel,
                    (UVec *)D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]] +
                        (d->point + 1),
                    &m->pos);
                set_vector_length_xy(&d->vel, &d->vel, *(float *)&D_L15_00161B14 * frame_time);
                d->vel.f[2] = FUN_L00_0025abf0(
                    &m->pos,
                    (UVec *)D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]] +
                        (d->point + 1),
                    0, *(float *)&D_L15_00161B14 * frame_time,
                    -(*(float *)&D_L15_00161B18 * frame_time_sq));
            } else if (*(float *)&D_L15_00161B28 < vector_distance(&m->pos, &tgt->pos)) {
                m->state = 4;
                if (m->anim != 0)
                    blend_moby_animation(m, 0, 0, scale_game_frames(0x14));
            }
        } else {
            float h = compute_interpolated_record_value(m);
            moby_point_position(m, 1, &attack_end);
            if (h >= 17.0f && h <= 24.0f) {
                qcopy(&attack_start, &m->pos);
                attack_start.f[2] += 0.5f;
                hit_direction.f[0] = fast_cos(m->rot.f[2]);
                hit_direction.f[1] = fast_sin(m->rot.f[2]);
                hit_direction.f[2] = 1.0f;
                make_hit_descriptor(&vector_or_hit_descriptor, m, 0x10007, &hit_direction, 4.0f);
                collect_mobies_in_sphere(&attack_end, 9, m, &vector_or_hit_descriptor, 2.5f);
                coll_line_fix(&attack_start, &attack_end, 0, m, &vector_or_hit_descriptor);
                sweep_hit_segment(&attack_start, &attack_end, &attack_start, &d->v1A0, m, &vector_or_hit_descriptor, 5);
                no_op_main_callback(&attack_start, &attack_end);
                qcopy(&d->v1A0, &attack_end);
            }
        }
        break;
    case ULTRAMECH_KNOCKBACK_RECOVERY: {
        int r = 0;
        if (d->w1D8 == 0) {
            r = mine_fall(m, d->hit);
        } else {
            m->animSpeed = 1.0f;
        }
        if ((r & 0x40) || (d->w1D8 != 0 && (m->animFlags & 2))) {
            if (d->w1D4 == 0) {
                m->state = 3;
                d->timer = 0;
                d->side = (d->side + 1) & 1;
                qcopy(&vector_or_hit_descriptor, (UVec *)D_L15_001B0AB0[((UPaths *)((char *)d + 0x180))->idx[d->side]] +
                                d->point + 1);
                vector_or_hit_descriptor.f[2] = probe_ground_height(&vector_or_hit_descriptor, 0, 0.5f);
                if (m->anim != 3)
                    blend_moby_animation(m, 3, 0, scale_game_frames(0x14));
                subtract_vector_xyz(&d->vel, &vector_or_hit_descriptor, &m->pos);
                set_vector_length_xy(&d->vel, &d->vel, *(float *)&D_L15_00161B14 * frame_time);
                d->vel.f[2] =
                    FUN_L00_0025abf0(&m->pos, &vector_or_hit_descriptor, 0, *(float *)&D_L15_00161B14 * frame_time,
                                     -(*(float *)&D_L15_00161B18 * frame_time_sq));
            } else {
                m->state = 4;
                if (m->anim != 0)
                    blend_moby_animation(m, 0, 0, scale_game_frames(0x14));
            }
        }
        break;
    }
    case ULTRAMECH_DEATH_EFFECTS: {
        void *pp = &m->pos;
        void *rr = &m->rot;
        float *dd;
        scale_vector_xyz(&vector_or_hit_descriptor, m->mtx,
                         *(float *)&D_L15_00161B38 * (dd = D_L15_0015F580, frame_scale));
        vector_or_hit_descriptor.f[2] += *(float *)&D_L15_00161B3C * frame_scale;
        FUN_L00_00263fd8(m, 0x64A, pp, rr, scale_game_frames(0x5A), 0, frame_time_sq * 12.0f, &vector_or_hit_descriptor, dd,
                         dd);
        FUN_L00_00263fd8(m, 0x64B, pp, rr, scale_game_frames(0x5A), 0, frame_time_sq * 12.0f, &vector_or_hit_descriptor, dd,
                         dd);
        FUN_L00_00263fd8(m, 0x64C, pp, rr, 0, 0, 0.0f, dd, dd, D_L15_0015F580);
    }
        FUN_L00_00263e30(m, 0x7A9, 1, 0x7A9, 2, 4, 2);
        m->state = 0x40;
        return;
    case ULTRAMECH_DEAD_WAIT_FOR_RESET:
        m->w94 = 0;
        m->flags = (m->flags | 0x41) & 0xEFFF;
        d->b58 = 0;
        D_L15_001BABD0.collected[(short)m->id] = 0;
        D_L15_001BB830.collected[(short)m->id] = 0;
        *(int *)(((char *)&D_0014C190) + (((short)m->id >> 5) * 4 + (current_level_index << 8))) &=
            ~(1 << (m->id & 0x1F));
        D_L15_001BAAD0[(short)m->id >> 5] &= ~(1 << (m->id & 0x1F));
        if (hero.vehicle_mode != 2)
            m->state = 0;
        break;
    }
    if (moving) {
        if (voice_is_playing(m, d->snd))
            goto spawn;
        d->snd = allocate_voice_for_target_entry(7, 4, (int)m);
    } else {
        idx = d->snd;
        if (idx != -1) {
            char *e = (char *)((char *)&D_0013E550) + idx * 0x70;
            if (*(UMoby **)(e + 0x88) == m) {
                if (*(unsigned char *)(e + 0x74) != 0)
                    release_voice_slot(idx);
            }
        }
        d->snd = -1;
    }
    if (moving) {
        int i;
        int j;
        int n;
    spawn:
        for (i = 0; i < 10; i = n) {
            n = i + 1;
            for (j = 2; j < 6; j++) {
                int k;
                position_or_motion_vector.q = 0;
                position_or_motion_vector.f[0] = random_float_between(1.0f, -1.0f) * (frame_time * 20.0f);
                position_or_motion_vector.f[1] = random_float_between(1.0f, -1.0f) * (frame_time * 20.0f);
                position_or_motion_vector.f[2] = random_float_between(0.8f, 1.2f) * (frame_time * -20.0f);
                vector_or_hit_descriptor.q = position_or_motion_vector.q;
                k = random_integer_below(100) & 1;
                moby_point_position(m, j, &position_or_motion_vector);
                transform_vector_by_basis(&vector_or_hit_descriptor, &vector_or_hit_descriptor, m->mtx);
                FUN_L00_0026ced0(&position_or_motion_vector, &vector_or_hit_descriptor, k ? 0x400F0F7F : 0x407F7F7F, 0x272727,
                                 random_float_between(1.5f, 3.0f) * 210000.0f,
                                 scale_game_frames(k ? FUN_L00_00257b90(0xA, 0xF)
                                                     : FUN_L00_00257b90(0x14, 0x1E)));
            }
        }
    } else if (m->state > 1 && m->state != 0x40) {
        float z;
        d->fE8 += frame_time_sq * 30.0f;
        m->pos.f[2] -= d->fE8 - 2.0f;
        z = probe_ground_height(&m->pos, 0, 0.5f);
        m->pos.f[2] -= 2.0f;
        if (m->pos.f[2] < z) {
            m->pos.f[2] = z;
            d->fE8 = 0.0f;
        }
        play_entity_sound((int)m, 2.5f, &m->pos, 0x10000, 40.0f, 1.0f, 0, 1, 0);
    }
}
extern float D_L15_00161B00 __attribute__((sda));
extern float D_L15_00161B04 __attribute__((sda));
extern float D_L15_00161B08 __attribute__((sda));
extern float D_L15_00161B2C __attribute__((sda));
extern float D_L15_00161B30 __attribute__((sda));
extern float D_0015ED70;
extern int D_001413D0 __attribute__((section(".data")));
extern char D_00178700[] __asm__("D_L15_00178700");
extern int FUN_00249d60(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int FUN_001f99c8_f(float) __asm__("FUN_001fa6d0");
extern void FUN_0023ee30(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_0024dcf8(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int FUN_0027bb68(int, int, void *, int) __asm__("FUN_L00_0028dc90");
extern void FUN_00246ad8(void *, int, long long) __asm__("FUN_L00_00257470");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void FUN_0024a430(void *, void *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void FUN_0024be00(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void FUN_00251c90(void *, float, float) __asm__("FUN_L03_00250a78");
extern void FUN_0024cd00(void *, void *) __asm__("FUN_L00_0025d458");
extern void FUN_0024cde0(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_0024f560(float, void *, void *) __asm__("FUN_L02_00261c80");
extern void FUN_0024f7e0(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern float FUN_001f8bc8(void *, void *) __asm__("FUN_001f9b48");

void FUN_L15_002d0fa8(void *arg) {
    enum {
        HIT_REACTION_NO_ACTION = 0,
        HIT_REACTION_DEATH = 1,
        HIT_REACTION_DEATH_ALTERNATE = 2,
        HIT_REACTION_KNOCKBACK = 3,
        HIT_REACTION_KNOCKBACK_ALTERNATE = 4,
        HIT_REACTION_DIRECTIONAL_KNOCKBACK = 5,
        HIT_REACTION_DIRECTIONAL_KNOCKBACK_ALTERNATE = 6,
        HIT_REACTION_DAMAGE_WITHOUT_KNOCKBACK = 9,
        HIT_REACTION_DAMAGE_WITHOUT_KNOCKBACK_ALTERNATE = 10,
        HIT_REACTION_NONE = 11
    };
    UMoby *m = arg;
    UData *d = m->data;
    char *slot = 0;
    int kind;
    float impact;
    int i;
    int hit;
    int code;
    float angle;
    UVec origin;
    UVec effect;
    float direction;
    float reaction_product;
    unsigned char state;

    *(float *)((char *)m + 0x2C) = D_L15_00161B2C * *(float *)(m->cls + 0x24);
    impact = 0.0f;
    d->w1C8 = 0;
    for (i = 0; i < 64; i++) {
        char *entry = D_00178700 + i * 0x40;
        if (*(UMoby **)(entry + 0x34) == m) {
            unsigned int flags = *(unsigned int *)(entry + 0x24);
            if (flags & 0xA0000) {
                if (*(int *)(entry + 0x20) == D_001413D0) {
                    if (!(flags & 0x80000)) {
                        *(int *)(entry + 0x34) = 0;
                        continue;
                    }
                }
                slot = entry;
                break;
            }
        }
    }
    code = FUN_00249d60(m, slot, &d->f20, 0, &kind, &impact, 0, 4);
    if (slot != 0) {
        if (kind != 1 && (state = m->state) != 10 && state != 0x40 &&
            *(short *)(*(char **)(slot + 0x20) + 0xA6) != 0x350) {
            d->f20 -= impact;
            if (*(unsigned short *)(slot + 0x2A) == 0x100)
                code = 9;
            if (m->state == 2)
                code = 9;
            if (d->f20 <= 0.0f)
                code = 1;
            hit = FUN_001f99c8_f(2560.0f);
            {
                float angle;
                angle = D_L15_00161B00 * D_0015ED70;
                *(int *)((char *)d + 0x90) = hit;
                *(float *)((char *)d + 0x98) = 2.5f;
                *(float *)((char *)d + 0x80) = angle;
                *(float *)((char *)d + 0x84) = 0.0005f;
                *(int *)((char *)d + 0x94) = 9;
                *(unsigned char *)((char *)d + 0xAD) = 0;
            }
            switch (code) {
            case HIT_REACTION_NO_ACTION:
            case HIT_REACTION_NONE:
                break;
            case HIT_REACTION_DAMAGE_WITHOUT_KNOCKBACK:
            case HIT_REACTION_DAMAGE_WITHOUT_KNOCKBACK_ALTERNATE:
                d->w1C8 = 1;
                *(unsigned char *)((char *)d + 0x67) = 0xFA;
                break;
            case HIT_REACTION_KNOCKBACK:
            case HIT_REACTION_KNOCKBACK_ALTERNATE:
            case HIT_REACTION_DIRECTIONAL_KNOCKBACK:
            case HIT_REACTION_DIRECTIONAL_KNOCKBACK_ALTERNATE:
            case 7:
            case 8:
                angle = D_L15_00161B08 * D_0015ED6C;
                reaction_product = D_L15_00161B04 * D_0015ED6C;
                d->w1C8 = 1;
                *(float *)((char *)d + 0x88) = angle;
                *(float *)((char *)d + 0x8C) = reaction_product;
                FUN_00251c90((char *)d + 0x70, 4.0f, 1.0f);
                *(float *)((char *)d + 0xC0) = 12.0f;
                *(float *)((char *)d + 0xC4) = 16.0f;
                angle = fast_arc_tan(m->pos.f[0] - *(float *)(*(char **)(slot + 0x20) + 0x10),
                                          m->pos.f[1] - *(float *)(*(char **)(slot + 0x20) + 0x14));
                origin.q = *(u128 *)(slot + 0x10);
                direction = angle;
                FUN_0024a430(&origin, &direction, (char *)d + 0x88, (char *)d + 0x8C);
                FUN_0024be00(direction, m, (char *)d + 0x70, 8, 1, 0);
                FUN_0027bb68(0, 0, m, 0x79);
                m->state = 9;
                *(unsigned char *)((char *)d + 0x67) = 0x78;
                break;
            case HIT_REACTION_DEATH:
            case HIT_REACTION_DEATH_ALTERNATE:
                origin.q = 0;
                m->flags &= 0xEFFF;
                FUN_0023ee30(m, 7, &effect);
                hit = -1;
                FUN_0024dcf8(m, &origin, &effect, 0.0f, 0.0f, 0x14, 6, 0x20,
                              16.0f, 8.0f, 9.0f, -1, 4.0f, 32.0f, 1, 16, hit, 0);
                FUN_0027bb68(0, 0, m, 0x79);
                m->state = 10;
                *(unsigned char *)((char *)d + 0x67) = 0xF0;
                FUN_00246ad8(m, 0x200, -1);
                break;
            }
            FUN_0024cd00(m, (char *)d + 0x60);
        }
        if (slot != 0)
            *(int *)(slot + 0x34) = 0;
    }
    *(unsigned char *)((char *)m + 0xA4) = 0xFF;
    FUN_0024cde0(m, (char *)d + 0x60);
    *(float *)((char *)m + 0x2C) = *(float *)(m->cls + 0x24) * D_L15_00161B30;
    d->w164 = 0;
    if (m->state != 1) {
        if (d->w1D0 != -1)
            goto follow_path;
        d->f1B8 = *(float *)((char *)d + 0x178);
    } else {
        d->f1B8 = d->f174;
    }
    FUN_0024f560(d->f1B8, m, (char *)d + 0x120);
    goto check_path;
follow_path:
    {
        UPath *path;
        d->f1B8 = *(float *)((char *)d + 0x178);
        path = D_L15_001B0AB0[d->w1D0];
        FUN_0024f7e0(d->f1B8, m, (char *)d + 0x120, 0, 0, &path->pt[0], path->n);
    }
check_path:
    if (d->f1B8 < FUN_001f8bc8(&m->pos, (char *)d + 0x120))
        d->w164 = 2;
    if (d->target == 0)
        d->target = (UMoby *)D_001413D0;
}

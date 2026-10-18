/* Level 10 moby classes: bombthrower_grass, platform, magnetboots, gadgebot_telepot_pad, gate, bomb, lab_door, button, block_cave. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/audio/sound_state.h"
#include "rnc/overlay/level_globals.h"

#define NOT_SDA

#define MACRO_ADDR

/* update: on a timer, fire a projectile from the moby's angle, then rearm the timer */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002D9FC0), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_001f9740(int *arg0);
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L10_002d8058_u() __asm__("FUN_L10_002d8058");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

int D_L10_00161BF0 = 240;
int D_L10_00161BF4 = 600;
extern float D_L10_00161BF8 __attribute__((sda));
extern float D_L10_00161BFC __attribute__((sda));
extern float D_L10_00161C00 __attribute__((sda));
extern float D_L10_00161C04 __attribute__((sda));
extern float D_L10_00161C08 __attribute__((sda));
void bombthrower_grass_update(struct Moby *moby) __asm__("FUN_L10_002d8c00");
void bombthrower_grass_update(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float a[4];
    float b[4];
    if (moby->state == 0) {
        *(int *)data =
            scale_game_frames(random_int_between(D_L10_00161BF0, D_L10_00161BF4));
        moby->state = 1;
    }
    if (FUN_001f9740((int *)data)) {
        float x = D_L10_00161BF8;
        float p = random_float_between_alt(-x, x) * DEG_TO_RAD;
        float q = fast_add_rotations(moby->rot.z, p);
        float r =
            random_float_between_alt(D_L10_00161BFC, D_L10_00161C00) *
            DEG_TO_RAD;
        build_spherical_offset(
            a,
            random_float_between_alt(D_L10_00161C04, D_L10_00161C08) *
                frame_time,
            q, r);
        fast_vec_add(b, &moby->pos, a);
        FUN_L10_002d8058_u(moby, b, a);
        allocate_voice_for_target_entry(0, 0, (int)moby);
        *(int *)data =
            scale_game_frames(random_int_between(D_L10_00161BF0, D_L10_00161BF4));
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
extern void *create_moby() __asm__("FUN_0020c4f8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DEC08), where it is exact; names translated to the US level program. */

extern void FUN_L00_00250df8(void *);

char *FUN_L10_002dd848(struct Moby *owner) {
    char *moby = create_moby(0x462);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        *(float *)(moby + 0x48) = owner->rot.z;
        *(long *)(moby + 0x38) = owner->spawn_frame;
        qcopy(moby + 0x10, &owner->pos);
        *(float *)(moby + 0x18) = 59.0f;
        FUN_L00_00250df8(moby);
    }
    return moby;
}

#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
extern int stub_printf() __asm__("FUN_001e93b0");
extern void delete_moby(void *) __asm__("FUN_0020c828");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int func_001F9850_95a38(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern char D_L10_001DC510[];
void FUN_L10_00295a38(char *m) {
    char *d = *(char **)(m + 0x78);
    if (d == 0) {
        stub_printf(D_L10_001DC510, *(short *)(m + 0xA6), *(short *)(m + 0xB2));
        delete_moby(m);
        return;
    }
    switch (*(unsigned char *)(m + 0x20)) {
    case 0:
        d[0x28] = 4;
        *(short *)(d + 0x3E) = 5;
        *(int *)(d + 0x20) = 0;
        *(short *)(d + 0x24) = 0;
        *(OvlQuad *)(d + 0xA0) = *(OvlQuad *)(m + 0x10);
        m[0x20] = 1;
        break;
    case 1: {
        float lim;
        *(float *)(m + 0x18) = *(float *)(m + 0x18) + 1.5f / scale_time(45.0f);
        lim = *(float *)(d + 0xA8) + 1.5f;
        if (lim <= *(float *)(m + 0x18)) {
            *(float *)(m + 0x18) = lim;
            m[0x20] = 2;
            *(int *)(d + 0xB0) = func_001F9850_95a38(0x78);
        }
        break;
    }
    case 2:
        if (fast_dec_timer((int *)(d + 0xB0))) {
            allocate_voice_for_target_entry(0, 0, (int)m);
            m[0x20] = 3;
        }
        break;
    case 3: {
        float lim;
        *(float *)(m + 0x18) = *(float *)(m + 0x18) - 1.5f / scale_time(45.0f);
        lim = *(float *)(d + 0xA8);
        if (*(float *)(m + 0x18) <= lim) {
            *(float *)(m + 0x18) = lim;
            m[0x20] = 4;
            *(int *)(d + 0xB0) = func_001F9850_95a38(0x78);
        }
        break;
    }
    case 4:
        if (fast_dec_timer((int *)(d + 0xB0))) {
            allocate_voice_for_target_entry(0, 0, (int)m);
            m[0x20] = 1;
        }
        break;
    }
}



/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_00296DC0), where it is exact; names translated to the US level program. */


extern f32 random_angle_radians(void) __asm__("func_00213308");
extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L01_00277fb8(void *);
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void FUN_L00_00262b00(float, float, char *, float *, float *);
extern void FUN_L00_00262b80(void *, char *, char *, float, float, float);
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

void FUN_L10_00295c20(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    float prev[4];
    float v[4];
    *(OvlQuad *)prev = *(OvlQuad *)&moby->pos;
    if (FUN_L01_00277fb8(moby)) moby->state = 2;
    fast_dec_timer((int *)(d + 0xBC));
    switch (moby->state) {
    case 0:
        d[0x28] = 4;
        *(short *)(d + 0x3E) = 5;
        *(int *)(d + 0x20) = 0;
        *(short *)(d + 0x24) = 0;
        *(float *)(d + 0xA0) = moby->pos.z;
        *(float *)(d + 0xA8) = random_angle_radians();
        *(float *)(d + 0xB4) = random_angle_radians();
        {
            float r = random_angle_radians();
            float e = frame_time * 0.13962634f;
            *(float *)(d + 0xB8) = r;
            *(float *)(d + 0xB0) = frame_time * 1.5707964f + randf_sym(0.0f, e);
        }
        moby->state = 1;
    case 1:
        *(int *)(d + 0xA4) = 0;
        FUN_L00_00262b00(0.1f, *(float *)(d + 0xB0), moby, (float *)(d + 0xA8), (float *)(d + 0xAC));
        FUN_L00_00262b80(moby, d + 0xB4, d + 0xB8, 0.047123890f, frame_time * 0.34906584f, frame_time * 0.5235988f);
        break;
    case 2: {
        float a = *(float *)(d + 0xA4) - (frame_time_sq + frame_time_sq);
        float lim = -(frame_time + frame_time);
        *(float *)(d + 0xA4) = a;
        if (a < lim) *(float *)(d + 0xA4) = lim;
        moby->pos.z += *(float *)(d + 0xA4);
        if (FUN_L01_00277fb8(moby) == 0) moby->state = 3;
        if (*(int *)(d + 0xBC) <= 0) {
            allocate_voice_for_target_entry(0, 0, (int)moby);
            *(int *)(d + 0xBC) = random_int_between(0x28, 0x3C);
        }
        break;
    }
    case 3: {
        float vz = *(float *)(d + 0xA4);
        float g = frame_time_sq;
        float dist, sq, t;
        if (vz < 0.0f || (t = g + g, dist = *(float *)(d + 0xA0) - moby->pos.z, sq = vz * vz,
                          sq / (t + t) < dist)) {
            float a = vz + g;
            float mx = frame_time + frame_time;
            *(float *)(d + 0xA4) = a;
            if (mx < a) *(float *)(d + 0xA4) = mx;
        } else if (0.0f < dist) {
            float a = vz - sq / (dist + dist);
            *(float *)(d + 0xA4) = a;
            if (a < 0.0f) *(float *)(d + 0xA4) = 0.0f;
        }
        moby->pos.z += *(float *)(d + 0xA4);
        if (*(float *)(d + 0xA0) <= moby->pos.z) {
            moby->pos.z = *(float *)(d + 0xA0);
            moby->state = 1;
        }
        break;
    }
    }
    subtract_vector_xyz(v, &moby->pos, prev);
    update_entity_matrix(d + 0x60, v, &moby->rot, &moby->rot);
}
/* Spinning pickup: waits until the hero is close and level with it, then plays the scene, saves and goes away. */
extern char *D_L10_0015FFD8_p __asm__("D_L10_0015FFD8");
extern void FUN_L00_002d6bf0_p(void *) __asm__("FUN_L00_002d6bf0");
extern void FUN_L10_00298940_p(void *) __asm__("FUN_L10_00298940");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern void start_scene_p(int) __asm__("FUN_L00_00298840");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void FUN_L00_00260860_p(int, int) __asm__("FUN_L00_00260860");
extern void FUN_L00_00284e50_p(void *, void *) __asm__("FUN_L00_00284e50");
extern int memcard_save_data(int, int) __asm__("FUN_0020b178");

extern int D_L10_0015F640;
void magnetboots_update(struct Moby *m) __asm__("FUN_L10_00298668");
void magnetboots_update(struct Moby *m) {
    char *d = (char *)m->pvars;
    m->rot.z = fast_add_rotations(m->rot.z, frame_time * 1.5707964f);
    switch (m->state) {
    case 0:
        FUN_L00_002d6bf0_p(m);
        if (D_0013D504[0] != 0) {
            delete_moby(m);
            return;
        }
        m->state = 1;
        m->pos.z = m->pos.z + 0.5f;
        m->scale = m->scale * 0.667f;
        break;
    case 1:
        FUN_L10_00298940_p(m);
        if (vector_distance(&m->pos, &hero.motion.pos) < 3.0f) {
            struct Hero *h = &hero;
            if (absolute_float(m->pos.z - h->motion.pos.f[2]) < 2.0f && h->health.hp != 0) {
                m->flags |= 0x41;
                start_scene_p(2);
                m->state = 2;
            }
        }
        break;
    case 2:
        if (D_L10_0015F5C4_vendor != 2) {
            queue_dialogue_message(0x271B, -1);
            D_L10_0015F640 = 0xB4;
            FUN_L00_00260860_p(0x1C, 1);
            if (*(int *)(d + 4) != -1) {
                char *o = D_L10_0015FFD8_p + (*(int *)(d + 4) << 8);
                FUN_L00_00284e50_p(o + 0x10, o + 0x40);
            }
            memcard_save_data(0, -1);
            m->state = 3;
        }
        break;
    case 3:
        delete_moby(m);
        break;
    }
}
#include "rnc/gameplay/state/usage_stats.h"

typedef struct {
    int target;
    int vehicle_spot;
    int spot;
    int pad0C[2];
    int spot_alt;
    int switch_a;
    int switch_b;
} PickupVars_98b68;

typedef struct {
    u8 pad[0x454];
    u8 collected[1];
} LevelState_98b68;

int D_L10_00161388 = 1;
extern char *D_L10_0015FFD8_98b68 __asm__("D_L10_0015FFD8");
extern u8 D_0014C050_98b68[] __asm__("D_0014C050");
extern LevelState_98b68 D_L10_001BB6B0_98b68 __asm__("D_L10_001BB6B0");
extern int D_0014C190_98b68[][64] __asm__("D_0014C190");
extern int D_L10_001BA950[];
extern u8 D_001413F4_98b68[] __asm__("D_001413F4");
extern char *D_L10_001600EC_98b68 __asm__("D_L10_001600EC");
extern int scale_game_frames_98b68(int) __asm__("FUN_001f96f8");
extern int try_set_help_message_98b68(int, int) __asm__("FUN_00215130");
extern void FUN_L00_00216f90_98b68(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void FUN_L00_00210a08_98b68(int, int, struct Moby *) __asm__("FUN_L00_00210a08");
extern void FUN_L01_0027a248_98b68(int, int) __asm__("FUN_L01_0027a248");
extern void FUN_L00_00210b30(void);
extern void mark_moby_for_removal_98b68(struct Moby *) __asm__("FUN_0020c828");

extern short D_00141948 __attribute__((section(".data")));
/* Level 10 pickup: saves its collected bit, flips the linked switch pair and moves the hero to the spot for the current state. */
void FUN_L10_00298b68(struct Moby *moby) {
    PickupVars_98b68 *vars;
    struct Moby *target;
    moby->update_dist = 0xFF;
    vars = (PickupVars_98b68 *)moby->pvars;
    if (D_L10_00161388 == 1) {
        D_L10_00161388 = 0;
        D_00141948 = 0;
    }
    if (vars->switch_a != -1 && vars->switch_b != -1) {
        struct Moby *a = (struct Moby *)((vars->switch_a << 8) + (int)D_L10_0015FFD8_98b68);
        if (a->oclass == 0x325) {
            struct Moby *b = (struct Moby *)((vars->switch_b << 8) + (int)D_L10_0015FFD8_98b68);
            if (b->oclass == a->oclass) {
                int *a_vars = (int *)a->pvars;
                int *b_vars = (int *)b->pvars;
                if (D_0014C050_98b68[moby->mission + current_level_index * 16] == 0xFF) {
                    a_vars[2] |= 8;
                    b_vars[2] &= ~8;
                } else {
                    a_vars[2] &= ~8;
                    b_vars[2] |= 8;
                }
            }
        }
    }
    if (D_L10_001BB6B0_98b68.collected[(short)moby->save_id] != 0 ||
        ((D_0014C190_98b68[current_level_index][(short)moby->save_id >> 5] >> (moby->save_id & 31)) & 1) != 0) {
        if (D_0013D4C6[0] != 0) {
            ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->flags |= 1;
            ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->flags |= 2;
            ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->collision = 0;
        } else {
            if (D_L10_0015F5C4_vendor == 0 && usage_stats.stat[32].count < scale_game_frames_98b68(300)) {
                if (usage_stats.stat[32].count <= 0xFFFE) {
                    usage_stats.stat[32].count++;
                }
                if (scale_game_frames_98b68(D_0015EEA4) / 600 > usage_stats.stat[32].unk2) {
                    usage_stats.stat[32].unk2 = scale_game_frames_98b68(D_0015EEA4) / 600;
                }
                usage_stats.stat[32].level_mask = usage_stats.stat[32].level_mask | (1 << current_level_index) | 0x80000000;
                try_set_help_message_98b68(1, 0x53F1);
                if (D_001413F4_98b68[0] == 1) {
                    return;
                }
            } else if (D_001413F4_98b68[0] == 1) {
                return;
            }
        }
    }
    D_0014C190_98b68[current_level_index][(short)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
    D_L10_001BA950[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
    if (vars->spot == -1 || vars->target == -1) {
        return;
    }
    if (D_0013D4C6[0] == 0) {
        target = (struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68);
        target->collision = *(u32 *)((char *)target->pclass + 0x10);
        if (D_0014C050_98b68[moby->mission + current_level_index * 16] == 0xFF || vars->spot_alt == -1) {
            FUN_L00_00216f90_98b68(D_L10_001600EC_98b68 + vars->spot_alt * 128 + 0x30,
                                   D_L10_001600EC_98b68 + vars->spot_alt * 128 + 0x70, 0, 1);
        } else {
            FUN_L00_00216f90_98b68(D_L10_001600EC_98b68 + vars->spot * 128 + 0x30,
                                   D_L10_001600EC_98b68 + vars->spot * 128 + 0x70, 0, 1);
        }
        FUN_L00_00210a08_98b68(1, 0x43, (struct Moby *)(D_L10_0015FFD8_98b68 + (vars->target << 8)));
        FUN_L01_0027a248_98b68(2, 4);
        return;
    }
    if (hero.vehicle_mode == 1) {
        FUN_L00_00210b30();
        FUN_L00_00216f90_98b68(D_L10_001600EC_98b68 + vars->vehicle_spot * 128 + 0x30,
                               D_L10_001600EC_98b68 + vars->vehicle_spot * 128 + 0x70, 0, 1);
        FUN_L01_0027a248_98b68(0, 5);
        ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->flags |= 1;
        ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->flags |= 2;
        ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->collision = 0;
        hero.unk22CB = 1;
        mark_moby_for_removal_98b68(moby);
    } else {
        hero.unk22CB = 1;
        mark_moby_for_removal_98b68(moby);
    }
}
#include "rnc/math/vector.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002BFBB8), where it is exact; names translated to the US level program. */

typedef unsigned int Q_2BFBB8 __attribute__((mode(TI), aligned(16)));

typedef struct { char p0[0x10]; float x; float y; char p1[8]; unsigned char state; char p2[0xDF]; } Ent;

extern struct Moby * D_L10_0015FFD8;
extern char D_0013F350[];
extern char D_L10_00167100[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int D_L10_0015F5CC;
extern int D_L10_001618A0 __attribute__((sda));
extern int D_L10_001618A4 __attribute__((sda));
extern int D_L10_001618A8;
extern int FUN_L00_00257b90(int, int);
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 fast_tween_color(s32, s32, f32) __asm__("func_001FA6E0");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void * FUN_L00_002715e8(void *, int, int, float, float);
extern void * FUN_L00_00272060(void *, int, float, float, int, int, int, void *, float);
extern void add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void FUN_L00_002e8568(int);
extern void follow_camera_set_lateral_spring(float, float) __asm__("FUN_L00_002e8590");
extern void FUN_L00_002e8648(void);
extern void FUN_L00_002e8680(void);
extern void fast_vec_normalize_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern struct Moby * D_L10_0015FFD8;

#define func_001F9850_2BFBB8 ((int (*)(int))scale_game_frames)
#define func_001F9D48_2BFBB8 ((float (*)(void *, void *))vector_distance_xy_alt)
#define func_002140F8_2BFBB8 ((float (*)(float, float))random_float_between)
#define func_00214158_2BFBB8 ((float (*)(void))random_angle_radians)
#define func_002140B0_2BFBB8 ((int (*)(int))random_integer_below)
#define func_L00_00258BC8_2BFBB8 ((int (*)(int, int))FUN_L00_00257b90)
#define func_001F9F90_2BFBB8 ((float (*)(float))fast_cos)
#define func_001F9FA8_2BFBB8 ((float (*)(float))fast_sin)
#define func_L00_001FF4B0_2BFBB8 ((void (*)(void *, void *, float))fast_vec_normalize_c)
#define func_L00_00272F00_2BFBB8 FUN_L00_00272060
#define func_L00_00272488_2BFBB8 FUN_L00_002715e8
#define func_0022ED80_2BFBB8 ((int (*)(int, int, int))allocate_voice_for_target_entry)
#define func_L00_00217718_2BFBB8 FUN_L00_00216f90
#define func_001FA888_2BFBB8 ((float (*)(int))convert_integer_to_float)
#define func_001FA898_2BFBB8 ((int (*)(float))truncate_float_to_s32)
#define func_001FA8A8_2BFBB8 fast_tween_color
#define func_001F9908_2BFBB8 ((int (*)(int *))tick_countdown_32_alt)
#define func_L00_002E9A40_2BFBB8 follow_camera_set_lateral_spring
#define func_L00_002E9B30_2BFBB8 FUN_L00_002e8680
#define func_L00_002E9AF8_2BFBB8 FUN_L00_002e8648
#define func_L00_002E9A18_2BFBB8 FUN_L00_002e8568

void gadgebot_telepot_pad_update(char *moby) __asm__("FUN_L10_002be858");
void gadgebot_telepot_pad_update(char *moby)
{
    int *data = *(int **)(moby + 0x78);
    float position[4];
    float velocity[4];
    float offset_buffer[4];
    float angle;
    float *offset;
    int i;
    char *hero_bytes;
    int life;
    if (*(unsigned char *)(moby + 0x20) == 0) {
        D_L10_001618A8 = 0;
        D_L10_001618A4 = func_001F9850_2BFBB8(D_L10_001618A4);
        D_L10_001618A0 = func_001F9850_2BFBB8(D_L10_001618A0);
        *(unsigned char *)(moby + 0x20) = 1;
    }
    hero_bytes = D_0013F350;
    if ((unsigned char)hero_bytes[0x20A4] == 1 && *data != -1 &&
        func_001F9D48_2BFBB8(moby + 0x10, hero_bytes + 0x80) < 0.5f) {
        offset = offset_buffer;
        i = 9;
        do {
            float size, size_scaled;
            int red, green, blue, color, spin, green_alpha;
            i--;
            size = func_002140F8_2BFBB8(0.1f, 1.0f);
            red = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            green = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            blue = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            velocity[0] = func_001F9F90_2BFBB8(func_00214158_2BFBB8()) * func_002140F8_2BFBB8(0.0f, 0.5f);
            velocity[1] = func_001F9FA8_2BFBB8(func_00214158_2BFBB8()) * func_002140F8_2BFBB8(0.0f, 0.5f);
            velocity[2] = 0.0f;
            velocity[2] += func_002140F8_2BFBB8(0.0f, 0.5f);
            add_vectors(offset, velocity, D_0013F350 + 0x80);
            func_L00_001FF4B0_2BFBB8(position, position, func_002140F8_2BFBB8(0.0f, D_0015ED6C));
            position[2] = func_002140F8_2BFBB8(D_0015ED6C * 2.5f, D_0015ED6C * 5.0f);
            life = func_001F9850_2BFBB8(func_L00_00258BC8_2BFBB8(15, 30));
            green_alpha = (green << 8) | 0x7f000000;
            color = (blue << 16) | green_alpha | red;
            size_scaled = size * 0.07f;
            size *= 0.7f;
            spin = func_002140B0_2BFBB8(2) ? 1 : -1;
            func_L00_00272F00_2BFBB8(velocity, life, size_scaled, size, color, 0, spin, position, D_0015ED70 * 15.0f);
        } while (i >= 0);
        func_0022ED80_2BFBB8(0, 0, (int)moby);
        {
            int moby_offset = *data << 8;
            char *effect = (*(char * *)&D_L10_0015FFD8) + moby_offset;
            func_L00_00217718_2BFBB8(effect + 0x10, effect + 0x40, 0x43, 0);
        }
        {
            char *table = D_L10_00167100;
            char *effect = *(char **)(table + 0x180);
            D_L10_001618A8 = D_L10_001618A0;
            if (*(short *)(effect + 0x86) == 0) {
                char *current = *(char **)(table + 0x180);
                short *timer = (short *)(*(char **)(current + 0x70) + 0x20);
                *timer = func_001F9850_2BFBB8(0x5a);
                qcopy(current + 0x40, current);
            }
        }
        i = 9;
        do {
            float size, size_scaled;
            int red, green, blue, color, spin, green_alpha;
            *(Q_2BFBB8 *)velocity = 0;
            velocity[0] = func_002140F8_2BFBB8(-1.0f, 1.0f);
            i--;
            velocity[1] = func_002140F8_2BFBB8(-1.0f, 1.0f);
            *(Q_2BFBB8 *)position = *(Q_2BFBB8 *)velocity;
            size = func_002140F8_2BFBB8(0.1f, 1.0f);
            red = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            green = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            blue = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            velocity[0] = func_001F9F90_2BFBB8(func_00214158_2BFBB8()) * func_002140F8_2BFBB8(0.0f, 0.5f);
            velocity[1] = func_001F9FA8_2BFBB8(func_00214158_2BFBB8()) * func_002140F8_2BFBB8(0.0f, 0.5f);
            velocity[2] = 0.0f;
            velocity[2] += func_002140F8_2BFBB8(0.0f, 0.5f);
            add_vectors(offset, velocity, (*(char * *)&D_L10_0015FFD8) + (*data << 8) + 0x10);
            func_L00_001FF4B0_2BFBB8(position, position, func_002140F8_2BFBB8(0.0f, D_0015ED6C));
            position[2] = func_002140F8_2BFBB8(D_0015ED6C * 2.5f, D_0015ED6C * 5.0f);
            life = func_001F9850_2BFBB8(func_L00_00258BC8_2BFBB8(15, 30));
            green_alpha = (green << 8) | 0x7f000000;
            color = (blue << 16) | green_alpha | red;
            size_scaled = size * 0.07f;
            size *= 0.7f;
            spin = func_002140B0_2BFBB8(2) ? 1 : -1;
            func_L00_00272F00_2BFBB8(velocity, life, size_scaled, size, color, 0, spin, position, D_0015ED70 * 15.0f);
        } while (i >= 0);
    }
    if (D_L10_0015F5CC % func_001F9850_2BFBB8(0x3c) == 0 && *data != -1) {
        for (i = 0; i < func_001FA898_2BFBB8(16.0f); i++) {
            char *effect;
            *(Q_2BFBB8 *)position = *(Q_2BFBB8 *)(moby + 0x10);
            position[2] += func_001FA888_2BFBB8(i) * 0.1f * 0.0625f + 1.8f;
            effect = (char *)func_L00_00272488_2BFBB8(position, 0, 0x1eff7f40, 0.0f, D_0015ED6C * 315000.0f);
            if (effect != 0) {
                float *vel = (float *)(effect + 0x20);
                float z = D_0015ED6C * -1.5f;
                vel[0] = 0.0f;
                vel[1] = 0.0f;
                vel[2] = z;
            }
        }
    }
    {
        float phase = (float)(D_L10_0015F5CC % 150) / 150.0f;
        phase += phase;
        angle = phase * 3.1415927f;
    }
    if (3.1415927f < angle) angle -= 6.2831855f;
    *(unsigned int *)(moby + 0x90) = func_001FA8A8_2BFBB8(0x80464646, 0x80828282, (func_001F9FA8_2BFBB8(angle) + 1.0f) * 0.5f);
    func_001F9908_2BFBB8(&D_L10_001618A8);
    if (D_L10_001618A8 != 0) {
        if (D_L10_001618A4 < D_L10_001618A8) angle = 0.002f;
        else angle = func_001FA888_2BFBB8(D_L10_001618A8) / func_001FA888_2BFBB8(D_L10_001618A4) * 0.007999999f + 0.002f;
        func_L00_002E9A40_2BFBB8(angle, 0.2f);
        func_L00_002E9B30_2BFBB8();
        func_L00_002E9AF8_2BFBB8();
        func_L00_002E9A18_2BFBB8(0);
    }
}
/* Collectible drop: waits until it is taken, then sinks below its rest height and records the pickup. */
typedef struct {
    char pad0[0x454];
    unsigned char collected[1];
} BfState;
extern BfState D_L10_001BB6B0_bf __asm__("D_L10_001BB6B0");
extern int D_0015ED84_bf __asm__("D_0015ED84");
extern float D_0015ED6C_bf __asm__("D_0015ED6C");
extern unsigned char D_0014C190_bf[] __asm__("D_0014C190");
extern unsigned char D_0014C050_bf[] __asm__("D_0014C050");

extern BfState D_L10_001BAA50;
extern unsigned char D_L10_0015FC88[8];
void gate_update(unsigned char *m) __asm__("FUN_L10_002befe8");
void gate_update(unsigned char *m) {
    float *d = *(float **)(m + 0x78);
    unsigned short id;
    switch (m[0x20]) {
    case 0:
        *(OvlQuad *)d = *(OvlQuad *)(m + 0x10);
        id = *(unsigned short *)(m + 0xB2);
        if (D_L10_001BB6B0_bf.collected[(short)id] != 0 ||
            (*(int *)(D_0014C190_bf + (((short)id >> 5) * 4 + (D_0015ED84_bf << 8))) >> (id & 0x1F)) & 1) {
            m[0x20] = 1;
        }
        break;
    case 1:
        *(float *)(m + 0x14) += D_0015ED6C_bf * -1.0f;
        if (*(float *)(m + 0x14) <= d[1] + -3.75f) {
            D_L10_001BAA50.collected[*(short *)(m + 0xB2)] = m[0xB0] + 2;
            if (m[0xB0] == 0xFF || (D_L10_0015FC88[m[0xB0]] != 0xFF && D_0014C050_bf[m[0xB0] + (D_0015ED84_bf << 4)] == 0xFF)) {
                D_L10_001BB6B0_bf.collected[*(short *)(m + 0xB2)] = m[0xB0] + 2;
            }
            m[0x20] = 2;
            if (D_0015ED84_bf == 10) {
                menu_item_enabled_68[0] = 1;
            }
        }
        break;
    case 2:
        *(float *)(m + 0x14) = d[1] + -3.75f;
        break;
    }
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002C8DE0), where it is exact; names translated to the US level program. */

extern char D_0013E550[];
extern float D_0015ED6C_c __asm__("D_0015ED6C");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L00_0028dc90(int i, int a1, int v, int k);
extern void *FUN_L00_0026fd28(char *a, char *b, unsigned char c, int d);
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern int tick_countdown_32(void *) __asm__("func_001F9740");

void FUN_L10_002c7a20(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float d[16];
    float v[4];
    float r[4];
    float s[4];
    switch (moby->state) {
    case 0:
        *(int *)(data + 0x10) = -1;
        if (tick_countdown_32((int *)(data + 0xC))) {
            *(int *)(data + 0xC) = *(int *)(data + 4);
            moby->state = 1;
        }
        break;
    case 1:
        qcopy(v, &moby->pos);
        euler_rotation_matrix(d, &moby->rot.x);
        clear_u64_value(r);
        r[2] = *(float *)data * 3.0f * D_0015ED6C_c;
        transform_vector(r, r, d);
        scale_vector_xyz(s, r, 0.33f);
        FUN_L00_0026fd28((char *)v, (char *)r, 0x40, (int)moby);
        add_vector_xyz(v, v, s);
        FUN_L00_0026fd28((char *)v, (char *)r, 0x40, (int)moby);
        add_vector_xyz(v, v, s);
        FUN_L00_0026fd28((char *)v, (char *)r, 0x40, (int)moby);
        if (voice_is_playing(moby, *(int *)(data + 0x10)) == 0) {
            *(int *)(data + 0x10) = FUN_L00_0028dc90(0, 4, (int)moby, 0x519);
        }
        if (tick_countdown_32((int *)(data + 0xC))) {
            int k;
            *(int *)(data + 0xC) = *(int *)(data + 8);
            moby->state = 0;
            k = *(int *)(data + 0x10);
            if (k != -1) {
                unsigned char *e = (unsigned char *)D_0013E550 + k * 0x70;
                if (*(unsigned char **)(e + 0x88) == moby && e[0x74] != 0) {
                    release_voice_slot(k);
                }
            }
            *(int *)(data + 0x10) = -1;
        }
        break;
    }
}
#include "qzero.h"
/* Box a moby sweeps through: hits other mobys inside it (FUN_L10_002c9958) */
typedef struct {
    f32 width;        /* 0x00: extent along the moby's x */
    f32 depth;        /* 0x04: extent along the moby's z */
    u32 flags;        /* 0x08: 0x100 disables the box */
    u8 pad0C[0x3C];
    f32 margin_x;     /* 0x48 */
    f32 margin_z;     /* 0x4C */
    f32 strength;     /* 0x50: passed to the hit */
} HitBox;

extern struct Moby *D_L10_00178380_u[] __asm__("D_L10_00178380");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void *memcpy(void *, const void *, u32);
extern void FUN_L10_0024c7f0(unsigned char *m, int x, int y, void *p, float f);

void FUN_L10_002c9958(struct Moby *moby, HitBox *box) {
    OvlVec4 mat[3];
    OvlVec4 center;
    OvlVec4 delta;
    OvlVec4 local;
    OvlVec4 inv[4];
    OvlVec4 point;
    f32 radius;
    s32 count;
    s32 i;
    s32 j;
    s32 n;

    if (box->flags & 0x100) {
        return;
    }
    euler_rotation_basis(mat, &moby->rot);
    if (box->width > box->depth) {
        radius = box->width * 0.5f;
    } else {
        radius = box->depth * 0.5f;
    }
    qzero(&center);
    center.f[2] = radius;
    transform_vector_by_basis(&center, &center, mat);
    fast_vec_add(&center, &center, &moby->pos);
    count = collect_mobies_in_sphere(&center, radius, 0x10, moby, 0);
    if (count != 0) {
        struct Moby *found[count];

        memcpy(found, D_L10_00178380_u, count * 4);
        for (i = 0; i < count; i++) {
            struct Moby *other = found[i];

            if (other == 0 || other->state == 0xFE || other->state == 0xFD) {
                continue;
            }
            subtract_vector_xyz(&delta, &other->pos, &moby->pos);
            transpose_rotation_matrix(inv, mat);
            transform_vector(&local, &delta, inv);
            qcopy(&point, &local);
            if (point.f[0] < -(box->width * 0.5f + box->margin_x)) {
                point.f[0] = -box->width * 0.5f;
            } else if (box->width * 0.5f + box->margin_x < point.f[0]) {
                point.f[0] = box->width * 0.5f;
            }
            if (point.f[2] < -(box->depth * 0.5f + box->margin_z)) {
                point.f[2] = -box->depth * 0.5f;
            } else if (box->depth * 0.5f + box->margin_z < point.f[2]) {
                point.f[2] = box->depth * 0.5f;
            }
            point.f[1] = 0.0f;
            transform_vector_by_basis(&point, &point, mat);
            fast_vec_add(&point, &point, &moby->pos);
            n = collect_mobies_in_sphere(&point, box->margin_x, 0x10, moby, 0);
            for (j = 0; j < n; j++) {
                if (D_L10_00178380_u[j] == other) {
                    FUN_L10_0024c7f0((unsigned char *)other, (int)moby, 0x10001, &point, box->strength);
                }
            }
        }
    }
}
#include "types.h"
extern struct Moby *D_L10_0015FFD8;
extern float FUN_001fa6c0_2c9c70(int) __asm__("FUN_001fa6c0");
extern int FUN_001fa6d0_2c9c70(float) __asm__("FUN_001fa6d0");
extern int FUN_001f9740_2c9c70(int *) __asm__("FUN_001f9740");
extern int D_0015ED84;
extern unsigned char D_0014C190_2c9c70[] __asm__("D_0014C190");
extern unsigned char D_0014C050_2c9c70[] __asm__("D_0014C050");

extern char linked_moby_warning[] __asm__("D_L10_001DCDA0");
extern char linked_moby_present_message[] __asm__("D_L10_001DCDD0");
extern char linked_moby_missing_message[] __asm__("D_L10_001DCE00");

void FUN_L10_002c9c70(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    int differences = 0;
    struct Moby *target = 0;
    int *slot = (int *)(d + 0x18);
    unsigned char state;

    if (*(int *)(d + 0x10) != -1) {
        target = &D_L10_0015FFD8[*(int *)(d + 0x10)];
    }
    if ((moby->mission == 0xff ||
         D_0014C050_2c9c70[moby->mission + (D_0015ED84 << 4)] != 0xff) &&
        moby->state != 0) {
        int i;
        for (i = 0; i < 5; i++) {
            int *entry = slot + i * 2;
            int index = *entry;
            if (index != -1) {
                struct Moby *other = &D_L10_0015FFD8[index];
                if (other != 0 && other->oclass == *(short *)(entry + 1) &&
                    other->state != 0xfe && other->state != 0xfd) {
                    unsigned short id = other->save_id;
                    if ((((*(int *)(D_0014C190_2c9c70 + (((short)id >> 5) * 4) +
                              (D_0015ED84 << 8)) >> (id & 31)) ^ 1) & 1) != 0) {
                        if (*(signed char *)((char *)entry + 6) == -1 ||
                            other->state != *(signed char *)((char *)entry + 6))
                            differences++;
                    }
                }
            }
        }
    }
    switch (state = moby->state) {
    case 0:
        /* Position flags select opposite copy directions; rotation flags have priority. */
        if (target != 0) {
            *(short *)(d + 0x14) = target->oclass;
            *(float *)(d + 0x44) = FUN_001fa6c0_2c9c70(target->alpha);
            if ((*(int *)(d + 8) & 0x10) != 0) {
                qcopy(&moby->pos, &target->pos);
            } else if ((*(int *)(d + 8) & 0x40) != 0) {
                qcopy(&target->pos, &moby->pos);
            }
            if ((*(int *)(d + 8) & 0x20) != 0) {
                qcopy_nc(&moby->rot, &target->rot);
            } else if ((*(int *)(d + 8) & 0x80) != 0) {
                qcopy_nc(&target->rot, &moby->rot);
            }
        } else {
            /* Retail logs only a missing target; neither rotation flag is silent. */
            stub_printf(linked_moby_warning);
        }
        {
            int *entry = slot;
            int i;
            for (i = 0; i < 5; i++, entry += 2) {
                int index = *entry;
                if (index != -1) {
                    char *base = D_L10_0015FFD8_p;
                    unsigned short oclass = *(unsigned short *)((index << 8) + (int)base + 0xa6);
                    *(short *)(entry + 1) = oclass;
                    stub_printf(linked_moby_present_message, ((char *)moby - base) >> 8,
                                i, index, (short)oclass);
                } else {
                    stub_printf(linked_moby_missing_message, ((char *)moby - D_L10_0015FFD8_p) >> 8, i);
                }
            }
        }
        {
            float initial = *(float *)(d + 4);
            float now = *(float *)d;
            float scale = moby->pclass->scale;
            float size = (now < initial ? initial * scale : now * scale);
            moby->state = 1;
            moby->scale = size;
        }
        break;
    case 1:
        if (differences == 0) {
            if (target != 0 && *(signed char *)(d + 0x16) != -1)
                target->state = *(unsigned char *)(d + 0x16);
            *(int *)(d + 0x40) = *(int *)(d + 0xc);
            moby->state = 2;
        }
        FUN_L10_002c9958(moby, (HitBox *)d);
        return;
    case 2:
        if (target != 0) {
            if ((*(int *)(d + 8) & 2) == 0) {
                float now = FUN_001fa6c0_2c9c70(*(int *)(d + 0x40));
                float initial = FUN_001fa6c0_2c9c70(*(int *)(d + 0xc));
                target->alpha = FUN_001fa6d0_2c9c70(*(float *)(d + 0x44) * now / initial);
            }
        }
        if ((*(int *)(d + 8) & 4) == 0 && target != 0 &&
            *(signed char *)(d + 0x17) != -1 && target->state == *(signed char *)(d + 0x17))
            moby->state = 3;
        if ((*(int *)(d + 8) & 8) == 0 && FUN_001f9740_2c9c70((int *)(d + 0x40)) != 0)
            moby->state = 3;
        FUN_L10_002c9958(moby, (HitBox *)d);
        return;
    case 3:
        if (target != 0 && (*(int *)(d + 8) & 1) == 0) delete_moby(target);
        delete_moby(moby);
        break;
    case 4:
        break;
    }
}
void FUN_L10_002d7b18(struct Moby *a, float t) {
    float c = a->pos.z;
    float d = t - c;
    if (0.1f < d) {
        a->pos.z = c + 0.1f;
    } else if (d < -0.1f) {
        a->pos.z = c - 0.1f;
    } else {
        a->pos.z = t;
    }
}
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L01_00274b78(float, void *, void *);
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_0026dd20(void *, void *, int, int, float);
extern void mark_moby_for_removal_d7b80(void *) __asm__("FUN_0020c828");
extern char D_L10_001742C0[];
extern char D_L10_00174300[];
extern float D_0015ED6C;
extern OvlQuad D_L10_00167240;
extern struct Moby *D_001413D0_d7b80 __asm__("D_001413D0") __attribute__((section(".data")));

void FUN_L10_002d7b80(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float old[4] __attribute__((aligned(16)));
    float found[20] __attribute__((aligned(16)));
    float check[4] __attribute__((aligned(16)));
    float raised[4] __attribute__((aligned(16)));
    float hit[12] __attribute__((aligned(16)));
    float rand_dir[4] __attribute__((aligned(16)));
    int i;

    qcopy(old, &moby->pos);
    fast_vec_add(&moby->pos, &moby->pos, data);
    FUN_L01_00274b78(20.0f, moby, found);
    switch (*(short *)(data + 0x12)) {
    case 0:
        break;
    case 1:
        FUN_L10_002d7b18(moby, *(float *)(data + 0x1c));
        break;
    case 2:
        qcopy(check, &moby->pos);
        qcopy(raised, &moby->pos);
        check[2] += 0.1f;
        raised[2] -= *(float *)(data + 0x1c) + *(float *)(data + 0x1c);
        if (collision_line(check, raised, 2, (int)moby, 0))
            FUN_L10_002d7b18(moby, *(float *)(D_L10_001742C0 + 0x28) + *(float *)(data + 0x1c));
        else
            FUN_L10_002d7b18(moby, moby->pos.z - 0.1f);
        break;
    case 3:
        if (*(int *)((char *)found + 0x40) != 0)
            FUN_L10_002d7b18(moby, found[2] + *(float *)(data + 0x1c));
        break;
    case 4:
        break;
    }
    qcopy(check, data);
    check[2] = 0.0f;
    set_vector_length_xy(check, check, 1.0f);
    check[2] = 1.0f;
    make_hit_descriptor(hit, moby, 0x10001, *(float *)(data + 0x18), check);
    if (fast_dec_timer_s16(data + 0x10)) {
        mark_moby_for_removal_d7b80(moby);
        return;
    }
    if (collision_line(old, &moby->pos, 0, *(int *)(data + 0x20), (int)hit)) {
        char *collision = D_L10_001742C0;
        struct Moby *other = *(struct Moby **)(collision + 0x18);
        if (other == 0 || other->oclass != 0x359) {
            qcopy(&moby->pos, collision + 0x20);
            if (*(char **)(data + 0x14) != 0) {
                if (other != 0) {
                    if (other == D_001413D0_d7b80 &&
                        *(struct Moby **)(data + 0x20) != 0 &&
                        (*(struct Moby **)(data + 0x20))->state != 0xfe &&
                        (*(struct Moby **)(data + 0x20))->state != 0xfd)
                        **(char **)(data + 0x14) = 1;
                }
            }
            for (i = 0; i < 5; i++) {
                *(OvlQuad *)rand_dir = 0;
                rand_dir[0] = random_float_between_alt(-1.0f, 1.0f);
                rand_dir[1] = random_float_between_alt(-1.0f, 1.0f);
                rand_dir[2] = random_float_between_alt(-1.0f, 1.0f);
                *(OvlQuad *)raised = *(OvlQuad *)rand_dir;
                FUN_L00_001ff660(rand_dir, data, D_L10_00174300);
                fast_vec_normalize(raised, raised, fast_vec_length(rand_dir) * 0.5f);
                fast_vec_add(raised, rand_dir, raised);
                fast_vec_normalize(raised, raised,
                    random_float_between_alt(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                FUN_L00_0026dd20(&moby->pos, raised, 0x7f2f4f6f,
                    random_int_between(scale_game_frames(10), scale_game_frames(15)), 30000.0f);
            }
            mark_moby_for_removal_d7b80(moby);
            return;
        }
    }
    if (moby->pos.x < 2.0f || moby->pos.y < 2.0f || moby->pos.z < 2.0f ||
        moby->pos.x > 510.0f || moby->pos.y > 510.0f || moby->pos.z > 510.0f ||
        vector_distance_xy(&moby->pos, &D_L10_00167240) > 80.0f)
        mark_moby_for_removal_d7b80(moby);
}
#ifndef NOT_SDA
#endif
#ifndef MACRO_ADDR
#endif
struct Swirl_d8058 {
    char pad[0x18];
    float a;
    float b;
};
extern char *func_0020D348_d8058(int) __asm__("FUN_0020c4f8");
extern float func_002140F8_d8058(float, float) __asm__("FUN_002132a8");
extern int func_001F9850_d8058(int) __asm__("FUN_001f96f8");
extern void upd_a(void *) __asm__("FUN_L00_00250df8");
extern f32 D_L10_00161BEC_gp __asm__("D_L10_00161BEC") __attribute__((sda));
extern float D_0015EE6C_d8058 __asm__("D_0015ED6C");
int FUN_L10_002d8058(int owner, char *pos, char *vec) {
    char *moby = func_0020D348_d8058(0x3AA);
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0x7F;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(int *)(data + 0x10) = owner;
        *(short *)(data + 0x16) = 0;
        qcopy(moby + 0x10, pos);
        *(float *)(moby + 0x18) = *(float *)(moby + 0x18) + *(float *)&D_L10_00161BEC_gp;
        qcopy(data, vec);
        ((struct Swirl_d8058 *)data)->a =
            func_002140F8_d8058(D_0015EE6C_d8058 * 1.0471976f, D_0015EE6C_d8058 * 3.1415927f);
        ((struct Swirl_d8058 *)data)->b =
            func_002140F8_d8058(D_0015EE6C_d8058 * 1.0471976f, D_0015EE6C_d8058 * 3.1415927f);
        *(short *)(data + 0x14) = func_001F9850_d8058(0x3C);
        upd_a(moby);
    }
    return moby;
}
/* Ported from rac1-decomp src/overlays/l10_orxon/vendor_00296BD8.c (func_L10_002D9530) */


extern char D_L10_001742C0[];
extern char D_L10_00174300[];
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED70;
extern float FUN_001f9e90_u(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_00200510(float, float, float);
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern float seg_sqrt(float) __asm__("func_001F9988");
extern float D_0015ED6C;
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");

int FUN_L10_002d8170(char *m, char *c, float *previous_position) {
    float v0[4] __attribute__((aligned(16)));
    float v10[4] __attribute__((aligned(16)));
    float v20[4] __attribute__((aligned(16)));
    float v30[4] __attribute__((aligned(16)));
    float v40[4] __attribute__((aligned(16)));
    char *g;
    int w6;
    float p, q, r, t, u;

    float one = 1.0f;
    float k;

    *(u128 *)v0 = *(u128 *)previous_position;
    *(u128 *)v10 = 0;
    v10[2] = one;
    if (*(short *)(c + 0x14) == 0) {
      if (collision_line(v0, m + 0x10, 0x10, (int)m, 0)) {
        g = D_L10_001742C0;
        w6 = *(int *)(g + 0x18);
        if (w6 != 0) {
            subtract_vector_xyz(g + 0x40, m + 0x10, (void *)(w6 + 0x10));
            qcopy(m + 0x10, g + 0x20);
            normalize_vector_xyz(v20, g + 0x40, D_0015ED70 * 9.7f);
            add_vector_xyz(m + 0x10, m + 0x10, v20);
            FUN_L00_001ff660(c, c, g + 0x40);
            scale_vector_xyz_c(c, c, 0.35f);
            normalize_vector_xyz(v20, g + 0x40, one);
            dot_vectors_xyz(v20, v10);
            qcopy(v30, g + 0x40);
                k = 0.052359879f;
                p = fast_cos(*(float *)(m + 0x48));
                q = fast_sin(*(float *)(m + 0x48));
                v40[0] = v30[0] * p + v30[1] * q;
                k = 0.052359879f;
                p = fast_cos(*(float *)(m + 0x48));
                q = fast_sin(*(float *)(m + 0x48));
                v40[2] = v30[2];
                v40[1] = v30[1] * p - v30[0] * q;
                r = seg_sqrt(v40[0] * v40[0] + v40[2] * v40[2]);
                t = FUN_001f9e90_u(r, v40[1]);
                *(float *)(m + 0x40) = FUN_L00_00200510(*(float *)(m + 0x40), -t, k);
                u = FUN_001f9e90_u(v40[2], v40[0]);
                *(float *)(m + 0x44) = FUN_L00_00200510(*(float *)(m + 0x44), u, k);
                allocate_voice_for_target_entry(1, 0, (int)m);
            return 1;
        }
        if (*(int *)(g + 0x1C) > 0) {
            qcopy(m + 0x10, g + 0x20);
            normalize_vector_xyz(v20, g + 0x40, D_0015ED70 * 9.7f);
            add_vector_xyz(m + 0x10, m + 0x10, v20);
            FUN_L00_001ff660(c, c, g + 0x40);
            scale_vector_xyz_c(c, c, 0.5f);
            normalize_vector_xyz(v20, g + 0x40, one);
            if (0.707f < dot_vectors_xyz(v20, v10)) {
                r = probe_ground_height(m + 0x10, 0, 0.5f);
                if (*(float *)(m + 0x18) < r) *(float *)(m + 0x18) = r;
                if (*(float *)(c + 0x8) < 0.0f) *(float *)(c + 0x8) = *(float *)(c + 0x8) * -0.4f;
                if (vector_length_xyz(c) < D_0015ED6C) {
                    clear_u64_value((float *)c);
                    return 3;
                }
            }
            qcopy(v30, D_L10_00174300);
                k = 0.052359879f;
                p = fast_cos(*(float *)(m + 0x48));
                q = fast_sin(*(float *)(m + 0x48));
                v40[0] = v30[0] * p + v30[1] * q;
                k = 0.052359879f;
                p = fast_cos(*(float *)(m + 0x48));
                q = fast_sin(*(float *)(m + 0x48));
                v40[2] = v30[2];
                v40[1] = v30[1] * p - v30[0] * q;
                r = seg_sqrt(v40[0] * v40[0] + v40[2] * v40[2]);
                t = FUN_001f9e90_u(r, v40[1]);
                *(float *)(m + 0x40) = FUN_L00_00200510(*(float *)(m + 0x40), -t, k);
                u = FUN_001f9e90_u(v40[2], v40[0]);
                *(float *)(m + 0x44) = FUN_L00_00200510(*(float *)(m + 0x44), u, k);
                allocate_voice_for_target_entry(1, 0, (int)m);
            return 2;
        }
      }
    }
    return 0;
}
#ifndef NON_MATCHING
#include "rnc/math/vector.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002D9988), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef int u128_2D9988 __attribute__((mode(TI)));

typedef struct {
    float x, y, z;
} Vel_2D9988;

typedef struct {
    char pad0[0x10];
    u128_2D9988 q10;
} Obj_2D9988;

typedef struct {
    float v[4];
    char *owner;
    int x34;
    unsigned char s38;
    unsigned char s39;
    short s3a;
    float f3c;
    int i40;
} Shot_2D9988;

extern char *fast_arc_tan(void *, int, int) __asm__("FUN_001f9e90");
extern char D_L10_00178380[];
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float FUN_L00_0025ab48(float, float);
extern float scale_game_frames_alt(float) __asm__("FUN_001f96f8");
extern float D_L10_00161BEC_gp __asm__("D_L10_00161BEC") __attribute__((sda));
extern int FUN_L00_001f2868();
extern int set_moby_color() __asm__("FUN_L00_002502f0");
extern int get_moby_color() __asm__("FUN_L00_00250320");
extern int FUN_L00_0025a9f8(float, void *, int, void *, void *);
extern void FUN_L00_0025a420(void *, void *, void *);
extern void FUN_L00_0025f090(void *,void *,void *,int,int,int,int,int,float,float,float);
extern void add_vector_xyz_alt(void *) __asm__("FUN_001f9a10");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
int FUN_L10_002d8170(char *m, char *c, float *a2);
extern char D_0013E633_2D9988[] __asm__("D_0013F350");
extern char *func_L00_0025B478_2D9988(void *, int, int) __asm__("FUN_L00_0025a420");
extern int func_L10_002D9530_2D9988(char *, char *, float *) __asm__("FUN_L10_002d8170");
extern char D_L10_00178380[];
extern void func_001F9BD8_2D9988(void *, void *, void *) __asm__("FUN_001f9a10");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern float func_001F9F90_2D9988(float x) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2D9988(float) __asm__("FUN_001f9de0");
extern int func_001F9850_2D9988(int) __asm__("FUN_001f96f8");
extern int func_001F9938_2D9988(void *) __asm__("FUN_001f9770");
extern void func_L00_00251358_2D9988(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
extern void func_L00_00251328_2D9988(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void trooper_scale(void *, void *, float) __asm__("FUN_001f9c48");
extern int func_L00_001F2BE8_2D9988(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void func_L00_0025BA50_2D9988(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void func_0020D678_2D9988(void *) __asm__("FUN_0020c828");

void FUN_L10_002d85c8(char *m) {
    char *data;
    char *r16;
    int state;
    int flag21;
    float v0[4];
    float v10[4];
    Shot_2D9988 sb;
    float s50, s54, s58;
    int i5c, i60, i64;
    float s68, s6c, s70;
    int rA, rB;

    data = *(char **)(m + 0x78);
    if (*(float *)(m + 0x10) < 1.0f || 511.0f < *(float *)(m + 0x10) || *(float *)(m + 0x14) < 1.0f ||
        511.0f < *(float *)(m + 0x14) || *(float *)(m + 0x18) < 1.0f || 511.0f < *(float *)(m + 0x18)) {
        func_0020D678_2D9988(m);
        return;
    }
    *(float *)(m + 0x18) = *(float *)(m + 0x18) - D_L10_00161BEC_gp;
    qcopy(v0, m + 0x10);
    state = *(unsigned char *)(m + 0x20);
    switch (state) {
    case 0:
        func_001F9BD8_2D9988(m + 0x10, m + 0x10, data);
        *(float *)(data + 0x8) = *(float *)(data + 0x8) - D_0015ED70 * 9.8f;
        r16 = func_L00_0025B478_2D9988(m, 0x830000, 0);
        if (r16 != 0 && *(char **)(r16 + 0x20) != 0 && *(short *)(*(char **)(r16 + 0x20) + 0xA6) != *(short *)(m + 0xA6)) {
            float t;
            if (*(unsigned char *)(r16 + 0x28) == 0) {
                s50 = angle_atan2(*(float *)(m + 0x10) - ((Hero *)D_0013E633_2D9988)->motion.pos.f[0],
                                               *(float *)(m + 0x14) - ((Hero *)D_0013E633_2D9988)->motion.pos.f[1]);
                *(u128_2D9988 *)v10 = ((Obj_2D9988 *)r16)->q10;
                s58 = s54 = D_0015ED6C * 10.0f;
                trooper_knockback_arc(v10, &s50, &s54, &s58);
                *(float *)data = func_001F9F90_2D9988(s50) * s54;
                *(float *)(data + 0x4) = func_001F9FA8_2D9988(s50) * s54;
                ((Vel_2D9988 *)data)->z = 0.0f;
                *(float *)(data + 0x8) = s58;
                *(short *)(data + 0x14) = func_001F9850_2D9988(0xF);
                *(unsigned char *)(m + 0x20) = 2;
            } else {
                *(unsigned char *)(m + 0x20) = 3;
            }
        }
        *(unsigned char *)(m + 0xA4) = 0xFF;
        if (func_001F9938_2D9988(data + 0x14) != 0 && func_L10_002D9530_2D9988(m, data, v0) == 3) {
            *(short *)(data + 0x14) = func_001F9850_2D9988(0xF0);
            *(unsigned char *)(m + 0x20) = 1;
        } else if (*(float *)(m + 0x18) < 2.0f) {
            *(unsigned char *)(m + 0x20) = 3;
        }
        break;
    case 1:
        func_L00_00251358_2D9988(m, &i5c, &i60, &i64);
        rA = func_001F9850_2D9988(0xF0);
        rB = func_001F9850_2D9988(0xF0);
        func_L00_00251328_2D9988(m, ((rA - *(short *)(data + 0x14)) * 127) / rB, i60, i64);
        if (func_001F9938_2D9988(data + 0x14) != 0) {
            *(unsigned char *)(m + 0x20) = 3;
            break;
        }
        r16 = func_L00_0025B478_2D9988(m, 0x830000, 0);
        if (r16 != 0 && *(char **)(r16 + 0x20) != 0 && *(short *)(*(char **)(r16 + 0x20) + 0xA6) != *(short *)(m + 0xA6)) {
            float t;
            if (*(unsigned char *)(r16 + 0x28) == 0) {
                s68 = angle_atan2(*(float *)(m + 0x10) - ((Hero *)D_0013E633_2D9988)->motion.pos.f[0],
                                               *(float *)(m + 0x14) - ((Hero *)D_0013E633_2D9988)->motion.pos.f[1]);
                *(u128_2D9988 *)v10 = ((Obj_2D9988 *)r16)->q10;
                s70 = s6c = D_0015ED6C * 10.0f;
                trooper_knockback_arc(v10, &s68, &s6c, &s70);
                *(float *)data = func_001F9F90_2D9988(s68) * s6c;
                *(float *)(data + 0x4) = func_001F9FA8_2D9988(s68) * s6c;
                ((Vel_2D9988 *)data)->z = 0.0f;
                *(float *)(data + 0x8) = s70;
                *(short *)(data + 0x14) = func_001F9850_2D9988(0xF);
                *(unsigned char *)(m + 0x20) = 2;
            } else {
                *(unsigned char *)(m + 0x20) = 3;
            }
        }
        *(unsigned char *)(m + 0xA4) = 0xFF;
        break;
    case 2:
        func_L00_00251358_2D9988(m, &i5c, &i60, &i64);
        func_L00_00251328_2D9988(m, 0xFF, i60, i64);
        sb.owner = m;
        sb.x34 = 0x830000;
        sb.f3c = 1.0f;
        sb.i40 = 1;
        qcopy(sb.v, data);
        trooper_scale(sb.v, sb.v, 1.0f);
        flag21 = 0;
        sb.v[2] = 1.0f;
        sb.v[3] = 5627.925f;
        sb.s38 = state;
        sb.s39 = 1;
        sb.s3a = *(unsigned short *)(m + 0xA6);
        func_001F9BD8_2D9988(m + 0x10, m + 0x10, data);
        *(float *)(data + 0x8) = *(float *)(data + 0x8) - D_0015ED70 * 9.8f;
        if ((func_001F9938_2D9988(data + 0x14) != 0 && func_L10_002D9530_2D9988(m, data, v0) != 0) || *(float *)(m + 0x18) < 2.0f) {
            flag21 = 1;
        }
        if (flag21) {
            *(unsigned char *)(m + 0x20) = 3;
        }
        break;
    case 3:
        rA = func_L00_001F2BE8_2D9988(1.5f, m + 0x10, 0x10, m, 0);
        *(u128_2D9988 *)v10 = *(u128_2D9988 *)(m + 0x10);
        func_L00_0025BA50_2D9988(m, v10, D_L10_00178380, rA, 0, 0x810001, 2, 1, 1.0f, 1.0f, 1.0f);
        cart_burst(m, m + 0x10, 0, 0.5f, 13.0f);
        func_0020D678_2D9988(m);
        return;
    }
    *(float *)(m + 0x18) = *(float *)(m + 0x18) + D_L10_00161BEC_gp;
    if (*(float *)(m + 0x10) < 1.0f || 511.0f < *(float *)(m + 0x10) || *(float *)(m + 0x14) < 1.0f ||
        511.0f < *(float *)(m + 0x14) || *(float *)(m + 0x18) < 1.0f || 511.0f < *(float *)(m + 0x18)) {
        func_0020D678_2D9988(m);
    }
}
#else
extern void get_moby_color(char *, void *, int *, int *) __asm__("FUN_L00_00250320");
extern void set_moby_color(char *, int, int, int) __asm__("FUN_L00_002502f0");
extern char *find_hit_moby(char *, int, int) __asm__("FUN_L00_0025a420");
extern void bounce_vector(void *, float *, float *, float *) __asm__("FUN_L00_0025ab48");
extern int check_projectile_collision(float, void *, int, char *, int) __asm__("FUN_L00_001f2868");
extern void spawn_projectile_effect(float, float, float, char *, void *, int, int, int, int, int, int) __asm__("FUN_L00_0025a9f8");
extern void spawn_projectile_particles(float, float, char *, void *, int) __asm__("FUN_L00_0025f090");
extern float D_0015ED6C_d85c8 __asm__("D_0015ED6C");
extern float D_0015ED70_d85c8 __asm__("D_0015ED70");
extern void scale_collision_vector(float, void *, void *) __asm__("FUN_001f9c48");

void FUN_L10_002d85c8(char *m) {
    char *c = *(char **)(m + 0x78);
    float prev[4] __attribute__((aligned(16)));
    float hit_pos[4] __attribute__((aligned(16)));
    float angle;
    float speed;
    float height;
    float sine;
    int color0, color1, color2;
    char *hit;
    int state;
    int first;
    int second;
    int effect;
    struct {
        float velocity[4];
        char *owner;
        int color;
        unsigned char state;
        unsigned char active;
        unsigned short id;
        float scale;
        int one;
    } collision __attribute__((aligned(16)));

    if (*(float *)(m + 0x10) < 1.0f || *(float *)(m + 0x10) > 511.0f ||
        *(float *)(m + 0x14) < 1.0f || *(float *)(m + 0x14) > 511.0f ||
        *(float *)(m + 0x18) < 1.0f || *(float *)(m + 0x18) > 511.0f) {
        delete_moby(m);
        return;
    }
    *(float *)(m + 0x18) -= *(float *)&D_L10_00161BEC_gp;
    qcopy(prev, m + 0x10);
    state = *(unsigned char *)(m + 0x20);
    if (state == 1) goto state1;
    if (state < 2) {
        if (state == 0) goto state0;
        goto finish;
    }
    if (state == 2) goto state2;
    if (state == 3) goto state3;
    goto finish;
state0:

        add_vector_xyz(m + 0x10, m + 0x10, c);
        *(float *)(c + 8) -= D_0015ED70_d85c8 * 9.8f;
        hit = find_hit_moby(m, 0x830000, 0);
        if (hit != 0 && *(char **)(hit + 0x20) != 0 &&
            *(short *)(*(char **)(hit + 0x20) + 0xa6) != *(short *)(m + 0xa6)) {
            if (*(unsigned char *)(hit + 0x28) == 0) {
                angle = FUN_001f9e90_u(*(float *)(m + 0x10) - hero.motion.pos.f[0],
                                       *(float *)(m + 0x14) - hero.motion.pos.f[1]);
                *(OvlQuad *)hit_pos = *(OvlQuad *)(hit + 0x10);
                speed = D_0015ED6C_d85c8 * 10.0f;
                height = speed;
                bounce_vector(hit_pos, &angle, &speed, &height);
                *(float *)c = fast_cos(angle) * speed;
                sine = fast_sin(angle);
                *(float *)(c + 8) = 0.0f;
                *(float *)(c + 4) = sine * speed;
                *(float *)(c + 8) = height;
                *(short *)(c + 0x14) = scale_game_frames(0xf);
                *(unsigned char *)(m + 0x20) = 2;
            } else {
                *(unsigned char *)(m + 0x20) = 3;
            }
        }
        *(unsigned char *)(m + 0xa4) = 0xff;
        if (fast_dec_timer_s16(c + 0x14)) {
            if (FUN_L10_002d8170(m, c, prev) == 3) {
                *(short *)(c + 0x14) = scale_game_frames(0xf0);
                *(unsigned char *)(m + 0x20) = 1;
                goto finish;
            }
        }
        if (*(float *)(m + 0x18) < 2.0f) *(unsigned char *)(m + 0x20) = 3;
        goto finish;
state1:
    {
    float angle1, speed1, height1;

        get_moby_color(m, &color0, &color1, &color2);
        first = scale_game_frames(0xf0);
        second = scale_game_frames(0xf0);
        set_moby_color(m, ((first - *(short *)(c + 0x14)) * 0x7f) / second, color1, color2);
        if (fast_dec_timer_s16(c + 0x14)) {
            *(unsigned char *)(m + 0x20) = 3;
        } else {
            hit = find_hit_moby(m, 0x830000, 0);
            if (hit != 0 && *(char **)(hit + 0x20) != 0 &&
                *(short *)(*(char **)(hit + 0x20) + 0xa6) != *(short *)(m + 0xa6)) {
                if (*(unsigned char *)(hit + 0x28) == 0) {
                    angle1 = FUN_001f9e90_u(*(float *)(m + 0x10) - hero.motion.pos.f[0],
                                           *(float *)(m + 0x14) - hero.motion.pos.f[1]);
                    *(OvlQuad *)hit_pos = *(OvlQuad *)(hit + 0x10);
                    speed1 = D_0015ED6C_d85c8 * 10.0f;
                    height1 = speed1;
                    bounce_vector(hit_pos, &angle1, &speed1, &height1);
                    *(float *)c = fast_cos(angle1) * speed1;
                    sine = fast_sin(angle1);
                    *(float *)(c + 8) = 0.0f;
                    *(float *)(c + 4) = sine * speed1;
                    *(float *)(c + 8) = height1;
                    *(short *)(c + 0x14) = scale_game_frames(0xf);
                    *(unsigned char *)(m + 0x20) = 2;
                } else {
                    *(unsigned char *)(m + 0x20) = 3;
                }
            }
            *(unsigned char *)(m + 0xa4) = 0xff;
        }
        }
    goto finish;
state2:
    {
    int done;

        get_moby_color(m, &color0, &color1, &color2);
        set_moby_color(m, 0xff, color1, color2);
        collision.owner = m;
        collision.color = 0x830000;
        collision.scale = 1.0f;
        collision.one = 1;
        qcopy(collision.velocity, c);
        scale_collision_vector(1.0f, collision.velocity, collision.velocity);
        done = 0;
        collision.velocity[2] = 1.0f;
        collision.velocity[3] = 5627.925f;
        collision.state = state;
        collision.active = 1;
        collision.id = *(unsigned short *)(m + 0xa6);
        add_vector_xyz(m + 0x10, m + 0x10, c);
        *(float *)(c + 8) -= D_0015ED70_d85c8 * 9.8f;
        if (fast_dec_timer_s16(c + 0x14)) {
            if (FUN_L10_002d8170(m, c, prev) != 0) done = 1;
            else if (*(float *)(m + 0x18) < 2.0f) done = 1;
        } else if (*(float *)(m + 0x18) < 2.0f) done = 1;
        if (done) *(unsigned char *)(m + 0x20) = 3;
    }
        goto finish;
state3:

        effect = check_projectile_collision(1.5f, m + 0x10, 0x10, m, 0);
        *(OvlQuad *)hit_pos = *(OvlQuad *)(m + 0x10);
        spawn_projectile_effect(1.0f, 1.0f, 1.0f, m, hit_pos, 0x178380, effect,
                                0, 0x810001, 2, 1);
        spawn_projectile_particles(0.5f, 13.0f, m, m + 0x10, 0);
        delete_moby(m);
        return;

finish:
    *(float *)(m + 0x18) += *(float *)&D_L10_00161BEC_gp;
    if (*(float *)(m + 0x10) < 1.0f || *(float *)(m + 0x10) > 511.0f ||
        *(float *)(m + 0x14) < 1.0f || *(float *)(m + 0x14) > 511.0f ||
        *(float *)(m + 0x18) < 1.0f || *(float *)(m + 0x18) > 511.0f)
        delete_moby(m);
}
#endif
/* Electric spark: runs along its wire path with a light and a buzzing sound, then rests for a random time. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DA0F0), where it is exact; names translated to the US level program. */


extern char D_L10_00180740[];
extern char D_L10_001DD120[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern int *D_L10_001B0930[];
extern int D_L10_0015F5CC;
extern int FUN_L00_0023e738(float *, float, float, float, float, float);
extern int FUN_L00_00262360();
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void FUN_L00_0023e838(int);
unsigned char *FUN_L10_002dd270(int a, void *pos, int b, float x, float y, float z);
extern int func_001E9730_2da0f0(void *, int) __asm__("FUN_001e93b0");
extern int func_001F9908_v(void *) __asm__("FUN_001f9740");
extern void func_L00_002633D8_f(float, float, void *, int *, float *,
                                void *) __asm__("FUN_L00_00262360");
extern int trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");

void FUN_L10_002d8d30(char *m) {
    char *d = *(char **)(m + 0x78);
    int *path;
    float pos[4];
    int seg;
    float frac;
    float len;
    if (*(int *)d == -1) {
        func_001E9730_2da0f0(D_L10_001DD120, *(short *)(m + 0xB2));
        delete_moby(m);
        return;
    }
    path = D_L10_001B0930[*(int *)d];
    ((unsigned char *)m)[0x30] = 0xFF;
    switch (((unsigned char *)m)[0x20]) {
    case 0:
        if (*(int *)(d + 0xC) != -1) {
            FUN_L00_0023e838(*(int *)(d + 0xC));
            *(int *)(d + 0xC) = -1;
        }
        if (*(int *)(d + 0x10) != -1) {
            char *e = ((char *)&D_0013E550) + *(int *)(d + 0x10) * 0x70;
            if (*(char **)(e + 0x88) == m && ((unsigned char *)e)[0x74] != 0) {
                release_voice_slot(*(int *)(d + 0x10));
            }
        }
        *(int *)(d + 0x10) = -1;
        if (func_001F9908_v(d + 8) != 0) {
            m[0x20] = 1;
            *(int *)(d + 4) = 0;
        }
        break;
    case 1:
        len = distance_xyz((char *)path + 0x10, (char *)path + 0x20);
        *(float *)(d + 4) += frame_time * 5.0f;
        func_L00_002633D8_f(*(float *)(d + 4), len, path, &seg, &frac, pos);
        if (*(int *)(d + 0xC) == -1) {
            *(int *)(d + 0xC) = FUN_L00_0023e738(pos, 3.0f, 0.0f, 1.0f, 1.0f, 0.5f);
        } else {
            char *l = D_L10_00180740 + *(int *)(d + 0xC) * 32;
            qcopy(l + 0x10, pos);
            *(float *)(l + 0) = 1.0f;
            *(float *)(l + 0x1C) = 3.0f;
            *(float *)(l + 4) = 1.0f;
            *(float *)(l + 8) = 0.5f;
            *(int *)(l + 0xC) = 0;
        }
        if (random_integer_below(2) != 0)
            return;
        if (D_L10_0015F5CC & 1) {
            FUN_L10_002dd270(m, pos, path, *(float *)(d + 4), frame_time * 5.0f, len);
        }
        if (voice_is_playing(m, *(int *)(d + 0x10)) == 0) {
            int r = trooper_play_sound(0, 0xC, m, 0x442);
            *(int *)(d + 0x10) = r;
            if (r != -1) {
                *(OvlQuad *)(((char *)&voice_positions) + r * 0x70) = *(OvlQuad *)pos;
            }
        } else {
            *(OvlQuad *)(((char *)&voice_positions) + *(int *)(d + 0x10) * 0x70) = *(OvlQuad *)pos;
        }
        if (seg == *path - 1) {
            m[0x20] = 0;
            *(int *)(d + 8) = scale_game_frames(0x78);
        }
        break;
    }
}
/* Test whether a point lies within a box around a moby, in the moby's local frame. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DA3C0), where it is exact; names translated to the US level program. */


extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");

extern float D_L10_00161C0C __attribute__((sda));
extern float D_L10_00161C10 __attribute__((sda));
int FUN_L10_002d9000(struct Moby *m, float *p) {
    float a[4] __attribute__((aligned(16)));
    float t10[16] __attribute__((aligned(16)));
    float t50[4] __attribute__((aligned(16)));
    float t60[4] __attribute__((aligned(16)));
    *(OvlQuad *)a = *(OvlQuad *)p;
    transpose_rotation_matrix(t10, &m->rot_row0);
    subtract_vector_xyz(t60, a, &m->pos);
    *(OvlQuad *)t50 = *(OvlQuad *)t60;
    transform_vector(t50, t50, t10);
    if (absolute_float(t50[0]) < D_L10_00161C10) {
        return absolute_float(t50[1]) < D_L10_00161C0C;
    }
    return 0;
}
/* update for a moby that glides between two heights and sets a flag */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DA468), where it is exact; names translated to the US level program. */


extern char D_0013D388[];
char D_L10_001DD160[] = "Moby Class %d Instance %d has no pVars!  May be in the art file.\n\tMobyDeleted\n";
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int FUN_L00_002591d0(int *, int, int, int);
extern int FUN_L00_002592b8(int *, int, int, int);
extern int FUN_L10_002d9000_c() __asm__("FUN_L10_002d9000");
extern OvlQuad D_L10_00167240;
void mark_moby_for_removal_u(struct Obj *obj) __asm__("FUN_0020c828");

void lab_door_update(unsigned char *m) __asm__("FUN_L10_002d90a8");
void lab_door_update(unsigned char *m) {
    int *d = *(int **)(m + 0x78);
    OvlQuad v[2];
    int it;
    int flag;
    int hit;
    if (d == 0) {
        stub_printf(D_L10_001DD160, *(short *)(m + 0xA6), *(short *)(m + 0xB2));
        mark_moby_for_removal_u(m);
        return;
    }
    switch (m[0x20]) {
    case 0:
        *(OvlQuad *)d = *(OvlQuad *)(m + 0x10);
        m[0x20] = 2;
        break;
    case 1:
        flag = 0;
        {
            float f = *(float *)(m + 0x18) - 4.0f / multiply_global_factor_ed64(20.0f);
            *(float *)(m + 0x18) = f;
            if (f <= *(float *)(d + 2)) {
                *(float *)(m + 0x18) = *(float *)(d + 2);
                m[0x20] = 2;
            }
        }
        if (d[4] != -1) {
            if (FUN_L00_002591d0(&it, d[4], 0, 0)) {
                do {
                    v[0] = *(OvlQuad *)(it + 0x10);
                    if (FUN_L10_002d9000_c(m, &v[0])) {
                        flag = 1;
                        break;
                    }
                } while (FUN_L00_002592b8(&it, it, 0, 0));
            }
        }
        hit = 0;
        if (flag == 0) {
            v[0] = D_L10_00167240;
            if (FUN_L10_002d9000_c(m, &v[0]) == 0) {
                v[1] = *(OvlQuad *)&hero.motion.pos;
                if (FUN_L10_002d9000_c(m, &v[1]) == 0)
                    goto done;
            }
        }
        hit = 1;
    done:
        if (hit)
            m[0x20] = 3;
        break;
    case 2:
        break;
    case 3: {
        float f = 4.0f / multiply_global_factor_ed64(20.0f);
        float g = *(float *)(m + 0x18) + f;
        float h;
        *(float *)(m + 0x18) = g;
        h = *(float *)(d + 2) + 4.0f;
        if (h <= g) {
            *(float *)(m + 0x18) = h;
            m[0x20] = 4;
            if (d[5] != -1)
                D_0013D388[d[5] + 0x4C] = 1;
        }
        break;
    }
    case 4:
        break;
    }
}

/* Pvars of a floor plate that opens a door while something stands on it. */
typedef struct {
    s32 door;           /* 0x00: level moby index of the door, -1 for none */
    s32 link;           /* 0x04: level moby index of a plate it holds down, -1 for none */
    u8 pad8[4];
    s32 held;           /* 0x0C: set by a linked plate */
    s32 sound_timer;    /* 0x10 */
} FloorPlateVars;

/* Result of the last probe_ground_height: the moby that was hit at 0x18. */
typedef struct {
    u8 pad0[0x18];
    struct Moby *moby;  /* 0x18 */
} GroundHit;

extern struct Moby *D_L10_0015FFD8;
extern struct Moby *nearby_mobys[] __asm__("D_L10_00178380");
extern GroundHit ground_hit __asm__("D_L10_001742C0");
extern u8 D_0014C050[][16];
extern void FUN_L00_002502f0_u(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern f32 fabsf_fast(f32) __asm__("FUN_001f99c0");
extern s32 FUN_0022da68(s32, s32, struct Moby *);

/* Floor plate: lit and opening its door while a moby (other than class 0x3EF) rests on it, dark and closing it otherwise. */
void button_update(struct Moby *m) __asm__("FUN_L10_002d92b8");
void button_update(struct Moby *m) {
    FloorPlateVars *v = (FloorPlateVars *)m->pvars;
    s32 pressed = 0;
    struct Moby *door;
    struct Moby *other;
    s32 i;
    f32 h;
    s32 n;

    if (v->door == -1) {
        FUN_L00_002502f0_u(m, 8, 8, 8);
        return;
    }
    door = &D_L10_0015FFD8[v->door];
    n = collect_mobies_in_sphere(&m->pos, 1.0f, 0, m, NULL);
    if (n != 0) {
        for (i = 0; i < n; i++) {
            other = nearby_mobys[i];
            h = probe_ground_height(&other->pos, 0, 0.5f);
            if (ground_hit.moby == m && fabsf_fast(h - nearby_mobys[i]->pos.z) < 0.1f) {
                if (nearby_mobys[i]->oclass != 0x3EF) {
                    pressed = 1;
                }
            }
        }
    }
    FUN_001f9740(&v->sound_timer);
    if (D_0014C050[current_level_index][m->mission] == 0xFF) {
        pressed = 1;
    }
    if (pressed || v->held != 0) {
        if (pressed && v->link != -1) {
            ((FloorPlateVars *)D_L10_0015FFD8[v->link].pvars)->held = 1;
        } else {
            v->held = 0;
        }
        if (door->state == 1 || door->state == 2) {
            door->state = 3;
            if (v->sound_timer == 0) {
                FUN_0022da68(0, 0, door);
                FUN_0022da68(0, 0, m);
                v->sound_timer = scale_game_frames(30);
            }
        }
        FUN_L00_002502f0_u(m, 0x7F, 0x7F, 0x7F);
    } else {
        if (door->state == 4) {
            door->state = 1;
            if (v->sound_timer == 0) {
                FUN_0022da68(1, 0, door);
                FUN_0022da68(0, 0, m);
                v->sound_timer = scale_game_frames(30);
            }
        }
        FUN_L00_002502f0_u(m, 8, 8, 8);
    }
}
/* Ported from rac1-decomp src/overlays/l10_orxon/vendor_00296BD8.c (func_L10_002DB278) */

/* Orxon moby update: destroys itself when its item is taken, else fires its effect once per pass. */

typedef struct { char pad0[0x454]; unsigned char collected[1]; } L10State;

typedef union { u128 q; float f[4]; } UVec;

extern L10State D_L10_001BB6B0;
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *FUN_L10_002dd848_c(char *owner) __asm__("FUN_L10_002dd848");
extern char *FUN_L10_002eb8c0(char *owner);
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern int D_0015ED84; /* no foreign declaration */
extern int D_L10_001BA950[];
extern unsigned char D_0014C190_c[] __asm__("D_0014C190");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void spawn_debris_chunk(void *, void *, unsigned int, float, int, float, float, float, int) __asm__("FUN_L01_002f8530");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");

void FUN_L10_002d9eb8(char *moby) {
    typedef union { u128 q; float f[4]; } UVec;
    UVec zero_vector, transformed_offset, v40, translated_position;
    unsigned short id = *(unsigned short *)(moby + 0xB2);
    int n;
    int k;
    int c16;
    int r2;
    float f20, f21;

    if (D_L10_001BB6B0.collected[(short)id] != 0
        || ((*(int *)(D_0014C190_c + (((short)id >> 5) * 4 + (D_0015ED84 << 8))) >> (id & 0x1F)) & 1)) {
        FUN_L10_002dd848_c(moby);
        FUN_L10_002eb8c0(moby);
        delete_moby(moby);
        return;
    }
    if (find_moby_hit(moby, 0x40000, 0)) {
        char *c0 = moby + 0xC0;
        zero_vector.q = 0;
        spawn_explosion(moby, &zero_vector, moby + 0x10, 0.0f, 0.0f, 5, 3, 4, 4.0f, 2.0f, 100000.0f, 3.0f, -1, 15.0f, 1, 1, -1, 0);
        trooper_play_sound(0, 0, moby, 0x99);
        n = 0x1D;
        do {
            k = 0x70F;
            v40.q = 0;
            v40.f[0] = -1.0f;
            v40.f[1] = random_float_between(-3.0f, 3.0f);
            v40.f[2] = random_float_between(-3.0f, 3.0f);
            n--;
            transformed_offset.q = v40.q;
            transform_vector_by_basis(&transformed_offset, &transformed_offset, c0);
            normalize_vector_xyz(&v40, &transformed_offset, random_float_between(5.0f, 10.0f) * D_0015ED6C);
            FUN_L00_001ff290(&translated_position, &transformed_offset, moby + 0x10);
            if (random_integer_below(1) == 0)
                k = 0x70E;
            f21 = random_float_between(0.5f, 1.0f);
            c16 = random_int_between(0x3C, 0x78);
            f20 = random_float_between(2.0f, 3.0f);
            r2 = random_integer_below(4);
            spawn_debris_chunk(&transformed_offset, &v40, k, f21, c16, f20, 1.0f, 3.0f, r2 == 0);
        } while (n >= 0);
        transformed_offset.q = 0;
        transformed_offset.f[0] = 2.8599999f;
        transformed_offset.f[1] = 1.57000005f;
        transformed_offset.f[2] = 1.83099997f;
        transform_vector_by_basis(&transformed_offset, &transformed_offset, c0);
        spawn_explosion(moby, &zero_vector, &transformed_offset, 0.0f, 0.0f, 5, 3, 4, 4.0f, 2.0f, 100000.0f, 3.0f, -1, 15.0f, 1, 1, -1, 0);
        FUN_L10_002dd848_c(moby);
        FUN_L10_002eb8c0(moby);
        *(int *)(D_0014C190_c + (((short)*(unsigned short *)(moby + 0xB2) >> 5) * 4 + (D_0015ED84 << 8))) |= 1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
        D_L10_001BA950[(short)*(unsigned short *)(moby + 0xB2) >> 5] |= 1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
        delete_moby(moby);
    }
}
extern char D_L10_001DD1D0[] __asm__("D_L10_001DD1D0");
extern char D_L10_001DD210[] __asm__("D_L10_001DD210");
extern unsigned char D_L10_00161CA8[] __asm__("D_L10_00161CA8");
extern unsigned char D_001413F4[];
void FUN_L10_002da690(u8 *moby);
extern int stub_printf() __asm__("FUN_001e93b0");
extern void delete_moby(void *) __asm__("FUN_0020c828");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_001fa6d0(float);
extern float square_root(float) __asm__("FUN_001f9988");
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern int fast_bsphere_check(void *, float) __asm__("FUN_001fa728");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern int tick_countdown_32(void *) __asm__("FUN_001f9740");

extern float D_L10_00161C98 __attribute__((sda));
void FUN_L10_002da2c8(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    union { unsigned char order[10]; float position2[4]; } scratch;
    float position1[4];
    int pass, i, copy_index;

    if (*(int *)(data + 0xC) == -1) {
        stub_printf(D_L10_001DD1D0);
        stub_printf(D_L10_001DD210, *(short *)&moby->save_id);
        delete_moby(moby);
    }
    switch (moby->state) {
    case 0:
        if (current_level_index == 10 && D_001413F4[0] == 1) {
            delete_moby(moby);
            break;
        }
        memcpy(scratch.order, D_L10_00161CA8, 10);
        for (pass = 0; pass < 10; pass++) {
            for (i = 0; i < random_integer_below(9); i++) {
                unsigned char tmp = scratch.order[i + 1];
                scratch.order[i + 1] = scratch.order[i];
                scratch.order[i] = tmp;
            }
        }
        copy_index = 0;
        if (*(int *)(data + 4) > 0) {
            char *dest = data + 0x10;
            do {
                dest[copy_index] = scratch.order[copy_index];
                copy_index++;
            } while (copy_index < *(int *)(data + 4));
        }
        {
            float timer;
            float new_rotation = fast_add_rotations(moby->rot.z, 1.5707964f);
            moby->update_dist = 0x60;
            moby->state = 1;
            timer = D_L10_00161C98 * 60.0f;
            moby->rot.z = new_rotation;
            *(int *)(data + 0x30) = -1;
            *(int *)(data + 0x34) = scale_game_frames(FUN_001fa6d0(timer));
        }

        moby->flags |= 0x41;
        break;
    case 1: {
        float half_height, half_width, radius_squared;
        half_height = (float)*(int *)(data + 4) * *(float *)(data + 8) * 0.5f;
        half_width = *(float *)data * 0.5f;
        *(OvlQuad *)position1 = *(OvlQuad *)&moby->pos;
        radius_squared = half_height * half_height;
        position1[2] += half_height;
        position1[3] = square_root(radius_squared + half_width * half_width);
        if (fast_vec_dist(&moby->pos, &D_L10_00167240) < 80.0f && 0.0f <= (float)fast_bsphere_check(position1, 80.0f))
            enqueue_callback_list_1((void *)FUN_L10_002da690, moby);
        if (voice_is_playing(moby, *(int *)(data + 0x30)) == 0)
            *(int *)(data + 0x30) = allocate_voice_for_target_entry(0, 4, (int)moby);
        break;
    }
    case 2: {
        int voice;
        float half_height, half_width, radius_squared;
        tick_countdown_32(data + 0x34);
        voice = *(int *)(data + 0x30);
        if (voice != -1) {
            unsigned char *entry = (unsigned char *)D_0013E550 + voice * 0x70;
            if (*(struct Moby **)(entry + 0x88) == moby && entry[0x74] != 0)
                release_voice_slot(voice);
        }
        *(int *)(data + 0x30) = -1;
        half_height = (float)*(int *)(data + 4) * *(float *)(data + 8) * 0.5f;
        half_width = *(float *)data * 0.5f;
        *(OvlQuad *)scratch.position2 = *(OvlQuad *)&moby->pos;
        radius_squared = half_height * half_height;
        scratch.position2[2] += half_height;
        scratch.position2[3] = square_root(radius_squared + half_width * half_width);
        if (fast_vec_dist(&moby->pos, &D_L10_00167240) < 80.0f && 0.0f <= (float)fast_bsphere_check(scratch.position2, 80.0f))
            enqueue_callback_list_1((void *)FUN_L10_002da690, moby);
        break;
    }
    }
}
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DBA50), where it is exact; names translated to the US level program. */

typedef int u128_c __attribute__((mode(TI)));

typedef int OvlQuad_c __attribute__((mode(TI)));

extern f32 D_L10_00161D60;
extern f32 D_L10_001D9FF0[];
extern f32 D_L10_001DA010[];
extern s32 D_L10_00161D64;
extern s32 D_L10_00161D68;
extern s32 fast_tween_color(s32, s32, f32) __asm__("FUN_001fa6e0");
extern int D_L10_00161C70 __attribute__((sda));
extern int D_L10_00161C74 __attribute__((sda));
extern int D_L10_00161C78 __attribute__((sda));
extern int D_L10_00161C7C __attribute__((sda));
extern int D_L10_00161C90 __attribute__((sda));
extern int D_L10_00161C94 __attribute__((sda));

extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b48");
extern void rotation_matrix_y(void *, f32) __asm__("FUN_001f9ff8");
extern void rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern s32 truncate_float_to_s32_da690(f32) __asm__("FUN_001fa6d0");
extern f32 multiply_global_factor(f32) __asm__("FUN_001f96b0");
extern u64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");

void FUN_L10_002da690(u8 *moby) {
    char *data = *(char **)(moby + 0x78);
    char *base = *(char **)((*(int *)(data + 0xc) << 8) + (int)D_L10_0015FFD8 + 0x78);
    char scratch[(unsigned int)*(int *)(data + 4) * 0x40];
    struct {
        unsigned char vertices[0x50];
        float matrix1[16];
        union {
            unsigned char quad[0x90];
            struct { unsigned char vertices[0x50]; float matrix2[16]; } fields;
        } packet;
        float rotation[16];
    } buffers __attribute__((aligned(16)));
    int i, j, group, row, vertex;
    float fade, phase;
    unsigned char *order;
    int group_age;
    char *age_base;
    char *ages;
    int group_vertices;
    float *matrix1;
    int next_group, next_row;
    float *matrix2, *uv1, *uv2;

    {
        float distance = vector_distance(moby + 0x10, (u8 *)&D_L10_00167240);
        if (distance > 48.0f) fade = (80.0f - distance) * 0.03125f;
        else fade = 1.0f;
    }
    rotation_matrix_y(buffers.rotation, *(float *)data * 0.5f);
    matrix1 = buffers.matrix1;
    matrix2 = buffers.packet.fields.matrix2;
    uv1 = matrix1 + 1;
    uv2 = matrix2 + 1;
    {
        int n;
        for (n = 0; n < *(int *)(data + 4); n++) {
            rotation_matrix(scratch + n * 0x40, moby + 0x40);
            multiply_matrix(scratch + n * 0x40, scratch + n * 0x40, buffers.rotation);
            *(u128_c *)(scratch + n * 0x40 + 0x30) = *(u128_c *)(moby + 0x10);
            *(float *)(scratch + n * 0x40 + 0x38) += convert_integer_to_float(n + 1) * *(float *)(data + 8);
        }
    }
    {
        u64 texture = get_effect_texture(14);
        u64 packet_flags = ((u64)D_L10_00161C70) | ((u64)D_L10_00161C74 << 2) |
                ((u64)D_L10_00161C78 << 4) | ((u64)D_L10_00161C7C << 6) | 0x8000000000ULL;
        ((u64 *)buffers.packet.fields.matrix2)[5] = texture;
        ((u64 *)buffers.matrix1)[5] = texture;
        ((u64 *)buffers.packet.fields.matrix2)[6] = 0xff9000000260ULL;
        ((u64 *)buffers.matrix1)[6] = 0xff9000000260ULL;
        ((u64 *)buffers.packet.fields.matrix2)[4] = 0;
        ((u64 *)buffers.matrix1)[4] = 0;
        ((u64 *)buffers.packet.fields.matrix2)[7] = packet_flags;
        ((u64 *)buffers.matrix1)[7] = packet_flags;
    }
    {
        int k;
        for (k = 0; k < 4; k++) {
            uv1[k * 2] = uv2[k * 2] = D_L10_001D9FF0[k * 2 + 1];
        }
    }
    group = 0;
    do {
        next_group = group + 1;
        phase = 0.0f;
        row = 0;
        do {
            int next;
            phase += D_L10_00161D60;
            if (phase > 1.0f) phase -= 1.0f;
            next_row = row + 1;
            for (j = 0; j < 4; j++) {
                matrix1[j * 2] = matrix2[j * 2] =
                    D_L10_001D9FF0[j * 2] + phase + *(float *)(base + 0x12de0);
            }
            for (i = 0; i < *(int *)(data + 4); i = next) {
                int offset;
                int age;
                float progress;
                unsigned int color;
                order = (unsigned char *)(data + 0x10);
                group_age = group * 0x78;
                age_base = base + 0x10000;
                ages = age_base + 0x2c00;
                age = *(int *)(ages + (order[i] * 4 + group_age));
                group_vertices = group * 0x2580;
                if (age > D_L10_00161D68) {
                    progress = convert_integer_to_float(age - D_L10_00161D68) /
                               convert_integer_to_float(D_L10_00161D64 - D_L10_00161D68);
                } else {
                    progress = 1.0f - convert_integer_to_float(age) /
                               convert_integer_to_float(D_L10_00161D68);
                }
                color = fast_tween_color(D_L10_00161C90, D_L10_00161C94, progress);
                color = (color & 0xffffff) +
                        (truncate_float_to_s32_da690(fade * fade *
                         convert_integer_to_float((int)color >> 24)) << 24);
                if (moby[0x20] == 2) {
                    float alpha = convert_integer_to_float(*(int *)(data + 0x34));
                    float duration = multiply_global_factor(D_L10_00161C98 * 60.0f);
                    color = (truncate_float_to_s32_da690(alpha * 255.0f / duration) << 24) | 0xffffff;
                }
                vertex = 0;
                offset = i * 0x40;
                next = i + 1;
                *(unsigned int *)(buffers.packet.quad + 0x4c) = color;
                *(unsigned int *)(buffers.packet.quad + 0x48) = color;
                *(unsigned int *)(buffers.packet.quad + 0x44) = color;
                *(unsigned int *)(buffers.packet.quad + 0x40) = color;
                for (; vertex < 4; vertex++) {
                    add_vector_xyz(buffers.packet.quad + vertex * 0x10, (char *)D_L10_001DA010 + vertex * 0x10,
                        base + group_vertices + (row + (vertex >> 1)) * 0x140 + order[i] * 0x10);
                }
                draw_geometry_quad(buffers.packet.quad, scratch + offset, 0);
            }
            row = next_row;
        } while (row < 19);
        group = next_group;
    } while (group < 4);
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L10_002dcc58.s", FUN_L10_002dcc58);
#else
extern unsigned char D_001413F4[];
extern int D_L10_00161D64;
extern int D_L10_00161D68;
extern float D_L10_00161D54 __attribute__((sda));
extern float D_L10_00161D58 __attribute__((sda));
extern float D_L10_00161D5C __attribute__((sda));
extern void FUN_001f28c8(void *) __asm__("FUN_001f99f8");
extern void FUN_001f28e0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f2968(float, void *, void *) __asm__("FUN_001f9a68");
extern void FUN_L10_0024a7f0(float, float, void *) __asm__("FUN_L00_00257d78");

void FUN_L10_002dcc58(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float scratch[0xC80 / 4];
    int group;
    long item;
    float phase;

    switch (moby->state) {
    case 0:
        if (current_level_index == 10 && D_001413F4[0] == 1) {
            delete_moby(moby);
        } else {
            int value = 0;
            long increment = 45;
            int item;
            D_L10_00161D68 = 0x16;
            D_L10_00161D64 = 0x2d;
            for (item = 0; item < 10; item++) {
                int quotient = value / 10;
                *(int *)(data + 0x12c00 + item * 4) = quotient;
                *(int *)(data + 0x12c78 + item * 4) = quotient + 11;
                *(int *)(data + 0x12cf0 + item * 4) = quotient + 23;
                *(int *)(data + 0x12d68 + item * 4) = quotient + 34;
                value += increment;
            }
            moby->state = 1;
        }
        break;
    case 1:
        phase = *(float *)(data + 0x12de0) + D_L10_00161D5C * frame_time;
        *(float *)(data + 0x12de0) = phase;
        if (phase > 1.0f) *(float *)(data + 0x12de0) = phase - 1.0f;
        {
            int group_table = 0x12c00;
            for (group = 0; group < 4; group++) {
                int group_offset = group * 0x2580;
                int timer_offset = group * 0x78;
                for (item = 0; item < 10; item++) {
                    char *entry = (data + group_table) + timer_offset + item * 4;
                    char *base = data + group_offset;
                    char *other = data + (group_offset + 0x9600);
                    char *buffer = (char *)scratch + item * 0x10;
                    if (fast_dec_timer(entry) != 0) {
                        char *dst = data + (item * 0x10 + group_offset);
                        char *current = base + item * 0x10;
                        char *out = buffer;
                        int row;
                        for (row = 0; row < 20; row++) {
                            float phase;
                            float value;
                            float quotient, half;
                            FUN_001f28c8(current);
                            current += 0x140;
                            phase = D_L10_00161D58;
                            value = D_L10_00161D54 * frame_time;
                            quotient = phase / 20.0f;
                            half = phase * 0.5f;
                            *(float *)(dst + 0xc) = 1.0f;
                            *(float *)(dst + 4) = (float)row * quotient - half;
                            FUN_L10_0024a7f0(0.0f, value, out);
                            dst += 0x140;
                            out += 0xa0;
                        }
                        {
                            char *dst = other + item * 0x10 + 0x140;
                            char *current = buffer;
                            char *out = buffer + 0x140;
                            int row;
                            for (row = 17; row >= 0; row--) {
                                FUN_001f28e0(dst, current, current + 0xa0);
                                FUN_001f28e0(dst, dst, out);
                                FUN_001f2968(0.333f, dst, dst);
                                dst += 0x140;
                                current += 0xa0;
                                out += 0xa0;
                            }
                        }
                        *(int *)entry = D_L10_00161D64;
                    } else {
                        char *dst = base + item * 0x10 + 0x140;
                        char *out = other + item * 0x10 + 0x140;
                        int row;
                        for (row = 17; row >= 0; row--) {
                            FUN_001f28e0(dst, dst, out);
                            dst += 0x140;
                            out += 0x140;
                        }
                    }
                }
            }
        }
        break;
    }
}
#endif

/* Spawns a moby at a spot, fills its data block and starts its fade and sound. */
extern float D_0015ED6C_dd __asm__("D_0015ED6C");
extern unsigned char *FUN_0020c4f8_dd(int) __asm__("FUN_0020c4f8");
extern void glow_n(unsigned char *, int, int, int) __asm__("FUN_L00_002502f0");
extern int FUN_001fa6d0_dd(float) __asm__("FUN_001fa6d0");
extern float FUN_002132a8_dd(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_0026daa0_dd(unsigned char *, int, int, int, float) __asm__("FUN_L00_0026daa0");

unsigned char *FUN_L10_002dd270(int a, void *pos, int b, float x, float y, float z) {
    unsigned char *o = FUN_0020c4f8_dd(0x442);
    int *d;
    float f;
    if (o != 0) {
        o[0x30] = 0xFF;
        *(short *)(o + 0x32) = 0xFF;
        o[0x31] = 1;
        o[0x23] = 0x40;
        o[0x20] = 0;
        d = *(int **)(o + 0x78);
        *(OvlQuad *)(o + 0x10) = *(OvlQuad *)pos;
        glow_n(o, 0x7F, 0x40, 0);
        f = 10.0f / (D_0015ED6C_dd * 5.0f);
        d[0] = b;
        ((float *)d)[1] = x;
        ((float *)d)[2] = y;
        ((float *)d)[3] = z;
        d[4] = a;
        d[5] = FUN_001fa6d0_dd(f);
        ((float *)d)[6] = FUN_002132a8_dd(0.999f, 0.97f);
        FUN_L00_0026daa0_dd(o, 0x1F4F7F7F, FUN_001fa6d0_dd(10.0f / (D_0015ED6C_dd * 5.0f)), -1, 126000.008f);
    }
    return o;
}
/* Advances an effect, scales its size over its lifetime and emits hits. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DE798), where it is exact; names translated to the US level program. */


typedef struct { int owner; float phase,speed,size; int source,timer; float growth; } EffectData;

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void send_blast_hits(void *,void *,void *,int,int,int,int,int,float,float,float) __asm__("FUN_L00_0025a9f8");
extern void FUN_L00_00262360_c(int,float *,float *,void *,float,float) __asm__("FUN_L00_00262360");

void FUN_L10_002dd3d8(char *moby) {
 float old[4],pos[4],a,b;
 char *d=*(char **)(moby+0x78);
 *(OvlQuad *)old=*(OvlQuad *)(moby+0x10);
 ((EffectData *)d)->phase+=((EffectData *)d)->speed;
 ((EffectData *)d)->speed*=(((EffectData *)d)->growth-1.0f)*frame_scale+1.0f;
 FUN_L00_00262360_c(((EffectData *)d)->owner,&a,&b,moby+0x10,((EffectData *)d)->phase,((EffectData *)d)->size);
 *(float *)(moby+0x40)=random_angle_radians();
 *(float *)(moby+0x44)=random_angle_radians();
 *(float *)(moby+0x48)=random_angle_radians();
 if(fast_dec_timer((int *)(d+0x14))) {delete_moby(moby);return;}
 if(*(int *)(d+0x14)<truncate_float_to_s32(10.0f/(frame_time*5.0f))/2) {
  float numerator=convert_integer_to_float(*(int *)(d+0x14));
  *(float *)(moby+0x2C)=*(float *)(*(char **)(moby+0x24)+0x24)*numerator/convert_integer_to_float(truncate_float_to_s32(10.0f/(frame_time*5.0f))/2)*0.5f;
 } else if((truncate_float_to_s32(10.0f/(frame_time*5.0f))/4)*3<*(int *)(d+0x14)) {
  float numerator=convert_integer_to_float(truncate_float_to_s32(10.0f/(frame_time*5.0f))-*(int *)(d+0x14));
  *(float *)(moby+0x2C)=*(float *)(*(char **)(moby+0x24)+0x24)*numerator/convert_integer_to_float(truncate_float_to_s32(10.0f/(frame_time*5.0f))/4)*0.5f;
 }
 if(*(float *)(*(char **)(moby+0x24)+0x24)*0.25f<*(float *)(moby+0x2C)) {
  int count=collect_mobies_in_sphere(moby+0x10,0.15f,16,*(void **)(d+0x10),0);
  *(OvlQuad *)pos=*(OvlQuad *)(moby+0x10);
  send_blast_hits(*(void **)(d+0x10),pos,D_L10_00178380_u,count,0,0x810001,2,1,2.0f,1.0f,1.0f);
 }
}
#include "sda.h"

/* Collectible trigger: deletes itself once taken, else watches for its toucher. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DEA10), where it is exact; names translated to the US level program. */


extern L10State D_L10_001BB6B0;
extern char D_L10_00178580[];
extern int D_0015ED84_m __asm__("D_0015ED84");
extern int D_L10_001BA950[];
extern unsigned char D_0014C190[] NOT_SDA;
void mark_moby_for_removal_d650(struct Obj *obj) __asm__("FUN_0020c828");

void block_cave_update(struct Moby *m) __asm__("FUN_L10_002dd650");
void block_cave_update(struct Moby *m) {
    int i;
    char *e;
    float w[16];
    float v[4];
    unsigned short id = m->save_id;
    if (D_L10_001BB6B0.collected[(short)id] != 0 ||
        (*(int *)(D_0014C190 + (((short)id >> 5) * 4 + (D_0015ED84_m << 8))) >> (id & 0x1F)) & 1) {
        mark_moby_for_removal_d650(m);
        return;
    }
    m->draw_dist = 0x80;
    e = D_L10_00178580;
    for (i = 0; i < 0x40; i++, e += 0x40) {
        if (*(char **)(e + 0x34) == m) {
            if (*(int *)(e + 0x24) & 0x20000) {
                char *o = *(char **)(e + 0x20);
                if (*(short *)(o + 0xA6) == 0xAC || *(short *)(o + 0xA6) == 0x99) {
                    subtract_vector_xyz(v, &m->pos, o + 0x10);
                    transpose_rotation_matrix(w, &m->rot_row0);
                    transform_vector(v, v, w);
                    if (v[0] > 0.25f) {
                        *(int *)(D_0014C190 + (((short)m->save_id >> 5) * 4 +
                                               (D_0015ED84_m << 8))) |=
                            1 << (m->save_id & 0x1F);
                        D_L10_001BA950[(short)m->save_id >> 5] |=
                            1 << (m->save_id & 0x1F);
                        mark_moby_for_removal_d650(m);
                        return;
                    }
                    *(int *)(e + 0x34) = 0;
                } else {
                    *(int *)(e + 0x34) = 0;
                }
            }
        }
    }
}

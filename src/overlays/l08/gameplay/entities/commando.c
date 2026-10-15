/* Level 08 moby classes: commando. */
#include "types.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

#include "rnc/overlay/swarm_path.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/gameplay/state/options.h"
#include "rnc/gameplay/state/game_globals.h"

typedef struct {
    SwarmPathFollower follow[2]; /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    SwarmPart root;              /* 0x60 */
    SwarmPart parts[16];         /* 0x70 */
    s32 target_index;            /* 0x170: moby slot of the target */
    f32 turn_speed;              /* 0x174 */
    s32 fire_timer;              /* 0x178 */
    s16 pad17C;
    s16 sound;                   /* 0x17E */
} SwarmVars;

extern s32 D_L08_00161D40 __attribute__((sda));
extern f32 D_L08_00161D34 __attribute__((sda));
extern SwarmPath *D_L08_001B0CB0[];
extern struct Moby *D_L08_0015FFD8;

extern s32 FUN_L00_0028d8c0(struct Moby *, s32);
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern f32 multiply_global_factor_ed64(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern s32 tick_countdown_32_alt(s32 *) __asm__("FUN_001f9740");
extern f32 FUN_001f9b48(void *, void *);
extern float vector_length_xyz(void *);
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *);
extern void moby_point_position(void *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern void FUN_L00_0025f090(void *, void *, s32, f32, f32);
extern struct Moby *FUN_L08_002de3e0(void *, struct Moby *, void *, f32, f32);
extern void FUN_L08_002e9558(struct Moby *);
extern void FUN_L08_002e9758(struct Moby *);
extern void FUN_L08_002e9a18(struct Moby *);

void FUN_L08_002e9b70(struct Moby *moby) {
    SwarmVars *vars = (SwarmVars *)moby->pvars;
    SwarmPathFollower *follower;
    struct Moby *target;
    struct Moby *shot;
    struct Moby *bolt;
    SwarmTargetVars *target_vars;
    Vec4 point;
    Vec4 step;
    Vec4 muzzle;
    Vec4 velocity;
    s32 waypoint;
    s32 next;
    s32 after;
    s32 i;
    s32 slot;
    f32 distance;
    f32 heading;
    f32 next_heading;
    f32 turn;
    f32 pitch;
    f32 yaw;

    if (D_L08_00161D40 != 0) {
        return;
    }
    if (!FUN_L00_0028d8c0(moby, vars->sound)) {
        vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case 0:
        if (visited52[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        vars->follow[0].path = D_L08_001B0CB0[vars->follow[0].path_id];
        vars->follow[1].path = D_L08_001B0CB0[vars->follow[1].path_id];
        vars->fire_timer = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(300.0f, 600.0f)));
        moby->flags |= 0x4000;
        qcopy(&moby->pos, &vars->follow[1].path->points[0]);
        FUN_L08_002e9558(moby);
        moby->state = 2;
        moby->rot.z = fast_arc_tan(vars->follow[1].path->points[1].f[0] - moby->pos.x,
                                   vars->follow[1].path->points[1].f[1] - moby->pos.y);
        break;
    case 1:
        waypoint = (vars->follow[0].point + vars->follow[0].path->count + vars->follow[0].direction) % vars->follow[0].path->count;
        target = &D_L08_0015FFD8[vars->target_index];
        FUN_L00_00258278(moby,
                         fast_arc_tan(vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                                      vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
                         &vars->turn_speed, 0.004f, 0.3f, 0.02f);
        qcopy(&point, &vars->follow[0].path->points[waypoint]);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            vars->follow[0].point = waypoint;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        if (vector_length_xyz(&step) > D_L08_00161D34 * frame_time) {
            normalize_vector_xyz(&step, &step, D_L08_00161D34 * frame_time);
        }
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        FUN_L08_002e9758(moby);
        FUN_L08_002e9a18(moby);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        moby_point_position(vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(0.17453292f, 0.5235988f);
        yaw = fast_add_rotations(fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                                 3.1415927f);
        yaw = fast_add_rotations(yaw, random_float_between(-0.2617994f, 0.7853982f));
        if (yaw > 0.0f && yaw < 0.34f) {
            yaw = 0.34f;
        } else if (yaw <= 0.0f && yaw > -0.34f) {
            yaw = -0.34f;
        }
        normalize_vector_xyz(&velocity, &step, (D_L08_00161D34 + D_L08_00161D34) * frame_time);
        shot = FUN_L08_002de3e0(&muzzle, target, &velocity, pitch, yaw);
        shot->scale = shot->pclass->scale / 5.0f;
        target_vars = (SwarmTargetVars *)target->pvars;
        for (slot = 0; slot < 8; slot++) {
            if (target_vars->shots[slot] == NULL) {
                target_vars->shots[slot] = shot;
                break;
            }
        }
        break;
    case 2:
        follower = &vars->follow[1];
        next = (vars->follow[1].point + 1) % follower->path->count;
        after = (vars->follow[1].point + 2) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next].f[0] - follower->path->points[vars->follow[1].point].f[0],
                               follower->path->points[next].f[1] - follower->path->points[vars->follow[1].point].f[1]);
        next_heading = fast_arc_tan(follower->path->points[after].f[0] - follower->path->points[next].f[0],
                                    follower->path->points[after].f[1] - follower->path->points[next].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance = FUN_001f9b48(&moby->pos, &follower->path->points[next]);
        distance /= FUN_001f9b48(&follower->path->points[vars->follow[1].point],
                                 &follower->path->points[next]);
        moby->rot.z = fast_add_rotations(turn * distance, next_heading);
        qcopy(&point, &follower->path->points[next]);
        if (FUN_001f9b48(&moby->pos, &point) < 0.5f) {
            vars->follow[1].point = next;
        }
        subtract_vector_xyz(&step, &point, &moby->pos);
        if (vector_length_xyz(&step) > D_L08_00161D34 * frame_time) {
            normalize_vector_xyz(&step, &step, D_L08_00161D34 * frame_time);
        }
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        FUN_L08_002e9a18(moby);
        if (!tick_countdown_32_alt(&vars->fire_timer)) {
            break;
        }
        vars->fire_timer = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(300.0f, 600.0f)));
        moby_point_position(vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        normalize_vector_xyz(&velocity, &step, (D_L08_00161D34 + D_L08_00161D34) * frame_time);
        bolt = FUN_L08_002de3e0(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = vars->parts[3].moby->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case 3: {
        SwarmPath *path;

        moby->state = 1;
        path = vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z = fast_arc_tan(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        FUN_L08_002e9a18(moby);
        break;
    }
    case 0x63:
        for (i = 0; i < 16; i++) {
            struct Moby *part = vars->parts[i].moby;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                FUN_L00_0025f090(moby, &part->pos, -1, 3.0f, 13.0f);
                subtract_vector_xyz(drift, &part->pos, &moby->pos);
                normalize_vector_xyz(drift, drift, frame_time * 5.0f);
                part->state = 1;
                vars->parts[i].moby = NULL;
            }
        }
        FUN_L00_0025f090(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}

extern char *D_L08_0015FFD8_g __asm__("D_L08_0015FFD8");
extern int FUN_0022da68_c(int, int, char *) __asm__("FUN_0022da68");
extern void FUN_00213ed8_c(char *, float, float) __asm__("FUN_00213ed8");

void FUN_L08_002ea398(char *m)
{
    char *d = *(char **)(m + 0x78);
    char *o;
    int i;
    switch (*(u8 *)(m + 0x20)) {
    case 0:
        *(float *)(d + 4) = *(float *)(m + 0x18);
        *(u8 *)(m + 0x20) = 1;
        break;
    case 1:
        i = *(int *)(d + 8);
        if (i < 0) break;
        o = (char *)((i << 8) + (int)D_L08_0015FFD8_g);
        if (*(short *)(o + 0xA6) == 0x267 || *(short *)(o + 0xA6) == 0x23F) {
            if (*(u8 *)(o + 0x20) != 4) break;
            menu_item_enabled_41 = 1;
            FUN_0022da68_c(0, 0, m);
            *(u8 *)(m + 0x20) = 2;
        } else {
            FUN_00213ed8_c(m + 0x18, (*(float *)d - *(float *)(d + 4)) * **(float **)(o + 0x78) + *(float *)(d + 4), frame_time * 10.0f);
        }
        break;
    case 2:
        FUN_00213ed8_c(m + 0x18, *(float *)d, frame_time * 10.0f);
        break;
    }
}
/* Pvars of a gate that swings with the progress of its driver moby. */
typedef struct {
    s32 driver;          /* 0x0: moby index whose pvars start with the 0..1 progress */
    f32 open_deg;        /* 0x4: yaw at full progress, in degrees */
    f32 shut_deg;        /* 0x8: yaw at zero progress, in degrees */
    s32 flag;            /* 0xC: byte of D_0013D388 set when fully open */
    Vec4 home;           /* 0x10: hinge position */
    s32 voice;           /* 0x20: creak voice slot, -1 if none */
} SwingGateVars;

extern int D_L08_0015F5C4;
/* Voice table entries are 0x70 bytes; the volume sits 0x80 past an entry's start. */
typedef struct {
    u8 pad0[0x80];
    s32 volume;          /* 0x80 */
} VoiceView;
extern char D_0013E550[];
extern u8 D_0013D388[];
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void FUN_L01_002a1968(s32, s32);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");

/* Swings the gate to its driver's progress, fading a creak in while it
 * moves and out when it stops, and latches its flag once fully open. */
void FUN_L08_002ea4c0(struct Moby *moby) {
    SwingGateVars *vars = (SwingGateVars *)moby->pvars;
    f32 progress = *(f32 *)D_L08_0015FFD8[vars->driver].pvars;
    Vec4 mat[4];
    Vec4 off;
    f32 yaw;

    off.q = 0;
    off.f[2] = 30.0f;
    if (D_L08_0015F5C4 == 2) {
        moby->visible = 0;
        moby->flags |= 1;
    } else {
        moby->visible = 1;
        moby->flags &= ~1;
    }
    if (moby->state == 0) {
        moby->state = 1;
        qcopy(&vars->home, &moby->pos);
        return;
    }
    if (moby->state != 1)
        return;
    yaw = fast_subtract_rotations(vars->open_deg * DEG_TO_RAD, vars->shut_deg * DEG_TO_RAD);
    yaw = fast_add_rotations(yaw * progress, vars->shut_deg * DEG_TO_RAD);
    if (fast_difference_between_rotations(moby->rot.y, yaw) != 0.0f) {
        moby->rot.y = yaw;
        if (moby->oclass == 0x1D4) {
            if (!FUN_L00_0028d8c0(moby, vars->voice)) {
                vars->voice = allocate_voice_for_target_entry(0, 4, moby);
                FUN_L01_002a1968(vars->voice, 1);
            } else if (((VoiceView *)(D_0013E550 + vars->voice * 0x70))->volume < 0x400) {
                FUN_L01_002a1968(vars->voice, ((VoiceView *)(D_0013E550 + vars->voice * 0x70))->volume +
                                                  0x400 / scale_game_frames(60));
            }
        }
    } else if (moby->oclass == 0x1D4 && FUN_L00_0028d8c0(moby, vars->voice)) {
        if (((VoiceView *)(D_0013E550 + vars->voice * 0x70))->volume < 0x100) {
            release_voice_slot(vars->voice);
            vars->voice = -1;
        } else {
            FUN_L01_002a1968(vars->voice, ((VoiceView *)(D_0013E550 + vars->voice * 0x70))->volume -
                                              0x400 / scale_game_frames(60));
        }
    }
    euler_rotation_matrix(mat, &moby->rot);
    transform_vector(&off, &off, mat);
    add_vector_xyz(&moby->pos, &vars->home, &off);
    if (progress == 1.0f && moby->oclass == 0x1D4) {
        D_0013D388[vars->flag + 0x30] = 1;
        moby->state = 2;
        allocate_voice_for_target_entry(1, 0, moby);
        if (moby->oclass == 0x1D4 && FUN_L00_0028d8c0(moby, vars->voice))
            release_voice_slot(vars->voice);
    }
}
/* Draws three rows of HUD elements with their colours. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_002F2288), where it is exact; names translated to the US level program. */

extern int D_L08_00161DD0 __attribute__((sda));
extern int D_L08_00161DD4;
extern int D_L08_00161DD8;
extern int D_L08_00161DF0 __attribute__((sda));
extern int D_L08_00161DF4;
extern int D_L08_00161E00 __attribute__((sda));
extern unsigned char D_L08_00161E08_u __asm__("D_L08_00161E08");
extern int get_effect_texture_alt(int) __asm__("FUN_001f44b8");
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void FUN_L08_002f0b40(int);
extern void FUN_L08_002f0c18(int);
extern void FUN_L08_002f0cf0(int);
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

void FUN_L08_002f0dc8(void) {
    vu1_add_g_sregister(0x42, 0x8000000044L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1);
    font_queue_vu_state();
    vu1_add_g_sregister(6, get_effect_texture_alt(0x3A));
    vu1_add_g_sregister(0x42, (long)D_L08_00161DD0 << 32 | 0x44);
    FUN_L08_002f0b40(0);
    vu1_add_g_sregister(6, get_effect_texture_alt(0x3B));
    vu1_add_g_sregister(0x42, (long)((D_L08_00161DD4 * D_L08_00161E08_u) >> 8) << 32 | 0x62);
    FUN_L08_002f0b40(1);
    vu1_add_g_sregister(0x42, (long)((D_L08_00161DD8 * D_L08_00161E08_u) >> 8) << 32 | 0x68);
    FUN_L08_002f0b40(2);
    vu1_add_g_sregister(6, get_effect_texture_alt(0x3C));
    vu1_add_g_sregister(0x42, (long)D_L08_00161DF0 << 32 | 0x48);
    FUN_L08_002f0c18(0);
    vu1_add_g_sregister(0x42, (long)D_L08_00161DF4 << 32 | 0x48);
    FUN_L08_002f0c18(1);
    vu1_add_g_sregister(6, get_effect_texture_alt(0x3D));
    vu1_add_g_sregister(0x42, (long)D_L08_00161E00 << 32 | 0x48);
    FUN_L08_002f0cf0(0);
}

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030AC70), where it is exact; names translated to the US level program. */

extern Pair D_L08_00161E10[] __attribute__((section(".sdata")));
extern Pair D_L08_00161E14[] __attribute__((section(".sdata")));
extern Pair D_L08_00161E24[] __attribute__((section(".sdata")));
extern Pair D_L08_001E8348[];
extern Pair D_L08_00161E28;
extern char D_L08_001675C0[];
extern s32 D_L08_001DDF40[];
extern s32 D_L08_001E2828[];
extern s32 D_L08_001E2858[];
extern s32 D_L08_001E8310[];
extern char D_L08_001E8350[];
extern float D_L08_00161DC4;
extern float D_L08_00161DC8;
extern float D_L08_00161DCC;
extern float vector_length_xyz(void *);
extern int D_L08_00161E08;
extern int FUN_001fa728(char *, float);
extern int truncate_float_to_s32() __asm__("FUN_001fa6d0");
extern short D_L08_00161DC0 __attribute__((sda));
extern short D_L08_00161E04 __attribute__((sda));
extern s32 D_L08_001DDF20[];
extern s32 D_L08_001DB518[];
extern void FUN_L08_002f0dc8(void);
extern void FUN_L08_002f0f68(void *, int, void *, float, float, float, float, int);
extern void FUN_L08_002f1158(void *, float, float, float, void *);
extern void FUN_L08_002f12a0(int);
extern void FUN_L08_002f1378(int);
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
extern void subtract_vector_xyz(void *, void *, void *);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern Pair D_L08_00161E20[] __attribute__((section(".sdata")));
void FUN_L08_002f1548(struct Moby *moby) {
    int i;
    float v[4];
    float w[4];
    float d;

    switch (moby->state) {
    case 0:
        if (*(int *)&D_L08_00161E04 == 0) {
            FUN_L08_002f0f68(D_L08_001E8310, 0xD, D_L08_001E2858, 0.5f, 0.5f, 0.65f, 0.5f, 1);
            FUN_L08_002f0f68(D_L08_001DDF20, 8, D_L08_001DB518, 0.5f, 0.5f, 0.5f, 1.0f, 1);
            FUN_L08_002f0f68(D_L08_001E2828, 0xB, D_L08_001DDF40, 0.25f, 0.25f, 0.25f, 1.0f, 1);
            *(int *)&D_L08_00161E04 = 1;
        }
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            D_L08_001E8348[i].a = 0;
            D_L08_001E8348[i].b = 0;
            D_L08_00161E28.a = 0;
            D_L08_00161E28.b = 0;
        }
        for (i = 0; i < 2; i++) {
            D_L08_00161E10[i].a = 0;
            *(float *)&D_L08_00161E14[i].a = i * 0.4f;
        }
        for (i = 0; i < 1; i++) {
            D_L08_00161E20[i].a = 0;
            *(float *)&D_L08_00161E24[i].a = i * 0.4f;
        }
        break;
    case 1:
        v[0] = *(float *)&D_L08_00161DC0;
        v[1] = D_L08_00161DC4;
        v[2] = D_L08_00161DC8;
        v[3] = D_L08_00161DCC;
        if (FUN_001fa728((char *)v, 1000.0f) >= 0) {
            w[0] = *(float *)&D_L08_00161DC0;
            w[1] = D_L08_00161DC4;
            w[2] = D_L08_00161DC8;
            w[3] = 1.0f;
            subtract_vector_xyz(w, D_L08_001675C0, w);
            d = vector_length_xyz(w);
            if (d <= 84.0f) {
                FUN_L08_002f12a0(0);
                FUN_L08_002f1158(D_L08_001E8350, 0.52f, 0.62f, 0.06f, (void *)&D_L08_00161E28);
                FUN_L08_002f12a0(2);
                if (d < 20.0f) {
                    *(unsigned char *)&D_L08_00161E08 = 0xFF;
                } else if (d < 84.0f) {
                    *(char *)&D_L08_00161E08 =
                        func_001FA898_r((1.0f - (d - 20.0f) * 0.015625f) * 255.0f);
                } else {
                    *(char *)&D_L08_00161E08 = 0;
                }
            }
            FUN_L08_002f1378(0);
            FUN_L08_002f1378(0);
            FUN_L08_002f1378(1);
            enqueue_callback_list_1(FUN_L08_002f0dc8, moby);
        }
        break;
    }
}
extern f32 D_L08_0015F580[] __attribute__((section(".sdata")));
extern struct Moby *D_0013F58C __attribute__((section(".data")));
/* Retail loads gp - 0x4C90; GP 0x166C00 places this scalar at 0x161F70. */

extern s32 find_moby_hit_44c0(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern f32 FUN_001f9b80_44c0(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_add_rotations_44c0(f32, f32) __asm__("FUN_001fa580");
extern f32 FUN_001f9e90_44c0(f32, f32) __asm__("FUN_001f9e90");
extern f32 FUN_001f9b48_44c0(void *, void *) __asm__("FUN_001f9b48");
extern void clear_u64_value_44c0(void *) __asm__("FUN_001f99f8");
extern void subtract_vector_xyz_44c0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz_44c0(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz_44c0(void *, void *, void *) __asm__("FUN_001f9a10");
extern s32 scale_game_frames_44c0(s32) __asm__("FUN_001f96f8");
extern f32 approach_value_44c0(void *, f32, f32) __asm__("FUN_00213ed8");
extern void mark_moby_for_removal_44c0(struct Moby *) __asm__("FUN_0020c828");
extern void allocate_voice_for_target_entry_44c0(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern void FUN_L00_00258278_44c0(struct Moby *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_00258278");
extern void FUN_L00_0025e450_44c0(char *, void *, char *, f32, f32, s32, s32, s32,
                         f32, f32, f32, f32, s32, f32, s32, s32, s32, u8) __asm__("FUN_L00_0025e450");

extern f32 D_L08_00161F70 __attribute__((sda));
void FUN_L08_002f44c0(struct Moby *moby)
{
    SwarmPathFollower *follower = (SwarmPathFollower *)moby->pvars;
    Vec4 point;
    Vec4 step;
    Vec4 velocity; /* Effect scratch is separate from the waypoint movement step. */
    f32 fade;
    s32 waypoint;
    f32 distance;

    switch (moby->state) {
    case 0: {
        fade = moby->pos.z;
        moby->state = 1;
        moby->pos.z = fade + 0.4f;
        return;
    }
    case 1: {
        s32 active = find_moby_hit_44c0(moby, 0x330000, 0);
        distance = FUN_001f9b80_44c0(&moby->pos, D_0013F3D0);
        if ((distance < 2.0f && *(s32 *)(D_0013F3D0 + 0x2004) == 0x42) || *(struct Moby **)(D_0013F3D0 + 0x1bc) == moby) {
            allocate_voice_for_target_entry_44c0(0, 0, moby);
            FUN_L00_0025e450_44c0(moby, D_L08_0015F580, &moby->pos, 3.0f, 1.0f, 10, 20, 60,
                          3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_44c0(moby);
            return;
        }
        if (active != 0) {
            allocate_voice_for_target_entry_44c0(0, 0, moby);
            FUN_L00_0025e450_44c0(moby, D_L08_0015F580, &moby->pos, 0.0f, 0.0f, 10, 20, 60,
                          1.0f, 3.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_44c0(moby);
        }
        return;
    }
    case 2: {
        moby->pos.z -= 0.4f;
        if (follower->path_id == -1 || follower->path->count == 0) {
            mark_moby_for_removal_44c0(moby);
            return;
        }
        moby->rot.y = fast_add_rotations_44c0(moby->rot.y, D_L08_00161F70 * 0.017453292f * frame_time);
        distance = FUN_001f9b80_44c0(&moby->pos, D_0013F3D0);
        if (distance < 2.0f && *(s32 *)(D_0013F3D0 + 0x2004) == 0x42) {
            clear_u64_value_44c0(&velocity);
            FUN_L00_0025e450_44c0(moby, &velocity, &moby->pos, 0.0f, 0.0f, 10, 20, 60,
                          3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_44c0(moby);
            return;
        }
        waypoint = (follower->point + follower->path->count + follower->direction) % follower->path->count;
        FUN_L00_00258278_44c0(moby, FUN_001f9e90_44c0(follower->path->points[waypoint].f[0] - moby->pos.x,
                                   follower->path->points[waypoint].f[1] - moby->pos.y),
                     (f32 *)((char *)follower + 0x34), 0.01f, 0.3f, 0.1f);
        qcopy(&point, &follower->path->points[waypoint]);
        if (waypoint != 0) {
            subtract_vector_xyz_44c0(&step, &point, &moby->pos);
            normalize_vector_xyz_44c0(&step, &step, frame_time * 3.0f);
            add_vector_xyz_44c0(&moby->pos, &moby->pos, &step);
        } else {
            distance = FUN_001f9b80_44c0(&moby->pos, D_0013F3D0);
            if (distance < 24.0f) {
                clear_u64_value_44c0(&velocity);
                FUN_L00_0025e450_44c0(moby, &velocity, &moby->pos, 0.0f, 0.0f, 10, 20, 60,
                              3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
                allocate_voice_for_target_entry_44c0(0, 0, moby);
                mark_moby_for_removal_44c0(moby);
                return;
            }
            moby->state = 3;
        }
        if (FUN_001f9b48_44c0(&moby->pos, &point) < 0.1f) {
            follower->point = waypoint;
        }
        moby->pos.z += 0.4f;
        return;
    }
    case 3: {
        Vec4 timer;
        timer.f[0] = (f32)moby->alpha;
        approach_value_44c0(&timer, 0.0f, 128.0f / (f32)scale_game_frames_44c0(30));
        moby->alpha = (u8)(s32)timer.f[0];
        if (moby->alpha == 0) mark_moby_for_removal_44c0(moby);
    }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Smooth noise: linearly interpolates between two entries of an 8-float ring at arg+0x90. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_002F7258), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32_u(f32) __asm__("FUN_001fa6d0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

float FUN_L08_002f5d98(float x, char *arg) {
    float f = x * 8.0f;
    int i = func_001FA898_r(f);
    float fr = f - convert_integer_to_float(i);
    char *t;
    int j;
    i = i % 8;
    j = i + 1;
    j = j % 8;
    t = arg + 0x90;
    return *(float *)(t - (-(i * 4))) * (1.0f - fr) + *(float *)(t - (-(j * 4))) * fr;
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L08_002f5e58.s", FUN_L08_002f5e58);
#else
#endif
/* Pvars of a flickering point light. */
typedef struct {
    f32 size_x;          /* 0x0: the larger of size_x/size_y sets the draw scale */
    f32 size_y;          /* 0x4 */
    u16 pad8;
    u16 flags;           /* 0xA: bit 2 = cast light; bit 1 set every update */
    u8 padC[0x64];
    f32 period_a;        /* 0x70: must be in (0, 8] */
    f32 period_b;        /* 0x74: must be in (0, 8] */
    u8 pad78[0x10];
    f32 jitter;          /* 0x88: lower bound of the random flicker levels */
    u8 pad8C[4];
    f32 levels_a[8];     /* 0x90: flicker ring (FUN_L08_002f5d98) */
    f32 levels_b[8];     /* 0xB0 */
    u8 padD0[0x23];
    u8 unkF3;
    u8 padF4[4];
    s32 unkF8;
    u8 padFC[0x38];
    u8 red;              /* 0x134 */
    u8 green;            /* 0x135 */
    u8 blue;             /* 0x136 */
    u8 radius;           /* 0x137 */
    u8 falloff;          /* 0x138 */
    u8 pad139[2];
    s8 light;            /* 0x13B: point light handle (-1: none) */
    u8 pad13C[4];
    Vec4 light_pos;      /* 0x140 */
    f32 fade_range;      /* 0x150: distance over which the light fades out */
    f32 intensity;       /* 0x154 */
} FlickerLightVars;

/* A point light slot, 0x20 bytes apart. */
typedef struct {
    f32 red;
    f32 green;
    f32 blue;
    u8 padC[0x14];
} PointLight;

extern PointLight D_L08_00180AC0[];
extern char D_L08_001FC220[];
extern char D_L08_001FC250[];
extern char D_L08_001FC2A0[];
extern char D_L08_001FC2D0[];
extern s32 DebugPrint();
extern int FUN_L00_0023e738(float *, float, float, float, float, float);
extern void FUN_L00_0023e838(int);
void FUN_L08_002f5e58(struct Moby *m);
#ifdef NON_MATCHING
extern u64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern s32 D_L08_0015F3F8;
extern s32 D_L08_0015F3F8_gp __asm__("D_L08_0015f3f8") __attribute__((sda));
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 square_root(f32) __asm__("FUN_001f9988");
extern f32 FUN_L00_0025e310(f32);
extern f32 FUN_L00_00200228(f32, void *) __asm__("FUN_L00_00200228");
extern s32 tick_countdown_16(s16 *) __asm__("FUN_001f9770");
extern s32 random_integer_between(s32, s32) __asm__("FUN_L00_00257b90");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void fast_draw_quad_real(void *, void *, s32) __asm__("FUN_001f7d30");
extern void FUN_L00_0026ced0(void *, void *, u32, u32, s32, f32);
extern void FUN_L08_0027b450(struct Moby *, void *, void *, s32, s32, s32, s32, s32, f32);

/* The draw helper reads this contiguous 0x90-byte packet. */
typedef struct {
    Vec4 points[4];
    u32 color[4];
    f32 uv[4][2];
    u64 gs[4];
} FlickerDrawPacket;

void FUN_L08_002f5e58(struct Moby *m) {
    FlickerLightVars *d;
    u8 *b;
    f32 *f;
    Vec4 corners[4], across, down[2], edge[2], point, width, normal, mat[4];
    FlickerDrawPacket packet;
    Vec4 velocity, offset, position, wave, sag;
    s32 stride;
    Vec4 *grid;
    f32 *weight;
    f32 inv_x, inv_y;
    f32 *u, *v;
    f32 phase, phase_step, amp_a, amp_step_a, amp_b, amp_step_b;
    f32 step_a, step_b, time, height_a, height_b, phase_a, phase_b;
    f32 x, y, noise_x, angle, base, sine, shape, bend, a, c, t;
    s32 alpha, i, j, k, timer, count;
    s16 lifetime;
    u32 color_a, color_b;
    s32 temp;
    s32 next_i;
    u8 red, green, blue;
    s8 light;
    Vec4 *at;
    void *mat_rot;
    if (m == NULL) return;
    d = (FlickerLightVars *)m->pvars;
    if (d == NULL) return;
    b = (u8 *)d;
    f = (f32 *)d;
    mat_rot = &m->rot;
    stride = (b[8] + 1) * 16;
    grid = __builtin_alloca((((u32)(b[8] + 1) << 7) * (b[9] + 1)) >> 3);
    inv_x = 1.0f / convert_integer_to_float(b[8]);
    inv_y = 1.0f / convert_integer_to_float(b[9]);
    u = __builtin_alloca((b[8] * 4 + 0x13) & 0x7f0);
    v = __builtin_alloca((b[9] * 4 + 0x13) & 0x7f0);
    time = convert_integer_to_float(D_L08_0015F3F8);
    {
        f32 *target = d->levels_b;
        f32 *current = d->levels_a;
        s32 i = 7;
        do {
            f32 value = *current;
            if (value < *target) {
                f32 t = value + f[0x22] * f[0x23];
                *current = t;
                if (*target <= t) *target = random_float_between(1.0f-f[0x22], 1.0f);
            } else {
                f32 t = value - f[0x22] * f[0x23];
                *current = t;
                if (t <= *target) *target = random_float_between(1.0f-f[0x22], 1.0f);
            }
            target++;
            i--;
            current++;
        } while (i >= 0);
    }
    {
        f32 a, c;
        a = convert_integer_to_float(b[0x13]);
        c = convert_integer_to_float(m->alpha);
        alpha = truncate_float_to_s32_u(d->intensity * a * c * 0.0078125f) & 0xff;
    }
    if (b[0xd] != 0) b[0xf] = b[0xc] + (D_L08_0015F3F8_gp / (b[0xe]+1)) % b[0xd];
    else b[0xf] = b[0xc];
    temp = b[0xf];
    if (!(d->flags & 1)) temp += 0x28;
    packet.gs[1] = get_effect_texture(temp);
    packet.gs[0] = 0;
    packet.gs[2] = 0xff9000000260ULL;
    packet.gs[3] = (u64)b[0x3c] | (u64)b[0x3d] << 2 | (u64)b[0x3e] << 4 |
                   (u64)b[0x3f] << 6 | (u64)b[0x13] << 32;
    packet.color[3] = (u32)b[0x13] << 24 | (u32)b[0x11] << 16 | (u32)b[0x12] << 8 | b[0x10];
    packet.color[0] = packet.color[1] = packet.color[2] = packet.color[3];
    corners[0].q = corners[1].q = corners[2].q = corners[3].q = 0;
    {
        f32 t = d->size_x * 0.5f;
        corners[1].f[0] = corners[3].f[0] = t;
        corners[0].f[0] = corners[2].f[0] = -t;
    }
    corners[0].f[2] = f[0x20] + 0.01f;
    corners[1].f[2] = f[0x21] + 0.01f;
    euler_rotation_matrix(mat, mat_rot);
    transform_vector(&corners[0], &corners[0], mat);
    transform_vector(&corners[1], &corners[1], mat);
    transform_vector(&corners[2], &corners[2], mat);
    transform_vector(&corners[3], &corners[3], mat);
    subtract_vector_xyz(&down[0], &corners[2], &corners[0]);
    fast_vec_scale(&down[0], &down[0], inv_y);
    subtract_vector_xyz(&down[1], &corners[3], &corners[1]);
    fast_vec_scale(&down[1], &down[1], inv_y);
    phase_step = (f[6] - f[5]) * inv_y;
    amp_step_a = (f[9] - f[7]) * inv_y;
    amp_step_b = (f[10] - f[8]) * inv_y;
    step_a = (f[0x1a] - f[0x18]) * inv_x;
    height_a = d->size_y - f[0x20];
    height_b = d->size_y - f[0x21];
    step_b = (f[0x1b] - f[0x19]) * inv_y;
    subtract_vector_xyz(&width, &corners[1], &corners[0]);
    fast_vec_cross(&normal, &width, &down[0]);
    normalize_vector_xyz(&normal, &normal, 1.0f);
    qcopy(&edge[0], &corners[0]);
    qcopy(&edge[1], &corners[1]);
    bend = d->levels_a[0];
    phase = f[5]; amp_a = f[7]; amp_b = f[8];
    phase_a = f[0x18]; phase_b = f[0x19];
    {
        s32 i;
        for (i = 0; i <= b[9]; i = next_i) {
            f32 next_phase, next_amp_a, next_amp_b, next_a, next_b;
            f32 noise_y;
            y = convert_integer_to_float(i) / convert_integer_to_float(b[9]);
            qcopy(&point, &edge[0]);
            next_i = i + 1;
            next_phase = phase + phase_step;
            next_amp_b = amp_b + amp_step_b;
            next_a = phase_a + step_a;
            next_amp_a = amp_a + amp_step_a;
            next_b = phase_b + step_b;
            subtract_vector_xyz(&across, &edge[1], &edge[0]);
            fast_vec_scale(&across, &across, inv_x);
            noise_y = FUN_L08_002f5d98(y, (char *)d);
            k = i * stride;
            weight = (f32 *)((char *)grid + k + 12);
            for (j = 0; j <= b[8]; j++) {
                f32 a, c, t, base, angle, shape;
                x = convert_integer_to_float(j) / convert_integer_to_float(b[8]);
                a = FUN_L00_0025e310(f[0x17] * time);
                angle = x * 3.1415927f;
                c = fast_sin(a);
                t = f[0x16] * c;
                base = time * phase + t;
                noise_x = FUN_L08_002f5d98(x, (char *)d);
                base = base + (noise_x * f[0x34] + noise_y * f[0x35] + bend * f[0x36]);
                shape = height_a * x + height_b * (1.0f-x);
                a = FUN_L00_0025e310(base + phase_a + f[0xb] * x + f[0xd] * y);
                c = FUN_L00_0025e310(base + phase_b + f[0xc] * y + f[0xe] * x);
                sine = square_root(fast_sin(angle));
                shape = shape * sine;
                shape = shape * (1.0f-y);
                t = fast_cos(a);
                normalize_vector_xyz(&velocity, &across, amp_a * t * sine * noise_x);
                t = fast_sin(c);
                normalize_vector_xyz(&offset, &down[0], amp_b * t * sine * noise_x);
                a = fast_sin(y * 3.1415927f);
                c = fast_sin(angle);
                t = fast_sin(FUN_L00_0025e310(base + f[0xb] * x));
                base = fast_sin(FUN_L00_0025e310(base + f[0xd] * y));
                normalize_vector_xyz(&wave, &normal, a * c * (f[0x1e] * t + f[0x1f] * base));
                normalize_vector_xyz(&sag, &down[0], shape);
                add_vector_xyz(&position, &velocity, &offset);
                add_vector_xyz(&position, &position, &wave);
                subtract_vector_xyz(&position, &position, &sag);
                add_vector_xyz((char *)grid + k + j*16, &point, &position);
                *weight = 1.0f /
                    (((1.0f-f[0x56]) + f[0x56]*sine*sine*sine) * ((1.0f-f[0x57])+f[0x57]*y));
                add_vector_xyz(&point, &point, &across);
                weight += 4;
            }
            add_vector_xyz(&edge[0], &edge[0], &down[0]);
            phase = next_phase; amp_a = next_amp_a;
            add_vector_xyz(&edge[1], &edge[1], &down[1]);
            amp_b = next_amp_b; phase_a = next_a; phase_b = next_b;
        }
    }
    {
        f32 a;
        s32 count;
        f32 *weight;
        a = fast_sin(FUN_L00_0025e310(time * f[0x12]));
        a = FUN_L00_00200228(time*f[0x10] + f[0x11]*a, &temp);
        count = 0;
        if (count <= b[8]) {
            weight = u;
            do {
                *weight++ = a;
                a = a + inv_x / d->period_a;
                count++;
            } while (count <= b[8]);
        }
        a = fast_sin(FUN_L00_0025e310(time * f[0x15]));
        a = FUN_L00_00200228(time*f[0x13] + f[0x14]*a, &temp);
        count = 0;
        if (count <= b[9]) {
            weight = v;
            do {
                *weight++ = a;
                a = a + inv_y / d->period_b;
                count++;
            } while (count <= b[9]);
        }
    }
    for (i=0; i<b[9]; i++) {
        {
            Vec4 *current;
            Vec4 *target;
            f32 *current_v = v + i;
            f32 *target_v = v + i + 1;
            if (b[8]) {
                k = i * stride;
                count = (i + 1) * stride;
                current = (Vec4 *)((char *)grid + k);
                target = (Vec4 *)((char *)grid + count);
                for (j=0; j<b[8]; j++) {
                    packet.uv[0][0] = u[j]; packet.uv[0][1] = *current_v;
                    packet.uv[1][0] = u[j+1]; packet.uv[1][1] = *current_v;
                    packet.uv[2][0] = u[j]; packet.uv[2][1] = *target_v;
                    packet.uv[3][0] = u[j+1]; packet.uv[3][1] = *target_v;
                    add_vector_xyz(&packet.points[0], &current[j], &m->pos);
                    add_vector_xyz(&packet.points[1], &current[j+1], &m->pos);
                    add_vector_xyz(&packet.points[2], &target[j], &m->pos);
                    add_vector_xyz(&packet.points[3], &target[j+1], &m->pos);
                    timer = truncate_float_to_s32_u(((f32 *)((char *)grid + k + j*16))[3]);
                    packet.color[0] = (u32)(alpha/timer) << 24 | (u32)b[0x11] << 16 | (u32)b[0x12] << 8 | b[0x10];
                    timer = truncate_float_to_s32_u(((f32 *)((char *)grid + k + (j+1)*16))[3]);
                    packet.color[1] = (u32)(alpha/timer) << 24 | (u32)b[0x11] << 16 | (u32)b[0x12] << 8 | b[0x10];
                    timer = truncate_float_to_s32_u(((f32 *)((char *)grid + count + j*16))[3]);
                    packet.color[2] = (u32)(alpha/timer) << 24 | (u32)b[0x11] << 16 | (u32)b[0x12] << 8 | b[0x10];
                    timer = truncate_float_to_s32_u(((f32 *)((char *)grid + count + (j+1)*16))[3]);
                    packet.color[3] = (u32)(alpha/timer) << 24 | (u32)b[0x11] << 16 | (u32)b[0x12] << 8 | b[0x10];
                    fast_draw_quad_real(&packet, NULL, 0);
                }
            }
        }
    }
    if (!(d->flags & 2)) return;
    if (tick_countdown_32_alt((s32 *)(b+0xf8))) {
        *(s32 *)(b+0xf8) = scale_game_frames(b[0xf3]);
        i=0;
        if (b[0xf2]) do {
            t = d->size_x * 0.5f;
            i++;
            a = random_float_between(-1.0f, 1.0f);
            a = random_float_between(0.0f, a*f[0x3d]);
            c = square_root(fast_cos(a*3.1415927f));
            normalize_vector_xyz(&position, &down[0], -(f[0x3f]+c*f[0x40]));
            normalize_vector_xyz(&offset, &across, a*t);
            add_vector_xyz(&offset, &offset, &m->pos);
            add_vector_xyz(&offset, &offset, &position);
            velocity.f[0] = random_float_between(-f[0x37], f[0x37]) * frame_scale;
            velocity.f[1] = random_float_between(-f[0x37], f[0x37]) * frame_scale;
            velocity.f[2] = random_float_between(0.0f, f[0x38]) * frame_scale;
            t = random_float_between(f[0x39], f[0x3a]);
            lifetime = (s16)scale_game_frames(*(s16 *)(b+0xf0));
            red = b[0xec]; green = b[0xed]; blue = b[0xee];
            {
                s32 temp = (u8)truncate_float_to_s32_u(convert_integer_to_float(b[0xef])*a);
                FUN_L08_0027b450(m, &offset, &velocity, lifetime, red, green, blue, temp, t);
            }
        } while (i < b[0xf2]);
    }
    if (tick_countdown_16((s16 *)(b+0x122))) {
        *(s16 *)(b+0x122) = scale_game_frames(b[0x121]);
        i=0;
        if (b[0x120]) do {
            s32 lifetime;
            i++;
            color_a = (u32)b[0x117]<<24 | (u32)b[0x116]<<16 | (u32)b[0x115]<<8 | b[0x114];
            color_b = (u32)b[0x11b]<<24 | (u32)b[0x11a]<<16 | (u32)b[0x119]<<8 | b[0x118];
            t = d->size_x * 0.5f;
            c = random_float_between(f[0x43], f[0x44]);
            lifetime = scale_game_frames(random_integer_between(*(s16 *)(b+0x11c), *(s16 *)(b+0x11e)));
            a = random_float_between(-1.0f, 1.0f);
            a = a*f[0x49];
            a = random_float_between(-a, a);
            shape = square_root(fast_cos(a*3.1415927f));
            normalize_vector_xyz(&position, &down[0], -(f[0x4b]+shape*f[0x4c]));
            normalize_vector_xyz(&offset, &across, a*t);
            add_vector_xyz(&offset, &offset, &m->pos);
            add_vector_xyz(&offset, &offset, &position);
            velocity.f[0] = random_float_between(-f[0x41], f[0x41])*frame_scale;
            velocity.f[1] = random_float_between(-f[0x41], f[0x41])*frame_scale;
            velocity.f[2] = random_float_between(0.0f, f[0x42])*frame_scale;
            fast_vec_scale(&velocity, &velocity, (1.0f-f[0x4a])+shape*f[0x4a]);
            FUN_L00_0026ced0(&offset, &velocity, color_a, color_b, lifetime, c);
        } while (i < b[0x120]);
    }
    light = d->light;
    if (light != -1) {
        at = (Vec4 *)((char *)&D_L08_00180AC0[light]+0x10);
        if (FUN_001f9b48(&d->light_pos, at) < f[0x4f]*frame_scale) {
            a = random_float_between((f32)-(s32)b[0x139], (f32)b[0x139]);
            a = (a+d->size_x)*0.5f;
            c = random_float_between((f32)-(s32)b[0x13a], (f32)b[0x13a]);
            normalize_vector_xyz(&velocity, &across, a);
            a = (c+d->size_y)*0.5f;
            normalize_vector_xyz(&offset, &down[0], -a);
            add_vector_xyz(&d->light_pos, &velocity, &offset);
            add_vector_xyz(&d->light_pos, &d->light_pos, &corners[0]);
            add_vector_xyz(&d->light_pos, &d->light_pos, &m->pos);
        } else {
            subtract_vector_xyz(&velocity, &d->light_pos, at);
            normalize_vector_xyz(&velocity, &velocity, f[0x4f]*frame_scale);
            add_vector_xyz(at, at, &velocity);
            at->f[3] = convert_integer_to_float(d->radius);
        }
    }
    d->flags ^= 2;
}
#endif

void FUN_L08_002f70a0(struct Moby *m) {
    FlickerLightVars *d;
    Vec4 pos;
    f32 view, dist, fade;
    s32 hidden;
    PointLight *l;
    Vec4f *at;
    int i;

    if (m == NULL)
        return;
    d = (FlickerLightVars *)m->pvars;
    if (d == NULL)
        return;
    hidden = 0;
    at = &m->pos;
    if (m->state == 0) {
        d->light = -1;
        d->unkF8 = d->unkF3;
        qcopy(&d->light_pos, at);
        if (d->period_a == 0.0f) {
            DebugPrint(D_L08_001FC220);
            d->period_a = 1.0f;
        } else if (8.0f < d->period_b) {
            DebugPrint(D_L08_001FC250, d->period_b);
        }
        if (d->period_b == 0.0f) {
            DebugPrint(D_L08_001FC2A0);
            d->period_b = 1.0f;
        } else if (8.0f < d->period_a) {
            DebugPrint(D_L08_001FC2D0, d->period_a);
        }
        for (i = 0; i < 8; i++) {
            d->levels_a[i] = random_float_between(d->jitter, 1.0f);
            d->levels_b[i] = random_float_between(d->jitter, 1.0f);
        }
        m->state = 1;
    }
    m->scale = m->pclass->scale * (d->size_x > d->size_y ? d->size_x : d->size_y) * 0.25f;
    view = convert_integer_to_float(m->draw_dist);
    dist = FUN_001f9b48(at, D_L08_001675C0);
    fade = view - d->fade_range;
    if (fade < 0.0f)
        fade = 0.0f;
    if (view < dist) {
        d->intensity = 0.0f;
        hidden = 1;
    } else {
        if (fade < dist) {
            dist -= fade;
            dist /= d->fade_range;
            fade = 1.0f - dist;
        } else
            fade = 1.0f;
        d->intensity = fade;
    }
    pos.q = *(u128 *)at;
    pos.f[3] = convert_integer_to_float(d->radius);
    if ((m->visible != 0 || FUN_001fa728((char *)&pos, 255.0f) != -1) && !hidden) {
        enqueue_callback_list_1(FUN_L08_002f5e58, m);
        if (d->flags & 4) {
            if (d->light == -1) {
                d->light = FUN_L00_0023e738(d->light_pos.f, (f32)d->radius,
                                            convert_integer_to_float(d->falloff) / 100.0f,
                                            d->intensity * convert_integer_to_float(d->red) / 100.0f,
                                            d->intensity * convert_integer_to_float(d->green) / 100.0f,
                                            d->intensity * convert_integer_to_float(d->blue) / 100.0f);
            } else {
                l = &D_L08_00180AC0[d->light];
                l->red = d->intensity * convert_integer_to_float(d->red) / 100.0f;
                l->green = d->intensity * convert_integer_to_float(d->green) / 100.0f;
                l->blue = d->intensity * convert_integer_to_float(d->blue) / 100.0f;
            }
        }
    } else if (d->light != -1) {
        FUN_L00_0023e838(d->light);
        d->light = -1;
    }
    d->flags |= 2;
}

extern int D_L08_0015F5C4;

#include "qcopy.h"
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
#include "sda.h"
typedef struct {
    char pad00[4];
    unsigned short sub;
    char pad06[0x1A];
    void *ptr20;
    char pad24[0x1C];
    char body[0x64];
    float pitch;
    float yaw;
    char padAC[4];
    float height;
    char padB4[0xC];
    char head[0x68];
    float head_yaw;
    char pad12C[0x14];
    float target[4];
    char pad150[4];
    int idle_timer;
    int moving_timer;
    int target_timer;
} WD;
typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0x10];
    unsigned char opacity;
    char pad32[0x16];
    float yaw;
    char pad4C[7];
    unsigned char animation;
    char pad54[0x1C];
    unsigned char flags70;
    char pad71[7];
    WD *data;
    char pad7C[3];
    unsigned char b7F;
} WM;
extern void FUN_L01_002783a8(void *, float);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9e90_cf(float, float) __asm__("FUN_001fa688");
extern float FUN_001fa688_cf(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float truncate_float_to_s32_cf(float, float) __asm__("FUN_002132a8");
extern float vector_length_xy(void *);
extern float vector_length_xyz_c(void *) __asm__("vector_length_xyz");
extern int FUN_L00_00266448(void *, void *);
extern int FUN_L00_002668a0(void *, void *);
extern int random_float_between_cf(float) __asm__("FUN_001fa6d0");
extern int tick_countdown_32(int *);
extern void FUN_L00_002628d8(float, float, void *, void *, int);
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("subtract_vector_xyz");
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");
typedef struct {
    char pad00[0x24];
    short h24;
    char pad26[2];
    unsigned char b28;
    char pad29[3];
    float f2C;
    char pad30[0x10];
    void *p40;
    char pad44[0x1C];
    char body[0x64];
    float pitch;
    float yaw;
    char padCC[4];
    float height;
    char padD4[0xC];
    char head[0x68];
    float head_yaw;
    char pad14C[0x14];
    float target[4];
    char pad170[0x10];
    int joint;
    char pad184[4];
    int moving_timer;
    int target_timer;
} WD8;
extern unsigned char D_0013D3B4 __attribute__((section(".data")));
extern char D_L08_001621E0[];
extern int D_L08_001621D8 __attribute__((sda));
extern char *D_L08_001600EC;
extern void FUN_L00_00262d38(int);
extern void FUN_L00_00284e50(void *, void *);
extern float D_L08_001675C0_c[] __asm__("D_L08_001675C0");
extern void FUN_L08_00305138_c(void *) __asm__("FUN_L02_002e0cd8");
extern void FUN_L00_0025a120(void *);
extern float FUN_001f9b48(void *, void *);
extern void FUN_L00_002502a0(int);
extern void unlock_planet(int) __asm__("FUN_L00_002607d0");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");

/* Watcher update, same routine as FUN_L08_00305270 with two more states and the joint pose copy. */
void FUN_L08_00302ce8(WM *m) {
    L16WatchScratch scratch;
    WD8 *d = (WD8 *)m->data;
    float rate;
    float head_rate;
    int tracking;
    FUN_L08_00305138_c(m);
    if (m->opacity) {
        if (FUN_001f9b48(m->position, D_L08_001675C0_c) < (float)D_L08_001621D8) {
            FUN_L00_0025a120(m);
            m->b7F = (D_L08_001621D8 * 13) >> 4;
        }
    }
    switch (m->state) {
    case 0:
        ((unsigned char *)m)[0x30] = 0xFF;
        if (m->animation != 1)
            blend_moby_animation(m, 1, 0, scale_game_frames(10));
        if (D_0013DD4A && visited52[((unsigned char *)m)[0xB0] + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(m);
            return;
        }
        d->f2C = 3.0f;
        d->p40 = D_L08_001621E0;
        if (!D_0013D3B4) {
            m->state = 1;
        } else {
            char *b = D_L08_001600EC;
            int j = d->joint;
            qcopy(m->position, b + j * 0x80 + 0x30);
            qcopy((char *)m + 0x40, b + j * 0x80 + 0x70);
            m->state = 2;
        }
        FUN_L00_002668a0(m, (char *)d + 0x20);
        break;
    case 1:
        if (FUN_L00_00266448(m, (char *)d + 0x20)) {
            char *b = D_L08_001600EC;
            int j = d->joint;
            qcopy(m->position, b + j * 0x80 + 0x30);
            qcopy((char *)m + 0x40, b + j * 0x80 + 0x70);
            m->state = 2;
            D_0013D3B4 = 1;
        }
        break;
    case 2:
        if (m->flags70 & 2) {
            if (m->animation != random_integer_below(2) + 1)
                blend_moby_animation(m, random_integer_below(2) + 1, 0, scale_game_frames(20));
        }
        if (FUN_L00_00266448(m, (char *)d + 0x20)) {
            m->state = 3;
            FUN_L01_002783a8(m, 2.2f);
        }
        break;
    case 3:
        if (D_L08_0015F5C4 != 2) {
            d->b28 = 1;
            if (d->h24 == 3) {
                unlock_planet(10);
                FUN_L00_00262d38(10);
                FUN_L00_002502a0(((unsigned char *)m)[0xB0]);
                FUN_L00_00284e50((char *)&D_0013E633_watch + 0xE9D, (char *)&D_0013E633_watch + 0xEAD);
                memcard_save_data(0, -1);
                mark_moby_for_removal(m);
                return;
            }
            m->state = 2;
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (m->animation == 1) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (FUN_001f9b80(m->position, player) < 8.0f &&
            FUN_001f9e90_cf(
                m->yaw,
                FUN_001fa688_cf(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                    m->position[0],
                                ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                    m->position[1])) < 1.5707964f) {
            if (vector_length_xyz_c(player + 0x80) > 0.01f)
                d->moving_timer = scale_game_frames_c(120);
            else
                tick_countdown_32(&d->moving_timer);
        } else if (d->moving_timer) {
            d->moving_timer = 0;
            qcopy(d->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (tick_countdown_32(&d->target_timer)) {
            float heading;
            d->target_timer =
                random_float_between_cf(multiply_global_factor_ed64(truncate_float_to_s32_cf(180.0f, 300.0f)));
            heading =
                fast_add_rotations(m->yaw, truncate_float_to_s32_cf(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(d->target, 6.0f, heading,
                                   truncate_float_to_s32_cf(0.0f, 30.0f) * DEG_TO_RAD);
            add_vector_xyz(d->target, d->target, m->position);
        }
        if (d->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, d->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, m->position);
        scratch.eye[2] += 1.0f;
        subtract_vector_xyz_c(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(FUN_001fa688_cf(scratch.delta[0], scratch.delta[1]), m->yaw);
        pitch = -FUN_001fa688_cf(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        d->pitch = pitch;
        d->yaw = yaw * 0.6f;
        d->head_yaw = yaw * 0.4f;
    }
    if (big_head_cheat_enabled)
        d->height = 2.75f;
    FUN_L00_002628d8(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->body, 0);
    FUN_L00_002628d8(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->head, 1);
}


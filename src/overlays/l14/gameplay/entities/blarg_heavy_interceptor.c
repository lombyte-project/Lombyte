/* Level 14 moby classes: platform, blarg_heavy_interceptor, machinegun_turret, minibomber, plasmabot. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/data_labels.h"

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

#define NOT_SDA

#define MACRO_ADDR

/* Attach the callback during the six active phases of this level event. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/initonce_00246A90.c: func_L14_00246A90), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x30];
    int state;
    int phase;
    char pad38[0x144];
    void *moby;
} Level14State;

extern Level14State D_L14_0016CF60;
extern void emit_moby_particle_pairs(void *) __asm__("FUN_L00_0023e008");
extern void func_L00_0023E4C8(void *) __asm__("FUN_L14_002459c0");

void update_level14_event_moby_effects(void) __asm__("FUN_L14_002460f8");

void update_level14_event_moby_effects(void) {
    void *moby;
    if (current_level_index != 14 || D_L14_0016CF60.state != 0) {
        return;
    }
    moby = D_L14_0016CF60.moby;
    if (moby != 0 && (unsigned)(D_L14_0016CF60.phase - 0x1C) < 6) {
        emit_moby_particle_pairs(moby);
        add_draw_callback(func_L00_0023E4C8, moby);
    }
}

extern u16 *D_L14_001ABF40_u[] __asm__("D_L14_001ABF40");
extern u8 *D_L14_0015FFD8;
extern void light_off_67e8(s32) __asm__("FUN_L00_0023e838");
extern void FUN_0020c828(void *);

/* Walk the record list at D_L14_001ABF40_u[index] (terminated by a negative
 * entry) and flag the objects whose state byte at 0x20 is 1. */
void activate_waiting_turrets_in_group(s32 index) __asm__("FUN_L14_002b44a8");

void activate_waiting_turrets_in_group(s32 index) {
    u16 *list_entry = D_L14_001ABF40_u[index];
    u8 *base;
    u8 *listed_moby;

    if (list_entry == 0) {
        return;
    }
    base = D_L14_0015FFD8;
    do {
        listed_moby = base + ((*list_entry & 0x7FFF) << 8);
        if (listed_moby[0x20] == 1) {
            *(s16 *)(*(u8 **)(listed_moby + 0x78) + 0x88) = 1;
        }
    } while (*(s16 *)list_entry++ >= 0);
}

/* Release the handle stored at 0x12 of the record, then hand the moby over. */
void remove_flying_debris(u8 *self) __asm__("FUN_L14_002bb4d8");

void remove_flying_debris(u8 *self) {
    u8 *debris_vars = *(u8 **)(self + 0x78);
    s16 light_index = *(s16 *)(debris_vars + 0x12);

    if (light_index != -1) {
        light_off_67e8(light_index);
        *(s16 *)(debris_vars + 0x12) = -1;
    }
    FUN_0020c828(self);
}

#include "rnc/gameplay/entities/moby.h"
#include "rnc/audio/voice_pool.h"

/* Path mover gated by an activation trigger and its collectable flag: slides between the two ends of its path,
   eases towards the path point and carries what stands on it. */
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 points[1];
} MoverPath;

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
    s16 unk24;
    u8 pad26[2];
    u8 unk28;
    u8 pad29[0x15];
    s16 unk3E;
    u8 pad40[0x20];
    u8 carry[0x40];      /* 0x60: carried-object state for update_entity_matrix */
    u8 start_end;        /* 0xA0: 0 starts at the far end */
    u8 padA1[3];
    f32 t;               /* 0xA4: position along the path, 0 to 1 */
    f32 speed;           /* 0xA8 */
    f32 target_speed;    /* 0xAC: +-1 / travel frames */
    f32 travel_time;     /* 0xB0: seconds per trip */
    s32 path;            /* 0xB4: index into D_L14_001B0BB0, -1 for none */
    s16 retrigger;       /* 0xB8: set when the hero steps off while stopped */
    s16 wait;            /* 0xBA: frames before it may move again */
    f32 rise;            /* 0xBC: height gained this frame */
    s32 trigger;         /* 0xC0: clip volume that activates it, -1 once active */
    s32 voice;           /* 0xC4 */
    s32 silent;          /* 0xC8 */
    s32 show_volume;     /* 0xCC: clip volume that makes it visible, -1 once shown */
    f32 ease[3];         /* 0xD0: per-axis easing state for vendor_spring */
} PathMoverVars;

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L14LevelState;

extern int *D_L14_001B0BB0[];
extern L14LevelState D_L14_001BB930;
extern s32 D_L14_001BABD0[];
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32 f) __asm__("FUN_001fa6d0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9b48(void *, void *);
extern s32 fast_dec_timer_s16(s16 *) __asm__("FUN_001f9770");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern f32 vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
void ease_path_mover_to_endpoint(struct Moby *moby) __asm__("FUN_L14_002ac2b8");

extern void ease_path_mover_to_endpoint(struct Moby *) __asm__("FUN_L14_002ac2b8");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void release_voice_slot(s32 idx) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("FUN_001f99c0");

extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
void update_path_mover(struct Moby *moby) __asm__("FUN_L14_002aba80");
void update_path_mover(struct Moby *moby) {
    enum {
        PATH_MOVER_COMMAND_WAIT_AT_LAST_POINT = 0,
        PATH_MOVER_COMMAND_WAIT_AT_FIRST_POINT = 1,
        PATH_MOVER_COMMAND_TRAVERSE_PATH = 2
    };
    PathMoverVars *vars = (PathMoverVars *)moby->pvars;
    MoverPath *path;
    Vec4 orig;
    Vec4 delta;
    Vec4 step;
    Vec4 goal;
    s32 on = 0;

    if (vars->path == -1) {
        return;
    }
    path = (MoverPath *)D_L14_001B0BB0[vars->path];
    if (vars->show_volume != -1) {
        if (is_point_inside_clip_volume(&hero.motion.pos, vars->show_volume) == 0) {
            moby->update_dist = 0xFF;
            moby->collision = 0;
            moby->flags |= 0x41;
            return;
        }
        if (vars->show_volume != -1 && is_point_inside_clip_volume(&hero.motion.pos, vars->show_volume)) {
            moby->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
            vars->show_volume = -1;
        }
    }
    {
        u16 id;

        if (vars->trigger == -1 || is_point_inside_clip_volume(&hero.motion.pos, vars->trigger) ||
            D_L14_001BB930.collected[(s16)(id = moby->save_id)] != 0 ||
            (D_0014C190_257218[current_level_index][(s16)id >> 5] >> (id & 0x1F)) & 1) {
            if (vars->trigger != -1) {
                D_0014C190_257218[current_level_index][(s16)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                D_L14_001BABD0[(s16)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                vars->trigger = -1;
            }
            on = 1;
        }
    }
    qcopy(&orig, &moby->pos);
    fast_dec_timer_s16(&vars->wait);
    if (moby->state == 0) {
        f32 t;
        Vec4 *points;

        vars->unk28 = 4;
        vars->unk3E = 5;
        vars->unk20 = 0;
        vars->unk24 = 0;
        t = 0.0f;
        if ((moby->command = vars->start_end) == 0) {
            t = 1.0f;
        }
        vars->t = t;
        points = path->points;
        /* the unsigned index keeps retail's (count - 1) << 4; a signed one folds the -1 into the pointer */
        qcopy(&moby->pos, truncate_float_to_s32(t) ? &points[(u32)(path->count - 1)] : points);
        vars->wait = 0;
        vars->speed = 0.0f;
        vars->rise = 0.0f;
        moby->state = 1;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0xFF;
        vars->voice = -1;
    }
    switch (moby->command) {
    case PATH_MOVER_COMMAND_WAIT_AT_LAST_POINT:
        if (on) {
            f32 far;

            if ((vars->retrigger != 0 && is_hero_on_moby(moby)) ||
                (far = FUN_001f9b48(&hero.motion.pos, &path->points[path->count - 1]),
                 FUN_001f9b48(&hero.motion.pos, &path->points[0]) < far)) {
                moby->command = 2;
                vars->target_speed = -1.0f / multiply_global_factor_ed64(vars->travel_time * 60.0f);
                vars->retrigger = 0;
            }
        }
        break;
    case PATH_MOVER_COMMAND_WAIT_AT_FIRST_POINT:
        if (on) {
            f32 near;

            if ((vars->retrigger != 0 && is_hero_on_moby(moby)) ||
                (near = FUN_001f9b48(&hero.motion.pos, &path->points[0]),
                 FUN_001f9b48(&hero.motion.pos, &path->points[path->count - 1]) < near)) {
                moby->command = 2;
                vars->target_speed = 1.0f / multiply_global_factor_ed64(vars->travel_time * 60.0f);
                vars->retrigger = 0;
            }
        }
        break;
    case PATH_MOVER_COMMAND_TRAVERSE_PATH:
        if (vars->silent == 0 && is_hero_on_moby(moby)) {
            hero.unk1F2 = 4;
            hero.unk1F4 = 4;
        }
        if (vars->wait != 0 ||
            (vars->rise < 0.0f && vector_distance(&moby->pos, &hero.motion.pos) < 2.0f &&
             absolute_float(hero.motion.pos.f[2] - moby->pos.z) < 4.0f &&
             moby->pos.z - hero.motion.pos.f[2] > 1.0f)) {
            s32 voice;

            if (vars->wait == 0) {
                vars->wait = scale_game_frames(30);
            }
            voice = vars->voice;
            vars->speed = 0.0f;
            if (voice != -1) {
                VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

                if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                    release_voice_slot(voice);
                }
            }
            vars->voice = -1;
        } else {
            s32 i;
            f32 frac;
            f32 fi;
            Vec4 *from;

            if (!voice_is_playing(moby, vars->voice)) {
                vars->voice = allocate_voice_for_target_entry(0, 4, moby);
            }
            vars->speed += vars->target_speed * frame_time;
            if (absolute_float(vars->speed) > absolute_float(vars->target_speed)) {
                vars->speed = vars->target_speed;
            }
            vars->t = vars->t + vars->speed;
            if (0.5f < absolute_float(vars->t - 0.5f)) {
                s32 voice;

                if (0.0f < vars->target_speed) {
                    moby->command = 0;
                    vars->t = 1.0f;
                } else {
                    moby->command = 1;
                    vars->t = 0.0f;
                }
                vars->speed = 0.0f;
                vars->wait = scale_game_frames(15);
                voice = vars->voice;
                if (voice != -1) {
                    VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

                    if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                        release_voice_slot(voice);
                    }
                }
                vars->voice = -1;
            }
            frac = convert_integer_to_float(path->count - 1);
            i = truncate_float_to_s32(frac * vars->t);
            frac = convert_integer_to_float(path->count - 1);
            fi = convert_integer_to_float(i);
            frac *= vars->t;
            frac -= fi;
            if (i == path->count - 1) {
                qcopy(&goal, &path->points[i]);
            } else {
                from = &path->points[i];
                subtract_vector_xyz(&step, &path->points[i + 1], from);
                scale_vector_xyz(&step, &step, frac);
                add_vector_xyz(&goal, &step, from);
            }
            vendor_spring(&moby->pos.x, &vars->ease[0], goal.f[0], 0.005f, 0.2f, 0.0f);
            vendor_spring(&moby->pos.y, &vars->ease[1], goal.f[1], 0.005f, 0.2f, 0.0f);
            vendor_spring(&moby->pos.z, &vars->ease[2], goal.f[2], 0.005f, 0.2f, 0.0f);
            vars->rise = moby->pos.z - orig.f[2];
        }
        break;
    }
    if (moby->command != 2) {
        ease_path_mover_to_endpoint(moby);
    }
    subtract_vector_xyz(&delta, &moby->pos, &orig);
    update_entity_matrix(vars->carry, &delta, &moby->rot, &moby->rot);
    if (!is_hero_on_moby(moby)) {
        if (2.0f < vector_distance(&moby->pos, &hero.motion.pos)) {
            if (vars->wait == 0) {
                vars->retrigger = 1;
            }
        }
    }
}
/* Eases the moby toward the first or last point of its path. */

extern int *D_L14_001B0BB0[];
extern f32 vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");

void ease_path_mover_to_endpoint(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float t[4] __attribute__((aligned(16)));
    int *path = D_L14_001B0BB0[*(int *)(moby_vars + 0xB4)];
    if (*(float *)(moby_vars + 0xA4) > 0.5f)
        qcopy(t, (char *)path + path[0] * 16);
    else
        qcopy(t, (char *)path + 0x10);
    vendor_spring(&moby->pos.x, (float *)(moby_vars + 0xD0), t[0], 0.005f, 0.2f, 0.0f);
    vendor_spring(&moby->pos.y, (float *)(moby_vars + 0xD4), t[1], 0.005f, 0.2f, 0.0f);
    vendor_spring(&moby->pos.z, (float *)(moby_vars + 0xD8), t[2], 0.005f, 0.2f, 0.0f);
}
/* Per-frame update of a path-following flyer, a state machine on m->state: 0 checks its three paths
 * and sets it up (or hides it, state 8), 1 patrols the path in pvars 0x78 and scans for a target,
 * 2 drops the engine sound and alerts its group, 3 chases the locked-on target, 4 waits on the
 * group trigger, 5 and 6 fly the path in 0x204 and rejoin the patrol path, 7 is the hit that sends it
 * tumbling and 9 the fall that ends in a burst of debris. 8 only draws. */
void process_blarg_heavy_interceptor_hit_reactions(struct Moby *moby) __asm__("FUN_L14_002ad8b8");

extern void process_blarg_heavy_interceptor_hit_reactions(struct Moby *) __asm__("FUN_L14_002ad8b8");
void initialize_blarg_heavy_interceptor_paths(struct Moby *moby) __asm__("FUN_L14_002ae448");

extern void initialize_blarg_heavy_interceptor_paths(struct Moby *) __asm__("FUN_L14_002ae448");
void update_blarg_heavy_interceptor_patrol_aim_point(struct Moby *moby) __asm__("FUN_L14_002ae6d8");

extern void update_blarg_heavy_interceptor_patrol_aim_point(struct Moby *) __asm__("FUN_L14_002ae6d8");
void update_blarg_heavy_interceptor_patrol_path_position(struct Moby *moby) __asm__("FUN_L14_002ae7d8");

extern void update_blarg_heavy_interceptor_patrol_path_position(struct Moby *) __asm__("FUN_L14_002ae7d8");
void update_blarg_heavy_interceptor_return_path_position(struct Moby *moby, float position_easing_factor, float path_traversal_speed, float path_direction_multiplier) __asm__("FUN_L14_002ae900");

extern void update_blarg_heavy_interceptor_return_path_position(struct Moby *, float, float, float) __asm__("FUN_L14_002ae900");
extern void FUN_L14_002aea08_c618(struct Moby *) __asm__("FUN_L14_002aea08");
extern void FUN_L14_002aead8_c618(struct Moby *) __asm__("FUN_L14_002aead8");
int update_blarg_heavy_interceptor_aim_transforms(struct Moby *moby, int flag) __asm__("FUN_L14_002aeba0");

extern int update_blarg_heavy_interceptor_aim_transforms(struct Moby *, int) __asm__("FUN_L14_002aeba0");
extern void FUN_L14_002aed50_c618(struct Moby *) __asm__("FUN_L14_002aed50");
void draw_blarg_heavy_interceptor_group_effects(struct Moby *moby) __asm__("FUN_L14_002aee28");

extern void draw_blarg_heavy_interceptor_group_effects(struct Moby *) __asm__("FUN_L14_002aee28");
int count_moby_group_members(struct Moby *moby) __asm__("FUN_L14_002af048");

extern int count_moby_group_members(struct Moby *) __asm__("FUN_L14_002af048");
int try_retract_turret_group(int index) __asm__("FUN_L14_002b4500");

extern int try_retract_turret_group(int) __asm__("FUN_L14_002b4500");
int is_turret_group_activated_or_empty(int index) __asm__("FUN_L14_002b4670");

extern int is_turret_group_activated_or_empty(int) __asm__("FUN_L14_002b4670");
extern void blend_moby_animation_c618(struct Moby *, int, int, int) __asm__("FUN_00212f90");
extern u8 *FUN_002141f8_c618(struct Moby *) __asm__("FUN_002141f8");
extern void FUN_L00_0025a120(void *);
extern void memset_c618(void *, int, int) __asm__("FUN_001f97e8");
extern void attach_manipulator_c618(void *, s32, void *) __asm__("FUN_0020cb10");
extern void lerp_vector_c618(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void FUN_001e93e8_c618(void *, float) __asm__("FUN_001e93e8");
extern int FUN_L00_001f2868_c618(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int FUN_001efa68_c618(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void FUN_L00_0024f7c8_c618(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_001fff28(void *, int, float);
extern f32 ease_c618(f32 *pos, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
extern int FUN_L00_0025df68_c618(void *, void *, void *, int *, float *, int, float, float, float) __asm__("FUN_L00_0025df68");
extern int FUN_L00_0025d808_c618(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");
extern void FUN_L00_0025e450_c618(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void subtract_vector_c618(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_c618(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_c618(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_vectors_c618(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void build_look_at_matrix_c618(void *, void *, void *, float) __asm__("FUN_00214890");
extern f32 random_float_c618(f32, f32) __asm__("FUN_002132a8");
extern f32 randf_sym(f32, f32) __asm__("FUN_L00_00257c48");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
extern unsigned char *FUN_L00_0026d000_c618(void *pos, float r, float lo, float hi, int n, void *vel, float size, int color) __asm__("FUN_L00_0026d000");
extern void spawn_particles_at_shape_points(void *, void *, int) __asm__("FUN_L00_0025c398");
extern s32 allocate_voice_c618(s32, s32, void *) __asm__("FUN_0022da68");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern s32 random_integer_below(s32 n) __asm__("FUN_00213260");
extern f32 FUN_001fa6c0(s16);
extern s32 fast_bsphere_check(u8 *, f32) __asm__("FUN_001fa728");
typedef struct {
    u8 pad0[0x18];
    struct Moby *target; /* 0x18 */
} LockOn_c618;
extern LockOn_c618 D_L14_00174540;
extern char D_L14_001674C0[];
extern f32 D_L14_0016139C __attribute__((sda));
extern char *D_001413D0_c618 __asm__("D_001413D0") __attribute__((section(".data")));
extern char D_L14_0015F580[] __attribute__((section(".sdata")));
extern f32 D_0015ED60_c618 __asm__("D_0015ED60");
extern s32 D_L14_001613D0 __attribute__((sda));
extern s32 D_L14_001613D4 __attribute__((sda));
extern f32 D_L14_001613D8 __attribute__((sda));
extern f32 D_L14_001613DC __attribute__((sda));
extern f32 D_L14_001613E0 __attribute__((sda));
extern f32 D_L14_001613E4 __attribute__((sda));
extern f32 D_L14_001613E8 __attribute__((sda));
extern f32 D_L14_001613EC __attribute__((sda));
extern f32 D_L14_001613F0 __attribute__((sda));
extern f32 D_L14_001613F4 __attribute__((sda));
extern f32 D_L14_001613F8 __attribute__((sda));
extern s32 D_L14_00161400 __attribute__((sda));
extern s32 D_L14_00161404 __attribute__((sda));
extern s32 D_L14_00161408 __attribute__((sda));
extern s32 D_L14_0016140C __attribute__((sda));
extern s32 D_L14_00161410 __attribute__((sda));
extern s32 D_L14_00161414 __attribute__((sda));
extern f32 D_L14_001613A0_c618 __asm__("D_L14_001613A0") __attribute__((sda));
extern f32 D_L14_001613A4_c618 __asm__("D_L14_001613A4") __attribute__((sda));

extern Vec4 D_L14_00174560;
void update_blarg_heavy_interceptor(struct Moby *m) __asm__("FUN_L14_002ac618");
void update_blarg_heavy_interceptor(struct Moby *moby) {
    enum {
        INTERCEPTOR_STATE_INITIALIZE = 0,
        INTERCEPTOR_STATE_PATROL_AND_SCAN = 1,
        INTERCEPTOR_STATE_ALERT_GROUP = 2,
        INTERCEPTOR_STATE_TRACK_TARGET = 3,
        INTERCEPTOR_STATE_WAIT_FOR_GROUP_READY = 4,
        INTERCEPTOR_STATE_FOLLOW_RETURN_PATH = 5,
        INTERCEPTOR_STATE_REJOIN_PATROL = 6,
        INTERCEPTOR_STATE_INITIALIZE_DEATH_FALL = 7,
        INTERCEPTOR_STATE_DEATH_FALL = 9
    };
    char *moby_vars = (char *)moby->pvars;
    float vector_scratch[4];
    float secondary_vector_scratch[4];
    int near_seg;
    float near_t;
    int path_seg;
    float path_t;

    if ((u8)moby_vars[0x2E] == 2) {
        *(int *)(moby_vars + 0x20C) = 0;
        FUN_0020c828(moby);
        return;
    }
    if (moby->state == 8) {
        FUN_L14_002aed50_c618(moby);
        add_draw_callback(draw_blarg_heavy_interceptor_group_effects, moby);
        return;
    }
    *(int *)(moby_vars + 0x20C) = 0;
    if (moby->anim_flags & 2) {
        if (moby->seq == moby->prev_seq) {
            if (moby->seq == 2) {
                blend_moby_animation_c618(moby, 3, 0, 3);
            }
            if (moby->seq == 1) {
                blend_moby_animation_c618(moby, 2, 0, 10);
            }
        }
    }
    process_blarg_heavy_interceptor_hit_reactions(moby);
    if (moby->state > 0 && moby->state < 7) {
        if (FUN_001f9b48(&moby->pos, D_L14_001674C0) < 40.0f) {
            FUN_L00_0025a120(moby);
        }
        *(s16 *)(moby_vars + 0x200) = 0;
    }
    switch (moby->state) {
    case INTERCEPTOR_STATE_INITIALIZE:
        FUN_002141f8_c618(moby)[0xE] = 1;
        if (*(s16 *)(moby_vars + 0x1B6) == 1) {
            moby->flags = (moby->flags | 0x41) & 0xEFFF;
            moby->collision = 0;
            moby->update_dist = 0xFF;
            moby->state = 8;
            moby_vars[0x58] = truncate_float_to_s32(17.0f);
            moby_vars[0x5A] = truncate_float_to_s32(6.0f);
            *(s16 *)(moby_vars + 0x20A) = count_moby_group_members(moby);
            return;
        }
        if (*(int *)(moby_vars + 0x74) == -1 || *D_L14_001B0BB0[*(int *)(moby_vars + 0x74)] == 0 ||
            *(int *)(moby_vars + 0x78) == -1 || *D_L14_001B0BB0[*(int *)(moby_vars + 0x78)] == 0 ||
            *(int *)(moby_vars + 0x70) < 0 ||
            *(int *)(moby_vars + 0x204) == -1 || *D_L14_001B0BB0[*(int *)(moby_vars + 0x204)] == 0) {
            FUN_0020c828(moby);
            return;
        }
        moby->shadow_range = 0x20;
        *(float *)(moby_vars + 0x20) = 4.0f;
        *(s16 *)(moby_vars + 0x24) = 4;
        moby_vars[0x28] = 3;
        qcopy(&moby->pos, (char *)D_L14_001B0BB0[*(int *)(moby_vars + 0x74)] + 0x10);
        qcopy(moby_vars + 0x170, (char *)D_L14_001B0BB0[*(int *)(moby_vars + 0x78)] + 0x10);
        initialize_blarg_heavy_interceptor_paths(moby);
        *(s16 *)(moby_vars + 0x88) = 1;
        *(float *)(moby_vars + 0x21C) = -1.0f;
        *(float *)(moby_vars + 0x180) = *(float *)(moby_vars + 0x184) = *(float *)(moby_vars + 0x188) = 0.0f;
        *(float *)(moby_vars + 0x194) = *(float *)(moby_vars + 0x198) = *(float *)(moby_vars + 0x19C) = 0.0f;
        *(float *)(moby_vars + 0x160) = 0.0f;
        *(int *)(moby_vars + 0x20C) = 0;
        *(float *)(moby_vars + 0x164) = 0.0f;
        *(float *)(moby_vars + 0x1A0) = 0.0f;
        *(float *)(moby_vars + 0x1A4) = 0.0f;
        *(float *)(moby_vars + 0x1A8) = 0.0f;
        *(float *)(moby_vars + 0x1AC) = 0.0f;
        *(float *)(moby_vars + 0x1B0) = moby->rot.z;
        *(s16 *)(moby_vars + 0x1B4) = 0;
        *(float *)(moby_vars + 0x1B8) = 0.0f;
        memset_c618(moby_vars + 0xE0, 0, 0x40);
        memset_c618(moby_vars + 0x120, 0, 0x40);
        attach_manipulator_c618(moby, 0, moby_vars + 0xE0);
        attach_manipulator_c618(moby, 1, moby_vars + 0x120);
        *(float *)(moby_vars + 0x1C8) = *(float *)(moby_vars + 0x1C4) = 0.0f;
        *(int *)(moby_vars + 0x18C) = 0;
        moby->state = 1;
        moby->command = 0;
        break;
    case INTERCEPTOR_STATE_PATROL_AND_SCAN: {
        int found;
        int hit;

        moby->glow_color = 0xFF000000 | (D_L14_00161408 << 16) | (D_L14_00161404 << 8) | D_L14_00161400;
        if (is_turret_group_activated_or_empty(*(int *)(moby_vars + 0x70))) {
            moby->state = 3;
            *(s16 *)(moby_vars + 0x1F2) = scale_game_frames(D_L14_001613D0);
        }
        *(s16 *)(moby_vars + 0x202) = 0;
        *(s16 *)(moby_vars + 0x200) = 1;
        found = 0;
        hit = update_blarg_heavy_interceptor_aim_transforms(moby, 1);
        update_blarg_heavy_interceptor_patrol_aim_point(moby);
        update_blarg_heavy_interceptor_patrol_path_position(moby);
        FUN_L14_002aea08_c618(moby);
        if (hit) {
            lerp_vector_c618(vector_scratch, moby_vars + 0x90, moby_vars + 0xD0, 0.5f);
            qcopy(secondary_vector_scratch, moby_vars + 0x90);
            FUN_001e93e8_c618(secondary_vector_scratch, *(float *)(moby_vars + 0x214) * 0.8f);
            FUN_001e93e8_c618(vector_scratch, *(float *)(moby_vars + 0x214) * 0.48f);
            if (FUN_L00_001f2868_c618(*(float *)(moby_vars + 0x214) * 0.48f, vector_scratch, 0, 0, 0) ||
                FUN_L00_001f2868_c618(*(float *)(moby_vars + 0x214) * 0.9f, secondary_vector_scratch, 0, 0, 0)) {
                struct Moby *t = D_L14_00174540.target;

                if ((char *)t == D_001413D0_c618 || (t != 0 && (t->oclass == 0xCB || t->oclass == 0x10E))) {
                    found = 1;
                    *(struct Moby **)(moby_vars + 0x18C) = D_L14_00174540.target;
                }
            }
        }
        if (!found) {
            break;
        }
    }
    case INTERCEPTOR_STATE_ALERT_GROUP: {
        s32 voice = *(s32 *)(moby_vars + 0x1D0);

        if (voice != -1) {
            VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        *(s32 *)(moby_vars + 0x1D0) = -1;
        activate_waiting_turrets_in_group(*(int *)(moby_vars + 0x70));
        moby->state = 3;
        *(s16 *)(moby_vars + 0x1F2) = scale_game_frames(D_L14_001613D0);
        break;
    }
    case INTERCEPTOR_STATE_TRACK_TARGET:
        moby->glow_color = 0xFF000000 | (D_L14_00161414 << 16) | (D_L14_00161410 << 8) | D_L14_0016140C;
        if (*(struct Moby **)(moby_vars + 0x18C) != 0) {
            *(s16 *)(moby_vars + 0x200) = 1;
            *(s16 *)(moby_vars + 0x202) = 1;
            update_blarg_heavy_interceptor_aim_transforms(moby, 1);
            qcopy(vector_scratch, &(*(struct Moby **)(moby_vars + 0x18C))->pos);
            vector_scratch[2] += 0.5f;
            qcopy(secondary_vector_scratch, vector_scratch);
            secondary_vector_scratch[2] -= 4.0f;
            if (FUN_001efa68_c618(vector_scratch, secondary_vector_scratch, 0x12, 0, 0)) {
                qcopy(vector_scratch, &D_L14_00174560);
            } else {
                vector_scratch[2] -= 0.5f;
            }
            ease_c618((f32 *)(moby_vars + 0x170), vector_scratch[0], (f32 *)(moby_vars + 0x180), 0.003f, 0.2f, 0.0f);
            ease_c618((f32 *)(moby_vars + 0x174), vector_scratch[1], (f32 *)(moby_vars + 0x184), 0.003f, 0.2f, 0.0f);
            ease_c618((f32 *)(moby_vars + 0x178), vector_scratch[2], (f32 *)(moby_vars + 0x188), 0.003f, 0.2f, 0.0f);
            update_blarg_heavy_interceptor_patrol_path_position(moby);
            if ((s8)(*(struct Moby **)(moby_vars + 0x18C))->state < 0 || fast_dec_timer_s16((s16 *)(moby_vars + 0x1F2))) {
                *(float *)(moby_vars + 0x8C) = 0.0f;
                *(int *)(moby_vars + 0x80) = 0;
                *(s16 *)(moby_vars + 0x208) = 0;
                moby->state = 4;
            }
        } else {
            *(int *)(moby_vars + 0x80) = 0;
            *(float *)(moby_vars + 0x8C) = 0.0f;
            *(s16 *)(moby_vars + 0x208) = 0;
            moby->state = 4;
        }
    case INTERCEPTOR_STATE_WAIT_FOR_GROUP_READY:
        moby->glow_color = 0xFF000000 | (D_L14_00161414 << 16) | (D_L14_00161410 << 8) | D_L14_0016140C;
        qcopy(vector_scratch, &moby->pos);
        vector_scratch[3] = D_L14_0016139C;
        if (fast_bsphere_check((u8 *)vector_scratch, FUN_001fa6c0(moby->draw_dist)) >= 0) {
            float *p;

            FUN_L00_0024f7c8_c618(moby, 2, secondary_vector_scratch);
            p = (float *)(moby_vars + 0x1D4);
            p[0] = secondary_vector_scratch[0];
            p[1] = secondary_vector_scratch[1];
            p[2] = secondary_vector_scratch[2];
            *(int *)(moby_vars + 0x20C) |= 8;
            FUN_L14_002aea08_c618(moby);
        }
        if (moby->state == 4) {
            if (moby->prev_seq == 0) {
                blend_moby_animation_c618(moby, 2, 0, 5);
            }
            update_blarg_heavy_interceptor_return_path_position(moby, D_L14_001613F8, D_L14_001613F4, 1.0f);
            if (*(float *)(moby_vars + 0x1C0) != 0.0f || *(float *)(moby_vars + 0x1BC) != 0.0f) {
                ease_c618((f32 *)(moby_vars + 0x1C0), 0.0f, (f32 *)(moby_vars + 0x1C8), 0.004f, 0.2f, 0.0f);
                ease_c618((f32 *)(moby_vars + 0x1BC), 0.0f, (f32 *)(moby_vars + 0x1C4), 0.004f, 0.2f, 0.0f);
                if (absolute_float(*(float *)(moby_vars + 0x1C0)) < 0.0001f) {
                    *(float *)(moby_vars + 0x1C0) = 0.0f;
                }
                if (absolute_float(*(float *)(moby_vars + 0x1BC)) < 0.0001f) {
                    *(float *)(moby_vars + 0x1BC) = 0.0f;
                }
                FUN_L00_001fff28(moby_vars + 0xF0, 2, *(float *)(moby_vars + 0x1BC));
                FUN_L00_001fff28(moby_vars + 0x130, 1, *(float *)(moby_vars + 0x1C0));
            }
            if (try_retract_turret_group(*(int *)(moby_vars + 0x70))) {
                MoverPath *p = (MoverPath *)D_L14_001B0BB0[*(int *)(moby_vars + 0x204)];

                moby->state = 5;
                *(int *)(moby_vars + 0x80) = p->count - 2;
                *(float *)(moby_vars + 0x8C) = p->points[*(int *)(moby_vars + 0x80)].f[3];
                *(s16 *)(moby_vars + 0x208) = 0;
                update_blarg_heavy_interceptor_return_path_position(moby, 0.01f, 5.0f, -1.0f);
                blend_moby_animation_c618(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case INTERCEPTOR_STATE_FOLLOW_RETURN_PATH:
        FUN_L14_002aea08_c618(moby);
        update_blarg_heavy_interceptor_return_path_position(moby, 0.01f, 5.0f, -1.0f);
        if (*(s16 *)(moby_vars + 0x208) != 0) {
            int *tab = D_L14_001B0BB0[*(int *)(moby_vars + 0x78)];

            near_seg = 0;
            near_t = 0.0f;
            FUN_L00_0025df68_c618(tab, &moby->pos, vector_scratch, &near_seg, &near_t, 0, 40.0f, 5.0f, 0.0f);
            *(int *)(moby_vars + 0x84) = near_seg;
            *(float *)(moby_vars + 0x1CC) = near_t;
            *(s16 *)(moby_vars + 0x200) = 0;
            moby->state = 6;
        }
        break;
    case INTERCEPTOR_STATE_REJOIN_PATROL: {
        int i;
        int source_segment_index;
        float source_path_distance;
        char *seg;
        int *tab;
        float t;
        int dir;

        FUN_L14_002aea08_c618(moby);
        source_segment_index = *(int *)(moby_vars + 0x84);
        seg = (char *)D_L14_001B0BB0[*(int *)(moby_vars + 0x78)];
        source_path_distance = 0.0f;
        for (i = 0; i < source_segment_index; i++) {
            source_path_distance += *(float *)(seg + i * 16 + 0x1C);
        }
        source_path_distance += *(float *)(moby_vars + 0x1CC);
        tab = D_L14_001B0BB0[*(int *)(moby_vars + 0x74)];
        t = source_path_distance / *(float *)(moby_vars + 0x16C) * *(float *)(moby_vars + 0x190);
        dir = *(s16 *)(moby_vars + 0x8A);
        path_seg = 0;
        path_t = 0.0f;
        FUN_L00_0025d808_c618(tab, vector_scratch, &path_seg, &path_t, dir, t);
        ease_c618(&moby->pos.x, vector_scratch[0], (f32 *)(moby_vars + 0x194), 0.005f, D_L14_001613A4_c618, 0.0f);
        ease_c618(&moby->pos.y, vector_scratch[1], (f32 *)(moby_vars + 0x198), 0.005f, D_L14_001613A4_c618, 0.0f);
        ease_c618(&moby->pos.z, vector_scratch[2], (f32 *)(moby_vars + 0x19C), 0.005f, D_L14_001613A4_c618, 0.0f);
        update_blarg_heavy_interceptor_aim_transforms(moby, 0);
        if (FUN_001f9b48(&moby->pos, vector_scratch) < 0.5f) {
            moby->state = 1;
        }
        FUN_L00_0025d808_c618(D_L14_001B0BB0[*(int *)(moby_vars + 0x78)], vector_scratch, moby_vars + 0x84, moby_vars + 0x1CC, *(s16 *)(moby_vars + 0x8A), 0.0f);
        ease_c618((f32 *)(moby_vars + 0x170), vector_scratch[0], (f32 *)(moby_vars + 0x180), D_L14_001613A0_c618, D_L14_001613A4_c618, 0.0f);
        ease_c618((f32 *)(moby_vars + 0x174), vector_scratch[1], (f32 *)(moby_vars + 0x184), D_L14_001613A0_c618, D_L14_001613A4_c618, 0.0f);
        ease_c618((f32 *)(moby_vars + 0x178), vector_scratch[2], (f32 *)(moby_vars + 0x188), D_L14_001613A0_c618, D_L14_001613A4_c618, 0.0f);
        break;
    }
    case INTERCEPTOR_STATE_INITIALIZE_DEATH_FALL: {
        s32 voice;
        float death_launch_angle;

        *(s16 *)(moby_vars + 0x1F0) = scale_game_frames(D_L14_001613D4);
        activate_waiting_turrets_in_group(*(int *)(moby_vars + 0x70));
        voice = *(s32 *)(moby_vars + 0x1D0);
        if (voice != -1) {
            VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        *(s32 *)(moby_vars + 0x1D0) = -1;
        FUN_L00_0025e450_c618(moby, moby_vars + 0x40, &moby->pos, 0.0f, 0.0f, 5, 2, 4, 4.0f, 2.0f, 9.0f, 1.0f, 1, 15.0f, 1, 5, -1, 0);
        subtract_vector_c618(vector_scratch, &moby->pos, &hero.motion.pos);
        vector_scratch[2] = 0.0f;
        normalize_vector_c618(vector_scratch, vector_scratch, random_float_c618(D_L14_001613D8 * D_0015ED60_c618, D_L14_001613DC * D_0015ED60_c618));
        cross_vectors_c618(secondary_vector_scratch, &hero.gravity_dir, vector_scratch);
        death_launch_angle = randf_sym(0.6981317f, 1.2217305f);
        build_look_at_matrix_c618(moby_vars + 0x1E0, vector_scratch, &hero.gravity_dir, death_launch_angle);
        *(float *)(moby_vars + 0x1E8) = random_float_c618(D_L14_001613E0 * D_0015ED60_c618, D_L14_001613E4 * D_0015ED60_c618);
        moby->scale *= 0.1f;
        *(float *)(moby_vars + 0x140) = 10.0f;
        *(float *)(moby_vars + 0x144) = 10.0f;
        *(float *)(moby_vars + 0x148) = 10.0f;
        *(float *)(moby_vars + 0x1F4) = randf_sym(0.0f, 0.08726646f);
        *(float *)(moby_vars + 0x1F8) = randf_sym(0.0f, 0.08726646f);
        *(float *)(moby_vars + 0x1FC) = randf_sym(0.0f, 0.08726646f);
        moby->rot.x = fast_add_rotations(moby->rot.x, randf_sym(0.0f, 0.7853982f));
        moby->rot.y = fast_add_rotations(moby->rot.y, randf_sym(0.0f, 0.7853982f));
        moby->state = 9;
        FUN_L14_002aead8_c618(moby);
        return;
    }
    case INTERCEPTOR_STATE_DEATH_FALL: {
        int i;

        add_vector_c618(&moby->pos, &moby->pos, moby_vars + 0x1E0);
        if (absolute_float(*(float *)(moby_vars + 0x1E0)) > D_L14_001613F0 * D_0015ED60_c618) {
            *(float *)(moby_vars + 0x1E0) *= (D_L14_001613EC - 1.0f) * D_0015ED60_c618 + 1.0f;
        }
        if (absolute_float(*(float *)(moby_vars + 0x1E4)) > D_L14_001613F0 * D_0015ED60_c618) {
            *(float *)(moby_vars + 0x1E4) *= (D_L14_001613EC - 1.0f) * D_0015ED60_c618 + 1.0f;
        }
        if (*(float *)(moby_vars + 0x1E8) > 0.0f) {
            *(float *)(moby_vars + 0x1E8) *= (D_L14_001613EC - 1.0f) * D_0015ED60_c618 + 1.0f;
        }
        *(float *)(moby_vars + 0x1E8) -= D_L14_001613E8 * frame_scale_sq;
        moby->rot.z = fast_add_rotations(moby->rot.z, *(float *)(moby_vars + 0x1FC));
        *(float *)(moby_vars + 0x140) = 10.0f;
        *(float *)(moby_vars + 0x144) = 10.0f;
        *(float *)(moby_vars + 0x148) = 10.0f;
        if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f || moby->pos.y > 1021.0f ||
            moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
            FUN_0020c828(moby);
            return;
        }
        if (fast_dec_timer_s16((s16 *)(moby_vars + 0x1F0)) || coll_sphere(0.5f, &moby->pos, 0, moby)) {
            FUN_L00_0025e450_c618(moby, moby_vars + 0x40, &moby->pos, 2.0f, 1.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, 1, 15.0f, 1, 5, -1, 0);
            FUN_0020c828(moby);
            return;
        }
        FUN_L00_0026cbb0(&moby->pos, D_L14_0015F580, 0x4F007FFF, 0x1FFFFFFF,
                         random_int_between(scale_game_frames(10), scale_game_frames(20)), 1, 60000.0f);
        for (i = 0; i < 2; i++) {
            int signed_puff_random_value = random_integer_below(6);
            float size;
            int puff_rgb_component;
            unsigned char *p;

            if (random_integer_below(2)) {
                signed_puff_random_value = -signed_puff_random_value;
            }
            size = random_float_c618(40000.0f, 100000.0f);
            puff_rgb_component = random_int_between(0x30, 0xFF);
            p = FUN_L00_0026d000_c618(&moby->pos, 0.025f, 1.0f, 1.0f, signed_puff_random_value, D_L14_0015F580, size, puff_rgb_component | (0x60000000 | (puff_rgb_component << 16) | (puff_rgb_component << 8)));
            if (p != 0) {
                unsigned char *puff_tail;

                p[3] = 0x44;
                puff_tail = p + 0x20;
                *(s16 *)(p + 0xA) = scale_game_frames(60);
                *(int *)(puff_tail + 4) = 2;
                puff_tail[0xA] = 0x60;
                puff_tail[0xB] = p[0xA];
            }
        }
        FUN_L14_002aead8_c618(moby);
        return;
    }
    }
    if ((s8)moby->state >= 0 && moby->prev_seq == 1) {
        if (moby->command == 1) {
            spawn_particles_at_shape_points(moby, D_L14_0015F580, 0xFFFF);
        }
    } else {
        moby->command = 0;
    }
    if (moby->state == 1 && !voice_is_playing(moby, *(s32 *)(moby_vars + 0x1D0))) {
        *(s32 *)(moby_vars + 0x1D0) = allocate_voice_c618(0, 4, moby);
    }
    if ((moby->state == 3 || moby->state == 4) && !voice_is_playing(moby, *(s32 *)(moby_vars + 0x1D0))) {
        *(s32 *)(moby_vars + 0x1D0) = allocate_voice_c618(2, 4, moby);
    }
}
/* per-frame update: runs the hit query, decays a timer, then dispatches on the hit result */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AEAF8), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void process_blarg_heavy_interceptor_hit_reactions(struct Moby *moby) {
    enum {
        HIT_REACTION_NO_ACTION = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DEATH_FALL = 2,
        HIT_REACTION_PLAY_HIT_ANIMATION = 3,
        HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION = 4,
        HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION = 5,
        HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH = 6,
        HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION = 7,
        HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH = 8,
        HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH = 9,
        HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION = 10,
        HIT_REACTION_NONE = 11
    };
    char *data = (char *)moby->pvars;
    int hit_reaction_status;
    float damage_amount;
    int hit_reaction_result;
    damage_amount = 0.0f;
    hit_reaction_result = hit_reaction(moby, find_moby_hit(moby, 0x330000, 0), data + 0x20, 0, &hit_reaction_status, &damage_amount, 0, 4);
    if (hit_reaction_status != 1 && moby->state < 7) {
        float remaining_health = *(float *)(data + 0x20) - damage_amount;
        *(float *)(data + 0x20) = remaining_health;
        if (remaining_health <= 0.0f)
            hit_reaction_result = 1;
        if (moby->state == 1)
            moby->state = 2;
        if (hit_reaction_status == 3)
            moby->command = 1;
        switch (hit_reaction_result) {
        case HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH:
        case HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION:
            data[0x67] = 0x64;
            blend_moby_animation(moby, 1, 0, 5);
            break;
        case HIT_REACTION_PLAY_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH:
            ((unsigned char *)data)[0x67] = 0xC8;
            blend_moby_animation(moby, 1, 0, 5);
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_DEATH_FALL:
            moby->state = 7;
            moby->flags &= 0xEFFF;
            FUN_L00_00257470(moby, 0, -1);
            break;
        case HIT_REACTION_NO_ACTION:
        case HIT_REACTION_NONE:
            break;
        }
        trooper_anim_update(moby, (short *)(data + 0x60));
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, data + 0x60);
}
/* Draws the moby's ring of lights: builds a matrix facing the camera from the moby's axis, transforms the ring
 * vertices through it and draws them, then flips the matrix and draws the back face. */
extern s32 D_L14_00161418 __attribute__((sda));
extern float D_L14_001D7A60[][3];
extern s32 D_L14_001D80C8[];
extern char D_L14_001D7E38[];
extern char D_L14_001674C0_r[] __asm__("D_L14_001674C0");
void draw_blarg_heavy_interceptor_camera_facing_glow(struct Moby *m) __asm__("FUN_L14_002ae0a8");

extern void draw_blarg_heavy_interceptor_camera_facing_glow(struct Moby *m) __asm__("FUN_L14_002ae0a8");
extern void FUN_L00_001fde98(int, void *, void *, void *, int);
extern void cross_r(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void normalize_r(void *, void *, float) __asm__("FUN_001f9bf8");
extern void scale_r(void *, void *, float) __asm__("FUN_001f9a68");
extern void sub_r(void *, void *, void *) __asm__("FUN_001f9a28");
extern void xform_r(void *, void *, void *) __asm__("FUN_001f9d20");
extern s64 tex_r(s32) __asm__("FUN_001f44b8");
extern void sreg_r(s32, s64) __asm__("FUN_00233980");
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void draw_blarg_heavy_interceptor_light_ring(char *moby) __asm__("FUN_L14_002ada18");

void draw_blarg_heavy_interceptor_light_ring(char *moby) {
    char *moby_vars = *(char **)(moby + 0x78);
    struct {
        float mat[4][4];    /* camera-facing frame */
        float verts[82][3]; /* ring vertices transformed by mat */
    } f;
    u32 cols[84];
    float ring_vertex_scratch[4];
    int i;
    int j;

    draw_blarg_heavy_interceptor_camera_facing_glow((struct Moby *)moby);
    qcopy(f.mat[0], moby_vars + 0xA0);
    sub_r(f.mat[2], moby_vars + 0xD0, D_L14_001674C0_r);
    cross_r(f.mat[1], f.mat[0], f.mat[2]);
    normalize_r(f.mat[1], f.mat[1], 1.0f);
    cross_r(f.mat[2], f.mat[1], f.mat[0]);
    qcopy(f.mat[3], moby_vars + 0xD0);
    f.mat[3][3] = 1.0f;
    sreg_r(6, tex_r(0x16));
    sreg_r(0x42, 0x8000000048);
    sreg_r(8, 5);
    sreg_r(0x14, 0xFF9000000260);
    sreg_r(0x47, 0x50000);
    sreg_r(0x4A, 0);
    font_queue_vu_state();
    for (i = 0; i < D_L14_00161418; i++) {
        float *s = D_L14_001D7A60[i];
        float *o = f.verts[i];
        ring_vertex_scratch[0] = s[0];
        ring_vertex_scratch[1] = s[1];
        ring_vertex_scratch[2] = s[2];
        ring_vertex_scratch[3] = 1.0f;
        xform_r(ring_vertex_scratch, ring_vertex_scratch, f.mat);
        o[0] = ring_vertex_scratch[0];
        o[1] = ring_vertex_scratch[1];
        o[2] = ring_vertex_scratch[2];
        if (*(short *)(moby_vars + 0x202) != 0) {
            cols[i] = D_L14_001D80C8[i] & 0xFF0000FF;
        }
    }
    if (*(short *)(moby_vars + 0x202) != 0) {
        FUN_L00_001fde98(D_L14_00161418, f.verts, cols, D_L14_001D7E38, 1);
    } else {
        FUN_L00_001fde98(D_L14_00161418, f.verts, D_L14_001D80C8, D_L14_001D7E38, 1);
    }
    scale_r(f.mat[1], f.mat[1], -1.0f);
    scale_r(f.mat[2], f.mat[2], -1.0f);
    for (j = 0; j < D_L14_00161418; j++) {
        float *o = f.verts[j];
        float *s = D_L14_001D7A60[j];
        ring_vertex_scratch[0] = s[0];
        ring_vertex_scratch[1] = s[1];
        ring_vertex_scratch[2] = s[2];
        ring_vertex_scratch[3] = 1.0f;
        xform_r(ring_vertex_scratch, ring_vertex_scratch, f.mat);
        o[0] = ring_vertex_scratch[0];
        o[1] = ring_vertex_scratch[1];
        o[2] = ring_vertex_scratch[2];
    }
    if (*(short *)(moby_vars + 0x202) != 0) {
        FUN_L00_001fde98(D_L14_00161418, f.verts, cols, D_L14_001D7E38, 1);
    } else {
        FUN_L00_001fde98(D_L14_00161418, f.verts, D_L14_001D80C8, D_L14_001D7E38, 1);
    }
}
/* Draws the moby's spinning beam: sixteen camera-facing quads stepped out along its heading, each pair
 * fainter and larger than the last. */
typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_2add48;

extern float D_L14_00161434 __attribute__((sda));
extern float D_L14_00161438 __attribute__((sda));
extern float D_L14_0016143C __attribute__((sda));
extern float D_L14_00161440 __attribute__((sda));
extern int D_L14_00161444 __attribute__((sda));
extern int D_L14_00161448 __attribute__((sda));
extern int D_L14_0016144C __attribute__((sda));
extern int D_L14_00161450 __attribute__((sda));
extern int D_L14_00161454 __attribute__((sda));
extern int D_L14_00161458 __attribute__((sda));
extern int D_L14_0016145C __attribute__((sda));
extern int D_L14_00161460 __attribute__((sda));
extern char D_L14_001674C0_b[] __asm__("D_L14_001674C0");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void scale_b(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_b(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_b(void *, void *, void *) __asm__("FUN_001f9a28");
extern unsigned long tex_b(s32) __asm__("FUN_001f44b8");
extern void sreg_b(s32, s64) __asm__("FUN_00233980");
extern float atan2_b(float, float) __asm__("FUN_001f9e90");
extern float lenxy_b(void *) __asm__("FUN_001f9b20");
extern void rotmat_b(void *, void *) __asm__("FUN_001fa030");
extern void xform_b(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_b(void *, int, int) __asm__("FUN_001f7d30");

float D_L14_001D8600[4][4] = {
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
};
void draw_blarg_heavy_interceptor_spinning_beam(char *moby) __asm__("FUN_L14_002add48");

void draw_blarg_heavy_interceptor_spinning_beam(char *moby) {
    char *d = *(char **)(moby + 0x78);
    float src[4][4];
    Pk_2add48 pk;
    float beam_quad_transform[4][4];
    float quad_translation_offset[4];
    float beam_offset_direction[4];
    float beam_to_camera_vector[4];
    float camera_facing_angles[4];
    float rm[3][4];
    float size;
    float step;
    int r, g, b, a;
    u32 col;
    int i, j;
    float *beam_origin;

    beam_origin = (float *)(d + 0x1D4);
    src[3][0] = beam_origin[0];
    src[3][1] = beam_origin[1];
    src[3][2] = beam_origin[2] + 0.125f;
    src[3][3] = 1.0f;
    src[0][0] = fast_cos(*(float *)(d + 0x160));
    src[0][1] = fast_sin(*(float *)(d + 0x160));
    src[0][2] = 0.0f;
    *(float *)(d + 0x160) = fast_add_rotations(*(float *)(d + 0x160), -0.06981317f);
    pk.b = tex_b(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    qcopy(beam_offset_direction, src[0]);
    qcopy(beam_quad_transform[3], src[3]);
    beam_quad_transform[3][3] = 1.0f;
    sub_b(beam_to_camera_vector, D_L14_001674C0_b, beam_quad_transform[3]);
    camera_facing_angles[2] = atan2_b(beam_to_camera_vector[0], beam_to_camera_vector[1]);
    camera_facing_angles[1] = -atan2_b(lenxy_b(beam_to_camera_vector), beam_to_camera_vector[2]);
    camera_facing_angles[0] = 0.0f;
    rotmat_b(rm, camera_facing_angles);
    qcopy(beam_quad_transform[0], rm[0]);
    qcopy(beam_quad_transform[1], rm[1]);
    qcopy(beam_quad_transform[2], rm[2]);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    size = D_L14_0016143C;
    step = D_L14_00161434;
    r = D_L14_00161444;
    g = D_L14_00161448;
    b = D_L14_0016144C;
    a = D_L14_00161450;
    sreg_b(0x47, 0x51001);
    for (i = 0; i < 16; i++) {
        col = (a << 24) | (b << 16) | (g << 8) | r;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
        scale_b(quad_translation_offset, beam_offset_direction, step);
        for (j = 0; j < 4; j++) {
            scale_b(pk.m[j], D_L14_001D8600[j], size);
            xform_b(pk.m[j], pk.m[j], beam_quad_transform);
            add_b(pk.m[j], pk.m[j], quad_translation_offset);
        }
        draw_b(&pk, 0, 0);
        scale_b(quad_translation_offset, beam_offset_direction, step * -2.0f);
        for (j = 0; j < 4; j++) {
            add_b(pk.m[j], pk.m[j], quad_translation_offset);
        }
        draw_b(&pk, 0, 0);
        r -= D_L14_00161454;
        g -= D_L14_00161458;
        b -= D_L14_0016145C;
        a -= D_L14_00161460;
        size += D_L14_00161440;
        step += D_L14_00161438;
        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
        if (a < 0)
            a = 0;
    }
}
/* Draws the moby's glow quad, turned to face the camera. */
typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_2ae0a8;

extern char D_L14_001674C0_e[] __asm__("D_L14_001674C0");
extern void scale_e(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_e(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_e(void *, void *, void *) __asm__("FUN_001f9a28");
extern unsigned long tex_e(s32) __asm__("FUN_001f44b8");
extern void sreg_e(s32, s64) __asm__("FUN_00233980");
extern float atan2_e(float, float) __asm__("FUN_001f9e90");
extern float lenxy_e(void *) __asm__("FUN_001f9b20");
extern void rotmat_e(void *, void *) __asm__("FUN_001fa030");
extern void xform_e(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_e(void *, int, int) __asm__("FUN_001f7d30");

extern float D_L14_00161474 __attribute__((sda));
extern float D_L14_00161478 __attribute__((sda));
void draw_blarg_heavy_interceptor_camera_facing_glow(struct Moby *m) {
    char *d = (char *)m->pvars;
    float glow_quad_transform[4][4];
    float glow_position_offset[4];
    Pk_2ae0a8 pk;
    float glow_to_camera_vector[4];
    float camera_facing_angles[4];
    u32 col;
    int i;
    qcopy(glow_quad_transform[3], d + 0xD0);
    glow_quad_transform[3][3] = 1.0f;
    scale_e(glow_position_offset, d + 0xA0, D_L14_00161478);
    add_e(glow_quad_transform[3], glow_quad_transform[3], glow_position_offset);
    pk.b = tex_e(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    sreg_e(0x47, 0x51001);
    sub_e(glow_to_camera_vector, D_L14_001674C0_e, glow_quad_transform[3]);
    camera_facing_angles[2] = atan2_e(glow_to_camera_vector[0], glow_to_camera_vector[1]);
    camera_facing_angles[1] = -atan2_e(lenxy_e(glow_to_camera_vector), glow_to_camera_vector[2]);
    camera_facing_angles[0] = 0.0f;
    rotmat_e(glow_quad_transform, camera_facing_angles);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    if (*(short *)(d + 0x202) != 0) {
        col = 0x7F40407F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    } else {
        col = 0x7F7F7F7F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    }
    for (i = 0; i < 4; i++) {
        scale_e(pk.m[i], D_L14_001D8600[i], D_L14_00161474);
        xform_e(pk.m[i], pk.m[i], glow_quad_transform);
    }
    draw_e(&pk, 0, 0);
}
/* Draws the moby's glow quad, pulled toward the camera so it is not hidden by the moby. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AF4A0), where it is exact; names translated to the US level program. */

typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    u64 a, b, c, d;
} Pk_2af4a0;

extern char D_L14_001674C0_d[] __asm__("D_L14_001674C0") __attribute__((section(".data")));
float D_L14_001D8640[4][4] = {
    {-1.0f, 1.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 0.0f, 1.0f},
};
extern float vector_length_xyz(void *a);
extern void add_vector_xyz(void *, void *, void *);
extern void draw_geometry_quad(void *, void *, int);
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
u64 get_effect_texture_alt(s32) __asm__("FUN_001f44b8");

extern short D_L14_0016147C __attribute__((sda));
extern short D_L14_00161480 __attribute__((sda));
void FUN_L14_002ae260(struct Moby *m) {
    char *d = (char *)m->pvars;
    float glow_quad_position[4];
    Pk_2af4a0 pk;
    float glow_to_camera_vector[4];
    float camera_pull_offset[4];
    float glow_to_camera_distance;
    u32 col;
    int j;
    qcopy(glow_quad_position, d + 0x90);
    glow_quad_position[2] += 0.01f;
    glow_quad_position[3] = 1.0f;
    pk.b = get_effect_texture_alt(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    subtract_vector_xyz(glow_to_camera_vector, D_L14_001674C0_d, glow_quad_position);
    glow_to_camera_distance = vector_length_xyz(glow_to_camera_vector);
    if (0.0f < glow_to_camera_distance) {
        float camera_pull_distance = *(float *)(d + 0x218);
        float camera_distance_margin = glow_to_camera_distance - *(float *)&D_L14_0016147C;
        if (camera_distance_margin < camera_pull_distance) {
            camera_pull_distance = camera_distance_margin;
            if (camera_distance_margin < 0.0f)
                camera_pull_distance = 0.0f;
        }
        normalize_vector_xyz(camera_pull_offset, glow_to_camera_vector, camera_pull_distance);
        add_vector_xyz(glow_quad_position, glow_quad_position, camera_pull_offset);
    }
    pk.uv[2] = 0.0f;
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    if (*(short *)(d + 0x202) != 0) {
        col = 0x7F40407F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    } else {
        col = *(u32 *)&D_L14_00161480;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    }
    for (j = 0; j < 4; j++) {
        scale_vector_xyz(pk.m[j], D_L14_001D8640[j], *(float *)(d + 0x214));
        add_vector_xyz(pk.m[j], pk.m[j], glow_quad_position);
    }
    draw_geometry_quad(&pk, 0, 0);
}
/* Measures the segments of the moby's three paths, sums two of them and places it on the first two. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AF688), where it is exact; names translated to the US level program. */

extern float FUN_001f9b48(void *, void *);
extern int *D_L14_001B0BB0[];
extern void sample_path(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");
extern int FUN_L00_0025d808_2af688(void *, void *, void *, void *, int,
                                    float) __asm__("FUN_L00_0025d808");

void initialize_blarg_heavy_interceptor_paths(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int *path;
    int i;
    float segment_length;
    path = D_L14_001B0BB0[*(int *)(moby_vars + 0x78)];
    *(int *)(moby_vars + 0x16C) = 0;
    for (i = 0; i < path[0]; i++) {
        segment_length = FUN_001f9b48((char *)path + (i * 16 + 0x10), (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
        *(float *)((char *)path + (i << 4) + 0x1C) = segment_length;
        if (i != path[0] - 1) {
            *(float *)(moby_vars + 0x16C) += segment_length;
        }
    }
    path = D_L14_001B0BB0[*(int *)(moby_vars + 0x74)];
    *(int *)(moby_vars + 0x190) = 0;
    for (i = 0; i < path[0]; i++) {
        segment_length = FUN_001f9b48((char *)path + (i * 16 + 0x10), (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
        *(float *)((char *)path + (i << 4) + 0x1C) = segment_length;
        if (i != path[0] - 1) {
            *(float *)(moby_vars + 0x190) += segment_length;
        }
    }
    path = D_L14_001B0BB0[*(int *)(moby_vars + 0x204)];
    for (i = 0; i < path[0]; i++) {
        *(float *)((char *)path + (i << 4) + 0x1C) =
            FUN_001f9b48((char *)path + (i * 16 + 0x10), (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
    }
    if (*(float *)(moby_vars + 0x210) != 0.0f) {
        int *sampling_path = D_L14_001B0BB0[*(int *)(moby_vars + 0x78)];
        *(int *)(moby_vars + 0x84) = 0;
        *(float *)(moby_vars + 0x1CC) = 0.0f;
        FUN_L00_0025d808_2af688(sampling_path, &moby->pos, moby_vars + 0x84, moby_vars + 0x1CC, *(short *)(moby_vars + 0x8A),
                                 *(float *)(moby_vars + 0x210) * *(float *)(moby_vars + 0x16C));
        *(float *)(moby_vars + 0x8C) = 0.0f;
        *(int *)(moby_vars + 0x80) = 0;
        FUN_L00_0025d808_2af688(sampling_path, &moby->pos, moby_vars + 0x80, moby_vars + 0x8C, *(short *)(moby_vars + 0x8A),
                                 *(float *)(moby_vars + 0x210) * *(float *)(moby_vars + 0x190));
    } else {
        *(int *)(moby_vars + 0x80) = 0;
        *(int *)(moby_vars + 0x84) = 0;
        *(float *)(moby_vars + 0x8C) = 0.0f;
        *(float *)(moby_vars + 0x1CC) = 0.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AF918), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2,
                                float eps) __asm__("FUN_L00_0025b8c0");
extern int *D_L14_001B0BB0[];
extern f32 D_L14_001613A0_d __asm__("D_L14_001613A0") __attribute__((sda));
extern f32 D_L14_001613A4_d __asm__("D_L14_001613A4") __attribute__((sda));
extern void sample_path(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");
extern int FUN_L00_0025d808_2af918(int *tab, float *out, int *a, float *b, int c,
                                    float d) __asm__("FUN_L00_0025d808");

void update_blarg_heavy_interceptor_patrol_aim_point(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float path_target_position[4];
    float save = *(float *)(data + 0x1CC);
    int save2 = *(int *)(data + 0x84);
    int r = FUN_L00_0025d808_2af918(D_L14_001B0BB0[*(int *)(data + 0x78)], path_target_position, (int *)(data + 0x84),
                                     (float *)(data + 0x1CC), *(short *)(data + 0x8A),
                                     *(float *)(data + 0x168));
    FUN_L00_0025b8c0_c((float *)(data + 0x170), (float *)(data + 0x180), path_target_position[0],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    FUN_L00_0025b8c0_c((float *)(data + 0x174), (float *)(data + 0x184), path_target_position[1],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    FUN_L00_0025b8c0_c((float *)(data + 0x178), (float *)(data + 0x188), path_target_position[2],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    if (*(short *)(data + 0x8A) == 0) {
        if (r != 0) {
            *(float *)(data + 0x1CC) = save;
            *(int *)(data + 0x84) = save2;
            *(float *)(data + 0x168) = -*(float *)(data + 0x168);
        }
    }
}
/* Moves the moby to the point on its second path that matches how far along its first path it is. */
void update_blarg_heavy_interceptor_patrol_path_position(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *seg = (char *)D_L14_001B0BB0[*(int *)(data + 0x78)];
    int source_segment_index = *(int *)(data + 0x84);
    float source_path_distance = 0.0f;
    float out[4];
    int destination_segment_index;
    float frac;
    int i;
    int *tab;
    int dir;
    float destination_path_distance;

    for (i = 0; i < source_segment_index; i++) {
        source_path_distance += *(float *)(seg + i * 16 + 0x1C);
    }
    source_path_distance += *(float *)(data + 0x1CC);
    tab = D_L14_001B0BB0[*(int *)(data + 0x74)];
    destination_path_distance = source_path_distance / *(float *)(data + 0x16C) * *(float *)(data + 0x190);
    dir = *(short *)(data + 0x8A);
    destination_segment_index = 0;
    frac = 0.0f;
    FUN_L00_0025d808_2af918(tab, out, &destination_segment_index, &frac, dir, destination_path_distance);
    FUN_L00_0025b8c0_c(&moby->pos.x, (float *)(data + 0x194), out[0],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.y, (float *)(data + 0x198), out[1],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.z, (float *)(data + 0x19C), out[2],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
}
/* Picks a target point from a list or searches for one, then eases the moby's position toward it per axis. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AFB40), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2,
                                float eps) __asm__("FUN_L00_0025b8c0");
extern int FUN_L00_0025d808_2afb40(void *, void *, void *, void *, int,
                                    float) __asm__("FUN_L00_0025d808");

extern short D_L14_001613FC __attribute__((sda));
void update_blarg_heavy_interceptor_return_path_position(struct Moby *moby, float position_easing_factor, float path_traversal_speed, float path_direction_multiplier) {
    char *data = (char *)moby->pvars;
    int *list = D_L14_001B0BB0[*(int *)(data + 0x204)];
    float target_position[4];
    if (*(short *)(data + 0x208) != 0) {
        qcopy(target_position, (char *)list + list[0] * 16);
    } else {
        if (FUN_L00_0025d808_2afb40(list, target_position, data + 0x80, data + 0x8C, 0, path_traversal_speed * frame_time * path_direction_multiplier) !=
            0) {
            *(short *)(data + 0x208) = 1;
        }
    }
    FUN_L00_0025b8c0_c(&moby->pos.x, (float *)(data + 0x194), target_position[0], position_easing_factor,
                       *(float *)&D_L14_001613FC, 0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.y, (float *)(data + 0x198), target_position[1], position_easing_factor,
                       *(float *)&D_L14_001613FC, 0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.z, (float *)(data + 0x19C), target_position[2], position_easing_factor,
                       *(float *)&D_L14_001613FC, 0.0f);
}
extern f32 D_L14_001613A8 __attribute__((sda));
extern f32 D_L14_001613AC __attribute__((sda));
extern f32 D_L14_001613B0 __attribute__((sda));
extern f32 D_L14_001613B4 __attribute__((sda));
extern f32 D_L14_001613B8 __attribute__((sda));
extern s32 D_L14_001613BC __attribute__((sda));
extern f32 D_L14_001613C0 __attribute__((sda));
extern f32 D_L14_001613C4 __attribute__((sda));
extern f32 D_L14_001613C8 __attribute__((sda));
extern f32 D_L14_001613CC __attribute__((sda));
extern void FUN_L00_00262b00(f32, f32, u8 *, f32 *, f32 *);
extern void FUN_L00_00262b80(u8 *, u8 *, u8 *, f32, f32, f32);
extern s32 fast_dec_timer_s16(s16 *) __asm__("FUN_001f9770");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");

/* 0x002AEA08, 208 bytes.  Per-frame update of a moby's aim and turn state from
 * level tuning constants.  The ten constants are small data ($gp-relative),
 * declared with __attribute__((sda)).
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002AFC48), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002aea08(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    f32 heading_offset;
    FUN_L00_00262b00(D_L14_001613A8, D_L14_001613AC, moby, (f32 *)(data + 0x1A0),
                     (f32 *)(data + 0x1A4));
    FUN_L00_00262b80(moby, data + 0x1A8, data + 0x1AC, D_L14_001613B0, D_L14_001613B4,
                     D_L14_001613B8);
    if (*(u8 *)(moby + 0x20) == 1) {
        if (fast_dec_timer_s16((s16 *)(data + 0x1B4))) {
            *(s16 *)(data + 0x1B4) = scale_game_frames(D_L14_001613BC);
            heading_offset = D_L14_001613C0;
            if (random_integer_below(2))
                heading_offset = -heading_offset;
            *(f32 *)(data + 0x1B0) = fast_add_rotations(*(f32 *)(moby + 0x48), heading_offset);
        }
        vendor_spring((f32 *)(moby + 0x48), (f32 *)(data + 0x1B8), *(f32 *)(data + 0x1B0),
                         D_L14_001613C4, D_L14_001613C8, D_L14_001613CC);
    }
}
extern s32 random_integer_below(s32 n) __asm__("FUN_00213260");
extern f32 FUN_001fa6c0(s16);
extern s32 fast_bsphere_check(u8 *, f32) __asm__("FUN_001fa728");
extern void build_moby_bone_transform(u8 *, s32, u8 *) __asm__("FUN_0020cca8");
extern void fast_vec_normalize(u8 *, u8 *, f32) __asm__("FUN_001f9bf8");

/* 0x002AEAD8, 200 bytes.  Checks a flag and a position test, then sets up
 * three vectors in the moby data.  The 16-byte copy is the qcopy idiom
 * (include/qcopy.h).
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002AFD18), where it is exact; names translated to the US level
 * program. */
void prepare_blarg_heavy_interceptor_light_ring_transform(u8 *moby) __asm__("FUN_L14_002aead8");

void prepare_blarg_heavy_interceptor_light_ring_transform(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    u8 *bone_transform;
    f32 bounding_sphere[4];
    if (random_integer_below(5)) {
        qcopy(bounding_sphere, moby + 0x10);
        bounding_sphere[3] = D_L14_0016139C;
        if (fast_bsphere_check((u8 *)bounding_sphere, FUN_001fa6c0(*(s16 *)(moby + 0x32))) >= 0) {
            bone_transform = data + 0xA0;
            build_moby_bone_transform(moby, 1, bone_transform);
            fast_vec_normalize(bone_transform, bone_transform, 1.0f);
            fast_vec_normalize(data + 0xB0, data + 0xB0, 1.0f);
            fast_vec_normalize(data + 0xC0, data + 0xC0, 1.0f);
            *(s32 *)(data + 0x20C) = 2;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AFDE0), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int FUN_001fa728_c(char *, float) __asm__("FUN_001fa728");
extern void FUN_0020cca8_c(char *, int, void *) __asm__("FUN_0020cca8");
extern void FUN_L00_001fff28(void *, int, float);
extern void FUN_L14_002ae260_u(void) __asm__("FUN_L14_002ae260");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
void enqueue_callback_list_1(int arg0, int arg1) __asm__("FUN_001f4600");

int update_blarg_heavy_interceptor_aim_transforms(struct Moby *moby, int flag) {
    int ok = 0;
    char *data = (char *)moby->pvars;
    float bounding_sphere[4];
    float half_span_vector[4];
    float rotation_angle;
    subtract_vector_xyz(half_span_vector, data + 0x170, data + 0xD0);
    scale_vector_xyz(half_span_vector, half_span_vector, 0.5f);
    add_vector_xyz(bounding_sphere, data + 0xD0, half_span_vector);
    bounding_sphere[3] = vector_length_xyz(half_span_vector) + 2.0f;
    if (FUN_001fa728_c((char *)bounding_sphere, convert_integer_to_float(moby->draw_dist)) >= 0) {
        FUN_0020cca8_c(moby, 1, data + 0xA0);
        ok = 1;
        normalize_vector_xyz(data + 0xA0, data + 0xA0, 1.0f);
        normalize_vector_xyz(data + 0xB0, data + 0xB0, 1.0f);
        normalize_vector_xyz(data + 0xC0, data + 0xC0, 1.0f);
        rotation_angle = fast_subtract_rotations(fast_arc_tan(half_span_vector[0], half_span_vector[1]), moby->rot.z);
        FUN_L00_001fff28(data + 0xF0, 2, rotation_angle);
        *(float *)(data + 0x1BC) = rotation_angle;
        rotation_angle = -fast_arc_tan(vector_length_xy(half_span_vector), half_span_vector[2]);
        FUN_L00_001fff28(data + 0x130, 1, rotation_angle);
        *(float *)(data + 0x1C0) = rotation_angle;
        qcopy(data + 0x90, data + 0x170);
        if (flag != 0) {
            enqueue_callback_list_1(FUN_L14_002ae260_u, moby);
            *(int *)(data + 0x20C) |= 2;
        }
    }
    return ok;
}
extern s32 truncate_float_to_s32(f32 f) __asm__("FUN_001fa6d0");
extern s32 D_L14_001D8210[];

/* 0x002AED50, 212 bytes.  Advances the blink phase and fades each light
 * entry's alpha up and down.  Its counter and its 1.0f constant are small
 * data ($gp-relative), declared with __attribute__((sda)).
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002AFF90), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002aed50(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    s32 step;
    s32 i;
    f32 advanced_blink_phase = *(f32 *)(data + 0x164) + 0.5f;
    *(f32 *)(data + 0x164) = advanced_blink_phase;
    step = truncate_float_to_s32(advanced_blink_phase);
    if (1.0f <= *(f32 *)(data + 0x164))
        *(s32 *)(data + 0x164) = 0;
    for (i = 0; i < D_L14_00161418; i++) {
        s32 light_fade_value = D_L14_001D8210[i];
        if (light_fade_value != 0) {
            light_fade_value += step;
            if (light_fade_value > 0x90)
                light_fade_value -= 0x80;
            D_L14_001D8210[i] = light_fade_value;
            if (light_fade_value > 0x50)
                light_fade_value = 0xA0 - light_fade_value;
            D_L14_001D80C8[i] = (D_L14_001D80C8[i] & 0xFFFFFF) | (light_fade_value << 24);
        }
    }
}
/* Resets the id table, then fills it with the ids of this moby's parts and starts each part by its flags. */
extern int D_L14_001D8680[];
/* Same table under a second name, so its address is loaded again. */
extern int D_L14_001D8680_b[] __asm__("D_L14_001D8680");
char *select_farthest_unselected_group_moby(int index, int *ids) __asm__("FUN_L14_002aef28");

char *select_farthest_unselected_group_moby(int index, int *ids) __asm__("FUN_L14_002aef28");
extern void draw_blarg_heavy_interceptor_spinning_beam(char *) __asm__("FUN_L14_002add48");
extern void draw_blarg_heavy_interceptor_light_ring(char *) __asm__("FUN_L14_002ada18");
void draw_blarg_heavy_interceptor_group_effects(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int i;
    int *out;
    {
        int unused_id_sentinel = -1;
        for (i = 19; i >= 0; i--)
            D_L14_001D8680[i] = unused_id_sentinel;
    }
    i = 0;
    if (*(short *)(moby_vars + 0x20A) > 0) {
        int *ids = D_L14_001D8680_b;
        out = ids;
        do {
            char *selected_moby = select_farthest_unselected_group_moby(moby->group, ids);
            char *md;
            *out++ = *(short *)(selected_moby + 0xB2);
            md = *(char **)(selected_moby + 0x78);
            if (*(int *)(md + 0x20C) & 8)
                draw_blarg_heavy_interceptor_spinning_beam(selected_moby);
            if (*(int *)(md + 0x20C) & 2)
                draw_blarg_heavy_interceptor_light_ring(selected_moby);
            i++;
        } while (i < *(short *)(moby_vars + 0x20A));
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Returns the farthest moby in a list that is not in the excluded-id table, by distance from a point. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B0168), where it is exact; names translated to the US level program. */

extern char D_L14_001674C0[];
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern unsigned char *D_L14_0015FFD0_e[] __asm__("D_L14_0015FFD0")
    __attribute__((section(".sdata")));
extern int D_L14_001D8680[];
extern short *D_L14_001AC2C0_2B0168[] __asm__("D_L14_001ABF40");

char *select_farthest_unselected_group_moby(int index, int *ids) {
    short *list_entry = D_L14_001AC2C0_2B0168[index];
    char *best = 0;
    float bestd = 0.0f;
    if (list_entry == 0)
        return 0;
    do {
        char *moby = (char *)D_L14_0015FFD0_e[2] + ((*(unsigned short *)list_entry & 0x7FFF) << 8);
        int found = 0;
        int i;
        for (i = 0; i < 20; i++) {
            if (*(short *)(moby + 0xB2) == D_L14_001D8680[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            float candidate_distance = vector_distance(D_L14_001674C0, moby + 0x10);
            if (bestd < candidate_distance) {
                bestd = candidate_distance;
                best = moby;
            }
        }
    } while (*list_entry++ >= 0);
    return best;
}
#include "sda.h"

/* counts table entries through the first negative terminator */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B0288), where it is exact; names translated to the US level program. */


int count_moby_group_members(struct Moby *moby) {
    short *table = D_L14_001AC2C0_2B0168[moby->group];
    int count = 0;
    if (table == 0)
        return 0;
    do {
        count++;
    } while (*table++ >= 0);
    return count;
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B4E40). */

typedef struct Moby Moby;

char *spawn_flying_bolt(void *a, float *b, int c, int d) __asm__("FUN_L14_002eccf0");
extern f32 fast_add_rotations_c3(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern int D_L14_0015F5CC __attribute__((section(".sdata"))); /* no foreign declaration */
extern int D_L14_001600EC; /* no foreign declaration */
extern float D_L14_00161508 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_0016150C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161510 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161514 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_0016151C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161520 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161524 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161528 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_0016152C __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00161530 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00161534 __attribute__((sda)); /* no foreign declaration */
extern int debug_print_alt() __asm__("FUN_001e93b0");
extern int FUN_001f9770_c2(void *) __asm__("FUN_001f9770");
extern int set_moby_color() __asm__("FUN_L00_002502f0");
extern int is_point_inside_clip_volume_c(void *arg0, int arg1) __asm__("func_00214720");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 random_integer_below_c2(s32) __asm__("func_00213260");
extern void FUN_L00_0025e450_c2(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c3(void *, void *, float) __asm__("FUN_001f9a68");
extern void set_moby_animation_c2(void *, s32, s32) __asm__("func_00212ED8");
int FUN_L14_00268038(char *a, char *o, float best);
void FUN_L14_002b4340_c(struct Moby *m) __asm__("FUN_L14_002b4340");
extern int func_001E9730_2B4E40() __asm__("FUN_001e93b0");
extern char D_L14_001FBD50[];
extern char D_L14_001FBD78[];
extern void func_L14_002B5590_2B4E40(unsigned char *m) __asm__("FUN_L14_002b4340");
extern void func_0020D678_2B4E40(void *) __asm__("FUN_0020c828");
extern int func_001F9908_2B4E40(void *) __asm__("FUN_001f9740");
extern void func_00213D28_2B4E40(void *, int, int) __asm__("FUN_00212ed8");
extern int func_00215570_2B4E40(void *, int) __asm__("FUN_00214720");
extern float func_001F9FA8_2B4E40(float) __asm__("FUN_001f9de0");
extern int func_L00_00260D30_2B4E40(void *, void *, float) __asm__("FUN_L14_00268038");
extern char *spawn_flying_bolt(void *, float *, int, int) __asm__("FUN_L14_002eccf0");
extern void func_L00_00251328_2B4E40(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void func_L00_0025F4A8_2B4E40(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int func_001F9850_2B4E40(int) __asm__("FUN_001f96f8");
extern float func_001FA888_2B4E40(int) __asm__("FUN_001fa6c0");
extern int func_001F9938_2B4E40(void *) __asm__("FUN_001f9770");
extern float func_001FA748_2B4E40(float, float) __asm__("FUN_001fa580");
extern void func_001F9C30_2B4E40(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9BD8_2B4E40(void *, void *, void *) __asm__("FUN_001f9a10");
extern float func_L00_001FF860_2B4E40(float, float) __asm__("FUN_001f9e90");
extern int func_002140B0_2B4E40(int) __asm__("FUN_00213260");
extern void func_L00_001FF4B0_2B4E40(void *, void *, float) __asm__("FUN_001f9bf8");

void machinegun_turret_update(char *moby) __asm__("FUN_L14_002b3bf0");
void machinegun_turret_update(char *moby) {
    enum {
        TURRET_STATE_INITIALIZE = 0,
        TURRET_STATE_WAIT_FOR_ACTIVATION = 1,
        TURRET_STATE_DEPLOY = 2,
        TURRET_STATE_WAIT_FOR_TARGET = 3,
        TURRET_STATE_AIM_AND_FIRE = 4,
        TURRET_STATE_RETRACT = 5,
        TURRET_STATE_DESTROY = 6
    };
    enum {
        TURRET_SPAWN_CENTER = 0,
        TURRET_SPAWN_UNNEGATED_OFFSET = 1
    };
    char *data;
    float target_point_scratch[4];
    float spawn_position[4];
    float spawn_vector_scratch[4];
    float spawn_position_offset[4];
    float v60[4];
    float target_query_result[4];
    float m80[16];
    float signed_phase_increment;
    float phase_angle, sine_input_angle, sine_scale, target_heading;
    int r, rem, sel, target_missing_or_outside_waking_cuboid, animation_sequence, aiming_cuboid_index;
    char *aiming_offset_vector;
    char *aiming_base_point;

    data = *(char **)(moby + 0x78);
    func_L14_002B5590_2B4E40((unsigned char *)moby);
    switch (*(unsigned char *)(moby + 0x20)) {
    case TURRET_STATE_INITIALIZE:
        *(float *)(data + 0x70) = *(float *)(moby + 0x18);
        *(float *)(data + 0x84) = *(float *)(moby + 0x48);
        *(int *)(data + 0x8C) = 0;
        *(short *)(data + 0x74) = 0;
        if (*(int *)(data + 0x80) == -1) {
            func_001E9730_2B4E40(D_L14_001FBD50, *(short *)(moby + 0xB2));
            func_0020D678_2B4E40(moby);
            return;
        }
        if (*(int *)(data + 0x90) == -1) {
            func_001E9730_2B4E40(D_L14_001FBD78, *(short *)(moby + 0xB2));
            func_0020D678_2B4E40(moby);
            return;
        }
        r = func_001F9850_2B4E40(D_L14_00161534);
        *(short *)(data + 0x76) = r;
        *(unsigned char *)(data + 0x8B) = 0;
        if (*(short *)(data + 0x88) != 0) {
            *(unsigned char *)(moby + 0x20) = 3;
            *(unsigned char *)(data + 0x8A) = 1;
        } else {
            *(unsigned char *)(moby + 0x20) = 1;
            *(float *)(moby + 0x18) = *(float *)(moby + 0x18) - D_L14_00161508;
            *(int *)(moby + 0x94) = 0;
            *(unsigned char *)(data + 0x8A) = 0;
            *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) & 0xEFFF;
        }
        *(int *)(data + 0x98) = func_001F9850_2B4E40(*(int *)(data + 0x94));
        return;
    case TURRET_STATE_WAIT_FOR_ACTIVATION:
        if (*(short *)(data + 0x88) == 0) {
            return;
        }
        if (func_001F9908_2B4E40(data + 0x98) == 0) {
            return;
        }
        *(unsigned char *)(moby + 0x20) = 2;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(D_L14_0016150C);
        *(float *)(data + 0x78) = 1.0f / func_001FA888_2B4E40(*(short *)(data + 0x74));
        func_00213D28_2B4E40(moby, 1, 0);
        *(float *)(moby + 0x58) = 1.0f / (func_001FA888_2B4E40(*(short *)(data + 0x74)) * D_L14_00161510);
        return;
    case TURRET_STATE_DEPLOY:
        {
            float t = (float)*(short *)(data + 0x74) * *(float *)(data + 0x78);
            *(float *)(moby + 0x18) = *(float *)(data + 0x70) - (0.0f + (D_L14_00161508 - 0.0f) * t);
        }
        if (*(unsigned char *)(moby + 0x70) & 2) {
            if (*(unsigned char *)(moby + 0x52) == 1) {
                func_00213D28_2B4E40(moby, 2, 0);
                *(float *)(moby + 0x58) = 1.0f;
            }
        }
        if (*(short *)(data + 0x74) < func_001F9850_2B4E40(D_L14_0016150C >> 1)) {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
            *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) | 0x1000;
        }
        if (func_001F9938_2B4E40(data + 0x74) == 0) {
            return;
        }
        *(short *)(data + 0x76) = func_001F9850_2B4E40(D_L14_00161534);
        *(unsigned char *)(moby + 0x20) = 3;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(60);
        *(float *)(moby + 0x18) = *(float *)(data + 0x70);
        *(int *)(data + 0x8C) = 0;
        return;
    case TURRET_STATE_WAIT_FOR_TARGET:
        r = func_L00_00260D30_2B4E40(moby, target_point_scratch, 34.0f);
        if (r != 2) {
            if (func_00215570_2B4E40(target_point_scratch, *(int *)(data + 0x90)) != 0) {
                *(unsigned char *)(moby + 0x20) = 4;
                func_00213D28_2B4E40(moby, 3, 0);
                *(short *)(data + 0x74) = func_001F9850_2B4E40(120);
                *(short *)(data + 0x9C) = func_001F9850_2B4E40(20);
                return;
            }
        }
        if (*(unsigned char *)(data + 0x8A) != 0) {
            return;
        }
        if (func_001F9938_2B4E40(data + 0x76) == 0) {
            return;
        }
        *(unsigned char *)(data + 0x8B) = 1;
        return;
    case TURRET_STATE_AIM_AND_FIRE:
        if (*(short *)(data + 0x9E) != 0) {
            signed_phase_increment = -D_L14_00161514;
        } else {
            signed_phase_increment = D_L14_00161514;
        }
        phase_angle = func_001FA748_2B4E40(*(float *)(data + 0x8C), signed_phase_increment);
        *(float *)(data + 0x8C) = phase_angle;
        sine_input_angle = func_001FA748_2B4E40(phase_angle, *(float *)(data + 0x84) * 0.01745329238474369f);
        aiming_cuboid_index = *(int *)(data + 0x80);
        aiming_offset_vector = D_L14_001600EC + (aiming_cuboid_index << 7) + 0x10;
        sine_scale = func_001F9FA8_2B4E40(sine_input_angle);
        func_001F9C30_2B4E40(target_point_scratch, aiming_offset_vector, sine_scale);
        aiming_cuboid_index = *(int *)(data + 0x80);
        aiming_base_point = D_L14_001600EC + (aiming_cuboid_index << 7) + 0x30;
        func_001F9BD8_2B4E40(target_point_scratch, target_point_scratch, aiming_base_point);
        target_heading = func_L00_001FF860_2B4E40(target_point_scratch[0] - *(float *)(moby + 0x10), target_point_scratch[1] - *(float *)(moby + 0x14));
        *(float *)(moby + 0x48) = target_heading;
        func_001F9938_2B4E40(data + 0x9C);
        rem = D_L14_0015F5CC % D_L14_00161530;
        if (rem == 0) {
            sel = func_002140B0_2B4E40(3);
            switch (sel) {
            case TURRET_SPAWN_CENTER:
                func_L00_001FF4B0_2B4E40(spawn_position, moby + 0xE0, D_L14_00161520);
                break;
            case TURRET_SPAWN_UNNEGATED_OFFSET:
                func_L00_001FF4B0_2B4E40(spawn_position, moby + 0xE0, D_L14_00161524);
                func_L00_001FF4B0_2B4E40(spawn_position_offset, moby + 0xD0, D_L14_0016152C);
                func_001F9BD8_2B4E40(spawn_position, spawn_position, spawn_position_offset);
                break;
            default:
                func_L00_001FF4B0_2B4E40(spawn_position, moby + 0xE0, D_L14_00161524);
                func_L00_001FF4B0_2B4E40(spawn_position_offset, moby + 0xD0, -D_L14_0016152C);
                func_001F9BD8_2B4E40(spawn_position, spawn_position, spawn_position_offset);
                break;
            }
            func_L00_001FF4B0_2B4E40(spawn_vector_scratch, moby + 0xC0, D_L14_00161528);
            func_001F9BD8_2B4E40(spawn_position, spawn_position, spawn_vector_scratch);
            func_001F9BD8_2B4E40(spawn_position, spawn_position, moby + 0x10);
            func_L00_001FF4B0_2B4E40(spawn_vector_scratch, moby + 0xC0, D_L14_0016151C * frame_time);
            if (*(short *)(data + 0x9C) == 0) {
                func_L00_00251328_2B4E40(spawn_flying_bolt(spawn_position, spawn_vector_scratch, (int)moby, 1), 0xE0, 0xE0, 0xE0);
            }
        }
        r = func_L00_00260D30_2B4E40(moby, target_query_result, 34.0f);
        target_missing_or_outside_waking_cuboid = (r == 2 || func_00215570_2B4E40(target_query_result, *(int *)(data + 0x90)) == 0);
        if (target_missing_or_outside_waking_cuboid == 0) {
            return;
        }
        if (func_001F9938_2B4E40(data + 0x74) == 0) {
            return;
        }
        if ((*(unsigned char *)(moby + 0x70) & 2) == 0) {
            return;
        }
        animation_sequence = *(unsigned char *)(moby + 0x52);
        if (animation_sequence != 3) {
            return;
        }
        *(short *)(data + 0x76) = func_001F9850_2B4E40(D_L14_00161534);
        *(unsigned char *)(moby + 0x20) = animation_sequence;
        func_00213D28_2B4E40(moby, 2, 0);
        *(float *)(moby + 0x58) = 1.0f;
        return;
    case TURRET_STATE_RETRACT:
        {
            float t = (float)*(short *)(data + 0x74) * *(float *)(data + 0x78);
            *(float *)(moby + 0x18) = *(float *)(data + 0x70) - (D_L14_00161508 + (0.0f - D_L14_00161508) * t);
            *(float *)(moby + 0x48) = *(float *)(data + 0x84) + (*(float *)(data + 0x8C) - *(float *)(data + 0x84)) * t;
        }
        if (*(unsigned char *)(moby + 0x70) & 2) {
            if (*(unsigned char *)(moby + 0x52) == 4) {
                func_00213D28_2B4E40(moby, 0, 0);
                *(float *)(moby + 0x58) = 1.0f;
            }
        }
        if (*(short *)(data + 0x74) < func_001F9850_2B4E40(D_L14_0016150C >> 1)) {
            *(int *)(moby + 0x94) = 0;
        }
        if (func_001F9938_2B4E40(data + 0x74) == 0) {
            return;
        }
        *(float *)(moby + 0x48) = *(float *)(data + 0x84);
        *(unsigned char *)(moby + 0x20) = 1;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(60);
        *(short *)(data + 0x88) = 0;
        *(int *)(data + 0x98) = func_001F9850_2B4E40(*(int *)(data + 0x94));
        *(float *)(moby + 0x18) = *(float *)(data + 0x70) - D_L14_00161508;
        *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) & 0xEFFF;
        return;
    case TURRET_STATE_DESTROY:
        func_L00_0025F4A8_2B4E40(moby, data + 0x40, moby + 0x10, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 0, 1.0f, 15.0f, 1, 1, -1, 0);
        func_0020D678_2B4E40(moby);
        return;
    default:
        return;
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B5590), where it is exact; names translated to the US level program. */

void FUN_L14_002b4340(struct Moby *moby) {
    enum {
        HIT_REACTION_NO_ACTION = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DESTROY = 2,
        HIT_REACTION_PLAY_HIT_ANIMATION = 3,
        HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION = 4,
        HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION = 5,
        HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH = 6,
        HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION = 7,
        HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH = 8,
        HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH = 9,
        HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION = 10,
        HIT_REACTION_NONE = 11
    };
    unsigned char *moby_vars = moby->pvars;
    int loc0;
    float damage_amount = 0.0f;
    char *hit_record;
    int k;
    float remaining_health;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    if (hit_record != 0) {
        if (*(int *)(hit_record + 0x20) != 0) {
            int hit_object_class = *(short *)(*(int *)(hit_record + 0x20) + 0xA6);
            if (hit_object_class == moby->oclass || hit_object_class == 0x2A9)
                hit_record = 0;
        }
    }
    k = hit_reaction(moby, hit_record, moby_vars + 0x20, 0, &loc0, &damage_amount, 0, 4);
    if (loc0 != 1 && moby->state != 6) {
        remaining_health = *(float *)(moby_vars + 0x20) - damage_amount;
        *(float *)(moby_vars + 0x20) = remaining_health;
        if (remaining_health <= 0.0f)
            k = 1;
        switch (k) {
        case HIT_REACTION_NO_ACTION:
        case HIT_REACTION_NONE:
            break;
        case HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH:
        case HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION:
            moby_vars[0x67] = 100;
            break;
        case HIT_REACTION_PLAY_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH:
            moby_vars[0x67] = 200;
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_DESTROY:
            moby->state = 6;
            moby->flags &= 0xEFFF;
            moby_vars[0x67] = 0xF0;
            FUN_L00_00257470(moby, 0, -1);
            break;
        }
        trooper_anim_update(moby, (short *)(moby_vars + 0x60));
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, moby_vars + 0x60);
}
/* Returns 0 unless every object on the list is ready, then wakes the waiting ones and sets their timers. */
extern short *D_L14_001ABF40_b45[] __asm__("D_L14_001ABF40");
extern unsigned char *D_L14_0015FFD8_b45 __asm__("D_L14_0015FFD8");
extern unsigned char *D_L14_0015FFD0_m45[] __asm__("D_L14_0015FFD0") __attribute__((section(".sdata")));
extern int D_L14_0016150C_b45 __asm__("D_L14_0016150C") __attribute__((sda));
extern float D_L14_00161510_b45 __asm__("D_L14_00161510") __attribute__((sda));
extern void FUN_00212ed8_b45(unsigned char *, int, int) __asm__("FUN_00212ed8");
extern int FUN_001f96f8_b45(int) __asm__("FUN_001f96f8");
extern float FUN_001fa6c0_b45(short) __asm__("FUN_001fa6c0");

int try_retract_turret_group(int index) {
    short *list_entry = D_L14_001ABF40_b45[index];
    unsigned char *base;
    unsigned char *checked_moby;
    unsigned char *retracting_moby;
    unsigned char *turret_vars;
    int moby_index_or_state;
    int moby_index_or_retract_frames;
    float inverse_retract_frames;
    float retract_animation_speed;
    if (list_entry == 0) {
        return 0;
    }
    base = D_L14_0015FFD8_b45;
    do {
        moby_index_or_state = *(unsigned short *)list_entry & 0x7FFF;
        checked_moby = base + (moby_index_or_state << 8);
        if ((*(unsigned char **)(checked_moby + 0x78))[0x8B] == 0) {
            moby_index_or_state = checked_moby[0x20];
            if (moby_index_or_state != 5 && moby_index_or_state != 1) {
                return 0;
            }
        }
    } while (*list_entry++ >= 0);
    list_entry = D_L14_001ABF40_b45[index];
    do {
        moby_index_or_retract_frames = *(unsigned short *)list_entry & 0x7FFF;
        retracting_moby = D_L14_0015FFD0_m45[2] + (moby_index_or_retract_frames << 8);
        if (retracting_moby[0x20] == 3) {
            turret_vars = *(unsigned char **)(retracting_moby + 0x78);
            FUN_00212ed8_b45(retracting_moby, 4, 0);
            retracting_moby[0x20] = 5;
            moby_index_or_retract_frames = FUN_001f96f8_b45(D_L14_0016150C_b45);
            *(short *)(turret_vars + 0x74) = moby_index_or_retract_frames;
            inverse_retract_frames = 1.0f / FUN_001fa6c0_b45(moby_index_or_retract_frames);
            retract_animation_speed = 1.0f / (*(short *)(turret_vars + 0x74) * D_L14_00161510_b45);
            *(float *)(turret_vars + 0x78) = inverse_retract_frames;
            *(float *)(retracting_moby + 0x58) = retract_animation_speed;
            *(float *)(turret_vars + 0x8C) = *(float *)(retracting_moby + 0x48);
            *(short *)(turret_vars + 0x88) = 0;
            turret_vars[0x8B] = 0;
        }
    } while (*list_entry++ >= 0);
    return 1;
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B58C0), where it is exact; names translated to the US level program. */

extern char *D_L14_ffd0_e[] __asm__("D_L14_0015FFD0") __attribute__((section(".sdata")));
extern short *D_L14_001AC2C0_2B0168[] __asm__("D_L14_001ABF40");

int is_turret_group_activated_or_empty(int index) {
    unsigned short *list = (unsigned short *)D_L14_001AC2C0_2B0168[index];
    int count = 0;
    char *base;
    if (list)
        goto scan;
success:
    return 1;
scan:
    base = D_L14_ffd0_e[2];
    do {
        unsigned short id = *list;
        char *moby = base + ((id & 0x7FFF) << 8);
        int state = *(signed char *)(moby + 0x20);
        char *data = *(char **)(moby + 0x78);
        if (state >= 0)
            count++;
        if (state >= 0 && *(short *)(data + 0x88))
            goto success;
        list++;
        if ((short)id < 0)
            return count == 0;
    } while (1);
}
/* Update of the minibomber (moby class 31): flies along its paths, turns at the ends, dives and drops bombs. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B5938), where it is exact; names translated to the US level program. */


typedef union PathNode_2B5938 {
    float v[4];
    int n;
} PathNode_2B5938;

typedef struct Class_2B5938 {
    char pad00[0x10];
    int f10;
} Class_2B5938;

typedef struct Moby_2B5938 {
    char pad00[0x10];
    float pos[4];               /* 0x10 */
    unsigned char state;        /* 0x20 */
    char pad21[3];
    Class_2B5938 *cls;          /* 0x24 */
    char pad28[8];
    unsigned char f30;          /* 0x30 */
    unsigned char f31;          /* 0x31 */
    char pad32[2];
    unsigned short f34;         /* 0x34 */
    char pad36[0x12];
    float rot;                  /* 0x48 */
    char pad4C[6];
    unsigned char f52;          /* 0x52 */
    unsigned char f53;          /* 0x53 */
    char pad54[0x1C];
    unsigned char f70;          /* 0x70 */
    char pad71[7];
    struct Data_2B5938 *d;      /* 0x78 */
    char pad7C[3];
    unsigned char f7F;          /* 0x7F */
    char pad80[0x14];
    int f94;                    /* 0x94 */
    char pad98[0x1A];
    short fB2;                  /* 0xB2 */
} Moby_2B5938;

typedef struct Data_2B5938 {
    char pad00[0x40];
    float f40[2];               /* 0x40 */
    float f48;                  /* 0x48 */
    char pad4C[0x64];
    Moby_2B5938 *fB0;           /* 0xB0 */
    int fB4;                    /* 0xB4 */
    char padB8[0x18];
    int fD0;                    /* 0xD0 */
    int fD4;                    /* 0xD4 */
    int fD8;                    /* 0xD8 */
    int fDC;                    /* 0xDC */
    char padE0[4];
    float fE4;                  /* 0xE4 */
    int fE8;                    /* 0xE8 */
    float fEC;                  /* 0xEC */
    float fF0[4];               /* 0xF0 */
    short f100;                 /* 0x100 */
    short f102;                 /* 0x102 */
    float f104;                 /* 0x104 */
    int f108;                   /* 0x108 */
    int f10C;                   /* 0x10C */
    char f110[0x10];            /* 0x110 */
    char f120[0x30];            /* 0x120 */
    int f150;                   /* 0x150 */
    float f154;                 /* 0x154 */
    float f158;                 /* 0x158 */
    float f15C;                 /* 0x15C */
    float f160;                 /* 0x160 */
    float f164;                 /* 0x164 */
    float f168;                 /* 0x168 */
    float f16C;                 /* 0x16C */
    float f170;                 /* 0x170 */
    float f174;                 /* 0x174 */
    float f178;                 /* 0x178 */
    float f17C;                 /* 0x17C */
    float f180[4];              /* 0x180 */
} Data_2B5938;

char *FUN_L14_003014e0(int a0, float *pos, float *a2, int a3, float f);
extern char D_0013E550_c[] __asm__("D_0013E550");
char D_L14_001FBDF0[] = "Hovertank with no introPath. UID: %d\n";
char D_L14_001FBE18[] = "Hovertank with a intro path that has no points. UID: %d\n";
char D_L14_001FBE58[] = "Hovertank with no boundPath. UID: %d\n";
char D_L14_001FBE80[] = "Hovertank with a bound path that has no points. UID: %d\n";
char D_L14_001FBEC0[] = "Hovertank with no bombPath. UID: %d\n";
char D_L14_001FBEE8[] = "Hovertank with a bomb path that has no points. UID: %d\n";
char D_L14_001FBF20[] = "Hovertank with no exit Path. UID: %d\n";
extern char D_L14_001FBF48[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern float D_0015ED60;
extern float D_L14_00161568 __attribute__((sda));
extern float blend_scalar_alt(float, float, float) __asm__("FUN_002133d0");
extern float D_L14_0016154C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161550 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161554 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_0016155C __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00161560 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00161564 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_0016156C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161570 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161574 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161578 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_0016157C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161580 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161584 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161588 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_0016158C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161590 __attribute__((sda)); /* no foreign declaration */
extern int debug_print_alt() __asm__("FUN_001e93b0");
extern int FUN_001f9770_c(void *) __asm__("FUN_001f9770");
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern int scale_game_frames_c(int) __asm__("FUN_001f96f8");
extern float D_L14_00161540 __attribute__((sda));
extern unsigned char *FUN_L14_002bb310_c(char *owner, float angle, char *posp) __asm__("FUN_L14_002bb310");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L14_002b5290_c(unsigned char *m) __asm__("FUN_L14_002b5290");
extern void FUN_L14_002b5428_c(char *moby) __asm__("FUN_L14_002b5428");
extern void FUN_L14_002b5530_c(char *moby) __asm__("FUN_L14_002b5530");
extern void FUN_L14_002b55b0_c(char *moby) __asm__("FUN_L14_002b55b0");
extern void ease_minibomber_position_and_heading(char *moby, float *target, float a, float b, float c, float d, float e, float f, float g) __asm__("FUN_L14_002b5850");
extern void FUN_L14_002b5968_u(char *m, float *input, float angle) __asm__("FUN_L14_002b5968");
extern void FUN_L14_002b5c30_c(char *moby) __asm__("FUN_L14_002b5c30");
extern void FUN_L14_002b5c98_c(char *moby) __asm__("FUN_L14_002b5c98");
extern void FUN_L14_002b5d18_c(char *moby) __asm__("FUN_L14_002b5d18");
extern void FUN_L14_002b5dd8_c(char *m) __asm__("FUN_L14_002b5dd8");
extern void fill_transfer_words(void *, s32, s32) __asm__("func_001F97E8");
extern void attach_manipulator_c(int, int, void *) __asm__("FUN_0020cb10");
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void release_voice_slot_c(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void set_moby_animation_c(void *, s32, s32) __asm__("func_00212ED8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
int FUN_L14_002b5730_c(struct Moby *moby, char *a1, int a2, int a3, float *v, float *vel, float f12, float f13, float f14, float f15, float f16, float f17, float f18) __asm__("FUN_L14_002b5730");
s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
extern PathNode_2B5938 *D_L14_001B0F30_2B5938[] __asm__("D_L14_001B0BB0");
extern char *func_L14_00302968_2B5938(int a0, float *a1, float *a2, float f, int a3) __asm__("FUN_L14_003014e0");
extern void func_L00_0025F4A8_2B5938(void *, void *, void *, float, float, int, int, int, float, float, float, float,
                                     int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void minibomber_update(Moby_2B5938 *moby) __asm__("FUN_L14_002b46e8");
void minibomber_update(Moby_2B5938 *moby) {
    enum {
        MINIBOMBER_STATE_INITIALIZE = 0,
        MINIBOMBER_STATE_WAIT_TO_FOLLOW_PATH = 1,
        MINIBOMBER_STATE_FOLLOW_INTRO_OR_EXIT_PATH = 2,
        MINIBOMBER_STATE_RETRACE_INTRO_OR_EXIT_PATH = 3,
        MINIBOMBER_STATE_HOLD_AT_PATH_END = 4,
        MINIBOMBER_STATE_AIM_AT_BOMB_POINT = 5,
        MINIBOMBER_STATE_RELEASE_BOMB = 6,
        MINIBOMBER_STATE_TRAVERSE_PATH = 7,
        MINIBOMBER_STATE_DESTROY = 9
    };
    Data_2B5938 *d = moby->d;
    float v[12];
    float *path_endpoint_target_position;
    char *manipulator_state;
    float tg;
    float blended_aim_angle;
    float n;
    float bomb_gravity;
    float spd;

    if (moby->f70 & 2) {
        if (moby->f52 == moby->f53 && moby->f52 != 0) {
            if (moby->f52 == 2) {
                blend_moby_animation_c(moby, 0, 0, 10);
            } else {
                set_moby_animation_c(moby, 0, 0);
            }
        }
    }
    path_endpoint_target_position = d->f180;
    FUN_L14_002b5290_c((unsigned char *)moby);
    FUN_L14_002b5530_c((char *)moby);
    ease_minibomber_position_and_heading((char *)moby, path_endpoint_target_position, d->f17C, d->f170, d->f174, d->f178, d->f154, d->f158, d->f15C);
    FUN_L14_002b5428_c((char *)moby);
    FUN_L14_002b5dd8_c((char *)moby);
    if (moby->f31 != 0) {
        if (distance_xyz(moby->pos, D_L14_001674C0) < 40.0f && moby->state != 0 && moby->state != 1 &&
            moby->state != 9) {
            FUN_L00_0025a120(moby);
        }
    }
    switch (moby->state) {
    case MINIBOMBER_STATE_INITIALIZE: {
        PathNode_2B5938 *path;
        if (d->fD0 == -1) {
            debug_print_alt(D_L14_001FBDF0, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        path = D_L14_001B0F30_2B5938[d->fD0];
        if (path->n == 0) {
            debug_print_alt(D_L14_001FBE18, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->fD4 == -1) {
            debug_print_alt(D_L14_001FBE58, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (D_L14_001B0F30_2B5938[d->fD4]->n == 0) {
            debug_print_alt(D_L14_001FBE80, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->fD8 == -1) {
            debug_print_alt(D_L14_001FBEC0, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (D_L14_001B0F30_2B5938[d->fD8]->n == 0) {
            debug_print_alt(D_L14_001FBEE8, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->f150 == -1) {
            debug_print_alt(D_L14_001FBF20, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (D_L14_001B0F30_2B5938[d->f150]->n == 0) {
            debug_print_alt(D_L14_001FBF48, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        qcopy(moby->pos, path + 1);
        subtract_vector_xyz_c(v, path + 2, moby->pos);
        manipulator_state = d->f110;
        moby->rot = fast_arc_tan(v[0], v[1]);
        d->f160 = *(float *)&D_L14_00161540;
        d->f164 = 0.0f;
        d->f168 = 0.0f;
        d->f16C = 0.0f;
        FUN_L14_002b55b0_c((char *)moby);
        d->f108 = -1;
        moby->f30 = 0x80;
        moby->state = 1;
        FUN_L14_002b5c98_c((char *)moby);
        fill_transfer_words(manipulator_state, 0, 0x40);
        attach_manipulator_c(moby, 0, manipulator_state);
        moby->f7F = 0x20;
        break;
    }
    case MINIBOMBER_STATE_WAIT_TO_FOLLOW_PATH:
        if (d->fB4 != 2) {
            moby->state = 2;
            moby->f34 &= 0xFFBE;
            moby->f94 = moby->cls->f10;
            FUN_L14_002b5d18_c((char *)moby);
        }
        break;
    case MINIBOMBER_STATE_FOLLOW_INTRO_OR_EXIT_PATH: {
        PathNode_2B5938 *path;
        if (d->f10C == 0) {
            path = D_L14_001B0F30_2B5938[d->fD0];
        } else {
            path = D_L14_001B0F30_2B5938[d->f150];
        }
        if (d->fB4 == 2) {
            moby->state = 3;
        } else if (FUN_L14_002b5730_c((char *)moby, (char *)path, (int)&d->fE8, (int)&d->fEC, d->fF0, &d->fE4,
                                     D_L14_0016154C * D_0015ED60, D_L14_00161574, D_L14_00161578, D_L14_0016157C,
                                     D_L14_00161580, D_L14_00161584, D_L14_00161588) != 0) {
            qcopy(d->f180, path + path->n);
            path = D_L14_001B0F30_2B5938[d->fDC];
            if (d->f10C == 0) {
                qcopy(v, path + 1);
                qcopy(v + 4, path + path->n);
            } else {
                qcopy(v, path + path->n);
                qcopy(v + 4, path + 1);
            }
            subtract_vector_xyz_c(v + 8, v + 4, v);
            d->f17C = fast_arc_tan(v[8], v[9]);
            d->f170 = D_L14_00161574;
            d->f174 = D_L14_00161578;
            d->f178 = D_L14_0016157C;
            d->f154 = D_L14_00161580;
            d->f158 = D_L14_00161584;
            d->f15C = D_L14_00161588;
            moby->state = 4;
            d->f104 = 0.0f;
            d->f102 = 0;
        }
        break;
    }
    case MINIBOMBER_STATE_RETRACE_INTRO_OR_EXIT_PATH: {
        PathNode_2B5938 *path;
        if (d->f10C == 0) {
            path = D_L14_001B0F30_2B5938[d->fD0];
        } else {
            path = D_L14_001B0F30_2B5938[d->f150];
        }
        if (d->fB4 != 2) {
            moby->state = 2;
        } else if (FUN_L14_002b5730_c((char *)moby, (char *)path, (int)&d->fE8, (int)&d->fEC, d->fF0, &d->fE4,
                                     -(D_L14_00161554 * D_0015ED60), D_L14_0016158C, D_L14_00161578, D_L14_0016157C,
                                     D_L14_00161590, D_L14_00161584, D_L14_00161588) != 0) {
            moby->state = 1;
            FUN_L14_002b5c98_c((char *)moby);
        }
        break;
    }
    case MINIBOMBER_STATE_HOLD_AT_PATH_END:
        tg = 0.0f;
        blended_aim_angle = blend_scalar_alt(tg, d->f104, (float)d->f102 / convert_integer_to_float(scale_game_frames_c(D_L14_00161560)));
        FUN_L00_001fff28(d->f120, 2, blended_aim_angle);
        if (vector_distance(moby->pos, d->fB0->pos) < D_L14_00161570) {
            if (d->f10C == 0) {
                d->fE8 = 0;
                d->fEC = 0.0f;
            } else {
                PathNode_2B5938 *q;
                d->fE8 = D_L14_001B0F30_2B5938[d->fDC]->n - 2;
                q = D_L14_001B0F30_2B5938[d->fDC] + d->fE8;
                d->fEC = q[1].v[3];
            }
            moby->state = 7;
            d->fE4 = 0.0f;
            clear_u64_value_c(d->fF0);
        } else if (d->fB4 == 2) {
            moby->state = 3;
            FUN_L14_002b5c30_c((char *)moby);
        } else if (FUN_001f9770_c(&d->f102) != 0) {
            if (moby->f53 != 2) {
                moby->state = 5;
                d->f102 = scale_game_frames_c(D_L14_0016155C);
                d->f104 = 0.0f;
                d->f100 = 0;
            }
        }
        break;
    case MINIBOMBER_STATE_TRAVERSE_PATH:
        spd = D_L14_00161550 * D_0015ED60;
        if (d->f10C == 1) {
            spd = -spd;
        }
        if (FUN_L14_002b5730_c((char *)moby, (char *)D_L14_001B0F30_2B5938[d->fDC], (int)&d->fE8, (int)&d->fEC, d->fF0,
                              &d->fE4, spd, D_L14_0016158C, D_L14_00161578, D_L14_0016157C, D_L14_00161590,
                              D_L14_00161584, D_L14_00161588) != 0) {
            PathNode_2B5938 *path;
            moby->state = 4;
            if (d->f10C == 0) {
                d->f10C = 1;
            } else {
                d->f10C = 0;
            }
            path = D_L14_001B0F30_2B5938[d->fDC];
            if (d->f10C == 0) {
                qcopy(v, path + 1);
                qcopy(v + 4, path + path->n);
            } else {
                qcopy(v, path + path->n);
                qcopy(v + 4, path + 1);
            }
            d->f170 = D_L14_0016158C;
            d->f174 = D_L14_00161578;
            d->f178 = D_L14_0016157C;
            d->f154 = D_L14_00161590;
            d->f158 = D_L14_00161584;
            d->f15C = D_L14_00161588;
            subtract_vector_xyz_c(v + 8, v + 4, v);
            d->f17C = fast_arc_tan(v[8], v[9]);
            qcopy(d->f180, v);
        }
        break;
    case MINIBOMBER_STATE_AIM_AT_BOMB_POINT: {
        PathNode_2B5938 *q = D_L14_001B0F30_2B5938[d->fD8] + d->f100;
        tg = fast_subtract_rotations(fast_arc_tan(q[1].v[0] - moby->pos[0], q[1].v[1] - moby->pos[1]), moby->rot);
        blended_aim_angle = blend_scalar_alt(tg, d->f104, (float)d->f102 / convert_integer_to_float(scale_game_frames_c(D_L14_0016155C)));
        FUN_L00_001fff28(d->f120, 2, blended_aim_angle);
        if (moby->f53 == 2) {
            moby->state = 4;
            d->f102 = scale_game_frames_c(D_L14_00161560);
        } else if (FUN_001f9770_c(&d->f102) != 0) {
            d->f104 = blended_aim_angle;
            moby->state = 6;
            set_moby_animation_c(moby, 1, 0);
        }
        break;
    }
    case MINIBOMBER_STATE_RELEASE_BOMB:
        if (is_value_within_interpolated_window(moby, 3.0f) != 0) {
            PathNode_2B5938 *q;
            n = (float)scale_game_frames_c(D_L14_00161564);
            FUN_L00_001fff28(d->f120, 2, d->f104);
            moby_point_position(moby, 1, v);
            subtract_vector_xyz_c(v + 4, D_L14_001B0F30_2B5938[d->fD8] + (d->f100 + 1), v);
            v[6] = 0.0f;
            scale_vector_xyz_c(v + 4, v + 4, 1.0f / n);
            bomb_gravity = D_L14_00161568 * frame_time_sq;
            q = D_L14_001B0F30_2B5938[d->fD8] + d->f100;
            d->f48 = -((v[2] - q[1].v[2]) + -bomb_gravity * (n * n) * 0.5f) / n;
            func_L14_00302968_2B5938((int)moby, v, v + 4, bomb_gravity, scale_game_frames_c(D_L14_0016156C));
            FUN_L14_002b5968_u((char *)moby, v, d->f104);
            d->f100++;
            if (d->f100 >= D_L14_001B0F30_2B5938[d->fD8]->n) {
                moby->state = 4;
                d->f102 = scale_game_frames_c(D_L14_00161560);
            } else {
                d->f102 = scale_game_frames_c(D_L14_0016155C);
                moby->state = 5;
            }
        }
        break;
    case MINIBOMBER_STATE_DESTROY: {
        int i;
        moby_point_position(moby, 2, v);
        FUN_L14_002bb310_c((char *)moby, moby->rot, (char *)v);
        moby_point_position(moby, 3, v);
        FUN_L14_002bb310_c((char *)moby, fast_add_rotations_c(moby->rot, 3.1415927f), (char *)v);
        func_L00_0025F4A8_2B5938(moby, d->f40, moby->pos, 0.0f, 0.0f, 5, 2, 6, 4.0f, 1.0f, 9.0f, 1.0f, 1, 15.0f, 1, 1,
                                 -1, 0);
        i = d->f108;
        if (i != -1) {
            char *voice_slot = D_0013E550_c + i * 0x70;
            if (*(Moby_2B5938 **)(voice_slot + 0x88) == moby && *(unsigned char *)(voice_slot + 0x74) != 0) {
                release_voice_slot_c(i);
            }
        }
        d->f108 = -1;
        mark_moby_for_removal(moby);
        return;
    }
    }
    if (moby->state > 1 && moby->state != 9) {
        if (voice_is_playing(moby, d->f108) == 0) {
            d->f108 = allocate_voice_for_target_entry_alt(0, 4, (int)moby);
        }
    }
}
/* Same source as the exact FUN_L14_002b4340: skips states 0 and 9, keeps the model scale, and hit reactions play animation 2. */
extern int FUN_L00_0025a478_c(void *, void *, void *, int, int *, float *, int,
                              int) __asm__("FUN_L00_0025a478");
extern void FUN_00212f90(void *, int, int, int);
extern void FUN_L00_0025d458_c(void *, void *) __asm__("FUN_L00_0025d458");

void FUN_L14_002b5290(struct Moby *hit_owner_moby) {
    enum {
        HIT_REACTION_NO_ACTION = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DESTROY = 2,
        HIT_REACTION_PLAY_HIT_ANIMATION = 3,
        HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION = 4,
        HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION = 5,
        HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH = 6,
        HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION = 7,
        HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH = 8,
        HIT_REACTION_RED_FLASH_ONLY = 9,
        HIT_REACTION_FLASH_RED = 10,
        HIT_REACTION_NONE = 11
    };
    unsigned char *moby_vars = hit_owner_moby->pvars;
    int loc0;
    float damage_amount;
    char *hit_record;
    int k;
    float remaining_health;
    if (hit_owner_moby->state == 0 || hit_owner_moby->state == 9)
        return;
    hit_owner_moby->scale = hit_owner_moby->pclass->scale;
    damage_amount = 0.0f;
    hit_record = find_moby_hit(hit_owner_moby, 0x330000, 0);
    if (hit_record != 0) {
        if (*(int *)(hit_record + 0x20) != 0) {
            int hit_object_class = *(short *)(*(int *)(hit_record + 0x20) + 0xA6);
            if (hit_object_class == hit_owner_moby->oclass || hit_object_class == 0x370)
                hit_record = 0;
        }
    }
    k = FUN_L00_0025a478_c(hit_owner_moby, hit_record, moby_vars + 0x20, 0, &loc0, &damage_amount, 0, 4);
    if (loc0 != 1 && hit_owner_moby->state != 9) {
        remaining_health = *(float *)(moby_vars + 0x20) - damage_amount;
        *(float *)(moby_vars + 0x20) = remaining_health;
        if (remaining_health <= 0.0f)
            k = 1;
        switch (k) {
        case HIT_REACTION_NO_ACTION:
        case HIT_REACTION_NONE:
            break;
        case HIT_REACTION_PLAY_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH:
            FUN_00212f90(hit_owner_moby, 2, 0, 5);
            hit_owner_moby->anim_speed = 1.75f;
        case HIT_REACTION_RED_FLASH_ONLY:
        case HIT_REACTION_FLASH_RED:
            moby_vars[0x67] = 0xFA;
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_DESTROY:
            hit_owner_moby->state = 9;
            hit_owner_moby->flags &= 0xEFFF;
            moby_vars[0x67] = 0xFA;
            FUN_L00_00257470(hit_owner_moby, 0, -1);
            break;
        }
        FUN_L00_0025d458_c(hit_owner_moby, moby_vars + 0x60);
    }
    hit_owner_moby->hit_slot = 0xFF;
    trooper_anim_finish(hit_owner_moby, moby_vars + 0x60);
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6678), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2,
                                float eps) __asm__("FUN_L00_0025b8c0");
extern f32 D_L14_00161540_d __asm__("D_L14_00161540") __attribute__((sda));
extern void FUN_L00_00262b00_c(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void FUN_L00_00262b80_c(void *, char *, char *, float, float,
                               float) __asm__("FUN_L00_00262b80");

extern short D_L14_00161538 __attribute__((sda));
extern short D_L14_0016153C __attribute__((sda));
extern short D_L14_00161544 __attribute__((sda));
extern short D_L14_00161548 __attribute__((sda));
void FUN_L14_002b5428(struct Moby *moby) {
    int raw = moby->state;
    unsigned char state = raw;
    char *data = (char *)moby->pvars;
    float spring_target;
    if (state == 0 || state == 9 || state == 1)
        return;
    spring_target = *(float *)&D_L14_00161540_d;
    if ((raw >= 2 && raw <= 3) || state == 7) {
        spring_target = spring_target * 1.5f;
    } else {
        FUN_L00_0025b8c0_c((float *)(data + 0x168), (float *)(data + 0x16C),
                           *(float *)&D_L14_0016153C, 0.002f, 0.2f, 0.0f);
        FUN_L00_00262b00_c(*(float *)&D_L14_00161538, *(float *)(data + 0x168), moby,
                           (float *)(data + 0xC0), (float *)(data + 0xC4));
    }
    FUN_L00_0025b8c0_c((float *)(data + 0x160), (float *)(data + 0x164), spring_target, 0.003f, 0.2f, 0.0f);
    FUN_L00_00262b80_c(moby, data + 0xC8, data + 0xCC, *(float *)(data + 0x160),
                       *(float *)&D_L14_00161544, *(float *)&D_L14_00161548);
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6780), where it is exact; names translated to the US level program. */

extern int *D_L14_001B0BB0[];
extern int FUN_L00_0025ff38(float a, char *m, char *b, int c, int d, int *e, int f);

void FUN_L14_002b5530(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (moby->state != 0 && moby->state != 9) {
        int *bound_path = D_L14_001B0BB0[*(int *)(data + 0xD4)];
        FUN_L00_0025ff38(255.0f, moby, data + 0x70, 0, 0, bound_path + 4, *bound_path);
        if (*(int *)(data + 0xB0) == 0) {
            *(int *)(data + 0xB0) = *(int *)(D_0013E533 + 0x2E9D);
        }
    }
}
/* Recomputes the per-segment lengths of three paths held by the moby, then clears a counter. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6800), where it is exact; names translated to the US level program. */

extern float FUN_001f9b48(void *, void *);

void FUN_L14_002b55b0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *path;
    int i;
    path = D_L14_001B0BB0[*(int *)(data + 0xD0)];
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) = FUN_001f9b48(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    path = D_L14_001B0BB0[*(int *)(data + 0x150)];
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) = FUN_001f9b48(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    path = D_L14_001B0BB0[*(int *)(data + 0xDC)];
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) = FUN_001f9b48(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    *(int *)(data + 0x10C) = 0;
}
#include "sda.h"

/* steer a moby's position toward a target and update its velocity */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6980), where it is exact; names translated to the US level program. */

extern float atan_5730(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025b8c0_5730(float *p, float *v, float t, float u1, float u2,
                                   float eps) __asm__("FUN_L00_0025b8c0");
extern void steer_5730(char *moby, float *vel, float target, float k, float d,
                       float max) __asm__("FUN_L00_00258278");

extern int FUN_L00_0025d808_5730() __asm__("FUN_L00_0025d808");
int FUN_L14_002b5730(struct Moby *moby, char *path, int segment_index_address, int segment_distance_address, float *position_easing_velocity, float *heading_easing_velocity, float f12,
                     float f13, float f14, float f15, float f16, float f17, float f18) {
    float path_target_position[3];
    int path_sample_result = FUN_L00_0025d808_5730(path, path_target_position, segment_index_address, segment_distance_address, 0);
    FUN_L00_0025b8c0_5730(&moby->pos.x, position_easing_velocity, path_target_position[0], f13, f14, f15);
    FUN_L00_0025b8c0_5730(&moby->pos.y, position_easing_velocity + 1, path_target_position[1], f13, f14, f15);
    FUN_L00_0025b8c0_5730(&moby->pos.z, position_easing_velocity + 2, path_target_position[2], f13, f14, f15);
    steer_5730(moby, heading_easing_velocity, atan_5730(position_easing_velocity[0], position_easing_velocity[1]), f16, f17, f18);
    return path_sample_result != 0;
}
/* eases a moby's position and heading toward targets while in states 4-6 or 8 */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6AA0), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025b8c0_c2(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void func_L00_002592B0_2B6AA0(char *moby, float target, float *vel, float k, float d, float max) __asm__("FUN_L00_00258278");

void ease_minibomber_position_and_heading(char *moby, float *target, float target_heading, float b, float c, float position_snap_threshold_scale, float heading_error_gain, float heading_velocity_damping, float heading_velocity_limit) {
    int data;
    int moby_state = moby[0x20] & 0xFF;
    if ((moby_state >= 4 && moby_state <= 6) || moby_state == 8) {
        data = *(int *)(moby + 0x78);
        FUN_L00_0025b8c0_c2((float *)(moby + 0x10), (float *)(data + 0xF0), target[0], b, c, position_snap_threshold_scale);
        FUN_L00_0025b8c0_c2((float *)(moby + 0x14), (float *)(data + 0xF4), target[1], b, c, position_snap_threshold_scale);
        FUN_L00_0025b8c0_c2((float *)(moby + 0x18), (float *)(data + 0xF8), target[2], b, c, position_snap_threshold_scale);
        func_L00_002592B0_2B6AA0(moby, target_heading, (float *)(data + 0xE4), heading_error_gain, heading_velocity_damping, heading_velocity_limit);
    }
}
typedef struct {
    unsigned char pad0[4];
    int w4;
    unsigned char pad8[2];
    unsigned char bA;
    unsigned char bB;
} PuffParticleTail;
typedef struct {
    unsigned char pad0[8];
    unsigned char b8;
    unsigned char b9;
    short life; /* 0xA: frames left (scale_game_frames) */
    unsigned char padC[0x14];
    PuffParticleTail tail;
} PuffParticle;
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void add_vector_xyz(void *, void *, void *);
extern void scale_vector_xyz(void *, void *, float);
extern int random_integer_below(int);
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float random_float_between(float, float);
extern int scale_game_frames(int);
extern void build_look_at_matrix(void *, void *, void *, float);
extern void spawn_two_point_effect_particle(void *, void *) __asm__("FUN_L00_0026c6e8");
extern void mine_camera_blast(int, void *, int, int) __asm__("FUN_L00_002d3838");
extern char D_L14_001D88D0[] __asm__("D_L14_001D88D0");
extern float D_L14_0015F580_x[] __asm__("D_L14_0015F580") __attribute__((section(".sdata")));
extern PuffParticle *spawn_puff(void *, float, float, float, int, void *, float,
                                int) __asm__("FUN_L00_0026d000");

void FUN_L14_002b5968(struct Moby *moby, float *point, float angle) {
    float offset[4];
    float effect_spawn_position[4];
    float small_offset[4];
    float direction[4];
    float original_pos[4];
    float value;
    float puff_size;
    float step;
    PuffParticle *part;
    PuffParticleTail *tail;
    int i;
    int j;
    int signed_puff_random_value;
    int color;
    int packed_color;
    int low_color;
    int high_color;

    value = fast_add_rotations(angle, moby->rot.z);
    offset[0] = fast_cos(value) * 0.8f;
    offset[1] = fast_sin(value) * 0.8f;
    offset[2] = 0.0f;
    add_vector_xyz(effect_spawn_position, point, offset);
    for (i = 5; i >= 0; i--) {
        signed_puff_random_value = random_integer_below(2);
        if (random_integer_below(2)) signed_puff_random_value = -signed_puff_random_value;
        puff_size = random_float_between(40000.0f, 150000.0f);
        color = random_int_between(10, 0x7f);
        low_color = (color << 8) | 0x7f000000;
        high_color = color << 16;
        packed_color = color | (high_color | low_color);
        part = spawn_puff(effect_spawn_position, 0.2f, 1.0f, 1.01f, signed_puff_random_value, D_L14_0015F580_x, puff_size, packed_color);
        if (part) {
            tail = &part->tail;
            part->life = scale_game_frames(120);
            tail->w4 = 2;
            tail->bA = 0x7f;
            tail->bB = part->life;
        }
    }
    qcopy(small_offset, offset);
    scale_vector_xyz(offset, offset, 0.0625f);
    offset[2] = random_float_between(0.025f, 0.06f);
    qcopy(original_pos, effect_spawn_position);
    step = 0.6283f;
    for (j = 0; j < 16; j++) {
        value = randf_sym(0.0f, 0.5f);
        build_look_at_matrix(direction, offset, small_offset, (float)j * step + value);
        effect_spawn_position[0] = original_pos[0] + randf_sym(0.0f, 0.2f);
        effect_spawn_position[1] = original_pos[1] + randf_sym(0.0f, 0.2f);
        effect_spawn_position[2] = original_pos[2] + randf_sym(0.0f, 0.2f);
        spawn_two_point_effect_particle(effect_spawn_position, direction);
    }
    mine_camera_blast((int)D_L14_001D88D0, original_pos, 0, 0);
}
/* Points the mover at the second-to-last node of its active path and clears its offset. */

typedef struct {
    char pad00[0xD0];
    int pathA;
    char padD4[0xE4 - 0xD4];
    int t;
    int node;
    float w;
    float off[4];
    char pad100[0x10C - 0x100];
    int alt;
    char pad110[0x150 - 0x110];
    int pathB;
} Mover_5c30;

extern int *D_L14_001B0BB0[];
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

void initialize_minibomber_path_retrace(struct Moby *moby) __asm__("FUN_L14_002b5c30");

void initialize_minibomber_path_retrace(struct Moby *moby) {
    Mover_5c30 *mover_vars = (Mover_5c30 *)moby->pvars;
    int *active_path;
    int penultimate_node_index;
    if (mover_vars->alt == 0)
        active_path = D_L14_001B0BB0[mover_vars->pathA];
    else
        active_path = D_L14_001B0BB0[mover_vars->pathB];
    penultimate_node_index = *active_path - 2;
    mover_vars->node = penultimate_node_index;
    mover_vars->w = *(float *)((penultimate_node_index << 4) + (char *)active_path + 0x1C);
    mover_vars->t = 0;
    clear_u64_value(mover_vars->off);
}
extern u8 D_0013E550[];
extern void release_voice_slot(s32 idx) __asm__("FUN_0022d798");

/* 0x002B5C98, 124 bytes.  Clears a moby's link to its slot entry, releasing
 * the slot if it still points back here.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002B6EE8), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002b5c98(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    s32 voice_slot_index;
    *(u16 *)(moby + 0x34) |= 0x41;
    *(s32 *)(moby + 0x94) = 0;
    voice_slot_index = *(s32 *)(data + 0x108);
    if (voice_slot_index != -1) {
        u8 *voice_slot = D_0013E550 + voice_slot_index * 0x70;
        if (*(u8 **)(voice_slot + 0x88) == moby && *(u8 *)(voice_slot + 0x74) != 0) {
            release_voice_slot(voice_slot_index);
        }
    }
    *(s32 *)(data + 0x108) = -1;
}
/* Resets the moby's path follow state and faces it along its current path. */

extern int *D_L14_001B0BB0[];
extern float D_L14_00161540 __attribute__((sda));
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

void FUN_L14_002b5d18(struct Moby *moby) {
    float initial_spring_value = D_L14_00161540 * 1.5f;
    char *moby_vars = (char *)moby->pvars;
    char *active_path;
    float v[4] __attribute__((aligned(16)));
    *(int *)(moby_vars + 0xE8) = 0;
    *(float *)(moby_vars + 0x160) = initial_spring_value;
    *(int *)(moby_vars + 0xEC) = 0;
    *(int *)(moby_vars + 0x164) = 0;
    *(int *)(moby_vars + 0x168) = 0;
    *(int *)(moby_vars + 0x16C) = 0;
    *(int *)(moby_vars + 0xC0) = 0;
    *(int *)(moby_vars + 0xC4) = 0;
    if (*(int *)(moby_vars + 0x10C) == 0)
        active_path = (char *)D_L14_001B0BB0[*(int *)(moby_vars + 0xD0)];
    else
        active_path = (char *)D_L14_001B0BB0[*(int *)(moby_vars + 0x150)];
    qcopy(&moby->pos, active_path + 0x10);
    fast_vec_sub(v, active_path + 0x20, &moby->pos);
    moby->rot.z = fast_arc_tan(v[0], v[1]);
    *(int *)(moby_vars + 0xE4) = 0;
    clear_u64_value(moby_vars + 0xF0);
}
/* Exhaust puffs: two red sparks and three white puffs of shrinking size behind the moby. */
extern int below_x(int) __asm__("FUN_00213260");
extern int frames_x(int) __asm__("FUN_001f96f8");
extern void scale_x(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_x(void *, void *, void *) __asm__("FUN_001f9a10");
extern float D_L14_0015F580_x[] __asm__("D_L14_0015F580") __attribute__((section(".sdata")));

void FUN_L14_002b5dd8(struct Moby *moby) {
    float exhaust_position_offset[4];
    float pos[4];
    int i;
    int j;
    int signed_puff_value;
    int life;
    float size;
    if (moby->state < 2 || moby->state == 9)
        return;
    scale_x(exhaust_position_offset, &moby->rot_row0, -1.25f);
    add_x(pos, &moby->pos, exhaust_position_offset);
    for (i = 0; i < 2; i++) {
        PuffParticle *part;
        PuffParticleTail *tail;
        signed_puff_value = below_x(0x10);
        part = spawn_puff(pos, 0.2f, 1.0f, 0.75f, below_x(2) == 0 ? signed_puff_value : -signed_puff_value, D_L14_0015F580_x,
                          160000.0f, 0x7F3030FF);
        if (part) {
            tail = &part->tail;
            part->life = frames_x(6);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->life;
        }
    }
    scale_x(exhaust_position_offset, &moby->rot_row0, -1.0f);
    add_x(pos, &moby->pos, exhaust_position_offset);
    signed_puff_value = 0x10;
    life = frames_x(2);
    size = 100000.0f;
    for (j = 0; j < 3; j++) {
        PuffParticle *part;
        PuffParticleTail *tail;
        part = spawn_puff(pos, 0.05f, 1.0f, 1.0f, signed_puff_value, D_L14_0015F580_x, size, 0x7FFFFFFF);
        if (part) {
            tail = &part->tail;
            part->life = life;
            part->b8 = below_x(0xFF);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->life;
        }
        signed_puff_value = -signed_puff_value;
        life *= 2;
        size -= 20000.0f;
    }
}
/* Flying debris: moves by its velocity under drag and gravity; while it is airborne it tumbles and trails
 * smoke, sparks and a light. When it hits the world or reaches the hero it explodes; once its timer runs out
 * or it leaves the level box it is removed. */
extern f32 D_L14_001615D0 __attribute__((sda));
extern f32 D_L14_001615D4 __attribute__((sda));
extern f32 D_L14_001615D8 __attribute__((sda));
extern char D_L14_001809C0[];
extern int tick_countdown_16_d(void *) __asm__("FUN_001f9770");
extern void add_d(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_d(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_d(void *, void *, float) __asm__("FUN_001f9a68");
extern void normalize_d(void *, void *, float) __asm__("FUN_001f9bf8");
extern void clear_d(void *) __asm__("FUN_001f99f8");
extern float abs_d(float) __asm__("FUN_001f99c0");
extern float add_rot_d(float, float) __asm__("FUN_001fa580");
extern int frames_d(int) __asm__("FUN_001f96f8");
extern int rand_below_d(int) __asm__("FUN_00213260");
extern float rand_between_d(float, float) __asm__("FUN_002132a8");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");
extern void e450_b78(void *, void *, void *, float, float, int, int, int, float, float,
                             float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern char *FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);
extern int FUN_L00_0023e738(float *, float, float, float, float, float);
extern PuffParticle *spawn_puff_d(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

void update_flying_debris(struct Moby *moby) __asm__("FUN_L14_002bac78");

void update_flying_debris(struct Moby *moby) {
    char *debris_vars = (char *)moby->pvars;
    float joint[4];
    float ofs[4];
    float at[4];
    float moby_to_joint_direction[4];
    float push[4];
    int i;
    int signed_puff_random_value;
    int puff_rgb_component;
    float size;
    float bright;
    u8 *puff_particle;
    u8 *puff_tail;
    float *light_record;
    float range = 3.0f;
    float green = 0.8f;
    float blue = 0.2f;

    if (tick_countdown_16_d(debris_vars + 0x10) != 0)
        { remove_flying_debris((u8 *)moby); return; }
    add_d(&moby->pos, &moby->pos, debris_vars);
    if (abs_d(*(float *)(debris_vars + 0x0)) > D_L14_001615D8 * frame_scale)
        *(float *)(debris_vars + 0x0) *= (D_L14_001615D4 - 1.0f) * frame_scale + 1.0f;
    if (abs_d(*(float *)(debris_vars + 0x4)) > D_L14_001615D8 * frame_scale)
        *(float *)(debris_vars + 0x4) *= (D_L14_001615D4 - 1.0f) * frame_scale + 1.0f;
    if (*(float *)(debris_vars + 0x8) > 0.0f)
        *(float *)(debris_vars + 0x8) *= (D_L14_001615D4 - 1.0f) * frame_scale + 1.0f;
    *(float *)(debris_vars + 0x8) -= D_L14_001615D0 * frame_scale_sq;
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f || moby->pos.y > 1021.0f || moby->pos.z < 2.0f ||
        moby->pos.z > 1021.0f)
        { remove_flying_debris((u8 *)moby); return; }
    if (coll_sphere(0.75f, &moby->pos, 0, 0) != 0 ||
        (abs_d(moby->pos.x - hero.motion.pos.f[0]) < 10.0f && abs_d(moby->pos.y - hero.motion.pos.f[1]) < 10.0f)) {
        clear_d(joint);
        trooper_play_sound(1, 0, moby, (*(struct Moby **)(debris_vars + 0x20))->oclass);
        e450_b78(moby, joint, &moby->pos, 2.0f, 4.0f, 5, 2, 4, 1.0f, 0.5f, 9.0f, 1.0f, -1, 5.0f, 0, 2,
                         -1, 0);
        remove_flying_debris((u8 *)moby);
        return;
    }
    moby->rot.x = add_rot_d(moby->rot.x, *(float *)(debris_vars + 0x14));
    moby->rot.y = add_rot_d(moby->rot.y, *(float *)(debris_vars + 0x18));
    moby->rot.z = add_rot_d(moby->rot.z, *(float *)(debris_vars + 0x1C));
    FUN_L00_0024f7c8(moby, 0, joint);
    size = randf_sym(0.01f, 0.5f);
    scale_d(ofs, (char *)moby + 0xC0, size);
    add_d(at, joint, ofs);
    sub_d(moby_to_joint_direction, joint, &moby->pos);
    normalize_d(moby_to_joint_direction, moby_to_joint_direction, 0.05f);
    FUN_L00_0026cbb0(at, moby_to_joint_direction, 0x4F007FFF, 0x1FFFFFFF, random_int_between(frames_d(10), frames_d(20)), 1, 40000.0f);
    for (i = 0; i < 3; i++) {
        if (i != 2) {
            scale_d(ofs, (char *)moby + 0xC0, randf_sym(0.01f, 0.5f));
            add_d(at, &moby->pos, ofs);
            scale_d(ofs, (char *)moby + 0xD0, randf_sym(0.01f, 0.5f));
            add_d(at, &moby->pos, ofs);
        }
        signed_puff_random_value = rand_below_d(6);
        if (rand_below_d(2) != 0)
            signed_puff_random_value = -signed_puff_random_value;
        size = rand_between_d(20000.0f, 100000.0f);
        puff_rgb_component = random_int_between(0x10, 0xFF);
        puff_particle = (u8 *)spawn_puff_d(at, 0.025f, 1.0f, 1.0f, signed_puff_random_value, D_L14_0015F580, size,
                         puff_rgb_component | (0x60000000 | (puff_rgb_component << 16) | (puff_rgb_component << 8)));
        if (puff_particle != NULL) {
            puff_particle[3] = 0x44;
            puff_tail = puff_particle + 0x20;
            *(s16 *)(puff_particle + 0xA) = frames_d(60);
            puff_tail[0xA] = 0x60;
            *(s32 *)(puff_tail + 4) = 2;
            puff_tail[0xB] = puff_particle[0xA];
        }
    }
    bright = 1.0f;
    sub_d(push, joint, &moby->pos);
    scale_d(push, push, 2.5f);
    add_d(at, &moby->pos, push);
    if (rand_below_d(2) != 0)
        scale_d(push, (char *)moby + 0xE0, -1.0f);
    else
        qcopy(push, (char *)moby + 0xE0);
    add_d(at, at, push);
    if (*(short *)(debris_vars + 0x12) == -1) {
        *(short *)(debris_vars + 0x12) = FUN_L00_0023e738(at, range, 0.0f, bright, green, blue);
    } else {
        light_record = (float *)(D_L14_001809C0 + *(short *)(debris_vars + 0x12) * 32);
        qcopy(light_record + 4, at);
        light_record[7] = range;
        light_record[0] = bright;
        light_record[1] = green;
        light_record[2] = blue;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002BC560), where it is exact; names translated to the US level program. */


extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int scale_game_frames(int);
extern void FUN_L00_00250df8(void *);
extern void build_look_at_matrix(void *dst, void *vec, void *axis,
                                 float angle) __asm__("FUN_00214890");
extern void cross_vectors_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern struct Moby *func_0020D348_m(int) __asm__("FUN_0020c4f8");

unsigned char *spawn_flying_debris(struct Moby *owner, float angle, char *posp) __asm__("FUN_L14_002bb310");

unsigned char *spawn_flying_debris(struct Moby *owner, float angle, char *posp) {
    float spawn_vector_scratch[12];
    unsigned char *spawned_moby;
    char *spawned_vars;
    char *hero_gravity_direction;
    float *spawn_position = spawn_vector_scratch;
    float *launch_vector;
    *(OvlQuad *)spawn_position = *(OvlQuad *)posp;
    spawned_moby = (unsigned char *)func_0020D348_m(0x51);
    if (spawned_moby != 0) {
        spawned_vars = *(char **)(spawned_moby + 0x78);
        qcopy(spawned_moby + 0x10, spawn_position);
        launch_vector = spawn_vector_scratch + 4;
        *(float *)(spawned_moby + 0x48) = angle;
        spawned_moby[0x20] = 0;
        spawned_moby[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        subtract_vector_xyz(launch_vector, spawn_position, &owner->pos);
        spawn_vector_scratch[6] = 0.0f;
        normalize_vector_xyz(launch_vector, launch_vector, random_float_between(frame_scale * 0.075f, frame_scale * 0.2f));
        hero_gravity_direction = (char *)&hero.gravity_dir;
        cross_vectors_xyz(spawn_vector_scratch + 8, hero_gravity_direction, launch_vector);
        build_look_at_matrix(spawned_vars, launch_vector, hero_gravity_direction, randf_sym(0.0f, 0.7853982f));
        *(float *)(spawned_vars + 8) = random_float_between(frame_scale * 0.05f, frame_scale * 0.32f);
        *(float *)(spawned_vars + 0x14) = randf_sym(0.0f, 0.034906585f);
        *(float *)(spawned_vars + 0x18) = randf_sym(0.0f, 0.034906585f);
        *(float *)(spawned_vars + 0x1C) = randf_sym(0.0f, 0.034906585f);
        *(short *)(spawned_vars + 0x10) = scale_game_frames(200);
        *(char **)(spawned_vars + 0x20) = owner;
        *(short *)(spawned_vars + 0x12) = -1;
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}

/* One record of the level's path table (D_L14_0015F70C), 0x20 bytes apart. */
struct L14PathRec {
    u8 pad0[0x10];
    s32 *points;       /* 0x10: first word is the point count */
    u8 pad14[0xC];
};

/* Pvars of a moby that waits for the race to start, rides its path and leaves at a trigger volume. */
struct L14RacerVars {
    u8 pad0[0x40];
    Vec4 trail;        /* 0x40: effect state (spawn_explosion) */
    u8 pad50[0x10];
    s32 path;          /* 0x60: index into D_L14_0015F70C (-1: none) */
    u8 pad64[8];
    s32 timer;         /* 0x6C */
    f32 inv_time;      /* 0x70: 1 / timer at the start of the run */
    u8 pad74[0xC];
    s32 delay;         /* 0x80: frames to wait once the race starts */
    s32 end_point;     /* 0x84: D_L14_001B0BB0 entry the run ends at */
    s32 start_point;   /* 0x88: D_L14_001B0BB0 entry the run starts from (-1: none) */
    u8 pad8C[0x64];
    s32 unkF0;
    s32 unkF4;
    u8 padF8[8];
    u8 manips[3][0x40]; /* 0x100: attached as manipulators 2..4 */
    u8 pad1C0[0xC];
    s32 volume;        /* 0x1CC: clip volume where the racer leaves (-1: none) */
};

/* Race controller; its phase reaches 0x14 when the race starts. */
struct L14RaceState {
    u8 pad0[0x86];
    s16 phase;         /* 0x86 */
};

extern struct L14PathRec *race_paths __asm__("D_L14_0015F70C") __attribute__((sda));
extern struct L14RaceState *race_state __asm__("D_L14_00167500") __attribute__((section(".data")));
extern s32 D_L14_00161A2C __attribute__((sda));
extern s32 D_L14_00161A34 __attribute__((sda));
extern s32 D_L14_00161A5C __attribute__((sda));
extern void memset_words(void *, int, int) __asm__("FUN_001f97e8");
extern void attach_manipulator(void *arg0, s32 arg1, void *arg2) __asm__("FUN_0020cb10");
extern int fast_dec_timer(s32 *) __asm__("FUN_001f9740");
extern void set_moby_animation(struct Moby *, s32, s32) __asm__("FUN_00212ed8");
extern f32 compute_interpolated_record_value(struct Moby *) __asm__("FUN_0020c9e0");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern unsigned char *spawn_plasmabot_death_debris(char *owner, char *pos) __asm__("FUN_L14_002dea98");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void spawn_path_or_ballistic_debris(struct Moby *, s32 *, int) __asm__("FUN_L14_002e0170");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
void initialize_racer_path_tracking(struct Moby *m) __asm__("FUN_L14_002d6358");
int interpolate_racer_to_path_start(struct Moby *m) __asm__("FUN_L14_002d64d0");
void update_racer_path_and_weapon(struct Moby *m) __asm__("FUN_L14_002d67e8");
void advance_moby_along_path(struct Moby *m) __asm__("FUN_L14_002d7198");
void update_moby_joint_visual_effects(struct Moby *m) __asm__("FUN_L14_002d7490");

void update_level14_racer(struct Moby *racer_moby) __asm__("FUN_L14_002d5f40");

void update_level14_racer(struct Moby *m) __asm__("FUN_L14_002d5f40");
void update_level14_racer(struct Moby *racer_moby) {
    enum {
        RACER_STATE_INITIALIZE = 0,
        RACER_STATE_WAIT_FOR_RACE_START = 1,
        RACER_STATE_WAIT_FOR_START_DELAY = 2,
        RACER_STATE_WAIT_FOR_START_ANIMATION = 3,
        RACER_STATE_COMPLETE_START_INTERPOLATION = 4,
        RACER_STATE_FOLLOW_PATH = 5,
        RACER_STATE_WAIT_FOR_EXIT_VOLUME = 6
    };
    struct L14RacerVars *racer_vars = (struct L14RacerVars *)racer_moby->pvars;
    u8 pos[16];
    f32 spawned_object_planar_velocity[3];
    unsigned char *spark;
    f32 *sv;

    switch (racer_moby->state) {
    case RACER_STATE_INITIALIZE:
        if (racer_vars->path == -1 || *race_paths[racer_vars->path].points == 0 || racer_vars->start_point == -1 ||
            *D_L14_001B0BB0[racer_vars->start_point] != 3 || racer_vars->volume == -1) {
            FUN_0020c828(racer_moby);
            return;
        }
        initialize_racer_path_tracking(racer_moby);
        racer_moby->state = 1;
        racer_moby->flags = (racer_moby->flags & 0xEFFF) | 0x41;
        racer_moby->collision = 0;
        racer_vars->unkF4 = -1;
        racer_vars->unkF0 = -1;
        memset_words(racer_vars->manips[0], 0, 0x40);
        attach_manipulator(racer_moby, 2, racer_vars->manips[0]);
        memset_words(racer_vars->manips[1], 0, 0x40);
        attach_manipulator(racer_moby, 3, racer_vars->manips[1]);
        memset_words(racer_vars->manips[2], 0, 0x40);
        attach_manipulator(racer_moby, 4, racer_vars->manips[2]);
        return;
    case RACER_STATE_WAIT_FOR_RACE_START:
        if (race_state->phase == 0x14) {
            racer_vars->timer = scale_game_frames(racer_vars->delay);
            racer_moby->state = 2;
        }
        return;
    case RACER_STATE_WAIT_FOR_START_DELAY:
        if (!fast_dec_timer(&racer_vars->timer))
            return;
        racer_moby->flags = (racer_moby->flags | 0x1000) & 0xFFBE;
        racer_moby->collision = racer_moby->pclass->collision;
        racer_moby->state = 3;
        set_moby_animation(racer_moby, 2, 0);
        racer_moby->anim_speed = 0.5f;
        racer_vars->timer = scale_game_frames(D_L14_00161A2C);
        racer_vars->inv_time = 1.0f / convert_integer_to_float(racer_vars->timer);
        qcopy(&racer_moby->pos, (char *)D_L14_001B0BB0[racer_vars->end_point] + 0x10);
    case RACER_STATE_WAIT_FOR_START_ANIMATION:
        if (compute_interpolated_record_value(racer_moby) >= 3.0f)
            interpolate_racer_to_path_start(racer_moby);
        if ((racer_moby->anim_flags & 2) && racer_moby->seq == racer_moby->prev_seq) {
            racer_moby->state = 4;
            blend_moby_animation((MobyAnim *)racer_moby, 0, 0, 10);
            racer_moby->anim_speed = 1.0f;
        }
        break;
    case RACER_STATE_COMPLETE_START_INTERPOLATION:
        if (interpolate_racer_to_path_start(racer_moby)) {
            racer_moby->state = 5;
            racer_vars->timer = scale_game_frames(D_L14_00161A34);
        }
        break;
    case RACER_STATE_FOLLOW_PATH:
        advance_moby_along_path(racer_moby);
        break;
    case RACER_STATE_WAIT_FOR_EXIT_VOLUME:
        update_racer_path_and_weapon(racer_moby);
        if (is_point_inside_clip_volume(&racer_moby->pos, racer_vars->volume)) {
            moby_point_position(racer_moby, 2, pos);
            spark = spawn_plasmabot_death_debris((char *)racer_moby, (char *)pos);
            if (spark) {
                sv = *(f32 **)(spark + 0x78);
                spawned_object_planar_velocity[0] = fast_cos(racer_moby->rot.z) * (frame_scale * 0.4f);
                spawned_object_planar_velocity[1] = fast_sin(racer_moby->rot.z) * (frame_scale * 0.4f);
                spawned_object_planar_velocity[2] = 0.0f;
                sv[0] = spawned_object_planar_velocity[0];
                sv[1] = spawned_object_planar_velocity[1];
            }
            spawn_path_or_ballistic_debris(racer_moby, &D_L14_00161A5C, 0);
            spawn_explosion(racer_moby, &racer_vars->trail, &racer_moby->pos, 0.0f, 0.0f, 5, 2, 4, 4.0f, 2.0f, 9.0f, 1, 1.0f,
                             15.0f, 1, 5, -1, 0);
            FUN_0020c828(racer_moby);
            return;
        }
        break;
    }
    update_moby_joint_visual_effects(racer_moby);
}

char D_L14_001FBD50[] = "Turret with no aiming cuboid. UID: %d\n";

char D_L14_001FBD78[] = "Turret with no waking cuboid. UID: %d\n";

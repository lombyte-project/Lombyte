/* Shared moby class drone: the drone device's drones that circle the hero and attack. */
#include "types.h"
#include "rnc/game/game_globals.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/math/fast_math.h"

typedef struct Moby Moby;

typedef unsigned int u128_2cdcc0 __attribute__((mode(TI), aligned(16)));

typedef struct {
    unsigned char  pad0[0x80];
    float          v80[4];
    unsigned char  pad90[8];
    float          f98;
    unsigned char  pad9c[0x1FF5 - 0x9C];
    unsigned char  b1FF5;
    unsigned char  b1FF6;
    unsigned char  pad1ff7[0x2080 - 0x1FF7];
    unsigned char *p2080;
} P_2cdcc0;

typedef struct {
    unsigned char  pad0[0xC0];
    unsigned short hC0;
    unsigned short hC2;
    unsigned int   wC4;
} S_2cdcc0;

/* Ported from rac1-decomp (nonmatching/shared/func_L00_002CF808.c: func_L00_002CF808). */
typedef int OvlQuad_2C9820 __attribute__((mode(TI)));

typedef struct {
    u8 pad0[0x18];
    s32 moby; /* 0x18: passed to FUN_002141f8 (reads moby+0x34, moby+0x78); 0 when no moby was hit */
    s32 unk1C; /* 0x1C: > 0 gates copying point and the normal into the hero (get_hero_idle_animation_index in shared/hero/hero_frame_update.c) */
    union {
        OvlQuad_2C9820 q;
        f32            f[4];
    } point;        /* 0x20: f[2] is read as the ground height */
    u8 pad30[0x10]; /* 0x30: position after push-out (copied into hero.motion.pos) */
    f32 normal_x; /* 0x40: start of the surface normal; its angle is tested against 50 degrees (0.87266463) */
    f32 normal_y; /* 0x44: fast_arc_tan (atan2) of normal_x, normal_y is stored in the hero */
    f32 normal_z; /* 0x48: ledge probes compare atan2(normal_z, xy length) with 20 degrees */
} CollisionHit_2C9820;

typedef struct {
    Vec4         position;  /* 0x00: movement this frame */
    Vec4         offset;    /* 0x10: last target position */
    f32          strength;  /* 0x20 */
    f32          elevation; /* 0x24 */
    f32          yaw;       /* 0x28 */
    f32          unused2c;
    struct Moby *target; /* 0x30 */
    f32          pitch;  /* 0x34 */
    f32          unused38;
    f32          height;      /* 0x3C */
    f32          desired_yaw; /* 0x40 */
    f32          unused44;
    s32          target_oclass; /* 0x48 */
    f32          unused4c;
    f32          unused50;
    s16          slot;           /* 0x54 */
    s16          health;         /* 0x56 */
    f32          yaw_velocity;   /* 0x58 */
    f32          pitch_velocity; /* 0x5C */
} DroneVars;

typedef struct {
    u8           pad_0[0x1FF4];
    u8           counter; /* 0x1FF4 */
    u8           unk1FF5;
    u8           ammo_used;     /* 0x1FF6: drones alive */
    u8           ammo_capacity; /* 0x1FF7: drones still orbiting */
    f32          phase;         /* 0x1FF8 */
    struct Moby *exclusions[9]; /* 0x1FFC */
    struct Moby *slots[6];      /* 0x2020 */
} DroneSlots;

/* Hit record passed to FUN_L00_00259a88; the drone builds it over its direction and rotated vectors. */
typedef struct {
    Vec4 direction; /* 0x00 */
    s32  unk10[2];
    u8   kind;   /* 0x18 */
    u8   mode;   /* 0x19 */
    u16  oclass; /* 0x1A */
    f32  amount; /* 0x1C: read back after the hit */
} BombHit;

extern s32          D_L00_0015F5C4_2c8440 __asm__("D_L00_0015F5C4"); /* nonzero: no throws */
extern CollisionHit coll_hit __asm__("D_L00_00173E40") NOT_SDA;

extern Vec4 g_debug_camera_lui __asm__("D_L00_00166DC0") NOT_SDA;
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern int  D_L00_0015FFE4;
extern s32  coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void update_entity(void *) __asm__("FUN_L00_00250df8");

extern char *get_moby_suck_cannon_record(char *) __asm__("FUN_L00_002db890");
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void  add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
void         update_entity(void *) __asm__("FUN_L00_00250df8");
void         copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");
extern int   get_ammo(int) __asm__("FUN_L00_00233e98");
extern int   rand_2cc608(void) __asm__("FUN_001160d8");

extern int     D_0015EEA4;
int            get_ammo(int) __asm__("FUN_L00_00233e98");
void           FUN_L00_00233db8_2cdcc0(int, int) __asm__("FUN_L00_00233db8");
float          FUN_001f9dc8_2cdcc0(float) __asm__("FUN_001f9dc8");
float          FUN_001f9de0_2cdcc0(float) __asm__("FUN_001f9de0");
int            FUN_001f96f8_2cdcc0(int) __asm__("FUN_001f96f8");
void           FUN_001f9a10_2cdcc0(void *, void *, void *) __asm__("FUN_001f9a10");
unsigned char *FUN_L00_002ce1f0_2cdcc0(unsigned char *, void *) __asm__("FUN_L00_002ce1f0");

extern int spawn_moby_point_attached_particle(
    void *, int, float, float, unsigned, unsigned) __asm__("FUN_L00_002725d8");

extern void fast_vec_normalize(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

extern void copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");

extern s32 func_L00_00259868_2CF808(f32, f32, f32, void *, void *, s32) __asm__("FUN_L00_00258830");
extern void initialize_hit_descriptor(void *, void *, s32, f32) __asm__("FUN_L00_00259858");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void jet_animate(f32, void *) __asm__("FUN_L00_00259fe8");
extern f32  func_L00_00259148_2CF808(f32, f32, f32, f32, f32, f32 *) __asm__("FUN_L00_00258110");
extern void func_L00_0025F4A8_2CF808(void *,
                                     void *,
                                     void *,
                                     f32,
                                     f32,
                                     s32,
                                     s32,
                                     s32,
                                     f32,
                                     f32,
                                     f32,
                                     s32,
                                     f32,
                                     f32,
                                     s32,
                                     s32,
                                     s32,
                                     s32) __asm__("FUN_L00_0025e450");
extern struct Moby *func_L00_002CF3D8_2CF808(void *, void *) __asm__("FUN_L00_002cdf28");
extern void         func_L00_002CF328_2CF808(void *) __asm__("FUN_L00_002cde78");

void replenish_hero_drones(void) __asm__("FUN_L00_002cdcc0");

void replenish_hero_drones(void) {
    P_2cdcc0    *hero_state = &(*(P_2cdcc0 *)((char *)&hero));
    S_2cdcc0    *usage_stats;
    float        initial_drone_velocity[4] __attribute__((aligned(16)));
    float        drone_spawn_position[4] __attribute__((aligned(16)));
    struct Moby *spawned_moby;
    int          i;
    hero_state->b1FF5 = 0;
    if (get_ammo(0x18)) {
        initial_drone_velocity[0] = FUN_001f9dc8_2cdcc0(hero_state->f98) * 0.25f;
        initial_drone_velocity[1] = FUN_001f9de0_2cdcc0(hero_state->f98) * 0.25f;
        initial_drone_velocity[2] = 0.25f;
        if (hero_state->b1FF6 < 6) {
            FUN_L00_00233db8_2cdcc0(0x18, 1);
            usage_stats = (S_2cdcc0 *)item_usage;
            if (usage_stats->hC0 < 0xFFFF) {
                usage_stats->hC0++;
            }
            if (FUN_001f96f8_2cdcc0(D_0015EEA4) / 600 > usage_stats->hC2) {
                usage_stats->hC2 = FUN_001f96f8_2cdcc0(D_0015EEA4) / 600;
            }
            usage_stats->wC4 = usage_stats->wC4 | (1 << current_level_index) | 0x80000000;

            FUN_001f9a10_2cdcc0(drone_spawn_position, hero_state->v80, hero_state->p2080 + 0xE0);
            FUN_001f9a10_2cdcc0(
                drone_spawn_position, drone_spawn_position, hero_state->p2080 + 0xC0);
            for (i = hero_state->b1FF6; i < 6; i++) {
                spawned_moby = FUN_L00_002ce1f0_2cdcc0(spawned_moby, drone_spawn_position);
                if (spawned_moby) {
                    qcopy(((void *)spawned_moby->pvars), initial_drone_velocity);
                }
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CF328), where it is exact; names translated to the US level program. */

void ensure_drone_attached_particles(struct Moby *owner_moby) __asm__("FUN_L00_002cde78");

void ensure_drone_attached_particles(struct Moby *owner_moby) {
    char *moby_vars = ((char *)owner_moby->pvars);
    if (*(int *)(moby_vars + 0x4C) == 0) {
        *(int *)(moby_vars + 0x4C) =
            spawn_moby_point_attached_particle(owner_moby, 0, 0.05f, 0.01f, 0x80808080, 0x10808080);
    }
    if (*(int *)(moby_vars + 0x50) == 0) {
        *(int *)(moby_vars + 0x50) =
            spawn_moby_point_attached_particle(owner_moby, 1, 0.05f, 0.01f, 0x80808080, 0x10808080);
    }
}

/* Exact match: tools/ps2eeas_nops.py keeps the hazard nop after the label the branch lands on. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CF3D8), where it is exact; names translated to the US level program. */

char *select_nearest_drone_target(char *search_position, int *excl) __asm__("FUN_L00_002cdf28");

char *select_nearest_drone_target(char *search_position, int *excl) {
    char *result = 0;
    char *candidate_moby;
    float best = 1e9f;
    float candidate_position_scratch[4];
    for (candidate_moby = (char *)D_L00_0015FFE4; candidate_moby != 0;
         candidate_moby = *(char **)(candidate_moby + 0x28)) {
        char *candidate_moby_class = *(char **)(candidate_moby + 0x24);
        int   class_category = 0;
        float search_to_candidate_distance;
        char *class_callback_data;
        char *x;
        if (candidate_moby_class != 0)
            class_category = *(unsigned char *)(candidate_moby_class + 0x46);
        if (candidate_moby == 0 || *(unsigned char *)(candidate_moby + 0x20) == 0xFE)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x20) == 0xFD)
            continue;
        if (class_category != 5 && class_category != 7 && class_category != 8)
            continue;
        if (excl != 0) {
            int found = 0;
            int i;
            for (i = 0; i < 9; i++) {
                if (excl[i] == (int)candidate_moby) {
                    found = 1;
                    break;
                }
            }
            if (found)
                continue;
        }
        if (class_category == 5 && (*(unsigned short *)(candidate_moby + 0x34) & 0x1000) == 0)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x31) == 0) {
            short c = *(short *)(candidate_moby + 0xA6);
            if (c != 0x350 && c != 0x31)
                continue;
        }
        search_to_candidate_distance = fast_vec_dist(search_position, candidate_moby + 0x10);
        if (*(short *)(candidate_moby + 0xA6) == 0x4D6) {
            fast_vec_normalize(candidate_position_scratch,
                               *(char **)(candidate_moby + 0x78),
                               search_to_candidate_distance);
            add_vector_xyz(
                candidate_position_scratch, candidate_position_scratch, candidate_moby + 0x10);
            if (search_to_candidate_distance <
                fast_vec_dist(search_position, candidate_position_scratch))
                continue;
        } else {
            float category_distance_limit;
            switch (class_category) {
            case 5:
                category_distance_limit = 4.0f;
                break;
            case 7:
                category_distance_limit = 5.3f;
                break;
            case 8:
                category_distance_limit = 6.0f;
                break;
            default:
                goto skip;
            }
            if (category_distance_limit < search_to_candidate_distance)
                continue;
        }
    skip:
        if (!(search_to_candidate_distance < best))
            continue;
        class_callback_data = get_moby_suck_cannon_record(candidate_moby);
        if (class_callback_data != 0 && *(short *)(class_callback_data + 0x68) > 0)
            continue;
        x = (char *)suck_get_aim_height(candidate_moby);
        qcopy(candidate_position_scratch, candidate_moby + 0x10);
        if (x != 0)
            candidate_position_scratch[2] =
                candidate_position_scratch[2] + (*(float *)(x + 0x10) + 0.05f);
        if (collision_line(
                search_position, candidate_position_scratch, 6, (int)candidate_moby, 0) == 0) {
            best = search_to_candidate_distance;
            result = candidate_moby;
        }
    }
    return result;
}

/* Spawns moby 0x1DF at pos, aims it at the player and puts it in the first
 * free of six slots. */

char *spawn_hero_drone(int previous_spawn_result, void *pos) __asm__("FUN_L00_002ce1f0");

char *spawn_hero_drone(int previous_spawn_result, void *pos) {
    char        *spawned_moby = create_moby(0x1DF);
    char        *moby_vars;
    struct Hero *player;

    if (spawned_moby != 0) {
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        *(unsigned char *)(spawned_moby + 0x31) = 1;
        *(unsigned char *)(spawned_moby + 0x20) = 2;
        moby_vars = *(char **)(spawned_moby + 0x78);
        *(unsigned char *)(spawned_moby + 0xBC) = 0;
        qcopy(spawned_moby + 0x10, pos);
        *(int *)(moby_vars + 0x44) = previous_spawn_result;
        player = &hero;
        player->ammo_used++;
        update_entity(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
        *(float *)(moby_vars + 0x28) =
            fast_arc_tan(*(float *)(spawned_moby + 0x10) - player->motion.pos.f[0],
                         *(float *)(spawned_moby + 0x14) - player->motion.pos.f[1]);
        *(float *)(moby_vars + 0x24) =
            fast_arc_tan(vector_distance_xy(&player->motion.pos, spawned_moby + 0x10),
                         *(float *)(spawned_moby + 0x18) - player->motion.pos.f[2]);
        *(float *)(moby_vars + 0x20) = vector_distance_xy(spawned_moby + 0x10, &player->motion.pos);
        *(float *)(moby_vars + 0x3C) = player->motion.pos.f[2];
        for (*(short *)(moby_vars + 0x54) = 0; *(short *)(moby_vars + 0x54) < 6;
             (*(short *)(moby_vars + 0x54))++) {
            if (hero.unk2020[*(short *)(moby_vars + 0x54)] == 0) {
                hero.unk2020[*(short *)(moby_vars + 0x54)] = spawned_moby;
                break;
            }
        }
        *(short *)(moby_vars + 0x56) = 100;
        *(int *)(spawned_moby + 0x94) = 0;
    }
    return spawned_moby;
}

void update_drone(struct Moby *updated_moby) __asm__("FUN_L00_002ce358");

void update_drone(struct Moby *updated_moby) {
    DroneVars   *drone_vars = (DroneVars *)updated_moby->pvars;
    Vec4         collision_data[1], old_pos;
    Vec4         direction, rotated;
    Vec4         target_pos, matrix[4];
    struct Moby *target;
    struct Moby *closest;
    struct Moby *other;
    f32          f, distance;
    s32          i, count, category, random, health;
    s32          state;
    struct Moby *drone;
    s32          k, exclusion_clear_index;

    if (D_L00_0015F5C4_2c8440 == 2 || D_L00_0015F5C4_2c8440 == 5 || D_L00_0015F5C4_2c8440 == 6 ||
        hero.state.current == 0x32) {
        updated_moby->flags |= 0x41;
        return;
    }
    updated_moby->flags &= 0xFFBE;
    if (updated_moby->state < 5) {
        ((DroneSlots *)&hero)->counter++;
        if (((DroneSlots *)&hero)->ammo_used < ((DroneSlots *)&hero)->counter) {
            closest = 0;
            count = 0;
            distance = 1000000.0f;
            ((DroneSlots *)&hero)->counter = 1;
            ((DroneSlots *)&hero)->ammo_capacity = 0;
            ((DroneSlots *)&hero)->phase =
                fast_add_rotations(((DroneSlots *)&hero)->phase, frame_scale * 0.05f);
            for (k = 0; k < 6; k++) {
                drone = ((DroneSlots *)&hero)->slots[k];
                if (drone != 0) {
                    if (drone->oclass == 0x1DF) {
                        if (drone->state == 0xFE) {
                            ((DroneSlots *)&hero)->slots[k] = 0;
                        } else if (drone->state == 0xFD) {
                            ((DroneSlots *)&hero)->slots[k] = 0;
                        } else {
                            ((DroneSlots *)&hero)->exclusions[count] =
                                *(struct Moby **)(drone->pvars + 0x30);
                            count++;
                            if (drone->state < 4)
                                ((DroneSlots *)&hero)->ammo_capacity++;
                            if (count > 8)
                                break;
                        }
                    } else {
                        ((DroneSlots *)&hero)->slots[k] = 0;
                    }
                }
            }
            for (exclusion_clear_index = count; exclusion_clear_index < 9; exclusion_clear_index++)
                ((DroneSlots *)&hero)->exclusions[exclusion_clear_index] = 0;
            other = func_L00_002CF3D8_2CF808(&hero.motion, ((DroneSlots *)&hero)->exclusions);
            if (other != 0) {
                char *h;
                h = suck_get_aim_height(other);
                qcopy(&old_pos, &other->pos);
                if (h != 0)
                    old_pos.f[2] += *(f32 *)(h + 0x10);
                for (i = 0; i < 6; i++) {
                    drone = ((DroneSlots *)&hero)->slots[i];
                    if (drone != 0) {
                        struct Moby *drone_target = *(struct Moby **)(drone->pvars + 0x30);
                        if (drone_target == 0 || drone_target->state == 0xFE ||
                            drone_target->state == 0xFD) {
                            f32 candidate_distance = vector_distance_xy(&drone->pos, &old_pos);
                            if (candidate_distance < distance) {
                                closest = drone;
                                distance = candidate_distance;
                            }
                        }
                    }
                }
                if (closest != 0) {
                    DroneVars *assigned = (DroneVars *)closest->pvars;
                    char      *target_height_data;
                    assigned->target = other;
                    assigned->target_oclass = other->oclass;
                    target_height_data = suck_get_aim_height(other);
                    qcopy(&old_pos, &other->pos);
                    if (target_height_data != 0)
                        old_pos.f[2] += *(f32 *)(target_height_data + 0x10);
                    qcopy(&assigned->offset, &old_pos);
                    closest->state = 3;
                    random = rand_2cc608() % 3;
                    allocate_voice_for_target_entry(random + (random > 0), 0, updated_moby);
                    ((DroneSlots *)&hero)->exclusions[count] = assigned->target;
                }
            }
        }
        {
            s32 capacity = ((DroneSlots *)&hero)->ammo_used;
            s32 divisor = capacity ? capacity : 1;
            s32 next = capacity + 1;
            s32 counter = ((DroneSlots *)&hero)->counter;
            if (next == 0)
                next = 1;
            drone_vars->desired_yaw =
                convert_integer_to_float(counter) / (f32)divisor * 6.2831855f - 3.1415927f;
            drone_vars->pitch = (convert_integer_to_float((counter * (next / 5)) % next) /
                                     (f32)divisor * 6.2831855f -
                                 3.1415927f) *
                                0.25f;
        }
        {
            void *hit = find_moby_hit(updated_moby, 1, 0);
            if (hit != 0) {
                char *h = *(char **)((char *)hit + 0x20);
                s32   cat = h && *(char **)(h + 0x24) ? *(s16 *)(*(char **)(h + 0x24) + 0x46) : 0;
                if (cat > 8)
                    goto dispatch_state;
                if (cat >= 5) {
                    if (hit == 0 || h == 0 || *(s16 *)(h + 0xA6) != 0x4D6 ||
                        fast_dec_timer_s16(&drone_vars->health))
                        updated_moby->state = 4;
                }
            }
        }
    }
dispatch_state:
    state = updated_moby->state;
    if (state == 3)
        goto state3;
    if (state < 4) {
        if (state == 2)
            goto state2;
    } else {
        if (state == 4)
            goto state4;
        if (state == 5)
            goto state5;
    }
    goto update_effects;
state2: {
    updated_moby->collision = 0;
    qcopy(&old_pos, &updated_moby->pos);
    func_L00_002CF328_2CF808(updated_moby);
    {
        f32 step = frame_scale * 0.05f;
        updated_moby->scale += (updated_moby->pclass->scale - updated_moby->scale) * step;
        drone_vars->strength = drone_vars->strength + (1.0f - drone_vars->strength) * step;
    }
    f = fast_add_rotations(drone_vars->desired_yaw, ((DroneSlots *)&hero)->phase);
    distance = fast_subtract_rotations(f, drone_vars->yaw);
    drone_vars->yaw = fast_add_rotations(drone_vars->yaw, distance * (frame_scale * 0.05f));
    distance = fast_subtract_rotations(drone_vars->pitch, drone_vars->elevation);
    drone_vars->elevation =
        fast_add_rotations(drone_vars->elevation, distance * (frame_scale * 0.01f));
    direction.f[0] = fast_cos(drone_vars->yaw) * drone_vars->strength;
    direction.f[1] = fast_sin(drone_vars->yaw) * drone_vars->strength;
    direction.f[2] = 0.0f;
    rotated.f[0] = 0.0f;
    rotated.f[1] = drone_vars->elevation;
    rotated.f[2] = 0.0f;
    rotated.f[3] = 0.0f;
    euler_rotation_matrix(matrix, &rotated);
    transform_vector(&direction, &direction, matrix);
    add_vector_xyz(&target_pos, &hero.motion, (u8 *)hero.moby + 0xE0);
    target_pos.f[0] += direction.f[0];
    target_pos.f[1] += direction.f[1];
    drone_vars->height =
        drone_vars->height + (target_pos.f[2] - drone_vars->height) * (frame_scale * 0.05f);
    target_pos.f[2] = drone_vars->height + direction.f[2];
    fast_vec_sub(&target_pos, &target_pos, &updated_moby->pos);
    clamp_vector_length(&target_pos, &target_pos, frame_time * 10.0f);
    if (func_L00_00259868_2CF808(0.0f, 0.18f, 0.0f, updated_moby, &target_pos, 0) & 1) {
        if (!updated_moby->visible) {
            Vec4 *hitv = &g_debug_camera_lui;
            add_vector_xyz(&updated_moby->pos, hitv, hitv + 0x23);
            fast_vec_sub(&updated_moby->pos, &updated_moby->pos, hitv + 0x21);
        }
    }
    qcopy(&collision_data[0], &updated_moby->pos);
    if (coll_sphere(0.2f, &collision_data[0], 0, updated_moby)) {
        if (((struct Moby *)coll_hit.moby) && ((struct Moby *)coll_hit.moby)->oclass == 0 &&
            ((struct Moby *)coll_hit.moby)->state != 0xFE &&
            ((struct Moby *)coll_hit.moby)->state != 0xFD)
            goto update_rotations;
        category = ((struct Moby *)coll_hit.moby) && ((struct Moby *)coll_hit.moby)->pclass
                       ? *(u8 *)((u8 *)((struct Moby *)coll_hit.moby)->pclass + 0x46)
                       : 0;
        if (((struct Moby *)coll_hit.moby) && ((struct Moby *)coll_hit.moby)->state != 0xFE &&
            ((struct Moby *)coll_hit.moby)->state != 0xFD &&
            (category == 5 || category == 7 || category == 8)) {
            initialize_hit_descriptor(((BombHit *)&direction), updated_moby, 0x10000, 1.0f);
            set_vector_length_xy(((BombHit *)&direction), &drone_vars->position, 1.0f);
            ((BombHit *)&direction)->direction.f[2] = 1.0f;
            ((BombHit *)&direction)->direction.f[3] = 5627.925f;
            ((BombHit *)&direction)->kind = 1;
            ((BombHit *)&direction)->mode = 2;
            ((BombHit *)&direction)->oclass = updated_moby->oclass;
            record_moby_hit((struct Moby *)coll_hit.moby, ((BombHit *)&direction));
            if (category == 5) {
                if (coll_hit.moby && ((struct Moby *)coll_hit.moby)->oclass == 0x4D6 &&
                    (health = drone_vars->health) > 0) {
                    random = truncate_float_to_s32(((BombHit *)&direction)->amount);
                    if (random > 0)
                        random = health - truncate_float_to_s32(((BombHit *)&direction)->amount);
                    else
                        random = health - 1;
                    drone_vars->health = random;
                } else {
                    func_L00_0025F4A8_2CF808(updated_moby,
                                             drone_vars,
                                             0,
                                             0.0f,
                                             0.0f,
                                             0,
                                             1,
                                             1,
                                             0.4f,
                                             0.2f,
                                             4.0f,
                                             1,
                                             1.0f,
                                             8.0f,
                                             1,
                                             1,
                                             -1,
                                             0);
                    updated_moby->state = 4;
                }
            }
        }
    }
update_rotations:
    fast_vec_sub(&drone_vars->position, &updated_moby->pos, &old_pos);
    {
        f32 yaw =
            fast_arc_tan(updated_moby->pos.x - old_pos.f[0], updated_moby->pos.y - old_pos.f[1]);
        f32 pitch = -fast_arc_tan(vector_distance_xy(&old_pos, &updated_moby->pos),
                                  updated_moby->pos.z - old_pos.f[2]);
        updated_moby->rot.z = func_L00_00259148_2CF808(updated_moby->rot.z,
                                                       yaw,
                                                       frame_time_sq * 6.2831855f,
                                                       frame_time_sq * 3.1415927f,
                                                       frame_time * 6.2831855f,
                                                       &drone_vars->yaw_velocity);
        updated_moby->rot.y = func_L00_00259148_2CF808(updated_moby->rot.y,
                                                       pitch,
                                                       frame_time_sq * 6.2831855f,
                                                       frame_time_sq * 3.1415927f,
                                                       frame_time * 6.2831855f,
                                                       &drone_vars->pitch_velocity);
    }
    goto update_effects;
}
state3: {
    struct MobyClass *pclass = updated_moby->pclass;
    updated_moby->collision = pclass->collision;
    qcopy(&old_pos, &updated_moby->pos);
    updated_moby->scale += (pclass->scale - updated_moby->scale) * (frame_scale * 0.05f);
    updated_moby->collision = pclass->collision;
    func_L00_002CF328_2CF808(updated_moby);
    target = drone_vars->target;
    if (target != 0 && target->oclass == drone_vars->target_oclass && target->state != 0xFE &&
        target->state != 0xFD) {
        char *h = suck_get_aim_height(target);
        qcopy(&target_pos, &drone_vars->target->pos);
        if (h != 0)
            target_pos.f[2] += *(f32 *)(h + 0x10);
        fast_vec_sub(&direction, &target_pos, &drone_vars->offset);
        fast_vec_scale(&direction, &direction, 3.1f);
        add_vector_xyz(&rotated, &target_pos, &direction);
        fast_vec_sub(&drone_vars->position, &rotated, &updated_moby->pos);
        fast_vec_scale(&drone_vars->position, &drone_vars->position, frame_scale * -0.8f + 1.0f);
        add_vector_xyz(&updated_moby->pos, &updated_moby->pos, &drone_vars->position);
        qcopy(&drone_vars->offset, &target_pos);
        qcopy(&collision_data[0], &updated_moby->pos);
        if (coll_sphere(0.25f, &collision_data[0], 0, updated_moby)) {
            struct Moby *hm = (struct Moby *)coll_hit.moby;
            if (!hm || hm->state == 0xFE || hm->state == 0xFD || hm != drone_vars->target ||
                hm->oclass == 0) {
                qcopy(&updated_moby->pos, &coll_hit_position);
            } else {
                s32 cat2 = hm->pclass ? *(u8 *)((u8 *)hm->pclass + 0x46) : 0;
                initialize_hit_descriptor(((BombHit *)&direction), updated_moby, 0x10000, 1.0f);
                set_vector_length_xy(((BombHit *)&direction), &drone_vars->position, 1.0f);
                ((BombHit *)&direction)->direction.f[2] = 1.0f;
                ((BombHit *)&direction)->direction.f[3] = 5627.925f;
                ((BombHit *)&direction)->kind = 1;
                ((BombHit *)&direction)->mode = 2;
                ((BombHit *)&direction)->oclass = updated_moby->oclass;
                record_moby_hit((struct Moby *)coll_hit.moby, ((BombHit *)&direction));
                if (cat2 == 5) {
                    if (coll_hit.moby && ((struct Moby *)coll_hit.moby)->oclass == 0x4D6 &&
                        (health = drone_vars->health) > 0) {
                        random = truncate_float_to_s32(((BombHit *)&direction)->amount);
                        if (random > 0)
                            random =
                                health - truncate_float_to_s32(((BombHit *)&direction)->amount);
                        else
                            random = health - 1;
                        drone_vars->health = random;
                    } else {
                        func_L00_0025F4A8_2CF808(updated_moby,
                                                 drone_vars,
                                                 0,
                                                 0.0f,
                                                 0.0f,
                                                 0,
                                                 1,
                                                 1,
                                                 0.4f,
                                                 0.2f,
                                                 4.0f,
                                                 1,
                                                 1.0f,
                                                 8.0f,
                                                 1,
                                                 1,
                                                 -1,
                                                 0);
                        updated_moby->state = 4;
                    }
                }
            }
        }
    } else {
        updated_moby->state = 2;
        drone_vars->target_oclass = -1;
        drone_vars->target = 0;
    }
    updated_moby->rot.z =
        fast_arc_tan(updated_moby->pos.x - old_pos.f[0], updated_moby->pos.y - old_pos.f[1]);
    updated_moby->rot.y = -fast_arc_tan(vector_distance_xy(&old_pos, &updated_moby->pos),
                                        updated_moby->pos.z - old_pos.f[2]);
    goto update_effects;
}
state4: {
    target = drone_vars->target;
    if (target == 0)
        goto fade_clear;
    if (target->oclass != drone_vars->target_oclass)
        goto fade_clear;
    if (target->state == 0xFE)
        goto fade_clear;
    if (target->state == 0xFD)
        goto fade_clear;
    {
        char *h = suck_get_aim_height(target);
        qcopy(&old_pos, &drone_vars->target->pos);
        if (h != 0)
            old_pos.f[2] += *(f32 *)(h + 0x10);
        fast_vec_sub(&drone_vars->position, &old_pos, &updated_moby->pos);
        fast_vec_scale(&drone_vars->position, &drone_vars->position, frame_scale * -0.8f + 1.0f);
        clamp_vector_length(&drone_vars->position, &drone_vars->position, frame_scale * 0.9f);
        add_vector_xyz(&updated_moby->pos, &updated_moby->pos, &drone_vars->position);
    }
    goto fade_alpha;
fade_clear:
    drone_vars->target = 0;
fade_alpha:
    updated_moby->alpha -= 4;
    if (updated_moby->alpha < 6)
        updated_moby->state = 5;
    goto update_effects;
}
state5: {
    ((DroneSlots *)&hero)->ammo_used--;
    ((DroneSlots *)&hero)->slots[drone_vars->slot] = 0;
    mark_moby_for_removal(updated_moby);
    return;
}
update_effects:
    jet_animate(0.17f, updated_moby);
}

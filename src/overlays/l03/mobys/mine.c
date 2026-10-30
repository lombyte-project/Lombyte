/* Level 03 moby class mine: the mine a mine_layer drops. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/hero/hero.h"
#include "rnc/overlay/quad.h"

void        handle_mine_hit(unsigned char *mine_moby) __asm__("FUN_L03_002cc828");
extern char D_L03_00166EC0[];
extern void blend_moby_animation(struct Moby *, int, int, int) __asm__("FUN_00212f90");
extern int  step_moby_motion(struct Moby *, void *) __asm__("FUN_L00_0025c698");

typedef float MineVec[4] __attribute__((aligned(16)));

/* Explosion description handed to mine_camera_blast (0x50 bytes, copied from a level constant). */
typedef struct {
    OvlQuad q[5];
} MineBlast;

/* Pvars of the l03 mine. */
typedef struct {
    u8           pad0[0x60]; /* 0x00: fall record (step_moby_motion) */
    s32          unk60;
    s32          timer;  /* 0x64: arming delay */
    struct Moby *target; /* 0x68: moby it is stuck to or homing on */
    s32          age;    /* 0x6C: frames alive */
} MineVars;

extern MineBlast    l03_mine_camera_blast_template __asm__("D_L03_001E32B0");
extern struct Moby *D_L03_00178000[];
extern char         mine_moby_index_list[] __asm__("D_L03_001B08B0");
extern void allocate_voice_for_target_entry(int, int, struct Moby *) __asm__("FUN_0022da68");
extern int
collect_mobies_in_sphere(void *, int, struct Moby *, void *, f32) __asm__("FUN_L00_001f2868");
extern void
send_blast_hits(struct Moby *, void *, void *, int, int, int, int, int, f32, f32, f32) __asm__("FUN_L00_0025a9f8");
extern s32  random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern int  mine_blend_color(int, int, f32) __asm__("FUN_L00_002371e0");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern void mine_debris(void *, void *, int, int, f32, int, int) __asm__("FUN_L00_0026bed0");
extern void mine_camera_blast(void *, void *, int, int) __asm__("FUN_L00_002d3838");
extern void mine_spawn_drops(struct Moby *, void *) __asm__("FUN_L00_0025f800");

/* l03 mine update: falls until it lands, sticks to or homes on its target, arms,
   and explodes on contact (debris, camera shake, blast) or shrinks away when old. */
void update_mine(struct Moby *mine_moby) __asm__("FUN_L03_002cbea8");

void update_mine(struct Moby *mine_moby) {
    enum {
        MINE_IDLE = 0,
        MINE_FALL_UNTIL_LANDING = 1,
        MINE_HOME_ON_TARGET = 2,
        MINE_ARM_AND_MONITOR_PROXIMITY = 3,
        MINE_EXPLODE_AND_REMOVE = 4,
        MINE_SHRINK_AND_REMOVE = 5
    };
    enum {
        MINE_DEBRIS_FIXED_PALETTE_COLOR = 0,
        MINE_DEBRIS_WIDE_SPAWN_RADIUS = 1,
        MINE_DEBRIS_RANDOM_PALETTE_COLOR = 2,
        MINE_DEBRIS_SPHERICAL_VELOCITY = 3
    };
    MineBlast    blast = l03_mine_camera_blast_template;
    MineVec      vector_scratch;
    MineVars    *mine_vars = (MineVars *)mine_moby->pvars;
    struct Moby *target_moby;
    float        explosion_reference_distance;
    int          nearby_moby_count;
    int          i;
    int          k;

    handle_mine_hit((unsigned char *)mine_moby);
    mine_vars->unk60 = 0;
    mine_vars->age++;
    switch (mine_moby->state) {
    case MINE_IDLE:
        break;
    case MINE_FALL_UNTIL_LANDING:
        if (step_moby_motion(mine_moby, mine_vars) & 1) {
            mine_moby->state = 3;
            if (mine_moby->prev_seq != 1) {
                blend_moby_animation(mine_moby, 1, 0, scale_game_frames(10));
            }
            mine_vars->timer = scale_game_frames(60);
        } else if (mine_moby->pos.z < 5.0f) {
            mark_moby_for_removal(mine_moby);
            return;
        }
        break;
    case MINE_HOME_ON_TARGET:
        fast_vec_sub(vector_scratch, &mine_vars->target->pos, &mine_moby->pos);
        if (fast_vec_length(vector_scratch) < 0.25f) {
            mark_moby_for_removal(mine_moby);
            return;
        }
        fast_vec_normalize(vector_scratch, vector_scratch, frame_time * 14.0f);
        vector_scratch[2] += frame_time * 4.0f;
        fast_vec_add(&mine_moby->pos, &mine_moby->pos, vector_scratch);
        break;
    case MINE_ARM_AND_MONITOR_PROXIMITY:
        fast_dec_timer(&mine_vars->timer);
        target_moby = mine_vars->target;
        if (target_moby != 0 && target_moby->state != 0xFE && target_moby->state != 0xFD &&
            vector_distance_xy(&mine_moby->pos, &target_moby->pos) < 3.0f &&
            mine_vars->timer == 0) {
            mine_moby->state = 2;
            allocate_voice_for_target_entry(2, 0, mine_moby);
        }
        if (scale_game_frames(3600) < mine_vars->age &&
            (mine_vars->target == 0 || mine_vars->target->state == 0xFE ||
             mine_vars->target->state == 0xFD || scale_game_frames(5400) < mine_vars->age)) {
            mine_moby->state = 5;
        }
        if (vector_distance_xy(&hero.moby->pos, &mine_moby->pos) < 0.5f &&
            absolute_float(mine_moby->pos.z - hero.motion.pos.f[2]) < 0.25f) {
            mine_moby->state = 4;
            break;
        }
        nearby_moby_count = collect_mobies_in_sphere(&mine_moby->pos, 0x10, mine_moby, 0, 1.0f);
        for (k = 0; k < nearby_moby_count; k++) {
            struct Moby *nearby_moby = D_L03_00178000[k];

            if ((nearby_moby->flags & 0x1000) &&
                (nearby_moby != mine_vars->target || mine_vars->timer == 0)) {
                mine_moby->state = 4;
                break;
            }
        }
        break;
    case MINE_EXPLODE_AND_REMOVE: {
        int debris_spawn_count;

        explosion_reference_distance = vector_distance_xy(&mine_moby->pos, D_L03_00166EC0);
        nearby_moby_count = collect_mobies_in_sphere(&mine_moby->pos, 0x10, mine_moby, 0, 2.0f);
        *(OvlQuad *)vector_scratch = *(OvlQuad *)&mine_moby->pos;
        send_blast_hits(mine_moby,
                        vector_scratch,
                        D_L03_00178000,
                        nearby_moby_count,
                        0,
                        0x810001,
                        4,
                        1,
                        3.0f,
                        0.25f,
                        1.5f);
        debris_spawn_count = truncate_float_to_s32(explosion_reference_distance * 15.0f);
        for (i = 0; i < debris_spawn_count; i++) {
            int kind = random_integer_below(4);

            *(Vec4f *)vector_scratch =
                (Vec4f){frame_time * 14.0f, frame_time * 22.0f, frame_time * 12.0f, 0.0f};
            {
                MineVec debris_velocity;
                MineVec debris_vector_scratch;
                float   rr;
                float   debris_azimuth;

                *(OvlQuad *)debris_vector_scratch = 0;
                debris_vector_scratch[0] =
                    random_float_between(frame_time * -0.5f, frame_time * 0.5f);
                debris_vector_scratch[1] =
                    random_float_between(frame_time * -0.5f, frame_time * 0.5f);
                debris_vector_scratch[2] =
                    random_float_between(frame_time * 4.0f, vector_scratch[kind]);
                *(OvlQuad *)debris_velocity = *(OvlQuad *)debris_vector_scratch;
                rr = random_float_between(0.0f, kind == 1 ? 0.5f : 0.25f);
                debris_azimuth = random_angle_radians();
                debris_vector_scratch[0] = fast_cos(debris_azimuth) * rr;
                debris_vector_scratch[1] = fast_sin(debris_azimuth) * rr;
                debris_vector_scratch[2] = 0.0f;
                fast_vec_add(debris_vector_scratch, debris_vector_scratch, &mine_moby->pos);
                debris_vector_scratch[2] -= rr * fast_cos(0.785398f);
                debris_velocity[2] -= rr * frame_time * 8.0f;
                switch (kind) {
                case MINE_DEBRIS_FIXED_PALETTE_COLOR:
                    mine_debris(
                        debris_vector_scratch,
                        debris_velocity,
                        0x1F101820,
                        0x101010,
                        random_float_between(200000.0f, 300000.0f),
                        random_int_between(scale_game_frames(0xB4), scale_game_frames(0xF0)),
                        0);
                    break;
                case MINE_DEBRIS_WIDE_SPAWN_RADIUS:
                    mine_debris(debris_vector_scratch,
                                debris_velocity,
                                0x3F081020,
                                0x0F081020,
                                random_float_between(50000.0f, 100000.0f),
                                scale_game_frames(0xB4),
                                1);
                    break;
                case MINE_DEBRIS_RANDOM_PALETTE_COLOR:
                    mine_debris(
                        debris_vector_scratch,
                        debris_velocity,
                        random_integer_below(100) < 0x28 ? 0x5FF8F8F8 : 0x2F486078,
                        0x0F000020,
                        150000.0f,
                        random_int_between(scale_game_frames(0x1E), scale_game_frames(0x2D)),
                        2);
                    break;
                case MINE_DEBRIS_SPHERICAL_VELOCITY: {
                    int debris_spawn_color =
                        mine_blend_color(0x7F000000, 0x7F182030, random_float_between(0.25f, 1.0f));
                    int debris_grayscale_color =
                        mine_blend_color(0, 0x5F5F5F, random_float_between(0.5f, 1.0f));

                    build_spherical_offset(debris_velocity,
                                           random_float_between(0.0f, 1.0f) * frame_time,
                                           debris_azimuth,
                                           random_angle_radians());
                    mine_debris(
                        debris_vector_scratch,
                        debris_velocity,
                        debris_spawn_color,
                        debris_grayscale_color,
                        random_float_between(200000.0f, 300000.0f),
                        random_int_between(scale_game_frames(0xF0), scale_game_frames(0x12C)),
                        3);
                    break;
                }
                }
            }
        }
        allocate_voice_for_target_entry(0, 0, mine_moby);
        l03_camera.shake_amount = explosion_reference_distance < 20.0f
                                      ? 0.4f - explosion_reference_distance * 0.0175f
                                      : 0.050000012f;
        l03_camera.shake_frames = scale_game_frames(0x19);
        mine_camera_blast(&blast, &mine_moby->pos, 0, 0);
        mine_spawn_drops(mine_moby, mine_moby_index_list);
        mark_moby_for_removal(mine_moby);
        return;
    }
    case MINE_SHRINK_AND_REMOVE:
        mine_moby->scale *= frame_scale * -0.074f + 1.0f;
        if (mine_moby->scale < mine_moby->pclass->scale * 0.01f) {
            mine_spawn_drops(mine_moby, mine_moby_index_list);
            mark_moby_for_removal(mine_moby);
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002CDBF0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

void handle_mine_hit(unsigned char *mine_moby) {
    char *item = find_moby_hit(mine_moby, 0x130000, 0);
    if (item != 0) {
        char *other = *(char **)(item + 0x20);
        if (other != 0 && *(short *)(other + 0xA6) != MOBY(mine_moby)->oclass) {
            mine_moby[0x20] = 4;
        }
    }
    mine_moby[0xA4] = 0xFF;
}

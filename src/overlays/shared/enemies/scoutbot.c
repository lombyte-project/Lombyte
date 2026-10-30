/* Shared moby class scoutbot. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/quad.h"

typedef struct Moby Moby;

typedef struct Data_3DF8 {
    char          pad0[0x29];
    unsigned char b29;
    char          pad2A[0x16];
    float         vel[4];
    char          pad50[8];
    unsigned char b58;
    char          pad59;
    unsigned char b5A;
    char          pad5B[0x6D];
    short         hC8;
    char          padCA[6];
    void         *pD0;
    char          padD4[0x3C];
    char          react[0x60];
    float         target_pos[4];
    char          pad180[0x30];
    Moby         *target;
    int           f1B4;
    char          pad1B8[8];
    float         home[4];
    float         glow_a[4];
    float         glow_b[4];
    float         wander_goal[4];
    float         f200;
    float         f204;
    float         f208;
    float         f20C;
    float         yaw_speed;
    short         voice;
    short         near_ground;
    Moby         *current_target;
    int           f21C;
    char          f220[0x10];
    char          sound_state[0x10];
} Data_3DF8;

typedef struct {
    float v[4];
} Vec_002d3878 __attribute__((aligned(16)));

typedef struct {
    Vec_002d3878 vertices[4];
    u32          color[4];
    float        uv[8];
    u64          tag[4];
} Packet_002d3878 __attribute__((aligned(16)));

typedef struct {
    float m[4][4];
    u32   col[4];
    float uv[8];
    u64   a, b, c, d;
} Pk;

extern int   FUN_L00_002dc9f0();
extern short D_L08_00161888 __attribute__((sda));
s32          is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
void         update_scoutbot_health_and_target(struct Moby *m) __asm__("FUN_L08_002d36e0");
void         draw_target_beam(struct Moby *m) __asm__("FUN_L08_002d3878");
void         update_scoutbot_movement(struct Moby *m) __asm__("FUN_L08_002d3d78");
void         draw_scoutbot_glow_quad(struct Moby *arg) __asm__("FUN_L08_002d4108");
extern void  sound_3DF8(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern void  wake_3DF8(void *) __asm__("FUN_L00_0025a120");
extern void  turn_3DF8(void *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern int   is_value_within_interpolated_window(void *, float) __asm__("FUN_00214cc8");
extern int   FUN_L00_002dc9f0(void *, void *);
extern void  explode_3DF8(void *,
                          void *,
                          void *,
                          float,
                          float,
                          int,
                          int,
                          int,
                          float,
                          float,
                          float,
                          int,
                          float,
                          float,
                          int,
                          int,
                          int,
                          int) __asm__("FUN_L00_0025e450");
extern int   colorlerp_3DF8(int, int, float) __asm__("FUN_001fa6e0");
extern void  draw_target_beam(struct Moby *) __asm__("FUN_L08_002d3878");
extern void  draw_scoutbot_glow_quad(struct Moby *) __asm__("FUN_L08_002d4108");
extern char  hero_pos_3DF8[] __asm__("D_L08_001675C0");

extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void FUN_L00_00257470(void *, int, int);
extern int
func_L00_00260FB0_f(char *, void *, float, int, int, void *, int) __asm__("FUN_L00_0025ff38");

extern void  scale_vector_002d3878(float, void *) __asm__("FUN_L00_0025f730");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L08_00161894_lui __asm__("D_L08_00161894") NOT_SDA;
extern u32   D_00151878_gp __asm__("D_00151878") __attribute__((sda));
extern u32   D_0015187C_gp __asm__("D_0015187C") __attribute__((sda));
extern u32   D_00151880_gp __asm__("D_00151880") __attribute__((sda));
extern u32   D_00151884_gp __asm__("D_00151884") __attribute__((sda));
void         draw_target_beam(struct Moby *moby) __asm__("FUN_L08_002d3878");

extern float scoutbot_probe_correction_target_position[] __asm__("D_L08_00174670");
extern int   FUN_L08_002025d0(void *, int, int, void *, int);

extern void *cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");

extern u8 D_L08_001675C0_lui[] __asm__("D_L08_001675C0") NOT_SDA;
void      FUN_001f7d30_002d4108(void *, s32, s32) __asm__("FUN_001f7d30");

extern float D_L08_001D1D80[4][4] NOT_SDA;

extern float scoutbot_glow_position_offset_scale_gp __asm__("D_L08_00161898") __attribute__((sda));
extern float scoutbot_glow_quad_vertex_scale_gp __asm__("D_L08_0016189C") __attribute__((sda));

/* Scoutbot update: hovers at its post, flies to its target and back, calls reinforcements, shoots, dies. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D3DF8), where it is exact; names translated to the US level program. */

void scoutbot_update(Moby *moby) __asm__("FUN_L08_002d2af0");

void scoutbot_update(Moby *moby) {
    enum {
        SCOUTBOT_STATE_INITIALIZE = 0,
        SCOUTBOT_STATE_IDLE = 1,
        SCOUTBOT_STATE_APPROACH_TARGET = 2,
        SCOUTBOT_STATE_RETURN_HOME = 3,
        SCOUTBOT_STATE_ATTACK = 4,
        SCOUTBOT_STATE_WAIT_FOR_VOICE = 6,
        SCOUTBOT_STATE_MOVE_TO_WANDER_GOAL = 7,
        SCOUTBOT_STATE_EXPLODE_AND_REMOVE = 0x63
    };
    Data_3DF8 *data = (struct Data_3DF8 *)moby->pvars;
    float      target_pos[4];
    float      offset[4];
    float      shot_pos[4];

    sound_3DF8(3.6f, moby, 2, data->sound_state);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, hero_pos_3DF8) < 28.0f) {
        wake_3DF8(moby);
        moby->shadow_range = 0x16;
    }
    update_scoutbot_health_and_target((char *)moby);
    data->current_target = data->target;
    qcopy(target_pos, data->target_pos);
    switch (moby->state) {
    case SCOUTBOT_STATE_INITIALIZE:
        if (data->f21C == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->flags |= 0x1000;
        moby->rot.z = random_angle_radians();
        data->f200 = 4.0f;
        data->f204 = random_angle_radians();
        data->f208 = 0.5f;
        clear_u64_value(data->vel);
        data->f20C = frame_time * 2.0943952f;
        data->b58 = truncate_float_to_s32(15.0f);
        data->b5A = truncate_float_to_s32(9.6f);
        data->pD0 = &D_L08_00161888;
        if (moby->prev_seq != 0) {
            blend_moby_animation(moby, 0, 0, 0);
        }
        moby->state = 1;
        qcopy(data->home, &moby->pos);
        if (probe_ground_height(&moby->pos, 0, 0.5f) > moby->pos.z - 10.0f) {
            data->home[2] = probe_ground_height(&moby->pos, 0, 0.5f);
            data->near_ground = 1;
        } else {
            data->home[2] = moby->pos.z;
            data->near_ground = 0;
        }
        qcopy(data->wander_goal, data->home);
        data->b29 = 0;
        break;
    case SCOUTBOT_STATE_IDLE:
        if (data->f1B4 != 2) {
            data->f200 = 0.0f;
            data->f208 = 0.1f;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, 6);
            }
            moby->state = 2;
        } else if (moby->anim_flags & 2) {
            float heading = random_angle_radians();
            float radius = random_float_between(0.0f, 3.0f);

            offset[0] = fast_cos(heading) * radius;
            offset[1] = fast_sin(heading) * radius;
            offset[2] = 0.0f;
            fast_vec_add(data->wander_goal, data->home, offset);
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, 6);
            }
            moby->state = 7;
        }
        break;
    case SCOUTBOT_STATE_MOVE_TO_WANDER_GOAL: {
        float heading =
            fast_arc_tan(data->wander_goal[0] - moby->pos.x, data->wander_goal[1] - moby->pos.y);

        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        if (fast_difference_between_rotations(moby->rot.z, heading) < 0.7853982f) {
            data->vel[0] += fast_cos(moby->rot.z) * (frame_time_sq * 12.0f);
            data->vel[1] += fast_sin(moby->rot.z) * (frame_time_sq * 12.0f);
        }
        if (vector_distance_xy(&moby->pos, data->wander_goal) < 0.5f) {
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 3);
            }
            moby->state = 1;
        }
        break;
    }
    case SCOUTBOT_STATE_APPROACH_TARGET: {
        float heading = fast_arc_tan(target_pos[0] - moby->pos.x, target_pos[1] - moby->pos.y);

        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        if (fast_difference_between_rotations(moby->rot.z, heading) < 0.7853982f) {
            data->vel[0] += fast_cos(moby->rot.z) * (frame_time_sq * 12.0f);
            data->vel[1] += fast_sin(moby->rot.z) * (frame_time_sq * 12.0f);
        }
        if (vector_distance_xy(&moby->pos, &data->current_target->pos) < 3.0f &&
            absolute_float(data->current_target->pos.z - moby->pos.z) < 2.0f) {
            data->voice = allocate_voice_for_target_entry(0, 0, moby);
            moby->state = 6;
        } else if (vector_distance_xy(target_pos, data->home) > 15.0f ||
                   absolute_float(data->home[2] - target_pos[2]) > 4.0f) {
            data->f200 = 4.0f;
            data->f208 = 0.5f;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, 6);
            }
            moby->state = 3;
        }
        break;
    }
    case SCOUTBOT_STATE_RETURN_HOME: {
        float heading = fast_arc_tan(data->home[0] - moby->pos.x, data->home[1] - moby->pos.y);

        data->f200 = 4.0f;
        data->f208 = 0.5f;
        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        if (fast_difference_between_rotations(moby->rot.z, heading) < 0.7853982f) {
            data->vel[0] += fast_cos(moby->rot.z) * (frame_time_sq * 12.0f);
            data->vel[1] += fast_sin(moby->rot.z) * (frame_time_sq * 12.0f);
        }
        if (vector_distance_xy(&moby->pos, data->home) < 1.0f &&
            absolute_float(data->home[2] - moby->pos.z) >= 3.9f) {
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 3);
            }
            moby->state = 1;
        }
        break;
    }
    case SCOUTBOT_STATE_ATTACK: {
        float heading = fast_arc_tan(target_pos[0] - moby->pos.x, target_pos[1] - moby->pos.y);
        float frame;

        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        frame = compute_interpolated_record_value(moby);
        if (moby->seq == moby->prev_seq && frame >= 9.0f && frame <= 14.0f) {
            float charge = (frame - 9.0f) / 5.0f;
            float spin;

            data->glow_a[3] = charge / 3.0f;
            data->glow_b[3] = (charge * 2.0f) / 3.0f;
            spin = random_angle_radians();
            data->glow_a[1] = fast_sin(spin) * 0.25f * charge;
            data->glow_a[2] = fast_cos(spin) * 0.25f * charge;
            data->glow_b[1] = fast_sin(spin) * 0.25f * charge;
            data->glow_b[2] = fast_cos(spin) * 0.25f * charge;
            add_draw_callback(draw_target_beam, moby);
        }
        if (is_value_within_interpolated_window(moby, 14.0f)) {
            if (vector_distance_xy(&data->target->pos, &moby->pos) < 3.0f &&
                absolute_float(data->current_target->pos.z - moby->pos.z) < 1.0f) {
                offset[0] = fast_cos(moby->rot.z) * 0.2f;
                offset[1] = fast_sin(moby->rot.z) * 0.2f;
                offset[2] = 0.0f;
                qcopy(shot_pos, target_pos);
                shot_pos[2] += 0.75f;
                spawn_nearby_effect(data->current_target, moby, 1, shot_pos, offset, 1.0f);
            }
            data->f200 = 4.0f;
            data->f208 = 0.5f;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, 3);
            }
            moby->state = 3;
        }
        break;
    }
    case SCOUTBOT_STATE_WAIT_FOR_VOICE: {
        float heading = fast_arc_tan(target_pos[0] - moby->pos.x, target_pos[1] - moby->pos.y);

        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        if (voice_is_playing(moby, data->voice) == 0) {
            if (moby->prev_seq != 3) {
                blend_moby_animation(moby, 3, 0, 3);
            }
            moby->state = 4;
        }
        break;
    }
    case 5:
        if (FUN_L00_002dc9f0(moby, data->react)) {
            data->f200 = 4.0f;
            data->f208 = 0.5f;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, 3);
            }
            data->hC8 = 0;
        }
        break;
    case SCOUTBOT_STATE_EXPLODE_AND_REMOVE:
        qcopy(offset, &moby->pos);
        offset[2] += 1.0f;
        allocate_voice_for_target_entry(1, 0, moby);
        explode_3DF8(moby,
                     data->vel,
                     offset,
                     0.0f,
                     0.0f,
                     5,
                     2,
                     4,
                     2.0f,
                     1.0f,
                     9.0f,
                     -1,
                     1.0f,
                     0.0f,
                     0,
                     1,
                     -1,
                     0);
        mark_moby_for_removal(moby);
        return;
    }
    update_scoutbot_movement((char *)moby);
    if (moby->state != 5 && (signed char)moby->state >= 0) {
        int col = 0x7F20FF20;

        if (moby->state == 4 || moby->state == 6) {
            col = 0x7F2828A0;
        }
        moby->glow_color = colorlerp_3DF8(moby->glow_color, col, 0.1f);
        moby_point_position(moby, 3, data->f220);
        add_draw_callback(draw_scoutbot_glow_quad, moby);
    }
}

/* Takes hits (dying at 0 health), then follows the moby's path and gives up on it when too far off. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D49E8), where it is exact; names translated to the US level program. */

void update_scoutbot_health_and_target(struct Moby *moby) {
    char *data = ((char *)moby->pvars);
    int   hit;
    float dmg;
    int   hit_reaction_code;
    dmg = 0.0f;
    hit_reaction_code =
        hit_reaction(moby, find_moby_hit(moby, 0x330000, 0), data + 0x20, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && ((unsigned char *)moby)[0x20] != 0x63) {
        *(float *)(data + 0x20) -= dmg;
        if (*(float *)(data + 0x20) <= 0.0f)
            hit_reaction_code = 1;
        if (hit_reaction_code < 12) {
            if (hit_reaction_code >= 0) {
                FUN_L00_00257470(moby, 0, -1);
                moby->state = 0x63;
            }
        }
    }
    ((unsigned char *)moby)[0xA4] = 0xFF;
    if (((unsigned char *)moby)[0x20] != 0 && ((unsigned char *)moby)[0x20] != 4) {
        char *path = ((char *)l08_paths[*(int *)(data + 0x21C)]);
        if (func_L00_00260FB0_f(moby, data + 0x170, 14.0f, 0, 0, path + 0x10, *(int *)path) != 2) {
            if (14.0f < vector_distance_xy(data + 0x1C0, data + 0x170)) {
                *(int *)(data + 0x1B4) = 2;
            } else if (3.0f < absolute_float(*(float *)(data + 0x1C8) - *(float *)(data + 0x178))) {
                *(int *)(data + 0x1B4) = 2;
            }
        }
    }
    if (*(int *)(data + 0x1B0) == 0) {
        *(int *)(data + 0x1B0) = ((int)hero.moby);
    }
}

void draw_target_beam(struct Moby *moby) {
    char           *data = ((char *)moby->pvars);
    Packet_002d3878 pk;
    Vec_002d3878    matrix[3], direction, pos, delta, angles;
    float           length;
    u64             tag;
    Vec_002d3878   *points;
    char           *source0;
    char           *source1;
    float          *vertex;
    int             i;
    tag = get_effect_texture(14);
    pk.color[0] = D_00151878_gp;
    pk.color[1] = D_0015187C_gp;
    pk.color[2] = D_00151880_gp;
    pk.color[3] = D_00151884_gp;
    pk.tag[1] = tag;
    pk.tag[2] = 0xff9000000260ULL;
    pk.tag[0] = 5;
    pk.tag[3] = 0x8000000048ULL;
    pk.uv[0] = 0.0f;
    pk.uv[1] = 0.0f;
    pk.uv[2] = 1.0f;
    pk.uv[3] = 0.0f;
    pk.uv[4] = 0.0f;
    pk.uv[5] = 1.0f;
    pk.uv[6] = 1.0f;
    pk.uv[7] = 1.0f;
    moby_point_position(moby, 2, &pos);
    fast_vec_sub(&delta, *(char **)(data + 0x218) + 0x10, &pos);
    scale_vector_002d3878(3.0f, &delta);
    delta.v[2] = (*(float *)(*(char **)(data + 0x218) + 0x18) + 0.25f) - pos.v[2];
    delta.v[3] = 1.0f;
    length = fast_vec_length(&delta);
    clear_u64_value(&angles);
    angles.v[1] = -fast_arc_tan(vector_length_xy(&delta), delta.v[2]);
    angles.v[2] = fast_arc_tan(delta.v[0], delta.v[1]);
    euler_rotation_matrix(matrix, &angles);
    qcopy(&direction, &pos);
    direction.v[3] = 1.0f;

    clear_u64_value(&pk.vertices[0]);
    pk.vertices[0].v[2] = D_L08_00161894_lui;
    pk.vertices[0].v[3] = 1.0f;
    qcopy(&pk.vertices[1], data + 0x1d0);
    pk.vertices[1].v[0] = length * *(float *)(data + 0x1dc);
    pk.vertices[1].v[2] += -0.25f;
    pk.vertices[1].v[3] = 1.0f;
    clear_u64_value(&pk.vertices[2]);
    pk.vertices[2].v[2] = 0.0f;
    pk.vertices[2].v[3] = 1.0f;
    qcopy(&pk.vertices[3], data + 0x1d0);
    pk.vertices[3].v[0] = length * *(float *)(data + 0x1dc);
    pk.vertices[3].v[2] += 0.25f;
    pk.vertices[3].v[3] = 1.0f;
    draw_geometry_quad(pk.vertices, matrix, 1);
    {
        for (i = 1; i < 4; i += 2) {
            if (i < 2) {
                pk.vertices[i].v[1] += -0.25f;
                pk.vertices[i].v[2] += 0.25f;
            } else {
                pk.vertices[i].v[1] += 0.25f;
                pk.vertices[i].v[2] += -0.25f;
            }
        }
    }
    draw_geometry_quad(pk.vertices, matrix, 1);

    qcopy(&pk.vertices[0], data + 0x1d0);
    pk.vertices[0].v[0] = length * *(float *)(data + 0x1dc);
    pk.vertices[0].v[2] += -0.25f;
    pk.vertices[0].v[3] = 1.0f;
    qcopy(&pk.vertices[1], data + 0x1e0);
    pk.vertices[1].v[0] = length * *(float *)(data + 0x1ec);
    pk.vertices[1].v[2] += -0.25f;
    pk.vertices[1].v[3] = 1.0f;
    qcopy(&pk.vertices[2], data + 0x1d0);
    pk.vertices[2].v[0] = length * *(float *)(data + 0x1dc);
    pk.vertices[2].v[2] += 0.25f;
    pk.vertices[2].v[3] = 1.0f;
    qcopy(&pk.vertices[3], data + 0x1e0);
    pk.vertices[3].v[0] = length * *(float *)(data + 0x1ec);
    pk.vertices[3].v[2] += 0.25f;
    pk.vertices[3].v[3] = 1.0f;
    draw_geometry_quad(pk.vertices, matrix, 1);
    {
        float *vertex;
        for (i = 0, vertex = pk.vertices[0].v; i < 4; i++, vertex += 4) {
            if (i < 2) {
                vertex[1] += -0.25f;
                vertex[2] += 0.25f;
            } else {
                vertex[1] += 0.25f;
                vertex[2] += -0.25f;
            }
        }
    }
    draw_geometry_quad(pk.vertices, matrix, 1);

    qcopy(&pk.vertices[0], data + 0x1e0);
    pk.vertices[0].v[0] = length * *(float *)(data + 0x1ec);
    pk.vertices[0].v[2] += -0.25f;
    pk.vertices[0].v[3] = 1.0f;
    clear_u64_value(&pk.vertices[1]);
    pk.vertices[1].v[0] = length * *(float *)(data + 0x1dc) * 3.0f;
    pk.vertices[1].v[2] += D_L08_00161894_lui;
    pk.vertices[1].v[3] = 1.0f;
    qcopy(&pk.vertices[2], data + 0x1e0);
    pk.vertices[2].v[0] = length * *(float *)(data + 0x1ec);
    pk.vertices[2].v[2] += 0.25f;
    pk.vertices[2].v[3] = 1.0f;
    clear_u64_value(&pk.vertices[3]);
    pk.vertices[3].v[0] = length * *(float *)(data + 0x1dc) * 3.0f;
    pk.vertices[3].v[2] = 0.0f;
    pk.vertices[3].v[3] = 1.0f;
    draw_geometry_quad(pk.vertices, matrix, 1);
    {
        float *vertex;
        for (i = 0, vertex = pk.vertices[0].v; i < 4; i += 2, vertex += 8) {
            if (i < 2) {
                vertex[1] += -0.25f;
                vertex[2] += 0.25f;
            } else {
                vertex[1] += 0.25f;
                vertex[2] += -0.25f;
            }
        }
    }
    draw_geometry_quad(pk.vertices, matrix, 1);
}

/* Steers a moby along its path state and lifts it clear of the ground. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D5080), where it is exact; names translated to the US level program. */

void update_scoutbot_movement(struct Moby *moby) {
    char  *data = ((char *)moby->pvars);
    float  movement_length = fast_vec_length((float *)(data + 0x40)) - frame_time_sq * 6.0f;
    float  movement_length_limit = frame_time * 3.0f;
    float  target_height;
    float  vertical_step;
    float  probe_position_or_correction[4];
    float *velocity_ptr;
    if (movement_length_limit < movement_length) {
        movement_length = movement_length_limit;
    } else if (movement_length < 0.0f) {
        movement_length = 0.0f;
    }
    velocity_ptr = (float *)(data + 0x40);
    fast_vec_normalize(velocity_ptr, velocity_ptr, movement_length);
    fast_vec_add((float *)&moby->pos, (float *)&moby->pos, velocity_ptr);
    *(float *)(data + 0x204) =
        fast_add_rotations(*(float *)(data + 0x204), *(float *)(data + 0x20C));
    target_height =
        *(float *)(data + 0x1C8) + *(float *)(data + 0x208) * fast_sin(*(float *)(data + 0x204));
    if (*(short *)(data + 0x216))
        target_height += *(float *)(data + 0x200);
    vertical_step = target_height - moby->pos.z;
    {
        float vertical_step_limit = frame_time * 3.0f;
        if (vertical_step_limit < vertical_step) {
            vertical_step = vertical_step_limit;
        } else {
            vertical_step_limit = -vertical_step_limit;
            if (vertical_step < vertical_step_limit)
                vertical_step = vertical_step_limit;
        }
    }
    *(float *)(data + 0x48) = vertical_step;
    qcopy(probe_position_or_correction, (float *)&moby->pos);
    probe_position_or_correction[2] += 0.5f;
    if (FUN_L08_002025d0(probe_position_or_correction, 0x190, 0, moby, 0)) {
        float z;
        fast_vec_sub(probe_position_or_correction,
                     scoutbot_probe_correction_target_position,
                     (float *)&moby->pos);
        z = frame_time * 3.0f;
        if (z < probe_position_or_correction[2]) {
            probe_position_or_correction[2] = z;
        } else if (probe_position_or_correction[2] < -z) {
            probe_position_or_correction[2] = -z;
        }
        fast_vec_add((float *)&moby->pos, (float *)&moby->pos, probe_position_or_correction);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D5358), where it is exact; names translated to the US level program. */

void *enter_scoutbot_return_home_state(struct Moby *moby) __asm__("FUN_L08_002d4050");

void *enter_scoutbot_return_home_state(struct Moby *moby) {
    void *result = cancel_suck_cannon_suction(moby);
    char *scoutbot_data = ((char *)moby->pvars);
    moby->state = 3;
    *(float *)(scoutbot_data + 0x200) = 4.0f;
    *(float *)(scoutbot_data + 0x208) = 0.5f;
    if ((unsigned char)((char)moby->prev_seq) != 2) {
        blend_moby_animation(moby, 2, 0, 3);
    }
    *(short *)(scoutbot_data + 0xC8) = 0;
    return result;
}

void draw_scoutbot_glow_quad(struct Moby *moby) {
    char   *data = ((char *)moby->pvars);
    Pk      pk;
    float   up[4];
    float   M[4][4];
    float   position_offset[4];
    s32     i;
    OvlQuad z = 0;
    u32     col;

    *(OvlQuad *)up = z;
    up[2] = 1.0f;
    up[3] = 1.0f;
    pk.b = get_effect_texture(0xB);
    pk.d = 0x8000000048ULL;
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 5;
    pk.uv[0] = 0.0f;
    pk.uv[1] = 0.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 1.0f;
    pk.uv[7] = 1.0f;
    qcopy(M[3], (float *)(data + 0x220));
    fast_vec_sub(M[0], D_L08_001675C0_lui, M[3]);
    fast_vec_normalize(M[0], M[0], 1.0f);
    fast_vec_cross(M[1], M[0], up);
    fast_vec_normalize(M[1], M[1], 1.0f);
    fast_vec_cross(M[2], M[1], M[0]);
    fast_vec_scale(position_offset, M[0], scoutbot_glow_position_offset_scale_gp);
    fast_vec_add(M[3], M[3], position_offset);
    col = (((u32)moby->glow_color) & 0xFFFFFF) | 0x40000000;
    pk.col[3] = col;
    pk.col[2] = col;
    pk.col[1] = col;
    pk.col[0] = col;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(pk.m[i], D_L08_001D1D80[i], scoutbot_glow_quad_vertex_scale_gp);
        transform_vector(pk.m[i], pk.m[i], M);
    }
    FUN_001f7d30_002d4108(&pk, 0, 0);
}

short D_L14_00161A78 = 0x202;
float D_L14_00161A84 = -0.0f;

float D_L14_001DE580[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

float D_L14_00161A88 = 0.2f;
float D_L14_00161A8C = 0.175f;

float D_L08_001D1D80[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

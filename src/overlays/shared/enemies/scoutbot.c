/* Shared moby classes: scoutbot, rocket, blarg_paratrooper. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"

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
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/math/vectors/compute_transformed_delta_component.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/rendering/commands/set_moby_color.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/game/fun_00213928.h"
#include "rnc/overlay/level_path.h"

/* Scoutbot update: hovers at its post, flies to its target and back, calls reinforcements, shoots, dies. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D3DF8), where it is exact; names translated to the US level program. */

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

extern char  D_L08_0016D2E8[];
extern float D_0015ED6C;
extern float D_0015ED70;
extern int   fast_tween_color() __asm__("FUN_001fa6e0");
extern int   FUN_L00_0024f7c8();
extern int   FUN_L00_002dc9f0();
extern short D_L08_00161888 __attribute__((sda));
s32          is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
void         FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
void         FUN_L00_0025a120(char *o);
void         spawn_explosion(f32   burst,
                             f32   unused_float,
                             f32   owner_effect,
                             f32   owner_effect2,
                             f32   cutoff,
                             f32   scale,
                             f32   flash,
                             void *owner,
                             s32   unused2,
                             void *pos,
                             s32   puff_count,
                             s32   spark_count,
                             s32   shard_count,
                             s32   sound,
                             s32   shake,
                             s32   spray_count,
                             s32   tint,
                             u8    alternate) __asm__("FUN_L00_0025e450");
void         FUN_L00_00263ac8(float x, int a, int b, unsigned char *p);
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

extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int  FUN_L00_0025ff38();
extern void FUN_L00_00257470(void *, int, int);
extern int
func_L00_00260FB0_f(char *, void *, float, int, int, void *, int) __asm__("FUN_L00_0025ff38");

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

typedef struct {
    float v[4];
} Vec_002d3878 __attribute__((aligned(16)));

typedef struct {
    Vec_002d3878 vertices[4];
    u32          color[4];
    float        uv[8];
    u64          tag[4];
} Packet_002d3878 __attribute__((aligned(16)));

extern void  scale_vector_002d3878(float, void *) __asm__("FUN_L00_0025f730");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L08_00161894_lui __asm__("D_L08_00161894") NOT_SDA;
extern u32   D_00151878_gp __asm__("D_00151878") __attribute__((sda));
extern u32   D_0015187C_gp __asm__("D_0015187C") __attribute__((sda));
extern u32   D_00151880_gp __asm__("D_00151880") __attribute__((sda));
extern u32   D_00151884_gp __asm__("D_00151884") __attribute__((sda));
void         draw_target_beam(struct Moby *moby) __asm__("FUN_L08_002d3878");

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

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Steers a moby along its path state and lifts it clear of the ground. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D5080), where it is exact; names translated to the US level program. */

extern float scoutbot_probe_correction_target_position[] __asm__("D_L08_00174670");
extern int   FUN_L08_002025d0(void *, int, int, void *, int);

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

extern void *cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");

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

typedef struct {
    float m[4][4];
    u32   col[4];
    float uv[8];
    u64   a, b, c, d;
} Pk;

extern u8 D_L08_001675C0_lui[] __asm__("D_L08_001675C0") NOT_SDA;
void      FUN_001f7d30_002d4108(void *, s32, s32) __asm__("FUN_001f7d30");

extern float D_L08_001D1D80[4][4] NOT_SDA;

#include "rnc/overlay/shared/enemies/scoutbot_globals.h"

extern float scoutbot_glow_position_offset_scale_gp __asm__("D_L08_00161898") __attribute__((sda));
extern float scoutbot_glow_quad_vertex_scale_gp __asm__("D_L08_0016189C") __attribute__((sda));

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

extern void *spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern float FUN_L08_00211a20(float);
extern void
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int D_L08_0015F5CC __asm__("D_L08_0015F5CC");

typedef union {
    float float_value;
    int   int_value;
} Data;

/* Retail inherits GP 0x166c00: these scalar labels preserve its GP-relative loads.
   Keep the existing timer name while binding it to the checked level-08 address. */
extern int D_L08_00161D08_gp __asm__("D_L08_00161D08") __attribute__((sda));
extern int D_L08_00161D0C_gp __asm__("D_L08_00161D0C") __attribute__((sda));
extern int D_L08_00161D10_gp __asm__("D_L08_00161D10") __attribute__((sda));

typedef struct {
    unsigned int words[4];
} Vec_002e7eb0 __attribute__((aligned(16)));

extern float moby_creation_default_timer_frames_gp __asm__("D_L08_00161CD8") __attribute__((sda));
extern float moby_creation_particle_count_limit_gp __asm__("D_L08_00161CE0") __attribute__((sda));
extern int   moby_creation_initial_spark_word_gp __asm__("D_L08_00161CF0") __attribute__((sda));
extern int   moby_creation_second_spark_word_gp __asm__("D_L08_00161CF4") __attribute__((sda));
extern int   moby_creation_initial_particle_word_gp __asm__("D_L08_00161CF8") __attribute__((sda));
extern int   moby_creation_second_particle_word_gp __asm__("D_L08_00161CFC") __attribute__((sda));
extern float moby_creation_particle_spread_speed_base_gp __asm__("D_L08_00161D04")
    __attribute__((sda));

#include "rnc/overlay/shared/enemies/scoutbot_data/create_moby_with_particles.h"

char *create_moby_with_particles(int    owner,
                                 float *position,
                                 float *rotation,
                                 int    value) __asm__("FUN_L08_002e7eb0");

char *create_moby_with_particles(int owner, float *position, float *rotation, int value) {
    float position_copy[4] __attribute__((aligned(16)));
    float rotation_copy[4] __attribute__((aligned(16)));
    float matrix[4][4] __attribute__((aligned(16)));
    float velocity[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    float angle, magnitude;
    int   i, particle_frame_count, particle_second_frame_count, particle_third_frame_count;
    char *moby;
    char *data;
    float *volatile particle_position_ptr;
    float *velocity_ptr, *offset_ptr, *matrix_row_ptr;

    *(u128 *)position_copy = *(u128 *)position;
    *(u128 *)rotation_copy = *(u128 *)rotation;
    position = position_copy;
    rotation = rotation_copy;
    moby = create_moby(0x1ca);
    if (moby != 0) {
        data = ((char *)MOBY(moby)->pvars);
        *(int *)(data + 0x14) = owner;
        *(int *)(data + 0x10) = value;
        set_moby_color(moby, 0xa0, 0xa0, 0xa0);
        *(volatile int *)(moby + 0x40) = 0;
        MOBY(moby)->update_dist = 0xff;
        MOBY(moby)->draw_dist = 0xff;
        MOBY(moby)->visible = 1;
        MOBY(moby)->rot.y = -fast_arc_tan(vector_length_xy(rotation_copy), rotation[2]);
        MOBY(moby)->rot.z = fast_arc_tan(rotation[0], rotation[1]);
        qcopy(moby + 0x10, position_copy);
        qcopy(data, rotation_copy);
        if (D_L08_0015F5CC & 1)
            *(volatile int *)(data + 0x1c) = 1;
        else
            *(volatile int *)(data + 0x1c) = -1;
        euler_rotation_matrix(matrix, moby + 0x40);
        i = 0;
        if ((float)i < moby_creation_particle_count_limit_gp) {
            /* Retail intentionally uses the matrix third row as the particle input. */
            particle_position_ptr = matrix[2];
            velocity_ptr = velocity;
            offset_ptr = offset;
            matrix_row_ptr = matrix[1];
            do {
                angle = random_angle_radians();
                {
                    float spread = moby_creation_particle_spread_speed_base_gp;
                    magnitude = random_float_between(spread, spread + spread) * frame_time;
                }
                fast_vec_normalize(
                    velocity_ptr, particle_position_ptr, magnitude * fast_cos(angle));
                fast_vec_normalize(offset_ptr, matrix_row_ptr, magnitude * fast_sin(angle));
                fast_vec_add(velocity_ptr, velocity_ptr, offset_ptr);
                qcopy(offset_ptr, velocity_ptr);
                velocity[3] = moby_creation_particle_primary_motion_w_gp;
                offset[3] = moby_creation_particle_secondary_motion_w_gp;
                i++;
                spawn_burst(moby + 0x10,
                            velocity_ptr,
                            moby_creation_initial_particle_word_gp,
                            moby_creation_second_particle_word_gp,
                            random_int_between(scale_game_frames(10), scale_game_frames(20)),
                            1,
                            20000.0f);
                particle_frame_count = truncate_float_to_s32(FUN_L08_00211a20(random_float_between(
                    (float)D_L08_00161D08_gp, (float)D_L08_00161D08_gp * 1.2f)));
                particle_second_frame_count =
                    truncate_float_to_s32(FUN_L08_00211a20(random_float_between(
                        (float)D_L08_00161D0C_gp, (float)D_L08_00161D0C_gp * 1.2f)));
                particle_third_frame_count =
                    truncate_float_to_s32(FUN_L08_00211a20(random_float_between(
                        (float)D_L08_00161D10_gp, (float)D_L08_00161D10_gp * 1.5f)));
                spawn_spark(moby + 0x10,
                            velocity_ptr,
                            offset_ptr,
                            moby_creation_initial_spark_word_gp,
                            moby_creation_second_spark_word_gp,
                            particle_frame_count,
                            particle_second_frame_count,
                            particle_third_frame_count,
                            -1);
            } while ((float)i < moby_creation_particle_count_limit_gp);
        }
        if (current_level_index == 12)
            *(int *)(data + 0x18) = scale_game_frames(120);
        else
            *(int *)(data + 0x18) =
                truncate_float_to_s32(FUN_L08_00211a20(moby_creation_default_timer_frames_gp));
        update_entity(moby);
    }
    return moby;
}

extern float scale_game_frames_002e8210(float) __asm__("FUN_L08_00211a20");
extern void  scale_vector_in_place_002e8210(float, void *) __asm__("FUN_L00_0025f730");
extern void  FUN_L00_0025e450_002e8210(void *,
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
extern float homing_projectile_trail_particle_count_limit_gp __asm__("D_L08_00161CDC")
    __attribute__((sda));
extern float homing_projectile_spin_rate_degrees_gp __asm__("D_L08_00161CE4") __attribute__((sda));
extern int   homing_projectile_trail_initial_spark_word_gp __asm__("D_L08_00161CE8")
    __attribute__((sda));
extern int homing_projectile_trail_second_spark_word_gp __asm__("D_L08_00161CEC")
    __attribute__((sda));
extern float homing_projectile_trail_spread_speed_limit_gp __asm__("D_L08_00161D00")
    __attribute__((sda));

#include "rnc/overlay/shared/enemies/scoutbot_data/update_homing_projectile.h"

typedef struct {
    unsigned int words[4];
} Vec_002e8210 __attribute__((aligned(16)));

extern float D_L08_00174660[4];
void         update_homing_projectile(char *moby) __asm__("FUN_L08_002e8210");

void update_homing_projectile(char *moby) {
    char *position_ptr = moby + 0x10;
    char *data = ((char *)MOBY(moby)->pvars);
    float position_copy[4] __attribute__((aligned(16)));
    float position[4] __attribute__((aligned(16)));
    float direction[4] __attribute__((aligned(16)));
    float velocity[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    float angle, magnitude, length, radius, spread;
    int   i, particle_frame_count, particle_second_frame_count, particle_third_frame_count;
    int  *timer_ptr;
    char *target;

    qcopy(position_copy, position_ptr);
    fast_vec_add(position_ptr, position_ptr, data);
    if (MOBY(moby)->pos.x < 10.0f || MOBY(moby)->pos.y < 10.0f || MOBY(moby)->pos.z < 10.0f ||
        500.0f < MOBY(moby)->pos.x || 500.0f < MOBY(moby)->pos.y || 500.0f < MOBY(moby)->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    fast_vec_scale(direction, data, -1.0f / homing_projectile_trail_particle_count_limit_gp);
    qcopy(position, position_ptr);
    i = 0;
    timer_ptr = (int *)(data + 0x18);
    while ((float)i < homing_projectile_trail_particle_count_limit_gp) {
        fast_vec_add(position, position, direction);
        angle = random_angle_radians();
        spread = homing_projectile_trail_spread_speed_limit_gp;
        magnitude = random_float_between(0.0f, spread) * frame_time;
        fast_vec_normalize(velocity, moby + 0xe0, magnitude * fast_cos(angle));
        fast_vec_normalize(offset, moby + 0xd0, magnitude * fast_sin(angle));
        fast_vec_add(velocity, velocity, offset);
        qcopy(offset, offset);
        velocity[3] = homing_projectile_trail_primary_motion_w_gp;
        offset[3] = homing_projectile_trail_secondary_motion_w_gp;
        i++;
        particle_frame_count = truncate_float_to_s32(scale_game_frames_002e8210(
            random_float_between((float)D_L08_00161D08_gp, (float)D_L08_00161D08_gp * 1.2f)));
        particle_second_frame_count = truncate_float_to_s32(scale_game_frames_002e8210(
            random_float_between((float)D_L08_00161D0C_gp, (float)D_L08_00161D0C_gp * 1.2f)));
        particle_third_frame_count = truncate_float_to_s32(scale_game_frames_002e8210(
            random_float_between((float)D_L08_00161D10_gp, (float)D_L08_00161D10_gp * 1.5f)));
        spawn_spark(position,
                    velocity,
                    offset,
                    homing_projectile_trail_initial_spark_word_gp,
                    homing_projectile_trail_second_spark_word_gp,
                    particle_frame_count,
                    particle_second_frame_count,
                    particle_third_frame_count,
                    -1);
    }
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x,
                                           homing_projectile_spin_rate_degrees_gp * 0.017453292f *
                                               frame_time * (float)*(int *)(data + 0x1c));
    target = *(char **)(data + 0x10);
    if (target != 0) {
        if ((unsigned char)target[0x20] == 0xfe)
            goto clear_target;
        if ((unsigned char)target[0x20] == 0xfd)
            goto clear_target;
        if (*(short *)(target + 0xa6) != 0x1b3)
            goto clear_target;
        {
            char *target_data = *(char **)(target + 0x78);
            length = fast_vec_length(data);
            spread = fast_vec_dist(moby + 0x10, *(char **)(data + 0x10) + 0x10);
            fast_vec_scale(position, target_data + 0x20, spread / length);
            fast_vec_add(position, position, *(char **)(data + 0x10) + 0x10);
            fast_vec_sub(direction, position, moby + 0x10);
            fast_vec_normalize(direction, direction, length);
            fast_vec_sub(velocity, direction, data);
            scale_vector_in_place_002e8210(frame_time * 3.7f, velocity);
            fast_vec_add(data, data, velocity);
            fast_vec_normalize(data, data, length);
        }
        goto target_done;
    clear_target:
        *(int *)(data + 0x10) = 0;
    }
target_done:
    ((int *)direction)[1] = 0x50001;
    ((int *)direction)[0] = (int)moby;
    direction[3] = 2.0f;
    ((int *)velocity)[0] = 1;
    qcopy(position, data);
    if (fast_dec_timer(timer_ptr) == 0)
        goto collision;
remove:
    mark_moby_for_removal(moby);
    return;
collision:
    if (collision_line(position_copy, moby + 0x10, 0, moby, position) == 0)
        return;
    qcopy(moby + 0x10, D_L08_00174660);
    if (15.0f < MOBY(moby)->pos.x && 15.0f < MOBY(moby)->pos.y && 15.0f < MOBY(moby)->pos.z) {
        radius = 24.0f;
        if (MOBY(moby)->pos.x - 2.0f < radius)
            radius = MOBY(moby)->pos.x - 2.0f;
        if (MOBY(moby)->pos.y - 2.0f < radius)
            radius = MOBY(moby)->pos.y - 2.0f;
        if (MOBY(moby)->pos.z - 2.0f < radius)
            radius = MOBY(moby)->pos.z - 2.0f;
        FUN_L00_0025e450_002e8210(moby,
                                  data,
                                  moby + 0x10,
                                  3.0f,
                                  3.0f,
                                  0,
                                  10,
                                  20,
                                  5.0f,
                                  3.0f,
                                  9.0f,
                                  -1,
                                  1.0f,
                                  radius,
                                  1,
                                  1,
                                  -1,
                                  0);
    }
    mark_moby_for_removal(moby);
}

/* Runs the two per-entry update calls over a 13-entry table. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2000), where it is exact; names translated to the US level program. */

extern Pair D_L08_001E8348[];
extern char D_L08_001E8360[];
extern int  D_L08_001E2858[];
extern int  scrolled_mesh_position_streams[] __asm__("D_L08_001E8268");
extern int  scrolled_mesh_uv_source_streams[] __asm__("D_L08_001E82A0");
extern int  D_L08_001E8310[];
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L08_00258090(void *, int, int, void *);

void draw_scrolled_mesh_batch(int pair_index) __asm__("FUN_L08_002f0b40");

void draw_scrolled_mesh_batch(int pair_index) {
    int i;
    for (i = 0; i < 13; i++) {
        FUN_L08_00258090(D_L08_001E8360,
                         scrolled_mesh_uv_source_streams[i],
                         D_L08_001E2858[i],
                         &D_L08_001E8348[pair_index]);
        FUN_L00_001fde98(D_L08_001E2858[i],
                         scrolled_mesh_position_streams[i],
                         D_L08_001E8310[i],
                         D_L08_001E8360,
                         1);
    }
}

extern Pair D_L08_00161E10[] MACRO_ADDR;
extern int  D_L08_001DDF40[];
extern int  eleven_entry_scrolled_base_uv_tables[] __asm__("D_L08_001E27C8");
extern int  eleven_entry_scrolled_vertex_position_tables[] __asm__("D_L08_001E2798");
extern int  D_L08_001E2828[];
void draw_eleven_entry_scrolled_texture_batch(int scroll_pair_index) __asm__("FUN_L08_002f0c18");

void draw_eleven_entry_scrolled_texture_batch(int scroll_pair_index) {
    int i;
    for (i = 0; i < 11; i++) {
        FUN_L08_00258090(D_L08_001E8360,
                         eleven_entry_scrolled_base_uv_tables[i],
                         D_L08_001DDF40[i],
                         &D_L08_00161E10[scroll_pair_index]);
        FUN_L00_001fde98(D_L08_001DDF40[i],
                         eleven_entry_scrolled_vertex_position_tables[i],
                         D_L08_001E2828[i],
                         D_L08_001E8360,
                         1);
    }
}

/* Scales every RGBA word of a set of colour tables by four factors, optionally using the flat 255 for colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2428), where it is exact; names translated to the US level program. */

void scale_rgba_tables(int **tabs,
                       int   table_count,
                       int  *counts,
                       int   flat,
                       float r,
                       float g,
                       float b,
                       float alpha_scale) __asm__("FUN_L08_002f0f68");

void scale_rgba_tables(int **tabs,
                       int   table_count,
                       int  *counts,
                       int   flat,
                       float r,
                       float g,
                       float b,

                       float alpha_scale) {
    int i, j;
    for (i = 0; i < table_count; i++) {
        for (j = 0; j < counts[i]; j++) {
            int   packed_rgba = tabs[i][j];
            int   red_channel = packed_rgba & 0xFF;
            int   green_channel = (packed_rgba & 0xFF00) >> 8;
            int   blue_channel = ((unsigned)packed_rgba >> 16) & 0xFF;
            int   alpha_channel = (unsigned)packed_rgba >> 24;
            float fr, fg, fb, fa;
            if (flat == 0) {
                fr = r * convert_integer_to_float(red_channel);
                fg = g * convert_integer_to_float(green_channel);
                fb = b * convert_integer_to_float(blue_channel);
                fa = alpha_scale * convert_integer_to_float(alpha_channel);
            } else {
                fr = r * 255.0f;
                fg = g * 255.0f;
                fb = b * 255.0f;
                fa = alpha_scale * convert_integer_to_float(alpha_channel);
            }
            tabs[i][j] = (truncate_float_to_s32(fa) << 24) + (truncate_float_to_s32(fb) << 16) +
                         (truncate_float_to_s32(fg) << 8) + truncate_float_to_s32(fr);
        }
    }
}
/* advance a 2-D wrapped phase by a and b and write scaled sines */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2618), where it is exact; names translated to the US level program. */

extern float frame_delta_time __asm__("D_0015ED7C");

void advance_wrapped_sine_pair(float *out,
                               float *ph,
                               float  first_phase_rate,
                               float  second_phase_rate,
                               float  scale) __asm__("FUN_L08_002f1158");

void advance_wrapped_sine_pair(

    float *out, float *ph, float first_phase_rate, float second_phase_rate, float scale) {
    ph[0] += first_phase_rate * frame_delta_time;
    if (ph[0] > 1.0f) {
        ph[0] -= 2.0f;
    }
    if (ph[0] < -1.0f) {
        ph[0] += 2.0f;
    }
    ph[1] += second_phase_rate * frame_delta_time;
    if (ph[1] > 1.0f) {
        ph[1] -= 2.0f;
    }
    if (ph[1] < -1.0f) {
        ph[1] += 2.0f;
    }
    out[0] = fast_sin(ph[0] * 3.1415927f) * scale;
    out[1] = fast_sin(ph[1] * 3.1415927f) * scale;
}

typedef struct {
    float x;
    float y;
} V2_12a0;

extern V2_12a0 D_L08_001E8348_12a0[] __asm__("D_L08_001E8348");

extern V2_12a0 l08_scroll_pair_rates[] __asm__("D_L08_001DB500");
void           advance_scroll_offsets(int scroll_pair_index) __asm__("FUN_L08_002f12a0");

void advance_scroll_offsets(int scroll_pair_index) {
    D_L08_001E8348_12a0[scroll_pair_index].x +=
        l08_scroll_pair_rates[scroll_pair_index].x * frame_delta_time;
    if (D_L08_001E8348_12a0[scroll_pair_index].x > 1.0f) {
        D_L08_001E8348_12a0[scroll_pair_index].x -= 1.0f;
    }
    if (D_L08_001E8348_12a0[scroll_pair_index].x < -1.0f) {
        D_L08_001E8348_12a0[scroll_pair_index].x += 1.0f;
    }
    D_L08_001E8348_12a0[scroll_pair_index].y +=
        l08_scroll_pair_rates[scroll_pair_index].y * frame_delta_time;
    if (D_L08_001E8348_12a0[scroll_pair_index].y > 1.0f) {
        D_L08_001E8348_12a0[scroll_pair_index].y -= 1.0f;
    }
    if (D_L08_001E8348_12a0[scroll_pair_index].y < -1.0f) {
        D_L08_001E8348_12a0[scroll_pair_index].y += 1.0f;
    }
}

extern float scroll_pos_x[] __asm__("D_L08_00161E10") MACRO_ADDR;
extern float scroll_pos_y[] __asm__("D_L08_00161E14") MACRO_ADDR;

extern float scroll_entry_x_rates_gp[2] __asm__("D_L08_00161DE0") __attribute__((sda));
extern float D_L08_00161DE4[] MACRO_ADDR;
/* Advances the X and Y scroll offsets of entry `i` by speed * frame step, wrapping them into [-1, 1]. */
void advance_scroll_entry(int i) __asm__("FUN_L08_002f1378");

void advance_scroll_entry(int i) {
    float *scroll_offset_ptr;
    scroll_offset_ptr = &scroll_pos_x[i * 2];
    *scroll_offset_ptr += scroll_entry_x_rates_gp[i * 2] * frame_delta_time;
    if (*scroll_offset_ptr > 1.0f)
        *scroll_offset_ptr -= 1.0f;
    if (*scroll_offset_ptr < -1.0f)
        *scroll_offset_ptr += 1.0f;
    scroll_offset_ptr = &scroll_pos_y[i * 2];
    *scroll_offset_ptr += D_L08_00161DE4[i * 2] * frame_delta_time;
    if (*scroll_offset_ptr > 1.0f)
        *scroll_offset_ptr -= 1.0f;
    if (*scroll_offset_ptr < -1.0f)
        *scroll_offset_ptr += 1.0f;
}

extern float scale_time(float) __asm__("FUN_001f96b0");

extern s32 spark_emitter_default_initial_spark_word_gp __asm__("D_L08_00161F74")
    __attribute__((sda));
extern s32 spark_emitter_default_second_spark_word_gp __asm__("D_L08_00161F78")
    __attribute__((sda));
extern f32 spark_emitter_default_primary_z_increment_base_gp __asm__("D_L08_00161F7C")
    __attribute__((sda));
extern f32 spark_emitter_default_primary_magnitude_base_gp __asm__("D_L08_00161F80")
    __attribute__((sda));
extern f32 spark_emitter_default_secondary_z_increment_base_gp __asm__("D_L08_00161F84")
    __attribute__((sda));
extern f32 spark_emitter_default_secondary_magnitude_base_gp __asm__("D_L08_00161F88")
    __attribute__((sda));

#include "rnc/overlay/shared/enemies/scoutbot_data/update_spark_emitter_2.h"

extern s32 spark_emitter_default_timer_reset_gp __asm__("D_L08_00161F98") __attribute__((sda));

#include "rnc/overlay/shared/enemies/scoutbot_data/update_spark_emitter.h"

extern f32 spark_emitter_default_primary_spread_limit_gp __asm__("D_L08_00161FA4")
    __attribute__((sda));
extern f32 spark_emitter_default_secondary_spread_limit_gp __asm__("D_L08_00161FA8")
    __attribute__((sda));

void update_spark_emitter(char *moby) __asm__("FUN_L08_002f5830");

void update_spark_emitter(char *moby) {
    f32 spark_motion_primary[4], spark_motion_secondary[4], spark_position[4],
        spark_motion_offset[4], spark_rotation_matrix[16];
    char *spark_data;
    s32   spark_frame_count, spark_second_frame_count, spark_third_frame_count;

    spark_data = ((char *)MOBY(moby)->pvars);
    if (fast_dec_timer((int *)(spark_data + 0x3C)) == 0)
        return;
    if (*(s32 *)(spark_data + 0x38) != 0) {
        *(s32 *)(spark_data + 0x3C) = spark_emitter_default_timer_reset_gp;
        euler_rotation_matrix(spark_rotation_matrix, moby + 0x40);
        qcopy(spark_position, moby + 0x10);
        spark_position[0] += random_float_between(-0.05f, 0.05f);
        spark_position[1] += random_float_between(-0.05f, 0.05f);
        spark_position[0] += random_float_between(-0.05f, 0.05f);
        random_float_between(-0.17453292f, 0.17453292f);
        fast_vec_normalize(
            spark_motion_primary,
            spark_rotation_matrix,
            random_float_between(spark_emitter_default_primary_magnitude_base_gp,
                                 spark_emitter_default_primary_magnitude_base_gp * 1.2f) *
                frame_time);
        fast_vec_normalize(spark_motion_offset,
                           spark_rotation_matrix + 4,
                           random_float_between(-spark_emitter_default_primary_spread_limit_gp,
                                                spark_emitter_default_primary_spread_limit_gp) *
                               frame_time);
        fast_vec_add(spark_motion_primary, spark_motion_primary, spark_motion_offset);
        spark_motion_primary[2] +=
            random_float_between(spark_emitter_default_primary_z_increment_base_gp,
                                 spark_emitter_default_primary_z_increment_base_gp * 1.2f) *
            frame_time;
        qcopy(spark_motion_secondary, spark_motion_primary);
        fast_vec_normalize(
            spark_motion_secondary,
            spark_rotation_matrix,
            random_float_between(spark_emitter_default_secondary_magnitude_base_gp,
                                 spark_emitter_default_secondary_magnitude_base_gp * 1.5f) *
                frame_time);
        fast_vec_normalize(spark_motion_offset,
                           spark_rotation_matrix + 4,
                           random_float_between(-spark_emitter_default_secondary_spread_limit_gp,
                                                spark_emitter_default_secondary_spread_limit_gp) *
                               frame_time);
        fast_vec_add(spark_motion_secondary, spark_motion_secondary, spark_motion_offset);
        spark_motion_secondary[2] +=
            random_float_between(spark_emitter_default_secondary_z_increment_base_gp,
                                 spark_emitter_default_secondary_z_increment_base_gp * 1.5f) *
            frame_time;
        spark_motion_primary[3] = spark_emitter_default_primary_motion_w_gp;
        spark_motion_secondary[3] = spark_emitter_default_secondary_motion_w_gp;
        spark_frame_count = truncate_float_to_s32(scale_time(
            random_float_between((f32)spark_emitter_default_frame_count_base_gp,
                                 (f32)spark_emitter_default_frame_count_base_gp * 1.2f)));
        spark_second_frame_count = truncate_float_to_s32(scale_time(
            random_float_between((f32)spark_emitter_default_second_frame_count_base_gp,
                                 (f32)spark_emitter_default_second_frame_count_base_gp * 1.2f)));
        spark_third_frame_count = truncate_float_to_s32(scale_time(
            random_float_between((f32)spark_emitter_default_third_frame_count_base_gp,
                                 (f32)spark_emitter_default_third_frame_count_base_gp * 1.2f)));
        spawn_spark(spark_position,
                    spark_motion_primary,
                    spark_motion_secondary,
                    spark_emitter_default_initial_spark_word_gp,
                    spark_emitter_default_second_spark_word_gp,
                    spark_frame_count,
                    spark_second_frame_count,
                    spark_third_frame_count,
                    -1);
    } else {
        *(s32 *)(spark_data + 0x3C) = *(s32 *)(spark_data + 0x14);
        euler_rotation_matrix(spark_rotation_matrix, moby + 0x40);
        qcopy(spark_position, moby + 0x10);
        spark_position[0] += random_float_between(-0.05f, 0.05f);
        spark_position[1] += random_float_between(-0.05f, 0.05f);
        spark_position[2] += random_float_between(-0.05f, 0.05f);
        random_float_between(-0.17453292f, 0.17453292f);
        fast_vec_normalize(
            spark_motion_primary,
            spark_rotation_matrix,
            random_float_between(*(f32 *)(spark_data + 0x18), *(f32 *)(spark_data + 0x18) * 1.2f) *
                frame_time);
        fast_vec_normalize(
            spark_motion_offset,
            spark_rotation_matrix + 4,
            random_float_between(-*(f32 *)(spark_data + 0x30), *(f32 *)(spark_data + 0x30)) *
                frame_time);
        fast_vec_add(spark_motion_primary, spark_motion_primary, spark_motion_offset);
        spark_motion_primary[2] +=
            random_float_between(*(f32 *)(spark_data + 0x1C), *(f32 *)(spark_data + 0x1C) * 1.2f) *
            frame_time;
        qcopy(spark_motion_secondary, spark_motion_primary);
        fast_vec_normalize(
            spark_motion_secondary,
            spark_rotation_matrix,
            random_float_between(*(f32 *)(spark_data + 0x20), *(f32 *)(spark_data + 0x20) * 1.5f) *
                frame_time);
        fast_vec_normalize(
            spark_motion_offset,
            spark_rotation_matrix + 4,
            random_float_between(-*(f32 *)(spark_data + 0x34), *(f32 *)(spark_data + 0x34)) *
                frame_time);
        fast_vec_add(spark_motion_secondary, spark_motion_secondary, spark_motion_offset);
        spark_motion_secondary[2] +=
            random_float_between(*(f32 *)(spark_data + 0x24), *(f32 *)(spark_data + 0x24) * 1.5f) *
            frame_time;
        spark_motion_primary[3] = *(f32 *)(spark_data + 0x28);
        spark_motion_secondary[3] = *(f32 *)(spark_data + 0x2C);
        spark_frame_count = truncate_float_to_s32(scale_time(random_float_between(
            (f32) * (s32 *)(spark_data + 0x08), (f32) * (s32 *)(spark_data + 0x08) * 1.2f)));
        spark_second_frame_count = truncate_float_to_s32(scale_time(random_float_between(
            (f32) * (s32 *)(spark_data + 0x0C), (f32) * (s32 *)(spark_data + 0x0C) * 1.2f)));
        spark_third_frame_count = truncate_float_to_s32(scale_time(random_float_between(
            (f32) * (s32 *)(spark_data + 0x10), (f32) * (s32 *)(spark_data + 0x10) * 1.2f)));
        spawn_spark(spark_position,
                    spark_motion_primary,
                    spark_motion_secondary,
                    *(s32 *)(spark_data + 0x00),
                    *(s32 *)(spark_data + 0x04),
                    spark_frame_count,
                    spark_second_frame_count,
                    spark_third_frame_count,
                    -1);
    }
}
/* Paratrooper: initializes its path and steering, patrols, attacks its target,
 * and handles hit recovery and death. Shared by Batalia and Gaspar. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00302618), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x80];
    float pos[4];
    char  pad90[0x108];
    int   timer;
    char  pad19c[0x1EE8];
    int   mode;
} Hero302618;

typedef struct {
    char          pad0[0x20];
    float         f20;
    short         f24;
    char          pad26[0x2];
    unsigned char f28;
    unsigned char f29;
    char          pad2A[0x4];
    unsigned char f2E;
    char          pad2F[0x29];
    unsigned char f58;
    char          pad59[0x1];
    unsigned char f5A;
    char          pad5B[0x75];
    int           fD0;
    char          padD4[0x4];
    float         fD8;
    float         fDC;
    float         fE0;
    char          padE4[0x14];
    float         fF8;
    float         fFC;
    char          pad100[0xC];
    float         f10C;
    float         f110;
    float         f114;
    char          pad118[0x48];
    char         *f160;
    int           f164;
    char          pad168[0x68];
    int           f1D0;
    int           f1D4;
    char          pad1D8[0x2];
    short         f1DA;
    char          pad1DC[0xC];
    float         f1E8;
    int           f1EC;
    int           f1F0;
    int           f1F4;
    char         *f1F8;
    int           f1FC;
    char          pad200[0xC];
    short         f20C;
    short         f20E;
} Paratrooper302618;

char *spawn_paratrooper_projectile(char *pos, char *dir, int a2, int a3, int a4, int a5) __asm__("FUN_L08_00307190");
extern char  D_L08_001675C0[];
extern char  D_L08_001FC8F0[];
extern char  paratrooper_invalid_jump_path_format[] __asm__("D_L08_001FC630");
extern float D_L08_0015F580[4] MACRO_ADDR;
extern float D_L08_0015F5D0;
extern float D_L08_0015F5D4;
extern int   collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern int   FUN_L00_00257b90(int, int);
extern int   FUN_L00_00259888();
extern s32   rand(void);
extern void  DebugPrint(char *, ...);
int          FUN_L00_00258830(char *m, char *v, int flags, float a, float b, float c);
int          FUN_L00_00259028(unsigned char *m, char *c, float *tgt, void *out);
void        *FUN_L00_00263fd8(char  *src,
                              int    cls,
                              float *pos,
                              void  *mat,
                              int    a8,
                              int    a9,
                              float *v10,
                              float *v11,
                              float  scale,
                              float *v12);
void         FUN_L00_0025f730(void *a, float x);
void         FUN_L01_0026d930(char *p);
void         update_blarg_paratrooper_damage_and_state(char *m) __asm__("FUN_L08_003024d8");
extern char  D_0013F350[];
extern int   level_game_mode __asm__("D_L08_0015F5C4");
extern char *target302618 __asm__("D_L08_00174658") NOT_SDA;
extern float speed302618 __asm__("D_L08_001621C4") __attribute__((sda));
extern float accel302618 __asm__("D_L08_001621C0") __attribute__((sda));
extern int   delay302618 __asm__("D_L08_001621BC") __attribute__((sda));
extern int   hops302618 __asm__("D_L08_001621CC") __attribute__((sda));
extern float melee302618 __asm__("D_L08_001621C8") __attribute__((sda));
extern float height302618 __asm__("D_L08_001621D0") __attribute__((sda));
extern float shove302618 __asm__("D_L08_001621B4") __attribute__((sda));
extern float shove_rate302618 __asm__("D_L08_001621B8") __attribute__((sda));
extern int   random302618(void) __asm__("FUN_001160d8");
extern float turn302618(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");
extern void  FUN_L00_00263ac8_c(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern void  FUN_L00_0025a120_c(void *) __asm__("FUN_L00_0025a120");
extern void  debug_print(char *, int) __asm__("FUN_001e93b0");
extern void  FUN_L01_0026d930_c(void *) __asm__("FUN_L01_0026d930");
extern void  FUN_L00_00259028_c(void *, void *, void *, void *) __asm__("FUN_L00_00259028");
extern void  FUN_L00_0025f730(void *, float);
extern void
           FUN_L00_00258830_c(void *, void *, float, float, float, int) __asm__("FUN_L00_00258830");
extern int damage302618(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void *
FUN_L00_00263fd8_c(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");

extern char paratrooper_invalid_wake_path_format[] __asm__("D_L08_001FC600");
void        blarg_paratrooper_update(char *moby) __asm__("FUN_L08_00301158");

void blarg_paratrooper_update(char *moby) {
    enum {
        PARATROOPER_STATE_INITIALIZE = 0,
        PARATROOPER_STATE_PATROL_PATH = 1,
        PARATROOPER_STATE_APPROACH_WAYPOINT = 2,
        PARATROOPER_STATE_WAIT_FACING_TARGET = 3,
        PARATROOPER_STATE_HOP_BETWEEN_WAYPOINTS = 4,
        PARATROOPER_STATE_FIRE_PROJECTILES = 5,
        PARATROOPER_STATE_MOVE_TOWARD_FIRST_WAYPOINT = 6,
        PARATROOPER_STATE_MELEE_ATTACK = 7,
        PARATROOPER_STATE_WAIT_FOR_HERO_TRIGGERED_ANIMATION = 8,
        PARATROOPER_STATE_PAUSE_AFTER_HOP_LIMIT = 9,
        PARATROOPER_STATE_KNOCKBACK_RECOVERY = 10,
        PARATROOPER_STATE_DEATH_EFFECTS_AND_REMOVE = 11
    };
    Paratrooper302618 *data = (Paratrooper302618 *)(*(char **)((char *)(moby) + 0x78));
    char              *path;
    char              *target;
    Hero302618        *hero;
    float              pos[4] __attribute__((aligned(16)));
    float              attack_vector_or_ray_endpoint[4] __attribute__((aligned(16)));
    char               hit[0x30] __attribute__((aligned(16)));
    if ((*(unsigned char *)((char *)(moby) + 0x31)) == 0 &&
        fast_vec_dist(moby + 0x10, D_L08_001675C0) > 40.0f)
        return;
    FUN_L00_00263ac8_c(2.5f, moby, 6, (char *)data + 0x170);
    if ((*(unsigned char *)((char *)(moby) + 0x31)) != 0 &&
        fast_vec_dist(moby + 0x10, D_L08_001675C0) < 29.0f) {
        FUN_L00_0025a120_c(moby);
        (*(unsigned char *)((char *)(moby) + 0x7F)) = 23;
    }
    if (level_game_mode == 2)
        return;
    path = 0;
    update_blarg_paratrooper_damage_and_state(moby);
    target = data->f160;
    if (data->f1FC != -1)
        path = ((char *)l08_paths[data->f1FC]);
    hero = (Hero302618 *)(((char *)&D_0013F350));
    if (hero->mode == 61 && hero->timer < scale_game_frames(30) &&
        (*(unsigned char *)((char *)(moby) + 0x20)) != 8 &&
        (*(unsigned char *)((char *)(moby) + 0x20)) != 10 &&
        (*(unsigned char *)((char *)(moby) + 0x20)) != 4 &&
        (*(unsigned char *)((char *)(moby) + 0x20)) != 11 &&
        vector_distance_xy(moby + 0x10, hero->pos) < 25.0f && !random_integer_below(49)) {
        if ((*(unsigned char *)((char *)(moby) + 0x53)) != 14)
            blend_moby_animation(moby, 14, 0, scale_game_frames(11));
        (*(unsigned char *)((char *)(moby) + 0x20)) = 8;
    }
    if ((*(unsigned char *)((char *)(moby) + 0x20)) != 0 && data->f2E == 2) {
        mark_moby_for_removal(moby);
        if (data->f1F8)
            mark_moby_for_removal(data->f1F8);
        return;
    }
    switch ((*(unsigned char *)((char *)(moby) + 0x20))) {
    case PARATROOPER_STATE_INITIALIZE: {
        float turn_rate;
        data->f29 = 0;
        data->f28 = 1;
        data->f2E = 1;
        data->f20 = 2.0f;
        data->f24 = 2;
        data->f58 = truncate_float_to_s32(15.0f);
        data->f5A = truncate_float_to_s32(12.0f);
        if (data->f1D0 == -1) {
            debug_print(paratrooper_invalid_wake_path_format, (*(short *)((char *)(moby) + 0xB2)));
            goto delete_moby;
        }
        if (data->f1D4 == -1) {
            debug_print(paratrooper_invalid_jump_path_format, (*(short *)((char *)(moby) + 0xB2)));
            goto delete_moby;
        }
        FUN_L01_0026d930_c((char *)data + 0xD0);
        data->fDC = 0.5f;
        data->fD8 = 0.5f;
        data->fD0 = truncate_float_to_s32(512.0f);
        turn_rate = D_0015ED70 * 6.981317f;
        data->f110 = data->f10C = turn_rate;
        data->f114 = D_0015ED6C * 8.726646f;
        data->fF8 = 0.34906584f;
        data->fFC = 0.2f;
        if (random302618() & 1)
            (*(unsigned short *)((char *)(moby) + 0x34)) |= 0x8000;
        qcopy(moby + 0x10, ((char *)l08_paths[data->f1D4]) + 0x10);
        if (path) {
            int   index;
            short direction;
            data->f20C = 2 * (random302618() & 1) - 1;
            index = select_point_index_by_distance(moby + 0x10, path, 0.0f) + data->f20C;
            direction = data->f20C;
            data->f1F4 = index;
            if (index >= (*(int *)((char *)(path) + 0)) || index < 0) {
                data->f20C = -direction;
                data->f1F4 = index + 2 * data->f20C;
            }
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 2)
                blend_moby_animation(moby, 2, 0, random302618() % 4 + 7);
            (*(unsigned char *)((char *)(moby) + 0x20)) = 1;
        } else
            (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
        data->f1DA = 0;
        break;
    }
    case PARATROOPER_STATE_PATROL_PATH:
        if (vector_distance_xy(moby + 0x10, path + 0x10 + data->f1F4 * 16) < data->fE0) {
            short direction = data->f20C;
            int   index = data->f1F4 + direction;
            data->f1F4 = index;
            if (index >= (*(int *)((char *)(path) + 0)) || index < 0) {
                data->fE0 = 0.0f;
                data->f20C = -direction;
                data->f1F4 = index + 2 * data->f20C;
            }
        }
        FUN_L00_00259028_c(moby, (char *)data + 0xD0, path + 0x10 + data->f1F4 * 16, pos);
        if (target && data->f164 != 2 && fast_dec_timer((char *)data + 0x1F0)) {
            data->f1F4 =
                select_point_index_by_distance(moby + 0x10, ((char *)l08_paths[data->f1D4]), 0.0f);
            (*(unsigned char *)((char *)(moby) + 0x20)) = 2;
        }
        break;
    case PARATROOPER_STATE_APPROACH_WAYPOINT: {
        float distance;
        data->f1F4 =
            select_point_index_by_distance(moby + 0x10, ((char *)l08_paths[data->f1D4]), 0.0f);
        distance = vector_distance_xy(moby + 0x10,
                                      ((char *)l08_paths[data->f1D4]) + 0x10 + data->f1F4 * 16);
        if (distance < 1.0f || distance < data->fE0) {
            data->fE0 = 0.0f;
            (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
        } else {
            FUN_L00_00259028_c(moby,
                               (char *)data + 0xD0,
                               ((char *)l08_paths[data->f1D4]) + 0x10 + data->f1F4 * 16,
                               pos);
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 2)
                blend_moby_animation(moby, 2, 0, random302618() % 4 + 7);
        }
        break;
    }
    case PARATROOPER_STATE_WAIT_FACING_TARGET: {
        float *rotation = (float *)&MOBY(moby)->rot.z;
        float  heading;
        float  frame;
        heading = fast_arc_tan(
            (*(float *)((char *)(target) + 0x10)) - (*(float *)((char *)(moby) + 0x10)),
            (*(float *)((char *)(target) + 0x14)) - (*(float *)((char *)(moby) + 0x14)));
        turn302618(rotation,
                   heading,
                   (char *)data + 0x1E0,
                   D_0015ED70 * 8.726646f,
                   D_0015ED70 * 8.726646f,
                   D_0015ED6C * 12.566371f);
        if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2) {
            int oldanim = (*(unsigned char *)((char *)(moby) + 0x53));
            if (oldanim != (random302618() & 1)) {
                int newanim = random302618() & 1;
                int ticks = scale_game_frames(7);
                blend_moby_animation(moby, newanim, 0, ticks + random302618() % 7);
            }
        }
        if (hops302618 < data->f1DA) {
            data->f1DA = 0;
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 10) {
                int ticks = scale_game_frames(7);
                blend_moby_animation(
                    moby, 10, 2, ticks + random_int_between(0, scale_game_frames(5)));
            }
            (*(unsigned char *)((char *)(moby) + 0x20)) = 9;
        } else if (target) {
            if (data->f164 != 2 && fast_dec_timer((char *)data + 0x1F0)) {
                qcopy((char *)data + 0x1C0, moby + 0x10);
                data->f1F4 = (data->f1F4 + 1) & 1;
                (*(unsigned char *)((char *)(moby) + 0x20)) = 4;
                fast_vec_sub(
                    pos, ((char *)l08_paths[data->f1D4]) + 0x10 + data->f1F4 * 16, moby + 0x10);
                if (fast_vec_dot(moby + 0xD0, pos) < 0.0f) {
                    if ((*(unsigned char *)((char *)(moby) + 0x53)) != 18)
                        blend_moby_animation(moby, 18, 0, scale_game_frames(5));
                } else if ((*(unsigned char *)((char *)(moby) + 0x53)) != 16)
                    blend_moby_animation(moby, 16, 0, scale_game_frames(5));
            } else if (target && vector_distance_xy(moby + 0x10, target + 0x10) < melee302618) {
                (*(unsigned char *)((char *)(moby) + 0x20)) = 7;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 13)
                    blend_moby_animation(moby, 13, 0, scale_game_frames(10));
            }
        }
        break;
    }
    case PARATROOPER_STATE_HOP_BETWEEN_WAYPOINTS: {
        float *rotation = (float *)&MOBY(moby)->rot.z;
        float  heading;
        float  frame;
        heading = fast_arc_tan(
            (*(float *)((char *)(target) + 0x10)) - (*(float *)((char *)(moby) + 0x10)),
            (*(float *)((char *)(target) + 0x14)) - (*(float *)((char *)(moby) + 0x14)));
        turn302618(rotation,
                   heading,
                   (char *)data + 0x1E0,
                   D_0015ED70 * 8.726646f,
                   D_0015ED70 * 8.726646f,
                   D_0015ED6C * 12.566371f);
        frame = compute_interpolated_record_value(moby);
        if (frame > 12.0f && frame < 23.0f &&
            (*(unsigned char *)((char *)(moby) + 0x52)) ==
                (*(unsigned char *)((char *)(moby) + 0x53))) {
            float progress = 1.0f - (23.0f - frame) / 11.0f;
            float arc;
            if (progress > 1.0f)
                progress = 1.0f;
            else if (progress < 0.0f)
                progress = 0.0f;
            lerp_vector(pos,
                        (char *)data + 0x1C0,
                        ((char *)l08_paths[data->f1D4]) + 0x10 + data->f1F4 * 16,
                        progress);
            arc = 2.0f * (progress - 0.5f);
            pos[2] += height302618 * (1.0f - arc * arc);
            qcopy(moby + 0x10, pos);
        } else if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2) {
            qcopy(pos, moby + 0x10);
            qcopy(attack_vector_or_ray_endpoint, target + 0x10);
            pos[2] += 1.0f;
            attack_vector_or_ray_endpoint[2] += 0.75f;
            collision_line(pos, attack_vector_or_ray_endpoint, 0, moby, 0);
            if (hops302618 < data->f1DA) {
                data->f1DA = 0;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 10) {
                    blend_moby_animation(moby, 10, 2, random302618() % 4 + 5);
                }
                (*(unsigned char *)((char *)(moby) + 0x20)) = 9;
            } else if (vector_distance_xy(moby + 0x10, target + 0x10) < melee302618) {
                goto melee_state;
            } else if (target302618 != target) {
                (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(5));
                data->f1F0 = truncate_float_to_s32(scale_time(random_float_between(45.0f, 90.0f)));
            } else {
                data->f1E8 = 1.0f;
                (*(unsigned char *)((char *)(moby) + 0x20)) = 5;
                data->f20E = 0;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 6)
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    }
    case PARATROOPER_STATE_FIRE_PROJECTILES:
        if (vector_distance_xy(moby + 0x10, target + 0x10) < melee302618) {
            (*(unsigned char *)((char *)(moby) + 0x20)) = 7;
            data->f1EC = 0;
            if ((*(unsigned char *)((char *)(moby) + 0x53)) == 13)
                break;
            goto animate_melee;
        } else if (fast_dec_timer((char *)data + 0x1EC)) {
            data->f1EC = scale_game_frames(
                delay302618 + truncate_float_to_s32(D_L08_0015F5D4 * 1.3f + D_L08_0015F5D0));
            data->f1E8 += accel302618;
            if ((D_L08_0015F5D4 < 0.8f && D_L08_0015F5D0 < 0.8f) ||
                !((*(unsigned short *)&data->f20E) & 1)) {
                char *shot;
                moby_point_position(data->f1F8, 0, pos);
                attack_vector_or_ray_endpoint[0] =
                    fast_cos((*(float *)((char *)(moby) + 0x48))) * (speed302618 * D_0015ED6C);
                attack_vector_or_ray_endpoint[1] =
                    fast_sin((*(float *)((char *)(moby) + 0x48))) * (speed302618 * D_0015ED6C);
                attack_vector_or_ray_endpoint[2] = 0.0f;
                attack_vector_or_ray_endpoint[2] =
                    (-1.0f / data->f1E8) * (speed302618 * D_0015ED6C);
                shot = spawn_paratrooper_projectile(attack_vector_or_ray_endpoint,
                                                    pos,
                                                    moby,
                                                    target,
                                                    scale_game_frames(180),
                                                    data->f20E);
                if (shot)
                    (*(float *)((char *)(shot) + 0x2C)) *= 1.7f;
            }
            data->f20E = (*(unsigned short *)&data->f20E) + 1;
            if (data->f20E >= 5)
                data->f20E = 0;
            if (data->f1E8 > 20.0f) {
                qcopy((char *)data + 0x1C0, moby + 0x10);
                (*(unsigned short *)&data->f1DA)++;
                data->f1F4 = (data->f1F4 + 1) & 1;
                (*(unsigned char *)((char *)(moby) + 0x20)) = 4;
                fast_vec_sub(
                    pos, ((char *)l08_paths[data->f1D4]) + 0x10 + data->f1F4 * 16, moby + 0x10);
                if (fast_vec_dot(moby + 0xD0, pos) < 0.0f) {
                    if ((*(unsigned char *)((char *)(moby) + 0x53)) != 18)
                        blend_moby_animation(moby, 18, 0, scale_game_frames(5));
                } else if ((*(unsigned char *)((char *)(moby) + 0x53)) != 16)
                    blend_moby_animation(moby, 16, 0, scale_game_frames(5));
            }
        }
        break;
    case PARATROOPER_STATE_MOVE_TOWARD_FIRST_WAYPOINT: {
        float *rotation = (float *)&MOBY(moby)->rot.z;
        float  heading;
        float  frame;
        heading = fast_arc_tan(
            (*(float *)((char *)(target) + 0x10)) - (*(float *)((char *)(moby) + 0x10)),
            (*(float *)((char *)(target) + 0x14)) - (*(float *)((char *)(moby) + 0x14)));
        turn302618(rotation,
                   heading,
                   (char *)data + 0x1E0,
                   D_0015ED70 * 8.726646f,
                   D_0015ED70 * 8.726646f,
                   D_0015ED6C * 12.566371f);
        frame = compute_interpolated_record_value(moby);
        if (frame > 12.0f && frame < 23.0f && (*(float *)((char *)(moby) + 0x54)) == 0.0f) {
            fast_vec_sub(pos, ((char *)l08_paths[data->f1D4]) + 0x10, moby + 0x10);
            FUN_L00_0025f730(pos, D_0015ED6C * 24.0f);
            FUN_L00_00258830_c(moby, pos, shove302618, shove_rate302618, 0.0f, 0);
        } else if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2) {
            data->f1F0 = truncate_float_to_s32(scale_time(random_float_between(45.0f, 90.0f)));
            (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    }
    case PARATROOPER_STATE_MELEE_ATTACK: {
        float *rotation = (float *)&MOBY(moby)->rot.z;
        float  heading;
        float  frame;
        heading = fast_arc_tan(
            (*(float *)((char *)(target) + 0x10)) - (*(float *)((char *)(moby) + 0x10)),
            (*(float *)((char *)(target) + 0x14)) - (*(float *)((char *)(moby) + 0x14)));
        turn302618(rotation,
                   heading,
                   (char *)data + 0x1E0,
                   D_0015ED70 * 8.726646f,
                   D_0015ED70 * 8.726646f,
                   D_0015ED6C * 12.566371f);
        frame = compute_interpolated_record_value(moby);
        if (frame > 16.5f && frame < 20.0f) {
            moby_point_position(moby, 4, pos);
            fast_vec_sub(attack_vector_or_ray_endpoint, pos, moby + 0x10);
            attack_vector_or_ray_endpoint[2] = 0.0f;
            fast_vec_normalize(attack_vector_or_ray_endpoint, attack_vector_or_ray_endpoint, 1.0f);
            attack_vector_or_ray_endpoint[2] = 1.0f;
            pos[2] -= 0.25f;
            make_hit_descriptor(hit, moby, 1, 1.0f, attack_vector_or_ray_endpoint);
            damage302618(0.8f, pos, 0, moby, hit);
        }
        if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2)
            (*(unsigned char *)((char *)(moby) + 0x20)) = 2;
        break;
    }
    case PARATROOPER_STATE_WAIT_FOR_HERO_TRIGGERED_ANIMATION:
    case PARATROOPER_STATE_PAUSE_AFTER_HOP_LIMIT:
        if ((*(unsigned char *)((char *)(moby) + 0x70)) & 2) {
            (*(unsigned char *)((char *)(moby) + 0x20)) = 3;
            if ((*(unsigned char *)((char *)(moby) + 0x53)) != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    case PARATROOPER_STATE_KNOCKBACK_RECOVERY: {
        int flags = mine_fall(moby, (char *)data + 0x70);
        if (flags & 0x40) {
            if (vector_distance_xy(moby + 0x10, target + 0x10) < melee302618) {
            melee_state:
                (*(unsigned char *)((char *)(moby) + 0x20)) = 7;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 13) {
                animate_melee:
                    blend_moby_animation(moby, 13, 0, random302618() % 4 + 8);
                }
            } else {
                data->f1E8 = 1.0f;
                (*(unsigned char *)((char *)(moby) + 0x20)) = 5;
                data->f20E = 0;
                if ((*(unsigned char *)((char *)(moby) + 0x53)) != 6) {
                    int ticks = scale_game_frames(10);
                    blend_moby_animation(
                        moby, 6, 0, ticks + random_int_between(0, scale_game_frames(7)));
                }
            }
        } else if (flags & 0x120) {
        delete_moby:
            mark_moby_for_removal(moby);
        }
    } break;
    case PARATROOPER_STATE_DEATH_EFFECTS_AND_REMOVE:
        if (mine_fall(moby, (char *)data + 0x70) & 0x160) {
            float *color;
            char  *rotation;
            clear_u64_value(pos);
            color = D_L08_0015F580;
            FUN_L00_00257470(moby, 0, -1);
            cart_burst(moby, moby + 0x10, 0, 0.5f, 13.0f);
            rotation = moby + 0x40;
            FUN_L00_00263fd8_c(moby, 0x69C, moby + 0x10, rotation, 0, 0, 0.0f, color, color, color);
            FUN_L00_00263fd8_c(moby, 0x69D, moby + 0x10, rotation, 0, 0, 0.0f, color, color, color);
            FUN_L00_00263fd8_c(moby, 0x69E, moby + 0x10, rotation, 0, 0, 0.0f, color, color, color);
            mark_moby_for_removal(moby);
            if (data->f1F8)
                mark_moby_for_removal(data->f1F8);
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00303998), where it is exact; names translated to the US level program. */
extern void trooper_knockback_arc(void *, void *, float *, float *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void spawn_randomized_bolt_drop(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int
activate_effect_l08(void *, void *, float, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern float D_L08_00162180 __attribute__((sda));
extern float D_L08_00162184 __attribute__((sda));
extern float D_L08_0016218C __attribute__((sda));
extern float D_L08_00162190 __attribute__((sda));
extern float D_L08_00162194 __attribute__((sda));
extern float D_L08_001621B0 __attribute__((sda));

void update_blarg_paratrooper_damage_and_state(char *moby) {
    char *data, *target, *child, *attached, *chased;
    float pos[4] __attribute__((aligned(16)));
    float matrix[16] __attribute__((aligned(16)));
    float damage, knock_angle, second_knock_angle;
    int   hit_state, reaction, turn, state;
    float angle, distance, height, dx, dy;

    data = ((char *)MOBY(moby)->pvars);
    if (MOBY(moby)->state == 0)
        return;
    {
        float timer_scale = D_L08_001621B0 * frame_time;
        ((Moby *)moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * D_L08_00162180;
        *(float *)(data + 0xf4) = timer_scale;
    }

    if (*(char **)(data + 0x1f8) == 0) {
        child = create_moby(0x401);
        *(char **)(data + 0x1f8) = child;
        if (child != 0) {
            *(short *)(child + 0x32) = 0x40;
            *(unsigned char *)(*(char **)(data + 0x1f8) + 0x30) = 0x40;
            *(unsigned char *)(*(char **)(data + 0x1f8) + 0x31) = 1;
            *(unsigned short *)(*(char **)(data + 0x1f8) + 0x34) |= 0x100;
            moby_point_position(moby, 4, pos);
            qcopy(*(char **)(data + 0x1f8) + 0x10, pos);
        }
        MOBY(moby)->flags |= 0x1000;
    } else {
        moby_point_position(moby, 4, pos);
        build_moby_bone_transform(moby, 4, matrix);
        copy_matrix3x4(*(char **)(data + 0x1f8) + 0xc0, matrix);
        attached = *(char **)(data + 0x1f8);
        qcopy(attached + 0x10, pos);
        update_entity(attached);
    }

    damage = 0.0f;
    target = find_moby_hit(moby, 0x330000, 0);
    reaction = fun_00213928(moby, target, data + 0x20, 0, &hit_state, &damage, 0, 4);
    if (hit_state != 1 && MOBY(moby)->state != 11) {
        {
            float health = *(float *)(data + 0x20) - damage;
            *(float *)(data + 0x20) = health;
            if (*(short *)(*(char **)(target + 0x20) + 0xa6) == 0x47)
                reaction = 3;
            if (health <= 0.0f)
                reaction = 1;
        }

        turn = truncate_float_to_s32(512.0f);
        {
            float pose_scale = D_L08_00162184 * frame_time_sq;
            *(int *)(data + 0x90) = turn;
            *(float *)(data + 0x98) = 0.5f;
            *(int *)(data + 0x94) = 9;
            *(float *)(data + 0x80) = pose_scale;
        }
        *(unsigned char *)(data + 0xad) = 0;

        if ((unsigned)reaction < 12) {
            switch (reaction) {
            case 0:
            case 11:
                break;
            case 9:
            case 10:
                *(unsigned char *)(data + 0x67) = 0xfa;
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8: {
                float pose_scale = D_L08_0016218C * frame_time;
                float timer_scale = frame_time * 3.0f;
                *(float *)(data + 0x88) = pose_scale;
                *(float *)(data + 0x8c) = timer_scale;
                *(float *)(data + 0xc0) = 5.0f;
                *(float *)(data + 0xc4) = 10.0f;
            }
                {
                    char *hit_moby = *(char **)(target + 0x20);
                    dx = MOBY(moby)->pos.x - MOBY(hit_moby)->pos.x;
                    dy = MOBY(moby)->pos.y - MOBY(hit_moby)->pos.y;
                }
                angle = fast_arc_tan(dx, dy);
                *(u128 *)pos = *(u128 *)(target + 0x10);
                knock_angle = angle;
                trooper_knockback_arc(
                    pos, &knock_angle, (float *)(data + 0x88), (float *)(data + 0x8c));
                trooper_start_knockback(moby, data + 0x70, knock_angle, 8, 1, 0);
                MOBY(moby)->state = 10;
                *(unsigned char *)(data + 0x67) = 0x78;
                break;
            case 1:
            case 2: {
                float push_limit = 16.0f;
                float pose_rate = D_L08_00162184;
                float timer_rate = D_L08_00162194;
                float angle_rate = D_L08_00162190;
                float step, pose_scale, timer_scale, angle_scale;
                ((Moby *)moby)->flags &= 0xefff;
                step = frame_time;
                pose_scale = pose_rate * frame_time_sq;
                timer_scale = timer_rate * step;
                angle_scale = angle_rate * step;
                *(float *)(data + 0x98) = 0.25f;
                *(float *)(data + 0x8c) = angle_scale;
                *(float *)(data + 0x80) = pose_scale;
                *(float *)(data + 0x88) = timer_scale;
                *(float *)(data + 0xc0) = 11.0f;
                *(float *)(data + 0xc4) = push_limit;
                *(int *)(data + 0x90) = 0x200;
            }
                {
                    char *hit_moby = *(char **)(target + 0x20);
                    dx = MOBY(moby)->pos.x - MOBY(hit_moby)->pos.x;
                    dy = MOBY(moby)->pos.y - MOBY(hit_moby)->pos.y;
                }
                angle = fast_arc_tan(dx, dy);
                *(u128 *)pos = *(u128 *)(target + 0x10);
                second_knock_angle = angle;
                trooper_knockback_arc(
                    pos, &second_knock_angle, (float *)(data + 0x88), (float *)(data + 0x8c));
                trooper_start_knockback(moby, data + 0x70, second_knock_angle, 12, 1, 0);
                MOBY(moby)->state = 11;
                *(unsigned char *)(data + 0x67) = 0xf0;
                spawn_randomized_bolt_drop(moby, 2, 3, 0, -1);
                break;
            default:
                break;
            }
        }
        trooper_anim_update(moby, data + 0x60);
    }
    MOBY(moby)->hit_slot = 0xff;
    trooper_anim_finish(moby, data + 0x60);

    if (*(int *)(data + 0x38) != 0) {
        angle = random_float_between(180.0f, 240.0f);
        angle = scale_time(angle);
        *(short *)(data + 0x1d8) = (short)truncate_float_to_s32(angle);
        *(int *)(data + 0x38) = 0;
    }
    fast_dec_timer_s16((short *)(data + 0x1d8));
    if (*(short *)(data + 0x1d8) != 0)
        *(float *)(data + 0x1dc) = *(float *)(data + 0x1e4) + 6.0f;
    else
        *(float *)(data + 0x1dc) = *(float *)(data + 0x1e4);

    if (random_integer_below(4) == 0) {
        {
            char *path = ((char *)l08_paths[*(int *)(data + 0x1d0)]);
            reaction = activate_effect_l08(
                moby, data + 0x120, *(float *)(data + 0x1dc), 0, 0, path + 0x10, *(int *)path);
        }
        if (reaction != 2) {
            distance = vector_distance_xy(moby + 0x10, data + 0x120);
            if (*(float *)(data + 0x1dc) < distance ||
                3.0f < absolute_float(MOBY(moby)->pos.z - *(float *)(data + 0x128)))
                *(int *)(data + 0x164) = 2;
        }
    } else {
        chased = *(char **)(data + 0x160);
        if (chased != 0 && *(unsigned char *)(chased + 0x20) != 0xfe &&
            *(unsigned char *)(chased + 0x20) != 0xfd) {
            qcopy(data + 0x120, chased + 0x10);
        } else {
            *(char **)(data + 0x160) = 0;
            *(int *)(data + 0x164) = 2;
        }
    }
    if (*(int *)(data + 0x160) == 0)
        *(char **)(data + 0x160) = (char *)hero.moby;

    state = MOBY(moby)->state;
    if (state == 0 || state == 5 || state == 7 || state == 4) {
        height = probe_ground_height(moby + 0x10, 0, 0.5f);
        if (height < MOBY(moby)->pos.z)
            MOBY(moby)->pos.z = height;
    }
}

float D_L09_00161C14 = 2e+01f;
float D_L09_00161C10 = 1.0f;
int   D_L09_00161C0C = 5;
int   D_L09_00161C1C = 5;
float D_L09_00161C18 = 3.3f;
float D_L09_00161C20 = 2.0f;
float D_L09_00161C04 = 0.6f;
float D_L09_00161C08 = 0.6f;
float D_L09_00161BD0 = 1.0f;
float D_L09_00161BD4 = 37.0f;
float D_L09_00161BDC = 1e+01f;
float D_L09_00161BE0 = 11.0f;
float D_L09_00161BE4 = 15.0f;
float D_L09_00161C00 = 3.0f;
s32   D_L10_00161954 = 0x20706060;
s32   D_L10_00161958 = 0x20f0d0d0;
f32   D_L10_0016195C = 0.4f;
f32   D_L10_00161960 = 0;
f32   D_L10_00161964 = 0.4f;
f32   D_L10_00161968 = 0.3f;
s32   D_L10_00161978 = 0x3c;
f32   D_L10_00161984 = 0;
f32   D_L10_00161988 = 0.3f;
int   D_L12_00161A88 = 7;
int   D_L12_00161A8C = 8;
int   D_L12_00161A90 = 0xf;
float D_L12_00161A58 = 2.4e+02f;
float D_L12_00161A60 = 1e+01f;
int   D_L12_00161A70 = 0x10b0c0c0;
int   D_L12_00161A74 = 0x8708080;
int   D_L12_00161A78 = 0x4f0090f0;
int   D_L12_00161A7C = 0x4f0000f0;
float D_L12_00161A84 = 2.0f;
float D_L12_00161A5C = 2.0f;
float D_L12_00161A64 = 3.2e+02f;
int   D_L12_00161A68 = 0x30e0ffff;
int   D_L12_00161A6C = 0x1080c0c0;
float D_L12_00161A80 = 1.0f;
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

s32 spark_emitter_default_timer_reset_gp __asm__("D_L08_00161F98") = 0x3c;

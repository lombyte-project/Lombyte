/* Shared moby classes: scoutbot, rocket, blarg_paratrooper. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"

#include "rnc/gameplay/entities/moby.h"

#include "qcopy.h"

/* Scoutbot update: hovers at its post, flies to its target and back, calls reinforcements, shoots, dies. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D3DF8), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct Data_3DF8 {
    char pad0[0x29];
    unsigned char b29;
    char pad2A[0x16];
    float vel[4];
    char pad50[8];
    unsigned char b58;
    char pad59;
    unsigned char b5A;
    char pad5B[0x6D];
    short hC8;
    char padCA[6];
    void *pD0;
    char padD4[0x3C];
    char react[0x60];
    float target_pos[4];
    char pad180[0x30];
    Moby *target;
    int f1B4;
    char pad1B8[8];
    float home[4];
    float glow_a[4];
    float glow_b[4];
    float wander_goal[4];
    float f200;
    float f204;
    float f208;
    float f20C;
    float yaw_speed;
    short voice;
    short near_ground;
    Moby *current_target;
    int f21C;
    char f220[0x10];
    char sound_state[0x10];
} Data_3DF8;

extern char D_L08_0016D2E8[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int fast_tween_color() __asm__("FUN_001fa6e0");
extern int FUN_L00_0024f7c8();
extern int FUN_L00_002dc9f0();
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern short D_L08_00161888 __attribute__((sda));
extern void spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
float absolute_float(float input) __asm__("func_001F99C0");
int voice_is_playing(int a, int i) __asm__("FUN_L00_0028d8c0");
s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
void FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
void FUN_L00_0025a120(char *o);
void spawn_explosion(f32 burst, f32 unused_float, f32 owner_effect, f32 owner_effect2, f32 cutoff, f32 scale, f32 flash, void *owner, s32 unused2, void *pos, s32 puff_count, s32 spark_count, s32 shard_count, s32 sound, s32 shake, s32 spray_count, s32 tint, u8 alternate) __asm__("FUN_L00_0025e450");
void FUN_L00_00263ac8(float x, int a, int b, unsigned char *p);
void FUN_L08_002d36e0_c(char *m) __asm__("FUN_L08_002d36e0");
void FUN_L08_002d3878(char *m);
void FUN_L08_002d3d78_c(char *m) __asm__("FUN_L08_002d3d78");
void FUN_L08_002d4108_c(char *arg) __asm__("FUN_L08_002d4108");
extern void sound_3DF8(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern float distance_3DF8(void *, void *) __asm__("FUN_001f9b48");
extern void wake_3DF8(void *) __asm__("FUN_L00_0025a120");
extern float randangle_3DF8(void) __asm__("FUN_00213308");
extern void zero_3DF8(void *) __asm__("FUN_001f99f8");
extern int randomf_3DF8(float) __asm__("FUN_001fa6d0");
extern void animate_3DF8(void *, int, int, int) __asm__("FUN_00212f90");
extern float ground_3DF8(void *, int, float) __asm__("FUN_00213508");
extern float rangef_3DF8(float, float) __asm__("FUN_002132a8");
extern float sine_3DF8(float) __asm__("FUN_001f9dc8");
extern float cosine_3DF8(float) __asm__("FUN_001f9de0");
extern void vadd_3DF8(void *, void *, void *) __asm__("FUN_001f9a10");
extern float angle_3DF8(float, float) __asm__("FUN_001f9e90");
extern void turn_3DF8(void *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern float anglediff_3DF8(float, float) __asm__("FUN_001fa688");
extern float fast_distance_3DF8(void *, void *) __asm__("FUN_001f9b80");
extern float fabs_3DF8(float) __asm__("FUN_001f99c0");
extern int sound_id_3DF8(int, int, void *) __asm__("FUN_0022da68");
extern float animframe_3DF8(void *) __asm__("FUN_0020c9e0");
extern void draw_3DF8(void *, void *) __asm__("FUN_001f4600");
extern int animdone_3DF8(void *, float) __asm__("FUN_00214cc8");
extern void shoot_3DF8(void *, void *, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern int sound_busy_3DF8(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L00_002dc9f0(void *, void *);
extern void explode_3DF8(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void delete_3DF8(void *) __asm__("FUN_0020c828");
extern int colorlerp_3DF8(int, int, float) __asm__("FUN_001fa6e0");
extern void joint_3DF8(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L08_002d3878();
extern void FUN_L08_002d4108();
extern char hero_pos_3DF8[] __asm__("D_L08_001675C0");
extern float frame_3DF8 __asm__("D_0015ED6C");
extern float dt_3DF8 __asm__("D_0015ED70");

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
    float target_pos[4];
    float offset[4];
    float shot_pos[4];

    sound_3DF8(3.6f, moby, 2, data->sound_state);
    if (moby->visible != 0 && distance_3DF8(&moby->pos, hero_pos_3DF8) < 28.0f) {
        wake_3DF8(moby);
        moby->shadow_range = 0x16;
    }
    FUN_L08_002d36e0_c((char *)moby);
    data->current_target = data->target;
    qcopy(target_pos, data->target_pos);
    switch (moby->state) {
    case SCOUTBOT_STATE_INITIALIZE:
        if (data->f21C == -1) {
            delete_3DF8(moby);
            return;
        }
        moby->flags |= 0x1000;
        moby->rot.z = randangle_3DF8();
        data->f200 = 4.0f;
        data->f204 = randangle_3DF8();
        data->f208 = 0.5f;
        zero_3DF8(data->vel);
        data->f20C = frame_3DF8 * 2.0943952f;
        data->b58 = randomf_3DF8(15.0f);
        data->b5A = randomf_3DF8(9.6f);
        data->pD0 = &D_L08_00161888;
        if (moby->prev_seq != 0) {
            animate_3DF8(moby, 0, 0, 0);
        }
        moby->state = 1;
        qcopy(data->home, &moby->pos);
        if (ground_3DF8(&moby->pos, 0, 0.5f) > moby->pos.z - 10.0f) {
            data->home[2] = ground_3DF8(&moby->pos, 0, 0.5f);
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
                animate_3DF8(moby, 2, 0, 6);
            }
            moby->state = 2;
        } else if (moby->anim_flags & 2) {
            float heading = randangle_3DF8();
            float radius = rangef_3DF8(0.0f, 3.0f);

            offset[0] = sine_3DF8(heading) * radius;
            offset[1] = cosine_3DF8(heading) * radius;
            offset[2] = 0.0f;
            vadd_3DF8(data->wander_goal, data->home, offset);
            if (moby->prev_seq != 2) {
                animate_3DF8(moby, 2, 0, 6);
            }
            moby->state = 7;
        }
        break;
    case SCOUTBOT_STATE_MOVE_TO_WANDER_GOAL: {
        float heading = angle_3DF8(data->wander_goal[0] - moby->pos.x, data->wander_goal[1] - moby->pos.y);

        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        if (anglediff_3DF8(moby->rot.z, heading) < 0.7853982f) {
            data->vel[0] += sine_3DF8(moby->rot.z) * (dt_3DF8 * 12.0f);
            data->vel[1] += cosine_3DF8(moby->rot.z) * (dt_3DF8 * 12.0f);
        }
        if (fast_distance_3DF8(&moby->pos, data->wander_goal) < 0.5f) {
            if (moby->prev_seq != 0) {
                animate_3DF8(moby, 0, 0, 3);
            }
            moby->state = 1;
        }
        break;
    }
    case SCOUTBOT_STATE_APPROACH_TARGET: {
        float heading = angle_3DF8(target_pos[0] - moby->pos.x, target_pos[1] - moby->pos.y);

        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        if (anglediff_3DF8(moby->rot.z, heading) < 0.7853982f) {
            data->vel[0] += sine_3DF8(moby->rot.z) * (dt_3DF8 * 12.0f);
            data->vel[1] += cosine_3DF8(moby->rot.z) * (dt_3DF8 * 12.0f);
        }
        if (fast_distance_3DF8(&moby->pos, &data->current_target->pos) < 3.0f && fabs_3DF8(data->current_target->pos.z - moby->pos.z) < 2.0f) {
            data->voice = sound_id_3DF8(0, 0, moby);
            moby->state = 6;
        } else if (fast_distance_3DF8(target_pos, data->home) > 15.0f || fabs_3DF8(data->home[2] - target_pos[2]) > 4.0f) {
            data->f200 = 4.0f;
            data->f208 = 0.5f;
            if (moby->prev_seq != 2) {
                animate_3DF8(moby, 2, 0, 6);
            }
            moby->state = 3;
        }
        break;
    }
    case SCOUTBOT_STATE_RETURN_HOME: {
        float heading = angle_3DF8(data->home[0] - moby->pos.x, data->home[1] - moby->pos.y);

        data->f200 = 4.0f;
        data->f208 = 0.5f;
        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        if (anglediff_3DF8(moby->rot.z, heading) < 0.7853982f) {
            data->vel[0] += sine_3DF8(moby->rot.z) * (dt_3DF8 * 12.0f);
            data->vel[1] += cosine_3DF8(moby->rot.z) * (dt_3DF8 * 12.0f);
        }
        if (fast_distance_3DF8(&moby->pos, data->home) < 1.0f && fabs_3DF8(data->home[2] - moby->pos.z) >= 3.9f) {
            if (moby->prev_seq != 0) {
                animate_3DF8(moby, 0, 0, 3);
            }
            moby->state = 1;
        }
        break;
    }
    case SCOUTBOT_STATE_ATTACK: {
        float heading = angle_3DF8(target_pos[0] - moby->pos.x, target_pos[1] - moby->pos.y);
        float frame;

        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        frame = animframe_3DF8(moby);
        if (moby->seq == moby->prev_seq && frame >= 9.0f && frame <= 14.0f) {
            float charge = (frame - 9.0f) / 5.0f;
            float spin;

            data->glow_a[3] = charge / 3.0f;
            data->glow_b[3] = (charge * 2.0f) / 3.0f;
            spin = randangle_3DF8();
            data->glow_a[1] = cosine_3DF8(spin) * 0.25f * charge;
            data->glow_a[2] = sine_3DF8(spin) * 0.25f * charge;
            data->glow_b[1] = cosine_3DF8(spin) * 0.25f * charge;
            data->glow_b[2] = sine_3DF8(spin) * 0.25f * charge;
            draw_3DF8(FUN_L08_002d3878, moby);
        }
        if (animdone_3DF8(moby, 14.0f)) {
            if (fast_distance_3DF8(&data->target->pos, &moby->pos) < 3.0f && fabs_3DF8(data->current_target->pos.z - moby->pos.z) < 1.0f) {
                offset[0] = sine_3DF8(moby->rot.z) * 0.2f;
                offset[1] = cosine_3DF8(moby->rot.z) * 0.2f;
                offset[2] = 0.0f;
                qcopy(shot_pos, target_pos);
                shot_pos[2] += 0.75f;
                shoot_3DF8(data->current_target, moby, 1, shot_pos, offset, 1.0f);
            }
            data->f200 = 4.0f;
            data->f208 = 0.5f;
            if (moby->prev_seq != 2) {
                animate_3DF8(moby, 2, 0, 3);
            }
            moby->state = 3;
        }
        break;
    }
    case SCOUTBOT_STATE_WAIT_FOR_VOICE: {
        float heading = angle_3DF8(target_pos[0] - moby->pos.x, target_pos[1] - moby->pos.y);

        turn_3DF8(moby, heading, &data->yaw_speed, 0.01f, 0.3f, 0.05f);
        if (sound_busy_3DF8(moby, data->voice) == 0) {
            if (moby->prev_seq != 3) {
                animate_3DF8(moby, 3, 0, 3);
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
                animate_3DF8(moby, 2, 0, 3);
            }
            data->hC8 = 0;
        }
        break;
    case SCOUTBOT_STATE_EXPLODE_AND_REMOVE:
        qcopy(offset, &moby->pos);
        offset[2] += 1.0f;
        sound_id_3DF8(1, 0, moby);
        explode_3DF8(moby, data->vel, offset, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, -1, 1.0f, 0.0f, 0, 1, -1, 0);
        delete_3DF8(moby);
        return;
    }
    FUN_L08_002d3d78_c((char *)moby);
    if (moby->state != 5 && (signed char)moby->state >= 0) {
        int col = 0x7F20FF20;

        if (moby->state == 4 || moby->state == 6) {
            col = 0x7F2828A0;
        }
        moby->glow_color = colorlerp_3DF8(moby->glow_color, col, 0.1f);
        joint_3DF8(moby, 3, data->f220);
        draw_3DF8(FUN_L08_002d4108, moby);
    }
}
/* Takes hits (dying at 0 health), then follows the moby's path and gives up on it when too far off. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D49E8), where it is exact; names translated to the US level program. */

extern char *D_L08_001B0CB0[];
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int FUN_L00_0025ff38();
extern void FUN_L00_00257470(void *, int, int);
float absolute_float(float input) __asm__("func_001F99C0");
extern int func_L00_00260FB0_f(char *, void *, float, int, int, void *,
                               int) __asm__("FUN_L00_0025ff38");

void FUN_L08_002d36e0(char *m) {
    char *d = *(char **)(m + 0x78);
    int hit;
    float dmg;
    int idx;
    dmg = 0.0f;
    idx = hit_reaction(m, find_moby_hit(m, 0x330000, 0), d + 0x20, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && ((unsigned char *)m)[0x20] != 0x63) {
        *(float *)(d + 0x20) -= dmg;
        if (*(float *)(d + 0x20) <= 0.0f)
            idx = 1;
        if (idx < 12) {
            if (idx >= 0) {
                FUN_L00_00257470(m, 0, -1);
                m[0x20] = 0x63;
            }
        }
    }
    ((unsigned char *)m)[0xA4] = 0xFF;
    if (((unsigned char *)m)[0x20] != 0 && ((unsigned char *)m)[0x20] != 4) {
        char *e = D_L08_001B0CB0[*(int *)(d + 0x21C)];
        if (func_L00_00260FB0_f(m, d + 0x170, 14.0f, 0, 0, e + 0x10, *(int *)e) != 2) {
            if (14.0f < vector_distance(d + 0x1C0, d + 0x170)) {
                *(int *)(d + 0x1B4) = 2;
            } else if (3.0f < absolute_float(*(float *)(d + 0x1C8) - *(float *)(d + 0x178))) {
                *(int *)(d + 0x1B4) = 2;
            }
        }
    }
    if (*(int *)(d + 0x1B0) == 0) {
        *(int *)(d + 0x1B0) = ((int)hero.moby);
    }
}
typedef struct { float v[4]; } Vec_002d3878 __attribute__((aligned(16)));
typedef struct {
    Vec_002d3878 vertices[4];
    u32 color[4];
    float uv[8];
    u64 tag[4];
} Packet_002d3878 __attribute__((aligned(16)));
extern u64 FUN_001f44b8_002d3878(int) __asm__("FUN_001f44b8");
extern void get_moby_position_002d3878(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_002d3878(float, void *) __asm__("FUN_L00_0025f730");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void clear_vector_002d3878(void *) __asm__("FUN_001f99f8");
extern float vector_length_xy_002d3878(void *) __asm__("FUN_001f9b20");
extern float angle_atan2_002d3878(float, float) __asm__("FUN_001f9e90");
extern void make_matrix_002d3878(void *, void *) __asm__("FUN_001fa050");
extern void FUN_001f7d30_002d3878(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L08_00161894_002d3878 __asm__("D_L08_00161894") __attribute__((section(".data")));
extern u32 D_00151878_002d3878 __asm__("D_00151878") __attribute__((sda));
extern u32 D_0015187C_002d3878 __asm__("D_0015187C") __attribute__((sda));
extern u32 D_00151880_002d3878 __asm__("D_00151880") __attribute__((sda));
extern u32 D_00151884_002d3878 __asm__("D_00151884") __attribute__((sda));
void FUN_L08_002d3878(char *moby);
void FUN_L08_002d3878(char *moby) {
    char *data = *(char **)(moby + 0x78);
    Packet_002d3878 pk;
    Vec_002d3878 matrix[3], direction, pos, delta, angles;
    float length;
    u64 tag;
    Vec_002d3878 *points;
    char *source0;
    char *source1;
    float *vertex;
    int i;
    tag = FUN_001f44b8_002d3878(14);
    pk.color[0] = D_00151878_002d3878;
    pk.color[1] = D_0015187C_002d3878;
    pk.color[2] = D_00151880_002d3878;
    pk.color[3] = D_00151884_002d3878;
    pk.tag[1] = tag;
    pk.tag[2] = 0xff9000000260ULL;
    pk.tag[0] = 5;
    pk.tag[3] = 0x8000000048ULL;
    pk.uv[0] = 0.0f; pk.uv[1] = 0.0f;
    pk.uv[2] = 1.0f; pk.uv[3] = 0.0f;
    pk.uv[4] = 0.0f; pk.uv[5] = 1.0f;
    pk.uv[6] = 1.0f; pk.uv[7] = 1.0f;
    get_moby_position_002d3878(moby, 2, &pos);
    fast_vec_sub(&delta, *(char **)(data + 0x218) + 0x10, &pos);
    scale_vector_002d3878(3.0f, &delta);
    delta.v[2] = (*(float *)(*(char **)(data + 0x218) + 0x18) + 0.25f) - pos.v[2];
    delta.v[3] = 1.0f;
    length = fast_vec_length(&delta);
    clear_vector_002d3878(&angles);
    angles.v[1] = -angle_atan2_002d3878(vector_length_xy_002d3878(&delta), delta.v[2]);
    angles.v[2] = angle_atan2_002d3878(delta.v[0], delta.v[1]);
    make_matrix_002d3878(matrix, &angles);
    qcopy(&direction, &pos);
    direction.v[3] = 1.0f;

    clear_vector_002d3878(&pk.vertices[0]);
    pk.vertices[0].v[2] = D_L08_00161894_002d3878;
    pk.vertices[0].v[3] = 1.0f;
    qcopy(&pk.vertices[1], data + 0x1d0);
    pk.vertices[1].v[0] = length * *(float *)(data + 0x1dc);
    pk.vertices[1].v[2] += -0.25f;
    pk.vertices[1].v[3] = 1.0f;
    clear_vector_002d3878(&pk.vertices[2]);
    pk.vertices[2].v[2] = 0.0f; pk.vertices[2].v[3] = 1.0f;
    qcopy(&pk.vertices[3], data + 0x1d0);
    pk.vertices[3].v[0] = length * *(float *)(data + 0x1dc);
    pk.vertices[3].v[2] += 0.25f; pk.vertices[3].v[3] = 1.0f;
    FUN_001f7d30_002d3878(pk.vertices, matrix, 1);
    {
        for (i = 1; i < 4; i += 2) {
            if (i < 2) { pk.vertices[i].v[1] += -0.25f; pk.vertices[i].v[2] += 0.25f; }
            else { pk.vertices[i].v[1] += 0.25f; pk.vertices[i].v[2] += -0.25f; }
        }
    }
    FUN_001f7d30_002d3878(pk.vertices, matrix, 1);

    qcopy(&pk.vertices[0], data + 0x1d0);
    pk.vertices[0].v[0] = length * *(float *)(data + 0x1dc);
    pk.vertices[0].v[2] += -0.25f; pk.vertices[0].v[3] = 1.0f;
    qcopy(&pk.vertices[1], data + 0x1e0);
    pk.vertices[1].v[0] = length * *(float *)(data + 0x1ec);
    pk.vertices[1].v[2] += -0.25f; pk.vertices[1].v[3] = 1.0f;
    qcopy(&pk.vertices[2], data + 0x1d0);
    pk.vertices[2].v[0] = length * *(float *)(data + 0x1dc);
    pk.vertices[2].v[2] += 0.25f; pk.vertices[2].v[3] = 1.0f;
    qcopy(&pk.vertices[3], data + 0x1e0);
    pk.vertices[3].v[0] = length * *(float *)(data + 0x1ec);
    pk.vertices[3].v[2] += 0.25f; pk.vertices[3].v[3] = 1.0f;
    FUN_001f7d30_002d3878(pk.vertices, matrix, 1);
    {
        float *vertex;
        for (i = 0, vertex = pk.vertices[0].v; i < 4; i++, vertex += 4) {
            if (i < 2) { vertex[1] += -0.25f; vertex[2] += 0.25f; }
            else { vertex[1] += 0.25f; vertex[2] += -0.25f; }
        }
    }
    FUN_001f7d30_002d3878(pk.vertices, matrix, 1);

    qcopy(&pk.vertices[0], data + 0x1e0);
    pk.vertices[0].v[0] = length * *(float *)(data + 0x1ec);
    pk.vertices[0].v[2] += -0.25f; pk.vertices[0].v[3] = 1.0f;
    clear_vector_002d3878(&pk.vertices[1]);
    pk.vertices[1].v[0] = length * *(float *)(data + 0x1dc) * 3.0f;
    pk.vertices[1].v[2] += D_L08_00161894_002d3878; pk.vertices[1].v[3] = 1.0f;
    qcopy(&pk.vertices[2], data + 0x1e0);
    pk.vertices[2].v[0] = length * *(float *)(data + 0x1ec);
    pk.vertices[2].v[2] += 0.25f; pk.vertices[2].v[3] = 1.0f;
    clear_vector_002d3878(&pk.vertices[3]);
    pk.vertices[3].v[0] = length * *(float *)(data + 0x1dc) * 3.0f;
    pk.vertices[3].v[2] = 0.0f; pk.vertices[3].v[3] = 1.0f;
    FUN_001f7d30_002d3878(pk.vertices, matrix, 1);
    {
        float *vertex;
        for (i = 0, vertex = pk.vertices[0].v; i < 4; i += 2, vertex += 8) {
            if (i < 2) { vertex[1] += -0.25f; vertex[2] += 0.25f; }
            else { vertex[1] += 0.25f; vertex[2] += -0.25f; }
        }
    }
    FUN_001f7d30_002d3878(pk.vertices, matrix, 1);
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Steers a moby along its path state and lifts it clear of the ground. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D5080), where it is exact; names translated to the US level program. */

extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float D_L08_00174670[];
extern float fast_sin(float) __asm__("func_001F9DE0");
extern int FUN_L08_002025d0(void *, int, int, void *, int);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

void FUN_L08_002d3d78(char *m) {
    char *s = *(char **)(m + 0x78);
    float f = vector_length_xyz((float *)(s + 0x40)) - frame_time_sq * 6.0f;
    float lim = frame_time * 3.0f;
    float t;
    float d;
    float tmp[4];
    float *v;
    if (lim < f) {
        f = lim;
    } else if (f < 0.0f) {
        f = 0.0f;
    }
    v = (float *)(s + 0x40);
    normalize_vector_xyz(v, v, f);
    add_vector_xyz((float *)(m + 0x10), (float *)(m + 0x10), v);
    *(float *)(s + 0x204) = fast_add_rotations(*(float *)(s + 0x204), *(float *)(s + 0x20C));
    t = *(float *)(s + 0x1C8) + *(float *)(s + 0x208) * fast_sin(*(float *)(s + 0x204));
    if (*(short *)(s + 0x216))
        t += *(float *)(s + 0x200);
    d = t - *(float *)(m + 0x18);
    {
        float l2 = frame_time * 3.0f;
        if (l2 < d) {
            d = l2;
        } else {
            l2 = -l2;
            if (d < l2)
                d = l2;
        }
    }
    *(float *)(s + 0x48) = d;
    qcopy(tmp, (float *)(m + 0x10));
    tmp[2] += 0.5f;
    if (FUN_L08_002025d0(tmp, 0x190, 0, m, 0)) {
        float z;
        subtract_vector_xyz(tmp, D_L08_00174670, (float *)(m + 0x10));
        z = frame_time * 3.0f;
        if (z < tmp[2]) {
            tmp[2] = z;
        } else if (tmp[2] < -z) {
            tmp[2] = -z;
        }
        add_vector_xyz((float *)(m + 0x10), (float *)(m + 0x10), tmp);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002D5358), where it is exact; names translated to the US level program. */

extern void *FUN_L00_002dbe20(void *);
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void *FUN_L08_002d4050(char *arg) {
    void *result = FUN_L00_002dbe20(arg);
    char *obj = *(char **)(arg + 0x78);
    arg[0x20] = 3;
    *(float *)(obj + 0x200) = 4.0f;
    *(float *)(obj + 0x208) = 0.5f;
    if ((unsigned char)arg[0x53] != 2) {
        blend_moby_animation(arg, 2, 0, 3);
    }
    *(short *)(obj + 0xC8) = 0;
    return result;
}
typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    u64 a, b, c, d;
} Pk_002d4108;
extern u8 D_L08_001675C0_002d4108[] __asm__("D_L08_001675C0") __attribute__((section(".data")));
u64 FUN_001f44b8_002d4108(s32) __asm__("FUN_001f44b8");
void FUN_001f9a28_002d4108(void *, void *, void *) __asm__("FUN_001f9a28");
void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9d20_002d4108(void *, void *, void *) __asm__("FUN_001f9d20");
void FUN_001f7d30_002d4108(void *, s32, s32) __asm__("FUN_001f7d30");

extern float D_L08_001D1D80[4][4] __attribute__((section(".data")));
extern float D_L08_00161898 __attribute__((sda));
extern float D_L08_0016189C __attribute__((sda));
void FUN_L08_002d4108(char *arg);
void FUN_L08_002d4108(char *arg) {
    char *data = *(char **)(arg + 0x78);
    Pk_002d4108 pk;
    float up[4];
    float M[4][4];
    float t[4];
    s32 i;
    OvlQuad z = 0;
    u32 col;

    *(OvlQuad *)up = z;
    up[2] = 1.0f;
    up[3] = 1.0f;
    pk.b = FUN_001f44b8_002d4108(0xB);
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
    FUN_001f9a28_002d4108(M[0], D_L08_001675C0_002d4108, M[3]);
    fast_vec_normalize(M[0], M[0], 1.0f);
    fast_vec_cross(M[1], M[0], up);
    fast_vec_normalize(M[1], M[1], 1.0f);
    fast_vec_cross(M[2], M[1], M[0]);
    fast_vec_scale(t, M[0], D_L08_00161898);
    fast_vec_add(M[3], M[3], t);
    col = (*(u32 *)(arg + 0x90) & 0xFFFFFF) | 0x40000000;
    pk.col[3] = col;
    pk.col[2] = col;
    pk.col[1] = col;
    pk.col[0] = col;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(pk.m[i], D_L08_001D1D80[i], D_L08_0016189C);
        FUN_001f9d20_002d4108(pk.m[i], pk.m[i], M);
    }
    FUN_001f7d30_002d4108(&pk, 0, 0);
}
extern char *create_moby_002e7eb0(int) __asm__("FUN_0020c4f8");
extern void set_moby_color_002e7eb0(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void update_moby_002e7eb0(void *) __asm__("FUN_L00_00250df8");
extern float vector_length_xy_002e7eb0(void *) __asm__("FUN_001f9b20");
extern float angle_atan2_002e7eb0(float, float) __asm__("FUN_001f9e90");
extern void make_matrix_002e7eb0(void *, void *) __asm__("FUN_001fa050");
extern float random_angle_002e7eb0(void) __asm__("FUN_00213308");
extern float random_float_between_002e7eb0(float, float) __asm__("FUN_002132a8");
extern float fast_cos_002e7eb0(float) __asm__("FUN_001f9dc8");
extern float fast_sin_002e7eb0(float) __asm__("FUN_001f9de0");
extern void normalize_vector_xyz_002e7eb0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void add_vector_xyz_002e7eb0(void *, void *, void *) __asm__("FUN_001f9a10");
extern int random_integer_002e7eb0(int, int) __asm__("FUN_L00_00257b90");
extern void *spawn_particle_002e7eb0(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern int truncate_float_to_s32_002e7eb0(float) __asm__("FUN_001fa6d0");
extern float FUN_L08_00211a20(float);
extern int scale_game_frames_int_002e7eb0(int) __asm__("FUN_001f96f8");
extern void FUN_00218888_002e7eb0(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int D_L08_0015F5CC __asm__("D_L08_0015F5CC");
typedef union { float float_value; int int_value; } Data_002e7eb0;
/* Retail inherits GP 0x166c00: these scalar labels preserve its GP-relative loads.
   Keep the existing timer name while binding it to the checked level-08 address. */
extern int D_L08_00161D08_gp __asm__("D_L08_00161D08") __attribute__((sda));
extern int D_L08_00161D0C_gp __asm__("D_L08_00161D0C") __attribute__((sda));
extern int D_L08_00161D10_gp __asm__("D_L08_00161D10") __attribute__((sda));
typedef struct { unsigned int words[4]; } Vec_002e7eb0 __attribute__((aligned(16)));

extern float D_L08_00161CD8 __attribute__((sda));
extern float D_L08_00161CE0 __attribute__((sda));
extern int D_L08_00161CF0 __attribute__((sda));
extern int D_L08_00161CF4 __attribute__((sda));
extern int D_L08_00161CF8 __attribute__((sda));
extern int D_L08_00161CFC __attribute__((sda));
extern float D_L08_00161D04 __attribute__((sda));
extern float D_L08_00161D14 __attribute__((sda));
extern float D_L08_00161D18 __attribute__((sda));
char *FUN_L08_002e7eb0(int owner, float *position, float *rotation, int value) {
    float position_copy[4] __attribute__((aligned(16)));
    float rotation_copy[4] __attribute__((aligned(16)));
    float matrix[4][4] __attribute__((aligned(16)));
    float velocity[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    float angle, magnitude;
    int i, a, b, c;
    char *moby;
    char *data;
    float * volatile particle_position_ptr;
    float *velocity_ptr, *offset_ptr, *matrix_row_ptr;

    *(u128 *)position_copy = *(u128 *)position;
    *(u128 *)rotation_copy = *(u128 *)rotation;
    position = position_copy;
    rotation = rotation_copy;
    moby = create_moby_002e7eb0(0x1ca);
    if (moby != 0) {
        data = *(char **)(moby + 0x78);
        *(int *)(data + 0x14) = owner;
        *(int *)(data + 0x10) = value;
        set_moby_color_002e7eb0(moby, 0xa0, 0xa0, 0xa0);
        *(volatile int *)(moby + 0x40) = 0;
        *(unsigned char *)(moby + 0x30) = 0xff;
        *(short *)(moby + 0x32) = 0xff;
        *(unsigned char *)(moby + 0x31) = 1;
        *(float *)(moby + 0x44) = -angle_atan2_002e7eb0(vector_length_xy_002e7eb0(rotation_copy), rotation[2]);
        *(float *)(moby + 0x48) = angle_atan2_002e7eb0(rotation[0], rotation[1]);
        qcopy(moby + 0x10, position_copy);
        qcopy(data, rotation_copy);
        if (D_L08_0015F5CC & 1)
            *(volatile int *)(data + 0x1c) = 1;
        else
            *(volatile int *)(data + 0x1c) = -1;
        make_matrix_002e7eb0(matrix, moby + 0x40);
        i = 0;
        if ((float)i < D_L08_00161CE0) {
            /* Retail intentionally uses the matrix third row as the particle input. */
            particle_position_ptr = matrix[2];
            velocity_ptr = velocity;
            offset_ptr = offset;
            matrix_row_ptr = matrix[1];
            do {
                angle = random_angle_002e7eb0();
                {
                    float spread = D_L08_00161D04;
                    magnitude = random_float_between_002e7eb0(spread, spread + spread) * frame_time;
                }
                normalize_vector_xyz_002e7eb0(velocity_ptr, particle_position_ptr, magnitude * fast_cos_002e7eb0(angle));
                normalize_vector_xyz_002e7eb0(offset_ptr, matrix_row_ptr, magnitude * fast_sin_002e7eb0(angle));
                add_vector_xyz_002e7eb0(velocity_ptr, velocity_ptr, offset_ptr);
                qcopy(offset_ptr, velocity_ptr);
                velocity[3] = D_L08_00161D14;
                offset[3] = D_L08_00161D18;
                i++;
                spawn_particle_002e7eb0(moby + 0x10, velocity_ptr, D_L08_00161CF8, D_L08_00161CFC,
                    random_integer_002e7eb0(scale_game_frames_int_002e7eb0(10), scale_game_frames_int_002e7eb0(20)), 1, 20000.0f);
                a = truncate_float_to_s32_002e7eb0(FUN_L08_00211a20(random_float_between_002e7eb0((float)D_L08_00161D08_gp, (float)D_L08_00161D08_gp * 1.2f)));
                b = truncate_float_to_s32_002e7eb0(FUN_L08_00211a20(random_float_between_002e7eb0((float)D_L08_00161D0C_gp, (float)D_L08_00161D0C_gp * 1.2f)));
                c = truncate_float_to_s32_002e7eb0(FUN_L08_00211a20(random_float_between_002e7eb0((float)D_L08_00161D10_gp, (float)D_L08_00161D10_gp * 1.5f)));
                FUN_00218888_002e7eb0(moby + 0x10, velocity_ptr, offset_ptr, D_L08_00161CF0, D_L08_00161CF4, a, b, c, -1);
            } while ((float)i < D_L08_00161CE0);
        }
        if (current_level_index == 12)
            *(int *)(data + 0x18) = scale_game_frames_int_002e7eb0(120);
        else
            *(int *)(data + 0x18) = truncate_float_to_s32_002e7eb0(FUN_L08_00211a20(D_L08_00161CD8));
        update_moby_002e7eb0(moby);
    }
    return moby;
}
extern void scale_vector_xyz_002e8210(void *, void *, float) __asm__("FUN_001f9a68");
extern float vector_distance_xyz_002e8210(void *, void *) __asm__("FUN_001f9b48");
extern float random_angle_002e8210(void) __asm__("FUN_00213308");
extern float random_float_between_002e8210(float, float) __asm__("FUN_002132a8");
extern float fast_cos_002e8210(float) __asm__("FUN_001f9dc8");
extern float fast_sin_002e8210(float) __asm__("FUN_001f9de0");
extern float scale_game_frames_002e8210(float) __asm__("FUN_L08_00211a20");
extern int truncate_float_to_s32_002e8210(float) __asm__("FUN_001fa6d0");
extern int decrement_timer_002e8210(int *) __asm__("FUN_001f9740");
extern void normalize_vector_xyz_002e8210(void *, void *, float) __asm__("FUN_001f9bf8");
extern int collision_line_002e8210(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void mark_moby_for_removal_002e8210(void *) __asm__("FUN_0020c828");
extern void spawn_particle_002e8210(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void scale_vector_in_place_002e8210(float, void *) __asm__("FUN_L00_0025f730");
extern void FUN_L00_0025e450_002e8210(void *, void *, void *, float, float, int, int, int,
                                         float, float, float, int, float, float, int, int, int, int)
    __asm__("FUN_L00_0025e450");
extern float D_L08_00161CDC __attribute__((sda));
extern float D_L08_00161CE4 __attribute__((sda));
extern int D_L08_00161CE8 __attribute__((sda));
extern int D_L08_00161CEC __attribute__((sda));
extern float D_L08_00161D00 __attribute__((sda));
extern int D_L08_00161D08_gp __asm__("D_L08_00161D08") __attribute__((sda));
extern int D_L08_00161D0C_gp __asm__("D_L08_00161D0C") __attribute__((sda));
extern int D_L08_00161D10_gp __asm__("D_L08_00161D10") __attribute__((sda));
extern float D_L08_00161D1C __attribute__((sda));
extern float D_L08_00161D20 __attribute__((sda));
typedef struct { unsigned int words[4]; } Vec_002e8210 __attribute__((aligned(16)));

extern float D_L08_00174660[4];
void FUN_L08_002e8210(char *moby);
void FUN_L08_002e8210(char *moby) {
    char *position_ptr = moby + 0x10;
    char *data = *(char **)(moby + 0x78);
    float position_copy[4] __attribute__((aligned(16)));
    float position[4] __attribute__((aligned(16)));
    float direction[4] __attribute__((aligned(16)));
    float velocity[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    float angle, magnitude, length, radius, spread;
    int i, a, b, c;
    int *timer_ptr;
    char *target;

    qcopy(position_copy, position_ptr);
    add_vector_xyz(position_ptr, position_ptr, data);
    if (*(float *)(moby + 0x10) < 10.0f || *(float *)(moby + 0x14) < 10.0f ||
        *(float *)(moby + 0x18) < 10.0f || 500.0f < *(float *)(moby + 0x10) ||
        500.0f < *(float *)(moby + 0x14) || 500.0f < *(float *)(moby + 0x18)) {
        mark_moby_for_removal_002e8210(moby);
        return;
    }
    scale_vector_xyz_002e8210(direction, data, -1.0f / D_L08_00161CDC);
    qcopy(position, position_ptr);
    i = 0;
    timer_ptr = (int *)(data + 0x18);
    while ((float)i < D_L08_00161CDC) {
        add_vector_xyz(position, position, direction);
        angle = random_angle_002e8210();
        spread = D_L08_00161D00;
        magnitude = random_float_between_002e8210(0.0f, spread) * frame_time;
        normalize_vector_xyz_002e8210(velocity, moby + 0xe0, magnitude * fast_cos_002e8210(angle));
        normalize_vector_xyz_002e8210(offset, moby + 0xd0, magnitude * fast_sin_002e8210(angle));
        add_vector_xyz(velocity, velocity, offset);
        qcopy(offset, offset);
        velocity[3] = D_L08_00161D1C;
        offset[3] = D_L08_00161D20;
        i++;
        a = truncate_float_to_s32_002e8210(scale_game_frames_002e8210(random_float_between_002e8210(
            (float)D_L08_00161D08_gp, (float)D_L08_00161D08_gp * 1.2f)));
        b = truncate_float_to_s32_002e8210(scale_game_frames_002e8210(random_float_between_002e8210(
            (float)D_L08_00161D0C_gp, (float)D_L08_00161D0C_gp * 1.2f)));
        c = truncate_float_to_s32_002e8210(scale_game_frames_002e8210(random_float_between_002e8210(
            (float)D_L08_00161D10_gp, (float)D_L08_00161D10_gp * 1.5f)));
        spawn_particle_002e8210(position, velocity, offset, D_L08_00161CE8,
                               D_L08_00161CEC, a, b, c, -1);
    }
    *(float *)(moby + 0x40) = fast_add_rotations(*(float *)(moby + 0x40),
        D_L08_00161CE4 * 0.017453292f * frame_time * (float)*(int *)(data + 0x1c));
    target = *(char **)(data + 0x10);
    if (target != 0) {
        if ((unsigned char)target[0x20] == 0xfe) goto clear_target;
        if ((unsigned char)target[0x20] == 0xfd) goto clear_target;
        if (*(short *)(target + 0xa6) != 0x1b3) goto clear_target;
        {
            char *target_data = *(char **)(target + 0x78);
            length = vector_length_xyz(data);
            spread = vector_distance_xyz_002e8210(moby + 0x10, *(char **)(data + 0x10) + 0x10);
            scale_vector_xyz_002e8210(position, target_data + 0x20, spread / length);
            add_vector_xyz(position, position, *(char **)(data + 0x10) + 0x10);
            subtract_vector_xyz(direction, position, moby + 0x10);
            normalize_vector_xyz_002e8210(direction, direction, length);
            subtract_vector_xyz(velocity, direction, data);
            scale_vector_in_place_002e8210(frame_time * 3.7f, velocity);
            add_vector_xyz(data, data, velocity);
            normalize_vector_xyz_002e8210(data, data, length);
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
    if (decrement_timer_002e8210(timer_ptr) == 0) goto collision;
remove:
    mark_moby_for_removal_002e8210(moby);
    return;
collision:
    if (collision_line_002e8210(position_copy, moby + 0x10, 0, moby, position) == 0)
        return;
    qcopy(moby + 0x10, D_L08_00174660);
    if (15.0f < *(float *)(moby + 0x10) && 15.0f < *(float *)(moby + 0x14) &&
        15.0f < *(float *)(moby + 0x18)) {
        radius = 24.0f;
        if (*(float *)(moby + 0x10) - 2.0f < radius) radius = *(float *)(moby + 0x10) - 2.0f;
        if (*(float *)(moby + 0x14) - 2.0f < radius) radius = *(float *)(moby + 0x14) - 2.0f;
        if (*(float *)(moby + 0x18) - 2.0f < radius) radius = *(float *)(moby + 0x18) - 2.0f;
        FUN_L00_0025e450_002e8210(moby, data, moby + 0x10, 3.0f, 3.0f, 0,
            10, 20, 5.0f, 3.0f, 9.0f, -1, 1.0f, radius, 1, 1, -1, 0);
    }
    mark_moby_for_removal_002e8210(moby);
}
#define NOT_SDA

#define MACRO_ADDR

/* Runs the two per-entry update calls over a 13-entry table. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2000), where it is exact; names translated to the US level program. */

extern Pair D_L08_001E8348[];
extern char D_L08_001E8360[];
extern int D_L08_001E2858[];
extern int D_L08_001E8268[];
extern int D_L08_001E82A0[];
extern int D_L08_001E8310[];
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L08_00258090(void *, int, int, void *);

void FUN_L08_002f0b40(int a) {
    int i;
    for (i = 0; i < 13; i++) {
        FUN_L08_00258090(D_L08_001E8360, D_L08_001E82A0[i], D_L08_001E2858[i], &D_L08_001E8348[a]);
        FUN_L00_001fde98(D_L08_001E2858[i], D_L08_001E8268[i], D_L08_001E8310[i], D_L08_001E8360,
                         1);
    }
}
extern Pair D_L08_00161E10[] __attribute__((section(".sdata")));
extern int D_L08_001DDF40[];
extern int D_L08_001E27C8[];
extern int D_L08_001E2798[];
extern int D_L08_001E2828[];
void FUN_L08_002f0c18(int a) {
    int i;
    for (i = 0; i < 11; i++) {
        FUN_L08_00258090(D_L08_001E8360, D_L08_001E27C8[i], D_L08_001DDF40[i], &D_L08_00161E10[a]);
        FUN_L00_001fde98(D_L08_001DDF40[i], D_L08_001E2798[i], D_L08_001E2828[i], D_L08_001E8360,
                         1);
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Scales every RGBA word of a set of colour tables by four factors, optionally using the flat 255 for colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2428), where it is exact; names translated to the US level program. */

extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L08_002f0f68(int **tabs, int n, int *counts, int flat, float r, float g, float b,
                      float a) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < counts[i]; j++) {
            int w = tabs[i][j];
            int c0 = w & 0xFF;
            int c1 = (w & 0xFF00) >> 8;
            int c2 = ((unsigned)w >> 16) & 0xFF;
            int c3 = (unsigned)w >> 24;
            float fr, fg, fb, fa;
            if (flat == 0) {
                fr = r * convert_integer_to_float(c0);
                fg = g * convert_integer_to_float(c1);
                fb = b * convert_integer_to_float(c2);
                fa = a * convert_integer_to_float(c3);
            } else {
                fr = r * 255.0f;
                fg = g * 255.0f;
                fb = b * 255.0f;
                fa = a * convert_integer_to_float(c3);
            }
            tabs[i][j] = (func_001FA898_r(fa) << 24) + (func_001FA898_r(fb) << 16) +
                         (func_001FA898_r(fg) << 8) + func_001FA898_r(fr);
        }
    }
}
/* advance a 2-D wrapped phase by a and b and write scaled sines */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2618), where it is exact; names translated to the US level program. */

extern float frame_delta_time __asm__("D_0015ED7C");
extern float fast_sin(float) __asm__("FUN_001f9de0");

void FUN_L08_002f1158(float *out, float *ph, float a, float b, float scale) {
    ph[0] += a * frame_delta_time;
    if (ph[0] > 1.0f) {
        ph[0] -= 2.0f;
    }
    if (ph[0] < -1.0f) {
        ph[0] += 2.0f;
    }
    ph[1] += b * frame_delta_time;
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
extern float frame_delta_time __asm__("D_0015ED7C");
extern float frame_delta_time __asm__("D_0015ED7C");

extern V2_12a0 D_L08_001DB500[];
void FUN_L08_002f12a0(int idx) {
    D_L08_001E8348_12a0[idx].x += D_L08_001DB500[idx].x * frame_delta_time;
    if (D_L08_001E8348_12a0[idx].x > 1.0f) {
        D_L08_001E8348_12a0[idx].x -= 1.0f;
    }
    if (D_L08_001E8348_12a0[idx].x < -1.0f) {
        D_L08_001E8348_12a0[idx].x += 1.0f;
    }
    D_L08_001E8348_12a0[idx].y += D_L08_001DB500[idx].y * frame_delta_time;
    if (D_L08_001E8348_12a0[idx].y > 1.0f) {
        D_L08_001E8348_12a0[idx].y -= 1.0f;
    }
    if (D_L08_001E8348_12a0[idx].y < -1.0f) {
        D_L08_001E8348_12a0[idx].y += 1.0f;
    }
}
extern float scroll_pos_x[] __asm__("D_L08_00161E10") __attribute__((section(".sdata")));
extern float scroll_pos_y[] __asm__("D_L08_00161E14") __attribute__((section(".sdata")));

extern float D_L08_00161DE0[2] __attribute__((sda));
extern float D_L08_00161DE4[] __attribute__((section(".sdata")));
/* Advances the X and Y scroll offsets of entry `i` by speed * frame step, wrapping them into [-1, 1]. */
void FUN_L08_002f1378(int i) {
    float *p;
    p = &scroll_pos_x[i * 2];
    *p += D_L08_00161DE0[i * 2] * frame_delta_time;
    if (*p > 1.0f) *p -= 1.0f;
    if (*p < -1.0f) *p += 1.0f;
    p = &scroll_pos_y[i * 2];
    *p += D_L08_00161DE4[i * 2] * frame_delta_time;
    if (*p > 1.0f) *p -= 1.0f;
    if (*p < -1.0f) *p += 1.0f;
}
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern void spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

extern s32 D_L08_00161F74 __attribute__((sda));
extern s32 D_L08_00161F78 __attribute__((sda));
extern f32 D_L08_00161F7C __attribute__((sda));
extern f32 D_L08_00161F80 __attribute__((sda));
extern f32 D_L08_00161F84 __attribute__((sda));
extern f32 D_L08_00161F88 __attribute__((sda));
extern s32 D_L08_00161F8C __attribute__((sda));
extern s32 D_L08_00161F90 __attribute__((sda));
extern s32 D_L08_00161F94 __attribute__((sda));
extern s32 D_L08_00161F98 __attribute__((sda));
extern f32 D_L08_00161F9C __attribute__((sda));
extern f32 D_L08_00161FA0 __attribute__((sda));
extern f32 D_L08_00161FA4 __attribute__((sda));
extern f32 D_L08_00161FA8 __attribute__((sda));

void FUN_L08_002f5830(char *m) {
    f32 spark_motion_primary[4], spark_motion_secondary[4], c[4], d[4], e[16];
    char *v;
    s32 spark_frame_count, spark_second_frame_count, spark_third_frame_count;

    v = *(char **)(m + 0x78);
    if (fast_dec_timer((int *)(v + 0x3C)) == 0)
        return;
    if (*(s32 *)(v + 0x38) != 0) {
        *(s32 *)(v + 0x3C) = D_L08_00161F98;
        euler_rotation_matrix(e, m + 0x40);
        qcopy(c, m + 0x10);
        c[0] += random_float_between(-0.05f, 0.05f);
        c[1] += random_float_between(-0.05f, 0.05f);
        c[0] += random_float_between(-0.05f, 0.05f);
        random_float_between(-0.17453292f, 0.17453292f);
        fast_vec_normalize(spark_motion_primary, e,
                     random_float_between(D_L08_00161F80, D_L08_00161F80 * 1.2f) * frame_time);
        fast_vec_normalize(d, e + 4, random_float_between(-D_L08_00161FA4, D_L08_00161FA4) * frame_time);
        fast_vec_add(spark_motion_primary, spark_motion_primary, d);
        spark_motion_primary[2] += random_float_between(D_L08_00161F7C, D_L08_00161F7C * 1.2f) * frame_time;
        qcopy(spark_motion_secondary, spark_motion_primary);
        fast_vec_normalize(spark_motion_secondary, e,
                     random_float_between(D_L08_00161F88, D_L08_00161F88 * 1.5f) * frame_time);
        fast_vec_normalize(d, e + 4, random_float_between(-D_L08_00161FA8, D_L08_00161FA8) * frame_time);
        fast_vec_add(spark_motion_secondary, spark_motion_secondary, d);
        spark_motion_secondary[2] += random_float_between(D_L08_00161F84, D_L08_00161F84 * 1.5f) * frame_time;
        spark_motion_primary[3] = D_L08_00161F9C;
        spark_motion_secondary[3] = D_L08_00161FA0;
        spark_frame_count = truncate_float_to_s32(
            multiply_global_factor_ed64(random_float_between((f32)D_L08_00161F8C, (f32)D_L08_00161F8C * 1.2f)));
        spark_second_frame_count = truncate_float_to_s32(
            multiply_global_factor_ed64(random_float_between((f32)D_L08_00161F90, (f32)D_L08_00161F90 * 1.2f)));
        spark_third_frame_count = truncate_float_to_s32(
            multiply_global_factor_ed64(random_float_between((f32)D_L08_00161F94, (f32)D_L08_00161F94 * 1.2f)));
        spawn_spark(c, spark_motion_primary, spark_motion_secondary, D_L08_00161F74, D_L08_00161F78, spark_frame_count, spark_second_frame_count, spark_third_frame_count, -1);
    } else {
        *(s32 *)(v + 0x3C) = *(s32 *)(v + 0x14);
        euler_rotation_matrix(e, m + 0x40);
        qcopy(c, m + 0x10);
        c[0] += random_float_between(-0.05f, 0.05f);
        c[1] += random_float_between(-0.05f, 0.05f);
        c[2] += random_float_between(-0.05f, 0.05f);
        random_float_between(-0.17453292f, 0.17453292f);
        fast_vec_normalize(spark_motion_primary, e,
                     random_float_between(*(f32 *)(v + 0x18), *(f32 *)(v + 0x18) * 1.2f) *
                         frame_time);
        fast_vec_normalize(d, e + 4,
                     random_float_between(-*(f32 *)(v + 0x30), *(f32 *)(v + 0x30)) * frame_time);
        fast_vec_add(spark_motion_primary, spark_motion_primary, d);
        spark_motion_primary[2] += random_float_between(*(f32 *)(v + 0x1C), *(f32 *)(v + 0x1C) * 1.2f) * frame_time;
        qcopy(spark_motion_secondary, spark_motion_primary);
        fast_vec_normalize(spark_motion_secondary, e,
                     random_float_between(*(f32 *)(v + 0x20), *(f32 *)(v + 0x20) * 1.5f) *
                         frame_time);
        fast_vec_normalize(d, e + 4,
                     random_float_between(-*(f32 *)(v + 0x34), *(f32 *)(v + 0x34)) * frame_time);
        fast_vec_add(spark_motion_secondary, spark_motion_secondary, d);
        spark_motion_secondary[2] += random_float_between(*(f32 *)(v + 0x24), *(f32 *)(v + 0x24) * 1.5f) * frame_time;
        spark_motion_primary[3] = *(f32 *)(v + 0x28);
        spark_motion_secondary[3] = *(f32 *)(v + 0x2C);
        spark_frame_count = truncate_float_to_s32(multiply_global_factor_ed64(
            random_float_between((f32) * (s32 *)(v + 0x08), (f32) * (s32 *)(v + 0x08) * 1.2f)));
        spark_second_frame_count = truncate_float_to_s32(multiply_global_factor_ed64(
            random_float_between((f32) * (s32 *)(v + 0x0C), (f32) * (s32 *)(v + 0x0C) * 1.2f)));
        spark_third_frame_count = truncate_float_to_s32(multiply_global_factor_ed64(
            random_float_between((f32) * (s32 *)(v + 0x10), (f32) * (s32 *)(v + 0x10) * 1.2f)));
        spawn_spark(c, spark_motion_primary, spark_motion_secondary, *(s32 *)(v + 0x00), *(s32 *)(v + 0x04), spark_frame_count, spark_second_frame_count, spark_third_frame_count, -1);
    }
}
/* Paratrooper: initializes its path and steering, patrols, attacks its target,
 * and handles hit recovery and death. Shared by Batalia and Gaspar. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00302618), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x80]; float pos[4]; char pad90[0x108];
    int timer; char pad19c[0x1EE8]; int mode;
} Hero302618;

typedef struct {
    char pad0[0x20];
    float f20;
    short f24;
    char pad26[0x2];
    unsigned char f28;
    unsigned char f29;
    char pad2A[0x4];
    unsigned char f2E;
    char pad2F[0x29];
    unsigned char f58;
    char pad59[0x1];
    unsigned char f5A;
    char pad5B[0x75];
    int fD0;
    char padD4[0x4];
    float fD8;
    float fDC;
    float fE0;
    char padE4[0x14];
    float fF8;
    float fFC;
    char pad100[0xC];
    float f10C;
    float f110;
    float f114;
    char pad118[0x48];
    char * f160;
    int f164;
    char pad168[0x68];
    int f1D0;
    int f1D4;
    char pad1D8[0x2];
    short f1DA;
    char pad1DC[0xC];
    float f1E8;
    int f1EC;
    int f1F0;
    int f1F4;
    char * f1F8;
    int f1FC;
    char pad200[0xC];
    short f20C;
    short f20E;
} Paratrooper302618;

char *FUN_L08_00307190(char *pos, char *dir, int a2, int a3, int a4, int a5);
extern char D_L08_001675C0[];
extern char D_L08_001FC8F0[];
extern char D_L08_001FC630[];
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern f32 random_float_between_c2(f32, f32) __asm__("func_002132A8");
extern float D_L08_0015F580[4] __attribute__((section(".sdata")));
extern float D_L08_0015F5D0;
extern float D_L08_0015F5D4;
extern int coll_line_fix(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int lerp_vector() __asm__("FUN_001f9a40");
extern int collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern int FUN_L00_00257b90(int, int);
extern int FUN_L00_00259888();
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 rand(void);
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern void DebugPrint(char *, ...);
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void blend_moby_animation_c2(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void normalize_vector_xyz_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
float approach_angle(float *cur, float *vel, float target, float lim, float accel, float maxv) __asm__("FUN_L00_0025be00");
int FUN_L00_00258830(char *m, char *v, int flags, float a, float b, float c);
int FUN_L00_00259028(unsigned char *m, char *c, float *tgt, void *out);
int mine_fall(void *moby, void *motion) __asm__("FUN_L00_0025c698");
int FUN_L01_0028b510(void *arg0, void *arg1, float target);
void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9, float *v10, float *v11, float scale, float *v12);
void FUN_L00_0025a120(char *o);
void FUN_L00_0025f730(void *a, float x);
void FUN_L00_00263ac8(float x, int a, int b, unsigned char *p);
void FUN_L01_0026d930(char *p);
void FUN_L08_003024d8(char *m);
extern char D_0013F350[];
extern int level_game_mode __asm__("D_L08_0015F5C4");
extern char *target302618 __asm__("D_L08_00174658") __attribute__((section(".data")));
extern float D_L08_0015F5D4;
extern float speed302618 __asm__("D_L08_001621C4") __attribute__((sda));
extern float accel302618 __asm__("D_L08_001621C0") __attribute__((sda));
extern int delay302618 __asm__("D_L08_001621BC") __attribute__((sda));
extern int hops302618 __asm__("D_L08_001621CC") __attribute__((sda));
extern float melee302618 __asm__("D_L08_001621C8") __attribute__((sda));
extern float height302618 __asm__("D_L08_001621D0") __attribute__((sda));
extern float shove302618 __asm__("D_L08_001621B4") __attribute__((sda));
extern float shove_rate302618 __asm__("D_L08_001621B8") __attribute__((sda));
extern int random302618(void) __asm__("FUN_001160d8");
extern int scale302618(int) __asm__("FUN_001f96f8");
extern int randomf302618(float) __asm__("FUN_001fa6d0");
extern int timer302618(void *) __asm__("FUN_001f9740");
extern float distance302618(void *, void *) __asm__("FUN_001f9b48");
extern float fast_distance302618(void *, void *) __asm__("FUN_001f9b80");
extern float angle302618(float, float) __asm__("FUN_001f9e90");
extern float turn302618(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");
extern void animate302618(void *, int, int, int) __asm__("FUN_00212f90");
extern void delete302618(void *) __asm__("FUN_0020c828");
extern void FUN_L08_003024d8_c(void *) __asm__("FUN_L08_003024d8");
extern void FUN_L00_00263ac8_c(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern void FUN_L00_0025a120_c(void *) __asm__("FUN_L00_0025a120");
extern int hero_anim302618(int) __asm__("FUN_00213260");
extern void log302618(char *, int) __asm__("FUN_001e93b0");
extern void FUN_L01_0026d930_c(void *) __asm__("FUN_L01_0026d930");
extern int FUN_L01_0028b510(void *, void *, float);
extern void FUN_L00_00259028_c(void *, void *, void *, void *) __asm__("FUN_L00_00259028");
extern int range302618(int, int) __asm__("FUN_L00_00257b90");
extern void subtract302618(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float animframe302618(void *) __asm__("FUN_0020c9e0");
extern void lerp302618(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern int ray302618(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern float rangef302618(float, float) __asm__("FUN_002132a8");
extern float scale_float302618(float) __asm__("FUN_001f96b0");
extern void joint302618(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float sine302618(float) __asm__("FUN_001f9dc8");
extern float cosine302618(float) __asm__("FUN_001f9de0");
extern char *FUN_L08_00307190_c(void *, void *, void *, void *, int, int) __asm__("FUN_L08_00307190");
extern void FUN_L00_0025f730(void *, float);
extern void FUN_L00_00258830_c(void *, void *, float, float, float, int) __asm__("FUN_L00_00258830");
extern void vec_scale302618(void *, void *, float) __asm__("FUN_001f9bf8");
extern void hit_init302618(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern int damage302618(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int dying302618(void *, void *) __asm__("FUN_L00_0025c698");
extern void zero302618(void *) __asm__("FUN_001f99f8");
extern void FUN_L00_00257470(void *, int, int);
extern void effect302618(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void *FUN_L00_00263fd8_c(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");

extern char D_L08_001FC600[];
void blarg_paratrooper_update(char *moby) __asm__("FUN_L08_00301158");
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
    Paratrooper302618 *data = (Paratrooper302618 *)(*(char * *)((char *)(moby)+0x78));
    char *path;
    char *target;
    Hero302618 *hero;
    float pos[4] __attribute__((aligned(16)));
    float dir[4] __attribute__((aligned(16)));
    char hit[0x30] __attribute__((aligned(16)));
    if ((*(unsigned char *)((char *)(moby)+0x31)) == 0 && distance302618(moby+0x10,D_L08_001675C0)>40.0f) return;
    FUN_L00_00263ac8_c(2.5f,moby,6,(char *)data+0x170);
    if ((*(unsigned char *)((char *)(moby)+0x31)) != 0 && distance302618(moby+0x10,D_L08_001675C0)<29.0f) {
        FUN_L00_0025a120_c(moby);
        (*(unsigned char *)((char *)(moby)+0x7F))=23;
    }
    if (level_game_mode == 2) return;
    path=0;
    FUN_L08_003024d8_c(moby);
    target = data->f160;
    if(data->f1FC!=-1) path=D_L08_001B0CB0[data->f1FC];
    hero=(Hero302618 *)(((char *)&D_0013F350));
    if(hero->mode == 61 && hero->timer < scale302618(30) &&
       (*(unsigned char *)((char *)(moby)+0x20))!=8 && (*(unsigned char *)((char *)(moby)+0x20))!=10 && (*(unsigned char *)((char *)(moby)+0x20))!=4 && (*(unsigned char *)((char *)(moby)+0x20))!=11 &&
       fast_distance302618(moby+0x10,hero->pos)<25.0f && !hero_anim302618(49)) {
        if((*(unsigned char *)((char *)(moby)+0x53))!=14) animate302618(moby,14,0,scale302618(11));
        (*(unsigned char *)((char *)(moby)+0x20))=8;
    }
    if((*(unsigned char *)((char *)(moby)+0x20))!=0 && data->f2E==2) {
        delete302618(moby);
        if(data->f1F8) delete302618(data->f1F8);
        return;
    }
    switch((*(unsigned char *)((char *)(moby)+0x20))) {
    case PARATROOPER_STATE_INITIALIZE: {
        float turn_rate;
        data->f29=0; data->f28=1; data->f2E=1;
        data->f20=2.0f; data->f24=2;
        data->f58=randomf302618(15.0f);
        data->f5A=randomf302618(12.0f);
        if(data->f1D0==-1) { log302618(D_L08_001FC600,(*(short *)((char *)(moby)+0xB2))); goto delete_moby; }
        if(data->f1D4==-1) { log302618(D_L08_001FC630,(*(short *)((char *)(moby)+0xB2))); goto delete_moby; }
        FUN_L01_0026d930_c((char *)data+0xD0);
        data->fDC=0.5f; data->fD8=0.5f;
        data->fD0=randomf302618(512.0f);
        turn_rate=D_0015ED70*6.981317f;
        data->f110=data->f10C=turn_rate;
        data->f114=D_0015ED6C*8.726646f;
        data->fF8=0.34906584f; data->fFC=0.2f;
        if(random302618()&1) (*(unsigned short *)((char *)(moby)+0x34))|=0x8000;
        qcopy(moby+0x10,D_L08_001B0CB0[data->f1D4]+0x10);
        if(path) {
            int index;
            short direction;
            data->f20C=2*(random302618()&1)-1;
            index=FUN_L01_0028b510(moby+0x10,path,0.0f)+data->f20C;
            direction=data->f20C;
            data->f1F4=index;
            if(index>=(*(int *)((char *)(path)+0))||index<0) {
                data->f20C=-direction;
                data->f1F4=index+2*data->f20C;
            }
            if((*(unsigned char *)((char *)(moby)+0x53))!=2) animate302618(moby,2,0,random302618()%4+7);
            (*(unsigned char *)((char *)(moby)+0x20))=1;
        } else (*(unsigned char *)((char *)(moby)+0x20))=3;
        data->f1DA=0;
        break;
    }
    case PARATROOPER_STATE_PATROL_PATH:
        if(fast_distance302618(moby+0x10,path+0x10+data->f1F4*16)<data->fE0) {
            short direction=data->f20C;
            int index = data->f1F4+direction;
            data->f1F4=index;
            if(index>=(*(int *)((char *)(path)+0))||index<0) {
                data->fE0=0.0f;
                data->f20C=-direction;
                data->f1F4=index+2*data->f20C;
            }
        }
        FUN_L00_00259028_c(moby,(char *)data+0xD0,path+0x10+data->f1F4*16,pos);
        if(target && data->f164!=2 && timer302618((char *)data+0x1F0)) {
            data->f1F4=FUN_L01_0028b510(moby+0x10,D_L08_001B0CB0[data->f1D4],0.0f);
            (*(unsigned char *)((char *)(moby)+0x20))=2;
        }
        break;
    case PARATROOPER_STATE_APPROACH_WAYPOINT: {
        float distance;
        data->f1F4=FUN_L01_0028b510(moby+0x10,D_L08_001B0CB0[data->f1D4],0.0f);
        distance=fast_distance302618(moby+0x10,D_L08_001B0CB0[data->f1D4]+0x10+data->f1F4*16);
        if(distance<1.0f || distance<data->fE0) {
            data->fE0=0.0f; (*(unsigned char *)((char *)(moby)+0x20))=3;
        } else {
            FUN_L00_00259028_c(moby,(char *)data+0xD0,D_L08_001B0CB0[data->f1D4]+0x10+data->f1F4*16,pos);
            if((*(unsigned char *)((char *)(moby)+0x53))!=2) animate302618(moby,2,0,random302618()%4+7);
        }
        break;
    }
    case PARATROOPER_STATE_WAIT_FACING_TARGET: {
        float *rotation=(float *)(moby+0x48);
        float heading;
        float frame;
        heading=angle302618((*(float *)((char *)(target)+0x10))-(*(float *)((char *)(moby)+0x10)),(*(float *)((char *)(target)+0x14))-(*(float *)((char *)(moby)+0x14)));
        turn302618(rotation,heading,(char *)data+0x1E0,D_0015ED70*8.726646f,D_0015ED70*8.726646f,D_0015ED6C*12.566371f);
        if((*(unsigned char *)((char *)(moby)+0x70))&2) {
            int oldanim=(*(unsigned char *)((char *)(moby)+0x53));
            if(oldanim!=(random302618()&1)) {
                int newanim=random302618()&1;
                int ticks=scale302618(7);
                animate302618(moby,newanim,0,ticks+random302618()%7);
            }
        }
        if(hops302618<data->f1DA) {
            data->f1DA=0;
            if((*(unsigned char *)((char *)(moby)+0x53))!=10) {
                int ticks=scale302618(7);
                animate302618(moby,10,2,ticks+range302618(0,scale302618(5)));
            }
            (*(unsigned char *)((char *)(moby)+0x20))=9;
        } else if(target) {
            if(data->f164!=2 && timer302618((char *)data+0x1F0)) {
                qcopy((char *)data+0x1C0,moby+0x10);
                data->f1F4=(data->f1F4+1)&1;
                (*(unsigned char *)((char *)(moby)+0x20))=4;
                subtract302618(pos,D_L08_001B0CB0[data->f1D4]+0x10+data->f1F4*16,moby+0x10);
                if(fast_vec_dot(moby+0xD0,pos)<0.0f) {
                    if((*(unsigned char *)((char *)(moby)+0x53))!=18) animate302618(moby,18,0,scale302618(5));
                } else if((*(unsigned char *)((char *)(moby)+0x53))!=16) animate302618(moby,16,0,scale302618(5));
            } else if(target && fast_distance302618(moby+0x10,target+0x10)<melee302618) {
                (*(unsigned char *)((char *)(moby)+0x20))=7;
                if((*(unsigned char *)((char *)(moby)+0x53))!=13) animate302618(moby,13,0,scale302618(10));
            }
        }
        break;
    }
    case PARATROOPER_STATE_HOP_BETWEEN_WAYPOINTS: {
        float *rotation=(float *)(moby+0x48);
        float heading;
        float frame;
        heading=angle302618((*(float *)((char *)(target)+0x10))-(*(float *)((char *)(moby)+0x10)),(*(float *)((char *)(target)+0x14))-(*(float *)((char *)(moby)+0x14)));
        turn302618(rotation,heading,(char *)data+0x1E0,D_0015ED70*8.726646f,D_0015ED70*8.726646f,D_0015ED6C*12.566371f);
        frame=animframe302618(moby);
        if(frame>12.0f && frame<23.0f && (*(unsigned char *)((char *)(moby)+0x52))==(*(unsigned char *)((char *)(moby)+0x53))) {
            float progress=1.0f-(23.0f-frame)/11.0f;
            float arc;
            if(progress>1.0f) progress=1.0f; else if(progress<0.0f) progress=0.0f;
            lerp302618(pos,(char *)data+0x1C0,D_L08_001B0CB0[data->f1D4]+0x10+data->f1F4*16,progress);
            arc=2.0f*(progress-0.5f);
            pos[2]+=height302618*(1.0f-arc*arc);
            qcopy(moby+0x10,pos);
        } else if((*(unsigned char *)((char *)(moby)+0x70))&2) {
            qcopy(pos,moby+0x10); qcopy(dir,target+0x10);
            pos[2]+=1.0f; dir[2]+=0.75f;
            ray302618(pos,dir,0,moby,0);
            if(hops302618<data->f1DA) {
                data->f1DA=0;
                if((*(unsigned char *)((char *)(moby)+0x53))!=10) {
                    animate302618(moby,10,2,random302618()%4+5);
                }
                (*(unsigned char *)((char *)(moby)+0x20))=9;
            } else if(fast_distance302618(moby+0x10,target+0x10)<melee302618) {
                goto melee_state;
            } else if(target302618!=target) {
                (*(unsigned char *)((char *)(moby)+0x20))=3;
                if((*(unsigned char *)((char *)(moby)+0x53))!=0) animate302618(moby,0,0,scale302618(5));
                data->f1F0=randomf302618(scale_float302618(rangef302618(45.0f,90.0f)));
            } else {
                data->f1E8=1.0f; (*(unsigned char *)((char *)(moby)+0x20))=5; data->f20E=0;
                if((*(unsigned char *)((char *)(moby)+0x53))!=6) animate302618(moby,6,0,scale302618(10));
            }
        }
        break;
    }
    case PARATROOPER_STATE_FIRE_PROJECTILES:
        if(fast_distance302618(moby+0x10,target+0x10)<melee302618) {
            (*(unsigned char *)((char *)(moby)+0x20))=7; data->f1EC=0;
            if((*(unsigned char *)((char *)(moby)+0x53))==13) break;
            goto animate_melee;
        } else if(timer302618((char *)data+0x1EC)) {
            data->f1EC=scale302618(delay302618+randomf302618(D_L08_0015F5D4*1.3f+D_L08_0015F5D0));
            data->f1E8+=accel302618;
            if((D_L08_0015F5D4<0.8f && D_L08_0015F5D0<0.8f)||!((*(unsigned short *)&data->f20E)&1)) {
                char *shot;
                joint302618(data->f1F8,0,pos);
                dir[0]=sine302618((*(float *)((char *)(moby)+0x48)))*(speed302618*D_0015ED6C);
                dir[1]=cosine302618((*(float *)((char *)(moby)+0x48)))*(speed302618*D_0015ED6C);
                dir[2]=0.0f;
                dir[2]=(-1.0f/data->f1E8)*(speed302618*D_0015ED6C);
                shot=FUN_L08_00307190_c(dir,pos,moby,target,scale302618(180),data->f20E);
                if(shot) (*(float *)((char *)(shot)+0x2C))*=1.7f;
            }
            data->f20E=(*(unsigned short *)&data->f20E)+1;
            if(data->f20E>=5) data->f20E=0;
            if(data->f1E8>20.0f) {
                qcopy((char *)data+0x1C0,moby+0x10);
                (*(unsigned short *)&data->f1DA)++;
                data->f1F4=(data->f1F4+1)&1;
                (*(unsigned char *)((char *)(moby)+0x20))=4;
                subtract302618(pos,D_L08_001B0CB0[data->f1D4]+0x10+data->f1F4*16,moby+0x10);
                if(fast_vec_dot(moby+0xD0,pos)<0.0f) {
                    if((*(unsigned char *)((char *)(moby)+0x53))!=18) animate302618(moby,18,0,scale302618(5));
                } else if((*(unsigned char *)((char *)(moby)+0x53))!=16) animate302618(moby,16,0,scale302618(5));
            }
        }
        break;
    case PARATROOPER_STATE_MOVE_TOWARD_FIRST_WAYPOINT: {
        float *rotation=(float *)(moby+0x48);
        float heading;
        float frame;
        heading=angle302618((*(float *)((char *)(target)+0x10))-(*(float *)((char *)(moby)+0x10)),(*(float *)((char *)(target)+0x14))-(*(float *)((char *)(moby)+0x14)));
        turn302618(rotation,heading,(char *)data+0x1E0,D_0015ED70*8.726646f,D_0015ED70*8.726646f,D_0015ED6C*12.566371f);
        frame=animframe302618(moby);
        if(frame>12.0f && frame<23.0f && (*(float *)((char *)(moby)+0x54))==0.0f) {
            subtract302618(pos,D_L08_001B0CB0[data->f1D4]+0x10,moby+0x10);
            FUN_L00_0025f730(pos,D_0015ED6C*24.0f);
            FUN_L00_00258830_c(moby,pos,shove302618,shove_rate302618,0.0f,0);
        } else if((*(unsigned char *)((char *)(moby)+0x70))&2) {
            data->f1F0=randomf302618(scale_float302618(rangef302618(45.0f,90.0f)));
            (*(unsigned char *)((char *)(moby)+0x20))=3;
            if((*(unsigned char *)((char *)(moby)+0x53))!=0) animate302618(moby,0,0,scale302618(10));
        }
        break;
    }
    case PARATROOPER_STATE_MELEE_ATTACK: {
        float *rotation=(float *)(moby+0x48);
        float heading;
        float frame;
        heading=angle302618((*(float *)((char *)(target)+0x10))-(*(float *)((char *)(moby)+0x10)),(*(float *)((char *)(target)+0x14))-(*(float *)((char *)(moby)+0x14)));
        turn302618(rotation,heading,(char *)data+0x1E0,D_0015ED70*8.726646f,D_0015ED70*8.726646f,D_0015ED6C*12.566371f);
        frame=animframe302618(moby);
        if(frame>16.5f && frame<20.0f) {
            joint302618(moby,4,pos);
            subtract302618(dir,pos,moby+0x10);
            dir[2]=0.0f;
            vec_scale302618(dir,dir,1.0f);
            dir[2]=1.0f; pos[2]-=0.25f;
            hit_init302618(hit,moby,1,1.0f,dir);
            damage302618(0.8f,pos,0,moby,hit);
        }
        if((*(unsigned char *)((char *)(moby)+0x70))&2) (*(unsigned char *)((char *)(moby)+0x20))=2;
        break;
    }
    case PARATROOPER_STATE_WAIT_FOR_HERO_TRIGGERED_ANIMATION: case PARATROOPER_STATE_PAUSE_AFTER_HOP_LIMIT:
        if((*(unsigned char *)((char *)(moby)+0x70))&2) {
            (*(unsigned char *)((char *)(moby)+0x20))=3;
            if((*(unsigned char *)((char *)(moby)+0x53))!=0) animate302618(moby,0,0,scale302618(10));
        }
        break;
    case PARATROOPER_STATE_KNOCKBACK_RECOVERY:
        {
            int flags=dying302618(moby,(char *)data+0x70);
            if(flags&0x40) {
                if(fast_distance302618(moby+0x10,target+0x10)<melee302618) {
melee_state:
                    (*(unsigned char *)((char *)(moby)+0x20))=7;
                    if((*(unsigned char *)((char *)(moby)+0x53))!=13) {
animate_melee:
                        animate302618(moby,13,0,random302618()%4+8);
                    }
                } else {
                    data->f1E8=1.0f; (*(unsigned char *)((char *)(moby)+0x20))=5; data->f20E=0;
                    if((*(unsigned char *)((char *)(moby)+0x53))!=6) {
                        int ticks=scale302618(10);
                        animate302618(moby,6,0,ticks+range302618(0,scale302618(7)));
                    }
                }
            } else if(flags&0x120) {
delete_moby:
                delete302618(moby);
            }
        }
        break;
    case PARATROOPER_STATE_DEATH_EFFECTS_AND_REMOVE:
        if(dying302618(moby,(char *)data+0x70)&0x160) {
            float *color;
            char *rotation;
            zero302618(pos);
            color=D_L08_0015F580;
            FUN_L00_00257470(moby,0,-1);
            effect302618(moby,moby+0x10,0,0.5f,13.0f);
            rotation=moby+0x40;
            FUN_L00_00263fd8_c(moby,0x69C,moby+0x10,rotation,0,0,0.0f,color,color,color);
            FUN_L00_00263fd8_c(moby,0x69D,moby+0x10,rotation,0,0,0.0f,color,color,color);
            FUN_L00_00263fd8_c(moby,0x69E,moby+0x10,rotation,0,0,0.0f,color,color,color);
            delete302618(moby);
            if(data->f1F8) delete302618(data->f1F8);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00303998), where it is exact; names translated to the US level program. */
extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
extern void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void update_moby_3024d8(void *) __asm__("FUN_L00_00250df8");
extern int hit_3024d8(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern void trooper_knockback_arc(void *, void *, float *, float *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void FUN_L00_00260a88(void *, int, int, int, int);
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int fast_dec_timer_s16(short *) __asm__("FUN_001f9770");
extern int scan_target_3024d8(void *, void *, float, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float D_L08_00162180 __attribute__((sda));
extern float D_L08_00162184 __attribute__((sda));
extern float D_L08_0016218C __attribute__((sda));
extern float D_L08_00162190 __attribute__((sda));
extern float D_L08_00162194 __attribute__((sda));
extern float D_L08_001621B0 __attribute__((sda));

void FUN_L08_003024d8(char *moby) {
    char *data, *target, *child, *attached, *chased;
    float pos[4] __attribute__((aligned(16)));
    float matrix[16] __attribute__((aligned(16)));
    float damage, knock_angle, second_knock_angle;
    int hit_state, reaction, turn, state;
    float angle, distance, height, dx, dy;

    data = *(char **)(moby + 0x78);
    if (*(unsigned char *)(moby + 0x20) == 0) return;
    {
        float timer_scale = D_L08_001621B0 * frame_time;
        ((Moby *)moby)->scale = *(float *)(*(char **)(moby + 0x24) + 0x24) * D_L08_00162180;
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
        *(unsigned short *)(moby + 0x34) |= 0x1000;
    } else {
        moby_point_position(moby, 4, pos);
        build_moby_bone_transform(moby, 4, matrix);
        copy_matrix3x4(*(char **)(data + 0x1f8) + 0xc0, matrix);
        attached = *(char **)(data + 0x1f8);
        qcopy(attached + 0x10, pos);
        update_moby_3024d8(attached);
    }

    damage = 0.0f;
    target = find_moby_hit(moby, 0x330000, 0);
    reaction = hit_3024d8(moby, target, data + 0x20, 0, &hit_state, &damage, 0, 4);
    if (hit_state != 1 && *(unsigned char *)(moby + 0x20) != 11) {
        {
            float health = *(float *)(data + 0x20) - damage;
            *(float *)(data + 0x20) = health;
            if (*(short *)(*(char **)(target + 0x20) + 0xa6) == 0x47) reaction = 3;
            if (health <= 0.0f) reaction = 1;
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
            case 8:
                {
                    float pose_scale = D_L08_0016218C * frame_time;
                    float timer_scale = frame_time * 3.0f;
                    *(float *)(data + 0x88) = pose_scale;
                    *(float *)(data + 0x8c) = timer_scale;
                    *(float *)(data + 0xc0) = 5.0f;
                    *(float *)(data + 0xc4) = 10.0f;
                }
                {
                    char *hit_moby = *(char **)(target + 0x20);
                    dx = *(float *)(moby + 0x10) - *(float *)(hit_moby + 0x10);
                    dy = *(float *)(moby + 0x14) - *(float *)(hit_moby + 0x14);
                }
                angle = fast_arc_tan(dx, dy);
                *(u128 *)pos = *(u128 *)(target + 0x10);
                knock_angle = angle;
                trooper_knockback_arc(pos, &knock_angle, (float *)(data + 0x88), (float *)(data + 0x8c));
                trooper_start_knockback(moby, data + 0x70, knock_angle, 8, 1, 0);
                *(unsigned char *)(moby + 0x20) = 10;
                *(unsigned char *)(data + 0x67) = 0x78;
                break;
            case 1:
            case 2:
                {
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
                    dx = *(float *)(moby + 0x10) - *(float *)(hit_moby + 0x10);
                    dy = *(float *)(moby + 0x14) - *(float *)(hit_moby + 0x14);
                }
                angle = fast_arc_tan(dx, dy);
                *(u128 *)pos = *(u128 *)(target + 0x10);
                second_knock_angle = angle;
                trooper_knockback_arc(pos, &second_knock_angle, (float *)(data + 0x88), (float *)(data + 0x8c));
                trooper_start_knockback(moby, data + 0x70, second_knock_angle, 12, 1, 0);
                *(unsigned char *)(moby + 0x20) = 11;
                *(unsigned char *)(data + 0x67) = 0xf0;
                FUN_L00_00260a88(moby, 2, 3, 0, -1);
                break;
            default:
                break;
            }
        }
        trooper_anim_update(moby, data + 0x60);
    }
    *(unsigned char *)(moby + 0xa4) = 0xff;
    trooper_anim_finish(moby, data + 0x60);

    if (*(int *)(data + 0x38) != 0) {
        angle = random_float_between(180.0f, 240.0f);
        angle = multiply_global_factor_ed64(angle);
        *(short *)(data + 0x1d8) = (short)truncate_float_to_s32(angle);
        *(int *)(data + 0x38) = 0;
    }
    fast_dec_timer_s16((short *)(data + 0x1d8));
    if (*(short *)(data + 0x1d8) != 0) *(float *)(data + 0x1dc) = *(float *)(data + 0x1e4) + 6.0f;
    else *(float *)(data + 0x1dc) = *(float *)(data + 0x1e4);

    if (random_integer_below(4) == 0) {
        {
            char *path = D_L08_001B0CB0[*(int *)(data + 0x1d0)];
            reaction = scan_target_3024d8(moby, data + 0x120, *(float *)(data + 0x1dc), 0, 0, path + 0x10, *(int *)path);
        }
        if (reaction != 2) {
            distance = vector_distance(moby + 0x10, data + 0x120);
            if (*(float *)(data + 0x1dc) < distance
                || 3.0f < absolute_float(*(float *)(moby + 0x18) - *(float *)(data + 0x128)))
                *(int *)(data + 0x164) = 2;
        }
    } else {
        chased = *(char **)(data + 0x160);
        if (chased != 0 && *(unsigned char *)(chased + 0x20) != 0xfe && *(unsigned char *)(chased + 0x20) != 0xfd) {
            qcopy(data + 0x120, chased + 0x10);
        } else {
            *(char **)(data + 0x160) = 0;
            *(int *)(data + 0x164) = 2;
        }
    }
    if (*(int *)(data + 0x160) == 0) *(char **)(data + 0x160) = (char *)hero.moby;

    state = *(unsigned char *)(moby + 0x20);
    if (state == 0 || state == 5 || state == 7 || state == 4) {
        height = probe_ground_height(moby + 0x10, 0, 0.5f);
        if (height < *(float *)(moby + 0x18)) *(float *)(moby + 0x18) = height;
    }
}

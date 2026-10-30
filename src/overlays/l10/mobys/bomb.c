/* Level 10 moby class bomb: the bomb the bombthrower_grass throws; it bounces, flashes and explodes. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/mobys/moby_spawn.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"

extern float D_0015ED6C;
extern float D_0015ED70;
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");

extern int  collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern char D_L10_001742C0[];
extern char D_L10_00174300[];

extern f32 D_L10_00161BEC_gp __asm__("D_L10_00161BEC") __attribute__((sda));
/* Ported from rac1-decomp src/overlays/l10_orxon/vendor_00296BD8.c (func_L10_002D9530) */

extern float FUN_001f9e90_u(float, float) __asm__("FUN_001f9e90");
extern float step_rotation_toward_target(float, float, float) __asm__("FUN_L00_00200510");

int resolve_bouncing_projectile_collision(char  *moby,
                                          char  *motion_data,
                                          float *previous_position) __asm__("FUN_L10_002d8170");

int resolve_bouncing_projectile_collision(char *moby, char *motion_data, float *previous_position) {
    float collision_start_position[4] __attribute__((aligned(16)));
    float world_up[4] __attribute__((aligned(16)));
    float scaled_collision_response[4] __attribute__((aligned(16)));
    float collision_alignment_vector[4] __attribute__((aligned(16)));
    float yaw_relative_collision_vector[4] __attribute__((aligned(16)));
    char *collision_result;
    int   hit_moby_address;
    float yaw_cosine, yaw_sine, collision_xz_length_or_ground_height, collision_tilt_angle,
        collision_rotation_y_target;

    float one = 1.0f;
    float k;

    *(u128 *)collision_start_position = *(u128 *)previous_position;
    *(u128 *)world_up = 0;
    world_up[2] = one;
    if (*(short *)(motion_data + 0x14) == 0) {
        if (collision_line(collision_start_position, moby + 0x10, 0x10, (int)moby, 0)) {
            collision_result = D_L10_001742C0;
            hit_moby_address = *(int *)(collision_result + 0x18);
            if (hit_moby_address != 0) {
                fast_vec_sub(
                    collision_result + 0x40, moby + 0x10, (void *)(hit_moby_address + 0x10));
                qcopy(moby + 0x10, collision_result + 0x20);
                fast_vec_normalize(
                    scaled_collision_response, collision_result + 0x40, D_0015ED70 * 9.7f);
                fast_vec_add(moby + 0x10, moby + 0x10, scaled_collision_response);
                reflect_vector(motion_data, motion_data, collision_result + 0x40);
                fast_vec_scale(motion_data, motion_data, 0.35f);
                fast_vec_normalize(scaled_collision_response, collision_result + 0x40, one);
                fast_vec_dot(scaled_collision_response, world_up);
                qcopy(collision_alignment_vector, collision_result + 0x40);
                k = 0.052359879f;
                yaw_cosine = fast_cos(MOBY(moby)->rot.z);
                yaw_sine = fast_sin(MOBY(moby)->rot.z);
                yaw_relative_collision_vector[0] = collision_alignment_vector[0] * yaw_cosine +
                                                   collision_alignment_vector[1] * yaw_sine;
                k = 0.052359879f;
                yaw_cosine = fast_cos(MOBY(moby)->rot.z);
                yaw_sine = fast_sin(MOBY(moby)->rot.z);
                yaw_relative_collision_vector[2] = collision_alignment_vector[2];
                yaw_relative_collision_vector[1] = collision_alignment_vector[1] * yaw_cosine -
                                                   collision_alignment_vector[0] * yaw_sine;
                collision_xz_length_or_ground_height = square_root(
                    yaw_relative_collision_vector[0] * yaw_relative_collision_vector[0] +
                    yaw_relative_collision_vector[2] * yaw_relative_collision_vector[2]);
                collision_tilt_angle = FUN_001f9e90_u(collision_xz_length_or_ground_height,
                                                      yaw_relative_collision_vector[1]);
                MOBY(moby)->rot.x =
                    step_rotation_toward_target(MOBY(moby)->rot.x, -collision_tilt_angle, k);
                collision_rotation_y_target = FUN_001f9e90_u(yaw_relative_collision_vector[2],
                                                             yaw_relative_collision_vector[0]);
                MOBY(moby)->rot.y =
                    step_rotation_toward_target(MOBY(moby)->rot.y, collision_rotation_y_target, k);
                allocate_voice_for_target_entry(1, 0, (int)moby);
                return 1;
            }
            if (*(int *)(collision_result + 0x1C) > 0) {
                qcopy(moby + 0x10, collision_result + 0x20);
                fast_vec_normalize(
                    scaled_collision_response, collision_result + 0x40, D_0015ED70 * 9.7f);
                fast_vec_add(moby + 0x10, moby + 0x10, scaled_collision_response);
                reflect_vector(motion_data, motion_data, collision_result + 0x40);
                fast_vec_scale(motion_data, motion_data, 0.5f);
                fast_vec_normalize(scaled_collision_response, collision_result + 0x40, one);
                if (0.707f < fast_vec_dot(scaled_collision_response, world_up)) {
                    collision_xz_length_or_ground_height =
                        probe_ground_height(moby + 0x10, 0, 0.5f);
                    if (MOBY(moby)->pos.z < collision_xz_length_or_ground_height)
                        MOBY(moby)->pos.z = collision_xz_length_or_ground_height;
                    if (*(float *)(motion_data + 0x8) < 0.0f)
                        *(float *)(motion_data + 0x8) = *(float *)(motion_data + 0x8) * -0.4f;
                    if (fast_vec_length(motion_data) < D_0015ED6C) {
                        clear_u64_value((float *)motion_data);
                        return 3;
                    }
                }
                qcopy(collision_alignment_vector, D_L10_00174300);
                k = 0.052359879f;
                yaw_cosine = fast_cos(MOBY(moby)->rot.z);
                yaw_sine = fast_sin(MOBY(moby)->rot.z);
                yaw_relative_collision_vector[0] = collision_alignment_vector[0] * yaw_cosine +
                                                   collision_alignment_vector[1] * yaw_sine;
                k = 0.052359879f;
                yaw_cosine = fast_cos(MOBY(moby)->rot.z);
                yaw_sine = fast_sin(MOBY(moby)->rot.z);
                yaw_relative_collision_vector[2] = collision_alignment_vector[2];
                yaw_relative_collision_vector[1] = collision_alignment_vector[1] * yaw_cosine -
                                                   collision_alignment_vector[0] * yaw_sine;
                collision_xz_length_or_ground_height = square_root(
                    yaw_relative_collision_vector[0] * yaw_relative_collision_vector[0] +
                    yaw_relative_collision_vector[2] * yaw_relative_collision_vector[2]);
                collision_tilt_angle = FUN_001f9e90_u(collision_xz_length_or_ground_height,
                                                      yaw_relative_collision_vector[1]);
                MOBY(moby)->rot.x =
                    step_rotation_toward_target(MOBY(moby)->rot.x, -collision_tilt_angle, k);
                collision_rotation_y_target = FUN_001f9e90_u(yaw_relative_collision_vector[2],
                                                             yaw_relative_collision_vector[0]);
                MOBY(moby)->rot.y =
                    step_rotation_toward_target(MOBY(moby)->rot.y, collision_rotation_y_target, k);
                allocate_voice_for_target_entry(1, 0, (int)moby);
                return 2;
            }
        }
    }
    return 0;
}

extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");

void bombthrower_bomb_update(char *m) __asm__("FUN_L10_002d85c8");

#ifndef NON_MATCHING
#include "rnc/math/vector.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002D9988), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef int u128_2D9988 __attribute__((mode(TI)));

typedef struct {
    float x, y, z;
} Vel;

typedef struct {
    char        pad0[0x10];
    u128_2D9988 q10;
} Obj_2D9988;

typedef struct {
    float         v[4];
    char         *owner;
    int           x34;
    unsigned char s38;
    unsigned char s39;
    short         s3a;
    float         f3c;
    int           i40;
} Shot;

extern char  D_L10_00178380[];
extern float FUN_L00_0025ab48(float, float);
extern float D_L10_00161BEC_gp __asm__("D_L10_00161BEC") __attribute__((sda));
extern int   set_moby_color() __asm__("FUN_L00_002502f0");
extern int   get_moby_color() __asm__("FUN_L00_00250320");
extern int   FUN_L00_0025a9f8(float, void *, int, void *, void *);
extern void  FUN_L00_0025a420(void *, void *, void *);
extern void  FUN_L00_0025f090(void *, void *, void *, int, int, int, int, int, float, float, float);
int resolve_bouncing_projectile_collision(char *m, char *c, float *a2) __asm__("FUN_L10_002d8170");
extern int
resolve_bouncing_projectile_collision(char *, char *, float *) __asm__("FUN_L10_002d8170");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void  func_L00_00251358_2D9988(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
extern void  func_L00_00251328_2D9988(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern int func_L00_001F2BE8_2D9988(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");

void bombthrower_bomb_update(char *m) {
    char *data;
    char *r16;
    int   state;
    int   flag21;
    float v0[4];
    float v10[4];
    Shot  sb;
    float s50, s54, s58;
    int   i5c, i60, i64;
    float s68, s6c, s70;
    int   rA, rB;

    data = *(char **)(m + 0x78);
    if (*(float *)(m + 0x10) < 1.0f || 511.0f < *(float *)(m + 0x10) ||
        *(float *)(m + 0x14) < 1.0f || 511.0f < *(float *)(m + 0x14) ||
        *(float *)(m + 0x18) < 1.0f || 511.0f < *(float *)(m + 0x18)) {
        mark_moby_for_removal(m);
        return;
    }
    *(float *)(m + 0x18) = *(float *)(m + 0x18) - D_L10_00161BEC_gp;
    qcopy(v0, m + 0x10);
    state = *(unsigned char *)(m + 0x20);
    switch (state) {
    case 0:
        fast_vec_add(m + 0x10, m + 0x10, data);
        *(float *)(data + 0x8) = *(float *)(data + 0x8) - D_0015ED70 * 9.8f;
        r16 = find_moby_hit(m, 0x830000, 0);
        if (r16 != 0 && *(char **)(r16 + 0x20) != 0 &&
            *(short *)(*(char **)(r16 + 0x20) + 0xA6) != *(short *)(m + 0xA6)) {
            float t;
            if (*(unsigned char *)(r16 + 0x28) == 0) {
                s50 = angle_atan2(*(float *)(m + 0x10) - hero.motion.pos.f[0],
                                  *(float *)(m + 0x14) - hero.motion.pos.f[1]);
                *(u128_2D9988 *)v10 = ((Obj_2D9988 *)r16)->q10;
                s58 = s54 = D_0015ED6C * 10.0f;
                trooper_knockback_arc(v10, &s50, &s54, &s58);
                *(float *)data = fast_cos(s50) * s54;
                *(float *)(data + 0x4) = fast_sin(s50) * s54;
                ((Vel *)data)->z = 0.0f;
                *(float *)(data + 0x8) = s58;
                *(short *)(data + 0x14) = scale_game_frames(0xF);
                *(unsigned char *)(m + 0x20) = 2;
            } else {
                *(unsigned char *)(m + 0x20) = 3;
            }
        }
        *(unsigned char *)(m + 0xA4) = 0xFF;
        if (fast_dec_timer_s16(data + 0x14) != 0 &&
            resolve_bouncing_projectile_collision(m, data, v0) == 3) {
            *(short *)(data + 0x14) = scale_game_frames(0xF0);
            *(unsigned char *)(m + 0x20) = 1;
        } else if (*(float *)(m + 0x18) < 2.0f) {
            *(unsigned char *)(m + 0x20) = 3;
        }
        break;
    case 1:
        func_L00_00251358_2D9988(m, &i5c, &i60, &i64);
        rA = scale_game_frames(0xF0);
        rB = scale_game_frames(0xF0);
        func_L00_00251328_2D9988(m, ((rA - *(short *)(data + 0x14)) * 127) / rB, i60, i64);
        if (fast_dec_timer_s16(data + 0x14) != 0) {
            *(unsigned char *)(m + 0x20) = 3;
            break;
        }
        r16 = find_moby_hit(m, 0x830000, 0);
        if (r16 != 0 && *(char **)(r16 + 0x20) != 0 &&
            *(short *)(*(char **)(r16 + 0x20) + 0xA6) != *(short *)(m + 0xA6)) {
            float t;
            if (*(unsigned char *)(r16 + 0x28) == 0) {
                s68 = angle_atan2(*(float *)(m + 0x10) - hero.motion.pos.f[0],
                                  *(float *)(m + 0x14) - hero.motion.pos.f[1]);
                *(u128_2D9988 *)v10 = ((Obj_2D9988 *)r16)->q10;
                s70 = s6c = D_0015ED6C * 10.0f;
                trooper_knockback_arc(v10, &s68, &s6c, &s70);
                *(float *)data = fast_cos(s68) * s6c;
                *(float *)(data + 0x4) = fast_sin(s68) * s6c;
                ((Vel *)data)->z = 0.0f;
                *(float *)(data + 0x8) = s70;
                *(short *)(data + 0x14) = scale_game_frames(0xF);
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
        set_vector_length_xy(sb.v, sb.v, 1.0f);
        flag21 = 0;
        sb.v[2] = 1.0f;
        sb.v[3] = 5627.925f;
        sb.s38 = state;
        sb.s39 = 1;
        sb.s3a = *(unsigned short *)(m + 0xA6);
        fast_vec_add(m + 0x10, m + 0x10, data);
        *(float *)(data + 0x8) = *(float *)(data + 0x8) - D_0015ED70 * 9.8f;
        if ((fast_dec_timer_s16(data + 0x14) != 0 &&
             resolve_bouncing_projectile_collision(m, data, v0) != 0) ||
            *(float *)(m + 0x18) < 2.0f) {
            flag21 = 1;
        }
        if (flag21) {
            *(unsigned char *)(m + 0x20) = 3;
        }
        break;
    case 3:
        rA = func_L00_001F2BE8_2D9988(1.5f, m + 0x10, 0x10, m, 0);
        *(u128_2D9988 *)v10 = *(u128_2D9988 *)(m + 0x10);
        send_blast_hits(m, v10, D_L10_00178380, rA, 0, 0x810001, 2, 1, 1.0f, 1.0f, 1.0f);
        cart_burst(m, m + 0x10, 0, 0.5f, 13.0f);
        mark_moby_for_removal(m);
        return;
    }
    *(float *)(m + 0x18) = *(float *)(m + 0x18) + D_L10_00161BEC_gp;
    if (*(float *)(m + 0x10) < 1.0f || 511.0f < *(float *)(m + 0x10) ||
        *(float *)(m + 0x14) < 1.0f || 511.0f < *(float *)(m + 0x14) ||
        *(float *)(m + 0x18) < 1.0f || 511.0f < *(float *)(m + 0x18)) {
        mark_moby_for_removal(m);
    }
}
#else
extern void get_moby_color(char *, void *, int *, int *) __asm__("FUN_L00_00250320");
extern void set_moby_color(char *, int, int, int) __asm__("FUN_L00_002502f0");
extern void trooper_knockback_arc(void *, float *, float *, float *) __asm__("FUN_L00_0025ab48");
extern int  collect_mobies_in_sphere(float, void *, int, char *, int) __asm__("FUN_L00_001f2868");
extern void send_blast_hits(
    float, float, float, char *, void *, int, int, int, int, int, int) __asm__("FUN_L00_0025a9f8");
extern void cart_burst(float, float, char *, void *, int) __asm__("FUN_L00_0025f090");

void bombthrower_bomb_update(char *m) {
    char *c = *(char **)(m + 0x78);
    float prev[4] __attribute__((aligned(16)));
    float hit_pos[4] __attribute__((aligned(16)));
    float angle;
    float speed;
    float height;
    float sine;
    int   color0, color1, color2;
    char *hit;
    int   state;
    int   first;
    int   second;
    int   effect;
    struct {
        float          velocity[4];
        char          *owner;
        int            color;
        unsigned char  state;
        unsigned char  active;
        unsigned short id;
        float          scale;
        int            one;
    } collision __attribute__((aligned(16)));

    if (*(float *)(m + 0x10) < 1.0f || *(float *)(m + 0x10) > 511.0f ||
        *(float *)(m + 0x14) < 1.0f || *(float *)(m + 0x14) > 511.0f ||
        *(float *)(m + 0x18) < 1.0f || *(float *)(m + 0x18) > 511.0f) {
        mark_moby_for_removal(m);
        return;
    }
    *(float *)(m + 0x18) -= *(float *)&D_L10_00161BEC_gp;
    qcopy(prev, m + 0x10);
    state = *(unsigned char *)(m + 0x20);
    if (state == 1)
        goto state1;
    if (state < 2) {
        if (state == 0)
            goto state0;
        goto finish;
    }
    if (state == 2)
        goto state2;
    if (state == 3)
        goto state3;
    goto finish;
state0:

    fast_vec_add(m + 0x10, m + 0x10, c);
    *(float *)(c + 8) -= frame_time_sq * 9.8f;
    hit = find_moby_hit(m, 0x830000, 0);
    if (hit != 0 && *(char **)(hit + 0x20) != 0 &&
        *(short *)(*(char **)(hit + 0x20) + 0xa6) != *(short *)(m + 0xa6)) {
        if (*(unsigned char *)(hit + 0x28) == 0) {
            angle = FUN_001f9e90_u(*(float *)(m + 0x10) - hero.motion.pos.f[0],
                                   *(float *)(m + 0x14) - hero.motion.pos.f[1]);
            *(OvlQuad *)hit_pos = *(OvlQuad *)(hit + 0x10);
            speed = frame_time * 10.0f;
            height = speed;
            trooper_knockback_arc(hit_pos, &angle, &speed, &height);
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
        if (resolve_bouncing_projectile_collision(m, c, prev) == 3) {
            *(short *)(c + 0x14) = scale_game_frames(0xf0);
            *(unsigned char *)(m + 0x20) = 1;
            goto finish;
        }
    }
    if (*(float *)(m + 0x18) < 2.0f)
        *(unsigned char *)(m + 0x20) = 3;
    goto finish;
state1: {
    float angle1, speed1, height1;

    get_moby_color(m, &color0, &color1, &color2);
    first = scale_game_frames(0xf0);
    second = scale_game_frames(0xf0);
    set_moby_color(m, ((first - *(short *)(c + 0x14)) * 0x7f) / second, color1, color2);
    if (fast_dec_timer_s16(c + 0x14)) {
        *(unsigned char *)(m + 0x20) = 3;
    } else {
        hit = find_moby_hit(m, 0x830000, 0);
        if (hit != 0 && *(char **)(hit + 0x20) != 0 &&
            *(short *)(*(char **)(hit + 0x20) + 0xa6) != *(short *)(m + 0xa6)) {
            if (*(unsigned char *)(hit + 0x28) == 0) {
                angle1 = FUN_001f9e90_u(*(float *)(m + 0x10) - hero.motion.pos.f[0],
                                        *(float *)(m + 0x14) - hero.motion.pos.f[1]);
                *(OvlQuad *)hit_pos = *(OvlQuad *)(hit + 0x10);
                speed1 = frame_time * 10.0f;
                height1 = speed1;
                trooper_knockback_arc(hit_pos, &angle1, &speed1, &height1);
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
state2: {
    int done;

    get_moby_color(m, &color0, &color1, &color2);
    set_moby_color(m, 0xff, color1, color2);
    collision.owner = m;
    collision.color = 0x830000;
    collision.scale = 1.0f;
    collision.one = 1;
    qcopy(collision.velocity, c);
    set_vector_length_xy(1.0f, collision.velocity, collision.velocity);
    done = 0;
    collision.velocity[2] = 1.0f;
    collision.velocity[3] = 5627.925f;
    collision.state = state;
    collision.active = 1;
    collision.id = *(unsigned short *)(m + 0xa6);
    fast_vec_add(m + 0x10, m + 0x10, c);
    *(float *)(c + 8) -= frame_time_sq * 9.8f;
    if (fast_dec_timer_s16(c + 0x14)) {
        if (resolve_bouncing_projectile_collision(m, c, prev) != 0)
            done = 1;
        else if (*(float *)(m + 0x18) < 2.0f)
            done = 1;
    } else if (*(float *)(m + 0x18) < 2.0f)
        done = 1;
    if (done)
        *(unsigned char *)(m + 0x20) = 3;
}
    goto finish;
state3:

    effect = collect_mobies_in_sphere(1.5f, m + 0x10, 0x10, m, 0);
    *(OvlQuad *)hit_pos = *(OvlQuad *)(m + 0x10);
    send_blast_hits(1.0f, 1.0f, 1.0f, m, hit_pos, 0x178380, effect, 0, 0x810001, 2, 1);
    cart_burst(0.5f, 13.0f, m, m + 0x10, 0);
    mark_moby_for_removal(m);
    return;

finish:
    *(float *)(m + 0x18) += *(float *)&D_L10_00161BEC_gp;
    if (*(float *)(m + 0x10) < 1.0f || *(float *)(m + 0x10) > 511.0f ||
        *(float *)(m + 0x14) < 1.0f || *(float *)(m + 0x14) > 511.0f ||
        *(float *)(m + 0x18) < 1.0f || *(float *)(m + 0x18) > 511.0f)
        mark_moby_for_removal(m);
}
#endif

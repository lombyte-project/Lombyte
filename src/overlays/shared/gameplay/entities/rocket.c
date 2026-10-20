/* Shared moby classes: rocket. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    float pad0[4];
    float a[4];
    float b[4];
    int c[4];
    float d[4];
} L10SpriteData_298940;
extern void func_001F9BF0_298940(void *, void *, void *) __asm__("FUN_001f9a28");
extern void func_L00_001FF4B0_298940(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_298940(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001F9908_298940(int *) __asm__("FUN_001f9740");
extern int func_001F9850_298940(int) __asm__("FUN_001f96f8");
extern float func_001FA888_298940(int) __asm__("FUN_001fa6c0");
extern int func_001FA8A8_298940(int, int, float) __asm__("FUN_001fa6e0");
extern float func_001F9B88_298940(float) __asm__("FUN_001f99c0");
extern unsigned char *func_L00_00273E08_298940(void *, int, unsigned char, int, int, int, int,
                                               float) __asm__("FUN_L00_00272f68");
extern int func_001FA898_298940(float) __asm__("FUN_001fa6d0");
void FUN_L10_00298940(char *m) {
    L10SpriteData_298940 *sprite_data = *(L10SpriteData_298940 **)(m + 0x78);
    float d[4] __attribute__((aligned(16)));
    float e[4] __attribute__((aligned(16)));
    int i, col;
    float f;

    *(float *)(m + 0x18) += 0.5f;
    func_001F9BF0_298940(d, D_L10_00167240, m + 0x10);
    func_L00_001FF4B0_298940(d, d, -0.3f);
    func_L00_001FF4B0_298940(e, d, 0.1f);
    func_001F9BD8_298940(d, d, m + 0x10);
    *(float *)(m + 0x18) -= 0.5f;
    for (i = 0; i < 4; i++) {
        float phase_sum = sprite_data->a[i] + sprite_data->b[i];
        sprite_data->a[i] = phase_sum;
        if (255.0f <= phase_sum) {
            sprite_data->a[i] = phase_sum - 255.0f;
        } else if (phase_sum <= 0.0f) {
            sprite_data->a[i] = phase_sum + 255.0f;
        }
        if (func_001F9908_298940(&sprite_data->c[i])) {
            sprite_data->c[i] = func_001F9850_298940(0xFF);
        }
        f = func_001FA888_298940(func_001F9850_298940(0xFF) - sprite_data->c[i]);
        f = f / (float)func_001F9850_298940(0xFF);
        col = func_001FA8A8_298940(0x4040FFFF, 0x1040FFFF, func_001F9B88_298940(0.5f - f));
        func_L00_00273E08_298940(d, col, func_001FA898_298940(sprite_data->a[i]) & 0xFF, 0x35, 1, 2, 0,
                                 sprite_data->d[i]);
        func_001F9BD8_298940(d, d, e);
    }
}
#include "sda.h"
#include "qcopy.h"
extern struct Moby *func_0020D348_m_cc1b8(int) __asm__("FUN_0020c4f8");
extern int func_001F9850_cc1b8(int) __asm__("FUN_001f96f8");
extern float func_001F9D10_cc1b8(void *, void *) __asm__("FUN_001f9b48");
extern float func_L00_001FF860_cc1b8(float, float) __asm__("FUN_001f9e90");
extern float func_001F9CE8_cc1b8(void *) __asm__("FUN_001f9b20");
extern void func_L00_00251E30_cc1b8(void *) __asm__("FUN_L00_00250df8");
char *FUN_L10_002cc1b8(float scale, void *vel, void *pos, int owner, int b, int c) {
    char *rocket = (char *)func_0020D348_m_cc1b8(0x333);
    char *shot_data;
    if (rocket) {
        *(float *)(rocket + 0x2C) = *(float *)(rocket + 0x2C) * scale;
        *(unsigned char *)(rocket + 0x30) = 0xFF;
        *(short *)(rocket + 0x32) = 0x7E;
        shot_data = *(char **)(rocket + 0x78);
        rocket[0x31] = 1;
        rocket[0x20] = 1;
        qcopy(rocket + 0x10, pos);
        qcopy(shot_data, vel);
        *(int *)(shot_data + 0x18) = func_001F9850_cc1b8(c);
        *(int *)(shot_data + 0x10) = owner;
        *(int *)(shot_data + 0x14) = b;
        *(int *)(shot_data + 0x28) = 0;
        *(float *)(shot_data + 0x38) = func_001F9D10_cc1b8(rocket + 0x10, &hero.motion.pos);
        *(int *)(shot_data + 0x30) = 0;
        *(int *)(shot_data + 0x34) = 0;
        *(float *)(rocket + 0x48) = func_L00_001FF860_cc1b8(*(float *)shot_data, *(float *)(shot_data + 4));
        *(float *)(rocket + 0x44) = -func_L00_001FF860_cc1b8(func_001F9CE8_cc1b8(shot_data), *(float *)(shot_data + 8));
        func_L00_00251E30_cc1b8(rocket);
    }
    return rocket;
}
/* Homing shot: steers toward the hero along an intercept course, trails sparks, then bursts. */

#include "qcopy.h"

typedef int Q_cc2e8 __attribute__((mode(TI), aligned(16)));

typedef struct {
    float vel[4];
    int owner;
    char *target;
    int timer;
    int pad1C[3];
    float yawSpeed;
    float pitchSpeed;
    int passed;
    int warned;
    float dist;
} Shot_cc2e8;

typedef struct {
    char pad00[0x40];
    float rx;
    float ry;
    float rz;
} Rot_cc2e8;

extern unsigned char D_0013F350_cc2e8[] __asm__("D_0013F350");
extern float vlen_cc2e8(void *) __asm__("FUN_001f9af0");
extern float vlenxy_cc2e8(void *) __asm__("FUN_001f9b20");
extern float vdist_cc2e8(void *, void *) __asm__("FUN_001f9b48");
extern float lensq_cc2e8(void *) __asm__("FUN_L00_001ff430");
extern float sqrt_cc2e8(float) __asm__("FUN_001f9988");
extern float atan2_cc2e8(float, float) __asm__("FUN_001f9e90");
extern float addrot_cc2e8(float, float) __asm__("FUN_001fa580");
extern float ground_cc2e8(void *, int, float) __asm__("FUN_00213508");
extern void vsub_cc2e8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void vadd_cc2e8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void vscale_cc2e8(void *, void *, float) __asm__("FUN_001f9a68");
extern void vdir_cc2e8(void *, void *, float) __asm__("FUN_001f9c48");
extern void vclear_cc2e8(void *) __asm__("FUN_001f99f8");
extern int frames_cc2e8(int) __asm__("FUN_001f96f8");
extern int expired_cc2e8(int *) __asm__("FUN_001f9740");
extern float turn_cc2e8(float, float, float, float, float, float *) __asm__("FUN_L00_00258110");
extern void setvel_cc2e8(float, float, float, void *) __asm__("FUN_00214db0");
extern int randint_cc2e8(int, int) __asm__("FUN_L00_00257b90");
extern void spark_cc2e8(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern int sound_cc2e8(int, int, void *) __asm__("FUN_0022da68");
extern int hit_cc2e8(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void damage_cc2e8(float, int, void *, int, void *, void *) __asm__("FUN_L00_00259bc8");
extern void burst_cc2e8(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void sfx_cc2e8(int, int, void *, int) __asm__("FUN_L00_0028dc90");
extern void kill_cc2e8(void *) __asm__("FUN_0020c828");

void rocket_update(char *m) __asm__("FUN_L10_002cc2e8");
void rocket_update(char *rocket) {
    enum {
        ROCKET_STATE_FLYING = 1,
        ROCKET_STATE_BURST = 2
    };
    Shot_cc2e8 *shot_data = *(Shot_cc2e8 **)(rocket + 0x78);
    float pos0[4] __attribute__((aligned(16)));
    float c[4] __attribute__((aligned(16)));
    float diff[4] __attribute__((aligned(16)));
    float hv[4] __attribute__((aligned(16)));
    float tp[4] __attribute__((aligned(16)));
    float aim[4] __attribute__((aligned(16)));
    float speed, a, b, discriminant_sqrt, r1, r2, intercept_time, yaw, pitch, dist;
    char *rocket_position;
    unsigned char *g;
    char *tg;
    float ground_aim_height;
    char *tb;

    switch ((unsigned char)rocket[0x20]) {
    case ROCKET_STATE_FLYING:
        qcopy(pos0, rocket + 0x10);
        if (shot_data->target == 0 || (unsigned char)shot_data->target[0x20] == 0xFE ||
            (unsigned char)shot_data->target[0x20] == 0xFD)
            rocket[0x20] = 2;
        rocket_position = rocket + 0x10;
        g = D_0013F350_cc2e8;
        if ((unsigned char)(g[0x20A4] - 1) < 2 && (tg = shot_data->target) == *(char **)(g + 0x2080)) {
            speed = vlen_cc2e8(shot_data);
            qcopy(tp, tg + 0x10);
            ground_aim_height = ground_cc2e8(tp, 0, 0.5f) + 0.2f;
            *(Q_cc2e8 *)hv = *(Q_cc2e8 *)(g + 0x100);
            hv[2] = 0.0f;
            tp[2] = ground_aim_height;
            if (g[0x20A4] == 2)
                tp[2] = ground_aim_height + 2.0f;
            vsub_cc2e8(diff, tp, rocket_position);
            a = speed * speed - lensq_cc2e8(hv);
            b = (hv[0] * diff[0] + hv[1] * diff[1] + hv[2] * diff[2]) * -2.0f;
            discriminant_sqrt = sqrt_cc2e8(b * b - a * 4.0f * -lensq_cc2e8(diff));
            r1 = (-b + discriminant_sqrt) / (a + a);
            r2 = (-b - discriminant_sqrt) / (a + a);
            if (r1 > 0.0f && r2 > 0.0f) {
                if (r1 > r2)
                    intercept_time = r1;
                else
                    intercept_time = r2;
            } else if (r1 > 0.0f) {
                intercept_time = r1;
            } else {
                intercept_time = -1.0f;
                if (r2 > 0.0f)
                    intercept_time = r2;
            }
            if (intercept_time > 0.0f) {
                vscale_cc2e8(aim, hv, intercept_time);
                vadd_cc2e8(aim, aim, diff);
                yaw = atan2_cc2e8(aim[0], aim[1]);
                pitch = -atan2_cc2e8(vlenxy_cc2e8(aim), aim[2]);
            } else {
                yaw = atan2_cc2e8(diff[0], diff[1]);
                pitch = -atan2_cc2e8(vlenxy_cc2e8(diff), diff[2]);
            }
            ((Rot_cc2e8 *)rocket)->rz = turn_cc2e8(((Rot_cc2e8 *)rocket)->rz, yaw, frame_time_sq * 6.2831855f,
                                              frame_time_sq * 3.1415927f, frame_time * 6.2831855f,
                                              &shot_data->yawSpeed);
            ((Rot_cc2e8 *)rocket)->ry = turn_cc2e8(((Rot_cc2e8 *)rocket)->ry, pitch, frame_time_sq * 6.2831855f,
                                              frame_time_sq * 3.1415927f, frame_time * 6.2831855f,
                                              &shot_data->pitchSpeed);
            ((Rot_cc2e8 *)rocket)->rx = addrot_cc2e8(((Rot_cc2e8 *)rocket)->rx, frame_time * 6.2831855f);
            setvel_cc2e8(speed, ((Rot_cc2e8 *)rocket)->rz, -((Rot_cc2e8 *)rocket)->ry, shot_data);
        }
        vclear_cc2e8(c);
        spark_cc2e8(rocket_position, c, 0x6F00AFFF, 0xFF, frames_cc2e8(randint_cc2e8(15, 22)), 0x28,
                    randint_cc2e8(20, 35), 1);
        spark_cc2e8(rocket_position, c, 0x1FFFFFFF, 0x4F4F4F, frames_cc2e8(randint_cc2e8(30, 60)), 0x28,
                    randint_cc2e8(50, 75), 0);
        dist = vdist_cc2e8(rocket_position, &hero.motion.pos);
        vadd_cc2e8(rocket_position, rocket_position, shot_data);
        if (shot_data->warned == 0 && shot_data->passed != 0 && shot_data->dist < dist && dist < 5.0f) {
            sound_cc2e8(0, 0, rocket);
            shot_data->warned = 1;
        }
        if (dist < shot_data->dist)
            shot_data->passed = 1;
        shot_data->dist = dist;
        if (hit_cc2e8(rocket_position, pos0, 0x10, shot_data->owner, 0)) {
            tb = ground_hit;
            if (*(int *)(tb + 0x18) != shot_data->owner) {
                if (*(int *)(tb + 0x18) != 0) {
                    vdir_cc2e8(diff, shot_data, 1.0f);
                    shot_data->vel[2] = 1.0f;
                    damage_cc2e8(1.0f, *(int *)(tb + 0x18), rocket, 0x10003, tb + 0x20, diff);
                }
                rocket[0x20] = 2;
            }
        }
        if (expired_cc2e8(&shot_data->timer))
            rocket[0x20] = 2;
        break;
    case ROCKET_STATE_BURST:
        if (current_level_index != 10)
            burst_cc2e8(rocket, rocket + 0x10, -1, 0.25f, 13.0f);
        else
            burst_cc2e8(rocket, rocket + 0x10, -1, 0.25f, 0.0f);
        sfx_cc2e8(0, 0, rocket, 0x99);
        kill_cc2e8(rocket);
        break;
    }
}

/* Level 05 moby classes: sharkigator, scenery_ship, amoeboid, extermibot, extermitank, skid_mcmarx, floating_platform, box, water. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/moby_anim.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8190), where it is exact; names translated to the US level program. */

extern int FUN_L00_002591d0(int *, int, int, int);
extern int FUN_L00_002592b8(int *, int, int, int);
extern void FUN_L00_00263618(void *, int, float, float);

void FUN_L05_002d6f48(unsigned char *moby) {
    unsigned char *it;
    int i;
    char *data;
    char *q;
    FUN_L00_002591d0((int *)&it, moby[0x21], 0, 0);
    while (it != 0) {
        if (it[0x31] != 0) {
            data = *(char **)(it + 0x78);
            if (*(short *)(it + 0xA6) == *(short *)(moby + 0xA6)) {
                q = data + 0x330;
                for (i = 2; i >= 0; i--) {
                    FUN_L00_00263618(q, *(int *)(data + 0x360), 0.45f, 0.4f);
                    q += 0x10;
                }
            }
        }
        FUN_L00_002592b8((int *)&it, (int)it, 0, 0);
    }
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8AD8), where it is exact; names translated to the US level program. */

extern void sample_camera_path(int *, int, void *, float *, int, float) __asm__("FUN_00214e58");

extern short D_L05_001613D0 __attribute__((sda));
void FUN_L05_002d7890(char *moby, int *path) {
    float *d = *(float **)(moby + 0x78);
    d[2] = d[2] + *(float *)&D_L05_001613D0 * frame_time / d[3];
    if ((float)path[0] < d[2]) {
        d[2] = d[2] - (float)path[0];
    }
    sample_camera_path(path, 1, moby + 0x10, (float *)(moby + 0x40), 0, d[2]);
    *(float *)(moby + 0x40) = *(float *)(moby + 0x4C);
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC1E8), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char D_0013E533[];
extern int FUN_001f96f8(int);
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L05_002db238_u(void *, void *) __asm__("FUN_L05_002db238");

void FUN_L05_002daf58(unsigned char *m) {
    char *r = find_moby_hit(m, 0x330000, 0);
    if (r != 0) {
        char *b = D_0013E533 + 0xF1D;
        char *o;
        m[0x20] = 2;
        m[0x31] = 0;
        *(int *)(m + 0x94) = 0;
        *(unsigned short *)(m + 0x34) |= 1;
        FUN_L05_002db238_u(m, b);
        allocate_voice_for_target_entry(0, 0, (int)m);
        o = *(char **)(r + 0x20);
        if (o != 0 && *(short *)(o + 0xA6) == 0) {
            int v = FUN_001f96f8(0x3C);
            char *y = b - 0x100;
            *(int *)(y + 0x8C4) += v;
        }
        m[0xA4] = 0xFF;
    }
}
#include "qcopy.h"
extern void *create_moby() __asm__("FUN_0020c4f8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002F9B10), where it is exact; names translated to the US level program. */

extern void FUN_L00_00250df8(void *);

char *FUN_L05_002f8718(char *owner) {
    char *moby = create_moby(0x1B7);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 2;
        qcopy(moby + 0x10, owner + 0x10);
        qcopy(moby + 0x40, owner + 0x40);
        *(unsigned long *)(moby + 0x38) = *(unsigned long *)(owner + 0x38);
        FUN_L00_00250df8(moby);
    }
    return moby;
}
/* Moves a point along a looped path of 16-byte nodes stored in the moby's data. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00308188), where it is exact; names translated to the US level program. */

extern float FUN_001f9dc8(float);
extern float FUN_001f9de0(float);
extern float FUN_001fa580(float, float);
extern float FUN_001f9e90(float, float);

void FUN_L05_00306d30(char *moby, int idx, float *out) {
    char *p = *(char **)(*(char **)(moby + 0x78) + 0x244);
    float v[4];
    float a;
    qcopy(out, p + 0x10 + idx * 16);
    qcopy(v, p + 0x10 + ((idx + 1) % *(int *)p) * 16);
    a = FUN_001fa580(FUN_001f9e90(v[0] - out[0], v[1] - out[1]), 1.5707964f);
    out[0] = out[0] + FUN_001f9dc8(a) * 0.0f;
    out[1] = out[1] + FUN_001f9de0(a) * 0.0f;
}
/* advances a moby's path waypoint until it reaches one flagged 2.0 */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003087B0), where it is exact; names translated to the US level program. */

extern char *D_L05_001B0930[];
extern int FUN_L00_0025d7a0(char *, int, int, int);

void FUN_L05_00307358(char *moby) {
    char *d = *(char **)(moby + 0x78);
    char *cur = *(char **)(d + 0x244);
    int idx;
    int off;
    if (cur != D_L05_001B0930[*(int *)(d + 0x220)]) {
        if (*(float *)(d + 0x24C) <= 0.0f) {
            idx = *(short *)(d + 0x258);
            off = idx * 16;
            if (*(float *)(cur + off + 0x1C) == 1.0f) {
                while (*(float *)(cur + off + 0x1C) != 2.0f) {
                    idx = FUN_L00_0025d7a0(cur, idx, 1, 1);
                    cur = *(char **)(d + 0x244);
                    off = idx * 16;
                }
                *(int *)(d + 0x27C) = idx;
                qcopy(d + 0x210, *(char **)(d + 0x244) + idx * 16 + 0x10);
                *(int *)(d + 0x26C) = 1;
            }
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003088C8), where it is exact; names translated to the US level program. */

int FUN_L05_00307470(int arg) {
    return arg == 5 ? 6 : arg;
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003088D8), where it is exact; names translated to the US level program. */

extern int FUN_L05_00307470(int);
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void FUN_L05_00307480(char *moby, int a1, int a2, int a3) {
    char *data;
    char *child;
    float ratio;
    float f;
    float max;
    int idx;
    blend_moby_animation(moby, a1, a2, a3);
    data = *(char **)(moby + 0x78);
    child = *(char **)(data + 0x268);
    if (child != 0) {
        ratio = *(float *)(child + 0x5C) / *(float *)(moby + 0x5C);
        idx = FUN_L05_00307470(a1);
        f = (float)a2 * ratio;
        max = (float)*(unsigned char *)(*(char **)(*(char **)(*(char **)(data + 0x268) + 0x24) -
                                                   (-(idx * 4)) + 0x48) +
                                        0x10);
        if (max < f)
            f = max;
        else if (f < 0.0f)
            f = 0.0f;
        blend_moby_animation(*(char **)(data + 0x268), idx, (int)f, a3);
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L05_002d1688.s", FUN_L05_002d1688);
#else
typedef unsigned int U128_2d1688 __attribute__((mode(TI)));
extern char *D_L05_001B0930[];
extern short D_L05_00161388 __attribute__((sda));
extern float D_L05_001612E8 __attribute__((section(".data")));
extern float D_L05_0013F640 __asm__("D_L05_0013F640");
extern float D_0013F3D0[4] __asm__("D_0013F3D0");
extern char D_L05_0013F3E0[] __asm__("D_L05_0013F3E0");
extern int D_L05_001413D4 __asm__("D_L05_001413D4");
extern int D_L05_001413DC __asm__("D_L05_001413DC");
extern unsigned char D_L05_001413F5 __asm__("D_L05_001413F5");
extern float D_L05_001671D8 __asm__("D_L05_001671D8");
extern char D_L05_001671C0[] __asm__("D_L05_001671C0");
extern char D_L05_001671D0[] __asm__("D_L05_001671D0");

extern int step_path_2d1688(int) __asm__("FUN_00213260");
extern int timer_2d1688(int) __asm__("FUN_001f96f8");
extern void mark_moby_2d1688(char *) __asm__("FUN_0020c828");
extern void set_action_2d1688(char *, int, int, int) __asm__("FUN_00212f90");
extern float angle_2d1688(float, float) __asm__("FUN_001f9e90");
extern void steer_2d1688(float, float, float, float, void *, void *) __asm__("FUN_L00_0025be00");
extern void fast_vec_normalize(float, void *, void *) __asm__("FUN_001f9bf8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float distance_2d1688(void *, void *) __asm__("FUN_001f9b48");
extern int tick_2d1688(void *) __asm__("FUN_001f9740");
extern float absolute_2d1688(float) __asm__("FUN_001f99c0");
extern float distance_to_2d1688(void *, void *) __asm__("FUN_001f9b80");
extern void move_2d1688(float, float, float, float, void *, void *) __asm__("FUN_00213f38");
extern float angle_from_2d1688(float, float) __asm__("FUN_001fa580");
extern void copy_direction_2d1688(void *, void *) __asm__("FUN_001fa030");
extern int check_path_2d1688(void *, void *, int) __asm__("FUN_L00_00259740");
extern void set_camera_2d1688(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern int allocate_voice_for_target_entry_2d1688(int, int, char *) __asm__("FUN_0022da68");
extern void sound_2d1688(void) __asm__("FUN_L00_00211250");
extern void reset_2d1688(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L05_002d1e30(char *);
extern void FUN_L05_002d1fd8(int, char *);

void FUN_L05_002d1688(char *moby) {
    char *data = *(char **)(moby + 0x78);
    int state = *(unsigned char *)(moby + 0x20);
    int one = 1;
    char *path;
    char *point;
    float v[4] __attribute__((aligned(16)));
    float w[4] __attribute__((aligned(16)));
    float z[4] __attribute__((aligned(16)));
    float speed;
    float height;
    int index;
    int duration;

    switch (state) {
    case 0: goto state_zero;
    case 1: goto state_one;
    case 2: goto state_two;
    case 3: goto state_three;
    default: return;
    }
state_zero:
    {
        if (*(int *)(data + 0x64) != 0) {
            *(unsigned char *)(moby + 0x20) = 2;
            *(unsigned short *)(moby + 0x34) |= 1;
            *(unsigned char *)(moby + 0x30) = 0xff;
            *(float *)(data + 0x74) = *(float *)(moby + 0x18);
            *(int *)(data + 0x7c) = timer_2d1688(30);
            return;
        }
        index = *(int *)(data + 0x60);
        if (index == -1) {
            mark_moby_2d1688(moby);
            return;
        }
        *(int *)(data + 0x68) = step_path_2d1688(*(int *)D_L05_001B0930[index]);
        *(int *)(data + 0x6c) = timer_2d1688(600);
        if (*(unsigned char *)(moby + 0x53) != 1) {
            set_action_2d1688(moby, 1, 0, timer_2d1688(10));
        }
        height = *(float *)(D_L05_001B0930[*(int *)(data + 0x60)] + 0x18) - (*(float *)&D_L05_00161388);
        *(float *)(moby + 0x18) = height;
        *(float *)(data + 0x80) = height;
        *(unsigned char *)(moby + 0x20) = one;
        return;
    }
state_one:
    {
        path = D_L05_001B0930[*(int *)(data + 0x60)];
        qcopy(v, path + *(int *)(data + 0x68) * 16 + 0x10);
        point = moby + 0x48;
        {
            float y = v[1];
            float x = v[0];
            height = angle_2d1688(x - *(float *)(moby + 0x10), y - *(float *)(moby + 0x14));
        }
        {
            float pi = 3.1415927f;
            float turn_step = frame_time_sq * pi;
            steer_2d1688(height, turn_step, turn_step, frame_time * pi, point, data + 0x70);
        }
        fast_vec_normalize(frame_time * 3.0f, w, moby + 0xc0);
        fast_vec_add(moby + 0x10, moby + 0x10, w);
        height = distance_2d1688(moby + 0x10, v);
        if (height < 3.0f || tick_2d1688(data + 0x6c) != 0) {
            index = step_path_2d1688(*(int *)D_L05_001B0930[*(int *)(data + 0x60)]);
            *(int *)(data + 0x68) = (*(int *)(data + 0x68) + index) % *(int *)D_L05_001B0930[*(int *)(data + 0x60)];
            *(int *)(data + 0x6c) = timer_2d1688(600);
        }
        if ((hero.state.control_mode == 0x12 || hero.state.current == 0x12) &&
            distance_to_2d1688(moby + 0x10, &hero.motion.pos) < 8.0f) {
            speed = (D_L05_001612E8 - (*(float *)&D_L05_00161388)) - 2.0f;
        } else {
            speed = (D_L05_001612E8 - (*(float *)&D_L05_00161388)) - 0.5f;
            FUN_L05_002d1e30(moby);
        }
        move_2d1688(speed, frame_time_sq + frame_time_sq,
                     frame_time_sq + frame_time_sq,
                     frame_time + frame_time, moby + 0x18, data + 0x78);
        return;
    }
state_two:
    {
        char *path;
        tick_2d1688(data + 0x7c);
        height = absolute_2d1688(hero.water_surface_z - *(float *)(data + 0x74));
        if (height < 0.33f && hero.state.control_mode == 0x12) {
            char *point;
            if (*(int *)(data + 0x7c) != 0) return;
            point = D_L05_001671C0 - 0x140;
            *(float *)(moby + 0x44) = -1.5533431f;
            *(float *)(moby + 0x48) = angle_from_2d1688(*(float *)(point + 0x158), 1.5707964f);
            copy_direction_2d1688(moby + 0xc0, moby + 0x40);
            qcopy(moby + 0x10, &hero.motion.pos);
            fast_vec_normalize((*(float *)&D_L05_00161388), v, moby + 0xe0);
            fast_vec_sub(moby + 0x10, moby + 0x10, v);
            {
                float speed = frame_time * 10.0f;
                *(float *)(moby + 0x18) -= 2.0f;
                *(float *)(data + 0x78) = speed;
            }
            *(unsigned short *)(moby + 0x34) &= 0xfffe;
            if (*(unsigned char *)(moby + 0x53) != state)
                set_action_2d1688(moby, 2, 0, timer_2d1688(5));
            *(unsigned char *)(moby + 0xbc) = 0;
            *(unsigned char *)(moby + 0x20) = 3;
            reset_2d1688(point + 0x140, point + 0x150, 0, 0, 0);
            return;
        }
        if (hero.state.current == 0x12) return;
        path = D_L05_001B0930[*(int *)(data + 0x60)];
        if (check_path_2d1688(&hero.motion.pos, path + 0x10, *(int *)path) != 0)
            *(int *)(data + 0x7c) = timer_2d1688(600);
        else
            *(int *)(data + 0x7c) = timer_2d1688(180);
        return;
    }
state_three:
    {
        float speed;
        {
            struct Hero *h = &hero;
            if (h->water_surface_z < *(float *)(moby + 0x18) || *(float *)(data + 0x78) < 0.0f) {
                if (h->state.current != 0x72) {
                    qcopy_nc(v, &h->motion.pos);
                    v[2] = h->water_surface_z;
                    FUN_L05_002d1fd8((int)moby, (char *)v);
                    set_camera_2d1688(&h->motion.pos, &h->motion.rot, 0x72, 0);
                } else {
                    qcopy(&h->motion.pos, moby + 0x10);
                }
                h->unk20A5 = one;
                h->motion.pos.f[2] += 20.0f;
            }
        }
        height = *(float *)(data + 0x78);
        speed = *(float *)(moby + 0x18);
        {
            float acceleration = frame_time_sq * 10.0f;
            float threshold = frame_time + frame_time;
            *(float *)(moby + 0x18) = speed + height;
            {
                float speed = *(float *)(data + 0x78) - acceleration;
                *(float *)(data + 0x78) = speed;
                if (speed < threshold) {
                    fast_vec_normalize((*(float *)&D_L05_00161388), v, moby + 0xe0);
                    steer_2d1688(1.5533431f, frame_time_sq * 12.566371f,
                                  frame_time_sq * 12.566371f, frame_time * 25.132742f,
                                  moby + 0x44, data + 0x70);
                    copy_direction_2d1688(moby + 0xc0, moby + 0x40);
                    fast_vec_normalize((*(float *)&D_L05_00161388), w, moby + 0xe0);
                    fast_vec_sub(z, w, v);
                    fast_vec_sub(moby + 0x10, moby + 0x10, z);
                }
            }
        }
        if (*(float *)(data + 0x78) < 0.0f) {
            if (*(float *)(moby + 0x18) < hero.water_surface_z && *(unsigned char *)(moby + 0xbc) == 0) {
                *(unsigned char *)(moby + 0xbc) = 1;
                allocate_voice_for_target_entry_2d1688(0, 0, moby);
                fast_vec_normalize((*(float *)&D_L05_00161388), v, moby + 0xe0);
                fast_vec_add(v, v, moby + 0x10);
                v[2] = hero.water_surface_z;
                FUN_L05_002d1fd8((int)moby, (char *)v);
            }
        }
        if (*(float *)(data + 0x78) < 0.0f &&
            *(float *)(moby + 0x18) < *(float *)(data + 0x74) - 8.0f) sound_2d1688();
        if ((*(unsigned char *)(moby + 0x70) & 2) != 0 && *(unsigned char *)(moby + 0x53) != 1)
            set_action_2d1688(moby, 1, 0, timer_2d1688(10));
    }
}
#endif

typedef float V2d1e30[4] __attribute__((aligned(16)));
extern int tick_2d1e30(void *) __asm__("FUN_001f9740");
extern float addrot_2d1e30(float, float) __asm__("FUN_001fa580");
extern float rand_2d1e30(float, float) __asm__("FUN_002132a8");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void vadd_2d1e30(void *, void *, void *) __asm__("FUN_001f9a10");
extern char *spawn_2d1e30(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");
extern int frames_2d1e30(int) __asm__("FUN_001f96f8");
extern int ftoi_2d1e30(float) __asm__("FUN_001fa6d0");
extern float D_L05_001612E8 __asm__("D_L05_001612E8");
extern char D_L05_0015F580_macro[] __asm__("D_L05_0015F580") __attribute__((section(".sdata")));

/* When the timer runs out, spawns two effects beside the moby, one to each side, and restarts the timer. */
void FUN_L05_002d1e30(char *moby) {
    V2d1e30 v;
    char *data;
    char *p;
    float a;
    int i;
    float *h;

    data = *(char **)(moby + 0x78);
    if (tick_2d1e30(data + 0x84) == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        a = addrot_2d1e30(*(float *)(moby + 0x48), 3.14159274f);
        addrot_2d1e30(a, rand_2d1e30(-0.785398006f, 0.785398006f));
        fast_vec_scale(v, moby + 0xD0, rand_2d1e30(-0.5f, 0.5f));
        vadd_2d1e30(v, v, moby + 0x10);
        v[2] = D_L05_001612E8 + 0.05f;
        h = &D_L05_001612E8;
        p = spawn_2d1e30(v, D_L05_0015F580_macro, h, rand_2d1e30(0.7f, 1.0f), i == 0 ? 2.0f : -2.0f);
        if (p != 0) {
            *(short *)(p + 0xA) = frames_2d1e30(15);
        }
        *(int *)(data + 0x84) = ftoi_2d1e30(rand_2d1e30(3.0f, 5.0f));
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Spawns a burst of particles and a ring of sparks around a position. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D3220), where it is exact; names translated to the US level program. */

extern char *FUN_L00_002d7e90(void *, float);
extern char D_L05_0015F580_macro[]  __asm__("D_L05_0015F580")__attribute__((section(".sdata")));
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float fast_cos(float);
extern float fast_sin(float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float D_L05_001612E8;
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern int truncate_float_to_s32() __asm__("FUN_001fa6d0");
extern unsigned char *FUN_L00_002718d0(void *, void *, void *, float, float);
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void FUN_L00_0026f548(void *, void *, int, int);
extern void add_vector_xyz(void *, void *, void *);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L05_002d1fd8(int unused, char *p) {
    float v[4];
    char *r;
    int i;
    char *base;
    int n;
    r = FUN_L00_002d7e90(p, 3.0f);
    if (r)
        r[0x23] = 0x70;
    for (base = (char *)&D_L05_001612E8, i = 15; i >= 0; i--) {
        float ang = random_angle_radians();
        float k = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
        v[0] = fast_cos(ang) * k;
        v[1] = fast_sin(ang) * k;
        v[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
        n = random_int_between(0x5A, 0x78);
        FUN_L00_0026f548(p, v, random_integer_below(2), n);
    }
    for (i = 0; i < 16; i++) {
        unsigned char *q;
        random_vector(v, 1.0f, 1.0f);
        add_vector_xyz(v, v, p);
        v[2] = D_L05_001612E8 + 0.05f;
        q = FUN_L00_002718d0(v, D_L05_0015F580_macro, base, random_float_between(0.7f, 1.0f),
                             i == 0 ? 2.0f : -2.0f);
        if (q) {
            *(short *)(q + 0xA) = func_001FA898_r(multiply_global_factor_ed64(random_float_between(30.0f, 60.0f)));
        }
    }
}

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float rot_2d7020(float, float) __asm__("FUN_001fa580");
extern float sin_2d7020(float) __asm__("FUN_001f9de0");
extern int lerp_2d7020(float, int, int) __asm__("FUN_001fa6e0");
extern int frame_number __asm__("D_L05_0015F5CC");
extern void queue_2d7020(void *, void *) __asm__("FUN_001f4600");
void FUN_L05_002d6f48(unsigned char *moby);

/* Per-frame glow: updates three parts, pulses the colour, and queues a redraw
 * once per frame. */
void FUN_L05_002d7020(char *m) {
    char *v;
    int i;
    int c;
    int f;
    int k;
    int *p;

    if (*(unsigned char *)(m + 0x21) != 0xFF && *(unsigned char *)(m + 0x31) != 0) {
        v = *(char **)(m + 0x78);
        for (i = 0; i < 3; i++) {
            moby_point_position(m, i, v + 0x330 + i * 16);
        }
        *(float *)(v + 0x368) = rot_2d7020(*(float *)(v + 0x368), frame_time * 1.9198622f);
        c = lerp_2d7020(sin_2d7020(*(float *)(v + 0x368)) * 0.5f + 0.5f, 0x1E1EB4, 0x1EB41E);
        f = frame_number;
        *(int *)(m + 0x90) = c;
        k = (c & 0xFF0000) | 0x30000000;
        *(int *)(v + 0x360) = k | (c & 0xFF00) | (c & 0xFF);
        if (f != 0) {
            p = *(int **)(v + 0x364);
            if (f != *p) {
                *p = f;
                queue_2d7020(FUN_L05_002d6f48, m);
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8388), where it is exact; names translated to the US level program. */

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f_2d7140;

typedef int u128_2D8388 __attribute__((mode(TI)));

typedef union { u128_2D8388 q; float f[4]; } Buf_2D8388;

typedef struct {
    /* 0x000 */ int path;
    /* 0x004 */ float f4;
    /* 0x008 */ float f8;
    /* 0x00C */ float fC;
    /* 0x010 */ int pad10[3];
    /* 0x01C */ float f1C;
    /* 0x020 */ Vec4f_2d7140 trail[3][16];
    /* 0x320 */ short f320;
    /* 0x322 */ unsigned short f322;
    /* 0x324 */ int *f324;
    /* 0x328 */ int f328;
    /* 0x32C */ int pad32C[15];
    /* 0x368 */ float f368;
} Data_2D8388;

extern char *FUN_L00_0025a420(void *, int, int);
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin_c4(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_angle_radians_c2(void) __asm__("func_00213308");
extern float D_0015ED6C;
extern float D_0015ED70;
extern int frame_number __asm__("D_L05_0015F5CC");
extern float D_L05_001613D4 __attribute__((sda));
int D_L05_001613D8 = 0x80000080;
int D_L05_001613DC = 0x80202020;
float D_L05_001613E0 = 5.0f;
int D_L05_001613E4 = 1;
extern float D_L05_001613E8 __attribute__((sda));
extern float D_L05_001613EC __attribute__((sda));
extern int FUN_001fa6e0(int, int, float);
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
float absolute_float(float input) __asm__("func_001F99C0");
int FUN_L00_0028d8c0(int a, int i);
void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9, float *v10, float *v11, float scale, float *v12);
void FUN_L05_002d7020_c(char *m) __asm__("FUN_L05_002d7020");
void FUN_L05_002d7890_c(char *moby, int *path) __asm__("FUN_L05_002d7890");
void FUN_L05_002d7920(char *moby);
extern char *func_L00_0025B478_2D8388(void *, int, int) __asm__("FUN_L00_0025a420");
extern float func_00214158_2D8388(void) __asm__("FUN_00213308");
extern float func_L00_001FF860_2D8388(float, float) __asm__("FUN_001f9e90");
extern float func_001FA790_2D8388(float, float) __asm__("FUN_001fa5c8");
extern float func_001F9B88_2D8388(float input) __asm__("FUN_001f99c0");
extern float func_001F9D10_2D8388(float *, float *) __asm__("FUN_001f9b48");
extern float func_001FA748_2D8388(float, float) __asm__("FUN_001fa580");
extern float func_001F9FA8_2D8388(float) __asm__("FUN_001f9de0");
extern int func_001FA8A8_2D8388(int, int, float) __asm__("FUN_001fa6e0");
extern void func_L05_002D8AD8_2D8388(char *moby, int *path) __asm__("FUN_L05_002d7890");
extern int func_L00_0028EB98_2D8388(void *, int) __asm__("FUN_L00_0028d8c0");
extern int func_0022ED80_2D8388(int, int, int) __asm__("FUN_0022da68");
extern void func_001F9EC0_2D8388(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void func_001F9BD8_2D8388(float *, float *, float *) __asm__("FUN_001f9a10");
extern void func_001F49B0_2D8388(void (*)(void), void *) __asm__("FUN_001f4600");
extern void func_001F9C30_2D8388(void *, void *, float) __asm__("FUN_001f9a68");
extern int func_001F9850_2D8388(int) __asm__("FUN_001f96f8");
extern void *func_L00_00265050_2D8388(char *src, int cls, float *pos, void *mat, int a8, int a9, float *v10, float *v11, float scale, float *v12) __asm__("FUN_L00_00263fd8");
extern void func_0020D678_2D8388(void *) __asm__("FUN_0020c828");
extern void func_L05_002D8268_2D8388(char *moby) __asm__("FUN_L05_002d7020");
extern void func_L05_002D8B68_2D8388(void) __asm__("FUN_L05_002d7920");
extern float D_L05_001672C8_2D8388[] __asm__("D_L05_001671C8");
extern char *D_L05_001B0CB0_2D8388[] __asm__("D_L05_001B0930");

void FUN_L05_002d7140(char *moby) {
    Data_2D8388 *data = *(Data_2D8388 **)(moby + 0x78);
    char *hit;
    char *path;
    char *current;
    char *prev;
    char *next;
    char *point;
    int i;
    int k;
    int lifetime;
    int damaged;
    Buf_2D8388 left_offset;
    Buf_2D8388 tail_offset;
    Buf_2D8388 right_offset;
    float in_angle;
    float out_angle;
    float turn;
    float max_turn;
    float turn_scale;
    float *pos, *rot, *tab;

    if (D_L05_001672C8_2D8388[0] < 55.0f) return;
    hit = func_L00_0025B478_2D8388(moby, 0x10000, 0);
    damaged = 0;
    switch (*(unsigned char *)(moby + 0x20)) {
    case 0: {
        *(unsigned char *)(moby + 0x30) = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24) * D_L05_001613D4;
        data->f1C = func_00214158_2D8388();
        data->f328 = -1;
        data->f368 = func_00214158_2D8388();
        path = D_L05_001B0CB0_2D8388[data->path];
        if (*(float *)(path + 0x1C) == -1.0f) {
            max_turn = 0.0f;
            point = path + ((*(int *)path - 1) << 4);
            in_angle = func_L00_001FF860_2D8388(*(float *)(path + 0x10) - *(float *)(point + 0x10), *(float *)(path + 0x14) - *(float *)(point + 0x14));
            out_angle = func_L00_001FF860_2D8388(*(float *)(path + 0x20) - *(float *)(path + 0x10), *(float *)(path + 0x24) - *(float *)(path + 0x14));
            *(float *)(path + 0x1C) = func_001FA790_2D8388(out_angle, in_angle);
            for (i = 1; i < *(int *)path - 1; i++) {
                current = path + i * 16;
                prev = path + (i - 1) * 16;
                in_angle = func_L00_001FF860_2D8388(*(float *)(current + 0x10) - *(float *)(prev + 0x10), *(float *)(current + 0x14) - *(float *)(prev + 0x14));
                next = path + (i + 1) * 16;
                out_angle = func_L00_001FF860_2D8388(*(float *)(next + 0x10) - *(float *)(current + 0x10), *(float *)(next + 0x14) - *(float *)(current + 0x14));
                turn = func_001FA790_2D8388(out_angle, in_angle);
                *(float *)(current + 0x1C) = turn;
                if (max_turn < func_001F9B88_2D8388(turn)) {
                    max_turn = func_001F9B88_2D8388(*(float *)(current + 0x1C));
                }
            }
            in_angle = func_L00_001FF860_2D8388(*(float *)(path + ((*(int *)path - 1) << 4) + 0x10) - *(float *)(path + ((*(int *)path - 2) << 4) + 0x10),
                                    *(float *)(path + ((*(int *)path - 1) << 4) + 0x14) - *(float *)(path + ((*(int *)path - 2) << 4) + 0x14));
            out_angle = func_L00_001FF860_2D8388(*(float *)(path + 0x10) - *(float *)(path + ((*(int *)path - 1) << 4) + 0x10),
                                   *(float *)(path + 0x14) - *(float *)(path + ((*(int *)path - 1) << 4) + 0x14));
            *(float *)(path + ((*(int *)path - 1) << 4) + 0x1C) = func_001FA790_2D8388(out_angle, in_angle);
            turn_scale = 1.04719755f / max_turn;
            for (k = 0; k < *(int *)path; k++) {
                point = path + (k << 4);
                *(float *)(point + 0x1C) = *(float *)(point + 0x1C) * turn_scale;
            }
        }
        data->fC = func_001F9D10_2D8388((float *)(path + 0x10), (float *)(path + 0x20));
        *(unsigned char *)(moby + 0x20) = 1;
        data->f8 = (float)*(int *)path * data->f4;
        break;
    }
    case 1: {
        float phase = func_001FA748_2D8388(data->f1C, D_0015ED6C * 12.566370614f);
        float wave;
        data->f1C = phase;
        wave = func_001F9FA8_2D8388(phase);
        *(int *)(moby + 0x90) = func_001FA8A8_2D8388(D_L05_001613D8, D_L05_001613DC, (wave + 1.0f) * 0.5f);
        func_L05_002D8AD8_2D8388(moby, (int *)D_L05_001B0CB0_2D8388[data->path]);
        if (func_L00_0028EB98_2D8388(moby, data->f328) == 0) {
            data->f328 = func_0022ED80_2D8388(1, 4, (int)moby);
        }
        if ((frame_number & 3) == 0) {
            data->f322 = (data->f322 + 1) & 0xF;
            if ((float)data->f320 < D_L05_001613E0) {
                data->f320++;
            }
        }
        left_offset.q = 0;
        left_offset.f[0] = -1.2f;
        left_offset.f[1] = 0.4f;
        left_offset.f[2] = 0.45f;
        func_001F9EC0_2D8388(&left_offset, &left_offset, moby + 0xC0);
        func_001F9BD8_2D8388((float *)&data->trail[0][(short)data->f322], left_offset.f, (float *)(moby + 0x10));
        data->trail[0][(short)data->f322].w = 1.0f;
        tail_offset.q = 0;
        tail_offset.f[0] = -1.5f;
        tail_offset.f[2] = -0.3f;
        func_001F9EC0_2D8388(&tail_offset, &tail_offset, moby + 0xC0);
        func_001F9BD8_2D8388((float *)&data->trail[1][(short)data->f322], tail_offset.f, (float *)(moby + 0x10));
        data->trail[1][(short)data->f322].w = 1.0f;
        right_offset.q = 0;
        right_offset.f[0] = -1.2f;
        right_offset.f[1] = -0.4f;
        right_offset.f[2] = 0.45f;
        func_001F9EC0_2D8388(&right_offset, &right_offset, moby + 0xC0);
        func_001F9BD8_2D8388((float *)&data->trail[2][(short)data->f322], right_offset.f, (float *)(moby + 0x10));
        data->trail[2][(short)data->f322].w = 1.0f;
        if (frame_number != *data->f324) {
            *data->f324 = frame_number;
            func_001F49B0_2D8388(func_L05_002D8B68_2D8388, moby);
        }
        if (*(float *)(moby + 0x10) < 8.0f) {
            *(int *)(moby + 0x94) = 0;
        } else if (*(float *)(moby + 0x14) < 8.0f) {
            *(int *)(moby + 0x94) = 0;
        } else {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
        }
        if (hit != 0 && 0.0f < *(float *)(hit + 0x2C)) damaged = 1;
        if (damaged) *(unsigned char *)(moby + 0x20) = 2;
        break;
    }
    case 2: {
        func_0022ED80_2D8388(0, 0, (int)moby);
        tab = (float *)D_L05_0015F580_macro;
        pos = (float *)(moby + 0x10);
        rot = (float *)(moby + 0x40);
        func_001F9C30_2D8388(&left_offset, moby + 0xC0, D_L05_001613E8 * frame_scale);
        left_offset.f[2] = left_offset.f[2] + D_L05_001613EC * frame_scale;
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby, 0x72E, pos, rot, lifetime, 0, left_offset.f, tab, D_0015ED70 * 12.0f, tab);
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby, 0x72E, pos, rot, lifetime, 0, left_offset.f, tab, D_0015ED70 * 12.0f, tab);
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby, 0x72E, pos, rot, lifetime, 0, left_offset.f, tab, D_0015ED70 * 12.0f, tab);
        func_0020D678_2D8388(moby);
        return;
    }
    }
    if (D_L05_001613E4) func_L05_002D8268_2D8388(moby);
}
/* Retail quad packet: vertices +0x00, colors +0x40, UVs +0x50, commands +0x70. */
typedef struct {
    char vertices[0x40];
    int color[4];
    float uv[4][2];
    u64 zero;
    u64 effect_texture;
    u64 texture;
    u64 flags;
} L05GlowPacket;

int D_L05_001613F0 = 0;
int D_L05_001613F4 = 0;
int D_L05_001613F8 = 0;
int D_L05_001613FC = 2;
int D_L05_00161400 = 0;
int D_L05_00161404 = 1;
int D_L05_00161408 = 0x40806060;
int D_L05_0016140C = 0x802020;
extern unsigned char D_L05_0015EDB2;
extern int frame_number __asm__("D_L05_0015F5CC");
extern struct Moby *D_L05_0015FFD8_7920 __asm__("D_L05_0015FFD8");
extern short *D_L05_001ABCC0_7920[] __asm__("D_L05_001ABCC0");
float D_L05_001CDD70[4][2] = {
    {0.0f, 0.5f},
    {1.0f, 0.5f},
    {0.0f, 0.5f},
    {1.0f, 0.5f},
};
extern u64 get_effect_texture_7920(int) __asm__("FUN_001f44b8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int interpolate_color_7920(float, int, int) __asm__("FUN_001fa6e0");
extern int sphere_in_view_7920(void *, float) __asm__("FUN_001fa728");
extern void transform_glow_vertex_7920(void *, void *, void *) __asm__("FUN_001f9a10");
extern void draw_geometry_quad_7920(void *, int, int) __asm__("FUN_001f7d30");
extern void clear_glow_matrix_7920(void *, int, int) __asm__("FUN_001153fc");

/* Builds 16 fading glow quads, then draws three ribbons of quads for each linked moby of class 0x4F in view. */
void FUN_L05_002d7920(char *moby) {
    L05GlowPacket packet[16];
    float matrix[8];
    float center[4];
    int i;
    int j;
    short *list;
    struct Moby *member;
    char *data;
    float phase;
    float ratio;
    u64 texture;
    L05GlowPacket *p;

    if (D_L05_001613F0 != 0)
        return;
    if (D_L05_0015EDB2 != 0)
        D_L05_001613E0 = 16.0f;
    else
        D_L05_001613E0 = 5.0f;
    for (i = 0; i < 16; i++) {
        texture = get_effect_texture_7920(0x13);
        p = &packet[i];
        p->effect_texture = texture;
        p->texture = 0xff9000000260ULL;
        p->zero = 0;
        p->flags = (u64)D_L05_001613F8 | ((u64)D_L05_001613FC << 2) |
                          ((u64)D_L05_00161400 << 4) | ((u64)D_L05_00161404 << 6) |
                          0x8000000000ULL;
        phase = convert_integer_to_float((frame_number + 3) & 3) / (D_L05_001613E0 * 4.0f);
        for (j = 0; j < 4; j++) {
            packet[i].uv[j][0] = D_L05_001CDD70[j][0];
            packet[i].uv[j][1] = D_L05_001CDD70[j][1];
            ratio = convert_integer_to_float(i + 1 - (j >> 1)) / (D_L05_001613E0 + phase);
            if (ratio > 1.0f)
                ratio = 1.0f;
            else if (ratio < 0.0f)
                ratio = 0.0f;
            packet[i].color[j] = interpolate_color_7920(ratio, D_L05_00161408, D_L05_0016140C);
        }
    }
    clear_glow_matrix_7920(matrix, 0, 0x20);
    matrix[2] = 0.15f;
    matrix[6] = -0.15f;
    list = D_L05_001ABCC0_7920[(unsigned char)moby[0x21]];
    do {
        member = (struct Moby *)(((*list & 0x7fff) << 8) + (int)D_L05_0015FFD8_7920);
        if (member->oclass == 0x4f && member->state == 1) {
            data = (char *)member->pvars;
            qcopy_nc(center, &member->pos);
            center[3] = 20.0f;
            if (sphere_in_view_7920(center, 512.0f) != -1) {
                int first;
                int ribbon;
                int k;
                char *points;
                int index;
                int frame;
                for (frame = 0; frame < *(short *)(data + 0x320); frame++) {
                    first = (*(short *)(data + 0x322) - frame + 15) & 15;
                    for (ribbon = 0; ribbon < 3; ribbon++) {
                        if (D_L05_001613F4 == 0) {
                            points = data + (ribbon * 0x100 + 0x20);
                            for (k = 0; k < 4; k++) {
                                index = (first + (k >> 1)) & 15;
                                transform_glow_vertex_7920(packet[frame].vertices + k * 0x10,
                                                           points + (index << 4),
                                                           (char *)matrix + (k & 1) * 0x10);
                            }
                            draw_geometry_quad_7920(&packet[frame], 0, 0);
                        }
                    }
                }
            }
        }
    } while (*list++ >= 0);
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DBF10), where it is exact; names translated to the US level program. */

extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern float FUN_001f9b80(void *, void *);
extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern int FUN_L00_00258b50(void *, void *, void *, void *, float);
extern int fast_dec_timer(int *arg0) __asm__("FUN_001f9740");
extern s32 random_integer_below_c(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void FUN_L01_0026d930(char *p);
extern void FUN_L05_002daf58_c(unsigned char *m) __asm__("FUN_L05_002daf58");
extern void FUN_L05_002db018_c(unsigned char *moby) __asm__("FUN_L05_002db018");
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern char *D_L05_001B0CB0_x[] __asm__("D_L05_001B0930");
extern int func_001F9908_v(void *) __asm__("FUN_001f9740");

void FUN_L05_002dac80(struct Moby *m) {
    char *d = (char *)m->pvars;
    float dir[4];
    FUN_L05_002daf58_c(m);
    switch (m->state) {
    case 0:
        m->state = 1;
        m->update_dist = 0x40;
        if (m->prev_seq != 0)
            blend_moby_animation_c(m, 0, 0, 0);
        if (*(int *)(d + 0xB0) == -1 || *(int *)D_L05_001B0CB0_x[*(int *)(d + 0xB0)] == 0) {
            mark_moby_for_removal(m);
            return;
        }
        FUN_L01_0026d930(d + 0x60);
        *(float *)(d + 0x6C) = 1.0f;
        *(float *)(d + 0x68) = 1.0f;
        {
            int r = truncate_float_to_s32_c(409.6f);
            float f = frame_time * 4.0f;
            *(int *)(d + 0x60) = r;
            *(float *)(d + 0x84) = f;
        }
        break;
    case 1: {
        char *p = D_L05_001B0CB0_x[*(int *)(d + 0xB0)] + *(int *)(d + 0xB4) * 16;
        float yaw = FUN_001f9e90(*(float *)(p + 0x10) - m->pos.x,
                                 *(float *)(p + 0x14) - m->pos.y);
        FUN_L00_0025be00(&m->rot.z, (float *)(d + 0xBC), yaw, frame_time_sq * 1.0471976f,
                         frame_time_sq * 1.0471976f, frame_time * 3.1415927f);
        dir[0] = fast_cos_c(m->rot.z);
        dir[1] = fast_sin_c(m->rot.z);
        dir[2] = 0.0f;
        FUN_L00_00258b50(m, d + 0x60, dir, d + 0x40, 1.0f);
        if (func_001F9908_v(d + 0xB8) != 0 ||
            FUN_001f9b80(&m->pos, D_L05_001B0CB0_x[*(int *)(d + 0xB0)] +
                                       (*(int *)(d + 0xB4) * 16 + 0x10)) < 1.0f) {
            int r = random_integer_below_c(*(int *)D_L05_001B0CB0_x[*(int *)(d + 0xB0)]);
            *(int *)(d + 0xB4) =
                (*(int *)(d + 0xB4) + r) % *(int *)D_L05_001B0CB0_x[*(int *)(d + 0xB0)];
            *(int *)(d + 0xB8) = scale_game_frames(600);
        }
        FUN_L05_002db018_c(m);
        break;
    }
    case 2:
        if (48.0f < FUN_001f9b80(&m->pos, &hero.motion.pos)) {
            m->state = 1;
            m->visible = 1;
            m->flags &= 0xFFFE;
            m->collision = m->pclass->collision;
        }
        break;
    }
}
/* Emits up to two particle effects around the moby, each in a random direction. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC2A8), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32_u(f32) __asm__("FUN_001fa6d0");
extern void FUN_L01_00287158(float *, float, float, int, int, int);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L05_002db018(unsigned char *moby) {
    float v[3];
    if (moby[0x31] != 0) {
        if (random_integer_below(0x13) == 0) {
            float a = random_angle_radians();
            float z = 0.0f;
            float b = random_float_between_alt(z, 0.1f);
            float c = random_float_between_alt(z, 0.5f);
            float e = random_float_between_alt(0.5f, 1.0f);
            int s = func_001FA898_r(random_float_between_alt(120.0f, 180.0f));
            v[0] = fast_cos(a) * b;
            v[1] = fast_sin(a) * b;
            v[2] = z;
            add_vector_xyz(v, v, moby + 0x10);
            FUN_L01_00287158(v, c, e, 0x30002028, 0x2020, scale_game_frames(s));
        }
        if (random_integer_below(4) == 0) {
            float a = random_angle_radians();
            float z = 0.0f;
            float b = random_float_between_alt(z, 0.1f);
            float c = random_float_between_alt(z, 1.75f);
            float e = random_float_between_alt(1.75f, 2.25f);
            int s = func_001FA898_r(random_float_between_alt(120.0f, 180.0f));
            v[0] = fast_cos(a) * b;
            v[1] = fast_sin(a) * b;
            v[2] = z;
            add_vector_xyz(v, v, moby + 0x10);
            FUN_L01_00287158(v, c, e, 0x20002020, 0x2030, scale_game_frames(s));
        }
    }
}
/* Explosion debris: 20 bursts thrown out along dir with random spread, each trailing 10 smoke puffs. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC4C8), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern f32 fast_cos_c2(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern int FUN_001fa6e0(int, int, float);
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern void FUN_L00_0025f8e0(float *v, float s);
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");

void FUN_L05_002db238(void *moby, void *dir) {
    float p[4];
    float v[4];
    float a[4];
    char *pos = (char *)moby + 0x10;
    int i, j;
    for (i = 0; i < 20; i++) {
        float ang, sp, up;
        qcopy(p, pos);
        FUN_L00_0025f8e0(p, 0.25f);
        p[2] += 1.0f;
        ang = random_angle_radians_c();
        sp = random_float_between_c(1.0f, 8.0f);
        up = random_float_between_c(2.0f, 6.0f);
        scale_vector_xyz(v, dir, random_float_between_c(0.0f, 1.0f));
        v[0] += fast_cos_c2(ang) * (sp * frame_time);
        v[1] += fast_sin_c2(ang) * (sp * frame_time);
        v[2] += up * frame_time;
        qcopy(a, v);
        a[2] -= frame_time_sq * 20.0f * (float)scale_game_frames(0x1E);
        for (j = 0; j < 10; j++) {
            float r = random_float_between_c(0.5f, 1.5f);
            int c1, c2, n1, n2;
            v[3] = r * 0.125f;
            a[3] = r * 0.065f;
            FUN_L00_0025f8e0(p, 0.2f);
            FUN_L00_0025f8e0(a, frame_time * 0.5f);
            c1 = FUN_001fa6e0(0x8000EEEE, 0x8000FF90, random_float_between_c(0.0f, 1.0f));
            c2 = FUN_001fa6e0(0xFFEE, 0xFFEE, random_float_between_c(0.0f, 1.0f));
            n1 = scale_game_frames(10);
            n2 = scale_game_frames(0x1E);
            spawn_spark(p, v, a, c1, c2, n1, n2, func_001FA898_r(multiply_global_factor_ed64(random_float_between_c(5.0f, 25.0f))), -1);
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L05_00301f48.s", FUN_L05_00301f48);
#else
extern void spawn_particle_305178(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern int D_L05_0015F5C4;
extern int D_L05_00161A84 __attribute__((sda));
extern float D_L05_00161A98_gp __asm__("D_L05_00161A98") __attribute__((sda));
extern int D_L05_00161A9C __attribute__((sda));
extern int D_L05_00161AA0 __attribute__((sda));
extern int D_L05_00161AA4 __attribute__((sda));
extern int D_L05_00161AA8 __attribute__((sda));
extern float D_L05_00161AAC __attribute__((sda));
extern float D_L05_00161AB0 __attribute__((sda));
extern float D_L05_00161AB4 __attribute__((sda));
extern float D_L05_00161AB8 __attribute__((sda));
extern float D_L05_00161ABC __attribute__((sda));
extern float D_L05_00161AC0 __attribute__((sda));
extern float D_L05_00161AC4 __attribute__((sda));
extern int D_L05_00161AC8 __attribute__((sda));
extern int D_001404A8;
extern float distance_2d1688(void *, void *) __asm__("FUN_001f9b48");
extern float distance_to_2d1688(void *, void *) __asm__("FUN_001f9b80");
extern float absolute_2d1688(float) __asm__("FUN_001f99c0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void mark_moby_2d1688(char *) __asm__("FUN_0020c828");
extern char D_L05_001671C0[];
extern float D_0013F3D0[4];
extern void FUN_L01_0026d930(char *);
extern int FUN_L00_00258b50(void *, void *, void *, void *, float);
extern void FUN_L05_0028be78(void *, int, int);
extern void FUN_L05_00303390(struct Moby *);
extern void FUN_L05_00303b08(struct Moby *, int, float);
extern void FUN_L00_00263b38(void *, float);
extern void FUN_L00_0025a120(void *);
extern int follow_path(void *, int, int, void *, int, void *, float) __asm__("FUN_L01_00276fe8");
extern float angle_diff(float, float) __asm__("FUN_001fa688");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern int FUN_L00_00261d78(int, void *, void *, float);
extern int FUN_L00_00261b48(int, void *, void *, void *);
extern int FUN_L00_0025c698(void *, void *);
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float FUN_0020c9e0(struct Moby *);
extern int FUN_001f9770(void *);
extern void FUN_L00_00259bc8(void *, int, int, void *, void *, float);
extern void FUN_L00_0025f090(void *, void *, int, float, float);
#define F301(p,o) (*(float *)((char *)(p)+(o)))
#define I301(p,o) (*(int *)((char *)(p)+(o)))
#define H301(p,o) (*(unsigned short *)((char *)(p)+(o)))
#define S301(p,o) (*(short *)((char *)(p)+(o)))
#define B301(p,o) (*(unsigned char *)((char *)(p)+(o)))
void FUN_L05_00301f48(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    struct Moby *target;
    float v[4] __attribute__((aligned(16)));
    float w[4] __attribute__((aligned(16)));
    float point[4] __attribute__((aligned(16)));
    float pos[4] __attribute__((aligned(16)));
    float distance, angle;
    float *rotation;
    float height_limit;
    float height;
    int timer, i, n1, n2, n3;
    int blocked;
    FUN_L00_00263b38(data + 0x170, 2.5f);
    if (D_L05_0015F5C4 == 2) {
        if (((moby->flags ^ 1) & 1) == 0) goto check_freeze;
        I301(data,0x2dc) = 1;
        moby->flags |= 0x41;
        goto freeze_done;
    }
check_freeze:
    if (I301(data,0x2dc) != 0 && D_L05_0015F5C4 == 0) {
        I301(data,0x2dc) = 0;
        moby->flags &= 0xffbe;
    }
freeze_done:
    if (moby->visible && distance_2d1688((char *)moby + 0x10, D_L05_001671C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x16;
    }
    FUN_L05_00303390(moby);
    target = *(struct Moby **)(data + 0x160);
    moby->scale = moby->pclass->scale * D_L05_00161A98_gp;
    switch (moby->state) {
    case 0:
        moby->flags |= 0x1000;
        qcopy(data + 0x270, (char *)moby + 0x10);
        F301(data,0x20) = 2.0f;
        B301(data,0x29) = 0;
        I301(data,0x2b0) = 0;
        B301(data,0x28) = 1;
        F301(data,0x30) = 1.0f;
        S301(data,0x24) = 2;
        B301(data,0x58) = func_001FA898_r(14.0f);
        B301(data,0x5a) = func_001FA898_r(11.2f);
        FUN_L01_0026d930(data + 0xd0);
        distance = D_L05_00161A98_gp * 409.6f;
        F301(data,0xdc) = 2.0f;
        F301(data,0xd8) = 2.0f;
        timer = func_001FA898_r(distance);
        {
            float speed = frame_time * 6.0f;
            I301(data,0xd0) = timer;
            F301(data,0xf4) = speed;
        }
        F301(data,0x2d8) = random_angle_radians();
        if (random_integer_below(2)) moby->flags |= 0x8000;
        else moby->flags &= 0x7fff;
        if (I301(data,0x2b4)) {
            moby->state = 12;
            moby->visible = 0;
            moby->flags &= 0xefff;
            moby->pos.z -= 20.0f;
            moby->collision = 0;
            moby->draw_dist = 0;
        } else {
            if (*(long *)(data + 0x2b8) != -1) {
                moby->state = 15;
                moby->flags &= 0xefff;
                if (moby->prev_seq) blend_moby_animation(moby,0,0,0);
            } else {
                moby->state = 1;
                if (moby->prev_seq) blend_moby_animation(moby,0,0,0);
            }
        }
        FUN_L05_0028be78(data + 0x2c0,1,I301(data,0x2c4));
        break;
    case 1:
        if (I301(data,0x164) != 2) {
            if (random_integer_below(9) == 0) {
                if (follow_path(data+0x2c0,1,I301(data,0x2c4),(char *)moby+0x10,(int)((char *)target+0x10),v,D_L05_00161A98_gp*0.4f) != 0) {
                    if (random_integer_below(0x100)&1) F301(data,0x298) = -F301(data,0x298);
                    F301(data,0x294) = F301(data,0x298)*0.017453292f;
                    I301(data,0x2ac) = FUN_001f96f8((int)random_float_between(30.0f,90.0f));
                    moby->state = 4;
                    if (moby->prev_seq != 1) blend_moby_animation(moby,1,0,FUN_001f96f8(10));
                } else {
                    I301(data,0x164) = 2;
                }
            }
        }
check_mode:
        if (I301(data,0x164) == 2 && I301(data,0x2b0)) {
            moby->state = 6;
            if (moby->prev_seq != 1) blend_moby_animation(moby,1,0,FUN_001f96f8(10));
            goto done;
        }
        if (!(moby->anim_flags&2)) break;
        if (moby->visible && (random_integer_below(255)&1)) {
            random_vector((float *)(data+0x280),0.0f,4.0f);
            F301(data,0x288) = 0.0f;
            fast_vec_add(data+0x280,data+0x280,data+0x270);
            H301(data,0x2d2) = 0;
            moby->command = moby->state;
            moby->state = 2;
            if (moby->prev_seq != 1) blend_moby_animation(moby,1,0,FUN_001f96f8(10));
            goto done;
        }
        if (moby->prev_seq != 0) blend_moby_animation(moby,0,0,FUN_001f96f8(10));
            goto done;
    case 2:
        rotation=&moby->rot.z;
        angle = FUN_001f9e90(F301(data,0x280)-moby->pos.x,F301(data,0x284)-moby->pos.y);
        approach_angle(rotation,angle,(float *)(data+0x290),frame_time_sq*6.2831855f,frame_time_sq*6.2831855f,frame_time*6.2831855f);
        if (angle_diff(moby->rot.z,FUN_001f9e90(F301(data,0x280)-moby->pos.x,F301(data,0x284)-moby->pos.y)) < 1.5707964f) {
            v[0] = FUN_001f9dc8(moby->rot.z); v[0] += v[0];
            v[1] = FUN_001f9de0(moby->rot.z); v[1] += v[1];
            v[2] = 0.0f;
            FUN_L00_00258b50(moby,data+0xd0,v,w,1.0f);
            FUN_L00_00261d78(I301(data,0x2c0),(char *)moby+0x10,(char *)moby+0x10,D_L05_00161A98_gp*0.4f);
        }
        H301(data,0x2d2) += 1;
        distance=distance_to_2d1688((char *)moby+0x10,data+0x280);
        timer=S301(data,0x2d2);
        if (distance < 1.0f) {
            if (timer > FUN_001f96f8(60)) goto finish_wander;
            timer=S301(data,0x2d2);
        }
        if (timer <= FUN_001f96f8(180)) break;
finish_wander:
        moby->state = moby->command;
        if (moby->prev_seq != 0) blend_moby_animation(moby,0,0,FUN_001f96f8(20));
            goto done;
    case 6:
        rotation=&moby->rot.z;
        approach_value((float *)(data+0xf4),frame_time*6.0f,frame_time_sq*6.0f);
        angle=FUN_001f9e90(target->pos.x-moby->pos.x,target->pos.y-moby->pos.y);
        approach_angle(rotation,angle,(float *)(data+0x290),frame_time_sq*6.2831855f,frame_time_sq*6.2831855f,frame_time*6.2831855f);
        if (angle_diff(moby->rot.z,FUN_001f9e90(target->pos.x-moby->pos.x,target->pos.y-moby->pos.y)) < 1.5707964f) {
            v[0]=FUN_001f9dc8(moby->rot.z); v[0]+=v[0];
            v[1]=FUN_001f9de0(moby->rot.z); v[1]+=v[1];
            v[2]=0.0f;
            FUN_L00_00258b50(moby,data+0xd0,v,w,1.0f);
            FUN_L00_00261d78(I301(data,0x2c0),(char *)moby+0x10,(char *)moby+0x10,D_L05_00161A98_gp*0.4f);
        }
        blocked = 0;
        if (FUN_L00_00261b48(I301(data,0x2c0),(char *)moby+0x10,(char *)target+0x10,point) && distance_2d1688((char *)moby+0x10,point)<1.5f) blocked = 1;
        if (blocked) {
            moby->state=5;
            if (moby->prev_seq != 4) blend_moby_animation(moby,4,0,FUN_001f96f8(20));
            goto done;
        }
        if (I301(data,0x2b0)==0) { moby->state=10; break; }
        if (distance_to_2d1688((char *)moby+0x10,(char *)target+0x10)<3.0f) {
            moby->state=7;
            H301(data,0x2d0)=FUN_001f96f8(D_L05_00161AC8);
            if (moby->prev_seq != 2) blend_moby_animation(moby,2,0,FUN_001f96f8(10));
            goto done;
        }
        break;
    case 5:
        if (I301(data,0x2b0)==0) {
            F301(data,0xf4)=0.0f;
            moby->state=10;
            if (moby->prev_seq != 1) blend_moby_animation(moby,1,0,FUN_001f96f8(20));
            goto done;
        }
        if (distance_to_2d1688((char *)moby+0x10,(char *)target+0x10) < 3.0f) {
            moby->state=7;
            H301(data,0x2d0)=FUN_001f96f8(D_L05_00161AC8);
            if (moby->prev_seq != 2) blend_moby_animation(moby,2,0,FUN_001f96f8(10));
        }
        break;
    case 15:
        if ((I301(data,0x2b8)!=-1 && B301(((char *)D_L05_0015FFD8_7920)+I301(data,0x2b8)*256,0xbc)) || (I301(data,0x2bc)!=-1 && is_point_inside_clip_volume(D_0013F3D0,I301(data,0x2bc)))) {
            moby->draw_dist=0x40; moby->state=1; moby->flags|=0x1000;
            break;
        }
        if (!(moby->anim_flags&2)) break;
        if (random_integer_below(255)&1) {
            random_vector((float *)(data+0x280),0.0f,4.0f);
            F301(data,0x288)=0.0f;
            fast_vec_add(data+0x280,data+0x280,data+0x270);
            H301(data,0x2d2)=FUN_001f96f8(90);
            moby->command=moby->state; moby->state=2;
            if (moby->prev_seq != 1) blend_moby_animation(moby,1,0,FUN_001f96f8(10));
            goto done;
        }
        if (moby->prev_seq != 0) blend_moby_animation(moby,0,0,FUN_001f96f8(10));
            goto done;
    case 4:
        distance=distance_to_2d1688((char *)moby+0x10,(char *)target+0x10);
        FUN_L05_00303b08(moby,(int)((char *)target+0x10),F301(data,0x294));
        if (distance < 3.0f && angle_diff(moby->rot.z,FUN_001f9e90(target->pos.x-moby->pos.x,target->pos.y-moby->pos.y)) < 0.7853982f) {
            moby->state=7;
            H301(data,0x2d0)=FUN_001f96f8(D_L05_00161AC8);
            if (moby->prev_seq!=2) blend_moby_animation(moby,2,0,FUN_001f96f8(10));
        }
        if (I301(data,0x164)==2) moby->state=10;
        break;
    case 7: {
        float angle;
        float zero;
        rotation=&moby->rot.z;
        angle=FUN_001f9e90(target->pos.x-moby->pos.x,target->pos.y-moby->pos.y);
        approach_angle(rotation,angle,(float *)(data+0x290),frame_time_sq*12.566371f,frame_time_sq*12.566371f,frame_time*25.132742f);
        if ((moby->anim_flags&2) && moby->prev_seq==2) blend_moby_animation(moby,7,0,FUN_001f96f8(5));
        if (!FUN_001f9770(data+0x2d0)) break;
        moby->state=8;
        H301(data,0x2d0)=0;
        if (moby->prev_seq!=3) blend_moby_animation(moby,3,0,FUN_001f96f8(1));
        zero=0.0f;
        angle=FUN_001fa580(moby->rot.z,zero);
        v[0]=FUN_001f9dc8(angle)*(D_L05_00161AB8*frame_time);
        v[1]=FUN_001f9de0(angle)*(D_L05_00161AB8*frame_time);
        v[2]=zero; v[3]=D_L05_00161AC0;
        w[2]=zero; w[3]=D_L05_00161AC4;
        moby_point_position(moby,0,point);
        i = 29;
        do {
            qcopy(pos,point);
            distance=random_float_between(-0.065f,0.065f);
            i--;
            pos[0]+=distance;
            pos[1]+=random_float_between(-0.065f,0.065f);
            pos[2]+=random_float_between(-0.065f,0.065f);
            w[0]=random_float_between(-frame_time,frame_time);
            w[1]=random_float_between(-frame_time,frame_time);
            w[2]=random_float_between(-(D_L05_00161ABC*frame_time),D_L05_00161ABC*frame_time);
            n1=func_001FA898_r(multiply_global_factor_ed64(random_float_between(D_L05_00161AAC,D_L05_00161AAC+D_L05_00161AAC)));
            n2=func_001FA898_r(multiply_global_factor_ed64(random_float_between(D_L05_00161AB0,D_L05_00161AB0*1.2f)));
            n3=func_001FA898_r(multiply_global_factor_ed64(random_float_between(D_L05_00161AB4,D_L05_00161AB4*1.2f)));
            spawn_spark(pos,v,w,D_L05_00161A9C,D_L05_00161AA0,n1,n2,n3,-1);
        } while (i >= 0);
        v[3]=D_L05_00161AC0*0.1f; w[3]=D_L05_00161AC4*0.1f;
        {
        int i = 29;
        do {
            qcopy(pos,point);
            distance=random_float_between(-0.065f,0.065f);
            i--;
            pos[0]+=distance;
            pos[1]+=random_float_between(-0.065f,0.065f);
            pos[2]+=random_float_between(-0.065f,0.065f);
            w[0]=random_float_between(-frame_time,frame_time);
            w[1]=random_float_between(-frame_time,frame_time);
            w[2]=random_float_between(-(D_L05_00161ABC*frame_time),D_L05_00161ABC*frame_time);
            n1=func_001FA898_r(multiply_global_factor_ed64(random_float_between(D_L05_00161AAC,D_L05_00161AAC+D_L05_00161AAC)));
            n2=func_001FA898_r(multiply_global_factor_ed64(random_float_between(D_L05_00161AB0,D_L05_00161AB0*1.2f)));
            n3=func_001FA898_r(multiply_global_factor_ed64(random_float_between(D_L05_00161AB4,D_L05_00161AB4*1.2f)));
            spawn_spark(pos,v,w,D_L05_00161AA4,D_L05_00161AA8,n1,n2,n3,-1);
        } while (i >= 0);
        }
        break;
    }
    case 8: {
        int flags;
        angle=FUN_001fa580(moby->rot.z,0.0f);
        if (FUN_0020c9e0(moby)<1.5f && angle_diff(angle,FUN_001f9e90(target->pos.x-moby->pos.x,target->pos.y-moby->pos.y))<0.2617994f && distance_to_2d1688((char *)moby+0x10,(char *)target+0x10)<3.0f && absolute_2d1688(target->pos.z-moby->pos.z)<2.0f) {
            v[0]=FUN_001f9dc8(moby->rot.z);
            v[1]=FUN_001f9de0(moby->rot.z);
            v[2]=0.0f; v[3]=5627.925f;
            qcopy(w,(char *)target+0x10);
            flags=0x10000;
            if (D_001404A8!=6) flags=0x10001;
            w[2]+=0.75f;
            FUN_L00_00259bc8(target,(int)moby,flags,w,v,1.0f);
        }
        if (moby->anim_flags&2) { moby->state=9; if (moby->prev_seq != 4) blend_moby_animation(moby,4,0,FUN_001f96f8(10));
            goto done; }
        break;
    }
    case 9:
        if (!(moby->anim_flags&2)) break;
        if (distance_to_2d1688((char *)moby+0x10,(char *)target+0x10)<3.0f && angle_diff(moby->rot.z,FUN_001f9e90(target->pos.x-moby->pos.x,target->pos.y-moby->pos.y))<0.2617994f) {
            moby->state=7; H301(data,0x2d0)=FUN_001f96f8(D_L05_00161AC8);
            if (moby->prev_seq != 7) blend_moby_animation(moby,7,0,FUN_001f96f8(5));
            goto done;
        }
        F301(data,0xf4)=0.0f; moby->state=4;
        if (moby->prev_seq != 1) blend_moby_animation(moby,1,0,FUN_001f96f8(10));
            goto done;
    case 10:
        FUN_L05_00303b08(moby,(int)(data+0x270),0.0f);
        if (distance_to_2d1688((char *)moby+0x10,data+0x270)<1.0f) {
            moby->state=1; if (moby->prev_seq != 0) blend_moby_animation(moby,0,0,FUN_001f96f8(3));
            goto done;
        }
        if (I301(data,0x164)!=2) {
            moby->state=4; if (moby->prev_seq != 1) blend_moby_animation(moby,1,0,FUN_001f96f8(3));
            goto done;
        } else {
            if (I301(data,0x2b0)) moby->state=6;
        }
        break;
    case 11:
        if (FUN_L00_0025c698(moby,data+0x70)&0x60) {
            if (follow_path(data+0x2c0,1,I301(data,0x2c4),(char *)moby+0x10,(int)(data+0x270),v,D_L05_00161A98_gp*0.4f)!=0) {
                moby->state=4;
                if (moby->prev_seq != 1) blend_moby_animation(moby,1,0,FUN_001f96f8(10));
                goto done;
            }
            FUN_L00_0025f090(moby,(char *)moby+0x10,-1,0.5f,13.0f);
            goto remove;
        }
        height_limit=2.0f;
        height=moby->pos.z;
        goto check_height;
    case 13:
        if (moby->anim_flags&2) { moby->state=1; if (moby->prev_seq != 0) blend_moby_animation(moby,0,0,FUN_001f96f8(10));
            goto done; }
        break;
    case 14:
        if (FUN_L00_0025c698(moby,data+0x70)&0x60) {
            FUN_L00_0025f090(moby,(char *)moby+0x10,-1,0.5f,13.0f);
            spawn_particle_305178((char *)moby,0x6da,(float *)((char *)moby+0x10),(char *)moby+0x40,0,0,0.0f,(float *)D_L05_0015F580_macro,(float *)D_L05_0015F580_macro,(float *)D_L05_0015F580_macro);
            spawn_particle_305178((char *)moby,0x6db,(float *)((char *)moby+0x10),(char *)moby+0x40,0,0,0.0f,(float *)D_L05_0015F580_macro,(float *)D_L05_0015F580_macro,(float *)D_L05_0015F580_macro);
            spawn_particle_305178((char *)moby,0x6dc,(float *)((char *)moby+0x10),(char *)moby+0x40,0,0,0.0f,(float *)D_L05_0015F580_macro,(float *)D_L05_0015F580_macro,(float *)D_L05_0015F580_macro);
            goto remove;
        }
        height=moby->pos.z;
        height_limit=0.0f;
        goto check_height;
    }
    goto done;
check_height:
    if (height_limit>height) goto remove;
    goto done;
remove:
    mark_moby_2d1688((char *)moby);
    return;
done:
    D_L05_00161A84=*(volatile int *)0x10000000;
}
#undef F301
#undef I301
#undef H301
#undef S301
#undef B301
#endif
#include "rnc/math/vector.h"

/* Level 5 moby update (a flying enemy): takes hits through func_L00_0025B4D0 and reacts per hit kind, then
   follows its path, retargets the closest moby and turns its head toward the hero. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003047E8), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int u128_3047E8 __attribute__((mode(TI)));

typedef float Vec_3047E8[4] __attribute__((aligned(16)));

typedef struct Hit_3047E8 {
    Vec_3047E8 at;
    u128_3047E8 dir;
    Moby *who;
} Hit_3047E8;

typedef struct Path_3047E8 {
    int n;
    char p0[0xC];
    int pts[1];
} Path_3047E8;

typedef struct Wing_3047E8 {
    char pad0[0x20];
    float hp;
    char pad1[0x14];
    int f38;
    char pad2[0x24];
    short f60[3];
    char pad3[0x1];
    unsigned char b67;
    char pad4[0x8];
    char f70[16];
    float f80;
    char pad5[0x4];
    float f88;
    float f8C;
    int f90;
    int f94;
    float f98;
    char pad6[0x11];
    unsigned char bAD;
    char pad7[0x12];
    float fC0;
    float fC4;
    char pad8[0x58];
    char f120[8];
    float f128;
    char pad9[0x34];
    struct Moby * target;
    int f164;
    char pad10[0x8];
    char f170[16];
    char pad11[0x54];
    float f1D4;
    float f1D8;
    char pad12[0x14];
    char f1F0[16];
    char pad13[0x58];
    float f258;
    char pad14[0x14];
    char f270[16];
    char pad15[0x1C];
    float f29C;
    float f2A0;
    char pad16[0xC];
    int f2B0;
    char pad17[0x14];
    int f2C8;
    int f2CC;
    char pad18[0x4];
    int f2D4;
    float f2D8;
} Wing_3047E8;

char *FUN_L05_00303f80_c(char *self, int idx) __asm__("FUN_L05_00303f80");
extern char *FUN_L00_0025a420(void *, int, int);
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin_c3(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_float_between_c2(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float frame_scale_sq __asm__("D_0015ED64");
extern float D_0015ED6C;
extern float D_0015ED70;
int D_L05_00161A74 = 0x80004000;
int D_L05_00161A78 = 0x80008000;
extern float D_L05_00161A88 __attribute__((sda));
extern float D_L05_00161A8C __attribute__((sda));
extern float D_L05_00161A90 __attribute__((sda));
extern float D_L05_00161A94 __attribute__((sda));
extern float D_L05_00161A98_gp __asm__("D_L05_00161A98") __attribute__((sda));
extern float D_L05_00161AD0 __attribute__((sda));
extern float D_L05_00161AD4 __attribute__((sda));
extern int coll_line_fix() __asm__("FUN_001efa68");
extern int fun_00213928() __asm__("FUN_00213928");
extern s32 truncate_float_to_s32_c3(f32) __asm__("func_001FA6D0");
extern void FUN_L00_002628d8(char *, char *, int, float, float);
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
int FUN_L00_0025ff38(float best, char *a, char *o, int *excluded, int excluded_count, void *region, int region_count);
void FUN_L00_00257470(unsigned char *m, int flags, int arg);
void FUN_L00_0025ab48(void *v, float *ang, float *s1, float *s2);
void FUN_L00_0025c558(u8 *a, void *b, int c, int d, int e, float f);
void FUN_L00_0025d458(void *a, void *s);
void FUN_L00_0025d538(s32 id, void *s);
extern int func_001F9908_3047E8(int *) __asm__("FUN_001f9740");
extern Hit_3047E8 *func_L00_0025B478_3047E8(void *, int, int) __asm__("FUN_L00_0025a420");
extern float func_001F9D10_3047E8(void *, void *) __asm__("FUN_001f9b48");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int update_collision_l08(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern int func_001FA898_3047E8(float) __asm__("FUN_001fa6d0");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern void trooper_knockback_arc(void *, float *, float *, float *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void stop_animation_l08(void *, int, int) __asm__("FUN_L00_00257470");
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, short *) __asm__("FUN_L00_0025d538");
extern float func_002140F8_3047E8(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int activate_effect_l08(float, void *, void *, int, int, int *, int) __asm__("FUN_L00_0025ff38");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float func_001F9B88_3047E8(float) __asm__("FUN_001f99c0");
extern Moby *func_L05_003053D8_3047E8(void *, int) __asm__("FUN_L05_00303f80");
extern float func_001FA748_3047E8(float, float) __asm__("FUN_001fa580");
extern float func_001F9FA8_3047E8(float) __asm__("FUN_001f9de0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void func_001F9BF0_3047E8(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_001FA790_3047E8(float, float) __asm__("FUN_001fa5c8");
extern float func_001F9CE8_3047E8(void *) __asm__("FUN_001f9b20");
extern void update_joint_animation(void *, void *, int, float, float) __asm__("FUN_L00_002628d8");
extern Path_3047E8 *D_L05_001B0CB0_3047E8[] __asm__("D_L05_001B0930");
extern char D_0013E633_3047E8[] __asm__("D_0013F420");
extern Moby *D_001413D0_3047E8 __asm__("D_001413D0") __attribute__((section(".data")));

void FUN_L05_00303390(Moby *m) {
    Vec_3047E8 v0;
    Vec_3047E8 v10;
    int cnt;
    float dmg;
    float ang;
    float ang2;
    Wing_3047E8 *d;
    Hit_3047E8 *r;
    int res;
    int k;

    d = *(struct Wing_3047E8 **)&m->pvars;
    func_001F9908_3047E8(&d->f2D4);
    dmg = 0.0f;
    r = func_L00_0025B478_3047E8(m, 0x330000, 0);
    if (r != 0) {
        qcopy(v0, ((float *)&m->pos));
        v0[2] = v0[2] + 0.5f;
        if (r->who != 0) {
            if (r->who->oclass == 0x47) {
                qcopy(v10, D_0013E633_3047E8);
            } else if ((*(struct Moby **)&r->who->pclass != 0 && *(short *)((u8 *)((struct Moby *)r->who->pclass) + 0x46) == 5) || r->who->oclass == 0xBA) {
                qcopy(v10, v0);
            } else {
                qcopy(v10, ((float *)&r->who->pos));
            }
        } else {
            qcopy(v10, r->at);
        }
        if (func_001F9D10_3047E8(v0, v10) < 24.0f && collision_line(v0, v10, 2, m, 0) != 0) {
            r = 0;
        }
    }
    res = update_collision_l08(m, r, &d->hp, 0, &cnt, &dmg, 0, 4);
    if (cnt != 1 && m->state != 14) {
        d->hp = d->hp - dmg;
        if (d->hp <= 0.0f) {
            res = 1;
        }
        k = func_001FA898_3047E8(D_L05_00161A98_gp * 409.6f);
        d->f98 = D_L05_00161A98_gp * 0.4f;
        d->f80 = D_L05_00161AD0 * D_0015ED70;
        d->f90 = k;
        d->f94 = 9;
        d->bAD = 0;
        switch (res) {
        case 0:
        case 11:
            break;
        case 9:
        case 10:
            d->b67 = 0xFA;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            d->fC0 = 5.0f;
            d->f88 = D_L05_00161A88 * D_0015ED6C;
            d->f8C = D_L05_00161A8C * D_0015ED6C;
            d->fC4 = 10.0f;
            ang = angle_atan2(m->pos.x - r->who->pos.x, m->pos.y - r->who->pos.y);
            *(u128_3047E8 *)v0 = r->dir;
            trooper_knockback_arc(v0, &ang, &d->f88, &d->f8C);
            trooper_start_knockback(m, d->f70, ang, 5, 1, 0);
            m->state = 11;
            d->b67 = 0x78;
            break;
        case 1:
        case 2:
            m->flags &= 0xEFFF;
            d->f80 = D_L05_00161AD4 * D_0015ED70;
            d->f88 = D_L05_00161A90 * D_0015ED6C;
            d->f8C = D_L05_00161A94 * D_0015ED6C;
            d->fC0 = 5.5f;
            d->fC4 = 11.0f;
            ang2 = angle_atan2(m->pos.x - r->who->pos.x, m->pos.y - r->who->pos.y);
            *(u128_3047E8 *)v0 = r->dir;
            trooper_knockback_arc(v0, &ang2, &d->f88, &d->f8C);
            trooper_start_knockback(m, d->f70, ang2, 6, 1, 0);
            m->state = 14;
            d->b67 = 0xF0;
            if (r->who != 0 && *(struct Moby **)&r->who->pclass != 0 && *(short *)((u8 *)((struct Moby *)r->who->pclass) + 0x46) == 5) {
                stop_animation_l08(m, 0x800, -1);
            } else {
                stop_animation_l08(m, 0, -1);
            }
            break;
        }
        trooper_anim_update(m, d->f60);
    }
    m->hit_slot = 0xFF;
    trooper_anim_finish(m, d->f60);
    if (d->f38 != 0) {
        d->f2B0 = func_001FA898_3047E8(scale_time(func_002140F8_3047E8(180.0f, 240.0f)));
        d->f38 = 0;
    }
    func_001F9908_3047E8(&d->f2B0);
    if (d->f2B0 != 0) {
        d->f2A0 = d->f29C + 20.0f;
    } else {
        d->f2A0 = d->f29C;
    }
    if (activate_effect_l08(d->f2A0, m, d->f120, 0, 0, D_L05_001B0CB0_3047E8[d->f2CC]->pts, D_L05_001B0CB0_3047E8[d->f2CC]->n) != 2) {
        if (d->f2A0 < vector_distance(d->f270, d->f120) || 3.0f < func_001F9B88_3047E8(m->pos.z - d->f128)) {
            d->f164 = 2;
        }
    }
    if (d->f2C8 >= 0) {
        Moby *t = func_L05_003053D8_3047E8(m, d->f2C8);
        if (t != 0) {
            if (d->f164 == 2 || vector_distance(((float *)&m->pos), ((float *)&t->pos)) < vector_distance(((float *)&m->pos), ((float *)&d->target->pos))) {
                d->target = t;
                d->f164 = 1;
            }
        }
    }
    if (d->target == 0) {
        d->target = D_001413D0_3047E8;
    }
    d->f2D8 = func_001FA748_3047E8(d->f2D8, D_0015ED6C * 9.424778f);
    m->glow_color = fast_tween_color(D_L05_00161A74, D_L05_00161A78, (func_001F9FA8_3047E8(d->f2D8) + 1.0f) * 0.5f);
    k = m->state;
    if (k < 10) {
    if (k >= 4 && d->target == D_001413D0_3047E8) {
        float yaw, pitch;
        func_001F9BF0_3047E8(v0, ((float *)&d->target->pos), ((float *)&m->pos));
        yaw = func_001FA790_3047E8(angle_atan2(v0[0], v0[1]), m->rot.z);
        pitch = -angle_atan2(func_001F9CE8_3047E8(v0), v0[2]);
        if (m->flags & 0x8000) {
            yaw = -yaw;
        }
        if (1.5707964f < yaw) {
            yaw = 1.5707964f;
        } else if (yaw < -1.5707964f) {
            yaw = -1.5707964f;
        }
        if (0.5235988f < pitch) {
            pitch = 0.5235988f;
        } else if (pitch < -0.5235988f) {
            pitch = -0.5235988f;
        }
        d->f1D4 = pitch;
        d->f1D8 = yaw * 0.7f;
        d->f258 = yaw * 0.3f;
    }
    }
    update_joint_animation(m, d->f170, 1, frame_scale_sq * 0.04f, frame_scale_sq * 0.3f);
    update_joint_animation(m, d->f1F0, 2, frame_scale_sq * 0.04f, frame_scale_sq * 0.3f);
}
#include "rnc/gameplay/entities/moby.h"

/* Pvars of a path-walking moby (only the fields this walker touches). */
typedef struct {
    u8 pad0[0xD0];
    u8 move[0x24];        /* 0xD0: ground-move state */
    f32 speed;            /* 0xF4 */
    u8 padF8[0x164 - 0xF8];
    s32 mode;             /* 0x164: 2 once the path is finished */
    u8 pad168[0x280 - 0x168];
    Vec4f goal;           /* 0x280: current path target */
    f32 yaw_vel;          /* 0x290 */
    u8 pad294[0x2C0 - 0x294];
    s32 path;             /* 0x2C0 */
    s32 path_pos;         /* 0x2C4 */
    u8 pad2C8[0x2D4 - 0x2C8];
    s32 stuck_timer;      /* 0x2D4 */
} PathWalkerVars;

extern f32 D_L05_00161A98_gp __asm__("D_L05_00161A98") __attribute__((sda));
extern s32 follow_path(void *, s32, s32, void *, s32, void *, f32) __asm__("FUN_L01_00276fe8");
extern f32 angle_diff(f32, f32) __asm__("FUN_001fa688");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32 approach_angle(f32 *angle, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025be00");
extern s32 FUN_L05_00303e50(char *moby, float *goal, float s);
extern f32 FUN_L05_00304058_u(s32, void *, void *, f32) __asm__("FUN_L05_00304058");

/* Walks m toward its next path node, steering `side` radians off the direct heading while
   FUN_L05_00303e50 reports the way blocked; slows near the goal and arms the stuck timer
   when it drifts off the path. */
void FUN_L05_00303b08(struct Moby *m, s32 node, f32 side) {
    PathWalkerVars *v = (PathWalkerVars *)m->pvars;
    Vec4f dir, out, goal;
    f32 a;
    f32 dist;

    if ((((s32)m >> 8) & 3) == (frame_number & 3)) {
        if (follow_path(&v->path, 1, v->path_pos, &m->pos, node, &v->goal, D_L05_00161A98_gp * 0.4f) == 0)
            v->mode = 2;
    }
    qcopy(&goal, &v->goal);
    dist = FUN_001f9b80(&m->pos, &goal);
    if (dist > 5.0f && side != 0.0f && FUN_L05_00303e50((char *)m, (float *)&goal, D_L05_00161A98_gp * 0.4f)) {
        a = FUN_001fa580(FUN_001f9e90(goal.x - m->pos.x, goal.y - m->pos.y), side);
        approach_angle(&m->rot.z, a, &v->yaw_vel, frame_time_sq * 12.566371f, frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
    } else {
        approach_angle(&m->rot.z, FUN_001f9e90(goal.x - m->pos.x, goal.y - m->pos.y), &v->yaw_vel,
                       frame_time_sq * 12.566371f, frame_time_sq * 12.566371f, frame_time * 25.132742f);
    }
    if (angle_diff(m->rot.z, FUN_001f9e90(goal.x - m->pos.x, goal.y - m->pos.y)) < 1.5707964f && dist > 3.0f)
        approach_value(&v->speed, frame_time * 6.0f, frame_time_sq * 6.0f);
    else
        approach_value(&v->speed, frame_time, frame_time_sq * 6.0f);
    dir.x = FUN_001f9dc8(m->rot.z) * 2.0f;
    dir.y = FUN_001f9de0(m->rot.z) * 2.0f;
    dir.z = 0.0f;
    FUN_L00_00258b50(m, v->move, &dir, &out, 1.0f);
    if (v->stuck_timer == 0) {
        if (FUN_L05_00304058_u(v->path, &m->pos, &m->pos, D_L05_00161A98_gp * 0.4f) > 2.0f)
            v->stuck_timer = FUN_001f96f8(5);
    }
}
extern float FUN_001f9e90_c(float, float) __asm__("FUN_001f9e90");
extern float FUN_001fa580_c(float, float) __asm__("FUN_001fa580");
extern float FUN_001f9dc8_c(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_c(float) __asm__("FUN_001f9de0");
extern float FUN_001f9b48_c(float *, float *) __asm__("FUN_001f9b48");
extern void FUN_001f9a10_c(float *, float *, float *) __asm__("FUN_001f9a10");
extern void FUN_001f9a28_c(float *, float *, float *) __asm__("FUN_001f9a28");
extern void FUN_001f9bf8_c(float *, float *, float) __asm__("FUN_001f9bf8");
extern int FUN_L00_00261968_c(int, float *, float *) __asm__("FUN_L00_00261968");

int FUN_L05_00303e50(char *moby, float *goal, float s)
{
    float v[4];
    float w[4];
    char *d = *(char **)(moby + 0x78);
    float a;
    a = FUN_001f9e90_c(goal[0] - *(float *)(moby + 0x10), goal[1] - *(float *)(moby + 0x14));
    a = FUN_001fa580_c(a, *(float *)(d + 0x294) * 1.5f);
    v[0] = FUN_001f9dc8_c(a) * FUN_001f9b48_c(goal, (float *)(moby + 0x10));
    v[1] = FUN_001f9de0_c(a) * FUN_001f9b48_c(goal, (float *)(moby + 0x10));
    *(int *)&v[2] = 0;
    FUN_001f9a10_c(v, v, (float *)(moby + 0x10));
    FUN_001f9a28_c(w, v, goal);
    FUN_001f9bf8_c(w, w, s);
    FUN_001f9a10_c(w, w, (float *)(moby + 0x10));
    return FUN_L00_00261968_c(*(int *)(d + 0x2C0), v, w) == 0;
}
#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
typedef struct {
    char p0[0x10];
    char pos[0x10];
    unsigned char state;
    char p1[0x13];
    unsigned short flags;
    char p2[0xCA];
} Ent_03f80;
extern float func_001F9D10_03f80(void *, void *) __asm__("FUN_001f9b48");
extern short *D_L05_001AC040_03f80[] __asm__("D_L05_001ABCC0");
extern Ent_03f80 *D_L05_00160098_m_03f80 __asm__("D_L05_0015FFD8");
extern short D_L05_00160098_03f80 __asm__("D_L05_0015FFD8");
char *FUN_L05_00303f80(char *self, int idx) {
    short *list = D_L05_001AC040_03f80[idx];
    float best = 120.0f;
    char *res = 0;
    if (list == 0) {
        return 0;
    }
    {
        do {
            int i = *(unsigned short *)list & 0x7FFF;
            if (D_L05_00160098_m_03f80[i].flags & 0x1000) {
                if (D_L05_00160098_m_03f80[i].state < 0x7F) {
                    float d = func_001F9D10_03f80(D_L05_00160098_m_03f80[i].pos, self + 0x10);
                    if (d < best) {
                        best = d;
                        res = *(char **)&D_L05_00160098_03f80 + (i << 8);
                    }
                }
            }
        } while (*list++ >= 0);
    }
    return res;
}
/* Pushes a point clear of a path and returns the nearest wall distance, like
 * FUN_L00_00261d78. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003054B0), where it is exact; names translated to the US level program. */

typedef int Quad_3054B0 __attribute__((mode(TI)));

typedef union {
    Quad_3054B0 q;
    f32 f[4];
    s32 i[4];
} PathVec_3054B0;

typedef struct {
    s32 count;
    u8 pad[12];
    PathVec_3054B0 points[1];
} SegmentPath_3054B0;

extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
extern SegmentPath_3054B0 *segment_paths_3054B0[] __asm__("D_L05_001B0930");
extern void func_001F9BD8_3054B0(PathVec_3054B0 *, PathVec_3054B0 *, PathVec_3054B0 *) __asm__("FUN_001f9a10");
extern void func_001F9BF0_3054B0(PathVec_3054B0 *, PathVec_3054B0 *, PathVec_3054B0 *) __asm__("FUN_001f9a28");
extern void func_001F9CA0_3054B0(PathVec_3054B0 *, PathVec_3054B0 *, PathVec_3054B0 *) __asm__("FUN_001f9ad8");
extern f32 func_001F9CB8_3054B0(PathVec_3054B0 *) __asm__("FUN_001f9af0");
extern void func_L00_001FF4B0_3054B0(PathVec_3054B0 *, PathVec_3054B0 *, f32) __asm__("FUN_001f9bf8");

f32 FUN_L05_00304058(s32 path_index, void *input, void *output, f32 distance) {
    PathVec_3054B0 *point = input;
    PathVec_3054B0 *out = output;
    PathVec_3054B0 difference;
    PathVec_3054B0 scaled_difference;
    PathVec_3054B0 working;
    PathVec_3054B0 segment;
    PathVec_3054B0 scaled_segment;
    PathVec_3054B0 cross;
    PathVec_3054B0 scaled_cross;
    SegmentPath_3054B0 **entry;
    s32 i;
    s32 found;
    f32 best = 512.0f;

    found = 0;
    qcopy_nc(&working, point);
    for (i = 0; i < segment_paths_3054B0[path_index]->count - 1; i++) {
        entry = &segment_paths_3054B0[path_index];
        if ((*entry)->points[i].f[3] == 0.0f && (*entry)->points[i + 1].f[3] == 0.0f)
            continue;
        func_001F9BF0_3054B0(&difference, &working, &(*entry)->points[i]);
        difference.i[2] = 0;
        func_001F9BF0_3054B0(&segment, &(*entry)->points[i + 1], &(*entry)->points[i]);
        segment.i[2] = 0;
        func_L00_001FF4B0_3054B0(&scaled_segment, &segment, 1.0f);
        func_001F9CA0_3054B0(&cross, &difference, &scaled_segment);
        {
            f32 wall_distance = absolute_float(cross.f[2]);
            if (wall_distance < best)
                best = wall_distance;
            if (distance < wall_distance)
                continue;
        }
        {
            f32 segment_length = func_001F9CB8_3054B0(&segment);
            f32 projection = dot_vectors_xyz(&difference, &scaled_segment);
            if (segment_length < projection || projection < 0.0f) {
                if (!(func_001F9CB8_3054B0(&difference) < distance))
                    continue;
                found = 1;
                func_L00_001FF4B0_3054B0(&scaled_difference, &difference, distance);
                func_001F9BD8_3054B0(&working, &scaled_difference, &(*entry)->points[i]);
                working.f[2] = point->f[2];
            } else {
                found = 1;
                func_L00_001FF4B0_3054B0(&scaled_cross, &scaled_segment, projection);
                func_001F9BF0_3054B0(&scaled_difference, &difference, &scaled_cross);
                func_L00_001FF4B0_3054B0(&scaled_difference, &scaled_difference, distance);
                func_001F9BD8_3054B0(&scaled_difference, &scaled_difference, &scaled_cross);
                func_001F9BD8_3054B0(&working, &scaled_difference, &(*entry)->points[i]);
                working.f[2] = point->f[2];
            }
        }
    }
    if (found)
        qcopy(out, &working);
    return best;
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L05_00304320.s", FUN_L05_00304320);
#else
extern char D_L05_001671C0[];
extern char *D_L05_001B0930[];
extern float D_0013F3D0[4];
extern int frame_number __asm__("D_L05_0015F5CC");
extern float distance_2d1688(void *, void *) __asm__("FUN_001f9b48");
extern float distance_to_2d1688(void *, void *) __asm__("FUN_001f9b80");
extern float absolute_2d1688(float) __asm__("FUN_001f99c0");
extern void mark_moby_2d1688(char *) __asm__("FUN_0020c828");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float FUN_001fa5c8(float, float);
extern float scale_particle_time(float) __asm__("FUN_L05_0022dbc8");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void FUN_L00_0025a120(void *);
extern void FUN_L01_0026d930(char *);
extern void FUN_L05_00305178(char *);
extern void FUN_L05_00305898(char *);
extern int FUN_L00_00259028(char *, char *, void *, char *);
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int collision_line_304320(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern float smooth_angle_304320(float, float, void *, int) __asm__("FUN_L00_0025bc00");
extern void rotate_axis_304320(float, void *, int) __asm__("FUN_L00_001fff28");
extern void attach_manipulator_304320(void *, int, void *) __asm__("FUN_0020cb10");
extern void clear_vector_305178(void *) __asm__("FUN_001f99f8");
extern Moby *D_001413D0_3047E8 __asm__("D_001413D0") __attribute__((section(".data")));
extern float D_L05_00161AF8 __attribute__((sda));
extern float D_L05_00161AFC __attribute__((sda));
extern float D_L05_00161B00 __attribute__((sda));
extern float D_L05_00161B04 __attribute__((sda));
extern float D_L05_00161B08 __attribute__((sda));
extern int D_L05_00161B0C __attribute__((sda));
extern int D_L05_00161B10 __attribute__((sda));
extern float D_L05_00161B14 __attribute__((sda));
extern float D_L05_00161B18 __attribute__((sda));
extern float D_L05_00161B1C __attribute__((sda));
extern float D_L05_00161B20 __attribute__((sda));
extern float D_L05_00161B24 __attribute__((sda));
extern float D_L05_00161B28 __attribute__((sda));
extern float D_L05_00161B2C __attribute__((sda));
extern float D_L05_00161B30 __attribute__((sda));
extern float D_L05_00161B34 __attribute__((sda));
extern float D_L05_00161B38 __attribute__((sda));
extern int D_L05_00161B3C __attribute__((sda));
extern int D_L05_00161B40 __attribute__((sda));
extern float D_L05_00161B44 __attribute__((sda));
extern float D_L05_00161B48 __attribute__((sda));
extern float D_L05_00161B4C __attribute__((sda));
extern float D_L05_00161B50 __attribute__((sda));
extern float D_L05_00161B54 __attribute__((sda));
extern float D_L05_00161B58 __attribute__((sda));
extern float D_L05_00161B5C __attribute__((sda));
extern float D_L05_00161B60 __attribute__((sda));
extern float D_L05_00161B64 __attribute__((sda));
extern float D_L05_00161B68 __attribute__((sda));
extern float D_L05_00161B6C __attribute__((sda));
extern float D_L05_00161B70 __attribute__((sda));
extern int scale_frames_304320(int) __asm__("FUN_L05_0022dbd8");
extern float D_L05_00161B78 __attribute__((sda));
extern float D_L05_00161B7C __attribute__((sda));
void FUN_L05_00304320(char *moby) {
    char *data = *(char **)(moby + 0x78);
    struct Hero *base;
    char *path;
    char *child;
    char *position;
    char *path_position;
    int state;
    int *path_entry;
    int direction;
    int index;
    int last;
    int offset;
    int i;
    int j;
    int n1, n2, n3;
    char *particle;
    float angle;
    float turn;
    float point[4] __attribute__((aligned(16)));
    float v[4] __attribute__((aligned(16)));
    float w[4] __attribute__((aligned(16)));
    float pos[4] __attribute__((aligned(16)));
    void *matrix0;
    void *matrix1;
    void *rotation0;
    void *rotation1;
    FUN_L05_00305178(moby);
    FUN_L05_00305898(moby);
    if ((*(unsigned char *)((char *)(data)+(0x2e))) == 2) {
        if (*(char **)(data+0x184)) mark_moby_2d1688(*(char **)(data+0x184));
        if (*(char **)(data+0x188)) mark_moby_2d1688(*(char **)(data+0x188));
        mark_moby_2d1688(moby);
        return;
    }
    if ((*(unsigned char *)((char *)(moby)+(0x31)))) {
        if (distance_2d1688(moby+0x10,D_L05_001671C0) < 27.0f) {
            FUN_L00_0025a120(moby);
            (*(unsigned char *)((char *)(moby)+(0x7f)))=0x15;
        }
    }
    state=(*(unsigned char *)((char *)(moby)+(0x20)));
    switch (state) {
    case 0: goto state_zero;
    case 1: goto state_one;
    case 2: goto state_two;
    case 3: goto state_three;
    default: return;
    }
state_zero:
    position=moby+0x10;
    if (*(char **)(data+0x184)==0) {
        *(char **)(data+0x184)=create_moby(0x368);
        (*(unsigned char *)((char *)(*(char **)(data+0x184))+(0x30)))=0x40;
        (*(unsigned short *)((char *)(*(char **)(data+0x184))+(0x32)))=0x40;
        (*(unsigned char *)((char *)(*(char **)(data+0x184))+(0x31)))=1;
        base=&hero;
        *(unsigned long *)(*(char **)(data+0x184)+0x38)=*(unsigned long *)((char *)*(Moby **)((char *)base+0x2080)+0x38);
        (*(unsigned short *)((char *)(*(char **)(data+0x184))+(0x34)))=0x4000;
        {
        char *child=*(char **)(data+0x184);
        qcopy(child+0x10,position);
        (*(int *)((char *)(child)+(0x94)))=0;
        }
        FUN_L00_00250df8(*(char **)(data+0x184));
    }
    if (*(char **)(data+0x188)==0) {
        *(char **)(data+0x188)=create_moby(0x369);
        (*(unsigned char *)((char *)(*(char **)(data+0x188))+(0x30)))=0x40;
        (*(unsigned short *)((char *)(*(char **)(data+0x188))+(0x32)))=0x40;
        (*(unsigned char *)((char *)(*(char **)(data+0x188))+(0x31)))=1;
        base=&hero;
        *(unsigned long *)(*(char **)(data+0x188)+0x38)=*(unsigned long *)((char *)*(Moby **)((char *)base+0x2080)+0x38);
        (*(unsigned short *)((char *)(*(char **)(data+0x188))+(0x34)))=0x4000;
        {
        char *child=*(char **)(data+0x188);
        qcopy(child+0x10,position);
        (*(int *)((char *)(child)+(0x94)))=0;
        }
        FUN_L00_00250df8(*(char **)(data+0x188));
    }
    attach_manipulator_304320(moby,0,data+0xf0);
    attach_manipulator_304320(moby,1,data+0x130);
    (*(unsigned short *)((char *)(moby)+(0x34)))|=0x1000;
    base=&hero;
    *(unsigned long *)(moby+0x38)=*(unsigned long *)((char *)*(Moby **)((char *)base+0x2080)+0x38);
    FUN_L01_0026d930(data+0x70);
    (*(int *)((char *)(data)+(0x70)))=0x38d;
    (*(float *)((char *)(data)+(0x7c)))=2.0f;
    (*(float *)((char *)(data)+(0x78)))=4.0f;
    (*(float *)((char *)(data)+(0xac)))=0.01f;
    (*(float *)((char *)(data)+(0xb0)))=0.3f;
    (*(float *)((char *)(data)+(0xb4)))=0.1f;
    path=D_L05_001B0930[(*(int *)((char *)(data)+(0xd4)))];
    *(int *)(data+0xc0)=1;
    *(char **)(data+0xd0)=path;
    qcopy(position,path+0x10);
    (*(unsigned char *)((char *)(moby)+(0x20)))=1;
    if ((*(unsigned char *)((char *)(moby)+(0x53)))) blend_moby_animation(moby,0,0,0);
    (*(float *)((char *)(data)+(0x94)))=D_L05_00161AF8*frame_time;
    clear_vector_305178(data+0x40);
    (*(unsigned char *)((char *)(data)+(0x2e)))=1;
    (*(unsigned char *)((char *)(data)+(0x29)))=0;
    (*(unsigned short *)((char *)(data)+(0x24)))=6;
    (*(unsigned char *)((char *)(data)+(0x28)))=3;
    (*(float *)((char *)(data)+(0x20)))=6.0f;
    (*(unsigned short *)((char *)(data)+(0x3e)))|=2;
    (*(int *)((char *)(data)+(0x180)))=scale_frames_304320(60);
    (*(float *)((char *)(data)+(0x198)))=0.0f;
    (*(float *)((char *)(data)+(0x194)))=(*(float *)((char *)(moby)+(0x48)));
    (*(unsigned char *)((char *)(data)+(0x58)))=func_001FA898_r(D_L05_00161B78);
    (*(unsigned char *)((char *)(data)+(0x5a)))=func_001FA898_r(D_L05_00161B7C*8.0f);
    return;
state_one:
    {
        int count;
        path_entry=(int *)(data+0xc0);
        count=**(int **)((char *)path_entry+0x10);
        direction=*(signed char *)((char *)path_entry+4);
        last=0;
        if (direction>=0) last=count-1;
        index=*path_entry+count;
        index+=direction;
        index%=count;
        path_position=moby+0x10;
        position=path_position;
        offset=index*16;
        offset+=16;
        FUN_L00_00259028(moby,data+0x70,*(char **)((char *)path_entry+0x10)+offset,data+0x40);
        if (distance_to_2d1688(path_position,*(char **)((char *)path_entry+0x10)+offset)<1.0f) {
            if (index==last) {
                path=*(char **)((char *)path_entry+0x10);
                if (distance_2d1688(path+*(int *)path*16,path+16)>5.0f)
                *(signed char *)((char *)path_entry+4)=-*(signed char *)((char *)path_entry+4);
            }
            *path_entry=index;
        }
    }
    if (distance_to_2d1688(position,hero.motion.pos.f)<(*(float *)((char *)(data)+(0x174))) &&
        absolute_2d1688((*(float *)((char *)(moby)+(0x18)))-hero.motion.pos.f[2])<4.0f &&
        ((*(int *)((char *)(data)+(0x190)))==-1 || point_in_path_xy(hero.motion.pos.f,D_L05_001B0930[(*(int *)((char *)(data)+(0x190)))]+16,*(int *)D_L05_001B0930[(*(int *)((char *)(data)+(0x190)))]))) {
        qcopy(point,position);
        point[2]+=3.0f;
        if (!collision_line_304320(point,(char *)&hero+0xd0,2,moby,0)) {
            if ((*(unsigned char *)((char *)(moby)+(0x53)))) blend_moby_animation(moby,0,0,3);
            (*(unsigned char *)((char *)(moby)+(0x20)))=2;
        }
    }
    if (distance_to_2d1688(position,hero.motion.pos.f)<16.0f) {
        angle=FUN_001f9e90(hero.motion.pos.f[0]-(*(float *)((char *)(moby)+(0x10))),hero.motion.pos.f[1]-(*(float *)((char *)(moby)+(0x14))));
        smooth_angle_304320(angle,frame_time*1.5707964f,data+0x194,0);
        smooth_angle_304320(0.08726646f,frame_time*1.5707964f,data+0x198,0);
    } else {
        smooth_angle_304320((*(float *)((char *)(moby)+(0x48))),frame_time*1.5707964f,data+0x194,0);
        smooth_angle_304320(0.0f,frame_time*1.5707964f,data+0x198,0);
    }
    child=data+0x100;
    rotate_axis_304320((*(float *)((char *)(data)+(0x198))),data+0x140,1);
    angle=FUN_001fa5c8((*(float *)((char *)(data)+(0x194))),(*(float *)((char *)(moby)+(0x48))));
    rotate_axis_304320(angle,child,2);
    return;
state_two:
    {
        int count;
        path_entry=(int *)(data+0xc0);
        count=**(int **)((char *)path_entry+0x10);
        direction=*(signed char *)((char *)path_entry+4);
        last=0;
        if (direction>=0) last=count-1;
        index=*path_entry+count;
        index+=direction;
        index%=count;
        path_position=moby+0x10;
        position=path_position;
        offset=index*16;
        offset+=16;
        FUN_L00_00259028(moby,data+0x70,*(char **)((char *)path_entry+0x10)+offset,data+0x40);
        if (distance_to_2d1688(path_position,*(char **)((char *)path_entry+0x10)+offset)<1.0f) {
            if (index==last) {
                path=*(char **)((char *)path_entry+0x10);
                if (distance_2d1688(path+*(int *)path*16,path+16)>5.0f)
                *(signed char *)((char *)path_entry+4)=-*(signed char *)((char *)path_entry+4);
            }
            *path_entry=index;
        }
    }
    {
        int count;
        v[3]=D_L05_00161B30;
        w[3]=D_L05_00161B34;
        moby_point_position(moby,2,point);
        i=0;
        do {
            qcopy(pos,point);
            pos[0]+=random_float_between(-0.065f,0.065f);
            pos[1]+=random_float_between(-0.065f,0.065f);
            pos[2]+=random_float_between(-0.065f,0.065f);
            v[0]=FUN_001f9dc8((*(float *)((char *)(data)+(0x194))))*(D_L05_00161B20*frame_time);
            v[1]=FUN_001f9de0((*(float *)((char *)(data)+(0x194))))*(D_L05_00161B20*frame_time);
            v[2]=0.0f;
            v[2]=random_float_between(D_L05_00161B24*frame_time*0.9f,D_L05_00161B24*frame_time*1.1f);
            turn=random_float_between(-D_L05_00161B38,D_L05_00161B38)*0.017453292f;
            w[0]=FUN_001f9dc8(FUN_001fa580((*(float *)((char *)(data)+(0x194))),turn))*(D_L05_00161B28*frame_time);
            w[1]=FUN_001f9de0(FUN_001fa580((*(float *)((char *)(data)+(0x194))),turn))*(D_L05_00161B28*frame_time);
            w[2]=0.0f;
            w[2]=random_float_between(D_L05_00161B2C*frame_time*0.9f,D_L05_00161B2C*frame_time*1.1f);
            n1=func_001FA898_r(scale_particle_time(random_float_between(D_L05_00161B14,D_L05_00161B14+D_L05_00161B14)));
            n2=func_001FA898_r(scale_particle_time(random_float_between(D_L05_00161B18,D_L05_00161B18*1.2f)));
            n3=func_001FA898_r(scale_particle_time(random_float_between(D_L05_00161B1C,D_L05_00161B1C*1.2f)));
            particle=spawn_spark(pos,v,w,D_L05_00161B0C,D_L05_00161B10,n1,n2,n3,-1);
            count=i+1;
            if (!(frame_number&7) && particle && i==0) {
                count=1;
                j=0;
                if (*(char **)(data+0x1a0)==0) *(char **)(data+0x1a0)=particle;
                else {
                    do {
                        j++;
                        if (j>=8) goto slots_done;
                        path_position=data+0x1a0+j*4;
                    } while (*(char **)(data+0x1a0+j*4));
                    *(char **)path_position=particle;
                }
            }
slots_done:
            i=count;
        } while (i<2);
    }
    v[3]=D_L05_00161B60;
    w[3]=D_L05_00161B64;
    last=1;
    do {
    qcopy(pos,point);
    pos[0]+=random_float_between(-0.2f,0.2f);
    pos[1]+=random_float_between(-0.2f,0.2f);
    pos[2]+=random_float_between(-0.2f,0.2f);
    last--;
    turn=random_float_between(-D_L05_00161B6C,D_L05_00161B6C)*0.017453292f;
    v[0]=FUN_001f9dc8(FUN_001fa580((*(float *)((char *)(data)+(0x194))),turn))*(D_L05_00161B50*frame_time);
    v[1]=FUN_001f9de0(FUN_001fa580((*(float *)((char *)(data)+(0x194))),turn))*(D_L05_00161B50*frame_time);
    v[2]=0.0f;
    v[2]=random_float_between(D_L05_00161B54*frame_time*0.7f,D_L05_00161B54*frame_time*1.5f);
    turn=random_float_between(-D_L05_00161B68,D_L05_00161B68)*0.017453292f;
    w[0]=FUN_001f9dc8(FUN_001fa580((*(float *)((char *)(data)+(0x194))),turn))*(D_L05_00161B58*frame_time);
    w[1]=FUN_001f9de0(FUN_001fa580((*(float *)((char *)(data)+(0x194))),turn))*(D_L05_00161B58*frame_time);
    w[2]=0.0f;
    w[2]=random_float_between(D_L05_00161B5C*frame_time*0.7f,D_L05_00161B5C*frame_time*1.5f);
    n1=func_001FA898_r(scale_particle_time(random_float_between(D_L05_00161B44,D_L05_00161B44+D_L05_00161B44)));
    n2=func_001FA898_r(scale_particle_time(random_float_between(D_L05_00161B48,D_L05_00161B48*1.2f)));
    n3=func_001FA898_r(scale_particle_time(random_float_between(D_L05_00161B4C,D_L05_00161B4C*1.2f)));
    particle=spawn_spark(pos,v,w,D_L05_00161B3C,D_L05_00161B40,n1,n2,n3,-1);
    } while (last>=0);
    qcopy(pos,position);
    pos[2]+=3.0f;
    if ((*(float *)((char *)(data)+(0x174)))<distance_to_2d1688(position,hero.motion.pos.f)) goto stop_attack;
    if (4.0f<=absolute_2d1688((*(float *)((char *)(moby)+(0x18)))-hero.motion.pos.f[2])) goto stop_attack;
    if ((*(int *)((char *)(data)+(0x190)))!=-1 && !point_in_path_xy(hero.motion.pos.f,D_L05_001B0930[(*(int *)((char *)(data)+(0x190)))]+16,*(int *)D_L05_001B0930[(*(int *)((char *)(data)+(0x190)))])) goto stop_attack;
    if (!collision_line_304320(pos,D_0013E633_3047E8,2,moby,0)) goto turn_attack;
stop_attack:
    if ((*(unsigned char *)((char *)(moby)+(0x53)))) blend_moby_animation(moby,0,0,3);
    (*(unsigned char *)((char *)(moby)+(0x20)))=1;
turn_attack:
    rotation0=data+0x194;
    rotation1=data+0x198;
    matrix1=data+0x140;
    matrix0=data+0x100;
    angle=FUN_001f9e90(hero.motion.pos.f[0]-(*(float *)((char *)(moby)+(0x10))),hero.motion.pos.f[1]-(*(float *)((char *)(moby)+(0x14))));
    smooth_angle_304320(angle,frame_time*1.5707964f,rotation0,0);
    smooth_angle_304320(0.08726646f,frame_time*1.5707964f,rotation1,0);
    rotate_axis_304320((*(float *)((char *)(data)+(0x198))),matrix1,1);
    angle=FUN_001fa5c8((*(float *)((char *)(data)+(0x194))),(*(float *)((char *)(moby)+(0x48))));
finish:
    rotate_axis_304320(angle,matrix0,2);
    return;
state_three:
        if (*(char **)(data+0x184)) mark_moby_2d1688(*(char **)(data+0x184));
        if (*(char **)(data+0x188)) mark_moby_2d1688(*(char **)(data+0x188));
        mark_moby_2d1688(moby);

}
#endif
extern char *find_collision_part_305178(void *, int, int) __asm__("FUN_L00_0025a420");
extern void *FUN_L05_002899f8(void *, void *, void *, int);
extern void hit_update_305178(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern int allocate_voice_305178(int, int, void *) __asm__("FUN_0022da68");
extern void stop_animation_305178(void *, int, int) __asm__("FUN_L00_00257470");
extern void spawn_effects_305178(void *, void *, void *, float, float, int, int, int,
                                 float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern float FUN_001f9dc8(float);
extern float FUN_001f9de0(float);
extern float FUN_001fa580(float, float);
extern int scale_frames_305178(int) __asm__("FUN_001f96f8");
extern void clear_vector_305178(void *) __asm__("FUN_001f99f8");
extern void spawn_particle_305178(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern void update_motion_305178(void *, void *) __asm__("FUN_L00_0025d458");
extern float FUN_001f9e90(float, float);
extern float FUN_001fa688(float, float);
extern int tick_305178(void *) __asm__("FUN_001f9740");
extern float vector_length_305178(void *) __asm__("FUN_001f9af0");
extern void finish_motion_305178(void *, void *) __asm__("FUN_L00_0025d538");
extern float D_0013F3D0[];

int D_L05_00161B74 = 5;
extern float D_L05_00161AF0 __attribute__((sda));
extern const float frame_time_const __asm__("D_0015ED6C");
extern float D_L05_00161AF4 __attribute__((sda));

void FUN_L05_00305178(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *first;
    char *second;
    char *part0;
    char *part1;
    char *part2;
    char *hit;
    char *owner;
    int result;
    float remaining = 0.0f;
    float position[4] __attribute__((aligned(16)));
    char effect_data[16] __attribute__((aligned(16)));
    float speed;
    float angle;
    struct Hero *base;

    part0 = find_collision_part_305178(moby, 0x210000, 0);
    first = *(char **)(data + 0x184);
    part1 = first ? find_collision_part_305178(first, 0x210000, 0) : 0;
    second = *(char **)(data + 0x188);
    part2 = second ? find_collision_part_305178(second, 0x210000, 0) : 0;
    part0 = FUN_L05_002899f8(part0, part1, part2, 0);
    if (part0 != 0) {
        if (*(char **)(part0 + 0x20) == moby)
            part0 = 0;
        owner = *(char **)(part0 + 0x20);
        if (owner != 0 && (*(short *)(owner + 0xa6) == 0x47 || *(short *)(owner + 0xa6) == 0))
            part0 = 0;
    }
    hit = part0;
    hit_update_305178(moby, hit, data + 0x20, 0, &result, &remaining, 0, 4);
    if (result == 1 || (unsigned char)moby[0x20] == 3) goto mark_children;
    {
        *(float *)(data + 0x20) -= remaining;
        if (*(float *)(data + 0x20) > 0.0f && remaining > 0.0f) {
            allocate_voice_305178(2, 0, moby);
            ((unsigned char *)data)[0x67] = 200;
            goto update_motion;
        } else {
            allocate_voice_305178(3, 0, moby);
            stop_animation_305178(moby, 0, -1);
            qcopy(position, moby + 0x10);
            position[2] += 3.0f;
            spawn_effects_305178(moby, data + 0x40, position, 0.0f, 0.0f, 0x14, 8, 0x14,
                                  4.0f, 2.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            qcopy(position, moby + 0x10);
            position[0] += 2.0f * FUN_001f9dc8(*(float *)(moby + 0x48));
            position[1] += 2.0f * FUN_001f9de0(*(float *)(moby + 0x48));
            spawn_effects_305178(moby, data + 0x40, position, 0.0f, 0.0f, 0x14, 8, 0x14,
                                  4.0f, 2.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            qcopy(position, moby + 0x10);
            angle = FUN_001fa580(*(float *)(moby + 0x48), 3.14159274f);
            position[0] += 2.0f * FUN_001f9dc8(angle);
            angle = FUN_001fa580(*(float *)(moby + 0x48), 3.14159274f);
            position[1] += 2.0f * FUN_001f9de0(angle);
            spawn_effects_305178(moby, data + 0x40, position, 0.0f, 0.0f, 0x14, 8, 0x14,
                                  4.0f, 2.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            *(short *)(data + 0x19e) = scale_frames_305178(D_L05_00161B74);
            moby[0x20] = 3;
            clear_vector_305178(effect_data);
            *(float *)(effect_data + 8) = *(float *)(data + 0x194);
            spawn_particle_305178(moby, 0x6dd, moby + 0x10, moby + 0x40, 0, 0, 0.0f,
                              D_L05_0015F580_macro, D_L05_0015F580_macro, D_L05_0015F580_macro);
            spawn_particle_305178(moby, 0x6df, moby + 0x10, moby + 0x40, 0, 0, 0.0f,
                              D_L05_0015F580_macro, D_L05_0015F580_macro, D_L05_0015F580_macro);
            spawn_particle_305178(moby, 0x6e0, moby + 0x10, moby + 0x40, 0, 0, 0.0f,
                              D_L05_0015F580_macro, D_L05_0015F580_macro, D_L05_0015F580_macro);
            spawn_particle_305178(moby, 0x6fe, moby + 0x10, moby + 0x40, 0, 0, 0.0f,
                              D_L05_0015F580_macro, D_L05_0015F580_macro, D_L05_0015F580_macro);
            spawn_particle_305178(moby, 0x6fe, moby + 0x10, moby + 0x40, 0, 0, 0.0f,
                              D_L05_0015F580_macro, D_L05_0015F580_macro, D_L05_0015F580_macro);
            spawn_particle_305178(moby, 0x6ff, moby + 0x10, moby + 0x40, 0, 0, 0.0f,
                              D_L05_0015F580_macro, D_L05_0015F580_macro, D_L05_0015F580_macro);
            spawn_particle_305178(moby, 0x6ff, moby + 0x10, moby + 0x40, 0, 0, 0.0f,
                              D_L05_0015F580_macro, D_L05_0015F580_macro, D_L05_0015F580_macro);
            spawn_particle_305178(moby, 0x6ff, moby + 0x10, moby + 0x40, 0, 0, 0.0f,
                              D_L05_0015F580_macro, D_L05_0015F580_macro, D_L05_0015F580_macro);
            return;
        }
    }
update_motion:
    update_motion_305178(moby, data + 0x60);
mark_children:
    ((unsigned char *)moby)[0xa4] = 0xff;
    if (*(char **)(data + 0x184) != 0)
        ((unsigned char *)*(char **)(data + 0x184))[0xa4] = 0xff;
    if (*(char **)(data + 0x188) != 0)
        ((unsigned char *)*(char **)(data + 0x188))[0xa4] = 0xff;
    if (*(int *)(data + 0x38) != 0 && *(int *)(data + 0x178) == 0) {
        *(int *)(data + 0x178) = scale_frames_305178(0xf0);
        *(int *)(data + 0x38) = 0;
        base = &hero;
        angle = FUN_001f9e90(base->motion.pos.f[0] - *(float *)(moby + 0x10),
                              base->motion.pos.f[1] - *(float *)(moby + 0x14));
        if (FUN_001fa688(*(float *)(moby + 0x48), angle) > 1.5707964f)
            ((signed char *)data)[0xc4] *= -1;
    }
    if (tick_305178(data + 0x178) != 0)
        *(float *)(data + 0x174) = 20.0f;
    else
        *(float *)(data + 0x174) = 14.0f;
    if (*(char **)(data + 0x184) != 0) {
        char *first;
        float offset_scale;
        speed = vector_length_305178(data + 0x40);
        speed /= D_L05_00161AF0 * frame_time_const;
        offset_scale = D_L05_00161AF4;
        *(float *)(*(char **)(data + 0x184) + 0x58) = speed;
        *(float *)(*(char **)(data + 0x184) + 0x58) +=
            *(float *)(data + 0x84) * offset_scale;
        first = *(char **)(data + 0x184);
        qcopy(first + 0x40, moby + 0x40);
        qcopy(first + 0x10, moby + 0x10);
    }
    if (*(char **)(data + 0x188) != 0) {
        char *second;
        float offset_scale;
        speed = vector_length_305178(data + 0x40);
        speed /= D_L05_00161AF0 * frame_time_const;
        offset_scale = D_L05_00161AF4;
        *(float *)(*(char **)(data + 0x188) + 0x58) = speed;
        *(float *)(*(char **)(data + 0x188) + 0x58) -=
            *(float *)(data + 0x84) * offset_scale;
        second = *(char **)(data + 0x188);
        qcopy(second + 0x40, moby + 0x40);
        qcopy(second + 0x10, moby + 0x10);
    }
    finish_motion_305178(moby, data + 0x60);
}
/* Drops stale entries from the moby's 8 slots, then pushes each remaining one away from the hero. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00306CF0), where it is exact; names translated to the US level program. */

typedef struct {
    float v[4];
    char *owner;
    int x14;
    unsigned char x18;
    unsigned char x19;
    short type;
    float x1C;
    int flags;
} Hit_306CF0;

extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern void subtract_vector_xyz(void *dst, void *a, void *b);

void FUN_L05_00305898(char *m) {
    char **slots = (char **)(*(char **)(m + 0x78) + 0x1A0);
    struct Hero *base;
    Hit_306CF0 h;
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        char *e = slots[i];
        if (e != 0) {
            if (*(unsigned char *)e != 2 || *(short *)(e + 0x32) == 0) {
                slots[i] = 0;
            }
        }
    }
    base = &hero;
    if (base->items[2].item_id == 6)
        return;
    h.flags |= 1;
    h.owner = m;
    h.x14 = 1;
    h.x1C = 1.0f;
    subtract_vector_xyz(h.v, &base->motion.pos, m + 0x10);
    h.v[2] = 0.0f;
    set_vector_length_xy(h.v, h.v, 1.5f);
    h.v[2] = 1.0f;
    h.v[3] = 5627.9248f;
    h.x19 = 1;
    h.type = *(short *)(m + 0xA6);
    h.x18 = 0;
    for (j = 0; j < 8; j++) {
        if (slots[j] != 0) {
            collect_mobies_in_sphere(slots[j] + 0x10, 0, m, &h, 0.75f);
        }
    }
}

extern float FUN_001f9e90(float, float);
extern float FUN_001f9b80(void *, void *);
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int FUN_L01_0028b510(void *, void *, float);
extern int FUN_L00_0025d7a0(char *, int, int, int);
extern char *D_L05_001B0930[];
extern int random_integer_below_e10(int) __asm__("FUN_00213260");
extern int trunc_f2i_e10(float) __asm__("FUN_001fa6d0");
void FUN_L05_00306d30(char *moby, int idx, float *out);

typedef struct {
    int count;                  /* 0x00: number of nodes */
    char pad04[0xC];
    float points[1][4];         /* 0x10: position; w marks the node */
} L16PatrolPath;

typedef struct {
    char pad0[0x200];
    float position[4];          /* 0x200 */
    char pad210[0x10];
    int base_path;              /* 0x220: index into D_L05_001B0930 */
    int branch_path[7];         /* 0x224: indexed by a node's mark - 1 */
    char pad240[4];
    L16PatrolPath *path;        /* 0x244: the path being followed */
    char pad248[0x10];
    short node;                 /* 0x258: the node being steered for */
} L05PatrolData;

typedef struct {
    char pad0[0x10];
    float position[4];          /* 0x10 */
    char pad20[0x28];
    float yaw;                  /* 0x48 */
    char pad4C[0x2C];
    L05PatrolData *data;        /* 0x78 */
} L16PatrolMoby;

/* Patrol step: advances the moby's path node until the next node is ahead and far enough, switching between the base path and its branches. */
/* Same routine as FUN_L16_002cf9f8, with this level's data layout. */
void FUN_L05_00306e10(char *obj) {
    L16PatrolMoby *moby = (L16PatrolMoby *)obj;
    L05PatrolData *d = moby->data;
    float target[4];
    float heading, distance;

    FUN_L05_00306d30((char *)moby, d->node, target);
    heading = FUN_001f9e90(target[0] - moby->position[0], target[1] - moby->position[1]);
    for (;;) {
        L16PatrolPath *path;
        L16PatrolPath *base;

        FUN_L05_00306d30((char *)moby, d->node, target);
        distance = FUN_001f9b80(target, d->position);
        if ((fast_difference_between_rotations(heading, moby->yaw) < 1.5707964f || distance > 4.0f) && distance > 2.0f) {
            break;
        }
        path = d->path;
        d->node = (d->node + 1) % path->count;
        base = (L16PatrolPath *)D_L05_001B0930[d->base_path];
        if (path != base) {
            /* At the last node of a branch: back to the base path, five nodes past its nearest one. */
            if (d->node == path->count - 1) {
                d->path = base;
                d->node = FUN_L01_0028b510(d->position, base, 0.0f);
                d->node = FUN_L00_0025d7a0((char *)d->path, d->node, 5, 1);
            }
        } else if (path->points[d->node][3] > 0.0f && random_integer_below_e10(100) > 0) {
            /* At a marked node of the base path: more often than not, onto the branch the mark names. */
            int branch = trunc_f2i_e10(d->path->points[d->node][3]) - 1;

            d->path = (L16PatrolPath *)D_L05_001B0930[d->branch_path[branch]];
            d->node = 0;
        }
        /* The same two statements as before the loop, written out again: the compiler merges the copies. */
        FUN_L05_00306d30((char *)moby, d->node, target);
        heading = FUN_001f9e90(target[0] - moby->position[0], target[1] - moby->position[1]);
    }
}

/* Same routine as FUN_L16_002d0058 without the pitch step: times the leap animation to the
 * remaining jump, then copies the animation speed to the rider. */
struct LeapVars {
    u8 pad0[0x208];
    f32 height;           /* 0x208 */
    u8 pad20C[0xC];
    f32 targetHeight;     /* 0x218 */
    u8 pad21C[0x30];
    f32 verticalSpeed;    /* 0x24C */
    u8 pad250[0x18];
    struct Moby *rider;   /* 0x268 */
    s32 timer;            /* 0x26C */
    u8 pad270[2];
    s16 sequence;         /* 0x272 */
};

extern int FUN_001f96f8(int);
extern int FUN_001fa6d0(float);
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L00_002595a0(float *, float *, float, float, float);
extern float FUN_0020c9e0(struct Moby *);
extern void leap_anim_307570(struct Moby *, int, int, int) __asm__("FUN_L05_00307480");

void FUN_L05_00307570(struct Moby *m) {
    float root0, root1;
    struct LeapVars *d = (struct LeapVars *)m->pvars;

    if (d->timer != 0) {
        float duration;
        float acceleration;
        if (m->prev_seq != 5 && FUN_001f96f8(3) >= d->timer) {
            leap_anim_307570(m, 5, 1, FUN_001f96f8(5));
            d->sequence = -1;
        }
        acceleration = frame_time_sq * 21.0f * -0.5f;
        duration = 30.0f;
        if (FUN_L00_002595a0(&root0, &root1, acceleration, d->verticalSpeed - acceleration,
                             d->height - d->targetHeight) > 0 &&
            root0 > 0.0f) {
            duration = FUN_001fa6d0(root0);
        }
        if (FUN_001f96f8(7) < d->timer && FUN_001f96f8(11) > d->timer && m->prev_seq == 5 &&
            (float)FUN_001f96f8(27) < duration) {
            short sequence;
            int chosen;
            chosen = random_int_between(0, 3) + 1;
            d->sequence = chosen;
            sequence = chosen;
            leap_anim_307570(m, sequence, 1, FUN_001f96f8(7));
        }
        if (d->sequence != -1) {
            if (m->seq == m->prev_seq) {
                float frame = FUN_0020c9e0(m);
                if (frame > 7.0f && frame < 20.0f && duration != 0.0f) {
                    float speed = 30.0f / duration;
                    if (speed > 1.7f)
                        speed = 1.7f;
                    if (speed < 0.3f)
                        speed = 0.3f;
                    m->anim_speed = speed;
                }
            }
            if (d->sequence != -1) {
                if (duration < (float)FUN_001f96f8(9)) {
                    leap_anim_307570(m, 5, 3, FUN_001f96f8(7));
                    d->sequence = -1;
                } else if (m->anim_flags & 2) {
                    leap_anim_307570(m, 5, 2, FUN_001f96f8(7));
                    d->sequence = -1;
                }
            }
        }
        if (m->seq == m->prev_seq && m->seq == 5 && duration != 0.0f) {
            float remaining;
            remaining = 25.0f;
            remaining -= FUN_0020c9e0(m);
            remaining /= duration * 0.5f;
            m->anim_speed = remaining;
        }
    } else {
        m->anim_speed = 1.0f;
        if ((m->anim_flags & 2) && m->prev_seq != 0) {
            leap_anim_307570(m, 0, 0, FUN_001f96f8(8));
        }
    }
    if (d->rider != 0)
        d->rider->anim_speed = m->anim_speed;
}
#include "sda.h"

/* Update function of moby class 717 (skid_mcmarx): a hoverboard racer that follows its path, jumps, and steers. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00308D68), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) Vec4_308D68;

typedef struct {
    int count;
    int pad[3];
    Vec4_308D68 points[1];
} Path_308D68;

typedef struct {
    char p00[0x64];
    float f64;
    float f68;
    char p6C[0x14];
} Manip_308D68;

typedef struct {
    Manip_308D68 m[4];
    float pos[4];
    float f210[4];
    int f220[5];
    int path_id;
    float cruise_speed;
    float launch_speed;
    float speed;
    Path_308D68 *path;
    float yaw_speed;
    float vertical_speed;
    int racer_index;
    int f254;
    short point_index;
    short launch_timer;
    int f25C;
    int f260;
    int f264;
    Moby *child;
    int jump_frames;
    short f270;
    short f272;
    float bob_phase;
    int f278;
    int jump_point;
} Data_308D68;

typedef struct {
    char p00[0x50];
    int f50;
} ChildData_308D68;

char *FUN_L05_002f8718_c(char *owner) __asm__("FUN_L05_002f8718");
extern char D_L05_001671C0[];
extern f32 approach_value(f32 *, f32, f32);
extern f32 fast_add_rotations_c3(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_c3(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c5(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians_c3(void) __asm__("func_00213308");
extern float frame_scale_sq_macro __asm__("D_0015ED64") __attribute__((section(".sdata")));
extern float D_0015ED6C_c __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern float FUN_L00_0025bc98(void *, float, void *, int, float, float, float);
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int D_0015EE38_macro __asm__("D_0015EE38") __attribute__((section(".sdata")));
extern int D_L05_00178300[];
extern int FUN_001f9770(void *);
extern int FUN_L00_0025e3b8(int);
extern unsigned char D_0013D388[];
extern void FUN_L00_001f2868(void *, int, void *, void *, float);
extern void FUN_L00_00259bc8(void *, int, int, void *, void *, float);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_00263b38(void *, float);
extern void FUN_L05_00306d30_c(char *moby, int idx, float *out) __asm__("FUN_L05_00306d30");
extern void FUN_L05_00307010(char *moby);
float absolute_float(float input) __asm__("func_001F99C0");
void FUN_L05_00306e10_c(char *obj) __asm__("FUN_L05_00306e10");
void FUN_L05_00307358_c(char *moby) __asm__("FUN_L05_00307358");
void FUN_L05_00307570_c(struct Moby *m) __asm__("FUN_L05_00307570");
extern char D_001413DC[];
extern char D_0013F350[];
extern int func_L00_001F2BE8_308D68(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");

void FUN_L05_00307910(Moby *moby) {
    float path_point[4];
    float step[4];
    float probe[4];
    float push[4];
    Data_308D68 *data = (Data_308D68 *)*(void **)&moby->pvars;

    if (moby->visible != 0 && distance_xyz(((float *)&moby->pos), D_L05_001671C0) < 29.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x17;
    }
    switch (moby->state) {
    case 0: {
        Path_308D68 *path;

        moby->update_dist = 0x18;
        moby->flags |= 0x41;
        path = (Path_308D68 *)D_L05_001B0930[data->path_id];
        data->path = path;
        FUN_L05_00306d30_c((char *)moby, 0, path_point);
        qcopy(((float *)&moby->pos), path_point);
        qcopy(data->pos, ((float *)&moby->pos));
        moby->rot.x = 0.0f;
        moby->rot.y = 0.0f;
        moby->rot.z = FUN_001f9e90(path->points[1].x - path->points[0].x, path->points[1].y - path->points[0].y);
        data->cruise_speed = D_0015ED6C_c * 17.0f + (float)data->racer_index * (D_0015ED6C_c * 1.55f);
        data->launch_speed = D_0015ED6C_c * 24.0f + (float)data->racer_index * (D_0015ED6C_c * 1.55f);
        if (D_0013D388[0] == 0) {
            float handicap = (float)(D_0015EE38_macro - 4);

            if (handicap < 0.0f) {
                handicap = 0.0f;
            }
            handicap = 1.0f - handicap * 0.01f;
            if (handicap < 0.77f) {
                handicap = 0.77f;
            }
            data->cruise_speed *= handicap;
            data->launch_speed *= handicap;
        }
        data->bob_phase = random_angle_radians_c3();
        moby->state = 1;
        break;
    }
    case 1:
        if (*(int *)(((char *)&D_001413DC)) == 0x16) {
            moby->update_dist = 0xFF;
            moby->flags &= ~0x41;
            data->point_index = 0;
            data->launch_timer = scale_game_frames_alt(0xB4);
            data->vertical_speed = 0.0f;
            data->yaw_speed = 0.0f;
            data->jump_frames = 0;
            data->f264 = 0;
            data->speed = 0.0f;
            if (data->child == 0) {
                Moby *child = (Moby *)FUN_L05_002f8718_c((char *)moby);

                data->child = child;
                if (child != 0) {
                    ((ChildData_308D68 *)*(void **)&child->pvars)->f50 = 1;
                }
            }
            if (data->racer_index % 2 == 1) {
                moby->flags |= 0x8000;
                if (data->child != 0) {
                    data->child->flags |= 0x8000;
                }
            }
            moby->state = 2;
        }
        break;
    case 2: {
        Hero *hero = (Hero *)(((char *)&D_0013F350));

        if (hero->state.control_mode != 0x16) {
            moby->state = 0;
        } else {
            float ground;
            float diff;
            float max_turn;
            float turn;
            int hit_count;
            int j;

            FUN_L05_00306e10_c((char *)moby);
            FUN_L05_00307010((char *)moby);
            FUN_L05_00307358_c((char *)moby);
            if (data->jump_frames != 0) {
                data->jump_frames++;
                if (data->jump_frames < scale_game_frames_alt(0x11)) {
                    float path_length = 0.0f;
                    float air_time;
                    float jump_speed;
                    int i;
                    int next;

                    for (i = data->point_index; i < data->jump_point; i = next) {
                        next = FUN_L00_0025d7a0((char *)data->path, i, 1, 1);
                        path_length += FUN_001f9b80((float (*)[4])data->path + (i + 1), (float (*)[4])data->path + (next + 1));
                    }
                    air_time = path_length / data->speed;
                    jump_speed = (data->path->points[data->jump_point].z - data->path->points[data->point_index].z) / air_time + frame_time_sq_macro * 21.0f * air_time * 0.5f;
                    if (data->vertical_speed < 0.0f) {
                        data->vertical_speed = 0.0f;
                    }
                    approach_value(&data->vertical_speed, jump_speed, frame_time_sq_macro * 110.0f);
                }
            }
            FUN_L05_00307570_c((unsigned char *)moby);
            FUN_L05_00306d30_c((char *)moby, data->point_index, path_point);
            FUN_L00_0025bc98(&moby->rot.z, FUN_001f9e90(path_point[0] - moby->pos.x, path_point[1] - moby->pos.y), &data->yaw_speed, 0,
                              frame_scale_sq_macro * 0.035f, frame_scale_sq_macro * 0.3f, D_0015ED6C_c * 6.981317f);
            if (data->pos[2] < path_point[2] - 5.0f) {
                data->pos[2] = path_point[2];
            }
            step[0] = fast_cos_c3(moby->rot.z) * data->speed;
            step[1] = fast_sin_c5(moby->rot.z) * data->speed;
            step[2] = 0.0f;
            add_vector_xyz(data->pos, data->pos, step);
            scale_vector_xyz(probe, step, 4.0f);
            add_vector_xyz(probe, probe, data->pos);
            hit_count = func_L00_001F2BE8_308D68(probe, 0, moby, 0, 1.0f);
            if (hit_count != 0) {
                scale_vector_xyz(push, step, 7.0f);
                for (j = 0; j < hit_count; j++) {
                    if (FUN_L00_0025e3b8(D_L05_00178300[j]) != 0) {
                        FUN_L00_00259bc8((void *)D_L05_00178300[j], (int)moby, 0x30000, data->pos, push, 1.0f);
                    }
                }
            }
            ground = probe_ground_height(data->pos, 0, 0.5f);
            diff = data->pos[2] - ground;
            if (diff > 0.0f || data->vertical_speed > 0.0f) {
                data->vertical_speed -= frame_time_sq_macro * 21.0f;
            } else if (absolute_float(diff) < 0.5f) {
                data->vertical_speed = 0.0f;
                data->pos[2] = ground;
                data->jump_frames = 0;
            }
            data->pos[2] += data->vertical_speed;
            if (FUN_001f9770(&data->launch_timer) != 0) {
                float target = data->cruise_speed;
                Hero *hero;

                if (ground < data->pos[2]) {
                    target = D_0015ED6C_c * 19.0f;
                }
                hero = (Hero *)(((char *)&D_0013F350));
                if (hero->unk894 < scale_game_frames_alt(0x2BC)) {
                    target += D_0015ED6C_c * 3.0f;
                }
                if (hero->unk7C0[data->racer_index] < hero->unk8C0) {
                    target -= D_0015ED6C_c * 1.5f;
                } else {
                    target += D_0015ED6C_c;
                }
                if (target < data->speed) {
                    approach_value(&data->speed, target, frame_time_sq_macro * 7.0f);
                } else {
                    approach_value(&data->speed, target, frame_time_sq_macro * 15.0f);
                }
            } else {
                approach_value(&data->speed, data->launch_speed, frame_time_sq_macro * 20.0f);
            }
            if (diff < 0.2f && data->vertical_speed < 0.0f) {
                data->pos[2] = ground;
                data->jump_frames = 0;
            }
            FUN_L00_00263b38(&data->m[3], 3.0f);
            max_turn = D_0015ED6C_c * 2.6179938f;
            turn = data->yaw_speed;
            if (max_turn < turn) {
                turn = max_turn;
            } else {
                max_turn = -max_turn;
                if (turn < max_turn) {
                    turn = max_turn;
                }
            }
            if (moby->flags & 0x8000) {
                turn = -turn;
            }
            data->m[3].f68 = turn * 30.0f;
            data->m[0].f68 = turn * 20.0f;
            data->m[2].f68 = turn * 50.0f;
            data->m[1].f64 = turn * 40.0f;
            FUN_L00_002628d8((char *)moby, (char *)&data->m[3], 3, frame_scale_sq_macro * 0.02f, frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby, (char *)data, 0, frame_scale_sq_macro * 0.02f, frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby, (char *)&data->m[2], 1, frame_scale_sq_macro * 0.02f, frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby, (char *)&data->m[1], 2, frame_scale_sq_macro * 0.015f, frame_scale_sq_macro * 0.3f);
            qcopy(((float *)&moby->pos), data->pos);
            moby->pos.z += 0.25f;
            data->bob_phase = fast_add_rotations_c3(data->bob_phase, D_0015ED6C_c * 6.108652f);
            moby->pos.z += fast_sin_c5(data->bob_phase) * 0.08f;
            if (data->child != 0) {
                Moby *child;

                *(long *)&data->child->spawn_frame = *(long *)&moby->spawn_frame;
                child = data->child;
                qcopy(((float *)&child->pos), ((float *)&moby->pos));
                qcopy(((float *)&child->rot), ((float *)&moby->rot));
                if (data->launch_timer != 0) {
                    child->command = 1;
                }
            }
        }
        break;
    }
    }
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_0030D230), where it is exact; names translated to the US level program. */

extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int fast_dec_timer(int *arg0) __asm__("FUN_001f9740");
extern void FUN_L00_00260738(char *, void *, void *, void *);
extern void allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

extern short D_L05_00161CD4 __attribute__((sda));
extern short D_L05_00161CD8 __attribute__((sda));
extern short D_L05_00161CDC __attribute__((sda));
extern short D_L05_00161CE0 __attribute__((sda));
extern short D_L05_00161CE4 __attribute__((sda));
extern short D_L05_00161CE8 __attribute__((sda));
void FUN_L05_0030bdd8(unsigned char *moby) {
    char *data = *(char **)(moby + 0x78);
    float v[4];
    float w[4];
    float d[4];
    char *p;
    char *q;
    float *r;
    float k;
    float t;
    switch (moby[0x20]) {
    case 0:
        *(int *)(data + 0x78) = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(
            (float)*(int *)&D_L05_00161CE4, (float)*(int *)&D_L05_00161CE8)));
        *(float *)(data + 0x60) = random_angle_radians();
        *(float *)(data + 0x6C) = random_angle_radians();
        moby[0x20] = 1;
        break;
    case 1:
        if (fast_dec_timer((int *)(data + 0x78))) {
            allocate_voice_for_target_entry_alt(0, 0, (int)moby);
            *(int *)(data + 0x78) = func_001FA898_r(multiply_global_factor_ed64(random_float_between_alt(
                (float)*(int *)&D_L05_00161CE4, (float)*(int *)&D_L05_00161CE8)));
        }
        p = (char *)moby + 0x10;
        qcopy(v, p);
        r = w;
        q = (char *)moby + 0x40;
        qcopy(r, q);
        t = fast_add_rotations(*(float *)(data + 0x60),
                               *(float *)&D_L05_00161CD4 * DEG_TO_RAD * frame_time);
        k = *(float *)&D_L05_00161CD8 * DEG_TO_RAD * frame_time;
        *(float *)(data + 0x60) = t;
        *(float *)(data + 0x6C) = fast_add_rotations(*(float *)(data + 0x6C), k);
        *(float *)(moby + 0x44) =
            *(float *)&D_L05_00161CDC * DEG_TO_RAD * fast_sin(*(float *)(data + 0x60));
        *(float *)(moby + 0x40) =
            *(float *)&D_L05_00161CE0 * DEG_TO_RAD * fast_sin(*(float *)(data + 0x6C));
        subtract_vector_xyz(d, p, v);
        FUN_L00_00260738(data + 0x20, d, r, q);
        break;
    }
}
extern char *D_L05_0015FFD8_c __asm__("D_L05_0015FFD8");
extern void FUN_L00_00260668(char *, char *, char *, char *, char *, char *);
extern void FUN_L00_002604f0(char *, char *, char *, char *, char *, char *);
extern void FUN_001f9a28(float *, char *, float *);
extern void FUN_L00_00260738_c(char *, float *, float *, char *) __asm__("FUN_L00_00260738");

void FUN_L05_0030bf98(char *moby)
{
    char *d = *(char **)(moby + 0x78);
    float p[4];
    float q[4];
    float r[4];
    switch (*(u8 *)(moby + 0x20)) {
    case 0:
        *(int *)(d + 0x5C) = 1;
        if (*(int *)(d + 0x80) >= 0) {
            FUN_L00_00260668(moby, D_L05_0015FFD8_c + (*(int *)(d + 0x80) << 8), moby + 0x10, moby + 0x40, d + 0x60, d + 0x70);
            *(u8 *)(moby + 0x20) = 1;
        }
        break;
    case 1:
        qcopy(p, moby + 0x10);
        qcopy(q, moby + 0x40);
        FUN_L00_002604f0(moby, D_L05_0015FFD8_c + (*(int *)(d + 0x80) << 8), d + 0x60, d + 0x70, moby + 0x10, moby + 0x40);
        FUN_001f9a28(r, moby + 0x10, p);
        FUN_L00_00260738_c(d + 0x20, r, q, moby + 0x40);
        break;
    }
}
extern float frame_time_30ca80 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float frame_time_sq_30ca80 __asm__("D_0015ED70");
extern void enqueue_callback_list_1_30ca80(void (*)(void), void *) __asm__("FUN_001f4600");
extern int scale_game_frames_30ca80(int) __asm__("FUN_001f96f8");
extern float distance_xyz_30ca80(void *, void *) __asm__("FUN_001f9b48");
extern float vector_distance_30ca80(void *, void *) __asm__("FUN_001f9b80");
extern float fast_cos_30ca80(float) __asm__("FUN_001f9dc8");
extern float fast_sin_30ca80(float) __asm__("FUN_001f9de0");
extern float angle_atan2_30ca80(float, float) __asm__("FUN_001f9e90");
extern int truncate_float_to_s32_30ca80(float) __asm__("FUN_001fa6d0");
extern void mark_moby_for_removal_30ca80(void *) __asm__("FUN_0020c828");
extern int random_integer_below_30ca80(int) __asm__("FUN_00213260");
extern float random_float_between_30ca80(float, float) __asm__("FUN_002132a8");
typedef struct {
    float x;
    float y;
    float z;
    char pad0C[0x12];
    unsigned short h1E;
    char rest[0x1170];
} WaterRecord_30ca80;
typedef struct {
    float raised_z;
    float lowered_z;
    int starts_raised;
    int rises;
    int f10;
    int record_index;
    int timer;
    int f1C;
    float speed;
    int moving;
} WaterVars_30ca80;
typedef struct {
    float f0;
    float f4;
    float f8;
    float fC;
    float f10;
    float f14;
    float f18;
    float f1C;
    float f20;
    int f24;
    int f28;
    int f2C;
    float dir_x;
    float dir_y;
    float dir_z;
    unsigned char b3C;
    unsigned char b3D;
} Light_30ca80;
typedef struct {
    char pad0[0x10];
    float f10;
    float f14;
} Cam_30ca80;

extern unsigned char mission_states_30ca80[] __asm__("D_0014C050");
extern WaterRecord_30ca80 water_records_30ca80[] __asm__("D_L05_001D6880");
extern void *D_L05_001612D0_30ca80 __asm__("D_L05_001612D0") __attribute__((section(".sdata")));
extern void initialize_slots_30ca80(void *, int) __asm__("FUN_L01_002b7a48");
extern void initialize_slot_data_30ca80(float) __asm__("FUN_L01_002b7ef0");
extern void copy_slot_30ca80(float *, int *) __asm__("FUN_L01_002b7c68");
extern void FUN_L05_0030d6a0_30ca80(int) __asm__("FUN_L05_0030d6a0");
extern void set_group_state_30ca80(int, int) __asm__("FUN_L01_0026e0e0");
extern float advance_accelerated_scalar_30ca80(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void update_group_30ca80(char *) __asm__("FUN_L05_0030d5c0");
extern void pool_splash_30ca80(void *, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern int FUN_L01_00275690_30ca80(char *, float) __asm__("FUN_L01_00275690");
extern void finish_slots_30ca80(char *, int) __asm__("FUN_L01_002b7fe0");
extern void FUN_L05_0030d678_30ca80(void) __asm__("FUN_L05_0030d678");

extern Cam_30ca80 D_L05_00180340;
extern Light_30ca80 light __asm__("D_L05_001CAD00");
extern int D_L05_0020B440[][4];
extern unsigned short record_draw_values[] __asm__("D_L05_0020B380");
extern int rise_frames[] __asm__("D_L05_0020B3E0");
extern float rise_speeds[] __asm__("D_L05_0020B3F8");
extern int base_rise_frames[] __asm__("D_L05_0020B410");
extern float base_rise_speeds[] __asm__("D_L05_0020B428");
extern void *D_L05_001612D4 __attribute__((section(".sdata")));
extern unsigned char D_L05_00161204[] __attribute__((section(".sdata")));
extern unsigned char D_L05_00161205[] __attribute__((section(".sdata")));
extern unsigned char D_L05_00161206[] __attribute__((section(".sdata")));
extern int D_L05_00161208 __attribute__((section(".sdata")));
extern float D_L05_0016120C __attribute__((section(".sdata")));
extern float D_L05_00161210 __attribute__((section(".sdata")));
extern float D_L05_00161214 __attribute__((section(".sdata")));
int D_L05_00161D10 = -1;
extern unsigned short D_L05_00161D14 __attribute__((sda));
int hero_hits __asm__("D_L05_00161D1C") = 0;
int rise_step __asm__("D_L05_00161D20") = 0;
extern float rise_checkpoint __asm__("D_L05_00161D24") __attribute__((sda));
float water __asm__("D_L05_00161D28") = 0;
int rise_checkpoint_hit __asm__("D_L05_00161D2C") = 0;
extern float move_speed __asm__("D_L05_00161D30") __attribute__((sda));
extern float move_accel __asm__("D_L05_00161D34") __attribute__((sda));
extern s32 D_L05_001612D8 __attribute__((section(".sdata")));
/* Update of the level 05 water mobys: sets up the 48-record water chain, moves the surface by state and keeps its record in step. */
void FUN_L05_0030ca80(struct Moby *moby) {
    WaterVars_30ca80 *d = *(WaterVars_30ca80 **)&moby->pvars;

    switch (moby->state) {
    case 0:
        if (d->record_index == 0) {
            int i;
            int j;
            float a;

            water = moby->pos.z;
            rise_step = 0;
            rise_checkpoint_hit = 0;
            initialize_slots_30ca80(water_records_30ca80, 0x30);
            initialize_slot_data_30ca80(1.0f);
            light.f0 = 16.0f;
            light.f4 = 16.0f;
            light.f8 = -8.0f;
            light.fC = -8.0f;
            light.f10 = 1.0f;
            light.f14 = 1.0f;
            light.f18 = 0.9f;
            light.f20 = 0.1f;
            a = angle_atan2_30ca80(D_L05_00180340.f10, D_L05_00180340.f14);
            light.dir_x = fast_cos_30ca80(a) * 0.57735002f;
            light.dir_y = fast_sin_30ca80(a) * 0.57735002f;
            light.dir_z = -0.57735002f;
            light.f28 = 0x29;
            light.f2C = 0x2A;
            light.b3C = 0x1C;
            light.b3D = 0x40;
            D_L05_001612D0_30ca80 = water_records_30ca80;
            D_L05_001612D4 = D_L05_0020B440;
            D_L05_001612D8 = 0x30;
            D_L05_00161204[0] = 0;
            D_L05_00161205[0] = 0xB;
            D_L05_00161206[0] = 0x3E;
            D_L05_00161208 = 0;
            D_L05_0016120C = 20480.0f;
            D_L05_00161210 = 255.0f;
            D_L05_00161214 = 26.0f;
            for (i = 0; i < 0x30; i++) {
                copy_slot_30ca80(&water_records_30ca80[i].x, D_L05_0020B440[i]);
                water_records_30ca80[d->record_index].h1E = record_draw_values[d->record_index];
            }
            for (j = 0; j < 0x30; j++) {
                FUN_L05_0030d6a0_30ca80(j);
            }
        }
        moby->draw_dist = 0;
        moby->flags |= 1;
        if (d->rises != 0) {
            if (d->f10 >= 0) {
                moby->update_dist = 0xFF;
                if (hero_hits >= 2) {
                    int k;
                    float s = (float)(hero_hits - 1) * 0.014285714f + 1.0f;
                    if (s > 1.4f) {
                        s = 1.4f;
                    }
                    for (k = 0; k < 6; k++) {
                        rise_frames[k] = truncate_float_to_s32_30ca80((float)base_rise_frames[k] * s);
                        rise_speeds[k] = base_rise_speeds[k] / s;
                    }
                }
            }
            if (mission_states_30ca80[moby->mission + (current_level_index << 4)] == 0xFF) {
                moby->state = 7;
                moby->pos.z = 44.0f;
            } else {
                moby->state = 3;
                d->timer = scale_game_frames_30ca80(0x3C);
                if (moby->pos.z < d->lowered_z) {
                    water_records_30ca80[d->record_index].h1E = 0;
                }
            }
        } else {
            if (d->f10 < 0) {
                float z = moby->pos.z;
                d->lowered_z = z;
                d->raised_z = z + 4.5f;
                if (d->starts_raised != 0) {
                    moby->state = 1;
                    moby->pos.z = d->raised_z;
                } else {
                    moby->state = 2;
                    moby->pos.z = d->lowered_z;
                }
            } else {
                moby->state = 2;
                moby->pos.z = d->lowered_z;
            }
        }
        if (d->record_index == 0x2F) {
            *(int *)&moby->collision = 0;
        }
        break;
    case 2:
        if (moby->command & 4) {
            d->moving = 1;
            set_group_state_30ca80(moby->group, 1);
        } else if (d->f10 >= 0) {
            if (mission_states_30ca80[moby->mission + (current_level_index << 4)] == 0xFF) {
                moby->state = 1;
                moby->pos.z = d->raised_z;
            }
        }
        if (d->moving != 0) {
            advance_accelerated_scalar_30ca80(&moby->pos.z, &d->speed, d->lowered_z, move_speed * frame_time_sq_30ca80, move_speed * frame_time_sq_30ca80, move_accel * frame_time_30ca80);
            update_group_30ca80((char *)moby);
        }
        moby->command = 1;
        if (random_integer_below_30ca80(0xC8) == 0) {
            float x = water_records_30ca80[d->record_index].x + random_float_between_30ca80(-4.0f, 4.0f);
            float r = random_float_between_30ca80(-4.0f, 4.0f);
            WaterRecord_30ca80 *e = &water_records_30ca80[d->record_index];
            pool_splash_30ca80(e, 1, 1, x, e->y + r, 1.0f, -0.05f);
        }
        break;
    case 1:
        if (moby->command & 8) {
            d->moving = 1;
            set_group_state_30ca80(moby->group, 2);
        } else {
            moby->command = 2;
        }
        if (d->moving != 0) {
            advance_accelerated_scalar_30ca80(&moby->pos.z, &d->speed, d->raised_z, move_speed * frame_time_sq_30ca80, move_speed * frame_time_sq_30ca80, move_accel * frame_time_30ca80);
            update_group_30ca80((char *)moby);
        }
        if (random_integer_below_30ca80(0xC8) == 0) {
            float x = water_records_30ca80[d->record_index].x + random_float_between_30ca80(-4.0f, 4.0f);
            float r = random_float_between_30ca80(-4.0f, 4.0f);
            WaterRecord_30ca80 *e = &water_records_30ca80[d->record_index];
            pool_splash_30ca80(e, 1, 1, x, e->y + r, 1.0f, -0.05f);
        }
        break;
    case 3:
        if (vector_distance_30ca80(((float *)&moby->pos), hero.motion.pos.f) < 16.0f) {
            Hero *h = &hero;
            if (h->motion.pos.f[2] < moby->pos.z) {
                if (h->state.control_mode == 0x11) {
                    if (h->water_surface_z < moby->pos.z) {
                        h->water_surface_z = moby->pos.z;
                    }
                }
            }
        }
        if (d->f10 >= 0) {
            if (vector_distance_30ca80(((float *)&moby->pos), hero.motion.pos.f) < 10.0f) {
                Hero *h = &hero;
                if (h->ground_moby != 0 && h->air_frames.s == 0 && h->ground_moby->oclass == 0x33E) {
                    d->f1C = 0;
                    moby->state = 8;
                    rise_step = 0;
                    d->timer = scale_game_frames_30ca80(rise_frames[0]);
                }
            }
        }
        if (random_integer_below_30ca80(0x32) == 0) {
            float x = water_records_30ca80[d->record_index].x + random_float_between_30ca80(-4.0f, 4.0f);
            float r = random_float_between_30ca80(-4.0f, 4.0f);
            WaterRecord_30ca80 *e = &water_records_30ca80[d->record_index];
            pool_splash_30ca80(e, 1, 1, x, e->y + r, 1.0f, -0.01f);
        }
        break;
    case 7:
        if (d->f10 >= 0) {
            Hero *h = &hero;
            if (h->state.current == 0x6A && h->state_timer == 2) {
                hero_hits++;
            }
        } else if (d->raised_z < moby->pos.z) {
            water_records_30ca80[d->record_index].h1E = 0;
            mark_moby_for_removal_30ca80(moby);
            return;
        }
        break;
    case 8:
        if (func_001F9908_v(&d->timer) != 0) {
            if (rise_step < 5) {
                rise_step++;
                d->timer = scale_game_frames_30ca80(rise_frames[rise_step]);
            }
        }
        moby->pos.z += rise_speeds[rise_step] * frame_time_30ca80;
        update_group_30ca80((char *)moby);
        water = moby->pos.z;
        {
            Vec4 v;

            v.q = 0;
            v.f[0] = 211.0f;
            v.f[1] = 338.0f;
            v.f[2] = 25.0f;
            if (distance_xyz_30ca80(v.f, &hero.motion.pos) < 6.0f) {
                if (rise_checkpoint_hit == 0) {
                    rise_checkpoint_hit = 1;
                    if (moby->pos.z > rise_checkpoint) {
                        rise_step = 5;
                    }
                }
            }
        }
        if (d->f10 >= 0) {
            Hero *h = &hero;
            if (h->state.current == 0x6A && h->state_timer == 2) {
                hero_hits++;
            }
        }
        if (moby->pos.z >= 44.0f) {
            set_group_state_30ca80(moby->group, 7);
        }
        break;
    }
    if (d->record_index >= 0) {
        WaterRecord_30ca80 *e = &water_records_30ca80[d->record_index];
        float t;

        e->z = moby->pos.z;
        t = 0.0f;
        if (FUN_L01_00275690_30ca80((char *)moby, 48.0f) != -1
            && (!(moby->state >= 3 && moby->state <= 7) || (moby->pos.z > d->lowered_z && moby->pos.z < d->raised_z))) {
            t = 1.0f;
        }
        if (t == 0.0f) {
            water_records_30ca80[d->record_index].h1E = 0;
        } else {
            water_records_30ca80[d->record_index].h1E = record_draw_values[d->record_index];
        }
        if (d->record_index == 0) {
            finish_slots_30ca80((char *)water_records_30ca80, 0x30);
            enqueue_callback_list_1_30ca80(FUN_L05_0030d678_30ca80, moby);
        }
        if (d->record_index == D_L05_00161D10) {
            water_records_30ca80[d->record_index].h1E = D_L05_00161D14;
            record_draw_values[d->record_index] = D_L05_00161D14;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_0030EA88), where it is exact; names translated to the US level program. */

extern char D_L05_001D6880[];
extern int D_L05_0015FFD8;
extern short *D_L05_001ABCC0[];

void FUN_L05_0030d5c0(char *moby) {
    char *data = *(char **)(moby + 0x78);
    short *p = D_L05_001ABCC0[*(unsigned char *)(moby + 0x21)];
    char(*base)[256] = (char(*)[256])D_L05_0015FFD8;
    do {
        int idx = *p & 0x7FFF;
        char *m = (char *)(idx * 256 + (int)base);
        if (*(short *)(m + 0xA6) == 0x33F) {
            char *d2 = *(char **)(m + 0x78);
            char *q;
            float f;
            if (*(int *)(data + 0xC) != 0 || *(float *)(d2 + 4) == *(float *)(data + 4)) {
                f = *(float *)(moby + 0x18);
            } else {
                f = *(float *)(d2 + 4) + (*(float *)(moby + 0x18) - *(float *)(data + 4));
            }
            *(float *)(m + 0x18) = f;
            q = D_L05_001D6880 + *(int *)(d2 + 0x14) * 0x1190;
            *(float *)(q + 8) = *(float *)(base[idx] + 0x18);
        }
    } while (*p++ >= 0);
}

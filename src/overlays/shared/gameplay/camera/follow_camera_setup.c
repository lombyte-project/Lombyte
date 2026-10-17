/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/hero.h"
#include "asm.h"
#include "eetypes.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/overlay/quad.h"
#include "rnc/globals.h"
extern int FUN_L00_001f0d60(float, void *, int, void *);

/* Resets the follow camera to its defaults: springs the orbit and target
   values back toward the defaults table, then clears the pending inputs. */
extern float D_L00_0015EF44_macro __asm__("D_L00_0015EF44") __attribute__((section(".sdata")));
extern unsigned char *D_L00_00168FC0_2e5e38 __asm__("D_L00_00168FC0") __attribute__((section(".data")));
extern int D_L00_0015EF60_macro __asm__("D_L00_0015EF60") __attribute__((section(".sdata")));
extern float D_L00_00161CA0_gp __asm__("D_L00_00161CA0") __attribute__((sda));
extern float D_L00_00161CDC_gp __asm__("D_L00_00161CDC") __attribute__((sda));
extern float D_L00_00161CE0_gp __asm__("D_L00_00161CE0") __attribute__((sda));
extern float D_L00_00161CE4_gp __asm__("D_L00_00161CE4") __attribute__((sda));
extern float D_L00_00161D1C_gp __asm__("D_L00_00161D1C") __attribute__((sda));
typedef struct { char pad0[0x2C]; float unk2C; char pad30[0xC]; short unk3C; short unk3E; int unk40; char pad44[8]; int unk4C; char pad50[8]; float unk58; } CamOrbit_2e5e38;
typedef struct { char pad0[0xB4]; int unkB4; char padB8[0x1E]; unsigned char unkD6; char padD7[3]; short unkDA; float unkDC; float unkE0; float unkE4; float unkE8; } CamTarget_2e5e38;
typedef struct { char pad0[0x10]; short unk10; char pad12[0x10]; short unk22; } Cam_2e5e38;
typedef struct { char pad0[0x3C]; float unk3C; float unk40; float unk44; int unk48; } CamSpring_2e5e38;
typedef struct { char pad0[0x6]; short unk6; char pad8[4]; float unkC; } CamEnd_2e5e38;
float cam_interp_values(void *, float, float, float, float, float) __asm__("FUN_001ebd78");

void follow_camera_approach(unsigned char *moby) __asm__("FUN_L00_002e5e38");
extern float D_L00_00161D04 __attribute__((sda));
extern float D_L00_00161D08 __attribute__((sda));
void follow_camera_approach(unsigned char *moby)
{
    unsigned char *init, *cam, *defaults, *orbit, *target, *default_target, *default_orbit, *reset;
    CamSpring_2e5e38 *spring_reset;
    init = *(unsigned char **)(moby + 0x70) + 0x1A8;
    *(float *)(init + 0x14) = D_L00_0015EF44_macro;
    *(int *)(init + 0x1C) = 0;
    *(int *)(init + 0x20) = 0;
    *(int *)(init + 0x10) = 0;
    *(int *)(init + 0x24) = 0;
    cam = *(unsigned char **)(moby + 0x70);
    defaults = D_L00_00168FC0_2e5e38;
    orbit = cam + 0x130;
    target = cam + 0x40;
    default_orbit = defaults + 0x130;
    default_target = defaults + 0x40;
    if (*(short *)(orbit + 0x3C)) {
        *(float *)(orbit + 0x2C) = cam_interp_values(cam + 0x174, *(float *)(orbit + 0x2C), *(float *)(orbit + 0x40), *(float *)(orbit + 0x48), D_L00_00161D04, *(float *)(init + 0x1C));
    } else {
        *(float *)(orbit + 0x2C) = cam_interp_values(cam + 0x174, *(float *)(orbit + 0x2C), *(float *)(default_orbit + 0x2C), *(float *)(orbit + 0x48), D_L00_00161D04, *(float *)(init + 0x1C));
    }
    if (*(short *)(orbit + 0x3E)) {
        *(float *)(orbit + 0x30) = cam_interp_values(orbit + 0x50, *(float *)(orbit + 0x30), *(float *)(orbit + 0x4C), *(float *)(orbit + 0x54), D_L00_00161D08, 0.0f);
    } else {
        *(float *)(orbit + 0x30) = cam_interp_values(orbit + 0x50, *(float *)(orbit + 0x30), *(float *)(default_orbit + 0x30), *(float *)(orbit + 0x54), D_L00_00161D08, 0.0f);
    }
    if (*(short *)(target + 0xDA)) {
        *(float *)(target + 0xB0) = cam_interp_values(target + 0xBC, *(float *)(target + 0xB0), *(float *)(target + 0xB4), *(float *)(target + 0xB8), 0.2f, 0.0f);
    } else {
        *(float *)(target + 0xB0) = cam_interp_values(target + 0xBC, *(float *)(target + 0xB0), *(float *)(default_target + 0xB0), *(float *)(target + 0xB8), 0.2f, 0.0f);
    }
    ((CamOrbit_2e5e38 *)orbit)->unk3C = 0;
    ((CamOrbit_2e5e38 *)orbit)->unk3E = 0;
    ((CamOrbit_2e5e38 *)orbit)->unk40 = 0;
    ((CamOrbit_2e5e38 *)orbit)->unk4C = 0;
    ((CamOrbit_2e5e38 *)orbit)->unk58 = ((CamOrbit_2e5e38 *)default_orbit)->unk2C;
    ((CamTarget_2e5e38 *)target)->unkDA = 0;
    ((CamTarget_2e5e38 *)target)->unkB4 = 0;
    (*(Cam_2e5e38 **)(moby + 0x70))->unk10 = 1;
    (*(Cam_2e5e38 **)(moby + 0x70))->unk22 = 0;
    ((CamTarget_2e5e38 *)target)->unkDC = D_L00_00161CA0_gp;
    ((CamTarget_2e5e38 *)target)->unkE4 = D_L00_00161CA0_gp;
    ((CamTarget_2e5e38 *)target)->unkE0 = 0.2f;
    ((CamTarget_2e5e38 *)target)->unkE8 = 0.2f;
    ((CamTarget_2e5e38 *)target)->unkD6 = 0;
    spring_reset = (CamSpring_2e5e38 *)(*(unsigned char **)(moby + 0x70) + 0x1D0);
    spring_reset->unk44 = D_L00_00161CDC_gp;
    spring_reset->unk3C = D_L00_00161CE4_gp;
    spring_reset->unk40 = D_L00_00161CE0_gp;
    spring_reset->unk48 = D_L00_0015EF60_macro;
    reset = *(unsigned char **)(moby + 0x70) + 0x220;
    ((CamEnd_2e5e38 *)reset)->unkC = D_L00_00161D1C_gp;
    ((CamEnd_2e5e38 *)reset)->unk6 = 0;
}
typedef struct { u8 pad[0xB0]; f32 fB0; } S40_2e6000;
typedef struct { u8 pad[0x3C]; f32 f3C; u8 pad2[4]; f32 f44; } S1D0_2e6000;
typedef struct { s32 i0; u8 pad[0xC]; s32 i10; } S220_2e6000;
typedef struct {
    u8 pad0[0x12];
    s16 h12;
    s16 h14;
    s16 h16;
    u8 pad1[0xF0 - 0x18];
    f32 fF0;
} M_2e6000;
typedef struct { u8 pad[0x70]; M_2e6000 *m; } O_2e6000;
typedef struct {
    u8 pad0[0x10];
    void (*fn)(void);
    u8 pad1[0x25C - 0x14];
    u8 *p25C;
    s32 i260;
    u8 pad2[0x380 - 0x264];
} A_2e6000;
typedef struct { u8 pad[0x100]; f32 f100; f32 f104; } C_2e6000;
typedef struct { u8 pad[0x2000]; s32 i2000; } L_2e6000;
extern A_2e6000 player_camera[] __asm__("D_L00_00166C80");
extern C_2e6000 D_0013C940_2e6000 __asm__("D_0013C940");
extern L_2e6000 D_0013F3D0_2e6000 __asm__("D_0013F3D0");
extern s32 D_L00_00177F00[];
extern s32 D_0013CAE0[];
extern f32 D_L00_00166E10[];
extern f32 D_L00_0015EF40;
extern s32 D_L00_00161D94 __attribute__((sda));
extern f32 D_L00_00161D84 __attribute__((sda));
extern f32 D_L00_00161D88 __attribute__((sda));
extern f32 D_L00_00161D98 __attribute__((sda));
extern f32 D_L00_00161D9C __attribute__((sda));
extern f32 D_L00_00161DA0 __attribute__((sda));
extern f32 D_L00_00161DA4 __attribute__((sda));
extern f32 D_L00_00161DA8 __attribute__((sda));
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 FUN_001f99c0(f32);
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32 FUN_001fa6c0(s32);
extern s32 FUN_001f96f8(s32);
extern s32 fast_dec_timer_s16(s16 *) __asm__("FUN_001f9770");
extern u8 *FUN_002141f8(s32);
extern s32 collect_mobies_in_sphere(void *, s32, s32, s32, f32) __asm__("FUN_L00_001f2868");
extern void FUN_L00_002e8918(f32 *, f32, f32);
extern void FUN_L00_002e8970(void *, f32, f32);
extern void follow_camera_start_slide(void) __asm__("FUN_L00_002e8648");
extern void FUN_L00_002e8620(void);
extern void follow_camera_set_height(f32, f32) __asm__("FUN_L00_002e84b8");
extern void follow_camera_set_distance(s32, f32, f32) __asm__("FUN_L00_002e8450");
extern void follow_camera_set_offset_scale(s32, f32, f32) __asm__("FUN_L00_002e84f0");
extern void FUN_L00_002e8568(s32);
extern void FUN_L00_002e8590(f32, f32);

/* Per-frame follow camera adjustments driven by the hero's state: sway, zoom and lookat tweaks, then the shake timers in m->h12/h14/h16. */
void follow_camera_apply_overrides(O_2e6000 *o) __asm__("FUN_L00_002e6000");
void follow_camera_apply_overrides(O_2e6000 *o) {
    S220_2e6000 *s;
    S40_2e6000 *q;
    S1D0_2e6000 *t;
    M_2e6000 *m;
    A_2e6000 *a;
    A_2e6000 *x;
    f32 v[4];
    f32 f, sum;
    s32 flag = 0;
    s32 n, i;
    s16 k;
    s32 *lp;
    u8 *e;

    s = (S220_2e6000 *)((u8 *)o->m + 0x220);
    if (hero.unk2284 == 0xD && s->i10 != 0x14D) {
        f = fast_add_rotations(hero.unk4E4, 3.14159265f);
        v[0] = fast_cos(f);
        v[1] = fast_sin(f);
        v[2] = 0.0f;
        FUN_L00_002e8918(v, 0.20943952f, v[2]);
        follow_camera_start_slide();
    }
    if (hero.state.control_mode == 0x11) {
        o->m->fF0 = 0.25f;
        follow_camera_set_height(0.5f, 0.003f);
    }
    if (hero.state.control_mode == 5) {
        flag = 1;
        {
            S40_2e6000 *q5 = (S40_2e6000 *)((u8 *)o->m + 0x40);
            if (hero.vehicle_mode == 1) {
                q5->fB0 = 0.25f;
                follow_camera_set_height(1.5f, 0.003f);
            } else {
                q5->fB0 = 0.25f;
                follow_camera_set_height(2.5f, 0.003f);
            }
        }
    }
    if (hero.state.control_mode == 0x10) o->m->fF0 = 0.25f;
    if (hero.vehicle_mode == 1) {
        if (s->i0 == 0) {
            q = (S40_2e6000 *)((u8 *)o->m + 0x40);
            follow_camera_set_distance(1, 3.0f, 0.003f);
            if (flag == 0) {
                q->fB0 = 1.0f;
                follow_camera_set_height(1.0f, 0.003f);
            }
        }
        *(f32 *)((u8 *)o->m + 0x20C) = 0.6f;
    }
    if (hero.vehicle_mode == 2) {
        if (s->i10 < 2) {
            follow_camera_set_offset_scale(0, D_L00_00161D98, 0.005f);
            follow_camera_set_distance(1, D_L00_00161D9C, 0.003f);
            follow_camera_set_height(D_L00_00161DA0, 0.003f);
        }
        FUN_L00_002e8620();
        t = (S1D0_2e6000 *)((u8 *)o->m + 0x1D0);
        t->f3C = D_L00_00161DA8;
        t->f44 = D_L00_00161DA4;
    }
    if (hero.unk20AA != 0 && hero.state.control_mode != 0xF) {
        FUN_L00_002e8568(0);
        follow_camera_start_slide();
        FUN_L00_002e8590(0.04f, 0.2f);
    }
    x = &player_camera[1];
    if (x->fn != 0) x->fn();
    a = &player_camera[0];
    if (a->p25C != 0 && s->i10 < 2) {
        if ((s8)a->p25C[0x20] < 0) {
            a->p25C = 0;
        } else {
            if (0.3f <= FUN_001f99c0(D_0013C940_2e6000.f100) || 0.3f <= FUN_001f99c0(D_0013C940_2e6000.f104)) {
                a->i260 = 0;
            }
            x = &player_camera[0];
            x->i260++;
            if (FUN_001f96f8(400) < x->i260) x->i260 = FUN_001f96f8(400);
            f = FUN_001fa6c0(x->i260) / FUN_001fa6c0(FUN_001f96f8(400));
            FUN_L00_002e8970(x->p25C + 0x10, f * 0.20943952f, 0.0f);
        }
    }
    s->i10 = 0;
    n = collect_mobies_in_sphere(&D_0013F3D0_2e6000, 1, D_0013F3D0_2e6000.i2000, 0, 15.0f);
    if (n > 0) {
        lp = D_L00_00177F00;
        for (i = 0; i < n; lp++, i++) {
            e = FUN_002141f8(*lp);
            if (e != 0 && (f32)e[0xD] != 0.0f) {
                s->i10 = 1;
                break;
            }
        }
    }
    if (hero.state.current == 0x81 && (D_0013CAE0[0] & 3)) {
        FUN_L00_002e8568(0);
        follow_camera_start_slide();
        FUN_L00_002e8590(0.04f, 0.2f);
        { M_2e6000 *mm = o->m;
        mm->h16 = 0;
        mm->h12 = 0;
        mm->h14 = 0; }
        return;
    }
    if (D_L00_00161D94 == 0) return;
    m = o->m;
    sum = 0.0f;
    {
        /* `next` is a separate variable: with a plain j++ gcc reverses the loop or gives the counter another saved register. */
        s32 j = 0, next;
        f32 *q = v;
        do {
            next = j + 1;
            *q = fast_difference_between_rotations(D_L00_00166E10[j + 43], D_L00_00166E10[j + 44]);
            sum += *q;
            q++;
            j = next;
        } while (j < 4);
    }

    sum *= 0.25f;
    { f32 c = sum * 5.0f;
    if (1.0f < c) c = 1.0f;
    c = 1.0f - c;
    if (c < D_L00_00161D84) m->h12 = FUN_001f96f8(0x20); }
    if (m->h12 != 0) {
        if (fast_dec_timer_s16(&m->h12)) m->h14 = FUN_001f96f8(0x2D);
        follow_camera_start_slide();
        k = ++m->h16;
        if (FUN_001f96f8(0x1E) < k) m->h16 = FUN_001f96f8(0x1E);
        f = FUN_001fa6c0(m->h16) / FUN_001fa6c0(FUN_001f96f8(0x1E));
        FUN_L00_002e8590((D_L00_00161D88 - 0.015f) * f + 0.015f, 0.2f);
        D_L00_0015EF40 = 0.0f;
    } else if (m->h14 != 0) {
        fast_dec_timer_s16(&m->h14);
        f = FUN_001fa6c0(m->h14) / FUN_001fa6c0(FUN_001f96f8(0x2D));
        f *= -0.75f;
        f += 0.75f;
        D_L00_0015EF40 = f;
        m->h16 = 0;
    } else {
        m->h16 = 0;
        D_L00_0015EF40 = 0.75f;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E7B68), where it is exact; names translated to the US level program. */

typedef float V[4] __attribute__((aligned(16)));

extern V D_L00_00173E60;
extern char D_L00_00166C80[];
extern char D_L00_00173E70_c[] __asm__("D_L00_00173E70");
extern f32 vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_L00_00166E40[];
extern int FUN_001efa68(void *, void *, int, int, int);
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern f32 D_L00_00161CC8;
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern char D_L00_00173F60_a[] __asm__("D_L00_00173E60");

/* Updates the follow camera: when `snap` is set, re-places the eye behind the followed target (backing off from walls found by
   ray traces, nudging it sideways when directly above or below), then rebuilds the camera basis looking at the target. */
void FUN_L00_002e66b8(int snap) {
    char *cams = D_L00_00166C80;
    char *cam = *(char **)(cams + 0x180);
    char *track = cams + 0x190; /* the followed player: position at 0, up vector at 0x30 */
    char *data = *(char **)(cam + 0x70);
    char *orbit = data + 0x130; /* distance at 0x2C, height at 0x30 */
    char *target = data + 0x40; /* point looked at; eye height at 0xB0, moby at 0xC0 */
    float up[4], rise[4], look[4], eye[4];
    int mask;
    float *from, *to;
    if (snap) {
        float offset[4], part[4], basis[12], delta[4], start[4];
        float length;
        struct Hero *player = &hero;
        struct Hero *hero2, *hero3;
        part[0] = -*(float *)(orbit + 0x2C);
        part[1] = 0.0f;
        part[2] = *(float *)(orbit + 0x30);
        part[3] = 0.0f;
        mask = 0x94;
        copy_matrix3x4(basis, player->unk0);
        transform_vector_by_basis(offset, part, *(char **)(target + 0xC0) + 0xC0);
        add_vector_xyz(cam + 0x30, target, offset);
        scale_vector_xyz(rise, *(char **)(target + 0xC0) + 0xE0, *(float *)(target + 0xB0));
        add_vector_xyz(eye, target, rise);
        if (FUN_001efa68(eye, cam + 0x30, mask, (int)player->moby, 0)) {
            if (vector_distance_xy(D_L00_00173F60_a, eye) < 0.001f) mask = 0x96;
        }
        hero2 = &hero;
        if (FUN_001efa68(eye, cam + 0x30, mask, (int)hero2->moby, 0)) {
            subtract_vector_xyz(delta, (float *)D_L00_00173F60_a, eye);
            length = vector_length_xyz(delta);
            if (length == 0.0f) {
                qcopy(cam + 0x30, target);
            } else if (length < *(float *)&D_L00_00161CC8) {
                /* the wall is too close to the eye: lift the eye out of it and trace again */
                float radius = 0.5f;
                int i;
                scale_vector_xyz(rise, *(char **)(target + 0xC0) + 0xE0, radius);
                add_vector_xyz(eye, target, rise);
                qcopy(start, eye);
                for (i = 0; i < 3; i++) {
                    if (FUN_L00_001f0d60(radius, eye, 4, 0)) {
                        qcopy(eye, D_L00_00173E70_c);
                    } else if (i == 0) {
                        radius += 0.25f;
                    } else {
                        break;
                    }
                }
                subtract_vector_xyz(delta, eye, start);
                length = vector_length_xyz(delta);
                if (length == 0.0f) {
                    qcopy(cam + 0x30, target);
                } else {
                    scale_vector_xyz(delta, delta, *(float *)(orbit + 0x2C) / length);
                    add_vector_xyz(cam + 0x30, start, delta);
                    hero3 = &hero;
                    if (FUN_001efa68(start, cam + 0x30, mask, (int)hero3->moby, 0)) {
                        subtract_vector_xyz(delta, (float *)D_L00_00173F60_a, eye);
                        length = vector_length_xyz(delta);
                        if (length == 0.0f) {
                            qcopy(cam + 0x30, target);
                        } else {
                            scale_vector_xyz(delta, delta, (length - 0.5f) / length);
                            add_vector_xyz(cam + 0x30, eye, delta);
                        }
                    }
                }
            } else {
                scale_vector_xyz(delta, delta, (length - 0.5f) / length);
                add_vector_xyz(cam + 0x30, eye, delta);
            }
        }
        /* straight above or below the target: nudge the camera sideways */
        subtract_vector_xyz(offset, (float *)(cam + 0x30), (float *)target);
        length = dot_vectors_xyz(offset, track + 0x30);
        scale_vector_xyz(part, track + 0x30, length);
        subtract_vector_xyz(offset, offset, part);
        if (vector_length_xyz(offset) < 0.05f) {
            if (random_integer_below(2)) *(float *)(cam + 0x30) += 0.5f;
            else *(float *)(cam + 0x30) -= 0.5f;
            if (random_integer_below(2)) *(float *)(cam + 0x34) += 0.5f;
            else *(float *)(cam + 0x34) -= 0.5f;
            *(float *)(cam + 0x38) += 0.5f;
        }
    }
    from = (float *)(cam + 0x30);
    to = (float *)(cam + 0x64);
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
    qcopy(up, D_L00_00166E40);
    normalize_vector_xyz(rise, up, *(float *)(target + 0xB0));
    add_vector_xyz(eye, rise, target);
    subtract_vector_xyz(look, eye, (float *)(cam + 0x30));
    normalize_vector_xyz(cam, look, 1.0f);
    qcopy(cam + 0x20, up);
    cross_vectors_xyz(cam + 0x10, cam, cam + 0x20);
    normalize_vector_xyz(cam + 0x10, cam + 0x10, 1.0f);
    cross_vectors_xyz(cam + 0x20, cam + 0x10, cam);
    qcopy(cam + 0x40, cam);
}

extern float vdot_2e6bf8(void *, void *) __asm__("FUN_001f9ab0");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

/* Resets the camera rig state from the moby position when idle. */
void FUN_L00_002e6bf8(char *m) {
    float dn[4];
    float t[4];
    char *d;
    char *b;
    char *c;
    char *e;

    if (*(short *)(m + 0x86) == 0) {
        b = *(char **)(m + 0x70);
        d = b;
        c = b + 0x130;
        b += 0x40;
        e = b;
        scale_vector_xyz(dn, &hero.gravity_dir, -1.0f);
        qcopy(e, &hero.motion.pos);
        qcopy(d + 0xA0, b);
        scale_vector_xyz(d + 0x50, dn, vdot_2e6bf8(b, dn));
        clear_u64_value(d + 0xB0);
        clear_u64_value(d + 0xC0);
        clear_u64_value(d + 0xE0);
        scale_vector_xyz(t, dn, *(float *)(c + 0x30));
        add_vector_xyz(d + 0x90, t, e);
        scale_vector_xyz(t, dn, *(float *)(b + 0xB0));
        add_vector_xyz(d + 0xD0, t, e);
        qcopy(d + 0x80, d + 0xD0);
        subtract_vector_xyz(c, m + 0x30, d + 0x90);
        qcopy(d + 0x140, c);
        qcopy(d, m + 0x30);
        add_vector_xyz(d + 0x1F0, d + 0x90, c);
        *(int *)(d + 0x220) = 0;
    }
}

/* Ported from rac1-decomp src/overlays/shared/vendor_002E1660.c (func_L00_002E8210) */

/* Initialises the type-0 camera: fills its data blocks and seeds the first position. */

typedef struct CamE_2E8210 {
    short h0;                     /* 0x00 */
    short h2;                     /* 0x02 */
    float f4;                     /* 0x04 */
    float f8;                     /* 0x08 */
    int iC;                       /* 0x0C */
    int i10;                      /* 0x10 */
    int i14;                      /* 0x14 */
    int i18;                      /* 0x18 */
    int pad1C;
} CamE_2E8210;

typedef struct CamA_2E8210 {
    float pos[4];                 /* 0x00 */
    char pad10[0x40];
    float f50[4];                 /* 0x50 */
    char pad60[0x50];
    float fB0;                    /* 0xB0 */
    int iB4;                      /* 0xB4 */
    float fB8;                    /* 0xB8 */
    int iBC;                      /* 0xBC */
    int iC0;                      /* 0xC0 */
    unsigned char bC4;            /* 0xC4 */
    char padC5;
    short hC6;                    /* 0xC6 */
    char padC8[0xE];
    unsigned char bD6;            /* 0xD6 */
    char padD7;
    short hD8;                    /* 0xD8 */
    short hDA;                    /* 0xDA */
    float fDC;                    /* 0xDC */
    float fE0;                    /* 0xE0 */
    float fE4;                    /* 0xE4 */
    float fE8;                    /* 0xE8 */
    char padEC[4];
} CamA_2E8210;

typedef struct CamB_2E8210 {
    float v0[4];                  /* 0x00 */
    float v10[4];                 /* 0x10 */
    int i20;                      /* 0x20 */
    int i24;                      /* 0x24 */
    int i28;                      /* 0x28 */
    float f2C;                    /* 0x2C */
    float f30;                    /* 0x30 */
    int i34;                      /* 0x34 */
    int i38;                      /* 0x38 */
    short h3C;                    /* 0x3C */
    short h3E;                    /* 0x3E */
    char pad40[4];
    int i44;                      /* 0x44 */
    float f48;                    /* 0x48 */
    char pad4C[4];
    int i50;                      /* 0x50 */
    float f54;                    /* 0x54 */
    float f58;                    /* 0x58 */
    char pad5C[4];
} CamB_2E8210;

typedef struct Cam190_2E8210 {
    int i0;                       /* 0x00 */
    int i4;                       /* 0x04 */
    float f8;                     /* 0x08 */
    float fC;                     /* 0x0C */
    float f10;                    /* 0x10 */
    float f14;                    /* 0x14 */
} Cam190_2E8210;

typedef struct Cam1A8_2E8210 {
    int flags;                    /* 0x00 */
    int i4;                       /* 0x04 */
    int i8;                       /* 0x08 */
    int iC;                       /* 0x0C */
    int i10;                      /* 0x10 */
    float f14;                    /* 0x14 */
    float f18;                    /* 0x18 */
    int i1C;                      /* 0x1C */
    int i20;                      /* 0x20 */
    int i24;                      /* 0x24 */
} Cam1A8_2E8210;

typedef struct CamG_2E8210 {
    char pad0[0x10];
    float v10[4];                 /* 0x10 */
    float v20[4];                 /* 0x20 */
    float f30;                    /* 0x30 */
    short h34;                    /* 0x34 */
    short h36;                    /* 0x36 */
    short h38;                    /* 0x38 */
    short h3A;                    /* 0x3A */
    float f3C;                    /* 0x3C */
    float f40;                    /* 0x40 */
    float f44;                    /* 0x44 */
    int i48;                      /* 0x48 */
    int i4C;                      /* 0x4C */
} CamG_2E8210;

typedef struct Cam220_2E8210 {
    int i0;                       /* 0x00 */
    short h4;                     /* 0x04 */
    short h6;                     /* 0x06 */
    float f8;                     /* 0x08 */
    float fC;                     /* 0x0C */
    int i10;                      /* 0x10 */
    char pad14[0xC];
} Cam220_2E8210;

typedef struct Cam240_2E8210 {
    float v0[4];                  /* 0x00 */
    float v10[4];                 /* 0x10 */
    char pad20[4];
    int i24;                      /* 0x24 */
    short h28;                    /* 0x28 */
} Cam240_2E8210;

typedef struct Cam_2E8210 {
    float pos[4];                 /* 0x000 */
    short h10;                    /* 0x010 */
    short h12;                    /* 0x012 */
    short h14;                    /* 0x014 */
    short h16;                    /* 0x016 */
    char pad18[8];
    CamE_2E8210 e;                /* 0x020 */
    CamA_2E8210 a;                /* 0x040 */
    CamB_2E8210 b;                /* 0x130 */
    Cam190_2E8210 p190;           /* 0x190 */
    Cam1A8_2E8210 p1A8;           /* 0x1A8 */
    CamG_2E8210 g;                /* 0x1D0 */
    Cam220_2E8210 p220;           /* 0x220 */
    Cam240_2E8210 p240;           /* 0x240 */
} Cam_2E8210;

typedef struct Moby_2E8210 {
    float v0[4];                  /* 0x00 */
    char pad10[0x20];
    float v30[4];                 /* 0x30 */
    float v40[4];                 /* 0x40 */
    char pad50[0x20];
    Cam_2E8210 *cam;              /* 0x70 */
    char pad74[9];
    unsigned char b7D;            /* 0x7D */
    short h7E;                    /* 0x7E */
} Moby_2E8210;

typedef struct Hero_2E8210 {
    char pad0[0x30C];
    short h30C;                   /* 0x30C */
    char pad30E[0x1D72];
    int i2080;                    /* 0x2080 */
} Hero_2E8210;

typedef struct Ctl_2E8210 {
    char pad0[0x86];
    short h86;                    /* 0x86 */
} Ctl_2E8210;

extern int D_L00_00161D90 __attribute__((sda));
extern float D_L00_0015EF40; /* no foreign declaration */
extern float D_L00_0015EF44; /* no foreign declaration */
extern int D_L00_0015EF60;
extern float D_L00_00161CA0_gp __asm__("D_L00_00161CA0") __attribute__((sda));
extern float D_L00_00161CDC_gp __asm__("D_L00_00161CDC") __attribute__((sda));
extern float D_L00_00161CE0_gp __asm__("D_L00_00161CE0") __attribute__((sda));
extern float D_L00_00161CE4_gp __asm__("D_L00_00161CE4") __attribute__((sda));
extern float D_L00_00161D1C_gp __asm__("D_L00_00161D1C") __attribute__((sda));
extern int D_L00_00161D24_gp __asm__("D_L00_00161D24") __attribute__((sda));
extern int D_L00_00161D8C __attribute__((sda));
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
void follow_camera_reset(char *o) __asm__("FUN_L00_002e4430");
void FUN_L00_002e66b8_c(int snap) __asm__("FUN_L00_002e66b8");
void FUN_L00_002e6bf8_c(char *m) __asm__("FUN_L00_002e6bf8");
extern Hero_2E8210 D_0013F450_2E8210 __asm__("D_0013F350");
extern char D_0013F4D0_2E8210[] __asm__("D_0013F3D0");
extern char D_0013F6E0_2E8210[] __asm__("D_0013F5E0");
extern Ctl_2E8210 *D_L00_00166F04_2E8210 __asm__("D_L00_00166E04") __attribute__((section(".data")));
extern float dot_vectors_xyz_c(void *, void *) __asm__("FUN_001f9ab0");
extern float func_001F9CB8_2E8210(void *) __asm__("FUN_001f9af0");
extern void func_L00_002E80A8_2E8210(Moby_2E8210 *) __asm__("FUN_L00_002e6bf8");
extern void func_L00_002E58E0_2E8210(Moby_2E8210 *) __asm__("FUN_L00_002e4430");
extern void func_L00_002E7B68_2E8210(int) __asm__("FUN_L00_002e66b8");

void follow_camera_setup(Moby_2E8210 *m) __asm__("FUN_L00_002e6d60");
void follow_camera_setup(Moby_2E8210 *m) {
    CamA_2E8210 *a;
    CamB_2E8210 *b;
    Cam_2E8210 *c;
    Cam_2E8210 *e;
    CamE_2E8210 *f;
    Cam_2E8210 *d;
    CamG_2E8210 *g;
    char *q1;
    char *q2;
    char *q3;
    Cam240_2E8210 *p240;
    int r;
    float t;
    float v0[4];
    float v1[4];

    D_L00_00161D8C = 0;
    D_L00_00161D90 = 0;
    a = &m->cam->a;
    qcopy(a->pos, D_0013F4D0_2E8210);
    a->fB0 = 1.5f;
    a->iC0 = D_0013F450_2E8210.i2080;
    a->bC4 = 0;
    a->bD6 = 0;
    a->fE0 = 0.2f;
    a->fDC = D_L00_00161CA0_gp;
    a->fE4 = D_L00_00161CA0_gp;
    a->fE8 = 0.2f;
    a->fB8 = 0.005f;
    a->hC6 = 0;
    a->hD8 = 0;
    a->iB4 = 0;
    a->hDA = 0;
    a->iBC = 0;

    b = &m->cam->b;
    b->f2C = 4.64f;
    b->f48 = 0.003f;
    b->f54 = 0.003f;
    b->f30 = 2.0f;
    b->f58 = 4.64f;
    b->i34 = 0;
    b->i38 = 0;
    b->i28 = 0;
    b->i20 = 0;
    b->i24 = 0;
    b->h3C = 0;
    b->h3E = 0;
    b->i44 = 0;
    b->i50 = 0;

    c = m->cam;
    c->h10 = 1;
    c->h12 = 0;
    c->h14 = 0;
    c->h16 = 0;
    D_L00_0015EF40 = 0.75f;

    e = m->cam;
    e->e.h0 = 0;
    f = &e->e;
    f->i10 = 0;
    f->iC = 0;
    f->h2 = 0;
    f->i14 = 0;
    f->i18 = 0;

    q1 = (char *)m->cam;
    ((Cam_2E8210 *)q1)->p190.i0 = 0;
    q1 = (char *)&((Cam_2E8210 *)q1)->p190;
    ((Cam190_2E8210 *)q1)->f8 = 0.005f;
    ((Cam190_2E8210 *)q1)->fC = 0.2f;
    ((Cam190_2E8210 *)q1)->f10 = 0.15707964f;
    ((Cam190_2E8210 *)q1)->i4 = 0;
    ((Cam190_2E8210 *)q1)->f14 = 14.2857f;

    q2 = (char *)m->cam;
    ((Cam_2E8210 *)q2)->p1A8.flags = 0;
    q2 = (char *)&((Cam_2E8210 *)q2)->p1A8;
    ((Cam1A8_2E8210 *)q2)->f14 = D_L00_0015EF44;
    ((Cam1A8_2E8210 *)q2)->f18 = 0.5235988f;
    ((Cam1A8_2E8210 *)q2)->i8 = 0;
    ((Cam1A8_2E8210 *)q2)->i4 = 0;
    ((Cam1A8_2E8210 *)q2)->iC = 0;
    ((Cam1A8_2E8210 *)q2)->i10 = 0;
    ((Cam1A8_2E8210 *)q2)->i1C = 0;
    ((Cam1A8_2E8210 *)q2)->i20 = 0;
    ((Cam1A8_2E8210 *)q2)->i24 = 0;

    q3 = (char *)m->cam;
    ((Cam_2E8210 *)q3)->p220.i0 = 0;
    q3 = (char *)&((Cam_2E8210 *)q3)->p220;
    ((Cam220_2E8210 *)q3)->i10 = 0;
    ((Cam220_2E8210 *)q3)->h4 = 0;
    ((Cam220_2E8210 *)q3)->h6 = 0;
    ((Cam220_2E8210 *)q3)->f8 = b->f2C;
    ((Cam220_2E8210 *)q3)->fC = D_L00_00161D1C_gp;

    d = m->cam;
    r = scale_game_frames(0x7D0);
    g = &d->g;
    g->f30 = 0.0f;
    g->h3A = 0;
    g->h36 = 0;
    g->h38 = 0;
    g->i4C = 0;
    g->h34 = r;
    g->f44 = D_L00_00161CDC_gp;
    g->f3C = D_L00_00161CE4_gp;
    g->f40 = D_L00_00161CE0_gp;
    g->i48 = D_L00_0015EF60;
    clear_u64_value(d->g.v10);

    p240 = &m->cam->p240;
    p240->h28 = 0;
    p240->i24 = 0;
    qcopy(p240->v0, D_0013F4D0_2E8210);
    qcopy(p240->v10, D_0013F4D0_2E8210);

    if (m->b7D == 2) {
        func_L00_002E80A8_2E8210(m);
        qcopy(m->v40, m->v0);
        if (D_L00_00166F04_2E8210->h86 == 7 && D_0013F450_2E8210.h30C == 0) {
            e->e.h0 = 0;
        } else {
            f->h0 = scale_game_frames(D_L00_00161D24_gp);
        }
    } else {
        func_L00_002E58E0_2E8210(m);
        func_L00_002E7B68_2E8210(1);
    }
    subtract_vector_xyz(b->v0, m->v30, a->f50);
    qcopy(b->v10, b->v0);
    qcopy(c->pos, m->v30);
    qcopy(g->v20, m->v30);
    scale_vector_xyz(v0, D_0013F6E0_2E8210, dot_vectors_xyz_c(b->v0, D_0013F6E0_2E8210));
    subtract_vector_xyz(v1, b->v0, v0);
    t = b->f2C - func_001F9CB8_2E8210(v1);
    g->f30 = t;
    if (t < -0.1f) {
        g->h3A = 1;
    }
    f->f8 = b->f30;
    f->f4 = a->fB0;
    m->h7E = 0;
}
typedef struct {
    unsigned char p0[0x208C];
    int w208C;
    unsigned char p1[0x20A4 - 0x2090];
    unsigned char b20A4;
} G_2e7138;
extern float D_0013CA40_2e7138 __asm__("D_0013CA40") __attribute__((section(".data")));
extern float D_L00_00161CC8_gp __asm__("D_L00_00161CC8") __attribute__((sda));
float FUN_001f9af0_2e7138(void *) __asm__("FUN_001f9af0");
void FUN_L00_001ff500_2e7138(void *, void *, float) __asm__("FUN_L00_001ff500");
float absolute_float(float) __asm__("FUN_001f99c0");
short FUN_001f96f8_2e7138(int) __asm__("FUN_001f96f8");

extern float D_L00_00161CB8 __attribute__((sda));
int FUN_L00_002e7138(unsigned char *m, void *b, int c, int d) {
    unsigned char *q = *(unsigned char **)(m + 0x70);
    unsigned char *A = q + 0x130;
    unsigned char *B = q + 0x1D0;
    unsigned char *C = q + 0x40;
    float len, dist, k;
    G_2e7138 *G;
    len = FUN_001f9af0_2e7138(A);
    subtract_vector_xyz(A, b, q + 0x90);
    if (*(short *)(B + 0x38)) {
        FUN_L00_001ff500_2e7138(A, A, *(float *)(A + 0x2C) - *(float *)(B + 0x30));
        return d;
    }
    dist = FUN_001f9af0_2e7138(A);
    if (hero.vehicle_mode != 2 && absolute_float(D_0013CA40_2e7138) > 0.05f && d) {
        dist -= D_L00_00161CB8 * frame_scale;
        *(short *)(B + 0x34) = FUN_001f96f8_2e7138(0x884);
    }
    k = D_L00_00161CC8_gp;
    if (!c && hero.state.control_mode != 2 && len < dist) {
        scale_vector_xyz(A, A, len / dist);
        *(float *)(B + 0x30) = *(float *)(A + 0x2C) - len;
        return d;
    }
    if (dist < k) {
        if (dist <= 1e-4f)
            FUN_L00_001ff500_2e7138(A, *(unsigned char **)(C + 0xC0) + 0xC0, -k);
        else
            scale_vector_xyz(A, A, k / dist);
        dist = D_L00_00161CC8_gp;
    }
    *(float *)(B + 0x30) = *(float *)(A + 0x2C) - dist;
    return 1;
}
typedef unsigned int Q002e7318 __attribute__((mode(TI)));
typedef union V002e7318 {
    f32 f[4];
    Q002e7318 q;
} V002e7318;
typedef struct X002e7318 {
    u8 pad[0x20];
    V002e7318 m;
    u8 pad30[8];
    s16 s38;
    u8 pad3a[0xe];
    s32 i48;
} X002e7318;
typedef struct P002e7318 {
    u8 pad[0x20c];
    f32 a;
    f32 b;
    f32 c;
} P002e7318;
typedef struct O002e7318 {
    u8 pad[0x70];
    u8 *p70;
} O002e7318;
typedef struct G002e7318 {
    u8 pad[0x30];
    V002e7318 v40;
    u8 pad50[0x84];
    s32 i114;
} G002e7318;
extern G002e7318 D_L00_00166E10_c __asm__("D_L00_00166E10");
extern f32 follow_cam_min_distance_gp __asm__("D_L00_00161CD8") __attribute__((sda));
extern V002e7318 D_L00_00173E70;
extern s32 FUN_L00_002e4180_c(V002e7318 *, f32) __asm__("FUN_L00_002e4180");
extern s32 coll_sphere(V002e7318 *, f32, s32, s32) __asm__("FUN_L00_001f0d60");
extern s32 FUN_L00_002e7138_c(O002e7318 *, V002e7318 *, s32, s32) __asm__("FUN_L00_002e7138");
s32 FUN_L00_002e7318(O002e7318 *o) {
    V002e7318 pos;
    V002e7318 tmp;
    u8 *b = o->p70;
    X002e7318 *x = (X002e7318 *)(b + 0x1D0);
    u8 *b40 = b + 0x40;
    u8 *b130 = b + 0x130;
    P002e7318 *p;
    f32 lim;
    f32 d;
    s32 w;
    s32 i;
    s32 ha = 0;
    s32 hb = 0;
    s32 r;
    G002e7318 *g = &D_L00_00166E10_c;

    add_vector_xyz(&pos, b + 0x90, b130);
    x->s38 = 2;
    p = (P002e7318 *)((O002e7318 **)g)[-4]->p70;
    w = x->i48;
    lim = follow_cam_min_distance_gp + p->c * p->a * (p->b - 1.0f);
    i = 0;
    while (i < 3) {
        if (FUN_L00_002e4180_c(&pos, lim)) {
            ha = 1;
        } else {
            if (!coll_sphere(&pos, lim, w, g->i114))
                break;
            hb = 1;
        }
        if (x->s38 != 0) {
            subtract_vector_xyz(&tmp, &(*(V002e7318 *)&D_L00_00173E60), &pos);
            normalize_vector_xyz(&tmp, &tmp, 1.0f);
            d = dot_vectors_xyz(&g->v40, &tmp);
            if (d > 0.75f)
                x->s38 = 1;
            else if (d < -0.75f)
                x->s38 = -1;
            else
                x->s38 = 0;
        }
        qcopy(&pos, &D_L00_00173E70);
        i++;
    }
    r = FUN_L00_002e7138_c(o, &pos, ha, hb);
    add_vector_xyz(&x->m, b40 + 0x50, b130);
    if (x->s38 == 2)
        x->s38 = 0;
    return r;
}
/* Does nothing and returns 0 (the buffer is unused). */
int FUN_L00_002e7520(void) {
    char buf[0x50];
    return 0;
}
typedef struct {
    f32 v[4];
} V002e7530 __attribute__((aligned(16)));
typedef struct {
    u8 p[0x70];
    u8 *m;
} O002e7530;
void FUN_L00_002e7530(O002e7530 *o, V002e7530 *a, V002e7530 *b, V002e7530 *c, V002e7530 *out,
                      f32 k1, f32 k2) {
    V002e7530 t0, t1, t2, t3;
    u8 *m = o->m;
    subtract_vector_xyz(&t0, b, c);
    m += 0x90;
    normalize_vector_xyz(&t1, &t0, k1);
    add_vector_xyz(b, b, &t1);
    subtract_vector_xyz(&t2, b, m);
    normalize_vector_xyz(&t2, &t2, k2);
    add_vector_xyz(&t3, m, &t2);
    subtract_vector_xyz(a, &t3, out);
}
extern char *current_follow_cam __asm__("D_L00_00166E00") __attribute__((section(".data")));
extern int D_L00_0015EF5C __asm__("D_L00_0015EF5C");
extern struct { u8 pad[0x2080]; int value; } D_0013F350_2e7618 __asm__("D_0013F350") __attribute__((section(".data")));
extern void clear_u64_value_2e7618(void *) __asm__("FUN_001f99f8");
extern void add_vector_xyz_2e7618(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_2e7618(void *, void *, void *) __asm__("FUN_001f9a28");
extern float distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern float resolve_level_path_segment_point_2e7618(V002e7530 *, V002e7530 *, V002e7530 *, V002e7530 *, float) __asm__("FUN_L00_0025dcd8");
extern int coll_sphere_2e7618(void *, int, int, float) __asm__("FUN_L00_001f0d60");

int FUN_L00_002e7618(O002e7530 *o, V002e7530 *out, int step, float dist, float spd) {
    V002e7530 delta, diff, start, hit, near, pos, temp, next;
    V002e7530 *path;
    V002e7530 *orbit;
    u8 *range;
    u8 *m;
    u8 *g;
    float lim, measure, radius;
    int found;

    m = o->m;
    g = *(u8 **)(current_follow_cam + 0x70);
    orbit = (V002e7530 *)(m + 0x130);
    range = m + 0x1d0;
    found = 0;
    lim = follow_cam_min_distance_gp + *(float *)(g + 0x214) * *(float *)(g + 0x20c) * (*(float *)(g + 0x210) - 1.0f);
    clear_u64_value_2e7618(&delta);
    qcopy(&start, out);
    qcopy(&hit, &D_L00_00173E60);
    qcopy(&near, &D_L00_00173E70);
    path = (V002e7530 *)(m + 0x90);
    add_vector_xyz_2e7618(&pos, path, orbit);
    measure = resolve_level_path_segment_point_2e7618(&temp, &hit, path, &pos, 0.0f);
    radius = distance_xyz(path, &temp) /
              (*(float *)((u8 *)orbit + 0x2c) - *(float *)(range + 0x30));
    radius = (follow_cam_min_distance_gp + (lim - follow_cam_min_distance_gp) * radius) * 0.75f;
    if (measure < radius) {
        found = 1;
        FUN_L00_002e7530(o, &delta, &temp, &hit, &start, radius - measure, dist);
        qcopy(out, &near);
        if (coll_sphere_2e7618(out, D_L00_0015EF5C, D_0013F350_2e7618.value, spd)) {
            subtract_vector_xyz_2e7618(&diff, &start, out);
            add_vector_xyz_2e7618(&hit, &D_L00_00173E60, &diff);
            measure = resolve_level_path_segment_point_2e7618(&temp, &hit, path, &pos, 0.0f);
            radius = distance_xyz(path, &temp) /
                      (*(float *)((u8 *)orbit + 0x2c) - *(float *)(range + 0x30));
            radius = (follow_cam_min_distance_gp + (lim - follow_cam_min_distance_gp) * radius) * 0.75f;
            if (measure < radius) {
                FUN_L00_002e7530(o, &next, &temp, &hit, &start, radius - measure, dist);
                add_vector_xyz_2e7618(&delta, &delta, &next);
                qcopy(out, &D_L00_00173E70);
                if (coll_sphere_2e7618(out, D_L00_0015EF5C, D_0013F350_2e7618.value, spd)) {
                    subtract_vector_xyz_2e7618(&diff, &start, out);
                    add_vector_xyz_2e7618(&hit, &D_L00_00173E60, &diff);
                    measure = resolve_level_path_segment_point_2e7618(&temp, &hit, path, &pos, 0.0f);
                    radius = distance_xyz(path, &temp) /
                              (*(float *)((u8 *)orbit + 0x2c) - *(float *)(range + 0x30));
                    radius = (follow_cam_min_distance_gp + (lim - follow_cam_min_distance_gp) * radius) * 0.75f;
                    if (measure < radius) {
                        FUN_L00_002e7530(o, &next, &temp, &hit, &start, radius - measure, dist);
                        add_vector_xyz_2e7618(&delta, &delta, &next);
                    }
                }
            }
        }
    }
    add_vector_xyz_2e7618(out, &start, &delta);
    return found;
}
#define NOT_SDA

#define MACRO_ADDR

/* Only for arg1==1: true when arg2 is past the edge plane
   (D_L00_00173F60 - arg2) . (arg0->+0x70 + 0x130) < 0. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E8E78), where it is exact; names translated to the US level program. */

extern V D_L00_00173E60;
extern float FUN_001f9ab0(void *, void *);
extern void FUN_001f9a28(float *, float *, float *);

/* For arg1 1: true when (D_L00_00173E60 - arg2) points against the object's +0x130 vector. */
int FUN_L00_002e79c8(void *arg0, int arg1, float *arg2) {
    V diff;
    char *ptr2;

    if (arg1 == 1) {
        ptr2 = *(char **)((char *)arg0 + 0x70) + 0x130;
        FUN_001f9a28(diff, D_L00_00173E60, arg2);
        if (FUN_001f9ab0(diff, ptr2) < 0.0f) {
            return 1;
        }
    }
    return 0;
}
typedef float V_2e7a30[4] __attribute__((aligned(16)));
extern float follow_cam_min_distance_gp __asm__("D_L00_00161CD8") __attribute__((sda));
extern float D_L00_00161CC8_gp __asm__("D_L00_00161CC8") __attribute__((sda));
extern float D_0013CA40_2e7a30 __asm__("D_0013CA40") __attribute__((section(".data")));
extern int D_L00_0015EF5C __asm__("D_L00_0015EF5C");
extern struct Hero hero_data __asm__("D_0013F350") __attribute__((section(".data")));
extern V_2e7a30 D_L00_00173E60_2e7a30 __asm__("D_L00_00173E60") __attribute__((section(".data")));
extern V_2e7a30 D_L00_00166E40_2e7a30 __asm__("D_L00_00166E40") __attribute__((section(".data")));
extern void FUN_001f9bf8_2e7a30(void *, void *, float) __asm__("FUN_001f9bf8");
extern int FUN_L00_002e4290_2e7a30(void *) __asm__("FUN_L00_002e4290");
extern int FUN_L00_001f0d60_2e7a30(void *, int, int, float) __asm__("FUN_L00_001f0d60");
extern int FUN_L00_002e79c8_2e7a30(void *, int, void *, float) __asm__("FUN_L00_002e79c8");
extern int FUN_L00_002e7618_2e7a30(void *, void *, int, float, float) __asm__("FUN_L00_002e7618");
extern float FUN_001f9ab0_2e7a30(void *, void *) __asm__("FUN_001f9ab0");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");

int FUN_L00_002e7a30(unsigned char *a) {
    OvlQuad dir[1];
    V_2e7a30 pos;
    V_2e7a30 ndir;
    V_2e7a30 step;
    V_2e7a30 save;
    V_2e7a30 d;
    unsigned char *base;
    unsigned char *c;
    unsigned char *e;
    struct Hero *g;
    unsigned char *p;
    int i;
    int found;
    float spd;
    float dist;
    float f;

    found = 0;
    base = *(unsigned char **)(a + 0x70);
    p = base + 0x40;
    c = base + 0x130;
    e = base + 0x1D0;
    qcopy(dir, c);
    dist = 0.0f;
    qcopy(pos, base + 0x90);
    spd = follow_cam_min_distance_gp;
    FUN_001f9bf8_2e7a30(ndir, dir, 1.0f);
    for (i = 0; i < truncate_float_to_s32(*(float *)(e + 0x40) - 1.0f); i++) {
        if (i > 0 && FUN_L00_002e4290_2e7a30(pos) == 0) {
            g = &hero_data;
            if (FUN_L00_001f0d60_2e7a30(pos, D_L00_0015EF5C, ((int)g->moby), spd) &&
                FUN_L00_002e79c8_2e7a30(a, i, pos, spd) == 0) {
                qcopy(step, D_L00_00173E60_2e7a30);
                qcopy(save, pos);
                if (FUN_L00_002e7618_2e7a30(a, pos, i, dist, spd)) {
                    found = 1;
                    if (g->vehicle_mode != 2 && absolute_float(D_0013CA40_2e7a30) > 0.05f) {
                        subtract_vector_xyz(d, step, save);
                        FUN_001f9bf8_2e7a30(d, d, 1.0f);
                        if (absolute_float(FUN_001f9ab0_2e7a30(D_L00_00166E40_2e7a30, d)) <
                            0.75f) {
                            float lim;
                            float cf;
                            int n = scale_game_frames(0x884);
                            float t = D_L00_00161CB8 * frame_scale;
                            lim = D_L00_00161CC8_gp;
                            *(short *)(e + 0x34) = n;
                            *(float *)(e + 0x30) += t;
                            cf = *(float *)(c + 0x2C);
                            if (cf - *(float *)(e + 0x30) < lim) {
                                *(float *)(e + 0x30) = cf - lim;
                            }
                        }
                    }
                    subtract_vector_xyz(d, pos, p + 0x50);
                    FUN_001f9bf8_2e7a30(c, d, *(float *)(c + 0x2C) - *(float *)(e + 0x30));
                    FUN_001f9bf8_2e7a30(ndir, c, 1.0f);
                }
            }
        }
        {
            float tot = *(float *)(c + 0x2C);
            f = (tot - *(float *)(e + 0x30)) / tot;
        }
        spd += *(float *)(e + 0x44) * *(float *)(e + 0x3C);
        dist += spd;
        scale_vector_xyz(step, ndir, spd * f);
        add_vector_xyz(pos, pos, step);
    }
    return found;
}
typedef int Q_2e7d20 __attribute__((mode(TI)));
typedef union { Q_2e7d20 q; f32 f[4]; s32 i[4]; } V_2e7d20;
typedef struct {
    u8 pad0[0x18];
    u8 b18;
    u8 b19;
    u16 h1A;
    u8 pad1C[0x14];
} W_2e7d20;

typedef struct {
    u8 pad0[0x80];
    u8 pad80[0x2080 - 0x80];
    u8 *p2080;
    s32 i2084;
    u8 pad2088[0x20A4 - 0x2088];
    u8 b20A4;
} GS_2e7d20;
extern GS_2e7d20 D_0013F350_2e7d20  __asm__("D_0013F350")__attribute__((section(".data")));
extern u8 D_L00_00166E10_2e7d20[] __asm__("D_L00_00166E10") __attribute__((section(".data")));
extern u8 D_L00_00166DC0[] __attribute__((section(".data")));
extern u8 *D_L00_00177F00_2e7d20[] __asm__("D_L00_00177F00") __attribute__((section(".data")));
extern u8 *D_L00_001E75C0[] __attribute__((section(".data")));
extern f32 D_L00_00161CD0 __attribute__((sda));
typedef struct { u8 pad[0x100]; f32 x; f32 y; } C940_2e7d20_t;
extern C940_2e7d20_t D_0013C940_2e7d20  __asm__("D_0013C940")__attribute__((section(".data")));
typedef struct { u8 pad[0x18]; s32 p18; s32 p1C; u8 v20[0x10]; } E40_2e7d20_t;
extern int D_L00_00173E40_lui[] __asm__("D_L00_00173E40") __attribute__((section(".data")));

void lerp_vector(void *, void *, void *, f32) __asm__("FUN_001f9a40");
extern s32 probe_world_sphere(void *, s32, void *, s32, f32) __asm__("FUN_L00_001f2868");
s32 FUN_L00_00259830(void *);
extern void FUN_001f9a28_2e7d20(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 FUN_001f9ab0_2e7d20(void *, void *) __asm__("FUN_001f9ab0");
extern void FUN_001f9a68_2e7d20(void *, void *, f32) __asm__("FUN_001f9a68");
extern void FUN_001f9bf8_2e7d20(void *, void *, f32) __asm__("FUN_001f9bf8");
f32 fast_arc_sin(f32) __asm__("FUN_001f9df8");
extern f32 FUN_001f99c0_2e7d20(f32) __asm__("FUN_001f99c0");
void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
void build_look_at_matrix(void *, void *, void *, f32) __asm__("FUN_00214890");
extern s32 FUN_L00_002e7a30_2e7d20(void *) __asm__("FUN_L00_002e7a30");
extern s32 FUN_L00_002e7520_2e7d20(void *) __asm__("FUN_L00_002e7520");
extern s32 FUN_L00_002e7318_2e7d20(void *) __asm__("FUN_L00_002e7318");
extern s32 FUN_001f96f8_2e7d20(s32) __asm__("FUN_001f96f8");
extern s32 fast_dec_timer(void *) __asm__("FUN_001f9770");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
f32 cosine_interpolate(f32, f32, f32) __asm__("FUN_002133d0");
extern void FUN_001f9a10_2e7d20(void *, void *, void *) __asm__("FUN_001f9a10");
extern s32 collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern s32 FUN_L00_002e4290_2e7d20(void *) __asm__("FUN_L00_002e4290");
s32 FUN_L00_0025e3b8(s32);
void make_hit_descriptor(void *, void *, s32, void *, f32) __asm__("FUN_L00_00259888");
void FUN_L00_00259a88(s32, void *);
void init_camera_system(void) __asm__("FUN_L00_001ed280");

#define AF_2e7d20(o) (*(f32 *)(a + (o)))
#define BF_2e7d20(o) (*(f32 *)(b + (o)))
#define BH_2e7d20(o) (*(s16 *)(b + (o)))
#define BI_2e7d20(o) (*(s32 *)(b + (o)))

/* Follow-camera leaf, called from follow_camera_build_view. */
void follow_camera_avoid_mobies(u8 *m) __asm__("FUN_L00_002e7d20");
extern s32 current_level_index_macro __asm__("D_0015ED84") __attribute__((section(".sdata")));
extern s32 camera_collision_mask __asm__("D_L00_0015EF58") __attribute__((sda));
void follow_camera_avoid_mobies(u8 *m) {
    V_2e7d20 v0, v10, v20, v30, v40;
    W_2e7d20 w;
    u8 *x;
    u8 *a;
    u8 *b;
    s32 hit, cnt, any;
    s32 j;
    u8 *o;
    s32 n;
    s32 i;
    s32 t;
    s32 s;
    f32 h;
    f32 f;
    u8 *base;
    f32 g;
    u8 *q;

    base = D_L00_00166E10_2e7d20;
    x = *(u8 **)(m + 0x70);
    a = x + 0x130;
    b = x + 0x1D0;
    hit = 0;
    cnt = 0;
    any = 0;
    h = AF_2e7d20(0x2C) - BF_2e7d20(0x30) + 1.5f;
    if (current_level_index_macro == 0xF) {
        any = D_0013F350_2e7d20.b20A4 == 2;
    }
    lerp_vector(&v0, x + 0x90, base - 0x50, 0.5f);
    n = probe_world_sphere(&v0, 1, D_0013F350_2e7d20.p2080, 0, h * 0.5f);
    if (n > 0) {
        i = n;
        j = 0;
        do {
            o = D_L00_00177F00_2e7d20[j];
            if (*(s32 *)(o + 0x94) == 0) {
                goto next;
            }
            if (FUN_L00_00259830(o) && !any) {
                goto next;
            }
            t = *(s16 *)(o + 0xA6);
            if (t != 0x72 && t != 0xB && t != 0x2C && t != 0x9A && t != 0xFF && t != 0x392 && t != 0x452 && t != 0x353) {
                D_L00_001E75C0[cnt] = o;
                cnt++;
                *(s32 *)(o + 0x94) = 0;
                goto next;
            }
            FUN_001f9a28_2e7d20(&v10, o + 0x10, (x + 0x90));
            FUN_001f9a68_2e7d20(&v20, base + 0x30, FUN_001f9ab0_2e7d20(base + 0x30, &v10));
            FUN_001f9a28_2e7d20(&v30, &v10, &v20);
            FUN_001f9bf8_2e7d20(&v30, &v30, 1.0f);
            f = FUN_001f9ab0_2e7d20(&v30, a);
            f = fast_arc_sin(f / (AF_2e7d20(0x2C) - BF_2e7d20(0x30)));
            if (FUN_001f99c0_2e7d20(1.5707964f - f) < 0.43633232f) {
                fast_vec_cross(&v40, a, base + 0x30);
                FUN_001f9bf8_2e7d20(&v40, &v40, 1.0f);
                if (0.0f < FUN_001f9ab0_2e7d20(&v40, &v10)) {
                    build_look_at_matrix(a, a, base + 0x20, -0.017453292f);
                } else {
                    build_look_at_matrix(a, a, base + 0x20, 0.017453292f);
                }
            }
        next:
            j++;
        } while (--i != 0);
    }
    if (D_0013F350_2e7d20.i2084 != 0x7F && (current_level_index_macro != 0xD || D_0013F350_2e7d20.i2084 != 0x7B)) {
        hit = FUN_L00_002e7a30_2e7d20(m) != 0;
    }
    hit = FUN_L00_002e7520_2e7d20(m) == 0 ? hit : 1;
    hit = FUN_L00_002e7318_2e7d20(m) == 0 ? hit : 1;
    g = AF_2e7d20(0x2C);
    if (g - BF_2e7d20(0x30) < D_L00_00161CD0) {
        BF_2e7d20(0x30) = g - D_L00_00161CD0;
        BH_2e7d20(0x34) = FUN_001f96f8_2e7d20(2000);
    }
    if (BH_2e7d20(0x3A) != 0) {
        goto dec;
    }
    if (hit) {
        goto slow;
    }
    s = BH_2e7d20(0x34);
    s = s < FUN_001f96f8_2e7d20(2000);
    if (s && BH_2e7d20(0x34) > 0) {
        goto dec;
    }
    if (!(D_0013C940_2e7d20.x < 0.3f)) {
        goto slow;
    }
    if (!(D_0013C940_2e7d20.y < 0.3f)) {
        goto slow;
    }
dec:
    if (fast_dec_timer(b + 0x34)) {
        BH_2e7d20(0x3A) = 0;
    }
    if (BH_2e7d20(0x3A) != 0) {
        if (fast_dec_timer(b + 0x34)) {
            BH_2e7d20(0x3A) = 0;
        }
    }
    f = convert_integer_to_float(BH_2e7d20(0x34));
    BF_2e7d20(0x30) = cosine_interpolate(0.0f, BF_2e7d20(0x30), f / convert_integer_to_float(FUN_001f96f8_2e7d20(2000)));
    goto move;
slow:
    s = BH_2e7d20(0x34);
    if (FUN_001f96f8_2e7d20(2000) < s) {
        fast_dec_timer(b + 0x34);
    } else {
        BH_2e7d20(0x34) = FUN_001f96f8_2e7d20(2000);
    }
move:
    FUN_001f9bf8_2e7d20(a, a, AF_2e7d20(0x2C) - BF_2e7d20(0x30));
    FUN_001f9a10_2e7d20(&v10, a, (x + 0x90));
    if (D_0013F350_2e7d20.i2084 == 0x7F) {
        goto clear;
    }
    if (current_level_index_macro == 0xD && D_0013F350_2e7d20.i2084 == 0x7B) {
        goto clear;
    }
    if (!collision_line((x + 0x90), &v10, camera_collision_mask, D_0013F350_2e7d20.p2080, 0)) {
        goto clear;
    }
    if (FUN_L00_002e4290_2e7d20((*(E40_2e7d20_t *)&D_L00_00173E40_lui).v20)) {
        goto clear;
    }
    if ((*(E40_2e7d20_t *)&D_L00_00173E40_lui).p18 != 0 && FUN_L00_0025e3b8((*(E40_2e7d20_t *)&D_L00_00173E40_lui).p18)) {
        FUN_001f9a28_2e7d20(&v20, (u8 *)(*(E40_2e7d20_t *)&D_L00_00173E40_lui).p18 + 0x10, D_L00_00166DC0);
        v20.i[2] = 0;
        FUN_001f9bf8_2e7d20(&v20, &v20, 1.0f);
        v20.f[3] = 5627.9248046875f;
        make_hit_descriptor(&w, D_0013F350_2e7d20.p2080, 0x800000, &v20, 20.0f);
        w.b18 = 3;
        w.b19 = 3;
        w.h1A = *(u16 *)(D_0013F350_2e7d20.p2080 + 0xA6);
        FUN_L00_00259a88((*(E40_2e7d20_t *)&D_L00_00173E40_lui).p18, &w);
    }
    BI_2e7d20(0x4C)++;
    if (BI_2e7d20(0x4C) >= FUN_001f96f8_2e7d20(0x1E)) {
        init_camera_system();
    }
    goto fin;
clear:
    BI_2e7d20(0x4C) = 0;
fin:
    if (cnt > 0) {
        u8 **op = D_L00_001E75C0;
        s32 k = cnt;
        do {
            q = *op++;
            *(s32 *)(q + 0x94) = *(s32 *)(*(u8 **)(q + 0x24) + 0x10);
        } while (--k != 0);
    }
}

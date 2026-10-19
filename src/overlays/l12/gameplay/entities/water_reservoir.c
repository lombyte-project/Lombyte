/* Level 12 moby classes: water_reservoir, rocket. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/input/pad_globals.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/level_labels.h"

extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int scale_game_frames_72(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer_72(int *) __asm__("FUN_001f9740");
extern float fast_add_rotations_72(float, float) __asm__("FUN_001fa580");
extern float convert_integer_to_float_72(int) __asm__("FUN_001fa6c0");
extern float random_float_between_72(float, float) __asm__("FUN_002132a8");
extern void random_vector_72(float *, float, float) __asm__("FUN_L00_00257d78");
extern void subtract_vector_72(float *, float *, float *) __asm__("FUN_001f9a28");
extern void scale_vector_72(float *, float *, float) __asm__("FUN_001f9a68");
extern void add_vector_72(float *, float *, float *) __asm__("FUN_001f9a10");
extern void *FUN_00218888_72(void *, float *, float *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float approach_value_72(float, float, float *) __asm__("FUN_00213ed8");
extern void enqueue_callback_list_1_72(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L12_002ecac0(struct Moby *);
extern float D_L12_0015F3FC;
extern float D_L12_001619CC __attribute__((sda));
int D_L12_001619D0 = 105;
extern float D_L12_001619D4 __attribute__((sda));
extern float D_L12_001619D8 __attribute__((sda));
extern int D_L12_001619DC __attribute__((sda));
extern float D_L12_001619E0 __attribute__((sda));
extern float D_L12_001619E4 __attribute__((sda));
int D_L12_001619E8 = 1;
extern int D_L12_001619EC __attribute__((sda));
int D_L12_001619F0 = 1;
int D_L12_001619F4 = 0x40404040;
int D_L12_001619F8 = 0x10022;
extern float D_L12_001619FC __attribute__((sda));
extern float D_L12_00161A00 __attribute__((sda));
extern float D_L12_00161A04 __attribute__((sda));
extern float D_L12_00161A08 __attribute__((sda));

void water_reservoir_update(struct Moby *moby) __asm__("FUN_L12_002ec720");
void water_reservoir_update(struct Moby *moby) {
    float *data = (float *)moby->pvars;
    float pos[4], offset0[4], offset1[4], diff0[4], diff1[4];
    float fraction, frame_scale;
    int scaled;
    if (moby->state != 0) {
        extern float D_L12_0015F3FC __attribute__((sda));
        frame_scale = D_L12_0015F3FC;
        if (moby->state == 1) goto active;
        goto update;
    }
    moby->state = 1;
    moby->command = 2;
    data[0] = random_angle_radians();
    data[2] = 5.0f;
    goto frame_active;
active:
    if (moby->command & 8) {
        moby->command = 1;
        *(int *)&data[1] = scale_game_frames_72(D_L12_001619D0);
    } else if (moby->command & 4) {
        moby->command = 2;
        *(int *)&data[1] = scale_game_frames_72(D_L12_001619D0);
    }
    data[0] = fast_add_rotations_72(data[0], D_L12_001619CC * 0.017453292f * frame_time);
frame_active:
    frame_scale = D_L12_0015F3FC;
update:
    if (frame_scale == 0.0f) {
        if (fast_dec_timer_72((int *)&data[1]) == 0) {
            if (moby->command & 1) {
                fraction = 1.0f - convert_integer_to_float_72(*(int *)&data[1]) /
                                  convert_integer_to_float_72(D_L12_001619D0);
            } else {
                fraction = convert_integer_to_float_72(*(int *)&data[1]) /
                           convert_integer_to_float_72(D_L12_001619D0);
            }
            if (random_float_between_72(0.0f, 1.0f) < fraction) {
                random_vector_72(pos, 0.0f, D_L12_001619D4);
                random_vector_72(offset0, 0.0f, D_L12_001619D4);
                random_vector_72(offset1, 0.0f, D_L12_001619D4);
                subtract_vector_72(diff0, pos, offset0);
                diff0[2] += D_L12_00161A00 - D_L12_001619FC;
                subtract_vector_72(diff1, offset0, offset1);
                diff1[2] += D_L12_00161A04 - D_L12_00161A00;
                scaled = D_L12_001619EC;
                scale_vector_72(diff0, diff0, 2.0f / (float)scaled);
                scale_vector_72(diff1, diff1, 2.0f / (float)scaled);
                add_vector_72(pos, pos, (float *)&moby->pos);
                if (moby->command & 1) {
                    pos[2] += D_L12_00161A04;
                    diff0[2] = -diff0[2];
                    diff1[2] = -diff1[2];
                } else {
                    pos[2] += D_L12_001619FC;
                }
                diff0[3] = random_float_between_72(D_L12_001619E0, D_L12_001619E4);
                diff1[3] = random_float_between_72(D_L12_001619E0, D_L12_001619E4);
                FUN_00218888_72(pos, diff0, diff1, D_L12_001619F4, D_L12_001619F4,
                                  D_L12_001619E8, scale_game_frames_72(D_L12_001619EC),
                                  D_L12_001619F0, D_L12_001619F8);
            }
        }
        if ((moby->command & 1) && *(int *)&data[1] == 0) {
            approach_value_72(-1.0f, D_L12_00161A08 * frame_time, &data[2]);
        } else if (moby->command & 2) {
            approach_value_72(3.0f, D_L12_00161A08 * frame_time, &data[2]);
        }
    }
    enqueue_callback_list_1_72(FUN_L12_002ecac0, moby);
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_002EDE40), where it is exact; names translated to the US level program. */

typedef struct {
    float v[16];
    int c[4];
    float uv[8];
    long g[4];
} Quad;

extern char D_L12_001F5B20[];
extern float fast_cos(float);
extern float fast_sin(float);
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int get_effect_texture(int) __asm__("FUN_001f44b8");
int D_L12_00161A10 = 1;
int D_L12_00161A14 = 0;
int D_L12_00161A0C = 0;
int D_L12_00161A18 = 1;
int D_L12_00161A1C = 0x40602020;
int D_L12_00161A20 = 0x40681818;
extern short D_L12_00161A24 __attribute__((sda));
extern void copy_matrix_rotation(void *, void *) __asm__("FUN_001fa298");
extern void draw_geometry_quad(void *, void *, int);

void FUN_L12_002ecac0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    Quad q;
    float m[12];
    float pos[4];
    float a, b;
    int i, col;
    copy_matrix_rotation(m, &moby->rot_row0);
    qcopy(pos, &moby->pos);
    pos[2] = pos[2] + *(float *)(data + 8);
    pos[3] = 1.0f;
    q.g[1] = get_effect_texture(0x10);
    q.g[0] = 0;
    q.g[2] = 0xFF9000000260UL;
    q.g[3] = (long)D_L12_00161A0C | ((long)D_L12_00161A10 << 2) |
             ((long)D_L12_00161A14 << 4) | ((long)D_L12_00161A18 << 6) |
             (0x8000L << 24);
    col = fast_tween_color(D_L12_00161A1C, D_L12_00161A20,
                       (fast_sin(*(float *)data) + 1.0f) * 0.5f);
    for (i = 0; i < 4; i++) {
        q.uv[i * 2] = 0.5f;
        *(&q.uv[i * 2] + 1) = 0.5f;
        q.c[i] = col;
        qcopy(&q.v[i * 4], D_L12_001F5B20 + i * 16);
    }
    for (a = -0.4f; a < 0.36f; a += 0.1f) {
        b = a + 0.1f;
        q.v[9] = a;
        q.v[1] = a;
        q.v[13] = b;
        q.v[5] = b;
        if (moby->command & 1) {
            q.v[2] = -fast_cos(a * *(float *)&D_L12_00161A24) + 2.0f;
            q.v[6] = -fast_cos(b * *(float *)&D_L12_00161A24) + 2.0f;
        } else {
            q.v[2] = fast_cos(a * *(float *)&D_L12_00161A24);
            q.v[6] = fast_cos(b * *(float *)&D_L12_00161A24);
        }
        draw_geometry_quad(&q, m, 0);
    }
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_002EE068), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00250df8(void *);

char *FUN_L12_002ecce8(char *src, char *pos, int owner, int seed, float f0, float f1, float f2) {
    struct Moby *moby = (struct Moby *)func_0020D348_m(0x199);
    if (moby != 0) {
        char *data;
        int r;
        data = (char *)moby->pvars;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0x7E;
        moby->visible = 1;
        moby->state = 1;
        qcopy(&moby->pos, pos);
        qcopy(data, src);
        moby->rot.z = fast_arc_tan(*(float *)data, *(float *)(data + 4));
        moby->rot.y = -fast_arc_tan(vector_length_xy(data), *(float *)(data + 8));
        r = scale_game_frames(seed);
        *(int *)(data + 0x10) = owner;
        *(float *)(data + 0x18) = f2;
        *(float *)(data + 0x1C) = f0;
        *(float *)(data + 0x24) = f1;
        *(int *)(data + 0x28) = 0;
        *(int *)(data + 0x14) = r;
        FUN_L00_00250df8(moby);
    }
    return (char *)moby;
}
typedef struct {
    float vel[4];
    struct Moby *owner;
    int timer;
    float unk18;
    float yaw;
} ShotVars_ece00;

typedef struct {
    float dir[4];
    float unk10[2];
    char unk18;
    char unk19;
    short oclass;
    int pad1C;
    int pad20[4];
} ShotHit_ece00;

typedef struct { char pad0[0x18]; int moby; } ShotColl_ece00;
extern ShotColl_ece00 D_L12_001742C0_ece00 __asm__("D_L12_001742C0") __attribute__((section(".data")));
extern int camera_secondary_mode_ece00 __asm__("D_001413D4") __attribute__((section(".data")));
extern char *FUN_L00_0025a420_ece00(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern void mark_moby_for_removal_ece00(struct Moby *) __asm__("FUN_0020c828");
extern void add_vector_ece00(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_ece00(void *, void *, void *) __asm__("FUN_001f9a28");
extern void set_vector_length_ece00(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_001ff290(void *, void *, void *);
extern int random_int_between_ece00(int, int) __asm__("FUN_L00_00257b90");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern int FUN_001efa68_ece00(void *, void *, int, struct Moby *, int) __asm__("FUN_001efa68");
extern int coll_sphere_ece00(void *, float, int, struct Moby *) __asm__("FUN_L00_001f0d60");
extern void make_hit_ece00(void *, struct Moby *, int, float, void *) __asm__("FUN_L00_00259888");
extern float fast_cos_ece00(float) __asm__("FUN_001f9dc8");
extern float fast_sin_ece00(float) __asm__("FUN_001f9de0");
extern void FUN_L00_00259a88(int, void *);
extern void spawn_explosion(struct Moby *, void *, void *, float, float, int, int, int, float, float, float, float,
                             int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");

/* Level 12 shot: flies with a smoke trail, damages the first moby it hits other than its owner, then pops. */
void rocket_update(struct Moby *moby) __asm__("FUN_L12_002ece00");
void rocket_update(struct Moby *moby) {
    enum {
        ROCKET_STATE_FLYING = 1,
        ROCKET_STATE_DETONATE = 2
    };
    ShotVars_ece00 *vars = (ShotVars_ece00 *)moby->pvars;
    float old_pos[4];
    float side[4];
    float back[4];
    float trail[4];
    ShotHit_ece00 hit;
    switch (moby->state) {
    case ROCKET_STATE_FLYING: {
        char *touch = FUN_L00_0025a420_ece00(moby, 0x230000, 0);
        int damage;
        if (touch != 0 && *(struct Moby **)(touch + 0x20) != vars->owner) {
            moby->state = 2;
            break;
        }
        if (current_level_index == 12 && camera_secondary_mode_ece00 == 50 && vars->owner != 0) {
            if ((signed char)vars->owner->state < 0) {
                mark_moby_for_removal_ece00(moby);
                return;
            }
            if (vars->owner->state < 2) {
                moby->state = 2;
                break;
            }
        }
        qcopy(old_pos, &moby->pos);
        add_vector_ece00(&moby->pos, &moby->pos, vars);
        moby->rot.z = vars->yaw;
        moby->rot.x = fast_add_rotations_72(moby->rot.x, frame_time * 6.2831855f);
        set_vector_length_ece00(side, vars, 0.2f);
        set_vector_length_ece00(back, vars, -0.2f);
        FUN_L00_001ff290(trail, back, &moby->pos);
        FUN_L00_00269958(back, side, 0x6F00AFFF, 0xFF, scale_game_frames(random_int_between_ece00(15, 22)), 40,
                         random_int_between_ece00(20, 35), 1);
        FUN_L00_00269958(back, side, 0x1FFFFFFF, 0x4F4F4F, scale_game_frames(random_int_between_ece00(30, 60)), 40,
                         random_int_between_ece00(50, 75), 0);
        if (FUN_001efa68_ece00(&moby->pos, old_pos, 0, moby, 0) || coll_sphere_ece00(&moby->pos, 0.2f, 0, moby)) {
            if (D_L12_001742C0_ece00.moby != 0 && D_L12_001742C0_ece00.moby != (int)vars->owner) {
                damage = 1;
                if (current_level_index == 12 && camera_secondary_mode_ece00 == 50) {
                    damage = D_L12_00161F20 >= 16 ? 2 : 3;
                }
                make_hit_ece00(&hit, moby, 0x10003, damage, vars);
                hit.dir[0] = fast_cos_ece00(vars->yaw);
                hit.dir[1] = fast_sin_ece00(vars->yaw);
                hit.dir[2] = 1.0f;
                hit.dir[3] = 5627.9248f;
                hit.unk18 = 1;
                hit.unk19 = 1;
                hit.oclass = moby->oclass;
                FUN_L00_00259a88(D_L12_001742C0_ece00.moby, &hit);
            }
            moby->state = 2;
        }
        if (fast_dec_timer_72(&vars->timer)) {
            moby->state = 2;
        }
        break;
    }
    case ROCKET_STATE_DETONATE:
        subtract_vector_ece00(old_pos, &moby->pos, D_L12_00167240);
        set_vector_length_ece00(old_pos, old_pos, 0.4f);
        add_vector_ece00(&moby->pos, &moby->pos, old_pos);
        if (current_level_index == 12 && camera_secondary_mode_ece00 == 50) {
            char *camera = D_L12_00167240 - 0x140;
            spawn_explosion(moby, &D_L12_0015F580, 0, 0.0f, 0.0f, 3, 3, 5, 0.3f, 0.2f, 0.0f, 0.15f, -1, 0.0f, 0, 0,
                             -1, 0);
            *(float *)(camera + 0x160) = 0.025f;
            *(int *)(camera + 0x168) = scale_game_frames(20);
        } else {
            cart_burst(moby, &moby->pos, 0, 0.25f, 13.0f);
        }
        mark_moby_for_removal_ece00(moby);
        break;
    }
}
/* Hoven challenge tracker: unlocks the area once, records the time and run flags for two skill points and
 * awards them (one for staying airborne long enough). */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_002EE600), where it is exact; names translated to the US level program. */

extern unsigned char D_0013D408_b[] __asm__("D_0013D408");
extern char D_00141968_h[] __asm__("D_00141968");
extern int FUN_L01_0026e008(int, int);
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void allocate_voice_for_bank_entry_alt(int, int, int) __asm__("FUN_0022db10");
extern unsigned char D_0013D50F_e[] __asm__("D_0013D408");

void FUN_L12_002ed280(struct Moby *m) {
    int *d = (int *)m->pvars;
    if (FUN_L01_0026e008(d[3], -1) == 0) {
        unsigned char *s = D_0013D50F_e;
        if (s[0x14] == 0) {
            s[0x14] = 1;
            allocate_voice_for_bank_entry_alt(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
    }
    {
        char *h = D_00141968_h;
        if (*(unsigned short *)(h + 0x368) == 0) {
            struct Hero *q = &hero;
            if (q->state.current == 0x81) {
                *(unsigned short *)(h + 0x368) = *(unsigned short *)(h + 0x368) + 1;
            }
            if (scale_game_frames_c(D_0015EEA4) / 600 > *(unsigned short *)(h + 0x36A)) {
                *(unsigned short *)(h + 0x36A) = scale_game_frames_c(D_0015EEA4) / 600;
            }
            *(unsigned int *)(h + 0x36C) = *(unsigned int *)(h + 0x36C) | (1 << current_level_index) | 0x80000000;
            if (is_point_inside_clip_volume(&q->motion.pos, d[0])) {
                help_request(0x2EE3, 0x6D);
            }
        }
    }
    {
        char *h = D_00141968_h;
        if (*(unsigned short *)(h + 0x370) == 0
            || (*(unsigned short *)(h + 0x370) < 2
                && scale_game_frames_c(D_0015EEA4) - *(unsigned short *)(h + 0x372) * 600 > scale_game_frames_c(72000))) {
            struct Hero *q = &hero;
            if (q->state.current == 0x81) {
                d[1]++;
                if ((pad_held[0] & 3) && 0.3f < q->unk229C) {
                    d[2]++;
                } else {
                    d[2] = 0;
                }
                if (scale_game_frames_c(0x28) < d[2]) {
                    char *g = D_00141968_h;
                    d[1] = 0;
                    if (*(unsigned short *)(g + 0x370) <= 0xFFFE) {
                        *(unsigned short *)(g + 0x370) = *(unsigned short *)(g + 0x370) + 1;
                    }
                    if (scale_game_frames_c(D_0015EEA4) / 600 > *(unsigned short *)(g + 0x372)) {
                        *(unsigned short *)(g + 0x372) = scale_game_frames_c(D_0015EEA4) / 600;
                    }
                    *(unsigned int *)(g + 0x374) = *(unsigned int *)(g + 0x374) | (1 << current_level_index) | 0x80000000;
                }
                {
                    int a = scale_game_frames_c(0x1C20);
                    int b = scale_game_frames_c(0x708);
                    char *k = D_00141968_h;
                    if (*(unsigned short *)(k + 0x370) * a + b < d[1]) {
                        help_request(0x2EE4, 0x6E);
                    }
                }
            }
        }
    }
    if (D_0013D408_b[0xBC] != 0) {
        char *k = D_00141968_h;
        if (*(unsigned short *)(k + 0x1F0) == 0) {
            help_request(0x2EE0, 0x3E);
        }
    }
}

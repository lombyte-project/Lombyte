/* Level 17 moby classes: blarg_ship. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/level_labels.h"

/* Update for moby class 1470: watches camera boxes around the hero, gives skill points and help messages, and turns the camera. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F4210), where it is exact; names translated to the US level program. */

extern char *D_L17_0015FFDC;
extern char D_0013E52B[];
extern char D_L17_00167580[];
extern char D_L17_001E6F60[];
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern int D_00141DC4 __attribute__((section(".data")));
extern int D_L17_00179D10[];
extern int help_request(int a, int b) __asm__("FUN_L00_00203908");
extern int FUN_L00_0020d568(void);
extern int allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int scale_game_frames(int);
extern int tick_countdown_32();
extern unsigned char D_0013D408[];
extern unsigned char D_0013D408_b[] __asm__("D_0013D408");
extern unsigned char D_0013D4DF __attribute__((section(".data")));
extern int D_0015ED80_e[] __asm__("D_0015ED80") __attribute__((section(".sdata")));
extern void DebugPrint();
extern void FUN_L00_002039a0(void);
extern void queue_dialogue_message(int arg0, int arg1) __asm__("FUN_L00_00263d40");
extern void FUN_L00_002e8680(void);
extern void FUN_L00_002e8970(int a, float x, float y);
extern void FUN_L06_00239528(void);
extern int hero_set_state(int, int) __asm__("FUN_L17_0021e530");
extern char D_00141D54[];
extern char D_00141D7C[];
#include "rnc/gameplay/state/game_globals.h"
extern int func_001F9908_i(int *) __asm__("FUN_001f9740");

void FUN_L17_002f26d0(struct Moby *moby) {
    int off;
    char *d = (char *)moby->pvars;
    int st;
    int i;
    int j;
    int found;
    int tgt;
    int idx;
    char *m;
    int *p;
    int *q;
    float f;
    int n;
    int mask;
    unsigned char *sk;
    int wrap;

    st = *(int *)(d + 0x6C);
    switch (st) {
    case 0: {
        char *h = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(h, *(int *)(d + 0x60))) {
            if (*(int *)(h + 0x200C) == 0x11) {
                *(int *)(d + 0x6C) = 1;
            }
        }
        break;
    }
    case 1: {
        struct Hero *h = &hero;
        int v = h->state.current;
        if (v == 0x76 || v == 0x32) {
            *(int *)(d + 0x6C) = 0;
        } else if (is_point_inside_clip_volume(&h->motion.pos, *(int *)(d + 0x64)) ||
                   is_point_inside_clip_volume(&h->motion.pos, *(int *)(d + 0x68))) {
            if (D_0013D408[0x1C] == 0) {
                D_0013D408[0x1C] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        break;
    }
    }

    if (D_0013D408[0x1B] == 0) {
        for (j = 0; j < 30; j++) {
            char *mb;
            sk = D_0013D408;
            mb = D_L17_0015FFD8 + (*(int *)(d + 0x70) << 8);
            if ((*(short *)(mb + 0xA6) == 0x2C || *(short *)(mb + 0xA6) == 0x566) &&
                *(signed char *)(mb + 0x20) >= 0 &&
                !is_point_inside_clip_volume(mb + 0x10, *(int *)(d + 0x78))) {
                *(int *)(d + 0x74) += 1;
            }
            n = *(int *)(d + 0x70) + 1;
            wrap = D_L17_0015FFD8 + (n << 8) < D_L17_0015FFDC;
            *(int *)(d + 0x70) = n;
            if (!wrap) {
                if (*(int *)(d + 0x74) == 0 && sk[0x1B] == 0) {
                    sk[0x1B] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    queue_dialogue_message(0x53D6, -1);
                }
                *(int *)(d + 0x70) = 0;
                *(int *)(d + 0x74) = 0;
            }
        }
    }

    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        *(int *)(d + 0x38) = scale_game_frames(600);
        *(unsigned char *)(d + 0x4C) = 0;
        if (*(int *)(d + 0x44) == -1 || *(int *)(d + 0x40) == -1 || *(int *)(d + 0x3C) == -1 ||
            *(int *)(d + 0x48) == -1) {
            DebugPrint(D_L17_001E6F60, *(int *)(d + 0x3C), *(int *)(d + 0x48), *(int *)(d + 0x40),
                       *(int *)(d + 0x44));
            *(unsigned char *)(d + 0x4C) = 1;
        }
        moby->state = 1;
        break;
    case 1:
        tgt = -1;
        idx = -1;
        found = 0;
        for (i = 0; i < 1; i++) {
            off = i * 4;
            if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(d + off))) {
                found = 1;
                if (*(int *)(d + 0x30) != *(int *)(d + off)) {
                    tgt = *(int *)(d + off);
                    p = (int *)(d + 0x10);
                    idx = p[i];
                }
                break;
            }
        }
        if (found) {
            *(int *)(d + 0xC) += 1;
        } else {
            *(int *)(d + 0xC) = 0;
            *(int *)(d + 0x30) = -1;
        }
        if (idx != -1) {
            m = D_L17_0015FFD8 + (idx << 8);
            if (m == 0)
                goto skip1;
            if (*(short *)(m + 0xA6) != 0x4E)
                goto skip1;
            if (*(unsigned char *)(m + 0x20) == 0xFE)
                goto skip1;
            if (*(unsigned char *)(m + 0x20) == 0xFD)
                goto skip1;
            if (*(unsigned char *)(m + 0x20) == 3)
                goto skip1;
        }
        if (*(int *)(d + 0xC) > *(int *)(d + 0x38) && tgt != -1 &&
            ((unsigned int)hero.state.control_mode < 2 ||
             hero.state.control_mode == 9) &&
            D_L17_00179D10[0] == 0 && D_L17_00179D10[9] == -1 && D_0013D4DF != 0) {
            *(int *)(d + 0x38) = scale_game_frames(0x708);
            if (*(int *)(((char *)&D_00141D54)) >= 0) {
                help_request(0x3A9A, 0x7D);
            }
            *(int *)(d + 0x30) = tgt;
            *(int *)(d + 0xC) = 0;
        }
    skip1:
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(d + 0x20)) && D_L17_00179D10[0] == 0 &&
            D_L17_00179D10[9] == -1 && D_0013D408_b[0xBC] == 0) {
            int w = *(int *)(((char *)&D_00141D7C));
            mask = 1 << D_0015ED80_e[1];
            if (!(w & mask)) {
                help_request(0x3A9D, 0x82);
            }
        }
        tgt = -1;
        found = 0;
        for (i = 0; i < 3; i++) {
            off = i * 4;
            q = (int *)(d + 0x20 + off);
            if (is_point_inside_clip_volume(&hero.motion.pos, *q)) {
                found = 1;
                if (*(int *)(d + 0x30) != *q) {
                    tgt = *q;
                }
                break;
            }
        }
        if (!found || (*(int *)(d + 0x2C) += 1, hero.state.current == 0x35)) {
            *(int *)(d + 0x2C) = 0;
            *(int *)(d + 0x30) = -1;
        }
        if (scale_game_frames(0xE10) < *(int *)(d + 0x2C) && tgt != -1 && FUN_L00_0020d568() &&
            D_L17_00179D10[0] == 0 && D_L17_00179D10[9] == -1 && D_0013D408_b[0xBC] != 0) {
            help_request(0x4E2E, 0x78);
            *(int *)(d + 0x30) = tgt;
            *(int *)(d + 0x2C) = 0;
        }
        if (*(int *)(d + 0x58) != 0xFF &&
            visited52[*(int *)(d + 0x58) + (current_level_index << 4)] == 0xFF) {
            *(unsigned char *)(d + 0x4C) = 1;
        }
        if (*(unsigned char *)(d + 0x4C) == 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, *(int *)(d + 0x3C))) {
            FUN_L00_002039a0();
            if (*(int *)(((char *)&D_00141DC4)) >= 0) {
                help_request(0x4268, 0x8B);
            }
            if (D_L17_00179D10[0] != 0 || D_L17_00179D10[9] != -1) {
                *(int *)(d + 0x50) = 0;
                moby->state = 2;
                *(int *)(d + 0x5C) = scale_game_frames(0xB4);
                hero_set_state(0x72, 0);
            }
        }
        break;
    case 2: {
        char *cam;
        FUN_L00_002e8680();
        FUN_L00_002e8970((int)(D_L17_001600EC + (*(int *)(d + 0x44) << 7) + 0x30), 0.02617994f,
                         0.0f);
        cam = D_L17_00167580;
        f = fast_arc_tan(*(float *)(D_L17_001600EC + ((*(int *)(d + 0x44) << 7) + 0x30)) -
                             *(float *)(cam + 0x140),
                         *(float *)(D_L17_001600EC + ((*(int *)(d + 0x44) << 7) + 0x34)) -
                             *(float *)(cam + 0x144));
        if (fast_difference_between_rotations(*(float *)(cam + 0x158), f) < 0.034906585f) {
            moby->state = 3;
            *(int *)(d + 0x5C) = scale_game_frames(0x3C);
        }
        break;
    }
    case 3:
        FUN_L00_002e8680();
        if (func_001F9908_i((int *)(d + 0x5C))) {
            char *cam;
            FUN_L00_002e8970((int)(D_L17_001600EC + (*(int *)(d + 0x40) << 7) + 0x30), 0.02617994f,
                             0.0f);
            cam = D_L17_00167580;
            f = fast_arc_tan(*(float *)(D_L17_001600EC + ((*(int *)(d + 0x40) << 7) + 0x30)) -
                                 *(float *)(cam + 0x140),
                             *(float *)(D_L17_001600EC + ((*(int *)(d + 0x40) << 7) + 0x34)) -
                                 *(float *)(cam + 0x144));
            if (fast_difference_between_rotations(*(float *)(cam + 0x158), f) < 0.034906585f) {
                FUN_L06_00239528();
                *(unsigned char *)(d + 0x4C) = 1;
                moby->state = 1;
            }
        }
        break;
    }
}
#include "sda.h"
#include "rnc/overlay/quad.h"
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F49B8), where it is exact; names translated to the US level program. */
extern void FUN_L00_002637f8_p() __asm__("FUN_L00_002637f8");

extern struct {
    int pad0[12];
    int v;
    int pad1[81];
    int tbl[3];
} D_L17_0016D160;
void FUN_L17_002f2e78(struct Moby *m) {
    switch (m->state) {
    case 0:
        m->update_dist = 0xFF;
        m->state = 1;
        break;
    case 1:
        if (D_L17_0015F5C4 == 2) {
            int v = D_L17_0016D160.v;
            int i;
            if (v != 0 && v != D_L17_0015F5C4)
                return;
            i = 0;
            if (v == 0 || v == D_L17_0015F5C4)
                i = 2;
            FUN_L00_002637f8_p(D_L17_0016D160.tbl[i]);
        }
        break;
    }
}

/* Draws the Drek's Fleet reflection/backdrop quads around a moby. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F4E30), where it is exact; names translated to the US level program. */


typedef struct {
    OvlQuad m[4];
    int col[4];
    float uv[8];
    int q[8];
} Pkt;

extern char D_L17_001676C0[];
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern float frame_delta_time __asm__("D_0015ED7C");
extern float fast_cos_c(float) __asm__("FUN_001f9dc8");
extern float fast_sin_c(float) __asm__("FUN_001f9de0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int get_effect_texture_alt(int) __asm__("FUN_001f44b8");
float D_L17_001623C0[4] __attribute__((section(".sdata"))) = {-0.5f, -1.0f, 1.0f, 1.0f};
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
void vu1_add_g_sregister_c(s32 register_id, s64 value) __asm__("FUN_00233980");

extern float D_L17_0016D3E4 __attribute__((section(".data")));
float D_L17_001DDFF8[6] = {3.5e+03f, 4e+03f, 2e+03f, 3e+03f, 3.3e+03f, 0.0f};
float D_L17_001DE040[6] = {0.0f, 1.0f, 0.33f, 0.55f, 0.22f, 0.0f};
float D_L17_001DE0A0[6] = {0.0f, 0.1f, 0.2f, 0.3f, 0.5f, 0.0f};
float D_L17_001DE0B8[6] = {2e+01f, 1e+01f, 2e+01f, 2e+01f, 2e+01f, 0.0f};
float D_L17_001DE0D0[1] __attribute__((section(".data"))) = {0};
float D_L17_001E6FA0[8] = {-8.0f, 0.0f, -8.0f, 1.0f, 8.0f, 0.0f, 8.0f, 1.0f};
int D_L17_001DE010[6] = {0x80252525, 0x80202020, 0x80252525, 0x80101010, 0x80151515, 0};
int D_L17_001DE028[6] = {0x80101030, 0x80301010, 0x80101015, 0x80151010, 0x80151015, 0};
int D_L17_001DE088[6] = {0xe, 0xe, 0x31, 0x31, 0x31, 0};
int D_L17_001623B8 = 0x807e4931;
void FUN_L17_002f32f0(struct Moby *moby) {
    Pkt pkt;
    OvlQuad vv[4];
    char *data = (char *)moby->pvars;
    char *pos;
    int i;
    int k;
    int j;
    OvlQuad *vb;
    float *pu;
    float *pv;
    vu1_add_g_sregister_c(0x47, 0x513F1);
    vb = &vv[2];
    pos = data + 0x10;
    pu = &pkt.uv[0];
    pv = &pkt.uv[1];
    D_L17_0016D3E4 = 8192000.0f;
    *(float *)(data + 0x14) = -8000.0f;
    for (i = 0; i < 2; i++) {
        subtract_vector_xyz(vb, data, pos);
        subtract_vector_xyz(&vv[0], data, D_L17_001676C0);
        subtract_vector_xyz(&vv[1], pos, D_L17_001676C0);
        cross_vectors_xyz(&vv[0], &vv[0], vb);
        normalize_vector_xyz(&vv[0], &vv[0], D_L17_001DE0B8[i]);
        cross_vectors_xyz(&vv[1], &vv[1], vb);
        normalize_vector_xyz(&vv[1], &vv[1], *(float *)(data + 0x28));
        *(long *)&pkt.q[4] = 0x0000FF9000000260LL;
        *(long *)&pkt.q[6] = 0x0000008000000048LL;
        *(long *)&pkt.q[0] = 0;
        pkt.col[3] = D_L17_001623B8;
        pkt.col[2] = D_L17_001623B8;
        pkt.col[1] = D_L17_001623B8;
        pkt.col[0] = D_L17_001623B8;
        {
            float *qu = &pkt.uv[0];
            float *qv = &pkt.uv[1];
            int m;
            for (m = 0; m < 4; m++) {
                qu[m * 2] = D_L17_001E6FA0[m * 2];
                qv[m * 2] = D_L17_001E6FA0[m * 2 + 1];
            }
        }
        subtract_vector_xyz(&vv[3], data, &vv[0]);
        pkt.m[0] = vv[3];
        add_vector_xyz(&vv[3], data, &vv[0]);
        pkt.m[1] = vv[3];
        subtract_vector_xyz(&vv[3], pos, &vv[1]);
        pkt.m[2] = vv[3];
        add_vector_xyz(&vv[3], pos, &vv[1]);
        pkt.m[3] = vv[3];
        *(long *)&pkt.q[2] = get_effect_texture_alt(D_L17_001DE088[i]);
        for (j = 0; j < 4; j++) {
            pu[j * 2] = D_L17_001E6FA0[j * 2] + D_L17_001DE0A0[i];
            pv[j * 2] = D_L17_001E6FA0[j * 2 + 1];
        }
        D_L17_001DE0A0[i] += D_L17_001623C0[i] * frame_delta_time;
        draw_geometry_quad(&pkt, 0, 0);
    }
    *(long *)&pkt.q[2] = get_effect_texture_alt(0x2F);
    for (k = 0; k < 5; k++) {
        float s = D_L17_001DDFF8[k];
        float ang = D_L17_001DE0D0[k];
        float x = *(float *)(data + 0x10) - s * 0.5f;
        float z;
        ((float *)&pkt)[0] = x;
        ((float *)&pkt)[1] = *(float *)(data + 0x14) + 1000.0f;
        z = *(float *)(data + 0x18) + s * 0.5f;
        ((float *)&pkt)[3] = 1.0f;
        ((float *)&pkt)[2] = z;
        pkt.m[3] = pkt.m[0];
        pkt.m[2] = pkt.m[0];
        pkt.m[1] = pkt.m[0];
        ((float *)&pkt)[0] = x + 0.0f;
        ((float *)&pkt)[2] = z + 0.0f;
        ((float *)&pkt)[4] = ((float *)&pkt)[4] + 0.0f;
        ((float *)&pkt)[6] = ((float *)&pkt)[6] - s;
        ((float *)&pkt)[8] = ((float *)&pkt)[8] + s;
        ((float *)&pkt)[10] = ((float *)&pkt)[10] + 0.0f;
        ((float *)&pkt)[12] = ((float *)&pkt)[12] + s;
        ((float *)&pkt)[14] = ((float *)&pkt)[14] - s;
        pkt.uv[0] = fast_sin_c(fast_add_rotations(ang, 4.712389f)) * 0.5f + 0.5f;
        pkt.uv[1] = fast_cos_c(fast_add_rotations(ang, 4.712389f)) * 0.5f + 0.5f;
        pkt.uv[2] = fast_sin_c(fast_add_rotations(ang, 3.1415927f)) * 0.5f + 0.5f;
        pkt.uv[3] = fast_cos_c(fast_add_rotations(ang, 3.1415927f)) * 0.5f + 0.5f;
        pkt.uv[4] = fast_sin_c(ang) * 0.5f + 0.5f;
        pkt.uv[5] = fast_cos_c(ang) * 0.5f + 0.5f;
        pkt.uv[6] = fast_sin_c(fast_add_rotations(ang, 1.5707964f)) * 0.5f + 0.5f;
        pkt.uv[7] = fast_cos_c(fast_add_rotations(ang, 1.5707964f)) * 0.5f + 0.5f;
        {
            int c = fast_tween_color(D_L17_001DE010[k], D_L17_001DE028[k], D_L17_001DE040[k]);
            pkt.col[3] = c;
            pkt.col[2] = c;
            pkt.col[1] = c;
            pkt.col[0] = c;
        }
        draw_geometry_quad(&pkt, 0, 0);
    }
    vu1_add_g_sregister_c(0x47, 0x5360B);
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F5388), where it is exact; names translated to the US level program. */


extern float D_L17_001623D0 __attribute__((sda));
extern float D_L17_001623D8 __attribute__((sda));
extern void FUN_L17_002f32f0_c(char *) __asm__("FUN_L17_002f32f0");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
s32 is_point_inside_clip_volume_u(s32 arg0, s32 arg1) __asm__("FUN_00214720");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

float D_L17_001DDFE0[6] = {0.5f, -0.24f, 0.3f, 1.0f, -0.6f, 0.0f};
float D_L17_001DE058[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};
float D_L17_001DE070[6] = {0.1f, 0.15f, 0.26f, 0.17f, 0.05f, 0.0f};
extern float D_L17_001623D4 __attribute__((sda));
void FUN_L17_002f3848(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    float t0[4];
    float t1[4];
    float t2[4];
    int i;
    if (*(int *)(d + 0x4C) != -1) {
        struct Hero *g = &hero;
        if (g->state.current != 0x32 || ((char *)g->ship_moby) == 0 ||
            *(short *)(((char *)g->ship_moby) + 0xA6) != 0x563) {
            if (is_point_inside_clip_volume_u(&hero.motion.pos, *(int *)(d + 0x4C))) {
                D_L17_0015F638 = 150.0f;
                goto done;
            }
        }
        D_L17_0015F638 = 10.0f;
    }
done:
    if (moby->state == 0) {
        moby->state = 1;
        for (i = 0; i < 5; i++) {
            D_L17_001DE0D0[i] = (float)i * 25.0f;
        }
        qcopy(d, &moby->pos);
        build_spherical_offset(
            t0, (D_L17_001623D4 + D_L17_001623D0) / D_L17_001623D8,
            moby->rot.z, 0.0f);
        build_spherical_offset(t1, D_L17_001623D0 / D_L17_001623D8,
                               *(float *)(d + 0x44), 0.0f);
        add_vector_xyz(t2, t0, t1);
        *(OvlQuad *)t0 = *(OvlQuad *)t2;
        add_vector_xyz(t2, d, t0);
        *(OvlQuad *)(d + 0x10) = *(OvlQuad *)t2;
        moby->update_dist = 0xFF;
        *(float *)(d + 0x10) -= 300.0f;
    } else {
        for (i = 0; i < 5; i++) {
            D_L17_001DE040[i] += D_L17_001DE070[i] * D_L17_001DE058[i] * frame_delta_time;
            if (1.0f < D_L17_001DE040[i]) {
                D_L17_001DE040[i] = 1.0f;
                D_L17_001DE058[i] = -1.0f;
            }
            if (D_L17_001DE040[i] < 0.0f) {
                D_L17_001DE040[i] = 0.0f;
                D_L17_001DE058[i] = 1.0f;
            }
        }
        for (i = 0; i < 5; i++) {
            D_L17_001DE0D0[i] =
                fast_add_rotations(D_L17_001DE0D0[i], D_L17_001DDFE0[i] * 0.1f * frame_delta_time);
        }
        for (i = 0; i < 2; i++) {
            D_L17_001DE0A0[i] += D_L17_001623C0[i] * frame_delta_time;
            if (1.0f <= D_L17_001DE0A0[i]) {
                D_L17_001DE0A0[i] -= 1.0f;
            }
            if (D_L17_001DE0A0[i] < 0.0f) {
                D_L17_001DE0A0[i] += 1.0f;
            }
        }
        enqueue_callback_list_1_alt(FUN_L17_002f32f0_c, moby);
    }
}
#include "sda.h"

#include "qcopy.h"

/* UpdateMoby 1843: a flying fleet craft that circles, follows a path, strafes the player and fires. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F5C18), where it is exact; names translated to the US level program. */

extern char *FUN_L11_00308f48(int owner, float *dir, float *pos, float size, float len, float z);
extern char D_L17_001676C0[];
char D_L17_001DE110[16] = {-102, -103, -103, 0x3e, -51, -52, -52, 0x3e, 0x33, 0x33, -109, -65, 0, 0, 0, 0};
char D_L17_001DE120[1920] = {-102, -103, -103, 0x3e, -51, -52, -52, -66, 0x33, 0x33, -109, -65, 0, 0, 0, 0, -2, -42, 0x2d, 0x44, 0x7b, 0x6f, 9, 0x44, 0x51, -14, 0x2b, 0x43, -72, -71, -69, 0x41, -122, -41, 0x2d, 0x44, -65, 0x2d, 9, 0x44, 2, 0x6d, 0x32, 0x43, 0x30, -102, -77, 0x41, -118, -72, 0x28, 0x44, 0x46, 0x5b, 7, 0x44, 0x19, -88, 0x2e, 0x43, 0x58, -85, 0x28, 0x44, -90, 0x60, 7, 0x44, -121, 0x3c, 0x29, 0x43, -92, 0x2e, 0x29, 0x44, -122, 0x55, 8, 0x44, 0x1c, -88, 0x2e, 0x43, -32, 0x22, 0x29, 0x44, -5, 0x5d, 8, 0x44, -119, 0x3c, 0x29, 0x43, -42, -33, 0x29, 0x44, 0x2d, 0x37, 9, 0x44, 0x1c, -88, 0x2e, 0x43, 0x39, -42, 0x29, 0x44, 0x66, 0x42, 9, 0x44, -123, 0x3c, 0x29, 0x43, 0x74, -63, 0x2a, 0x44, 0x58, -24, 9, 0x44, 0x1c, -88, 0x2e, 0x43, -95, -70, 0x2a, 0x44, -65, -11, 9, 0x44, -123, 0x3c, 0x29, 0x43, -71, -69, 0x2b, 0x44, 0x70, 0x5e, 0xa, 0x44, 0x1b, -88, 0x2e, 0x43, -8, -73, 0x2b, 0x44, 0x4b, 0x6d, 0xa, 0x44, -123, 0x3c, 0x29, 0x43, -83, -58, 0x2c, 0x44, 0x40, -93, 0xa, 0x44, 0x1c, -88, 0x2e, 0x43, 0x38, -58, 0x2c, 0x44, -14, -78, 0xa, 0x44, -123, 0x3c, 0x29, 0x43, -110, -43, 0x2d, 0x44, 0x42, -72, 0xa, 0x44, 0x1c, -88, 0x2e, 0x43, 0x69, -40, 0x2d, 0x44, 0x36, -56, 0xa, 0x44, -123, 0x3c, 0x29, 0x43, 0x73, -28, 0x2e, 0x44, 0x41, -93, 0xa, 0x44, 0x1d, -88, 0x2e, 0x43, -92, -22, 0x2e, 0x44, -14, -78, 0xa, 0x44, -122, 0x3c, 0x29, 0x43, 0x6b, -17, 0x2f, 0x44, 0x70, 0x5e, 0xa, 0x44, 0x1c, -88, 0x2e, 0x43, -30, -8, 0x2f, 0x44, 0x4b, 0x6d, 0xa, 0x44, -122, 0x3c, 0x29, 0x43, -78, -23, 0x30, 0x44, 0x59, -24, 9, 0x44, 0x1d, -88, 0x2e, 0x43, 0x37, -10, 0x30, 0x44, -65, -11, 9, 0x44, -119, 0x3c, 0x29, 0x43, 0x4e, -53, 0x31, 0x44, 0x2b, 0x37, 9, 0x44, 0x19, -88, 0x2e, 0x43, -100, -38, 0x31, 0x44, 0x64, 0x42, 9, 0x44, -119, 0x3c, 0x29, 0x43, 0x75, 0x7c, 0x32, 0x44, -124, 0x55, 8, 0x44, 0x18, -88, 0x2e, 0x43, -9, -115, 0x32, 0x44, -5, 0x5d, 8, 0x44, -121, 0x3c, 0x29, 0x43, -112, -14, 0x32, 0x44, 0x44, 0x5b, 7, 0x44, 0x19, -88, 0x2e, 0x43, -126, 5, 0x33, 0x44, -90, 0x60, 7, 0x44, -121, 0x3c, 0x29, 0x43, -9, 0x2a, 0x29, 0x44, 0x32, 0x2c, 7, 0x44, -68, -66, 0x34, 0x43, 9, -38, 0x28, 0x44, -124, 0x4d, 7, 0x44, 0x46, 0x1b, 0x30, 0x43, -2, -106, 0x29, 0x44, 0x20, 0x11, 8, 0x44, -67, -66, 0x34, 0x43, 0x18, 0x4d, 0x29, 0x44, 0x62, 0x41, 8, 0x44, 0x49, 0x1b, 0x30, 0x43, 0x12, 0x39, 0x2a, 0x44, -122, -33, 8, 0x44, -68, -66, 0x34, 0x43, -66, -7, 0x29, 0x44, 0x3f, 0x1d, 9, 0x44, 0x4b, 0x1b, 0x30, 0x43, 0x77, 7, 0x2b, 0x44, -101, -127, 9, 0x44, -68, -66, 0x34, 0x43, -103, -43, 0x2a, 0x44, -26, -55, 9, 0x44, 0x4b, 0x1b, 0x30, 0x43, 0x66, -20, 0x2b, 0x44, -97, -19, 9, 0x44, -68, -66, 0x34, 0x43, 0x78, -55, 0x2b, 0x44, -9, 0x3c, 0xa, 0x44, 0x49, 0x1b, 0x30, 0x43, -104, -32, 0x2c, 0x44, -108, 0x2c, 0xa, 0x44, -68, -66, 0x34, 0x43, -104, -51, 0x2c, 0x44, 4, -128, 0xa, 0x44, 0x4a, 0x1b, 0x30, 0x43, 0x65, -40, 0x2d, 0x44, -53, 0x3f, 0xa, 0x44, -67, -66, 0x34, 0x43, -110, -43, 0x2d, 0x44, 0x7b, -108, 0xa, 0x44, 0x4b, 0x1b, 0x30, 0x43, 0x2a, -48, 0x2e, 0x44, -108, 0x2c, 0xa, 0x44, -64, -66, 0x34, 0x43, -127, -35, 0x2e, 0x44, 4, -128, 0xa, 0x44, 0x4b, 0x1b, 0x30, 0x43, 0x65, -60, 0x2f, 0x44, -97, -19, 9, 0x44, -68, -66, 0x34, 0x43, 0x7c, -28, 0x2f, 0x44, -67, 0x3b, 0xa, 0x44, 0x46, 0x1b, 0x30, 0x43, 0x52, -87, 0x30, 0x44, -101, -127, 9, 0x44, -69, -66, 0x34, 0x43, 0x59, -40, 0x30, 0x44, -85, -56, 9, 0x44, 0x49, 0x1b, 0x30, 0x43, -71, 0x77, 0x31, 0x44, -122, -33, 8, 0x44, -68, -66, 0x34, 0x43, 0x3b, -76, 0x31, 0x44, 6, 0x1c, 9, 0x44, 0x46, 0x1b, 0x30, 0x43, -54, 0x19, 0x32, 0x44, 0x1f, 0x11, 8, 0x44, -68, -66, 0x34, 0x43, -37, 0x60, 0x32, 0x44, 0x27, 0x40, 8, 0x44, 0x45, 0x1b, 0x30, 0x43, -48, -123, 0x32, 0x44, 0x32, 0x2c, 7, 0x44, -71, -66, 0x34, 0x43, -18, -45, 0x32, 0x44, 0x4b, 0x4c, 7, 0x44, 0x45, 0x1b, 0x30, 0x43, -110, -21, 0x26, 0x3b, 0, 0, 0, 0, 0, 0, 0, 0, -117, -1, 0x7f, 0x3f, 4, -85, 0x6a, 0x3f, 0, 0, 0, 0, -63, -30, 0x6c, 0x3f, 0, 0, -128, 0x3f, 0x5b, 0x22, 0x73, 0x3f, 0, 0, 0, 0, 0x4b, -108, 0x75, 0x3f, 0, 0, -128, 0x3f, -117, 0x1b, 0x73, 0x3f, 0, 0, 0, 0, -80, -112, 0x75, 0x3f, -17, -1, 0x7f, 0x3f, 0x5f, 0x60, 0x6a, 0x3f, 0, 0, 0, 0, -108, -35, 0x6c, 0x3f, -50, -1, 0x7f, 0x3f, -71, 0x16, 0x69, 0x3f, 0, 0, 0, 0, -14, 6, 0x6c, 0x3f, 0, 0, -128, 0x3f, 2, -72, 0x65, 0x3f, 0, 0, 0, 0, -44, 0x27, 0x69, 0x3f, 0, 0, -128, 0x3f, 0x47, -80, 0x65, 0x3f, 0, 0, 0, 0, 0x5d, -31, 0x69, 0x3f, -17, -1, 0x7f, 0x3f, 0x4e, 0xc, 0x69, 0x3f, 0, 0, 0, 0, 0x65, -87, 0x6d, 0x3f, -50, -1, 0x7f, 0x3f, -4, -29, 0x69, 0x3f, 0, 0, 0, 0, -103, -96, 0x6e, 0x3f, -117, -1, 0x7f, 0x3f, -16, 0x6d, 0x72, 0x3f, 0, 0, 0, 0, 0x44, -65, 0x76, 0x3f, 0x47, -1, 0x7f, 0x3f, -31, -76, 0x60, 0x3c, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x45, -14, 0x69, 0x3f, 0, 0, 0, 0, -38, -86, 0x6c, 0x3f, 0, 0, -128, 0x3f, -124, -39, -124, 0x3c, 0, 0, 0, 0, 0, 0, 0, 0, -80, -2, 0x7f, 0x3f, -28, -98, 0x72, 0x3f, 0, 0, 0, 0, -118, 5, 0x7e, 0x3f, 0, 0, -128, 0x3f, -114, 0x5c, 0x77, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0x58, -2, 0x78, 0x3f, -9, 6, 0x7b, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, -52, -21, 0x78, 0x3f, -13, 0x1d, 0x7c, 0x3f, 0, 0, 0, 0, -127, 0x41, 0x7e, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -65, 0x7a, 0x51, 0x67, -65, 0x6d, 0x55, -38, 0x3e, -76, 0x1c, 0x28, 0x3d, 0x7a, 0x51, 0x67, -65, 0x6d, 0x55, -38, 0x3e, -76, 0x1c, 0x28, 0x3d, 0x10, 0x3e, 0x58, -65, 0x36, -111, 5, 0x3f, -100, -119, 0x29, 0x3d, 0x10, 0x3e, 0x58, -65, 0x36, -111, 5, 0x3f, -100, -119, 0x29, 0x3d, -105, -112, 0x33, -65, 0x64, -112, 0x33, 0x3f, -19, -125, 0x2c, 0x3d, -105, -112, 0x33, -65, 0x64, -112, 0x33, 0x3f, -19, -125, 0x2c, 0x3d, -101, -115, 5, -65, 0x2e, 0x3b, 0x58, 0x3f, 2, -70, 0x2f, 0x3d, -101, -115, 5, -65, 0x2e, 0x3b, 0x58, 0x3f, 2, -70, 0x2f, 0x3d, 0x55, -5, -84, -66, -110, 0x7a, 0x6f, 0x3f, -34, 0, 0x33, 0x3d, 0x55, -5, -84, -66, -110, 0x7a, 0x6f, 0x3f, -34, 0, 0x33, 0x3d, -25, 0x38, 0x27, -66, 0x6e, 0x50, 0x7b, 0x3f, 0x31, -45, 0x36, 0x3d, -25, 0x38, 0x27, -66, 0x6e, 0x50, 0x7b, 0x3f, 0x31, -45, 0x36, 0x3d, -117, -31, -22, -74, 0x3c, -9, 0x7e, 0x3f, -56, -104, 0x3b, 0x3d, -117, -31, -22, -74, 0x3c, -9, 0x7e, 0x3f, -56, -104, 0x3b, 0x3d, -58, 0x30, 0x27, 0x3e, -111, 0x48, 0x7b, 0x3f, -36, -41, 0x41, 0x3d, -58, 0x30, 0x27, 0x3e, -111, 0x48, 0x7b, 0x3f, -36, -41, 0x41, 0x3d, -111, -19, -84, 0x3e, 0x3c, 0x6b, 0x6f, 0x3f, 0x20, -75, 0x49, 0x3d, -111, -19, -84, 0x3e, 0x3c, 0x6b, 0x6f, 0x3f, 0x20, -75, 0x49, 0x3d, -40, 0x7f, 5, 0x3f, 0x70, 0x24, 0x58, 0x3f, -14, -74, 0x52, 0x3d, -40, 0x7f, 5, 0x3f, 0x70, 0x24, 0x58, 0x3f, -14, -74, 0x52, 0x3d, 0x68, 0x78, 0x33, 0x3f, -5, 0x75, 0x33, 0x3f, 0x29, -106, 0x5b, 0x3d, 0x68, 0x78, 0x33, 0x3f, -5, 0x75, 0x33, 0x3f, 0x29, -106, 0x5b, 0x3d, -58, 0x18, 0x58, 0x3f, -102, 0x7a, 5, 0x3f, 0x68, 0x5a, 0x62, 0x3d, -58, 0x18, 0x58, 0x3f, -102, 0x7a, 5, 0x3f, 0x68, 0x5a, 0x62, 0x3d, 0x4a, 0x26, 0x67, 0x3f, -95, 0x2e, -38, 0x3e, -99, 0x11, 0x65, 0x3d, 0x4a, 0x26, 0x67, 0x3f, -95, 0x2e, -38, 0x3e, -99, 0x11, 0x65, 0x3d, -125, 0x16, 0x5e, -65, -48, -104, -47, 0x3e, 0x1a, -92, -112, 0x3e, -125, 0x16, 0x5e, -65, -48, -104, -47, 0x3e, 0x1a, -92, -112, 0x3e, -59, -111, 0x4f, -65, -87, 0x32, 0, 0x3f, -30, 5, -111, 0x3e, -59, -111, 0x4f, -65, -87, 0x32, 0, 0x3f, -30, 5, -111, 0x3e, 0x47, 0x58, 0x2c, -65, -68, 0x58, 0x2c, 0x3f, -8, 0x32, -111, 0x3e, 0x47, 0x58, 0x2c, -65, -68, 0x58, 0x2c, 0x3f, -8, 0x32, -111, 0x3e, -26, 0x3f, 0, -65, -21, -85, 0x4f, 0x3f, 0x4b, 0x3c, -112, 0x3e, -26, 0x3f, 0, -65, -21, -85, 0x4f, 0x3f, 0x4b, 0x3c, -112, 0x3e, -61, 0x45, -90, -66, -126, 0x39, 0x66, 0x3f, -61, -98, -114, 0x3e, -61, 0x45, -90, -66, -126, 0x39, 0x66, 0x3f, -61, -98, -114, 0x3e, -50, -29, 0x20, -66, 0x78, -45, 0x71, 0x3f, 0x48, 0x15, -115, 0x3e, -50, -29, 0x20, -66, 0x78, -45, 0x71, 0x3f, 0x48, 0x15, -115, 0x3e, 0x39, -101, -114, 0x37, 0x20, 0x7f, 0x75, 0x3f, 0x27, 0x12, -116, 0x3e, 0x39, -101, -114, 0x37, 0x20, 0x7f, 0x75, 0x3f, 0x27, 0x12, -116, 0x3e, 5, 0x19, 0x21, 0x3e, -41, 0x12, 0x72, 0x3f, -49, 0x4d, -117, 0x3e, 5, 0x19, 0x21, 0x3e, -41, 0x12, 0x72, 0x3f, -49, 0x4d, -117, 0x3e, 0x29, -105, -90, 0x3e, 0, -88, 0x66, 0x3f, -65, 0x64, -117, 0x3e, 0x29, -105, -90, 0x3e, 0, -88, 0x66, 0x3f, -65, 0x64, -117, 0x3e, 0x54, -117, 0, 0x3f, -39, 0x23, 0x50, 0x3f, -48, 0x63, -116, 0x3e, 0x54, -117, 0, 0x3f, -39, 0x23, 0x50, 0x3f, -48, 0x63, -116, 0x3e, -9, -54, 0x2c, 0x3f, -34, -56, 0x2c, 0x3f, 0x4d, -38, -116, 0x3e, -9, -54, 0x2c, 0x3f, -34, -56, 0x2c, 0x3f, 0x4d, -38, -116, 0x3e, 0x5f, 0x24, 0x50, 0x3f, -121, -117, 0, 0x3f, 0x1b, 0x62, -116, 0x3e, 0x5f, 0x24, 0x50, 0x3f, -121, -117, 0, 0x3f, 0x1b, 0x62, -116, 0x3e, 0x42, -76, 0x5e, 0x3f, -61, 0x2e, -46, 0x3e, -119, -20, -117, 0x3e, 0x42, -76, 0x5e, 0x3f, -61, 0x2e, -46, 0x3e, -119, -20, -117, 0x3e, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f};
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float FUN_001f9b48(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float FUN_L00_0025be00(float *p, float *v, float a, float b, float c, float d);
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int FUN_001fa728(char *, float);
extern int FUN_0022da68(int, int, int);
extern int random_integer_below_c(int) __asm__("FUN_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern unsigned char *FUN_L13_002e1c50(char *position, int moby_class);
extern void FUN_L00_0025f090(void *, void *, int, float, float);
extern void FUN_L00_0025f730(void *a, float x);
extern void FUN_L11_00319670(void *);
extern void FUN_L11_0031a1c0(void *);
extern void FUN_L11_0031a438(void *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern void vu_euler_rotation_basis(void *, void *) __asm__("func_001FA030");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

extern float D_L17_001623E8 __attribute__((sda));
void FUN_L17_002f40d8(struct Moby *moby) {
    float s0[4];
    float s10[4];
    float s20[4];
    float s30[4];
    float m40[4];
    float m50[4];
    float m60[4];
    float s70[4];
    float s80[4];
    float s90[4];
    float sA0[4];
    float sB0[4];
    float c0;
    char *d = (char *)moby->pvars;
    float f;
    float len;
    char *p;

    FUN_L11_0031a1c0(moby);
    switch (moby->state) {
    case 0:
        moby->draw_dist = 0xFF;
        moby->update_dist = 0xFF;
        if (*(int *)(d + 0x88) == 0) {
            moby->state = 1;
            *(float *)(d + 0x6C) = randf_sym(0.3f, 0.5f);
            *(float *)(d + 0x70) = random_angle_radians();
            *(float *)(d + 0x74) = randf_sym(60.0f, 120.0f) * DEG_TO_RAD * frame_time;
            *(float *)(d + 0x78) = random_float_between_alt(8.0f, 12.0f);
        } else {
            moby->state = 4;
            moby->collision = 0;
            moby->flags |= 1;
        }
        break;
    case 1:
    case 2:
        FUN_L11_00319670(moby);
        *(float *)(d + 0x68) += *(float *)(d + 0x6C);
        if (*(float *)(d + 0x68) > 1.0f) {
            *(float *)(d + 0x68) -= 1.0f;
            *(int *)(d + 0x64) =
                (*(int *)(d + 0x64) + 1) % **(int **)(D_L17_001B0DB0 + *(int *)(d + 0x60) * 4);
        } else if (*(float *)(d + 0x68) < 0.0f) {
            *(float *)(d + 0x68) += 1.0f;
            *(int *)(d + 0x64) =
                (*(int *)(d + 0x64) + **(int **)(D_L17_001B0DB0 + *(int *)(d + 0x60) * 4) - 1) %
                **(int **)(D_L17_001B0DB0 + *(int *)(d + 0x60) * 4);
        }
        fast_add_rotations(*(float *)(d + 0x70), *(float *)(d + 0x74));
        FUN_L11_0031a438(moby);
        if (moby->state == 2) {
            FUN_L00_0025f090(moby, &moby->pos, -1, 2.0f, 13.0f);
            if (random_integer_below_c(2)) {
                FUN_L13_002e1c50((char *)&moby->pos, 0xE0);
            } else {
                FUN_L13_002e1c50((char *)&moby->pos, 0xE4);
            }
            FUN_0022da68(1, 0, (int)moby);
            moby->state = 3;
            moby->collision = 0;
            moby->flags |= 1;
        } else if (moby->visible == 0) {
            if (FUN_001f9b48(&moby->pos, &hero.motion.pos) < 32.0f) {
                moby->state = 3;
                moby->collision = 0;
                moby->flags |= 1;
            }
        }
        break;
    case 3:
        qcopy(s0, &moby->pos);
        s0[3] = 3.0f;
        if (FUN_001fa728((char *)&moby->pos, 512.0f) == -1) {
            if (FUN_001f9b48(&moby->pos, &hero.motion.pos) > 32.0f) {
                *(int *)(d + 0x84) += 1;
                if (scale_game_frames(60) < *(int *)(d + 0x84)) {
                    moby->state = 1;
                    if (moby->prev_seq) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                    }
                    moby->flags &= 0xFFFE;
                    moby->collision = moby->pclass->collision;
                }
            } else {
                *(int *)(d + 0x84) = 0;
            }
        } else {
            *(int *)(d + 0x84) = 0;
        }
        break;
    case 4:
        break;
    case 5:
        c0 = 0.0f;
        qcopy(s30, D_L17_001676D0);
        s30[0] = 0.0f;
        if (s30[1] > 0.2617994f) {
            s30[1] = 0.2617994f;
        } else if (s30[1] < -0.2617994f) {
            s30[1] = -0.2617994f;
        }
        vu_euler_rotation_basis(m40, s30);
        subtract_vector_xyz(s20, &hero.motion.pos, d + 0xB0);
        qcopy(d + 0xB0, &hero.motion.pos);
        add_vector_xyz(&moby->pos, &moby->pos, s20);
        f = fast_add_rotations(0.7853982f, (float)*(int *)(d + 0x88) * 1.5707964f);
        normalize_vector_xyz(s0, m50, fast_sin(f) * 8.0f);
        add_vector_xyz(s10, D_L17_001676C0, s0);
        normalize_vector_xyz(s0, m60, fast_cos(f) * 8.0f);
        add_vector_xyz(s10, s10, s0);
        normalize_vector_xyz(s0, m40, 20.0f);
        add_vector_xyz(s10, s10, s0);
        subtract_vector_xyz(s0, s10, &moby->pos);
        len = vector_length_xyz(s0);
        if (*(char *)&hero.ship_flags & 2) {
            advance_accelerated_scalar(&c0, (float *)(d + 0x6C), len, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, frame_time);
        } else if (moby->command) {
            advance_accelerated_scalar(&c0, (float *)(d + 0x6C), len, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, frame_time * 15.0f);
        } else {
            advance_accelerated_scalar(&c0, (float *)(d + 0x6C), len, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, frame_time * 5.0f);
        }
        FUN_L00_0025f730(s0, *(float *)(d + 0x6C));
        add_vector_xyz(&moby->pos.x, &moby->pos.x, s0);
        if (moby->command) {
            if (moby->visible) {
                *(int *)(d + 0x98) += 1;
            }
            if (FUN_001f9b48(s10, &moby->pos.x) < 2.0f) {
                moby->command = 0;
            } else if (scale_game_frames(120) < *(int *)(d + 0x98)) {
                moby->command = 0;
            }
        } else {
            float *tgt = (float *)(&hero.motion.pos);
            float *yaw = &moby->rot.z;
            float *pitch = &moby->rot.y;
            float ang;
            subtract_vector_xyz(s70, tgt, &moby->pos.x);
            ang = fast_arc_tan(s70[0], s70[1]);
            FUN_L00_0025be00(yaw, (float *)(d + 0x8C), ang, frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f, frame_time * 12.566371f);
            ang = -fast_arc_tan(vector_length_xy(s70), s70[2]);
            FUN_L00_0025be00(pitch, (float *)(d + 0x90), ang, frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f, frame_time * 12.566371f);
            if (D_L17_0015F5CC % 10 == 0) {
                FUN_0022da68(2, 0, (int)moby);
                approach_value((float *)(d + 0x94), 0.0f, 0.25f);
                normalize_vector_xyz(sB0, s20, *(float *)(d + 0x94));
                add_vector_xyz(sB0, sB0, tgt);
                if (D_L17_0015F5CC % 20 == 0) {
                    transform_vector_by_basis(sA0, D_L17_001DE110, &moby->rot_row0);
                } else {
                    transform_vector_by_basis(sA0, D_L17_001DE120, &moby->rot_row0);
                }
                add_vector_xyz(sA0, sA0, &moby->pos.x);
                subtract_vector_xyz(s80, sB0, sA0);
                normalize_vector_xyz(s90, s80, D_L17_001623E8 * frame_time);
                add_vector_xyz(s90, s90, s20);
                p = FUN_L11_00308f48((int)moby, s90, sA0, 128.0f, -1.0f, 10.0f);
                if (p) {
                    subtract_vector_xyz(s90, s90, s20);
                    *(float *)(p + 0x44) = -fast_arc_tan(vector_length_xy(s90), s90[2]);
                    *(float *)(p + 0x48) = fast_arc_tan(s90[0], s90[1]);
                }
            }
            if (moby->visible == 0) {
                moby->state = 4;
                moby->collision = 0;
                moby->flags |= 1;
            }
        }
        break;
    case 6:
        subtract_vector_xyz(s0, &hero.motion.pos, d + 0xB0);
        qcopy(d + 0xB0, &hero.motion.pos);
        add_vector_xyz(&moby->pos, &moby->pos, s0);
        FUN_L00_0025f090(moby, &moby->pos, -1, 2.0f, 13.0f);
        if (random_integer_below_c(2)) {
            FUN_L13_002e1c50((char *)&moby->pos, 0xE0);
        } else {
            FUN_L13_002e1c50((char *)&moby->pos, 0xE4);
        }
        FUN_0022da68(1, 0, (int)moby);
        moby->state = 4;
        moby->collision = 0;
        moby->flags |= 1;
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Finds the listed moby of type 0x733 in state 4 and launches it along a rotated vector. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6AF8), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern char D_L17_001676C0[];
extern float FUN_001f9dc8(float);
extern float FUN_001f9de0(float);
extern float FUN_001fa580(float, float);
extern int FUN_001f96f8(int);
extern void FUN_001f9a10(void *, void *, void *);
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

char *FUN_L17_002f4fb8(int list) {
    short *s = D_L17_001AC140[list];
    float A[4], B[4], C[4], D[4], E[4], F[4];
    char *m;
    char *d;
    int idx;
    do {
        idx = (*s & 0x7FFF) << 8;
        if (*(short *)(0xA6 + idx + D_L17_0015FFD8) != 0x733)
            continue;
        if (*(unsigned char *)(0x20 + idx + D_L17_0015FFD8) != 4)
            continue;
        if (random_integer_below(10))
            continue;
        m = D_L17_0015FFD8 + idx;
        d = *(char **)(m + 0x78);
        qcopy(C, D_L17_001676D0);
        C[0] = 0;
        if (C[1] > 0.17453292f)
            C[1] = 0.17453292f;
        else if (C[1] < -0.17453292f)
            C[1] = -0.17453292f;
        euler_rotation_basis(D, C);
        {
            float f = FUN_001fa580(0.7853982f, (float)*(int *)(d + 0x88) * 1.5707964f);
            fast_vec_normalize(A, E, FUN_001f9de0(f) * 12.0f);
            FUN_001f9a10(B, D_L17_001676C0, A);
            fast_vec_normalize(A, F, FUN_001f9dc8(f) * 12.0f);
        }
        FUN_001f9a10(B, B, A);
        fast_vec_normalize(A, D, 6.0f);
        FUN_001f9a10(B, B, A);
        qcopy(d + 0xB0, D_0013E533 + 0xE9D);
        qcopy(m + 0x10, B);
        m[0x20] = 5;
        m[0xBC] = 1;
        *(float *)(d + 0x6C) = frame_time * 20.0f;
        *(float *)(d + 0x94) = 5.0f;
        if (*(unsigned char *)(m + 0x53))
            blend_moby_animation(m, 0, 0, FUN_001f96f8(10));
        *(unsigned short *)(m + 0x34) &= 0xFFFE;
        *(int *)(m + 0x94) = *(int *)(*(char **)(m + 0x24) + 0x10);
        return m;
    } while (*s++ >= 0);
    return 0;
}
/* UpdateMoby 1873: wait for a target, then play two effects and delete itself. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6DA0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int FUN_0022da68(int, int, int);
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00263e30(void *, int, int, int, int, int, int);
extern void FUN_L01_002787a0(void *);
extern void FUN_L01_00278e20(void *, int);
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void FUN_L17_002f5260(struct Moby *m) {
    int flag = 0;
    char *p = find_moby_hit(m, 0x10000, 0);
    switch (m->state) {
    case 0:
        m->state = 1;
        break;
    case 1:
        if (p && *(float *)(p + 0x2C) > 0.0f)
            flag = 1;
        if (flag)
            m->state = 2;
        break;
    case 2:
        FUN_0022da68(0, 0, (int)m);
        FUN_L00_00257470(m, 0, -1);
        FUN_L01_002787a0(m);
        FUN_L00_00263e30(m, 0x753, 1, 0x753, 1, 11, 2);
        FUN_L00_00263e30(m, 0x756, 1, 0x756, 1, 1, 2);
        FUN_L01_00278e20(m, 0x752);
        mark_moby_for_removal(m);
        break;
    }
}
/* Update moby 1876: waits for a trigger, then plays an effect and finishes. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6ED0), where it is exact; names translated to the US level program. */

extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
extern void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9, float *v10,
                              float *v11, float scale, float *v12);
void mark_moby_for_removal_c(void *obj) __asm__("FUN_0020c828");

extern float D_L17_0015F580_macro[] __asm__("D_L17_0015F580") __attribute__((section(".sdata")));
void FUN_L17_002f5390(struct Moby *m) {
    int flag = 0;
    char *r = find_moby_hit(m, 0x10000, 0);
    switch (m->state) {
    case 0:
        m->state = 1;
        break;
    case 1:
        if (r) {
            if (*(float *)(r + 0x2C) > 0.0f)
                flag = 1;
        }
        if (flag)
            m->state = 2;
        break;
    case 2:
        allocate_voice_for_target_entry(0, 0, (int)m);
        FUN_L00_00257470(m, 0, -1);
        FUN_L01_002787a0(m);
        FUN_L00_00263fd8((char *)m, 0x753, &m->pos.x, &m->rot, 0, 0, D_L17_0015F580_macro,
                         D_L17_0015F580_macro, 0.0f, D_L17_0015F580_macro);
        FUN_L00_00263e30(m, 0x756, 1, 0x756, 1, 1, 2);
        FUN_L01_00278e20(m, 0x755);
        mark_moby_for_removal_c(m);
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Projects each direction pair into 2D texture coordinates (reflection mapping). */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F7018), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z;
} V3;

typedef struct {
    V3 v;
    float w;
} V4;

extern char D_L17_001676C0[];
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void FUN_001f9a28(void *, void *, void *);
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void FUN_L17_002f54d8(int n, V3 *b, V3 *a, int unused, float *out) {
    V4 v0;
    V4 p;
    V4 r;
    V4 q;
    int i;
    for (i = 0; i < n; i++) {
        float d;
        float len;
        p.w = 1.0f;
        p.v.x = a[i].x;
        p.v.y = a[i].y;
        p.v.z = a[i].z;
        FUN_001f9a28(&v0, &p, D_L17_001676C0);
        fast_vec_normalize(&v0, &v0, 1.0f);
        q.w = 1.0f;
        q.v.x = b[i].x;
        q.v.y = b[i].y;
        q.v.z = b[i].z;
        fast_vec_normalize(&q, &q, 1.0f);
        d = fast_vec_dot(&q, &v0);
        fast_vec_scale(&r, &q, d + d);
        FUN_001f9a28(&r, &v0, &r);
        fast_vec_normalize(&r, &r, 1.0f);
        r.v.z = r.v.z + 1.0f;
        len = fast_vec_length(&r);
        len = len + len;
        out[i * 2] = (r.v.x / len + 0.5f) * 4.0f;
        out[i * 2 + 1] = (r.v.y / len + 0.5f) * 4.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F71D0), where it is exact; names translated to the US level program. */

char D_L17_001DE8A0[3584] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x50, -51, 3, 0x44, 0x66, -7, 0x1a, 0x44, -2, 0x7f, 0x18, 0x43, 0x30, 2, 3, 0x44, 0x66, -7, 0x1a, 0x44, 0x28, -111, 0x20, 0x43, -59, -62, 3, 0x44, 0x19, 0x62, 0x1b, 0x44, -2, 0x7f, 0x18, 0x43, -117, -5, 2, 0x44, 0x78, 0x3a, 0x1b, 0x44, 0x28, -111, 0x20, 0x43, -61, 0x5c, 3, 0x44, 0x5e, -78, 0x1c, 0x44, -2, 0x7f, 0x18, 0x43, 0x18, -95, 2, 0x44, -93, 0x64, 0x1c, 0x44, 0x28, -111, 0x20, 0x43, 0x1c, -73, 2, 0x44, 0x48, -24, 0x1d, 0x44, -2, 0x7f, 0x18, 0x43, 0x37, 0xe, 2, 0x44, 0x6e, 0x77, 0x1d, 0x44, 0x28, -111, 0x20, 0x43, 0x2f, -40, 1, 0x44, -20, -9, 0x1e, 0x44, -2, 0x7f, 0x18, 0x43, -115, 0x48, 1, 0x44, 0x49, 0x68, 0x1e, 0x44, 0x28, -111, 0x20, 0x43, -117, -56, 0, 0x44, -39, -42, 0x1f, 0x44, -2, 0x7f, 0x18, 0x43, -80, 0x57, 0, 0x44, -12, 0x2d, 0x1f, 0x44, 0x28, -111, 0x20, 0x43, 0x43, 0x25, -1, 0x43, -128, 0x7c, 0x20, 0x44, -2, 0x7f, 0x18, 0x43, -53, -119, -2, 0x43, -42, -64, 0x1f, 0x44, 0x28, -111, 0x20, 0x43, -73, -124, -4, 0x43, -126, -30, 0x20, 0x44, -2, 0x7f, 0x18, 0x43, 0x75, 0x35, -4, 0x43, 0x49, 0x1b, 0x20, 0x44, 0x28, -111, 0x20, 0x43, 0x4a, -55, -7, 0x43, -12, 4, 0x21, 0x44, -2, 0x7f, 0x18, 0x43, 0x4a, -55, -7, 0x43, -45, 0x39, 0x20, 0x44, 0x28, -111, 0x20, 0x43, -35, 0xd, -9, 0x43, -126, -30, 0x20, 0x44, -2, 0x7f, 0x18, 0x43, 0x1e, 0x5d, -9, 0x43, 0x49, 0x1b, 0x20, 0x44, 0x28, -111, 0x20, 0x43, 0x52, 0x6d, -12, 0x43, -128, 0x7c, 0x20, 0x44, -2, 0x7f, 0x18, 0x43, -55, 8, -11, 0x43, -42, -64, 0x1f, 0x44, 0x28, -111, 0x20, 0x43, 0x7e, 1, -14, 0x43, -38, -42, 0x1f, 0x44, -2, 0x7f, 0x18, 0x43, 0x33, -29, -14, 0x43, -11, 0x2d, 0x1f, 0x44, 0x28, -111, 0x20, 0x43, 0x37, -30, -17, 0x43, -20, -9, 0x1e, 0x44, -2, 0x7f, 0x18, 0x43, 0x7b, 1, -15, 0x43, 0x4a, 0x68, 0x1e, 0x44, 0x28, -111, 0x20, 0x43, 0x5c, 0x24, -18, 0x43, 0x48, -24, 0x1d, 0x44, -2, 0x7f, 0x18, 0x43, 0x25, 0x76, -17, 0x43, 0x6e, 0x77, 0x1d, 0x44, 0x28, -111, 0x20, 0x43, 0xe, -39, -20, 0x43, 0x5f, -78, 0x1c, 0x44, -2, 0x7f, 0x18, 0x43, 0x62, 0x50, -18, 0x43, -93, 0x64, 0x1c, 0x44, 0x28, -111, 0x20, 0x43, 0xa, 0xd, -20, 0x43, 0x19, 0x62, 0x1b, 0x44, -2, 0x7f, 0x18, 0x43, 0x7d, -101, -19, 0x43, 0x78, 0x3a, 0x1b, 0x44, 0x28, -111, 0x20, 0x43, 0x27, -56, -21, 0x43, 0x62, 4, 0x1a, 0x44, -2, 0x7f, 0x18, 0x43, 0x69, 0x5e, -19, 0x43, 0x62, 4, 0x1a, 0x44, 0x28, -111, 0x20, 0x43, 0xa, 0xd, -20, 0x43, -84, -90, 0x18, 0x44, -2, 0x7f, 0x18, 0x43, 0x7d, -101, -19, 0x43, 0x4d, -50, 0x18, 0x44, 0x28, -111, 0x20, 0x43, 0xe, -39, -20, 0x43, 0x66, 0x56, 0x17, 0x44, -2, 0x7f, 0x18, 0x43, 0x62, 0x50, -18, 0x43, 0x22, -92, 0x17, 0x44, 0x28, -111, 0x20, 0x43, 0x5b, 0x24, -18, 0x43, 0x7d, 0x20, 0x16, 0x44, -2, 0x7f, 0x18, 0x43, 0x25, 0x76, -17, 0x43, 0x57, -111, 0x16, 0x44, 0x28, -111, 0x20, 0x43, 0x37, -30, -17, 0x43, -39, 0x10, 0x15, 0x44, -2, 0x7f, 0x18, 0x43, 0x7b, 1, -15, 0x43, 0x7b, -96, 0x15, 0x44, 0x28, -111, 0x20, 0x43, 0x7e, 1, -14, 0x43, -21, 0x31, 0x14, 0x44, -2, 0x7f, 0x18, 0x43, 0x33, -29, -14, 0x43, -48, -38, 0x14, 0x44, 0x28, -111, 0x20, 0x43, 0x50, 0x6d, -12, 0x43, 0x44, -116, 0x13, 0x44, -2, 0x7f, 0x18, 0x43, -55, 8, -11, 0x43, 0x20, -110, 0x14, 0x44, 0x34, -76, 0x22, 0x43, -35, 0xd, -9, 0x43, 0x42, 0x26, 0x13, 0x44, -2, 0x7f, 0x18, 0x43, 0x1e, 0x5d, -9, 0x43, -83, 0x37, 0x14, 0x44, -99, 0x5b, 0x23, 0x43, 0x4a, -55, -7, 0x43, -48, 3, 0x13, 0x44, -2, 0x7f, 0x18, 0x43, 0x4a, -55, -7, 0x43, 0x22, 0x19, 0x14, 0x44, -99, 0x5b, 0x23, 0x43, -73, -124, -4, 0x43, 0x42, 0x26, 0x13, 0x44, -2, 0x7f, 0x18, 0x43, 0x75, 0x35, -4, 0x43, -83, 0x37, 0x14, 0x44, -99, 0x5b, 0x23, 0x43, 0x43, 0x25, -1, 0x43, 0x44, -116, 0x13, 0x44, -2, 0x7f, 0x18, 0x43, -53, -119, -2, 0x43, 0x20, -110, 0x14, 0x44, 0x34, -76, 0x22, 0x43, -117, -56, 0, 0x44, -21, 0x31, 0x14, 0x44, -2, 0x7f, 0x18, 0x43, -79, 0x57, 0, 0x44, -48, -38, 0x14, 0x44, 0x28, -111, 0x20, 0x43, 0x2f, -40, 1, 0x44, -39, 0x10, 0x15, 0x44, -2, 0x7f, 0x18, 0x43, -115, 0x48, 1, 0x44, 0x7b, -96, 0x15, 0x44, 0x28, -111, 0x20, 0x43, 0x1d, -73, 2, 0x44, 0x7d, 0x20, 0x16, 0x44, -2, 0x7f, 0x18, 0x43, 0x37, 0xe, 2, 0x44, 0x56, -111, 0x16, 0x44, 0x28, -111, 0x20, 0x43, -61, 0x5c, 3, 0x44, 0x66, 0x56, 0x17, 0x44, -2, 0x7f, 0x18, 0x43, 0x19, -95, 2, 0x44, 0x22, -92, 0x17, 0x44, 0x28, -111, 0x20, 0x43, -59, -62, 3, 0x44, -84, -90, 0x18, 0x44, -2, 0x7f, 0x18, 0x43, -117, -5, 2, 0x44, 0x4d, -50, 0x18, 0x44, 0x28, -111, 0x20, 0x43, 0x50, -51, 3, 0x44, 0x25, -5, 0x18, 0x44, -2, 0x7f, 0x18, 0x43, 0x30, 2, 3, 0x44, 0x25, -5, 0x18, 0x44, 0x28, -111, 0x20, 0x43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0x28, 0x2c, 0x6d, 0x3f, -32, 0xf, -65, 0x3d, -6, -76, -70, 0x3e, 0x28, 0x2c, 0x6d, 0x3f, -32, 0xf, -65, 0x3d, -6, -76, -70, 0x3e, -96, -92, 0x68, 0x3f, -57, 0x29, 0x3a, 0x3e, -123, -80, -70, 0x3e, -96, -92, 0x68, 0x3f, -57, 0x29, 0x3a, 0x3e, -123, -80, -70, 0x3e, 0x30, 0x2c, 0x5b, 0x3f, -51, -111, -75, 0x3e, -51, -85, -70, 0x3e, 0x30, 0x2c, 0x5b, 0x3f, -51, -111, -75, 0x3e, -51, -85, -70, 0x3e, 0x46, 0x40, 0x45, 0x3f, 0x29, -52, 3, 0x3f, -17, -85, -70, 0x3e, 0x46, 0x40, 0x45, 0x3f, 0x29, -52, 3, 0x3f, -17, -85, -70, 0x3e, 0x51, -65, 0x27, 0x3f, 0x51, -65, 0x27, 0x3f, 0x32, -84, -70, 0x3e, 0x51, -65, 0x27, 0x3f, 0x51, -65, 0x27, 0x3f, 0x32, -84, -70, 0x3e, 0x3a, -52, 3, 0x3f, 0x35, 0x40, 0x45, 0x3f, -51, -85, -70, 0x3e, 0x3a, -52, 3, 0x3f, 0x35, 0x40, 0x45, 0x3f, -51, -85, -70, 0x3e, -51, -111, -75, 0x3e, 0x41, 0x2c, 0x5b, 0x3f, 0x69, -85, -70, 0x3e, -51, -111, -75, 0x3e, 0x41, 0x2c, 0x5b, 0x3f, 0x69, -85, -70, 0x3e, 4, 0x21, 0x39, 0x3e, 0x3b, -84, 0x68, 0x3f, -84, -85, -70, 0x3e, 4, 0x21, 0x39, 0x3e, 0x3b, -84, 0x68, 0x3f, -84, -85, -70, 0x3e, 0, 0, 0, -128, 0x2a, 0x3b, 0x6d, 0x3f, -118, -85, -70, 0x3e, 0, 0, 0, -128, 0x2a, 0x3b, 0x6d, 0x3f, -118, -85, -70, 0x3e, 0x47, 0x21, 0x39, -66, 0x3b, -84, 0x68, 0x3f, 0x47, -85, -70, 0x3e, 0x47, 0x21, 0x39, -66, 0x3b, -84, 0x68, 0x3f, 0x47, -85, -70, 0x3e, 0x47, -111, -75, -66, 0x62, 0x2c, 0x5b, 0x3f, -118, -85, -70, 0x3e, 0x47, -111, -75, -66, 0x62, 0x2c, 0x5b, 0x3f, -118, -85, -70, 0x3e, 0x3a, -52, 3, -65, 0x35, 0x40, 0x45, 0x3f, -51, -85, -70, 0x3e, 0x3a, -52, 3, -65, 0x35, 0x40, 0x45, 0x3f, -51, -85, -70, 0x3e, -108, -65, 0x27, -65, 0x40, -65, 0x27, 0x3f, -118, -85, -70, 0x3e, -108, -65, 0x27, -65, 0x40, -65, 0x27, 0x3f, -118, -85, -70, 0x3e, 0x25, 0x40, 0x45, -65, 0x7d, -52, 3, 0x3f, 0x69, -85, -70, 0x3e, 0x25, 0x40, 0x45, -65, 0x7d, -52, 3, 0x3f, 0x69, -85, -70, 0x3e, 0x30, 0x2c, 0x5b, -65, -17, -111, -75, 0x3e, -118, -85, -70, 0x3e, 0x30, 0x2c, 0x5b, -65, -17, -111, -75, 0x3e, -118, -85, -70, 0x3e, 0x2a, -84, 0x68, -65, 0x3a, 0x20, 0x39, 0x3e, -51, -85, -70, 0x3e, 0x2a, -84, 0x68, -65, 0x3a, 0x20, 0x39, 0x3e, -51, -85, -70, 0x3e, 0x2a, 0x3b, 0x6d, -65, -67, 0x37, -122, -75, -51, -85, -70, 0x3e, 0x2a, 0x3b, 0x6d, -65, -67, 0x37, -122, -75, -51, -85, -70, 0x3e, 0x3b, -84, 0x68, -65, 0x3a, 0x20, 0x39, -66, -118, -85, -70, 0x3e, 0x3b, -84, 0x68, -65, 0x3a, 0x20, 0x39, -66, -118, -85, -70, 0x3e, 0x41, 0x2c, 0x5b, -65, -84, -111, -75, -66, -118, -85, -70, 0x3e, 0x41, 0x2c, 0x5b, -65, -84, -111, -75, -66, -118, -85, -70, 0x3e, 0x25, 0x40, 0x45, -65, 0x6c, -52, 3, -65, -84, -85, -70, 0x3e, 0x25, 0x40, 0x45, -65, 0x6c, -52, 3, -65, -84, -85, -70, 0x3e, -125, -65, 0x27, -65, 0x51, -65, 0x27, -65, -51, -85, -70, 0x3e, -125, -65, 0x27, -65, 0x51, -65, 0x27, -65, -51, -85, -70, 0x3e, -126, 0x72, 3, -65, 0x63, -105, 0x44, -65, -32, 0x65, -66, 0x3e, -126, 0x72, 3, -65, 0x63, -105, 0x44, -65, -32, 0x65, -66, 0x3e, 0x67, -44, -76, -66, 0x7e, 0x74, 0x5a, -65, -65, -74, -66, 0x3e, 0x67, -44, -76, -66, 0x7e, 0x74, 0x5a, -65, -65, -74, -66, 0x3e, 0x55, -9, 0x38, -66, -27, 0x62, 0x68, -65, -60, 0x24, -68, 0x3e, 0x55, -9, 0x38, -66, -27, 0x62, 0x68, -65, -60, 0x24, -68, 0x3e, 0, 0, 0, -128, -25, -58, 0x6c, -65, -4, -5, -68, 0x3e, 0, 0, 0, -128, -25, -58, 0x6c, -65, -4, -5, -68, 0x3e, 0x44, -62, 0x37, 0x3e, -11, 0x63, 0x67, -65, -128, 0x46, -63, 0x3e, 0x44, -62, 0x37, 0x3e, -11, 0x63, 0x67, -65, -128, 0x46, -63, 0x3e, -10, -19, -76, 0x3e, 0x1d, 0x1e, 0x5a, -65, 0x47, 0x1e, -64, 0x3e, -10, -19, -76, 0x3e, 0x1d, 0x1e, 0x5a, -65, 0x47, 0x1e, -64, 0x3e, 0x7d, -52, 3, 0x3f, 0x14, 0x40, 0x45, -65, -84, -85, -70, 0x3e, 0x7d, -52, 3, 0x3f, 0x14, 0x40, 0x45, -65, -84, -85, -70, 0x3e, 0x62, -65, 0x27, 0x3f, 0x62, -65, 0x27, -65, -84, -85, -70, 0x3e, 0x62, -65, 0x27, 0x3f, 0x62, -65, 0x27, -65, -84, -85, -70, 0x3e, 0x35, 0x40, 0x45, 0x3f, 0x4b, -52, 3, -65, -84, -85, -70, 0x3e, 0x35, 0x40, 0x45, 0x3f, 0x4b, -52, 3, -65, -84, -85, -70, 0x3e, 0x52, 0x2c, 0x5b, 0x3f, 0x68, -111, -75, -66, -51, -85, -70, 0x3e, 0x52, 0x2c, 0x5b, 0x3f, 0x68, -111, -75, -66, -51, -85, -70, 0x3e, -31, 0x5f, 0x68, 0x3f, -51, 0x76, 0x45, -66, -126, 0x6f, -70, 0x3e, -31, 0x5f, 0x68, 0x3f, -51, 0x76, 0x45, -66, -126, 0x6f, -70, 0x3e, -104, -94, 0x6c, 0x3f, 0xf, 0x46, -20, -67, -12, 0x32, -70, 0x3e, -104, -94, 0x6c, 0x3f, 0xf, 0x46, -20, -67, -12, 0x32, -70, 0x3e, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f};
extern int get_effect_texture_alt(int) __asm__("FUN_001f44b8");
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void FUN_L00_001fde98(int, int, int, void *, int);
void FUN_L17_002f54d8(int n, V3 *b, V3 *a, int unused, float *out);
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

int D_L17_001623F0[2] = {26, 26};
extern float D_L17_001DE150[];
extern float D_L17_001DE288[];
float *D_L17_001623F8[2] = {D_L17_001DE150, D_L17_001DE288};
extern float D_L17_001DE3C0[];
extern float D_L17_001DE490[];
float *D_L17_00162400[2] = {D_L17_001DE3C0, D_L17_001DE490};
extern float D_L17_001DE560[];
extern float D_L17_001DE698[];
float *D_L17_00162408[2] = {D_L17_001DE560, D_L17_001DE698};
extern float D_L17_001DE7D0[];
extern float D_L17_001DE838[];
float *D_L17_00162410[2] = {D_L17_001DE7D0, D_L17_001DE838};
void FUN_L17_002f5690(void) {
    int i;
    float *p;
    vu1_add_g_sregister(6, get_effect_texture_alt(0x30));
    vu1_add_g_sregister(0x42, 0x2000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    p = (float *)D_L17_001DE8A0;
    for (i = 0; i < 2; i++) {
        FUN_L17_002f54d8(D_L17_001623F0[i], ((int *)D_L17_00162408)[i],
                         ((int *)D_L17_001623F8)[i], ((int *)D_L17_00162400)[i], p);
        FUN_L00_001fde98(D_L17_001623F0[i], ((int *)D_L17_001623F8)[i],
                         ((int *)D_L17_00162410)[i], p, 1);
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F74E8), where it is exact; names translated to the US level program. */

char D_L17_001DF6A0[2752] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -48, -84, 1, 0x44, -83, -97, -76, 0x43, -57, 0x67, 0xf, 0x43, 0x13, 0xa, 0x5a, 0x41, 0xa, -2, -12, 0x43, 0x6d, -110, -80, 0x43, -56, 0x67, 0xf, 0x43, 0x74, -102, -66, 0x41, -71, -15, 0, 0x44, -23, -103, -81, 0x43, 0x24, -24, 0xb, 0x43, 0x7a, -40, 0, 0x44, -100, -50, -81, 0x43, 0x6b, -25, 0x12, 0x43, -103, 0x4b, 1, 0x44, -127, 0x7a, -80, 0x43, 0x24, -24, 0xb, 0x43, -22, 0x2d, 1, 0x44, 0x34, -81, -80, 0x43, 0x6b, -25, 0x12, 0x43, -38, -56, 1, 0x44, 0x29, 0x4f, -78, 0x43, 0x24, -24, 0xb, 0x43, -34, -89, 1, 0x44, 0x29, 0x4f, -78, 0x43, 0x6b, -25, 0x12, 0x43, -5, 0x15, 2, 0x44, -83, 0x4b, -76, 0x43, 0x24, -24, 0xb, 0x43, -9, -14, 1, 0x44, 5, 0x4c, -76, 0x43, 0x6b, -25, 0x12, 0x43, 6, 0x30, 2, 0x44, -123, 0x5c, -74, 0x43, 0x24, -24, 0xb, 0x43, 0x52, 0xc, 2, 0x44, -124, 0x5c, -74, 0x43, 0x6b, -25, 0x12, 0x43, -5, 0x15, 2, 0x44, 0x5c, 0x6d, -72, 0x43, 0x24, -24, 0xb, 0x43, -10, -14, 1, 0x44, 0x5c, 0x6d, -72, 0x43, 0x6b, -25, 0x12, 0x43, -39, -56, 1, 0x44, -76, -81, -70, 0x43, 0x24, -24, 0xb, 0x43, -35, -89, 1, 0x44, -76, -81, -70, 0x43, 0x6b, -25, 0x12, 0x43, -50, -30, -2, 0x43, 0x2b, 0x1b, -83, 0x43, 0x24, -24, 0xb, 0x43, 0x22, -69, -2, 0x43, 0x45, 0x4c, -83, 0x43, 0x6b, -25, 0x12, 0x43, -89, -42, -3, 0x43, 0x1c, -108, -84, 0x43, 0x24, -24, 0xb, 0x43, 0x53, -69, -3, 0x43, 0x42, -48, -84, 0x43, 0x6b, -25, 0x12, 0x43, 0x22, -38, -5, 0x43, -39, -7, -85, 0x43, 0x24, -24, 0xb, 0x43, 0x34, -52, -5, 0x43, -49, 0x3c, -84, 0x43, 0x6b, -25, 0x12, 0x43, 0x4a, -55, -7, 0x43, -60, -59, -85, 0x43, 0x24, -24, 0xb, 0x43, 0x4a, -55, -7, 0x43, 6, 0xb, -84, 0x43, 0x6b, -25, 0x12, 0x43, 0x73, -72, -9, 0x43, -39, -7, -85, 0x43, 0x24, -24, 0xb, 0x43, 0x61, -58, -9, 0x43, -49, 0x3c, -84, 0x43, 0x6b, -25, 0x12, 0x43, -18, -69, -11, 0x43, 0x1c, -108, -84, 0x43, 0x24, -24, 0xb, 0x43, 0x41, -41, -11, 0x43, 0x42, -48, -84, 0x43, 0x6b, -25, 0x12, 0x43, 0x46, -25, -13, 0x43, -100, -114, -83, 0x43, 0x24, -24, 0xb, 0x43, -13, 0xe, -12, 0x43, -74, -65, -83, 0x43, 0x6b, -25, 0x12, 0x43, -128, 0x4c, -14, 0x43, -71, -33, -82, 0x43, 0x24, -24, 0xb, 0x43, -3, 0x7e, -14, 0x43, 0x6d, 0x14, -81, 0x43, 0x6b, -25, 0x12, 0x43, 0x61, -5, -16, 0x43, -127, 0x7a, -80, 0x43, 0x24, -24, 0xb, 0x43, -63, 0x36, -15, 0x43, 0x34, -81, -80, 0x43, 0x6b, -25, 0x12, 0x43, -31, 0, -16, 0x43, 0x29, 0x4f, -78, 0x43, 0x24, -24, 0xb, 0x43, -39, 0x42, -16, 0x43, 0x29, 0x4f, -78, 0x43, 0x6b, -25, 0x12, 0x43, -96, 0x66, -17, 0x43, -83, 0x4b, -76, 0x43, 0x24, -24, 0xb, 0x43, -89, -84, -17, 0x43, 5, 0x4c, -76, 0x43, 0x6b, -25, 0x12, 0x43, -118, 0x32, -17, 0x43, -123, 0x5c, -74, 0x43, 0x24, -24, 0xb, 0x43, -14, 0x79, -17, 0x43, -124, 0x5c, -74, 0x43, 0x6b, -25, 0x12, 0x43, -96, 0x66, -17, 0x43, 0x5c, 0x6d, -72, 0x43, 0x24, -24, 0xb, 0x43, -89, -84, -17, 0x43, 0x5c, 0x6d, -72, 0x43, 0x6b, -25, 0x12, 0x43, -31, 0, -16, 0x43, -75, -81, -70, 0x43, 0x24, -24, 0xb, 0x43, -39, 0x42, -16, 0x43, -76, -81, -70, 0x43, 0x6b, -25, 0x12, 0x43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0x6a, 0x17, 0x47, 0x3f, -21, 0x56, 0x1f, -65, 0xc, -108, -76, 0x3d, 0x6a, 0x17, 0x47, 0x3f, -21, 0x56, 0x1f, -65, 0xc, -108, -76, 0x3d, -116, 0x31, 0x54, 0x3f, -4, -31, 0xb, -65, 0x76, -86, -100, 0x3d, -116, 0x31, 0x54, 0x3f, -4, -31, 0xb, -65, 0x76, -86, -100, 0x3d, 0x32, -53, 0x6a, 0x3f, 0x27, -120, -62, -66, -84, -27, -114, 0x3d, 0x32, -53, 0x6a, 0x3f, 0x27, -120, -62, -66, -84, -27, -114, 0x3d, 0x68, 0x21, 0x79, 0x3f, -5, 0x3c, 0x46, -66, -3, -126, -99, 0x3d, 0x68, 0x21, 0x79, 0x3f, -5, 0x3c, 0x46, -66, -3, -126, -99, 0x3d, -8, -5, 0x7d, 0x3f, -100, 0x53, -55, 0x36, 0x3f, 0x71, -96, 0x3d, -8, -5, 0x7d, 0x3f, -100, 0x53, -55, 0x36, 0x3f, 0x71, -96, 0x3d, 0x18, 0x5d, 0x7a, 0x3f, 0x15, -88, 0x35, 0x3e, 0x4f, 0x24, -104, 0x3d, 0x18, 0x5d, 0x7a, 0x3f, 0x15, -88, 0x35, 0x3e, 0x4f, 0x24, -104, 0x3d, 0x49, -70, 0x76, 0x3f, 0x77, -97, -125, 0x3e, 0x28, 0x62, -111, 0x3d, 0x49, -70, 0x76, 0x3f, 0x77, -97, -125, 0x3e, 0x28, 0x62, -111, 0x3d, -88, -60, -27, 0x3e, -58, 0x18, 0x64, -65, -79, -65, -116, 0x3d, -88, -60, -27, 0x3e, -58, 0x18, 0x64, -65, -79, -65, -116, 0x3d, 0x2e, 0, -67, 0x3e, -50, 0x35, 0x6c, -65, -65, 0xd, -111, 0x3d, 0x2e, 0, -67, 0x3e, -50, 0x35, 0x6c, -65, -65, 0xd, -111, 0x3d, 0x1c, 0x45, 0x46, 0x3e, 0x1f, 0x2e, 0x79, -65, -113, 0x34, -104, 0x3d, 0x1c, 0x45, 0x46, 0x3e, 0x1f, 0x2e, 0x79, -65, -113, 0x34, -104, 0x3d, -100, 0x53, 0x49, 0x36, -72, 6, 0x7e, -65, -75, 0x18, -100, 0x3d, -100, 0x53, 0x49, 0x36, -72, 6, 0x7e, -65, -75, 0x18, -100, 0x3d, -17, 0x3b, 0x46, -66, -64, 0x24, 0x79, -65, -55, 0x3a, -100, 0x3d, -17, 0x3b, 0x46, -66, -64, 0x24, 0x79, -65, -55, 0x3a, -100, 0x3d, -76, 0x73, -62, -66, -119, -73, 0x6a, -65, -2, -44, -104, 0x3d, -76, 0x73, -62, -66, -119, -73, 0x6a, -65, -2, -44, -104, 0x3d, 0x56, 0x2d, 0xd, -65, 0x2c, 0x48, 0x53, -65, -48, 0x2a, -109, 0x3d, 0x56, 0x2d, 0xd, -65, 0x2c, 0x48, 0x53, -65, -48, 0x2a, -109, 0x3d, -36, -97, 0x33, -65, 0x77, -93, 0x33, -65, -93, -110, -102, 0x3d, -36, -97, 0x33, -65, 0x77, -93, 0x33, -65, -93, -110, -102, 0x3d, -117, 0x17, 0x53, -65, -65, 0xd, 0xd, -65, -53, -123, -86, 0x3d, -117, 0x17, 0x53, -65, -65, 0xd, 0xd, -65, -53, -123, -86, 0x3d, -117, -89, 0x6a, -65, -39, 0x5b, -62, -66, 0x52, -15, -97, 0x3d, -117, -89, 0x6a, -65, -39, 0x5b, -62, -66, 0x52, -15, -97, 0x3d, 0x4d, 0x2f, 0x79, -65, -44, 0x49, 0x46, -66, -35, 0x78, -105, 0x3d, 0x4d, 0x2f, 0x79, -65, -44, 0x49, 0x46, -66, -35, 0x78, -105, 0x3d, -25, -5, 0x7d, -65, -84, -59, -89, -74, 0x10, 0x78, -96, 0x3d, -25, -5, 0x7d, -65, -84, -59, -89, -74, 0x10, 0x78, -96, 0x3d, 0x44, 0x4f, 0x7a, -65, 0x24, -103, 0x35, 0x3e, -39, 0x21, -98, 0x3d, 0x44, 0x4f, 0x7a, -65, 0x24, -103, 0x35, 0x3e, -39, 0x21, -98, 0x3d, 0x7f, -90, 0x76, -65, 0x19, -110, -125, 0x3e, 0x24, 0x47, -102, 0x3d, 0x7f, -90, 0x76, -65, 0x19, -110, -125, 0x3e, 0x24, 0x47, -102, 0x3d, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f};
extern float D_L17_001DED50[];
float *D_L17_00162440[2] = {D_L17_001DED50, 0};
extern float D_L17_001DF280[];
float *D_L17_00162450[2] = {D_L17_001DF280, 0};
extern float D_L17_001DF598[];
float *D_L17_00162458[2] = {D_L17_001DF598, 0};
void FUN_L17_002f54d8(int n, V3 *b, V3 *a, int unused, float *out);
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

int D_L17_00162420[2] = {66, 0};
extern float D_L17_001DF070[];
float *D_L17_00162448[2] = {D_L17_001DF070, 0};
void FUN_L17_002f59a8(void) {
    int i;
    float *p;
    vu1_add_g_sregister(6, get_effect_texture_alt(0x30));
    vu1_add_g_sregister(0x42, 0x2000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    p = (float *)D_L17_001DF6A0;
    for (i = 0; i < 1; i++) {
        FUN_L17_002f54d8(D_L17_00162420[i], (V3 *)((int *)D_L17_00162450)[i],
                         (V3 *)((int *)D_L17_00162440)[i], ((int *)D_L17_00162448)[i], p);
        FUN_L00_001fde98(D_L17_00162420[i], ((int *)D_L17_00162440)[i],
                         ((int *)D_L17_00162458)[i], p, 1);
    }
}
/* Computes a per-vertex 2D sphere-map texture coordinate from position and normal arrays. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F7C78), where it is exact; names translated to the US level program. */

void FUN_L17_002f6138(int n, char *nrm, char *pos, int unused, float *out) {
    float a[4];
    float t[4];
    float r[4];
    float c[4];
    int i;
    for (i = 0; i < n; i++) {
        float *p = (float *)(i * 12 + (int)pos);
        float *q;
        float len;
        t[3] = 1.0f;
        t[0] = p[0];
        t[1] = p[1];
        t[2] = p[2];
        FUN_001f9a28(a, D_L17_001676C0, t);
        fast_vec_normalize(a, a, 1.0f);
        c[3] = 1.0f;
        q = (float *)(i * 12 + (int)nrm);
        c[0] = q[0];
        c[1] = q[1];
        c[2] = q[2];
        fast_vec_normalize(c, c, 1.0f);
        fast_vec_scale(r, c, fast_vec_dot(c, a) * 2.0f);
        FUN_001f9a28(r, a, r);
        fast_vec_normalize(r, r, 1.0f);
        r[2] += 1.0f;
        len = fast_vec_length(r) * 2.0f;
        out[i * 2] = (r[0] / len + 0.5f) * 4.0f;
        out[i * 2 + 1] = (r[1] / len + 0.5f) * 4.0f;
    }
}

u8 D_L17_001E3B20[1200] = {0};

/* Shared moby class mine (0x4D): a group moby that seeks the hero. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "sda.h"

typedef struct {
    float              v[4][4];
    int                col[4];
    float              uv[4][2];
    long long          r70;
    long long          texture; /* get_effect_texture */
    unsigned long long r80;
    unsigned long long alpha; /* GS ALPHA register value (0x8000000044) */
} EffectQuad;

extern float        fast_sin(float);
extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");
extern int          random_integer_below(int);
extern int          scale_game_frames(int);
extern void         cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void         add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern void         FUN_L00_00258830(int, int, float, float, float, int);

extern int              D_L15_001ABE40[];
extern char             D_0013F420[];
extern float            D_0015ED6C;
extern void             random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern float D_0015ED6C MACRO_ADDR;
extern int              D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") MACRO_ADDR;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int   D_L15_00161AB8_lui __asm__("D_L15_00161AB8"); /* no foreign declaration */
extern float group_moby_seeking_speed_gp __asm__("D_L15_001614B4") __attribute__((sda));
extern int   group_moby_particle_color_gp __asm__("D_L15_001614B8") __attribute__((sda));
extern float group_moby_particle_scale_gp __asm__("D_L15_001614BC") __attribute__((sda));
extern int   group_moby_particle_lifetime_frames_gp __asm__("D_L15_001614C0") __attribute__((sda));
extern float group_moby_effect_z_offset_gp __asm__("D_L15_001614C4") __attribute__((sda));
extern void  spawn_moving_hero_relative_alpha_fade_particle(float        *pos,
                                                            char         *vel,
                                                            int           color,
                                                            unsigned char life,
                                                            unsigned char b,
                                                            int           mode,
                                                            float scale) __asm__("FUN_L00_002730e0");
void         draw_group_moby_effect_quads(unsigned char *moby) __asm__("FUN_L15_002a29b8");

extern void draw_group_moby_effect_quads(unsigned char *) __asm__("FUN_L15_002a29b8");
extern void func_L00_0025A8E8_m(void *, void *, int, float, float, float, int, int, int) __asm__("FUN_L00_002598b0");

extern float group_moby_effect_quad_vertices[4][4][4] __asm__("D_L15_001CE110");
extern float group_moby_effect_quad_uvs[8] __asm__("D_L15_001CE210");
extern int   group_moby_effect_first_quad_color_gp __asm__("D_L15_001614C8") __attribute__((sda));
extern long long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void      draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

/* Shared level 15/17 moby update (class 77): steers the moby toward its target and runs its state machine. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A3668), where it is exact; names translated to the US level program. */

void update_hero_seeking_group_moby(char *moby) __asm__("FUN_L15_002a2488");

void update_hero_seeking_group_moby(char *moby) {
    float         va[4];
    float         pb[4];
    char         *data;
    char         *callback_frame_stamp;
    char         *hero_base;
    float         fa;
    float         fb;
    float         yaw_cosine;
    float         yaw_sine;
    float         reference_distance_xy;
    float         yaw;
    float         target_z;
    float         fd;
    int           state;
    int           animation_blend_frames;
    int           particle_lifetime_frames;
    int           converted_particle_lifetime_frames;
    int           particle_initial_rotation;
    unsigned char life;

    data = ((char *)MOBY(moby)->pvars);
    if (find_moby_hit(moby, 0x330000, 0) != 0) {
        ((unsigned char *)moby)[0x20] = 4;
        ((unsigned char *)moby)[0xA4] = 0xFF;
    }
    if (((unsigned char *)moby)[0x20] != 1) {
        random_vector(va, 0.0f, D_0015ED6C + D_0015ED6C);
        qcopy(pb, moby + 0x10);
        pb[2] = pb[2] + *(float *)&group_moby_effect_z_offset_gp;
        if (D_L15_00161AB8_lui == 0) {
            ((unsigned char *)moby)[0x20] = 1;
            goto tailA;
        }
        particle_lifetime_frames = scale_game_frames(group_moby_particle_lifetime_frames_gp);
        converted_particle_lifetime_frames = truncate_float_to_s32((float)particle_lifetime_frames);
        life = (unsigned char)converted_particle_lifetime_frames;
        particle_initial_rotation = random_integer_below(0xFF);
        spawn_moving_hero_relative_alpha_fade_particle(pb,
                                                       (char *)va,
                                                       group_moby_particle_color_gp,
                                                       life,
                                                       (unsigned char)particle_initial_rotation,
                                                       0,
                                                       *(float *)&group_moby_particle_scale_gp);
        callback_frame_stamp = *(char **)(data + 0x6C);
        if (callback_frame_stamp == 0 || *(int *)callback_frame_stamp != D_L15_0015F5CC_macro) {
            if (callback_frame_stamp != 0) {
                *(int *)callback_frame_stamp = D_L15_0015F5CC_macro;
            }
            add_draw_callback(draw_group_moby_effect_quads, moby);
        }
    }
    state = ((unsigned char *)moby)[0x20];
    if (state == 2)
        goto s2;
    if (state < 3) {
        if (state == 0)
            goto s0;
        return;
    }
    if (state == 3)
        goto s3;
    if (state == 4)
        goto s4;
    return;

s2:
    MOBY(moby)->pos.z = MOBY(moby)->pos.z + (D_0015ED6C + D_0015ED6C);
    if (fast_dec_timer(data + 0x68) == 0)
        return;
    ((unsigned char *)moby)[0x20] = 3;
    if (((unsigned char *)moby)[0x53] != 1) {
        animation_blend_frames = scale_game_frames(10);
        blend_moby_animation(moby, 1, 0, animation_blend_frames);
    }
    *(int *)(data + 0x68) = scale_game_frames(0xF0);
    return;

s3:
    hero_base = (char *)&hero;
    fa = *(float *)(hero_base + 0x80) - MOBY(moby)->pos.x;
    fb = *(float *)(hero_base + 0x84) - MOBY(moby)->pos.y;
    yaw = fast_add_rotations(*(float *)(data + 0x64), fast_arc_tan(fa, fb));
    target_z = *(float *)(hero_base + 0xC8);
    yaw_cosine = fast_cos(yaw);
    va[0] = yaw_cosine * (*(float *)&group_moby_seeking_speed_gp * D_0015ED6C);
    yaw_sine = fast_sin(yaw);
    va[1] = yaw_sine * (*(float *)&group_moby_seeking_speed_gp * D_0015ED6C);
    va[2] = 0.0f;
    fd = MOBY(moby)->pos.z - target_z;
    if (0.6f < fd) {
        va[2] = -(D_0015ED6C * 3.0f);
    } else if (fd < 0.4f) {
        va[2] = D_0015ED6C * 3.0f;
    }
    FUN_L00_00258830((int)moby, (int)va, 0.25f, 0.25f, 0.0f, 0);
    reference_distance_xy =
        vector_distance_xy((float *)((char *)((char *)&D_0013F420)), (float *)&MOBY(moby)->pos);
    if (reference_distance_xy < 0.5f || fast_dec_timer(data + 0x68)) {
        ((unsigned char *)moby)[0x20] = 4;
        return;
    }
    if (((unsigned char *)moby)[0x53] != 1)
        return;
    if ((((unsigned char *)moby)[0x70] & 2) == 0)
        return;
    animation_blend_frames = scale_game_frames(10);
    blend_moby_animation(moby, 2, 0, animation_blend_frames);
    return;

s4:
    cart_burst(moby, moby + 0x10, -1, 0.25f, 13.0f);
    func_L00_0025A8E8_m(moby, moby + 0x10, 0x10001, 1.0f, 1.0f, 1.0f, 0, 1, 0);

s0:
    ((unsigned char *)moby)[0x20] = 1;
    ((unsigned char *)moby)[0x31] = 0;
    MOBY(moby)->flags = MOBY(moby)->flags | 1;
tailA:
    MOBY(moby)->collision = 0;
}

void draw_group_moby_effect_quads(unsigned char *moby) {
    EffectQuad      effect_quad;
    float           angles[4] __attribute__((aligned(16)));
    float           rotation[12] __attribute__((aligned(16)));
    float           position[4] __attribute__((aligned(16)));
    unsigned short *group_moby_entry;
    int             i, j;

    effect_quad.texture = get_effect_texture(8);
    effect_quad.r80 = 0xff9000000260LL;
    effect_quad.alpha = 0x8000000044LL;
    effect_quad.r70 = 0;
    {
        int    count = 3;
        float *source = group_moby_effect_quad_uvs;
        float *v = &effect_quad.uv[0][1];
        float *u = &effect_quad.uv[0][0];
        do {
            count--;
            *u = source[0];
            *v = source[1];
            source += 2;
            u += 2;
            v += 2;
        } while (count >= 0);
    }
    group_moby_entry = (unsigned short *)D_L15_001ABE40[MOBY(moby)->group];
    do {
        unsigned char *other =
            (unsigned char *)((int)l15_moby_array + ((*group_moby_entry & 0x7fff) << 8));
        if (MOBY(other)->state != 1) {
            clear_u64_value(angles);
            angles[2] = fast_arc_tan(l15_camera.pos.f[0] - MOBY(other)->pos.x,
                                     l15_camera.pos.f[1] - MOBY(other)->pos.y);
            angles[1] = -fast_arc_tan(vector_distance_xy(&MOBY(other)->pos, l15_camera.pos.f),
                                      l15_camera.pos.f[2] - MOBY(other)->pos.z);
            euler_rotation_matrix(rotation, angles);
            qcopy(position, &MOBY(other)->pos);
            position[2] += group_moby_effect_z_offset_gp;
            position[3] = 1.0f;
            for (i = 0; i < 4; i++) {
                int *colors = effect_quad.col;
                int  color = (&group_moby_effect_first_quad_color_gp)[i];
                for (j = 0; j < 4; j++) {
                    colors[j] = color;
                    qcopy(effect_quad.v[j], group_moby_effect_quad_vertices[i][j]);
                }
                draw_geometry_quad(&effect_quad, rotation, 0);
            }
        }
    } while ((short)*group_moby_entry++ >= 0);
}

int   D_L17_0016198C = 0;
float D_L17_00161454 = 1e+01f;
int   D_L17_00161458 = 0x60206080;
float D_L17_0016145C = 0.25f;
int   D_L17_00161460 = 0x14;
float D_L17_00161464 = 0.333f;

float D_L17_001CE390[4][4][4] = {
    {  {0.0f, 0.2f, -0.2f, 1.0f},
     {0.0f, -0.2f, -0.2f, 1.0f},
     {0.0f, 0.2f, 0.2f, 1.0f},
     {0.0f, -0.2f, 0.2f, 1.0f}  },
    {  {0.0f, 0.1f, -0.1f, 1.0f},
     {0.0f, -0.1f, -0.1f, 1.0f},
     {0.0f, 0.1f, 0.1f, 1.0f},
     {0.0f, -0.1f, 0.1f, 1.0f}  },
    {{0.0f, 0.25f, -0.25f, 1.0f},
     {0.0f, -0.25f, -0.25f, 1.0f},
     {0.0f, 0.25f, 0.25f, 1.0f},
     {0.0f, -0.25f, 0.25f, 1.0f}},
    {  {0.0f, 0.3f, -0.3f, 1.0f},
     {0.0f, -0.3f, -0.3f, 1.0f},
     {0.0f, 0.3f, 0.3f, 1.0f},
     {0.0f, -0.3f, 0.3f, 1.0f}  },
};

float D_L17_001CE490[8] = {1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

int D_L17_00161468 = 0x40602080;

int group_moby_particle_lifetime_frames_gp __asm__("D_L15_001614C0") = 0x14;

float group_moby_effect_quad_vertices[4][4][4] __asm__("D_L15_001CE110") = {
    {  {0.0f, 0.2f, -0.2f, 1.0f},
     {0.0f, -0.2f, -0.2f, 1.0f},
     {0.0f, 0.2f, 0.2f, 1.0f},
     {0.0f, -0.2f, 0.2f, 1.0f}  },
    {  {0.0f, 0.1f, -0.1f, 1.0f},
     {0.0f, -0.1f, -0.1f, 1.0f},
     {0.0f, 0.1f, 0.1f, 1.0f},
     {0.0f, -0.1f, 0.1f, 1.0f}  },
    {{0.0f, 0.25f, -0.25f, 1.0f},
     {0.0f, -0.25f, -0.25f, 1.0f},
     {0.0f, 0.25f, 0.25f, 1.0f},
     {0.0f, -0.25f, 0.25f, 1.0f}},
    {  {0.0f, 0.3f, -0.3f, 1.0f},
     {0.0f, -0.3f, -0.3f, 1.0f},
     {0.0f, 0.3f, 0.3f, 1.0f},
     {0.0f, -0.3f, 0.3f, 1.0f}  },
};

float group_moby_effect_quad_uvs[8] __asm__("D_L15_001CE210") = {
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

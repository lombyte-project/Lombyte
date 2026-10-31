/* Giant Clank's charged blast (class 0x5F3, Quartu and Veldin 2): charges up, then bursts into particles; and the functions that spawn it, move it and remove it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "sda.h"

extern int   D_L00_0015F320;
extern int   D_L00_0015F330;
extern int   allocate_packed_color_point_light_slot(float *pos,
                                                    float  radius,
                                                    float  intensity,
                                                    int    color) __asm__("FUN_L00_0023e5e0");
extern short D_L00_001613C8_gp __asm__("D_L00_001613C8") __attribute__((sda));

extern int  D_L00_0015F324;
extern int  D_L00_0015F334;
extern long D_L00_0015F328;
extern long D_L00_0015F338;

extern short D_L00_001613C8 __attribute__((sda));
void         FUN_L00_002a9aa0_u(void *, OvlQuad *) __asm__("FUN_L00_002a9aa0");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAD40), where it is exact; names translated to the US level program. */

void *spawn_giant_clank_charged_blast(int    unused,
                                      float *spawn_position) __asm__("FUN_L00_002a9aa0");

void *spawn_giant_clank_charged_blast(int unused, float *spawn_position) {
    char *spawned_moby = create_moby(0x5F3);
    if (spawned_moby != 0 && *(int *)&D_L00_001613C8_gp == 0) {
        char *moby_vars;
        spawned_moby[0x20] = 0;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        *(OvlQuad *)&MOBY(spawned_moby)->pos = *(OvlQuad *)spawn_position;
        D_L00_0015F324 = 0xFF;
        D_L00_0015F334 = 0xFFFFFF;
        D_L00_0015F338 = 0x1F00000042L;
        *(int *)&D_L00_001613C8_gp = (int)spawned_moby;
        D_L00_0015F328 = 0;
        D_L00_0015F320 = 0;
        D_L00_0015F330 = 0;
        *(int *)(moby_vars + 0x10) = 0;
        *(short *)(moby_vars + 0x2C) =
            allocate_packed_color_point_light_slot(spawn_position, 0.0f, 0.0f, 0);
    }
    return spawned_moby;
}

void set_giant_clank_charged_blast_position(void    *unused_creation_arg,
                                            OvlQuad *position) __asm__("FUN_L00_002a9b80");

void set_giant_clank_charged_blast_position(void *unused_creation_arg, OvlQuad *position) {
    OvlQuad position_copy = *position;
    u8     *moby;
    u8     *moby_vars;
    u8     *tracked_moby;
    tracked_moby = *(u8 **)&D_L00_001613C8;
    if (tracked_moby == 0) {
        FUN_L00_002a9aa0_u(unused_creation_arg, &position_copy);
        tracked_moby = *(u8 **)&D_L00_001613C8;
        if (tracked_moby == 0)
            return;
    }
    moby = *(u8 **)&D_L00_001613C8;
    moby_vars = ((u8 *)MOBY(moby)->pvars);
    if (MOBY(moby)->oclass == 0x5F3) {
        *(OvlQuad *)&MOBY(moby)->pos = position_copy;
        *(s16 *)(moby_vars + 0x2E) = 1;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAE80), where it is exact; names translated to the US level program. */

void remove_giant_clank_charged_blast(void) __asm__("FUN_L00_002a9be0");

void remove_giant_clank_charged_blast(void) {
    struct Moby *tracked_moby = *(char **)&D_L00_001613C8_gp;
    if (tracked_moby != 0) {
        if (tracked_moby->oclass == 0x5F3) {
            char *tracked_moby_vars;
            int   point_light_slot_index;
            if (tracked_moby->state != 0)
                return;
            tracked_moby_vars = ((char *)tracked_moby->pvars);
            D_L00_0015F320 = 0;
            D_L00_0015F330 = 0;
            point_light_slot_index = *(short *)(tracked_moby_vars + 0x2C);
            if (point_light_slot_index != -1)
                release_point_light_slot(point_light_slot_index);
            mark_moby_for_removal(*(char **)&D_L00_001613C8_gp);
        }
        *(int *)&D_L00_001613C8_gp = 0;
    }
}

int   D_L04_0015F320 = 0;
int   D_L04_0015F330 = 0;
short D_L04_0016139C = 0;
int   D_L04_0015F324 = 0;
int   D_L04_0015F334 = 0;
int   D_L07_0015F320 = 0;
int   D_L07_0015F330 = 0;
short D_L07_0016139C = 0;
int   D_L07_0015F324 = 0;
int   D_L07_0015F334 = 0;
int   D_L09_0015F320 = 0;
int   D_L09_0015F330 = 0;
short D_L09_00161424 = 0;
int   D_L09_0015F324 = 0;
int   D_L09_0015F334 = 0;
int   D_L10_0015F320 = 0;
int   D_L10_0015F330 = 0;
short D_L10_001613A0 = 0;
int   D_L10_0015F324 = 0;
int   D_L10_0015F334 = 0;
int   D_L13_0015F320 = 0;
int   D_L13_0015F330 = 0;
short D_L13_00161464 = 0;
int   D_L13_0015F324 = 0;
int   D_L13_0015F334 = 0;
int   D_L15_0015F324 = 0;
int   D_L15_0015F334 = 0;
int   D_L18_0015F320 = 0;
int   D_L18_0015F330 = 0;
short D_L18_0016143C = 0;
int   D_L18_0015F324 = 0;
int   D_L18_0015F334 = 0;

int D_L00_0015F324 = 0;

int D_L00_0015F334 = 0;

short D_L00_001613C8_gp __asm__("D_L00_001613C8") = 0;

typedef int u128_29FF90 __attribute__((mode(TI)));

typedef struct {
    float vel[4];
    int   f10;
    int   f14;
    char  pad18[0x14];
    short light;
    short active;
} Data29FF90;

typedef struct {
    char          pad0[0x10];
    float         pos[4];
    unsigned char state;
    char          pad21[0x57];
    Data29FF90   *data;
} Moby29FF90;

typedef struct {
    char          pad0[0x98];
    float         f98;
    char          pad9C[0x2008];
    unsigned char f20A4;
} Hero29FF90;

typedef struct {
    float f0;
    float f4;
    char  pad8[8];
    float f10;
    float f14;
    float f18;
    float f1C;
} Light29FF90;

typedef struct {
    char  pad0[0x10];
    float f10;
    short f14;
    short f16;
    int   f18;
} PartSub29FF90;

typedef struct {
    char          pad0[0xA];
    short         fA;
    float         fC;
    char          pad10[0x10];
    PartSub29FF90 sub;
} Part29FF90;

typedef struct {
    int on;
    union {
        unsigned int  rgba;
        unsigned char b[4];
    } col;
    long reg;
} Fade29FF90;

extern float fast_sin(float);
extern int   random_integer_below(int);
extern int   scale_game_frames(int);

extern char  D_001413F4[];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float D_0015ED6C;

extern float D_L15_0015F580[] MACRO_ADDR;
extern int   D_L15_00178500[];

extern Fade29FF90 D_L15_0015F330 MACRO_ADDR;
extern Fade29FF90 D_L15_0015F320 MACRO_ADDR;
extern Light29FF90               D_L15_001808C0[];
extern s32                       D_L15_00178500[];
extern char                     *spawn_mode_controlled_rotating_alpha_fade_particle(void *,
                                                                                    void *,
                                                                                    int,
                                                                                    void *) __asm__("FUN_L00_00274948");
extern float D_0015ED6C          MACRO_ADDR;
extern int                       D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") MACRO_ADDR;
extern int charged_blast_removal_target __asm__("D_L15_00161484"); /* no foreign declaration */
extern int allocate_packed_color_point_light_slot(float *pos,
                                                  float  radius,
                                                  float  intensity,
                                                  int    color) __asm__("FUN_L00_0023e5e0");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern unsigned char *D_L15_001B26AC NOT_SDA;
extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void spawn_owner_target_particle(int a0, void *a1, char *a2) __asm__("FUN_L15_0025e2c0");
void        random_spherical_offset(void *out, f32 a, f32 b) __asm__("FUN_00213358");
extern void func_L00_0026AA10_29FF90(void *,
                                     unsigned char,
                                     unsigned char,
                                     unsigned char,
                                     float,
                                     float,
                                     int) __asm__("FUN_L00_00269b70");
extern int func_L00_001F2BE8_29FF90(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern char *func_L00_0026E940_29FF90(float, char *, int, int, int) __asm__("FUN_L00_0026daa0");
extern void *func_L00_0026CA10_29FF90(void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");

extern f32 frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;

/* update of moby class 1523: charges up a screen fade and a light, then bursts into particles and deletes itself */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029FF90), where it is exact; names translated to the US level program. */

void giant_clank_charged_blast_update(Moby29FF90 *moby) __asm__("FUN_L15_0029edb0");

/* Moby class 1523 */
void giant_clank_charged_blast_update(Moby29FF90 *moby) {
    enum {
        CHARGING = 0,
        EXPANDING_BLAST = 1
    };
    float       random_effect_vector[4];
    float       effect_vector_scratch[4];
    float       temporary_position[4];
    Data29FF90 *moby_data = moby->data;

    switch (moby->state) {
    case CHARGING:
        if (moby_data->active != 0 && ((Hero29FF90 *)&hero)->f20A4 == 2) {
            float charge_emission_factor;
            int   i;

            if (moby_data->f10 == 0) {
                D_L15_0015F330.on = 1;
                D_L15_0015F330.col.rgba = 0xFFFF00;
            }
            moby_data->f10++;
            charge_emission_factor = convert_integer_to_float(moby_data->f10) / scale_time(30.0f);
            if (charge_emission_factor > 1.0f)
                charge_emission_factor = 1.0f;
            charge_emission_factor = charge_emission_factor * charge_emission_factor;
            for (i = 0; i < truncate_float_to_s32(charge_emission_factor * 10.0f); i++) {
                int g;
                int r;
                int rc;

                *(u128_29FF90 *)effect_vector_scratch = 0;
                effect_vector_scratch[0] = random_float_between(-1.0f, 1.0f);
                effect_vector_scratch[1] = random_float_between(-1.0f, 1.0f);
                effect_vector_scratch[2] = random_float_between(-1.0f, 1.0f);
                *(u128_29FF90 *)random_effect_vector = *(u128_29FF90 *)effect_vector_scratch;
                g = random_int_between(0x4F, 0xAF);
                r = random_int_between(0x4F, 0xFF);
                rc = r > 0xFF ? 0xFF : r;
                if (g > 0xFF)
                    g = 0xFF;
                fast_vec_normalize(
                    random_effect_vector, random_effect_vector, random_float_between(20.0f, 40.0f));
                add_vectors(effect_vector_scratch, random_effect_vector, moby->pos);
                spawn_owner_target_particle((0x7F << 24) | (rc << 16) | (g << 8) | 0x4F,
                                            random_effect_vector,
                                            (char *)moby);
            }
            if (moby_data->f10 < 0x32) {
                int fade_alpha = D_L15_0015F330.col.b[3];

                if (D_L15_0015F330.reg == 0 && fade_alpha == 0xFF)
                    fade_alpha = 0x25;
                D_L15_0015F330.reg = 0x1F00000042L;
                if (fade_alpha < 0x25 - scale_game_frames(1))
                    fade_alpha = fade_alpha + scale_game_frames(1);
                else
                    fade_alpha = 0x25;
                D_L15_0015F330.col.rgba = (D_L15_0015F330.col.rgba & 0xFFFF00) | (fade_alpha << 24);
            } else {
                D_L15_0015F330.col.rgba = D_L15_0015F330.col.rgba | 0xFF000000;
            }
            if (moby_data->light != -1) {
                float light_charge_fraction;
                int   n;

                (D_L15_001808C0 + moby_data->light)->f10 = moby->pos[0];
                (D_L15_001808C0 + moby_data->light)->f14 = moby->pos[1];
                (D_L15_001808C0 + moby_data->light)->f18 = moby->pos[2];
                n = moby_data->f10;
                if (n > 0x32)
                    n = 0x32;
                if (n < 0)
                    n = 0;
                (D_L15_001808C0 + moby_data->light)->f1C =
                    convert_integer_to_float(n) * 7.0f / 50.0f;
                light_charge_fraction =
                    convert_integer_to_float(moby_data->f10) / scale_time(30.0f);
                if (light_charge_fraction > 1.0f)
                    light_charge_fraction = 1.0f;
                func_L00_0026AA10_29FF90(&D_L15_001808C0[moby_data->light].f10,
                                         random_int_between(0xF, 0x2F),
                                         random_int_between(0x2F, 0x3F),
                                         random_int_between(0x3F, 0x7F),
                                         light_charge_fraction * 4.0f * 210000.0f,
                                         0.0f,
                                         scale_game_frames(0x14));
                (D_L15_001808C0 + moby_data->light)->f0 =
                    random_float_between((D_L15_001808C0 + moby_data->light)->f0 * 0.8f,
                                         (D_L15_001808C0 + moby_data->light)->f1C * 2.0f / 7.0f);
                (D_L15_001808C0 + moby_data->light)->f4 =
                    random_float_between((D_L15_001808C0 + moby_data->light)->f0 <
                                                 (D_L15_001808C0 + moby_data->light)->f4 * 0.8f
                                             ? (D_L15_001808C0 + moby_data->light)->f0
                                             : (D_L15_001808C0 + moby_data->light)->f4 * 0.8f,
                                         (D_L15_001808C0 + moby_data->light)->f0);
            } else if ((D_L15_0015F5CC_macro & 3) == 0) {
                moby_data->light = allocate_packed_color_point_light_slot(moby->pos, 0.0f, 0.0f, 0);
            }
        } else {
            D_L15_0015F320.on = 0;
            D_L15_0015F330.on = 0;
            moby_data->f10 = 0;
            moby_data->vel[0] = fast_cos(((Hero29FF90 *)&hero)->f98) * (D_0015ED6C * 40.0f);
            moby_data->vel[1] = fast_sin(((Hero29FF90 *)&hero)->f98) * (D_0015ED6C * 40.0f);
            moby_data->vel[2] = 0.0f;
            moby->state = 1;
            moby_data->f14 = 0;
        }
        break;
    case EXPANDING_BLAST: {
        float r = D_0015ED6C * 40.0f * convert_integer_to_float(moby_data->f14) * 0.5f + 3.0f;
        int   capped_blast_remaining_frames;
        int   n;
        int   col;

        moby_data->f14++;
        if (moby_data->light != -1) {
            (D_L15_001808C0 + moby_data->light)->f1C =
                (D_L15_001808C0 + moby_data->light)->f1C * (frame_scale_macro * -0.6f + 1.0f);
            if ((D_L15_001808C0 + moby_data->light)->f1C < 0.0001f) {
                release_point_light_slot(moby_data->light);
                moby_data->light = -1;
            }
        }
        if (scale_game_frames(0x3C) < moby_data->f14 ||
            *(unsigned char *)(((char *)&D_001413F4)) != 2 ||
            (add_vectors(temporary_position, moby->pos, moby_data), moby->pos[0] < 2.0f) ||
            moby->pos[0] > 1021.0f || moby->pos[1] < 2.0f || moby->pos[1] > 1021.0f ||
            moby->pos[2] < 2.0f || moby->pos[2] > 1021.0f) {
            mark_moby_for_removal(charged_blast_removal_target);
            charged_blast_removal_target = 0;
            return;
        }
        n = func_L00_001F2BE8_29FF90(moby->pos, r, 0x10, moby, 0);
        *(u128_29FF90 *)temporary_position = *(u128_29FF90 *)moby->pos;
        send_blast_hits(
            moby, temporary_position, D_L15_00178500, n, 0, 0x8B0000, 2, 1, 20.0f, 1.0f, 1.0f);
        capped_blast_remaining_frames =
            scale_game_frames(0x3C) - moby_data->f14 < scale_game_frames(5)
                ? scale_game_frames(0x3C) - moby_data->f14
                : scale_game_frames(5);
        func_L00_0026E940_29FF90(r * 0.5f * 210000.0f,
                                 (char *)moby,
                                 (0x4F << 24) | (random_int_between(0x3F, 0x7F) << 16) |
                                     (random_int_between(0x2F, 0x3F) << 8) |
                                     random_int_between(0xF, 0x2F),
                                 capped_blast_remaining_frames,
                                 -1);
        random_spherical_offset(random_effect_vector, r * D_0015ED6C, r * 2.0f * D_0015ED6C);
        col = (0x7F << 24) | (random_int_between(0x3F, 0x7F) << 16) |
              (random_int_between(0x2F, 0x3F) << 8) | random_int_between(0xF, 0x2F);
        func_L00_0026CA10_29FF90(moby->pos,
                                 random_effect_vector,
                                 col,
                                 0x4F7F4F2F,
                                 210000.0f,
                                 scale_game_frames(0x2D),
                                 1,
                                 *D_L15_001B26AC,
                                 -1);
        {
            int i;

            for (i = 0; i < 8; i++) {
                Part29FF90 *p;

                random_spherical_offset(
                    random_effect_vector, r * D_0015ED6C, r * 2.0f * D_0015ED6C);
                random_spherical_offset(effect_vector_scratch, 0.0f, r * 0.5f);
                add_vectors(temporary_position, effect_vector_scratch, moby->pos);
                p = (Part29FF90 *)spawn_mode_controlled_rotating_alpha_fade_particle(
                    effect_vector_scratch, random_effect_vector, 0x7F, moby);
                if (p != 0) {
                    p->fC = random_float_between(6000.0f, 32000.0f) * r * 0.4f;
                }
            }
        }
        {
            int i;

            for (i = 0; i < 12; i++) {
                Part29FF90 *p = (Part29FF90 *)spawn_mode_controlled_rotating_alpha_fade_particle(
                    moby->pos, D_L15_0015F580, 0x7F, moby);

                if (p != 0) {
                    PartSub29FF90 *particle_subrecord = &p->sub;

                    if (i == 2 && random_integer_below(8) == 0) {
                        p->fC = 180000.0f;
                    } else {
                        p->fC = random_float_between(80000.0f, 120000.0f);
                    }
                    p->fC = p->fC * r;
                    p->fA = scale_game_frames(2);
                    particle_subrecord->f10 = 1.0f / convert_integer_to_float(p->fA);
                    particle_subrecord->f18 = 0x7F7F7F;
                    particle_subrecord->f16 = 3;
                }
            }
        }
        break;
    }
    }
    moby_data->active = 0;
}

unsigned char *D_L18_001B2AAC NOT_SDA = 0;

int charged_blast_removal_target __asm__("D_L15_00161484") = 0;

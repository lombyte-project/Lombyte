/* Level 11 moby class oil_tanker_link (class 0x4F1): link effects, bubbles and destruction. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/math/random/random_vector.h"

extern float frame_scale_inv_macro[] __asm__("D_0015ED68") MACRO_ADDR;
extern float D_L11_0015F580[] MACRO_ADDR;

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00319660), where it is exact; names translated to the US level program. */

char       *spawn_class_5f4_moby_with_random_rotation(void *position,
                                                      void *vector,
                                                      void *owner,
                                                      float scale) __asm__("FUN_L11_0031ada0");
extern char tide_level_macro[] __asm__("D_L11_00161E18") MACRO_ADDR;
extern char D_L11_0015FFBC[4] __attribute__((sda));
extern char oil_tanker_link_effect_spawn_random_limit_gp[4] __asm__("D_L11_00162340")
    __attribute__((sda));
extern char oil_tanker_link_effect_local_spawn_offset_gp[4] __asm__("D_L11_00162350")
    __attribute__((sda));
extern char oil_tanker_link_effect_scale_range_start_gp[4] __asm__("D_L11_00162360")
    __attribute__((sda));
extern char oil_tanker_link_effect_scale_range_end_gp[4] __asm__("D_L11_00162364")
    __attribute__((sda));
extern char oil_tanker_link_effect_random_motion_radius_scale_gp[4] __asm__("D_L11_00162368")
    __attribute__((sda));
extern char oil_tanker_link_fragment_velocity_scale_gp[4] __asm__("D_L11_0016236C")
    __attribute__((sda));
extern void  transform_vector_by_basis(void *, void *, void *);
extern char *func_L11_0031C210_5(void *, void *, void *, int, float) __asm__("FUN_L11_0031ada0");
extern void *func_L00_00265050_s(char *,
                                 int,
                                 void *,
                                 void *,
                                 int,
                                 int,
                                 float *,
                                 float *,
                                 float,
                                 float *) __asm__("FUN_L00_00263fd8");

void update_oil_tanker_link_effects_and_destruction(struct Moby *moby) __asm__("FUN_L11_003181f0");
extern float D_L11_0015F580[];
extern int   scale_game_frames(int);

void update_oil_tanker_link_effects_and_destruction(struct Moby *moby) {
    enum {
        TANKER_LINK_EMIT_EFFECT_AND_SAVE_POSITION = 0,
        TANKER_LINK_BURST_AND_HIDE = 1
    };
    char *link_vars = (char *)moby->pvars;
    float effect_scratch_vector[4];
    float effect_motion_vector[4];
    float random_motion_offset[4];
    switch (moby->state) {
    case TANKER_LINK_EMIT_EFFECT_AND_SAVE_POSITION:
        if (fast_dec_timer(link_vars + 0x28) == 0 &&
            random_integer_below(*(int *)&oil_tanker_link_effect_spawn_random_limit_gp) == 0 &&
            moby->visible != 0 && *(int *)&D_L11_0015FFBC >= 0x47) {
            transform_vector_by_basis(effect_scratch_vector,
                                      &oil_tanker_link_effect_local_spawn_offset_gp,
                                      &moby->rot_row0);
            fast_vec_add(effect_scratch_vector, effect_scratch_vector, &moby->pos);
            random_vector(random_motion_offset,
                          0.0f,
                          *(float *)&oil_tanker_link_effect_random_motion_radius_scale_gp *
                              frame_scale_inv_macro[1]);
            fast_vec_sub(effect_motion_vector, &moby->pos, link_vars);
            effect_motion_vector[2] -= frame_scale_inv_macro[1] + frame_scale_inv_macro[1];
            fast_vec_add(effect_motion_vector, effect_motion_vector, random_motion_offset);
            func_L11_0031C210_5(
                effect_scratch_vector,
                effect_motion_vector,
                tide_level_macro,
                0,
                random_float_between(*(float *)&oil_tanker_link_effect_scale_range_start_gp,
                                     *(float *)&oil_tanker_link_effect_scale_range_end_gp));
        }
        qcopy(link_vars, &moby->pos);
        break;
    case TANKER_LINK_BURST_AND_HIDE: {
        char  *link_position = &moby->pos;
        float *dv;
        char  *link_rotation;
        cart_burst(moby, link_position, -1, 5.0f, 13.0f);
        fast_vec_scale(effect_scratch_vector,
                       &moby->rot_row0,
                       (dv = D_L11_0015F580,
                        link_rotation = &moby->rot,
                        *(float *)&oil_tanker_link_fragment_velocity_scale_gp * frame_scale));
        effect_scratch_vector[2] += frame_scale * 0.08f;
        func_L00_00265050_s(moby,
                            0x785,
                            link_position,
                            link_rotation,
                            scale_game_frames(0x5A),
                            0,
                            effect_scratch_vector,
                            dv,
                            frame_time_sq * 12.0f,
                            dv);
        func_L00_00265050_s(moby,
                            0x786,
                            link_position,
                            link_rotation,
                            scale_game_frames(0x5A),
                            0,
                            effect_scratch_vector,
                            dv,
                            frame_time_sq * 12.0f,
                            dv);
        func_L00_00265050_s(moby,
                            0x787,
                            link_position,
                            link_rotation,
                            scale_game_frames(0x5A),
                            0,
                            effect_scratch_vector,
                            dv,
                            frame_time_sq * 12.0f,
                            dv);
        moby->state = 2;
        moby->visible = 0;
        moby->flags |= 1;
        break;
    }
    }
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031C210), where it is exact; names translated to the US level program. */

char *

spawn_class_5f4_moby_with_random_rotation(void *position, void *vector, void *owner, float scale) {
    char *moby = create_moby(0x5F4);
    if (moby != 0) {
        char *data = ((char *)MOBY(moby)->pvars);
        moby[0x20] = 1;
        qcopy(moby + 0x10, position);
        MOBY(moby)->rot.x = random_angle_radians();
        MOBY(moby)->rot.z = random_angle_radians();
        MOBY(moby)->draw_dist = 0xFF;
        ((unsigned char *)moby)[0x30] = 0xFF;
        moby[0x31] = 1;
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * scale;
        *(void **)(data + 0x24) = owner;
        qcopy(data, vector);
    }
    return moby;
}

char oil_tanker_link_effect_spawn_random_limit_gp[4] __asm__("D_L11_00162340") = {0xc, 0, 0, 0};

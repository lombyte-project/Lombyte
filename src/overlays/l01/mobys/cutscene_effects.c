/* Level 01 moby class cutscene_effects (0x60a): the camera targets, splashes, debris and explosions of the Novalis cutscenes. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "qcopy.h"

/* Cutscene state at D_L01_0016CCE0 (as struct L10Data in l10/mobys/cutscene_camera_target.c). */
typedef struct {
    u8           pad0[0x30];
    s32          scene;
    s32          frame;
    u8           pad38[0x178 - 0x38];
    struct Moby *slot[5];
} CutsceneFx;

typedef union {
    u128 q;
    f32  f[4];
} FxVec;

/* FlockTail/FlockPart in l00/mobys/cutscene_effects.c */
typedef struct {
    u8  pad0[4];
    s32 unk4;
    u8  pad8[2];
    u8  unkA;
    u8  unkB;
} FxSparkTail;

typedef struct {
    u8          pad0[9];
    u8          unk9;
    s16         unkA;
    u8          padC[0x20 - 0xC];
    FxSparkTail tail;
} FxSpark;

extern CutsceneFx fx_cutscene __asm__("D_L01_0016CCE0");
extern s32        cutscene_mode_macro __asm__("D_L01_0015F5C4") MACRO_ADDR;
extern s32        D_L01_0015F580;
/* MACRO_ADDR: retail rebuilds this address with lui/addiu at each use */
extern u128  fx_prev_pos __asm__("D_L01_00162070") MACRO_ADDR;
extern void  moby_point_position(struct Moby *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern s32   random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern char *spawn_splash(f32, f32, void *, void *, void *) __asm__("FUN_L00_002718d0");
extern void  spawn_debris_chunk(void *, void *, s32, s32) __asm__("FUN_L00_0026f548");
extern FxSpark             *
spawn_spark(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            f32,
                            f32,
                            s32,
                            s32,
                            s32,
                            f32,
                            f32,
                            f32,
                            f32,
                            s32,
                            f32,
                            s32,
                            s32,
                            s32,
                            s32) __asm__("FUN_L00_0025e450");
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");
extern void render_environment_mapped_object(void) __asm__("FUN_002327a0");

/* Cutscene effects: camera targets, splashes, sparks and trails keyed on the cutscene scene and frame. */
void level01_cutscene_effects_update(struct Moby *moby) __asm__("FUN_L01_0030c190");

/* Moby class 1546 */
void level01_cutscene_effects_update(struct Moby *moby) {
    enum {
        CUTSCENE_FX_STATE_INITIALIZE = 0,
        CUTSCENE_FX_STATE_UPDATE = 1
    };
    FxVec        effect_position_or_motion_delta;
    FxVec        debris_vector_or_effect_position;
    FxVec        primary_trail_position;
    FxVec        secondary_effect_position;
    struct Moby *cutscene_moby;
    char        *splash;
    FxSpark     *spark;
    f32          debris_horizontal_angle_radians;
    f32          debris_horizontal_magnitude;
    s32          i;
    s32          j;
    s32          debris_spawn_random_value;
    s32          color;
    s32          spread;
    s32          step;
    s32          spark_emitter_point_index;

    switch (moby->state) {
    case CUTSCENE_FX_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case CUTSCENE_FX_STATE_UPDATE:
        if (cutscene_mode_macro == 2) {
            if (fx_cutscene.scene == 1 ||
                (fx_cutscene.scene == 2 && fx_cutscene.frame <= scale_game_frames(148)) ||
                fx_cutscene.scene == 3 || fx_cutscene.scene == 4) {
                s32 target = 3;

                if (fx_cutscene.scene == 1 || fx_cutscene.scene == 3 || fx_cutscene.scene == 4) {
                    target = 3;
                } else if (fx_cutscene.scene == 2) {
                    target = 2;
                }
                cutscene_set_camera_target(fx_cutscene.slot[target]);
            }
            if (fx_cutscene.scene == 1) {
                if (fx_cutscene.frame >= scale_game_frames(472) &&
                    fx_cutscene.frame <= scale_game_frames(537) && fx_cutscene.slot[0] != 0) {
                    moby_point_position(fx_cutscene.slot[0], 1, &effect_position_or_motion_delta);
                    splash = spawn_splash(random_float_between(0.5f, 2.0f),
                                          random_integer_below(2) ? 2.0f : -2.0f,
                                          &effect_position_or_motion_delta,
                                          &D_L01_0015F580,
                                          &effect_position_or_motion_delta.f[2]);
                    if (splash != 0) {
                        *(s16 *)(splash + 0xA) = scale_game_frames(60);
                    }
                }
                if (fx_cutscene.scene == 1 && fx_cutscene.frame == scale_game_frames(472) &&
                    fx_cutscene.slot[0] != 0) {
                    moby_point_position(fx_cutscene.slot[0], 1, &effect_position_or_motion_delta);
                    {
                        char *burst = spawn_burst_moby(&effect_position_or_motion_delta, 3.0f);

                        if (burst != 0) {
                            burst[0x23] = 0x70;
                        }
                    }
                    for (i = 0; i < 16; i++) {
                        debris_horizontal_angle_radians = random_angle_radians();
                        debris_horizontal_magnitude =
                            random_float_between(frame_time * 0.0f, frame_time * 3.0f);
                        debris_vector_or_effect_position.f[0] =
                            fast_cos(debris_horizontal_angle_radians) * debris_horizontal_magnitude;
                        debris_vector_or_effect_position.f[1] =
                            fast_sin(debris_horizontal_angle_radians) * debris_horizontal_magnitude;
                        debris_vector_or_effect_position.f[2] =
                            random_float_between(frame_time * 3.0f, frame_time * 6.5f);
                        debris_spawn_random_value = random_int_between(90, 120);
                        spawn_debris_chunk(&effect_position_or_motion_delta,
                                           &debris_vector_or_effect_position,
                                           random_integer_below(2),
                                           debris_spawn_random_value);
                    }
                }
            }
            if (fx_cutscene.scene == 4) {
                cutscene_moby = fx_cutscene.slot[4];
                if (cutscene_moby != 0 && cutscene_moby->oclass == 0x213) {
                    add_draw_callback(render_environment_mapped_object, cutscene_moby);
                    if (fx_cutscene.frame >= scale_game_frames(850) &&
                        fx_cutscene.frame <= scale_game_frames(1160)) {
                        color = 0x7F204080;
                        for (spark_emitter_point_index = 1; spark_emitter_point_index < 7;
                             spark_emitter_point_index++) {
                            moby_point_position(cutscene_moby,
                                                spark_emitter_point_index,
                                                &effect_position_or_motion_delta);
                            for (j = 0; j < 2; j++) {
                                spread = random_integer_below(16);
                                spark = spawn_spark(&effect_position_or_motion_delta,
                                                    0.1f,
                                                    1.0f,
                                                    0.9f,
                                                    random_integer_below(2) == 0 ? spread : -spread,
                                                    &D_L01_0015F580,
                                                    200000.0f,
                                                    color);
                                if (spark != 0) {
                                    FxSparkTail *spark_tail = &spark->tail;

                                    if (fx_cutscene.frame >= scale_game_frames(1100)) {
                                        spark->unkA = scale_game_frames(30);
                                    } else {
                                        spark->unkA = scale_game_frames(6);
                                    }
                                    spark->unk9 = truncate_float_to_s32(4.0f) + 0x40;
                                    spark_tail->unk4 = 2;
                                    spark_tail->unkA = color >> 24;
                                    spark_tail->unkB = spark->unkA;
                                }
                            }
                        }
                    }
                }
            }
            if (fx_cutscene.scene == 5 && fx_cutscene.frame < scale_game_frames(120)) {
                cutscene_moby = fx_cutscene.slot[2];
                if (cutscene_moby != 0) {
                    effect_position_or_motion_delta.q = 0;
                    moby_point_position(cutscene_moby, 5, &debris_vector_or_effect_position);
                    if (fx_cutscene.frame >= 2) {
                        fast_vec_sub(&effect_position_or_motion_delta,
                                     &debris_vector_or_effect_position,
                                     &fx_prev_pos);
                        qcopy(&fx_prev_pos, &debris_vector_or_effect_position);
                    } else {
                        qcopy(&fx_prev_pos, &debris_vector_or_effect_position);
                    }
                    if (fx_cutscene.frame == scale_game_frames(118)) {
                        clear_u64_value(&primary_trail_position);
                        spawn_explosion(cutscene_moby,
                                        &primary_trail_position,
                                        &debris_vector_or_effect_position,
                                        0.0f,
                                        0.0f,
                                        3,
                                        200,
                                        200,
                                        5.0f,
                                        2.5f,
                                        9.0f,
                                        1.0f,
                                        -1,
                                        60.0f,
                                        0,
                                        60,
                                        -1,
                                        0);
                        qcopy(&secondary_effect_position, &debris_vector_or_effect_position);
                        secondary_effect_position.f[2] = secondary_effect_position.f[2] - 1.0f;
                        spawn_explosion(cutscene_moby,
                                        &primary_trail_position,
                                        &secondary_effect_position,
                                        0.0f,
                                        0.0f,
                                        300,
                                        2,
                                        2,
                                        1.0f,
                                        0.5f,
                                        9.0f,
                                        1.0f,
                                        -1,
                                        60.0f,
                                        0,
                                        0,
                                        -1,
                                        0);
                    }
                    add_draw_callback(render_environment_mapped_object, cutscene_moby);
                    moby_point_position(cutscene_moby, 1, &primary_trail_position);
                    moby_point_position(cutscene_moby, 2, &secondary_effect_position);
                    fast_vec_sub(&primary_trail_position,
                                 &primary_trail_position,
                                 &effect_position_or_motion_delta);
                    fast_vec_sub(&secondary_effect_position,
                                 &secondary_effect_position,
                                 &effect_position_or_motion_delta);
                    fast_vec_scale(
                        &effect_position_or_motion_delta, &effect_position_or_motion_delta, 0.25f);
                    for (step = 0; step < 4; step++) {
                        draw_trail(&primary_trail_position,
                                   &primary_trail_position,
                                   &D_L01_0015F580,
                                   150000.0f,
                                   60000.0f);
                        draw_trail(&secondary_effect_position,
                                   &secondary_effect_position,
                                   &D_L01_0015F580,
                                   150000.0f,
                                   60000.0f);
                        fast_vec_add(&primary_trail_position,
                                     &primary_trail_position,
                                     &effect_position_or_motion_delta);
                        fast_vec_add(&secondary_effect_position,
                                     &secondary_effect_position,
                                     &effect_position_or_motion_delta);
                    }
                }
            }
        }
        break;
    }
}

/* Level 01 moby classes: tall_shroom. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/mobys/cutscene_set_camera_target.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/shared/mobys/draw_trail.h"
#include "rnc/overlay/shared/vehicles/veldin_ship.h"

typedef struct {
    u8  pad0[0x5C];
    f32 f5c;
} Ent;

extern Ent l01_sine_modulated_scroll_layers[] __asm__("D_L01_00202D80");
extern s32 level_frame_count __asm__("D_L01_0015F5CC");
void       mine_blend_color(void) __asm__("FUN_L00_002371e0");
void       update_and_draw_scrolling_overlay_layers(s32, void *) __asm__("FUN_L01_002b96e0");

void draw_level01_scroll_layers(void) __asm__("FUN_L01_00309bf8");

void draw_level01_scroll_layers(void) {
    f32 normalized_sine_value;
    s32 i;

    mine_blend_color();
    update_and_draw_scrolling_overlay_layers(3, l01_sine_modulated_scroll_layers);
    normalized_sine_value =
        fast_sin((convert_integer_to_float(level_frame_count & 0x3F) - 32.0f) * 0.09817477f);
    normalized_sine_value = normalized_sine_value * 0.5f + 0.5f;
    for (i = 2; i >= 0; i--) {
        l01_sine_modulated_scroll_layers[i].f5c = normalized_sine_value;
    }
}

typedef struct {
    u8  pad0[0x30];
    u8  b30;
    u8  b31;
    s16 h32;
} FlagMoby;

extern void draw_level01_scroll_layers(void) __asm__("FUN_L01_00309bf8");
void        add_draw_callback(void *fn, void *arg) __asm__("FUN_001f4600");

/* Sets both flag fields to 0xFF and queues FUN_L01_00309bf8 for the moby. */
void set_moby_flags_and_queue_draw_callback(FlagMoby *moby) __asm__("FUN_L01_00309c98");

void set_moby_flags_and_queue_draw_callback(FlagMoby *moby) {
    int flag_init_value = 0xFF;
    moby->h32 = flag_init_value;
    moby->b30 = flag_init_value;
    add_draw_callback(draw_level01_scroll_layers, moby);
}

#include "rnc/hero/hero.h"
#include "rnc/game/usage_stats.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/level_state.h"
#include "sda.h"

/* Help message playback state (same layout as S00203908 in shared/ui/help/help_request.c). */
struct L01HelpState {
    s32 state;
    u8  pad4[0x20];
    s32 cur;
};

/* Per-message help record at D_00141968: id 0xFFFF = disabled. */
struct L01HelpRecord {
    u16 id;
    u16 pad;
    s32 v;
};

extern struct L01HelpState  help_state __asm__("D_L01_00179890");
extern struct L01HelpRecord help_records[] __asm__("D_00141968");
extern s32                  cutscene_mode_macro __asm__("D_L01_0015F5C4") MACRO_ADDR;
extern s32 D_L01_0015F688   MACRO_ADDR;
extern s32 D_L01_0015F68C   MACRO_ADDR;
extern struct Moby         *moby_list __asm__("D_L01_0015FFE4") MACRO_ADDR;
extern s32                  help_request(s32, s32) __asm__("FUN_L00_00203908");
extern s32                  is_moby_in_state_one(void *) __asm__("FUN_L00_002d8128");
extern s32                  check_scaled_vector_bsphere(f32, void *) __asm__("FUN_L01_00275690");

extern int D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
/* Novalis tutorial hint triggers: plays help messages and counts usage stats. */
void update_level01_tutorial_hints(struct Moby *moby) __asm__("FUN_L01_0030acb8");

void update_level01_tutorial_hints(struct Moby *moby) {
    enum {
        TUTORIAL_HINT_STATE_INITIALIZE = 0,
        TUTORIAL_HINT_STATE_UPDATE = 1
    };
    s32         *data;
    struct Moby *hint_candidate_moby;
    struct Moby *health_hint_candidate_moby;
    s32          usage_time_value;

    data = (s32 *)moby->pvars;
    fast_dec_timer(&data[6]);
    switch (moby->state) {
    case TUTORIAL_HINT_STATE_INITIALIZE:
        data[5] = 0;
        moby->update_dist = 0xFF;
        moby->state = 1;
        data[10] = 0;
        break;
    case TUTORIAL_HINT_STATE_UPDATE:
        if (level_frame_count < scale_game_frames(400) && usage_stats.stat[12].count != 0 &&
            usage_stats.stat[11].count == 0) {
            if (scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[12].unk2 * 600 <
                    scale_game_frames(1100) &&
                help_records[0x51].v >= 0) {
                help_request(0x4E27, 0x51);
            }
        }
        if (level_frame_count < scale_game_frames(400) && usage_stats.stat[29].count != 0) {
            if (scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[29].unk2 * 600 <
                    scale_game_frames(1100) &&
                help_records[0x72].v >= 0) {
                help_request(0x4E28, 0x72);
            }
        }
        if (cutscene_mode_macro == 0) {
            if (level_available[3] != 0 && level_available[2] == 0 && help_state.state == 0 &&
                help_state.cur == -1 && help_records[4].id == 0) {
                help_request(0x3E8, 4);
            } else if (level_available[3] == 0 && level_available[2] != 0 &&
                       help_state.state == 0 && help_state.cur == -1 && help_records[5].id == 0) {
                help_request(0x3E9, 5);
            } else if (level_available[3] != 0 && level_available[2] != 0 &&
                       help_state.state == 0 && help_state.cur == -1 && help_records[6].id == 0 &&
                       help_records[4].id == 0) {
                help_request(0x3EA, 6);
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, data[15]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            help_state.state == 0 && help_state.cur == -1) {
            usage_time_value = scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[9].unk2 * 600;
            /* "< 1" rather than "!": cc1 would otherwise test the bit as (v >> level) & 1 */
            if (scale_game_frames(216000) < usage_time_value &&
                (u32)(help_records[0x43].v & (1 << current_level_index_macro)) < 1) {
                help_request(0x3EF, 0x43);
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, data[14]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            D_L01_0015F68C < 3 && help_state.state == 0 && help_state.cur == -1 &&
            help_records[0x40].id == 0) {
            help_request(0x3EC, 0x40);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, data[14]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            hero.state.current == 1 && D_L01_0015F688 == -1 && help_state.state == 0 &&
            help_state.cur == -1 && help_records[0x40].id != 0 && help_records[0x41].id == 0) {
            help_request(0x3ED, 0x41);
        }
        if (help_records[0x42].id < 2 && is_point_inside_clip_volume(&hero.motion.pos, data[12]) &&
            hero.state.prev == 0x3B && hero.state_timer == 0) {
            for (hint_candidate_moby = moby_list; hint_candidate_moby != 0;
                 hint_candidate_moby = hint_candidate_moby->next) {
                if (hint_candidate_moby->oclass == 0x118 &&
                    fast_vec_dist(&hint_candidate_moby->pos, &hero.motion.pos) < 3.0f &&
                    *(f32 *)hint_candidate_moby->pvars != 1.0f) {
                    data[13]++;
                    if (data[13] == 3 || data[13] == 6) {
                        help_request(0x3EE, 0x42);
                    }
                }
            }
        }
        if (hero.state.control_mode == 0x12 && data[10] == 0 && help_state.state == 0 &&
            help_state.cur == -1 &&
            (is_point_inside_clip_volume(&hero.motion.pos, data[8]) ||
             is_point_inside_clip_volume(&hero.motion.pos, data[9]))) {
            data[11]++;
            if (is_point_inside_clip_volume(&hero.motion.pos, data[8]) &&
                scale_game_frames(240) < data[11] && usage_stats.stat[22].count == 0) {
                help_request(0x3F0, 0x44);
                if (usage_stats.stat[22].count < 0xFFFF) {
                    usage_stats.stat[22].count++;
                }
                usage_time_value = scale_game_frames(D_0015EEA4_macro) / 600;
                if (usage_stats.stat[22].unk2 < usage_time_value) {
                    usage_stats.stat[22].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                }
                usage_stats.stat[22].level_mask =
                    usage_stats.stat[22].level_mask | (1 << current_level_index_macro) | 0x80000000;
            } else if (is_point_inside_clip_volume(&hero.motion.pos, data[9]) &&
                       scale_game_frames(360) < data[11] && usage_stats.stat[23].count == 0) {
                help_request(0x3F0, 0x44);
                if (usage_stats.stat[23].count < 0xFFFF) {
                    usage_stats.stat[23].count++;
                }
                usage_time_value = scale_game_frames(D_0015EEA4_macro) / 600;
                if (usage_stats.stat[23].unk2 < usage_time_value) {
                    usage_stats.stat[23].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                }
                usage_stats.stat[23].level_mask =
                    usage_stats.stat[23].level_mask | (1 << current_level_index_macro) | 0x80000000;
            }
        } else if (hero.state.control_mode == 0x11) {
            help_records[0x44].id = 0xFFFF;
        } else {
            data[11] = 0;
        }
        for (health_hint_candidate_moby = moby_list; health_hint_candidate_moby != 0;
             health_hint_candidate_moby = health_hint_candidate_moby->next) {
            if (health_hint_candidate_moby->oclass == 0x326 &&
                fast_vec_dist(&health_hint_candidate_moby->pos, &hero.motion.pos) < 10.0f &&
                help_state.state == 0 && help_state.cur == -1 && help_records[1].id == 0 &&
                help_records[2].id == 0 && usage_stats.stat[13].count == 0 &&
                is_moby_in_state_one(health_hint_candidate_moby) == 0) {
                if (hero.health.hp < hero_max_health) {
                    if (usage_stats.stat[13].count < 0xFFFF) {
                        usage_stats.stat[13].count++;
                    }
                    usage_time_value = scale_game_frames(D_0015EEA4_macro) / 600;
                    if (usage_stats.stat[13].unk2 < usage_time_value) {
                        usage_stats.stat[13].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                    }
                    usage_stats.stat[13].level_mask = usage_stats.stat[13].level_mask |
                                                      (1 << current_level_index_macro) | 0x80000000;
                    help_request(1, 1);
                } else if (check_scaled_vector_bsphere(255.0f, health_hint_candidate_moby) != 0) {
                    help_request(2, 2);
                }
            }
        }
        break;
    }
}

#include "rnc/mobys/moby.h"

/* pvars of the lava blob FUN_L01_0030ba18 moves */
typedef struct {
    Vec4f vel;   /* 0x00 */
    Vec4f spin;  /* 0x10 */
    s32   timer; /* 0x20 */
    s32   owner; /* 0x24 */
} BlobVars;

typedef struct {
    u8    pad0[0x1C];
    s32   have_normal; /* 0x1C */
    u8    pad20[0x20];
    Vec4f normal; /* 0x40 */
} BlobHitInfo;

#include "rnc/overlay/l01/mobys/tall_shroom_globals.h"

extern f32 explosive_moby_gravity_coefficient_gp __asm__("D_L01_00162058") __attribute__((sda));
extern f32 explosive_moby_velocity_multiplier_gp __asm__("D_L01_0016205C") __attribute__((sda));
extern f32 explosive_moby_horizontal_damping_threshold_gp __asm__("D_L01_00162060")
    __attribute__((sda));
extern BlobHitInfo collision_hit __asm__("D_L01_001742C0");
extern s32         D_L01_0015F580;
extern void        draw_moby_offset_textured_quad(void) __asm__("FUN_L01_0030bfa8");
extern int         coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void        FUN_L00_0025e450(void *,
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
                                    s32,
                                    f32,
                                    f32,
                                    s32,
                                    s32,
                                    s32,
                                    s32);
extern void        spawn_burst(void *, void *, s32, s32, f32, s32, s32) __asm__("FUN_L00_0026cbb0");
extern int         FUN_L00_00257b90(int lo, int hi);
extern u8 *spawn_puff(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");

void update_explosive_moby(struct Moby *moby) __asm__("FUN_L01_0030ba18");

void update_explosive_moby(struct Moby *moby) {
    BlobVars *blob_vars;
    s32       hit;
    s32       i;
    s32       signed_puff_variation;
    s32       grey_intensity;
    s32       alpha;
    s32       grey;
    f32       s;
    u8       *puff;
    u8       *puff_data;

    blob_vars = (BlobVars *)moby->pvars;
    if (random_integer_below(2)) {
        add_draw_callback(draw_moby_offset_textured_quad, moby);
    }
    fast_vec_add(&moby->pos, &moby->pos, blob_vars);
    if (absolute_float(blob_vars->vel.x) >
        explosive_moby_horizontal_damping_threshold_gp * frame_scale) {
        blob_vars->vel.x *= (explosive_moby_velocity_multiplier_gp - 1.0f) * frame_scale + 1.0f;
    }
    if (absolute_float(blob_vars->vel.y) >
        explosive_moby_horizontal_damping_threshold_gp * frame_scale) {
        blob_vars->vel.y *= (explosive_moby_velocity_multiplier_gp - 1.0f) * frame_scale + 1.0f;
    }
    if (blob_vars->vel.z > 0.0f) {
        blob_vars->vel.z *= (explosive_moby_velocity_multiplier_gp - 1.0f) * frame_scale + 1.0f;
    }
    blob_vars->vel.z -= explosive_moby_gravity_coefficient_gp * frame_time_sq;
    moby->rot.x = fast_add_rotations(moby->rot.x, blob_vars->spin.x);
    moby->rot.y = fast_add_rotations(moby->rot.y, blob_vars->spin.y);
    moby->rot.z = fast_add_rotations(moby->rot.z, blob_vars->spin.z);
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    hit = coll_sphere(2.0f, &moby->pos, 0, (void *)blob_vars->owner);
    if (fast_dec_timer(&blob_vars->timer) || hit) {
        if (hit) {
            if (collision_hit.have_normal) {
                reflect_vector(blob_vars, blob_vars, &collision_hit.normal);
            } else {
                clear_u64_value(blob_vars);
            }
        }
        FUN_L00_0025e450(moby,
                         blob_vars,
                         &moby->pos,
                         0.0f,
                         0.0f,
                         0x14,
                         3,
                         4,
                         4.0f,
                         2.0f,
                         100000.0f,
                         0,
                         3.0f,
                         15.0f,
                         1,
                         1,
                         -1,
                         0);
        mark_moby_for_removal(moby);
        return;
    }
    s = random_float_between(0.5f, 1.5f) * 210000.0f;
    spawn_burst(&moby->pos,
                &D_L01_0015F580,
                0x4F007FFF,
                0x1FFFFFFF,
                s,
                FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)),
                1);
    alpha = 0x60000000;
    for (i = 0; i < 2; i++) {
        signed_puff_variation = random_integer_below(6);
        if (random_integer_below(2)) {
            signed_puff_variation = -signed_puff_variation;
        }
        s = random_float_between(40000.0f, 100000.0f);
        grey_intensity = FUN_L00_00257b90(0x30, 0xFF);
        grey = (grey_intensity << 8) | alpha;
        puff = spawn_puff(&moby->pos,
                          0.025f,
                          1.0f,
                          1.0f,
                          signed_puff_variation,
                          &D_L01_0015F580,
                          s,
                          grey_intensity | ((grey_intensity << 16) | grey));
        if (puff != NULL) {
            puff[3] = 0x44;
            puff_data = puff + 0x20;
            *(s16 *)(puff + 0xA) = scale_game_frames(60);
            *(s32 *)(puff_data + 4) = 2;
            puff_data[0xA] = 0x60;
            puff_data[0xB] = puff[0xA];
        }
    }
}

#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    u8    pad0[0x10];
    f32   spin_x;
    f32   spin_y;
    f32   spin_z;
    u8    pad1c[4];
    s32   timer;
    void *parent;
} DebrisVars;

typedef struct {
    u8          pad0[0x10];
    u128        pos;
    u8          state;
    u8          pad21[0xB];
    f32         scale;
    u8          b30;
    u8          b31;
    s16         h32;
    u8          pad34[0xC];
    u128        rot;
    u8          pad50[0x28];
    DebrisVars *vars;
} DebrisMoby;

extern f32 randf_sym(f32 lo, f32 hi) __asm__("FUN_L00_00257c48");

DebrisMoby *spawn_debris(DebrisMoby *parent_moby,
                         void       *debris_vector_source,
                         f32         scale) __asm__("FUN_L01_0030be70");

DebrisMoby *spawn_debris(DebrisMoby *parent_moby, void *debris_vector_source, f32 scale) {
    DebrisMoby *spawned_moby;
    DebrisVars *debris_vars;

    spawned_moby = create_moby(0x5E6);
    if (spawned_moby != NULL) {
        debris_vars = spawned_moby->vars;
        debris_vars->parent = parent_moby;
        qcopy(&spawned_moby->pos, &parent_moby->pos);
        spawned_moby->rot = parent_moby->rot;
        spawned_moby->state = 0;
        spawned_moby->b30 = 0xFF;
        spawned_moby->h32 = 0xFF;
        spawned_moby->b31 = 1;
        spawned_moby->scale = spawned_moby->scale * scale;
        fast_vec_normalize(debris_vars, debris_vector_source, random_float_between(0.8f, 1.2f));
        debris_vars->spin_x = randf_sym(0.0f, 0.008726646f);
        debris_vars->spin_y = randf_sym(0.05235988f, 0.13962634f);
        debris_vars->spin_z = randf_sym(0.0f, DEG_TO_RAD);
        debris_vars->timer = scale_game_frames(200);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/* Cutscene state at D_L01_0016CCE0 (as struct L10Data in l10/mobys/update_configurable_particle_emitter.c). */
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

/* FlockTail/FlockPart in l00/mobys/veldin_robot_enemy.c */
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
void update_level01_cutscene_effects(struct Moby *moby) __asm__("FUN_L01_0030c190");

void update_level01_cutscene_effects(struct Moby *moby) {
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
    s32          node;

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
                        for (node = 1; node < 7; node++) {
                            moby_point_position(
                                cutscene_moby, node, &effect_position_or_motion_delta);
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

typedef struct {
    u8  pad0[0x2C];
    f32 f2c;
} TriggerHit;

typedef struct {
    u8 pad0[0x10];
    u8 pos[0x10];
    u8 state;
    u8 pad21[0x1F];
    u8 rot[0x10];
} TriggerMoby;

void        *find_moby_hit(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern void  spawn_proximity_gated_bolts() __asm__("FUN_L01_002787a0");
extern void *FUN_L00_00263fd8(void *, s32, void *, void *, s32, s32, void *, void *, f32, void *);
extern void  spawn_moby_from_placement() __asm__("FUN_L01_00278e20");

void tall_shroom_update(TriggerMoby *obj) __asm__("FUN_L01_0030d0f0");

void tall_shroom_update(TriggerMoby *shroom_moby) {
    enum {
        TALL_SHROOM_STATE_INITIALIZE = 0,
        TALL_SHROOM_STATE_WAIT_FOR_HIT = 1,
        TALL_SHROOM_STATE_REMOVE_WITH_EFFECTS = 2
    };
    TriggerHit *hit;
    s32         flag;

    flag = 0;
    hit = find_moby_hit(shroom_moby, 0x10000, 0);
    switch (shroom_moby->state) {
    case TALL_SHROOM_STATE_INITIALIZE:
        shroom_moby->state = 1;
        break;
    case TALL_SHROOM_STATE_WAIT_FOR_HIT:
        if (hit != 0 && hit->f2c > 0.0f) {
            flag = 1;
        }
        if (flag) {
            shroom_moby->state = 2;
        }
        break;
    case TALL_SHROOM_STATE_REMOVE_WITH_EFFECTS:
        allocate_voice_for_target_entry(0, 0, shroom_moby);
        spawn_proximity_gated_bolts(shroom_moby);
        FUN_L00_00263fd8(shroom_moby,
                         0x717,
                         shroom_moby->pos,
                         shroom_moby->rot,
                         0,
                         0,
                         &D_L01_0015F580,
                         &D_L01_0015F580,
                         0.0f,
                         &D_L01_0015F580);
        spawn_moby_from_placement(shroom_moby, 0x718);
        mark_moby_for_removal(shroom_moby);
        break;
    }
}

typedef struct {
    f32 x, y, z, w;
} EnvVec;

extern EnvVec camera_pos __asm__("D_L01_00167240");
extern f32    level01_environment_map_uv_scroll_offsets[2] __asm__("D_L01_00162110");

void generate_level01_environment_map_uvs(s32 vertex_count,
                                          f32 (*normals)[3],
                                          f32 (*verts)[3],
                                          s32 unused,
                                          f32 (*uv)[2]) __asm__("FUN_L01_0030ef18");

void generate_level01_environment_map_uvs(

    s32 vertex_count, f32 (*normals)[3], f32 (*verts)[3], s32 unused, f32 (*uv)[2]) {
    EnvVec eye;
    EnvVec vertex_position;
    EnvVec refl;
    EnvVec nrm;
    s32    i;
    f32    uv_projection_denominator;

    for (i = 0; i < vertex_count; i++) {
        vertex_position.w = 1.0f;
        vertex_position.x = verts[i][0];
        vertex_position.y = verts[i][1];
        vertex_position.z = verts[i][2];
        fast_vec_sub(&eye, &vertex_position, &camera_pos);
        fast_vec_normalize(&eye, &eye, 1.0f);
        nrm.w = 1.0f;
        nrm.x = -normals[i][0];
        nrm.y = -normals[i][1];
        nrm.z = -normals[i][2];
        fast_vec_normalize(&nrm, &nrm, 1.0f);
        fast_vec_scale(&refl, &nrm, fast_vec_dot(&nrm, &eye) * 2.0f);
        fast_vec_sub(&refl, &eye, &refl);
        fast_vec_normalize(&refl, &refl, 1.0f);
        refl.z += 1.0f;
        uv_projection_denominator = fast_vec_length(&refl) * 2.0f;
        uv[i][0] = (refl.x / uv_projection_denominator + 0.5f) * 2.0f +
                   level01_environment_map_uv_scroll_offsets[0];
        uv[i][1] = (refl.y / uv_projection_denominator + 0.5f) * 2.0f +
                   level01_environment_map_uv_scroll_offsets[1];
    }
}

extern s32 level01_environment_mesh_vertex_counts[] __asm__("D_L01_00202EA0");
extern s32 level01_environment_mesh_position_addresses[] __asm__("D_L01_00208108");
extern s32 D_L01_00208120[];
extern s32 level01_environment_mesh_normal_addresses[] __asm__("D_L01_00208138");
extern s32 level01_environment_mesh_vertex_attribute_addresses[] __asm__("D_L01_00208150");
extern u8  level01_environment_mesh_uv_scratch[] __asm__("D_L01_00208170");
extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
void       generate_level01_environment_map_uvs();
void       FUN_L00_001fde98(s32, s32, s32, void *, s32);

void draw_level01_environment_mapped_meshes(void) __asm__("FUN_L01_0030f0e0");

void draw_level01_environment_mapped_meshes(void) {
    s32 i;

    vu1_add_g_sregister(6, get_effect_texture(0x28));
    vu1_add_g_sregister(0x42, 0x2000000064);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 5; i++) {
        generate_level01_environment_map_uvs(level01_environment_mesh_vertex_counts[i],
                                             level01_environment_mesh_normal_addresses[i],
                                             level01_environment_mesh_position_addresses[i],
                                             D_L01_00208120[i],
                                             level01_environment_mesh_uv_scratch);
        FUN_L00_001fde98(level01_environment_mesh_vertex_counts[i],
                         level01_environment_mesh_position_addresses[i],
                         level01_environment_mesh_vertex_attribute_addresses[i],
                         level01_environment_mesh_uv_scratch,
                         1);
    }
}

typedef struct {
    u8 pad0[0x20];
    u8 state;
} ScrollMoby;

extern f32 frame_delta_time __asm__("D_0015ED7C");

void update_level01_environment_map_uv_scroll(ScrollMoby *moby) __asm__("FUN_L01_0030f208");

void update_level01_environment_map_uv_scroll(ScrollMoby *moby) {
    enum {
        UV_SCROLL_STATE_INITIALIZE = 0,
        UV_SCROLL_STATE_UPDATE_AND_QUEUE_DRAW = 1
    };
    f32 uv_scroll_delta;

    switch (moby->state) {
    case UV_SCROLL_STATE_INITIALIZE:
        level01_environment_map_uv_scroll_offsets[0] = 0.0f;
        level01_environment_map_uv_scroll_offsets[1] = 0.0f;
        moby->state = 1;
        break;
    case UV_SCROLL_STATE_UPDATE_AND_QUEUE_DRAW:
        uv_scroll_delta = frame_delta_time * 0.025f;
        level01_environment_map_uv_scroll_offsets[0] += uv_scroll_delta;
        level01_environment_map_uv_scroll_offsets[1] += uv_scroll_delta;
        if (level01_environment_map_uv_scroll_offsets[0] > 1.0f) {
            level01_environment_map_uv_scroll_offsets[0] -= 1.0f;
        }
        if (level01_environment_map_uv_scroll_offsets[0] < -1.0f) {
            level01_environment_map_uv_scroll_offsets[0] += 1.0f;
        }
        if (level01_environment_map_uv_scroll_offsets[1] > 1.0f) {
            level01_environment_map_uv_scroll_offsets[1] -= 1.0f;
        }
        if (level01_environment_map_uv_scroll_offsets[1] < -1.0f) {
            level01_environment_map_uv_scroll_offsets[1] += 1.0f;
        }
        add_draw_callback(draw_level01_environment_mapped_meshes, moby);
        break;
    }
}

f32 level01_environment_map_uv_scroll_offsets[2] __asm__("D_L01_00162110") = {0};

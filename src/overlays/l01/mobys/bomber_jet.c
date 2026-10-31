/* Level 01 moby class bomber_jet. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/l01/hero/hero_set_state.h"
#include "rnc/overlay/shared/vehicles/gunship.h"

#include "rnc/overlay/level_path.h"
#include "rnc/overlay/level_labels.h"

extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern s32  scale_game_frames(s32) __asm__("FUN_001f96f8");

#include "rnc/overlay/cuboid.h"

extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern int   cutscene_active __asm__("D_L01_0015F404");
extern int   collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             float,
                             float,
                             int,
                             int,
                             int,
                             float,
                             float,
                             float,
                             float,
                             int,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void  jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  jet_end_mount(int) __asm__("FUN_L00_002eac18");
extern void  fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void update_entity(void *) __asm__("FUN_L00_00250df8");

#include "qcopy.h"

typedef struct {
    Vec4  dir;
    u128  color;
    u8    pad20[0x10];
    void *owner;
    s32   kind;
    f32   len;
} BeamVars;

typedef struct {
    u8        pad0[0x10];
    u128      pos;
    u8        state;
    u8        pad21[0xB];
    f32       scale;
    u8        b30;
    u8        b31;
    s16       h32;
    u8        pad34[0xC];
    s32       w40;
    f32       rot_x;
    f32       rot_z;
    u8        pad4c[0x2C];
    BeamVars *vars;
} BeamMoby;

extern void update_entity() __asm__("FUN_L00_00250df8");
extern void spawn_source_attached_fading_effect_particle(void *, u32, s32, s32, f32) __asm__("FUN_L00_0026daa0");

BeamMoby *FUN_L01_002f68a0(void *owner, u128 *pos, u128 *dir, u128 *color, s32 kind) {
    Vec4      position_copy;
    Vec4      direction_copy;
    Vec4      target_position_copy;
    BeamMoby *created_moby;
    BeamVars *beam_vars;
    Vec4     *pp;
    Vec4     *pd;
    Vec4     *pc;

    pp = &position_copy;
    pd = &direction_copy;
    pc = &target_position_copy;
    position_copy.q = *pos;
    direction_copy.q = *dir;
    target_position_copy.q = *color;
    created_moby = create_moby(0x2AE);
    if (created_moby != NULL) {
        beam_vars = created_moby->vars;
        beam_vars->owner = owner;
        created_moby->w40 = 0;
        created_moby->b30 = 0xFF;
        created_moby->h32 = 0xFF;
        created_moby->b31 = 1;
        created_moby->scale = created_moby->scale * 0.25f;
        created_moby->rot_x = -fast_arc_tan(vector_length_xy(pd), pd->f[2]);
        created_moby->rot_z = fast_arc_tan(direction_copy.f[0], pd->f[1]);
        qcopy(&created_moby->pos, pp);
        qcopy(&beam_vars->dir, pd);
        qcopy(&beam_vars->color, pc);
        beam_vars->kind = kind;
        beam_vars->len = fast_vec_length(pd);
        spawn_source_attached_fading_effect_particle(
            created_moby, 0x2F4F7F7F, scale_game_frames(0x78), -1, 420000.0f);
        spawn_source_attached_fading_effect_particle(
            created_moby, 0x4F6F7F7F, scale_game_frames(0x78), -1, 210000.0f);
        allocate_voice_for_target_entry(0, 0, created_moby);
        update_entity(created_moby);
    }
    return created_moby;
}

typedef struct {
    s32 v[4];
} GunshipTargets;

typedef struct {
    s32 cur;
    u8  pad4[0x10];
    s32 path;
} GunshipFollow;

typedef struct {
    u8            pad0[0x2B];
    u8            b2B;
    u8            b2C;
    u8            pad2D[0x33];
    GunshipFollow sub;
    u8            pad78[0x58];
    f32           vD0[4];
    f32           vE0[4];
    u8            padF0[0xC];
    f32           speed;
    u8            pad100[0x40];
    s32           tethers[4];
    u8            pad150[0x1C];
    s32           last_fire_node;
    s32           trigger_volume;
    s32           cutscene_camera;
    f32           base_scale;
    f32           base_speed;
    u8            stop_nodes[4];
    s16           gun_side;
    s16           cutscene_timer;
    s32           camera_block_volume;
    s32           beam_volume;
    s32           targets[4][4];
    s32           guards[8];
    s32           turrets[4];
} GunshipVars;

typedef struct {
    u8    pad0[0x10];
    f32   pos[4];
    u8    state;
    u8    pad21[2];
    u8    alpha;
    u8    pad24[8];
    f32   scale;
    u8    b30;
    u8    pad31;
    s16   h32;
    u16   flags;
    u8    pad36[0x42];
    void *vars;
    u8    pad7C[0x18];
    s32   w94;
    u8    pad98[0xC];
    u8    bA4;
    u8    padA5;
    s16   oclass;
    u8    padA8[0xA];
    u16   uid;
    u8    padB4[0x4C];
} GunshipMoby;

typedef struct {
    u8  pad0[0x1E8];
    s32 activated;
    s16 activate_delay;
} GunshipTurretVars;

typedef struct {
    s32 f0;
    s16 f4;
    s16 pad6;
    s32 f8;
} GunshipGuardVars;

typedef struct {
    s32 count;
    u8  pad4[0x18];
    f32 w;
} GunshipPath;

typedef struct {
    u8           pad0[0x18];
    GunshipMoby *moby;
} GunshipHit;

f32 gunship_min_scale __asm__("D_L01_00161BD0") = 0.5f;
f32 gunship_far_dist __asm__("D_L01_00161BD4") = 2e+02f;
f32 gunship_near_dist __asm__("D_L01_00161BD8") = 2e+01f;

GunshipTargets gunship_no_targets __asm__("D_L01_00161BE0") = {
    {-1, -1, -1, -1}
};

extern char         camera_pos[] __asm__("D_L01_00167240");
extern char         gunship_no_path_fmt[] __asm__("D_L01_0020B4A0");
extern u8           moby_collected[] __asm__("D_L01_001BBB04");
extern GunshipHit   collision_hit __asm__("D_L01_001742C0");
extern s32          ents_macro __asm__("D_L01_0015FFD8") MACRO_ADDR;
extern LevelCuboid *l01_cuboids_macro __asm__("D_L01_001600EC") MACRO_ADDR;
extern s32          cutscene_active __asm__("D_L01_0015F404") MACRO_ADDR;

extern void allocate_voice_for_bank_entry(s32, s32, s32) __asm__("FUN_0022db10");
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
extern void spawn_debris(char *, void *, f32) __asm__("FUN_L01_0030be70");
extern int  debug_print() __asm__("FUN_001e93b0");
extern void jet_mount(void *, void *, s32, s32, s32) __asm__("FUN_L00_002eaaa0");
extern void jet_end_mount(s32) __asm__("FUN_L00_002eac18");
extern s32  collision_line(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");
extern GunshipMoby *
spawn_beam(void *, void *, void *, void *, s32, s32) __asm__("FUN_L01_002f68a0");

extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
/* Ported from rac1-decomp src/overlays/l01_novalis/vendor_002BA898.c (func_L01_002F8B00) */
void bomber_jet_update(GunshipMoby *m) __asm__("FUN_L01_002f7728");

/* Moby class 688 */
void bomber_jet_update(GunshipMoby *bomber_jet_moby) {
    GunshipVars   *gunship_vars = bomber_jet_moby->vars;
    GunshipFollow *sub = &gunship_vars->sub;
    float          distance_scale;
    void          *hit;
    int            i;
    int            j;
    GunshipTargets explosion_vector_or_target_indices;
    GunshipTargets explosion_vector_or_filtered_target_indices;
    Vec4           beam_step_vector;
    Vec4           pos;
    Vec4           col;
    {
        float camera_distance = fast_vec_dist(bomber_jet_moby->pos, camera_pos);
        float x0 = gunship_near_dist;
        float k = -((1.0f - gunship_min_scale) / (gunship_far_dist - x0));
        distance_scale = k * camera_distance + (1.0f - k * x0);
    }
    hit = find_moby_hit(bomber_jet_moby, 0x800000, 0);
    bomber_jet_moby->bA4 = 0xFF;
    if (hit != 0) {
        if (item_unlocked[0] == 0) {
            item_unlocked[0] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        fast_vec_sub(
            &explosion_vector_or_filtered_target_indices, gunship_vars->vD0, gunship_vars->vE0);
        *(u128 *)&explosion_vector_or_target_indices =
            *(u128 *)&explosion_vector_or_filtered_target_indices;
        spawn_explosion(bomber_jet_moby,
                        &explosion_vector_or_target_indices,
                        bomber_jet_moby->pos,
                        0.0f,
                        0.0f,
                        0x14,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        3.0f,
                        1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        spawn_debris(
            (char *)bomber_jet_moby, &explosion_vector_or_target_indices, distance_scale * 2.0f);
        for (i = 0; i < 4; i++) {
            if (gunship_vars->tethers[i] != -1) {
                mark_moby_for_removal(
                    (GunshipMoby *)(ents_macro + (gunship_vars->tethers[i] << 8)));
            }
        }
        mark_moby_for_removal(bomber_jet_moby);
        return;
    }
    do {
        if (bomber_jet_moby->state == 0) {
            if (sub->path != -1) {
                GunshipPath *path = (GunshipPath *)l01_paths[sub->path];
                j = 0;
                gunship_vars->speed = gunship_vars->speed * 2.0f;
                if (fast_vec_dist((char *)path + 0x10, (char *)path + (path->count << 4)) < 0.5f) {
                    path->count--;
                }
                for (i = 0; i < path->count; i++) {
                    if (((GunshipPath *)((char *)path + (i << 4)))->w == 1.0f) {
                        gunship_vars->stop_nodes[j] = i;
                        j++;
                        if (j >= 4)
                            break;
                    }
                }
                gunship_vars->gun_side = 0;
                gunship_vars->last_fire_node = -1;
                gunship_vars->cutscene_timer = -1;
                gunship_vars->base_scale = bomber_jet_moby->scale;
                gunship_vars->base_speed = gunship_vars->speed;
            } else {
                debug_print(
                    gunship_no_path_fmt,
                    bomber_jet_moby->oclass,
                    truncate_float_to_s32((float)(((int)bomber_jet_moby - ents_macro) >> 8)));
            }
            bomber_jet_moby->b30 = 0xFF;
            bomber_jet_moby->h32 = 0xFF;
        }
        if (1.0f < distance_scale)
            distance_scale = 1.0f;
        if (distance_scale < gunship_min_scale)
            distance_scale = gunship_min_scale;
        bomber_jet_moby->scale = gunship_vars->base_scale * distance_scale;
        gunship_vars->speed = gunship_vars->base_speed * distance_scale;
        if (175.0f <= bomber_jet_moby->pos[2]) {
            bomber_jet_moby->alpha = 0;
        } else if (125.0f < bomber_jet_moby->pos[2] && bomber_jet_moby->pos[2] < 175.0f) {
            bomber_jet_moby->alpha =
                truncate_float_to_s32((175.0f - bomber_jet_moby->pos[2]) * 128.0f / 50.0f);
        } else if (bomber_jet_moby->pos[2] <= 125.0f) {
            bomber_jet_moby->alpha = 0x80;
        }
        if (gunship_vars->trigger_volume != -1) {
            if (bomber_jet_moby->state == 0) {
                gunship_update((char *)bomber_jet_moby);
            }
            if (gunship_vars->cutscene_timer == -1 &&
                is_point_inside_clip_volume(&hero.motion.pos, gunship_vars->trigger_volume)) {
                if (moby_collected[(short)bomber_jet_moby->uid] != 0 ||
                    (D_0014C190_257218[current_level_index_macro]
                                      [(short)bomber_jet_moby->uid >> 5] >>
                     (bomber_jet_moby->uid & 0x1F)) &
                        1) {
                    for (j = 0; j < 4; j++) {
                        if (gunship_vars->turrets[j] != -1) {
                            GunshipMoby *o =
                                (GunshipMoby *)(ents_macro + (gunship_vars->turrets[j] << 8));
                            if (o != 0 && o->oclass == 0x1CB && o->state != 0xFE &&
                                o->state != 0xFD) {
                                GunshipTurretVars *t = o->vars;
                                t->activated = 1;
                                t->activate_delay = scale_game_frames(2);
                            }
                        }
                    }
                    mark_moby_for_removal(bomber_jet_moby);
                    return;
                }
                bomber_jet_moby->flags &= 0xFFBE;
                if (gunship_vars->cutscene_camera != -1) {
                    l01_hero_set_state(0x72, 0);
                    jet_mount(&l01_cuboids_macro[gunship_vars->cutscene_camera].pos,
                              &l01_cuboids_macro[gunship_vars->cutscene_camera].rot,
                              0,
                              0,
                              0);
                    cutscene_active = 1;
                    bomber_jet_moby->w94 = 0;
                    bomber_jet_moby->flags &= 0xEFFF;
                    gunship_vars->cutscene_timer = scale_game_frames(0x1FE);
                }
            } else if (gunship_vars->cutscene_timer != -1) {
                if (fast_dec_timer_s16(&gunship_vars->cutscene_timer)) {
                    jet_end_mount(0);
                    l01_hero_set_state(0, 1);
                    cutscene_active = 0;
                    D_0014C190_257218[current_level_index_macro]
                                     [(short)bomber_jet_moby->uid >> 5] |=
                        1 << (bomber_jet_moby->uid & 0x1F);
                    moby_killed[(short)bomber_jet_moby->uid >> 5] |=
                        1 << (bomber_jet_moby->uid & 0x1F);
                    mark_moby_for_removal(bomber_jet_moby);
                    return;
                }
                if (gunship_vars->cutscene_timer == scale_game_frames(0x3C)) {
                    for (j = 0; j < 4; j++) {
                        if (gunship_vars->turrets[j] != -1) {
                            GunshipMoby *o =
                                (GunshipMoby *)((gunship_vars->turrets[j] << 8) + ents_macro);
                            if (o->oclass == 0x1CB) {
                                GunshipTurretVars *t = o->vars;
                                t->activated = 1;
                                t->activate_delay = scale_game_frames(2);
                            }
                        }
                    }
                }
                gunship_update((char *)bomber_jet_moby);
            } else {
                bomber_jet_moby->flags |= 0x41;
                break;
            }
        } else {
            gunship_update((char *)bomber_jet_moby);
        }
        bomber_jet_moby->flags |= 0x1000;
        gunship_vars->b2B = 1;
        gunship_vars->b2C = 0x2F;
        if (gunship_vars->last_fire_node != gunship_vars->sub.cur) {
            int found = -1;
            for (i = 0; i < 4; i++) {
                unsigned char *t = gunship_vars->stop_nodes;
                if (gunship_vars->sub.cur == t[i])
                    found = i;
            }
            if (found != -1) {
                int remaining_target_count;
                explosion_vector_or_target_indices = gunship_no_targets;
                j = 0;
                explosion_vector_or_filtered_target_indices = gunship_no_targets;
                remaining_target_count = 0;
                for (i = 0; i < 4; i++) {
                    if (gunship_vars->targets[found][i] != -1) {
                        explosion_vector_or_target_indices.v[j] = gunship_vars->targets[found][i];
                        j++;
                        explosion_vector_or_filtered_target_indices.v[remaining_target_count] =
                            gunship_vars->targets[found][i];
                        remaining_target_count++;
                    }
                }
                if (j == 0)
                    break;
                if (is_point_inside_clip_volume(camera_pos, gunship_vars->camera_block_volume))
                    break;
                for (i = j - 1; i >= 0; i--) {
                    if (collision_line(
                            camera_pos,
                            &l01_cuboids_macro[explosion_vector_or_target_indices.v[i]].pos,
                            0x86,
                            bomber_jet_moby,
                            0)) {
                        if (collision_hit.moby == 0 || collision_hit.moby != bomber_jet_moby) {
                            explosion_vector_or_filtered_target_indices.v[i] = -1;
                            remaining_target_count--;
                            break;
                        }
                    }
                }
                if (remaining_target_count != 0) {
                    int k = 0;
                    for (i = 0; i < j; i++) {
                        if (explosion_vector_or_filtered_target_indices.v[i] != -1) {
                            explosion_vector_or_target_indices.v[k] =
                                explosion_vector_or_filtered_target_indices.v[i];
                            k++;
                        }
                    }
                    j = remaining_target_count;
                }
                if (j != 0) {
                    GunshipMoby *bm;
                    i = explosion_vector_or_target_indices.v[random_integer_below(j)];
                    gunship_vars->gun_side ^= 1;
                    moby_point_position(bomber_jet_moby, gunship_vars->gun_side, &pos);
                    fast_vec_sub(&beam_step_vector, &l01_cuboids_macro[i].pos, &pos);
                    fast_vec_normalize(
                        &beam_step_vector, &beam_step_vector, frame_time_macro * 160.0f);
                    col.q = *(u128 *)&l01_cuboids_macro[i].pos;
                    bm = spawn_beam(
                        bomber_jet_moby,
                        &pos,
                        &beam_step_vector,
                        &col,
                        is_point_inside_clip_volume(&hero.motion.pos, gunship_vars->beam_volume),
                        gunship_vars->trigger_volume != -1);
                    if (bm != 0) {
                        int  k = 0;
                        int *beam_guard_moby_indices = (int *)((char *)bm->vars + 0x20);
                        {
                            unsigned char *t = gunship_vars->stop_nodes;
                            gunship_vars->last_fire_node = t[found];
                        }
                        for (i = 0; i < 8; i++) {
                            if (gunship_vars->guards[i] != -1) {
                                GunshipMoby *o =
                                    (GunshipMoby *)((gunship_vars->guards[i] << 8) + ents_macro);
                                if (o->oclass == 0x2BC) {
                                    GunshipGuardVars *guard_vars = o->vars;
                                    if (guard_vars->f4 == 0 && guard_vars->f0 == 0 &&
                                        random_integer_below(0x64) < 0x64 &&
                                        is_point_inside_clip_volume(&hero.motion.pos,
                                                                    guard_vars->f8) &&
                                        is_point_inside_clip_volume(camera_pos, guard_vars->f8)) {
                                        beam_guard_moby_indices[k] = gunship_vars->guards[i];
                                        k++;
                                    }
                                    if (k >= 4)
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (gunship_vars->cutscene_timer > scale_game_frames(0x1FE) - scale_game_frames(0x78));
}

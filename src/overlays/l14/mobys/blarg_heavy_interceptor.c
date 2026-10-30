/* Level 14 Blarg heavy interceptor moby class: patrol paths, aiming, light ring effects and turret groups. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/level_path.h"
#include "sda.h"
#include "rnc/overlay/l14/mobys/blarg_heavy_interceptor_globals.h"

extern u16 *D_L14_001ABF40_u[] __asm__("D_L14_001ABF40");

extern u8 *D_L14_0015FFD8;

extern void FUN_0020c828(void *);

void activate_waiting_turrets_in_group(s32 index) __asm__("FUN_L14_002b44a8");

extern void release_voice_slot(s32 idx) __asm__("FUN_0022d798");

void process_blarg_heavy_interceptor_hit_reactions(struct Moby *moby) __asm__("FUN_L14_002ad8b8");

extern void
process_blarg_heavy_interceptor_hit_reactions(struct Moby *) __asm__("FUN_L14_002ad8b8");

void initialize_blarg_heavy_interceptor_paths(struct Moby *moby) __asm__("FUN_L14_002ae448");

extern void initialize_blarg_heavy_interceptor_paths(struct Moby *) __asm__("FUN_L14_002ae448");

void update_blarg_heavy_interceptor_patrol_aim_point(struct Moby *moby) __asm__("FUN_L14_002ae6d8");

extern void
update_blarg_heavy_interceptor_patrol_aim_point(struct Moby *) __asm__("FUN_L14_002ae6d8");

void update_blarg_heavy_interceptor_patrol_path_position(struct Moby *moby) __asm__("FUN_L14_002ae7d8");

extern void
update_blarg_heavy_interceptor_patrol_path_position(struct Moby *) __asm__("FUN_L14_002ae7d8");

void update_blarg_heavy_interceptor_return_path_position(
    struct Moby *moby,
    float        position_easing_factor,
    float        path_traversal_speed,
    float        path_direction_multiplier) __asm__("FUN_L14_002ae900");

extern void update_blarg_heavy_interceptor_return_path_position(struct Moby *,
                                                                float,
                                                                float,
                                                                float) __asm__("FUN_L14_002ae900");

extern void FUN_L14_002aea08_c618(struct Moby *) __asm__("FUN_L14_002aea08");

extern void FUN_L14_002aead8_c618(struct Moby *) __asm__("FUN_L14_002aead8");

int update_blarg_heavy_interceptor_aim_transforms(struct Moby *moby,
                                                  int          flag) __asm__("FUN_L14_002aeba0");

extern int update_blarg_heavy_interceptor_aim_transforms(struct Moby *,
                                                         int) __asm__("FUN_L14_002aeba0");

extern void FUN_L14_002aed50_c618(struct Moby *) __asm__("FUN_L14_002aed50");

void draw_blarg_heavy_interceptor_group_effects(struct Moby *moby) __asm__("FUN_L14_002aee28");

extern void draw_blarg_heavy_interceptor_group_effects(struct Moby *) __asm__("FUN_L14_002aee28");

int count_moby_group_members(struct Moby *moby) __asm__("FUN_L14_002af048");

extern int count_moby_group_members(struct Moby *) __asm__("FUN_L14_002af048");

int try_retract_turret_group(int index) __asm__("FUN_L14_002b4500");

extern int try_retract_turret_group(int) __asm__("FUN_L14_002b4500");

int is_turret_group_activated_or_empty(int index) __asm__("FUN_L14_002b4670");

extern int is_turret_group_activated_or_empty(int) __asm__("FUN_L14_002b4670");

extern void blend_moby_animation_c618(struct Moby *, int, int, int) __asm__("FUN_00212f90");

extern u8 *get_moby_target_info(struct Moby *) __asm__("FUN_002141f8");

extern void FUN_L00_0025a120(void *);

extern void fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");

extern void attach_manipulator_c618(void *, s32, void *) __asm__("FUN_0020cb10");

extern void noop_main_callback(void *, float) __asm__("FUN_001e93e8");

extern int FUN_L00_001f2868_c618(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");

extern int FUN_001efa68_c618(void *, void *, int, int, int) __asm__("FUN_001efa68");

extern void FUN_L00_001fff28(void *, int, float);

extern f32 ease_c618(f32 *pos, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025b8c0");

extern int FUN_L00_0025df68_c618(
    void *, void *, void *, int *, float *, int, float, float, float) __asm__("FUN_L00_0025df68");

extern int
FUN_L00_0025d808_c618(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

extern void FUN_L00_0025e450_c618(void *,
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

extern void normalize_vector_c618(void *, void *, float) __asm__("FUN_001f9bf8");

extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

extern char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");

extern unsigned char *FUN_L00_0026d000_c618(void *pos,
                                            float r,
                                            float lo,
                                            float hi,
                                            int   n,
                                            void *vel,
                                            float size,
                                            int   color) __asm__("FUN_L00_0026d000");

extern void spawn_particles_at_shape_points(void *, void *, int) __asm__("FUN_L00_0025c398");

extern s32 allocate_voice_c618(s32, s32, void *) __asm__("FUN_0022da68");

typedef struct {
    u8           pad0[0x18];
    struct Moby *target; /* 0x18 */
} LockOn_c618;

extern LockOn_c618 D_L14_00174540;

extern char D_L14_001674C0[];

extern f32 blarg_heavy_interceptor_culling_sphere_radius_gp __asm__("D_L14_0016139C")
    __attribute__((sda));

extern char *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;

extern char D_L14_0015F580[] MACRO_ADDR;

#include "rnc/overlay/l14/mobys/blarg_heavy_interceptor_data/update_blarg_heavy_interceptor.h"

extern f32 D_L14_001613A0_gp __asm__("D_L14_001613A0") __attribute__((sda));

extern f32 D_L14_001613A4_gp __asm__("D_L14_001613A4") __attribute__((sda));

extern Vec4 D_L14_00174560;

void update_blarg_heavy_interceptor(struct Moby *m) __asm__("FUN_L14_002ac618");

void update_blarg_heavy_interceptor(struct Moby *moby) {
    enum {
        INTERCEPTOR_STATE_INITIALIZE = 0,
        INTERCEPTOR_STATE_PATROL_AND_SCAN = 1,
        INTERCEPTOR_STATE_ALERT_GROUP = 2,
        INTERCEPTOR_STATE_TRACK_TARGET = 3,
        INTERCEPTOR_STATE_WAIT_FOR_GROUP_READY = 4,
        INTERCEPTOR_STATE_FOLLOW_RETURN_PATH = 5,
        INTERCEPTOR_STATE_REJOIN_PATROL = 6,
        INTERCEPTOR_STATE_INITIALIZE_DEATH_FALL = 7,
        INTERCEPTOR_STATE_DEATH_FALL = 9
    };
    char *moby_vars = (char *)moby->pvars;
    float vector_scratch[4];
    float secondary_vector_scratch[4];
    int   near_seg;
    float near_t;
    int   path_seg;
    float path_t;

    if ((u8)moby_vars[0x2E] == 2) {
        *(int *)(moby_vars + 0x20C) = 0;
        FUN_0020c828(moby);
        return;
    }
    if (moby->state == 8) {
        FUN_L14_002aed50_c618(moby);
        add_draw_callback(draw_blarg_heavy_interceptor_group_effects, moby);
        return;
    }
    *(int *)(moby_vars + 0x20C) = 0;
    if (moby->anim_flags & 2) {
        if (moby->seq == moby->prev_seq) {
            if (moby->seq == 2) {
                blend_moby_animation_c618(moby, 3, 0, 3);
            }
            if (moby->seq == 1) {
                blend_moby_animation_c618(moby, 2, 0, 10);
            }
        }
    }
    process_blarg_heavy_interceptor_hit_reactions(moby);
    if (moby->state > 0 && moby->state < 7) {
        if (fast_vec_dist(&moby->pos, D_L14_001674C0) < 40.0f) {
            FUN_L00_0025a120(moby);
        }
        *(s16 *)(moby_vars + 0x200) = 0;
    }
    switch (moby->state) {
    case INTERCEPTOR_STATE_INITIALIZE:
        get_moby_target_info(moby)[0xE] = 1;
        if (*(s16 *)(moby_vars + 0x1B6) == 1) {
            moby->flags = (moby->flags | 0x41) & 0xEFFF;
            moby->collision = 0;
            moby->update_dist = 0xFF;
            moby->state = 8;
            moby_vars[0x58] = truncate_float_to_s32(17.0f);
            moby_vars[0x5A] = truncate_float_to_s32(6.0f);
            *(s16 *)(moby_vars + 0x20A) = count_moby_group_members(moby);
            return;
        }
        if (*(int *)(moby_vars + 0x74) == -1 ||
            *((int *)l14_paths[*(int *)(moby_vars + 0x74)]) == 0 ||
            *(int *)(moby_vars + 0x78) == -1 ||
            *((int *)l14_paths[*(int *)(moby_vars + 0x78)]) == 0 ||
            *(int *)(moby_vars + 0x70) < 0 || *(int *)(moby_vars + 0x204) == -1 ||
            *((int *)l14_paths[*(int *)(moby_vars + 0x204)]) == 0) {
            FUN_0020c828(moby);
            return;
        }
        moby->shadow_range = 0x20;
        *(float *)(moby_vars + 0x20) = 4.0f;
        *(s16 *)(moby_vars + 0x24) = 4;
        moby_vars[0x28] = 3;
        qcopy(&moby->pos, (char *)l14_paths[*(int *)(moby_vars + 0x74)] + 0x10);
        qcopy(moby_vars + 0x170, (char *)l14_paths[*(int *)(moby_vars + 0x78)] + 0x10);
        initialize_blarg_heavy_interceptor_paths(moby);
        *(s16 *)(moby_vars + 0x88) = 1;
        *(float *)(moby_vars + 0x21C) = -1.0f;
        *(float *)(moby_vars + 0x180) = *(float *)(moby_vars + 0x184) =
            *(float *)(moby_vars + 0x188) = 0.0f;
        *(float *)(moby_vars + 0x194) = *(float *)(moby_vars + 0x198) =
            *(float *)(moby_vars + 0x19C) = 0.0f;
        *(float *)(moby_vars + 0x160) = 0.0f;
        *(int *)(moby_vars + 0x20C) = 0;
        *(float *)(moby_vars + 0x164) = 0.0f;
        *(float *)(moby_vars + 0x1A0) = 0.0f;
        *(float *)(moby_vars + 0x1A4) = 0.0f;
        *(float *)(moby_vars + 0x1A8) = 0.0f;
        *(float *)(moby_vars + 0x1AC) = 0.0f;
        *(float *)(moby_vars + 0x1B0) = moby->rot.z;
        *(s16 *)(moby_vars + 0x1B4) = 0;
        *(float *)(moby_vars + 0x1B8) = 0.0f;
        fill_transfer_words(moby_vars + 0xE0, 0, 0x40);
        fill_transfer_words(moby_vars + 0x120, 0, 0x40);
        attach_manipulator_c618(moby, 0, moby_vars + 0xE0);
        attach_manipulator_c618(moby, 1, moby_vars + 0x120);
        *(float *)(moby_vars + 0x1C8) = *(float *)(moby_vars + 0x1C4) = 0.0f;
        *(int *)(moby_vars + 0x18C) = 0;
        moby->state = 1;
        moby->command = 0;
        break;
    case INTERCEPTOR_STATE_PATROL_AND_SCAN: {
        int found;
        int hit;

        moby->glow_color = 0xFF000000 | (blarg_heavy_interceptor_patrol_glow_blue_gp << 16) |
                           (blarg_heavy_interceptor_patrol_glow_green_gp << 8) |
                           blarg_heavy_interceptor_patrol_glow_red_gp;
        if (is_turret_group_activated_or_empty(*(int *)(moby_vars + 0x70))) {
            moby->state = 3;
            *(s16 *)(moby_vars + 0x1F2) =
                scale_game_frames(blarg_heavy_interceptor_target_tracking_duration_frames_gp);
        }
        *(s16 *)(moby_vars + 0x202) = 0;
        *(s16 *)(moby_vars + 0x200) = 1;
        found = 0;
        hit = update_blarg_heavy_interceptor_aim_transforms(moby, 1);
        update_blarg_heavy_interceptor_patrol_aim_point(moby);
        update_blarg_heavy_interceptor_patrol_path_position(moby);
        FUN_L14_002aea08_c618(moby);
        if (hit) {
            lerp_vector(vector_scratch, moby_vars + 0x90, moby_vars + 0xD0, 0.5f);
            qcopy(secondary_vector_scratch, moby_vars + 0x90);
            noop_main_callback(secondary_vector_scratch, *(float *)(moby_vars + 0x214) * 0.8f);
            noop_main_callback(vector_scratch, *(float *)(moby_vars + 0x214) * 0.48f);
            if (FUN_L00_001f2868_c618(
                    *(float *)(moby_vars + 0x214) * 0.48f, vector_scratch, 0, 0, 0) ||
                FUN_L00_001f2868_c618(
                    *(float *)(moby_vars + 0x214) * 0.9f, secondary_vector_scratch, 0, 0, 0)) {
                struct Moby *t = D_L14_00174540.target;

                if ((char *)t == D_001413D0_lui ||
                    (t != 0 && (t->oclass == 0xCB || t->oclass == 0x10E))) {
                    found = 1;
                    *(struct Moby **)(moby_vars + 0x18C) = D_L14_00174540.target;
                }
            }
        }
        if (!found) {
            break;
        }
    }
    case INTERCEPTOR_STATE_ALERT_GROUP: {
        s32 voice = *(s32 *)(moby_vars + 0x1D0);

        if (voice != -1) {
            VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        *(s32 *)(moby_vars + 0x1D0) = -1;
        activate_waiting_turrets_in_group(*(int *)(moby_vars + 0x70));
        moby->state = 3;
        *(s16 *)(moby_vars + 0x1F2) =
            scale_game_frames(blarg_heavy_interceptor_target_tracking_duration_frames_gp);
        break;
    }
    case INTERCEPTOR_STATE_TRACK_TARGET:
        moby->glow_color = 0xFF000000 |
                           (blarg_heavy_interceptor_tracking_and_group_wait_glow_blue_gp << 16) |
                           (blarg_heavy_interceptor_tracking_and_group_wait_glow_green_gp << 8) |
                           blarg_heavy_interceptor_tracking_and_group_wait_glow_red_gp;
        if (*(struct Moby **)(moby_vars + 0x18C) != 0) {
            *(s16 *)(moby_vars + 0x200) = 1;
            *(s16 *)(moby_vars + 0x202) = 1;
            update_blarg_heavy_interceptor_aim_transforms(moby, 1);
            qcopy(vector_scratch, &(*(struct Moby **)(moby_vars + 0x18C))->pos);
            vector_scratch[2] += 0.5f;
            qcopy(secondary_vector_scratch, vector_scratch);
            secondary_vector_scratch[2] -= 4.0f;
            if (FUN_001efa68_c618(vector_scratch, secondary_vector_scratch, 0x12, 0, 0)) {
                qcopy(vector_scratch, &D_L14_00174560);
            } else {
                vector_scratch[2] -= 0.5f;
            }
            ease_c618((f32 *)(moby_vars + 0x170),
                      vector_scratch[0],
                      (f32 *)(moby_vars + 0x180),
                      0.003f,
                      0.2f,
                      0.0f);
            ease_c618((f32 *)(moby_vars + 0x174),
                      vector_scratch[1],
                      (f32 *)(moby_vars + 0x184),
                      0.003f,
                      0.2f,
                      0.0f);
            ease_c618((f32 *)(moby_vars + 0x178),
                      vector_scratch[2],
                      (f32 *)(moby_vars + 0x188),
                      0.003f,
                      0.2f,
                      0.0f);
            update_blarg_heavy_interceptor_patrol_path_position(moby);
            if ((s8)(*(struct Moby **)(moby_vars + 0x18C))->state < 0 ||
                fast_dec_timer_s16((s16 *)(moby_vars + 0x1F2))) {
                *(float *)(moby_vars + 0x8C) = 0.0f;
                *(int *)(moby_vars + 0x80) = 0;
                *(s16 *)(moby_vars + 0x208) = 0;
                moby->state = 4;
            }
        } else {
            *(int *)(moby_vars + 0x80) = 0;
            *(float *)(moby_vars + 0x8C) = 0.0f;
            *(s16 *)(moby_vars + 0x208) = 0;
            moby->state = 4;
        }
    case INTERCEPTOR_STATE_WAIT_FOR_GROUP_READY:
        moby->glow_color = 0xFF000000 |
                           (blarg_heavy_interceptor_tracking_and_group_wait_glow_blue_gp << 16) |
                           (blarg_heavy_interceptor_tracking_and_group_wait_glow_green_gp << 8) |
                           blarg_heavy_interceptor_tracking_and_group_wait_glow_red_gp;
        qcopy(vector_scratch, &moby->pos);
        vector_scratch[3] = blarg_heavy_interceptor_culling_sphere_radius_gp;
        if (fast_bsphere_check((u8 *)vector_scratch, convert_integer_to_float(moby->draw_dist)) >=
            0) {
            float *p;

            moby_point_position(moby, 2, secondary_vector_scratch);
            p = (float *)(moby_vars + 0x1D4);
            p[0] = secondary_vector_scratch[0];
            p[1] = secondary_vector_scratch[1];
            p[2] = secondary_vector_scratch[2];
            *(int *)(moby_vars + 0x20C) |= 8;
            FUN_L14_002aea08_c618(moby);
        }
        if (moby->state == 4) {
            if (moby->prev_seq == 0) {
                blend_moby_animation_c618(moby, 2, 0, 5);
            }
            update_blarg_heavy_interceptor_return_path_position(
                moby,
                blarg_heavy_interceptor_group_wait_return_path_position_easing_factor_gp,
                blarg_heavy_interceptor_group_wait_return_path_traversal_speed_gp,
                1.0f);
            if (*(float *)(moby_vars + 0x1C0) != 0.0f || *(float *)(moby_vars + 0x1BC) != 0.0f) {
                ease_c618((f32 *)(moby_vars + 0x1C0),
                          0.0f,
                          (f32 *)(moby_vars + 0x1C8),
                          0.004f,
                          0.2f,
                          0.0f);
                ease_c618((f32 *)(moby_vars + 0x1BC),
                          0.0f,
                          (f32 *)(moby_vars + 0x1C4),
                          0.004f,
                          0.2f,
                          0.0f);
                if (absolute_float(*(float *)(moby_vars + 0x1C0)) < 0.0001f) {
                    *(float *)(moby_vars + 0x1C0) = 0.0f;
                }
                if (absolute_float(*(float *)(moby_vars + 0x1BC)) < 0.0001f) {
                    *(float *)(moby_vars + 0x1BC) = 0.0f;
                }
                FUN_L00_001fff28(moby_vars + 0xF0, 2, *(float *)(moby_vars + 0x1BC));
                FUN_L00_001fff28(moby_vars + 0x130, 1, *(float *)(moby_vars + 0x1C0));
            }
            if (try_retract_turret_group(*(int *)(moby_vars + 0x70))) {
                LevelPath *p = l14_paths[*(int *)(moby_vars + 0x204)];

                moby->state = 5;
                *(int *)(moby_vars + 0x80) = p->count - 2;
                *(float *)(moby_vars + 0x8C) = p->points[*(int *)(moby_vars + 0x80)].f[3];
                *(s16 *)(moby_vars + 0x208) = 0;
                update_blarg_heavy_interceptor_return_path_position(moby, 0.01f, 5.0f, -1.0f);
                blend_moby_animation_c618(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case INTERCEPTOR_STATE_FOLLOW_RETURN_PATH:
        FUN_L14_002aea08_c618(moby);
        update_blarg_heavy_interceptor_return_path_position(moby, 0.01f, 5.0f, -1.0f);
        if (*(s16 *)(moby_vars + 0x208) != 0) {
            int *tab = ((int *)l14_paths[*(int *)(moby_vars + 0x78)]);

            near_seg = 0;
            near_t = 0.0f;
            FUN_L00_0025df68_c618(
                tab, &moby->pos, vector_scratch, &near_seg, &near_t, 0, 40.0f, 5.0f, 0.0f);
            *(int *)(moby_vars + 0x84) = near_seg;
            *(float *)(moby_vars + 0x1CC) = near_t;
            *(s16 *)(moby_vars + 0x200) = 0;
            moby->state = 6;
        }
        break;
    case INTERCEPTOR_STATE_REJOIN_PATROL: {
        int   i;
        int   source_segment_index;
        float source_path_distance;
        char *seg;
        int  *tab;
        float t;
        int   rejoin_path_loop_enabled;

        FUN_L14_002aea08_c618(moby);
        source_segment_index = *(int *)(moby_vars + 0x84);
        seg = (char *)l14_paths[*(int *)(moby_vars + 0x78)];
        source_path_distance = 0.0f;
        for (i = 0; i < source_segment_index; i++) {
            source_path_distance += *(float *)(seg + i * 16 + 0x1C);
        }
        source_path_distance += *(float *)(moby_vars + 0x1CC);
        tab = ((int *)l14_paths[*(int *)(moby_vars + 0x74)]);
        t = source_path_distance / *(float *)(moby_vars + 0x16C) * *(float *)(moby_vars + 0x190);
        rejoin_path_loop_enabled = *(s16 *)(moby_vars + 0x8A);
        path_seg = 0;
        path_t = 0.0f;
        FUN_L00_0025d808_c618(tab, vector_scratch, &path_seg, &path_t, rejoin_path_loop_enabled, t);
        ease_c618(&moby->pos.x,
                  vector_scratch[0],
                  (f32 *)(moby_vars + 0x194),
                  0.005f,
                  D_L14_001613A4_gp,
                  0.0f);
        ease_c618(&moby->pos.y,
                  vector_scratch[1],
                  (f32 *)(moby_vars + 0x198),
                  0.005f,
                  D_L14_001613A4_gp,
                  0.0f);
        ease_c618(&moby->pos.z,
                  vector_scratch[2],
                  (f32 *)(moby_vars + 0x19C),
                  0.005f,
                  D_L14_001613A4_gp,
                  0.0f);
        update_blarg_heavy_interceptor_aim_transforms(moby, 0);
        if (fast_vec_dist(&moby->pos, vector_scratch) < 0.5f) {
            moby->state = 1;
        }
        FUN_L00_0025d808_c618(((int *)l14_paths[*(int *)(moby_vars + 0x78)]),
                              vector_scratch,
                              moby_vars + 0x84,
                              moby_vars + 0x1CC,
                              *(s16 *)(moby_vars + 0x8A),
                              0.0f);
        ease_c618((f32 *)(moby_vars + 0x170),
                  vector_scratch[0],
                  (f32 *)(moby_vars + 0x180),
                  D_L14_001613A0_gp,
                  D_L14_001613A4_gp,
                  0.0f);
        ease_c618((f32 *)(moby_vars + 0x174),
                  vector_scratch[1],
                  (f32 *)(moby_vars + 0x184),
                  D_L14_001613A0_gp,
                  D_L14_001613A4_gp,
                  0.0f);
        ease_c618((f32 *)(moby_vars + 0x178),
                  vector_scratch[2],
                  (f32 *)(moby_vars + 0x188),
                  D_L14_001613A0_gp,
                  D_L14_001613A4_gp,
                  0.0f);
        break;
    }
    case INTERCEPTOR_STATE_INITIALIZE_DEATH_FALL: {
        s32   voice;
        float death_launch_angle;

        *(s16 *)(moby_vars + 0x1F0) =
            scale_game_frames(blarg_heavy_interceptor_death_fall_duration_frames_gp);
        activate_waiting_turrets_in_group(*(int *)(moby_vars + 0x70));
        voice = *(s32 *)(moby_vars + 0x1D0);
        if (voice != -1) {
            VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        *(s32 *)(moby_vars + 0x1D0) = -1;
        FUN_L00_0025e450_c618(moby,
                              moby_vars + 0x40,
                              &moby->pos,
                              0.0f,
                              0.0f,
                              5,
                              2,
                              4,
                              4.0f,
                              2.0f,
                              9.0f,
                              1.0f,
                              1,
                              15.0f,
                              1,
                              5,
                              -1,
                              0);
        fast_vec_sub(vector_scratch, &moby->pos, &hero.motion.pos);
        vector_scratch[2] = 0.0f;
        normalize_vector_c618(
            vector_scratch,
            vector_scratch,
            random_float_between(
                blarg_heavy_interceptor_death_launch_vector_magnitude_range_start_gp * frame_scale,
                blarg_heavy_interceptor_death_launch_vector_magnitude_range_end_gp * frame_scale));
        fast_vec_cross(secondary_vector_scratch, &hero.gravity_dir, vector_scratch);
        death_launch_angle = randf_sym(0.6981317f, 1.2217305f);
        build_look_at_matrix(
            moby_vars + 0x1E0, vector_scratch, &hero.gravity_dir, death_launch_angle);
        *(float *)(moby_vars + 0x1E8) = random_float_between(
            blarg_heavy_interceptor_death_launch_vertical_velocity_range_start_gp * frame_scale,
            blarg_heavy_interceptor_death_launch_vertical_velocity_range_end_gp * frame_scale);
        moby->scale *= 0.1f;
        *(float *)(moby_vars + 0x140) = 10.0f;
        *(float *)(moby_vars + 0x144) = 10.0f;
        *(float *)(moby_vars + 0x148) = 10.0f;
        *(float *)(moby_vars + 0x1F4) = randf_sym(0.0f, 0.08726646f);
        *(float *)(moby_vars + 0x1F8) = randf_sym(0.0f, 0.08726646f);
        *(float *)(moby_vars + 0x1FC) = randf_sym(0.0f, 0.08726646f);
        moby->rot.x = fast_add_rotations(moby->rot.x, randf_sym(0.0f, 0.7853982f));
        moby->rot.y = fast_add_rotations(moby->rot.y, randf_sym(0.0f, 0.7853982f));
        moby->state = 9;
        FUN_L14_002aead8_c618(moby);
        return;
    }
    case INTERCEPTOR_STATE_DEATH_FALL: {
        int i;

        fast_vec_add(&moby->pos, &moby->pos, moby_vars + 0x1E0);
        if (absolute_float(*(float *)(moby_vars + 0x1E0)) >
            blarg_heavy_interceptor_death_fall_horizontal_drag_speed_threshold_gp * frame_scale) {
            *(float *)(moby_vars + 0x1E0) *=
                (blarg_heavy_interceptor_death_fall_velocity_retention_factor_gp - 1.0f) *
                    frame_scale +
                1.0f;
        }
        if (absolute_float(*(float *)(moby_vars + 0x1E4)) >
            blarg_heavy_interceptor_death_fall_horizontal_drag_speed_threshold_gp * frame_scale) {
            *(float *)(moby_vars + 0x1E4) *=
                (blarg_heavy_interceptor_death_fall_velocity_retention_factor_gp - 1.0f) *
                    frame_scale +
                1.0f;
        }
        if (*(float *)(moby_vars + 0x1E8) > 0.0f) {
            *(float *)(moby_vars + 0x1E8) *=
                (blarg_heavy_interceptor_death_fall_velocity_retention_factor_gp - 1.0f) *
                    frame_scale +
                1.0f;
        }
        *(float *)(moby_vars + 0x1E8) -=
            blarg_heavy_interceptor_death_fall_gravity_gp * frame_scale_sq;
        moby->rot.z = fast_add_rotations(moby->rot.z, *(float *)(moby_vars + 0x1FC));
        *(float *)(moby_vars + 0x140) = 10.0f;
        *(float *)(moby_vars + 0x144) = 10.0f;
        *(float *)(moby_vars + 0x148) = 10.0f;
        if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
            moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
            FUN_0020c828(moby);
            return;
        }
        if (fast_dec_timer_s16((s16 *)(moby_vars + 0x1F0)) ||
            coll_sphere(0.5f, &moby->pos, 0, moby)) {
            FUN_L00_0025e450_c618(moby,
                                  moby_vars + 0x40,
                                  &moby->pos,
                                  2.0f,
                                  1.0f,
                                  5,
                                  2,
                                  4,
                                  2.0f,
                                  1.0f,
                                  9.0f,
                                  1.0f,
                                  1,
                                  15.0f,
                                  1,
                                  5,
                                  -1,
                                  0);
            FUN_0020c828(moby);
            return;
        }
        spawn_burst(&moby->pos,
                    D_L14_0015F580,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    random_int_between(scale_game_frames(10), scale_game_frames(20)),
                    1,
                    60000.0f);
        for (i = 0; i < 2; i++) {
            int            signed_puff_random_value = random_integer_below(6);
            float          size;
            int            puff_rgb_component;
            unsigned char *p;

            if (random_integer_below(2)) {
                signed_puff_random_value = -signed_puff_random_value;
            }
            size = random_float_between(40000.0f, 100000.0f);
            puff_rgb_component = random_int_between(0x30, 0xFF);
            p = FUN_L00_0026d000_c618(
                &moby->pos,
                0.025f,
                1.0f,
                1.0f,
                signed_puff_random_value,
                D_L14_0015F580,
                size,
                puff_rgb_component |
                    (0x60000000 | (puff_rgb_component << 16) | (puff_rgb_component << 8)));
            if (p != 0) {
                unsigned char *puff_tail;

                p[3] = 0x44;
                puff_tail = p + 0x20;
                *(s16 *)(p + 0xA) = scale_game_frames(60);
                *(int *)(puff_tail + 4) = 2;
                puff_tail[0xA] = 0x60;
                puff_tail[0xB] = p[0xA];
            }
        }
        FUN_L14_002aead8_c618(moby);
        return;
    }
    }
    if ((s8)moby->state >= 0 && moby->prev_seq == 1) {
        if (moby->command == 1) {
            spawn_particles_at_shape_points(moby, D_L14_0015F580, 0xFFFF);
        }
    } else {
        moby->command = 0;
    }
    if (moby->state == 1 && !voice_is_playing(moby, *(s32 *)(moby_vars + 0x1D0))) {
        *(s32 *)(moby_vars + 0x1D0) = allocate_voice_c618(0, 4, moby);
    }
    if ((moby->state == 3 || moby->state == 4) &&
        !voice_is_playing(moby, *(s32 *)(moby_vars + 0x1D0))) {
        *(s32 *)(moby_vars + 0x1D0) = allocate_voice_c618(2, 4, moby);
    }
}

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");

extern void FUN_L00_00257470(void *, int, int);

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void process_blarg_heavy_interceptor_hit_reactions(struct Moby *moby) {
    enum {
        HIT_REACTION_NO_ACTION = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DEATH_FALL = 2,
        HIT_REACTION_PLAY_HIT_ANIMATION = 3,
        HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION = 4,
        HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION = 5,
        HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH = 6,
        HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION = 7,
        HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH = 8,
        HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH = 9,
        HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION = 10,
        HIT_REACTION_NONE = 11
    };
    char *data = (char *)moby->pvars;
    int   hit_reaction_status;
    float damage_amount;
    int   hit_reaction_result;
    damage_amount = 0.0f;
    hit_reaction_result = hit_reaction(moby,
                                       find_moby_hit(moby, 0x330000, 0),
                                       data + 0x20,
                                       0,
                                       &hit_reaction_status,
                                       &damage_amount,
                                       0,
                                       4);
    if (hit_reaction_status != 1 && moby->state < 7) {
        float remaining_health = *(float *)(data + 0x20) - damage_amount;
        *(float *)(data + 0x20) = remaining_health;
        if (remaining_health <= 0.0f)
            hit_reaction_result = 1;
        if (moby->state == 1)
            moby->state = 2;
        if (hit_reaction_status == 3)
            moby->command = 1;
        switch (hit_reaction_result) {
        case HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH:
        case HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION:
            data[0x67] = 0x64;
            blend_moby_animation(moby, 1, 0, 5);
            break;
        case HIT_REACTION_PLAY_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH:
            ((unsigned char *)data)[0x67] = 0xC8;
            blend_moby_animation(moby, 1, 0, 5);
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_DEATH_FALL:
            moby->state = 7;
            moby->flags &= 0xEFFF;
            FUN_L00_00257470(moby, 0, -1);
            break;
        case HIT_REACTION_NO_ACTION:
        case HIT_REACTION_NONE:
            break;
        }
        trooper_anim_update(moby, (short *)(data + 0x60));
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, data + 0x60);
}

extern s32 blarg_heavy_interceptor_light_ring_vertex_count_gp __asm__("D_L14_00161418")
    __attribute__((sda));

extern float blarg_heavy_interceptor_light_ring_vertex_positions[][3] __asm__("D_L14_001D7A60");

extern s32 blarg_heavy_interceptor_light_ring_vertex_colors[] __asm__("D_L14_001D80C8");

extern char blarg_heavy_interceptor_light_ring_texture_coordinates[] __asm__("D_L14_001D7E38");

extern char D_L14_001674C0_r[] __asm__("D_L14_001674C0");

void draw_blarg_heavy_interceptor_camera_facing_glow(struct Moby *m) __asm__("FUN_L14_002ae0a8");

extern void
draw_blarg_heavy_interceptor_camera_facing_glow(struct Moby *m) __asm__("FUN_L14_002ae0a8");

extern void FUN_L00_001fde98(int, void *, void *, void *, int);

extern void normalize_r(void *, void *, float) __asm__("FUN_001f9bf8");

extern s64 tex_r(s32) __asm__("FUN_001f44b8");

extern void sreg_r(s32, s64) __asm__("FUN_00233980");

extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void draw_blarg_heavy_interceptor_light_ring(char *moby) __asm__("FUN_L14_002ada18");

void draw_blarg_heavy_interceptor_light_ring(char *moby) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    struct {
        float mat[4][4];    /* camera-facing frame */
        float verts[82][3]; /* ring vertices transformed by mat */
    } f;
    u32   cols[84];
    float ring_vertex_scratch[4];
    int   i;
    int   j;

    draw_blarg_heavy_interceptor_camera_facing_glow((struct Moby *)moby);
    qcopy(f.mat[0], moby_vars + 0xA0);
    fast_vec_sub(f.mat[2], moby_vars + 0xD0, D_L14_001674C0_r);
    fast_vec_cross(f.mat[1], f.mat[0], f.mat[2]);
    normalize_r(f.mat[1], f.mat[1], 1.0f);
    fast_vec_cross(f.mat[2], f.mat[1], f.mat[0]);
    qcopy(f.mat[3], moby_vars + 0xD0);
    f.mat[3][3] = 1.0f;
    sreg_r(6, tex_r(0x16));
    sreg_r(0x42, 0x8000000048);
    sreg_r(8, 5);
    sreg_r(0x14, 0xFF9000000260);
    sreg_r(0x47, 0x50000);
    sreg_r(0x4A, 0);
    font_queue_vu_state();
    for (i = 0; i < blarg_heavy_interceptor_light_ring_vertex_count_gp; i++) {
        float *s = blarg_heavy_interceptor_light_ring_vertex_positions[i];
        float *o = f.verts[i];
        ring_vertex_scratch[0] = s[0];
        ring_vertex_scratch[1] = s[1];
        ring_vertex_scratch[2] = s[2];
        ring_vertex_scratch[3] = 1.0f;
        transform_vector(ring_vertex_scratch, ring_vertex_scratch, f.mat);
        o[0] = ring_vertex_scratch[0];
        o[1] = ring_vertex_scratch[1];
        o[2] = ring_vertex_scratch[2];
        if (*(short *)(moby_vars + 0x202) != 0) {
            cols[i] = blarg_heavy_interceptor_light_ring_vertex_colors[i] & 0xFF0000FF;
        }
    }
    if (*(short *)(moby_vars + 0x202) != 0) {
        FUN_L00_001fde98(blarg_heavy_interceptor_light_ring_vertex_count_gp,
                         f.verts,
                         cols,
                         blarg_heavy_interceptor_light_ring_texture_coordinates,
                         1);
    } else {
        FUN_L00_001fde98(blarg_heavy_interceptor_light_ring_vertex_count_gp,
                         f.verts,
                         blarg_heavy_interceptor_light_ring_vertex_colors,
                         blarg_heavy_interceptor_light_ring_texture_coordinates,
                         1);
    }
    fast_vec_scale(f.mat[1], f.mat[1], -1.0f);
    fast_vec_scale(f.mat[2], f.mat[2], -1.0f);
    for (j = 0; j < blarg_heavy_interceptor_light_ring_vertex_count_gp; j++) {
        float *o = f.verts[j];
        float *s = blarg_heavy_interceptor_light_ring_vertex_positions[j];
        ring_vertex_scratch[0] = s[0];
        ring_vertex_scratch[1] = s[1];
        ring_vertex_scratch[2] = s[2];
        ring_vertex_scratch[3] = 1.0f;
        transform_vector(ring_vertex_scratch, ring_vertex_scratch, f.mat);
        o[0] = ring_vertex_scratch[0];
        o[1] = ring_vertex_scratch[1];
        o[2] = ring_vertex_scratch[2];
    }
    if (*(short *)(moby_vars + 0x202) != 0) {
        FUN_L00_001fde98(blarg_heavy_interceptor_light_ring_vertex_count_gp,
                         f.verts,
                         cols,
                         blarg_heavy_interceptor_light_ring_texture_coordinates,
                         1);
    } else {
        FUN_L00_001fde98(blarg_heavy_interceptor_light_ring_vertex_count_gp,
                         f.verts,
                         blarg_heavy_interceptor_light_ring_vertex_colors,
                         blarg_heavy_interceptor_light_ring_texture_coordinates,
                         1);
    }
}

typedef struct {
    float         m[4][4];
    u32           col[4];
    float         uv[8];
    unsigned long a, b, c, d;
} Pk;

extern float blarg_heavy_interceptor_spinning_beam_initial_offset_scale_gp __asm__("D_L14_00161434")
    __attribute__((sda));

extern float blarg_heavy_interceptor_spinning_beam_offset_scale_increment_per_pair_gp __asm__("D_L14_00161438") __attribute__((sda));

extern float blarg_heavy_interceptor_spinning_beam_initial_quad_scale_gp __asm__("D_L14_0016143C")
    __attribute__((sda));

extern float
    blarg_heavy_interceptor_spinning_beam_quad_scale_increment_per_pair_gp __asm__("D_L14_00161440")
        __attribute__((sda));

extern int blarg_heavy_interceptor_spinning_beam_initial_red_gp __asm__("D_L14_00161444")
    __attribute__((sda));

extern int blarg_heavy_interceptor_spinning_beam_initial_green_gp __asm__("D_L14_00161448")
    __attribute__((sda));

extern int blarg_heavy_interceptor_spinning_beam_initial_blue_gp __asm__("D_L14_0016144C")
    __attribute__((sda));

extern int blarg_heavy_interceptor_spinning_beam_initial_alpha_gp __asm__("D_L14_00161450")
    __attribute__((sda));

extern int blarg_heavy_interceptor_spinning_beam_red_decrement_per_pair_gp __asm__("D_L14_00161454")
    __attribute__((sda));

extern int
    blarg_heavy_interceptor_spinning_beam_green_decrement_per_pair_gp __asm__("D_L14_00161458")
        __attribute__((sda));

extern int
    blarg_heavy_interceptor_spinning_beam_blue_decrement_per_pair_gp __asm__("D_L14_0016145C")
        __attribute__((sda));

extern int
    blarg_heavy_interceptor_spinning_beam_alpha_decrement_per_pair_gp __asm__("D_L14_00161460")
        __attribute__((sda));

extern char D_L14_001674C0_b[] __asm__("D_L14_001674C0");

extern unsigned long tex_b(s32) __asm__("FUN_001f44b8");

extern void sreg_b(s32, s64) __asm__("FUN_00233980");

extern void draw_b(void *, int, int) __asm__("FUN_001f7d30");

float D_L14_001D8600[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

void draw_blarg_heavy_interceptor_spinning_beam(struct Moby *moby) __asm__("FUN_L14_002add48");

void draw_blarg_heavy_interceptor_spinning_beam(struct Moby *moby) {
    char  *d = ((char *)moby->pvars);
    float  beam_placement_scratch[4][4];
    Pk     pk;
    float  beam_quad_transform[4][4];
    float  quad_translation_offset[4];
    float  beam_offset_direction[4];
    float  beam_to_camera_vector[4];
    float  camera_facing_angles[4];
    float  rm[3][4];
    float  size;
    float  step;
    int    r, g, b, a;
    u32    col;
    int    i, j;
    float *beam_origin;

    beam_origin = (float *)(d + 0x1D4);
    beam_placement_scratch[3][0] = beam_origin[0];
    beam_placement_scratch[3][1] = beam_origin[1];
    beam_placement_scratch[3][2] = beam_origin[2] + 0.125f;
    beam_placement_scratch[3][3] = 1.0f;
    beam_placement_scratch[0][0] = fast_cos(*(float *)(d + 0x160));
    beam_placement_scratch[0][1] = fast_sin(*(float *)(d + 0x160));
    beam_placement_scratch[0][2] = 0.0f;
    *(float *)(d + 0x160) = fast_add_rotations(*(float *)(d + 0x160), -0.06981317f);
    pk.b = tex_b(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    qcopy(beam_offset_direction, beam_placement_scratch[0]);
    qcopy(beam_quad_transform[3], beam_placement_scratch[3]);
    beam_quad_transform[3][3] = 1.0f;
    fast_vec_sub(beam_to_camera_vector, D_L14_001674C0_b, beam_quad_transform[3]);
    camera_facing_angles[2] = fast_arc_tan(beam_to_camera_vector[0], beam_to_camera_vector[1]);
    camera_facing_angles[1] =
        -fast_arc_tan(vector_length_xy(beam_to_camera_vector), beam_to_camera_vector[2]);
    camera_facing_angles[0] = 0.0f;
    euler_rotation_basis(rm, camera_facing_angles);
    qcopy(beam_quad_transform[0], rm[0]);
    qcopy(beam_quad_transform[1], rm[1]);
    qcopy(beam_quad_transform[2], rm[2]);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    size = blarg_heavy_interceptor_spinning_beam_initial_quad_scale_gp;
    step = blarg_heavy_interceptor_spinning_beam_initial_offset_scale_gp;
    r = blarg_heavy_interceptor_spinning_beam_initial_red_gp;
    g = blarg_heavy_interceptor_spinning_beam_initial_green_gp;
    b = blarg_heavy_interceptor_spinning_beam_initial_blue_gp;
    a = blarg_heavy_interceptor_spinning_beam_initial_alpha_gp;
    sreg_b(0x47, 0x51001);
    for (i = 0; i < 16; i++) {
        col = (a << 24) | (b << 16) | (g << 8) | r;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
        fast_vec_scale(quad_translation_offset, beam_offset_direction, step);
        for (j = 0; j < 4; j++) {
            fast_vec_scale(pk.m[j], D_L14_001D8600[j], size);
            transform_vector(pk.m[j], pk.m[j], beam_quad_transform);
            fast_vec_add(pk.m[j], pk.m[j], quad_translation_offset);
        }
        draw_b(&pk, 0, 0);
        fast_vec_scale(quad_translation_offset, beam_offset_direction, step * -2.0f);
        for (j = 0; j < 4; j++) {
            fast_vec_add(pk.m[j], pk.m[j], quad_translation_offset);
        }
        draw_b(&pk, 0, 0);
        r -= blarg_heavy_interceptor_spinning_beam_red_decrement_per_pair_gp;
        g -= blarg_heavy_interceptor_spinning_beam_green_decrement_per_pair_gp;
        b -= blarg_heavy_interceptor_spinning_beam_blue_decrement_per_pair_gp;
        a -= blarg_heavy_interceptor_spinning_beam_alpha_decrement_per_pair_gp;
        size += blarg_heavy_interceptor_spinning_beam_quad_scale_increment_per_pair_gp;
        step += blarg_heavy_interceptor_spinning_beam_offset_scale_increment_per_pair_gp;
        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
        if (a < 0)
            a = 0;
    }
}

typedef struct {
    float         m[4][4];
    u32           col[4];
    float         uv[8];
    unsigned long a, b, c, d;
} Pk_2ae0a8;

extern char D_L14_001674C0_e[] __asm__("D_L14_001674C0");

extern unsigned long tex_e(s32) __asm__("FUN_001f44b8");

extern void sreg_e(s32, s64) __asm__("FUN_00233980");

extern void draw_e(void *, int, int) __asm__("FUN_001f7d30");

extern float blarg_heavy_interceptor_glow_quad_scale_gp __asm__("D_L14_00161474")
    __attribute__((sda));

extern float blarg_heavy_interceptor_glow_position_offset_scale_gp __asm__("D_L14_00161478")
    __attribute__((sda));

void draw_blarg_heavy_interceptor_camera_facing_glow(struct Moby *m) {
    char     *d = (char *)m->pvars;
    float     glow_quad_transform[4][4];
    float     glow_position_offset[4];
    Pk_2ae0a8 pk;
    float     glow_to_camera_vector[4];
    float     camera_facing_angles[4];
    u32       col;
    int       i;
    qcopy(glow_quad_transform[3], d + 0xD0);
    glow_quad_transform[3][3] = 1.0f;
    fast_vec_scale(
        glow_position_offset, d + 0xA0, blarg_heavy_interceptor_glow_position_offset_scale_gp);
    fast_vec_add(glow_quad_transform[3], glow_quad_transform[3], glow_position_offset);
    pk.b = tex_e(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    sreg_e(0x47, 0x51001);
    fast_vec_sub(glow_to_camera_vector, D_L14_001674C0_e, glow_quad_transform[3]);
    camera_facing_angles[2] = fast_arc_tan(glow_to_camera_vector[0], glow_to_camera_vector[1]);
    camera_facing_angles[1] =
        -fast_arc_tan(vector_length_xy(glow_to_camera_vector), glow_to_camera_vector[2]);
    camera_facing_angles[0] = 0.0f;
    euler_rotation_basis(glow_quad_transform, camera_facing_angles);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    if (*(short *)(d + 0x202) != 0) {
        col = 0x7F40407F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    } else {
        col = 0x7F7F7F7F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    }
    for (i = 0; i < 4; i++) {
        fast_vec_scale(pk.m[i], D_L14_001D8600[i], blarg_heavy_interceptor_glow_quad_scale_gp);
        transform_vector(pk.m[i], pk.m[i], glow_quad_transform);
    }
    draw_e(&pk, 0, 0);
}

typedef struct {
    float m[4][4];
    u32   col[4];
    float uv[8];
    u64   a, b, c, d;
} Pk_2af4a0;

extern char D_L14_001674C0_lui[] __asm__("D_L14_001674C0") NOT_SDA;

float D_L14_001D8640[4][4] = {
    {-1.0f,  1.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    { 1.0f,  1.0f, 0.0f, 1.0f},
    { 1.0f, -1.0f, 0.0f, 1.0f},
};

extern void draw_geometry_quad(void *, void *, int);

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

u64 get_effect_texture_alt(s32) __asm__("FUN_001f44b8");

extern short blarg_heavy_interceptor_glow_minimum_camera_separation_gp __asm__("D_L14_0016147C")
    __attribute__((sda));

extern short blarg_heavy_interceptor_camera_pulled_glow_default_color_gp __asm__("D_L14_00161480")
    __attribute__((sda));

void draw_blarg_heavy_interceptor_camera_pulled_glow(struct Moby *m) __asm__("FUN_L14_002ae260");

void draw_blarg_heavy_interceptor_camera_pulled_glow(struct Moby *m) {
    char     *d = (char *)m->pvars;
    float     glow_quad_position[4];
    Pk_2af4a0 pk;
    float     glow_to_camera_vector[4];
    float     camera_pull_offset[4];
    float     glow_to_camera_distance;
    u32       col;
    int       j;
    qcopy(glow_quad_position, d + 0x90);
    glow_quad_position[2] += 0.01f;
    glow_quad_position[3] = 1.0f;
    pk.b = get_effect_texture_alt(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    fast_vec_sub(glow_to_camera_vector, D_L14_001674C0_lui, glow_quad_position);
    glow_to_camera_distance = fast_vec_length(glow_to_camera_vector);
    if (0.0f < glow_to_camera_distance) {
        float camera_pull_distance = *(float *)(d + 0x218);
        float camera_distance_margin =
            glow_to_camera_distance -
            *(float *)&blarg_heavy_interceptor_glow_minimum_camera_separation_gp;
        if (camera_distance_margin < camera_pull_distance) {
            camera_pull_distance = camera_distance_margin;
            if (camera_distance_margin < 0.0f)
                camera_pull_distance = 0.0f;
        }
        fast_vec_normalize(camera_pull_offset, glow_to_camera_vector, camera_pull_distance);
        fast_vec_add(glow_quad_position, glow_quad_position, camera_pull_offset);
    }
    pk.uv[2] = 0.0f;
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    if (*(short *)(d + 0x202) != 0) {
        col = 0x7F40407F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    } else {
        col = *(u32 *)&blarg_heavy_interceptor_camera_pulled_glow_default_color_gp;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    }
    for (j = 0; j < 4; j++) {
        fast_vec_scale(pk.m[j], D_L14_001D8640[j], *(float *)(d + 0x214));
        fast_vec_add(pk.m[j], pk.m[j], glow_quad_position);
    }
    draw_geometry_quad(&pk, 0, 0);
}

extern int
FUN_L00_0025d808_2af688(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

void initialize_blarg_heavy_interceptor_paths(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int  *path;
    int   i;
    float segment_length;
    path = ((int *)l14_paths[*(int *)(moby_vars + 0x78)]);
    *(int *)(moby_vars + 0x16C) = 0;
    for (i = 0; i < path[0]; i++) {
        segment_length = fast_vec_dist((char *)path + (i * 16 + 0x10),
                                       (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
        *(float *)((char *)path + (i << 4) + 0x1C) = segment_length;
        if (i != path[0] - 1) {
            *(float *)(moby_vars + 0x16C) += segment_length;
        }
    }
    path = ((int *)l14_paths[*(int *)(moby_vars + 0x74)]);
    *(int *)(moby_vars + 0x190) = 0;
    for (i = 0; i < path[0]; i++) {
        segment_length = fast_vec_dist((char *)path + (i * 16 + 0x10),
                                       (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
        *(float *)((char *)path + (i << 4) + 0x1C) = segment_length;
        if (i != path[0] - 1) {
            *(float *)(moby_vars + 0x190) += segment_length;
        }
    }
    path = ((int *)l14_paths[*(int *)(moby_vars + 0x204)]);
    for (i = 0; i < path[0]; i++) {
        *(float *)((char *)path + (i << 4) + 0x1C) = fast_vec_dist(
            (char *)path + (i * 16 + 0x10), (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
    }
    if (*(float *)(moby_vars + 0x210) != 0.0f) {
        int *sampling_path = ((int *)l14_paths[*(int *)(moby_vars + 0x78)]);
        *(int *)(moby_vars + 0x84) = 0;
        *(float *)(moby_vars + 0x1CC) = 0.0f;
        FUN_L00_0025d808_2af688(sampling_path,
                                &moby->pos,
                                moby_vars + 0x84,
                                moby_vars + 0x1CC,
                                *(short *)(moby_vars + 0x8A),
                                *(float *)(moby_vars + 0x210) * *(float *)(moby_vars + 0x16C));
        *(float *)(moby_vars + 0x8C) = 0.0f;
        *(int *)(moby_vars + 0x80) = 0;
        FUN_L00_0025d808_2af688(sampling_path,
                                &moby->pos,
                                moby_vars + 0x80,
                                moby_vars + 0x8C,
                                *(short *)(moby_vars + 0x8A),
                                *(float *)(moby_vars + 0x210) * *(float *)(moby_vars + 0x190));
    } else {
        *(int *)(moby_vars + 0x80) = 0;
        *(int *)(moby_vars + 0x84) = 0;
        *(float *)(moby_vars + 0x8C) = 0.0f;
        *(float *)(moby_vars + 0x1CC) = 0.0f;
    }
}

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");

extern int FUN_L00_0025d808_2af918(int *tab, float *out, int *a, float *b, int c, float d) __asm__("FUN_L00_0025d808");

void update_blarg_heavy_interceptor_patrol_aim_point(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float path_target_position[4];
    float save = *(float *)(data + 0x1CC);
    int   save2 = *(int *)(data + 0x84);
    int   r = FUN_L00_0025d808_2af918(((int *)l14_paths[*(int *)(data + 0x78)]),
                                    path_target_position,
                                    (int *)(data + 0x84),
                                    (float *)(data + 0x1CC),
                                    *(short *)(data + 0x8A),
                                    *(float *)(data + 0x168));
    FUN_L00_0025b8c0_c((float *)(data + 0x170),
                       (float *)(data + 0x180),
                       path_target_position[0],
                       *(float *)&D_L14_001613A0_gp,
                       *(float *)&D_L14_001613A4_gp,
                       0.0f);
    FUN_L00_0025b8c0_c((float *)(data + 0x174),
                       (float *)(data + 0x184),
                       path_target_position[1],
                       *(float *)&D_L14_001613A0_gp,
                       *(float *)&D_L14_001613A4_gp,
                       0.0f);
    FUN_L00_0025b8c0_c((float *)(data + 0x178),
                       (float *)(data + 0x188),
                       path_target_position[2],
                       *(float *)&D_L14_001613A0_gp,
                       *(float *)&D_L14_001613A4_gp,
                       0.0f);
    if (*(short *)(data + 0x8A) == 0) {
        if (r != 0) {
            *(float *)(data + 0x1CC) = save;
            *(int *)(data + 0x84) = save2;
            *(float *)(data + 0x168) = -*(float *)(data + 0x168);
        }
    }
}

void update_blarg_heavy_interceptor_patrol_path_position(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *seg = (char *)l14_paths[*(int *)(data + 0x78)];
    int   source_segment_index = *(int *)(data + 0x84);
    float source_path_distance = 0.0f;
    float out[4];
    int   destination_segment_index;
    float frac;
    int   i;
    int  *tab;
    int   destination_path_loop_enabled;
    float destination_path_distance;

    for (i = 0; i < source_segment_index; i++) {
        source_path_distance += *(float *)(seg + i * 16 + 0x1C);
    }
    source_path_distance += *(float *)(data + 0x1CC);
    tab = ((int *)l14_paths[*(int *)(data + 0x74)]);
    destination_path_distance =
        source_path_distance / *(float *)(data + 0x16C) * *(float *)(data + 0x190);
    destination_path_loop_enabled = *(short *)(data + 0x8A);
    destination_segment_index = 0;
    frac = 0.0f;
    FUN_L00_0025d808_2af918(tab,
                            out,
                            &destination_segment_index,
                            &frac,
                            destination_path_loop_enabled,
                            destination_path_distance);
    FUN_L00_0025b8c0_c(&moby->pos.x,
                       (float *)(data + 0x194),
                       out[0],
                       *(float *)&D_L14_001613A0_gp,
                       *(float *)&D_L14_001613A4_gp,
                       0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.y,
                       (float *)(data + 0x198),
                       out[1],
                       *(float *)&D_L14_001613A0_gp,
                       *(float *)&D_L14_001613A4_gp,
                       0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.z,
                       (float *)(data + 0x19C),
                       out[2],
                       *(float *)&D_L14_001613A0_gp,
                       *(float *)&D_L14_001613A4_gp,
                       0.0f);
}

extern int
FUN_L00_0025d808_2afb40(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

extern short
    blarg_heavy_interceptor_return_path_position_damping_coefficient_gp __asm__("D_L14_001613FC")
        __attribute__((sda));

void update_blarg_heavy_interceptor_return_path_position(struct Moby *moby,
                                                         float        position_easing_factor,
                                                         float        path_traversal_speed,

                                                         float path_direction_multiplier) {
    char *data = (char *)moby->pvars;
    int  *list = ((int *)l14_paths[*(int *)(data + 0x204)]);
    float target_position[4];
    if (*(short *)(data + 0x208) != 0) {
        qcopy(target_position, (char *)list + list[0] * 16);
    } else {
        if (FUN_L00_0025d808_2afb40(list,
                                    target_position,
                                    data + 0x80,
                                    data + 0x8C,
                                    0,
                                    path_traversal_speed * frame_time *
                                        path_direction_multiplier) != 0) {
            *(short *)(data + 0x208) = 1;
        }
    }
    FUN_L00_0025b8c0_c(
        &moby->pos.x,
        (float *)(data + 0x194),
        target_position[0],
        position_easing_factor,
        *(float *)&blarg_heavy_interceptor_return_path_position_damping_coefficient_gp,
        0.0f);
    FUN_L00_0025b8c0_c(
        &moby->pos.y,
        (float *)(data + 0x198),
        target_position[1],
        position_easing_factor,
        *(float *)&blarg_heavy_interceptor_return_path_position_damping_coefficient_gp,
        0.0f);
    FUN_L00_0025b8c0_c(
        &moby->pos.z,
        (float *)(data + 0x19C),
        target_position[2],
        position_easing_factor,
        *(float *)&blarg_heavy_interceptor_return_path_position_damping_coefficient_gp,
        0.0f);
}

extern f32 blarg_heavy_interceptor_vertical_oscillation_amplitude_gp __asm__("D_L14_001613A8")
    __attribute__((sda));

extern f32 blarg_heavy_interceptor_vertical_oscillation_phase_increment_gp __asm__("D_L14_001613AC")
    __attribute__((sda));

extern f32 blarg_heavy_interceptor_rotation_oscillation_amplitude_gp __asm__("D_L14_001613B0")
    __attribute__((sda));

extern f32
    blarg_heavy_interceptor_rotation_oscillation_first_phase_increment_gp __asm__("D_L14_001613B4")
        __attribute__((sda));

extern f32
    blarg_heavy_interceptor_rotation_oscillation_second_phase_increment_gp __asm__("D_L14_001613B8")
        __attribute__((sda));

extern s32
    blarg_heavy_interceptor_patrol_heading_change_interval_frames_gp __asm__("D_L14_001613BC")
        __attribute__((sda));

extern f32 blarg_heavy_interceptor_patrol_heading_offset_magnitude_gp __asm__("D_L14_001613C0")
    __attribute__((sda));

extern f32 blarg_heavy_interceptor_patrol_heading_error_gain_gp __asm__("D_L14_001613C4")
    __attribute__((sda));

extern f32 blarg_heavy_interceptor_patrol_heading_damping_coefficient_gp __asm__("D_L14_001613C8")
    __attribute__((sda));

extern f32 blarg_heavy_interceptor_patrol_heading_snap_threshold_scale_gp __asm__("D_L14_001613CC")
    __attribute__((sda));

extern void
update_moby_vertical_oscillation(f32, f32, u8 *, f32 *, f32 *) __asm__("FUN_L00_00262b00");

extern void
update_moby_rotation_oscillation(u8 *, u8 *, u8 *, f32, f32, f32) __asm__("FUN_L00_00262b80");

void update_blarg_heavy_interceptor_oscillation_and_patrol_heading(struct Moby *moby) __asm__("FUN_L14_002aea08");

void update_blarg_heavy_interceptor_oscillation_and_patrol_heading(struct Moby *moby) {
    u8 *data = ((u8 *)moby->pvars);
    f32 heading_offset;
    update_moby_vertical_oscillation(
        blarg_heavy_interceptor_vertical_oscillation_amplitude_gp,
        blarg_heavy_interceptor_vertical_oscillation_phase_increment_gp,
        moby,
        (f32 *)(data + 0x1A0),
        (f32 *)(data + 0x1A4));
    update_moby_rotation_oscillation(
        moby,
        data + 0x1A8,
        data + 0x1AC,
        blarg_heavy_interceptor_rotation_oscillation_amplitude_gp,
        blarg_heavy_interceptor_rotation_oscillation_first_phase_increment_gp,
        blarg_heavy_interceptor_rotation_oscillation_second_phase_increment_gp);
    if (moby->state == 1) {
        if (fast_dec_timer_s16((s16 *)(data + 0x1B4))) {
            *(s16 *)(data + 0x1B4) =
                scale_game_frames(blarg_heavy_interceptor_patrol_heading_change_interval_frames_gp);
            heading_offset = blarg_heavy_interceptor_patrol_heading_offset_magnitude_gp;
            if (random_integer_below(2))
                heading_offset = -heading_offset;
            *(f32 *)(data + 0x1B0) = fast_add_rotations(moby->rot.z, heading_offset);
        }
        vendor_spring((f32 *)&moby->rot.z,
                      (f32 *)(data + 0x1B8),
                      *(f32 *)(data + 0x1B0),
                      blarg_heavy_interceptor_patrol_heading_error_gain_gp,
                      blarg_heavy_interceptor_patrol_heading_damping_coefficient_gp,
                      blarg_heavy_interceptor_patrol_heading_snap_threshold_scale_gp);
    }
}

void prepare_blarg_heavy_interceptor_light_ring_transform(u8 *moby) __asm__("FUN_L14_002aead8");

void prepare_blarg_heavy_interceptor_light_ring_transform(u8 *moby) {
    u8 *data = ((u8 *)MOBY(moby)->pvars);
    u8 *bone_transform;
    f32 bounding_sphere[4];
    if (random_integer_below(5)) {
        qcopy(bounding_sphere, moby + 0x10);
        bounding_sphere[3] = blarg_heavy_interceptor_culling_sphere_radius_gp;
        if (fast_bsphere_check((u8 *)bounding_sphere,
                               convert_integer_to_float(MOBY(moby)->draw_dist)) >= 0) {
            bone_transform = data + 0xA0;
            build_moby_bone_transform(moby, 1, bone_transform);
            fast_vec_normalize(bone_transform, bone_transform, 1.0f);
            fast_vec_normalize(data + 0xB0, data + 0xB0, 1.0f);
            fast_vec_normalize(data + 0xC0, data + 0xC0, 1.0f);
            *(s32 *)(data + 0x20C) = 2;
        }
    }
}

extern void FUN_L14_002ae260_u(void) __asm__("FUN_L14_002ae260");

int update_blarg_heavy_interceptor_aim_transforms(struct Moby *moby, int flag) {
    int   ok = 0;
    char *data = (char *)moby->pvars;
    float bounding_sphere[4];
    float half_span_vector[4];
    float rotation_angle;
    fast_vec_sub(half_span_vector, data + 0x170, data + 0xD0);
    fast_vec_scale(half_span_vector, half_span_vector, 0.5f);
    fast_vec_add(bounding_sphere, data + 0xD0, half_span_vector);
    bounding_sphere[3] = fast_vec_length(half_span_vector) + 2.0f;
    if (fast_bsphere_check((char *)bounding_sphere, convert_integer_to_float(moby->draw_dist)) >=
        0) {
        build_moby_bone_transform(moby, 1, data + 0xA0);
        ok = 1;
        fast_vec_normalize(data + 0xA0, data + 0xA0, 1.0f);
        fast_vec_normalize(data + 0xB0, data + 0xB0, 1.0f);
        fast_vec_normalize(data + 0xC0, data + 0xC0, 1.0f);
        rotation_angle = fast_subtract_rotations(
            fast_arc_tan(half_span_vector[0], half_span_vector[1]), moby->rot.z);
        FUN_L00_001fff28(data + 0xF0, 2, rotation_angle);
        *(float *)(data + 0x1BC) = rotation_angle;
        rotation_angle = -fast_arc_tan(vector_length_xy(half_span_vector), half_span_vector[2]);
        FUN_L00_001fff28(data + 0x130, 1, rotation_angle);
        *(float *)(data + 0x1C0) = rotation_angle;
        qcopy(data + 0x90, data + 0x170);
        if (flag != 0) {
            add_draw_callback(FUN_L14_002ae260_u, moby);
            *(int *)(data + 0x20C) |= 2;
        }
    }
    return ok;
}

extern s32 blarg_heavy_interceptor_light_ring_fade_values[] __asm__("D_L14_001D8210");

void update_blarg_heavy_interceptor_light_ring_alpha(struct Moby *moby) __asm__("FUN_L14_002aed50");

void update_blarg_heavy_interceptor_light_ring_alpha(struct Moby *moby) {
    u8 *data = ((u8 *)moby->pvars);
    s32 step;
    s32 i;
    f32 advanced_blink_phase = *(f32 *)(data + 0x164) + 0.5f;
    *(f32 *)(data + 0x164) = advanced_blink_phase;
    step = truncate_float_to_s32(advanced_blink_phase);
    if (1.0f <= *(f32 *)(data + 0x164))
        *(s32 *)(data + 0x164) = 0;
    for (i = 0; i < blarg_heavy_interceptor_light_ring_vertex_count_gp; i++) {
        s32 light_fade_value = blarg_heavy_interceptor_light_ring_fade_values[i];
        if (light_fade_value != 0) {
            light_fade_value += step;
            if (light_fade_value > 0x90)
                light_fade_value -= 0x80;
            blarg_heavy_interceptor_light_ring_fade_values[i] = light_fade_value;
            if (light_fade_value > 0x50)
                light_fade_value = 0xA0 - light_fade_value;
            blarg_heavy_interceptor_light_ring_vertex_colors[i] =
                (blarg_heavy_interceptor_light_ring_vertex_colors[i] & 0xFFFFFF) |
                (light_fade_value << 24);
        }
    }
}

extern int D_L14_001D8680[];

extern int D_L14_001D8680_b[] __asm__("D_L14_001D8680");

char *select_farthest_unselected_group_moby(int index, int *ids) __asm__("FUN_L14_002aef28");

extern void draw_blarg_heavy_interceptor_spinning_beam(struct Moby *) __asm__("FUN_L14_002add48");

extern void draw_blarg_heavy_interceptor_light_ring(char *) __asm__("FUN_L14_002ada18");

void draw_blarg_heavy_interceptor_group_effects(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int   i;
    int  *out;
    {
        int unused_id_sentinel = -1;
        for (i = 19; i >= 0; i--)
            D_L14_001D8680[i] = unused_id_sentinel;
    }
    i = 0;
    if (*(short *)(moby_vars + 0x20A) > 0) {
        int *ids = D_L14_001D8680_b;
        out = ids;
        do {
            char *selected_moby = select_farthest_unselected_group_moby(moby->group, ids);
            char *md;
            *out++ = ((short)MOBY(selected_moby)->save_id);
            md = ((char *)MOBY(selected_moby)->pvars);
            if (*(int *)(md + 0x20C) & 8)
                draw_blarg_heavy_interceptor_spinning_beam(selected_moby);
            if (*(int *)(md + 0x20C) & 2)
                draw_blarg_heavy_interceptor_light_ring(selected_moby);
            i++;
        } while (i < *(short *)(moby_vars + 0x20A));
    }
}

extern unsigned char *D_L14_0015FFD0_e[] __asm__("D_L14_0015FFD0")
    __attribute__((section(".sdata")));

extern short *D_L14_001AC2C0_2B0168[] __asm__("D_L14_001ABF40");

char *select_farthest_unselected_group_moby(int index, int *ids) {
    short *list_entry = D_L14_001AC2C0_2B0168[index];
    char  *best = 0;
    float  bestd = 0.0f;
    if (list_entry == 0)
        return 0;
    do {
        char *moby = (char *)D_L14_0015FFD0_e[2] + ((*(unsigned short *)list_entry & 0x7FFF) << 8);
        int   found = 0;
        int   i;
        for (i = 0; i < 20; i++) {
            if (((short)MOBY(moby)->save_id) == D_L14_001D8680[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            float candidate_distance = vector_distance_xy(D_L14_001674C0, moby + 0x10);
            if (bestd < candidate_distance) {
                bestd = candidate_distance;
                best = moby;
            }
        }
    } while (*list_entry++ >= 0);
    return best;
}

int count_moby_group_members(struct Moby *moby) {
    short *table = D_L14_001AC2C0_2B0168[moby->group];
    int    count = 0;
    if (table == 0)
        return 0;
    do {
        count++;
    } while (*table++ >= 0);
    return count;
}

void activate_waiting_turrets_in_group(s32 index) {
    u16 *list_entry = D_L14_001ABF40_u[index];
    u8  *base;
    u8  *listed_moby;

    if (list_entry == 0) {
        return;
    }
    base = D_L14_0015FFD8;
    do {
        listed_moby = base + ((*list_entry & 0x7FFF) << 8);
        if (listed_moby[0x20] == 1) {
            *(s16 *)(((u8 *)MOBY(listed_moby)->pvars) + 0x88) = 1;
        }
    } while (*(s16 *)list_entry++ >= 0);
}

extern short *D_L14_001ABF40_b45[] __asm__("D_L14_001ABF40");

extern unsigned char *D_L14_0015FFD8_b45 __asm__("D_L14_0015FFD8");

extern unsigned char *D_L14_0015FFD0_macro[] __asm__("D_L14_0015FFD0") MACRO_ADDR;

extern int D_L14_0016150C_gp __asm__("D_L14_0016150C") __attribute__((sda));

extern float D_L14_00161510_gp __asm__("D_L14_00161510") __attribute__((sda));

extern void FUN_00212ed8_b45(unsigned char *, int, int) __asm__("FUN_00212ed8");

extern int FUN_001f96f8_b45(int) __asm__("FUN_001f96f8");

extern float FUN_001fa6c0_b45(short) __asm__("FUN_001fa6c0");

int try_retract_turret_group(int index) {
    short         *list_entry = D_L14_001ABF40_b45[index];
    unsigned char *base;
    unsigned char *checked_moby;
    unsigned char *retracting_moby;
    unsigned char *turret_vars;
    int            moby_index_or_state;
    int            moby_index_or_retract_frames;
    float          inverse_retract_frames;
    float          retract_animation_speed;
    if (list_entry == 0) {
        return 0;
    }
    base = D_L14_0015FFD8_b45;
    do {
        moby_index_or_state = *(unsigned short *)list_entry & 0x7FFF;
        checked_moby = base + (moby_index_or_state << 8);
        if ((((unsigned char *)MOBY(checked_moby)->pvars))[0x8B] == 0) {
            moby_index_or_state = checked_moby[0x20];
            if (moby_index_or_state != 5 && moby_index_or_state != 1) {
                return 0;
            }
        }
    } while (*list_entry++ >= 0);
    list_entry = D_L14_001ABF40_b45[index];
    do {
        moby_index_or_retract_frames = *(unsigned short *)list_entry & 0x7FFF;
        retracting_moby = D_L14_0015FFD0_macro[2] + (moby_index_or_retract_frames << 8);
        if (retracting_moby[0x20] == 3) {
            turret_vars = ((unsigned char *)MOBY(retracting_moby)->pvars);
            FUN_00212ed8_b45(retracting_moby, 4, 0);
            retracting_moby[0x20] = 5;
            moby_index_or_retract_frames = FUN_001f96f8_b45(D_L14_0016150C_gp);
            *(short *)(turret_vars + 0x74) = moby_index_or_retract_frames;
            inverse_retract_frames = 1.0f / FUN_001fa6c0_b45(moby_index_or_retract_frames);
            retract_animation_speed = 1.0f / (*(short *)(turret_vars + 0x74) * D_L14_00161510_gp);
            *(float *)(turret_vars + 0x78) = inverse_retract_frames;
            MOBY(retracting_moby)->anim_speed = retract_animation_speed;
            *(float *)(turret_vars + 0x8C) = MOBY(retracting_moby)->rot.z;
            *(short *)(turret_vars + 0x88) = 0;
            turret_vars[0x8B] = 0;
        }
    } while (*list_entry++ >= 0);
    return 1;
}

extern char *D_L14_ffd0_e[] __asm__("D_L14_0015FFD0") MACRO_ADDR;

int is_turret_group_activated_or_empty(int index) {
    unsigned short *list = (unsigned short *)D_L14_001AC2C0_2B0168[index];
    int             count = 0;
    char           *base;
    if (list)
        goto scan;
success:
    return 1;
scan:
    base = D_L14_ffd0_e[2];
    do {
        unsigned short id = *list;
        char          *moby = base + ((id & 0x7FFF) << 8);
        int            state = ((signed char)MOBY(moby)->state);
        char          *data = ((char *)MOBY(moby)->pvars);
        if (state >= 0)
            count++;
        if (state >= 0 && *(short *)(data + 0x88))
            goto success;
        list++;
        if ((short)id < 0)
            return count == 0;
    } while (1);
}

extern void release_voice_slot(s32) __asm__("FUN_0022d798");

extern int random_integer_below(int);

extern float random_float_between(float, float);

extern int scale_game_frames(int);

extern void build_look_at_matrix(void *, void *, void *, float);

extern char *spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");

extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

s32 blarg_heavy_interceptor_patrol_heading_change_interval_frames_gp __asm__("D_L14_001613BC") =
    0x104;

int D_L14_0016150C_gp __asm__("D_L14_0016150C") = 0x3c;

int blarg_heavy_interceptor_spinning_beam_initial_alpha_gp __asm__("D_L14_00161450") = 0x5a;

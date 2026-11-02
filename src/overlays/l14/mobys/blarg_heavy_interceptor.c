/* Level 14 Blarg heavy interceptor moby class: patrol paths, aiming, light ring effects and turret groups. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/overlay/shared/mobys/moby_anim.h"
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
#include "rnc/overlay/shared/world/level_path.h"
#include "sda.h"
#include "rnc/overlay/l14/mobys/blarg_heavy_interceptor_globals.h"

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

extern void build_axis_rotation_quaternion(void *, int, float) __asm__("FUN_L00_001fff28");

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

#include "rnc/overlay/l14/mobys/blarg_heavy_interceptor_data/blarg_heavy_interceptor_update.h"

extern f32 l14_blarg_heavy_interceptor_path_easing_error_gain __asm__("D_L14_001613A0")
    __attribute__((sda));

extern f32 l14_blarg_heavy_interceptor_path_easing_damping_coefficient __asm__("D_L14_001613A4")
    __attribute__((sda));

extern Vec4 D_L14_00174560;

void blarg_heavy_interceptor_update(struct Moby *m) __asm__("FUN_L14_002ac618");

typedef struct BlargHeavyInterceptorVars {
    u8  pad_0[0x20];
    f32 health;
    s16 max_health;
    u8  pad_26[0x2];
    u8  unk28;
    u8  pad_29[0x5];
    u8  unk2E;
    u8  pad_2F[0x11];
    f32 unk40[4];
    u8  pad_50[0x8];
    u8  unk58;
    u8  pad_59[0x1];
    u8  unk5A;
    u8  pad_5B[0x5];
    s16 anim_state;
    u8  pad_62[0x5];
    u8  unk67;
    u8  pad_68[0x8];
    s32 turret_group;
    s32 spawn_path;
    s32 unk78;
    u8  pad_7C[0x4];
    s32 path_point;
    s32 path_segment;
    s16 unk88;
    s16 rejoin_path_loop;
    f32 unk8C;
    f32 unk90[4];
    f32 unkA0[4];
    f32 unkB0[4];
    f32 unkC0[4];
    f32 unkD0[4];
    f32 unkE0[4];
    f32 unkF0[4];
    u8  pad_100[0x20];
    f32 unk120[4];
    f32 unk130[4];
    f32 unk140[4];
    u8  pad_150[0x10];
    f32 beam_rotation_blink_and_aim_path_state[4];
    f32 unk170[4];
    f32 unk180[4];
    f32 unk190[4];
    f32 oscillation_state[4];
    f32 unk1B0[4];
    f32 unk1C0[4];
    f32 unk1D0[4];
    f32 unk1E0[4];
    s16 timer_1F0;
    s16 timer_1F2;
    f32 unk1F4;
    f32 unk1F8;
    f32 yaw_drift;
    s16 unk200;
    s16 unk202;
    s32 patrol_path;
    s16 unk208;
    s16 group_member_count;
    s32 flags;
    f32 unk210;
    f32 unk214;
    f32 unk218;
    f32 unk21C;
} BlargHeavyInterceptorVars;

/* Moby class 8 */
void blarg_heavy_interceptor_update(struct Moby *moby) {
    enum {
        INTERCEPTOR_STATE_GROUP_EFFECTS_CONTROLLER = 8
    };
    BlargHeavyInterceptorVars *moby_vars = (BlargHeavyInterceptorVars *)moby->pvars;
    float                      vector_scratch[4];
    float                      secondary_vector_scratch[4];
    int                        near_seg;
    float                      near_t;
    int                        path_seg;
    float                      path_t;

    if ((u8)moby_vars->unk2E == 2) {
        moby_vars->flags = 0;
        FUN_0020c828(moby);
        return;
    }
    if (moby->state == INTERCEPTOR_STATE_GROUP_EFFECTS_CONTROLLER) {
        FUN_L14_002aed50_c618(moby);
        add_draw_callback(draw_blarg_heavy_interceptor_group_effects, moby);
        return;
    }
    moby_vars->flags = 0;
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
    if (moby->state > INTERCEPTOR_STATE_INITIALIZE &&
        moby->state < INTERCEPTOR_STATE_INITIALIZE_DEATH_FALL) {
        if (fast_vec_dist(&moby->pos, D_L14_001674C0) < 40.0f) {
            FUN_L00_0025a120(moby);
        }
        moby_vars->unk200 = 0;
    }
    switch (moby->state) {
    case INTERCEPTOR_STATE_INITIALIZE:
        get_moby_target_info(moby)[0xE] = 1;
        if (*(s16 *)((u8 *)moby_vars + 0x1B6) == 1) {
            moby->flags = (moby->flags | 0x41) & 0xEFFF;
            moby->collision = 0;
            moby->update_dist = 0xFF;
            moby->state = INTERCEPTOR_STATE_GROUP_EFFECTS_CONTROLLER;
            moby_vars->unk58 = truncate_float_to_s32(17.0f);
            moby_vars->unk5A = truncate_float_to_s32(6.0f);
            moby_vars->group_member_count = count_moby_group_members(moby);
            return;
        }
        if (moby_vars->spawn_path == -1 || *((int *)l14_paths[moby_vars->spawn_path]) == 0 ||
            moby_vars->unk78 == -1 || *((int *)l14_paths[moby_vars->unk78]) == 0 ||
            moby_vars->turret_group < 0 || moby_vars->patrol_path == -1 ||
            *((int *)l14_paths[moby_vars->patrol_path]) == 0) {
            FUN_0020c828(moby);
            return;
        }
        moby->shadow_range = 0x20;
        moby_vars->health = 4.0f;
        moby_vars->max_health = 4;
        moby_vars->unk28 = 3;
        qcopy(&moby->pos, (char *)l14_paths[moby_vars->spawn_path] + 0x10);
        qcopy(moby_vars->unk170, (char *)l14_paths[moby_vars->unk78] + 0x10);
        initialize_blarg_heavy_interceptor_paths(moby);
        moby_vars->unk88 = 1;
        moby_vars->unk21C = -1.0f;
        moby_vars->unk180[0] = moby_vars->unk180[1] = moby_vars->unk180[2] = 0.0f;
        moby_vars->unk190[1] = moby_vars->unk190[2] = moby_vars->unk190[3] = 0.0f;
        moby_vars->beam_rotation_blink_and_aim_path_state[0] = 0.0f;
        moby_vars->flags = 0;
        moby_vars->beam_rotation_blink_and_aim_path_state[1] = 0.0f;
        moby_vars->oscillation_state[0] = 0.0f;
        moby_vars->oscillation_state[1] = 0.0f;
        moby_vars->oscillation_state[2] = 0.0f;
        moby_vars->oscillation_state[3] = 0.0f;
        moby_vars->unk1B0[0] = moby->rot.z;
        *(s16 *)&moby_vars->unk1B0[1] = 0;
        moby_vars->unk1B0[2] = 0.0f;
        fill_transfer_words(moby_vars->unkE0, 0, 0x40);
        fill_transfer_words(moby_vars->unk120, 0, 0x40);
        attach_manipulator_c618(moby, 0, moby_vars->unkE0);
        attach_manipulator_c618(moby, 1, moby_vars->unk120);
        moby_vars->unk1C0[2] = moby_vars->unk1C0[1] = 0.0f;
        *(int *)&moby_vars->unk180[3] = 0;
        moby->state = INTERCEPTOR_STATE_PATROL_AND_SCAN;
        moby->command = 0;
        break;
    case INTERCEPTOR_STATE_PATROL_AND_SCAN: {
        int found;
        int hit;

        moby->glow_color = 0xFF000000 | (blarg_heavy_interceptor_patrol_glow_blue_gp << 16) |
                           (blarg_heavy_interceptor_patrol_glow_green_gp << 8) |
                           blarg_heavy_interceptor_patrol_glow_red_gp;
        if (is_turret_group_activated_or_empty(moby_vars->turret_group)) {
            moby->state = INTERCEPTOR_STATE_TRACK_TARGET;
            moby_vars->timer_1F2 =
                scale_game_frames(blarg_heavy_interceptor_target_tracking_duration_frames_gp);
        }
        moby_vars->unk202 = 0;
        moby_vars->unk200 = 1;
        found = 0;
        hit = update_blarg_heavy_interceptor_aim_transforms(moby, 1);
        update_blarg_heavy_interceptor_patrol_aim_point(moby);
        update_blarg_heavy_interceptor_patrol_path_position(moby);
        FUN_L14_002aea08_c618(moby);
        if (hit) {
            lerp_vector(vector_scratch, moby_vars->unk90, moby_vars->unkD0, 0.5f);
            qcopy(secondary_vector_scratch, moby_vars->unk90);
            noop_main_callback(secondary_vector_scratch, moby_vars->unk214 * 0.8f);
            noop_main_callback(vector_scratch, moby_vars->unk214 * 0.48f);
            if (FUN_L00_001f2868_c618(moby_vars->unk214 * 0.48f, vector_scratch, 0, 0, 0) ||
                FUN_L00_001f2868_c618(
                    moby_vars->unk214 * 0.9f, secondary_vector_scratch, 0, 0, 0)) {
                struct Moby *t = D_L14_00174540.target;

                if ((char *)t == D_001413D0_lui ||
                    (t != 0 && (t->oclass == 0xCB || t->oclass == 0x10E))) {
                    found = 1;
                    *(struct Moby **)(&moby_vars->unk180[3]) = D_L14_00174540.target;
                }
            }
        }
        if (!found) {
            break;
        }
    }
    case INTERCEPTOR_STATE_ALERT_GROUP: {
        s32 voice = *(s32 *)&moby_vars->unk1D0[0];

        if (voice != -1) {
            VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        *(s32 *)&moby_vars->unk1D0[0] = -1;
        activate_waiting_turrets_in_group(moby_vars->turret_group);
        moby->state = INTERCEPTOR_STATE_TRACK_TARGET;
        moby_vars->timer_1F2 =
            scale_game_frames(blarg_heavy_interceptor_target_tracking_duration_frames_gp);
        break;
    }
    case INTERCEPTOR_STATE_TRACK_TARGET:
        moby->glow_color = 0xFF000000 |
                           (blarg_heavy_interceptor_tracking_and_group_wait_glow_blue_gp << 16) |
                           (blarg_heavy_interceptor_tracking_and_group_wait_glow_green_gp << 8) |
                           blarg_heavy_interceptor_tracking_and_group_wait_glow_red_gp;
        if (*(struct Moby **)(&moby_vars->unk180[3]) != 0) {
            moby_vars->unk200 = 1;
            moby_vars->unk202 = 1;
            update_blarg_heavy_interceptor_aim_transforms(moby, 1);
            qcopy(vector_scratch, &(*(struct Moby **)(&moby_vars->unk180[3]))->pos);
            vector_scratch[2] += 0.5f;
            qcopy(secondary_vector_scratch, vector_scratch);
            secondary_vector_scratch[2] -= 4.0f;
            if (FUN_001efa68_c618(vector_scratch, secondary_vector_scratch, 0x12, 0, 0)) {
                qcopy(vector_scratch, &D_L14_00174560);
            } else {
                vector_scratch[2] -= 0.5f;
            }
            ease_c618(moby_vars->unk170, vector_scratch[0], moby_vars->unk180, 0.003f, 0.2f, 0.0f);
            ease_c618(&moby_vars->unk170[1],
                      vector_scratch[1],
                      &moby_vars->unk180[1],
                      0.003f,
                      0.2f,
                      0.0f);
            ease_c618(&moby_vars->unk170[2],
                      vector_scratch[2],
                      &moby_vars->unk180[2],
                      0.003f,
                      0.2f,
                      0.0f);
            update_blarg_heavy_interceptor_patrol_path_position(moby);
            if ((s8)(*(struct Moby **)(&moby_vars->unk180[3]))->state < 0 ||
                fast_dec_timer_s16(&moby_vars->timer_1F2)) {
                moby_vars->unk8C = 0.0f;
                moby_vars->path_point = 0;
                moby_vars->unk208 = 0;
                moby->state = INTERCEPTOR_STATE_WAIT_FOR_GROUP_READY;
            }
        } else {
            moby_vars->path_point = 0;
            moby_vars->unk8C = 0.0f;
            moby_vars->unk208 = 0;
            moby->state = INTERCEPTOR_STATE_WAIT_FOR_GROUP_READY;
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
            p = &moby_vars->unk1D0[1];
            p[0] = secondary_vector_scratch[0];
            p[1] = secondary_vector_scratch[1];
            p[2] = secondary_vector_scratch[2];
            moby_vars->flags |= 8;
            FUN_L14_002aea08_c618(moby);
        }
        if (moby->state == INTERCEPTOR_STATE_WAIT_FOR_GROUP_READY) {
            if (moby->prev_seq == 0) {
                blend_moby_animation_c618(moby, 2, 0, 5);
            }
            update_blarg_heavy_interceptor_return_path_position(
                moby,
                blarg_heavy_interceptor_group_wait_return_path_position_easing_factor_gp,
                blarg_heavy_interceptor_group_wait_return_path_traversal_speed_gp,
                1.0f);
            if (moby_vars->unk1C0[0] != 0.0f || moby_vars->unk1B0[3] != 0.0f) {
                ease_c618(moby_vars->unk1C0, 0.0f, &moby_vars->unk1C0[2], 0.004f, 0.2f, 0.0f);
                ease_c618(&moby_vars->unk1B0[3], 0.0f, &moby_vars->unk1C0[1], 0.004f, 0.2f, 0.0f);
                if (absolute_float(moby_vars->unk1C0[0]) < 0.0001f) {
                    moby_vars->unk1C0[0] = 0.0f;
                }
                if (absolute_float(moby_vars->unk1B0[3]) < 0.0001f) {
                    moby_vars->unk1B0[3] = 0.0f;
                }
                build_axis_rotation_quaternion(moby_vars->unkF0, 2, moby_vars->unk1B0[3]);
                build_axis_rotation_quaternion(moby_vars->unk130, 1, moby_vars->unk1C0[0]);
            }
            if (try_retract_turret_group(moby_vars->turret_group)) {
                LevelPath *p = l14_paths[moby_vars->patrol_path];

                moby->state = INTERCEPTOR_STATE_FOLLOW_RETURN_PATH;
                moby_vars->path_point = p->count - 2;
                moby_vars->unk8C = p->points[moby_vars->path_point].f[3];
                moby_vars->unk208 = 0;
                update_blarg_heavy_interceptor_return_path_position(moby, 0.01f, 5.0f, -1.0f);
                blend_moby_animation_c618(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case INTERCEPTOR_STATE_FOLLOW_RETURN_PATH:
        FUN_L14_002aea08_c618(moby);
        update_blarg_heavy_interceptor_return_path_position(moby, 0.01f, 5.0f, -1.0f);
        if (moby_vars->unk208 != 0) {
            int *tab = ((int *)l14_paths[moby_vars->unk78]);

            near_seg = 0;
            near_t = 0.0f;
            FUN_L00_0025df68_c618(
                tab, &moby->pos, vector_scratch, &near_seg, &near_t, 0, 40.0f, 5.0f, 0.0f);
            moby_vars->path_segment = near_seg;
            moby_vars->unk1C0[3] = near_t;
            moby_vars->unk200 = 0;
            moby->state = INTERCEPTOR_STATE_REJOIN_PATROL;
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
        source_segment_index = moby_vars->path_segment;
        seg = (char *)l14_paths[moby_vars->unk78];
        source_path_distance = 0.0f;
        for (i = 0; i < source_segment_index; i++) {
            source_path_distance += *(float *)(seg + i * 16 + 0x1C);
        }
        source_path_distance += moby_vars->unk1C0[3];
        tab = ((int *)l14_paths[moby_vars->spawn_path]);
        t = source_path_distance / moby_vars->beam_rotation_blink_and_aim_path_state[3] * moby_vars->unk190[0];
        rejoin_path_loop_enabled = moby_vars->rejoin_path_loop;
        path_seg = 0;
        path_t = 0.0f;
        FUN_L00_0025d808_c618(tab, vector_scratch, &path_seg, &path_t, rejoin_path_loop_enabled, t);
        ease_c618(&moby->pos.x,
                  vector_scratch[0],
                  &moby_vars->unk190[1],
                  0.005f,
                  l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                  0.0f);
        ease_c618(&moby->pos.y,
                  vector_scratch[1],
                  &moby_vars->unk190[2],
                  0.005f,
                  l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                  0.0f);
        ease_c618(&moby->pos.z,
                  vector_scratch[2],
                  &moby_vars->unk190[3],
                  0.005f,
                  l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                  0.0f);
        update_blarg_heavy_interceptor_aim_transforms(moby, 0);
        if (fast_vec_dist(&moby->pos, vector_scratch) < 0.5f) {
            moby->state = INTERCEPTOR_STATE_PATROL_AND_SCAN;
        }
        FUN_L00_0025d808_c618(((int *)l14_paths[moby_vars->unk78]),
                              vector_scratch,
                              &moby_vars->path_segment,
                              &moby_vars->unk1C0[3],
                              moby_vars->rejoin_path_loop,
                              0.0f);
        ease_c618(moby_vars->unk170,
                  vector_scratch[0],
                  moby_vars->unk180,
                  l14_blarg_heavy_interceptor_path_easing_error_gain,
                  l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                  0.0f);
        ease_c618(&moby_vars->unk170[1],
                  vector_scratch[1],
                  &moby_vars->unk180[1],
                  l14_blarg_heavy_interceptor_path_easing_error_gain,
                  l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                  0.0f);
        ease_c618(&moby_vars->unk170[2],
                  vector_scratch[2],
                  &moby_vars->unk180[2],
                  l14_blarg_heavy_interceptor_path_easing_error_gain,
                  l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                  0.0f);
        break;
    }
    case INTERCEPTOR_STATE_INITIALIZE_DEATH_FALL: {
        s32   voice;
        float death_launch_angle;

        moby_vars->timer_1F0 =
            scale_game_frames(blarg_heavy_interceptor_death_fall_duration_frames_gp);
        activate_waiting_turrets_in_group(moby_vars->turret_group);
        voice = *(s32 *)&moby_vars->unk1D0[0];
        if (voice != -1) {
            VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        *(s32 *)&moby_vars->unk1D0[0] = -1;
        FUN_L00_0025e450_c618(moby,
                              moby_vars->unk40,
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
            moby_vars->unk1E0, vector_scratch, &hero.gravity_dir, death_launch_angle);
        moby_vars->unk1E0[2] = random_float_between(
            blarg_heavy_interceptor_death_launch_vertical_velocity_range_start_gp * frame_scale,
            blarg_heavy_interceptor_death_launch_vertical_velocity_range_end_gp * frame_scale);
        moby->scale *= 0.1f;
        moby_vars->unk140[0] = 10.0f;
        moby_vars->unk140[1] = 10.0f;
        moby_vars->unk140[2] = 10.0f;
        moby_vars->unk1F4 = randf_sym(0.0f, 0.08726646f);
        moby_vars->unk1F8 = randf_sym(0.0f, 0.08726646f);
        moby_vars->yaw_drift = randf_sym(0.0f, 0.08726646f);
        moby->rot.x = fast_add_rotations(moby->rot.x, randf_sym(0.0f, 0.7853982f));
        moby->rot.y = fast_add_rotations(moby->rot.y, randf_sym(0.0f, 0.7853982f));
        moby->state = INTERCEPTOR_STATE_DEATH_FALL;
        FUN_L14_002aead8_c618(moby);
        return;
    }
    case INTERCEPTOR_STATE_DEATH_FALL: {
        int i;

        fast_vec_add(&moby->pos, &moby->pos, moby_vars->unk1E0);
        if (absolute_float(moby_vars->unk1E0[0]) >
            blarg_heavy_interceptor_death_fall_horizontal_drag_speed_threshold_gp * frame_scale) {
            moby_vars->unk1E0[0] *=
                (blarg_heavy_interceptor_death_fall_velocity_retention_factor_gp - 1.0f) *
                    frame_scale +
                1.0f;
        }
        if (absolute_float(moby_vars->unk1E0[1]) >
            blarg_heavy_interceptor_death_fall_horizontal_drag_speed_threshold_gp * frame_scale) {
            moby_vars->unk1E0[1] *=
                (blarg_heavy_interceptor_death_fall_velocity_retention_factor_gp - 1.0f) *
                    frame_scale +
                1.0f;
        }
        if (moby_vars->unk1E0[2] > 0.0f) {
            moby_vars->unk1E0[2] *=
                (blarg_heavy_interceptor_death_fall_velocity_retention_factor_gp - 1.0f) *
                    frame_scale +
                1.0f;
        }
        moby_vars->unk1E0[2] -= blarg_heavy_interceptor_death_fall_gravity_gp * frame_scale_sq;
        moby->rot.z = fast_add_rotations(moby->rot.z, moby_vars->yaw_drift);
        moby_vars->unk140[0] = 10.0f;
        moby_vars->unk140[1] = 10.0f;
        moby_vars->unk140[2] = 10.0f;
        if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
            moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
            FUN_0020c828(moby);
            return;
        }
        if (fast_dec_timer_s16(&moby_vars->timer_1F0) || coll_sphere(0.5f, &moby->pos, 0, moby)) {
            FUN_L00_0025e450_c618(moby,
                                  moby_vars->unk40,
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
    if ((s8)moby->state >= INTERCEPTOR_STATE_INITIALIZE && moby->prev_seq == 1) {
        if (moby->command == 1) {
            spawn_particles_at_shape_points(moby, D_L14_0015F580, 0xFFFF);
        }
    } else {
        moby->command = 0;
    }
    if (moby->state == INTERCEPTOR_STATE_PATROL_AND_SCAN &&
        !voice_is_playing(moby, *(s32 *)&moby_vars->unk1D0[0])) {
        *(s32 *)&moby_vars->unk1D0[0] = allocate_voice_c618(0, 4, moby);
    }
    if ((moby->state == INTERCEPTOR_STATE_TRACK_TARGET ||
         moby->state == INTERCEPTOR_STATE_WAIT_FOR_GROUP_READY) &&
        !voice_is_playing(moby, *(s32 *)&moby_vars->unk1D0[0])) {
        *(s32 *)&moby_vars->unk1D0[0] = allocate_voice_c618(2, 4, moby);
    }
}

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");

extern void FUN_L00_00257470(void *, int, int);

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

void blend_moby_animation(MobyAnim *m, int a, int b, int c) __asm__("FUN_00212f90");

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
    enum {
        HIT_REACTION_STATUS_SKIP_DAMAGE_RESPONSE = 1
    };
    enum {
        HIT_REACTION_STATUS_ENABLE_HIT_PARTICLES = 3
    };
    BlargHeavyInterceptorVars *blarg_heavy_interceptor_vars =
        (BlargHeavyInterceptorVars *)moby->pvars;
    int   hit_reaction_status;
    float damage_amount;
    int   hit_reaction_result;
    damage_amount = 0.0f;
    hit_reaction_result = hit_reaction(moby,
                                       find_moby_hit(moby, 0x330000, 0),
                                       &blarg_heavy_interceptor_vars->health,
                                       0,
                                       &hit_reaction_status,
                                       &damage_amount,
                                       0,
                                       4);
    if (hit_reaction_status != HIT_REACTION_STATUS_SKIP_DAMAGE_RESPONSE &&
        moby->state < INTERCEPTOR_STATE_INITIALIZE_DEATH_FALL) {
        float remaining_health = blarg_heavy_interceptor_vars->health - damage_amount;
        blarg_heavy_interceptor_vars->health = remaining_health;
        if (remaining_health <= 0.0f)
            hit_reaction_result = 1;
        if (moby->state == INTERCEPTOR_STATE_PATROL_AND_SCAN)
            moby->state = INTERCEPTOR_STATE_ALERT_GROUP;
        if (hit_reaction_status == HIT_REACTION_STATUS_ENABLE_HIT_PARTICLES)
            moby->command = 1;
        switch (hit_reaction_result) {
        case HIT_REACTION_ANIMATE_WITH_LOW_RED_FLASH:
        case HIT_REACTION_LOW_RED_FLASH_AND_HIT_ANIMATION:
            blarg_heavy_interceptor_vars->unk67 = 0x64;
            blend_moby_animation(moby, 1, 0, 5);
            break;
        case HIT_REACTION_PLAY_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_AND_HIT_ANIMATION:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_WITH_HIGH_RED_FLASH:
        case HIT_REACTION_HIGH_RED_FLASH_WITH_BLENDED_HIT_ANIMATION:
        case HIT_REACTION_BLEND_HIT_ANIMATION_AND_HIGH_RED_FLASH:
            ((unsigned char *)blarg_heavy_interceptor_vars)[0x67] = 0xC8;
            blend_moby_animation(moby, 1, 0, 5);
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_DEATH_FALL:
            moby->state = INTERCEPTOR_STATE_INITIALIZE_DEATH_FALL;
            moby->flags &= 0xEFFF;
            FUN_L00_00257470(moby, 0, -1);
            break;
        case HIT_REACTION_NO_ACTION:
        case HIT_REACTION_NONE:
            break;
        }
        trooper_anim_update(moby, &blarg_heavy_interceptor_vars->anim_state);
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, &blarg_heavy_interceptor_vars->anim_state);
}

extern s32 blarg_heavy_interceptor_light_ring_vertex_count_gp __asm__("D_L14_00161418")
    __attribute__((sda));

/* The light ring mesh: 82 vertices (the count above is 82, the draw transforms them into an 82-entry stack array). */
#define BLARG_LIGHT_RING_VERTEX_CAPACITY 82

float blarg_heavy_interceptor_light_ring_vertex_positions[82][3] __asm__("D_L14_001D7A60") = {
    {  3.71458f, -3.686132f,     -0.0f},
    { 3.612892f, -1.164749f,     -0.0f},
    { 0.743221f, -0.617193f,      0.0f},
    { 0.838738f, -0.432467f,     -0.0f},
    { 0.003774f, -0.298786f,      0.0f},
    { 0.838738f, -0.432467f,     -0.0f},
    {-0.000161f, -0.211026f,      0.0f},
    { 0.838738f, -0.432467f,     -0.0f},
    {-0.000161f, -0.200697f,  0.06509f},
    { 0.838738f, -0.432467f,     -0.0f},
    { 0.967502f, -0.443626f, 0.143877f},
    { 3.612892f, -1.164749f,     -0.0f},
    { 0.967502f, -0.443626f, 0.143877f},
    { 3.887644f, -1.176718f, 0.381633f},
    { 0.967502f, -0.443626f, 0.143877f},
    { 3.912533f, -1.006291f, 0.729763f},
    { 0.967502f, -0.443626f, 0.143877f},
    { 0.975131f,    -0.379f, 0.274851f},
    {-0.000161f, -0.200697f,  0.06509f},
    { 0.975131f,    -0.379f, 0.274851f},
    {-0.000161f, -0.170723f, 0.123809f},
    { 0.975131f,    -0.379f, 0.274851f},
    {-0.000161f, -0.124038f, 0.170408f},
    { 0.975131f,    -0.379f, 0.274851f},
    {  0.97663f, -0.275592f,  0.37862f},
    { 3.912533f, -1.006291f, 0.729763f},
    {  0.97663f, -0.275592f,  0.37862f},
    { 3.827825f,  -0.71797f, 0.986377f},
    {  0.97663f, -0.275592f,  0.37862f},
    { 3.581488f, -0.357366f, 1.097827f},
    {  0.97663f, -0.275592f,  0.37862f},
    { 0.772205f, -0.128213f, 0.393869f},
    {-0.000161f, -0.124038f, 0.170408f},
    { 0.772205f, -0.128213f, 0.393869f},
    {-0.000161f, -0.065211f, 0.200327f},
    { 0.772205f, -0.128213f, 0.393869f},
    {-0.000161f,       0.0f, 0.210636f},
    { 0.772205f, -0.128213f, 0.393869f},
    { 0.634602f,       0.0f, 0.377883f},
    { 3.581488f, -0.357366f, 1.097827f},
    { 0.634602f,       0.0f, 0.377883f},
    { 3.280884f,       0.0f, 1.075121f},
    { 0.634602f,       0.0f, 0.377883f},
    { 3.295124f,  0.334007f, 1.026069f},
    { 0.634602f,       0.0f, 0.377883f},
    { 0.665676f,  0.119523f, 0.367174f},
    {-0.000161f,       0.0f, 0.210636f},
    { 0.665676f,  0.119523f, 0.367174f},
    {-0.000161f,  0.065211f, 0.200327f},
    { 0.665676f,  0.119523f, 0.367174f},
    {-0.000161f,  0.124038f, 0.170408f},
    { 0.665676f,  0.119523f, 0.367174f},
    { 0.811952f,  0.250042f, 0.343517f},
    { 3.295124f,  0.334007f, 1.026069f},
    { 0.811952f,  0.250042f, 0.343517f},
    { 3.482124f,  0.664333f, 0.912688f},
    { 0.811952f,  0.250042f, 0.343517f},
    { 3.664679f,  0.953361f, 0.691379f},
    { 0.811952f,  0.250042f, 0.343517f},
    { 0.859364f,  0.354278f, 0.256923f},
    {-0.000161f,  0.124038f, 0.170408f},
    { 0.859364f,  0.354278f, 0.256923f},
    {-0.000161f,  0.170723f, 0.123809f},
    { 0.859364f,  0.354278f, 0.256923f},
    {-0.000161f,  0.200697f,  0.06509f},
    { 0.859364f,  0.354278f, 0.256923f},
    { 0.799961f,  0.401565f, 0.130236f},
    { 3.664679f,  0.953361f, 0.691379f},
    { 0.799961f,  0.401565f, 0.130236f},
    { 3.729607f,  1.137043f, 0.368765f},
    { 0.727714f,  0.403161f,      0.0f},
    { 3.762307f,  1.204196f,      0.0f},
    { 0.727714f,  0.403161f,      0.0f},
    { 3.762307f,  1.204196f,      0.0f},
    { 0.727714f,  0.403161f,      0.0f},
    { 0.727714f,  0.403161f,      0.0f},
    { 0.727714f,  0.403161f,      0.0f},
    {-0.000161f,  0.211026f,      0.0f},
    { 0.727714f,  0.403161f,      0.0f},
    {-0.000161f,  0.211026f,      0.0f},
    { 0.799961f,  0.401565f, 0.130236f},
    {-0.000161f,  0.200697f,  0.06509f},
};

s32 blarg_heavy_interceptor_light_ring_vertex_colors[82] __asm__("D_L14_001D80C8") = {
    0x507f7f,   0x507f7f,   0x507f7f,   0x10507f7f, 0x507f7f,   0x16507f7f, 0x7f507f7f, 0x20507f7f,
    0x7f507f7f, 0x26507f7f, 0x30507f7f, 0x507f7f,   0x36507f7f, 0x507f7f,   0x40507f7f, 0x507f7f,
    0x46507f7f, 0x50507f7f, 0x7f507f7f, 0x46507f7f, 0x7f507f7f, 0x40507f7f, 0x7f507f7f, 0x36507f7f,
    0x30507f7f, 0x507f7f,   0x26507f7f, 0x507f7f,   0x20507f7f, 0x507f7f,   0x16507f7f, 0x10507f7f,
    0x7f507f7f, 0x16507f7f, 0x7f507f7f, 0x20507f7f, 0x7f507f7f, 0x26507f7f, 0x30507f7f, 0x507f7f,
    0x36507f7f, 0x507f7f,   0x40507f7f, 0x507f7f,   0x46507f7f, 0x50507f7f, 0x7f507f7f, 0x46507f7f,
    0x7f507f7f, 0x40507f7f, 0x7f507f7f, 0x36507f7f, 0x30507f7f, 0x507f7f,   0x26507f7f, 0x507f7f,
    0x20507f7f, 0x507f7f,   0x16507f7f, 0x10507f7f, 0x7f507f7f, 0x16507f7f, 0x7f507f7f, 0x20507f7f,
    0x7f507f7f, 0x26507f7f, 0x30507f7f, 0x507f7f,   0x36507f7f, 0x507f7f,   0x40507f7f, 0x507f7f,
    0x46507f7f, 0x507f7f,   0x50507f7f, 0x507f7f,   0x46507f7f, 0x507f7f,   0x40507f7f, 0x7f507f7f,
    0x36507f7f, 0x7f507f7f};

/* One (u, v) pair per vertex: FUN_L00_001fde98 copies 8 bytes per vertex from this argument. */
float blarg_heavy_interceptor_light_ring_texture_coordinates[82][2] __asm__("D_L14_001D7E38") = {
    {0.5f, 0.0f},
    {0.5f, 0.0f},
    {0.5f, 0.0f},
    {0.5f, 0.3f},
    {0.5f, 0.0f},
    {0.5f, 0.3f},
    {0.5f, 1.0f},
    {0.5f, 0.3f},
    {0.5f, 1.0f},
    {0.5f, 0.3f},
    {0.5f, 0.2f},
    {0.5f, 0.0f},
    {0.5f, 0.2f},
    {0.5f, 0.0f},
    {0.5f, 0.2f},
    {0.5f, 0.0f},
    {0.5f, 0.2f},
    {0.5f, 0.1f},
    {0.5f, 1.0f},
    {0.5f, 0.1f},
    {0.5f, 1.0f},
    {0.5f, 0.1f},
    {0.5f, 1.0f},
    {0.5f, 0.1f},
    {0.5f, 0.2f},
    {0.5f, 0.0f},
    {0.5f, 0.2f},
    {0.5f, 0.0f},
    {0.5f, 0.2f},
    {0.5f, 0.0f},
    {0.5f, 0.2f},
    {0.5f, 0.3f},
    {0.5f, 1.0f},
    {0.5f, 0.3f},
    {0.5f, 1.0f},
    {0.5f, 0.3f},
    {0.5f, 1.0f},
    {0.5f, 0.3f},
    {0.5f, 0.4f},
    {0.5f, 0.0f},
    {0.5f, 0.4f},
    {0.5f, 0.0f},
    {0.5f, 0.4f},
    {0.5f, 0.0f},
    {0.5f, 0.4f},
    {0.5f, 0.5f},
    {0.5f, 1.0f},
    {0.5f, 0.5f},
    {0.5f, 1.0f},
    {0.5f, 0.5f},
    {0.5f, 1.0f},
    {0.5f, 0.5f},
    {0.5f, 0.6f},
    {0.5f, 0.0f},
    {0.5f, 0.6f},
    {0.5f, 0.0f},
    {0.5f, 0.6f},
    {0.5f, 0.0f},
    {0.5f, 0.6f},
    {0.5f, 0.7f},
    {0.5f, 1.0f},
    {0.5f, 0.7f},
    {0.5f, 1.0f},
    {0.5f, 0.7f},
    {0.5f, 1.0f},
    {0.5f, 0.7f},
    {0.5f, 0.8f},
    {0.5f, 0.0f},
    {0.5f, 0.8f},
    {0.5f, 0.0f},
    {0.5f, 0.9f},
    {0.5f, 0.0f},
    {0.5f, 0.9f},
    {0.5f, 0.0f},
    {0.5f, 0.9f},
    {0.5f, 0.0f},
    {0.5f, 0.9f},
    {0.5f, 0.0f},
    {0.5f, 0.9f},
    {0.5f, 1.0f},
    {0.5f, 0.8f},
    {0.5f, 1.0f},
};

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
    BlargHeavyInterceptorVars *moby_vars = (BlargHeavyInterceptorVars *)MOBY(moby)->pvars;
    struct {
        float mat[4][4];                                  /* camera-facing frame */
        float verts[BLARG_LIGHT_RING_VERTEX_CAPACITY][3]; /* ring vertices transformed by mat */
    } f;
    u32   cols[84];
    float ring_vertex_scratch[4];
    int   i;
    int   j;

    draw_blarg_heavy_interceptor_camera_facing_glow((struct Moby *)moby);
    qcopy(f.mat[0], moby_vars->unkA0);
    fast_vec_sub(f.mat[2], moby_vars->unkD0, D_L14_001674C0_r);
    fast_vec_cross(f.mat[1], f.mat[0], f.mat[2]);
    normalize_r(f.mat[1], f.mat[1], 1.0f);
    fast_vec_cross(f.mat[2], f.mat[1], f.mat[0]);
    qcopy(f.mat[3], moby_vars->unkD0);
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
        if (moby_vars->unk202 != 0) {
            cols[i] = blarg_heavy_interceptor_light_ring_vertex_colors[i] & 0xFF0000FF;
        }
    }
    if (moby_vars->unk202 != 0) {
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
    if (moby_vars->unk202 != 0) {
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
    BlargHeavyInterceptorVars *d = (BlargHeavyInterceptorVars *)moby->pvars;
    float                      beam_placement_scratch[4][4];
    Pk                         pk;
    float                      beam_quad_transform[4][4];
    float                      quad_translation_offset[4];
    float                      beam_offset_direction[4];
    float                      beam_to_camera_vector[4];
    float                      camera_facing_angles[4];
    float                      rm[3][4];
    float                      size;
    float                      step;
    int                        r, g, b, a;
    u32                        col;
    int                        i, j;
    float                     *beam_origin;

    beam_origin = &d->unk1D0[1];
    beam_placement_scratch[3][0] = beam_origin[0];
    beam_placement_scratch[3][1] = beam_origin[1];
    beam_placement_scratch[3][2] = beam_origin[2] + 0.125f;
    beam_placement_scratch[3][3] = 1.0f;
    beam_placement_scratch[0][0] = fast_cos(d->beam_rotation_blink_and_aim_path_state[0]);
    beam_placement_scratch[0][1] = fast_sin(d->beam_rotation_blink_and_aim_path_state[0]);
    beam_placement_scratch[0][2] = 0.0f;
    d->beam_rotation_blink_and_aim_path_state[0] = fast_add_rotations(d->beam_rotation_blink_and_aim_path_state[0], -0.06981317f);
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
    BlargHeavyInterceptorVars *d = (BlargHeavyInterceptorVars *)m->pvars;
    float                      glow_quad_transform[4][4];
    float                      glow_position_offset[4];
    Pk_2ae0a8                  pk;
    float                      glow_to_camera_vector[4];
    float                      camera_facing_angles[4];
    u32                        col;
    int                        i;
    qcopy(glow_quad_transform[3], d->unkD0);
    glow_quad_transform[3][3] = 1.0f;
    fast_vec_scale(
        glow_position_offset, d->unkA0, blarg_heavy_interceptor_glow_position_offset_scale_gp);
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
    if (d->unk202 != 0) {
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
    BlargHeavyInterceptorVars *d = (BlargHeavyInterceptorVars *)m->pvars;
    float                      glow_quad_position[4];
    Pk_2af4a0                  pk;
    float                      glow_to_camera_vector[4];
    float                      camera_pull_offset[4];
    float                      glow_to_camera_distance;
    u32                        col;
    int                        j;
    qcopy(glow_quad_position, d->unk90);
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
        float camera_pull_distance = d->unk218;
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
    if (d->unk202 != 0) {
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
        fast_vec_scale(pk.m[j], D_L14_001D8640[j], d->unk214);
        fast_vec_add(pk.m[j], pk.m[j], glow_quad_position);
    }
    draw_geometry_quad(&pk, 0, 0);
}

extern int
FUN_L00_0025d808_2af688(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

void initialize_blarg_heavy_interceptor_paths(struct Moby *moby) {
    BlargHeavyInterceptorVars *moby_vars = (BlargHeavyInterceptorVars *)moby->pvars;
    int                       *path;
    int                        i;
    float                      segment_length;
    path = ((int *)l14_paths[moby_vars->unk78]);
    *(int *)&moby_vars->beam_rotation_blink_and_aim_path_state[3] = 0;
    for (i = 0; i < path[0]; i++) {
        segment_length = fast_vec_dist((char *)path + (i * 16 + 0x10),
                                       (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
        *(float *)((char *)path + (i << 4) + 0x1C) = segment_length;
        if (i != path[0] - 1) {
            moby_vars->beam_rotation_blink_and_aim_path_state[3] += segment_length;
        }
    }
    path = ((int *)l14_paths[moby_vars->spawn_path]);
    *(int *)&moby_vars->unk190[0] = 0;
    for (i = 0; i < path[0]; i++) {
        segment_length = fast_vec_dist((char *)path + (i * 16 + 0x10),
                                       (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
        *(float *)((char *)path + (i << 4) + 0x1C) = segment_length;
        if (i != path[0] - 1) {
            moby_vars->unk190[0] += segment_length;
        }
    }
    path = ((int *)l14_paths[moby_vars->patrol_path]);
    for (i = 0; i < path[0]; i++) {
        *(float *)((char *)path + (i << 4) + 0x1C) = fast_vec_dist(
            (char *)path + (i * 16 + 0x10), (char *)path + (((i + 1) % path[0]) * 16 + 0x10));
    }
    if (moby_vars->unk210 != 0.0f) {
        int *sampling_path = ((int *)l14_paths[moby_vars->unk78]);
        moby_vars->path_segment = 0;
        moby_vars->unk1C0[3] = 0.0f;
        FUN_L00_0025d808_2af688(sampling_path,
                                &moby->pos,
                                &moby_vars->path_segment,
                                &moby_vars->unk1C0[3],
                                moby_vars->rejoin_path_loop,
                                moby_vars->unk210 * moby_vars->beam_rotation_blink_and_aim_path_state[3]);
        moby_vars->unk8C = 0.0f;
        moby_vars->path_point = 0;
        FUN_L00_0025d808_2af688(sampling_path,
                                &moby->pos,
                                &moby_vars->path_point,
                                &moby_vars->unk8C,
                                moby_vars->rejoin_path_loop,
                                moby_vars->unk210 * moby_vars->unk190[0]);
    } else {
        moby_vars->path_point = 0;
        moby_vars->path_segment = 0;
        moby_vars->unk8C = 0.0f;
        moby_vars->unk1C0[3] = 0.0f;
    }
}

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");

extern int FUN_L00_0025d808_2af918(int *tab, float *out, int *a, float *b, int c, float d) __asm__("FUN_L00_0025d808");

void update_blarg_heavy_interceptor_patrol_aim_point(struct Moby *moby) {
    BlargHeavyInterceptorVars *interceptor_instance_vars = (BlargHeavyInterceptorVars *)moby->pvars;
    float                      path_target_position[4];
    float                      save = interceptor_instance_vars->unk1C0[3];
    int                        save2 = interceptor_instance_vars->path_segment;
    int r = FUN_L00_0025d808_2af918(((int *)l14_paths[interceptor_instance_vars->unk78]),
                                    path_target_position,
                                    &interceptor_instance_vars->path_segment,
                                    &interceptor_instance_vars->unk1C0[3],
                                    interceptor_instance_vars->rejoin_path_loop,
                                    interceptor_instance_vars->beam_rotation_blink_and_aim_path_state[2]);
    FUN_L00_0025b8c0_c(interceptor_instance_vars->unk170,
                       interceptor_instance_vars->unk180,
                       path_target_position[0],
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_error_gain,
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                       0.0f);
    FUN_L00_0025b8c0_c(&interceptor_instance_vars->unk170[1],
                       &interceptor_instance_vars->unk180[1],
                       path_target_position[1],
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_error_gain,
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                       0.0f);
    FUN_L00_0025b8c0_c(&interceptor_instance_vars->unk170[2],
                       &interceptor_instance_vars->unk180[2],
                       path_target_position[2],
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_error_gain,
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                       0.0f);
    if (interceptor_instance_vars->rejoin_path_loop == 0) {
        if (r != 0) {
            interceptor_instance_vars->unk1C0[3] = save;
            interceptor_instance_vars->path_segment = save2;
            interceptor_instance_vars->beam_rotation_blink_and_aim_path_state[2] = -interceptor_instance_vars->beam_rotation_blink_and_aim_path_state[2];
        }
    }
}

void update_blarg_heavy_interceptor_patrol_path_position(struct Moby *moby) {
    BlargHeavyInterceptorVars *heavy_interceptor_vars = (BlargHeavyInterceptorVars *)moby->pvars;
    char                      *seg = (char *)l14_paths[heavy_interceptor_vars->unk78];
    int                        source_segment_index = heavy_interceptor_vars->path_segment;
    float                      source_path_distance = 0.0f;
    float                      destination_path_target_position[4];
    int                        destination_segment_index;
    float                      frac;
    int                        i;
    int                       *tab;
    int                        destination_path_loop_enabled;
    float                      destination_path_distance;

    for (i = 0; i < source_segment_index; i++) {
        source_path_distance += *(float *)(seg + i * 16 + 0x1C);
    }
    source_path_distance += heavy_interceptor_vars->unk1C0[3];
    tab = ((int *)l14_paths[heavy_interceptor_vars->spawn_path]);
    destination_path_distance = source_path_distance / heavy_interceptor_vars->beam_rotation_blink_and_aim_path_state[3] *
                                heavy_interceptor_vars->unk190[0];
    destination_path_loop_enabled = heavy_interceptor_vars->rejoin_path_loop;
    destination_segment_index = 0;
    frac = 0.0f;
    FUN_L00_0025d808_2af918(tab,
                            destination_path_target_position,
                            &destination_segment_index,
                            &frac,
                            destination_path_loop_enabled,
                            destination_path_distance);
    FUN_L00_0025b8c0_c(&moby->pos.x,
                       &heavy_interceptor_vars->unk190[1],
                       destination_path_target_position[0],
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_error_gain,
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                       0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.y,
                       &heavy_interceptor_vars->unk190[2],
                       destination_path_target_position[1],
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_error_gain,
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
                       0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.z,
                       &heavy_interceptor_vars->unk190[3],
                       destination_path_target_position[2],
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_error_gain,
                       *(float *)&l14_blarg_heavy_interceptor_path_easing_damping_coefficient,
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
    BlargHeavyInterceptorVars *return_path_moby_vars = (BlargHeavyInterceptorVars *)moby->pvars;
    int                       *list = ((int *)l14_paths[return_path_moby_vars->patrol_path]);
    float                      target_position[4];
    if (return_path_moby_vars->unk208 != 0) {
        qcopy(target_position, (char *)list + list[0] * 16);
    } else {
        if (FUN_L00_0025d808_2afb40(list,
                                    target_position,
                                    &return_path_moby_vars->path_point,
                                    &return_path_moby_vars->unk8C,
                                    0,
                                    path_traversal_speed * frame_time *
                                        path_direction_multiplier) != 0) {
            return_path_moby_vars->unk208 = 1;
        }
    }
    FUN_L00_0025b8c0_c(
        &moby->pos.x,
        &return_path_moby_vars->unk190[1],
        target_position[0],
        position_easing_factor,
        *(float *)&blarg_heavy_interceptor_return_path_position_damping_coefficient_gp,
        0.0f);
    FUN_L00_0025b8c0_c(
        &moby->pos.y,
        &return_path_moby_vars->unk190[2],
        target_position[1],
        position_easing_factor,
        *(float *)&blarg_heavy_interceptor_return_path_position_damping_coefficient_gp,
        0.0f);
    FUN_L00_0025b8c0_c(
        &moby->pos.z,
        &return_path_moby_vars->unk190[3],
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
    BlargHeavyInterceptorVars *interceptor_private_vars = (BlargHeavyInterceptorVars *)moby->pvars;
    f32                        heading_offset;
    update_moby_vertical_oscillation(
        blarg_heavy_interceptor_vertical_oscillation_amplitude_gp,
        blarg_heavy_interceptor_vertical_oscillation_phase_increment_gp,
        moby,
        interceptor_private_vars->oscillation_state,
        &interceptor_private_vars->oscillation_state[1]);
    update_moby_rotation_oscillation(
        moby,
        &interceptor_private_vars->oscillation_state[2],
        &interceptor_private_vars->oscillation_state[3],
        blarg_heavy_interceptor_rotation_oscillation_amplitude_gp,
        blarg_heavy_interceptor_rotation_oscillation_first_phase_increment_gp,
        blarg_heavy_interceptor_rotation_oscillation_second_phase_increment_gp);
    if (moby->state == INTERCEPTOR_STATE_PATROL_AND_SCAN) {
        if (fast_dec_timer_s16((s16 *)&interceptor_private_vars->unk1B0[1])) {
            *(s16 *)&interceptor_private_vars->unk1B0[1] =
                scale_game_frames(blarg_heavy_interceptor_patrol_heading_change_interval_frames_gp);
            heading_offset = blarg_heavy_interceptor_patrol_heading_offset_magnitude_gp;
            if (random_integer_below(2))
                heading_offset = -heading_offset;
            interceptor_private_vars->unk1B0[0] = fast_add_rotations(moby->rot.z, heading_offset);
        }
        vendor_spring((f32 *)&moby->rot.z,
                      &interceptor_private_vars->unk1B0[2],
                      interceptor_private_vars->unk1B0[0],
                      blarg_heavy_interceptor_patrol_heading_error_gain_gp,
                      blarg_heavy_interceptor_patrol_heading_damping_coefficient_gp,
                      blarg_heavy_interceptor_patrol_heading_snap_threshold_scale_gp);
    }
}

void prepare_blarg_heavy_interceptor_light_ring_transform(u8 *moby) __asm__("FUN_L14_002aead8");

void prepare_blarg_heavy_interceptor_light_ring_transform(u8 *moby) {
    BlargHeavyInterceptorVars *interceptor_vars = (BlargHeavyInterceptorVars *)MOBY(moby)->pvars;
    u8                        *bone_transform;
    f32                        bounding_sphere[4];
    if (random_integer_below(5)) {
        qcopy(bounding_sphere, moby + 0x10);
        bounding_sphere[3] = blarg_heavy_interceptor_culling_sphere_radius_gp;
        if (fast_bsphere_check((u8 *)bounding_sphere,
                               convert_integer_to_float(MOBY(moby)->draw_dist)) >= 0) {
            bone_transform = interceptor_vars->unkA0;
            build_moby_bone_transform(moby, 1, bone_transform);
            fast_vec_normalize(bone_transform, bone_transform, 1.0f);
            fast_vec_normalize(interceptor_vars->unkB0, interceptor_vars->unkB0, 1.0f);
            fast_vec_normalize(interceptor_vars->unkC0, interceptor_vars->unkC0, 1.0f);
            interceptor_vars->flags = 2;
        }
    }
}

extern void FUN_L14_002ae260_u(void) __asm__("FUN_L14_002ae260");

int update_blarg_heavy_interceptor_aim_transforms(struct Moby *moby, int flag) {
    int                        ok = 0;
    BlargHeavyInterceptorVars *blarg_heavy_interceptor_private_vars =
        (BlargHeavyInterceptorVars *)moby->pvars;
    float bounding_sphere[4];
    float half_span_vector[4];
    float rotation_angle;
    fast_vec_sub(half_span_vector,
                 blarg_heavy_interceptor_private_vars->unk170,
                 blarg_heavy_interceptor_private_vars->unkD0);
    fast_vec_scale(half_span_vector, half_span_vector, 0.5f);
    fast_vec_add(bounding_sphere, blarg_heavy_interceptor_private_vars->unkD0, half_span_vector);
    bounding_sphere[3] = fast_vec_length(half_span_vector) + 2.0f;
    if (fast_bsphere_check((char *)bounding_sphere, convert_integer_to_float(moby->draw_dist)) >=
        0) {
        build_moby_bone_transform(moby, 1, blarg_heavy_interceptor_private_vars->unkA0);
        ok = 1;
        fast_vec_normalize(blarg_heavy_interceptor_private_vars->unkA0,
                           blarg_heavy_interceptor_private_vars->unkA0,
                           1.0f);
        fast_vec_normalize(blarg_heavy_interceptor_private_vars->unkB0,
                           blarg_heavy_interceptor_private_vars->unkB0,
                           1.0f);
        fast_vec_normalize(blarg_heavy_interceptor_private_vars->unkC0,
                           blarg_heavy_interceptor_private_vars->unkC0,
                           1.0f);
        rotation_angle = fast_subtract_rotations(
            fast_arc_tan(half_span_vector[0], half_span_vector[1]), moby->rot.z);
        build_axis_rotation_quaternion(
            blarg_heavy_interceptor_private_vars->unkF0, 2, rotation_angle);
        blarg_heavy_interceptor_private_vars->unk1B0[3] = rotation_angle;
        rotation_angle = -fast_arc_tan(vector_length_xy(half_span_vector), half_span_vector[2]);
        build_axis_rotation_quaternion(
            blarg_heavy_interceptor_private_vars->unk130, 1, rotation_angle);
        blarg_heavy_interceptor_private_vars->unk1C0[0] = rotation_angle;
        qcopy(blarg_heavy_interceptor_private_vars->unk90,
              blarg_heavy_interceptor_private_vars->unk170);
        if (flag != 0) {
            add_draw_callback(FUN_L14_002ae260_u, moby);
            blarg_heavy_interceptor_private_vars->flags |= 2;
        }
    }
    return ok;
}

s32 blarg_heavy_interceptor_light_ring_fade_values[82] __asm__("D_L14_001D8210") = {
    0,    0, 0,    0x30, 0,    0x30, 0,    0x30, 0,    0x30, 0x20, 0, 0x20, 0,
    0x20, 0, 0x20, 0x10, 0,    0x10, 0,    0x10, 0,    0x10, 0x20, 0, 0x20, 0,
    0x20, 0, 0x20, 0x30, 0,    0x30, 0,    0x30, 0,    0x30, 0x40, 0, 0x40, 0,
    0x40, 0, 0x40, 0x50, 0,    0x50, 0,    0x50, 0,    0x50, 0x40, 0, 0x40, 0,
    0x40, 0, 0x40, 0x30, 0,    0x30, 0,    0x30, 0,    0x30, 0x20, 0, 0x20, 0,
    0x10, 0, 0x10, 0,    0x10, 0x10, 0x10, 0,    0x10, 0,    0x20, 0};

void update_blarg_heavy_interceptor_light_ring_alpha(struct Moby *moby) __asm__("FUN_L14_002aed50");

void update_blarg_heavy_interceptor_light_ring_alpha(struct Moby *moby) {
    BlargHeavyInterceptorVars *moby_private_vars = (BlargHeavyInterceptorVars *)moby->pvars;
    s32                        step;
    s32                        i;
    f32                        advanced_blink_phase = moby_private_vars->beam_rotation_blink_and_aim_path_state[1] + 0.5f;
    moby_private_vars->beam_rotation_blink_and_aim_path_state[1] = advanced_blink_phase;
    step = truncate_float_to_s32(advanced_blink_phase);
    if (1.0f <= moby_private_vars->beam_rotation_blink_and_aim_path_state[1])
        *(s32 *)&moby_private_vars->beam_rotation_blink_and_aim_path_state[1] = 0;
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

/* Save ids of the group mobys already drawn this frame: reset as 20 entries, searched as 20 entries. */
extern int blarg_heavy_interceptor_drawn_save_ids[20] __asm__("D_L14_001D8680");

/* Second name for the same table: the selection loop reads it through its own address load, as retail does. */
int blarg_heavy_interceptor_drawn_save_ids_b[20] __asm__("D_L14_001D8680") = {0};

char *select_farthest_unselected_group_moby(int index, int *ids) __asm__("FUN_L14_002aef28");

extern void draw_blarg_heavy_interceptor_spinning_beam(struct Moby *) __asm__("FUN_L14_002add48");

extern void draw_blarg_heavy_interceptor_light_ring(char *) __asm__("FUN_L14_002ada18");

void draw_blarg_heavy_interceptor_group_effects(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int   i;
    int  *selected_id_slot;
    {
        int unused_id_sentinel = -1;
        for (i = 19; i >= 0; i--)
            blarg_heavy_interceptor_drawn_save_ids[i] = unused_id_sentinel;
    }
    i = 0;
    if (*(short *)(moby_vars + 0x20A) > 0) {
        int *ids = blarg_heavy_interceptor_drawn_save_ids_b;
        selected_id_slot = ids;
        do {
            char *selected_moby = select_farthest_unselected_group_moby(moby->group, ids);
            char *md;
            *selected_id_slot++ = ((short)MOBY(selected_moby)->save_id);
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
            if (((short)MOBY(moby)->save_id) == blarg_heavy_interceptor_drawn_save_ids[i]) {
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
    u8  *moby_array_base;
    u8  *listed_moby;

    if (list_entry == 0) {
        return;
    }
    moby_array_base = D_L14_0015FFD8;
    do {
        listed_moby = moby_array_base + ((*list_entry & 0x7FFF) << 8);
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
    enum {
        TURRET_STATE_WAITING = 1,
        TURRET_STATE_RETRACTING = 5
    };
    short         *list_entry = D_L14_001ABF40_b45[index];
    unsigned char *moby_array_start;
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
    moby_array_start = D_L14_0015FFD8_b45;
    do {
        moby_index_or_state = *(unsigned short *)list_entry & 0x7FFF;
        checked_moby = moby_array_start + (moby_index_or_state << 8);
        if ((((unsigned char *)MOBY(checked_moby)->pvars))[0x8B] == 0) {
            moby_index_or_state = checked_moby[0x20];
            if (moby_index_or_state != TURRET_STATE_RETRACTING &&
                moby_index_or_state != TURRET_STATE_WAITING) {
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
    char           *moby_pool_base;
    if (list)
        goto scan;
success:
    return 1;
scan:
    moby_pool_base = D_L14_ffd0_e[2];
    do {
        unsigned short id = *list;
        char          *moby = moby_pool_base + ((id & 0x7FFF) << 8);
        int            state = ((signed char)MOBY(moby)->state);
        char          *group_member_vars = ((char *)MOBY(moby)->pvars);
        if (state >= 0)
            count++;
        if (state >= 0 && *(short *)(group_member_vars + 0x88))
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

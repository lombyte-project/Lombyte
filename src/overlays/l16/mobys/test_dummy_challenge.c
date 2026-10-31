/* Level 16 moby class test_dummy_challenge (0x28e): in its volume, activates a test dummy group and the doors, and completes the objective once the dummies are gone. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/entities.h"
#include "rnc/audio/voice_alloc.h"

#include "rnc/overlay/l16/mobys/test_dummy.h"

#include "sda.h"

s32        is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");
extern int D_L16_001600EC;
extern int scale_game_frames(int);

#include "rnc/overlay/l16/mobys/door.h"
/* Drive a triggered scene, player pose and objective completion. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D72C0), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad[0x72];
    unsigned char complete;
} L16ObjectiveEntry_q;

typedef struct {
    int trigger, helper, path, hidden, pose, camera, objective, pad1, pad2, timer_q;
} L16SceneData_q;

extern char D_0013E633_q[], D_0014171B[], D_0013D388[], D_L16_00167240[] __asm__("D_0013E633");
extern char D_0014162A_lui[] __asm__("D_0014162A") NOT_SDA;
extern char D_L16_001671C0_lui[] __asm__("D_L16_001671C0") NOT_SDA;
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void FUN_L00_002598b0_q(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void FUN_L16_002d6260_q(char *, void *, float *) __asm__("FUN_L16_002d6260");
void        mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

extern char D_0013F3D0[] NOT_SDA;
void        test_dummy_challenge_update(struct Moby *moby) __asm__("FUN_L16_002d5ef8");

/* Moby class 654 */
void test_dummy_challenge_update(struct Moby *moby) {
    enum {
        SCENE_STATE_INITIALIZE = 0,
        SCENE_STATE_WAIT_FOR_TRIGGER = 1,
        SCENE_STATE_WAIT_TO_HIDE = 2,
        SCENE_STATE_WAIT_FOR_LINKED_MOBYS_AND_TIMER = 3,
        SCENE_STATE_COMPLETE = 4,
        SCENE_STATE_HIDDEN_WAIT_FOR_TRIGGER = 5,
        SCENE_STATE_HIDDEN_WAIT_FOR_LINKED_MOBYS = 6
    };
    float           position[4], rotation[4];
    L16SceneData_q *scene_data = (L16SceneData_q *)moby->pvars;
    void           *helper;
    switch (moby->state) {
    case SCENE_STATE_INITIALIZE:
        if (scene_data == 0 || scene_data->trigger == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        if (scene_data->hidden == -1) {
            moby->state = 5;
            moby->flags |= 1;
            moby->visible = 0;
            moby->collision = 0;
        } else {
            moby->state = 1;
            set_class_0287_group_activation_and_find_nearest(scene_data->helper, 1);
        }
        break;
    case SCENE_STATE_WAIT_FOR_TRIGGER: {
        char *player = D_0013F3D0;
        if (!is_point_inside_clip_volume(player, scene_data->trigger))
            break;
        scene_data->timer_q = scale_game_frames(60);
        moby->state = 2;
        if (scene_data->camera >= 0)
            *(unsigned short *)(player + 0x225A) = *(unsigned short *)&scene_data->camera;
        jet_mount(D_L16_001671C0_lui, D_L16_001671C0_lui + 0x10, 1, 0, 0);
        helper = activate_class_0287_group_and_find_nearest(scene_data->helper, 0);
        goto notify;
    }
    case SCENE_STATE_WAIT_TO_HIDE:
        if (scene_data->camera >= 0)
            *(unsigned short *)(D_0014162A_lui) = *(unsigned short *)&scene_data->camera;
        if (fast_dec_timer(&scene_data->timer_q)) {
            moby->state = 3;
            scene_data->timer_q = scale_game_frames(60);
            moby->visible = 0;
            moby->flags |= 1;
            activate_test_dummy_group(scene_data->path);
        }
        break;
    case SCENE_STATE_WAIT_FOR_LINKED_MOBYS_AND_TIMER:
        FUN_L16_002d6260_q(moby, position, rotation);
        set_camera_position(position);
        set_camera_rotation(rotation);
        if (scene_data->camera >= 0)
            *(unsigned short *)(D_0014162A_lui) = *(unsigned short *)&scene_data->camera;
        if (count_active_moby_group_members(scene_data->path, -1))
            break;
        if (!fast_dec_timer(&scene_data->timer_q))
            break;
        moby->visible = 1;
        moby->state = 4;
        moby->flags &= 0xFFFE;
        jet_end_mount(3);
        set_race_camera(D_L16_001600EC + (scene_data->pose << 7) + 0x30,
                        D_L16_001600EC + (scene_data->pose << 7) + 0x70,
                        0,
                        1);
        FUN_L00_002598b0_q((int)moby,
                           5.0f,
                           D_L16_001600EC + (scene_data->pose << 7) + 0x30,
                           0x10000,
                           50.0f,
                           1.0f,
                           0,
                           1,
                           0);
        ((L16ObjectiveEntry_q *)(D_0013D388 + scene_data->objective))->complete = 1;
        helper = set_class_0287_group_activation_and_find_nearest(scene_data->helper, 0);
    notify:
        if (helper)
            allocate_voice_for_target_entry(1, 0, helper);
        break;
    case SCENE_STATE_COMPLETE:
        break;
    case SCENE_STATE_HIDDEN_WAIT_FOR_TRIGGER:
        if (is_point_inside_clip_volume(D_0013F3D0, scene_data->trigger)) {
            activate_class_0287_group_and_find_nearest(scene_data->helper, 0);
            activate_test_dummy_group(scene_data->path);
            moby->state = 6;
        }
        break;
    case SCENE_STATE_HIDDEN_WAIT_FOR_LINKED_MOBYS: {
        char *player = D_0013F3D0;
        if (is_point_inside_clip_volume(player, scene_data->trigger) && scene_data->camera >= 0)
            *(unsigned short *)(player + 0x225A) = *(unsigned short *)&scene_data->camera;
        if (!count_active_moby_group_members(scene_data->path, -1)) {
            moby->state = 4;
            helper = set_class_0287_group_activation_and_find_nearest(scene_data->helper, 0);
            if (helper)
                allocate_voice_for_target_entry(1, 0, helper);
            ((L16ObjectiveEntry_q *)(D_0013D388 + scene_data->objective))->complete = 1;
        }
        break;
    }
    }
}

extern float D_0015ED6C;

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float D_0015ED6C, frame_time;
extern float frame_time_sq;

extern char *D_L16_001B0930[];

extern float D_0015ED6C;
extern float frame_time_sq;
/* Computes a moby's direction vectors from its table entry. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D7628), where it is exact; names translated to the US level program. */

extern int D_L16_001600EC;

#include "rnc/overlay/l16/mobys/test_dummy_challenge_globals.h"

/* no foreign declaration */

float triggered_scene_rotation_y_gp __asm__("D_L16_00161A8C") = 0.35f;
void  compute_triggered_scene_path_pose(struct Moby *moby,
                                        void        *out1,
                                        float       *out2) __asm__("FUN_L16_002d6260");

void compute_triggered_scene_path_pose(struct Moby *moby,
                                       void        *position_output,

                                       float *rotation_output) {
    char *triggered_scene_path_data = (char *)moby->pvars;
    float hero_offset[4];
    char *scene_reference_pose = D_L16_001600EC + (*(int *)triggered_scene_path_data << 7);
    char *position_path = D_L16_001B0930[*(int *)(triggered_scene_path_data + 0xC)];
    fast_vec_sub(hero_offset, &hero.motion.pos, scene_reference_pose + 0x30);
    hero_offset[3] = 0;
    transform_vector_by_basis(hero_offset, hero_offset, scene_reference_pose + 0x40);
    advance_accelerated_scalar((hero_offset[1] + 1.0f) * 0.5f,
                               frame_time_sq,
                               frame_time_sq,
                               D_0015ED6C + D_0015ED6C,
                               (float *)(triggered_scene_path_data + 0x1C),
                               (float *)(triggered_scene_path_data + 0x20));
    lerp_vector(position_output,
                position_path + 0x10,
                position_path + 0x20,
                *(float *)(triggered_scene_path_data + 0x1C));
    clear_u64_value(rotation_output);
    rotation_output[1] = triggered_scene_rotation_y_gp;
    rotation_output[2] = moby->rot.z;
}

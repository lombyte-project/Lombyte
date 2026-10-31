/* Level 06 moby class 0x451: fred. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/l06/mobys/alien_queen.h"

extern float D_L06_00167540[];
extern int   is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern void  blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void  set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void  jet_end_mount(int) __asm__("FUN_L00_002eac18");

extern char  *D_L06_0015FFD8_301070 __asm__("D_L06_0015FFD8");
extern int    is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
typedef float Vec_301070[4] __attribute__((aligned(16)));
typedef int   u128q_301070 __attribute__((mode(TI)));

typedef union {
    u128q_301070 q;
    float        f[4];
} VecQ_301070 __attribute__((aligned(16)));

struct Data_301070;

typedef struct Data_301070 {
    char          pad00[4];
    short         f4;
    char          pad06[2];
    unsigned char f8;
    char          pad09[3];
    float         fC;
    char          pad10[0x10];
    char         *f20;
    char          pad24[0x1C];
    char          f40[0x64];
    float         fA4;
    float         fA8;
    char          padAC[4];
    float         fB0;
    char          padB4[0xC];
    char          fC0[0x68];
    float         f128;
    char          pad12C[0x14];
    float         f140[4];
    char          pad150[8];
    int           f158, f15C;
    int           f160, f164, f168, f16C, f170;
    short         f174, f176;
    float         f178, f17C;
} Data_301070;

typedef struct Obj_301070 {
    char  pad00[0x86];
    short f86;
} Obj_301070;

typedef struct Busy {
    int  f0;
    char pad4[0x20];
    int  f24;
} Busy;

extern void  attach_shared_manipulator_to_matching_class_moby(char *) __asm__("FUN_L03_002db480");
extern void  FUN_L00_0025a120_301070(void *) __asm__("FUN_L00_0025a120");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern int   initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
extern int   update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");
extern void  begin_dialogue_camera_transition(float, void *) __asm__("FUN_L01_002783a8");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void  grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern int   memcard_save_data(int, int) __asm__("FUN_0020b178");
extern int   help_request(int a, int b) __asm__("FUN_L00_00203908");
extern float advance_accelerated_scalar_301070(
    float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern int   D_L06_0015F5C4_macro __asm__("D_L06_0015F5C4") MACRO_ADDR;
extern float D_L06_0015F3FC_macro __asm__("D_L06_0015F3FC") MACRO_ADDR;
extern s32   D_L06_00179B90[] __asm__("D_L06_00179B90");
extern unsigned char big_head_cheat_enabled[] __asm__("D_0015EDB0") MACRO_ADDR;
extern int           D_0013D8AC_lui __asm__("D_0013D8AC") NOT_SDA;

char D_L06_00201880[] = "Scrawny Scientist";

extern Obj_301070 *D_L06_00167580 NOT_SDA;
extern unsigned char              D_0013D505[];
extern f32                        frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern f32                        frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern f32                        frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;

/* Update of moby class 1105 (fred): steps the conversation and camera states, then turns the head toward the hero. */
void       fred_update(struct Moby *m) __asm__("FUN_L06_00301070");
extern int D_L06_00179B90[] __asm__("D_L06_00179B90");
extern int is_point_inside_clip_volume(void *, int);

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");

/* Moby class 1105 */
void fred_update(struct Moby *moby) {
    enum {
        INITIALIZE_FRED_DIALOGUE = 0,
        UPDATE_FRED_DIALOGUE = 1,
        HANDLE_FRED_DIALOGUE_RESULT = 2,
        INTERPOLATE_FRED_CUTSCENE_CAMERA = 3,
        RAMP_FRED_POST_CUTSCENE_SCALAR_TO_ZERO = 4,
        WAIT_FOR_FRED_REMOVAL_TRIGGER = 5
    };
    Data_301070 *fred_vars = *(struct Data_301070 **)&moby->pvars;
    float        rate, head_rate;
    int          tracking;
    attach_shared_manipulator_to_matching_class_moby((char *)moby);
    if (moby->visible != 0 &&
        fast_vec_dist(((float *)&moby->pos), ((char *)D_L06_00167540)) < 24.0f) {
        FUN_L00_0025a120_301070(moby);
        moby->shadow_range = 0x13;
    }
    switch (moby->state) {
    case INITIALIZE_FRED_DIALOGUE:
        if (D_0013D505[0] != 0)
            goto del;
        if (D_0013D8AC_lui == 0) {
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
            fred_vars->f176 = 1;
            set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_301070 + (fred_vars->f164 << 8));
        } else {
            {
                Vec_301070 t;
                fast_vec_normalize(t, ((char *)&moby->rot_row0), 2.0f);
                fast_vec_add(((float *)&moby->pos), ((float *)&moby->pos), t);
                moby->pos.z = probe_ground_height(((float *)&moby->pos), 0, 0.5f);
                fred_vars->f176 = 0;
            }
        }
        fred_vars->f174 = scale_game_frames(0x2D);
        set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_301070 + (fred_vars->f160 << 8));
        fred_vars->f20 = D_L06_00201880;
        moby->state = 1;
        initialize_npc_dialogue(moby, fred_vars);
        break;
    case UPDATE_FRED_DIALOGUE:
        if (fred_vars->f8 == 1 &&
            is_point_inside_clip_volume(&hero.motion.pos, fred_vars->f168) != 0) {
            fred_vars->fC = 16.0f;
        } else {
            fred_vars->fC = 3.7f;
        }
        if (check_all_listed_frog_states_fe_fd_or_10(fred_vars->f170) != 0) {
            if (fast_dec_timer_s16(&fred_vars->f174) != 0 &&
                update_npc_dialogue(moby, fred_vars) != 0) {
                moby->state = 2;
                if (fred_vars->f176 != 0) {
                    Vec_301070 t;
                    fast_vec_normalize(t, ((char *)&moby->rot_row0), 2.0f);
                    fast_vec_add(((float *)&moby->pos), ((float *)&moby->pos), t);
                    moby->pos.z = probe_ground_height(((float *)&moby->pos), 0, 0.5f);
                    fred_vars->f176 = 0;
                }
                begin_dialogue_camera_transition(2.2f, moby);
            }
        }
        if (hero.state.control_mode == 15) {
            set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_301070 + (fred_vars->f160 << 8));
        }
        break;
    case HANDLE_FRED_DIALOGUE_RESULT:
        if (D_L06_0015F5C4_macro != 2) {
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
            if (fred_vars->f4 < 2) {
                jet_clear_persistent(moby->mission);
                set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_301070 +
                                                     (fred_vars->f164 << 8));
                moby->state = 1;
            }
            if (fred_vars->f4 == 2) {
                grant_inventory_item(0x1D, 1);
                memcard_save_data(0, -1);
                help_request(0x1775, 0x2C);
                D_L06_0015F3FC_macro = 1.0f;
                fred_vars->f178 = 0.0f;
                {
                    VecQ_301070 pos;
                    VecQ_301070 rot;
                    pos.q = 0;
                    rot.q = 0;
                    pos.f[0] = 178.47f;
                    pos.f[1] = 319.76f;
                    pos.f[2] = 146.31f;
                    rot.f[0] = 0.07f;
                    rot.f[1] = 0.18f;
                    rot.f[2] = -1.77f;
                    jet_mount(pos.f, rot.f, 1, 0, 0);
                }
                set_race_camera(&hero.motion.pos, &hero.motion.rot, 0x72, 1);
                set_moby_and_links_to_states_8_and_9(D_L06_0015FFD8_301070 +
                                                     (fred_vars->f164 << 8));
                moby->state = 3;
                moby->visible = 0;
                moby->flags |= 1;
                *(int *)&moby->collision = 0;
            }
        }
        break;
    case INTERPOLATE_FRED_CUTSCENE_CAMERA: {
        {
            VecQ_301070 camera_start_position;
            VecQ_301070 camera_end_position;
            VecQ_301070 camera_start_rotation;
            VecQ_301070 camera_end_rotation;
            Vec_301070  pos;
            Vec_301070  rot;
            camera_start_position.q = 0;
            camera_end_position.q = 0;
            camera_start_rotation.q = 0;
            camera_end_rotation.q = 0;
            camera_start_position.f[0] = 178.47f;
            camera_start_position.f[1] = 319.76f;
            camera_start_position.f[2] = 146.31f;
            camera_end_position.f[0] = 172.55f;
            camera_end_position.f[1] = 316.01f;
            camera_end_position.f[2] = 145.27f;
            camera_start_rotation.f[0] = 0.07f;
            camera_start_rotation.f[1] = 0.18f;
            camera_start_rotation.f[2] = -1.77f;
            camera_end_rotation.f[0] = 0.07f;
            camera_end_rotation.f[1] = 0.13f;
            camera_end_rotation.f[2] = -1.21f;
            advance_accelerated_scalar_301070(&fred_vars->f178,
                                              &fred_vars->f17C,
                                              1.0f,
                                              frame_time_sq_macro * 0.1f,
                                              frame_time_sq_macro * 0.1f,
                                              frame_time_macro * 0.1f);
            if (fred_vars->f178 > 0.1f) {
                set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_301070 +
                                                     (fred_vars->f160 << 8));
            }
            lerp_vector(pos, camera_start_position.f, camera_end_position.f, fred_vars->f178);
            rot[2] = fast_add_rotations(
                fast_subtract_rotations(camera_end_rotation.f[2], camera_start_rotation.f[2]) *
                    fred_vars->f178,
                camera_start_rotation.f[2]);
            rot[1] = fast_add_rotations(
                fast_subtract_rotations(camera_end_rotation.f[1], camera_start_rotation.f[1]) *
                    fred_vars->f178,
                camera_start_rotation.f[1]);
            rot[0] = fast_add_rotations(
                fast_subtract_rotations(camera_end_rotation.f[0], camera_start_rotation.f[0]) *
                    fred_vars->f178,
                camera_start_rotation.f[0]);
            if (D_L06_00167580 != 0 && D_L06_00167580->f86 != 5) {
                jet_mount(camera_start_position.f, camera_start_rotation.f, 1, 0, 0);
            }
            set_camera_position(pos);
            set_camera_rotation(rot);
            if (fred_vars->f178 >= 1.0f ||
                (((Busy *)D_L06_00179B90)->f0 == 0 && ((Busy *)D_L06_00179B90)->f24 == -1 &&
                 fred_vars->f178 > 0.2f)) {
                approach_value(&D_L06_0015F3FC_macro, 1.0f, frame_time_macro * 4.0f);
            } else {
                approach_value(&D_L06_0015F3FC_macro, 0.0f, frame_time_macro * 4.0f);
            }
            if (D_L06_0015F3FC_macro == 1.0f) {
                set_race_camera(&hero.motion.pos, &hero.motion.rot, 0, 0);
                jet_end_mount(0);
                moby->state = 4;
            }
        }
        break;
    }
    case RAMP_FRED_POST_CUTSCENE_SCALAR_TO_ZERO:
        approach_value(&D_L06_0015F3FC_macro, 0.0f, frame_time_macro * 4.0f);
        if (D_L06_0015F3FC_macro == 0.0f) {
            moby->state = 5;
        }
        break;
    case WAIT_FOR_FRED_REMOVAL_TRIGGER:
        if (hero.state.control_mode == 15) {
            set_moby_and_links_to_states_4_and_5(D_L06_0015FFD8_301070 + (fred_vars->f164 << 8));
        del:
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    {
        Vec_301070 target;
        Vec_301070 eye;
        Vec_301070 delta;
        if (moby->prev_seq == 0) {
            char *player = &hero.motion.pos;
            tracking = 1;
            if (vector_distance_xy(((float *)&moby->pos), player) < 8.0f &&
                fast_difference_between_rotations(
                    moby->rot.z,
                    fast_arc_tan(hero.motion.body_center.f[0] - moby->pos.x,
                                 hero.motion.body_center.f[1] - moby->pos.y)) < 1.5707964f) {
                if (fast_vec_length(player + 0x80) > 0.01f)
                    fred_vars->f158 = scale_game_frames(120);
                else
                    fast_dec_timer(&fred_vars->f158);
            } else if (fred_vars->f158) {
                fred_vars->f158 = 0;
                qcopy(fred_vars->f140, &hero.motion.body_center);
            }
            if (fast_dec_timer(&fred_vars->f15C)) {
                float heading;
                fred_vars->f15C =
                    truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
                heading = fast_add_rotations(moby->rot.z,
                                             random_float_between(-90.0f, 90.0f) * 0.017453292f);
                build_spherical_offset(fred_vars->f140,
                                       6.0f,
                                       heading,
                                       random_float_between(0.0f, 30.0f) * 0.017453292f);
                fast_vec_add(fred_vars->f140, fred_vars->f140, ((float *)&moby->pos));
            }
            if (fred_vars->f158) {
                qcopy(target, &hero.motion.body_center);
                rate = 0.04f;
                head_rate = 0.3f;
            } else {
                qcopy(target, fred_vars->f140);
            }
        }
        if (tracking) {
            float yaw;
            float pitch;
            qcopy(eye, ((float *)&moby->pos));
            eye[2] += 1.0f;
            fast_vec_sub(delta, target, eye);
            yaw = fast_subtract_rotations(fast_arc_tan(delta[0], delta[1]), moby->rot.z);
            pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
            if (yaw > 1.5707964f)
                yaw = 1.5707964f;
            else if (yaw < -1.5707964f)
                yaw = -1.5707964f;
            if (pitch > 0.5235988f)
                pitch = 0.5235988f;
            else if (pitch < -0.5235988f)
                pitch = -0.5235988f;
            fred_vars->fA4 = pitch;
            fred_vars->fA8 = yaw * 0.6f;
            fred_vars->f128 = yaw * 0.4f;
        }
    }
    if (big_head_cheat_enabled[0])
        fred_vars->fB0 = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq_macro, head_rate * frame_scale_sq_macro, moby, fred_vars->f40, 0);
    update_joint_animation(
        rate * frame_scale_sq_macro, head_rate * frame_scale_sq_macro, moby, fred_vars->fC0, 2);
}

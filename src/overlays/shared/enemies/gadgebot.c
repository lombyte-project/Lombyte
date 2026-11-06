/* Shared moby class gadgebot: comes out of its container, picks targets and fights. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_class_id.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "sda.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/detach_manipulator.h"
#include "rnc/overlay/shared/mobys/build_symmetric_adjacency_masks.h"
#include "rnc/overlay/shared/enemies/trooper_set_knockback.h"
#include "rnc/overlay/shared/mobys/update_object_wander_movement.h"
#include "rnc/overlay/shared/mobys/entities.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/overlay/shared/enemies/gadgebot_data/gadgebot_update.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/shared/enemies/gadgebot_spawn_container.h"
#include "rnc/overlay/shared/enemies/gadgebot_spawn_pad.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

enum {
    GADGEBOT_ABSORPTION_DISABLED = 2
};

enum {
    GADGEBOT_INITIALIZE = 0,
    GADGEBOT_WAIT_FOR_SPAWN_PAD = 1,
    GADGEBOT_WANDER_NEAR_HOME = 2,
    GADGEBOT_FOLLOW_HERO = 3,
    GADGEBOT_STAND_AND_FACE_HERO = 4,
    GADGEBOT_SEEK_AND_ATTACK_TARGET = 5,
    GADGEBOT_SEEK_INTERACTION_TARGET = 7,
    GADGEBOT_ABSORB_INTO_OWNER = 8,
    GADGEBOT_UPDATE_KNOCKBACK = 9
};

#define GB_FLAG                (*(unsigned char *)0x15EDB3)
#define PVARS(p)               (*(char **)((p) + 0x78))
#define S16(p, current_target) (*(short *)((p) + (current_target)))
#define I32(p, current_target) (*(int *)((p) + (current_target)))
#define F32(p, current_target) (*(float *)((p) + (current_target)))
#define CANDIDATES(vars)       (((GbSlots *)(vars))->candidates)
#define CANDIDATE_SCORES(vars) (((GbSlots *)(vars))->candidate_scores)
#define CANDIDATE_COUNT(vars)  (((GbSlots *)(vars))->candidate_count)

typedef struct Moby Moby;

typedef union {
    OvlQuad q;
    float   f[4];
} GbVec;

typedef struct {
    char  p0[0x10];
    int   collision;
    char  p14[0x10];
    float scale;
} GbClass;

typedef struct GbMoby {
    char           p0[0x10];
    float          pos[4] __attribute__((aligned(16)));
    unsigned char  state;
    char           p21[3];
    GbClass       *cls;
    char           p28[4];
    float          scale;
    char           p30;
    unsigned char  visible;
    char           p32[2];
    unsigned short flags;
    char           p36[0x12];
    float          rot;
    char           p4C[7];
    unsigned char  anim;
    char           p54[0x1C];
    unsigned char  anim_flags;
    char           p71[7];
    void          *vars;
    char           p7C[3];
    unsigned char  shadow_range;
    char           p80[0x14];
    int            collision;
    char           p98[0xE];
    short          cls_id;
    char           pA8[8];
    unsigned char  mission;
    char           pB1[0xB];
    unsigned char  sub;
    char           pBD[3];
    float          rot_row0[4];
    char           pD0[0x30];
} GbMoby;

typedef struct {
    int   collision_radius_1024ths;
    float step_height;
    float max_drop;
    float max_climb;
    float speed;
    float turn_step;
    float vertical_velocity;
    float acceleration;
    float brake;
    float max_speed;
    float heading_tolerance;
    float arrive_distance;
    float ground_probe_depth;
    float unk34;
    int   movement_option_flags;
    float turn_spring;
    float turn_damping;
    float max_turn_step;
    int   move_timer;
    float move_heading;
} GbCtl;

typedef struct {
    int id[16];
    int node[16];
} GbPaths;

typedef struct {
    float         follow_anchor[4] __attribute__((aligned(16)));
    float         unk10;
    float         unk14;
    float         unk18;
    char          p1C[0x14];
    GbCtl         ctl;
    char          knockback_motion[0x60];
    float         home_position[4] __attribute__((aligned(16)));
    float         stall_check_position[4] __attribute__((aligned(16)));
    unsigned char scale_manipulator;
    unsigned char scale_manipulator_attached;
    char          p102[0x1E];
    float         scale_manipulator_scale_x;
    float         scale_manipulator_scale_y;
    float         scale_manipulator_scale_z;
    char          p12C[0x44];
    GbMoby       *spawn_container_moby;
    GbMoby       *target; /* nearest mine/box moby found by FUN_L06_002f2910 (0: none) */
    char          p178[4];
    float         approach_heading_offset;
    char          p180[4];
    int           follow_frame_count;
    float         stall_hero_distance;
    int           attack_target_search_frame;
    short         unk190;
    char          p192[2];
    int           attack_substate_timer;
    short         has_target;
    short         target_searched; /* set once FUN_L06_002f2910 has run the search */
    GbMoby       *collision_contact_moby;
    char          p1A0[0x40];
    GbPaths       paths;
    char          p260[0x1C];
    int           selected_path_slot;
    char          p280[4];
    float         collision_correction_speed;
    float         ground_spring_velocity;
    float         glow_pulse_angle;
    char          p290[0x10];
    int           follow_gate_moby_indices[8];
    int           follow_gate_volumes[8];
    GbVec         position_history_latest;
    GbVec         position_history_previous;
    float         ground_height_change_limit;
    GbMoby       *absorption_owner_moby;
    float         manipulator_scale;
    int           target_group; /* moby group searched for targets (-1: whole level list) */
    int           configured_path_count;
} GbVars;

typedef struct {
    char p0[0x14];
    int  absorbs_remaining;
} GbOwnerVars;

typedef struct {
    char    p0[0x18];
    GbMoby *moby;
    int     unk1C;
    char    p20[0x10];
    float   position[4];
    float   normal_xy[2];
    float   normal_z;
} GbHit;

typedef struct {
    char p0[0x2C];
    int  in_range_count; /* gadgebots near the hero that take a command this frame */
} GbStats;

typedef int u128_c __attribute__((mode(TI)));

typedef struct {
    char  p0[0x18];
    Moby *moby;
    int   unk1C;
    char  p20[0x10];
    float position[4];
    float normal_xy[2];
    float normal_z;
} GbHit_2f2250;

typedef struct {
    char  p0[0x174];
    char *target;
    int   claimed_target;
    float spread_angle;
    char  p180[0xC];
    int   search_frame;
    char  p190[2];
    short candidate_count;
    char  p194[4];
    short has_target;
    short target_searched;
    char  p19C[4];
    int   candidates[8];
    float candidate_scores[8];
} GbSlots;

typedef int u128_F4058 __attribute__((mode(TI)));

typedef struct {
    int list;
    int out;
} Iter32f0;

typedef unsigned int Q __attribute__((mode(TI), aligned(16)));

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void  add_draw_callback(int, int) __asm__("FUN_001f4600");
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern void  FUN_L00_0025a120(void *);
extern void  blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
void         trooper_start_knockback(u8 *a, void *b, int c, int d, int e, float f) __asm__("FUN_L00_0025c558");
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void
resolve_point_path_segment_xy_clearance(float, int, void *, void *) __asm__("FUN_L00_00261d78");
extern float fast_cos(float);
extern float fast_sin(float);
extern char *spawn_source_attached_fading_effect_particle(float, char *, int, int, int) __asm__("FUN_L00_0026daa0");
extern float step_angle_toward_target(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float fast_add_rotations(float, float);
extern float vector_length_xy(void *);
extern int   coll_sphere(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
extern int   move_moby_with_ground_and_collision_checks(
      void *, void *, void *, void *, float) __asm__("FUN_L00_00258b50");
extern int is_moby_in_class_range_500_to_540(void *) __asm__("FUN_L00_0025e3b8");
extern int test_path_segment_strict_crossing(int, void *, void *) __asm__("FUN_L00_00261968");
extern int trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");
extern int
compute_clear_path_point_mask(float, int *, int, int, void *) __asm__("FUN_L01_00276a48");
extern int  follow_path(int *, int, int, void *, void *, void *, float) __asm__("FUN_L01_00276fe8");
extern int  FUN_L06_002f32f0_u(void *, int) __asm__("FUN_L06_002f32f0");
extern int  scale_game_frames(int);
extern void add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void FUN_L00_00258278(char *moby, float target, float *vel, float k, float d, float max);
extern void make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_00272060(void *, int, float, float, int, int, int, float, void *);
extern void assign_gadgebot_attack_targets(void) __asm__("FUN_L06_002f2250");
void        select_nearest_gadgebot_interaction_target(void *moby_arg) __asm__("FUN_L06_002f2910");
extern void FUN_L06_002f3640_u(void *) __asm__("FUN_L06_002f3640");
void        update_gadgebot_glow_effects(char *moby) __asm__("FUN_L06_002f38c8");
extern void attach_manipulator(char *, int, void *) __asm__("FUN_0020cb10");
extern unsigned char *get_moby_target_info(int);
extern int            get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int            get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void FUN_0022da68(int, int, void *);
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");
extern int  FUN_001fa6e0_c(int, int, float) __asm__("FUN_001fa6e0");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern char *D_001413D0       NOT_SDA;
extern float                  D_0015ED6C;
extern int                    level_frame_count __asm__("D_L06_0015F5CC");
extern char                   D_L06_00167540[];
extern s32                    level_frame_count __asm__("D_L06_0015F5CC");
extern float D_0015ED70       MACRO_ADDR;
extern int                   *D_L06_001B0C30[];
extern f32                    D_0015ED6C;
extern GbHit                  D_L06_001745C0;
extern Moby                  *D_L06_0015FFD8;
extern GbMoby *D_L06_001745D8 NOT_SDA;
extern GbStats                l06_gadgebot_command_range_stats __asm__("D_L06_0017EC58");

extern int           D_00141610[];
extern int           D_L06_001AC180[];
extern float         D_0015ED70;
extern unsigned char big_head_cheat_enabled_macro[] __asm__("D_0015EDB0") MACRO_ADDR;

GbStats D_L10_0017E958 = {0};

/* Update function of the gadgebot (moby class 857): waits by its dispenser, then follows, attacks and returns to the hero along the level's paths. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F1470), where it is exact; names translated to the US level program. */
void gadgebot_update(GbMoby *moby) __asm__("FUN_L06_002f0040");

/* Moby classes 857, 1906 */
void gadgebot_update(GbMoby *moby) {
    enum {
        GADGEBOT_ATTACK_SEEK_AND_APPROACH_TARGET = 1,
        GADGEBOT_ATTACK_STRIKE = 2,
        GADGEBOT_ATTACK_RECOVER_AFTER_STRIKE = 3,
        GADGEBOT_ATTACK_WAIT_FOR_TARGET = 4
    };
    enum {
        GADGEBOT_COMMAND_FOLLOW_HERO = 1,
        GADGEBOT_COMMAND_STAND_AND_FACE_HERO = 2,
        GADGEBOT_COMMAND_SEEK_AND_ATTACK_TARGET = 3,
        GADGEBOT_COMMAND_SEEK_INTERACTION_TARGET = 4
    };
    enum {
        GADGEBOT_FOLLOW_INITIALIZE = 0,
        GADGEBOT_FOLLOW_MOVE_TOWARD_HERO = 1,
        GADGEBOT_FOLLOW_WAIT_AND_FACE_HERO = 2
    };
    enum {
        GADGEBOT_INTERACTION_SEARCH_TARGET = 0,
        GADGEBOT_INTERACTION_APPROACH_TARGET = 1
    };
    float   movement_displacement_output[4];
    float   raised_position_snapshot[4];
    float   save0[4];
    float   save1[4];
    float   vector_and_hit_scratch[4];
    float   position_and_displacement_scratch[4];
    float   temporary_position[4];
    float   path_and_attack_vector[4];
    float   attack_probe_near_position[4];
    float   owner_radial_angle;
    float   step;
    GbVars *gadgebot_vars = moby->vars;
    GbCtl  *movement_control;
    int     flags;
    float   hero_distance;
    float   dz;
    int     interaction_result;

    flags = 0;
    movement_control = &gadgebot_vars->ctl;
    hero_distance = vector_distance_xy(moby->pos, hero.motion.pos.f);
    dz = absolute_float(moby->pos[2] - hero.ground_point.f[2]);
    *(OvlQuad *)save1 = *(OvlQuad *)save0 = *(OvlQuad *)moby->pos;
    if (moby->state != GADGEBOT_WAIT_FOR_SPAWN_PAD) {
        FUN_L06_002f3640_u(moby);
        if (hero_distance < 16.0f && absolute_float(moby->pos[2] - hero.motion.pos.f[2]) < 1.5f) {
            int ok = 0;
            if (moby->state == GADGEBOT_WANDER_NEAR_HOME ||
                moby->state == GADGEBOT_SEEK_AND_ATTACK_TARGET ||
                moby->state == GADGEBOT_FOLLOW_HERO || moby->state == 6 ||
                moby->state == GADGEBOT_SEEK_INTERACTION_TARGET ||
                moby->state == GADGEBOT_STAND_AND_FACE_HERO) {
                ok = 1;
            }
            if (ok) {
                l06_gadgebot_command_range_stats.in_range_count++;
                switch (D_00141610[0]) {
                case GADGEBOT_COMMAND_FOLLOW_HERO:
                    if (moby->state != GADGEBOT_FOLLOW_HERO) {
                        moby->state = GADGEBOT_FOLLOW_HERO;
                        moby->sub = 0;
                    }
                    break;
                case GADGEBOT_COMMAND_SEEK_AND_ATTACK_TARGET:
                    if (moby->state != GADGEBOT_SEEK_AND_ATTACK_TARGET) {
                        moby->state = GADGEBOT_SEEK_AND_ATTACK_TARGET;
                        moby->sub = 0;
                    }
                    break;
                case GADGEBOT_COMMAND_STAND_AND_FACE_HERO:
                    moby->state = GADGEBOT_STAND_AND_FACE_HERO;
                    break;
                case GADGEBOT_COMMAND_SEEK_INTERACTION_TARGET:
                    moby->sub = 0;
                    moby->state = GADGEBOT_SEEK_INTERACTION_TARGET;
                    break;
                }
            }
        }
    }
    if (GB_FLAG != 0) {
        if (gadgebot_vars->scale_manipulator_attached == 0) {
            attach_manipulator((char *)moby, 0, &gadgebot_vars->scale_manipulator);
        }
        if (moby->state != GADGEBOT_WAIT_FOR_SPAWN_PAD) {
            approach_value(&gadgebot_vars->manipulator_scale, 2.3f, frame_scale * 0.1f);
        } else {
            approach_value(&gadgebot_vars->manipulator_scale, 1.0f, frame_scale * 0.1f);
        }
        gadgebot_vars->scale_manipulator_scale_x = gadgebot_vars->manipulator_scale;
        gadgebot_vars->scale_manipulator_scale_y = gadgebot_vars->manipulator_scale;
        gadgebot_vars->scale_manipulator_scale_z = gadgebot_vars->manipulator_scale;
    } else if (gadgebot_vars->scale_manipulator_attached != 0) {
        detach_manipulator(moby, &gadgebot_vars->scale_manipulator);
    }
    switch (moby->state) {
    case GADGEBOT_INITIALIZE: {
        int i;

        gadgebot_vars->manipulator_scale = 1.0f;
        moby->pos[2] = probe_ground_height(moby->pos, 0, 0.5f);
        spawn_gadgebot_pad((char *)moby);
        gadgebot_vars->position_history_previous.q = gadgebot_vars->position_history_latest.q =
            *(OvlQuad *)moby->pos;
        if (visited52[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        gadgebot_vars->spawn_container_moby =
            (GbMoby *)spawn_gadgebot_spawn_container((char *)moby);
        moby->pos[2] = probe_ground_height(moby->pos, 0, 0.5f);
        qcopy(gadgebot_vars->follow_anchor, moby->pos);
        qcopy(gadgebot_vars->home_position, moby->pos);
        gadgebot_vars->unk14 = frame_time * 2.0f;
        gadgebot_vars->unk18 = frame_time * 2.6179938f;
        gadgebot_vars->ctl.collision_radius_1024ths = 0x12C;
        gadgebot_vars->ctl.step_height = 0.51f;
        gadgebot_vars->ctl.max_drop = gadgebot_vars->ground_height_change_limit;
        gadgebot_vars->ctl.max_climb = gadgebot_vars->ground_height_change_limit;
        gadgebot_vars->ctl.max_speed = frame_time * 3.0f + frame_time * 0.1f;
        gadgebot_vars->ctl.acceleration = frame_scale_sq * 0.02f;
        gadgebot_vars->ctl.brake = frame_scale_sq * 0.3f;
        gadgebot_vars->ctl.heading_tolerance = 0.6981317f;
        gadgebot_vars->ctl.arrive_distance = 0.2f;
        gadgebot_vars->ctl.turn_spring = frame_scale_sq * 0.03f;
        gadgebot_vars->ctl.turn_damping = frame_scale_sq * 0.3f;
        gadgebot_vars->ctl.max_turn_step = frame_time * 3.1415927f;
        gadgebot_vars->ctl.move_timer = scale_game_frames(4);
        gadgebot_vars->ctl.movement_option_flags |= 1;
        for (i = 0; i < 16; i++) {
            if (gadgebot_vars->paths.id[i] >= 0) {
                gadgebot_vars->configured_path_count++;
            }
        }
        gadgebot_vars->glow_pulse_angle = random_angle_radians();
        gadgebot_vars->selected_path_slot = -1;
        moby->state = GADGEBOT_WAIT_FOR_SPAWN_PAD;
        moby->collision = 0;
        break;
    }
    case GADGEBOT_WAIT_FOR_SPAWN_PAD:
        if (gadgebot_vars->spawn_container_moby == 0 ||
            gadgebot_vars->spawn_container_moby->state == 0xFE ||
            gadgebot_vars->spawn_container_moby->state == 0xFD ||
            gadgebot_vars->spawn_container_moby->state >= 2) {
            if (gadgebot_vars->configured_path_count > 0) {
                int i;

                for (i = 0; i < 16; i++) {
                    if (gadgebot_vars->paths.id[i] >= 0) {
                        build_symmetric_adjacency_masks(
                            &gadgebot_vars->paths.id[i], 1, gadgebot_vars->paths.node[i]);
                        if (compute_clear_path_point_mask(0.0f,
                                                          &gadgebot_vars->paths.id[i],
                                                          1,
                                                          gadgebot_vars->paths.node[i],
                                                          moby->pos) != 0) {
                            gadgebot_vars->selected_path_slot = i;
                        }
                    }
                }
            }
            if (GB_FLAG != 0) {
                gadgebot_position_snapshot_z_offset_gp = 0.35f;
                gadgebot_vars->ctl.collision_radius_1024ths = 0x177;
            } else {
                gadgebot_vars->ctl.collision_radius_1024ths = 0x12C;
                gadgebot_position_snapshot_z_offset_gp = 0.27f;
            }
            moby->flags = (moby->flags & 0xFFBE) | 0x1000;
            moby->collision = moby->cls->collision;
            moby->state = GADGEBOT_FOLLOW_HERO;
        } else {
            if (hero.vehicle_mode == HERO_VEHICLE_MODE_ON_FOOT) {
                moby->flags |= 0x41;
                gadgebot_vars->spawn_container_moby->flags |= 0x41;
                moby->flags &= 0xEFFF;
                gadgebot_vars->spawn_container_moby->flags &= 0xEFFF;
                gadgebot_vars->spawn_container_moby->collision = 0;
            } else {
                moby->flags &= 0xFFBE;
                gadgebot_vars->spawn_container_moby->flags &= 0xFFBE;
                moby->flags |= 0x1000;
                gadgebot_vars->spawn_container_moby->flags |= 0x1000;
                gadgebot_vars->spawn_container_moby->collision =
                    gadgebot_vars->spawn_container_moby->cls->collision;
            }
            if (moby->visible != 0) {
                update_gadgebot_glow_effects((char *)moby);
            }
            return;
        }
        break;
    case GADGEBOT_WANDER_NEAR_HOME:
        if (moby->anim != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(7));
        }
        update_object_wander_movement((char *)moby, (char *)gadgebot_vars, 0.35f, 0.25f);
        break;
    case GADGEBOT_FOLLOW_HERO: {
        int j;

        qcopy(vector_and_hit_scratch, moby->pos);
        if (current_level_index != LEVEL_NEBULA_G34) {
            gadgebot_vars->selected_path_slot = -1;
            if (gadgebot_vars->configured_path_count >= 2) {
                int i;

                for (i = 0; i < 16; i++) {
                    if (gadgebot_vars->paths.id[i] >= 0) {
                        int *p = D_L06_001B0C30[gadgebot_vars->paths.id[i]];

                        if (point_in_path_xy(hero.motion.pos.f, p + 4, *p) != 0) {
                            int *q = D_L06_001B0C30[gadgebot_vars->paths.id[i]];

                            if (point_in_path_xy(moby->pos, q + 4, *q) != 0) {
                                gadgebot_vars->selected_path_slot = i;
                            }
                        }
                    }
                }
            }
        }
        for (j = 0; j < 8; j++) {
            GbMoby *referenced_moby;
            int    *ta;
            int    *tb;

            {
                int *ca = &gadgebot_vars->follow_gate_moby_indices[j];
                int *cb = &gadgebot_vars->follow_gate_volumes[j];

                if (*ca == -1 || *cb == -1) {
                    break;
                }
            }
            ta = &gadgebot_vars->follow_gate_moby_indices[j];
            tb = &gadgebot_vars->follow_gate_volumes[j];
            referenced_moby = &(*(GbMoby **)&D_L06_0015FFD8)[*ta];
            if (referenced_moby->cls_id == 0x3F7 && referenced_moby->state != 4 &&
                is_point_inside_clip_volume(((GbMoby *)hero.moby)->pos, *tb) != 0 &&
                is_point_inside_clip_volume(moby->pos, *tb) == 0) {
                moby->state = GADGEBOT_STAND_AND_FACE_HERO;
                *(OvlQuad *)moby->pos = *(OvlQuad *)vector_and_hit_scratch;
                break;
            }
        }
        if (moby->state != GADGEBOT_STAND_AND_FACE_HERO) {
            switch (moby->sub) {
            case GADGEBOT_FOLLOW_INITIALIZE:
                moby->sub = 1;
                gadgebot_vars->follow_frame_count = 0;
                gadgebot_vars->unk190 = 0;
                qcopy(gadgebot_vars->stall_check_position, moby->pos);
                gadgebot_vars->approach_heading_offset = randf_sym(0.13962634f, 0.34906584f);
            case GADGEBOT_FOLLOW_MOVE_TOWARD_HERO: {
                int hit;

                if (moby->anim != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(7));
                }
                *(OvlQuad *)temporary_position = *(OvlQuad *)moby->pos;
                if (gadgebot_vars->selected_path_slot >= 0) {
                    int *p;

                    if (follow_path(&gadgebot_vars->paths.id[gadgebot_vars->selected_path_slot],
                                    1,
                                    gadgebot_vars->paths.node[gadgebot_vars->selected_path_slot],
                                    moby->pos,
                                    hero.motion.pos.f,
                                    position_and_displacement_scratch,
                                    0.1f) == 0) {
                        qcopy(position_and_displacement_scratch, hero.motion.pos.f);
                    }
                    *(OvlQuad *)path_and_attack_vector = *(OvlQuad *)moby->pos;
                    resolve_point_path_segment_xy_clearance(
                        0.17f,
                        gadgebot_vars->paths.id[gadgebot_vars->selected_path_slot],
                        moby->pos,
                        path_and_attack_vector);
                    p = D_L06_001B0C30[gadgebot_vars->paths.id[gadgebot_vars->selected_path_slot]];
                    if (point_in_path_xy(path_and_attack_vector, p + 4, *p) != 0) {
                        *(OvlQuad *)moby->pos = *(OvlQuad *)path_and_attack_vector;
                    }
                } else {
                    qcopy(position_and_displacement_scratch, hero.motion.pos.f);
                }
                FUN_L00_00258278(
                    (char *)moby,
                    fast_add_rotations(
                        fast_arc_tan(position_and_displacement_scratch[0] - moby->pos[0],
                                     position_and_displacement_scratch[1] - moby->pos[1]),
                        gadgebot_vars->approach_heading_offset),
                    &gadgebot_vars->ctl.turn_step,
                    frame_scale_sq * 0.07f,
                    frame_scale_sq * 0.3f,
                    frame_time * 5.2359877f);
                if (hero_distance > 2.3f) {
                    approach_value(
                        &gadgebot_vars->ctl.max_speed, frame_time * 3.0f * 1.35f, frame_time_sq);
                } else if (hero_distance < 1.2f) {
                    approach_value(&gadgebot_vars->ctl.max_speed,
                                   frame_time * 3.0f * 0.77f,
                                   frame_time_sq * 2.0f);
                } else {
                    approach_value(
                        &gadgebot_vars->ctl.max_speed, frame_time * 3.0f * 0.95f, frame_time_sq);
                }
                if (frame_time * 3.0f * 1.35f < gadgebot_vars->ctl.max_speed) {
                    gadgebot_vars->ctl.max_speed = frame_time * 3.0f * 1.35f;
                }
                if (hero_distance > 0.8f) {
                    flags = move_moby_with_ground_and_collision_checks(moby,
                                                                       &gadgebot_vars->ctl,
                                                                       hero.motion.pos.f,
                                                                       movement_displacement_output,
                                                                       1.0f);
                }
                {
                    int ph = ((int)moby >> 8) % 8;

                    if (level_frame_count % 8 == ph) {
                        play_entity_sound(
                            (int)moby, 0.4f, moby->pos, 0x1000000, 1.0f, 1.0f, 0, 1, 0);
                    }
                }
                hero_distance = vector_distance_xy(moby->pos, hero.motion.pos.f);
                dz = absolute_float(moby->pos[2] - hero.ground_point.f[2]);
                probe_ground_height(moby->pos, 0, 0.5f);
                if (D_L06_001745D8 != 0 && D_L06_001745D8->cls_id == 0x3B1) {
                    *(OvlQuad *)moby->pos = *(OvlQuad *)temporary_position;
                }
                hit = 0;
                if (flags & 1) {
                    if (gadgebot_vars->follow_frame_count % 14 == 0) {
                        if (vector_distance_xy(gadgebot_vars->stall_check_position, moby->pos) <
                            gadgebot_vars->ctl.speed) {
                            hit = 1;
                            gadgebot_vars->stall_hero_distance =
                                vector_distance_xy(hero.motion.pos.f, moby->pos);
                        }
                        qcopy(gadgebot_vars->stall_check_position, moby->pos);
                    }
                }
                if (flags & 0x20) {
                    moby->state = GADGEBOT_STAND_AND_FACE_HERO;
                    *(OvlQuad *)moby->pos = *(OvlQuad *)temporary_position;
                }
                if (hero_distance < 0.8f || hit != 0 || dz > 1.75f) {
                    qcopy(gadgebot_vars->follow_anchor, moby->pos);
                    moby->sub = 2;
                }
                break;
            }
            case GADGEBOT_FOLLOW_WAIT_AND_FACE_HERO: {
                float hd = vector_distance_xy(gadgebot_vars->follow_anchor, hero.motion.pos.f);

                if ((gadgebot_vars->unk190 == 0 && hd > 1.8f) ||
                    (gadgebot_vars->unk190 != 0 &&
                     hd > gadgebot_vars->stall_hero_distance + 0.5f)) {
                    if (dz < 1.75f) {
                        moby->sub = 0;
                    }
                }
                if (moby->anim != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(7));
                }
                FUN_L00_00258278((char *)moby,
                                 fast_arc_tan(hero.motion.pos.f[0] - moby->pos[0],
                                              hero.motion.pos.f[1] - moby->pos[1]),
                                 &gadgebot_vars->ctl.turn_step,
                                 frame_scale_sq * 0.05f,
                                 frame_scale_sq * 0.3f,
                                 frame_time * 3.4906585f);
                break;
            }
            }
            gadgebot_vars->follow_frame_count++;
        }
        break;
    }
    case GADGEBOT_STAND_AND_FACE_HERO:
        if (moby->anim != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(7));
        }
        FUN_L00_00258278(
            (char *)moby,
            fast_arc_tan(hero.motion.pos.f[0] - moby->pos[0], hero.motion.pos.f[1] - moby->pos[1]),
            &gadgebot_vars->ctl.turn_step,
            frame_scale_sq * 0.05f,
            frame_scale_sq * 0.3f,
            frame_time * 3.4906585f);
        break;
    case GADGEBOT_SEEK_AND_ATTACK_TARGET:
        if (current_level_index != LEVEL_NEBULA_G34) {
            int i;

            gadgebot_vars->selected_path_slot = -1;
            for (i = 0; i < 16; i++) {
                if (gadgebot_vars->paths.id[i] >= 0) {
                    int *p = D_L06_001B0C30[gadgebot_vars->paths.id[i]];

                    if (point_in_path_xy(hero.motion.pos.f, p + 4, *p) != 0) {
                        int *q = D_L06_001B0C30[gadgebot_vars->paths.id[i]];

                        if (point_in_path_xy(moby->pos, q + 4, *q) != 0) {
                            gadgebot_vars->selected_path_slot = i;
                        }
                    }
                }
            }
        }
        if (moby->sub == 0) {
            moby->sub = 1;
            gadgebot_vars->target = 0;
            gadgebot_vars->has_target = 0;
            gadgebot_vars->target_searched = 0;
            gadgebot_vars->approach_heading_offset = randf_sym(0.13962634f, 0.34906584f);
            gadgebot_vars->attack_target_search_frame = level_frame_count;
        }
        switch (moby->sub) {
        case GADGEBOT_ATTACK_SEEK_AND_APPROACH_TARGET:
            if (gadgebot_vars->attack_target_search_frame < level_frame_count) {
                assign_gadgebot_attack_targets();
            }
            if (gadgebot_vars->target != 0) {
                float approach_heading;

                if (moby->anim != 5) {
                    blend_moby_animation(moby, 5, 0, scale_game_frames(7));
                }
                if (gadgebot_vars->selected_path_slot >= 0) {
                    int *p;

                    if (follow_path(&gadgebot_vars->paths.id[gadgebot_vars->selected_path_slot],
                                    1,
                                    gadgebot_vars->paths.node[gadgebot_vars->selected_path_slot],
                                    moby->pos,
                                    gadgebot_vars->target->pos,
                                    vector_and_hit_scratch,
                                    0.1f) == 0) {
                        qcopy(vector_and_hit_scratch, gadgebot_vars->target->pos);
                    }
                    *(OvlQuad *)temporary_position = *(OvlQuad *)moby->pos;
                    resolve_point_path_segment_xy_clearance(
                        0.17f,
                        gadgebot_vars->paths.id[gadgebot_vars->selected_path_slot],
                        moby->pos,
                        temporary_position);
                    p = D_L06_001B0C30[gadgebot_vars->paths.id[gadgebot_vars->selected_path_slot]];
                    if (point_in_path_xy(temporary_position, p + 4, *p) != 0) {
                        *(OvlQuad *)moby->pos = *(OvlQuad *)temporary_position;
                    }
                } else {
                    qcopy(vector_and_hit_scratch, gadgebot_vars->target->pos);
                }
                approach_heading =
                    fast_add_rotations(fast_arc_tan(vector_and_hit_scratch[0] - moby->pos[0],
                                                    vector_and_hit_scratch[1] - moby->pos[1]),
                                       gadgebot_vars->approach_heading_offset);
                position_and_displacement_scratch[0] = fast_cos(approach_heading) * 3.0f;
                position_and_displacement_scratch[1] = fast_sin(approach_heading) * 3.0f;
                position_and_displacement_scratch[2] = 0.0f;
                fast_vec_add(position_and_displacement_scratch,
                             position_and_displacement_scratch,
                             moby->pos);
                if (gadgebot_vars->selected_path_slot != -1 &&
                    test_path_segment_strict_crossing(
                        gadgebot_vars->paths.id[gadgebot_vars->selected_path_slot],
                        moby->pos,
                        position_and_displacement_scratch) != 0) {
                    step_angle_toward_target(
                        &gadgebot_vars->approach_heading_offset, 0, 0.0f, frame_time * 1.5707964f);
                }
                FUN_L00_00258278((char *)moby,
                                 approach_heading,
                                 &gadgebot_vars->ctl.turn_step,
                                 frame_scale_sq * 0.07f,
                                 frame_scale_sq * 0.3f,
                                 frame_time * 5.2359877f);
                gadgebot_vars->ctl.max_speed = frame_time * 4.5f;
                if (move_moby_with_ground_and_collision_checks(moby,
                                                               &gadgebot_vars->ctl,
                                                               gadgebot_vars->target->pos,
                                                               movement_displacement_output,
                                                               1.0f) &
                    0x20) {
                    moby->state = GADGEBOT_STAND_AND_FACE_HERO;
                    *(OvlQuad *)moby->pos = *(OvlQuad *)save0;
                }
                if (vector_distance_xy(moby->pos, gadgebot_vars->target->pos) < 1.5f) {
                    qcopy(path_and_attack_vector, moby->pos);
                    path_and_attack_vector[2] += 0.4f;
                    path_and_attack_vector[0] += fast_cos(moby->rot) * 0.4f;
                    path_and_attack_vector[1] += fast_sin(moby->rot) * 0.4f;
                    qcopy(attack_probe_near_position, path_and_attack_vector);
                    path_and_attack_vector[0] += fast_cos(moby->rot) * 0.3f;
                    path_and_attack_vector[1] += fast_sin(moby->rot) * 0.3f;
                    if (collision_line(
                            path_and_attack_vector, attack_probe_near_position, 0, (int)moby, 0) !=
                            0 &&
                        D_L06_001745D8 == gadgebot_vars->target) {
                        moby->sub = 2;
                    }
                }
            } else if (gadgebot_vars->target_searched != 0) {
                gadgebot_vars->target_searched = 0;
                gadgebot_vars->attack_substate_timer = scale_game_frames(0x3C);
                moby->sub = 4;
            }
            break;
        case GADGEBOT_ATTACK_STRIKE:
            if (moby->anim != 2) {
                blend_moby_animation(moby, 2, 8, scale_game_frames(7));
            }
            if (gadgebot_vars->target != 0) {
                float t = compute_interpolated_record_value(moby);

                if (t >= 12.0f && t <= 15.0f) {
                    path_and_attack_vector[0] = fast_cos(moby->rot);
                    path_and_attack_vector[1] = fast_sin(moby->rot);
                    path_and_attack_vector[2] = 0.0f;
                    make_hit_descriptor(
                        vector_and_hit_scratch, moby, 0x10000, path_and_attack_vector, 2.0f);
                    record_moby_hit(gadgebot_vars->target, vector_and_hit_scratch);
                }
            }
            if (moby->anim_flags & 2) {
                gadgebot_vars->target_searched = 0;
                gadgebot_vars->attack_substate_timer = scale_game_frames(0x1E);
                moby->sub = 3;
            }
            break;
        case GADGEBOT_ATTACK_RECOVER_AFTER_STRIKE:
            if (moby->anim != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(7));
            }
            if (gadgebot_vars->target_searched != 0 && gadgebot_vars->target == 0) {
                gadgebot_vars->target_searched = 0;
                gadgebot_vars->attack_substate_timer = scale_game_frames(0x3C);
                moby->sub = 4;
            }
            if (fast_dec_timer(&gadgebot_vars->attack_substate_timer) != 0) {
                if (gadgebot_vars->target != 0) {
                    moby->sub = 1;
                }
            }
            break;
        case GADGEBOT_ATTACK_WAIT_FOR_TARGET:
            if (gadgebot_vars->attack_target_search_frame < level_frame_count) {
                assign_gadgebot_attack_targets();
            }
            if (moby->anim != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(7));
            }
            if (fast_dec_timer(&gadgebot_vars->attack_substate_timer) != 0 &&
                gadgebot_vars->target_searched != 0 && gadgebot_vars->target == 0) {
                moby->state = GADGEBOT_FOLLOW_HERO;
                moby->sub = 0;
            } else if (gadgebot_vars->target_searched != 0 && gadgebot_vars->target == 0) {
                gadgebot_vars->target_searched = 0;
            }
            if (gadgebot_vars->target != 0) {
                moby->sub = 1;
            }
            break;
        }
        break;
    case GADGEBOT_SEEK_INTERACTION_TARGET:
        switch (moby->sub) {
        case GADGEBOT_INTERACTION_SEARCH_TARGET:
            moby->sub = 1;
            select_nearest_gadgebot_interaction_target(moby);
            break;
        case GADGEBOT_INTERACTION_APPROACH_TARGET:
            if (gadgebot_vars->target != 0) {
                if (moby->anim != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(7));
                }
                if (gadgebot_vars->target->cls_id != 0x515 &&
                    gadgebot_vars->target->cls_id != 0x15F) {
                    fast_vec_add(vector_and_hit_scratch,
                                 gadgebot_vars->target->pos,
                                 gadgebot_vars->target->rot_row0);
                } else {
                    *(OvlQuad *)vector_and_hit_scratch = *(OvlQuad *)gadgebot_vars->target->pos;
                }
                FUN_L00_00258278((char *)moby,
                                 fast_arc_tan(vector_and_hit_scratch[0] - moby->pos[0],
                                              vector_and_hit_scratch[1] - moby->pos[1]),
                                 &gadgebot_vars->ctl.turn_step,
                                 frame_scale_sq * 0.07f,
                                 frame_scale_sq * 0.3f,
                                 frame_time * 5.2359877f);
                if (move_moby_with_ground_and_collision_checks(moby,
                                                               &gadgebot_vars->ctl,
                                                               vector_and_hit_scratch,
                                                               movement_displacement_output,
                                                               1.0f) &
                    0x20) {
                    moby->state = GADGEBOT_STAND_AND_FACE_HERO;
                    *(OvlQuad *)moby->pos = *(OvlQuad *)save0;
                }
            } else if (gadgebot_vars->target_searched != 0) {
                moby->sub = 0;
                moby->state = GADGEBOT_FOLLOW_HERO;
            }
            break;
        case 2:
            break;
        }
        break;
    case GADGEBOT_ABSORB_INTO_OWNER: {
        float rr;

        owner_radial_angle =
            fast_arc_tan(moby->pos[0] - gadgebot_vars->absorption_owner_moby->pos[0],
                         moby->pos[1] - gadgebot_vars->absorption_owner_moby->pos[1]);
        rr = vector_distance_xy(gadgebot_vars->absorption_owner_moby->pos, moby->pos);
        step_angle_toward_target(&owner_radial_angle,
                                 0,
                                 gadgebot_vars->absorption_owner_moby->rot,
                                 frame_time * 6.2831855f);
        rr -= frame_time * 4.0f;
        if (rr <= 0.0f) {
            GbOwnerVars *ov = gadgebot_vars->absorption_owner_moby->vars;

            if (ov->absorbs_remaining > 0) {
                ov->absorbs_remaining--;
            }
            if (ov->absorbs_remaining == 0) {
                trooper_play_sound(1, 0, (int)gadgebot_vars->absorption_owner_moby, 0x516);
            } else {
                trooper_play_sound(0, 0, (int)gadgebot_vars->absorption_owner_moby, 0x516);
            }
            mark_moby_for_removal(moby);
            return;
        }
        vector_and_hit_scratch[0] = fast_cos(owner_radial_angle) * rr;
        vector_and_hit_scratch[1] = fast_sin(owner_radial_angle) * rr;
        vector_and_hit_scratch[2] = 0.0f;
        moby->pos[0] = vector_and_hit_scratch[0] + gadgebot_vars->absorption_owner_moby->pos[0];
        moby->pos[1] = vector_and_hit_scratch[1] + gadgebot_vars->absorption_owner_moby->pos[1];
        if (rr < 1.0f) {
            moby->scale = moby->cls->scale * rr;
            spawn_source_attached_fading_effect_particle(
                (1.0f - rr) * 0.5f * 210000.0f, (char *)moby, 0x3000407F, scale_game_frames(2), 3);
        }
        if (moby->visible != 0) {
            update_gadgebot_glow_effects((char *)moby);
        }
        moby->collision = 0;
        moby->flags &= 0xEFFF;
        return;
    }
    case GADGEBOT_UPDATE_KNOCKBACK:
        if (step_moby_motion(moby, gadgebot_vars->knockback_motion) & 0x41) {
            int i;

            cart_burst(moby, moby->pos, -1, 0.5f, 13.0f);
            qcopy(moby->pos, gadgebot_vars->home_position);
            moby->state = GADGEBOT_STAND_AND_FACE_HERO;
            for (i = 0; i < 10; i++) {
                float effect_scale_factor = random_float_between(0.1f, 1.0f);
                int   cr = random_int_between(0x40, 0xFF);
                int   cg = random_int_between(0x40, 0xFF);
                int   cb = random_int_between(0x40, 0xFF);
                int   t;
                int   ga;
                float fa;
                float fb;

                position_and_displacement_scratch[0] =
                    fast_cos(random_angle_radians()) * random_float_between(0.0f, 0.25f);
                position_and_displacement_scratch[1] =
                    fast_sin(random_angle_radians()) * random_float_between(0.0f, 0.25f);
                position_and_displacement_scratch[2] = 0.0f;
                position_and_displacement_scratch[2] += random_float_between(0.0f, 0.5f);
                add_vectors(temporary_position, position_and_displacement_scratch, moby->pos);
                fast_vec_normalize(vector_and_hit_scratch,
                                   vector_and_hit_scratch,
                                   random_float_between(0.0f, frame_time));
                vector_and_hit_scratch[2] =
                    random_float_between(frame_time * 2.5f, frame_time * 5.0f);
                t = scale_game_frames(random_int_between(0xF, 0x1E));
                ga = (cg << 8) | 0x7F000000;
                fa = effect_scale_factor * 0.07f;
                fb = effect_scale_factor * 0.7f;
                FUN_L00_00272060(position_and_displacement_scratch,
                                 t,
                                 fa,
                                 fb,
                                 (cb << 16) | ga | cr,
                                 0,
                                 random_integer_below(2) ? 1 : -1,
                                 frame_time_sq * 15.0f,
                                 vector_and_hit_scratch);
            }
            return;
        }
        break;
    }
    interaction_result = FUN_L06_002f32f0_u(moby, moby->state != GADGEBOT_SEEK_INTERACTION_TARGET);
    if (interaction_result == 2) {
        return;
    }
    if (moby->state != GADGEBOT_WAIT_FOR_SPAWN_PAD) {
        float saved_control_f10;
        float saved_control_f24;

        qcopy(raised_position_snapshot, moby->pos);
        raised_position_snapshot[2] += gadgebot_position_snapshot_z_offset_gp + 0.02f;
        qcopy(vector_and_hit_scratch, moby->pos);
        saved_control_f10 = gadgebot_vars->ctl.speed;
        saved_control_f24 = gadgebot_vars->ctl.max_speed;
        gadgebot_vars->ctl.speed = 0.0f;
        gadgebot_vars->ctl.max_speed = 0.0f;
        move_moby_with_ground_and_collision_checks(
            moby, &gadgebot_vars->ctl, moby->pos, movement_displacement_output, 1.0f);
        gadgebot_vars->ctl.speed = saved_control_f10;
        gadgebot_vars->ctl.max_speed = saved_control_f24;
        moby->pos[2] = vector_and_hit_scratch[2];
        if (fast_vec_dist(vector_and_hit_scratch, moby->pos) > 0.005f) {
            float max;
            float collision_displacement_length;

            fast_vec_sub(position_and_displacement_scratch, moby->pos, vector_and_hit_scratch);
            if (fast_vec_length(position_and_displacement_scratch) < 0.25f) {
                fast_vec_normalize(
                    position_and_displacement_scratch, position_and_displacement_scratch, 0.25f);
            }
            qcopy(moby->pos, vector_and_hit_scratch);
            max = frame_time * 3.0f;
            step = -fast_vec_length(position_and_displacement_scratch);
            vendor_spring(&step,
                          &gadgebot_vars->collision_correction_speed,
                          0.0f,
                          frame_scale_sq * 0.025f,
                          frame_scale_sq * 0.3f,
                          max);
            collision_displacement_length = fast_vec_length(position_and_displacement_scratch);
            if (collision_displacement_length < gadgebot_vars->collision_correction_speed) {
                gadgebot_vars->collision_correction_speed = collision_displacement_length;
            }
            if (gadgebot_vars->collision_correction_speed < collision_displacement_length) {
                fast_vec_normalize(position_and_displacement_scratch,
                                   position_and_displacement_scratch,
                                   gadgebot_vars->collision_correction_speed);
            }
            fast_vec_add(moby->pos, moby->pos, position_and_displacement_scratch);
        } else {
            gadgebot_vars->collision_correction_speed = 0.0f;
            qcopy(moby->pos, vector_and_hit_scratch);
        }
        if (moby->state != GADGEBOT_UPDATE_KNOCKBACK) {
            float ground_height;

            if (interaction_result == 0) {
                float  z = moby->pos[2];
                float *hp = D_L06_001745C0.position;
                int    i;

                i = 0;
                do {
                    qcopy(position_and_displacement_scratch, moby->pos);
                    position_and_displacement_scratch[2] += gadgebot_vars->ctl.step_height;
                    if (coll_sphere(
                            position_and_displacement_scratch,
                            convert_integer_to_float(gadgebot_vars->ctl.collision_radius_1024ths) *
                                0.0009765625f,
                            6,
                            moby) == 0) {
                        break;
                    }
                    if (fast_arc_tan(D_L06_001745C0.normal_z,
                                     vector_length_xy(D_L06_001745C0.normal_xy)) > 0.5235988f) {
                        qcopy(moby->pos, hp);
                        moby->pos[2] -= gadgebot_vars->ctl.step_height;
                    }
                    i++;
                } while (i < 6);
                moby->pos[2] = z;
                if (absolute_float(save1[2] - probe_ground_height(moby->pos, 0, 0.5f)) >=
                    gadgebot_vars->ctl.max_climb) {
                    *(OvlQuad *)moby->pos = *(OvlQuad *)save1;
                    gadgebot_vars->collision_correction_speed = 0.0f;
                }
            }
            movement_control->vertical_velocity -= frame_time_sq * 7.0f;
            if (movement_control->vertical_velocity < frame_time * -8.0f) {
                movement_control->vertical_velocity = frame_time * -8.0f;
            }
            moby->pos[2] += 0.25f;
            ground_height = probe_ground_height(moby->pos, 0, 0.5f);
            moby->pos[2] -= 0.25f;
            if (moby->pos[2] < ground_height) {
                vendor_spring(&moby->pos[2],
                              &gadgebot_vars->ground_spring_velocity,
                              ground_height,
                              frame_scale_sq * 0.04f,
                              frame_scale_sq * 0.3f,
                              frame_time * 1.5f);
                movement_control->vertical_velocity = 0.0f;
            } else if (ground_height < moby->pos[2]) {
                moby->pos[2] += movement_control->vertical_velocity;
                if (moby->pos[2] < ground_height) {
                    moby->pos[2] = ground_height;
                }
            }
        }
        if (moby->pos[2] < gadgebot_vars->home_position[2] - 5.0f || moby->pos[0] < 2.0f ||
            moby->pos[0] > 1021.0f || moby->pos[1] < 2.0f || moby->pos[1] > 1021.0f ||
            moby->pos[2] < 2.0f || moby->pos[2] > 1021.0f) {
            qcopy(moby->pos, gadgebot_vars->home_position);
            moby->state = GADGEBOT_STAND_AND_FACE_HERO;
            return;
        }
    }
    if (D_L06_001745C0.moby != 0 && D_L06_001745C0.unk1C > 0) {
        gadgebot_vars->collision_contact_moby = D_L06_001745C0.moby;
    } else {
        gadgebot_vars->collision_contact_moby = 0;
    }
    *(OvlQuad *)vector_and_hit_scratch = *(OvlQuad *)moby->pos;
    vector_and_hit_scratch[2] -= 2.0f;
    if (vector_and_hit_scratch[2] < 0.5f) {
        vector_and_hit_scratch[2] = 0.5f;
    }
    *(OvlQuad *)position_and_displacement_scratch = *(OvlQuad *)moby->pos;
    position_and_displacement_scratch[2] += 1.0f;
    if (collision_line(
            position_and_displacement_scratch, vector_and_hit_scratch, 0, (int)moby, 0) != 0 &&
        D_L06_001745C0.moby != 0 &&
        (D_L06_001745C0.moby->cls_id == 0x409 || D_L06_001745C0.moby->cls_id == 0x359 ||
         is_moby_in_class_range_500_to_540(D_L06_001745C0.moby) != 0)) {
        *(OvlQuad *)moby->pos = *(OvlQuad *)save0;
    }
    if (moby->visible != 0) {
        if (fast_vec_dist(moby->pos, D_L06_00167540) < 17.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0xD;
        }
        if (moby->visible != 0) {
            update_gadgebot_glow_effects((char *)moby);
        }
    }
    gadgebot_vars->position_history_previous.q = gadgebot_vars->position_history_latest.q;
    gadgebot_vars->position_history_latest.q = *(OvlQuad *)moby->pos;
}

/* scores a target moby by distance, heading differences and flags */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F3578), where it is exact; names translated to the US level program. */
float score_gadgebot_target_candidate(char *moby,
                                      char *b,
                                      int   target_kind) __asm__("FUN_L06_002f2148");

float score_gadgebot_target_candidate(char *moby, char *candidate_moby, int target_kind) {
    float candidate_score = vector_distance_xy(&MOBY(moby)->pos, candidate_moby + 0x10);
    float t = fast_difference_between_rotations(
        l06_camera.yaw,
        fast_arc_tan(*(float *)(candidate_moby + 0x10) - MOBY(moby)->pos.x,
                     *(float *)(candidate_moby + 0x14) - MOBY(moby)->pos.y));
    float u = fast_difference_between_rotations(
        MOBY(moby)->rot.z,
        fast_arc_tan(*(float *)(candidate_moby + 0x10) - MOBY(moby)->pos.x,
                     *(float *)(candidate_moby + 0x14) - MOBY(moby)->pos.y));
    candidate_score = candidate_score + u * 10.0f;
    candidate_score = candidate_score + t * 3.0f;
    if (((unsigned char *)candidate_moby)[0x31] == 0) {
        candidate_score += 10.0f;
    }
    if (target_kind == 9) {
        candidate_score += 100.0f;
    }
    return candidate_score;
}

/* Shared level-06/10 routine: gathers candidate mobies, scores them against the target list and writes the slot tables. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F3680), where it is exact; names translated to the US level program. */

/* Shares targets among the idle class-0x359 mobys: each one ranks up to 8 candidates
   by score, each target goes to its best-scoring mobys, and their approach angles
   are spread apart. */
void assign_gadgebot_attack_targets(void) {
    enum {
        GADGEBOT_TARGET_KIND_UNAVAILABLE = 0,
        GADGEBOT_TARGET_KIND_ATTACKABLE = 5
    };
    char       *mobys[50];
    char        end[16];
    char        start[16];
    int         targets[50];
    signed char weights[50];
    char       *selected[16];
    int         moby_count;
    int         i;
    int         target_count;
    int         pass;
    int         index;

    moby_count = 0;
    {
        char *gadgebot_candidate;
        for (i = 0; (gadgebot_candidate = (char *)D_L06_001AC180[i]) != 0; i++) {
            if (*(short *)(gadgebot_candidate + 0xA6) == 0x359 &&
                ((unsigned char *)gadgebot_candidate)[0x20] == 5 &&
                ((unsigned char *)gadgebot_candidate)[0xBC] != 2 && moby_count != 0x32) {
                char *current_target = ((GbSlots *)PVARS(gadgebot_candidate))->target;
                if (current_target == 0 || ((unsigned char *)current_target)[0x20] == 0xFE ||
                    ((unsigned char *)current_target)[0x20] == 0xFD) {
                    mobys[moby_count++] = gadgebot_candidate;
                }
            }
        }
    }

    for (i = 0; i < moby_count; i++) {
        char *moby = mobys[i];
        char *vars = PVARS(moby);
        char *candidate;
        int   j;
        CANDIDATE_COUNT(vars) = 0;
        j = 0;
        if ((candidate = (char *)D_L06_001AC180[j]) != 0)
            do {
                int   k;
                float range;
                float score;
                int   kind;
                kind = GADGEBOT_TARGET_KIND_UNAVAILABLE;
                if (*(char **)(candidate + 0x24) != 0)
                    kind = *(short *)(*(char **)(candidate + 0x24) + 0x46);
                if (kind != GADGEBOT_TARGET_KIND_ATTACKABLE)
                    continue;
                range = 12.0f;
                if (S16(vars, 0x198) != 0)
                    range = 5.0f;
                if (range < vector_distance_xy(&MOBY(moby)->pos, candidate + 0x10))
                    continue;
                if (1.0f < absolute_float(F32(moby, 0x18) - F32(candidate, 0x18)))
                    continue;
                *(u128_c *)start = *(u128_c *)&MOBY(moby)->pos;
                *(u128_c *)end = *(u128_c *)(candidate + 0x10);
                *(float *)(start + 8) += 0.25f;
                *(float *)(end + 8) += 0.25f;
                if (collision_line(start, end, 2, (int)moby, 0) != 0 &&
                    (char *)D_L06_001745C0.moby != candidate)
                    continue;
                score = score_gadgebot_target_candidate(moby, candidate, 5);
                for (k = 0; k < CANDIDATE_COUNT(vars); k++) {
                    if (score < CANDIDATE_SCORES(vars)[k]) {
                        int last_index = CANDIDATE_COUNT(vars) - 1;
                        int shift_from = (last_index < 7) ? last_index : 6;
                        int m;
                        for (m = shift_from; m >= k; m--) {
                            CANDIDATES(vars)[m + 1] = CANDIDATES(vars)[m];
                            CANDIDATE_SCORES(vars)[m + 1] = CANDIDATE_SCORES(vars)[m];
                        }
                        break;
                    }
                }
                if (k < 8) {
                    CANDIDATES(vars)[k] = (int)candidate;
                    CANDIDATE_SCORES(vars)[k] = score;
                    CANDIDATE_COUNT(vars)++;
                    if (CANDIDATE_COUNT(vars) >= 9)
                        CANDIDATE_COUNT(vars) = 8;
                }
            } while ((candidate = (char *)D_L06_001AC180[++j]) != 0);
    }

    target_count = 0;
    for (i = 0; i < moby_count; i++) {
        char *vars = PVARS(mobys[i]);
        int   k;
        for (k = 0; k < CANDIDATE_COUNT(vars); k++) {
            if (target_count < 0x32) {
                int rank_weight = 8 - k;
                int m;
                for (m = 0; m < target_count; m++) {
                    if (targets[m] == CANDIDATES(vars)[k]) {
                        weights[m] += rank_weight;
                        break;
                    }
                }
                if (m == target_count) {
                    targets[target_count] = CANDIDATES(vars)[k];
                    weights[target_count] = rank_weight;
                    target_count++;
                }
            }
        }
    }

    for (i = 0; i < moby_count; i++) {
        ((GbSlots *)PVARS(mobys[i]))->claimed_target = 0;
    }

    for (pass = 0; pass < target_count; pass++) {
        float spread_per_gap = 0.5235988f;
        float max_spread = 1.7453293f;
        int   weight = 0;
        int   m;
        int   count;
        int   wanted;
        int   target;
        char *target_capacity_record;
        index = 0;
        for (m = 0; m < target_count; m++) {
            if (weight < weights[m]) {
                weight = weights[m];
                index = m;
            }
        }
        weights[index] = -1;
        target = targets[index];
        count = 0;
        wanted = 1;
        target_capacity_record = (char *)get_moby_target_info(target);
        if (target_capacity_record != 0)
            wanted = S16(target_capacity_record, 4);
        while (count < wanted) {
            float best_score = 9999999.0f;
            char *best_moby = 0;
            for (i = 0; i < moby_count; i++) {
                char *moby = mobys[i];
                char *vars = PVARS(moby);
                int   k;
                if (((GbSlots *)vars)->claimed_target != 0)
                    continue;
                for (k = 0; k < CANDIDATE_COUNT(vars); k++) {
                    if (CANDIDATES(vars)[k] == target && CANDIDATE_SCORES(vars)[k] < best_score) {
                        best_score = CANDIDATE_SCORES(vars)[k];
                        best_moby = moby;
                    }
                }
            }
            if (best_moby == 0)
                break;
            ((GbSlots *)PVARS(best_moby))->claimed_target = target;
            selected[count] = best_moby;
            count++;
        }
        if (count != 0) {
            float gaps = (float)(count - 1);
            float spread = gaps * spread_per_gap;
            float step;
            float first_angle;
            if (max_spread < spread)
                spread = 1.7453293f;
            step = 0.0f;
            if (count >= 2)
                step = spread / gaps;
            first_angle = spread * -0.5f;
            for (i = 0; i < count; i++) {
                char *vars = PVARS(selected[i]);
                ((GbSlots *)vars)->spread_angle = fast_add_rotations(first_angle, (float)i * step);
            }
        }
    }

    for (i = 0; i < moby_count; i++) {
        GbSlots *slots = (GbSlots *)PVARS(mobys[i]);
        int      claimed = slots->claimed_target;
        slots->search_frame = level_frame_count;
        slots->target = (char *)claimed;
        slots->target_searched = 1;
        if (claimed != 0)
            slots->has_target = 1;
    }
}

void select_nearest_gadgebot_interaction_target(void *moby_arg) {
    GbMoby *moby = moby_arg;
    GbVars *gadgebot_vars = moby->vars;
    GbMoby *candidate;
    float   best = 12.0f;
    int     list;
    int    *cursor;

    gadgebot_vars->target = 0;
    if (gadgebot_vars->target_group != -1) {
        get_first_moby_in_group(&list, gadgebot_vars->target_group, 0, 0);
        while (list != 0) {
            if (((GbMoby *)list)->cls_id == 0x3b1 || ((GbMoby *)list)->cls_id == 0x400 ||
                ((GbMoby *)list)->cls_id == 0x515 || ((GbMoby *)list)->cls_id == 0x516) {
                if (!(best < vector_distance_xy(moby->pos, ((GbMoby *)list)->pos)) &&
                    !(0.2f < absolute_float(moby->pos[2] - ((GbMoby *)list)->pos[2])) &&
                    !(1.5f < absolute_float(moby->pos[2] - ((GbMoby *)list)->pos[2]))) {
                    if ((((GbMoby *)list)->cls_id != 0x400 && ((GbMoby *)list)->cls_id != 0x516) ||
                        ((GbMoby *)list)->state != 2) {
                        if (((GbMoby *)list)->cls_id != 0x515 ||
                            *(int *)((GbMoby *)list)->vars != -1) {
                            best = vector_distance_xy(moby->pos, ((GbMoby *)list)->pos);
                            gadgebot_vars->target = (GbMoby *)list;
                        }
                    }
                }
            }
            get_next_moby_in_group(&list, list, 0, 0);
        }
    } else {
        cursor = D_L06_001AC180;
        while ((candidate = (GbMoby *)*cursor) != 0) {
            if (candidate->cls_id == 0x3b1 || candidate->cls_id == 0x400 ||
                candidate->cls_id == 0x515 || candidate->cls_id == 0x516) {
                if (!(best < vector_distance_xy(moby->pos, candidate->pos)) &&
                    !(0.2f < absolute_float(moby->pos[2] - candidate->pos[2])) &&
                    !(1.5f < absolute_float(moby->pos[2] - candidate->pos[2]))) {
                    if ((candidate->cls_id != 0x400 && candidate->cls_id != 0x516) ||
                        candidate->state != GADGEBOT_ABSORPTION_DISABLED) {
                        if (candidate->cls_id != 0x515 || *(int *)candidate->vars != -1) {
                            best = vector_distance_xy(moby->pos, candidate->pos);
                            gadgebot_vars->target = candidate;
                        }
                    }
                }
            }
            cursor++;
        }
    }
    gadgebot_vars->target_searched = 1;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F4058), where it is exact; names translated to the US level program. */
int process_gadgebot_interaction_candidate(
    char *ctx, char *ow, char *moby, int flag, int *out) __asm__("FUN_L06_002f2c28");

int process_gadgebot_interaction_candidate(

    char *ctx, char *ow, char *moby, int flag, int *interaction_status_out) {
    float candidate_position[4];
    float vB[4];
    float vA[4];
    float vC[4];
    float vD[4];
    int   placement_index;
    int   k;
    char *relocation_trigger_vars;
    char *destination_moby;
    float f20;

    if (MOBY(moby)->oclass != 0x515 && MOBY(moby)->oclass != 0x15F) {
        fast_vec_add(candidate_position, &MOBY(moby)->pos, moby + 0xC0);
    } else {
        *(u128_F4058 *)candidate_position = *(u128_F4058 *)&MOBY(moby)->pos;
    }

    switch (MOBY(moby)->oclass) {
    case MOBY_CLASS_RELOCATION_TRIGGER:
    case MOBY_CLASS_OFFSET_RELOCATION_TRIGGER:
    case MOBY_CLASS_RELOCATION_TRIGGER_WITH_SOUND:
        if (!(vector_distance_xy(ctx + 0x10, candidate_position) < 1.0f))
            return 0;
        relocation_trigger_vars = ((char *)MOBY(moby)->pvars);
        if (*(int *)relocation_trigger_vars < 0)
            return 0;
        placement_index = 0;
        if (MOBY(moby)->oclass == 0x3B1) {
            placement_index = *(int *)(relocation_trigger_vars + 4);
            *(int *)(relocation_trigger_vars + 4) = placement_index + 1;
        } else if (MOBY(moby)->oclass == 0x515 || MOBY(moby)->oclass == 0x15F) {
            placement_index = *(int *)(relocation_trigger_vars + 4);
            *(int *)(relocation_trigger_vars + 4) = placement_index + 1;
        }
        if (MOBY(moby)->oclass == 0x515) {
            trooper_play_sound(0, 0, (int)moby, 0x515);
        }

        for (k = 0; k < 10; k++) {
            int   r16, r17, r18, r2, gc;
            float f0, lift, f20, f21;
            f21 = random_float_between(0.1f, 1.0f);
            r18 = random_int_between(0x40, 0xFF);
            r17 = random_int_between(0x40, 0xFF);
            r16 = random_int_between(0x40, 0xFF);
            f20 = fast_cos(random_angle_radians()) * random_float_between(0.0f, 0.25f);
            vA[0] = f20;
            f20 = fast_sin(random_angle_radians());
            f20 = f20 * random_float_between(0.0f, 0.25f);
            vA[2] = 0.0f;
            vA[1] = f20;
            lift = vA[2] + random_float_between(0.0f, 0.5f);
            vA[2] = lift;
            add_vectors(vC, vA, ctx + 0x10);
            f0 = random_float_between(0.0f, D_0015ED6C);
            fast_vec_normalize(vB, vB, f0);
            f0 = random_float_between(D_0015ED6C * 2.5f, D_0015ED6C * 5.0f);
            vB[2] = f0;
            r2 = scale_game_frames(random_int_between(0xF, 0x1E));
            gc = (r17 << 8) | 0x7F000000;
            FUN_L00_00272060(vA,
                             r2,
                             f21 * 0.07f,
                             f21 * 0.7f,
                             (r16 << 16) | gc | r18,
                             0,
                             random_integer_below(2) ? 1 : -1,
                             D_0015ED70 * 15.0f,
                             vB);
        }

        destination_moby = (*(char **)&D_L06_0015FFD8) + (*(int *)relocation_trigger_vars << 8);
        f20 =
            fast_normalize_angle(convert_integer_to_float(placement_index) * 60.0f * 0.017453292f);
        vB[0] = fast_cos(fast_add_rotations(MOBY(destination_moby)->rot.z, f20));
        vB[1] = fast_sin(fast_add_rotations(MOBY(destination_moby)->rot.z, f20));
        vB[2] = 0.0f;
        fast_vec_add(ctx + 0x10, vB, &MOBY(destination_moby)->pos);
        *(u128_F4058 *)(ow + 0x2F0) = *(u128_F4058 *)(ow + 0x2E0) = *(u128_F4058 *)(ctx + 0x10);

        for (k = 0; k < 10; k++) {
            int   r16, r17, r18, r2, gc;
            float f0, lift, f20, f21;
            f21 = random_float_between(0.1f, 1.0f);
            r18 = random_int_between(0x40, 0xFF);
            r17 = random_int_between(0x40, 0xFF);
            r16 = random_int_between(0x40, 0xFF);
            f20 = fast_cos(random_angle_radians()) * random_float_between(0.0f, 0.25f);
            vC[0] = f20;
            f20 = fast_sin(random_angle_radians());
            f20 = f20 * random_float_between(0.0f, 0.25f);
            vC[2] = 0.0f;
            vC[1] = f20;
            lift = vC[2] + random_float_between(0.0f, 0.5f);
            vC[2] = lift;
            add_vectors(vD, vC, ctx + 0x10);
            f0 = random_float_between(0.0f, D_0015ED6C);
            fast_vec_normalize(vA, vA, f0);
            f0 = random_float_between(D_0015ED6C * 2.5f, D_0015ED6C * 5.0f);
            vA[2] = f0;
            r2 = scale_game_frames(random_int_between(0xF, 0x1E));
            gc = (r17 << 8) | 0x7F000000;
            FUN_L00_00272060(vC,
                             r2,
                             f21 * 0.07f,
                             f21 * 0.7f,
                             (r16 << 16) | gc | r18,
                             0,
                             random_integer_below(2) ? 1 : -1,
                             D_0015ED70 * 15.0f,
                             vA);
        }

        *(char *)(ctx + 0x20) = 4;
        *interaction_status_out = 1;
        if (*(int *)(ow + 0x310) >= 2) {
            int  path_slot_index;
            int *ids = (int *)(ow + 0x1E0);
            int *path_nodes = (int *)(ow + 0x220);
            for (path_slot_index = 0; path_slot_index < 16; path_slot_index++) {
                if (ids[path_slot_index] >= 0) {
                    if (compute_clear_path_point_mask(0.0f,
                                                      (int *)(ow + 0x1E0) + path_slot_index,
                                                      1,
                                                      path_nodes[path_slot_index],
                                                      ctx + 0x10) != 0) {
                        *(int *)(ow + 0x27C) = path_slot_index;
                    }
                }
            }
        }
        return 0;
    case MOBY_CLASS_GADGEBOT_ABSORPTION_TRIGGER:
    case MOBY_CLASS_GADGEBOT_ABSORPTION_TARGET:
        if (flag != 0)
            return 0;
        if (MOBY(moby)->state == GADGEBOT_ABSORPTION_DISABLED)
            return 0;
        if (!(vector_distance_xy(ctx + 0x10, candidate_position) < 1.0f))
            return 0;
        *(char *)(ctx + 0x20) = 8;
        *(int *)(ow + 0x304) = (int)moby;
        *interaction_status_out = 2;
        return 0;
    }
    return 0;
}

/* Runs the candidate check FUN_L06_002f2c28 over the moby's candidate list (or the global default list when it has none);
   returns the output of the first accepted candidate. */
int process_gadgebot_interactions(char *moby, int flag) __asm__("FUN_L06_002f32f0");

int process_gadgebot_interactions(char *moby, int flag) {
    char   **moby_ref;
    Iter32f0 iter;
    char    *state;
    int      list_id;
    int     *cursor;
    int      interaction_candidate_moby;

    state = ((char *)MOBY(moby)->pvars);
    iter.out = 0;
    list_id = *(int *)(state + 0x30C);
    if (list_id != -1) {
        get_first_moby_in_group(&iter.list, list_id, 0, 0);
        while (iter.list != 0) {
            if (process_gadgebot_interaction_candidate(moby, state, iter.list, flag, &iter.out))
                return iter.out;
            get_next_moby_in_group(&iter.list, iter.list, 0, 0);
        }
    } else {
        cursor = D_L06_001AC180;
        interaction_candidate_moby = *cursor;
        while (interaction_candidate_moby != 0) {
            moby_ref = &moby;
            if (process_gadgebot_interaction_candidate(
                    *moby_ref, state, interaction_candidate_moby, flag, &iter.out))
                return iter.out;
            cursor++;
            interaction_candidate_moby = *cursor;
        }
    }
    return iter.out;
}

/* Looks for a target (query 0x330001); if its owner is a different hostile, sets up the aim/attack data and enters state 9. */
void handle_gadgebot_hit_knockback(unsigned char *moby) __asm__("FUN_L06_002f3640");

void handle_gadgebot_hit_knockback(unsigned char *moby) {
    char *state = ((char *)MOBY(moby)->pvars);
    if (MOBY(moby)->state != GADGEBOT_UPDATE_KNOCKBACK && MOBY(moby)->seq != 2 &&
        MOBY(moby)->prev_seq != 2) {
        char *target = find_moby_hit(moby, 0x330001, 0);
        if (target != 0) {
            Q     hit_vector_copy;
            float base_angle = fast_arc_tan(*(float *)(target + 0x10), *(float *)(target + 0x14));
            char *hit_source_moby = *(char **)(target + 0x20);
            float knockback_angle;
            knockback_angle = base_angle;
            if (hit_source_moby != D_001413D0 &&
                MOBY(hit_source_moby)->oclass != MOBY(moby)->oclass) {
                int frames_sq = scale_game_frames(0x50);
                frames_sq *= scale_game_frames(0x50);
                state[0xBD] = 0;
                *(int *)(state + 0xA4) = 9;
                *(float *)(state + 0x90) = 4.0f / (float)frames_sq;
                trooper_set_knockback(state + 0x80, 2.0f, 0.5f);
                hit_vector_copy = *(Q *)(target + 0x10);
                trooper_knockback_arc(
                    &hit_vector_copy, &knockback_angle, state + 0x98, state + 0x9C);
                trooper_start_knockback(moby, state + 0x80, 4, 5, 2, knockback_angle);
                {
                    float doubled_frame_time = frame_time + frame_time;
                    *(float *)(state + 0xD0) = 10.0f;
                    *(float *)(state + 0xD4) = 20.0f;
                    *(float *)(state + 0xCC) = doubled_frame_time;
                }
                FUN_0022da68(2, 0, moby);
                MOBY(moby)->state = GADGEBOT_UPDATE_KNOCKBACK;
            }
        }
    }
    MOBY(moby)->hit_slot = 0xFF;
}

/* for each moby in a search list with the owner's id, start three effects on its data */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F4C00), where it is exact; names translated to the US level program. */
void draw_gadgebot_group_glow_effects(char *owner) __asm__("FUN_L06_002f37d0");

void draw_gadgebot_group_glow_effects(char *group_reference_moby) {
    int   list;
    float effect_scale = 0.057f;
    int   i;
    char *moby_vars;
    char *effect_position;
    if (big_head_cheat_enabled_macro[3] != 0)
        effect_scale = 0.114f;
    get_first_moby_in_group(&list, ((unsigned char *)group_reference_moby)[0x21], 0, 0);
    while (list != 0) {
        if (*(unsigned char *)(list + 0x31) != 0) {
            moby_vars = *(char **)(list + 0x78);
            if (*(short *)(list + 0xA6) == MOBY(group_reference_moby)->oclass) {
                effect_position = moby_vars + 0x140;
                for (i = 2; i >= 0; i--) {
                    draw_offset_textured_effect_quad(
                        effect_position, *(int *)(moby_vars + 0x290), effect_scale, 0.08f);
                    effect_position += 0x10;
                }
            }
        }
        get_next_moby_in_group(&list, list, 0, 0);
    }
}

void update_gadgebot_glow_effects(char *moby) {
    char *moby_vars;
    int   tint;
    int   i;
    f32   color_pulse_multiplier;
    int   r, g, b, a;

    if (MOBY(moby)->group == 0xFF) {
        return;
    }
    moby_vars = ((char *)MOBY(moby)->pvars);
    tint = 0x3228AAAA;
    if (MOBY(moby)->state == GADGEBOT_FOLLOW_HERO || MOBY(moby)->state == 6) {
        tint = 0x3228AA28;
    } else if (MOBY(moby)->state == GADGEBOT_SEEK_AND_ATTACK_TARGET ||
               MOBY(moby)->state == GADGEBOT_UPDATE_KNOCKBACK) {
        tint = 0x322828AA;
    } else if (MOBY(moby)->state == GADGEBOT_SEEK_INTERACTION_TARGET) {
        tint = 0x32AA2828;
    }

    *(f32 *)(moby_vars + 0x28C) =
        fast_add_rotations(*(f32 *)(moby_vars + 0x28C), frame_time * 2.9670596f);
    color_pulse_multiplier = 0.5f * fast_sin(*(f32 *)(moby_vars + 0x28C)) + 1.0f;

    r = tint & 0xFF;
    b = (tint >> 16) & 0xFF;
    g = (tint >> 8) & 0xFF;
    a = ((unsigned int)tint) >> 24;

    r = truncate_float_to_s32((f32)r * color_pulse_multiplier);
    if (r >= 0x100)
        r = 0xFF;
    g = truncate_float_to_s32((f32)g * color_pulse_multiplier);
    if (g >= 0x100)
        g = 0xFF;
    b = truncate_float_to_s32((f32)b * color_pulse_multiplier);
    if (b >= 0x100)
        b = 0xFF;

    {
        int packed = a << 24;
        packed |= b << 16;
        packed |= g << 8;
        packed |= r;
        MOBY(moby)->glow_color = FUN_001fa6e0_c(MOBY(moby)->glow_color, packed, 0.1f);
    }

    for (i = 0; i < 3; i++) {
        moby_point_position(moby, i + 1, moby_vars + 0x140 + i * 0x10);
    }
    {
        int dv = level_frame_count;
        *(int *)(moby_vars + 0x290) = MOBY(moby)->glow_color;
        if (dv != 0) {
            int *last_effect_frame_ptr = *(int **)(moby_vars + 0x294);
            if (dv != *last_effect_frame_ptr) {
                *last_effect_frame_ptr = dv;
                add_draw_callback(draw_gadgebot_group_glow_effects, moby);
            }
        }
    }
}

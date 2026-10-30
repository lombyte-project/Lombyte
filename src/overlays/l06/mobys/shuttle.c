/* Level 06 moby class 0x455: shuttle. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/game/queue_planet_unlock_message.h"
#include "rnc/overlay/shared/hero/update_hero_vehicle_attachments_and_effects.h"

/* Shuttle route selection; independent of Moby.state. */
enum ShuttleRoute {
    SHUTTLE_ROUTE_INITIAL_TO_SECOND_STOP = 0x0,
    SHUTTLE_ROUTE_SECOND_TO_INITIAL_STOP = 0x1,
    SHUTTLE_ROUTE_FORCED_TO_INITIAL_STOP = 0x2,
    SHUTTLE_ROUTE_INITIAL_TO_THIRD_STOP = 0x3,
    SHUTTLE_ROUTE_THIRD_TO_INITIAL_STOP = 0x4
};

extern int  is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

#include "rnc/overlay/cuboid.h"

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void jet_end_mount(int) __asm__("FUN_L00_002eac18");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_003023C8), where it is exact; names translated to the US level program. */
void         begin_moby_linear_and_angular_motion(char *moby,
                                                  void *linear_velocity,
                                                  void *angular_velocity) __asm__("FUN_L06_00300f70");
extern int   is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern int   help_request(int a, int b) __asm__("FUN_L00_00203908");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern s32   D_L06_00179B90[] __asm__("D_L06_00179B90");

#include "rnc/game/game_globals.h"

extern int  D_L06_0015F5C4 __asm__("D_L06_0015F5C4");
extern void set_selected_object_float_triplets(float, float, float, float, float, float) __asm__("FUN_L02_002f8aa0");
typedef int u128_302578 __attribute__((mode(TI)));

typedef struct {
    char  pad0[0x20];
    char  area[0x40];
    int   a[5];
    int   b[5];
    char  pad88[0x14];
    int   f9C;
    int   cam[3];
    int   cut;
    int   fB0;
    char  padB4[8];
    int   sel;
    int   side;
    float fC4;
    float fC8;
    char  padCC[0x24];
    int   fF0;
    int   fF4;
    int   fF8;
    int   fFC;
    int   zone[3];
    int   f10C;
    int   go;
    int   f114;
} Data;

typedef struct {
    char         pad0[0x2FC];
    struct Moby *f2FC;
    char         pad300[0xE];
    short        f30E;
} Hero_302578;

typedef struct {
    char          pad0[0x21];
    unsigned char f21;
} Flags;

typedef struct {
    int  f0;
    char pad4[0x20];
    int  f24;
} Busy_302578;

extern int   D_0015EE1D;
extern int   D_0015EE1C;
extern int   D_L06_0015F5C4;
extern int   D_L06_0015F594 __asm__("D_L06_0015F594");
extern char *D_L06_0015FFD8_302578 __asm__("D_L06_0015FFD8");
extern int   D_L06_00179B90[] __asm__("D_L06_00179B90");
extern char *D_L06_001B0C30_302578[] __asm__("D_L06_001B0C30");
extern char  D_0013D3A7_lui[] __asm__("D_0013D3A7") NOT_SDA;

#include "rnc/game/item_state.h"

extern char D_0013A4E0_lui[] __asm__("D_0013A4E0") NOT_SDA;
void update_shuttle_route_selection_and_prompt(char *shuttle_moby) __asm__("FUN_L06_00302fd8");
extern void update_shuttle_route_selection_and_prompt(char *) __asm__("FUN_L06_00302fd8");
void        mark_shuttle_linked_mobys_for_removal(char *moby) __asm__("FUN_L06_00304028");
extern void mark_shuttle_linked_mobys_for_removal(char *) __asm__("FUN_L06_00304028");
extern int  update_shuttle_path_ride(char *, int) __asm__("FUN_L06_00303160");
extern void spawn_timed_projected_sprite_effect(void *, int, int, int) __asm__("FUN_L06_0030bef8");
extern int  is_point_inside_clip_volume(void *, int);
extern void play_sound_302578(int, int) __asm__("FUN_L01_0027a248");
extern int  remove_help_message_for_selector(int) __asm__("FUN_L00_002644e0");
extern void fade_to_black(int) __asm__("FUN_001f4a58");
extern void initialize_moby_particle_effect_state(void *) __asm__("FUN_L06_002e6148");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void unlock_planet(int) __asm__("FUN_L00_002607d0");
extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");

#include "rnc/save/memcard_save_data.h"

extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

#include "rnc/audio/voice_alloc.h"

extern int D_L06_0015FDF8;

#include "rnc/overlay/l06/mobys/shuttle_data/shuttle_update_2.h"

extern float D_L06_001620F8 __attribute__((sda));
extern int   shuttle_forced_return_requested_gp __asm__("D_L06_00162100") __attribute__((sda));

#include "rnc/overlay/l06/mobys/shuttle_data/shuttle_update.h"

extern int           D_L06_0016210C __attribute__((sda));
extern int           D_L06_00162110 __attribute__((sda));
extern unsigned char D_0013D4D5[] NOT_SDA;

/* Update of moby class 1109 (shuttle): waits at one of three camera points, takes the hero's choice of destination, flies there through a cutscene and hands over to the level's travel sequence. */
void                             shuttle_update(struct Moby *moby) __asm__("FUN_L06_00302578");
extern unsigned short D_00141C00 NOT_SDA;
extern void                      try_set_help_message(int, int) __asm__("FUN_00215130");

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) PathPoint_303160;

typedef struct {
    int              n;
    int              pad[3];
    PathPoint_303160 p[1];
} Path;

struct PathRide {
    char  pad0[0x60];
    int   path_a[5];
    int   path_b[5];
    int   path_c[5];
    char  pad1[0x14];
    float sway;
    float sway_speed;
    char  pad2[0x0C];
    float scale_start;
    float scale_end;
    char  pad3[4];
    int   sound_slot[9];
    float progress;
};

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float
approach_angle(float *p, float *v, float a, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern int allocate_voice_for_group_entry(int, int, void *) __asm__("FUN_0022dba0");
extern void
push_piece_303160(float, struct Moby *, void *, void *, int) __asm__("FUN_L06_00303748");
extern void break_apart_shuttle_linked_mobys(struct Moby *) __asm__("FUN_L06_00303b68");
extern char D_L06_001B0C30_303160[] __asm__("D_L06_001B0C30");

typedef struct {
    int v[6];
} Colors;

Colors D_L06_00201960 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

Colors D_L06_00201948 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

extern float D_L06_00162114 __attribute__((sda));
extern float D_L06_00162118 __attribute__((sda));
extern float D_L06_0016211C __attribute__((sda));
extern float D_L06_00162120 __attribute__((sda));
extern float D_L06_00162124 __attribute__((sda));
extern float D_L06_00162128 __attribute__((sda));
extern float D_L06_0016212C __attribute__((sda));
extern float D_L06_00162130 __attribute__((sda));
extern int   D_L06_00162134 __attribute__((sda));
extern int   D_L06_00162138 __attribute__((sda));
extern int   D_L06_0016213C __attribute__((sda));
extern int   D_L06_00162140 __attribute__((sda));
extern int   D_L06_00162144 __attribute__((sda));
extern int   D_L06_00162148 __attribute__((sda));
extern int   D_L06_0016214C __attribute__((sda));
extern int   D_L06_00162150 __attribute__((sda));
extern int   D_L06_00162154 __attribute__((sda));
extern int   D_L06_00162158 __attribute__((sda));
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void  random_vector(void *, float, float) __asm__("FUN_L00_00257d78");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern u8 *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void spawn_particle(void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");
extern char  *D_L06_0015FFD8_03b68 __asm__("D_L06_0015FFD8");
extern short *D_L06_001ABFC0_03b68[] __asm__("D_L06_001ABFC0");
extern float  shuttle_breakup_particle_max_speed_gp __asm__("D_L06_0016215C") __attribute__((sda));

#include "rnc/overlay/l06/mobys/shuttle_data/break_apart_shuttle_linked_mobys_2.h"

extern int shuttle_breakup_particles_per_piece_gp __asm__("D_L06_00162190") __attribute__((sda));

#include "rnc/overlay/l06/mobys/shuttle_data/break_apart_shuttle_linked_mobys.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305468), where it is exact; names translated to the US level program. */
extern char  *D_L06_0015FFD8_u __asm__("D_L06_0015FFD8");
extern short *D_L06_001ABFC0_c[] __asm__("D_L06_001ABFC0");

void begin_moby_linear_and_angular_motion(char *moby,
                                          void *linear_velocity,

                                          void *angular_velocity) {
    char *motion_vars = ((char *)MOBY(moby)->pvars);
    moby[0x20] = 2;
    qcopy(motion_vars, linear_velocity);
    qcopy(motion_vars + 0x10, angular_velocity);
}

void shuttle_update(struct Moby *moby) {
    enum {
        INITIALIZE_SHUTTLE = 0,
        WAIT_FOR_SHUTTLE_ROUTE_REQUEST = 1,
        WAIT_FOR_FORCED_SHUTTLE_RETURN_TRIGGER = 2,
        UPDATE_SHUTTLE_PATH_RIDE = 3,
        RESTORE_SHUTTLE_IDLE_ANIMATION = 4,
        WAIT_FOR_HERO_NEAR_LINKED_MOBY = 6,
        WAIT_TO_SELECT_SHUTTLE_SAVE_FLOW = 7,
        WAIT_TO_ENTER_SHUTTLE_SAVE_FLOW = 8,
        WAIT_TO_SAVE_AND_RESUME_SHUTTLE_REQUESTS = 9
    };

    Data *shuttle_vars = *(Data **)&moby->pvars;
    float old_pos[4];
    float old_rot[4];
    float delta[4];
    float b[4];
    float rot[4];
    float a[4];
    int   route_index;
    int   busy;
    int   now;

    qcopy(old_pos, ((float *)&moby->pos));
    qcopy(old_rot, ((float *)&moby->rot));
    moby->scale = moby->pclass->scale * D_L06_001620F8;
    if (class_076a_moby_spawn_requested_gp != 0) {
        class_076a_moby_spawn_requested_gp = 0;
        *(u128_302578 *)b = *(u128_302578 *)((char *)&hero.motion.pos + 0x50);
        spawn_timed_projected_sprite_effect((char *)b,
                                            scale_game_frames(class_076a_moby_rate_input_frames_gp),
                                            D_L06_0016210C,
                                            D_L06_00162110);
    }
    switch (moby->state) {
    case INITIALIZE_SHUTTLE:
        D_L06_0015FDF8 = 0;
        if (*(unsigned char *)(shuttle_vars->fF8 + (current_level_index << 4) + (int)visited52) ==
            0xFF) {
            mark_shuttle_linked_mobys_for_removal((char *)moby);
        } else {
            D_0013D3A7_lui[5] = 0;
        }
        moby->update_dist = 0xFF;
        moby->draw_dist = 0x80;
        moby->spawn_frame = hero.moby->spawn_frame;
        moby->state = 1;
        if (moby->prev_seq != 2) {
            blend_moby_animation(moby, 2, 0, 0);
        }
        shuttle_vars->sel = 0;
        shuttle_vars->side = 0;
        break;
    case WAIT_FOR_SHUTTLE_ROUTE_REQUEST:
        busy = 0;
        if (*(unsigned char *)&D_0015EE1D != 0 || *(unsigned char *)&D_0015EE1C != 0) {
            if (((Busy_302578 *)D_L06_00179B90)->f0 != 0 ||
                ((Busy_302578 *)D_L06_00179B90)->f24 != -1) {
                busy = 1;
            }
        }
        if ((busy == 0 && ((Hero_302578 *)((char *)&hero))->f2FC == moby &&
             ((Hero_302578 *)((char *)&hero))->f30E == 0) ||
            shuttle_forced_return_requested_gp != 0) {
            update_shuttle_route_selection_and_prompt((char *)moby);
            now = shuttle_forced_return_requested_gp;
            if (now != 0 || ((*(int *)(D_0013A4E0_lui + 0x2604) & 0x10) && D_L06_0015F594 == 10 &&
                             shuttle_vars->f9C >= 0)) {
                shuttle_vars->go = 1;
                D_L06_0015FDF8 = 0;
                if (now != 0) {
                    shuttle_vars->f9C = 2;
                    shuttle_forced_return_requested_gp = 0;
                }
                switch (shuttle_vars->f9C) {
                case SHUTTLE_ROUTE_INITIAL_TO_SECOND_STOP:
                    shuttle_vars->sel = 1;
                    shuttle_vars->fC4 = D_L06_001620F8;
                    shuttle_vars->fC8 = shuttle_second_stop_ride_scale_multiplier_gp;
                    shuttle_vars->side = 0;
                    play_sound_302578(4, 8);
                    break;
                case SHUTTLE_ROUTE_SECOND_TO_INITIAL_STOP:
                    shuttle_vars->side = 1;
                    shuttle_vars->fC4 = shuttle_second_stop_ride_scale_multiplier_gp;
                    shuttle_vars->fC8 = D_L06_001620F8;
                    shuttle_vars->sel = 0;
                    play_sound_302578(0, 10);
                    break;
                case SHUTTLE_ROUTE_FORCED_TO_INITIAL_STOP:
                    shuttle_vars->side = 2;
                    shuttle_vars->fC4 = shuttle_second_stop_ride_scale_multiplier_gp;
                    shuttle_vars->fC8 = D_L06_001620F8;
                    shuttle_vars->sel = 0;
                    break;
                case SHUTTLE_ROUTE_INITIAL_TO_THIRD_STOP:
                    shuttle_vars->side = 3;
                    shuttle_vars->sel = 2;
                    shuttle_vars->fC4 = D_L06_001620F8;
                    shuttle_vars->fC8 = D_L06_001620F8;
                    break;
                case SHUTTLE_ROUTE_THIRD_TO_INITIAL_STOP:
                    shuttle_vars->side = 4;
                    shuttle_vars->sel = 0;
                    shuttle_vars->fC4 = D_L06_001620F8;
                    shuttle_vars->fC8 = D_L06_001620F8;
                    break;
                }
            }
        } else if (shuttle_vars->go == 0) {
            if (shuttle_vars->sel == 0 &&
                is_point_inside_clip_volume((char *)&hero.motion.pos, shuttle_vars->zone[0]) != 0) {
                int base = (int)l06_cuboids;
                int ofs = shuttle_vars->cam[2] << 7;
                qcopy(((float *)&moby->pos), (char *)(ofs + base + 0x30));
                qcopy(((float *)&moby->rot), (char *)(ofs + base + 0x70));
                shuttle_vars->sel = 2;
                D_L06_0015FDF8 = 0;
            } else if (shuttle_vars->sel == 2 &&
                       is_point_inside_clip_volume((char *)&hero.motion.pos,
                                                   shuttle_vars->zone[1]) != 0) {
                int ofs = shuttle_vars->cam[0] << 7;
                int base = (int)l06_cuboids;
                qcopy(((float *)&moby->pos), (char *)(ofs + base + 0x30));
                qcopy(((float *)&moby->rot), (char *)(ofs + base + 0x70));
                shuttle_vars->sel = 0;
                D_L06_0015FDF8 = 0;
            } else if (shuttle_vars->sel != 1 &&
                       is_point_inside_clip_volume((char *)&hero.motion.pos,
                                                   shuttle_vars->zone[2]) != 0) {
                int base = (int)l06_cuboids;
                int ofs = shuttle_vars->cam[1] << 7;
                qcopy(((float *)&moby->pos), (char *)(ofs + base + 0x30));
                qcopy(((float *)&moby->rot), (char *)(ofs + base + 0x70));
                shuttle_vars->sel = 1;
                D_L06_0015FDF8 = 1;
                if (((Flags *)(alternate_item_available))->f21 == 0) {
                    ((Flags *)(alternate_item_available))->f21 = 1;
                }
            }
        }
        if (shuttle_vars->go != 0) {
            shuttle_vars->go = 0;
            moby->state = 3;
            remove_help_message_for_selector(10);
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 0);
            }
            fade_to_black(scale_game_frames(10));
            shuttle_vars->fB0 = 0;
            route_index = shuttle_vars->side;
            qcopy(a, ((char **)D_L06_001B0C30_302578)[shuttle_vars->a[route_index]] + 0x10);
            qcopy(b, ((char **)D_L06_001B0C30_302578)[shuttle_vars->b[route_index]] + 0x10);
            clear_u64_value(rot);
            rot[2] = fast_arc_tan(b[0] - a[0], b[1] - a[1]);
            rot[1] = -fast_arc_tan(vector_distance_xy(a, b), b[2] - a[2]);
            jet_mount(a, rot, 2, scale_game_frames(300), 0);
            set_selected_object_float_triplets(0.001f, 1.0f, 1.0f, 0.001f, 1.0f, 1.0f);
            set_camera_position(a);
            set_camera_rotation(rot);
            set_race_camera(
                &l06_cuboids[shuttle_vars->cut].pos, &l06_cuboids[shuttle_vars->cut].rot, 0x72, 0);
            allocate_voice_for_target_entry(2, 0, moby);
        }
        break;
    case UPDATE_SHUTTLE_PATH_RIDE:
        if (update_shuttle_path_ride((char *)moby, shuttle_vars->side) != 0) {
            int ofs;
            int base;
            moby->state = 4;
            shuttle_vars->fF4 = 0;
            ofs = shuttle_vars->cam[shuttle_vars->sel] << 7;
            base = (int)l06_cuboids;
            qcopy(((float *)&moby->pos), (char *)(ofs + base + 0x30));
            qcopy(((float *)&moby->rot), (char *)(ofs + base + 0x70));
            euler_rotation_basis(((float *)&moby->rot_row0), ((float *)&moby->rot));
            moby->scale = moby->pclass->scale;
            if (shuttle_vars->sel == 1) {
                D_L06_0015FDF8 = 1;
                if (((Flags *)(alternate_item_available))->f21 == 0) {
                    ((Flags *)(alternate_item_available))->f21 = 1;
                }
            } else {
                D_L06_0015FDF8 = 0;
            }
            qcopy(b, ((float *)&moby->pos));
            fast_vec_normalize(a, ((float *)&moby->rot_row0), -2.0f);
            fast_vec_add(a, a, ((float *)&moby->rot_row2));
            fast_vec_add(b, b, a);
            clear_u64_value(rot);
            rot[2] = fast_add_rotations(moby->rot.z, 3.1415927f);
            set_race_camera(b, rot, 0, 0);
            if (shuttle_vars->f114 != 0) {
                D_0013D3A7_lui[5] = 1;
                initialize_moby_particle_effect_state(D_L06_0015FFD8_302578 +
                                                      (shuttle_vars->f10C << 8));
                shuttle_vars->f114 = 0;
                moby->state = 6;
                if (moby->prev_seq != 2) {
                    blend_moby_animation(moby, 2, 0, scale_game_frames(30));
                }
            }
            jet_end_mount(0);
        }
        break;
    case WAIT_FOR_HERO_NEAR_LINKED_MOBY:
        if (vector_distance_xy((char *)&hero.motion.pos,
                               D_L06_0015FFD8_302578 + (shuttle_vars->f10C << 8) + 0x10) < 3.5f) {
            char *linked_moby = D_L06_0015FFD8_302578 + (shuttle_vars->f10C << 8);
            qcopy(b, linked_moby + 0x10);
            qcopy(rot, linked_moby + 0x40);
            rot[2] = fast_add_rotations(rot[2], 3.14159f);
            set_race_camera(b, rot, 0x72, 1);
            play_sound_302578(0, 10);
            start_scene(12);
            mark_moby_for_removal(linked_moby);
            unlock_planet(5);
            moby->state = 8;
            jet_clear_persistent(shuttle_vars->fF8);
            jet_reset_camera(b, rot);
        }
        break;
    case WAIT_FOR_FORCED_SHUTTLE_RETURN_TRIGGER:
        if (is_point_inside_clip_volume((char *)&hero.motion.pos, shuttle_vars->fF0) != 0) {
            shuttle_vars->side = 2;
            shuttle_vars->fC4 = shuttle_second_stop_ride_scale_multiplier_gp;
            shuttle_vars->fC8 = D_L06_001620F8;
            shuttle_vars->sel = 0;
            shuttle_vars->go = 1;
            shuttle_vars->f114 = 1;
            moby->state = 1;
        }
        break;
    case RESTORE_SHUTTLE_IDLE_ANIMATION:
        moby->state = 1;
        if (moby->prev_seq != 2) {
            blend_moby_animation(moby, 2, 0, scale_game_frames(30));
        }
        break;
    case WAIT_TO_SELECT_SHUTTLE_SAVE_FLOW:
        if (D_L06_0015F5C4 != 2) {
            if (D_0013D4D5[9] != 0) {
                start_scene(5);
                moby->state = 9;
            } else {
                queue_planet_unlock_message(5);
                moby->state = 1;
                enter_hero_idle_state_for_current_vehicle_mode();
                memcard_save_data(0, -1);
            }
        }
        break;
    case WAIT_TO_SAVE_AND_RESUME_SHUTTLE_REQUESTS:
        if (D_L06_0015F5C4 != 2) {
            queue_planet_unlock_message(5);
            memcard_save_data(0, -1);
            moby->state = 1;
        }
        break;
    case WAIT_TO_ENTER_SHUTTLE_SAVE_FLOW:
        if (D_L06_0015F5C4 != 2) {
            queue_scene_movie_with_game_language(5);
            moby->state = 7;
        }
        break;
    }
    fast_vec_sub(delta, ((float *)&moby->pos), old_pos);
    update_entity_matrix(shuttle_vars->area, delta, old_rot, ((float *)&moby->rot));
}

void update_shuttle_route_selection_and_prompt(char *shuttle_moby) {
    enum {
        SHUTTLE_AT_INITIAL_STOP = 0,
        SHUTTLE_AT_SECOND_STOP = 1,
        SHUTTLE_AT_THIRD_STOP = 2
    };

    char *shuttle_vars = ((char *)MOBY(shuttle_moby)->pvars);
    switch (*(int *)(shuttle_vars + 0xBC)) {
    case SHUTTLE_AT_INITIAL_STOP: {
        int level_visit_byte_offset = current_level_index << 4;
        int primary_visit_byte = *(unsigned char *)(*(int *)(shuttle_vars + 0xF8) +
                                                    level_visit_byte_offset + (int)visited52);
        if (primary_visit_byte != 0xFF) {
            *(int *)(shuttle_vars + 0x9C) = 0;
        } else {
            int secondary_visit_byte = *(unsigned char *)(*(int *)(shuttle_vars + 0xFC) +
                                                          level_visit_byte_offset + (int)visited52);
            if (secondary_visit_byte == primary_visit_byte) {
                *(int *)(shuttle_vars + 0x9C) = 3;
            } else {
                *(int *)(shuttle_vars + 0x9C) = -1;
            }
        }
        break;
    }
    case SHUTTLE_AT_SECOND_STOP:
        *(int *)(shuttle_vars + 0x9C) = 1;
        break;
    case SHUTTLE_AT_THIRD_STOP:
        *(int *)(shuttle_vars + 0x9C) = 4;
        break;
    }
    if (D_00141C00 == 0 && *(int *)(shuttle_vars + 0x9C) == 0 &&
        (*(unsigned char *)&D_0015EE1D != 0 || *(unsigned char *)&D_0015EE1C != 0)) {
        help_request(0x177A, 0x53);
        *(int *)(shuttle_vars + 0x9C) = -1;
        return;
    }
    switch (*(int *)(shuttle_vars + 0x9C)) {
    case SHUTTLE_ROUTE_INITIAL_TO_SECOND_STOP:
        try_set_help_message(10, 0x1782);
        break;
    case SHUTTLE_ROUTE_SECOND_TO_INITIAL_STOP:
    case SHUTTLE_ROUTE_THIRD_TO_INITIAL_STOP:
        try_set_help_message(10, 0x1785);
        break;
    case SHUTTLE_ROUTE_FORCED_TO_INITIAL_STOP:
        try_set_help_message(10, 0x1786);
        break;
    case SHUTTLE_ROUTE_INITIAL_TO_THIRD_STOP:
        try_set_help_message(10, 0x1784);
        break;
    }
}

/* Moves the moby along three paths and plays the sound marked on the path points. */
int update_shuttle_path_ride(char *moby, int path) {
    float            rotation[4], position[4];
    struct PathRide *ride = ((struct PathRide *)MOBY(moby)->pvars);
    Path            *first_path = ((Path **)D_L06_001B0C30_303160)[ride->path_a[path]];
    Path            *middle_path = ((Path **)D_L06_001B0C30_303160)[ride->path_b[path]];
    Path            *third_path = ((Path **)D_L06_001B0C30_303160)[ride->path_c[path]];
    float            time, first_index_f, first_fraction, third_fraction, angle, rate;
    int              first_index, middle_index, third_index, marker, prev_marker;
    LevelCuboid     *sound;

    approach_value(&ride->progress, 1.0f, 1.0f / (float)scale_game_frames(0x4B0));
    time = ride->progress * (float)(first_path->n - 1);
    first_index = truncate_float_to_s32(time);
    first_index_f = (float)first_index;
    first_fraction = time - first_index_f;
    middle_index = truncate_float_to_s32(ride->progress * (float)(middle_path->n - 1));
    time = ride->progress * (float)(third_path->n - 1);
    third_fraction = time - first_index_f;
    third_index = truncate_float_to_s32(time);
    lerp_vector(
        position, &first_path->p[first_index], &first_path->p[first_index + 1], first_fraction);
    lerp_vector(
        moby + 0x10, &third_path->p[third_index], &third_path->p[third_index + 1], third_fraction);
    set_camera_position(position);
    if (third_index > 0) {
        angle = fast_arc_tan(third_path->p[third_index].x - third_path->p[third_index - 1].x,
                             third_path->p[third_index].y - third_path->p[third_index - 1].y);
        MOBY(moby)->rot.z = fast_add_rotations(
            fast_subtract_rotations(
                fast_arc_tan(third_path->p[third_index + 1].x - third_path->p[third_index].x,
                             third_path->p[third_index + 1].y - third_path->p[third_index].y),
                angle) *
                third_fraction,
            angle);
        angle = -fast_arc_tan(
            vector_distance_xy(&third_path->p[third_index - 1], &third_path->p[third_index]),
            third_path->p[third_index].z - third_path->p[third_index - 1].z);
        MOBY(moby)->rot.y = fast_add_rotations(
            fast_subtract_rotations(
                -fast_arc_tan(vector_distance_xy(&third_path->p[third_index],
                                                 &third_path->p[third_index + 1]),
                              third_path->p[third_index + 1].z - third_path->p[third_index].z),
                angle) *
                third_fraction,
            angle);
        MOBY(moby)->rot.y = MOBY(moby)->rot.y * fast_cos(ride->sway);
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, ride->sway);
    } else {
        MOBY(moby)->rot.y = -fast_arc_tan(vector_distance_xy(&third_path->p[0], &third_path->p[1]),
                                          third_path->p[1].z - third_path->p[0].z);
        MOBY(moby)->rot.z = fast_arc_tan(third_path->p[1].x - third_path->p[0].x,
                                         third_path->p[1].y - third_path->p[0].y);
    }
    if (first_index > 0 && middle_index > 0) {
        clear_u64_value(rotation);
        angle = fast_arc_tan(middle_path->p[middle_index].x - position[0],
                             middle_path->p[middle_index].y - position[1]);
        rotation[2] = fast_add_rotations(
            fast_subtract_rotations(fast_arc_tan(middle_path->p[middle_index + 1].x - position[0],
                                                 middle_path->p[middle_index + 1].y - position[1]),
                                    angle) *
                third_fraction,
            angle);
        angle = -fast_arc_tan(vector_distance_xy(position, &middle_path->p[middle_index]),
                              middle_path->p[middle_index].z - position[2]);
        rotation[1] = fast_add_rotations(
            fast_subtract_rotations(
                -fast_arc_tan(vector_distance_xy(position, &middle_path->p[middle_index + 1]),
                              middle_path->p[middle_index + 1].z - position[2]),
                angle) *
                third_fraction,
            angle);
        set_camera_rotation(rotation);
    }
    MOBY(moby)->scale =
        *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
        (ride->scale_start - (ride->scale_start - ride->scale_end) * ride->progress);
    if (ride->progress > 0.8f) {
        rate = frame_time_sq * 12.566371f;
        approach_angle(
            &ride->sway, &ride->sway_speed, 3.1415927f, rate, rate, frame_time * 6.2831855f);
    }
    marker = truncate_float_to_s32(first_path->p[first_index].w);
    if (first_index != 0)
        prev_marker = truncate_float_to_s32(first_path->p[first_index - 1].w);
    else
        prev_marker = -1;
    if (marker >= 0 || prev_marker >= 0) {
        first_path->p[first_index].w = -1.0f;
        first_path->p[first_index - 1].w = -1.0f;
        if (marker < 8 && prev_marker < 8) {
            allocate_voice_for_group_entry(0, 0, moby);
            sound = &l06_cuboids[ride->sound_slot[marker]];
            push_piece_303160(6.0f, (struct Moby *)moby, &sound->pos, sound, 6);
        } else if (marker == 10 || prev_marker == 10) {
            allocate_voice_for_group_entry(1, 0, moby);
            break_apart_shuttle_linked_mobys((struct Moby *)moby);
        }
    }
    return ride->progress == 1.0f;
}

void FUN_L06_00303748(float scale, char *moby, char *pos, char *push, int count) {
    float  position[4], direction[4], velocity[4], acceleration[4];
    Colors first_colors, second_colors;
    int    i;
    float  strength;
    cart_burst(moby, pos, -1, 2.0f, 13.0f);
    first_colors = D_L06_00201948;
    second_colors = D_L06_00201960;
    {
        int piece;
        for (piece = 0; piece < count; piece++) {
            int *first_color = first_colors.v + random_integer_below(6);
            int *second_color;
            int  first_time, second_time;
            strength = scale * 400000.0f;
            second_color = second_colors.v + random_integer_below(6);
            {
                int low, high;
                low = scale_game_frames(15);
                high = scale_game_frames(20);
                first_time = random_int_between(low, high);
            }
            {
                int low, high;
                low = scale_game_frames(25);
                high = scale_game_frames(30);
                second_time = random_int_between(low, high);
            }
            spawn_particle(pos,
                           push,
                           *first_color,
                           *second_color,
                           strength,
                           first_time,
                           second_time,
                           0,
                           0,
                           frame_time * 10.0f * scale);
        }
    }
    for (i = 0; i < count * D_L06_00162150;) {
        int color1, color2, life, size, fade;
        i++;
        random_vector(position, 0.0f, 1.0f);
        fast_vec_add(position, position, pos);
        random_vector(direction, 0.0f, 0.9f);
        fast_vec_add(direction, direction, push);
        fast_vec_scale(
            velocity, direction, random_float_between(D_L06_00162114, D_L06_00162118) * frame_time);
        random_vector(direction, 0.0f, 0.9f);
        fast_vec_add(direction, direction, push);
        fast_vec_scale(acceleration,
                       direction,
                       random_float_between(D_L06_00162124, D_L06_00162128) * frame_time);
        velocity[3] = random_float_between(D_L06_0016211C, D_L06_00162120) * scale;
        acceleration[3] = random_float_between(D_L06_0016212C, D_L06_00162130) * scale;
        color1 = fast_tween_color(D_L06_00162134, D_L06_00162138, random_float_between(0.0f, 1.0f));
        color2 = fast_tween_color(D_L06_0016213C, D_L06_00162140, random_float_between(0.0f, 1.0f));
        life = truncate_float_to_s32(
            scale_time(random_float_between((float)D_L06_00162144, (float)(D_L06_00162144 << 1))));
        size = truncate_float_to_s32(
            scale_time(random_float_between((float)D_L06_00162148, (float)D_L06_00162148 * 1.2f)));
        fade = truncate_float_to_s32(
            scale_time(random_float_between((float)D_L06_0016214C * 0.8f, (float)D_L06_0016214C)));
        spawn_spark(position,
                    velocity,
                    acceleration,
                    color1,
                    color2,
                    life,
                    size,
                    fade,
                    D_L06_00162154 + (D_L06_00162158 << 16));
    }
}

/* Breaks the linked pieces apart: pushes each one out from their middle (smaller pieces faster), spins it and sheds dust. */
void break_apart_shuttle_linked_mobys(struct Moby *moby) {
    char        *vars = (char *)moby->pvars;
    float        center[4];
    float        middle[4];
    float        push[4];
    float        spin[4];
    float        pos[4];
    float        particle_velocity[4];
    float        particle_acceleration[4];
    struct Moby *piece;
    short       *list;
    short        count;
    int          id;
    int          i;
    float        largest;
    u8          *particle;
    *(u128 *)center = 0;
    center[0] = 426.0f;
    center[1] = 189.75f;
    center[2] = 173.07f;
    spawn_timed_projected_sprite_effect(center,
                                        scale_game_frames(class_076a_moby_rate_input_frames_gp),
                                        D_L06_0016210C,
                                        D_L06_00162110);
    largest = 0.0f;
    clear_u64_value(middle);
    list = D_L06_001ABFC0_03b68[*(int *)(vars + 0xCC)];
    count = 0;
    do {
        id = *list & 0x7FFF;
        random_vector(push, 1.0f, 2.0f);
        FUN_L06_00303748(
            30.0f, moby, &((struct Moby *)(D_L06_0015FFD8_03b68 + (id << 8)))->pos, push, 1);
        fast_vec_add(middle, middle, &((struct Moby *)(D_L06_0015FFD8_03b68 + (id << 8)))->pos);
        count++;
        if (largest < ((struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68))->bsphere.w) {
            largest = ((struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68))->bsphere.w;
        }
    } while (*list++ >= 0);
    fast_vec_scale(middle, middle, 1.0f / convert_integer_to_float(count));
    list = D_L06_001ABFC0_03b68[*(int *)(vars + 0xCC)];
    do {
        float speed;
        id = *list & 0x7FFF;
        fast_vec_sub(push, &((struct Moby *)(D_L06_0015FFD8_03b68 + (id << 8)))->pos, middle);
        piece = (struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68);
        speed = frame_time * 30.0f;
        fast_vec_normalize(push, push, -((piece->bsphere.w - largest) / largest) * speed + speed);
        push[3] = 6.0f;
        random_vector(spin, frame_time * 0.2617994f, frame_time * 0.08726646f);
        begin_moby_linear_and_angular_motion(
            (char *)(D_L06_0015FFD8_03b68 + (id << 8)), push, spin);
        for (i = 0; i < shuttle_breakup_particles_per_piece_gp; i++) {
            piece = (struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68);
            random_vector(pos, 0.0f, piece->bsphere.w * 0.00048828125f);
            fast_vec_add(pos, pos, &((struct Moby *)(D_L06_0015FFD8_03b68 + (id << 8)))->pos);
            random_vector(
                particle_velocity, 0.0f, shuttle_breakup_particle_max_speed_gp * frame_time);
            clear_u64_value(particle_acceleration);
            particle_velocity[3] =
                random_float_between(shuttle_breakup_particle_velocity_w_random_bound_a_gp,
                                     shuttle_breakup_particle_velocity_w_random_bound_b_gp);
            particle_acceleration[3] =
                random_float_between(shuttle_breakup_particle_acceleration_w_random_bound_a_gp,
                                     shuttle_breakup_particle_acceleration_w_random_bound_b_gp);
            particle = spawn_spark(
                pos,
                particle_velocity,
                particle_acceleration,
                fast_tween_color(shuttle_breakup_particle_first_color_blend_a_gp,
                                 shuttle_breakup_particle_first_color_blend_b_gp,
                                 random_float_between(0.0f, 1.0f)),
                fast_tween_color(shuttle_breakup_particle_second_color_blend_a_gp,
                                 shuttle_breakup_particle_second_color_blend_b_gp,
                                 random_float_between(0.0f, 1.0f)),
                truncate_float_to_s32(
                    scale_time((float)shuttle_breakup_particle_timing_random_span_frames_gp *
                                   random_float_between(0.0f, 1.0f) +
                               1.0f)),
                truncate_float_to_s32(scale_time(
                    (float)shuttle_breakup_particle_timing_base_a_frames_gp *
                    (random_float_between(-shuttle_breakup_particle_timing_variation_fraction_gp,
                                          shuttle_breakup_particle_timing_variation_fraction_gp) +
                     1.0f))),
                truncate_float_to_s32(scale_time(
                    (float)shuttle_breakup_particle_timing_base_b_frames_gp *
                    (random_float_between(-shuttle_breakup_particle_timing_variation_fraction_gp,
                                          shuttle_breakup_particle_timing_variation_fraction_gp) +
                     1.0f))),
                shuttle_breakup_particle_table_index_and_flags_gp);
            if (particle != 0) {
                particle[9] = truncate_float_to_s32(4.0f) - 0x20;
            }
        }
    } while (*list++ >= 0);
    ((struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68))->command = 30;
}

/* Marks every moby in the moby's list for removal; the list ends after its first negative entry. */
void mark_shuttle_linked_mobys_for_removal(char *moby) {
    char  *data = ((char *)MOBY(moby)->pvars);
    short *moby_list_entry = D_L06_001ABFC0_c[*(int *)(data + 0xCC)];
    do {
        mark_moby_for_removal(D_L06_0015FFD8_u + ((*moby_list_entry & 0x7FFF) << 8));
    } while (*moby_list_entry++ >= 0);
}

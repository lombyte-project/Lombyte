/* Level 05 moby classes: sharkigator, scenery_ship, amoeboid, extermibot, extermitank, skid_mcmarx, floating_platform, box, water. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "macros.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/shared/mobys/manipulator_set_scale.h"
#include "rnc/overlay/shared/props/pool_set_scale.h"
#include "rnc/overlay/shared/vehicles/veldin_ship.h"
#include "rnc/overlay/shared/mobys/build_symmetric_adjacency_masks.h"
#include "rnc/overlay/shared/props/pool_init_tiles.h"

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8190), where it is exact; names translated to the US level program. */

extern int  get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int  get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

void draw_group_moby_glow_quads(unsigned char *moby) __asm__("FUN_L05_002d6f48");

void draw_group_moby_glow_quads(unsigned char *moby) {
    unsigned char *it;
    int            i;
    char          *data;
    char          *glow_position;
    get_first_moby_in_group((int *)&it, moby[0x21], 0, 0);
    while (it != 0) {
        if (it[0x31] != 0) {
            data = *(char **)(it + 0x78);
            if (*(short *)(it + 0xA6) == MOBY(moby)->oclass) {
                glow_position = data + 0x330;
                for (i = 2; i >= 0; i--) {
                    draw_offset_textured_effect_quad(
                        glow_position, *(int *)(data + 0x360), 0.45f, 0.4f);
                    glow_position += 0x10;
                }
            }
        }
        get_next_moby_in_group((int *)&it, (int)it, 0, 0);
    }
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8AD8), where it is exact; names translated to the US level program. */

extern void sample_camera_path(int *, int, void *, float *, int, float) __asm__("FUN_00214e58");

#include "rnc/overlay/l05/mobys/sharkigator_globals.h"

extern float scenery_ship_path_advance_scale_gp __asm__("D_L05_001613D0") __attribute__((sda));
void         advance_scenery_ship_along_path(char *moby, int *path) __asm__("FUN_L05_002d7890");

void advance_scenery_ship_along_path(char *moby, int *path) {
    float *moby_data = ((float *)MOBY(moby)->pvars);
    moby_data[2] = moby_data[2] + scenery_ship_path_advance_scale_gp * frame_time / moby_data[3];
    if ((float)path[0] < moby_data[2]) {
        moby_data[2] = moby_data[2] - (float)path[0];
    }
    sample_camera_path(path, 1, moby + 0x10, (float *)&MOBY(moby)->rot, 0, moby_data[2]);
    MOBY(moby)->rot.x = MOBY(moby)->rot.w;
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC1E8), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char  D_0013E533[];
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern void  allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void  spawn_amoeboid_hit_sparks(void *, void *) __asm__("FUN_L05_002db238");

void handle_amoeboid_hits(unsigned char *moby) __asm__("FUN_L05_002daf58");

void handle_amoeboid_hits(unsigned char *moby) {
    char *hit_record = find_moby_hit(moby, 0x330000, 0);
    if (hit_record != 0) {
        char *base_spark_vector = D_0013E533 + 0xF1D;
        char *hit_owner;
        moby[0x20] = 2;
        moby[0x31] = 0;
        MOBY(moby)->collision = 0;
        MOBY(moby)->flags |= 1;
        spawn_amoeboid_hit_sparks(moby, base_spark_vector);
        allocate_voice_for_target_entry(0, 0, (int)moby);
        hit_owner = *(char **)(hit_record + 0x20);
        if (hit_owner != 0 && *(short *)(hit_owner + 0xA6) == 0) {
            int   scaled_frame_increment = scale_game_frames(0x3C);
            char *y = base_spark_vector - 0x100;
            *(int *)(y + 0x8C4) += scaled_frame_increment;
        }
        moby[0xA4] = 0xFF;
    }
}

#include "qcopy.h"

extern void *create_moby() __asm__("FUN_0020c4f8");

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002F9B10), where it is exact; names translated to the US level program. */

extern void update_entity(void *) __asm__("FUN_L00_00250df8");

char *spawn_racer_child(char *owner) __asm__("FUN_L05_002f8718");

char *spawn_racer_child(char *owner) {
    char *moby = create_moby(0x1B7);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 2;
        qcopy(moby + 0x10, owner + 0x10);
        qcopy(moby + 0x40, owner + 0x40);
        MOBY(moby)->spawn_frame = *(unsigned long *)(owner + 0x38);
        update_entity(moby);
    }
    return moby;
}
/* Moves a point along a looped path of 16-byte nodes stored in the moby's data. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00308188), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");

void copy_racer_path_node(char *moby, int path_node_index, float *out) __asm__("FUN_L05_00306d30");

void copy_racer_path_node(char *moby, int path_node_index, float *out) {
    char *path = *(char **)(((char *)MOBY(moby)->pvars) + 0x244);
    float next_path_node[4];
    float perpendicular_heading;
    qcopy(out, path + 0x10 + path_node_index * 16);
    qcopy(next_path_node, path + 0x10 + ((path_node_index + 1) % *(int *)path) * 16);
    perpendicular_heading = fast_add_rotations(
        fast_arc_tan(next_path_node[0] - out[0], next_path_node[1] - out[1]), 1.5707964f);
    out[0] = out[0] + fast_cos(perpendicular_heading) * 0.0f;
    out[1] = out[1] + fast_sin(perpendicular_heading) * 0.0f;
}
/* advances a moby's path waypoint until it reaches one flagged 2.0 */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003087B0), where it is exact; names translated to the US level program. */

extern int offset_path_index(char *, int, int, int) __asm__("FUN_L00_0025d7a0");

void start_racer_branch_jump(char *moby) __asm__("FUN_L05_00307358");

void start_racer_branch_jump(char *moby) {
    char *moby_data = ((char *)MOBY(moby)->pvars);
    char *active_path = *(char **)(moby_data + 0x244);
    int   path_node_index;
    int   path_node_byte_offset;
    if (active_path != ((char *)l05_paths[*(int *)(moby_data + 0x220)])) {
        if (*(float *)(moby_data + 0x24C) <= 0.0f) {
            path_node_index = *(short *)(moby_data + 0x258);
            path_node_byte_offset = path_node_index * 16;
            if (*(float *)(active_path + path_node_byte_offset + 0x1C) == 1.0f) {
                while (*(float *)(active_path + path_node_byte_offset + 0x1C) != 2.0f) {
                    path_node_index = offset_path_index(active_path, path_node_index, 1, 1);
                    active_path = *(char **)(moby_data + 0x244);
                    path_node_byte_offset = path_node_index * 16;
                }
                *(int *)(moby_data + 0x27C) = path_node_index;
                qcopy(moby_data + 0x210,
                      *(char **)(moby_data + 0x244) + path_node_index * 16 + 0x10);
                *(int *)(moby_data + 0x26C) = 1;
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003088C8), where it is exact; names translated to the US level program. */

int map_racer_animation_to_child_index(int animation_index) __asm__("FUN_L05_00307470");

int map_racer_animation_to_child_index(int animation_index) {
    return animation_index == 5 ? 6 : animation_index;
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003088D8), where it is exact; names translated to the US level program. */

extern int  map_racer_animation_to_child_index(int) __asm__("FUN_L05_00307470");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

void blend_racer_and_child_animations(char *moby,
                                      int   animation_index,
                                      int   frame_index,
                                      int   blend_frames) __asm__("FUN_L05_00307480");

void blend_racer_and_child_animations(char *moby,
                                      int   animation_index,
                                      int   frame_index,

                                      int blend_frames) {
    char *data;
    char *child;
    float ratio;
    float child_frame_index;
    float max;
    int   child_animation_index;
    blend_moby_animation(moby, animation_index, frame_index, blend_frames);
    data = ((char *)MOBY(moby)->pvars);
    child = *(char **)(data + 0x268);
    if (child != 0) {
        ratio = *(float *)(child + 0x5C) / *(float *)(moby + 0x5C);
        child_animation_index = map_racer_animation_to_child_index(animation_index);
        child_frame_index = (float)frame_index * ratio;
        max = (float)*(unsigned char *)(*(char **)(*(char **)(*(char **)(data + 0x268) + 0x24) -
                                                   (-(child_animation_index * 4)) + 0x48) +
                                        0x10);
        if (max < child_frame_index)
            child_frame_index = max;
        else if (child_frame_index < 0.0f)
            child_frame_index = 0.0f;
        blend_moby_animation(
            *(char **)(data + 0x268), child_animation_index, (int)child_frame_index, blend_frames);
    }
}

typedef unsigned int U128_2d1688 __attribute__((mode(TI)));
extern float         D_L05_00161388 __attribute__((sda));
extern float         D_L05_001612E0_macro[4] __asm__("D_L05_001612E0") MACRO_ADDR;
extern float         D_L05_0013F640 __asm__("D_L05_0013F640");
extern float         D_0013F3D0[4] __asm__("D_0013F3D0");
extern char          D_L05_0013F3E0[] __asm__("D_L05_0013F3E0");
extern int           D_L05_001413D4 __asm__("D_L05_001413D4");
extern int           D_L05_001413DC __asm__("D_L05_001413DC");
extern unsigned char D_L05_001413F5 __asm__("D_L05_001413F5");
extern float         D_L05_001671D8 __asm__("D_L05_001671D8");
extern char          D_L05_001671C0[] __asm__("D_L05_001671C0");
extern char          D_L05_001671D0[] __asm__("D_L05_001671D0");

extern int   random_integer_below(int) __asm__("FUN_00213260");
extern void  mark_moby_2d1688(char *) __asm__("FUN_0020c828");
extern void  set_action_2d1688(char *, int, int, int) __asm__("FUN_00212f90");
extern float steer_2d1688(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern void  fast_vec_normalize(float, void *, void *) __asm__("FUN_001f9bf8");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float absolute_float(float) __asm__("func_001F99C0");
extern float
advance_accelerated_scalar(float *, float, float *, float, float, float) __asm__("FUN_00213f38");
extern int  point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern int  allocate_voice_for_target_entry_2d1688(int, int, char *) __asm__("FUN_0022da68");
extern void hero_respawn(void) __asm__("FUN_L00_00211250");
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void spawn_sharkigator_surface_effects(char *) __asm__("FUN_L05_002d1e30");
extern void spawn_sharkigator_splash_effects(int, char *) __asm__("FUN_L05_002d1fd8");

void sharkigator_update(char *moby) __asm__("FUN_L05_002d1688");

void sharkigator_update(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    int   state = MOBY(moby)->state;
    int   one = 1;
    char *path;
    char *point;
    float v[4] __attribute__((aligned(16)));
    float w[4] __attribute__((aligned(16)));
    float z[4] __attribute__((aligned(16)));
    float speed;
    float height;
    int   index;
    int   duration;

    switch (state) {
    case 0:
        goto state_zero;
    case 1:
        goto state_one;
    case 2:
        goto state_two;
    case 3:
        goto state_three;
    default:
        return;
    }
state_zero: {
    float height;
    if (*(int *)(data + 0x64) != 0) {
        MOBY(moby)->state = 2;
        MOBY(moby)->flags |= 1;
        MOBY(moby)->update_dist = 0xff;
        *(float *)(data + 0x74) = MOBY(moby)->pos.z;
        *(int *)(data + 0x7c) = scale_game_frames(30);
        return;
    }
    index = *(int *)(data + 0x60);
    if (index == -1) {
        mark_moby_2d1688(moby);
        return;
    }
    *(int *)(data + 0x68) = random_integer_below(*(int *)l05_paths[index]);
    *(int *)(data + 0x6c) = scale_game_frames(600);
    if (MOBY(moby)->prev_seq != 1) {
        set_action_2d1688(moby, 1, 0, scale_game_frames(10));
    }
    height = *(float *)(((char *)l05_paths[*(int *)(data + 0x60)]) + 0x18) - (D_L05_00161388);
    MOBY(moby)->pos.z = height;
    *(float *)(data + 0x80) = height;
    MOBY(moby)->state = one;
    return;
}
state_one: {
    path = ((char *)l05_paths[*(int *)(data + 0x60)]);
    qcopy(v, path + *(int *)(data + 0x68) * 16 + 0x10);
    point = moby + 0x48;
    {
        float y = v[1];
        float x = v[0];
        height = fast_arc_tan(x - MOBY(moby)->pos.x, y - MOBY(moby)->pos.y);
    }
    {
        float pi = 3.1415927f;
        float turn_step = frame_time_sq * pi;
        steer_2d1688((float *)(point),
                     (float *)(data + 0x70),
                     height,
                     turn_step,
                     turn_step,
                     frame_time * pi);
    }
    fast_vec_normalize(frame_time * 3.0f, w, moby + 0xc0);
    fast_vec_add(moby + 0x10, moby + 0x10, w);
    height = fast_vec_dist(moby + 0x10, v);
    if (height < 3.0f || fast_dec_timer(data + 0x6c) != 0) {
        index = random_integer_below(*(int *)l05_paths[*(int *)(data + 0x60)]);
        *(int *)(data + 0x68) =
            (*(int *)(data + 0x68) + index) % *(int *)l05_paths[*(int *)(data + 0x60)];
        *(int *)(data + 0x6c) = scale_game_frames(600);
    }
    if ((hero.state.control_mode == 0x12 || hero.state.current == 0x12) &&
        vector_distance_xy(moby + 0x10, &hero.motion.pos) < 8.0f) {
        speed = (D_L05_001612E0_macro[2] - D_L05_00161388) - 2.0f;
    } else {
        speed = (D_L05_001612E0_macro[2] - D_L05_00161388) - 0.5f;
        spawn_sharkigator_surface_effects(moby);
    }
    advance_accelerated_scalar((float *)&MOBY(moby)->pos.z,
                               speed,
                               (float *)(data + 0x78),
                               frame_time_sq + frame_time_sq,
                               frame_time_sq + frame_time_sq,
                               frame_time + frame_time);
    return;
}
state_two: {
    char *path;
    fast_dec_timer(data + 0x7c);
    height = absolute_float(hero.water_surface_z - *(float *)(data + 0x74));
    if (height < 0.33f && hero.state.control_mode == 0x12) {
        char *point;
        if (*(int *)(data + 0x7c) != 0)
            return;
        point = D_L05_001671C0 - 0x140;
        MOBY(moby)->rot.y = -1.5533431f;
        MOBY(moby)->rot.z = fast_add_rotations(*(float *)(point + 0x158), 1.5707964f);
        euler_rotation_basis(moby + 0xc0, moby + 0x40);
        qcopy(moby + 0x10, &hero.motion.pos);
        fast_vec_normalize((D_L05_00161388), v, moby + 0xe0);
        fast_vec_sub(moby + 0x10, moby + 0x10, v);
        {
            float speed = frame_time * 10.0f;
            MOBY(moby)->pos.z -= 2.0f;
            *(float *)(data + 0x78) = speed;
        }
        MOBY(moby)->flags &= 0xfffe;
        if (MOBY(moby)->prev_seq != state)
            set_action_2d1688(moby, 2, 0, scale_game_frames(5));
        MOBY(moby)->command = 0;
        MOBY(moby)->state = 3;
        jet_mount(point + 0x140, point + 0x150, 0, 0, 0);
        return;
    }
    if (hero.state.current == 0x12)
        return;
    path = ((char *)l05_paths[*(int *)(data + 0x60)]);
    if (point_in_path_xy(&hero.motion.pos, path + 0x10, *(int *)path) != 0)
        *(int *)(data + 0x7c) = scale_game_frames(600);
    else
        *(int *)(data + 0x7c) = scale_game_frames(180);
    return;
}
state_three: {
    float speed;
    {
        struct Hero *h = &hero;
        if (h->water_surface_z < MOBY(moby)->pos.z || *(float *)(data + 0x78) < 0.0f) {
            if (h->state.current != 0x72) {
                qcopy_nc(v, &h->motion.pos);
                {
                    /* Fakematch: the pin and the empty barriers order the call setup like
                           retail ($5 before the v[2] store, $4 after it, in the jal slot). */
                    register char *splash_pos __asm__("$5") = (char *)v;
                    BARRIER();
                    v[2] = h->water_surface_z;
                    BARRIER();
                    spawn_sharkigator_splash_effects((int)moby, splash_pos);
                }
                set_race_camera(&h->motion.pos, &h->motion.rot, 0x72, 0);
                /* Fakematch: the two join statements are repeated in both branches so
                       that a real insn sits between set_camera (the last call of this
                       block) and the block-ending branch; jump2 cross-jumps the identical
                       tails back into one join (same bytes). With the call adjacent to the
                       branch, haifa add_branch_dependences makes it depend on every insn,
                       including the a0=moby move, which then ties with a1 and the water
                       store for the 2-wide issue and the store takes the jal delay slot;
                       retail has a0 in the slot. Sibling rocket.c FUN_L18_002a7220 gets the
                       same order from a real intermediate call. */
                h->unk20A5 = one;
                h->motion.pos.f[2] += 20.0f;
            } else {
                qcopy(&h->motion.pos, moby + 0x10);
                h->unk20A5 = one;
                h->motion.pos.f[2] += 20.0f;
            }
            /* Fakematch: a gcse-time set of moby so PRE does not hoist the
                   *(moby + 0x18) load below out of the join; folds to nothing. */
            moby++;
            moby--;
            speed = *(float *)(data + 0x78);
        } else {
            speed = *(float *)(data + 0x78);
        }
    }
    {
        float acceleration = frame_time_sq * 10.0f;
        float threshold = frame_time + frame_time;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + speed;
        {
            float speed = *(float *)(data + 0x78) - acceleration;
            *(float *)(data + 0x78) = speed;
            if (speed < threshold) {
                fast_vec_normalize((D_L05_00161388), v, moby + 0xe0);
                steer_2d1688((float *)&MOBY(moby)->rot.y,
                             (float *)(data + 0x70),
                             1.5533431f,
                             frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f,
                             frame_time * 25.132742f);
                euler_rotation_basis(moby + 0xc0, moby + 0x40);
                fast_vec_normalize((D_L05_00161388), w, moby + 0xe0);
                fast_vec_sub(z, w, v);
                fast_vec_sub(moby + 0x10, moby + 0x10, z);
            }
        }
    }
    if (*(float *)(data + 0x78) < 0.0f) {
        if (MOBY(moby)->pos.z < hero.water_surface_z && MOBY(moby)->command == 0) {
            MOBY(moby)->command = 1;
            allocate_voice_for_target_entry_2d1688(0, 0, moby);
            fast_vec_normalize((D_L05_00161388), v, moby + 0xe0);
            fast_vec_add(v, v, moby + 0x10);
            v[2] = hero.water_surface_z;
            spawn_sharkigator_splash_effects((int)moby, (char *)v);
        }
    }
    if (*(float *)(data + 0x78) < 0.0f && MOBY(moby)->pos.z < *(float *)(data + 0x74) - 8.0f)
        hero_respawn();
    if ((MOBY(moby)->anim_flags & 2) != 0 && MOBY(moby)->prev_seq != 1)
        set_action_2d1688(moby, 1, 0, scale_game_frames(10));
}
}

typedef float V2d1e30[4] __attribute__((aligned(16)));
extern float  random_float_between(float, float) __asm__("FUN_002132a8");
extern void   fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern char  *spawn_2d1e30(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");
extern float  D_L05_001612E8 __asm__("D_L05_001612E8");
extern char   D_L05_0015F580_macro[] __asm__("D_L05_0015F580") MACRO_ADDR;

/* When the timer runs out, spawns two effects beside the moby, one to each side, and restarts the timer. */
void spawn_sharkigator_surface_effects(char *moby) __asm__("FUN_L05_002d1e30");

void spawn_sharkigator_surface_effects(char *moby) {
    V2d1e30 spawn_position;
    char   *data;
    char   *spawned_effect;
    float   opposite_heading;
    int     i;
    float  *spawn_height_ptr;

    data = ((char *)MOBY(moby)->pvars);
    if (fast_dec_timer(data + 0x84) == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        opposite_heading = fast_add_rotations(MOBY(moby)->rot.z, 3.14159274f);
        fast_add_rotations(opposite_heading, random_float_between(-0.785398006f, 0.785398006f));
        fast_vec_scale(spawn_position, moby + 0xD0, random_float_between(-0.5f, 0.5f));
        fast_vec_add(spawn_position, spawn_position, moby + 0x10);
        spawn_position[2] = D_L05_001612E8 + 0.05f;
        spawn_height_ptr = &D_L05_001612E8;
        spawned_effect = spawn_2d1e30(spawn_position,
                                      D_L05_0015F580_macro,
                                      spawn_height_ptr,
                                      random_float_between(0.7f, 1.0f),
                                      i == 0 ? 2.0f : -2.0f);
        if (spawned_effect != 0) {
            *(short *)(spawned_effect + 0xA) = scale_game_frames(15);
        }
        *(int *)(data + 0x84) = truncate_float_to_s32(random_float_between(3.0f, 5.0f));
    }
}

/* Spawns a burst of particles and a ring of sparks around a position. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D3220), where it is exact; names translated to the US level program. */

extern char  D_L05_0015F580_macro[] __asm__("D_L05_0015F580") MACRO_ADDR;
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float fast_cos(float);
extern float fast_sin(float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float D_L05_001612E8;
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern unsigned char             *
spawn_splash(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void spawn_debris_chunk(void *, void *, int, int) __asm__("FUN_L00_0026f548");

void spawn_sharkigator_splash_effects(int unused, char *position) __asm__("FUN_L05_002d1fd8");

void spawn_sharkigator_splash_effects(int unused, char *position) {
    float effect_spawn_vector[4];
    char *spawned_object;
    int   i;
    char *base;
    int   random_spawn_value;
    spawned_object = spawn_burst_moby(position, 3.0f);
    if (spawned_object)
        spawned_object[0x23] = 0x70;
    for (base = (char *)&D_L05_001612E8, i = 15; i >= 0; i--) {
        float effect_xy_heading = random_angle_radians();
        float k = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
        effect_spawn_vector[0] = fast_cos(effect_xy_heading) * k;
        effect_spawn_vector[1] = fast_sin(effect_xy_heading) * k;
        effect_spawn_vector[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
        random_spawn_value = random_int_between(0x5A, 0x78);
        spawn_debris_chunk(
            position, effect_spawn_vector, random_integer_below(2), random_spawn_value);
    }
    for (i = 0; i < 16; i++) {
        unsigned char *splash;
        random_vector(effect_spawn_vector, 1.0f, 1.0f);
        fast_vec_add(effect_spawn_vector, effect_spawn_vector, position);
        effect_spawn_vector[2] = D_L05_001612E8 + 0.05f;
        splash = spawn_splash(effect_spawn_vector,
                              D_L05_0015F580_macro,
                              base,
                              random_float_between(0.7f, 1.0f),
                              i == 0 ? 2.0f : -2.0f);
        if (splash) {
            *(short *)(splash + 0xA) =
                truncate_float_to_s32(scale_time(random_float_between(30.0f, 60.0f)));
        }
    }
}

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int  lerp_2d7020(float, int, int) __asm__("FUN_001fa6e0");
extern int  frame_number __asm__("D_L05_0015F5CC");
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Per-frame glow: updates three parts, pulses the colour, and queues a redraw
 * once per frame. */
void update_moby_glow_and_queue_draw(char *moby) __asm__("FUN_L05_002d7020");

void update_moby_glow_and_queue_draw(char *moby) {
    char *moby_data;
    int   i;
    int   interpolated_glow_color;
    int   current_frame;
    int   k;
    int  *last_queued_frame_ptr;

    if (MOBY(moby)->group != 0xFF && MOBY(moby)->visible != 0) {
        moby_data = ((char *)MOBY(moby)->pvars);
        for (i = 0; i < 3; i++) {
            moby_point_position(moby, i, moby_data + 0x330 + i * 16);
        }
        *(float *)(moby_data + 0x368) =
            fast_add_rotations(*(float *)(moby_data + 0x368), frame_time * 1.9198622f);
        interpolated_glow_color =
            lerp_2d7020(fast_sin(*(float *)(moby_data + 0x368)) * 0.5f + 0.5f, 0x1E1EB4, 0x1EB41E);
        current_frame = frame_number;
        MOBY(moby)->glow_color = interpolated_glow_color;
        k = (interpolated_glow_color & 0xFF0000) | 0x30000000;
        *(int *)(moby_data + 0x360) =
            k | (interpolated_glow_color & 0xFF00) | (interpolated_glow_color & 0xFF);
        if (current_frame != 0) {
            last_queued_frame_ptr = *(int **)(moby_data + 0x364);
            if (current_frame != *last_queued_frame_ptr) {
                *last_queued_frame_ptr = current_frame;
                add_draw_callback(draw_group_moby_glow_quads, moby);
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D8388), where it is exact; names translated to the US level program. */

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f_2d7140;

typedef int u128_2D8388 __attribute__((mode(TI)));

typedef union {
    u128_2D8388 q;
    float       f[4];
} Buf;

typedef struct {
    /* 0x000 */ int            path;
    /* 0x004 */ float          f4;
    /* 0x008 */ float          f8;
    /* 0x00C */ float          fC;
    /* 0x010 */ int            pad10[3];
    /* 0x01C */ float          f1C;
    /* 0x020 */ Vec4f_2d7140   trail[3][16];
    /* 0x320 */ short          f320;
    /* 0x322 */ unsigned short f322;
    /* 0x324 */ int           *f324;
    /* 0x328 */ int            f328;
    /* 0x32C */ int            pad32C[15];
    /* 0x368 */ float          f368;
} Data_2D8388;

extern char *FUN_L00_0025a420(void *, int, int);
extern float D_0015ED6C;
extern float D_0015ED70;

#include "rnc/overlay/l05/mobys/sharkigator_data/scenery_ship_update.h"

int          D_L05_001613D8 = 0x80000080;
int          D_L05_001613DC = 0x80202020;
float        D_L05_001613E0 = 5.0f;
int          D_L05_001613E4 = 1;
extern float scenery_ship_removal_effect_basis_scale_gp __asm__("D_L05_001613E8")
    __attribute__((sda));
extern float scenery_ship_removal_effect_z_addend_gp __asm__("D_L05_001613EC") __attribute__((sda));
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void  allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void  enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
extern void  mark_moby_for_removal_c(void *) __asm__("func_0020C828");
float        absolute_float(float input) __asm__("func_001F99C0");
int          voice_is_playing(int a, int i) __asm__("FUN_L00_0028d8c0");
void        *FUN_L00_00263fd8(char  *src,
                              int    cls,
                              float *pos,
                              void  *mat,
                              int    a8,
                              int    a9,
                              float *v10,
                              float *v11,
                              float  scale,
                              float *v12);
void         update_moby_glow_and_queue_draw(char *m) __asm__("FUN_L05_002d7020");
void         draw_group_scenery_ship_trails(struct Moby *moby) __asm__("FUN_L05_002d7920");
extern float func_00214158_2D8388(void) __asm__("FUN_00213308");
extern float func_001F9B88_2D8388(float input) __asm__("FUN_001f99c0");
extern float func_001F9FA8_2D8388(float) __asm__("FUN_001f9de0");
extern int   func_001FA8A8_2D8388(int, int, float) __asm__("FUN_001fa6e0");
extern void  advance_scenery_ship_along_path(char *moby, int *path) __asm__("FUN_L05_002d7890");
extern int   func_L00_0028EB98_2D8388(void *, int) __asm__("FUN_L00_0028d8c0");
extern int   func_0022ED80_2D8388(int, int, int) __asm__("FUN_0022da68");
extern void  func_001F9BD8_2D8388(float *, float *, float *) __asm__("FUN_001f9a10");
extern void  func_001F49B0_2D8388(void (*)(void), void *) __asm__("FUN_001f4600");
extern int   func_001F9850_2D8388(int) __asm__("FUN_001f96f8");
extern void *func_L00_00265050_2D8388(char  *src,
                                      int    cls,
                                      float *pos,
                                      void  *mat,
                                      int    a8,
                                      int    a9,
                                      float *v10,
                                      float *v11,
                                      float  scale,
                                      float *v12) __asm__("FUN_L00_00263fd8");
extern void  func_0020D678_2D8388(void *) __asm__("FUN_0020c828");
extern void  update_moby_glow_and_queue_draw(char *moby) __asm__("FUN_L05_002d7020");
extern void  func_L05_002D8B68_2D8388(void) __asm__("FUN_L05_002d7920");
extern float D_L05_001672C8_2D8388[] __asm__("D_L05_001671C8");

void scenery_ship_update(char *moby) __asm__("FUN_L05_002d7140");

void scenery_ship_update(char *moby) {
    enum {
        SCENERY_SHIP_INITIALIZE = 0,
        SCENERY_SHIP_FOLLOW_PATH = 1,
        SCENERY_SHIP_SPAWN_EFFECTS_AND_REMOVE = 2
    };
    Data_2D8388 *data = ((Data_2D8388 *)MOBY(moby)->pvars);
    char        *hit;
    char        *path;
    char        *current;
    char        *previous_path_node_base;
    char        *next;
    char        *point;
    int          i;
    int          k;
    int          lifetime;
    int          damaged;
    Buf          left_offset;
    Buf          tail_offset;
    Buf          right_offset;
    float        in_angle;
    float        out_angle;
    float        turn;
    float        max_turn;
    float        turn_scale;
    float       *effect_origin_position, *effect_origin_rotation, *tab;

    if (D_L05_001672C8_2D8388[0] < 55.0f)
        return;
    hit = find_moby_hit(moby, 0x10000, 0);
    damaged = 0;
    switch (MOBY(moby)->state) {
    case SCENERY_SHIP_INITIALIZE: {
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
                            scenery_ship_initialization_scale_multiplier_gp;
        data->f1C = func_00214158_2D8388();
        data->f328 = -1;
        data->f368 = func_00214158_2D8388();
        path = ((char *)l05_paths[data->path]);
        if (*(float *)(path + 0x1C) == -1.0f) {
            max_turn = 0.0f;
            point = path + ((*(int *)path - 1) << 4);
            in_angle = fast_arc_tan(*(float *)(path + 0x10) - *(float *)(point + 0x10),
                                    *(float *)(path + 0x14) - *(float *)(point + 0x14));
            out_angle = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                     *(float *)(path + 0x24) - *(float *)(path + 0x14));
            *(float *)(path + 0x1C) = fast_subtract_rotations(out_angle, in_angle);
            for (i = 1; i < *(int *)path - 1; i++) {
                current = path + i * 16;
                previous_path_node_base = path + (i - 1) * 16;
                in_angle = fast_arc_tan(
                    *(float *)(current + 0x10) - *(float *)(previous_path_node_base + 0x10),
                    *(float *)(current + 0x14) - *(float *)(previous_path_node_base + 0x14));
                next = path + (i + 1) * 16;
                out_angle = fast_arc_tan(*(float *)(next + 0x10) - *(float *)(current + 0x10),
                                         *(float *)(next + 0x14) - *(float *)(current + 0x14));
                turn = fast_subtract_rotations(out_angle, in_angle);
                *(float *)(current + 0x1C) = turn;
                if (max_turn < func_001F9B88_2D8388(turn)) {
                    max_turn = func_001F9B88_2D8388(*(float *)(current + 0x1C));
                }
            }
            in_angle = fast_arc_tan(*(float *)(path + ((*(int *)path - 1) << 4) + 0x10) -
                                        *(float *)(path + ((*(int *)path - 2) << 4) + 0x10),
                                    *(float *)(path + ((*(int *)path - 1) << 4) + 0x14) -
                                        *(float *)(path + ((*(int *)path - 2) << 4) + 0x14));
            out_angle = fast_arc_tan(
                *(float *)(path + 0x10) - *(float *)(path + ((*(int *)path - 1) << 4) + 0x10),
                *(float *)(path + 0x14) - *(float *)(path + ((*(int *)path - 1) << 4) + 0x14));
            *(float *)(path + ((*(int *)path - 1) << 4) + 0x1C) =
                fast_subtract_rotations(out_angle, in_angle);
            turn_scale = 1.04719755f / max_turn;
            for (k = 0; k < *(int *)path; k++) {
                point = path + (k << 4);
                *(float *)(point + 0x1C) = *(float *)(point + 0x1C) * turn_scale;
            }
        }
        data->fC = fast_vec_dist((float *)(path + 0x10), (float *)(path + 0x20));
        MOBY(moby)->state = 1;
        data->f8 = (float)*(int *)path * data->f4;
        break;
    }
    case SCENERY_SHIP_FOLLOW_PATH: {
        float phase = fast_add_rotations(data->f1C, D_0015ED6C * 12.566370614f);
        float wave;
        data->f1C = phase;
        wave = func_001F9FA8_2D8388(phase);
        MOBY(moby)->glow_color =
            func_001FA8A8_2D8388(D_L05_001613D8, D_L05_001613DC, (wave + 1.0f) * 0.5f);
        advance_scenery_ship_along_path(moby, (int *)l05_paths[data->path]);
        if (func_L00_0028EB98_2D8388(moby, data->f328) == 0) {
            data->f328 = func_0022ED80_2D8388(1, 4, (int)moby);
        }
        if ((frame_number & 3) == 0) {
            data->f322 = (data->f322 + 1) & 0xF;
            if ((float)data->f320 < D_L05_001613E0) {
                data->f320++;
            }
        }
        left_offset.q = 0;
        left_offset.f[0] = -1.2f;
        left_offset.f[1] = 0.4f;
        left_offset.f[2] = 0.45f;
        transform_vector_by_basis(&left_offset, &left_offset, moby + 0xC0);
        func_001F9BD8_2D8388(
            (float *)&data->trail[0][(short)data->f322], left_offset.f, (float *)&MOBY(moby)->pos);
        data->trail[0][(short)data->f322].w = 1.0f;
        tail_offset.q = 0;
        tail_offset.f[0] = -1.5f;
        tail_offset.f[2] = -0.3f;
        transform_vector_by_basis(&tail_offset, &tail_offset, moby + 0xC0);
        func_001F9BD8_2D8388(
            (float *)&data->trail[1][(short)data->f322], tail_offset.f, (float *)&MOBY(moby)->pos);
        data->trail[1][(short)data->f322].w = 1.0f;
        right_offset.q = 0;
        right_offset.f[0] = -1.2f;
        right_offset.f[1] = -0.4f;
        right_offset.f[2] = 0.45f;
        transform_vector_by_basis(&right_offset, &right_offset, moby + 0xC0);
        func_001F9BD8_2D8388(
            (float *)&data->trail[2][(short)data->f322], right_offset.f, (float *)&MOBY(moby)->pos);
        data->trail[2][(short)data->f322].w = 1.0f;
        if (frame_number != *data->f324) {
            *data->f324 = frame_number;
            func_001F49B0_2D8388(func_L05_002D8B68_2D8388, moby);
        }
        if (MOBY(moby)->pos.x < 8.0f) {
            MOBY(moby)->collision = 0;
        } else if (MOBY(moby)->pos.y < 8.0f) {
            MOBY(moby)->collision = 0;
        } else {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        }
        if (hit != 0 && 0.0f < *(float *)(hit + 0x2C))
            damaged = 1;
        if (damaged)
            MOBY(moby)->state = 2;
        break;
    }
    case SCENERY_SHIP_SPAWN_EFFECTS_AND_REMOVE: {
        func_0022ED80_2D8388(0, 0, (int)moby);
        tab = (float *)D_L05_0015F580_macro;
        effect_origin_position = (float *)&MOBY(moby)->pos;
        effect_origin_rotation = (float *)&MOBY(moby)->rot;
        fast_vec_scale(
            &left_offset, moby + 0xC0, scenery_ship_removal_effect_basis_scale_gp * frame_scale);
        left_offset.f[2] = left_offset.f[2] + scenery_ship_removal_effect_z_addend_gp * frame_scale;
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby,
                                 0x72E,
                                 effect_origin_position,
                                 effect_origin_rotation,
                                 lifetime,
                                 0,
                                 left_offset.f,
                                 tab,
                                 D_0015ED70 * 12.0f,
                                 tab);
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby,
                                 0x72E,
                                 effect_origin_position,
                                 effect_origin_rotation,
                                 lifetime,
                                 0,
                                 left_offset.f,
                                 tab,
                                 D_0015ED70 * 12.0f,
                                 tab);
        lifetime = func_001F9850_2D8388(0x5A);
        func_L00_00265050_2D8388(moby,
                                 0x72E,
                                 effect_origin_position,
                                 effect_origin_rotation,
                                 lifetime,
                                 0,
                                 left_offset.f,
                                 tab,
                                 D_0015ED70 * 12.0f,
                                 tab);
        func_0020D678_2D8388(moby);
        return;
    }
    }
    if (D_L05_001613E4)
        update_moby_glow_and_queue_draw(moby);
}

/* Retail quad packet: vertices +0x00, colors +0x40, UVs +0x50, commands +0x70. */
typedef struct {
    char  vertices[0x40];
    int   color[4];
    float uv[4][2];
    u64   zero;
    u64   effect_texture;
    u64   texture;
    u64   flags;
} L05GlowPacket;

int                  D_L05_001613F0 = 0;
int                  D_L05_001613F4 = 0;
int                  D_L05_001613F8 = 0;
int                  D_L05_001613FC = 2;
int                  D_L05_00161400 = 0;
int                  D_L05_00161404 = 1;
int                  D_L05_00161408 = 0x40806060;
int                  D_L05_0016140C = 0x802020;
extern unsigned char l05_scenery_ship_extended_trail_selector __asm__("D_L05_0015EDB2");
extern struct Moby  *D_L05_0015FFD8_7920 __asm__("D_L05_0015FFD8");
extern short        *D_L05_001ABCC0_7920[] __asm__("D_L05_001ABCC0");

float D_L05_001CDD70[4][2] = {
    {0.0f, 0.5f},
    {1.0f, 0.5f},
    {0.0f, 0.5f},
    {1.0f, 0.5f},
};

extern u64  get_effect_texture(int) __asm__("FUN_001f44b8");
extern int  interpolate_color_7920(float, int, int) __asm__("FUN_001fa6e0");
extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
extern void memset(void *, int, int) __asm__("FUN_001153fc");

/* Builds 16 fading glow quads, then draws three ribbons of quads for each linked moby of class 0x4F in view. */
void draw_group_scenery_ship_trails(struct Moby *moby) {
    L05GlowPacket  packet[16];
    float          matrix[8];
    float          center[4];
    int            i;
    int            j;
    short         *list;
    struct Moby   *member;
    char          *data;
    float          phase;
    float          ratio;
    u64            texture;
    L05GlowPacket *glow_packet;

    if (D_L05_001613F0 != 0)
        return;
    if (l05_scenery_ship_extended_trail_selector != 0)
        D_L05_001613E0 = 16.0f;
    else
        D_L05_001613E0 = 5.0f;
    for (i = 0; i < 16; i++) {
        texture = get_effect_texture(0x13);
        glow_packet = &packet[i];
        glow_packet->effect_texture = texture;
        glow_packet->texture = 0xff9000000260ULL;
        glow_packet->zero = 0;
        glow_packet->flags = (u64)D_L05_001613F8 | ((u64)D_L05_001613FC << 2) |
                             ((u64)D_L05_00161400 << 4) | ((u64)D_L05_00161404 << 6) |
                             0x8000000000ULL;
        phase = convert_integer_to_float((frame_number + 3) & 3) / (D_L05_001613E0 * 4.0f);
        for (j = 0; j < 4; j++) {
            packet[i].uv[j][0] = D_L05_001CDD70[j][0];
            packet[i].uv[j][1] = D_L05_001CDD70[j][1];
            ratio = convert_integer_to_float(i + 1 - (j >> 1)) / (D_L05_001613E0 + phase);
            if (ratio > 1.0f)
                ratio = 1.0f;
            else if (ratio < 0.0f)
                ratio = 0.0f;
            packet[i].color[j] = interpolate_color_7920(ratio, D_L05_00161408, D_L05_0016140C);
        }
    }
    memset(matrix, 0, 0x20);
    matrix[2] = 0.15f;
    matrix[6] = -0.15f;
    list = D_L05_001ABCC0_7920[(unsigned char)((char)moby->group)];
    do {
        member = (struct Moby *)(((*list & 0x7fff) << 8) + (int)D_L05_0015FFD8_7920);
        if (member->oclass == 0x4f && member->state == 1) {
            data = (char *)member->pvars;
            qcopy_nc(center, &member->pos);
            center[3] = 20.0f;
            if (fast_bsphere_check(center, 512.0f) != -1) {
                int   first;
                int   ribbon;
                int   k;
                char *points;
                int   index;
                int   frame;
                for (frame = 0; frame < *(short *)(data + 0x320); frame++) {
                    first = (*(short *)(data + 0x322) - frame + 15) & 15;
                    for (ribbon = 0; ribbon < 3; ribbon++) {
                        if (D_L05_001613F4 == 0) {
                            points = data + (ribbon * 0x100 + 0x20);
                            for (k = 0; k < 4; k++) {
                                index = (first + (k >> 1)) & 15;
                                fast_vec_add(packet[frame].vertices + k * 0x10,
                                             points + (index << 4),
                                             (char *)matrix + (k & 1) * 0x10);
                            }
                            draw_geometry_quad(&packet[frame], 0, 0);
                        }
                    }
                }
            }
        }
    } while (*list++ >= 0);
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DBF10), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern int   FUN_L00_00258b50(void *, void *, void *, void *, float);
extern void  FUN_L01_0026d930(char *p);
extern void  handle_amoeboid_hits(unsigned char *m) __asm__("FUN_L05_002daf58");
extern void  spawn_amoeboid_particles(unsigned char *moby) __asm__("FUN_L05_002db018");
extern void  blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void amoeboid_update(struct Moby *m) __asm__("FUN_L05_002dac80");

void amoeboid_update(struct Moby *moby) {
    enum {
        AMOEBOID_INITIALIZE = 0,
        AMOEBOID_FOLLOW_PATH = 1,
        AMOEBOID_WAIT_FOR_HERO_TO_LEAVE = 2
    };
    char *moby_data = (char *)moby->pvars;
    float facing_direction[4];
    handle_amoeboid_hits(moby);
    switch (moby->state) {
    case AMOEBOID_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0x40;
        if (moby->prev_seq != 0)
            blend_moby_animation_c(moby, 0, 0, 0);
        if (*(int *)(moby_data + 0xB0) == -1 ||
            *(int *)l05_paths[*(int *)(moby_data + 0xB0)] == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        FUN_L01_0026d930(moby_data + 0x60);
        *(float *)(moby_data + 0x6C) = 1.0f;
        *(float *)(moby_data + 0x68) = 1.0f;
        {
            int   r = truncate_float_to_s32(409.6f);
            float movement_speed_limit = frame_time * 4.0f;
            *(int *)(moby_data + 0x60) = r;
            *(float *)(moby_data + 0x84) = movement_speed_limit;
        }
        break;
    case AMOEBOID_FOLLOW_PATH: {
        char *path_node_base =
            ((char *)l05_paths[*(int *)(moby_data + 0xB0)]) + *(int *)(moby_data + 0xB4) * 16;
        float yaw = fast_arc_tan(*(float *)(path_node_base + 0x10) - moby->pos.x,
                                 *(float *)(path_node_base + 0x14) - moby->pos.y);
        FUN_L00_0025be00(&moby->rot.z,
                         (float *)(moby_data + 0xBC),
                         yaw,
                         frame_time_sq * 1.0471976f,
                         frame_time_sq * 1.0471976f,
                         frame_time * 3.1415927f);
        facing_direction[0] = fast_cos(moby->rot.z);
        facing_direction[1] = fast_sin(moby->rot.z);
        facing_direction[2] = 0.0f;
        FUN_L00_00258b50(moby, moby_data + 0x60, facing_direction, moby_data + 0x40, 1.0f);
        if (fast_dec_timer(moby_data + 0xB8) != 0 ||
            vector_distance_xy(&moby->pos,
                               ((char *)l05_paths[*(int *)(moby_data + 0xB0)]) +
                                   (*(int *)(moby_data + 0xB4) * 16 + 0x10)) < 1.0f) {
            int r = random_integer_below(*(int *)l05_paths[*(int *)(moby_data + 0xB0)]);
            *(int *)(moby_data + 0xB4) =
                (*(int *)(moby_data + 0xB4) + r) % *(int *)l05_paths[*(int *)(moby_data + 0xB0)];
            *(int *)(moby_data + 0xB8) = scale_game_frames(600);
        }
        spawn_amoeboid_particles(moby);
        break;
    }
    case AMOEBOID_WAIT_FOR_HERO_TO_LEAVE:
        if (48.0f < vector_distance_xy(&moby->pos, &hero.motion.pos)) {
            moby->state = 1;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
            moby->collision = moby->pclass->collision;
        }
        break;
    }
}
/* Emits up to two particle effects around the moby, each in a random direction. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC2A8), where it is exact; names translated to the US level program. */

extern void
spawn_type_52_particle(float *, float, float, int, int, int) __asm__("FUN_L01_00287158");

void spawn_amoeboid_particles(unsigned char *moby) {
    float particle_spawn_position[3];
    if (moby[0x31] != 0) {
        if (random_integer_below(0x13) == 0) {
            float a = random_angle_radians();
            float z = 0.0f;
            float b = random_float_between(z, 0.1f);
            float c = random_float_between(z, 0.5f);
            float e = random_float_between(0.5f, 1.0f);
            int   s = truncate_float_to_s32(random_float_between(120.0f, 180.0f));
            particle_spawn_position[0] = fast_cos(a) * b;
            particle_spawn_position[1] = fast_sin(a) * b;
            particle_spawn_position[2] = z;
            fast_vec_add(particle_spawn_position, particle_spawn_position, moby + 0x10);
            spawn_type_52_particle(
                particle_spawn_position, c, e, 0x30002028, 0x2020, scale_game_frames(s));
        }
        if (random_integer_below(4) == 0) {
            float a = random_angle_radians();
            float z = 0.0f;
            float b = random_float_between(z, 0.1f);
            float c = random_float_between(z, 1.75f);
            float e = random_float_between(1.75f, 2.25f);
            int   s = truncate_float_to_s32(random_float_between(120.0f, 180.0f));
            particle_spawn_position[0] = fast_cos(a) * b;
            particle_spawn_position[1] = fast_sin(a) * b;
            particle_spawn_position[2] = z;
            fast_vec_add(particle_spawn_position, particle_spawn_position, moby + 0x10);
            spawn_type_52_particle(
                particle_spawn_position, c, e, 0x20002020, 0x2030, scale_game_frames(s));
        }
    }
}
/* Explosion debris: 20 bursts thrown out along dir with random spread, each trailing 10 smoke puffs. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC4C8), where it is exact; names translated to the US level program. */

extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");

void spawn_amoeboid_hit_sparks(void *moby, void *base_spark_vector) {
    float spark_position[4];
    float first_spark_vector[4];
    float second_spark_vector[4];
    char *moby_position_ptr = (char *)moby + 0x10;
    int   i, j;
    for (i = 0; i < 20; i++) {
        float spark_xy_spread_heading, sp, up;
        qcopy(spark_position, moby_position_ptr);
        add_random_offset_to_vector_xyz(spark_position, 0.25f);
        spark_position[2] += 1.0f;
        spark_xy_spread_heading = random_angle_radians();
        sp = random_float_between(1.0f, 8.0f);
        up = random_float_between(2.0f, 6.0f);
        fast_vec_scale(first_spark_vector, base_spark_vector, random_float_between(0.0f, 1.0f));
        first_spark_vector[0] += fast_cos(spark_xy_spread_heading) * (sp * frame_time);
        first_spark_vector[1] += fast_sin(spark_xy_spread_heading) * (sp * frame_time);
        first_spark_vector[2] += up * frame_time;
        qcopy(second_spark_vector, first_spark_vector);
        second_spark_vector[2] -= frame_time_sq * 20.0f * (float)scale_game_frames(0x1E);
        for (j = 0; j < 10; j++) {
            float spark_vector_w_scale = random_float_between(0.5f, 1.5f);
            int first_spark_color, second_spark_color, spark_frame_count, second_spark_frame_count;
            first_spark_vector[3] = spark_vector_w_scale * 0.125f;
            second_spark_vector[3] = spark_vector_w_scale * 0.065f;
            add_random_offset_to_vector_xyz(spark_position, 0.2f);
            add_random_offset_to_vector_xyz(second_spark_vector, frame_time * 0.5f);
            first_spark_color =
                fast_tween_color(0x8000EEEE, 0x8000FF90, random_float_between(0.0f, 1.0f));
            second_spark_color = fast_tween_color(0xFFEE, 0xFFEE, random_float_between(0.0f, 1.0f));
            spark_frame_count = scale_game_frames(10);
            second_spark_frame_count = scale_game_frames(0x1E);
            spawn_spark(spark_position,
                        first_spark_vector,
                        second_spark_vector,
                        first_spark_color,
                        second_spark_color,
                        spark_frame_count,
                        second_spark_frame_count,
                        truncate_float_to_s32(scale_time(random_float_between(5.0f, 25.0f))),
                        -1);
        }
    }
}
/* Extermibot update: wakes from its dormant pose, walks its path, closes in on the target and strikes it, bursts into sparks between strikes, and dies on a hit. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003033A0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

extern char  D_L05_0015F580[] MACRO_ADDR;
extern char  D_L05_001671C0[];
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float FUN_L04_001f1978(float);
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_dist(void *, void *);
extern int   D_L05_0015FFD8;
extern int   D_L05_0015F5C4;
extern int   mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int   FUN_L00_00261b48(int, void *, void *, void *);
extern int   is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern s32   follow_path(void *, s32, s32, void *, s32, void *, f32) __asm__("FUN_L01_00276fe8");
extern s32
spawn_spark_c(void *, void *, void *, s32, s32, s32, s32, s32, s32) __asm__("func_00218888");
extern int   D_L05_00161A84 __attribute__((sda));
extern float D_L05_00161A98 __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/extermibot_update_2.h"

extern float extermibot_spark_paired_frame_count_min_gp __asm__("D_L05_00161AB0")
    __attribute__((sda));
extern float D_L05_00161AB4 __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/extermibot_update.h"

extern void FUN_L00_0024f7c8(void *, int, void *);
extern void FUN_L00_00257d78(float *, float, float);
extern void
spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern void FUN_L00_0025a120(void *);
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void
resolve_point_path_segment_xy_clearance(int, void *, void *, float) __asm__("FUN_L00_00261d78");
extern void
FUN_L00_00263fd8_c(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern void  mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
f32          compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
void         FUN_L05_00303390_c(Moby *m) __asm__("FUN_L05_00303390");
void         FUN_L05_00303b08_c(struct Moby *m, s32 node, f32 side) __asm__("FUN_L05_00303b08");
extern float D_0013F3D0[];
extern char  extermibot_strike_flags_selector[] __asm__("D_001404A8");

void extermibot_update(unsigned char *m) __asm__("FUN_L05_00301f48");

void extermibot_update(unsigned char *moby) {
    enum {
        EXTERMIBOT_INITIALIZE = 0,
        EXTERMIBOT_IDLE_AND_CHECK_TARGET_PATH = 1,
        EXTERMIBOT_WANDER_NEAR_START = 2,
        EXTERMIBOT_FOLLOW_TARGET = 4,
        EXTERMIBOT_WAIT_FOR_TARGET_IN_STRIKE_RANGE = 5,
        EXTERMIBOT_APPROACH_TARGET_DIRECTLY = 6,
        EXTERMIBOT_AIM_BEFORE_STRIKE = 7,
        EXTERMIBOT_STRIKE_TARGET = 8,
        EXTERMIBOT_RECOVER_AFTER_STRIKE = 9,
        EXTERMIBOT_RETURN_TO_START = 10,
        EXTERMIBOT_NONLETHAL_KNOCKBACK = 11,
        EXTERMIBOT_WAIT_FOR_ANIMATION_BEFORE_IDLE = 13,
        EXTERMIBOT_DEATH_KNOCKBACK = 14,
        EXTERMIBOT_WAIT_FOR_ACTIVATION_TRIGGER = 15
    };
    char          *moby_data = ((char *)MOBY(moby)->pvars);
    char          *target_moby;
    float          path_goal_or_facing_vector[4];
    float          out[4];
    float          hit[4];
    float          spark_spawn_position[4];
    int            i;
    int            j;
    float         *moby_yaw_ptr;
    float          yaw;
    unsigned char *moby_position;
    extern float   fast_vec_dist(void *, void *);

    manipulator_set_scale(moby_data + 0x170, 2.5f);
    if (D_L05_0015F5C4 == 2 && ((MOBY(moby)->flags ^ 1) & 1)) {
        *(int *)(moby_data + 0x2DC) = 1;
        MOBY(moby)->flags |= 0x41;
    } else if (*(int *)(moby_data + 0x2DC) != 0 && D_L05_0015F5C4 == 0) {
        *(int *)(moby_data + 0x2DC) = 0;
        MOBY(moby)->flags &= 0xFFBE;
    }
    if (moby[0x31] != 0 &&
        fast_vec_dist((float *)&MOBY(moby)->pos, (float *)D_L05_001671C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7F] = 0x16;
    }
    FUN_L05_00303390_c((Moby *)moby);
    target_moby = *(char **)(moby_data + 0x160);
    MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * (D_L05_00161A98);
    switch (moby[0x20]) {
    case EXTERMIBOT_INITIALIZE:
        MOBY(moby)->flags |= 0x1000;
        qcopy(moby_data + 0x270, moby + 0x10);
        *(float *)(moby_data + 0x20) = 2.0f;
        moby_data[0x29] = 0;
        *(int *)(moby_data + 0x2B0) = 0;
        moby_data[0x28] = 1;
        *(float *)(moby_data + 0x30) = 1.0f;
        *(short *)(moby_data + 0x24) = 2;
        moby_data[0x58] = truncate_float_to_s32(14.0f);
        moby_data[0x5A] = truncate_float_to_s32(11.2f);
        FUN_L01_0026d930(moby_data + 0xD0);
        {
            float movement_scale = D_L05_00161A98;
            *(float *)(moby_data + 0xDC) = 2.0f;
            *(float *)(moby_data + 0xD8) = 2.0f;
            {
                int   quantized_movement_scale = truncate_float_to_s32(movement_scale * 409.6f);
                float initial_movement_speed = D_0015ED6C * 6.0f;
                *(int *)(moby_data + 0xD0) = quantized_movement_scale;
                *(float *)(moby_data + 0xF4) = initial_movement_speed;
            }
        }
        *(float *)(moby_data + 0x2D8) = random_angle_radians();
        if (random_integer_below(2) != 0) {
            MOBY(moby)->flags |= 0x8000;
        } else {
            MOBY(moby)->flags &= 0x7FFF;
        }
        if (*(int *)(moby_data + 0x2B4) != 0) {
            moby[0x20] = 0xC;
            moby[0x31] = 0;
            MOBY(moby)->collision = 0;
            MOBY(moby)->flags &= 0xEFFF;
            MOBY(moby)->pos.z -= 20.0f;
            MOBY(moby)->draw_dist = 0;
        } else if (*(long *)(moby_data + 0x2B8) != -1) {
            MOBY(moby)->flags &= 0xEFFF;
            moby[0x20] = 0xF;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, 0);
        } else {
            moby[0x20] = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, 0);
        }
        build_symmetric_adjacency_masks((int *)(moby_data + 0x2C0), 1, *(int *)(moby_data + 0x2C4));
        break;
    case EXTERMIBOT_IDLE_AND_CHECK_TARGET_PATH:
        if (*(int *)(moby_data + 0x164) != 2 && random_integer_below(9) == 0) {
            if (follow_path(moby_data + 0x2C0,
                            1,
                            *(int *)(moby_data + 0x2C4),
                            moby + 0x10,
                            (s32)(target_moby + 0x10),
                            path_goal_or_facing_vector,
                            (D_L05_00161A98) * 0.4f) != 0) {
                if (random_integer_below(0x100) & 1) {
                    *(float *)(moby_data + 0x298) = -*(float *)(moby_data + 0x298);
                }
                *(float *)(moby_data + 0x294) = *(float *)(moby_data + 0x298) * 0.017453292f;
                *(int *)(moby_data + 0x2AC) =
                    scale_game_frames((int)random_float_between(30.0f, 90.0f));
                moby[0x20] = 4;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            } else {
                *(int *)(moby_data + 0x164) = 2;
            }
        }
        if (*(int *)(moby_data + 0x164) == 2 && *(int *)(moby_data + 0x2B0) != 0) {
            moby[0x20] = 6;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        } else if (moby[0x70] & 2) {
            if (moby[0x31] != 0 && (random_integer_below(0xFF) & 1)) {
                FUN_L00_00257d78((float *)(moby_data + 0x280), 0.0f, 4.0f);
                *(float *)(moby_data + 0x288) = 0.0f;
                fast_vec_add(moby_data + 0x280, moby_data + 0x280, moby_data + 0x270);
                *(short *)(moby_data + 0x2D2) = 0;
                moby[0xBC] = moby[0x20];
                moby[0x20] = 2;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            } else if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case EXTERMIBOT_WANDER_NEAR_START:
        moby_yaw_ptr = (float *)&MOBY(moby)->rot.z;
        moby_position = moby + 0x10;
        yaw = fast_arc_tan(*(float *)(moby_data + 0x280) - MOBY(moby)->pos.x,
                           *(float *)(moby_data + 0x284) - MOBY(moby)->pos.y);
        FUN_L00_0025be00(moby_yaw_ptr,
                         (float *)(moby_data + 0x290),
                         yaw,
                         D_0015ED70 * 6.2831855f,
                         D_0015ED70 * 6.2831855f,
                         D_0015ED6C * 6.2831855f);
        if (fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(*(float *)(moby_data + 0x280) - MOBY(moby)->pos.x,
                             *(float *)(moby_data + 0x284) - MOBY(moby)->pos.y)) < 1.5707964f) {
            path_goal_or_facing_vector[0] = fast_cos(MOBY(moby)->rot.z) * 2.0f;
            path_goal_or_facing_vector[1] = fast_sin(MOBY(moby)->rot.z) * 2.0f;
            path_goal_or_facing_vector[2] = 0.0f;
            FUN_L00_00258b50(moby, moby_data + 0xD0, path_goal_or_facing_vector, out, 1.0f);
            resolve_point_path_segment_xy_clearance(
                *(int *)(moby_data + 0x2C0), moby_position, moby_position, (D_L05_00161A98) * 0.4f);
        }
        (*(short *)(moby_data + 0x2D2))++;
        if ((vector_distance_xy(moby_position, moby_data + 0x280) < 1.0f &&
             *(short *)(moby_data + 0x2D2) > scale_game_frames(0x3C)) ||
            *(short *)(moby_data + 0x2D2) > scale_game_frames(0xB4)) {
            moby[0x20] = moby[0xBC];
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
        }
        break;
    case EXTERMIBOT_APPROACH_TARGET_DIRECTLY: {
        int near;
        moby_yaw_ptr = (float *)&MOBY(moby)->rot.z;
        moby_position = moby + 0x10;
        approach_value((float *)(moby_data + 0xF4), D_0015ED6C * 6.0f, D_0015ED70 * 6.0f);
        yaw = fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                           MOBY(target_moby)->pos.y - MOBY(moby)->pos.y);
        FUN_L00_0025be00(moby_yaw_ptr,
                         (float *)(moby_data + 0x290),
                         yaw,
                         D_0015ED70 * 6.2831855f,
                         D_0015ED70 * 6.2831855f,
                         D_0015ED6C * 6.2831855f);
        if (fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                             MOBY(target_moby)->pos.y - MOBY(moby)->pos.y)) < 1.5707964f) {
            path_goal_or_facing_vector[0] = fast_cos(MOBY(moby)->rot.z) * 2.0f;
            path_goal_or_facing_vector[1] = fast_sin(MOBY(moby)->rot.z) * 2.0f;
            path_goal_or_facing_vector[2] = 0.0f;
            FUN_L00_00258b50(moby, moby_data + 0xD0, path_goal_or_facing_vector, out, 1.0f);
            resolve_point_path_segment_xy_clearance(
                *(int *)(moby_data + 0x2C0), moby_position, moby_position, (D_L05_00161A98) * 0.4f);
        }
        near = 0;
        if (FUN_L00_00261b48(*(int *)(moby_data + 0x2C0), moby_position, target_moby + 0x10, hit) !=
                0 &&
            fast_vec_dist((float *)moby_position, hit) < 1.5f) {
            near = 1;
        }
        if (near) {
            moby[0x20] = 5;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(20));
        } else if (*(int *)(moby_data + 0x2B0) != 0) {
            if (vector_distance_xy(moby_position, target_moby + 0x10) < 3.0f) {
                moby[0x20] = 7;
                *(short *)(moby_data + 0x2D0) =
                    scale_game_frames(extermibot_pre_strike_delay_frames_gp);
                if (moby[0x53] != 2)
                    blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        } else {
            moby[0x20] = 10;
        }
        break;
    }
    case EXTERMIBOT_WAIT_FOR_TARGET_IN_STRIKE_RANGE:
        if (*(int *)(moby_data + 0x2B0) == 0) {
            *(float *)(moby_data + 0xF4) = 0.0f;
            moby[0x20] = 10;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
        } else if (vector_distance_xy(moby + 0x10, target_moby + 0x10) < 3.0f) {
            moby[0x20] = 7;
            *(short *)(moby_data + 0x2D0) =
                scale_game_frames(extermibot_pre_strike_delay_frames_gp);
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        break;
    case EXTERMIBOT_WAIT_FOR_ACTIVATION_TRIGGER:
        if ((*(int *)(moby_data + 0x2B8) != -1 &&
             ((unsigned char *)*(int *)&D_L05_0015FFD8)[*(int *)(moby_data + 0x2B8) * 256 + 0xBC] !=
                 0) ||
            (*(int *)(moby_data + 0x2BC) != -1 &&
             is_point_inside_clip_volume(((char *)D_0013F3D0), *(int *)(moby_data + 0x2BC)) != 0)) {
            MOBY(moby)->draw_dist = 0x40;
            MOBY(moby)->flags |= 0x1000;
            moby[0x20] = 1;
        } else if (moby[0x70] & 2) {
            if (random_integer_below(0xFF) & 1) {
                FUN_L00_00257d78((float *)(moby_data + 0x280), 0.0f, 4.0f);
                *(float *)(moby_data + 0x288) = 0.0f;
                fast_vec_add(moby_data + 0x280, moby_data + 0x280, moby_data + 0x270);
                *(short *)(moby_data + 0x2D2) = scale_game_frames(0x5A);
                moby[0xBC] = moby[0x20];
                moby[0x20] = 2;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            } else if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case EXTERMIBOT_FOLLOW_TARGET: {
        float target_distance_xy = vector_distance_xy(moby + 0x10, target_moby + 0x10);
        FUN_L05_00303b08_c(
            (struct Moby *)moby, (s32)(target_moby + 0x10), *(float *)(moby_data + 0x294));
        if (target_distance_xy < 3.0f &&
            fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                             MOBY(target_moby)->pos.y - MOBY(moby)->pos.y)) < 0.7853982f) {
            moby[0x20] = 7;
            *(short *)(moby_data + 0x2D0) =
                scale_game_frames(extermibot_pre_strike_delay_frames_gp);
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        if (*(int *)(moby_data + 0x164) == 2) {
            moby[0x20] = 10;
        }
        break;
    }
    case EXTERMIBOT_AIM_BEFORE_STRIKE:
        moby_yaw_ptr = (float *)&MOBY(moby)->rot.z;
        yaw = fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                           MOBY(target_moby)->pos.y - MOBY(moby)->pos.y);
        FUN_L00_0025be00(moby_yaw_ptr,
                         (float *)(moby_data + 0x290),
                         yaw,
                         D_0015ED70 * 12.566371f,
                         D_0015ED70 * 12.566371f,
                         D_0015ED6C * 25.132742f);
        if ((moby[0x70] & 2) && moby[0x53] == 2) {
            blend_moby_animation(moby, 7, 0, scale_game_frames(5));
        }
        if (fast_dec_timer_s16(moby_data + 0x2D0) != 0) {
            float a;
            moby[0x20] = 8;
            *(short *)(moby_data + 0x2D0) = 0;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(1));
            a = fast_add_rotations(MOBY(moby)->rot.z, 0.0f);
            path_goal_or_facing_vector[0] =
                fast_cos(a) * (extermibot_spark_facing_xy_scale_gp * D_0015ED6C);
            path_goal_or_facing_vector[1] =
                fast_sin(a) * (extermibot_spark_facing_xy_scale_gp * D_0015ED6C);
            path_goal_or_facing_vector[2] = 0.0f;
            path_goal_or_facing_vector[3] = extermibot_spark_facing_vector_w_gp;
            out[2] = 0.0f;
            out[3] = extermibot_spark_random_vector_w_gp;
            FUN_L00_0024f7c8(moby, 0, hit);
            for (i = 0x1D; i >= 0; i--) {
                int c1, c2;
                qcopy(spark_spawn_position, hit);
                spark_spawn_position[0] += random_float_between(-0.065f, 0.065f);
                spark_spawn_position[1] += random_float_between(-0.065f, 0.065f);
                spark_spawn_position[2] += random_float_between(-0.065f, 0.065f);
                out[0] = random_float_between(-D_0015ED6C, D_0015ED6C);
                out[1] = random_float_between(-D_0015ED6C, D_0015ED6C);
                out[2] = random_float_between(-(extermibot_spark_random_z_scale_gp * D_0015ED6C),
                                              extermibot_spark_random_z_scale_gp * D_0015ED6C);
                c1 = truncate_float_to_s32(FUN_L04_001f1978(
                    random_float_between(extermibot_spark_lifetime_min_frames_gp,
                                         extermibot_spark_lifetime_min_frames_gp * 2.0f)));
                c2 = truncate_float_to_s32(FUN_L04_001f1978(
                    random_float_between(extermibot_spark_paired_frame_count_min_gp,
                                         extermibot_spark_paired_frame_count_min_gp * 1.2f)));
                spawn_spark_c(spark_spawn_position,
                              path_goal_or_facing_vector,
                              out,
                              extermibot_primary_spark_initial_color_gp,
                              extermibot_primary_spark_paired_color_gp,
                              c1,
                              c2,
                              truncate_float_to_s32(FUN_L04_001f1978(
                                  random_float_between(D_L05_00161AB4, D_L05_00161AB4 * 1.2f))),
                              -1);
            }
            path_goal_or_facing_vector[3] = extermibot_spark_facing_vector_w_gp * 0.1f;
            out[3] = extermibot_spark_random_vector_w_gp * 0.1f;
            for (j = 0x1D; j >= 0; j--) {
                int c1, c2;
                qcopy(spark_spawn_position, hit);
                spark_spawn_position[0] += random_float_between(-0.065f, 0.065f);
                spark_spawn_position[1] += random_float_between(-0.065f, 0.065f);
                spark_spawn_position[2] += random_float_between(-0.065f, 0.065f);
                out[0] = random_float_between(-D_0015ED6C, D_0015ED6C);
                out[1] = random_float_between(-D_0015ED6C, D_0015ED6C);
                out[2] = random_float_between(-(extermibot_spark_random_z_scale_gp * D_0015ED6C),
                                              extermibot_spark_random_z_scale_gp * D_0015ED6C);
                c1 = truncate_float_to_s32(FUN_L04_001f1978(
                    random_float_between(extermibot_spark_lifetime_min_frames_gp,
                                         extermibot_spark_lifetime_min_frames_gp * 2.0f)));
                c2 = truncate_float_to_s32(FUN_L04_001f1978(
                    random_float_between(extermibot_spark_paired_frame_count_min_gp,
                                         extermibot_spark_paired_frame_count_min_gp * 1.2f)));
                spawn_spark_c(spark_spawn_position,
                              path_goal_or_facing_vector,
                              out,
                              extermibot_secondary_spark_initial_color_gp,
                              extermibot_secondary_spark_paired_color_gp,
                              c1,
                              c2,
                              truncate_float_to_s32(FUN_L04_001f1978(
                                  random_float_between(D_L05_00161AB4, D_L05_00161AB4 * 1.2f))),
                              -1);
            }
        }
        break;
    case EXTERMIBOT_STRIKE_TARGET: {
        int   fl;
        float a = fast_add_rotations(MOBY(moby)->rot.z, 0.0f);
        if (compute_interpolated_record_value(moby) < 1.5f &&
            fast_difference_between_rotations(
                a,
                fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                             MOBY(target_moby)->pos.y - MOBY(moby)->pos.y)) < 0.2617994f &&
            vector_distance_xy(moby + 0x10, target_moby + 0x10) < 3.0f &&
            absolute_float(MOBY(target_moby)->pos.z - MOBY(moby)->pos.z) < 2.0f) {
            path_goal_or_facing_vector[0] = fast_cos(MOBY(moby)->rot.z);
            path_goal_or_facing_vector[1] = fast_sin(MOBY(moby)->rot.z);
            path_goal_or_facing_vector[2] = 0.0f;
            path_goal_or_facing_vector[3] = 5627.9248f;
            qcopy(out, target_moby + 0x10);
            out[2] += 0.75f;
            spawn_nearby_effect(
                target_moby,
                (int)moby,
                *(int *)(((char *)&extermibot_strike_flags_selector)) != 6 ? 0x10001 : 0x10000,
                out,
                path_goal_or_facing_vector,
                1.0f);
        }
        if (moby[0x70] & 2) {
            moby[0x20] = 9;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    }
    case EXTERMIBOT_RECOVER_AFTER_STRIKE:
        if (moby[0x70] & 2) {
            if (vector_distance_xy(moby + 0x10, target_moby + 0x10) < 3.0f &&
                fast_difference_between_rotations(
                    MOBY(moby)->rot.z,
                    fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                                 MOBY(target_moby)->pos.y - MOBY(moby)->pos.y)) < 0.2617994f) {
                moby[0x20] = 7;
                *(short *)(moby_data + 0x2D0) =
                    scale_game_frames(extermibot_pre_strike_delay_frames_gp);
                if (moby[0x53] != 7)
                    blend_moby_animation(moby, 7, 0, scale_game_frames(5));
            } else {
                *(float *)(moby_data + 0xF4) = 0.0f;
                moby[0x20] = 4;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case EXTERMIBOT_RETURN_TO_START:
        FUN_L05_00303b08_c((struct Moby *)moby, (s32)(moby_data + 0x270), 0.0f);
        if (vector_distance_xy(moby + 0x10, moby_data + 0x270) < 1.0f) {
            moby[0x20] = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(3));
        } else if (*(int *)(moby_data + 0x164) != 2) {
            moby[0x20] = 4;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(3));
        } else if (*(int *)(moby_data + 0x2B0) != 0) {
            moby[0x20] = 6;
        }
        break;
    case EXTERMIBOT_NONLETHAL_KNOCKBACK:
        if (mine_fall(moby, moby_data + 0x70) & 0x60) {
            if (follow_path(moby_data + 0x2C0,
                            1,
                            *(int *)(moby_data + 0x2C4),
                            moby + 0x10,
                            (s32)(moby_data + 0x270),
                            path_goal_or_facing_vector,
                            (D_L05_00161A98) * 0.4f) != 0) {
                moby[0x20] = 4;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                break;
            }
            cart_burst(moby, moby + 0x10, -1, 0.5f, 13.0f);
            mark_moby_for_removal_c2(moby);
            return;
        }
        if (MOBY(moby)->pos.z < 2.0f) {
            mark_moby_for_removal_c2(moby);
            return;
        }
        break;
    case EXTERMIBOT_WAIT_FOR_ANIMATION_BEFORE_IDLE:
        if (moby[0x70] & 2) {
            moby[0x20] = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    case EXTERMIBOT_DEATH_KNOCKBACK:
        if (mine_fall(moby, moby_data + 0x70) & 0x60) {
            cart_burst(moby, moby + 0x10, -1, 0.5f, 13.0f);
            FUN_L00_00263fd8_c(moby,
                               0x6DA,
                               moby + 0x10,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L05_0015F580,
                               D_L05_0015F580,
                               D_L05_0015F580);
            FUN_L00_00263fd8_c(moby,
                               0x6DB,
                               moby + 0x10,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L05_0015F580,
                               D_L05_0015F580,
                               D_L05_0015F580);
            FUN_L00_00263fd8_c(moby,
                               0x6DC,
                               moby + 0x10,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L05_0015F580,
                               D_L05_0015F580,
                               D_L05_0015F580);
            mark_moby_for_removal_c2(moby);
            return;
        }
        if (MOBY(moby)->pos.z < 0.0f) {
            mark_moby_for_removal_c2(moby);
            return;
        }
        break;
    }
    D_L05_00161A84 = *(volatile int *)0x10000000;
}

#include "rnc/math/vector.h"

/* Level 5 moby update (a flying enemy): takes hits through func_L00_0025B4D0 and reacts per hit kind, then
   follows its path, retargets the closest moby and turns its head toward the hero. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003047E8), where it is exact; names translated to the US level program. */

typedef int u128_3047E8 __attribute__((mode(TI)));

typedef float Vec_3047E8[4] __attribute__((aligned(16)));

typedef struct Hit_3047E8 {
    Vec_3047E8  at;
    u128_3047E8 dir;
    Moby       *who;
} Hit_3047E8;

typedef struct Path_3047E8 {
    int  n;
    char p0[0xC];
    int  pts[1];
} Path_3047E8;

typedef struct Wing {
    char          pad0[0x20];
    float         hp;
    char          pad1[0x14];
    int           f38;
    char          pad2[0x24];
    short         f60[3];
    char          pad3[0x1];
    unsigned char b67;
    char          pad4[0x8];
    char          f70[16];
    float         f80;
    char          pad5[0x4];
    float         f88;
    float         f8C;
    int           f90;
    int           f94;
    float         f98;
    char          pad6[0x11];
    unsigned char bAD;
    char          pad7[0x12];
    float         fC0;
    float         fC4;
    char          pad8[0x58];
    char          f120[8];
    float         f128;
    char          pad9[0x34];
    struct Moby  *target;
    int           f164;
    char          pad10[0x8];
    char          f170[16];
    char          pad11[0x54];
    float         f1D4;
    float         f1D8;
    char          pad12[0x14];
    char          f1F0[16];
    char          pad13[0x58];
    float         f258;
    char          pad14[0x14];
    char          f270[16];
    char          pad15[0x1C];
    float         f29C;
    float         f2A0;
    char          pad16[0xC];
    int           f2B0;
    char          pad17[0x14];
    int           f2C8;
    int           f2CC;
    char          pad18[0x4];
    int           f2D4;
    float         f2D8;
} Wing;

char        *FUN_L05_00303f80_c(char *self, int idx) __asm__("FUN_L05_00303f80");
extern float frame_scale_sq __asm__("D_0015ED64");
int          D_L05_00161A74 = 0x80004000;
int          D_L05_00161A78 = 0x80008000;

#include "rnc/overlay/l05/mobys/sharkigator_data/handle_extermibot_hits_and_update_targeting_2.h"

extern float D_L05_00161A98_gp __asm__("D_L05_00161A98") __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/handle_extermibot_hits_and_update_targeting.h"

extern int  collision_line() __asm__("FUN_001efa68");
extern int  fun_00213928() __asm__("FUN_00213928");
extern void FUN_L00_002628d8(char *, char *, int, float, float);
int         FUN_L00_0025ff38(float best,
                             char *a,
                             char *o,
                             int  *excluded,
                             int   excluded_count,
                             void *region,
                             int   region_count);
void        FUN_L00_00257470(unsigned char *m, int flags, int arg);
void trooper_knockback_arc(void *v, float *ang, float *s1, float *s2) __asm__("FUN_L00_0025ab48");
void FUN_L00_0025c558(u8 *a, void *b, int c, int d, int e, float f);
void FUN_L00_0025d458(void *a, void *s);
void FUN_L00_0025d538(s32 id, void *s);
extern Hit_3047E8 *func_L00_0025B478_3047E8(void *, int, int) __asm__("FUN_L00_0025a420");
extern int         collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int
update_collision_l08(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void trooper_knockback_arc(void *, float *, float *, float *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void  stop_animation_l08(void *, int, int) __asm__("FUN_L00_00257470");
extern void  trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, short *) __asm__("FUN_L00_0025d538");
extern float func_002140F8_3047E8(float, float) __asm__("FUN_002132a8");
extern int
activate_effect_l08(float, void *, void *, int, int, int *, int) __asm__("FUN_L00_0025ff38");
extern float func_001F9B88_3047E8(float) __asm__("FUN_001f99c0");
extern Moby *func_L05_003053D8_3047E8(void *, int) __asm__("FUN_L05_00303f80");
extern float func_001F9FA8_3047E8(float) __asm__("FUN_001f9de0");
extern void  func_001F9BF0_3047E8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  update_joint_animation(void *, void *, int, float, float) __asm__("FUN_L00_002628d8");
extern char  D_0013F420[];
extern s32   D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;

void handle_extermibot_hits_and_update_targeting(Moby *moby) __asm__("FUN_L05_00303390");

void handle_extermibot_hits_and_update_targeting(Moby *moby) {
    enum {
        EXTERMIBOT_HIT_NO_RESPONSE = 0,
        EXTERMIBOT_HIT_LETHAL = 1,
        EXTERMIBOT_HIT_DEATH_KNOCKBACK = 2,
        EXTERMIBOT_HIT_NONLETHAL_KNOCKBACK = 3,
        EXTERMIBOT_HIT_KNOCKBACK_AND_FLASH = 4,
        EXTERMIBOT_HIT_NONLETHAL_KNOCKBACK_AND_FLASH = 5,
        EXTERMIBOT_HIT_START_NONLETHAL_KNOCKBACK = 6,
        EXTERMIBOT_HIT_START_NONLETHAL_KNOCKBACK_AND_FLASH = 7,
        EXTERMIBOT_HIT_START_KNOCKBACK_WITH_FLASH = 8,
        EXTERMIBOT_HIT_FLASH = 9,
        EXTERMIBOT_HIT_FLASH_ONLY = 10,
        EXTERMIBOT_HIT_NO_REACTION = 11
    };
    Vec_3047E8  working_vector;
    Vec_3047E8  hit_test_endpoint;
    int         collision_hit_status;
    float       dmg;
    float       nonlethal_knockback_heading;
    float       death_knockback_heading;
    Wing       *moby_data;
    Hit_3047E8 *hit_record;
    int         hit_response;
    int         k;

    moby_data = *(struct Wing **)&moby->pvars;
    fast_dec_timer(&moby_data->f2D4);
    dmg = 0.0f;
    hit_record = func_L00_0025B478_3047E8(moby, 0x330000, 0);
    if (hit_record != 0) {
        qcopy(working_vector, ((float *)&moby->pos));
        working_vector[2] = working_vector[2] + 0.5f;
        if (hit_record->who != 0) {
            if (hit_record->who->oclass == 0x47) {
                qcopy(hit_test_endpoint, D_0013F420);
            } else if ((*(struct Moby **)&hit_record->who->pclass != 0 &&
                        *(short *)((u8 *)((struct Moby *)hit_record->who->pclass) + 0x46) == 5) ||
                       hit_record->who->oclass == 0xBA) {
                qcopy(hit_test_endpoint, working_vector);
            } else {
                qcopy(hit_test_endpoint, ((float *)&hit_record->who->pos));
            }
        } else {
            qcopy(hit_test_endpoint, hit_record->at);
        }
        if (fast_vec_dist(working_vector, hit_test_endpoint) < 24.0f &&
            collision_line(working_vector, hit_test_endpoint, 2, moby, 0) != 0) {
            hit_record = 0;
        }
    }
    hit_response = update_collision_l08(
        moby, hit_record, &moby_data->hp, 0, &collision_hit_status, &dmg, 0, 4);
    if (collision_hit_status != 1 && moby->state != 14) {
        moby_data->hp = moby_data->hp - dmg;
        if (moby_data->hp <= 0.0f) {
            hit_response = 1;
        }
        k = truncate_float_to_s32(D_L05_00161A98_gp * 409.6f);
        moby_data->f98 = D_L05_00161A98_gp * 0.4f;
        moby_data->f80 = extermibot_knockback_gravity_gp * D_0015ED70;
        moby_data->f90 = k;
        moby_data->f94 = 9;
        moby_data->bAD = 0;
        switch (hit_response) {
        case EXTERMIBOT_HIT_NO_RESPONSE:
        case EXTERMIBOT_HIT_NO_REACTION:
            break;
        case EXTERMIBOT_HIT_FLASH:
        case EXTERMIBOT_HIT_FLASH_ONLY:
            moby_data->b67 = 0xFA;
            break;
        case EXTERMIBOT_HIT_NONLETHAL_KNOCKBACK:
        case EXTERMIBOT_HIT_KNOCKBACK_AND_FLASH:
        case EXTERMIBOT_HIT_NONLETHAL_KNOCKBACK_AND_FLASH:
        case EXTERMIBOT_HIT_START_NONLETHAL_KNOCKBACK:
        case EXTERMIBOT_HIT_START_NONLETHAL_KNOCKBACK_AND_FLASH:
        case EXTERMIBOT_HIT_START_KNOCKBACK_WITH_FLASH:
            moby_data->fC0 = 5.0f;
            moby_data->f88 = extermibot_nonlethal_knockback_xy_scale_gp * D_0015ED6C;
            moby_data->f8C = extermibot_nonlethal_knockback_z_scale_gp * D_0015ED6C;
            moby_data->fC4 = 10.0f;
            nonlethal_knockback_heading = fast_arc_tan(moby->pos.x - hit_record->who->pos.x,
                                                       moby->pos.y - hit_record->who->pos.y);
            *(u128_3047E8 *)working_vector = hit_record->dir;
            trooper_knockback_arc(
                working_vector, &nonlethal_knockback_heading, &moby_data->f88, &moby_data->f8C);
            trooper_start_knockback(moby, moby_data->f70, nonlethal_knockback_heading, 5, 1, 0);
            moby->state = 11;
            moby_data->b67 = 0x78;
            break;
        case EXTERMIBOT_HIT_LETHAL:
        case EXTERMIBOT_HIT_DEATH_KNOCKBACK:
            moby->flags &= 0xEFFF;
            moby_data->f80 = extermibot_death_knockback_gravity_gp * D_0015ED70;
            moby_data->f88 = extermibot_death_knockback_xy_scale_gp * D_0015ED6C;
            moby_data->f8C = extermibot_death_knockback_z_scale_gp * D_0015ED6C;
            moby_data->fC0 = 5.5f;
            moby_data->fC4 = 11.0f;
            death_knockback_heading = fast_arc_tan(moby->pos.x - hit_record->who->pos.x,
                                                   moby->pos.y - hit_record->who->pos.y);
            *(u128_3047E8 *)working_vector = hit_record->dir;
            trooper_knockback_arc(
                working_vector, &death_knockback_heading, &moby_data->f88, &moby_data->f8C);
            trooper_start_knockback(moby, moby_data->f70, death_knockback_heading, 6, 1, 0);
            moby->state = 14;
            moby_data->b67 = 0xF0;
            if (hit_record->who != 0 && *(struct Moby **)&hit_record->who->pclass != 0 &&
                *(short *)((u8 *)((struct Moby *)hit_record->who->pclass) + 0x46) == 5) {
                stop_animation_l08(moby, 0x800, -1);
            } else {
                stop_animation_l08(moby, 0, -1);
            }
            break;
        }
        trooper_anim_update(moby, moby_data->f60);
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, moby_data->f60);
    if (moby_data->f38 != 0) {
        moby_data->f2B0 = truncate_float_to_s32(scale_time(func_002140F8_3047E8(180.0f, 240.0f)));
        moby_data->f38 = 0;
    }
    fast_dec_timer(&moby_data->f2B0);
    if (moby_data->f2B0 != 0) {
        moby_data->f2A0 = moby_data->f29C + 20.0f;
    } else {
        moby_data->f2A0 = moby_data->f29C;
    }
    if (activate_effect_l08(moby_data->f2A0,
                            moby,
                            moby_data->f120,
                            0,
                            0,
                            ((Path_3047E8 *)l05_paths[moby_data->f2CC])->pts,
                            ((Path_3047E8 *)l05_paths[moby_data->f2CC])->n) != 2) {
        if (moby_data->f2A0 < vector_distance_xy(moby_data->f270, moby_data->f120) ||
            3.0f < func_001F9B88_3047E8(moby->pos.z - moby_data->f128)) {
            moby_data->f164 = 2;
        }
    }
    if (moby_data->f2C8 >= 0) {
        Moby *candidate_target = func_L05_003053D8_3047E8(moby, moby_data->f2C8);
        if (candidate_target != 0) {
            if (moby_data->f164 == 2 ||
                vector_distance_xy(((float *)&moby->pos), ((float *)&candidate_target->pos)) <
                    vector_distance_xy(((float *)&moby->pos), ((float *)&moby_data->target->pos))) {
                moby_data->target = candidate_target;
                moby_data->f164 = 1;
            }
        }
    }
    if (moby_data->target == 0) {
        moby_data->target = (*(Moby **)D_001413D0_lui);
    }
    moby_data->f2D8 = fast_add_rotations(moby_data->f2D8, D_0015ED6C * 9.424778f);
    moby->glow_color = fast_tween_color(
        D_L05_00161A74, D_L05_00161A78, (func_001F9FA8_3047E8(moby_data->f2D8) + 1.0f) * 0.5f);
    k = moby->state;
    if (k < 10) {
        if (k >= 4 && moby_data->target == (*(Moby **)D_001413D0_lui)) {
            float yaw, pitch;
            func_001F9BF0_3047E8(
                working_vector, ((float *)&moby_data->target->pos), ((float *)&moby->pos));
            yaw = fast_subtract_rotations(fast_arc_tan(working_vector[0], working_vector[1]),
                                          moby->rot.z);
            pitch = -fast_arc_tan(vector_length_xy(working_vector), working_vector[2]);
            if (moby->flags & 0x8000) {
                yaw = -yaw;
            }
            if (1.5707964f < yaw) {
                yaw = 1.5707964f;
            } else if (yaw < -1.5707964f) {
                yaw = -1.5707964f;
            }
            if (0.5235988f < pitch) {
                pitch = 0.5235988f;
            } else if (pitch < -0.5235988f) {
                pitch = -0.5235988f;
            }
            moby_data->f1D4 = pitch;
            moby_data->f1D8 = yaw * 0.7f;
            moby_data->f258 = yaw * 0.3f;
        }
    }
    update_joint_animation(moby, moby_data->f170, 1, frame_scale_sq * 0.04f, frame_scale_sq * 0.3f);
    update_joint_animation(moby, moby_data->f1F0, 2, frame_scale_sq * 0.04f, frame_scale_sq * 0.3f);
}

#include "rnc/mobys/moby.h"

/* Pvars of a path-walking moby (only the fields this walker touches). */
typedef struct {
    u8    pad0[0xD0];
    u8    move[0x24]; /* 0xD0: ground-move state */
    f32   speed;      /* 0xF4 */
    u8    padF8[0x164 - 0xF8];
    s32   mode; /* 0x164: 2 once the path is finished */
    u8    pad168[0x280 - 0x168];
    Vec4f goal;    /* 0x280: current path target */
    f32   yaw_vel; /* 0x290 */
    u8    pad294[0x2C0 - 0x294];
    s32   path;     /* 0x2C0 */
    s32   path_pos; /* 0x2C4 */
    u8    pad2C8[0x2D4 - 0x2C8];
    s32   stuck_timer; /* 0x2D4 */
} PathWalkerVars;

extern f32 D_L05_00161A98_gp __asm__("D_L05_00161A98") __attribute__((sda));
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32
    approach_angle(f32 *angle, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025be00");
int test_extermibot_offset_path_clearance(char  *moby,
                                          float *goal,
                                          float  probe_offset_length) __asm__("FUN_L05_00303e50");

extern s32
test_extermibot_offset_path_clearance(char *moby, float *goal, float s) __asm__("FUN_L05_00303e50");
extern f32 FUN_L05_00304058_u(s32, void *, void *, f32) __asm__("FUN_L05_00304058");

/* Walks m toward its next path node, steering `side` radians off the direct heading while
   FUN_L05_00303e50 reports the way blocked; slows near the goal and arms the stuck timer
   when it drifts off the path. */
void move_extermibot_along_path(struct Moby *moby, s32 node, f32 side) __asm__("FUN_L05_00303b08");

void move_extermibot_along_path(struct Moby *moby, s32 node, f32 side) {
    PathWalkerVars *walker_vars = (PathWalkerVars *)moby->pvars;
    Vec4f           facing_direction, out, goal;
    f32             offset_heading;
    f32             distance_to_goal;

    if ((((s32)moby >> 8) & 3) == (frame_number & 3)) {
        if (follow_path(&walker_vars->path,
                        1,
                        walker_vars->path_pos,
                        &moby->pos,
                        node,
                        &walker_vars->goal,
                        D_L05_00161A98_gp * 0.4f) == 0)
            walker_vars->mode = 2;
    }
    qcopy(&goal, &walker_vars->goal);
    distance_to_goal = vector_distance_xy(&moby->pos, &goal);
    if (distance_to_goal > 5.0f && side != 0.0f &&
        test_extermibot_offset_path_clearance(
            (char *)moby, (float *)&goal, D_L05_00161A98_gp * 0.4f)) {
        offset_heading =
            fast_add_rotations(fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y), side);
        approach_angle(&moby->rot.z,
                       offset_heading,
                       &walker_vars->yaw_vel,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
    } else {
        approach_angle(&moby->rot.z,
                       fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y),
                       &walker_vars->yaw_vel,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
    }
    if (fast_difference_between_rotations(
            moby->rot.z, fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y)) < 1.5707964f &&
        distance_to_goal > 3.0f)
        approach_value(&walker_vars->speed, frame_time * 6.0f, frame_time_sq * 6.0f);
    else
        approach_value(&walker_vars->speed, frame_time, frame_time_sq * 6.0f);
    facing_direction.x = fast_cos(moby->rot.z) * 2.0f;
    facing_direction.y = fast_sin(moby->rot.z) * 2.0f;
    facing_direction.z = 0.0f;
    FUN_L00_00258b50(moby, walker_vars->move, &facing_direction, &out, 1.0f);
    if (walker_vars->stuck_timer == 0) {
        if (FUN_L05_00304058_u(
                walker_vars->path, &moby->pos, &moby->pos, D_L05_00161A98_gp * 0.4f) > 2.0f)
            walker_vars->stuck_timer = scale_game_frames(5);
    }
}

extern float FUN_001f9dc8_c(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_c(float) __asm__("FUN_001f9de0");
extern void  FUN_001f9a10_c(float *, float *, float *) __asm__("FUN_001f9a10");
extern void  FUN_001f9a28_c(float *, float *, float *) __asm__("FUN_001f9a28");
extern void  FUN_001f9bf8_c(float *, float *, float) __asm__("FUN_001f9bf8");
extern int   test_path_segment_strict_crossing(int, float *, float *) __asm__("FUN_L00_00261968");

int test_extermibot_offset_path_clearance(char *moby, float *goal, float probe_offset_length) {
    float probe_position[4];
    float offset_probe_position[4];
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float probe_heading;
    probe_heading = fast_arc_tan(goal[0] - MOBY(moby)->pos.x, goal[1] - MOBY(moby)->pos.y);
    probe_heading = fast_add_rotations(probe_heading, *(float *)(moby_data + 0x294) * 1.5f);
    probe_position[0] =
        FUN_001f9dc8_c(probe_heading) * fast_vec_dist(goal, (float *)&MOBY(moby)->pos);
    probe_position[1] =
        FUN_001f9de0_c(probe_heading) * fast_vec_dist(goal, (float *)&MOBY(moby)->pos);
    *(int *)&probe_position[2] = 0;
    FUN_001f9a10_c(probe_position, probe_position, (float *)&MOBY(moby)->pos);
    FUN_001f9a28_c(offset_probe_position, probe_position, goal);
    FUN_001f9bf8_c(offset_probe_position, offset_probe_position, probe_offset_length);
    FUN_001f9a10_c(offset_probe_position, offset_probe_position, (float *)&MOBY(moby)->pos);
    return test_path_segment_strict_crossing(
               *(int *)(moby_data + 0x2C0), probe_position, offset_probe_position) == 0;
}

extern short *D_L05_001AC040_03f80[] __asm__("D_L05_001ABCC0");
extern short  D_L05_00160098_03f80 __asm__("D_L05_0015FFD8");
char *find_nearest_extermibot_group_target(char *self, int group_index) __asm__("FUN_L05_00303f80");

char *find_nearest_extermibot_group_target(char *self, int group_index) {
    short *list = D_L05_001AC040_03f80[group_index];
    float  best = 120.0f;
    char  *nearest_moby = 0;
    if (list == 0) {
        return 0;
    }
    {
        do {
            int i = *(unsigned short *)list & 0x7FFF;
            if (D_L05_0015FFD8_7920[i].flags & 0x1000) {
                if (D_L05_0015FFD8_7920[i].state < 0x7F) {
                    float candidate_distance =
                        fast_vec_dist(&D_L05_0015FFD8_7920[i].pos, self + 0x10);
                    if (candidate_distance < best) {
                        best = candidate_distance;
                        nearest_moby = *(char **)&D_L05_00160098_03f80 + (i << 8);
                    }
                }
            }
        } while (*list++ >= 0);
    }
    return nearest_moby;
}
/* Pushes a point clear of a path and returns the nearest wall distance, like
 * FUN_L00_00261d78. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003054B0), where it is exact; names translated to the US level program. */

typedef int Quad_3054B0 __attribute__((mode(TI)));

typedef union {
    Quad_3054B0 q;
    f32         f[4];
    s32         i[4];
} PathVec;

typedef struct {
    s32     count;
    u8      pad[12];
    PathVec points[1];
} SegmentPath;

extern f32  fast_vec_dot(void *, void *) __asm__("func_001F9AB0");
extern f32  fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_3054B0(PathVec *, PathVec *, PathVec *) __asm__("FUN_001f9a10");
extern void func_001F9BF0_3054B0(PathVec *, PathVec *, PathVec *) __asm__("FUN_001f9a28");
extern void func_001F9CA0_3054B0(PathVec *, PathVec *, PathVec *) __asm__("FUN_001f9ad8");
extern f32  func_001F9CB8_3054B0(PathVec *) __asm__("FUN_001f9af0");
extern void func_L00_001FF4B0_3054B0(PathVec *, PathVec *, f32) __asm__("FUN_001f9bf8");

f32 push_point_from_path(s32   path_index,
                         void *input,
                         void *output,
                         f32   distance) __asm__("FUN_L05_00304058");

f32 push_point_from_path(s32 path_index, void *input, void *output, f32 distance) {
    PathVec      *point = input;
    PathVec      *out = output;
    PathVec       difference;
    PathVec       scaled_difference;
    PathVec       working;
    PathVec       segment;
    PathVec       scaled_segment;
    PathVec       cross;
    PathVec       scaled_cross;
    SegmentPath **entry;
    s32           i;
    s32           found;
    f32           best = 512.0f;

    found = 0;
    qcopy_nc(&working, point);
    for (i = 0; i < ((SegmentPath *)l05_paths[path_index])->count - 1; i++) {
        entry = ((SegmentPath **)&l05_paths[path_index]);
        if ((*entry)->points[i].f[3] == 0.0f && (*entry)->points[i + 1].f[3] == 0.0f)
            continue;
        func_001F9BF0_3054B0(&difference, &working, &(*entry)->points[i]);
        difference.i[2] = 0;
        func_001F9BF0_3054B0(&segment, &(*entry)->points[i + 1], &(*entry)->points[i]);
        segment.i[2] = 0;
        func_L00_001FF4B0_3054B0(&scaled_segment, &segment, 1.0f);
        func_001F9CA0_3054B0(&cross, &difference, &scaled_segment);
        {
            f32 wall_distance = absolute_float(cross.f[2]);
            if (wall_distance < best)
                best = wall_distance;
            if (distance < wall_distance)
                continue;
        }
        {
            f32 segment_length = func_001F9CB8_3054B0(&segment);
            f32 projection = fast_vec_dot(&difference, &scaled_segment);
            if (segment_length < projection || projection < 0.0f) {
                if (!(func_001F9CB8_3054B0(&difference) < distance))
                    continue;
                found = 1;
                func_L00_001FF4B0_3054B0(&scaled_difference, &difference, distance);
                func_001F9BD8_3054B0(&working, &scaled_difference, &(*entry)->points[i]);
                working.f[2] = point->f[2];
            } else {
                found = 1;
                func_L00_001FF4B0_3054B0(&scaled_cross, &scaled_segment, projection);
                func_001F9BF0_3054B0(&scaled_difference, &difference, &scaled_cross);
                func_L00_001FF4B0_3054B0(&scaled_difference, &scaled_difference, distance);
                func_001F9BD8_3054B0(&scaled_difference, &scaled_difference, &scaled_cross);
                func_001F9BD8_3054B0(&working, &scaled_difference, &(*entry)->points[i]);
                working.f[2] = point->f[2];
            }
        }
    }
    if (found)
        qcopy(out, &working);
    return best;
}
/* Extermitank update: spawns its two parts, follows its path, turns its turret toward the hero and sprays particles while firing. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00305778), where it is exact; names translated to the US level program. */

extern float D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float step_angle_toward_target(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern int   frame_number_gp __asm__("D_L05_0015F5CC") MACRO_ADDR;
extern int   FUN_L00_00259028(char *m, char *p, float *pos, char *v);
extern int   FUN_L00_00259740(void *, void *, int);
extern s32
spawn_spark_c2(void *, void *, void *, s32, s32, s32, s32, s32, s32) __asm__("func_00218888");
extern float D_L05_00161AF8 __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/extermitank_update_6.h"

extern float D_L05_00161B18 __attribute__((sda));
extern float D_L05_00161B1C __attribute__((sda));
extern float extermitank_tracked_particle_vector_a_xy_scale_gp __asm__("D_L05_00161B20")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/extermitank_update_5.h"

extern float extermitank_tracked_particle_vector_b_xy_scale_gp __asm__("D_L05_00161B28")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/extermitank_update_4.h"

extern float extermitank_tracked_particle_vector_b_spread_degrees_gp __asm__("D_L05_00161B38")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/extermitank_update_3.h"

extern float D_L05_00161B48 __attribute__((sda));
extern float D_L05_00161B4C __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_xy_scale_gp __asm__("D_L05_00161B50")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/extermitank_update_2.h"

extern float extermitank_secondary_particle_vector_b_xy_scale_gp __asm__("D_L05_00161B58")
    __attribute__((sda));

#include "rnc/overlay/l05/mobys/sharkigator_data/extermitank_update.h"

extern float extermitank_secondary_particle_vector_b_spread_degrees_gp __asm__("D_L05_00161B68")
    __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_spread_degrees_gp __asm__("D_L05_00161B6C")
    __attribute__((sda));
extern short D_L05_00161B78 __attribute__((sda));
extern float D_L05_00161B7C __attribute__((sda));
extern void  FUN_L00_001fff28(void *, int, float);
extern void  mark_moby_for_removal_c3(void *) __asm__("func_0020C828");
void         handle_extermitank_hits_and_sync_children(char *moby) __asm__("FUN_L05_00305178");
void         process_extermitank_particle_hits(char *m) __asm__("FUN_L05_00305898");
void         attach_manipulator(void *owner, s32 slot_index, void *manip) __asm__("FUN_0020cb10");
extern char  D_0013F350[];

typedef char *(*Spawn_305778)(void *, void *, void *, int, int, long, long, long, int);

void extermitank_update(unsigned char *m) __asm__("FUN_L05_00304320");

void extermitank_update(unsigned char *moby) {
    enum {
        EXTERMITANK_INITIALIZE = 0,
        EXTERMITANK_FOLLOW_PATH = 1,
        EXTERMITANK_FIRE_PARTICLES = 2,
        EXTERMITANK_REMOVE = 3
    };
    char *data = ((char *)MOBY(moby)->pvars);
    float origin_position[4];
    float particle_vector_a[4];
    float particle_vector_b[4];
    float working_position[4];

    handle_extermitank_hits_and_sync_children((char *)moby);
    process_extermitank_particle_hits((char *)moby);
    if (*(unsigned char *)(data + 0x2E) == 2) {
        if (*(char **)(data + 0x184) != 0)
            mark_moby_for_removal_c3(*(char **)(data + 0x184));
        if (*(char **)(data + 0x188) != 0)
            mark_moby_for_removal_c3(*(char **)(data + 0x188));
        mark_moby_for_removal_c3(moby);
        return;
    }
    if (moby[0x31] != 0) {
        if (fast_vec_dist(moby + 0x10, D_L05_001671C0) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby[0x7F] = 0x15;
        }
    }
    switch (moby[0x20]) {
    case EXTERMITANK_INITIALIZE: {
        char *child_moby;
        char *g;
        if (*(char **)(data + 0x184) == 0) {
            *(char **)(data + 0x184) = create_moby(0x368);
            (*(char **)(data + 0x184))[0x30] = 0x40;
            *(short *)(*(char **)(data + 0x184) + 0x32) = 0x40;
            (*(char **)(data + 0x184))[0x31] = 1;
            {
                char *g2 = ((char *)&D_0013F350);
                *(long *)(*(char **)(data + 0x184) + 0x38) =
                    *(long *)(*(char **)(g2 + 0x2080) + 0x38);
            }
            *(short *)(*(char **)(data + 0x184) + 0x34) = 0x4000;
            child_moby = *(char **)(data + 0x184);
            qcopy(child_moby + 0x10, moby + 0x10);
            MOBY(child_moby)->collision = 0;
            update_entity(*(char **)(data + 0x184));
        }
        if (*(char **)(data + 0x188) == 0) {
            *(char **)(data + 0x188) = create_moby(0x369);
            (*(char **)(data + 0x188))[0x30] = 0x40;
            *(short *)(*(char **)(data + 0x188) + 0x32) = 0x40;
            (*(char **)(data + 0x188))[0x31] = 1;
            {
                char *g2 = ((char *)&D_0013F350);
                *(long *)(*(char **)(data + 0x188) + 0x38) =
                    *(long *)(*(char **)(g2 + 0x2080) + 0x38);
            }
            *(short *)(*(char **)(data + 0x188) + 0x34) = 0x4000;
            child_moby = *(char **)(data + 0x188);
            qcopy(child_moby + 0x10, moby + 0x10);
            MOBY(child_moby)->collision = 0;
            update_entity(*(char **)(data + 0x188));
        }
        attach_manipulator((char *)moby, 0, (unsigned char *)(data + 0xF0));
        attach_manipulator((char *)moby, 1, (unsigned char *)(data + 0x130));
        MOBY(moby)->flags |= 0x1000;
        g = ((char *)&D_0013F350);
        MOBY(moby)->spawn_frame = *(long *)(*(char **)(g + 0x2080) + 0x38);
        FUN_L01_0026d930(data + 0x70);
        *(float *)(data + 0x7C) = 2.0f;
        *(float *)(data + 0x78) = 4.0f;
        *(int *)(data + 0x70) = 0x38D;
        *(float *)(data + 0xAC) = 0.01f;
        *(float *)(data + 0xB0) = 0.3f;
        *(float *)(data + 0xB4) = 0.1f;
        *(char **)(data + 0xD0) = ((char *)l05_paths[*(int *)(data + 0xD4)]);
        *(int *)(data + 0xC0) = 1;
        qcopy(moby + 0x10, *(char **)(data + 0xD0) + 0x10);
        moby[0x20] = 1;
        if (moby[0x53] != 0)
            blend_moby_animation(moby, 0, 0, 0);
        *(float *)(data + 0x94) = D_L05_00161AF8 * D_0015ED6C_gp;
        clear_u64_value(data + 0x40);
        *(unsigned short *)(data + 0x3E) |= 2;
        data[0x2E] = 1;
        data[0x29] = 0;
        *(short *)(data + 0x24) = 6;
        data[0x28] = 3;
        *(float *)(data + 0x20) = 6.0f;
        *(int *)(data + 0x180) = scale_game_frames(0x3C);
        *(int *)(data + 0x198) = 0;
        {
            float t = *(volatile float *)&D_L05_00161B78;
            *(float *)(data + 0x194) = *(volatile float *)(moby + 0x48);
            data[0x58] = truncate_float_to_s32(t);
        }
        data[0x5A] = truncate_float_to_s32(D_L05_00161B7C * 8.0f);
        break;
    }
    case EXTERMITANK_FOLLOW_PATH: {
        char       *pp = data + 0xC0;
        char       *path = *(char **)(pp + 0x10);
        int         n = *(int *)path;
        signed char dir = *(signed char *)(pp + 4);
        int         last = 0;
        int         idx;
        char       *h;
        if (dir >= 0)
            last = n - 1;
        idx = (*(int *)(data + 0xC0) + n + dir) % n;
        FUN_L00_00259028(
            (char *)moby, data + 0x70, (float *)(path + (idx * 16 + 0x10)), data + 0x40);
        if (vector_distance_xy(moby + 0x10, *(char **)(pp + 0x10) + (idx * 16 + 0x10)) < 1.0f) {
            if (idx == last) {
                char *q = *(char **)(pp + 0x10);
                if (5.0f < fast_vec_dist(q + *(int *)q * 16, q + 0x10)) {
                    int t = -*(signed char *)(pp + 4);
                    *(signed char *)(pp + 4) = t;
                }
            }
            *(int *)pp = idx;
        }
        h = ((char *)&D_0013F350 + 0x80);
        if (vector_distance_xy(moby + 0x10, h) < *(float *)(data + 0x174)) {
            if (absolute_float(MOBY(moby)->pos.z - *(float *)(h + 8)) < 4.0f) {
                if (*(int *)(data + 0x190) == -1 ||
                    FUN_L00_00259740(h,
                                     ((char *)l05_paths[*(int *)(data + 0x190)]) + 0x10,
                                     *(int *)l05_paths[*(int *)(data + 0x190)]) != 0) {
                    qcopy(origin_position, moby + 0x10);
                    origin_position[2] = origin_position[2] + 3.0f;
                    if (collision_line(origin_position, h + 0x50, 2, moby, 0) == 0) {
                        if (moby[0x53] != 0)
                            blend_moby_animation(moby, 0, 0, 3);
                        moby[0x20] = 2;
                    }
                }
            }
        }
        if (vector_distance_xy(moby + 0x10, ((char *)&D_0013F350 + 0x80)) < 16.0f) {
            char *g = ((char *)&D_0013F350);
            float a = fast_arc_tan(*(float *)(g + 0x80) - MOBY(moby)->pos.x,
                                   *(float *)(g + 0x84) - MOBY(moby)->pos.y);
            step_angle_toward_target((float *)(data + 0x194), 0, a, D_0015ED6C_gp * 1.5707964f);
            step_angle_toward_target(
                (float *)(data + 0x198), 0, 0.08726646f, D_0015ED6C_gp * 1.5707964f);
        } else {
            step_angle_toward_target(
                (float *)(data + 0x194), 0, MOBY(moby)->rot.z, D_0015ED6C_gp * 1.5707964f);
            step_angle_toward_target((float *)(data + 0x198), 0, 0.0f, D_0015ED6C_gp * 1.5707964f);
        }
        FUN_L00_001fff28(data + 0x140, 1, *(float *)(data + 0x198));
        FUN_L00_001fff28(
            data + 0x100, 2, fast_subtract_rotations(*(float *)(data + 0x194), MOBY(moby)->rot.z));
        break;
    }
    case EXTERMITANK_FIRE_PARTICLES: {
        char       *pp = data + 0xC0;
        char       *path = *(char **)(pp + 0x10);
        int         n = *(int *)path;
        signed char dir = *(signed char *)(pp + 4);
        int         last = 0;
        int         idx;
        int         i;
        int         k;
        char       *h;
        if (dir >= 0)
            last = n - 1;
        idx = (*(int *)(data + 0xC0) + n + dir) % n;
        FUN_L00_00259028(
            (char *)moby, data + 0x70, (float *)(path + (idx * 16 + 0x10)), data + 0x40);
        if (vector_distance_xy(moby + 0x10, *(char **)(pp + 0x10) + (idx * 16 + 0x10)) < 1.0f) {
            if (idx == last) {
                char *q = *(char **)(pp + 0x10);
                if (5.0f < fast_vec_dist(q + *(int *)q * 16, q + 0x10)) {
                    int t = -*(signed char *)(pp + 4);
                    *(signed char *)(pp + 4) = t;
                }
            }
            *(int *)pp = idx;
        }
        particle_vector_a[3] = extermitank_tracked_particle_vector_a_w_gp;
        particle_vector_b[3] = extermitank_tracked_particle_vector_b_w_gp;
        FUN_L00_0024f7c8(moby, 2, origin_position);
        for (i = 0; i < 2; i++) {
            int   a, b, e;
            float ang;
            char *r;
            qcopy(working_position, origin_position);
            working_position[0] += random_float_between(-0.065f, 0.065f);
            working_position[1] += random_float_between(-0.065f, 0.065f);
            working_position[2] += random_float_between(-0.065f, 0.065f);
            particle_vector_a[0] =
                fast_cos(*(float *)(data + 0x194)) *
                (extermitank_tracked_particle_vector_a_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_a[1] =
                fast_sin(*(float *)(data + 0x194)) *
                (extermitank_tracked_particle_vector_a_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_a[2] = 0.0f;
            particle_vector_a[2] = random_float_between(
                extermitank_tracked_particle_vector_a_z_scale_gp * D_0015ED6C_gp * 0.9f,
                extermitank_tracked_particle_vector_a_z_scale_gp * D_0015ED6C_gp * 1.1f);
            ang = random_float_between(-extermitank_tracked_particle_vector_b_spread_degrees_gp,
                                       extermitank_tracked_particle_vector_b_spread_degrees_gp) *
                  0.017453292f;
            particle_vector_b[0] =
                fast_cos(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_tracked_particle_vector_b_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_b[1] =
                fast_sin(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_tracked_particle_vector_b_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_b[2] = 0.0f;
            particle_vector_b[2] = random_float_between(
                extermitank_tracked_particle_vector_b_z_scale_gp * D_0015ED6C_gp * 0.9f,
                extermitank_tracked_particle_vector_b_z_scale_gp * D_0015ED6C_gp * 1.1f);
            a = truncate_float_to_s32(scale_time(
                random_float_between(extermitank_tracked_particle_lifetime_base_frames_gp,
                                     extermitank_tracked_particle_lifetime_base_frames_gp +
                                         extermitank_tracked_particle_lifetime_base_frames_gp)));
            b = truncate_float_to_s32(
                scale_time(random_float_between(D_L05_00161B18, D_L05_00161B18 * 1.2f)));
            e = truncate_float_to_s32(
                scale_time(random_float_between(D_L05_00161B1C, D_L05_00161B1C * 1.2f)));
            r = ((Spawn_305778)spawn_spark_c2)(working_position,
                                               particle_vector_a,
                                               particle_vector_b,
                                               extermitank_tracked_particle_color_a_gp,
                                               extermitank_tracked_particle_color_b_gp,
                                               a,
                                               b,
                                               e,
                                               -1);
            if ((frame_number_gp & 7) == 0 && r != 0 && i == 0) {
                int j;
                for (j = 0; j < 8; j++) {
                    int   slot_byte_offset = j * 4;
                    char *particle_slots = data + 0x1A0;
                    if (*(char **)(particle_slots + slot_byte_offset) == 0) {
                        *(char **)(particle_slots + slot_byte_offset) = r;
                        break;
                    }
                }
            }
        }
        particle_vector_a[3] = extermitank_secondary_particle_vector_a_w_gp;
        particle_vector_b[3] = extermitank_secondary_particle_vector_b_w_gp;
        for (k = 1; k >= 0; k--) {
            int   a, b, e;
            float ang;
            qcopy(working_position, origin_position);
            working_position[0] += random_float_between(-0.2f, 0.2f);
            working_position[1] += random_float_between(-0.2f, 0.2f);
            working_position[2] += random_float_between(-0.2f, 0.2f);
            ang = random_float_between(-extermitank_secondary_particle_vector_a_spread_degrees_gp,
                                       extermitank_secondary_particle_vector_a_spread_degrees_gp) *
                  0.017453292f;
            particle_vector_a[0] =
                fast_cos(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_secondary_particle_vector_a_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_a[1] =
                fast_sin(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_secondary_particle_vector_a_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_a[2] = 0.0f;
            particle_vector_a[2] = random_float_between(
                extermitank_secondary_particle_vector_a_z_scale_gp * D_0015ED6C_gp * 0.7f,
                extermitank_secondary_particle_vector_a_z_scale_gp * D_0015ED6C_gp * 1.5f);
            ang = random_float_between(-extermitank_secondary_particle_vector_b_spread_degrees_gp,
                                       extermitank_secondary_particle_vector_b_spread_degrees_gp) *
                  0.017453292f;
            particle_vector_b[0] =
                fast_cos(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_secondary_particle_vector_b_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_b[1] =
                fast_sin(fast_add_rotations(*(float *)(data + 0x194), ang)) *
                (extermitank_secondary_particle_vector_b_xy_scale_gp * D_0015ED6C_gp);
            particle_vector_b[2] = 0.0f;
            particle_vector_b[2] = random_float_between(
                extermitank_secondary_particle_vector_b_z_scale_gp * D_0015ED6C_gp * 0.7f,
                extermitank_secondary_particle_vector_b_z_scale_gp * D_0015ED6C_gp * 1.5f);
            a = truncate_float_to_s32(scale_time(
                random_float_between(extermitank_secondary_particle_lifetime_base_frames_gp,
                                     extermitank_secondary_particle_lifetime_base_frames_gp +
                                         extermitank_secondary_particle_lifetime_base_frames_gp)));
            b = truncate_float_to_s32(
                scale_time(random_float_between(D_L05_00161B48, D_L05_00161B48 * 1.2f)));
            e = truncate_float_to_s32(
                scale_time(random_float_between(D_L05_00161B4C, D_L05_00161B4C * 1.2f)));
            ((Spawn_305778)spawn_spark_c2)(working_position,
                                           particle_vector_a,
                                           particle_vector_b,
                                           extermitank_secondary_particle_color_a_gp,
                                           extermitank_secondary_particle_color_b_gp,
                                           a,
                                           b,
                                           e,
                                           -1);
        }
        qcopy(working_position, moby + 0x10);
        working_position[2] = working_position[2] + 3.0f;
        h = ((char *)&D_0013F350 + 0x80);
        if (*(float *)(data + 0x174) < vector_distance_xy(moby + 0x10, h) ||
            4.0f <= absolute_float(MOBY(moby)->pos.z - *(float *)(h + 8)) ||
            (*(int *)(data + 0x190) != -1 &&
             FUN_L00_00259740(h,
                              ((char *)l05_paths[*(int *)(data + 0x190)]) + 0x10,
                              *(int *)l05_paths[*(int *)(data + 0x190)]) == 0) ||
            collision_line(working_position, D_0013E533 + 0xEED, 2, moby, 0) != 0) {
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, 3);
            moby[0x20] = 1;
        }
        {
            char *g = ((char *)&D_0013F350);
            float a = fast_arc_tan(*(float *)(g + 0x80) - MOBY(moby)->pos.x,
                                   *(float *)(g + 0x84) - MOBY(moby)->pos.y);
            step_angle_toward_target((float *)(data + 0x194), 0, a, D_0015ED6C_gp * 1.5707964f);
            step_angle_toward_target(
                (float *)(data + 0x198), 0, 0.08726646f, D_0015ED6C_gp * 1.5707964f);
        }
        FUN_L00_001fff28(data + 0x140, 1, *(float *)(data + 0x198));
        FUN_L00_001fff28(
            data + 0x100, 2, fast_subtract_rotations(*(float *)(data + 0x194), MOBY(moby)->rot.z));
        break;
    }
    case EXTERMITANK_REMOVE:
        if (*(char **)(data + 0x184) != 0)
            mark_moby_for_removal_c3(*(char **)(data + 0x184));
        if (*(char **)(data + 0x188) != 0)
            mark_moby_for_removal_c3(*(char **)(data + 0x188));
        mark_moby_for_removal_c3(moby);
        break;
    }
}

extern void *
select_candidate_by_max_float_2c(void *, void *, void *, int) __asm__("FUN_L05_002899f8");
extern void
hit_update_305178(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern int  allocate_voice_305178(int, int, void *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void spawn_explosion(void *,
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
extern void
spawn_particle_305178(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern void  update_motion_305178(void *, void *) __asm__("FUN_L00_0025d458");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void  finish_motion_305178(void *, void *) __asm__("FUN_L00_0025d538");

int          D_L05_00161B74 = 5;
extern float extermitank_child_animation_speed_divisor_gp __asm__("D_L05_00161AF0")
    __attribute__((sda));
extern const float frame_time_const __asm__("D_0015ED6C");
extern float       extermitank_child_animation_speed_offset_scale_gp __asm__("D_L05_00161AF4")
    __attribute__((sda));

void handle_extermitank_hits_and_sync_children(char *moby) {
    char        *data = ((char *)MOBY(moby)->pvars);
    char        *first;
    char        *second;
    char        *part0;
    char        *part1;
    char        *part2;
    char        *hit;
    char        *owner;
    int          result;
    float        remaining = 0.0f;
    float        position[4] __attribute__((aligned(16)));
    char         effect_data[16] __attribute__((aligned(16)));
    float        speed;
    float        angle;
    struct Hero *base;

    part0 = find_moby_hit(moby, 0x210000, 0);
    first = *(char **)(data + 0x184);
    part1 = first ? find_moby_hit(first, 0x210000, 0) : 0;
    second = *(char **)(data + 0x188);
    part2 = second ? find_moby_hit(second, 0x210000, 0) : 0;
    part0 = select_candidate_by_max_float_2c(part0, part1, part2, 0);
    if (part0 != 0) {
        if (*(char **)(part0 + 0x20) == moby)
            part0 = 0;
        owner = *(char **)(part0 + 0x20);
        if (owner != 0 && (*(short *)(owner + 0xa6) == 0x47 || *(short *)(owner + 0xa6) == 0))
            part0 = 0;
    }
    hit = part0;
    hit_update_305178(moby, hit, data + 0x20, 0, &result, &remaining, 0, 4);
    if (result == 1 || (unsigned char)moby[0x20] == 3)
        goto mark_children;
    {
        *(float *)(data + 0x20) -= remaining;
        if (*(float *)(data + 0x20) > 0.0f && remaining > 0.0f) {
            allocate_voice_305178(2, 0, moby);
            ((unsigned char *)data)[0x67] = 200;
            goto update_motion;
        } else {
            allocate_voice_305178(3, 0, moby);
            mark_moby_for_removal_cf(moby, 0, -1);
            qcopy(position, moby + 0x10);
            position[2] += 3.0f;
            spawn_explosion(moby,
                            data + 0x40,
                            position,
                            0.0f,
                            0.0f,
                            0x14,
                            8,
                            0x14,
                            4.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            qcopy(position, moby + 0x10);
            position[0] += 2.0f * fast_cos(MOBY(moby)->rot.z);
            position[1] += 2.0f * fast_sin(MOBY(moby)->rot.z);
            spawn_explosion(moby,
                            data + 0x40,
                            position,
                            0.0f,
                            0.0f,
                            0x14,
                            8,
                            0x14,
                            4.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            qcopy(position, moby + 0x10);
            angle = fast_add_rotations(MOBY(moby)->rot.z, 3.14159274f);
            position[0] += 2.0f * fast_cos(angle);
            angle = fast_add_rotations(MOBY(moby)->rot.z, 3.14159274f);
            position[1] += 2.0f * fast_sin(angle);
            spawn_explosion(moby,
                            data + 0x40,
                            position,
                            0.0f,
                            0.0f,
                            0x14,
                            8,
                            0x14,
                            4.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            *(short *)(data + 0x19e) = scale_game_frames(D_L05_00161B74);
            moby[0x20] = 3;
            clear_u64_value(effect_data);
            *(float *)(effect_data + 8) = *(float *)(data + 0x194);
            spawn_particle_305178(moby,
                                  0x6dd,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6df,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6e0,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6fe,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6fe,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6ff,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6ff,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            spawn_particle_305178(moby,
                                  0x6ff,
                                  moby + 0x10,
                                  moby + 0x40,
                                  0,
                                  0,
                                  0.0f,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro,
                                  D_L05_0015F580_macro);
            return;
        }
    }
update_motion:
    update_motion_305178(moby, data + 0x60);
mark_children:
    ((unsigned char *)moby)[0xa4] = 0xff;
    if (*(char **)(data + 0x184) != 0)
        ((unsigned char *)*(char **)(data + 0x184))[0xa4] = 0xff;
    if (*(char **)(data + 0x188) != 0)
        ((unsigned char *)*(char **)(data + 0x188))[0xa4] = 0xff;
    if (*(int *)(data + 0x38) != 0 && *(int *)(data + 0x178) == 0) {
        *(int *)(data + 0x178) = scale_game_frames(0xf0);
        *(int *)(data + 0x38) = 0;
        base = &hero;
        angle = fast_arc_tan(base->motion.pos.f[0] - MOBY(moby)->pos.x,
                             base->motion.pos.f[1] - MOBY(moby)->pos.y);
        if (fast_difference_between_rotations(MOBY(moby)->rot.z, angle) > 1.5707964f)
            ((signed char *)data)[0xc4] *= -1;
    }
    if (fast_dec_timer(data + 0x178) != 0)
        *(float *)(data + 0x174) = 20.0f;
    else
        *(float *)(data + 0x174) = 14.0f;
    if (*(char **)(data + 0x184) != 0) {
        char *first;
        float offset_scale;
        speed = fast_vec_length(data + 0x40);
        speed /= extermitank_child_animation_speed_divisor_gp * frame_time_const;
        offset_scale = extermitank_child_animation_speed_offset_scale_gp;
        *(float *)(*(char **)(data + 0x184) + 0x58) = speed;
        *(float *)(*(char **)(data + 0x184) + 0x58) += *(float *)(data + 0x84) * offset_scale;
        first = *(char **)(data + 0x184);
        qcopy(first + 0x40, moby + 0x40);
        qcopy(first + 0x10, moby + 0x10);
    }
    if (*(char **)(data + 0x188) != 0) {
        char *second;
        float offset_scale;
        speed = fast_vec_length(data + 0x40);
        speed /= extermitank_child_animation_speed_divisor_gp * frame_time_const;
        offset_scale = extermitank_child_animation_speed_offset_scale_gp;
        *(float *)(*(char **)(data + 0x188) + 0x58) = speed;
        *(float *)(*(char **)(data + 0x188) + 0x58) -= *(float *)(data + 0x84) * offset_scale;
        second = *(char **)(data + 0x188);
        qcopy(second + 0x40, moby + 0x40);
        qcopy(second + 0x10, moby + 0x10);
    }
    finish_motion_305178(moby, data + 0x60);
}
/* Drops stale entries from the moby's 8 slots, then pushes each remaining one away from the hero. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00306CF0), where it is exact; names translated to the US level program. */

typedef struct {
    float         v[4];
    char         *owner;
    int           x14;
    unsigned char x18;
    unsigned char x19;
    short         type;
    float         x1C;
    int           flags;
} Hit_306CF0;

extern void
collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern void fast_vec_sub(void *dst, void *a, void *b) __asm__("FUN_001f9a28");

void process_extermitank_particle_hits(char *moby) {
    char       **slots = (char **)(((char *)MOBY(moby)->pvars) + 0x1A0);
    struct Hero *base;
    Hit_306CF0   hit_info;
    int          i;
    int          j;
    for (i = 0; i < 8; i++) {
        char *tracked_particle = slots[i];
        if (tracked_particle != 0) {
            if (*(unsigned char *)tracked_particle != 2 ||
                *(short *)(tracked_particle + 0x32) == 0) {
                slots[i] = 0;
            }
        }
    }
    base = &hero;
    if (base->items[2].item_id == 6)
        return;
    hit_info.flags |= 1;
    hit_info.owner = moby;
    hit_info.x14 = 1;
    hit_info.x1C = 1.0f;
    fast_vec_sub(hit_info.v, &base->motion.pos, moby + 0x10);
    hit_info.v[2] = 0.0f;
    set_vector_length_xy(hit_info.v, hit_info.v, 1.5f);
    hit_info.v[2] = 1.0f;
    hit_info.v[3] = 5627.9248f;
    hit_info.x19 = 1;
    hit_info.type = MOBY(moby)->oclass;
    hit_info.x18 = 0;
    for (j = 0; j < 8; j++) {
        if (slots[j] != 0) {
            collect_mobies_in_sphere(slots[j] + 0x10, 0, moby, &hit_info, 0.75f);
        }
    }
}

extern int select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
void       copy_racer_path_node(char *moby, int idx, float *out) __asm__("FUN_L05_00306d30");

typedef struct {
    int   count; /* 0x00: number of nodes */
    char  pad04[0xC];
    float points[1][4]; /* 0x10: position; w marks the node */
} L16PatrolPath;

typedef struct {
    char           pad0[0x200];
    float          position[4]; /* 0x200 */
    char           pad210[0x10];
    int            base_path;      /* 0x220: index into D_L05_001B0930 */
    int            branch_path[7]; /* 0x224: indexed by a node's mark - 1 */
    char           pad240[4];
    L16PatrolPath *path; /* 0x244: the path being followed */
    char           pad248[0x10];
    short          node; /* 0x258: the node being steered for */
} L05PatrolData;

typedef struct {
    char           pad0[0x10];
    float          position[4]; /* 0x10 */
    char           pad20[0x28];
    float          yaw; /* 0x48 */
    char           pad4C[0x2C];
    L05PatrolData *data; /* 0x78 */
} L16PatrolMoby;

/* Patrol step: advances the moby's path node until the next node is ahead and far enough, switching between the base path and its branches. */
/* Same routine as FUN_L16_002cf9f8, with this level's data layout. */
void advance_racer_path_node(char *patrol_moby) __asm__("FUN_L05_00306e10");

void advance_racer_path_node(char *patrol_moby) {
    L16PatrolMoby *moby = (L16PatrolMoby *)patrol_moby;
    L05PatrolData *patrol_data = moby->data;
    float          target[4];
    float          heading, distance;

    copy_racer_path_node((char *)moby, patrol_data->node, target);
    heading = fast_arc_tan(target[0] - moby->position[0], target[1] - moby->position[1]);
    for (;;) {
        L16PatrolPath *path;
        L16PatrolPath *base;

        copy_racer_path_node((char *)moby, patrol_data->node, target);
        distance = vector_distance_xy(target, patrol_data->position);
        if ((fast_difference_between_rotations(heading, moby->yaw) < 1.5707964f ||
             distance > 4.0f) &&
            distance > 2.0f) {
            break;
        }
        path = patrol_data->path;
        patrol_data->node = (patrol_data->node + 1) % path->count;
        base = (L16PatrolPath *)l05_paths[patrol_data->base_path];
        if (path != base) {
            /* At the last node of a branch: back to the base path, five nodes past its nearest one. */
            if (patrol_data->node == path->count - 1) {
                patrol_data->path = base;
                patrol_data->node =
                    select_point_index_by_distance(patrol_data->position, base, 0.0f);
                patrol_data->node =
                    offset_path_index((char *)patrol_data->path, patrol_data->node, 5, 1);
            }
        } else if (path->points[patrol_data->node][3] > 0.0f && random_integer_below(100) > 0) {
            /* At a marked node of the base path: more often than not, onto the branch the mark names. */
            int branch = truncate_float_to_s32(patrol_data->path->points[patrol_data->node][3]) - 1;

            patrol_data->path = (L16PatrolPath *)l05_paths[patrol_data->branch_path[branch]];
            patrol_data->node = 0;
        }
        /* The same two statements as before the loop, written out again: the compiler merges the copies. */
        copy_racer_path_node((char *)moby, patrol_data->node, target);
        heading = fast_arc_tan(target[0] - moby->position[0], target[1] - moby->position[1]);
    }
}

/* Same routine as FUN_L16_002d0058 without the pitch step: times the leap animation to the
 * remaining jump, then copies the animation speed to the rider. */
struct LeapVars {
    u8           pad0[0x208];
    f32          height; /* 0x208 */
    u8           pad20C[0xC];
    f32          targetHeight; /* 0x218 */
    u8           pad21C[0x30];
    f32          verticalSpeed; /* 0x24C */
    u8           pad250[0x18];
    struct Moby *rider; /* 0x268 */
    s32          timer; /* 0x26C */
    u8           pad270[2];
    s16          sequence; /* 0x272 */
};

extern int
solve_quadratic_equation(float *, float *, float, float, float) __asm__("FUN_L00_002595a0");
extern float FUN_0020c9e0(struct Moby *);

void update_racer_jump_animations(struct Moby *moby) __asm__("FUN_L05_00307570");

void update_racer_jump_animations(struct Moby *moby) {
    float            root0, root1;
    struct LeapVars *leap_data = (struct LeapVars *)moby->pvars;

    if (leap_data->timer != 0) {
        float duration;
        float acceleration;
        if (moby->prev_seq != 5 && scale_game_frames(3) >= leap_data->timer) {
            blend_racer_and_child_animations(moby, 5, 1, scale_game_frames(5));
            leap_data->sequence = -1;
        }
        acceleration = frame_time_sq * 21.0f * -0.5f;
        duration = 30.0f;
        if (solve_quadratic_equation(&root0,
                                     &root1,
                                     acceleration,
                                     leap_data->verticalSpeed - acceleration,
                                     leap_data->height - leap_data->targetHeight) > 0 &&
            root0 > 0.0f) {
            duration = truncate_float_to_s32(root0);
        }
        if (scale_game_frames(7) < leap_data->timer && scale_game_frames(11) > leap_data->timer &&
            moby->prev_seq == 5 && (float)scale_game_frames(27) < duration) {
            short sequence;
            int   chosen;
            chosen = random_int_between(0, 3) + 1;
            leap_data->sequence = chosen;
            sequence = chosen;
            blend_racer_and_child_animations(moby, sequence, 1, scale_game_frames(7));
        }
        if (leap_data->sequence != -1) {
            if (moby->seq == moby->prev_seq) {
                float frame = FUN_0020c9e0(moby);
                if (frame > 7.0f && frame < 20.0f && duration != 0.0f) {
                    float speed = 30.0f / duration;
                    if (speed > 1.7f)
                        speed = 1.7f;
                    if (speed < 0.3f)
                        speed = 0.3f;
                    moby->anim_speed = speed;
                }
            }
            if (leap_data->sequence != -1) {
                if (duration < (float)scale_game_frames(9)) {
                    blend_racer_and_child_animations(moby, 5, 3, scale_game_frames(7));
                    leap_data->sequence = -1;
                } else if (moby->anim_flags & 2) {
                    blend_racer_and_child_animations(moby, 5, 2, scale_game_frames(7));
                    leap_data->sequence = -1;
                }
            }
        }
        if (moby->seq == moby->prev_seq && moby->seq == 5 && duration != 0.0f) {
            float remaining;
            remaining = 25.0f;
            remaining -= FUN_0020c9e0(moby);
            remaining /= duration * 0.5f;
            moby->anim_speed = remaining;
        }
    } else {
        moby->anim_speed = 1.0f;
        if ((moby->anim_flags & 2) && moby->prev_seq != 0) {
            blend_racer_and_child_animations(moby, 0, 0, scale_game_frames(8));
        }
    }
    if (leap_data->rider != 0)
        leap_data->rider->anim_speed = moby->anim_speed;
}

/* Update function of moby class 717 (skid_mcmarx): a hoverboard racer that follows its path, jumps, and steers. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00308D68), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) Vec4_308D68;

typedef struct {
    int         count;
    int         pad[3];
    Vec4_308D68 points[1];
} Path;

typedef struct {
    char  p00[0x64];
    float f64;
    float f68;
    char  p6C[0x14];
} Manip_308D68;

typedef struct {
    Manip_308D68 m[4];
    float        pos[4];
    float        f210[4];
    int          f220[5];
    int          path_id;
    float        cruise_speed;
    float        launch_speed;
    float        speed;
    Path        *path;
    float        yaw_speed;
    float        vertical_speed;
    int          racer_index;
    int          f254;
    short        point_index;
    short        launch_timer;
    int          f25C;
    int          f260;
    int          f264;
    Moby        *child;
    int          jump_frames;
    short        f270;
    short        f272;
    float        bob_phase;
    int          f278;
    int          jump_point;
} Data;

typedef struct {
    char p00[0x50];
    int  f50;
} ChildData;

extern float frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern float
spring_angle(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int   D_0015EE38_macro __asm__("D_0015EE38") MACRO_ADDR;
extern int   D_L05_00178300[];
extern int   is_moby_in_class_range_500_to_540(int) __asm__("FUN_L00_0025e3b8");
extern void  copy_racer_path_node(char *moby, int idx, float *out) __asm__("FUN_L05_00306d30");
extern void  add_moby_frames_for_group_proximity(char *moby) __asm__("FUN_L05_00307010");
void         advance_racer_path_node(char *obj) __asm__("FUN_L05_00306e10");
void         FUN_L05_00307570_c(struct Moby *m) __asm__("FUN_L05_00307570");
extern char  D_001413DC[];
extern int func_L00_001F2BE8_308D68(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");

void skid_mcmarx_update(Moby *moby) __asm__("FUN_L05_00307910");

void skid_mcmarx_update(Moby *moby) {
    enum {
        SKID_MCMARX_INITIALIZE = 0,
        SKID_MCMARX_WAIT_FOR_RACE_MODE = 1,
        SKID_MCMARX_FOLLOW_PATH = 2
    };
    float path_point[4];
    float step[4];
    float probe[4];
    float push[4];
    Data *data = (Data *)*(void **)&moby->pvars;

    if (moby->visible != 0 && fast_vec_dist(((float *)&moby->pos), D_L05_001671C0) < 29.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x17;
    }
    switch (moby->state) {
    case SKID_MCMARX_INITIALIZE: {
        Path *path;

        moby->update_dist = 0x18;
        moby->flags |= 0x41;
        path = (Path *)l05_paths[data->path_id];
        data->path = path;
        copy_racer_path_node((char *)moby, 0, path_point);
        qcopy(((float *)&moby->pos), path_point);
        qcopy(data->pos, ((float *)&moby->pos));
        moby->rot.x = 0.0f;
        moby->rot.y = 0.0f;
        moby->rot.z = fast_arc_tan(path->points[1].x - path->points[0].x,
                                   path->points[1].y - path->points[0].y);
        data->cruise_speed =
            D_0015ED6C_gp * 17.0f + (float)data->racer_index * (D_0015ED6C_gp * 1.55f);
        data->launch_speed =
            D_0015ED6C_gp * 24.0f + (float)data->racer_index * (D_0015ED6C_gp * 1.55f);
        if (((unsigned char *)alternate_item_available)[0] == 0) {
            float handicap = (float)(D_0015EE38_macro - 4);

            if (handicap < 0.0f) {
                handicap = 0.0f;
            }
            handicap = 1.0f - handicap * 0.01f;
            if (handicap < 0.77f) {
                handicap = 0.77f;
            }
            data->cruise_speed *= handicap;
            data->launch_speed *= handicap;
        }
        data->bob_phase = random_angle_radians();
        moby->state = 1;
        break;
    }
    case SKID_MCMARX_WAIT_FOR_RACE_MODE:
        if (*(int *)(((char *)&D_001413DC)) == 0x16) {
            moby->update_dist = 0xFF;
            moby->flags &= ~0x41;
            data->point_index = 0;
            data->launch_timer = scale_game_frames(0xB4);
            data->vertical_speed = 0.0f;
            data->yaw_speed = 0.0f;
            data->jump_frames = 0;
            data->f264 = 0;
            data->speed = 0.0f;
            if (data->child == 0) {
                Moby *child = (Moby *)spawn_racer_child((char *)moby);

                data->child = child;
                if (child != 0) {
                    ((ChildData *)*(void **)&child->pvars)->f50 = 1;
                }
            }
            if (data->racer_index % 2 == 1) {
                moby->flags |= 0x8000;
                if (data->child != 0) {
                    data->child->flags |= 0x8000;
                }
            }
            moby->state = 2;
        }
        break;
    case SKID_MCMARX_FOLLOW_PATH: {
        Hero *hero = (Hero *)(((char *)&D_0013F350));

        if (hero->state.control_mode != 0x16) {
            moby->state = 0;
        } else {
            float ground;
            float height_above_ground;
            float max_turn;
            float turn;
            int   hit_count;
            int   j;

            advance_racer_path_node((char *)moby);
            add_moby_frames_for_group_proximity((char *)moby);
            start_racer_branch_jump((char *)moby);
            if (data->jump_frames != 0) {
                data->jump_frames++;
                if (data->jump_frames < scale_game_frames(0x11)) {
                    float path_length = 0.0f;
                    float air_time;
                    float jump_speed;
                    int   i;
                    int   next;

                    for (i = data->point_index; i < data->jump_point; i = next) {
                        next = offset_path_index((char *)data->path, i, 1, 1);
                        path_length += vector_distance_xy((float(*)[4])data->path + (i + 1),
                                                          (float(*)[4])data->path + (next + 1));
                    }
                    air_time = path_length / data->speed;
                    jump_speed = (data->path->points[data->jump_point].z -
                                  data->path->points[data->point_index].z) /
                                     air_time +
                                 frame_time_sq_macro * 21.0f * air_time * 0.5f;
                    if (data->vertical_speed < 0.0f) {
                        data->vertical_speed = 0.0f;
                    }
                    approach_value(&data->vertical_speed, jump_speed, frame_time_sq_macro * 110.0f);
                }
            }
            FUN_L05_00307570_c((unsigned char *)moby);
            copy_racer_path_node((char *)moby, data->point_index, path_point);
            spring_angle(&moby->rot.z,
                         fast_arc_tan(path_point[0] - moby->pos.x, path_point[1] - moby->pos.y),
                         &data->yaw_speed,
                         0,
                         frame_scale_sq_macro * 0.035f,
                         frame_scale_sq_macro * 0.3f,
                         D_0015ED6C_gp * 6.981317f);
            if (data->pos[2] < path_point[2] - 5.0f) {
                data->pos[2] = path_point[2];
            }
            step[0] = fast_cos(moby->rot.z) * data->speed;
            step[1] = fast_sin(moby->rot.z) * data->speed;
            step[2] = 0.0f;
            fast_vec_add(data->pos, data->pos, step);
            fast_vec_scale(probe, step, 4.0f);
            fast_vec_add(probe, probe, data->pos);
            hit_count = func_L00_001F2BE8_308D68(probe, 0, moby, 0, 1.0f);
            if (hit_count != 0) {
                fast_vec_scale(push, step, 7.0f);
                for (j = 0; j < hit_count; j++) {
                    if (is_moby_in_class_range_500_to_540(D_L05_00178300[j]) != 0) {
                        spawn_nearby_effect(
                            (void *)D_L05_00178300[j], (int)moby, 0x30000, data->pos, push, 1.0f);
                    }
                }
            }
            ground = probe_ground_height(data->pos, 0, 0.5f);
            height_above_ground = data->pos[2] - ground;
            if (height_above_ground > 0.0f || data->vertical_speed > 0.0f) {
                data->vertical_speed -= frame_time_sq_macro * 21.0f;
            } else if (absolute_float(height_above_ground) < 0.5f) {
                data->vertical_speed = 0.0f;
                data->pos[2] = ground;
                data->jump_frames = 0;
            }
            data->pos[2] += data->vertical_speed;
            if (fast_dec_timer_s16(&data->launch_timer) != 0) {
                float target = data->cruise_speed;
                Hero *hero;

                if (ground < data->pos[2]) {
                    target = D_0015ED6C_gp * 19.0f;
                }
                hero = (Hero *)(((char *)&D_0013F350));
                if (hero->race_time_frames < scale_game_frames(0x2BC)) {
                    target += D_0015ED6C_gp * 3.0f;
                }
                if (hero->race_rank[data->racer_index] < hero->race_position) {
                    target -= D_0015ED6C_gp * 1.5f;
                } else {
                    target += D_0015ED6C_gp;
                }
                if (target < data->speed) {
                    approach_value(&data->speed, target, frame_time_sq_macro * 7.0f);
                } else {
                    approach_value(&data->speed, target, frame_time_sq_macro * 15.0f);
                }
            } else {
                approach_value(&data->speed, data->launch_speed, frame_time_sq_macro * 20.0f);
            }
            if (height_above_ground < 0.2f && data->vertical_speed < 0.0f) {
                data->pos[2] = ground;
                data->jump_frames = 0;
            }
            manipulator_set_scale(&data->m[3], 3.0f);
            max_turn = D_0015ED6C_gp * 2.6179938f;
            turn = data->yaw_speed;
            if (max_turn < turn) {
                turn = max_turn;
            } else {
                max_turn = -max_turn;
                if (turn < max_turn) {
                    turn = max_turn;
                }
            }
            if (moby->flags & 0x8000) {
                turn = -turn;
            }
            data->m[3].f68 = turn * 30.0f;
            data->m[0].f68 = turn * 20.0f;
            data->m[2].f68 = turn * 50.0f;
            data->m[1].f64 = turn * 40.0f;
            FUN_L00_002628d8((char *)moby,
                             (char *)&data->m[3],
                             3,
                             frame_scale_sq_macro * 0.02f,
                             frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby,
                             (char *)data,
                             0,
                             frame_scale_sq_macro * 0.02f,
                             frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby,
                             (char *)&data->m[2],
                             1,
                             frame_scale_sq_macro * 0.02f,
                             frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby,
                             (char *)&data->m[1],
                             2,
                             frame_scale_sq_macro * 0.015f,
                             frame_scale_sq_macro * 0.3f);
            qcopy(((float *)&moby->pos), data->pos);
            moby->pos.z += 0.25f;
            data->bob_phase = fast_add_rotations(data->bob_phase, D_0015ED6C_gp * 6.108652f);
            moby->pos.z += fast_sin(data->bob_phase) * 0.08f;
            if (data->child != 0) {
                Moby *child;

                *(long *)&data->child->spawn_frame = *(long *)&moby->spawn_frame;
                child = data->child;
                qcopy(((float *)&child->pos), ((float *)&moby->pos));
                qcopy(((float *)&child->rot), ((float *)&moby->rot));
                if (data->launch_timer != 0) {
                    child->command = 1;
                }
            }
        }
        break;
    }
    }
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_0030D230), where it is exact; names translated to the US level program. */

extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void fast_vec_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");

extern float floating_platform_rotation_y_phase_rate_degrees_gp __asm__("D_L05_00161CD4")
    __attribute__((sda));
extern float floating_platform_rotation_x_phase_rate_degrees_gp __asm__("D_L05_00161CD8")
    __attribute__((sda));
extern float floating_platform_rotation_y_amplitude_degrees_gp __asm__("D_L05_00161CDC")
    __attribute__((sda));
extern float floating_platform_rotation_x_amplitude_degrees_gp __asm__("D_L05_00161CE0")
    __attribute__((sda));
extern int floating_platform_voice_interval_min_gp __asm__("D_L05_00161CE4") __attribute__((sda));
extern int floating_platform_voice_interval_max_gp __asm__("D_L05_00161CE8") __attribute__((sda));
void       floating_platform_update(unsigned char *moby) __asm__("FUN_L05_0030bdd8");

void floating_platform_update(unsigned char *moby) {
    enum {
        FLOATING_PLATFORM_INITIALIZE = 0,
        FLOATING_PLATFORM_UPDATE_ROCKING = 1
    };
    char  *data = ((char *)MOBY(moby)->pvars);
    float  previous_position[4];
    float  previous_rotation[4];
    float  position_delta[4];
    char  *position;
    char  *current_rotation;
    float *previous_rotation_ptr;
    float  k;
    float  rotation_y_phase;
    switch (moby[0x20]) {
    case FLOATING_PLATFORM_INITIALIZE:
        *(int *)(data + 0x78) = truncate_float_to_s32(
            scale_time(random_float_between((float)floating_platform_voice_interval_min_gp,
                                            (float)floating_platform_voice_interval_max_gp)));
        *(float *)(data + 0x60) = random_angle_radians();
        *(float *)(data + 0x6C) = random_angle_radians();
        moby[0x20] = 1;
        break;
    case FLOATING_PLATFORM_UPDATE_ROCKING:
        if (fast_dec_timer((int *)(data + 0x78))) {
            allocate_voice_for_target_entry_alt(0, 0, (int)moby);
            *(int *)(data + 0x78) = truncate_float_to_s32(
                scale_time(random_float_between((float)floating_platform_voice_interval_min_gp,
                                                (float)floating_platform_voice_interval_max_gp)));
        }
        position = (char *)moby + 0x10;
        qcopy(previous_position, position);
        previous_rotation_ptr = previous_rotation;
        current_rotation = (char *)moby + 0x40;
        qcopy(previous_rotation_ptr, current_rotation);
        rotation_y_phase = fast_add_rotations(*(float *)(data + 0x60),
                                              floating_platform_rotation_y_phase_rate_degrees_gp *
                                                  DEG_TO_RAD * frame_time);
        k = floating_platform_rotation_x_phase_rate_degrees_gp * DEG_TO_RAD * frame_time;
        *(float *)(data + 0x60) = rotation_y_phase;
        *(float *)(data + 0x6C) = fast_add_rotations(*(float *)(data + 0x6C), k);
        MOBY(moby)->rot.y = floating_platform_rotation_y_amplitude_degrees_gp * DEG_TO_RAD *
                            fast_sin(*(float *)(data + 0x60));
        MOBY(moby)->rot.x = floating_platform_rotation_x_amplitude_degrees_gp * DEG_TO_RAD *
                            fast_sin(*(float *)(data + 0x6C));
        fast_vec_sub(position_delta, position, previous_position);
        update_entity_matrix(data + 0x20, position_delta, previous_rotation_ptr, current_rotation);
        break;
    }
}

extern char *D_L05_0015FFD8_c __asm__("D_L05_0015FFD8");
extern void convert_pose_to_reference_local(char *, char *, char *, char *, char *, char *) __asm__("FUN_L00_00260668");
extern void transform_attachment_pose(char *, char *, char *, char *, char *, char *) __asm__("FUN_L00_002604f0");
extern void FUN_L00_00260738_c(char *, float *, float *, char *) __asm__("FUN_L00_00260738");

void box_update(char *moby) __asm__("FUN_L05_0030bf98");

void box_update(char *moby) {
    enum {
        BOX_INITIALIZE = 0,
        BOX_UPDATE_TRANSFORM = 1
    };
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float previous_position[4];
    float previous_rotation[4];
    float position_delta[4];
    switch (MOBY(moby)->state) {
    case BOX_INITIALIZE:
        *(int *)(moby_data + 0x5C) = 1;
        if (*(int *)(moby_data + 0x80) >= 0) {
            convert_pose_to_reference_local(moby,
                                            D_L05_0015FFD8_c + (*(int *)(moby_data + 0x80) << 8),
                                            moby + 0x10,
                                            moby + 0x40,
                                            moby_data + 0x60,
                                            moby_data + 0x70);
            MOBY(moby)->state = 1;
        }
        break;
    case BOX_UPDATE_TRANSFORM:
        qcopy(previous_position, moby + 0x10);
        qcopy(previous_rotation, moby + 0x40);
        transform_attachment_pose(moby,
                                  D_L05_0015FFD8_c + (*(int *)(moby_data + 0x80) << 8),
                                  moby_data + 0x60,
                                  moby_data + 0x70,
                                  moby + 0x10,
                                  moby + 0x40);
        fast_vec_sub(position_delta, moby + 0x10, previous_position);
        FUN_L00_00260738_c(moby_data + 0x20, position_delta, previous_rotation, moby + 0x40);
        break;
    }
}

extern void enqueue_callback_list_1_30ca80(void (*)(void), void *) __asm__("FUN_001f4600");
extern void mark_moby_for_removal_30ca80(void *) __asm__("FUN_0020c828");

typedef struct {
    float raised_z;
    float lowered_z;
    int   starts_raised;
    int   rises;
    int   f10;
    int   record_index;
    int   timer;
    int   f1C;
    float speed;
    int   moving;
} WaterVars;

typedef struct {
    float         f0;
    float         f4;
    float         f8;
    float         fC;
    float         f10;
    float         f14;
    float         f18;
    float         f1C;
    float         f20;
    int           f24;
    int           f28;
    int           f2C;
    float         dir_x;
    float         dir_y;
    float         dir_z;
    unsigned char b3C;
    unsigned char b3D;
} Light;

typedef struct {
    char  pad0[0x10];
    float f10;
    float f14;
} Cam;

#include "rnc/overlay/water_grid.h"

extern void *D_L05_001612D0_macro __asm__("D_L05_001612D0") MACRO_ADDR;
extern void  record_grid_entry_neighbor_indices(int) __asm__("FUN_L05_0030d6a0");
extern void  set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern float advance_accelerated_scalar_30ca80(
    float *, float *, float, float, float, float) __asm__("FUN_00213f38");
void synchronize_group_water_heights(char *moby) __asm__("FUN_L05_0030d5c0");

extern void synchronize_group_water_heights(char *) __asm__("FUN_L05_0030d5c0");
extern void pool_splash(void *, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern int  check_scaled_vector_bsphere(char *, float) __asm__("FUN_L01_00275690");
extern void render_level_water_grids(void) __asm__("FUN_L05_0030d678");

extern Cam                  l05_water_light_heading_source __asm__("D_L05_00180340");
extern Light                light __asm__("D_L05_001CAD00");
extern int                  water_grid_fixed_point_bounds[][4] __asm__("D_L05_0020B440");
extern unsigned short       record_draw_values[] __asm__("D_L05_0020B380");
extern int                  rise_frames[] __asm__("D_L05_0020B3E0");
extern float                rise_speeds[] __asm__("D_L05_0020B3F8");
extern int                  base_rise_frames[] __asm__("D_L05_0020B410");
extern float                base_rise_speeds[] __asm__("D_L05_0020B428");
extern void *D_L05_001612D4 MACRO_ADDR;
extern unsigned char        D_L05_00161204[] MACRO_ADDR;
extern unsigned char        D_L05_00161205[] MACRO_ADDR;
extern unsigned char        D_L05_00161206[] MACRO_ADDR;
extern int D_L05_00161208   MACRO_ADDR;
extern float D_L05_0016120C MACRO_ADDR;
extern float D_L05_00161210 MACRO_ADDR;
extern float D_L05_00161214 MACRO_ADDR;
int                         D_L05_00161D10 = -1;
extern unsigned short       l05_water_record_draw_value_override __asm__("D_L05_00161D14")
    __attribute__((sda));
int                       hero_hits __asm__("D_L05_00161D1C") = 0;
int                       rise_step __asm__("D_L05_00161D20") = 0;
extern float              rise_checkpoint __asm__("D_L05_00161D24") __attribute__((sda));
float                     water __asm__("D_L05_00161D28") = 0;
int                       rise_checkpoint_hit __asm__("D_L05_00161D2C") = 0;
extern float              move_speed __asm__("D_L05_00161D30") __attribute__((sda));
extern float              move_accel __asm__("D_L05_00161D34") __attribute__((sda));
extern s32 D_L05_001612D8 MACRO_ADDR;
/* Update of the level 05 water mobys: sets up the 48-record water chain, moves the surface by state and keeps its record in step. */
void water_update(struct Moby *moby) __asm__("FUN_L05_0030ca80");

void water_update(struct Moby *moby) {
    enum {
        WATER_INITIALIZE = 0,
        WATER_MOVE_TO_RAISED_HEIGHT = 1,
        WATER_MOVE_TO_LOWERED_HEIGHT = 2,
        WATER_WAIT_FOR_RISE_TRIGGER = 3,
        WATER_RISE_COMPLETE = 7,
        WATER_TIMED_RISE = 8
    };
    WaterVars *water_vars = *(WaterVars **)&moby->pvars;

    switch (moby->state) {
    case WATER_INITIALIZE:
        if (water_vars->record_index == 0) {
            int   i;
            int   j;
            float light_direction_angle;

            water = moby->pos.z;
            rise_step = 0;
            rise_checkpoint_hit = 0;
            pool_init_tiles(l05_water_grids, 0x30);
            pool_set_scale(1.0f);
            light.f0 = 16.0f;
            light.f4 = 16.0f;
            light.f8 = -8.0f;
            light.fC = -8.0f;
            light.f10 = 1.0f;
            light.f14 = 1.0f;
            light.f18 = 0.9f;
            light.f20 = 0.1f;
            light_direction_angle = fast_arc_tan(l05_water_light_heading_source.f10,
                                                 l05_water_light_heading_source.f14);
            light.dir_x = fast_cos(light_direction_angle) * 0.57735002f;
            light.dir_y = fast_sin(light_direction_angle) * 0.57735002f;
            light.dir_z = -0.57735002f;
            light.f28 = 0x29;
            light.f2C = 0x2A;
            light.b3C = 0x1C;
            light.b3D = 0x40;
            D_L05_001612D0_macro = l05_water_grids;
            D_L05_001612D4 = water_grid_fixed_point_bounds;
            D_L05_001612D8 = 0x30;
            D_L05_00161204[0] = 0;
            D_L05_00161205[0] = 0xB;
            D_L05_00161206[0] = 0x3E;
            D_L05_00161208 = 0;
            D_L05_0016120C = 20480.0f;
            D_L05_00161210 = 255.0f;
            D_L05_00161214 = 26.0f;
            for (i = 0; i < 0x30; i++) {
                pool_bind_tile(&l05_water_grids[i].x, water_grid_fixed_point_bounds[i]);
                l05_water_grids[water_vars->record_index].draw_value =
                    record_draw_values[water_vars->record_index];
            }
            for (j = 0; j < 0x30; j++) {
                record_grid_entry_neighbor_indices(j);
            }
        }
        moby->draw_dist = 0;
        moby->flags |= 1;
        if (water_vars->rises != 0) {
            if (water_vars->f10 >= 0) {
                moby->update_dist = 0xFF;
                if (hero_hits >= 2) {
                    int   k;
                    float s = (float)(hero_hits - 1) * 0.014285714f + 1.0f;
                    if (s > 1.4f) {
                        s = 1.4f;
                    }
                    for (k = 0; k < 6; k++) {
                        rise_frames[k] = truncate_float_to_s32((float)base_rise_frames[k] * s);
                        rise_speeds[k] = base_rise_speeds[k] / s;
                    }
                }
            }
            if (visited52[moby->mission + (current_level_index << 4)] == 0xFF) {
                moby->state = 7;
                moby->pos.z = 44.0f;
            } else {
                moby->state = 3;
                water_vars->timer = scale_game_frames(0x3C);
                if (moby->pos.z < water_vars->lowered_z) {
                    l05_water_grids[water_vars->record_index].draw_value = 0;
                }
            }
        } else {
            if (water_vars->f10 < 0) {
                float z = moby->pos.z;
                water_vars->lowered_z = z;
                water_vars->raised_z = z + 4.5f;
                if (water_vars->starts_raised != 0) {
                    moby->state = 1;
                    moby->pos.z = water_vars->raised_z;
                } else {
                    moby->state = 2;
                    moby->pos.z = water_vars->lowered_z;
                }
            } else {
                moby->state = 2;
                moby->pos.z = water_vars->lowered_z;
            }
        }
        if (water_vars->record_index == 0x2F) {
            *(int *)&moby->collision = 0;
        }
        break;
    case WATER_MOVE_TO_LOWERED_HEIGHT:
        if (moby->command & 4) {
            water_vars->moving = 1;
            set_moby_list_state(moby->group, 1);
        } else if (water_vars->f10 >= 0) {
            if (visited52[moby->mission + (current_level_index << 4)] == 0xFF) {
                moby->state = 1;
                moby->pos.z = water_vars->raised_z;
            }
        }
        if (water_vars->moving != 0) {
            advance_accelerated_scalar_30ca80(&moby->pos.z,
                                              &water_vars->speed,
                                              water_vars->lowered_z,
                                              move_speed * D_0015ED70,
                                              move_speed * D_0015ED70,
                                              move_accel * D_0015ED6C_gp);
            synchronize_group_water_heights((char *)moby);
        }
        moby->command = 1;
        if (random_integer_below(0xC8) == 0) {
            float x =
                l05_water_grids[water_vars->record_index].x + random_float_between(-4.0f, 4.0f);
            float      r = random_float_between(-4.0f, 4.0f);
            WaterGrid *e = &l05_water_grids[water_vars->record_index];
            pool_splash(e, 1, 1, x, e->y + r, 1.0f, -0.05f);
        }
        break;
    case WATER_MOVE_TO_RAISED_HEIGHT:
        if (moby->command & 8) {
            water_vars->moving = 1;
            set_moby_list_state(moby->group, 2);
        } else {
            moby->command = 2;
        }
        if (water_vars->moving != 0) {
            advance_accelerated_scalar_30ca80(&moby->pos.z,
                                              &water_vars->speed,
                                              water_vars->raised_z,
                                              move_speed * D_0015ED70,
                                              move_speed * D_0015ED70,
                                              move_accel * D_0015ED6C_gp);
            synchronize_group_water_heights((char *)moby);
        }
        if (random_integer_below(0xC8) == 0) {
            float x =
                l05_water_grids[water_vars->record_index].x + random_float_between(-4.0f, 4.0f);
            float      r = random_float_between(-4.0f, 4.0f);
            WaterGrid *e = &l05_water_grids[water_vars->record_index];
            pool_splash(e, 1, 1, x, e->y + r, 1.0f, -0.05f);
        }
        break;
    case WATER_WAIT_FOR_RISE_TRIGGER:
        if (vector_distance_xy(((float *)&moby->pos), hero.motion.pos.f) < 16.0f) {
            Hero *h = &hero;
            if (h->motion.pos.f[2] < moby->pos.z) {
                if (h->state.control_mode == 0x11) {
                    if (h->water_surface_z < moby->pos.z) {
                        h->water_surface_z = moby->pos.z;
                    }
                }
            }
        }
        if (water_vars->f10 >= 0) {
            if (vector_distance_xy(((float *)&moby->pos), hero.motion.pos.f) < 10.0f) {
                Hero *h = &hero;
                if (h->ground_moby != 0 && h->air_frames.s == 0 &&
                    h->ground_moby->oclass == 0x33E) {
                    water_vars->f1C = 0;
                    moby->state = 8;
                    rise_step = 0;
                    water_vars->timer = scale_game_frames(rise_frames[0]);
                }
            }
        }
        if (random_integer_below(0x32) == 0) {
            float x =
                l05_water_grids[water_vars->record_index].x + random_float_between(-4.0f, 4.0f);
            float      r = random_float_between(-4.0f, 4.0f);
            WaterGrid *e = &l05_water_grids[water_vars->record_index];
            pool_splash(e, 1, 1, x, e->y + r, 1.0f, -0.01f);
        }
        break;
    case WATER_RISE_COMPLETE:
        if (water_vars->f10 >= 0) {
            Hero *h = &hero;
            if (h->state.current == 0x6A && h->state_timer == 2) {
                hero_hits++;
            }
        } else if (water_vars->raised_z < moby->pos.z) {
            l05_water_grids[water_vars->record_index].draw_value = 0;
            mark_moby_for_removal_30ca80(moby);
            return;
        }
        break;
    case WATER_TIMED_RISE:
        if (fast_dec_timer(&water_vars->timer) != 0) {
            if (rise_step < 5) {
                rise_step++;
                water_vars->timer = scale_game_frames(rise_frames[rise_step]);
            }
        }
        moby->pos.z += rise_speeds[rise_step] * D_0015ED6C_gp;
        synchronize_group_water_heights((char *)moby);
        water = moby->pos.z;
        {
            Vec4 rise_checkpoint_position;

            rise_checkpoint_position.q = 0;
            rise_checkpoint_position.f[0] = 211.0f;
            rise_checkpoint_position.f[1] = 338.0f;
            rise_checkpoint_position.f[2] = 25.0f;
            if (fast_vec_dist(rise_checkpoint_position.f, &hero.motion.pos) < 6.0f) {
                if (rise_checkpoint_hit == 0) {
                    rise_checkpoint_hit = 1;
                    if (moby->pos.z > rise_checkpoint) {
                        rise_step = 5;
                    }
                }
            }
        }
        if (water_vars->f10 >= 0) {
            Hero *h = &hero;
            if (h->state.current == 0x6A && h->state_timer == 2) {
                hero_hits++;
            }
        }
        if (moby->pos.z >= 44.0f) {
            set_moby_list_state(moby->group, 7);
        }
        break;
    }
    if (water_vars->record_index >= 0) {
        WaterGrid *e = &l05_water_grids[water_vars->record_index];
        float      restore_record_draw_value;

        e->z = moby->pos.z;
        restore_record_draw_value = 0.0f;
        if (check_scaled_vector_bsphere((char *)moby, 48.0f) != -1 &&
            (!(moby->state >= 3 && moby->state <= 7) ||
             (moby->pos.z > water_vars->lowered_z && moby->pos.z < water_vars->raised_z))) {
            restore_record_draw_value = 1.0f;
        }
        if (restore_record_draw_value == 0.0f) {
            l05_water_grids[water_vars->record_index].draw_value = 0;
        } else {
            l05_water_grids[water_vars->record_index].draw_value =
                record_draw_values[water_vars->record_index];
        }
        if (water_vars->record_index == 0) {
            pool_flush((char *)l05_water_grids, 0x30);
            enqueue_callback_list_1_30ca80(render_level_water_grids, moby);
        }
        if (water_vars->record_index == D_L05_00161D10) {
            l05_water_grids[water_vars->record_index].draw_value =
                l05_water_record_draw_value_override;
            record_draw_values[water_vars->record_index] = l05_water_record_draw_value_override;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_0030EA88), where it is exact; names translated to the US level program. */

extern short *D_L05_001ABCC0[];

void synchronize_group_water_heights(char *moby) {
    char  *data = ((char *)MOBY(moby)->pvars);
    short *group_member_list = D_L05_001ABCC0[MOBY(moby)->group];
    char(*base)[256] = (char(*)[256])D_L05_0015FFD8;
    do {
        int   group_member_moby_index = *group_member_list & 0x7FFF;
        char *linked_water_moby = (char *)(group_member_moby_index * 256 + (int)base);
        if (MOBY(linked_water_moby)->oclass == 0x33F) {
            char      *linked_water_data = ((char *)MOBY(linked_water_moby)->pvars);
            WaterGrid *water_record;
            float      linked_water_height;
            if (*(int *)(data + 0xC) != 0 ||
                *(float *)(linked_water_data + 4) == *(float *)(data + 4)) {
                linked_water_height = MOBY(moby)->pos.z;
            } else {
                linked_water_height =
                    *(float *)(linked_water_data + 4) + (MOBY(moby)->pos.z - *(float *)(data + 4));
            }
            MOBY(linked_water_moby)->pos.z = linked_water_height;
            water_record = &l05_water_grids[*(int *)(linked_water_data + 0x14)];
            water_record->z = *(float *)(base[group_member_moby_index] + 0x18);
        }
    } while (*group_member_list++ >= 0);
}

s32 D_L05_001CAD28[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CAD2C[1] __attribute__((section(".data"))) = {0};

f32 D_L05_001CAD30[1] __attribute__((section(".data"))) = {0};

f32 D_L05_001CAD34[1] __attribute__((section(".data"))) = {0};

f32 D_L05_001CAD38[1] __attribute__((section(".data"))) = {0};

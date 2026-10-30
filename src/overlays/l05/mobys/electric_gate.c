/* Level 05 moby classes: electric_gate, drain_cover, pipe_mounted_grating, elevator_with_button, ocean_rilgar, qwarks_bouncer, shady_salesman, television, taxi, qwarks_trailer. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/watch.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"
#include "rnc/rendering/geometry_quad.h"
#include "eetypes.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_pool.h"
#include "sda.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/audio/streaming/initialize_npc_dialogue.h"
#include "rnc/overlay/shared/audio/streaming/update_npc_dialogue.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/vendor/set_camera_rotation.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/overlay/shared/audio/streaming/set_moby_dialogue_record_value.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/overlay/shared/props/trackdrive_left.h"
#include "rnc/overlay/shared/mobys/record_moby_context_bytes.h"
#include "rnc/overlay/shared/enemies/blarg_helicopter.h"
#include "rnc/overlay/shared/props/hydrodisplacer_faucet.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/ui/help/help_request.h"

/* Taxi stop selection shared by route helpers. */
enum TaxiStop {
    TAXI_PAIRED_ROUTE_FIRST_STOP = 0x0,
    TAXI_PAIRED_ROUTE_SECOND_STOP = 0x1,
    TAXI_BRANCHING_STOP = 0x2,
    TAXI_PRIMARY_BRANCH_STOP = 0x3,
    TAXI_FALLBACK_BRANCH_STOP = 0x4
};

/* records, for entry arg, which of the 48 entries sit one grid step away */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0030EB68), where it is exact; names translated to the US level program. */

#include "rnc/overlay/water_grid.h"

void record_grid_entry_neighbor_indices(int selected_entry_index) __asm__("FUN_L05_0030d6a0");

void record_grid_entry_neighbor_indices(int selected_entry_index) {
    int        i;
    WaterGrid *entry_table = l05_water_grids;
    for (i = 0; i < 48; i++) {
        if (selected_entry_index != i) {
            WaterGrid *selected_entry = &entry_table[selected_entry_index];
            float      x = selected_entry->x - entry_table[i].x;
            float      y = selected_entry->y - entry_table[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    selected_entry->neighbors[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    selected_entry->neighbors[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    selected_entry->neighbors[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    selected_entry->neighbors[3] = i;
            } else {
                if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(x - 16.0f) < 0.1f)
                        selected_entry->neighbors[5] = i;
                    if (absolute_float(x + 16.0f) < 0.1f)
                        selected_entry->neighbors[7] = i;
                } else if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(x - 16.0f) < 0.1f)
                        selected_entry->neighbors[4] = i;
                    if (absolute_float(x + 16.0f) < 0.1f)
                        selected_entry->neighbors[6] = i;
                }
            }
        }
    }
}

#include "qcopy.h"
#include "rnc/overlay/level_path.h"

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0030F130), where it is exact; names translated to the US level program. */

extern char  D_0013E550[];
extern char  D_L05_001671C0[];
extern int   D_L05_0015F5CC_e[] __asm__("D_L05_0015F5C4") MACRO_ADDR;
extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern short D_L05_0015FFD8_d __asm__("D_L05_0015FFD8");
extern void  draw_electric_gate_beams(struct Moby *moby) __asm__("FUN_L05_0030de90");
void         add_draw_callback(int arg0, int arg1) __asm__("FUN_001f4600");

extern short electric_gate_beam_callback_frame_gp __asm__("D_L05_00161D38") __attribute__((sda));
void         electric_gate_update(char *m) __asm__("FUN_L05_0030dc68");

void electric_gate_update(char *gate_moby) {
    enum {
        ELECTRIC_GATE_INITIALIZE = 0,
        ELECTRIC_GATE_WAIT_FOR_TRIGGER = 1,
        ELECTRIC_GATE_TRIGGERED = 2
    };
    char *data = ((char *)MOBY(gate_moby)->pvars);
    int   state = MOBY(gate_moby)->state;
    char *base;
    char *tab;
    char *gate_position;
    char *voice_position_base;
    char *voice_owner;
    float gate_reference_distance;
    int   voice_slot_index;
    switch (state) {
    case ELECTRIC_GATE_INITIALIZE:
        MOBY(gate_moby)->flags |= 1;
        MOBY(gate_moby)->state = 1;
        MOBY(gate_moby)->visible = 0;
        **(int **)(data + 4) = -1;
        break;
    case ELECTRIC_GATE_WAIT_FOR_TRIGGER:
        if (*(int *)&electric_gate_beam_callback_frame_gp != D_L05_0015F5CC_e[2]) {
            *(int *)&electric_gate_beam_callback_frame_gp = D_L05_0015F5CC_e[2];
            add_draw_callback(draw_electric_gate_beams, gate_moby);
        }
        gate_position = gate_moby + 0x10;
        tab = D_L05_001671C0;
        if (fast_vec_dist(gate_position, tab) < 64.0f) {
            voice_slot_index = **(int **)(data + 4);
            if (voice_slot_index >= 0) {
                base = D_0013E550;
                voice_owner = *(char **)(base + voice_slot_index * 0x70 + 0x88);
                if (voice_owner != 0) {
                    if (*(short *)(voice_owner + 0xA6) == 0x346) {
                        if (voice_owner != gate_moby) {
                            if (*(unsigned char *)(voice_owner + 0x20) == state) {
                                gate_reference_distance = fast_vec_dist(gate_position, tab);
                                if (!(gate_reference_distance <
                                      fast_vec_dist(voice_owner + 0x10, tab)))
                                    goto after;
                            }
                            voice_position_base = base + 0x90;
                            *(char **)(base + **(int **)(data + 4) * 0x70 + 0x88) = gate_moby;
                            qcopy(**(int **)(data + 4) * 0x70 + voice_position_base, gate_position);
                        }
                        goto after;
                    }
                }
            }
            **(int **)(data + 4) = allocate_voice_for_target_entry(0, 4, gate_moby);
        }
    after:
        if (*(int *)data >= 0) {
            if ((*(unsigned char **)&D_L05_0015FFD8_d)[*(int *)data * 256 + 0xBC] != 0) {
                allocate_voice_for_target_entry(1, 0, gate_moby);
                MOBY(gate_moby)->collision = 0;
                MOBY(gate_moby)->state = 2;
            }
        }
        break;
    case ELECTRIC_GATE_TRIGGERED:
        if (((int)MOBY(gate_moby)->collision) != 0)
            MOBY(gate_moby)->collision = 0;
        break;
    }
}

extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern int l05_electric_gate_outer_color_blend_start __asm__("D_L05_00161D3C") __attribute__((sda));
extern int l05_electric_gate_outer_color_blend_end __asm__("D_L05_00161D40") __attribute__((sda));
extern int l05_electric_gate_inner_color_blend_start __asm__("D_L05_00161D44") __attribute__((sda));
extern int l05_electric_gate_inner_color_blend_end __asm__("D_L05_00161D48") __attribute__((sda));
extern int l05_electric_gate_blend_minuend_selector __asm__("D_L05_00161D70") __attribute__((sda));
extern int l05_electric_gate_blend_subtrahend_selector __asm__("D_L05_00161D74")
    __attribute__((sda));
extern int l05_electric_gate_blend_alpha_factor_selector __asm__("D_L05_00161D78")
    __attribute__((sda));
extern int l05_electric_gate_blend_addend_selector __asm__("D_L05_00161D7C") __attribute__((sda));
extern int l05_electric_gate_beam_effect_texture_index __asm__("D_L05_00161D98")
    __attribute__((sda));
extern float  D_L05_0020B740[4][2];
extern Vec4   l05_electric_gate_outer_beam_base_positions[4] __asm__("D_L05_0020B760");
extern Vec4   l05_electric_gate_inner_beam_base_positions[4] __asm__("D_L05_0020B7A0");
extern short *D_L05_001ABCC0[];
extern int    frame_number __asm__("D_L05_0015F5CC");

void draw_electric_gate_beams(struct Moby *moby) {
    struct GeometryQuad quads[2][5];
    char                matrix[0x40];
    u64                 texture;
    int                 i, j, k, vertex_index, outer_color, inner_color, offset;
    float               angle, blend;
    unsigned short     *list;
    unsigned short     *next;
    short               id;

    angle = convert_integer_to_float(frame_number & 0x3F) / 63.0f * 6.28318f;
    for (i = 0; i < 5; i++) {
        blend = (fast_sin(angle) + 1.0f) * 0.5f;
        outer_color = fast_tween_color(l05_electric_gate_outer_color_blend_start,
                                       l05_electric_gate_outer_color_blend_end,
                                       blend);
        inner_color = fast_tween_color(l05_electric_gate_inner_color_blend_start,
                                       l05_electric_gate_inner_color_blend_end,
                                       blend);
        angle = fast_add_rotations(angle, 1.2566371f);
        for (j = 0; j < 2; j++) {
            texture = get_effect_texture(l05_electric_gate_beam_effect_texture_index);
            quads[j][i].primitive = (s64)l05_electric_gate_blend_minuend_selector |
                                    ((s64)l05_electric_gate_blend_subtrahend_selector << 2) |
                                    ((s64)l05_electric_gate_blend_alpha_factor_selector << 4) |
                                    ((s64)l05_electric_gate_blend_addend_selector << 6) |
                                    0x8000000000LL;
            quads[j][i].texture = texture;
            quads[j][i].texture_state = 0xFF9000000260LL;
            quads[j][i].reserved.value = 0;
            for (vertex_index = 0; vertex_index < 4; vertex_index++) {
                quads[j][i].texture_coordinates[vertex_index][0] = D_L05_0020B740[vertex_index][0];
                quads[j][i].texture_coordinates[vertex_index][1] = D_L05_0020B740[vertex_index][1];
                if (j != 0) {
                    quads[j][i].colors[vertex_index] = outer_color;
                    qcopy(&quads[j][i].positions[vertex_index],
                          &l05_electric_gate_outer_beam_base_positions[vertex_index]);
                    quads[j][i].positions[vertex_index].f[2] += (float)(i + 1);
                } else {
                    quads[0][i].colors[vertex_index] = inner_color;
                    qcopy(&quads[0][i].positions[vertex_index],
                          &l05_electric_gate_inner_beam_base_positions[vertex_index]);
                    quads[0][i].positions[vertex_index].f[2] += (float)(i + 1);
                }
            }
        }
    }

    list = (unsigned short *)D_L05_001ABCC0[moby->group];
    do {
        char *entry;
        offset = (*list & 0x7FFF) << 8;
        entry = (char *)(offset + (int)*(char **)&D_L05_0015FFD8_d);
        next = list + 1;
        if (*(short *)(entry + 0xA6) == 0x346 && *(unsigned char *)(entry + 0x20) == 1) {
            copy_matrix_rotation(matrix, entry + 0xC0);
            qcopy(matrix + 0x30, (char *)(offset + (int)*(char **)&D_L05_0015FFD8_d) + 0x10);
            *(float *)(matrix + 0x3C) = 1.0f;
            for (k = 0; k < 5; k++) {
                draw_geometry_quad(&quads[0][k], matrix, 0);
                draw_geometry_quad(&quads[1][k], matrix, 0);
            }
        }
        id = *(short *)list;
        list = next;
    } while (id >= 0);
}

/* Fog zone: switches the level fog between its two settings when the hero enters either trigger volume, and
 * tints a colour by the hero's side of a gate while inside the third. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0030F6C0), where it is exact; names translated to the US level program. */

extern float D_L05_0015F448;
extern float D_L05_0015F44C;
extern float D_L05_0015F450;
extern float D_L05_0015F454;

#include "rnc/overlay/cuboid.h"

extern unsigned char D_L05_0015F444;
extern unsigned char D_L05_0015F445;
extern unsigned char D_L05_0015F446;
extern unsigned char l05_volume_blended_color_upper_middle_byte __asm__("D_L05_00161202");
extern unsigned char l05_volume_blended_color_high_byte __asm__("D_L05_00161203");
extern unsigned char l05_volume_blended_color_lower_middle_byte __asm__("D_L05_00161201");
extern unsigned char l05_volume_blended_color_low_byte __asm__("D_L05_00161200");

void update_fog_zone(struct Moby *moby) __asm__("FUN_L05_0030e1f8");

void update_fog_zone(struct Moby *moby) {
    enum {
        FOG_ZONE_INITIALIZE = 0,
        FOG_ZONE_WAIT_TO_APPLY_PRESET = 1,
        FOG_ZONE_WAIT_TO_RESTORE_SAVED_SETTINGS = 2
    };
    char *data = ((char *)moby->pvars);
    float hero_offset_from_volume_origin[4];
    float hero_position_in_volume_space[4];
    char *hero_position = (char *)&hero.motion.pos;
    if (is_point_inside_clip_volume(hero_position, *(int *)(data + 0x28))) {
        *(float *)(hero_position + 0x270) = 500.0f;
    }
    switch (((unsigned char *)moby)[0x20]) {
    case FOG_ZONE_INITIALIZE:
        data[0x20] = D_L05_0015F444;
        data[0x21] = D_L05_0015F445;
        data[0x22] = D_L05_0015F446;
        {
            float f0 = D_L05_0015F448;
            float f1 = D_L05_0015F44C;
            float f2 = D_L05_0015F450;
            float f3 = D_L05_0015F454;
            *(float *)(data + 0) = f0;
            *(float *)(data + 8) = f1;
            *(float *)(data + 4) = f2;
            *(float *)(data + 0xC) = f3;
        }
        data[0x23] = 0x49;
        data[0x24] = 0x53;
        data[0x25] = 0xC;
        *(float *)(data + 0x18) = 33792.0f;
        *(float *)(data + 0x14) = 255.0f;
        *(int *)(data + 0x10) = 0;
        *(int *)(data + 0x1C) = 0;
        {
            int c = l05_volume_blended_color_low_byte +
                    (l05_volume_blended_color_lower_middle_byte << 8) +
                    (l05_volume_blended_color_upper_middle_byte << 16) +
                    (l05_volume_blended_color_high_byte << 24);
            *(int *)(data + 0x34) = 0x40408070;
            *(int *)(data + 0x30) = c;
        }
        moby->state = 1;
        break;
    case FOG_ZONE_WAIT_TO_APPLY_PRESET:
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0x2C))) {
            D_L05_0015F444 = data[0x23];
            D_L05_0015F445 = data[0x24];
            D_L05_0015F446 = data[0x25];
            {
                float f0 = *(float *)(data + 0x10);
                float f1 = *(float *)(data + 0x18);
                float f2 = *(float *)(data + 0x14);
                float f3 = *(float *)(data + 0x1C);
                D_L05_0015F448 = f0;
                D_L05_0015F44C = f1;
                D_L05_0015F450 = f2;
                D_L05_0015F454 = f3;
            }
            moby->state = 2;
        }
        break;
    case FOG_ZONE_WAIT_TO_RESTORE_SAVED_SETTINGS:
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0x28))) {
            D_L05_0015F444 = data[0x20];
            D_L05_0015F445 = data[0x21];
            D_L05_0015F446 = data[0x22];
            {
                float f0 = *(float *)(data + 0);
                float f1 = *(float *)(data + 8);
                float f2 = *(float *)(data + 4);
                float f3 = *(float *)(data + 0xC);
                D_L05_0015F448 = f0;
                D_L05_0015F44C = f1;
                D_L05_0015F450 = f2;
                D_L05_0015F454 = f3;
            }
            moby->state = 1;
        }
        break;
    }
    hero_position = (char *)&hero.motion.pos;
    if (is_point_inside_clip_volume(hero_position, *(int *)(data + 0x38))) {
        int c;
        fast_vec_sub(
            hero_offset_from_volume_origin, hero_position, &l05_cuboids[*(int *)(data + 0x38)].pos);
        hero_offset_from_volume_origin[3] = 0.0f;
        transform_vector_by_basis(hero_position_in_volume_space,
                                  hero_offset_from_volume_origin,
                                  l05_cuboids[*(int *)(data + 0x38)].to_local);
        c = fast_tween_color(*(int *)(data + 0x30),
                             *(int *)(data + 0x34),
                             (hero_position_in_volume_space[0] + 1.0f) * 0.5f);
        l05_volume_blended_color_lower_middle_byte = c >> 8;
        l05_volume_blended_color_upper_middle_byte = c >> 16;
        l05_volume_blended_color_low_byte = c;
        l05_volume_blended_color_high_byte = c >> 24;
    }
}

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

/* Slides the moby out along its pose-table entry's direction: 0 places it,
   1 waits for the trigger byte 0xBC of its D_L05_0015FFD8 record, 2 eases
   it toward the end point, 3 lowers it to 5.9 below the entry's height. */
void update_triggered_moby_slide_and_lowering(char *moby) __asm__("FUN_L05_0030e508");

void update_triggered_moby_slide_and_lowering(char *moby) {
    enum {
        MOBY_PLACE_AT_POSE_OFFSET = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_SLIDE_TO_END_POINT = 2,
        MOBY_LOWER_BELOW_POSE_HEIGHT = 3
    };
    char        *data = ((char *)MOBY(moby)->pvars);
    float        slide_motion_scratch[4];
    float        slide_step_distance;
    float        speed;
    LevelCuboid *pose_entry;
    float        remaining_slide_distance;
    float        reach;

    switch (MOBY(moby)->state) {
    case MOBY_PLACE_AT_POSE_OFFSET: {
        LevelCuboid *initial_pose_entry;

        initial_pose_entry = &l05_cuboids[*(int *)(data + 0x10)];
        qcopy(moby + 0x10, &initial_pose_entry->pos);
        MOBY(moby)->rot.z = initial_pose_entry->rot.z;
        fast_vec_normalize(
            slide_motion_scratch, &initial_pose_entry->axes[1], *(float *)(data + 0x18));
        if (*(int *)(data + 0x1C) != 0) {
            fast_vec_scale(slide_motion_scratch, slide_motion_scratch, -1.0f);
            MOBY(moby)->flags |= 0x8000;
        }
        fast_vec_add(moby + 0x10, moby + 0x10, slide_motion_scratch);
        MOBY(moby)->state = 1;
        break;
    }
    case MOBY_WAIT_FOR_TRIGGER:
        if ((*(unsigned char **)&D_L05_0015FFD8_d)[*(int *)(data + 0x14) * 256 + 0xBC] != 0) {
            allocate_voice_for_target_entry(0, 0, moby);
            MOBY(moby)->state = 2;
            clear_u64_value(data);
        }
        break;
    case MOBY_SLIDE_TO_END_POINT:
        pose_entry = &l05_cuboids[*(int *)(data + 0x10)];
        reach = *(float *)(data + 0x18) * 3.0f;
        slide_step_distance = 0.0f;
        fast_vec_normalize(slide_motion_scratch, &pose_entry->axes[1], reach);
        if (*(int *)(data + 0x1C) != 0) {
            fast_vec_scale(slide_motion_scratch, slide_motion_scratch, -1.0f);
        }
        fast_vec_add(slide_motion_scratch, &pose_entry->pos, slide_motion_scratch);
        fast_vec_sub(slide_motion_scratch, slide_motion_scratch, moby + 0x10);
        remaining_slide_distance = fast_vec_length(slide_motion_scratch);
        speed = fast_vec_length(data);
        advance_accelerated_scalar(remaining_slide_distance,
                                   frame_time_sq * 20.0f,
                                   frame_time_sq * 30.0f,
                                   frame_time * 10.0f,
                                   &slide_step_distance,
                                   &speed);
        fast_vec_normalize(data, slide_motion_scratch, speed);
        fast_vec_add(moby + 0x10, moby + 0x10, data);
        if (fast_vec_length(slide_motion_scratch) < 0.01f) {
            MOBY(moby)->state = 3;
        }
        break;
    case MOBY_LOWER_BELOW_POSE_HEIGHT:
        pose_entry = l05_cuboids;
        pose_entry += *(int *)(data + 0x10);
        advance_accelerated_scalar(pose_entry->pos.z - 5.9f,
                                   frame_time_sq * 20.0f,
                                   frame_time_sq * 40.0f,
                                   frame_time * 10.0f,
                                   (float *)&MOBY(moby)->pos.z,
                                   (float *)(data + 8));
        if (MOBY(moby)->pos.z < pose_entry->pos.z - 5.9f) {
            MOBY(moby)->state = 4;
        }
        break;
    }
}

void emit_drain_cover_particles(char *particle_source_moby,
                                float lower_height_bound,
                                float upper_height_bound) __asm__("FUN_L05_0030f218");

void emit_drain_cover_particles(char *, float, float) __asm__("FUN_L05_0030f218");
void update_drain_cover_tilt(char *moby,
                             float turn_scale,
                             float speed_scale) __asm__("FUN_L05_0030f408");

void         update_drain_cover_tilt(char *, float, float) __asm__("FUN_L05_0030f408");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void approach_position(float, float, float, float, void *, void *) __asm__("FUN_00213f38");
extern void
spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern float l05_drain_cover_gravity __asm__("D_L05_00161DAC") __attribute__((sda));
extern float l05_drain_cover_hover_acceleration __asm__("D_L05_00161DB0") __attribute__((sda));
extern int   D_L05_00161DB8 __asm__("D_L05_00161DB8") __attribute__((sda));
extern float l05_drain_cover_pre_rise_particle_height_span __asm__("D_L05_00161DE8")
    __attribute__((sda));
extern float l05_drain_cover_pre_rise_particle_radial_speed_lower_bound __asm__("D_L05_00161DEC")
    __attribute__((sda));
extern float l05_drain_cover_pre_rise_particle_radial_speed_upper_bound __asm__("D_L05_00161DF0")
    __attribute__((sda));
extern int l05_drain_cover_pre_rise_particle_configuration __asm__("D_L05_00161DF4")
    __attribute__((sda));
extern int   D_L05_00161DF8 __asm__("D_L05_00161DF8") __attribute__((sda));
extern float l05_drain_cover_pre_rise_particle_vertical_speed_upper_bound __asm__("D_L05_00161DFC")
    __attribute__((sda));
extern float l05_drain_cover_nearby_effect_distance_limit __asm__("D_L05_00161E00")
    __attribute__((sda));
extern float D_L05_00161DD4 __asm__("D_L05_00161DD4") __attribute__((sda));
extern float D_0013F3D0[];
extern int   D_L05_0015F5C4;

typedef struct {
    char           pad00[0x10];
    float          pos[4];
    char           pad20[0x10];
    unsigned char  f30;
    unsigned char  f31;
    short          f32;
    unsigned short flags;
} DrainCoverLid;

typedef struct {
    char           pad00[0x20];
    char           f20[0x40];
    float          vel[4];
    float          home[4];
    float          f80;
    float          f84;
    float          f88;
    int            timer;
    char           pad90[8];
    float          f98;
    float          f9C;
    float          fA0;
    float          fA4;
    int            sound;
    DrainCoverLid *lid;
} DrainCoverData;

typedef struct {
    char            pad00[0x10];
    float           pos[4];
    unsigned char   state;
    char            pad21[0x1F];
    float           rot[4];
    char            pad50[0x28];
    DrainCoverData *d;
} DrainCoverMoby;

typedef struct {
    char  pad00[0x80];
    float pos[4];
    char  pad90[0x1FF0];
    int   f2080;
} DrainCoverHero;

void drain_cover_update(DrainCoverMoby *moby) __asm__("FUN_L05_0030e7f8");

void drain_cover_update(DrainCoverMoby *moby) {
    enum {
        DRAIN_COVER_INITIALIZE = 0,
        DRAIN_COVER_WAIT_TO_RISE = 1,
        DRAIN_COVER_RISE = 2,
        DRAIN_COVER_HOVER = 3,
        DRAIN_COVER_FALL_TO_HOME = 4
    };
    DrainCoverData *data = moby->d;
    float           old_position[4];
    float           old_rotation[4];
    float           difference[4];
    float           effect_velocity[4];
    float           effect_position[4];
    float           particle_position[4];
    int             r, g, bl, particle_configuration, b;
    int             i;
    int             index;
    char           *entry;
    DrainCoverHero *hero;
    DrainCoverLid  *lid;
    float           minimum_rise_height;
    float           particle_offset_angle;

    qcopy(old_position, moby->pos);
    qcopy(old_rotation, moby->rot);
    if (moby->state >= 2) {
        if (voice_is_playing(moby, data->sound) == 0) {
            data->sound = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else if (voice_is_playing(moby, data->sound) != 0) {
        index = data->sound;
        if (index != -1) {
            entry = D_0013E550 + index * 0x70;
            if (*(DrainCoverMoby **)(entry + 0x88) == moby &&
                *(unsigned char *)(entry + 0x74) != 0) {
                release_voice_slot(index);
            }
        }
        data->sound = -1;
    }
    switch (moby->state) {
    case DRAIN_COVER_INITIALIZE:
        moby->state = 1;
        data->timer = scale_game_frames(0x78);
        data->sound = 0xFF;
        qcopy(data->home, moby->pos);
        data->f98 = random_angle_radians();
        data->f9C = random_angle_radians();
        data->fA0 = randf_sym(70.0f, 120.0f) * 0.017453292f * frame_time;
        data->fA4 = randf_sym(70.0f, 120.0f) * 0.017453292f * frame_time;
        minimum_rise_height = data->f84 + 1.0f;
        if (data->f80 <= minimum_rise_height) {
            data->f80 = minimum_rise_height;
        }
        break;
    case DRAIN_COVER_WAIT_TO_RISE:
        update_drain_cover_tilt((char *)moby, 0.0f, 8.0f);
        if (D_L05_0015F5C4 == 0 && vector_distance_xy(moby->pos, D_0013F3D0) < 24.0f &&
            fast_dec_timer(&data->timer) != 0) {
            moby->state = 2;
            clear_u64_value(data->vel);
            data->vel[2] =
                square_root(data->f80 * 2.0f * (l05_drain_cover_gravity * frame_time_sq));
        } else if ((float)data->timer < (float)scale_game_frames(*(int *)&D_L05_00161DB8) * 0.5f) {
            emit_drain_cover_particles((char *)moby,
                                       data->home[2],
                                       l05_drain_cover_pre_rise_particle_height_span +
                                           data->home[2]);
            emit_drain_cover_particles((char *)moby,
                                       data->home[2],
                                       l05_drain_cover_pre_rise_particle_height_span +
                                           data->home[2]);
            if ((float)data->timer < (float)scale_game_frames(*(int *)&D_L05_00161DB8) * 0.2f) {
                particle_configuration = l05_drain_cover_pre_rise_particle_configuration;
                b = D_L05_00161DF8;
                r = truncate_float_to_s32(scale_time(random_float_between(10.0f, 20.0f)));
                g = truncate_float_to_s32(scale_time(random_float_between(10.0f, 20.0f)));
                bl = truncate_float_to_s32(scale_time(random_float_between(10.0f, 40.0f)));
                for (i = 0; i < 20; i++) {
                    particle_offset_angle = random_angle_radians();
                    particle_position[0] = fast_cos(particle_offset_angle) * 0.75f;
                    particle_position[1] = fast_sin(particle_offset_angle) * 0.75f;
                    particle_position[2] = 0.0f;
                    fast_vec_normalize(
                        effect_velocity,
                        particle_position,
                        random_float_between(
                            l05_drain_cover_pre_rise_particle_radial_speed_lower_bound,
                            l05_drain_cover_pre_rise_particle_radial_speed_upper_bound) *
                            frame_time);
                    effect_velocity[2] =
                        random_float_between(
                            0.0f, l05_drain_cover_pre_rise_particle_vertical_speed_upper_bound) *
                        frame_time;
                    set_vector_length_xy(effect_position, effect_velocity, 0.0f);
                    fast_vec_add(particle_position, moby->pos, particle_position);
                    particle_position[2] =
                        random_float_between(moby->pos[2] - 0.25f, moby->pos[2] + 0.5f);
                    effect_velocity[3] = effect_position[3] = random_float_between(0.5f, 1.0f);
                    spawn_spark(particle_position,
                                effect_velocity,
                                effect_position,
                                particle_configuration,
                                b,
                                r,
                                g,
                                bl,
                                0x30);
                }
            }
        }
        break;
    case DRAIN_COVER_RISE:
        if ((frame_number & 3) == 0) {
            play_entity_sound((int)moby, 1.0f, moby->pos, 0x10000, 2.0f, 1.0f, 0, 1, 0);
        }
        emit_drain_cover_particles(
            (char *)moby,
            data->home[2],
            data->vel[2] * (float)scale_game_frames(*(int *)&D_L05_00161DB8) * 0.5f + moby->pos[2]);
        update_drain_cover_tilt((char *)moby, D_L05_00161DD4, 1.0f);
        fast_vec_add(moby->pos, moby->pos, data->vel);
        data->vel[2] = data->vel[2] - l05_drain_cover_gravity * frame_time_sq;
        if (data->vel[2] <= 0.0f) {
            moby->state = 3;
            data->home[3] = data->f84 - 0.5f;
            data->timer = truncate_float_to_s32(scale_time(data->f88));
        }
        break;
    case DRAIN_COVER_HOVER:
        emit_drain_cover_particles((char *)moby, data->home[2], moby->pos[2]);
        update_drain_cover_tilt((char *)moby, D_L05_00161DD4, 1.0f);
        approach_position(data->home[2] + data->home[3],
                          l05_drain_cover_hover_acceleration * frame_time_sq,
                          l05_drain_cover_hover_acceleration * frame_time_sq,
                          frame_time * 20.0f,
                          &moby->pos[2],
                          &data->vel[2]);
        if (data->vel[2] == 0.0f) {
            if (data->home[3] > data->f84) {
                data->home[3] = data->f84 - 0.5f;
            } else {
                data->home[3] = data->f84 + 0.5f;
            }
        }
        if (fast_dec_timer(&data->timer) != 0) {
            moby->state = 4;
        }
        break;
    case DRAIN_COVER_FALL_TO_HOME:
        emit_drain_cover_particles(
            (char *)moby,
            data->home[2],
            data->vel[2] * (float)scale_game_frames(*(int *)&D_L05_00161DB8) + moby->pos[2]);
        update_drain_cover_tilt((char *)moby,
                                data->home[2] + 1.0f < moby->pos[2] ? D_L05_00161DD4 : 0.0f,
                                data->home[2] + 1.0f < moby->pos[2] ? 1.0f : 10.0f);
        fast_vec_add(moby->pos, moby->pos, data->vel);
        data->vel[2] = data->vel[2] - l05_drain_cover_gravity * frame_time_sq;
        if (moby->pos[2] < data->home[2]) {
            allocate_voice_for_target_entry(1, 0, moby);
            moby->state = 1;
            data->timer = scale_game_frames(0x78);
            qcopy(moby->pos, data->home);
            data->f98 = random_angle_radians();
            data->f9C = random_angle_radians();
            data->fA0 = randf_sym(70.0f, 120.0f) * 0.017453292f * frame_time;
            data->fA4 = randf_sym(70.0f, 120.0f) * 0.017453292f * frame_time;
        }
        break;
    }
    hero = (DrainCoverHero *)(D_0013E550 + 0xE00);
    if (moby->pos[2] - 2.0f > hero->pos[2] &&
        vector_distance_xy(moby->pos, hero->pos) < l05_drain_cover_nearby_effect_distance_limit) {
        effect_velocity[0] =
            fast_cos(fast_arc_tan(hero->pos[0] - moby->pos[0], hero->pos[1] - moby->pos[1])) * 0.2f;
        effect_velocity[1] =
            fast_sin(fast_arc_tan(hero->pos[0] - moby->pos[0], hero->pos[1] - moby->pos[1])) * 0.2f;
        effect_velocity[2] = 0.0f;
        qcopy(effect_position, hero->pos);
        effect_position[2] += 0.75f;
        spawn_nearby_effect((void *)hero->f2080,
                            (int)moby,
                            1,
                            effect_position,
                            effect_velocity,
                            effect_velocity[2]);
    }
    fast_vec_sub(difference, moby->pos, old_position);
    update_entity_matrix(data->f20, difference, old_rotation, moby->rot);
    if (data->lid == 0) {
        data->lid = create_moby(0x388);
        if (data->lid != 0) {
            data->lid->f30 = 0;
            data->lid->flags |= 1;
            data->lid->f32 = 0;
            data->lid->f31 = 0;
            lid = data->lid;
            qcopy(lid->pos, moby->pos);
            update_entity(lid);
        }
    } else {
        lid = data->lid;
        qcopy(lid->pos, moby->pos);
        update_entity(lid);
    }
}
/* Scatters points along a range, placing each one with a heading and radius. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_003106E0), where it is exact; names translated to the US level program. */

extern float FUN_L00_00257c48(float lo, float hi);
extern short D_L05_00161DB8_gp __asm__("D_L05_00161DB8") __attribute__((sda));
extern void  create_particle_with_velocity_and_lifetime(
     float, void *, void *, int, int, int) __asm__("FUN_L05_0029bbe0");

extern short drain_cover_particle_height_span_per_particle_gp __asm__("D_L05_00161DB4")
    __attribute__((sda));
extern short drain_cover_particle_scalar_lower_bound_gp __asm__("D_L05_00161DBC")
    __attribute__((sda));
extern short drain_cover_particle_scalar_upper_bound_gp __asm__("D_L05_00161DC0")
    __attribute__((sda));
extern short drain_cover_particle_size_sampling_bound_gp __asm__("D_L05_00161DC4")
    __attribute__((sda));
extern short drain_cover_particle_vertical_speed_gp __asm__("D_L05_00161DC8") __attribute__((sda));
extern short drain_cover_particle_color_gp __asm__("D_L05_00161DCC") __attribute__((sda));
extern short drain_cover_particle_spawn_radius_upper_bound_gp __asm__("D_L05_00161DD0")
    __attribute__((sda));
void emit_drain_cover_particles(char *particle_source_moby,
                                float lower_height_bound,

                                float upper_height_bound) {
    float v[4];
    float particle_position[4];
    float ground, x, y, spawn_height_candidate, u;
    int   i, j, k, particle_size, particle_color;
    clear_u64_value(v);
    v[2] = *(float *)&drain_cover_particle_vertical_speed_gp * frame_time;
    ground = v[2] * 0.75f * convert_integer_to_float(*(int *)&D_L05_00161DB8_gp);
    j = 0;
    for (i = j; (float)i < (upper_height_bound - lower_height_bound) /
                               *(float *)&drain_cover_particle_height_span_per_particle_gp;
         i++) {
        float fi = (float)j;
        x = random_float_between(fi, *(float *)&drain_cover_particle_spawn_radius_upper_bound_gp);
        y = random_angle_radians();
        particle_position[0] = fast_cos(y) * x;
        particle_position[1] = fast_sin(y) * x;
        particle_position[2] = fi;
        fast_vec_add(particle_position, particle_source_moby + 0x10, particle_position);
        particle_position[2] =
            random_float_between(lower_height_bound, upper_height_bound) - ground;
        spawn_height_candidate =
            random_float_between((lower_height_bound + upper_height_bound) * 0.5f,
                                 upper_height_bound) -
            ground;
        if (spawn_height_candidate < particle_position[2])
            particle_position[2] = spawn_height_candidate;
        u = random_float_between(*(float *)&drain_cover_particle_scalar_lower_bound_gp,
                                 *(float *)&drain_cover_particle_scalar_upper_bound_gp);
        k = scale_game_frames(*(int *)&D_L05_00161DB8_gp);
        particle_size = truncate_float_to_s32(
            FUN_L00_00257c48(fi, (float)*(int *)&drain_cover_particle_size_sampling_bound_gp));
        particle_color = *(int *)&drain_cover_particle_color_gp;
        create_particle_with_velocity_and_lifetime(
            u, particle_position, v, k, particle_size, particle_color);
    }
}

extern float D_L05_00161DD8 __attribute__((sda));

#include "rnc/overlay/l05/mobys/electric_gate_globals.h"

extern float drain_cover_tilt_acceleration_gp __asm__("D_L05_00161DDC") __attribute__((sda));
extern float drain_cover_tilt_deceleration_gp __asm__("D_L05_00161DE0") __attribute__((sda));
extern float drain_cover_tilt_max_speed_gp __asm__("D_L05_00161DE4") __attribute__((sda));
extern float
approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");

void update_drain_cover_tilt(char *moby, float turn_scale, float speed_scale) {
    char        *data = (char *)MOBY(moby)->pvars;
    struct Hero *game = &hero;
    float        offset[4];
    float        horizontal;
    float        vertical;
    float        angle0;
    float        angle1;
    float        target0;
    float        target1;
    float        rate0;
    float        rate1;
    float        limit;
    float        dot0;
    float        dot1;

    if ((char *)game->ground_moby == moby && game->air_frames.s == 0) {
        fast_vec_sub(offset, &game->motion.pos, &MOBY(moby)->pos);
        dot0 = fast_vec_dot(offset, &MOBY(moby)->rot_row0);
        dot1 = fast_vec_dot(offset, &MOBY(moby)->rot_row1);
        horizontal = turn_scale * dot0;
        vertical = -turn_scale * dot1;
    } else {
        horizontal = 0.0f;
        vertical = 0.0f;
    }

    *(float *)(data + 0x98) = fast_add_rotations(*(float *)(data + 0x98), *(float *)(data + 0xa0));
    *(float *)(data + 0x9c) = fast_add_rotations(*(float *)(data + 0x9c), *(float *)(data + 0xa4));
    angle0 = fast_sin(*(float *)(data + 0x98));
    target0 = fast_add_rotations(vertical, angle0 * turn_scale * D_L05_00161DD8);
    angle1 = fast_sin(*(float *)(data + 0x9c));
    target1 = fast_add_rotations(horizontal, angle1 * turn_scale * D_L05_00161DD8);

    rate0 = drain_cover_tilt_acceleration_gp * speed_scale * 0.017453292f * frame_time_sq;
    rate1 = drain_cover_tilt_deceleration_gp * speed_scale * 0.017453292f * frame_time_sq;
    limit = drain_cover_tilt_max_speed_gp * 0.017453292f * frame_time;
    approach_angle(&MOBY(moby)->rot.x, target0, (float *)(data + 0x90), rate0, rate1, limit);
    approach_angle(&MOBY(moby)->rot.y,
                   target1,
                   (float *)(data + 0x94),
                   drain_cover_tilt_acceleration_gp * speed_scale * 0.017453292f * frame_time_sq,
                   drain_cover_tilt_deceleration_gp * speed_scale * 0.017453292f * frame_time_sq,
                   drain_cover_tilt_max_speed_gp * 0.017453292f * frame_time);
}

extern void *find_moby_hit(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern void  random_vector(void *, float, float) __asm__("FUN_L00_00257d78");
extern u8   *FUN_L00_00272060(
      void *pos, s32 life, f32 size0, f32 size1, s32 color, s32 mode, s32 spin, void *vel, f32 f2);
extern char pipe_mounted_grating_command_records[] __asm__("D_L05_001BAAA0");

void pipe_mounted_grating_update(struct Moby *moby) __asm__("FUN_L05_0030f5c8");

void pipe_mounted_grating_update(struct Moby *moby) {
    float        piece_initial_motion_step[4];
    float        spin_vel[4];
    float        particle_spawn_offset[4];
    float        drift[4];
    float        pos[4];
    struct Moby *piece;
    float        angle;
    float        life;
    float        speed;
    int          j, spin;
    if (find_moby_hit(moby, 0xB30000, 0) != 0) {
        piece = create_moby(0x3D4);
        if (piece != 0) {
            char *vars;
            piece->spawn_frame = moby->spawn_frame;
            piece->visible = 1;
            vars = (char *)piece->pvars;
            piece->draw_dist = 0x40;
            piece->update_dist = 0x40;
            qcopy(&piece->pos, &moby->pos);
            qcopy(&piece->rot, &moby->rot);
            speed = random_float_between(5.0f, 12.0f);
            fast_vec_scale(piece_initial_motion_step, &moby->rot_row0, -(speed * frame_time));
            clear_u64_value(spin_vel);
            initialize_moby_launch_motion(
                piece, moby, piece_initial_motion_step, spin_vel, scale_game_frames(60));
            *(int *)(vars + 0xC) = 0x200;
        }
        allocate_voice_for_target_entry(0, 0, piece);
        angle = 0.0f;
        {
            int i;
            for (i = 0; i < 6; i++) {
                angle = fast_add_rotations(angle, 1.0471976f);
                piece = create_moby(0x3D5);
                if (piece != 0) {
                    char *vars = (char *)piece->pvars;
                    *(u128 *)spin_vel = 0;
                    *(u128 *)drift = 0;
                    spin_vel[2] = 0.75f;
                    drift[1] = fast_sin(angle) * 1.4f;
                    drift[2] = fast_cos(angle) * 1.4f;
                    *(u128 *)particle_spawn_offset = *(u128 *)drift;
                    transform_vector_by_basis(
                        particle_spawn_offset, particle_spawn_offset, &moby->rot_row0);
                    for (j = 0; j < 10; j++) {
                        float size;
                        random_vector(drift, frame_time * 1.5f, frame_time * 2.0f);
                        fast_vec_scale(pos, drift, 5.0f);
                        fast_vec_add(pos, pos, particle_spawn_offset);
                        fast_vec_add(pos, pos, &moby->pos);
                        spin = random_integer_below(2);
                        if (spin == 0) {
                            spin--;
                        }
                        size = random_float_between(0.4f, 0.5f);
                        FUN_L00_00272060(pos,
                                         scale_game_frames(25),
                                         size * 0.2f,
                                         size,
                                         0x7F207F7F,
                                         0,
                                         spin,
                                         drift,
                                         0.0f);
                        spin = -spin;
                        FUN_L00_00272060(pos,
                                         scale_game_frames(25),
                                         size * 0.14f,
                                         size * 0.7f,
                                         0x7F7F7F7F,
                                         1,
                                         spin,
                                         drift,
                                         0.0f);
                    }
                    piece->spawn_frame = moby->spawn_frame;
                    piece->draw_dist = 0x40;
                    piece->visible = 1;
                    piece->update_dist = 0x40;
                    qcopy(&piece->pos, &moby->pos);
                    qcopy(&piece->rot, &moby->rot);
                    speed = random_float_between(0.0f, 24.0f);
                    life = random_float_between(60.0f, 120.0f);
                    fast_vec_scale(
                        piece_initial_motion_step, &moby->rot_row0, -(speed * frame_time));
                    clear_u64_value(spin_vel);
                    initialize_moby_launch_motion(piece,
                                                  moby,
                                                  piece_initial_motion_step,
                                                  spin_vel,
                                                  truncate_float_to_s32(scale_time(life)));
                    *(int *)(vars + 0xC) = 0x200;
                    piece->rot.x = angle;
                    update_entity(piece);
                }
            }
        }
        moby->command = 1;
        record_moby_context_bytes(&moby->command, 1, moby, 1, pipe_mounted_grating_command_records);
        mark_moby_for_removal(moby);
    } else if (moby->command) {
        mark_moby_for_removal(moby);
    }
}

struct GateVars {
    s32 trigger; /* D_L05_0015FFD8 index of the moby whose byte 0xBC opens it */
    s32 riseTimer;
    s32 swingTimer;
    f32 topZ;
    f32 riseSpeed;
    f32 swingSpeed;
};

extern float gate_rise_acceleration_gp __asm__("D_L05_00161E80") __attribute__((sda));
extern float gate_rise_deceleration_gp __asm__("D_L05_00161E84") __attribute__((sda));
extern float gate_rise_max_speed_gp __asm__("D_L05_00161E88") __attribute__((sda));
extern float gate_swing_acceleration_gp __asm__("D_L05_00161E8C") __attribute__((sda));
extern float gate_swing_deceleration_gp __asm__("D_L05_00161E90") __attribute__((sda));
extern float gate_swing_max_speed_gp __asm__("D_L05_00161E94") __attribute__((sda));
extern int   gate_mobys_314eb0 __asm__("D_L05_0015FFD8");
extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float FUN_L00_0025be00(float *, float *, float, float, float, float);

/* Gate: waits for its trigger moby, then rises to topZ and swings its yaw back to 0, with sounds per class. */
void update_triggered_gate_motion(struct Moby *gate_moby) __asm__("FUN_L05_00314eb0");

void update_triggered_gate_motion(struct Moby *gate_moby) {
    enum {
        GATE_INITIALIZE = 0,
        GATE_WAIT_FOR_TRIGGER = 1,
        GATE_RAISE_AND_SWING = 2
    };
    struct GateVars *gate_vars = (struct GateVars *)gate_moby->pvars;
    struct Moby     *trigger_moby;

    switch (gate_moby->state) {
    case GATE_INITIALIZE:
        gate_vars->topZ = gate_moby->pos.z + 1.0f;
        gate_moby->state = 1;
        gate_moby->pos.z -= 8.0f;
        break;
    case GATE_WAIT_FOR_TRIGGER:
        trigger_moby = (struct Moby *)((gate_vars->trigger << 8) + gate_mobys_314eb0);
        if (trigger_moby->command == 1) {
            gate_moby->state = 2;
            break;
        }
        if (trigger_moby->command == 2) {
            gate_moby->pos.z = gate_vars->topZ;
            gate_moby->state = 3;
            gate_moby->rot.y = 0.0f;
        }
        break;
    case GATE_RAISE_AND_SWING:
        if (fast_dec_timer(&gate_vars->riseTimer)) {
            advance_accelerated_scalar(gate_vars->topZ,
                                       gate_rise_acceleration_gp * frame_time_sq,
                                       gate_rise_deceleration_gp * frame_time_sq,
                                       gate_rise_max_speed_gp * frame_time,
                                       &gate_moby->pos.z,
                                       &gate_vars->riseSpeed);
            if (gate_moby->oclass == 0x355 &&
                absolute_float(gate_moby->pos.z - gate_vars->topZ) < 3.0f &&
                3.0f < absolute_float(gate_vars->topZ - gate_moby->pos.z + gate_vars->riseSpeed))
                allocate_voice_for_target_entry(0, 0, gate_moby);
        }
        if (fast_dec_timer(&gate_vars->swingTimer)) {
            FUN_L00_0025be00(&gate_moby->rot.y,
                             &gate_vars->swingSpeed,
                             0.0f,
                             gate_swing_acceleration_gp * 0.017453292f * frame_time_sq,
                             gate_swing_deceleration_gp * 0.017453292f * frame_time_sq,
                             gate_swing_max_speed_gp * 0.017453292f * frame_time);
            if (gate_moby->rot.y == 0.0f)
                gate_moby->state = 3;
        } else if (gate_vars->swingTimer == 1 && gate_moby->oclass == 0x354) {
            allocate_voice_for_target_entry(0, 0, gate_moby);
        }
        break;
    }
}

/* pvars of the 0x357 emitter: once woken it sprays particles along its facing (unkC0) that fall to the
   floor height of its D_L05_001612D0 row and splash there, and keeps the shared voice at the emitter nearest D_L05_001671C0. */
struct EmitterVars {
    s32  unk0;
    s32  floor;   /* row of D_L05_001612D0 */
    s32  trigger; /* D_L05_0015FFD8 index of the moby whose byte 0xBC wakes it, -1: none */
    s32 *voice;   /* voice slot shared by the emitters, -1: none */
    s32  age;     /* frames since it started spraying */
};

/* D_0013E550 seen from slot n (0x70 bytes per slot) */
struct VoiceSlot {
    u8           pad0[0x88];
    struct Moby *owner;
    u8           pad8C[4];
    Vec4f        pos;
};

/* 0x1190-byte rows; word 8 is the floor height the emitter particles land on */
struct FloorRow {
    u8  pad0[8];
    f32 z;
    u8  padC[0x1190 - 0xC];
};

extern void set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern void create_particle_with_velocity_and_randomized_parameters(
    float, float, float, void *, void *, int, int) __asm__("FUN_L05_0029be70");
extern void          spawn_splash(float, float, void *, void *, void *) __asm__("FUN_L00_002718d0");
extern unsigned char D_0014C050[][16];
extern struct Moby  *D_L05_0015FFD8;
extern struct FloorRow *D_L05_001612D0;
extern char             D_L05_0015F580_macro[] __asm__("D_L05_0015F580") MACRO_ADDR;

#include "rnc/overlay/l05/mobys/electric_gate_data/update_triggered_particle_spray_emitter.h"

void update_triggered_particle_spray_emitter(struct Moby *emitter_moby) __asm__("FUN_L05_003150f0");

void update_triggered_particle_spray_emitter(struct Moby *emitter_moby) {
    enum {
        EMITTER_INITIALIZE = 0,
        EMITTER_WAIT_FOR_TRIGGER = 1,
        EMITTER_SPRAY_PARTICLES = 2
    };
    float               v[4];
    float               particle_velocity[4];
    float               u[4];
    struct EmitterVars *emitter_vars = (struct EmitterVars *)emitter_moby->pvars;
    float               floor;
    float               minimum_particle_speed;
    float               particle_speed, second_particle_speed_sample, third_particle_speed_sample;
    float               scale;
    int                 kind;
    int                 big;
    int                 i;
    unsigned char       state;

    if (emitter_vars == 0) {
        return;
    }
    switch (state = emitter_moby->state) {
    case EMITTER_INITIALIZE:
        if (D_0014C050[current_level_index][emitter_moby->mission] != 0xFF) {
            emitter_moby->state = 1;
        } else {
            emitter_moby->state = 3;
        }
        break;
    case EMITTER_WAIT_FOR_TRIGGER:
        if (emitter_vars->trigger >= 0 && D_L05_0015FFD8[emitter_vars->trigger].command != 0) {
            set_moby_list_state(emitter_moby->group, 2);
        }
        break;
    case EMITTER_SPRAY_PARTICLES:
        emitter_vars->age++;
        if (fast_vec_dist(&emitter_moby->pos, D_L05_001671C0) < 64.0f) {
            struct Moby *voice_owner;
            if (*emitter_vars->voice >= 0 &&
                (voice_owner =
                     ((struct VoiceSlot *)(D_0013E550 + *emitter_vars->voice * 0x70))->owner) !=
                    0 &&
                voice_owner->oclass == 0x357) {
                if (voice_owner != emitter_moby &&
                    (voice_owner->state != state ||
                     fast_vec_dist(&emitter_moby->pos, D_L05_001671C0) <
                         fast_vec_dist(&voice_owner->pos, D_L05_001671C0))) {
                    fast_vec_normalize(v, &emitter_moby->rot_row0, 2.5f);
                    fast_vec_add(
                        &((struct VoiceSlot *)(D_0013E550 + *emitter_vars->voice * 0x70))->pos,
                        v,
                        &emitter_moby->pos);
                    ((struct VoiceSlot *)(D_0013E550 + *emitter_vars->voice * 0x70))->owner =
                        emitter_moby;
                }
            } else {
                *emitter_vars->voice = allocate_voice_for_target_entry(0, 0xD, emitter_moby);
                if (*emitter_vars->voice >= 0) {
                    fast_vec_normalize(v, &emitter_moby->rot_row0, 2.5f);
                    fast_vec_add(
                        &((struct VoiceSlot *)(D_0013E550 + *emitter_vars->voice * 0x70))->pos,
                        v,
                        &emitter_moby->pos);
                }
            }
        }
        floor = D_L05_001612D0[emitter_vars->floor].z;
        if (emitter_moby->pos.z < floor) {
            emitter_moby->state = 3;
        }
        for (i = 0; (float)i < spray_particle_count_limit_per_update_gp; i++) {
            big = 1;
            if (spray_big_particle_selection_threshold_percent_gp <
                random_float_between(0.0f, 100.0f)) {
                big = 0;
            }
            minimum_particle_speed = spray_big_particle_speed_lower_bound_gp;
            if (!big) {
                minimum_particle_speed = spray_small_particle_speed_lower_bound_gp;
            }
            qcopy(v, &emitter_moby->pos);
            fast_vec_normalize(
                u,
                &emitter_moby->rot_row1,
                random_float_between(-spray_particle_lateral_spread_and_height_offset_gp,
                                     spray_particle_lateral_spread_and_height_offset_gp));
            fast_vec_add(v, v, u);
            particle_speed =
                random_float_between(minimum_particle_speed, spray_particle_speed_upper_bound_gp);
            second_particle_speed_sample =
                random_float_between(minimum_particle_speed, spray_particle_speed_upper_bound_gp);
            third_particle_speed_sample =
                random_float_between(minimum_particle_speed, spray_particle_speed_upper_bound_gp);
            if (particle_speed < second_particle_speed_sample) {
                particle_speed = second_particle_speed_sample;
            }
            if (particle_speed < third_particle_speed_sample) {
                particle_speed = third_particle_speed_sample;
            }
            fast_vec_normalize(
                particle_velocity, &emitter_moby->rot_row0, particle_speed * frame_time);
            if (big) {
                scale = spray_big_particle_base_scale_gp;
                kind = spray_big_particle_kind_gp;
                v[2] += spray_particle_lateral_spread_and_height_offset_gp - scale +
                        random_float_between(-0.4f, 0.4f);
            } else {
                scale = spray_small_particle_base_scale_gp;
                kind = spray_small_particle_kind_gp;
                v[2] += spray_particle_lateral_spread_and_height_offset_gp - scale + 0.4f;
            }
            create_particle_with_velocity_and_randomized_parameters(
                scale * random_float_between(0.8f, 1.2f),
                floor,
                frame_time_sq * 10.0f,
                v,
                particle_velocity,
                kind,
                big);
            if (random_integer_below(2)) {
                /* fall time from the emitter to the floor */
                float fall_time_frames =
                    square_root(2.0f * (emitter_moby->pos.z - floor) / (frame_time_sq * 10.0f));
                if (fall_time_frames < (float)emitter_vars->age) {
                    float splash_forward_distance = fall_time_frames * (frame_time * 4.0f);
                    float splash_offset_angle = random_angle_radians();
                    float splash_offset_radius = random_float_between(0.0f, 1.25f);
                    fast_vec_scale(v, &emitter_moby->rot_row0, splash_forward_distance);
                    fast_vec_add(v, v, &emitter_moby->pos);
                    v[0] += fast_cos(splash_offset_angle) * splash_offset_radius;
                    v[1] += fast_sin(splash_offset_angle) * splash_offset_radius;
                    v[2] = floor + 0.05f;
                    spawn_splash(random_float_between(1.0f, 1.5f),
                                 random_integer_below(2) ? 2.0f : -2.0f,
                                 v,
                                 D_L05_0015F580_macro,
                                 &D_L05_001612D0[emitter_vars->floor].z);
                }
            }
        }
        break;
    }
}

/* pvars of the lift: it rides between two heights with the hero standing on it. */
typedef struct {
    u8   pad0[0x20];
    char rider[0x40]; /* 0x20: carried-object state for FUN_L00_00260738 */
    f32  top;         /* 0x60 */
    f32  bottom;      /* 0x64 */
    s32  marker;      /* 0x68: marker ring in D_L05_001B0930 and camera id, -1: none */
    s32  pose;        /* 0x6C: index into l05_cuboids */
    f32  progress;    /* 0x70: 0 at the bottom, 1 at the top */
    s32  sound;       /* 0x74: voice slot, -1: none */
    f32  fade;        /* 0x78 */
    s32  timer;       /* 0x7C */
    f32  phase;       /* 0x80: glow pulse angle */
    f32  base_yaw;    /* 0x84 */
    f32  speed;       /* 0x88 */
} LiftVars;

typedef struct {
    u32          handle;
    u8           state;
    u8           pad5[0x13];
    struct Moby *owner; /* 0x18 */
    u8           pad1C[0x54];
} LiftVoice;

typedef struct {
    u8        header[0x70];
    LiftVoice voices[30];
} LiftVoicePool;

/* The pool seen from slot n: its voice field is voices[n]. */
typedef struct {
    u8        header[0x70];
    LiftVoice voice;
} LiftVoiceWindow;

extern u16 D_0014162A NOT_SDA;

extern f32  approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32  approach_with_speed(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern void FUN_L00_00260738(char *a, void *b, void *c, void *d);
extern void lift_camera_view(void *, void *, void *) __asm__("FUN_L05_00315ee0");
extern void place_marker_ring(void *) __asm__("FUN_L05_00315f70");

void elevator_with_button_update(struct Moby *moby) __asm__("FUN_L05_003156d0");

void elevator_with_button_update(struct Moby *moby) {
    enum {
        ELEVATOR_INITIALIZE = 0,
        ELEVATOR_FADE_BEFORE_CAMERA_MOUNT = 1,
        ELEVATOR_MOVE_TO_TOP = 2,
        ELEVATOR_MOVE_TO_BOTTOM = 3,
        ELEVATOR_WAIT_AT_TOP = 4,
        ELEVATOR_WAIT_AT_BOTTOM = 5,
        ELEVATOR_WAIT_TO_RETURN_FROM_TOP = 6,
        ELEVATOR_WAIT_TO_RETURN_FROM_BOTTOM = 7
    };
    LiftVars *vars = (LiftVars *)moby->pvars;
    Vec4      delta;
    Vec4      rot;
    Vec4      eye;
    Vec4      look;
    f32       scale;
    f32       progress_speed_limit;
    f32       max_speed;
    f32       target;
    s32       level;

    fast_vec_scale(&delta, &moby->pos, -1.0f);
    qcopy(&rot, &moby->rot);
    switch (moby->state) {
    case ELEVATOR_INITIALIZE:
        vars->phase = random_angle_radians();
        vars->sound = -1;
        vars->base_yaw = moby->rot.z;
        moby->state = 3;
        break;
    case ELEVATOR_FADE_BEFORE_CAMERA_MOUNT:
        D_0014162A = vars->marker;
        approach_value(&vars->fade, 1.0f, 0.133f);
        elevator_camera_transition_fade = vars->fade;
        if (vars->fade == 1.0f) {
            lift_camera_view(moby, &eye, &look);
            jet_mount(&eye, &look, 1, 0, 0);
            moby->state = moby->command;
            moby->command = 1;
        }
        break;
    case ELEVATOR_MOVE_TO_TOP:
    case ELEVATOR_MOVE_TO_BOTTOM:
        if (!voice_is_playing(moby, vars->sound)) {
            vars->sound = allocate_voice_for_target_entry(0, 4, moby);
        }
        if (moby->command) {
            D_0014162A = vars->marker;
            approach_value(&vars->fade, 0.0f, 0.133f);
            elevator_camera_transition_fade = vars->fade;
            lift_camera_view(moby, &eye, &look);
            set_camera_position(&eye);
            set_camera_rotation(&look);
            scale = 1.0f / (vars->top - vars->bottom);
            progress_speed_limit = frame_time * (moby->command ? 6.0f : 16.0f) * scale;
            max_speed = frame_time_sq * (moby->command ? 6.0f : 16.0f) * scale;
        } else {
            progress_speed_limit = frame_time;
            max_speed = frame_time_sq;
        }
        target = moby->state == 2 ? 1.0f : 0.0f;
        approach_with_speed(
            &vars->progress, &vars->speed, target, max_speed, max_speed, progress_speed_limit);
        moby->pos.z = (vars->top - vars->bottom) * vars->progress + vars->bottom;
        moby->rot.z =
            fast_subtract_rotations(l05_cuboids[vars->pose].rot.z, vars->base_yaw) * vars->progress;
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->base_yaw);
        if (absolute_float(vars->progress - target) < 0.01f) {
            vars->timer = scale_game_frames(120);
            if (vars->sound != -1) {
                LiftVoiceWindow *window =
                    (LiftVoiceWindow *)((u8 *)&voice_pool + vars->sound * sizeof(LiftVoice));
                if (window->voice.owner == moby && window->voice.state != 0) {
                    release_voice_slot(vars->sound);
                }
            }
            vars->sound = -1;
            if (vars->speed > 0.00125f) {
                vars->speed = 0.00125f;
            } else if (vars->speed < -0.00125f) {
                vars->speed = -0.00125f;
            }
            if (moby->command) {
                if (moby->state == 2) {
                    moby->state = 6;
                } else {
                    moby->state = 7;
                }
            } else {
                if (moby->state == 2) {
                    moby->state = 4;
                } else {
                    moby->state = 5;
                }
            }
        }
        break;
    case ELEVATOR_WAIT_TO_RETURN_FROM_TOP:
    case ELEVATOR_WAIT_TO_RETURN_FROM_BOTTOM:
        if (voice_is_playing(moby, vars->sound)) {
            release_voice_slot(vars->sound);
            vars->sound = -1;
        }
        if (vars->timer > scale_game_frames(30) ||
            vector_distance_xy(&hero.motion.pos, &moby->pos) > 0.5f) {
            fast_dec_timer(&vars->timer);
        }
        if (hero.ground_moby != moby) {
            jet_end_mount(1);
            if (moby->state == 7) {
                moby->state = 5;
            } else {
                moby->state = 4;
            }
            break;
        }
        if (vars->timer != 0) {
            break;
        }
        vars->phase = fast_add_rotations(vars->phase, frame_time * 6.2831855f);
        level = truncate_float_to_s32((fast_sin(vars->phase) * 4.0f - 3.0f) * 128.0f);
        if (level > 0x80) {
            level = 0x80;
        } else if (level < 0x20) {
            level = 0x20;
        }
        moby->glow_color = 0x80000000 | (level << 16) | (level << 8) | level;
        if (hero.ground_moby == moby && hero.air_frames.s == 0 &&
            vector_distance_xy(&hero.motion.pos, &moby->pos) < 0.5f) {
            allocate_voice_for_target_entry(1, 0, moby);
            moby->glow_color = 0x80208020;
            if (moby->state == 7) {
                moby->state = 2;
            } else {
                moby->state = 3;
            }
        }
        break;
    case ELEVATOR_WAIT_AT_TOP:
    case ELEVATOR_WAIT_AT_BOTTOM:
        vars->phase = fast_add_rotations(vars->phase, frame_time * 6.2831855f);
        level = truncate_float_to_s32((fast_sin(vars->phase) * 4.0f - 3.0f) * 128.0f);
        if (level > 0x80) {
            level = 0x80;
        } else if (level < 0x20) {
            level = 0x20;
        }
        moby->glow_color = 0x80000000 | (level << 16) | (level << 8) | level;
        if (hero.ground_moby == moby && hero.air_frames.s == 0 &&
            vector_distance_xy(&hero.motion.pos, &moby->pos) < 0.5f) {
            allocate_voice_for_target_entry(1, 0, moby);
            if (vars->marker != -1) {
                place_marker_ring(moby);
            }
            moby->glow_color = 0x80208020;
            if (moby->state == 5) {
                moby->command = 2;
            } else {
                moby->command = 3;
            }
            moby->state = 1;
            vars->fade = 0.0f;
        } else if (moby->state == 4) {
            f32 dist = vector_distance_xy(&moby->pos, &hero.motion.pos);
            if (absolute_float(hero.ground_z.f - vars->bottom) < 2.0f && dist < 32.0f &&
                dist > 5.0f) {
                moby->command = 0;
                moby->state = 3;
            }
        } else if (moby->state == 5) {
            f32 dist = vector_distance_xy(&moby->pos, &hero.motion.pos);
            if (absolute_float(hero.ground_z.f - vars->top) < 2.0f && dist < 32.0f && dist > 5.0f) {
                moby->command = 0;
                moby->state = 2;
            }
        }
        break;
    }
    fast_vec_add(&delta, &delta, &moby->pos);
    FUN_L00_00260738(vars->rider, &delta, &rot, &moby->rot);
}

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_003173A8), where it is exact; names translated to the US level program. */

extern short lift_camera_height_offset_gp __asm__("D_L05_00161EC0") __attribute__((sda));
extern short lift_camera_pitch_degrees_gp __asm__("D_L05_00161EC4") __attribute__((sda));
extern short lift_camera_distance_along_facing_gp __asm__("D_L05_00161EC8") __attribute__((sda));

void FUN_L05_00315ee0(char *lift_moby, char *camera_position, char *camera_rotation) {
    float pitch_degrees;
    fast_vec_normalize(
        camera_position, lift_moby + 0xC0, *(float *)&lift_camera_distance_along_facing_gp);
    fast_vec_add(camera_position, camera_position, lift_moby + 0x10);
    *(float *)(camera_position + 8) =
        *(float *)(camera_position + 8) + *(float *)&lift_camera_height_offset_gp;
    clear_u64_value(camera_rotation);
    pitch_degrees = *(float *)&lift_camera_pitch_degrees_gp;
    *(float *)(camera_rotation + 8) = MOBY(lift_moby)->rot.z;
    *(float *)(camera_rotation + 4) = pitch_degrees * DEG_TO_RAD;
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_00317438), where it is exact; names translated to the US level program. */

void FUN_L05_00315f70(char *moby) {
    int   i = 0;
    char *data = ((char *)MOBY(moby)->pvars);
    char *tab = ((char *)l05_paths[*(int *)(data + 0x68)]);
    float ring_point_angle = 0.0f;
    float step = 6.28318f / (float)(*(int *)tab - 1);
    for (; i < *(int *)tab; i++) {
        char *ring_point_position = tab + 0x10 + i * 16;
        *(float *)ring_point_position = fast_cos(ring_point_angle) * 3.5f;
        *(float *)(ring_point_position + 4) = fast_sin(ring_point_angle) * 3.5f;
        *(int *)(ring_point_position + 8) = 0;
        fast_vec_add(ring_point_position, ring_point_position, moby + 0x10);
        ring_point_angle = fast_add_rotations(ring_point_angle, step);
    }
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_00317538), where it is exact; names translated to the US level program. */

extern f32 D_L05_00211B20[];

void draw_ocean_grid_outside_clip_volumes(struct Moby *moby) __asm__("FUN_L05_00316070");

void draw_ocean_grid_outside_clip_volumes(struct Moby *moby) {
    int *clip_volume_index_cursor = ((int *)moby->pvars);
    int  i;
    int  none = 1;
    for (i = 0; i < 8; clip_volume_index_cursor++, i++) {
        if (*clip_volume_index_cursor == -1)
            continue;
        if (is_point_inside_clip_volume(D_L05_001671C0, *clip_volume_index_cursor) != 0) {
            none = 0;
            break;
        }
    }
    if (none)
        draw_textured_grid(D_L05_00211B20);
}

/* Updates the pulsing marker: spawns it, then each frame animates its height and queues the draw. */
extern char  D_L05_00211B28[];
extern float D_L05_00211B20[];

void ocean_rilgar_update(struct Moby *moby) __asm__("FUN_L05_00316110");

void ocean_rilgar_update(struct Moby *moby) {
    enum {
        OCEAN_RILGAR_INITIALIZE = 0,
        OCEAN_RILGAR_UPDATE_BOBBING = 1
    };
    switch ((unsigned char)((char)moby->state)) {
    case OCEAN_RILGAR_INITIALIZE:
        prepare_scaled_table_with_byte_128(D_L05_00211B20, 1.0f);
        moby->state = 1;
        ((unsigned char *)moby)[0x30] = 0xFF;
        break;
    case OCEAN_RILGAR_UPDATE_BOBBING:
        if (hero.state.control_mode == 0x16) {
            D_L05_00211B20[2] =
                fast_sin((float)(frame_number % 360) * 0.017444444f - 3.14f) * 0.25f + 61.5f;
        } else {
            D_L05_00211B20[2] =
                fast_sin((float)(frame_number % 360) * 0.017444444f - 3.14f) * 0.25f + 59.5f;
        }
        add_draw_callback(draw_ocean_grid_outside_clip_volumes, moby);
        break;
    }
}

extern void  FUN_L00_001fff28(void *, int, float);
extern char *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;

void update_triggered_moby_motion_and_manipulator_angle(char *moby) __asm__("FUN_L05_00316258");

void update_triggered_moby_motion_and_manipulator_angle(char *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_WAIT_TO_RETURN_TO_SAVED_POSITION = 2,
        MOBY_MOVE_TO_OFFSET_POSITION = 3,
        MOBY_RETURN_TO_SAVED_POSITION = 4,
        MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE = 5,
        MOBY_SETTLE_AT_ZERO_MANIPULATOR_ANGLE = 6
    };
    char *data = ((char *)MOBY(moby)->pvars);
    float target[4];
    float delta[4];
    float stopped;

    switch (MOBY(moby)->state) {
    case MOBY_INITIALIZE:
        if (*(int *)(data + 0x50) != 0) {
            MOBY(moby)->rot.x = 3.1415927f;
            MOBY(moby)->pos.z += 4.0f;
        }
        qcopy(data, moby + 0x10);
        attach_manipulator(moby, 0, data + 0x10);
        if (*(int *)(data + 0x54) != 0) {
            *(float *)(data + 0x60) = 0.0f;
            MOBY(moby)->state = 2;
            if (*(int *)(data + 0x50) != 0)
                fast_vec_scale(target, moby + 0xD0, 2.0f);
            else
                fast_vec_scale(target, moby + 0xD0, -2.0f);
            fast_vec_add(moby + 0x10, moby + 0x10, target);
        } else {
            MOBY(moby)->state = 1;
            *(float *)(data + 0x60) = 0.5934119f;
            FUN_L00_001fff28(data + 0x20, 0, 0.5934119f);
        }
        *(long long *)(moby + 0x38) = *(long long *)(D_001413D0_lui + 0x38);
        break;
    case MOBY_WAIT_FOR_TRIGGER:
    case MOBY_WAIT_TO_RETURN_TO_SAVED_POSITION: {
        u8 trigger = ((u8 *)*(int *)&D_L05_0015FFD8_d)[*(int *)(data + 0x58) * 256 + 0xBC];
        if (trigger == 0)
            return;
        if (MOBY(moby)->state == 2 && trigger != 2) {
            MOBY(moby)->state = 4;
            break;
        }
        if (MOBY(moby)->state != 1)
            return;
        allocate_voice_for_target_entry(0, 0, moby);
        MOBY(moby)->state = 6;
        return;
    }
    case MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE:
    case MOBY_SETTLE_AT_ZERO_MANIPULATOR_ANGLE: {
        float end;
        float approach;
        if (MOBY(moby)->state == 5) {
            end = 0.5934119f;
            approach = 1.1868238f;
        } else {
            end = 0.0f;
            approach = -0.5934119f;
        }
        FUN_L00_0025be00((float *)(data + 0x60),
                         (float *)(data + 0x64),
                         approach,
                         frame_time_sq * 4.1887903f,
                         frame_time_sq * 4.1887903f,
                         frame_time * 12.566371f);
        if (*(float *)(data + 0x60) > 0.5934119f || *(float *)(data + 0x60) < 0.0f) {
            if (*(float *)(data + 0x60) > 0.5934119f)
                *(float *)(data + 0x60) = 0.5934119f;
            else if (*(float *)(data + 0x60) < 0.0f)
                *(float *)(data + 0x60) = 0.0f;
            if (MOBY(moby)->command == 0) {
                MOBY(moby)->command = 1;
                *(float *)(data + 0x64) *= -0.33f;
            } else if (*(float *)(data + 0x60) == end) {
                if (MOBY(moby)->state == 5)
                    MOBY(moby)->state = 7;
                else
                    MOBY(moby)->state = 3;
            }
        }
        FUN_L00_001fff28(data + 0x20, 0, *(float *)(data + 0x60));
    } break;
    case MOBY_MOVE_TO_OFFSET_POSITION:
    case MOBY_RETURN_TO_SAVED_POSITION:
        if (MOBY(moby)->state == 3) {
            fast_vec_scale(target, moby + 0xD0, -1.9f);
            fast_vec_add(target, data, target);
        } else {
            qcopy(target, data);
        }
        fast_vec_sub(delta, target, moby + 0x10);
        stopped = 0.0f;
        advance_accelerated_scalar(fast_vec_length(delta),
                                   frame_time_sq * 6.0f,
                                   frame_time_sq * 12.0f,
                                   frame_time * 10.0f,
                                   &stopped,
                                   (float *)(data + 0x5C));
        fast_vec_sub(delta, target, moby + 0x10);
        fast_vec_normalize(delta, delta, *(float *)(data + 0x5C));
        fast_vec_add(moby + 0x10, moby + 0x10, delta);
        if (!(fast_vec_dist(moby + 0x10, target) < 0.0001f))
            return;
        if (MOBY(moby)->state == 3) {
            allocate_voice_for_target_entry(1, 0, moby);
            MOBY(moby)->state = 7;
            return;
        }
        MOBY(moby)->state = 5;
        break;
    }
}

void update_triggered_moby_yaw(struct Moby *moby) __asm__("FUN_L05_003166a0");

void update_triggered_moby_yaw(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE_YAW = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_APPROACH_SAVED_YAW_OFFSET = 2
    };
    int *data = (int *)moby->pvars;

    switch (moby->state) {
    case MOBY_INITIALIZE_YAW:
        if (data[0] != 0)
            moby->flags |= 0x8000;
        moby->state = 1;
        *(float *)&data[3] = moby->rot.z;
        if (data[0] != 0)
            moby->rot.z = fast_subtract_rotations(moby->rot.z, 1.5707964f);
        else
            moby->rot.z = fast_add_rotations(moby->rot.z, 1.5707964f);
        /* fall through */
    case MOBY_WAIT_FOR_TRIGGER:
        if (((unsigned char *)*(int *)&D_L05_0015FFD8_d)[data[2] * 256 + 0xBC] != 0) {
            allocate_voice_for_target_entry(0, 0, moby);
            moby->state = 2;
        }
        break;
    case MOBY_APPROACH_SAVED_YAW_OFFSET: {
        float target;
        if (data[0] != 0)
            target = fast_subtract_rotations(*(float *)&data[3], 0.6981317f);
        else
            target = fast_add_rotations(*(float *)&data[3], 0.6981317f);
        approach_angle(&moby->rot.z,
                       target,
                       (float *)&data[1],
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        break;
    }
    }
}

#include "sda.h"

/* Applies breast growth (D_L05_00161ED0) and big-head manipulators to
 * matching-class cutscene instances of the hoverboard girl. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_00317E58), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x44];
    short n;
    char  pad1[0x132];
    char *m[1];
} G;

extern G             D_L05_0016CC60;
extern s32           breast_growth_count_gp __asm__("D_L05_00161ED0") __attribute__((sda));
extern unsigned char D_0015EDB0[] MACRO_ADDR;
extern unsigned char D_L05_0017C8A0[];
extern void update_hoverboard_girl_boobs_animation(char *, int) __asm__("FUN_L05_00316810");

void update_hoverboard_girl_cutscene_instances(char *arg) __asm__("FUN_L05_00316990");

void update_hoverboard_girl_cutscene_instances(char *reference_moby) {
    if (D_L05_0015F5C4 == 2 && (D_0015EDB0[0] != 0 || *(int *)&breast_growth_count_gp != 0)) {
        int i;
        for (i = 0; i < D_L05_0016CC60.n; i++) {
            char *moby = D_L05_0016CC60.m[i];
            if (MOBY(moby)->oclass == MOBY(reference_moby)->oclass) {
                if (D_0015EDB0[0] != 0) {
                    unsigned char *manipulator = D_L05_0017C8A0;
                    if (manipulator[1] == 0) {
                        attach_manipulator(moby, 0, manipulator);
                        *(float *)(manipulator + 0x20) = 2.75f;
                        *(float *)(manipulator + 0x24) = 2.75f;
                        *(float *)(manipulator + 0x28) = 2.75f;
                    }
                }
                if (*(int *)&breast_growth_count_gp != 0) {
                    update_hoverboard_girl_boobs_animation(moby, *(int *)(reference_moby + 0x78));
                }
            }
        }
    }
}

typedef struct {
    char           pad00[4];
    unsigned short sub;
    char           pad06[0x1A];
    void          *ptr20;
    char           pad24[0x1C];
    char           body[0x64];
    float          pitch;
    float          yaw;
    char           padAC[4];
    float          height;
    char           padB4[0xC];
    char           head[0x68];
    float          head_yaw;
    char           pad12C[0x14];
    float          target[4];
    char           pad150[4];
    int            idle_timer;
    int            moving_timer;
    int            target_timer;
} WD_c;

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0x10];
    unsigned char opacity;
    char          pad32[0x16];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x1C];
    unsigned char flags70;
    char          pad71[7];
    WD_c         *data;
    char          pad7C[3];
    unsigned char b7F;
} WM_c;

typedef struct {
    char  pad00[0x80];
    float position[4];
    char  pad90[0x40];
    float aim[4];
    char  padE0[0x22E];
    short disabled;
    char  pad310[0x1D7C];
    int   mode;
} L16WatchPlayer_c;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L16WatchScratch_c;

extern void          begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void          memcard_save_data(int, int) __asm__("FUN_0020b178");
extern float         FUN_001f96b0_c(float) __asm__("FUN_001f96b0");
extern float         fast_add_rotations_c(float, float) __asm__("fast_add_rotations");
extern float         fast_subtract_rotations_c(float, float) __asm__("fast_subtract_rotations");
extern float         vector_length_xy_c(void *) __asm__("vector_length_xy");
extern int           scale_game_frames_c(int) __asm__("scale_game_frames");
extern unsigned char D_0015EDB0_b_c __asm__("D_0015EDB0");
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern L16WatchPlayer_c D_0013E633_watch_c __asm__("D_0013E533");

typedef struct {
    char  pad00[4];
    short sub;
    char  pad06[6];
    float fC;
    char  pad10[0x10];
    void *ptr20;
    char  pad24[0x1C];
    char  body[0x64];
    float pitch;
    float yaw;
    char  padAC[4];
    float height;
    char  padB4[0xC];
    char  head[0x68];
    float head_yaw;
    char  pad12C[0x18];
    int   idle_timer;
    int   moving_timer;
    int   target_timer;
    float target[4];
} WD2;

extern char qwarks_bouncer_dialogue_name[] __asm__("D_L05_00161EF0");

extern unsigned char D_0013DD47 NOT_SDA;
/* Watcher moby update (variant of shady_salesman_update): turns its head toward the hero or a random target. */
void qwarks_bouncer_update(WM_c *m) __asm__("FUN_L05_00317470");

void qwarks_bouncer_update(WM_c *bouncer_moby) {
    enum {
        QWARKS_BOUNCER_INITIALIZE = 0,
        QWARKS_BOUNCER_UPDATE_DIALOGUE = 1
    };
    L16WatchScratch_c scratch;
    WD2              *bouncer_data = (WD2 *)bouncer_moby->data;
    float             rate;
    float             head_rate;
    int               tracking;
    attach_big_head_cheat_manipulator(bouncer_moby);
    if (!bouncer_data)
        return;
    switch (bouncer_moby->state) {
    case QWARKS_BOUNCER_INITIALIZE:
        if (D_0013DD47) {
            mark_moby_for_removal(bouncer_moby);
            return;
        }
        bouncer_data->ptr20 = (void *)qwarks_bouncer_dialogue_name;
        bouncer_moby->state = 1;
        initialize_npc_dialogue(bouncer_moby, bouncer_data);
        bouncer_data->fC = 3.7f;
        initialize_moby_grounding(bouncer_moby);
        break;
    case QWARKS_BOUNCER_UPDATE_DIALOGUE:
        if (update_npc_dialogue(bouncer_moby, bouncer_data))
            begin_dialogue_camera_transition(bouncer_moby, 2.5f);
        if (bouncer_data->sub == 5) {
            unlock_planet(7);
            memcard_save_data(0, -1);
            mark_moby_for_removal(bouncer_moby);
            return;
        }
        if (bouncer_moby->animation == 0 && fast_dec_timer(&bouncer_data->idle_timer)) {
            int idle_animation = 2;
            bouncer_data->idle_timer =
                truncate_float_to_s32(FUN_001f96b0_c(random_float_between(1200.0f, 2400.0f)));
            if (random_integer_below(2))
                idle_animation = 1;
            if (bouncer_moby->animation != idle_animation)
                blend_moby_animation(bouncer_moby, idle_animation, 0, scale_game_frames(10));
        } else if ((bouncer_moby->flags70 & 2) && bouncer_moby->animation) {
            blend_moby_animation(bouncer_moby, 0, 0, scale_game_frames(10));
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (bouncer_moby->animation == 0) {
        char *player = (char *)&D_0013E633_watch_c + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(bouncer_moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                bouncer_moby->yaw,
                fast_arc_tan(((L16WatchPlayer_c *)((char *)&D_0013E633_watch_c + 0xE1D))->aim[0] -
                                 bouncer_moby->position[0],
                             ((L16WatchPlayer_c *)((char *)&D_0013E633_watch_c + 0xE1D))->aim[1] -
                                 bouncer_moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                bouncer_data->moving_timer = scale_game_frames_c(120);
            else
                fast_dec_timer(&bouncer_data->moving_timer);
        } else if (bouncer_data->moving_timer) {
            bouncer_data->moving_timer = 0;
            qcopy(bouncer_data->target, (char *)&D_0013E633_watch_c + 0xEED);
        }
        if (fast_dec_timer(&bouncer_data->target_timer)) {
            float heading;
            bouncer_data->target_timer =
                truncate_float_to_s32(FUN_001f96b0_c(random_float_between(180.0f, 300.0f)));
            heading = fast_add_rotations_c(bouncer_moby->yaw,
                                           random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(bouncer_data->target,
                                   6.0f,
                                   heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(bouncer_data->target, bouncer_data->target, bouncer_moby->position);
        }
        if (bouncer_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch_c + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, bouncer_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, bouncer_moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations_c(fast_arc_tan(scratch.delta[0], scratch.delta[1]),
                                        bouncer_moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy_c(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        bouncer_data->pitch = pitch;
        bouncer_data->head_yaw = bouncer_data->yaw = yaw * 0.5f;
    }
    if (D_0015EDB0_b_c)
        bouncer_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, bouncer_moby, bouncer_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, bouncer_moby, bouncer_data->head, 1);
}
/* Level overlay code; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

#include "qcopy.h"

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad00[4];
    unsigned short sub;
    char           pad06[0x1A];
    void          *ptr20;
    char           pad24[0x1C];
    char           body[0x64];
    float          pitch;
    float          yaw;
    char           padAC[4];
    float          height;
    char           padB4[0xC];
    char           head[0x68];
    float          head_yaw;
    char           pad12C[0x14];
    float          target[4];
    char           pad150[4];
    int            idle_timer;
    int            moving_timer;
    int            target_timer;
} WD;

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0x10];
    unsigned char opacity;
    char          pad32[0x16];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x1C];
    unsigned char flags70;
    char          pad71[7];
    WD           *data;
    char          pad7C[3];
    unsigned char b7F;
} WM;

typedef struct {
    char          pad00[0x70];
    unsigned char entered;
    unsigned char mission;
} L16WatchFlags;

extern unsigned char D_0013D4FF NOT_SDA;
extern void                     FUN_L00_0025a120(void *);

extern char           D_0013E633[], D_0014171B[], D_0013D24D[], D_L16_00167240[];
extern float          fast_add_rotations(float, float);
extern float          fast_subtract_rotations(float, float);
extern float          vector_length_xy(void *);
extern int            scale_game_frames(int);
extern char           shady_salesman_dialogue_name[] __asm__("D_L05_00161EF8");
extern unsigned char  D_0015EDB0_b __asm__("D_0015EDB0");
extern void           jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void           queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");

void shady_salesman_update(WM *m) __asm__("FUN_L05_003180a0");

void shady_salesman_update(WM *moby) {
    enum {
        SHADY_SALESMAN_INITIALIZE = 0,
        SHADY_SALESMAN_UPDATE_DIALOGUE = 1
    };
    L16WatchScratch scratch;
    WD             *npc_data = moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_big_head_cheat_manipulator_to_matching_moby((char *)moby);
    if (moby->opacity) {
        if (fast_vec_dist(moby->position, D_L05_001671C0) < 40.0f) {
            FUN_L00_0025a120(moby);
            moby->b7F = 0x20;
        }
    }
    switch (moby->state) {
    case SHADY_SALESMAN_INITIALIZE:
        if (D_0013D4FF) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->position[2] += 0.5f;
        moby->position[2] = probe_ground_height(moby->position, 0, 0.5f);
        npc_data->ptr20 = (void *)shady_salesman_dialogue_name;
        moby->state = 1;
        initialize_npc_dialogue(moby, npc_data);
        break;
    case SHADY_SALESMAN_UPDATE_DIALOGUE:
        if (update_npc_dialogue(moby, npc_data))
            begin_dialogue_camera_transition(moby, 2.0f);
        if ((unsigned)(npc_data->sub - 3) < 2) {
            grant_inventory_item(0x17, 1);
            queue_dialogue_message(0x1396, -1);
            memcard_save_data(0, -1);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->animation == 0 && fast_dec_timer((char *)npc_data + 0x154)) {
            npc_data->idle_timer =
                truncate_float_to_s32(scale_time(random_float_between(1200.0f, 2400.0f)));
            if (moby->animation != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        } else if (moby->animation == 1 && (moby->flags70 & 2)) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->yaw,
                fast_arc_tan(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                 moby->position[0],
                             ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                 moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                npc_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&npc_data->moving_timer);
        } else if (npc_data->moving_timer) {
            npc_data->moving_timer = 0;
            qcopy(npc_data->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (fast_dec_timer(&npc_data->target_timer)) {
            float heading;
            npc_data->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                npc_data->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(npc_data->target, npc_data->target, moby->position);
        }
        if (npc_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, npc_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        npc_data->pitch = pitch;
        npc_data->yaw = yaw * 0.6f;
        npc_data->head_yaw = yaw * 0.4f;
    }
    if (D_0015EDB0_b)
        npc_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, npc_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, npc_data->head, 1);
}

extern int   D_L05_001612D8_macro[] __asm__("D_L05_001612D8") MACRO_ADDR;
extern float D_L05_001612E0_gp[4] __asm__("D_L05_001612E0") MACRO_ADDR __attribute__((sda));
extern float D_L05_001612E8 __asm__("D_L05_001612E8");
extern int   D_L05_0015F5C4_macro[] __asm__("D_L05_0015F5C4") MACRO_ADDR;

/* Two-state marker: arms the moby, then once its target is found copies the
 * target position and sets the bob height from the frame counter. */
void update_hero_clip_volume_outputs(struct Moby *moby) __asm__("FUN_L05_00318a68");

void update_hero_clip_volume_outputs(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_CHECK_HERO_VOLUME = 1
    };
    int   state = moby->state;
    int  *clip_volume_index_ptr = ((int *)moby->pvars);
    char *hero_position;

    switch (state) {
    case MOBY_INITIALIZE:
        moby->state = 1;
        moby->flags |= 1;
        moby->update_dist = 0xFF;
        moby->collision = 0;
        break;
    case MOBY_CHECK_HERO_VOLUME:
        hero_position = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(hero_position, *clip_volume_index_ptr) != 0) {
            D_L05_001612D8_macro[1] = state;
            qcopy(D_L05_001612E0_gp, hero_position);
            *(short *)(hero_position + 0x15E) = 5;
            hero_clip_volume_output_w = 48.0f;
            *(volatile float *)&D_L05_001612E8 =
                fast_sin((float)(D_L05_0015F5C4_macro[2] % 360) * 0.017444444f - 3.14f) * 0.25f +
                59.5f;
        } else {
            hero_inside_clip_volume = 0;
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0031A0A8), where it is exact; names translated to the US level program. */

extern short television_scale_multiplier_gp __asm__("D_L05_00161F08") __attribute__((sda));
void         television_update(struct Moby *moby) __asm__("FUN_L05_00318b98");

void television_update(struct Moby *moby) {
    enum {
        TELEVISION_INITIALIZE = 0,
        TELEVISION_UPDATE_BOBBING = 1
    };
    float bobbing_phase;
    char *data = (char *)moby->pvars;
    int   visibility_control_mode;
    moby->scale = moby->pclass->scale * *(float *)&television_scale_multiplier_gp;
    switch (moby->state) {
    case TELEVISION_INITIALIZE:
        moby->state = 1;
        *(float *)data = random_angle_radians();
        *(float *)(data + 4) = moby->pos.z;
        break;
    case TELEVISION_UPDATE_BOBBING:
        visibility_control_mode = D_L05_0015F5C4;
        if (visibility_control_mode == 2) {
            moby->visible = 0;
            moby->flags |= 1;
        } else {
            moby->visible = 1;
            moby->flags &= 0xFFFE;
        }
        bobbing_phase = fast_add_rotations(*(float *)data, frame_time * 0.87266463f);
        *(float *)data = bobbing_phase;
        moby->pos.z = *(float *)(data + 4) + fast_sin(bobbing_phase);
        break;
    }
}
/* Same source as the exact taxi_update, with its moby data 8 bytes further on, an x range on the pickup test and a table test for state 6. */

extern char D_0013CAE4[];
extern float
advance_accelerated_scalar_c(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
void        initialize_taxi_stop_state(void *moby_v) __asm__("FUN_L05_00319208");
extern void select_taxi_route(struct Moby *) __asm__("FUN_L05_003193a8");
extern void sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
extern void func_00215CA8_f5848(float, int *, int, void *, float *, int) __asm__("FUN_00214e58");

void taxi_update(struct Moby *moby) __asm__("FUN_L05_00318c78");

void taxi_update(struct Moby *moby) {
    enum {
        TAXI_INITIALIZE = 0,
        TAXI_WAIT_FOR_PICKUP = 1,
        TAXI_BLEND_TO_PATH_START = 3,
        TAXI_FOLLOW_PATH = 4,
        TAXI_BLEND_TO_DESTINATION = 5,
        TAXI_WAIT_FOR_ACTIVATION = 6
    };
    char *taxi_data = (char *)moby->pvars;
    float old_position[4], old_rotation[4], position_delta[4],
        rotation_or_start_position_scratch[4], interpolation_end_position[4],
        interpolation_start_angles[4], interpolation_end_angles[4], hero_offset[4];
    struct Hero *pickup_hero;
    struct Hero *travel_hero;
    float        z;
    struct Moby *destination_stop_moby;
    char        *voice_slot;
    LevelCuboid *tab;
    float       *qa;
    int          voice_slot_index;
    int          s;
    qcopy(old_position, &moby->pos);
    qcopy(old_rotation, &moby->rot);
    set_moby_dialogue_record_value(moby, (moby->flags ^ 1) & 1);
    switch (moby->state) {
    case TAXI_INITIALIZE:
        *(short *)(taxi_data + 0xB2) = *(short *)(taxi_data + 0xB0) = -1;
        initialize_taxi_stop_state(moby);
        break;
    case TAXI_WAIT_FOR_PICKUP:
        pickup_hero = &hero;
        if (((char *)pickup_hero->ground_moby) == moby && pickup_hero->air_frames.s == 0) {
            transpose_rotation_matrix(rotation_or_start_position_scratch, &moby->rot_row0);
            fast_vec_sub(hero_offset, &pickup_hero->motion.pos, &moby->pos);
            hero_offset[3] = 0;
            transform_vector(hero_offset, hero_offset, rotation_or_start_position_scratch);
            if (absolute_float(hero_offset[1]) < 1.2f && hero_offset[0] < 1.0f &&
                hero_offset[0] > -0.75f) {
                select_taxi_route(moby);
                request_moby_variant_help_message(moby);
                if (*(int *)(D_0013CAE4) & 0x10) {
                    moby->state = 3;
                    destination_stop_moby =
                        find_moby_in_owner_list_by_id(moby, *(short *)(taxi_data + 0xB6));
                    if (destination_stop_moby != 0) {
                        destination_stop_moby->state = 2;
                        destination_stop_moby->flags |= 1;
                        destination_stop_moby->visible = 0;
                        destination_stop_moby->collision = 0;
                    }
                }
            }
        }
        break;
    case TAXI_BLEND_TO_PATH_START:
        advance_accelerated_scalar_c(1.0f,
                                     frame_time_sq,
                                     frame_time_sq,
                                     frame_time * 0.5f,
                                     (float *)(taxi_data + 0xC0),
                                     (float *)(taxi_data + 0xC4));
        qa = rotation_or_start_position_scratch;
        s = *(int *)(taxi_data + 0x80 - (-(*(short *)(taxi_data + 0xB4) * 4)));
        tab = l05_cuboids;
        qcopy(qa, &tab[s].pos);
        qcopy(interpolation_start_angles, &tab[s].rot);
        z = 0.0f;
        func_00215CA8_f5848(z,
                            *(int **)(taxi_data + 0xAC),
                            0,
                            interpolation_end_position,
                            interpolation_end_angles,
                            0);
        lerp_vector(taxi_data + 0x60,
                    rotation_or_start_position_scratch,
                    interpolation_end_position,
                    *(float *)(taxi_data + 0xC0));
        *(float *)(taxi_data + 0x74) = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[1], interpolation_start_angles[1]) *
                *(float *)(taxi_data + 0xC0),
            interpolation_start_angles[1]);
        *(float *)(taxi_data + 0x78) = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[2], interpolation_start_angles[2]) *
                *(float *)(taxi_data + 0xC0),
            interpolation_start_angles[2]);
        if (1.0f <= *(float *)(taxi_data + 0xC0)) {
            *(float *)(taxi_data + 0xC0) = z;
            *(float *)(taxi_data + 0xC4) = z;
            moby->state = 4;
        }
        break;
    case TAXI_FOLLOW_PATH:
        if (advance_moby_path(moby)) {
            *(int *)(taxi_data + 0xC0) = 0;
            *(int *)(taxi_data + 0xC4) = 0;
            moby->state = 5;
        }
        break;
    case TAXI_BLEND_TO_DESTINATION:
        advance_accelerated_scalar_c(1.0f,
                                     frame_time_sq,
                                     frame_time_sq,
                                     frame_time * 0.5f,
                                     (float *)(taxi_data + 0xC0),
                                     (float *)(taxi_data + 0xC4));
        qa = interpolation_end_position;
        s = *(int *)(taxi_data + 0x80 - (-(*(short *)(taxi_data + 0xB6) * 4)));
        tab = l05_cuboids;
        qcopy(qa, &tab[s].pos);
        qcopy(interpolation_end_angles, &tab[s].rot);
        sample_camera_path(*(int **)(taxi_data + 0xAC),
                           0,
                           rotation_or_start_position_scratch,
                           interpolation_start_angles,
                           0,
                           (float)**(int **)(taxi_data + 0xAC));
        lerp_vector(taxi_data + 0x60,
                    rotation_or_start_position_scratch,
                    interpolation_end_position,
                    *(float *)(taxi_data + 0xC0));
        *(float *)(taxi_data + 0x74) = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[1], interpolation_start_angles[1]) *
                *(float *)(taxi_data + 0xC0),
            interpolation_start_angles[1]);
        *(float *)(taxi_data + 0x78) = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[2], interpolation_start_angles[2]) *
                *(float *)(taxi_data + 0xC0),
            interpolation_start_angles[2]);
        if (1.0f <= *(float *)(taxi_data + 0xC0)) {
            *(int *)(taxi_data + 0xC0) = 0;
            *(int *)(taxi_data + 0xC4) = 0;
            moby->state = 1;
            reset_selected_moby_to_current_placement(moby);
        }
        break;
    case TAXI_WAIT_FOR_ACTIVATION:
        if (D_0014C050[current_level_index][*(int *)(taxi_data + 0xB8)] == 0xFF ||
            D_0014C050[current_level_index][*(int *)(taxi_data + 0xBC)] == 0xFF) {
            char          *model = moby->pclass;
            unsigned short flags = moby->flags;
            int            value = *(int *)(model + 0x10);
            moby->visible = 1;
            moby->collision = value;
            moby->flags = flags & 0xFFFE;
            moby->state = 1;
        }
        break;
    }
    apply_moby_position_and_rotation_wobble(moby);
    if (moby->state >= 3 && moby->state <= 5) {
        travel_hero = &hero;
        travel_hero->jump_lock_timer = 2;
        travel_hero->unk1F4 = 2;
        transform_moby_vector_table_to_world_space(moby);
        if (voice_is_playing(moby, *(short *)(taxi_data + 0xB2)) == 0) {
            *(short *)(taxi_data + 0xB2) = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else if (voice_is_playing(moby, *(short *)(taxi_data + 0xB2)) != 0) {
        voice_slot_index = *(short *)(taxi_data + 0xB2);
        if (voice_slot_index != -1) {
            voice_slot = D_0013E550 + voice_slot_index * 0x70;
            if (*(char **)(voice_slot + 0x88) == moby &&
                *(unsigned char *)(voice_slot + 0x74) != 0) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(short *)(taxi_data + 0xB2) = -1;
    }
    fast_vec_sub(position_delta, &moby->pos, old_position);
    FUN_L00_00260738(taxi_data + 0x20, position_delta, old_rotation, &moby->rot);
}

#include "qcopy.h"

void initialize_taxi_stop_state(void *moby_v) {
    char *moby = moby_v;
    char *data = ((char *)MOBY(moby)->pvars);
    int  *list = (int *)(data + 0x80);
    int   i;
    int   selected_pose_byte_offset;
    char *pose_table_base;
    *(short *)(data + 0xB4) = -1;
    for (i = 0; i < 5; i++) {
        if (fast_vec_dist(moby + 0x10, &l05_cuboids[list[i]].pos) < 1.0f) {
            *(short *)(data + 0xB4) = i;
        }
    }
    switch (*(short *)(data + 0xB4)) {
    case TAXI_BRANCHING_STOP: {
        int k = current_level_index << 4;
        if (visited52[*(int *)(data + 0xB8) + k] != 0xFF &&
            visited52[*(int *)(data + 0xBC) + k] != 0xFF) {
            moby[0x20] = 6;
            MOBY(moby)->flags = MOBY(moby)->flags | 1;
            moby[0x31] = 0;
            MOBY(moby)->collision = 0;
            break;
        } else {
            int v = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            moby[0x31] = 1;
            MOBY(moby)->collision = v;
            MOBY(moby)->flags = MOBY(moby)->flags & 0xFFFE;
            moby[0x20] = 1;
            break;
        }
    }
    case TAXI_PAIRED_ROUTE_FIRST_STOP:
    case TAXI_PAIRED_ROUTE_SECOND_STOP:
    case TAXI_PRIMARY_BRANCH_STOP:
    case TAXI_FALLBACK_BRANCH_STOP: {
        int v = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        moby[0x31] = 1;
        MOBY(moby)->collision = v;
        MOBY(moby)->flags = MOBY(moby)->flags & 0xFFFE;
        moby[0x20] = 1;
        break;
    }
    }
    {
        int *selected = list;
        selected += *(short *)(data + 0xB4);
        selected_pose_byte_offset = *selected << 7;
    }
    pose_table_base = (char *)l05_cuboids;
    qcopy(data + 0x60, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x30));
    qcopy(data + 0x70, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x70));
}
/* Picks the path for the current point (five points, point 2 chosen by the table entry) and sets the travel speeds; same source as FUN_L16_002e4a58. */

void select_taxi_route(struct Moby *moby) {
    char *taxi_data = ((char *)moby->pvars);
    char *selected_path;
    float inv, frame_scaled_travel_step, frame_squared_travel_step, first_path_segment_length;
    switch (*(short *)(taxi_data + 0xB4)) {
    case TAXI_PAIRED_ROUTE_FIRST_STOP:
        *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0x94)]);
        *(short *)(taxi_data + 0xB6) = 1;
        break;
    case TAXI_PAIRED_ROUTE_SECOND_STOP:
        *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0x98)]);
        *(short *)(taxi_data + 0xB6) = 0;
        break;
    case TAXI_BRANCHING_STOP:
        if (D_0014C050[current_level_index][*(int *)(taxi_data + 0xB8)] != 0xFF) {
            *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0x9C)]);
            *(short *)(taxi_data + 0xB6) = 3;
        } else {
            *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0xA4)]);
            *(short *)(taxi_data + 0xB6) = 4;
        }
        break;
    case TAXI_PRIMARY_BRANCH_STOP:
        *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0xA0)]);
        *(short *)(taxi_data + 0xB6) = 2;
        break;
    case TAXI_FALLBACK_BRANCH_STOP:
        *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0xA8)]);
        *(short *)(taxi_data + 0xB6) = 2;
        break;
    }
    selected_path = *(char **)(taxi_data + 0xAC);
    first_path_segment_length = fast_vec_dist(selected_path + 0x10, selected_path + 0x20);
    frame_scaled_travel_step = frame_time * 20.0f;
    frame_squared_travel_step = frame_time_sq * 5.0f;
    inv = 1.0f / (first_path_segment_length * (float)**(int **)(taxi_data + 0xAC));
    *(int *)(taxi_data + 0xC4) = 0;
    *(int *)(taxi_data + 0xC0) = 0;
    *(float *)(taxi_data + 0xD4) = frame_scaled_travel_step * inv;
    *(float *)(taxi_data + 0xD8) = frame_squared_travel_step * inv;
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0031B138), where it is exact; names translated to the US level program. */

extern int D_L05_0015F5C4_c __asm__("D_L05_0015F5C4");

void qwarks_trailer_update(struct Moby *moby) __asm__("FUN_L05_00319c28");

void qwarks_trailer_update(struct Moby *moby) {
    if (D_L05_0015F5C4_c == 2) {
        moby->visible = 0;
        moby->flags |= 1;
    } else {
        moby->visible = 1;
        moby->flags &= 0xFFFE;
    }
}

float D_L05_0020B740[4][2] = {
    {0.5f, 0.0f},
    {0.5f, 1.0f},
    {0.5f, 0.0f},
    {0.5f, 1.0f},
};

int l05_electric_gate_beam_effect_texture_index __asm__("D_L05_00161D98") = 0x13;

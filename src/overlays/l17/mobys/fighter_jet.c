/* Level 17 moby class fighter_jet: flight, HUD and lock marker, targeting and weapons, and the class flags helper it uses. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_globals.h"
#include "rnc/input/pad_state.h"
#include "rnc/mobys/moby_class_id.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/overlay/l17/hero/hero_set_state.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/ui/help/force_help_message.h"

#include "qzero.h"

/* For every live moby of one class: flag2 and flag1 set or clear flag bits 2 and 1 (flag1 also the active byte), and
   collision restores the class collision or clears it; -1 leaves that one alone. */
/* Ported from rac1-decomp (src/overlays/l17/mobyutil_00254B40.c: func_L17_00254B40), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x10];
    int  value;
} Level17MobyLink;

typedef struct {
    char             pad0[0x20];
    signed char      state;
    char             pad21[3];
    Level17MobyLink *link; /* class header; its collision at 0x10 */
    char             pad28[9];
    unsigned char    active;
    char             pad32[2];
    unsigned short   flags;
    char             pad36[0x5E];
    int              collision;
    char             pad98[0xE];
    short            class_id;
    char             padA8[0x58];
} Level17Moby;

extern Level17Moby *l17_moby_array_last __asm__("D_L17_0015FFE0");

void set_class_mobys_flags_and_collision(int class_id,
                                         int flag2,
                                         int flag1,
                                         int collision) __asm__("FUN_L17_00253a90");

void set_class_mobys_flags_and_collision(int class_id, int flag2, int flag1, int collision) {
    Level17Moby *moby;
    for (moby = (Level17Moby *)D_L17_0015FFD8; moby <= l17_moby_array_last; moby++) {
        if (moby->class_id == class_id && moby->state >= 0) {
            if (flag2 != -1) {
                if (flag2 != 0)
                    moby->flags &= ~2;
                else
                    moby->flags |= 2;
            }
            if (flag1 != -1) {
                if (flag1 != 0) {
                    moby->active = 1;
                    moby->flags &= ~1;
                } else {
                    moby->active = 0;
                    moby->flags |= 1;
                }
            }
            if (collision != -1) {
                if (collision != 0)
                    moby->collision = moby->link->value;
                else
                    moby->collision = 0;
            }
        }
    }
}

/* Draws one moby's icon on the radar: its offset from the viewer turned into the radar's frame, faded near the rim. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EC5D0), where it is exact; names translated to the US level program. */

typedef struct {
    short tex;
    short u;
    short v;
    short w;
    short h;
    short fA;
    short x;
    short y;
} IconEC5D0;

extern IconEC5D0     l17_fighter_jet_radar_contact_icon_definitions[] __asm__("D_L17_001D9B38");
extern unsigned char frame_size[] __asm__("D_0013E504");
extern unsigned char D_0015EDB4[4];
extern void
    draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("func_001F5450");

void draw_fighter_jet_radar_contact_icon(char *radar_rotation,
                                         char *from,
                                         char *moby,
                                         int   icon,
                                         long  color,
                                         int   tex) __asm__("FUN_L17_002eaae0");

void draw_fighter_jet_radar_contact_icon(

    char *radar_rotation, char *from, char *moby, int icon, long color, int tex) {
    float      turn[4];
    float      radar_rotation_matrix[16];
    float      delta[4];
    float      radar_offset[4];
    float      radar_distance;
    float      side;
    int        x, y;
    IconEC5D0 *radar_icon_definition;

    qzero(turn);
    turn[2] = *(float *)(radar_rotation + 8);
    turn[2] = fast_subtract_rotations(1.5707964f, turn[2]);
    euler_rotation_matrix(radar_rotation_matrix, turn);
    fast_vec_sub(delta, moby + 0x10, from);
    delta[2] = 0.0f;
    transform_vector(radar_offset, delta, radar_rotation_matrix);
    fast_vec_scale(radar_offset, radar_offset, 0.0714285746f);
    radar_distance = fast_vec_length(radar_offset);
    side = 1.0f;
    if (D_0015EDB4[0] != 0) {
        side = -1.0f;
    }
    x = truncate_float_to_s32(side * radar_offset[0]);
    y = truncate_float_to_s32(-radar_offset[1]);
    if (radar_distance < 46.0f) {
        if (radar_distance > 38.0f) {
            float alpha_distance_product;
            radar_distance = 46.0f - radar_distance;
            alpha_distance_product =
                convert_integer_to_float(((unsigned long)color >> 24) & 0xFF) * radar_distance;
            color &= 0xFFFFFF;
            color = (truncate_float_to_s32(alpha_distance_product * 0.125f) << 24) | color;
        }
        radar_icon_definition = &l17_fighter_jet_radar_contact_icon_definitions[icon];
        y -= 0x50;
        y += *(int *)(frame_size);
        x += 0x1B0;
        draw_textured_quad(x - radar_icon_definition->x,
                           y - radar_icon_definition->y,
                           radar_icon_definition->w,
                           radar_icon_definition->h,
                           radar_icon_definition->u,
                           radar_icon_definition->v,
                           radar_icon_definition->w,
                           radar_icon_definition->h,
                           color,
                           get_effect_texture(tex + 0x28 + radar_icon_definition->tex));
    }
}

#include "qcopy.h"
/* Walks the moby list: picks the on-screen target, marks targetable mobys and points an arrow at the nearest pickup. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EC7D8), where it is exact; names translated to the US level program. */

extern char         *fighter_jet_contact_moby_list_head __asm__("D_L17_0015FFE4");
extern char          fighter_jet_arrow_direction_basis[] __asm__("D_L17_001678D0");
extern char          fighter_jet_screen_marker_vertices[] __asm__("D_L17_001D9AC0");
extern float         fighter_jet_aim_radius_offset_vector[] __asm__("D_L17_001678E0");
extern float         normalize_angle(float) __asm__("FUN_L00_0025e310");
extern unsigned char D_0013E500[];
extern void
project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");
extern void emit_transformed_2d_vertex_packet(
    void *, int, int, long, float, float, float, float) __asm__("FUN_L13_002b7d50");
void draw_fighter_jet_radar_contact_icon(
    char *rot, char *from, char *moby, int icon, long color, int tex) __asm__("FUN_L17_002eaae0");

short D_L17_00162188 = 2;
int   select_fighter_jet_target_and_draw_contacts(int   ship_moby,
                                                  char *ship_vars,
                                                  char *radar_origin,
                                                  char *radar_rotation,
                                                  float f0,
                                                  float f1,
                                                  float f2,
                                                  char *current_target,
                                                  char *excluded_target) __asm__("FUN_L17_002eace8");

int select_fighter_jet_target_and_draw_contacts(int   ship_moby,
                                                char *ship_vars,
                                                char *radar_origin,
                                                char *radar_rotation,
                                                float f0,
                                                float f1,
                                                float f2,
                                                char *current_target,

                                                char *excluded_target) {
    float v[4];
    float arrow_target_position_scratch[4];
    float arrow_direction_rotation_matrix[16];
    float u[4];
    int   sx, sy, tx, ty;
    char *scanned_moby;
    char *selected_target_moby;
    int   flag;
    char *near;
    int   best;
    int  *scr;
    float bestd;
    int   dx, dy, aim_selection_screen_radius, aim_screen_distance_squared;
    float x, y, ang, dd, ship_to_arrow_target_distance, heading_error_magnitude, k,
        angular_error_magnitude, arrow_pulse_phase;

    scanned_moby = fighter_jet_contact_moby_list_head;
    best = 0x7FFFFFFF;
    selected_target_moby = 0;
    *(int *)(ship_vars + 0xE8) = 0;
    flag = 0;
    bestd = 100000.0f;
    near = 0;
    while (scanned_moby != 0) {
        scr = (int *)(D_0013E500);
        if ((MOBY(scanned_moby)->flags & 0x1000) && MOBY(scanned_moby)->visible != 0 &&
            scanned_moby != 0 && ((char *)MOBY(scanned_moby)->pclass) != 0 &&
            *(short *)(((char *)MOBY(scanned_moby)->pclass) + 0x46) == 5) {
            project_world_point_to_screen_pixels(scanned_moby + 0x10, &sx, &sy, 0);
            dx = sx - *(int *)(ship_vars + 0xE0);
            dy = sy - *(int *)(ship_vars + 0xE4);
            if (dx < 0)
                dx = -dx;
            if (dy < 0)
                dy = -dy;
            qcopy(v, fighter_jet_aim_radius_offset_vector);
            fast_vec_scale(
                v, fighter_jet_aim_radius_offset_vector, MOBY(scanned_moby)->bsphere.w / 1280.0f);
            fast_vec_add(v, v, scanned_moby + 0x10);
            project_world_point_to_screen_pixels(v, &tx, &ty, 0);
            tx -= sx;
            ty -= sy;
            aim_selection_screen_radius =
                truncate_float_to_s32(square_root(convert_integer_to_float(tx * tx + ty * ty))) +
                0x1C;
            if (dx < aim_selection_screen_radius && dy < aim_selection_screen_radius) {
                if (current_target != 0) {
                    if (scanned_moby == current_target) {
                        best = 0;
                        selected_target_moby = scanned_moby;
                    }
                } else if (scanned_moby != excluded_target) {
                    aim_screen_distance_squared = dx * dx + dy * dy;
                    if (aim_screen_distance_squared < best) {
                        selected_target_moby = scanned_moby;
                        best = aim_screen_distance_squared;
                    }
                }
            }
        }
        switch (MOBY(scanned_moby)->oclass) {
        case MOBY_CLASS_GRAY_RADAR_CONTACT:
        case MOBY_CLASS_GRAY_RADAR_ICON_TWO_CONTACT:
            draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                radar_origin,
                                                scanned_moby,
                                                2,
                                                0x70808080,
                                                *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_GRAY_RADAR_QUAD_CONTACT:
        case MOBY_CLASS_GRAY_RADAR_BLIP_CONTACT:
            draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                radar_origin,
                                                scanned_moby,
                                                2,
                                                0x70808080,
                                                *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_ORANGE_RADAR_CONTACT:
            draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                radar_origin,
                                                scanned_moby,
                                                1,
                                                0x70004080,
                                                *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_YELLOW_RADAR_CONTACT:
            draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                radar_origin,
                                                scanned_moby,
                                                1,
                                                0x70007080,
                                                *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_UNMARKED_COUNTED_TARGET:
        case MOBY_CLASS_UNCOUNTED_MARKED_TARGET:
        case MOBY_CLASS_PRIORITY_LOCK_TARGET:
        case MOBY_CLASS_MARKED_COUNTED_TARGET:
            project_to_screen(v, scanned_moby + 0x10);
            if (MOBY(scanned_moby)->oclass != 0x184) {
                if (MOBY(scanned_moby)->visible != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang =
                            normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                        emit_transformed_2d_vertex_packet(fighter_jet_screen_marker_vertices,
                                                          0x1E,
                                                          0xFFFFF3,
                                                          0x500000FF,
                                                          x,
                                                          y,
                                                          1.0f,
                                                          ang);
                    }
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang =
                            normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                        emit_transformed_2d_vertex_packet(fighter_jet_screen_marker_vertices,
                                                          0x1E,
                                                          0xFFFFF3,
                                                          0x500000FF,
                                                          x,
                                                          y,
                                                          1.3f,
                                                          -ang);
                    }
                }
            }
            if (MOBY(scanned_moby)->oclass != 0x191) {
                *(int *)(ship_vars + 0xE8) = *(int *)(ship_vars + 0xE8) + 1;
            }
            draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                radar_origin,
                                                scanned_moby,
                                                0,
                                                0x70808080,
                                                *(int *)(ship_vars + 0x104));
            break;
        case MOBY_CLASS_OFFSCREEN_ARROW_TARGET:
            qcopy(arrow_target_position_scratch, scanned_moby + 0x10);
            arrow_target_position_scratch[2] += 4.0f;
            project_to_screen(v, arrow_target_position_scratch);
            if (MOBY(scanned_moby)->visible != 0) {
                flag = 1;
                {
                    float x = (v[0] - scr[4]) * 0.0625f;
                    float y = (v[1] - scr[5]) * 0.0625f;
                    float ang = normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                    emit_transformed_2d_vertex_packet(fighter_jet_screen_marker_vertices,
                                                      0x1E,
                                                      0xFFFFF3,
                                                      0x400000FF,
                                                      x,
                                                      y,
                                                      2.0f,
                                                      ang);
                }
                {
                    float x = (v[0] - scr[4]) * 0.0625f;
                    float y = (v[1] - scr[5]) * 0.0625f;
                    float ang = normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                    emit_transformed_2d_vertex_packet(fighter_jet_screen_marker_vertices,
                                                      0x1E,
                                                      0xFFFFF3,
                                                      0x400000FF,
                                                      x,
                                                      y,
                                                      2.5f,
                                                      -ang);
                }
            }
            dd = fast_vec_dist(scanned_moby + 0x10, (&hero.motion.pos));
            if (dd < bestd) {
                near = scanned_moby;
                bestd = dd;
            }
            draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                (char *)arrow_target_position_scratch,
                                                scanned_moby,
                                                0,
                                                0x70808080,
                                                *(int *)(ship_vars + 0x104));
            *(int *)(ship_vars + 0xE8) = *(int *)(ship_vars + 0xE8) + 1;
            break;
        case MOBY_CLASS_FLAG_GATED_MARKED_TARGET:
        case MOBY_CLASS_FLAG_GATED_SCREEN_MARKER_TARGET:
            if (!(MOBY(scanned_moby)->flags & 1)) {
                project_to_screen(v, scanned_moby + 0x10);
                if (MOBY(scanned_moby)->visible != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang =
                            normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                        emit_transformed_2d_vertex_packet(fighter_jet_screen_marker_vertices,
                                                          0x1E,
                                                          0xFFFFF3,
                                                          0xFF00FF00,
                                                          x,
                                                          y,
                                                          1.0f,
                                                          -ang);
                    }
                }
                draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                    radar_origin,
                                                    scanned_moby,
                                                    1,
                                                    0x70808080,
                                                    *(int *)(ship_vars + 0x104));
            }
            break;
        case MOBY_CLASS_STATE_GATED_MARKED_TARGET:
            if (!(MOBY(scanned_moby)->flags & 1) && MOBY(scanned_moby)->state >= 2) {
                project_to_screen(v, scanned_moby + 0x10);
                if (MOBY(scanned_moby)->visible != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang =
                            normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 20.0f);
                        emit_transformed_2d_vertex_packet(fighter_jet_screen_marker_vertices,
                                                          0x1E,
                                                          0xFFFFF3,
                                                          0xFF00FF00,
                                                          x,
                                                          y,
                                                          1.0f,
                                                          -ang);
                    }
                }
                draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                    radar_origin,
                                                    scanned_moby,
                                                    1,
                                                    0x70808080,
                                                    *(int *)(ship_vars + 0x104));
            }
            break;
        case MOBY_CLASS_EXCLUDED_FROM_RADAR:
        case MOBY_CLASS_RADAR_RENDER_BYPASS:
        case MOBY_CLASS_RADAR_ICON_SUPPRESSED:
        case MOBY_CLASS_RADAR_CONTACT_DRAW_BYPASS:
        case MOBY_CLASS_RADAR_DISPATCH_BYPASS:
        case MOBY_CLASS_RADAR_AND_MARKER_DRAW_BYPASS:
        case MOBY_CLASS_RADAR_AND_SCREEN_MARKER_BYPASS:
        case MOBY_CLASS_RADAR_AND_PROJECTED_MARKER_BYPASS:
        case MOBY_CLASS_CONTACT_DRAW_BYPASS:
        case MOBY_CLASS_CONTACT_RENDER_SUPPRESSED:
        case MOBY_CLASS_RADAR_DRAW_SUPPRESSED:
        case MOBY_CLASS_RADAR_CONTACT_SUPPRESSED:
        case MOBY_CLASS_RADAR_ICON_DRAW_BYPASS:
        case MOBY_CLASS_RADAR_ICON_RENDER_BYPASS:
        case MOBY_CLASS_SCREEN_AND_RADAR_MARKERS_SUPPRESSED:
        case MOBY_CLASS_HUD_CONTACT_DRAW_BYPASS:
        case MOBY_CLASS_HUD_CONTACT_RENDER_BYPASS:
            break;
        default:
            if (scanned_moby != 0 && ((char *)MOBY(scanned_moby)->pclass) != 0 &&
                *(short *)(((char *)MOBY(scanned_moby)->pclass) + 0x46) == 5) {
                draw_fighter_jet_radar_contact_icon(radar_rotation,
                                                    radar_origin,
                                                    scanned_moby,
                                                    1,
                                                    0x70808080,
                                                    *(int *)(ship_vars + 0x104));
            }
            break;
        }
        scanned_moby = ((char *)MOBY(scanned_moby)->next);
    }
    if (flag == 0 && near != 0) {
        char *np;
        char *ap;
        qcopy(u, near + 0x10);
        np = near + 0x10;
        u[2] += 4.0f;
        ap = (char *)ship_moby + 0x10;
        project_to_screen(arrow_target_position_scratch, u);
        transpose_rotation_matrix(arrow_direction_rotation_matrix,
                                  fighter_jet_arrow_direction_basis);
        fast_vec_sub(v, np, (&hero.motion.pos));
        transform_vector(v, v, arrow_direction_rotation_matrix);
        v[0] = -v[1];
        v[1] = -v[2];
        v[2] = 0.0f;
        fast_vec_normalize(v, v, 160.0f);
        ang = fast_add_rotations(fast_arc_tan(v[0], v[1]), 1.5707964f);
        v[0] += 256.0f;
        v[1] += 208.0f;
        ship_to_arrow_target_distance = fast_vec_dist(ap, np);
        heading_error_magnitude = absolute_float(fast_subtract_rotations(
            fast_arc_tan(*(float *)(near + 0x10) - *(float *)((char *)ship_moby + 0x10),
                         *(float *)(near + 0x14) - *(float *)((char *)ship_moby + 0x14)),
            *(float *)(ship_vars + 0x38)));
        k = absolute_float(fast_subtract_rotations(
            -fast_arc_tan(vector_distance_xy(ap, np),
                          *(float *)(near + 0x18) - *(float *)((char *)ship_moby + 0x18)),
            *(float *)(ship_vars + 0x34)));
        angular_error_magnitude =
            square_root(heading_error_magnitude * heading_error_magnitude + k * k);
        arrow_pulse_phase = fast_add_rotations(*(float *)(ship_vars + 0x114),
                                               0.1f / (angular_error_magnitude / 3.1415927f));
        *(float *)(ship_vars + 0x114) = arrow_pulse_phase;
        emit_transformed_2d_vertex_packet(
            &D_L17_00162188,
            3,
            0xFFFFF3,
            (truncate_float_to_s32((fast_sin(arrow_pulse_phase) + 1.4f) * 53.0f) << 24) | 0x8000,
            v[0],
            v[1],
            240.0f / ship_to_arrow_target_distance + 1.5f,
            ang);
    }
    return (int)selected_target_moby;
}

extern int D_L17_0015F5C4;

extern void append_rotated_textured_screen_quad(float,
                                                float,
                                                float,
                                                float,
                                                float,
                                                int,
                                                int,
                                                int,
                                                int,
                                                int,
                                                int,
                                                int,
                                                int) __asm__("FUN_L11_00310cf8");

/* Draws the target lock marker: four quads a quarter turn apart; same routine as FUN_L11_00311048 with this
   level's texture. */
void draw_fighter_jet_lock_marker(float         x,
                                  float         y,
                                  float         scale,
                                  float         quad_rotation_radians,
                                  unsigned char red,
                                  unsigned char green,
                                  unsigned char blue,
                                  unsigned char alpha) __asm__("FUN_L17_002ea418");

void draw_fighter_jet_lock_marker(float         x,
                                  float         y,
                                  float         scale,
                                  float         quad_rotation_radians,
                                  unsigned char red,
                                  unsigned char green,
                                  unsigned char blue,

                                  unsigned char alpha) {
    int   texture_handle;
    float quad_size;
    /* Fakematch: retail uses only the low word of the texture; the cast keeps it. */
    texture_handle = ((s32(*)(s32))get_effect_texture)(0x2E);
    quad_size = scale * 20.0f;
    append_rotated_textured_screen_quad(x,
                                        y,
                                        quad_size,
                                        quad_size,
                                        quad_rotation_radians,
                                        0x3F,
                                        0x3F,
                                        texture_handle,
                                        0xFFFFF3,
                                        red,
                                        green,
                                        blue,
                                        alpha);
    quad_rotation_radians = fast_add_rotations(quad_rotation_radians, 1.5707964f);
    append_rotated_textured_screen_quad(x,
                                        y,
                                        quad_size,
                                        quad_size,
                                        quad_rotation_radians,
                                        0x3F,
                                        0x3F,
                                        texture_handle,
                                        0xFFFFF3,
                                        red,
                                        green,
                                        blue,
                                        alpha);
    quad_rotation_radians = fast_add_rotations(quad_rotation_radians, 1.5707964f);
    append_rotated_textured_screen_quad(x,
                                        y,
                                        quad_size,
                                        quad_size,
                                        quad_rotation_radians,
                                        0x3F,
                                        0x3F,
                                        texture_handle,
                                        0xFFFFF3,
                                        red,
                                        green,
                                        blue,
                                        alpha);
    quad_rotation_radians = fast_add_rotations(quad_rotation_radians, 1.5707964f);
    append_rotated_textured_screen_quad(x,
                                        y,
                                        quad_size,
                                        quad_size,
                                        quad_rotation_radians,
                                        0x3F,
                                        0x3F,
                                        texture_handle,
                                        0xFFFFF3,
                                        red,
                                        green,
                                        blue,
                                        alpha);
}

/* picks the moby to home in on, keeping or dropping the current target */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002ED258), where it is exact; names translated to the US level program. */

int        D_L17_0016216C = 600;
extern int select_fighter_jet_target_and_draw_contacts(
    int, char *, char *, char *, float, float, float, char *, char *) __asm__("FUN_L17_002eace8");
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(char **)((char *)(p) + (o)))
#define H(p, o) (*(short *)((char *)(p) + (o)))
#define B(p, o) (*(unsigned char *)((char *)(p) + (o)))
#define FLAG    (hero.ship_ammo)

extern char D_L17_001676D0_lui[] __asm__("D_L17_001676D0") NOT_SDA;
void update_fighter_jet_target_lock(int ship_moby, char *ship_vars) __asm__("FUN_L17_002eb768");

void update_fighter_jet_target_lock(int ship_moby, char *ship_vars) {
    char *excluded_target = 0;
    char *pad;
    char *locked_target;
    char *candidate_target;
    float radar_rotation[4];
    int   lock_timer_reload;
    if (!(D_L17_0015F5C4 >= 3 && D_L17_0015F5C4 <= 4)) {
        fast_dec_timer((int *)(ship_vars + 0x8C));
    }
    if (W(ship_vars, 0x8C) != 0 && P(ship_vars, 0x88) != 0 &&
        H(P(ship_vars, 0x88), 0xA6) == W(ship_vars, 0x128) && B(P(ship_vars, 0x88), 0x20) != 0xFE &&
        B(P(ship_vars, 0x88), 0x20) != 0xFD) {
        pad = (char *)&controller_state;
        if (B(P(ship_vars, 0x88), 0x31) != 0) {
            if (W(pad, 0x1B4) & 0x2000) {
                goto take;
            }
            if (FLAG != 0) {
                goto skip;
            }
        }
    }
    pad = (char *)&controller_state;
    if (W(pad, 0x1B4) & 0x2000) {
    take:
        excluded_target = P(ship_vars, 0x88);
    }
    W(ship_vars, 0xEC) = 0;
    W(ship_vars, 0x128) = -1;
    W(ship_vars, 0x88) = 0;
skip:
    qcopy(radar_rotation, D_L17_001676D0_lui);
    candidate_target =
        (char *)select_fighter_jet_target_and_draw_contacts(ship_moby,
                                                            ship_vars,
                                                            D_L17_001676D0_lui - 0x10,
                                                            (char *)radar_rotation,
                                                            0.19634955f,
                                                            0.19634955f,
                                                            255.0f,
                                                            P(ship_vars, 0x88),
                                                            excluded_target);
    if ((candidate_target != P(ship_vars, 0x88) && W(ship_vars, 0x8C) >= 0x1F5) || FLAG == 0) {
        W(ship_vars, 0xEC) = 0;
        W(ship_vars, 0x128) = -1;
        W(ship_vars, 0x88) = 0;
    }
    if (candidate_target != 0 &&
        (P(ship_vars, 0x88) == 0 ||
         (H(P(ship_vars, 0x88), 0xA6) != 0x3EB && H(candidate_target, 0xA6) == 0x3EB)) &&
        FLAG != 0) {
        P(ship_vars, 0x88) = candidate_target;
        lock_timer_reload = D_L17_0016216C;
        W(ship_vars, 0x128) = H(candidate_target, 0xA6);
        W(ship_vars, 0x8C) = lock_timer_reload;
    }
    if (candidate_target != P(ship_vars, 0x88)) {
        W(ship_vars, 0x118) = W(ship_vars, 0x118) + 1;
        if (scale_game_frames(0x3C) < W(ship_vars, 0x118)) {
            W(ship_vars, 0xEC) = 0;
            W(ship_vars, 0x128) = -1;
            W(ship_vars, 0x88) = 0;
        }
    } else {
        W(ship_vars, 0x118) = 0;
    }
    locked_target = P(ship_vars, 0x88);
    if (locked_target != 0 && W(ship_vars, 0x84) == 0) {
        if (H(locked_target, 0xA6) != W(ship_vars, 0x128) || B(locked_target, 0x20) == 0xFE ||
            B(locked_target, 0x20) == 0xFD) {
            W(ship_vars, 0x88) = 0;
            W(ship_vars, 0x128) = -1;
        }
    }
}

/* Draws the fleet HUD: target marker, ammo icons, gauge sprites and warnings, and refreshes the gauge colour table. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002ED498), where it is exact; names translated to the US level program. */

extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern char  fighter_jet_ammo_icon_strip_data[] __asm__("D_L17_001D99E0");
/* Saved copy of the hp bar CLUT: 256 colour words, filled by one 0x400-byte
   block copy in the fighter jet's init and read back entry by entry (CSM1
   swizzled index of j < 0xFE) when the bar is redrawn. */
int fighter_jet_saved_hp_bar_clut[256] __asm__("D_L17_001D9B68") = {0};

int          lock_hold_frames __asm__("D_L17_00162170") = 500;
int          lock_fade_frames __asm__("D_L17_00162174") = 100;
extern short D_L17_00162180 __attribute__((sda));
extern void  append_rotated_sprite_quad(float,
                                        float,
                                        float,
                                        float,
                                        float,
                                        int,
                                        int,
                                        long,
                                        int,
                                        int,
                                        int,
                                        int,
                                        float,
                                        float) __asm__("FUN_001f5ab0");
extern void  append_scaled_screen_triangle_strip(
     float, float, float, void *, int, int, long) __asm__("FUN_L11_00310ad0");
extern void update_fighter_jet_target_lock(int a, char *p) __asm__("FUN_L17_002eb768");
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");
extern int  font_print_center_small(int, int, long, void *, int) __asm__("FUN_001f6b88");

void draw_fighter_jet_hud(struct Moby *moby) __asm__("FUN_L17_002eb9a8");

void draw_fighter_jet_hud(struct Moby *moby) {
    char         *ship_vars = (char *)moby->pvars;
    char         *screen_dimensions;
    int           locked_target_screen_x;
    int           locked_target_screen_y;
    long          aim_reticle_texture;
    int           i;
    unsigned char ammo_capacity;
    unsigned char lock_marker_fade_byte;
    int           j;
    int           x;
    int           y;
    int           ex;
    int           ey;
    int           lock_marker_blink_value;
    int           hp_bar_fill_threshold;
    int           lo;
    int           hi;
    int           hp_bar_clut_index;
    float         lock_fade_ratio;
    float         k;
    float         lock_marker_fade_factor;
    float         px;
    float         py;
    float         hp_display_step;
    float         hp_display_step_magnitude;

    vu1_add_g_sregister(0x42, 0x8000000044L);
    draw_textured_quad(0x170,
                       (screen_dimensions = (char *)D_0013E500, *(int *)(screen_dimensions + 4)) -
                           0x90,
                       0x80,
                       0x80,
                       0,
                       0,
                       0x80,
                       0x80,
                       0x70808080,
                       get_effect_texture(*(int *)(ship_vars + 0x104) + 0x28));
    draw_textured_quad(0x170,
                       *(int *)(screen_dimensions + 4) - 0x90,
                       0x80,
                       0x80,
                       0,
                       0,
                       0x80,
                       0x80,
                       0x70808080,
                       get_effect_texture(*(int *)(ship_vars + 0x104) + 0x29));
    vu1_add_g_sregister(0x42, 0x8000000048L);
    update_fighter_jet_target_lock((int)moby, ship_vars);
    if (*(int *)(ship_vars + 0x88) != 0) {
        project_world_point_to_screen_pixels(*(char **)(ship_vars + 0x88) + 0x10,
                                             &locked_target_screen_x,
                                             &locked_target_screen_y,
                                             0);
        if (*(int *)(ship_vars + 0x8C) > lock_hold_frames) {
            lock_fade_ratio =
                convert_integer_to_float(*(int *)(ship_vars + 0x8C) - lock_hold_frames);
            lock_fade_ratio = lock_fade_ratio / convert_integer_to_float(lock_fade_frames);
            lock_marker_fade_factor = 2.0f * (1.0f - lock_fade_ratio);
            k = lock_fade_ratio * 5.0f + 1.0f;
            if (lock_marker_fade_factor > 1.0f)
                lock_marker_fade_factor = 1.0f;
            lock_marker_fade_byte = truncate_float_to_s32(lock_marker_fade_factor * 96.0f);
            draw_fighter_jet_lock_marker(
                convert_integer_to_float(locked_target_screen_x),
                convert_integer_to_float(locked_target_screen_y),
                k,
                normalize_angle(convert_integer_to_float(D_L17_0015F5CC) / 30.0f),
                0,
                0xFF,
                0,
                lock_marker_fade_byte);
            *(int *)(ship_vars + 0xEC) = 0;
        } else {
            lock_marker_blink_value = 0xFF;
            if ((*(int *)(ship_vars + 0x8C) / scale_game_frames(0x14)) & 1)
                lock_marker_blink_value = 0;
            draw_fighter_jet_lock_marker(convert_integer_to_float(locked_target_screen_x),
                                         convert_integer_to_float(locked_target_screen_y),
                                         1.0f,
                                         0.0f,
                                         0xFF,
                                         lock_marker_blink_value,
                                         0,
                                         0x60);
            *(int *)(ship_vars + 0xEC) = *(int *)(ship_vars + 0x88);
        }
    }
    y = 0x40;
    x = 0x18;
    for (i = 0; i < hero.ship_ammo_max; i++) {
        if (i < hero.ship_ammo) {
            append_scaled_screen_triangle_strip(
                x, y, 0.5f, fighter_jet_ammo_icon_strip_data, 0x19, 0xFFFFF3, 0x50008F00);
        } else {
            append_scaled_screen_triangle_strip(
                x, y, 0.5f, fighter_jet_ammo_icon_strip_data, 0x19, 0xFFFFF3, 0x20004F00);
        }
        ammo_capacity = hero.ship_ammo_max;
        if (i == ammo_capacity >> 1) {
            y = 0x2E;
            x += 0x1E;
        }
        y += 0x12;
    }
    ex = *(int *)(ship_vars + 0xE0);
    ey = *(int *)(ship_vars + 0xE4);
    aim_reticle_texture = get_effect_texture(0x11);
    append_rotated_sprite_quad(ex,
                               ey,
                               40.0f,
                               40.0f,
                               0.0f,
                               0x3F,
                               0x3F,
                               aim_reticle_texture,
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);
    append_rotated_sprite_quad(ex,
                               ey,
                               40.0f,
                               40.0f,
                               0.0f,
                               0x3F,
                               0x3F,
                               get_effect_texture(0x12),
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);
    append_rotated_sprite_quad(ex,
                               ey,
                               10.0f,
                               10.0f,
                               0.0f,
                               0x1F,
                               0x1F,
                               get_effect_texture(8),
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);
    if (hero.ship_hp < hero.ship_hp_max / 10.0f && (D_L17_0015F5CC / 90) & 1) {
        font_print_center_small(0x100, 0x186, 0x80000080L, get_help_message_text(0x5269), 0x64);
    }
    if (moby->state == 8) {
        font_print_center_small(0x100, 0xC8, 0x80005080L, get_help_message_text(0x523E), 0x64);
    }
    hp_display_step = hero.ship_hp - *(float *)(ship_vars + 0xF0);
    hp_display_step *= 0.2f;
    hp_display_step_magnitude = absolute_float(hp_display_step);
    if (hp_display_step_magnitude > 1.0f)
        hp_display_step /= hp_display_step_magnitude;
    *(float *)(ship_vars + 0xF0) += hp_display_step;
    hp_bar_fill_threshold =
        truncate_float_to_s32(*(float *)(ship_vars + 0xF0) * 251.0f / hero.ship_hp_max) + 2;
    if (hp_bar_fill_threshold > 0xFD)
        hp_bar_fill_threshold = 0xFD;
    if (hp_bar_fill_threshold < 2)
        hp_bar_fill_threshold = 2;
    lo = *(int *)(ship_vars + 0xF4) <= hp_bar_fill_threshold ? *(int *)(ship_vars + 0xF4)
                                                             : hp_bar_fill_threshold;
    hi = *(int *)(ship_vars + 0xF4) < hp_bar_fill_threshold ? hp_bar_fill_threshold
                                                            : *(int *)(ship_vars + 0xF4);
    for (j = lo; j <= hi; j++) {
        hp_bar_clut_index = (j & 0xE7) | ((j & 0x10) >> 1) | ((j & 8) << 1);
        if (j < hp_bar_fill_threshold) {
            (*(int **)&D_L17_00162180)[hp_bar_clut_index] =
                fighter_jet_saved_hp_bar_clut[hp_bar_clut_index];
        } else {
            (*(int **)&D_L17_00162180)[hp_bar_clut_index] = 0x80000000;
        }
    }
    *(int *)(ship_vars + 0xF4) = hp_bar_fill_threshold;
}

/* Builds a rotated offset vector from the moby, flips a facing flag on a pad press, and updates the aim. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EDAF0), where it is exact; names translated to the US level program. */

extern int
spawn_fighter_jet_missile(void *, void *, void *, float, float) __asm__("FUN_L11_00308848");
extern f32           D_L17_001620A0_gp __asm__("D_L17_001620A0") __attribute__((sda));
extern unsigned char D_0013A4E0_u[] __asm__("D_0013A4E0");

extern short fighter_jet_aim_direction_base_length_gp __asm__("D_L17_0016206C")
    __attribute__((sda));
int  D_L17_00162070 = 4;
void update_fighter_jet_aim_and_fire_projectile(char *moby,
                                                char *ship_vars,
                                                float aim_azimuth,
                                                float aim_elevation) __asm__("FUN_L17_002ec000");

void update_fighter_jet_aim_and_fire_projectile(char *moby,
                                                char *ship_vars,
                                                float aim_azimuth,

                                                float aim_elevation) {
    char aim_direction[16];
    char projectile_spawn_position[16];
    char projectile_spawn_offset[16];
    char aim_world_position[16];
    build_spherical_offset(aim_direction,
                           *(float *)&fighter_jet_aim_direction_base_length_gp *
                               *(float *)&D_L17_001620A0_gp,
                           aim_azimuth,
                           aim_elevation);
    fast_vec_add(aim_direction, aim_direction, ship_vars);
    if (fast_dec_timer(ship_vars + 0x80) && (*(int *)(D_0013A4E0_u + 0x2610) & 0x84)) {
        allocate_voice_for_target_entry(3, 0, (int)moby);
        fast_vec_normalize(projectile_spawn_offset, aim_direction, 2.2f);
        moby_point_position(moby, 3 + (ship_vars[0x61] & 1), projectile_spawn_position);
        fast_vec_add(projectile_spawn_position, projectile_spawn_position, projectile_spawn_offset);
        spawn_fighter_jet_missile(moby, aim_direction, projectile_spawn_position, 200.0f, -1.0f);
        *(int *)(ship_vars + 0x80) = D_L17_00162070;
        ship_vars[0x61] ^= 1;
    }
    fast_vec_normalize(aim_direction, aim_direction, 23.0f);
    fast_vec_add(aim_world_position, aim_direction, moby + 0x10);
    project_world_point_to_screen_pixels(aim_world_position, ship_vars + 0xE0, ship_vars + 0xE4, 0);
}
/* Fires a projectile from the moby when the trigger is ready and the player presses a button. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EDC40), where it is exact; names translated to the US level program. */

extern int
spawn_fighter_jet_rocket(void *, void *, int, void *, float, int) __asm__("FUN_L13_002e6a58");

extern short fighter_jet_rocket_base_scale_gp __asm__("D_L17_00162074") __attribute__((sda));
int          D_L17_00162078 = 30;
void         try_fire_fighter_jet_rocket(char *moby, char *ship_vars) __asm__("FUN_L17_002ec150");

void try_fire_fighter_jet_rocket(char *moby, char *ship_vars) {
    char projectile_spawn_position[16];
    char projectile_spawn_offset[16];
    if (fast_dec_timer(ship_vars + 0x84) && (*(int *)(D_0013A4E0_u + 0x2610) & 0x28)) {
        if (hero.ship_ammo != 0) {
            char          *projectile_initial_rotation = moby + 0x40;
            float          projectile_scale;
            int            scaled_frame_count;
            unsigned short t = (*(unsigned short *)(ship_vars + 0x6A) + 1) & 3;
            *(unsigned short *)(ship_vars + 0x6A) = t;
            moby_point_position(moby, t + 7, projectile_spawn_position);
            fast_vec_normalize(projectile_spawn_offset, ship_vars, 0.3f);
            fast_vec_add(
                projectile_spawn_position, projectile_spawn_position, projectile_spawn_offset);
            projectile_scale =
                *(float *)&fighter_jet_rocket_base_scale_gp * *(float *)&D_L17_001620A0_gp;
            scaled_frame_count = scale_game_frames(300);
            if (spawn_fighter_jet_rocket(moby,
                                         projectile_spawn_position,
                                         *(int *)(ship_vars + 0xEC),
                                         projectile_initial_rotation,
                                         projectile_scale,
                                         scaled_frame_count)) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                hero.ship_ammo -= 1;
                *(int *)(ship_vars + 0x84) = D_L17_00162078;
            }
        }
    }
}

/* Flies the player's ship: throttle and stick input, steering toward the target, camera, and collision response. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EDE50), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad00[0x10];
    sceVu0FVECTOR  pos;
    unsigned char  state;
    char           pad21[0x13];
    unsigned short flags;
    char           pad36[0xA];
    float          r40;
    float          r44;
    float          r48;
    char           pad4C[0x70];
    unsigned char  bc;
} FighterJetMoby;

typedef struct {
    char          pad00[0x10];
    sceVu0FVECTOR v10;
    float         f20, f24, f28, f2C;
    float         f30, f34, f38, f3C;
    char          pad40[0x20];
    unsigned char sel;
    unsigned char flip;
    short         s62;
    float         speed;
    short         count;
    short         s6A;
    float         f6C;
    float         f70;
    char          pad74[0x1C];
    float         f90, f94, f98, f9C;
    float         fA0, fA4, fA8, fAC;
    float         fB0, fB4, fB8, fBC;
    float         fC0, fC4, fC8, fCC;
    float         fD0, fD4, fD8;
    int           iDC;
    char          padE0[0x2C];
    int           i10C;
    int           i110;
} FighterJetVars;

typedef unsigned int Q_c4 __attribute__((mode(TI), aligned(16)));

typedef union {
    Q_c4          q;
    sceVu0FVECTOR v;
} UQ_c4;

typedef struct {
    UQ_c4 e[2];
} T2_c4;

extern char                *D_L17_001600EC;
extern char                 D_L17_00174740[];
extern float D_L17_0016D3F0 NOT_SDA;
extern float
spring_angle(char *, char *, int, float, float, float, float) __asm__("FUN_L00_0025bc98");
extern float interpolate_value_with_repeated_squaring(
    int, int, float, float, float, float, float) __asm__("FUN_L13_002ba088");
extern float fast_add_rotations(float, float);
extern float vector_length_xy(void *);

#include "rnc/overlay/l17/mobys/fighter_jet_data/update_fighter_jet_flight_8.h"

int D_L17_00162000 = 120;

#include "rnc/overlay/l17/mobys/fighter_jet_data/update_fighter_jet_flight_7.h"

int D_L17_00162014 = 5;

#include "rnc/overlay/l17/mobys/fighter_jet_data/update_fighter_jet_flight_6.h"

extern float D_L17_00162024 __attribute__((sda));
extern float D_L17_00162028 __attribute__((sda));
extern float D_L17_0016202C __attribute__((sda));
extern float D_L17_00162030 __attribute__((sda));

#include "rnc/overlay/l17/mobys/fighter_jet_data/update_fighter_jet_flight_5.h"

extern float D_L17_00162044 __attribute__((sda));
extern float D_L17_00162048 __attribute__((sda));

#include "rnc/overlay/l17/mobys/fighter_jet_data/update_fighter_jet_flight_4.h"

extern float D_L17_0016205C __attribute__((sda));

#include "rnc/overlay/l17/mobys/fighter_jet_data/update_fighter_jet_flight_3.h"

extern float D_L17_0016208C __attribute__((sda));
extern float D_L17_00162090 __attribute__((sda));
extern float D_L17_0016209C __attribute__((sda));
extern float D_L17_001620A0_gp __asm__("D_L17_001620A0") __attribute__((sda));

#include "rnc/overlay/l17/mobys/fighter_jet_data/update_fighter_jet_flight_2.h"

extern float D_L17_00162138 __attribute__((sda));
extern float D_L17_0016213C __attribute__((sda));
extern float D_L17_00162140 __attribute__((sda));

#include "rnc/overlay/l17/mobys/fighter_jet_data/update_fighter_jet_flight.h"

extern float D_L17_00162160 __attribute__((sda));
extern float D_L17_00162164 __attribute__((sda));
extern float D_L17_00162168 __attribute__((sda));
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
int          D_L17_0016215C = 10;
float        D_L17_001621F0[2] = {512.0f, 512.0f};

unsigned short l17_fighter_jet_collision_death_respawn_delay __asm__("D_L17_0016217C") = 0x1e;
extern void    reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void    set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void    set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void    draw_fighter_jet_hud();
extern void    transform_vector_by_basis(void *, void *, void *);
extern void    update_view_context() __asm__("func_001F2D98");
void           update_fighter_jet_aim_and_fire_projectile(char *moby,
                                                          char *obj,
                                                          float p2,
                                                          float p3) __asm__("FUN_L17_002ec000");
void           try_fire_fighter_jet_rocket(char *moby, char *obj) __asm__("FUN_L17_002ec150");
void           adjust_fighter_jet_steering_for_flight_bounds(
              char  *moby,
              float *horizontal_steering_input,
              float *vertical_steering_input) __asm__("FUN_L17_002edf10");

void        adjust_fighter_jet_steering_for_flight_bounds(char  *moby,
                                                          float *a,
                                                          float *b) __asm__("FUN_L17_002edf10");
extern char D_0013C940[];
extern void func_L00_0025F4A8_alt(void *,
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

void update_fighter_jet_flight(FighterJetMoby *ship_moby,
                               FighterJetVars *ship_vars) __asm__("FUN_L17_002ec360");

void update_fighter_jet_flight(FighterJetMoby *ship_moby, FighterJetVars *ship_vars) {
    sceVu0FVECTOR pre_movement_ship_position;
    sceVu0FVECTOR death_explosion_velocity;
    UQ_c4         movement_delta;
    sceVu0FVECTOR side;
    UQ_c4         up;
    sceVu0FVECTOR camera_rotation;
    sceVu0FVECTOR target;
    sceVu0FVECTOR target_record_sum_or_camera_local_offset;
    float         horizontal_steering_input;
    float         vertical_steering_input;
    float         ka;
    float         kb;
    float         lenA;
    float         lenB;
    float         distance_to_default_target;
    float         target_height_distance, target_height_distance_for_moby_pitch;
    float         x, y;
    float         half_fov, tn;
    float         ratio;
    float         collision_adjusted_pitch;
    float         k;
    char         *pad;
    struct Hero  *steering_input_hero;
    char         *e;
    char         *collision_result;
    int           voice_slot_index;
    int           hc;

    movement_delta.q = 0;
    up.q = 0;
    movement_delta.v[2] = frame_time * 8.0f * D_L17_001620A0_gp;
    up.v[2] = 1.0f;
    ship_moby->flags &= 0xFEFF;
    *(Q_c4 *)death_explosion_velocity = movement_delta.q;
    if (ship_moby->bc == 0) {
        if (D_L17_00162000 < ship_vars->count) {
            pad = (char *)((char *)&D_0013C940);
            if (*(int *)(pad + 0x1B4) & 0x40) {
                voice_slot_index = ship_vars->i10C;
                if (voice_slot_index != -1) {
                    e = (char *)&voice_pool + voice_slot_index * 0x70;
                    if (*(char **)(e + 0x88) == (char *)ship_moby &&
                        *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                ship_vars->i10C = -1;
                ship_vars->i10C = allocate_voice_for_target_entry(1, 4, (int)ship_moby);
            } else if (*(int *)(pad + 0x1B8) & 0x40) {
                voice_slot_index = ship_vars->i10C;
                if (voice_slot_index != -1) {
                    e = (char *)&voice_pool + voice_slot_index * 0x70;
                    if (*(char **)(e + 0x88) == (char *)ship_moby &&
                        *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                ship_vars->i10C = -1;
            }
            pad = (char *)((char *)&D_0013C940);
            if (*(int *)(pad + 0x1B0) & 0x40) {
                float accelerated_speed =
                    ship_vars->speed + fighter_jet_boost_speed_step_gp * D_L17_001620A0_gp;
                float max = fighter_jet_boost_speed_limit_gp * D_L17_001620A0_gp;
                ship_vars->speed = accelerated_speed;
                if (max < accelerated_speed) {
                    ship_vars->speed = max;
                }
            } else {
                float hi = fighter_jet_cruise_speed_gp * D_L17_001620A0_gp;
                float step = fighter_jet_cruise_speed_step_gp * D_L17_001620A0_gp;
                float current_speed = ship_vars->speed;
                if (current_speed < hi - step) {
                    ship_vars->speed = current_speed + step;
                } else if (hi < current_speed) {
                    ship_vars->speed =
                        current_speed +
                        (hi - current_speed) * fighter_jet_cruise_speed_return_factor_gp;
                } else {
                    ship_vars->speed = hi;
                }
            }
        } else {
            ship_vars->speed += fighter_jet_startup_speed_step_gp * D_L17_001620A0_gp;
            ship_vars->count += 1;
        }
    } else {
        ship_vars->speed = 0.0001f;
    }

    if (D_L17_00162014 < ship_vars->count) {
        steering_input_hero = &hero;
        x = steering_input_hero->stick_x;
        y = steering_input_hero->stick_y;
        ka = fighter_jet_horizontal_steering_settle_threshold_scale_gp;
        kb = fighter_jet_vertical_steering_settle_threshold_scale_gp;
        horizontal_steering_input = x;
        vertical_steering_input = y;
        if (*(int *)(((char *)&pad_held)) & 0x200) {
            horizontal_steering_input = x / D_L17_00162028 * D_L17_00162030;
            vertical_steering_input = y / D_L17_00162024 * D_L17_0016202C;
            ka = fighter_jet_rescaled_horizontal_steering_settle_threshold_scale_gp;
            kb = fighter_jet_rescaled_vertical_steering_settle_threshold_scale_gp;
        }
        adjust_fighter_jet_steering_for_flight_bounds(
            (char *)ship_moby, &horizontal_steering_input, &vertical_steering_input);
        vendor_spring(&ship_vars->f6C,
                      &ship_vars->f90,
                      horizontal_steering_input,
                      fighter_jet_horizontal_steering_error_gain_gp,
                      fighter_jet_horizontal_steering_velocity_damping_gp,
                      ka);
        vendor_spring(&ship_vars->f70,
                      &ship_vars->f94,
                      vertical_steering_input,
                      fighter_jet_vertical_steering_error_gain_gp,
                      fighter_jet_vertical_steering_velocity_damping_gp,
                      kb);
        spring_angle((char *)&ship_vars->f30,
                     (char *)&ship_vars->f98,
                     0,
                     ship_vars->f6C * D_L17_0016208C,
                     fighter_jet_roll_error_gain_gp,
                     fighter_jet_roll_velocity_damping_gp,
                     fighter_jet_roll_settle_threshold_scale_gp);
        ship_vars->f34 = fast_subtract_rotations(ship_vars->f34, ship_vars->f70 * D_L17_00162024);
        ship_vars->f38 = fast_subtract_rotations(ship_vars->f38, ship_vars->f6C * D_L17_00162028);
        if (ship_vars->f34 > 1.3962634f) {
            ship_vars->f34 = 1.3962634f;
        } else if (ship_vars->f34 < -1.3962634f) {
            ship_vars->f34 = -1.3962634f;
        }
        if (ship_moby->r44 > 1.3962634f) {
            ship_moby->r44 = 1.3962634f;
        } else if (ship_moby->r44 < -1.3962634f) {
            ship_moby->r44 = -1.3962634f;
        }
    }

    lenA = 310.0f;
    lenB = 500.0f;
    if (ship_vars->i110 != -1) {
        int   target_record_byte_offset = ship_vars->i110 << 7;
        char *target_record_array = D_L17_001600EC;
        qcopy(target, ((char(*)[128])target_record_array)[ship_vars->i110] + 0x30);
        lenA = fast_vec_length(target_record_array + target_record_byte_offset + 0x20);
        fast_vec_add(target_record_sum_or_camera_local_offset,
                     (ship_vars->i110 << 7) + D_L17_001600EC,
                     (ship_vars->i110 << 7) + D_L17_001600EC + 0x10);
        lenB = fast_vec_length(target_record_sum_or_camera_local_offset);
    } else {
        qcopy(target, D_L17_001621F0);
    }

    distance_to_default_target = vector_distance_xy(ship_moby->pos, D_L17_001621F0);
    ship_vars->f38 = interpolate_value_with_repeated_squaring(
        1,
        5,
        distance_to_default_target,
        ship_vars->f38,
        fast_arc_tan(target[0] - ship_moby->pos[0], target[1] - ship_moby->pos[1]),
        50.0f,
        lenB);
    ship_moby->r48 = interpolate_value_with_repeated_squaring(
        1,
        5,
        distance_to_default_target,
        ship_moby->r48,
        fast_arc_tan(target[0] - ship_moby->pos[0], target[1] - ship_moby->pos[1]),
        50.0f,
        lenB);
    target_height_distance = absolute_float(target[2] - ship_moby->pos[2]);
    ship_vars->f34 = interpolate_value_with_repeated_squaring(
        1,
        5,
        target_height_distance,
        ship_vars->f34,
        -fast_arc_tan(vector_distance_xy(ship_moby->pos, target), target[2] - ship_moby->pos[2]),
        50.0f,
        lenA);
    target_height_distance_for_moby_pitch = absolute_float(target[2] - ship_moby->pos[2]);
    ship_moby->r44 = interpolate_value_with_repeated_squaring(
        1,
        5,
        target_height_distance_for_moby_pitch,
        ship_moby->r44,
        -fast_arc_tan(vector_distance_xy(ship_moby->pos, target), target[2] - ship_moby->pos[2]),
        50.0f,
        lenA);
    build_spherical_offset(movement_delta.v, ship_vars->speed, ship_vars->f38, -ship_vars->f34);
    fast_vec_normalize(ship_vars->v10, movement_delta.v, 1.0f);
    qcopy(pre_movement_ship_position, ship_moby->pos);
    if (ship_moby->bc == 0) {
        fast_vec_add(ship_moby->pos, ship_moby->pos, movement_delta.v);
    }
    {
        float lo = fighter_jet_cruise_speed_gp;
        float sp = ship_vars->speed - lo;
        float range = fighter_jet_boost_speed_limit_gp - lo;
        if (sp < 0.0f) {
            ratio = 0.0f / range;
        } else {
            ratio = sp / range;
        }
    }
    ship_vars->fC4 = ratio;
    {
        T2_c4 camera_offset_targets;
        T2_c4 wr;
        float ship_rotation_basis[12];
        wr.e[0].q = 0;
        wr.e[1].q = 0;
        wr.e[0].v[0] = (fighter_jet_camera_base_offset_x_gp - ship_vars->f70 * D_L17_00162138 -
                        ratio * D_L17_00162140) *
                       D_L17_0016209C;
        wr.e[0].v[1] = ship_vars->f6C * D_L17_00162090 * D_L17_0016209C;
        wr.e[0].v[2] = (fighter_jet_camera_base_offset_z_gp + ship_vars->f70 * D_L17_0016213C) *
                       D_L17_0016209C;
        wr.e[1].v[0] = fighter_jet_camera_secondary_offset_x_gp * D_L17_0016209C;
        wr.e[1].v[2] = fighter_jet_camera_secondary_offset_z_gp * D_L17_0016209C;
        camera_offset_targets.e[0].q = wr.e[0].q;
        camera_offset_targets.e[1].q = wr.e[1].q;
        vendor_spring(&ship_vars->fB4,
                      &ship_vars->fC8,
                      camera_offset_targets.e[ship_vars->sel].v[0],
                      fighter_jet_camera_offset_x_error_gain_gp,
                      fighter_jet_camera_offset_x_velocity_damping_gp,
                      fighter_jet_camera_offset_x_settle_threshold_scale_gp);
        vendor_spring(&ship_vars->fB8,
                      &ship_vars->fCC,
                      camera_offset_targets.e[ship_vars->sel].v[1],
                      fighter_jet_camera_offset_y_error_gain_gp,
                      fighter_jet_camera_offset_y_velocity_damping_gp,
                      fighter_jet_camera_offset_y_settle_threshold_scale_gp);
        vendor_spring(&ship_vars->fBC,
                      &ship_vars->fD0,
                      camera_offset_targets.e[ship_vars->sel].v[2],
                      fighter_jet_camera_offset_z_error_gain_gp,
                      fighter_jet_camera_offset_z_velocity_damping_gp,
                      fighter_jet_camera_offset_z_settle_threshold_scale_gp);
        target_record_sum_or_camera_local_offset[0] = ship_vars->fB4;
        target_record_sum_or_camera_local_offset[1] = ship_vars->fB8;
        target_record_sum_or_camera_local_offset[2] = ship_vars->fBC;
        target_record_sum_or_camera_local_offset[3] = 0.0f;
        euler_rotation_basis(ship_rotation_basis, (char *)&ship_vars->f30);
        transform_vector_by_basis(
            target, target_record_sum_or_camera_local_offset, ship_rotation_basis);
        fast_vec_add(wr.e[0].v, ship_moby->pos, target);
        set_camera_position(wr.e[0].v);
        wr.e[1].v[0] =
            fast_add_rotations(ship_vars->f30 * D_L17_00162048, ship_vars->f6C * D_L17_00162044);
        wr.e[1].v[1] = fast_subtract_rotations(
            fast_add_rotations(ship_vars->f34, fighter_jet_camera_pitch_bias_gp),
            ship_vars->f70 * D_L17_00162024);
        wr.e[1].v[2] = ship_vars->f38;
        spring_angle((char *)&ship_vars->f20,
                     (char *)&ship_vars->fA0,
                     0,
                     wr.e[1].v[0],
                     fighter_jet_camera_roll_error_gain_gp,
                     fighter_jet_camera_roll_velocity_damping_gp,
                     fighter_jet_camera_roll_settle_threshold_scale_gp);
        spring_angle((char *)&ship_vars->f24,
                     (char *)&ship_vars->fA4,
                     0,
                     wr.e[1].v[1],
                     fighter_jet_camera_pitch_error_gain_gp,
                     fighter_jet_camera_pitch_velocity_damping_gp,
                     fighter_jet_camera_pitch_settle_threshold_scale_gp);
        spring_angle((char *)&ship_vars->f28,
                     (char *)&ship_vars->fA8,
                     0,
                     wr.e[1].v[2],
                     fighter_jet_camera_yaw_error_gain_gp,
                     fighter_jet_camera_yaw_velocity_damping_gp,
                     fighter_jet_camera_yaw_settle_threshold_scale_gp);
    }
    camera_rotation[0] = ship_vars->f20;
    camera_rotation[2] = ship_vars->f28;
    camera_rotation[1] = ship_vars->f24;
    set_camera_rotation(camera_rotation);
    vendor_spring(&ship_vars->fB0,
                  &ship_vars->fC0,
                  fighter_jet_cruise_fov_gp +
                      (fighter_jet_boost_fov_gp - fighter_jet_cruise_fov_gp) * ratio,
                  fighter_jet_fov_error_gain_gp,
                  fighter_jet_fov_velocity_damping_gp,
                  fighter_jet_fov_settle_threshold_scale_gp);
    half_fov = ship_vars->fB0 * 0.5f;
    tn = fast_sin(half_fov) / fast_cos(half_fov);
    D_L17_0016D3F0 = tn;
    update_view_context();
    ship_moby->r40 = fast_add_rotations(ship_vars->f30, ship_vars->f6C * D_L17_00162160);
    ship_moby->r44 = fast_add_rotations(ship_vars->f34, -ship_vars->f70 * D_L17_00162164);
    ship_moby->r48 = fast_add_rotations(ship_vars->f38, -ship_vars->f6C * D_L17_00162168);
    if (coll_sphere(1.2f, ship_moby->pos, 0, ship_moby)) {
        collision_result = D_L17_00174740;
        if (*(char **)(collision_result + 0x18) == 0 ||
            (hc = *(short *)(*(char **)(collision_result + 0x18) + 0xA6),
             hc != 0 && hc != 0x192 && hc != 0x4C2 && hc != 0x4C3 && hc != 0x52 && hc != 0x53)) {
            qcopy(ship_moby->pos, collision_result + 0x30);
            reflect_vector(movement_delta.v, movement_delta.v, collision_result + 0x40);
            ship_vars->f38 = fast_add_rotations(
                ship_vars->f38,
                fast_subtract_rotations(fast_arc_tan(movement_delta.v[0], movement_delta.v[1]),
                                        ship_vars->f38) *
                    fighter_jet_impact_yaw_response_scale_gp);
            collision_adjusted_pitch = fast_add_rotations(
                ship_vars->f34,
                fast_subtract_rotations(
                    -fast_arc_tan(vector_length_xy(movement_delta.v), movement_delta.v[2]),
                    ship_vars->f34) *
                    fighter_jet_impact_pitch_response_scale_gp);
            k = ship_vars->speed * D_L17_0016205C;
            ship_vars->f34 = collision_adjusted_pitch;
            ship_vars->speed = k;
            if (*(char **)(collision_result + 0x18) != 0 &&
                *(short *)(*(char **)(collision_result + 0x18) + 0xA6) != 0x15B) {
                if (ship_vars->iDC == 0) {
                    hero.ship_hp = hero.ship_hp - fighter_jet_collision_damage_gp;
                    if (hero.ship_hp < 0.0f) {
                        fast_vec_sub(side, collision_result + 0x50, collision_result + 0x60);
                        fast_vec_normalize(side, side, 1.0f);
                        reflect_vector(
                            death_explosion_velocity, movement_delta.v, collision_result + 0x40);
                        fast_vec_normalize(death_explosion_velocity,
                                           death_explosion_velocity,
                                           frame_time + frame_time);
                        fast_vec_normalize(up.v, collision_result + 0x40, 1.0f);
                        func_L00_0025F4A8_alt(ship_moby,
                                              death_explosion_velocity,
                                              0,
                                              8.0f,
                                              100.0f,
                                              0x1E,
                                              0xA,
                                              0x18,
                                              16.0f,
                                              8.0f,
                                              9.0f,
                                              1.0f,
                                              -1,
                                              50.0f,
                                              1,
                                              1,
                                              -1,
                                              0);
                        ship_moby->state = 7;
                    }
                    ship_vars->iDC = D_L17_0016215C;
                }
            } else {
                *(int *)&hero.ship_hp = 0;
                func_L00_0025F4A8_alt(ship_moby,
                                      death_explosion_velocity,
                                      0,
                                      8.0f,
                                      100.0f,
                                      0x1E,
                                      0xA,
                                      0x18,
                                      16.0f,
                                      8.0f,
                                      9.0f,
                                      1.0f,
                                      -1,
                                      50.0f,
                                      1,
                                      1,
                                      -1,
                                      0);
                ship_vars->s62 = l17_fighter_jet_collision_death_respawn_delay;
                ship_moby->state = 7;
                ship_moby->flags |= 0x41;
            }
        }
    }
    add_draw_callback(draw_fighter_jet_hud, ship_moby);
    try_fire_fighter_jet_rocket((char *)ship_moby, (char *)ship_vars);
    update_fighter_jet_aim_and_fire_projectile(
        (char *)ship_moby,
        (char *)ship_vars,
        fast_add_rotations(ship_vars->f28, -ship_vars->f6C / 7.0f),
        -fast_add_rotations(ship_vars->f24, -(ship_vars->f70 / 7.0f) - 0.1f));
}

void        mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
/* Per-frame update for the player's ship moby on Veldin Orbit: takes hits, boards and leaves the ship, and runs the flight states. */

typedef struct {
    char           pad00[0x10];
    sceVu0FVECTOR  pos;
    unsigned char  state;
    char           pad21[3];
    char          *pclass;
    char           pad28[4];
    float          scale;
    unsigned char  b30;
    char           pad31;
    unsigned short h32;
    unsigned short flags;
    char           pad36[0xA];
    sceVu0FVECTOR  rot;
    char           pad50[0x28];
    char          *vars;
    char           pad7C[0x28];
    unsigned char  bA4;
    char           padA5;
    unsigned short oclass;
    char           padA8[8];
    unsigned char  bB0;
} ShipMoby;

typedef struct {
    sceVu0FVECTOR dir;
    sceVu0FVECTOR v10;
    float         f20, f24, f28, f2C;
    sceVu0FVECTOR rot;
    sceVu0FVECTOR home_pos;
    sceVu0FVECTOR home_rot;
    unsigned char b60;
    char          pad61;
    short         hit_timer;
    float         speed;
    short         s68;
    short         s6A;
    float         f6C;
    float         f70;
    char          pad74[0xC];
    int           i80;
    int           i84;
    char          pad88[0x24];
    float         fAC;
    float         fB0;
    float         fB4;
    float         fB8;
    float         fBC;
    float         fC0;
    char          padC4[0x18];
    int           timerDC;
    char          padE0[8];
    int           iE8;
    char          padEC[4];
    int           iF0;
    int           iF4;
    int           timerF8;
    int           exit_path;
    int           exit_path2;
    int           i104;
    int           voice;
    int           voice2;
    char          pad110[0xC];
    int           i11C;
    int           enter_path;
    int           i124;
    char          pad128[8];
    float         fade;
} ShipVars;

typedef struct {
    char  pad00[0x2C];
    float damage;
    int   flags;
} ShipHit;

extern char D_0013CAE4_ed[] __asm__("D_0013CAE4");

typedef struct {
    char           pad0[0xA];
    unsigned short idx;
    char           padC[4];
} ShipLevelEntry;

char fighter_jet_missing_victory_placement_message[56] __asm__("D_L17_001E6E00") =
    "no final place for jet during victory, so use init pos\n";

char fighter_jet_missing_victory_hero_placement_message[] __asm__("D_L17_001E6E38") =
    "no final place for hero during victory, so just fall\n";

#include "rnc/overlay/l17/mobys/fighter_jet_globals.h"

extern float fighter_jet_boarding_distance_limit_gp __asm__("D_L17_00161FE0") __attribute__((sda));
extern float fighter_jet_initial_flight_speed_gp __asm__("D_L17_00161FE4") __attribute__((sda));
extern float D_L17_00162054 __attribute__((sda));
extern float fighter_jet_starting_hp_gp __asm__("D_L17_00162058") __attribute__((sda));
/* tan(fov / 2) of the view (set by FUN_L17_002ec360); read as an array element, which keeps
   the load after the stores to the ship vars like retail. */
extern float  D_L17_0016D3F0_a[] __asm__("D_L17_0016D3F0");
unsigned char l17_fighter_jet_starting_ammo_gp __asm__("D_L17_00162064") = 0xa;
unsigned char l17_fighter_jet_ammo_capacity_gp __asm__("D_L17_00162068") = 0x14;
extern float  fighter_jet_max_hp_gp __asm__("D_L17_00162084") __attribute__((sda));
extern float  D_L17_00162088 __attribute__((sda));
int           D_L17_00162178 = 180;
char         *hp_bar_clut __asm__("D_L17_00162180") = 0;
int           hp_bar_saved __asm__("D_L17_00162184") = 0;

typedef struct {
    char pad[0x98];
    int  unk98;
} ShipCamera;

typedef struct {
    char b[0x400];
} ShipBlock400;

extern float FUN_00213ed8(float *, float, float);
extern int   FUN_00215130(int, int);
extern void  refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern void  set_animation_parameter(int, int) __asm__("FUN_001ff570");
extern void  debug_print(char *) __asm__("FUN_001e93b0");
extern float jet_distance(void *, void *) __asm__("FUN_L00_00250430");
extern void  jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern int   remove_help_message_for_selector(int) __asm__("FUN_L00_002644e0");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  jet_end_mount(int) __asm__("FUN_L00_002eac18");
extern void  jet_camera_mode(void) __asm__("FUN_L00_00203b18");
extern void  clear_jet_camera_mode_latch(void) __asm__("FUN_L00_00203b78");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void  request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void  emit_type_74_particles_from_moby_joints(void *, void *) __asm__("FUN_L11_003112b8");
extern void  jet_group_state(int, int, int, int) __asm__("FUN_L13_002650a8");
extern void  FUN_L17_0021e530(int, int);

#include "rnc/overlay/l17/mobys/fighter_jet_data/fighter_jet_update.h"

extern int            D_L17_0015F608;
extern char          *fighter_jet_hp_bar_clut_lookup_base __asm__("D_L17_0015F420");
extern ShipLevelEntry l17_fighter_jet_hp_bar_clut_entries[] __asm__("D_L17_0016DA80");
void                  fighter_jet_update(ShipMoby *moby) __asm__("FUN_L17_002ed018");

/* Moby class 1379 */
void fighter_jet_update(ShipMoby *moby) {
    enum {
        FIGHTER_JET_STATE_INITIALIZE = 0,
        FIGHTER_JET_STATE_WAIT_FOR_BOARDING = 1,
        FIGHTER_JET_STATE_BOARDING_FADE = 2,
        FIGHTER_JET_STATE_PILOTED_FLIGHT = 4,
        FIGHTER_JET_STATE_DISMOUNT = 5,
        FIGHTER_JET_STATE_DEATH_RESPAWN_DELAY = 7,
        FIGHTER_JET_STATE_VICTORY_EXIT = 8,
        FIGHTER_JET_STATE_FADE_THEN_REMOVE = 9
    };
    ShipVars     *ship_vars;
    ShipHit      *hit;
    sceVu0FVECTOR dir;
    char         *e;
    int           idx;
    int           board;
    float         k;
    float         fov;

    if (moby != 0) {
        ship_vars = (ShipVars *)moby->vars;
        fast_dec_timer(&ship_vars->timerDC);
        if (moby->state != 7 && moby->state != 1 && moby->state != 2) {
            hit = (ShipHit *)find_moby_hit(moby, 0x30001, 0);
            if (hit != 0 && moby->state == 4) {
                struct Hero *g = &hero;
                if (hit->damage < g->ship_hp) {
                    g->ship_hp -= hit->damage;
                    if (hit->flags & 1) {
                        LevelCamera *s = &l17_camera;
                        s->shake_amount = 1.0f;
                        s->shake_frames = scale_game_frames(4);
                        k = hit->damage * D_L17_00162088;
                        build_spherical_offset(
                            dir, ship_vars->speed, ship_vars->rot[2], -ship_vars->rot[1]);
                        clamp_vector_length(
                            (char *)hit + 0x10, (char *)hit + 0x10, frame_time * 3.0f);
                        fast_vec_add(moby->pos, moby->pos, (char *)hit + 0x10);
                        reflect_vector(dir, dir, (char *)hit + 0x10);
                        ship_vars->rot[2] =
                            fast_add_rotations(ship_vars->rot[2],
                                               fast_subtract_rotations(fast_arc_tan(dir[0], dir[1]),
                                                                       ship_vars->rot[2]) *
                                                   fighter_jet_impact_yaw_response_scale_gp * k);
                        ship_vars->rot[1] = fast_add_rotations(
                            ship_vars->rot[1],
                            fast_subtract_rotations(fast_arc_tan(vector_length_xy(dir), dir[2]),
                                                    ship_vars->rot[1]) *
                                fighter_jet_impact_pitch_response_scale_gp * k);
                    }
                } else {
                    build_spherical_offset(
                        dir, ship_vars->speed, ship_vars->rot[2], -ship_vars->rot[1]);
                    {
                        LevelCamera *s = &l17_camera;
                        s->shake_amount = 0.2f;
                        s->shake_frames = scale_game_frames(5);
                    }
                    g->ship_hp = 0.0f;
                    moby->state = 7;
                    ship_vars->hit_timer = scale_game_frames(30);
                    g->motion.pos.f[2] += 0.2f;
                    ((ShipCamera *)g->moby)->unk98 = 0;
                    FUN_L17_0021e530(0, 1);
                    g->ship_moby = 0;
                    g->ship_oclass = -1;
                    spawn_explosion(moby,
                                    dir,
                                    0,
                                    0.0f,
                                    0.0f,
                                    10,
                                    3,
                                    16,
                                    4.0f,
                                    2.0f,
                                    9.0f,
                                    -1,
                                    1.0f,
                                    15.0f,
                                    1,
                                    1,
                                    -1,
                                    0);
                    moby->flags |= 0x41;
                }
            }
        }
        moby->bA4 = 0xFF;
        {
            struct Hero *g = &hero;
            if (g->ship_moby == (struct Moby *)moby) {
                g->ship_hp_percent = g->ship_hp / fighter_jet_max_hp_gp * 200.0f;
            }
        }
        switch (moby->state) {
        case FIGHTER_JET_STATE_INITIALIZE:
            moby->state = 1;
            ship_vars->b60 = 0;
            moby->scale = *(float *)(moby->pclass + 0x24);
            ship_vars->f20 = ship_vars->rot[0];
            ship_vars->f24 = ship_vars->rot[1];
            fov = fast_arc_tan(1.0f, D_L17_0016D3F0_a[0]) * 2.0f;
            ship_vars->fC0 = 0.0f;
            ship_vars->fAC = fov;
            ship_vars->fB0 = fov;
            qcopy_nc(ship_vars->rot, moby->rot);
            qcopy_nc(ship_vars->home_rot, moby->rot);
            qcopy_nc(ship_vars->home_pos, moby->pos);
            {
                int hp_bar_level_entry_index = ship_vars->i104 + 0x28;
                hp_bar_clut =
                    fighter_jet_hp_bar_clut_lookup_base +
                    (l17_fighter_jet_hp_bar_clut_entries + hp_bar_level_entry_index)->idx * 16;
            }
            if (hp_bar_saved == 0) {
                *(ShipBlock400 *)fighter_jet_saved_hp_bar_clut = *(ShipBlock400 *)hp_bar_clut;
            }
            hp_bar_saved = 1;
            ship_vars->timerF8 = 1;
            ship_vars->iE8 = 1;
            if (moby->bB0 != 0xFF && visited52[moby->bB0 + current_level_index * 16] == 0xFF) {
                moby->state = 9;
            }
            if (ship_vars->i11C > 0) {
                jet_group_state(ship_vars->i11C, 0, 0, 0);
            }
            set_class_mobys_flags_and_collision(0x325, 1, 1, -1);
            ship_vars->i124 = -1;
            break;
        case FIGHTER_JET_STATE_WAIT_FOR_BOARDING: {
            struct Hero *g = &hero;
            board = jet_distance(moby, (ShipCamera *)g->moby) <
                        fighter_jet_boarding_distance_limit_gp &&
                    FUN_00215130(7, 0x53E4) != 0 && (*(int *)D_0013CAE4_ed & 0x10);
            if (board) {
                remove_help_message_for_selector(7);
                moby->state = 2;
                ship_vars->i124 = -1;
            } else if (ship_vars->fade != 0.0f) {
                FUN_00213ed8(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                fighter_jet_transition_fade_value = ship_vars->fade;
            }
            jet_animate(moby, D_L17_00162054 * D_L17_0016209C);
            break;
        }
        case FIGHTER_JET_STATE_BOARDING_FADE:
            FUN_00213ed8(&ship_vars->fade, 1.0f, frame_time * 4.0f);
            fighter_jet_transition_fade_value = ship_vars->fade;
            if (ship_vars->fade == 1.0f) {
                struct Hero *g = &hero;
                g->ship_moby = (struct Moby *)moby;
                g->ship_oclass = moby->oclass;
                g->ship_ammo = l17_fighter_jet_starting_ammo_gp;
                g->unk160E = 1;
                g->ship_ammo_max = l17_fighter_jet_ammo_capacity_gp;
                g->unk15F8 = l17_fighter_jet_ammo_capacity_gp;
                g->ship_hp = fighter_jet_starting_hp_gp;
                ship_vars->iF0 = 0;
                ship_vars->iF4 = 0xFF;
                g->ship_hp_max = fighter_jet_max_hp_gp;
                g->unk1604 = 100.0f;
                g->unk15F9 = 3;
                g->unk15FA = 3;
                g->unk15FB = 3;
                g->ship_flags = 0;
                request_music_track(2, 4);
                if (ship_vars->enter_path != -1) {
                    char *p = D_L17_001600EC + (ship_vars->enter_path << 7);
                    set_race_camera(p + 0x30, p + 0x70, 50, 1);
                    qcopy_nc(moby->pos,
                             ((char(*)[128])D_L17_001600EC)[ship_vars->enter_path] + 0x30);
                    qcopy_nc(moby->rot,
                             ((char(*)[128])D_L17_001600EC)[ship_vars->enter_path] + 0x70);
                }
                force_help_message(6, 0);
                FUN_L17_0021e530(50, 1);
                jet_mount(moby->pos, moby->rot, 1, 0, 0);
                ship_vars->s68 = 0;
                ship_vars->speed = fighter_jet_initial_flight_speed_gp;
                ship_vars->f6C = 0.0f;
                ship_vars->f70 = 0.0f;
                ship_vars->i84 = 0;
                ship_vars->i80 = 0;
                moby->h32 = 0xFF;
                moby->b30 = 0xFF;
                ship_vars->f28 = ship_vars->rot[2] = moby->rot[2];
                ship_vars->fB4 = fighter_jet_camera_base_offset_x_gp * D_L17_0016209C;
                ship_vars->fBC = fighter_jet_camera_base_offset_z_gp * D_L17_0016209C;
                ship_vars->fB8 = 0.0f;
                ((ShipCamera *)g->moby)->unk98 = -1;
                build_spherical_offset(ship_vars->dir,
                                       fighter_jet_initial_flight_speed_gp,
                                       ship_vars->rot[2],
                                       -ship_vars->rot[1]);
                if (ship_vars->i11C > 0) {
                    jet_group_state(ship_vars->i11C, 1, 1, 1);
                }
                set_class_mobys_flags_and_collision(0x325, 0, 0, -1);
                moby->pos[2] += 3.0f;
                moby->scale = *(float *)(moby->pclass + 0x24) * D_L17_0016209C;
                g->unk20A5 = 1;
                if (ship_vars->voice == -1) {
                    ship_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
                }
                jet_camera_mode();
                D_L17_0015F608 = 2;
                update_fighter_jet_flight((FighterJetMoby *)moby, (FighterJetVars *)ship_vars);
                moby->state = 4;
            }
            break;
        case FIGHTER_JET_STATE_PILOTED_FLIGHT:
            force_help_message(6, 0);
            D_L17_0015F608 = 2;
            e = (char *)&voice_pool + ship_vars->voice * 0x70;
            if (*(ShipMoby **)(e + 0x88) != moby || *(unsigned char *)(e + 0x74) == 0) {
                ship_vars->voice = -1;
                ship_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
            }
            update_fighter_jet_flight((FighterJetMoby *)moby, (FighterJetVars *)ship_vars);
            if (ship_vars->iE8 <= 0) {
                ship_vars->timerF8 = D_L17_00162178;
                moby->state = 8;
            }
            if (ship_vars->fade != 0.0f) {
                FUN_00213ed8(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                fighter_jet_transition_fade_value = ship_vars->fade;
            }
            {
                struct Hero *g = &hero;
                if (g->ship_flags & 1) {
                    moby->state = 5;
                }
            }
            break;
        case FIGHTER_JET_STATE_DISMOUNT: {
            char        *p;
            struct Hero *g = &hero;
            g->unk20A5 = 0;
            ((ShipCamera *)g->moby)->unk98 = 0;
            FUN_L17_0021e530(0, 1);
            jet_end_mount(0);
            set_animation_parameter(g->unk160C, 0);
            qcopy_nc(moby->pos, ship_vars->home_pos);
            qcopy_nc(moby->rot, ship_vars->home_rot);
            moby->scale = *(float *)(moby->pclass + 0x24);
            refresh_moby_spatial_bounds_from_basis(moby);
            p = D_L17_001600EC + (ship_vars->exit_path << 7);
            set_race_camera(p + 0x30, p + 0x70, 0, 1);
            request_music_track(0, 5);
            idx = ship_vars->voice;
            if (idx != -1) {
                char *e = (char *)&voice_pool + idx * 0x70;
                if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(idx);
                }
            }
            ship_vars->voice = -1;
            idx = ship_vars->voice2;
            if (idx != -1) {
                char *e = (char *)&voice_pool + idx * 0x70;
                if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(idx);
                }
            }
            ship_vars->voice2 = -1;
            moby->state = 1;
            ship_vars->fade = 0.99f;
            fighter_jet_transition_fade_value = 0.99f;
            break;
        }
        case FIGHTER_JET_STATE_DEATH_RESPAWN_DELAY:
            force_help_message(6, 0);
            D_L17_0015F608 = 2;
            if (fast_dec_timer_s16(&ship_vars->hit_timer)) {
                struct Hero *g = &hero;
                clear_jet_camera_mode_latch();
                ((ShipCamera *)g->moby)->unk98 = 0;
                hero_respawn();
                jet_end_mount(0);
                request_music_track(0, 5);
                qcopy_nc(moby->pos, ship_vars->home_pos);
                qcopy_nc(moby->rot, ship_vars->home_rot);
                refresh_moby_spatial_bounds_from_basis(moby);
                idx = ship_vars->voice;
                if (idx != -1) {
                    char *e = (char *)&voice_pool + idx * 0x70;
                    if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_slot(idx);
                    }
                }
                ship_vars->voice = -1;
                idx = ship_vars->voice2;
                if (idx != -1) {
                    char *e = (char *)&voice_pool + idx * 0x70;
                    if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_slot(idx);
                    }
                }
                ship_vars->voice2 = -1;
                moby->state = 0;
            }
            break;
        case FIGHTER_JET_STATE_VICTORY_EXIT:
            force_help_message(6, 0);
            D_L17_0015F608 = 2;
            update_fighter_jet_flight((FighterJetMoby *)moby, (FighterJetVars *)ship_vars);
            if (fast_dec_timer(&ship_vars->timerF8)) {
                FUN_00213ed8(&ship_vars->fade, 1.0f, frame_time * 4.0f);
                fighter_jet_transition_fade_value = ship_vars->fade;
            }
            if (ship_vars->fade >= 1.0f) {
                char        *p;
                struct Hero *g = &hero;
                g->unk20A5 = 0;
                ((ShipCamera *)g->moby)->unk98 = 0;
                FUN_L17_0021e530(0, 1);
                jet_end_mount(0);
                request_music_track(0, 5);
                if (ship_vars->exit_path2 == -1) {
                    debug_print(fighter_jet_missing_victory_placement_message);
                    qcopy_nc(moby->pos, ship_vars->home_pos);
                    qcopy_nc(moby->rot, ship_vars->home_rot);
                } else {
                    qcopy_nc(moby->pos,
                             ((char(*)[128])D_L17_001600EC)[ship_vars->exit_path2] + 0x30);
                    qcopy_nc(moby->rot,
                             ((char(*)[128])D_L17_001600EC)[ship_vars->exit_path2] + 0x70);
                    moby->pos[2] += 0.57318f;
                }
                moby->scale = *(float *)(moby->pclass + 0x24);
                refresh_moby_spatial_bounds_from_basis(moby);
                if (ship_vars->exit_path == -1) {
                    debug_print(fighter_jet_missing_victory_hero_placement_message);
                } else {
                    if (ship_vars->i11C > 0) {
                        jet_group_state(ship_vars->i11C, 0, 0, 0);
                    }
                    set_class_mobys_flags_and_collision(0x325, 1, 1, -1);
                    p = D_L17_001600EC + (ship_vars->exit_path << 7);
                    set_race_camera(p + 0x30, p + 0x70, 0, 1);
                }
                if (moby->bB0 != 0xFF) {
                    jet_clear_persistent(moby->bB0);
                }
                idx = ship_vars->voice;
                if (idx != -1) {
                    char *e = (char *)&voice_pool + idx * 0x70;
                    if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_slot(idx);
                    }
                }
                ship_vars->voice = -1;
                idx = ship_vars->voice2;
                if (idx != -1) {
                    char *e = (char *)&voice_pool + idx * 0x70;
                    if (*(ShipMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                        release_voice_slot(idx);
                    }
                }
                ship_vars->voice2 = -1;
                jet_reset_camera((&hero.motion.pos), &hero.motion.rot);
                moby->state = 9;
            }
            break;
        case 10:
            break;
        case FIGHTER_JET_STATE_FADE_THEN_REMOVE:
            if (ship_vars->fade == 0.0f) {
                mark_moby_for_removal((struct Obj *)moby);
                return;
            }
            FUN_00213ed8(&ship_vars->fade, 0.0f, frame_time * 4.0f);
            fighter_jet_transition_fade_value = ship_vars->fade;
            break;
        }
        if (moby->pos[0] < 15.0f) {
            moby->pos[0] = 15.0f;
        }
        if (moby->pos[0] > 1008.0f) {
            moby->pos[0] = 1008.0f;
        }
        if (moby->pos[1] < 15.0f) {
            moby->pos[1] = 15.0f;
        }
        if (moby->pos[1] > 1008.0f) {
            moby->pos[1] = 1008.0f;
        }
        if (moby->pos[2] < 15.0f) {
            moby->pos[2] = 15.0f;
        }
        if (moby->pos[2] > 1008.0f) {
            moby->pos[2] = 1008.0f;
        }
        if (moby->state == 4) {
            qcopy_nc((&hero.motion.pos), moby->pos);
            qcopy_nc(&hero.motion.rot, ship_vars->rot);
        }
        if (moby->state == 3 || moby->state == 4) {
            emit_type_74_particles_from_moby_joints(moby, ship_vars);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EFA20), where it is exact; names translated to the US level program. */

extern char (*D_L17_001600EC_t)[128] __asm__("D_L17_001600EC") MACRO_ADDR;

void adjust_fighter_jet_steering_for_flight_bounds(char  *moby,
                                                   float *horizontal_steering_input,

                                                   float *vertical_steering_input) {
    char *ship_vars = ((char *)MOBY(moby)->pvars);
    float flight_bounds_prediction_scratch[4];
    float zz, predicted_horizontal_distance_from_boundary_center, horizontal_boundary_radius, hi,
        lo, inward_heading, upper_height_limit, k, horizontal_boundary_blend_factor, heading_error;
    fast_vec_scale(flight_bounds_prediction_scratch,
                   ship_vars + 0x10,
                   *(float *)(ship_vars + 0x64) * (float)scale_game_frames(0x3C));
    fast_vec_add(flight_bounds_prediction_scratch, flight_bounds_prediction_scratch, moby + 0x10);
    zz = flight_bounds_prediction_scratch[2];
    fast_vec_sub(flight_bounds_prediction_scratch,
                 flight_bounds_prediction_scratch,
                 (*(int *)(ship_vars + 0x12C) << 7) + D_L17_001600EC + 0x30);
    predicted_horizontal_distance_from_boundary_center =
        vector_length_xy(flight_bounds_prediction_scratch);
    horizontal_boundary_radius =
        vector_length_xy((*(int *)(ship_vars + 0x12C) << 7) + D_L17_001600EC);
    hi = *(float *)(D_L17_001600EC_t[*(int *)(ship_vars + 0x12C)] + 0x28);
    lo = *(float *)(D_L17_001600EC_t[*(int *)(ship_vars + 0x12C)] + 0x38);
    upper_height_limit = hi + lo;
    k = -hi + lo;
    inward_heading = fast_add_rotations(
        fast_arc_tan(flight_bounds_prediction_scratch[0], flight_bounds_prediction_scratch[1]),
        3.141590118408203f);
    if (horizontal_boundary_radius - 10.0f < predicted_horizontal_distance_from_boundary_center) {
        horizontal_boundary_blend_factor = (predicted_horizontal_distance_from_boundary_center -
                                            horizontal_boundary_radius + 10.0f) /
                                           10.0f;
        heading_error = fast_subtract_rotations(inward_heading, *(float *)(ship_vars + 0x38));
        if (horizontal_boundary_blend_factor > 1.0f)
            horizontal_boundary_blend_factor = 1.0f;
        else if (horizontal_boundary_blend_factor < 0.0f)
            horizontal_boundary_blend_factor = 0.0f;
        if (heading_error > 0.0f) {
            *horizontal_steering_input =
                (-1.0f - *horizontal_steering_input) * horizontal_boundary_blend_factor +
                *horizontal_steering_input;
        } else {
            *horizontal_steering_input =
                (1.0f - *horizontal_steering_input) * horizontal_boundary_blend_factor +
                *horizontal_steering_input;
        }
        hero.ship_flags |= 2;
    }
    if (upper_height_limit - 10.0f < zz) {
        float q, w;
        q = (zz - upper_height_limit + 10.0f) / 10.0f;
        w = (0.5235987901687622f - *(float *)(ship_vars + 0x34)) / 0.5235987901687622f;
        if (q > 1.0f)
            q = 1.0f;
        else if (q < 0.0f)
            q = 0.0f;
        if (w > 1.0f)
            w = 1.0f;
        else if (w < 0.0f)
            w = 0.0f;
        q = q * w;
        *vertical_steering_input =
            (-1.0f - *vertical_steering_input) * q + *vertical_steering_input;
        hero.ship_flags |= 2;
    } else if (zz < k + 10.0f) {
        float q, w;
        q = (k - zz + 10.0f) / 10.0f;
        w = (*(float *)(ship_vars + 0x34) + 0.5235987901687622f) / 0.5235987901687622f;
        if (q > 1.0f)
            q = 1.0f;
        else if (q < 0.0f)
            q = 0.0f;
        if (w > 1.0f)
            w = 1.0f;
        else if (w < 0.0f)
            w = 0.0f;
        q = q * w;
        *vertical_steering_input = (1.0f - *vertical_steering_input) * q + *vertical_steering_input;
        hero.ship_flags |= 2;
    }
}

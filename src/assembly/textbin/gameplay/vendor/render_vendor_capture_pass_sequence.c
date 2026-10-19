#include "types.h"
#include "asm.h"
#include "rnc/ui/vendor/vendor_capture.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/vendor/render_vendor_capture_pass_sequence/FUN_00239780.s", FUN_00239780);
#else
/* Near miss, 4 rows: sched1 issues the a0 argument copy before the a2 one at the first call
   (an LUID tie). The loop locals sit at function scope with the point_indices pointer set
   before the loop, which gives retail's spill-slot order. */
/* Ported from rac1-decomp (src/game/vendor.c: func_0023AA38). */
#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"

typedef u128 CaptureVector;
extern int capture_primary_transition __asm__("D_00161298") MACRO_ADDR;
extern int capture_secondary_transition_active __asm__("D_001612A0");
extern int capture_secondary_transition_step __asm__("D_0016129C");
extern int capture_primary_transition_step __asm__("D_00161294");
extern int pal_mode_macro __asm__("D_0015ED80") MACRO_ADDR;
extern long capture_texture_tex0_macro __asm__("D_0015EED0") MACRO_ADDR;
extern void transform_scaled_vertex_batch(char *, int, int *, char *) __asm__("func_0020CD48");
extern void fast_vec_sub(void *, void *, void *) __asm__("func_001F9A28");
extern void fast_vec_add(void *, void *, void *) __asm__("func_001F9A10");
extern float fast_vec_length(void *) __asm__("func_001F9AF0");
extern void fast_vec_normalize(void *, void *, float) __asm__("func_001F9BF8");
extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
extern int truncate_float_to_s32(float) __asm__("func_001FA6D0");
extern void project_graphics_bounds_float(void *, void *, float *, float *, float *,
                                          float *) __asm__("func_00237C80");
extern void configure_capture_target(int, int, float) __asm__("func_00239690");
extern void draw_centered_capture_background(int, int) __asm__("func_001FB740");
extern void vu1_add_g_sregister(int, long) __asm__("func_00233980");
extern void update_scrolling_status_message(char *) __asm__("func_00238520");
extern void render_vendor_item_details_pass(char *) __asm__("func_002389E0");
extern void render_capture_effect_pass(char *) __asm__("func_00238630");
extern void render_vendor_buy_label_pass(char *, int, int) __asm__("func_00238F08");
extern void render_vendor_purchase_prompt_pass(char *) __asm__("func_00239160");
extern void render_vendor_item_icon_strip_pass(char *) __asm__("func_002386E8");
extern void render_vendor_capture_texture_overlays_pass(int, float, float) __asm__("func_00239328");
extern void setup_view_context(void) __asm__("func_00239750");
extern void vu1_tex_flush(void) __asm__("func_00233B68");
extern void append_subpixel_textured_screen_quad(float, float, float, float, int, int, int, int,
                                                 long, long) __asm__("func_001F55D8");

void render_vendor_capture_pass_sequence(char *capture_context) __asm__("FUN_00239780");

/* Renders the six vendor monitor screens: projects each screen's joint box, renders its view
   off-screen and draws it back. */
void render_vendor_capture_pass_sequence(char *capture_context) {
    CaptureVector first_edge[1];
    CaptureVector second_edge[1];
    CaptureVector origin[1];
    CaptureVector attachment_points[3];
    int point_indices[3];
    CaptureVector edge_offset[1];
    CaptureVector corners[4];
    /* Fakematch: unused, it puts the locals at retail's stack offsets. */
    CaptureVector unused_stack[5];
    CaptureVector first_corner[1];
    CaptureVector opposite_corner[1];
    float outer_width, outer_height, outer_x, outer_y;
    float inner_width, inner_height, inner_x, inner_y;
    float edge_length;
    float transition_factor;
    float remaining_factor;
    int pass_index;
    long texture_tex0;
    int *point_index_pointer = point_indices;

    for (pass_index = 0; pass_index < 6; pass_index++) {
        point_indices[0] = pass_index * 4;
        point_indices[1] = pass_index * 4 + 1;
        point_indices[2] = pass_index * 4 + 2;
        transform_scaled_vertex_batch(capture_context, 3, point_index_pointer,
                                      (char *)attachment_points);
        qcopy(origin, attachment_points);
        fast_vec_sub(first_edge, &attachment_points[1], attachment_points);
        fast_vec_sub(second_edge, &attachment_points[2], attachment_points);
        edge_length = fast_vec_length(first_edge);
        fast_vec_normalize(edge_offset, first_edge,
                           capture_bounds_adjustments[pass_index].first_origin);
        fast_vec_add(origin, origin, edge_offset);
        fast_vec_normalize(first_edge, first_edge,
                           edge_length -
                               2.0f * capture_bounds_adjustments[pass_index].first_extent);
        edge_length = fast_vec_length(second_edge);
        fast_vec_normalize(edge_offset, second_edge,
                           capture_bounds_adjustments[pass_index].second_origin);
        fast_vec_add(origin, origin, edge_offset);
        fast_vec_normalize(second_edge, second_edge,
                           edge_length -
                               2.0f * capture_bounds_adjustments[pass_index].second_extent);
        fast_vec_add(edge_offset, origin, first_edge);
        fast_vec_add(edge_offset, edge_offset, second_edge);
        project_graphics_bounds_float(origin, edge_offset, &outer_width, &outer_height, &outer_x,
                                      &outer_y);
        if (capture_primary_transition != 0 || capture_secondary_transition_active != 0) {
            transition_factor = 1.0f;
            if (capture_secondary_transition_active != 0) {
                transition_factor =
                    convert_integer_to_float(8 - capture_secondary_transition_step) * 0.125f;
            }
            if (capture_primary_transition != 0) {
                transition_factor =
                    convert_integer_to_float(capture_primary_transition_step) * 0.125f;
            }
            edge_length = fast_vec_length(first_edge);
            remaining_factor = 1.0f - transition_factor;
            fast_vec_normalize(edge_offset, first_edge, edge_length * remaining_factor * 0.5f);
            fast_vec_normalize(first_edge, first_edge, edge_length * transition_factor);
            fast_vec_add(origin, origin, edge_offset);
            edge_length = fast_vec_length(second_edge);
            fast_vec_normalize(edge_offset, second_edge, edge_length * remaining_factor * 0.5f);
            fast_vec_normalize(second_edge, second_edge, edge_length * transition_factor);
            fast_vec_add(origin, origin, edge_offset);
        }
        qcopy(&corners[0], origin);
        fast_vec_add(&corners[1], origin, first_edge);
        fast_vec_add(&corners[2], origin, second_edge);
        fast_vec_add(&corners[3], &corners[2], first_edge);
        first_corner[0] = corners[0];
        opposite_corner[0] = corners[3];
        project_graphics_bounds_float(first_corner, opposite_corner, &inner_width, &inner_height,
                                      &inner_x, &inner_y);
        configure_capture_target(9, 7, 1.0f);
        draw_centered_capture_background(0x200, 0x200);
        vu1_add_g_sregister(0x42, 0x8000000064L);
        switch (pass_index) {
        case 0:
            update_scrolling_status_message(capture_context);
            break;
        case 1:
            render_vendor_item_details_pass(capture_context);
            break;
        case 2:
            render_capture_effect_pass(capture_context);
            break;
        case 3:
            render_vendor_buy_label_pass(capture_context, (int)inner_width, (int)inner_height);
            break;
        case 4:
            render_vendor_purchase_prompt_pass(capture_context);
            break;
        case 5:
            render_vendor_item_icon_strip_pass(capture_context);
            break;
        }
        render_vendor_capture_texture_overlays_pass(pass_index, inner_width, inner_height);
        setup_view_context();
        draw_centered_capture_background(0x200, 0x200);
        vu1_add_g_sregister(0x42, 0x8000000064L);
        vu1_add_g_sregister(8, 5);
        vu1_tex_flush();
        texture_tex0 = capture_texture_tex0_macro;
        if (pal_mode_macro != 0) {
            append_subpixel_textured_screen_quad(
                inner_x, inner_y, inner_width, inner_height, 0, 0, (int)(outer_width - 1.0f),
                truncate_float_to_s32(outer_height * 0.92857146f - 1.0f), 0x80808080, texture_tex0);
        } else {
            append_subpixel_textured_screen_quad(
                inner_x, inner_y, inner_width, inner_height, 0, 0, (int)(outer_width - 1.0f),
                (int)(outer_height - 1.0f), 0x80808080, texture_tex0);
        }
        vu1_add_g_sregister(8, 0);
    }
}
#endif /* NON_MATCHING */

struct CaptureBoundsAdjustment capture_bounds_adjustments[6] = {
    {0.01f, 0.01f, 0.009f, 0.01f}, {0.01f, 0.03f, 0.03f, 0.03f}, {0.03f, 0.0f, 0.0f, 0.0f},
    {0.02f, 0.06f, 0.04f, 0.035f}, {0.07f, 0.05f, 0.03f, 0.03f}, {0.03f, 0.06f, 0.02f, 0.03f}};

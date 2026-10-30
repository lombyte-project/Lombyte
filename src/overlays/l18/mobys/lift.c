/* Level 18 class 0x57a: a lift and its quad meshes. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"

/* Runs the lift's state machine, then queues the draw callback for its current look. */
typedef struct {
    char           p0[0x34];
    unsigned short h34;
} M_2310;

extern float D_L18_00162308_gp __asm__("D_L18_00162308") __attribute__((sda));
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
void synchronize_lift_group_height(void *source_moby) __asm__("FUN_L18_002f2598");

extern void synchronize_lift_group_height(void *a) __asm__("FUN_L18_002f2598");
void        draw_lift_25_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2620");

extern void draw_lift_25_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2620");
void        draw_lift_38_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f27c8");

extern void draw_lift_38_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f27c8");
void        draw_lift_22_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2970");

extern void draw_lift_22_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2970");

#include "rnc/overlay/l18/mobys/lift_globals.h"

extern int   lift_alternate_height_requested_gp __asm__("D_L18_001622FC") __attribute__((sda));
extern int   next_lift_instance_index_gp __asm__("D_L18_001622F8") __attribute__((sda));
extern float lift_texture_u_scroll_rate_gp __asm__("D_L18_0016230C") __attribute__((sda));
extern unsigned char D_00141400 NOT_SDA;
extern unsigned char D_0013D406 NOT_SDA;
void update_lift_state_and_queue_draw(struct Moby *lift_moby) __asm__("FUN_L18_002f2310");

void update_lift_state_and_queue_draw(struct Moby *lift_moby) {
    enum {
        LIFT_INITIALIZE = 0,
        LIFT_SELECT_INITIAL_HEIGHT = 1,
        LIFT_APPROACH_BASE_HEIGHT = 2,
        LIFT_APPROACH_ALTERNATE_HEIGHT = 3
    };
    enum {
        LIFT_DRAW_22_QUAD_MESH = 0,
        LIFT_DRAW_25_QUAD_MESH = 1,
        LIFT_DRAW_38_QUAD_MESH = 2
    };
    char *lift_vars = (char *)lift_moby->pvars;
    int   initial_height_selector;
    switch (lift_moby->state) {
    case LIFT_INITIALIZE:
        if (lift_alternate_height_requested_gp != 0) {
            D_00141400 = *(unsigned char *)&lift_alternate_height_requested_gp;
        }
        *(int *)(lift_vars + 0x1C) = next_lift_instance_index_gp++;
        lift_moby->update_dist = 0xFF;
        lift_moby->state = 1;
        ((M_2310 *)lift_moby)->h34 |= 1;
        lift_moby->draw_dist = 0;
        lift_moby->collision = 0;
        break;
    case LIFT_SELECT_INITIAL_HEIGHT:
        if (D_0013D406 != 0) {
            *(int *)(lift_vars + 8) = (*(int *)(lift_vars + 8) + 1) & 1;
        }
        initial_height_selector = *(int *)(lift_vars + 8);
        next_lift_instance_index_gp = 0;
        if (initial_height_selector != 0 && D_00141400 == 0) {
            lift_moby->state = 2;
            lift_moby->pos.z = *(float *)(lift_vars + 0);
        } else {
            lift_moby->state = 3;
            lift_moby->pos.z = *(float *)(lift_vars + 4);
        }
        break;
    case LIFT_APPROACH_ALTERNATE_HEIGHT:
        if (lift_moby->command & 4) {
            lift_alternate_height_requested_gp = 0;
            lift_moby->state = 2;
            if (*(int *)(lift_vars + 8) == 0) {
                D_0013D406 = 1;
            }
        }
        advance_accelerated_scalar(&lift_moby->pos.z,
                                   (float *)(lift_vars + 0x18),
                                   *(float *)(lift_vars + 4),
                                   frame_time_sq * 4.0f,
                                   frame_time_sq * 4.0f,
                                   frame_time * 4.0f);
        synchronize_lift_group_height(lift_moby);
        lift_moby->command = 1;
        break;
    case LIFT_APPROACH_BASE_HEIGHT:
        if (lift_moby->command & 8) {
            lift_moby->state = 3;
            lift_alternate_height_requested_gp = 1;
        }
        lift_moby->command = 2;
        synchronize_lift_group_height(lift_moby);
        advance_accelerated_scalar(&lift_moby->pos.z,
                                   (float *)(lift_vars + 0x18),
                                   *(float *)(lift_vars + 0),
                                   frame_time_sq * 4.0f,
                                   frame_time_sq * 4.0f,
                                   frame_time * 4.0f);
        break;
    }
    switch (*(int *)(lift_vars + 0x14)) {
    case LIFT_DRAW_25_QUAD_MESH:
        D_L18_00162308_gp = D_L18_00162308_gp + lift_texture_u_scroll_rate_gp * frame_time;
        if (1.0f < D_L18_00162308_gp) {
            D_L18_00162308_gp -= 1.0f;
        }
        add_draw_callback(draw_lift_25_quad_mesh, lift_moby);
        break;
    case LIFT_DRAW_38_QUAD_MESH:
        add_draw_callback(draw_lift_38_quad_mesh, lift_moby);
        break;
    case LIFT_DRAW_22_QUAD_MESH:
        add_draw_callback(draw_lift_22_quad_mesh, lift_moby);
        break;
    }
}

extern unsigned char *D_L18_0015FFD8;
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F39F8), where it is exact; names translated to the US level program. */

typedef struct {
    int pad[7];
    int idx;
} Lx;

typedef struct {
    char  pad[0x18];
    float f;
    char  pad1[0x78 - 0x1C];
    Lx   *l;
    char  pad2[0xA6 - 0x7C];
    short kind;
    char  pad3[0x100 - 0xA8];
} Mx;

typedef struct {
    int   pad[2];
    float f;
    char  pad2[0x1190 - 12];
} E;

extern E      l18_lift_height_records[] __asm__("D_L18_001DB500");
extern short *D_L18_001AC240_c[] __asm__("D_L18_001AC240");

void synchronize_lift_group_height(void *source_moby) {
    unsigned char *source_moby_bytes = source_moby;
    short         *group_entry = D_L18_001AC240_c[source_moby_bytes[0x21]];
    E             *height_records = l18_lift_height_records;
    do {
        Mx *group_moby =
            (Mx *)(((*(unsigned short *)group_entry & 0x7FFF) << 8) + (int)D_L18_0015FFD8);
        group_moby->f = *(float *)(source_moby_bytes + 0x18);
        if (group_moby->kind == 0x57A) {
            height_records[group_moby->l->idx].f = *(float *)(source_moby_bytes + 0x18);
        }
    } while (*group_entry++ >= 0);
}

/* Draws 25 textured quads from the L18 tables, in the moby's frame. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3A80), where it is exact; names translated to the US level program. */

typedef float FVec4[4] __attribute__((aligned(16)));

typedef struct {
    FVec4 v;
} FRow;

extern short lift_mesh_blend_minuend_selector_gp __asm__("D_L18_00162310") __attribute__((sda));
extern short lift_mesh_blend_subtrahend_selector_gp __asm__("D_L18_00162314") __attribute__((sda));
extern short lift_mesh_blend_factor_selector_gp __asm__("D_L18_00162318") __attribute__((sda));
extern short lift_mesh_blend_addend_selector_gp __asm__("D_L18_0016231C") __attribute__((sda));
extern short lift_mesh_blend_upper_word_gp __asm__("D_L18_00162320") __attribute__((sda));
extern short lift_mesh_effect_texture_index_gp __asm__("D_L18_00162324") __attribute__((sda));
extern short lift_mesh_quad_color_gp __asm__("D_L18_00162328") __attribute__((sda));
extern f32   D_L18_00162308_gp __asm__("D_L18_00162308") __attribute__((sda));
extern float D_L18_001DA7A0[][2] NOT_SDA;
extern FRow  D_L18_001DA3C0[] NOT_SDA;
extern short D_L18_001DA610[][4][2] NOT_SDA;
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  fast_draw_quad_real(void *, void *, int) __asm__("FUN_001f7d30");

void draw_lift_25_quad_mesh(struct Moby *moby) {
    FRow          quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *data = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)data;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 25; i++) {
        float ofs = *(float *)&D_L18_00162308_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = D_L18_001DA610[i][j][0];
            int uv_index = D_L18_001DA610[i][j][1];
            qcopy(&quad_positions[j], &D_L18_001DA3C0[vertex_index]);
            uv[j][0] = D_L18_001DA7A0[uv_index][0] + ofs;
            uv[j][1] = D_L18_001DA7A0[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}

typedef float FVec4_l18[4] __attribute__((aligned(16)));

typedef struct {
    FVec4_l18 v;
} FRow_l18;

extern float    D_L18_001DAE80[][2] NOT_SDA;
extern FRow_l18 D_L18_001DA8D0[] NOT_SDA;
extern short    D_L18_001DAC20[][4][2] NOT_SDA;
extern int      get_effect_texture(int) __asm__("FUN_001f44b8");

void draw_lift_38_quad_mesh(struct Moby *moby) {
    FRow_l18      quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *data = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)data;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 38; i++) {
        float ofs = *(float *)&D_L18_00162308_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = D_L18_001DAC20[i][j][0];
            int uv_index = D_L18_001DAC20[i][j][1];

            qcopy(&quad_positions[j], &D_L18_001DA8D0[vertex_index]);
            uv[j][0] = D_L18_001DAE80[uv_index][0] + ofs;
            uv[j][1] = D_L18_001DAE80[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}

/* Draws 22 textured quads from the L18 tables, in the moby's frame. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3DD0), where it is exact; names translated to the US level program. */

extern float    D_L18_001DB3A0[][2] NOT_SDA;
extern FRow_l18 D_L18_001DB030[] NOT_SDA;
extern short    D_L18_001DB240[][4][2] NOT_SDA;

void draw_lift_22_quad_mesh(struct Moby *moby) {
    FRow_l18      quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *data = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)data;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 22; i++) {
        float ofs = *(float *)&D_L18_00162308_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = D_L18_001DB240[i][j][0];
            int uv_index = D_L18_001DB240[i][j][1];
            qcopy(&quad_positions[j], &D_L18_001DB030[vertex_index]);
            uv[j][0] = D_L18_001DB3A0[uv_index][0] + ofs;
            uv[j][1] = D_L18_001DB3A0[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}

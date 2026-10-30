/* Beam slots: up to three beams with endpoint sparks, lights and sounds that firing mobys register, and the controller class (0x533, Oltanis and Quartu) that resets, services and frees them each frame. */
#include "types.h"
#include "qcopy.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "sda.h"
#include "rnc/overlay/point_light.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/math/random/random_spherical_offset.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/rendering/debug/debug_menu.h"

extern int  D_L14_00162148[] MACRO_ADDR;
extern int  D_L14_001621D8[] MACRO_ADDR;
extern void release_beam_slot(int) __asm__("FUN_L00_002e20f8");
extern void update_slot_panes_and_effects(int, int) __asm__("FUN_L00_002e2250");
extern void draw_active_beam_slots(char *) __asm__("FUN_L00_002e2af0");

/* Slot update of moby class 1331: state 0 clears the two slot arrays, state 1 services the pending slots and frees a slot whose moby has died. Level 14 relative of func_L00_002E32A0. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_00304E68), where it is exact; names translated to the US level program. */
void update_beam_slots(unsigned char *moby) __asm__("FUN_L14_003039e0");

void update_beam_slots(unsigned char *moby) {
    enum {
        SLOT_UPDATE_STATE_INITIALIZE = 0,
        SLOT_UPDATE_STATE_PROCESS_SLOTS = 1
    };
    int i;
    int j;
    int started_slot_count;
    int slot_state;

    switch (moby[0x20]) {
    case SLOT_UPDATE_STATE_INITIALIZE:
        for (j = 0; j < 3; j++) {
            D_L14_00162148[j] = 0;
            D_L14_001621D8[j] = 0;
        }
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        break;
    case SLOT_UPDATE_STATE_PROCESS_SLOTS:
        started_slot_count = 0;
        for (i = 0; i < 3; i++) {
            slot_state = D_L14_00162148[i];
            if (slot_state == 1) {
                if (((signed char *)D_L14_001621D8[i])[0x20] < 0) {
                    D_L14_00162148[i] = 0;
                    D_L14_001621D8[i] = 0;
                }
            } else if (slot_state == 4) {
                release_beam_slot(i);
            } else if (slot_state != 0) {
                started_slot_count++;
                update_slot_panes_and_effects(D_L14_001621D8[i], i);
                D_L14_00162148[i] = 4;
            }
        }
        if (started_slot_count != 0) {
            add_draw_callback(draw_active_beam_slots, moby);
        }
        break;
    }
}

typedef int s32_2e1f28;

typedef struct {
    f32 basis[3][4];
    f32 origin[4];
} BeamTransform;

typedef struct {
    u8 b[0x10];
} Q002e2190;

typedef struct {
    unsigned char b[0x40];
} B002E3640;

typedef struct {
    unsigned char b[0x10];
} Q002E3640;

typedef float Vec_2E3700[4] __attribute__((aligned(16)));

typedef struct {
    float x, y, z, w;
} Pt;

typedef struct {
    float v[4];
} Q_2E3700;

typedef struct {
    char  pad0[0x10];
    float f10;
    short f14;
    short f16;
    int   f18;
} PartSub;

typedef struct {
    char    pad0[0xA];
    short   fA;
    float   fC;
    char    pad10[0x10];
    PartSub sub;
} Part;

/* The origin row of a BeamTransform, read through its own label. */
typedef struct {
    f32 origin[4];
    u8  pad10[0x30];
} BeamOrigin;

typedef struct {
    Pt points[20];
} BeamStrand;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E4138). */
typedef struct {
    OvlVec4 axis, unk10, position, unk30;
} TrailSlot;

typedef struct {
    f32 v[4][4];
    s32 col[4];
    f32 uv[4][2];
    s64 r70;
    s64 r78;
    u64 r80;
    u64 r88;
} Q_e3388;

extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
f32         normalize_angle(f32 a) __asm__("FUN_L00_0025e310");
int         allocate_colored_effect_slot(
            void *v, float a, float b_0023e738, float c, float d, float e) __asm__("FUN_L00_0023e738");
unsigned char *spawn_mode_controlled_rotating_alpha_fade_particle(
    void *a, void *b, int c, int d) __asm__("FUN_L00_00274948");
extern void  func_001F9BF0_2E3700(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_001FA748_2E3700(float, float) __asm__("FUN_001fa580");
extern float func_001FA790_2E3700(float, float) __asm__("FUN_001fa5c8");
extern float func_001FA888_2E3700(int) __asm__("FUN_001fa6c0");
extern float func_001F9FA8_2E3700(float) __asm__("FUN_001f9de0");
extern float func_001F9F90_2E3700(float) __asm__("FUN_001f9dc8");
extern void  func_L00_001FF4B0_2E3700(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  func_001F9BD8_2E3700(void *, void *, void *) __asm__("FUN_001f9a10");
extern int   func_001F9850_2E3700(int) __asm__("FUN_001f96f8");
extern void draw_beam_strand_with_glow(int, int, void *, int, int, int) __asm__("FUN_L00_002e2c88");
extern void draw_beam_endpoint_quads_with_glow(int, int) __asm__("FUN_L00_002e3388");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern s64   get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void  draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
void         FUN_001f9a28_e3388(void *, void *, void *) __asm__("FUN_001f9a28");
void         FUN_001f9bf8_e3388(void *, void *, f32) __asm__("FUN_001f9bf8");
void         FUN_001f9ad8_e3388(void *, void *, void *) __asm__("FUN_001f9ad8");
void         FUN_001f9a68_e3388(void *, void *, f32) __asm__("FUN_001f9a68");
void         FUN_001f9a10_e3388(void *, void *, void *) __asm__("FUN_001f9a10");

extern char          D_L00_00166DC0[] NOT_SDA;
extern int           beam_slot_state[] __asm__("D_L00_00161BF8") MACRO_ADDR;
extern float         beam_amplitude_phase[] __asm__("D_L00_00161C08") MACRO_ADDR;
extern float         beam_twist_phase[] __asm__("D_L00_00161C18") MACRO_ADDR;
extern float         beam_swell_phase[] __asm__("D_L00_00161C28") MACRO_ADDR;
extern int           beam_endpoint_light[] __asm__("D_L00_00161C38") MACRO_ADDR;
extern int           beam_tip_light[] __asm__("D_L00_00161C48") MACRO_ADDR;
extern int           beam_endpoint_effect_flags[] __asm__("D_L00_00161C58") MACRO_ADDR;
extern int           beam_endpoint_spark_tag_mode[] __asm__("D_L00_00161C68") MACRO_ADDR;
extern float         beam_scroll_macro[] __asm__("D_L00_00161C78") MACRO_ADDR;
extern int           beam_slot_id[] __asm__("D_L00_00161C88") MACRO_ADDR;
extern BeamTransform beam_transforms[] __asm__("D_L00_001E6D20");
extern Q002e2190     D_L00_001E6DE0[];
extern BeamStrand    beam_strands[] __asm__("D_L00_001E6E40");
extern Vec4          zero_vector __asm__("D_L00_0015F580") MACRO_ADDR;
extern BeamOrigin    beam_origins[] __asm__("D_L00_001E6D50");
extern Q_2E3700      D_L00_001E7130_2E3700[] __asm__("D_L00_001E6DE0");
extern Q_2E3700      D_L00_001E7160_2E3700[] __asm__("D_L00_001E6E10");
extern char          D_0013E633_2E3700[] __asm__("D_0013F5E0");
extern float         D_L00_00161BF0_gp __asm__("D_L00_00161BF0") __attribute__((sda));
extern float         beam_scroll[] __asm__("D_L00_00161C78") MACRO_ADDR;
extern TrailSlot     l00_beam_strand_axis_and_origin_slots[] __asm__("D_L00_001E6D30");
extern f32           D_L00_001E6E10_e3388[][4] __asm__("D_L00_001E6E10");
extern f32           beam_endpoint_quad_local_vertices[4][4] __asm__("D_L00_001E6CE0");

s32           D_L14_00162188 = 0;
s32           D_L14_00162198 = 0;
BeamTransform D_L14_001EDD40 = {0};

Q002e2190 D_L14_001EDE00 = {0};

BeamStrand D_L14_001EDE60 = {0};

float D_L14_00162140 = 5.0f;

f32 D_L14_001EDD00[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

s32           D_L15_00161FD8 = 0;
s32           D_L15_00162018 = 0;
s32           D_L15_00162028 = 0;
s32           D_L15_00162068 = 0;
BeamTransform D_L15_001D9570 = {0};

Q002e2190 D_L15_001D9630 = {0};

BeamStrand D_L15_001D9690 = {0};

float D_L15_00161FD0 = 5.0f;

f32 D_L15_001D9530[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

s32_2e1f28
register_beam_slot(s32_2e1f28 id, s32_2e1f28 a, s32_2e1f28 b) __asm__("FUN_L00_002e1f28");

s32_2e1f28 register_beam_slot(s32_2e1f28 id,
                              s32_2e1f28 endpoint_effect_flags,

                              s32_2e1f28 endpoint_spark_tag_mode) {
    s32_2e1f28 i, j;
    for (i = 0; i < 3; i++) {
        if (beam_slot_id[i] == id)
            return 1;
    }
    for (j = 0; j < 3; j++) {
        if (beam_slot_id[j] == 0) {
            beam_slot_id[j] = id;
            beam_amplitude_phase[j] = 0;
            beam_twist_phase[j] = 0;
            beam_swell_phase[j] = 0;
            beam_endpoint_light[j] = -1;
            beam_tip_light[j] = -1;
            beam_endpoint_effect_flags[j] = endpoint_effect_flags;
            beam_endpoint_spark_tag_mode[j] = endpoint_spark_tag_mode;
            beam_scroll_macro[j] = 0;
            beam_slot_state[j] = 1;
            return 1;
        }
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E34F0), where it is exact; names translated to the US level program. */
int release_beam_slot_by_id(int id) __asm__("FUN_L00_002e2040");

int release_beam_slot_by_id(int id) {
    unsigned int i;
    for (i = 0; (int)i < 3; i++) {
        if (beam_slot_id[i] == id) {
            beam_slot_state[i] = 0;
            beam_slot_id[i] = 0;
            if (beam_endpoint_light[i] != -1) {
                release_point_light_slot(beam_endpoint_light[i]);
                beam_endpoint_light[i] = -1;
            }
            if (beam_tip_light[i] != -1) {
                release_point_light_slot(beam_tip_light[i]);
                beam_tip_light[i] = -1;
            }
            return 1;
        }
    }
    return 0;
}

void release_beam_slot(s32 i) __asm__("FUN_L00_002e20f8");

void release_beam_slot(s32 i) {
    s32 first_endpoint_light_index;
    first_endpoint_light_index = beam_endpoint_light[i];
    beam_slot_state[i] = 0;
    beam_slot_id[i] = 0;
    if (first_endpoint_light_index != -1) {
        release_point_light_slot(first_endpoint_light_index);
        beam_endpoint_light[i] = -1;
    }
    if (beam_tip_light[i] != -1) {
        release_point_light_slot(beam_tip_light[i]);
        beam_tip_light[i] = -1;
    }
}

s32 set_beam_origin_transform_and_endpoint(s32 id, void *src, void *v) __asm__("FUN_L00_002e2190");

s32 set_beam_origin_transform_and_endpoint(s32   id,
                                           void *beam_origin_transform,

                                           void *endpoint_position) {
    s32 i;
    for (i = 0; i < 3; i++) {
        if (id == beam_slot_id[i]) {
            if (beam_slot_state[i] == 1)
                beam_slot_state[i] = 2;
            else if (beam_slot_state[i] == 4)
                beam_slot_state[i] = 3;
            fast_mem_copy(&beam_transforms[i], beam_origin_transform, 0x40);
            qcopy(&D_L00_001E6DE0[i], endpoint_position);
            return 1;
        }
    }
    return 0;
}

/* Per-frame update of one beam slot: lays out the two 20-point strips between the slot's ends
   (a sine wave and a noisy one), spawns sparks at both ends and keeps the two looping sounds
   at the ends. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E3700), where it is exact; names translated to the US level program. */
void update_slot_panes_and_effects(int unused, int beam) __asm__("FUN_L00_002e2250");

void update_slot_panes_and_effects(int unused, int beam) {
    Vec_2E3700 span;
    Vec_2E3700 beam_direction;
    Vec_2E3700 side;
    Vec_2E3700 up;
    Vec_2E3700 from;
    Vec_2E3700 span2;
    Vec_2E3700 tip;
    Vec4       particle_velocity;
    Vec_2E3700 first_endpoint_effect_position;
    int        strand = beam * 2;
    int        next_strand = strand + 1;
    float      step;
    float      phase_step;
    float      one;
    Pt        *point;
    int        i;
    int        j;
    int        second_strand_point_index;

    func_001F9BF0_2E3700(span, &D_L00_001E7130_2E3700[beam], &beam_origins[beam]);
    step = fast_vec_length(span) / 20.0f;
    if (beam_slot_state[beam] == 2) {
        beam_slot_state[beam] = 3;
        for (i = 0; i < 20; i++) {
            Pt *first = &beam_strands[strand].points[i];
            Pt *second = &beam_strands[next_strand].points[i];

            first->x = step * (float)i;
            first->y = first->z = 0.0f;
            first->w = 1.0f;
            second->x = step * (float)i;
            second->y = second->z = 0.0f;
            second->w = 1.0f;
        }
    }
    beam_amplitude_phase[beam] = func_001FA748_2E3700(beam_amplitude_phase[beam], 0.17453292f);
    beam_twist_phase[beam] = func_001FA790_2E3700(beam_twist_phase[beam], 0.80285144f);
    beam_swell_phase[beam] = func_001FA748_2E3700(beam_swell_phase[beam], 0.05236f);
    phase_step = 360.0f / (func_001FA888_2E3700(20) * 0.5f) * 0.017453292f;
    for (j = 0; j < 19; j++) {
        float fj = (float)j;
        float amplitude = func_001FA888_2E3700(j) / func_001FA888_2E3700(20) * 0.3f + 0.1f;
        Pt   *point = &beam_strands[strand].points[j];

        amplitude = amplitude * func_001F9FA8_2E3700(
                                    normalize_angle(beam_amplitude_phase[beam] + fj * phase_step));
        point->y = 0.0f;
        point->w = 1.0f;
        point->z = amplitude;
        point->x = step * fj;
    }
    qcopy_nc(from, &beam_origins[beam]);
    one = 1.0f;
    func_001F9BF0_2E3700(span2, &D_L00_001E7130_2E3700[beam], from);
    func_L00_001FF4B0_2E3700(beam_direction, span2, one);
    fast_vec_cross(side, beam_direction, D_0013E633_2E3700);
    func_L00_001FF4B0_2E3700(side, side, -1.0f);
    fast_vec_cross(up, side, beam_direction);
    transform_vector(tip, &beam_strands[strand].points[18], beam_direction);
    qcopy(&D_L00_001E7160_2E3700[beam], tip);
    point = beam_strands[next_strand].points;
    for (second_strand_point_index = 0; second_strand_point_index < 19;
         second_strand_point_index++) {
        float fn = (float)second_strand_point_index;
        float twist;
        float swell;
        float height;
        float ripple;
        float random;

        point->z = 0.0f;
        twist = normalize_angle(beam_twist_phase[beam] + fn * 0.99483764f);
        swell = normalize_angle(beam_swell_phase[beam] + fn * 0.20944f);
        height = func_001F9FA8_2E3700(swell) * 0.5f;
        ripple = func_001F9FA8_2E3700(twist);
        random = randf_sym(0.0f, 0.2f);
        point->z += height * ripple + random;
        point->y = func_001F9F90_2E3700(twist) * 0.3f;
        point->w = 1.0f;
        point->x = step * fn;
        point++;
    }
    {
        float one;
        float speed = random_float_between(0.01f, 0.05f);
        float angle;

        one = 1.0f;
        angle = randf_sym(0.34906584f, 0.6981317f);
        particle_velocity.q = 0;
        particle_velocity.f[0] = one;
        particle_velocity.f[3] = one;
        particle_velocity.f[0] = speed * func_001F9F90_2E3700(angle);
        particle_velocity.f[1] = speed * func_001F9FA8_2E3700(angle);
        transform_vector_by_basis(&particle_velocity, &particle_velocity, &beam_transforms[beam]);
        spawn_mode_controlled_rotating_alpha_fade_particle(
            beam_transforms[beam].origin, &particle_velocity, 0x7F, beam_slot_id[beam]);
        if (random_integer_below(3) == 0) {
            Part *spark;

            fast_vec_scale(first_endpoint_effect_position, &beam_transforms[beam], 0.3f);
            func_001F9BD8_2E3700(first_endpoint_effect_position,
                                 first_endpoint_effect_position,
                                 beam_transforms[beam].origin);
            spark = spawn_mode_controlled_rotating_alpha_fade_particle(
                first_endpoint_effect_position, &zero_vector, 0x7F, beam_slot_id[beam]);
            if (spark != 0) {
                PartSub *sub = &spark->sub;

                spark->fC = 120000.0f;
                spark->fA = func_001F9850_2E3700(4);
                sub->f10 = one / func_001FA888_2E3700(spark->fA);
                sub->f18 = 0x7F7F7F;
                if (beam_endpoint_spark_tag_mode[beam] == 1)
                    sub->f16 = 1;
            }
        }
        random_float_between(0.01f, 0.05f);
        random_spherical_offset(&particle_velocity, 0.01f, 0.05f);
        spawn_mode_controlled_rotating_alpha_fade_particle(
            tip, &particle_velocity, 0x7F, beam_slot_id[beam]);
        if (random_integer_below(3) == 0) {
            Part *spark = spawn_mode_controlled_rotating_alpha_fade_particle(
                tip, &zero_vector, 0x7F, beam_slot_id[beam]);

            if (spark != 0) {
                PartSub *sub = &spark->sub;

                spark->fC = 120000.0f;
                spark->fA = func_001F9850_2E3700(4);
                sub->f10 = 1.0f / func_001FA888_2E3700(spark->fA);
                sub->f18 = 0x7F7F7F;
                if (beam_endpoint_spark_tag_mode[beam] == 1)
                    sub->f16 = 2;
            }
        }
    }
    if (beam_endpoint_effect_flags[beam] != 0) {
        if (beam_endpoint_effect_flags[beam] & 1) {
            qcopy(first_endpoint_effect_position, &beam_origins[beam]);
            first_endpoint_effect_position[2] -= 0.25f;
            if (beam_endpoint_light[beam] == -1) {
                beam_endpoint_light[beam] = allocate_colored_effect_slot(
                    first_endpoint_effect_position, D_L00_00161BF0_gp, 0.0f, 1.0f, 1.0f, 2.0f);
            }
            if (beam_endpoint_light[beam] >= 0) {
                PointLight *light = &point_lights[beam_endpoint_light[beam]];

                qcopy(&light->position, first_endpoint_effect_position);
                light->position.f[3] = D_L00_00161BF0_gp;
            }
        }
        if (beam_endpoint_effect_flags[beam] & 2) {
            tip[2] += 0.5f;
            if (beam_tip_light[beam] == -1) {
                beam_tip_light[beam] =
                    allocate_colored_effect_slot(tip, D_L00_00161BF0_gp, 0.0f, 1.0f, 1.0f, 2.0f);
            }
            if (beam_tip_light[beam] >= 0) {
                PointLight *light = &point_lights[beam_tip_light[beam]];

                qcopy(&light->position, tip);
                light->position.f[3] = D_L00_00161BF0_gp;
            }
        }
    }
}

/* Scrolls and draws the two panes of each active slot (3 slots). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E3FA0), where it is exact; names translated to the US level program. */
void draw_active_beam_slots(char *moby) __asm__("FUN_L00_002e2af0");

void draw_active_beam_slots(char *moby) {
    int *moby_data = ((int *)MOBY(moby)->pvars);
    int  i;
    vu1_add_g_sregister(0x47, 0x51001);
    for (i = 0; i < 3; i++) {
        float scroll;
        int   texture_id, glow_texture_id;
        int   pane, next_pane;
        if (beam_slot_id[i] == 0 || beam_slot_state[i] < 2)
            continue;
        pane = i << 1;
        next_pane = pane + 1;
        scroll = beam_scroll[i] - frame_scale * 0.2f;
        beam_scroll[i] = scroll;
        if (scroll <= -8.0f)
            beam_scroll[i] = scroll + 8.0f;
        draw_beam_endpoint_quads_with_glow(beam_slot_id[i], i);
        texture_id = moby_data[1] + 0x28;
        glow_texture_id = moby_data[0] + 0x28;
        draw_beam_strand_with_glow(texture_id, glow_texture_id, &beam_strands[pane], 0x80, 0x40, i);
        draw_beam_strand_with_glow(
            texture_id, glow_texture_id, &beam_strands[next_pane], 0x30, 0x10, i);
    }
}

/* Builds the trail quad and its glow quad from the slot tables and draws them along the
   19 steps of points. */
void draw_beam_strand_with_glow(

    int texture_id, int glow_texture_id, void *points, int alpha, int glow_alpha, int slot) {
    struct GeometryQuad quads[2];
    OvlVec4             width, glow_width;
    OvlVec4             basis[4];
    OvlVec4             transform[4];
    OvlVec4             direction, from, to;
    float               zero, half_width, uv_offset;
    int                 color, glow_color;
    int                 i;

    quads[0].texture = get_effect_texture(texture_id);
    color = (alpha << 24) | 0x7F7F7F;
    quads[0].primitive = 0x8000000048L;
    quads[0].texture_state = 0xFF9000000260L;
    quads[0].reserved.value = 0;
    quads[0].colors[0] = quads[0].colors[1] = quads[0].colors[2] = quads[0].colors[3] = color;
    quads[1].texture = get_effect_texture(glow_texture_id);
    *(int *)&quads[1].texture_coordinates[0][1] = 0;
    glow_color = (glow_alpha << 24) | 0x7F4040;
    zero = quads[1].texture_coordinates[0][1];
    uv_offset = beam_scroll_macro[slot];
    quads[1].colors[0] = quads[1].colors[1] = quads[1].colors[2] = quads[1].colors[3] = glow_color;
    quads[1].primitive = quads[0].primitive;
    quads[1].texture_state = quads[0].texture_state;
    quads[1].reserved.value = quads[0].reserved.value;
    quads[1].texture_coordinates[0][0] = 1.0f;
    quads[1].texture_coordinates[1][0] = 1.0f;
    quads[1].texture_coordinates[1][1] = 1.0f;
    quads[1].texture_coordinates[2][0] = 0.0f;
    quads[1].texture_coordinates[2][1] = 0.0f;
    quads[1].texture_coordinates[3][0] = 0.0f;
    quads[1].texture_coordinates[3][1] = 1.0f;
    quads[0].texture_coordinates[1][0] = quads[0].texture_coordinates[0][0] = uv_offset + zero;
    quads[0].texture_coordinates[0][1] = zero;
    quads[0].texture_coordinates[1][1] = 1.0f;
    quads[0].texture_coordinates[3][0] = quads[0].texture_coordinates[2][0] = uv_offset + 1.0f;
    quads[0].texture_coordinates[2][1] = zero;
    quads[0].texture_coordinates[3][1] = 1.0f;
    half_width = 0.6f;
    if (*(short *)(beam_slot_id[slot] + 0xA6) == 0x1C)
        half_width = 0.3f;
    width.q = 0;
    glow_width.q = 0;
    glow_width.f[2] = half_width + half_width;
    width.f[3] = 1.0f;
    width.f[2] = half_width;
    glow_width.f[3] = 1.0f;

    qcopy(&basis[0], &l00_beam_strand_axis_and_origin_slots[slot].axis);
    fast_vec_sub(&basis[1], D_L00_00166DC0, &l00_beam_strand_axis_and_origin_slots[slot].position);
    fast_vec_scale(&basis[1], &basis[1], 1.0f);
    fast_vec_cross(&basis[2], &basis[1], &basis[0]);
    normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
    fast_vec_cross(&basis[0], &basis[2], &basis[1]);
    qcopy(&basis[3], &l00_beam_strand_axis_and_origin_slots[slot].position);
    qcopy(&transform[3], &l00_beam_strand_axis_and_origin_slots[slot].position);
    fast_vec_sub(&direction, &D_L00_001E6DE0[slot], &transform[3]);
    normalize_vector_xyz(&transform[0], &direction, 1.0f);
    fast_vec_cross(&transform[1], &transform[0], D_0013E633_2E3700);
    normalize_vector_xyz(&transform[1], &transform[1], -1.0f);
    fast_vec_cross(&transform[2], &transform[1], &transform[0]);

    qcopy(&quads[0].positions[0], &width);
    fast_vec_scale(&quads[0].positions[1], &quads[0].positions[0], -1.0f);
    {
        /* Fakematch: plain C copies &glow_width here and in the loop and stores glow_width.f[2];
           the pointer copies, the empty loop (it stops regmove) and the dead reset in the loop
           make 2.95.2 copy the pointer to $v1 before this qcopy, as retail does. */
        OvlVec4 *glow_src = &glow_width;
        OvlVec4 *glow;
        {
            OvlVec4 *glow_dst = &quads[1].positions[0];
            glow = glow_src;
            /* Fakematch: the empty loop stops regmove (see above). */
            do {
            } while (0);
            qcopy(glow_dst, glow);
        }
        fast_vec_scale(&quads[1].positions[1], &quads[1].positions[0], -1.0f);
        transform_vector(&from, &((OvlVec4 *)points)[0], transform);
        transform_vector(&to, &((OvlVec4 *)points)[1], transform);
        fast_vec_sub(&basis[0], &to, &from);
        normalize_vector_xyz(&basis[0], &basis[0], 1.0f);
        fast_vec_sub(&basis[1], D_L00_00166DC0, &from);
        fast_vec_cross(&basis[2], &basis[1], &basis[0]);
        normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
        fast_vec_cross(&basis[1], &basis[0], &basis[2]);
        qcopy(&basis[3], &from);
        transform_vector(&quads[0].positions[0], &quads[0].positions[0], basis);
        transform_vector(&quads[0].positions[1], &quads[0].positions[1], basis);
        transform_vector(&quads[1].positions[0], &quads[1].positions[0], basis);
        transform_vector(&quads[1].positions[1], &quads[1].positions[1], basis);

        i = 1;
        do {
            float taper, scroll;
            qcopy(&quads[0].positions[2], &width);
            fast_vec_scale(&quads[0].positions[3], &quads[0].positions[2], -1.0f);
            qcopy(&quads[1].positions[2], glow);
            fast_vec_scale(&quads[1].positions[3], &quads[1].positions[2], -1.0f);
            transform_vector(&from, &((OvlVec4 *)points)[i], transform);
            transform_vector(&to, &((OvlVec4 *)points)[i + 1], transform);
            fast_vec_sub(&basis[0], &to, &from);
            normalize_vector_xyz(&basis[0], &basis[0], 1.0f);
            fast_vec_sub(&basis[1], D_L00_00166DC0, &from);
            fast_vec_cross(&basis[2], &basis[1], &basis[0]);
            normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
            fast_vec_cross(&basis[1], &basis[0], &basis[2]);
            qcopy(&basis[3], &from);
            transform_vector(&quads[0].positions[2], &quads[0].positions[2], basis);
            transform_vector(&quads[0].positions[3], &quads[0].positions[3], basis);
            transform_vector(&quads[1].positions[2], &quads[1].positions[2], basis);
            transform_vector(&quads[1].positions[3], &quads[1].positions[3], basis);
            if (i == 1) {
                int saved_color = quads[0].colors[0];
                int saved_glow_color = quads[1].colors[0];
                quads[0].colors[0] = quads[0].colors[1] = 0xFFFFFF;
                quads[1].colors[0] = quads[1].colors[1] = 0x7F4040;
                draw_geometry_quad(&quads[0], 0, 0);
                draw_geometry_quad(&quads[1], 0, 0);
                quads[0].colors[0] = quads[0].colors[1] = quads[0].colors[2] = quads[0].colors[3] =
                    saved_color;
                quads[1].colors[0] = quads[1].colors[1] = quads[1].colors[2] = quads[1].colors[3] =
                    saved_glow_color;
            } else {
                draw_geometry_quad(&quads[0], 0, 0);
                draw_geometry_quad(&quads[1], 0, 0);
            }
            qcopy(&quads[0].positions[0], &quads[0].positions[2]);
            qcopy(&quads[0].positions[1], &quads[0].positions[3]);
            qcopy(&quads[1].positions[0], &quads[1].positions[2]);
            qcopy(&quads[1].positions[1], &quads[1].positions[3]);
            taper =
                cubic_hermite_scalar(-1.0f,
                                     zero,
                                     1.0f,
                                     zero,
                                     convert_integer_to_float(i) / convert_integer_to_float(20));
            scroll = beam_scroll[slot];
            quads[0].texture_coordinates[0][0] = scroll + zero;
            quads[0].texture_coordinates[0][1] = zero;
            quads[0].texture_coordinates[1][0] = scroll + zero;
            quads[0].texture_coordinates[1][1] = 1.0f;
            quads[0].texture_coordinates[2][0] = scroll + 1.0f;
            quads[0].texture_coordinates[2][1] = zero;
            quads[0].texture_coordinates[3][0] = scroll + 1.0f;
            quads[0].texture_coordinates[3][1] = 1.0f;
            width.f[2] = half_width + (0.05f - half_width) * taper;
            /* Fakematch: plain C stores glow_width.f[2]; the dead reset keeps glow_src live up to the loop. */
            {
                OvlVec4 *next_glow = &glow_width;
                next_glow->f[2] = width.f[2] + width.f[2];
            }
            glow_src = &glow_width;
            i++;
        } while (i < 19);
    }
}

void draw_beam_endpoint_quads_with_glow(s32 unused, s32 beam_slot) __asm__("FUN_L00_002e3388");

void draw_beam_endpoint_quads_with_glow(s32 unused, s32 beam_slot) {
    Q_e3388 geometry_quad;
    f32     endpoint_to_camera_axis[4] __attribute__((aligned(16)));
    f32     endpoint_quad_side_axis[4] __attribute__((aligned(16)));
    f32     endpoint_quad_up_axis[4] __attribute__((aligned(16)));
    f32     quad_endpoint_position[4] __attribute__((aligned(16)));
    f32     core_quad_jitter_offset[4] __attribute__((aligned(16)));
    f32     quad_scale;
    s32     i;
    s32     j;

    qcopy(quad_endpoint_position, &beam_origins[beam_slot]);
    quad_endpoint_position[3] = 1.0f;
    FUN_001f9a28_e3388(endpoint_to_camera_axis, g_debug_camera.position, quad_endpoint_position);
    FUN_001f9bf8_e3388(endpoint_to_camera_axis, endpoint_to_camera_axis, 1.0f);
    FUN_001f9ad8_e3388(endpoint_quad_side_axis, endpoint_to_camera_axis, &hero.gravity_dir);
    FUN_001f9bf8_e3388(endpoint_quad_side_axis, endpoint_quad_side_axis, -1.0f);
    FUN_001f9ad8_e3388(endpoint_quad_up_axis, endpoint_quad_side_axis, endpoint_to_camera_axis);
    geometry_quad.r78 = get_effect_texture(0xB);
    geometry_quad.r88 = 0xFF00000044;
    geometry_quad.r80 = 0xFF9000000260;
    geometry_quad.r70 = 5;
    geometry_quad.col[3] = -1;
    geometry_quad.col[2] = -1;
    geometry_quad.col[1] = -1;
    geometry_quad.col[0] = -1;
    geometry_quad.uv[0][0] = 0.0f;
    geometry_quad.uv[0][1] = 0.0f;
    geometry_quad.uv[1][0] = 0.0f;
    geometry_quad.uv[1][1] = 1.0f;
    geometry_quad.uv[2][0] = 1.0f;
    geometry_quad.uv[2][1] = 0.0f;
    geometry_quad.uv[3][0] = 1.0f;
    geometry_quad.uv[3][1] = 1.0f;
    quad_scale = random_float_between(0.3f, 0.325f);
    core_quad_jitter_offset[0] = randf_sym(0.0f, 0.05f);
    core_quad_jitter_offset[1] = randf_sym(0.0f, 0.05f);
    core_quad_jitter_offset[2] = randf_sym(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(geometry_quad.v[i], beam_endpoint_quad_local_vertices[i], quad_scale);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], endpoint_to_camera_axis);
        FUN_001f9a10_e3388(geometry_quad.v[i], geometry_quad.v[i], core_quad_jitter_offset);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    geometry_quad.r88 = 0xFF00000048;
    geometry_quad.col[3] = 0x607F4040;
    geometry_quad.col[2] = 0x607F4040;
    geometry_quad.col[1] = 0x607F4040;
    geometry_quad.col[0] = 0x607F4040;
    for (j = 0; j < 4; j++) {
        FUN_001f9a68_e3388(
            geometry_quad.v[j], beam_endpoint_quad_local_vertices[j], quad_scale * 3.0f);
        transform_vector(geometry_quad.v[j], geometry_quad.v[j], endpoint_to_camera_axis);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    qcopy(quad_endpoint_position, D_L00_001E6E10_e3388[beam_slot]);
    FUN_001f9a28_e3388(endpoint_to_camera_axis, g_debug_camera.position, quad_endpoint_position);
    FUN_001f9bf8_e3388(endpoint_to_camera_axis, endpoint_to_camera_axis, 1.0f);
    FUN_001f9ad8_e3388(endpoint_quad_side_axis, endpoint_to_camera_axis, &hero.gravity_dir);
    FUN_001f9bf8_e3388(endpoint_quad_side_axis, endpoint_quad_side_axis, -1.0f);
    FUN_001f9ad8_e3388(endpoint_quad_up_axis, endpoint_quad_side_axis, endpoint_to_camera_axis);
    quad_scale = random_float_between(0.2f, 0.225f);
    geometry_quad.r88 = 0xFF00000044;
    geometry_quad.col[3] = -1;
    geometry_quad.col[2] = -1;
    geometry_quad.col[1] = -1;
    geometry_quad.col[0] = -1;
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(geometry_quad.v[i], beam_endpoint_quad_local_vertices[i], quad_scale);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], endpoint_to_camera_axis);
        FUN_001f9a10_e3388(geometry_quad.v[i], geometry_quad.v[i], core_quad_jitter_offset);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    geometry_quad.r88 = 0xFF00000048;
    geometry_quad.col[3] = 0x607F4040;
    geometry_quad.col[2] = 0x607F4040;
    geometry_quad.col[1] = 0x607F4040;
    geometry_quad.col[0] = 0x607F4040;
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(
            geometry_quad.v[i], beam_endpoint_quad_local_vertices[i], quad_scale * 3.0f);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], endpoint_to_camera_axis);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
}

f32 beam_endpoint_quad_local_vertices[4][4] __asm__("D_L00_001E6CE0") = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

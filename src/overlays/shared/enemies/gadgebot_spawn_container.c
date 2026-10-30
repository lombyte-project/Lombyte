/* Shared moby class gadgebot_spawn_container: the container a gadgebot comes out of, and the function that spawns it. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/level_camera.h"

typedef struct {
    float corner[4][4];
    int   color[4];
    float uv[4][2];
    long  unk70;
    long  tex;
    long  unk80;
    long  unk88;
} L06BackdropQuad;

extern unsigned char *spawn_spinning_fade_in_out_effect_particle(
    void *, int, int, float, float, float, float, float) __asm__("FUN_L00_0026b790");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern unsigned char              *
spawn_spinning_moby_fragment(char *, float *, float *) __asm__("FUN_L06_00300c60");
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int   FUN_L00_001fefc8(void *);
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void  draw_gadgebot_spawn_container(char *) __asm__("FUN_L06_002d8738");
extern void  add_draw_callback(int, int) __asm__("FUN_001f4600");
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern float fast_cos(float);
extern float fast_sin(float);
extern int   scale_game_frames(int);

extern char *D_001413D0 NOT_SDA;
extern float            D_0015ED6C;
extern int              level_frame_count __asm__("D_L06_0015F5CC");
extern char             D_L06_00167540[];
extern float            gadgebot_container_local_vertices[][4] __asm__("D_L06_001D3290");
extern float            gadgebot_container_static_uvs[][2] __asm__("D_L06_001D38C0");
extern float            gadgebot_container_face_normals[][4] __asm__("D_L06_001D3A90");
extern float            gadgebot_container_vertex_normals[][4] __asm__("D_L06_001D3D30");
extern float            gadgebot_container_face_positions[][4] __asm__("D_L06_001D40C0");
extern float            gadgebot_container_reflection_uvs[][2] __asm__("D_L06_001D4360");
extern float            gadgebot_container_transformed_vertices[][4] __asm__("D_L06_001D4530");
extern short            gadgebot_container_blend_minuend_selector_gp __asm__("D_L06_00161870")
    __attribute__((sda));
extern short gadgebot_container_blend_subtrahend_selector_gp __asm__("D_L06_00161874")
    __attribute__((sda));
extern short gadgebot_container_blend_factor_selector_gp __asm__("D_L06_00161878")
    __attribute__((sda));
extern short gadgebot_container_blend_addend_selector_gp __asm__("D_L06_0016187C")
    __attribute__((sda));
extern short gadgebot_container_blend_fixed_alpha_gp __asm__("D_L06_00161880") __attribute__((sda));
extern short gadgebot_container_texture_index_gp __asm__("D_L06_00161884") __attribute__((sda));
extern short gadgebot_container_quad_color_gp __asm__("D_L06_00161888") __attribute__((sda));
extern short gadgebot_container_vertex_normal_scale_gp __asm__("D_L06_0016188C")
    __attribute__((sda));
extern short gadgebot_container_face_vertex_uv_indices[][8] __asm__("D_L06_001D3620");
extern s32   level_frame_count __asm__("D_L06_0015F5CC");
extern f32   D_0015ED6C;

f32 D_L10_001D4060[1] __attribute__((section(".data"))) = {0};

f32 D_L10_001D4064[1] __attribute__((section(".data"))) = {0};

short D_L10_00161850 = 0;
short D_L10_00161854 = 1;
short D_L10_00161858 = 2;
short D_L10_0016185C = 1;
short D_L10_00161860 = 0x20;
short D_L10_00161864 = 0x29;
short D_L10_00161868 = -32640;
short D_L10_0016186C = -13107;

static inline void transform_point(float *dst, float *src, float (*m)[4]) {
    transform_vector(dst, src, m);
}

void destroy_gadgebot_spawn_container(void *moby_ptr, void *target_ptr) __asm__("FUN_L06_002d8178");

void destroy_gadgebot_spawn_container(void *moby_ptr, void *target_ptr) {
    int            i;
    int            j;
    char          *moby = moby_ptr;
    char          *target = target_ptr;
    float          origin[4];
    float          displacement[4];
    float          pos[4];
    float          fragment_velocity[4];
    float          angle;
    float          direction_angle;
    float          speed;
    unsigned char *fragment;

    allocate_voice_for_target_entry(0, 0, (int)moby);
    qcopy(origin, moby + 0x10);
    origin[2] += 0.3f;
    spawn_spinning_fade_in_out_effect_particle(
        origin, 1, 0x40808080, 0.3f, 1.01f, 1.075f, 0.05f, 50000.0f);
    for (i = 4; i >= 0; i--) {
        spawn_spinning_fade_in_out_effect_particle(
            origin, 1, 0x40808080, 0.3f, 1.01f, 1.07f, 0.05f, 50000.0f);
    }
    if (target != 0) {
        qcopy(displacement, target + 0x10);
    } else {
        fast_vec_sub(displacement, moby + 0x10, D_001413D0 + 0x10);
    }
    for (j = 9; j >= 0; j--) {
        qcopy(pos, moby + 0x10);
        pos[0] += randf_sym(0.0f, 0.2f);
        pos[1] += randf_sym(0.0f, 0.2f);
        pos[2] += random_float_between(0.15f, 0.55f);
        angle = random_angle_radians();
        speed = random_float_between(2.0f, 4.5f) * D_0015ED6C;
        fragment_velocity[0] = fast_cos(angle) * speed;
        fragment_velocity[1] = fast_sin(angle) * speed;
        fragment_velocity[2] = 0.0f;
        speed = random_float_between(D_0015ED6C * 0.7f, D_0015ED6C * 1.5f);
        if (target != 0) {
            direction_angle = fast_arc_tan(*(float *)(target + 0x10), *(float *)(target + 0x14));
            fragment_velocity[0] += fast_cos(direction_angle) * speed;
            fragment_velocity[1] +=
                fast_sin(fast_arc_tan(*(float *)(target + 0x10), *(float *)(target + 0x14))) *
                speed;
        } else {
            direction_angle = fast_arc_tan(MOBY(moby)->pos.x - hero.motion.pos.f[0],
                                           MOBY(moby)->pos.y - hero.motion.pos.f[1]);
            fragment_velocity[0] += fast_cos(direction_angle) * speed;
            fragment_velocity[1] += fast_sin(direction_angle) * speed;
        }
        fragment_velocity[2] = random_float_between(2.0f, 7.0f) * D_0015ED6C;
        fragment = spawn_spinning_moby_fragment(moby, pos, fragment_velocity);
        if (fragment != 0) {
            *(float *)(fragment + 0x2c) *= 0.3f;
        }
    }
    MOBY(moby)->collision = 0;
    moby[0x20] = 2;
    MOBY(moby)->flags |= 1;
    moby[0xbc] = (unsigned char)scale_game_frames(15);
}

/* Spawns moby class 0x12E at the owner's position, copying its fields. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002D98E0), where it is exact; names translated to the US level program. */
char *spawn_gadgebot_spawn_container(char *owner) __asm__("FUN_L06_002d8510");

char *spawn_gadgebot_spawn_container(char *owner) {
    char *moby = create_moby(0x12E);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0x60;
        MOBY(moby)->draw_dist = 0x60;
        moby[0x31] = 1;
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        MOBY(moby)->rot.z = *(float *)(owner + 0x48);
        MOBY(moby)->spawn_frame = *(long *)(owner + 0x38);
        MOBY(moby)->flags |= 0x1000;
        qcopy(moby + 0x10, owner + 0x10);
        update_entity(moby);
    }
    return moby;
}

void gadgebot_spawn_container_update(unsigned char *moby) __asm__("FUN_L06_002d85a0");

void gadgebot_spawn_container_update(unsigned char *moby) {
    enum {
        GADGEBOT_CONTAINER_ACTIVATE = 0,
        GADGEBOT_CONTAINER_PULSE_AND_HANDLE_HITS = 1,
        GADGEBOT_CONTAINER_WAIT_FOR_REMOVAL = 2
    };
    char *hit_record = find_moby_hit(moby, 0x10000, 0);
    float raised_position_snapshot[4];
    int   frame_count_snapshot;
    float color_pulse_value;

    moby[0xA4] = 0xFF;
    switch (moby[0x20]) {
    case GADGEBOT_CONTAINER_ACTIVATE:
        moby[0x20] = 1;
        break;
    case GADGEBOT_CONTAINER_PULSE_AND_HANDLE_HITS:
        frame_count_snapshot = level_frame_count;
        color_pulse_value = (float)(frame_count_snapshot % 150) / 150.0f * 2.0f * 3.14159274f;
        if (color_pulse_value > 3.14159274f) {
            color_pulse_value -= 6.28318548f;
        }
        color_pulse_value = (fast_sin(color_pulse_value) + 1.0f) * 0.5f;
        MOBY(moby)->glow_color = fast_tween_color(0x80464646, 0x80828282, color_pulse_value);
        if (hit_record != 0) {
            destroy_gadgebot_spawn_container(moby, hit_record);
        }
        if (moby[0x31] != 0) {
            add_draw_callback((int)draw_gadgebot_spawn_container, (int)moby);
        }
        break;
    case GADGEBOT_CONTAINER_WAIT_FOR_REMOVAL:
        qcopy(raised_position_snapshot, moby + 0x10);
        raised_position_snapshot[2] += 0.3f;
        if (FUN_L00_001fefc8(moby + 0xBC) != 0) {
            mark_moby_for_removal(moby);
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002D9B08), where it is exact; names translated to the US level program. */
void draw_gadgebot_spawn_container(char *moby) {
    L06BackdropQuad quad;
    float           matrix[3][4] __attribute__((aligned(16)));
    float           container_position_snapshot[4] __attribute__((aligned(16)));
    float           direction[4] __attribute__((aligned(16)));
    float           facePos[4] __attribute__((aligned(16)));
    float           normal[4] __attribute__((aligned(16)));
    float           projected[4] __attribute__((aligned(16)));
    int             near;
    int             i;
    float           scale;
    qcopy(matrix[0], moby + 0xc0);
    qcopy(matrix[1], moby + 0xd0);
    qcopy(matrix[2], moby + 0xe0);
    container_position_snapshot[0] = MOBY(moby)->pos.x;
    container_position_snapshot[1] = MOBY(moby)->pos.y;
    container_position_snapshot[2] = MOBY(moby)->pos.z;
    container_position_snapshot[3] = 1.0f;
    near = 0;
    if (absolute_float(l06_camera.pos.f[0] - container_position_snapshot[0]) < 16.0f &&
        absolute_float(l06_camera.pos.f[1] - MOBY(moby)->pos.y) < 16.0f) {
        near = 1;
    }
    if (near) {
        float *uv = &gadgebot_container_reflection_uvs[0][0];
        for (i = 0; i < 57; i++) {
            transform_vector(gadgebot_container_transformed_vertices[i],
                             gadgebot_container_local_vertices[i],
                             matrix);
            fast_vec_sub(direction, gadgebot_container_transformed_vertices[i], D_L06_00167540);
            fast_vec_normalize(direction, direction, 1.0f);
            transform_vector(normal, gadgebot_container_vertex_normals[i], matrix);
            fast_vec_normalize(
                normal, normal, *(float *)&gadgebot_container_vertex_normal_scale_gp);
            scale = fast_vec_dot(normal, direction);
            fast_vec_scale(projected, normal, scale + scale);
            fast_vec_sub(projected, direction, projected);
            fast_vec_normalize(projected, projected, 1.0f);
            projected[2] += 1.0f;
            scale = square_root(projected[2] + projected[2]);
            uv[0] = projected[0] / (scale + scale) + 0.5f;
            uv[1] = projected[1] / (scale + scale) + 0.5f;
            uv += 2;
        }
    } else {
        for (i = 0; i < 57; i++) {
            transform_vector(gadgebot_container_transformed_vertices[i],
                             gadgebot_container_local_vertices[i],
                             matrix);
        }
    }
    quad.tex = get_effect_texture(*(int *)&gadgebot_container_texture_index_gp);
    quad.unk80 = 0xff9000000260L;
    quad.unk88 = (long)*(int *)&gadgebot_container_blend_minuend_selector_gp |
                 (long)*(int *)&gadgebot_container_blend_subtrahend_selector_gp << 2 |
                 (long)*(int *)&gadgebot_container_blend_factor_selector_gp << 4 |
                 (long)*(int *)&gadgebot_container_blend_addend_selector_gp << 6 |
                 (long)*(int *)&gadgebot_container_blend_fixed_alpha_gp << 32;
    quad.unk70 = 0;
    quad.color[3] = *(int *)&gadgebot_container_quad_color_gp;
    quad.color[2] = *(int *)&gadgebot_container_quad_color_gp;
    quad.color[1] = *(int *)&gadgebot_container_quad_color_gp;
    quad.color[0] = *(int *)&gadgebot_container_quad_color_gp;
    i = 0;
    do {
        float *center = facePos;
        transform_point(center, gadgebot_container_face_positions[i], matrix);
        fast_vec_sub(direction, center, D_L06_00167540);
        fast_vec_normalize(direction, direction, 1.0f);
        transform_vector_by_basis(center, gadgebot_container_face_normals[i], matrix);
        if (!(0.0f < fast_vec_dot(direction, center))) {
            short *indices = gadgebot_container_face_vertex_uv_indices[i];
            int    j;
            for (j = 0; j < 4; j++, indices += 2) {
                short vertex = indices[0];
                qcopy(quad.corner[j], gadgebot_container_transformed_vertices[vertex]);
                if (near) {
                    quad.uv[j][0] = gadgebot_container_reflection_uvs[vertex][0];
                    quad.uv[j][1] = gadgebot_container_reflection_uvs[vertex][1];
                } else {
                    quad.uv[j][0] = gadgebot_container_static_uvs[indices[1]][0];
                    quad.uv[j][1] = gadgebot_container_static_uvs[indices[1]][1];
                }
            }
            draw_geometry_quad(&quad, 0, 0);
        }
        i++;
    } while (i < 42);
}

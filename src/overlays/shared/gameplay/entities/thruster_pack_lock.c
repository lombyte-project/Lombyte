/* Shared moby classes: thruster_pack_lock. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030FB58), where it is exact; names translated to the US level program. */

extern int D_L11_0015FFD8; /* no foreign declaration */
extern short *D_L11_001AC240[];

void synchronize_linked_moby_heights(char *moby) __asm__("FUN_L11_0030e690");

void synchronize_linked_moby_heights(char *moby) {
    short *moby_index_cursor = D_L11_001AC240[*(unsigned char *)(moby + 0x21)];
    int base = D_L11_0015FFD8;
    do {
        int i = (*moby_index_cursor & 0x7FFF) * 256;
        char *linked_moby = (char *)(i + base);
        char *destination_record;
        *(float *)(linked_moby + 0x18) = *(float *)(moby + 0x18);
        destination_record = D_L11_001DA880 + *(int *)(*(char **)(linked_moby + 0x78) + 4) * 0x1190;
        *(float *)(destination_record + 8) = *(float *)(moby + 0x18);
    } while (*moby_index_cursor++ >= 0);
}
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math/vector.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_003102C8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct { unsigned char pad0[0x454]; unsigned char collected[1]; } LvState_3102C8;

typedef struct {
    char pad000[0x360];
    unsigned short count;   /* 0x360 */
    unsigned short best;    /* 0x362 */
    unsigned int levels;    /* 0x364 */
    char pad368[0x22];
    unsigned short best2;   /* 0x38A */
} Stats_3102C8;

typedef struct {
    char pad00[0x10];
    float pos[2];           /* 0x10 */
    float z;                /* 0x18 */
    char pad1C[0x4];
    unsigned char state;    /* 0x20 */
    char pad21[0x57];
    char *vars;             /* 0x78 */
    char pad7C[0x34];
    unsigned char bB0;      /* 0xB0 */
    char padB1;
    unsigned short uid;     /* 0xB2 */
} Lock_3102C8;

extern unsigned char D_00141968_r[] __asm__("D_00141968");
extern unsigned char D_0014C190_r[] __asm__("D_0014C190");
extern unsigned char D_0013F350_r[] __asm__("D_0013F350");
extern unsigned char D_0014C050_r[] __asm__("D_0014C050");
extern unsigned char D_0013F3D0_r[] __asm__("D_0013F3D0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int D_0014C050; /* no foreign declaration */
extern int D_L11_0015FC88; /* no foreign declaration */
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
extern unsigned char D_0013F350[];
s32 FUN_L00_00203908(s32 a, s32 b);
extern u8 D_0013D4C0[];
extern unsigned char D_0014171B_3102C8[] __asm__("D_0013F350");
extern LvState_3102C8 D_L11_001BBF40_3102C8 __asm__("D_L11_001BBC30");
extern LvState_3102C8 D_L11_001BB2E0_3102C8 __asm__("D_L11_001BAFD0");
extern unsigned char D_L11_0015FD08_3102C8[] __asm__("D_L11_0015FC88");
extern void func_0022ED80_3102C8(int, int, void *) __asm__("FUN_0022da68");
extern int func_L00_00203F20_3102C8(int a, int b) __asm__("FUN_L00_00203908");
extern float func_001F9D48_3102C8(void *, void *) __asm__("FUN_001f9b80");
extern int func_001F9850_3102C8(int) __asm__("FUN_001f96f8");

void thruster_pack_lock_update(Lock_3102C8 *moby) __asm__("FUN_L11_0030ee00");
void thruster_pack_lock_update(Lock_3102C8 *moby)
{
    enum {
        THRUSTER_PACK_LOCK_STATE_INIT = 0,
        THRUSTER_PACK_LOCK_STATE_ACTIVE = 1
    };
    char *vars = moby->vars;

    switch (moby->state) {
    case THRUSTER_PACK_LOCK_STATE_INIT: {
        int level = current_level_index;
        ((Stats_3102C8 *)(D_00141968_r))->count = 0;
        ((Stats_3102C8 *)(D_00141968_r))->best = 0;
        *(unsigned short *)(vars + 0x3E) |= 8;
        if (level == 0xB && moby->bB0 != 0xFF) {
            if (D_L11_001BBF40_3102C8.collected[(short)moby->uid] != 0 ||
                ((*(int *)(D_0014C190_r + (((short)moby->uid >> 5) * 4 + (level << 8))) >> (moby->uid & 0x1F)) & 1)) {
                moby->state = 2;
                moby->z = moby->z - 1.5f;
                func_0022ED80_3102C8(0, 0, moby);
                return;
            }
        }
        moby->state = 1;
        break;
    }
    case THRUSTER_PACK_LOCK_STATE_ACTIVE:
        if (((void *)((Hero *)(D_0013F350_r))->ground_moby) == moby && D_0013D4C0[3] != 0 &&
            ((Hero *)(D_0013F350_r))->air_frames.s == 0 && ((Hero *)(D_0013F350_r))->state.current == 0x22) {
            if (current_level_index == 0xB && moby->bB0 != 0xFF) {
                int group;
                int group_index;
                D_L11_001BB2E0_3102C8.collected[(short)moby->uid] = moby->bB0 + 2;
                group = moby->bB0;
                group_index = group & 0xFF;
                if (group == 0xFF || (D_L11_0015FD08_3102C8[group_index] != 0xFF &&
                                  (D_0014C050_r)[group_index + (current_level_index << 4)] == 0xFF)) {
                    D_L11_001BBF40_3102C8.collected[(short)moby->uid] = group + 2;
                }
            }
            moby->state = 2;
            moby->z = moby->z - 1.5f;
            func_0022ED80_3102C8(0, 0, moby);
        }
        if (moby->bB0 == 0xFF) {
            return;
        }
        if (!(func_001F9D48_3102C8(moby->pos, D_0013F3D0_r) < 4.0f)) {
            return;
        }
        if (!((unsigned int)*(int *)(D_0013F3D0_r + 0x200C) < 2)) {
            return;
        }
        if (D_0013D4C0[3] == 0) {
            int elapsed = func_001F9850_3102C8(D_0015EEA4) - ((Stats_3102C8 *)(D_00141968_r))->best2 * 600;
            if ((int)((float)func_001F9850_3102C8(0x12) * 60.0f) < elapsed ||
                ((Stats_3102C8 *)(D_00141968_r))->best2 * 600 == 0) {
                func_L00_00203F20_3102C8(0x2B02, 0x71);
                return;
            }
            {
                int now = func_001F9850_3102C8(D_0015EEA4) / 600;
                if (((Stats_3102C8 *)(D_00141968_r))->best2 < now) {
                    ((Stats_3102C8 *)(D_00141968_r))->best2 = func_001F9850_3102C8(D_0015EEA4) / 600;
                }
            }
        } else if (((Stats_3102C8 *)(D_00141968_r))->count != 0) {
            int elapsed = func_001F9850_3102C8(D_0015EEA4) - ((Stats_3102C8 *)(D_00141968_r))->best * 600;
            if ((int)((float)func_001F9850_3102C8(0x12) * 60.0f) < elapsed ||
                ((Stats_3102C8 *)(D_00141968_r))->best * 600 == 0) {
                func_L00_00203F20_3102C8(0x2B01, 0x6C);
                return;
            }
            {
                int now = func_001F9850_3102C8(D_0015EEA4) / 600;
                if (((Stats_3102C8 *)(D_00141968_r))->best < now) {
                    ((Stats_3102C8 *)(D_00141968_r))->best = func_001F9850_3102C8(D_0015EEA4) / 600;
                }
            }
        } else {
            ((Stats_3102C8 *)(D_00141968_r))->count += 1;
            {
                int now = func_001F9850_3102C8(D_0015EEA4) / 600;
                if (((Stats_3102C8 *)(D_00141968_r))->best < now) {
                    ((Stats_3102C8 *)(D_00141968_r))->best = func_001F9850_3102C8(D_0015EEA4) / 600;
                }
            }
            ((Stats_3102C8 *)(D_00141968_r))->levels =
                ((Stats_3102C8 *)(D_00141968_r))->levels | (1 << current_level_index) | 0x80000000;
        }
        break;
    }
}
#include "qcopy.h"

/* Builds one GIF packet for a rotated textured sprite and advances the packet pointer. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_003121C0), where it is exact; names translated to the US level program. */

extern int D_L11_001611C0 __attribute__((section(".sdata")));
extern unsigned char screen_extent_c[] __asm__("D_0013E500");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern int ftoi(float) __asm__("FUN_001fa6d0");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_cos(float) __asm__("FUN_001f9dc8");

void append_rotated_textured_screen_quad(float x, float y, float width, float height, float angle, int texture_width,
                       int texture_height, long texture, int z_and_fog, int red, int green, int blue,
                       int alpha) __asm__("FUN_L11_00310cf8");

void append_rotated_textured_screen_quad(float x, float y, float width, float height, float angle, int texture_width,
                       int texture_height, long texture, int z_and_fog, int red, int green, int blue,
                       int alpha) {
    float vertical[4];
    float horizontal[4];
    float center[4];
    float top_left[4];
    float bottom_left[4];
    float top_right[4];
    float bottom_right[4];
    unsigned long rgba;
    unsigned long green_bits;
    unsigned long blue_bits;
    unsigned long alpha_bits;
    unsigned long texture_packet_value;
    int depth;
    unsigned long packed_depth;
    unsigned long gif_tag;
    unsigned long gif_regs;
    unsigned long prim;
    unsigned long tex_size;
    int gif_dma_tag;
    int u_end;
    int v_end;
    char *tag;
    unsigned long *packet;
    int *extent;
    float scale;

    rgba = red & 0xFF;
    green_bits = green & 0xFF;
    blue_bits = blue & 0xFF;
    alpha_bits = alpha & 0xFF;
    texture_packet_value = texture;
    depth = z_and_fog;

    center[0] = x;
    center[1] = y;
    vertical[0] = height * fast_sin(angle);
    vertical[1] = height * fast_cos(angle);
    horizontal[0] = width * fast_cos(angle);
    horizontal[1] = -width * fast_sin(angle);
    add_vector_xyz(top_left, center, vertical);
    subtract_vector_xyz(top_left, top_left, horizontal);
    add_vector_xyz(bottom_left, center, vertical);
    subtract_vector_xyz(top_right, center, horizontal);
    qcopy(bottom_right, center);

    *(int *)(D_L11_001611C0 + 0) = 0x10000007;
    green_bits = green_bits << 8;
    rgba = rgba | green_bits;
    gif_dma_tag = 0x50000007;
    gif_regs = 0x53535353106;
    *(int *)(D_L11_001611C0 + 4) = 0;
    blue_bits = blue_bits << 16;
    rgba = rgba | blue_bits;
    alpha_bits = alpha_bits << 24;
    prim = 0x154;
    tex_size = 0x100010;
    gif_tag = 0xB400000000008001;
    rgba = rgba | alpha_bits;
    *(int *)(D_L11_001611C0 + 8) = 0;
    scale = 16.0f;
    u_end = texture_width << 4;
    v_end = texture_height << 20;
    packed_depth = (unsigned long)depth << 32;
    *(int *)(D_L11_001611C0 + 12) = gif_dma_tag;
    tag = (char *)D_L11_001611C0;
    packet = (unsigned long *)(tag + 0x10);
    D_L11_001611C0 = (int)packet;
    *(unsigned long *)(tag + 0x10) = gif_tag;
    packet[1] = gif_regs;
    packet[2] = texture_packet_value;
    packet[3] = prim;
    packet[4] = rgba;
    packet[5] = tex_size;

    {
        int a = ftoi(top_left[0] * scale);
        int b = ftoi(top_left[1] * scale);
        extent = (int *)(screen_extent_c);
        packet[6] = (unsigned long)(a + extent[4] - 8) | ((unsigned long)(b + extent[5] - 8) << 16) | packed_depth;
        packet[7] = u_end;
    }
    {
        int a = ftoi(bottom_left[0] * scale);
        int b = ftoi(bottom_left[1] * scale);
        packet[8] = (unsigned long)(a + extent[4] - 8) | ((unsigned long)(b + extent[5] - 8) << 16) | packed_depth;
        packet[9] = v_end;
    }
    {
        int a = ftoi(top_right[0] * scale);
        int b = ftoi(top_right[1] * scale);
        v_end = v_end + u_end;
        packet[10] = (unsigned long)(a + extent[4] - 8) | ((unsigned long)(b + extent[5] - 8) << 16) | packed_depth;
        packet[11] = v_end;
    }
    {
        int a = ftoi(bottom_right[0] * scale);
        int b = ftoi(bottom_right[1] * scale);
        packet[12] = (unsigned long)(a + extent[4] - 8) | ((unsigned long)(b + extent[5] - 8) << 16) | packed_depth;
        packet[13] = 0;
    }

    /* Fakematch: the store through the address keeps the reload after the last packet store. */
    *(char **)&D_L11_001611C0 += 0x70;
}
#define NOT_SDA

#define MACRO_ADDR

/* Draws four quads with the angle advanced by a quarter turn each time. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00312510), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern s32 get_effect_texture() __asm__("FUN_001f44b8");

void draw_quarter_turn_textured_quads(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, float x,
                      float y, float scale, float quad_rotation_angle) __asm__("FUN_L11_00311048");

void draw_quarter_turn_textured_quads(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, float x,
                      float y, float scale, float quad_rotation_angle) {
    int texture;
    float quad_size;
    texture = get_effect_texture(0x42);
    quad_size = scale * 20.0f;
    append_rotated_textured_screen_quad(x, y, quad_size, quad_size, quad_rotation_angle, 0x3F, 0x3F, texture, 0xFFFFF3, red, green, blue, alpha);
    quad_rotation_angle = fast_add_rotations(quad_rotation_angle, 1.5707964f);
    append_rotated_textured_screen_quad(x, y, quad_size, quad_size, quad_rotation_angle, 0x3F, 0x3F, texture, 0xFFFFF3, red, green, blue, alpha);
    quad_rotation_angle = fast_add_rotations(quad_rotation_angle, 1.5707964f);
    append_rotated_textured_screen_quad(x, y, quad_size, quad_size, quad_rotation_angle, 0x3F, 0x3F, texture, 0xFFFFF3, red, green, blue, alpha);
    quad_rotation_angle = fast_add_rotations(quad_rotation_angle, 1.5707964f);
    append_rotated_textured_screen_quad(x, y, quad_size, quad_size, quad_rotation_angle, 0x3F, 0x3F, texture, 0xFFFFF3, red, green, blue, alpha);
}
/* Projects a world point to screen and stores its x and y in 1/16 units, y plus an offset. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_003126D8), where it is exact; names translated to the US level program. */

extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void project_to_screen(f32 *, void *) __asm__("FUN_001f2070");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void project_world_point_to_screen_pixels(void *pt, int *outx, int *outy, int yoff) __asm__("FUN_L11_00311210");

void project_world_point_to_screen_pixels(void *pt, int *outx, int *outy, int yoff) {
    float projected_point[4];
    int *screen_extent_data;
    project_to_screen(projected_point, pt);
    screen_extent_data = (int *)(D_0013E05A + 0x4A6);
    *outx = func_001FA898_r((projected_point[0] - (float)screen_extent_data[4]) * 0.0625f);
    *outy = func_001FA898_r((projected_point[1] - (float)screen_extent_data[5]) * 0.0625f) + yoff;
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00312780), where it is exact; names translated to the US level program. */


extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern f32 D_L11_00161FF4_gp __asm__("D_L11_00161FF4") __attribute__((sda));
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");
extern void spawn_type_74_particle(void *, float *, float *, void *, int) __asm__("FUN_L01_0028a7a8");

extern short D_L11_001621C8 __attribute__((sda));
extern short D_L11_001621D8 __attribute__((sda));
extern short D_L11_001621E8 __attribute__((sda));
extern short D_L11_001621F8 __attribute__((sda));
extern short D_L11_00162208 __attribute__((sda));
void emit_type_74_particles_from_moby_joints(void *moby, char *particle_emission_data) __asm__("FUN_L11_003112b8");

void emit_type_74_particles_from_moby_joints(void *moby, char *particle_emission_data) {
    int i;
    for (i = 1; i < 3; i++) {
        float initial_particle_velocity[4];
        float joint_world_position[4];
        float first_particle_position[4];
        float second_particle_position[4];
        float third_particle_position[4];
        float scale;

        *(OvlQuad *)initial_particle_velocity = 0;
        scale = *(float *)(particle_emission_data + 0x64) / *(float *)&D_L11_00161FF4_gp;
        if (scale > 1.0f) {
            scale = 1.0f;
        } else if (scale < 0.35f) {
            scale = 0.35f;
        }
        moby_point_position(moby, i, joint_world_position);
        joint_world_position[3] = 0.0f;
        qcopy(first_particle_position, joint_world_position);
        qcopy(second_particle_position, joint_world_position);
        qcopy(third_particle_position, joint_world_position);
        add_random_offset_to_vector_xyz(first_particle_position, *(float *)(particle_emission_data + 0xC4) * 0.02f + 0.02f);
        add_random_offset_to_vector_xyz(second_particle_position, *(float *)(particle_emission_data + 0xC4) * 0.02f + 0.02f);
        add_random_offset_to_vector_xyz(third_particle_position, *(float *)(particle_emission_data + 0xC4) * 0.01f + 0.02f);
        initial_particle_velocity[0] =
            -((random_float_between_alt(6.0f, 7.0f) + *(float *)(particle_emission_data + 0xC4) * 2.0f) * frame_time) *
            scale;
        spawn_type_74_particle(moby, first_particle_position, initial_particle_velocity, &D_L11_001621C8, 1);
        initial_particle_velocity[0] =
            -((random_float_between_alt(6.0f, 7.0f) + *(float *)(particle_emission_data + 0xC4) * 3.0f) * frame_time) *
            scale;
        spawn_type_74_particle(moby, second_particle_position, initial_particle_velocity, &D_L11_001621D8, 1);
        initial_particle_velocity[0] =
            -((random_float_between_alt(6.0f, 7.0f) + *(float *)(particle_emission_data + 0xC4) * 4.0f) * frame_time) *
            scale;
        spawn_type_74_particle(moby, third_particle_position, initial_particle_velocity, &D_L11_001621E8, 1);
        if (*(float *)(particle_emission_data + 0xC4) > 0.5f) {
            add_random_offset_to_vector_xyz(first_particle_position, *(float *)(particle_emission_data + 0xC4) * 0.02f + 0.01f);
            add_random_offset_to_vector_xyz(second_particle_position, *(float *)(particle_emission_data + 0xC4) * 0.02f + 0.01f);
            add_random_offset_to_vector_xyz(third_particle_position, *(float *)(particle_emission_data + 0xC4) * 0.01f + 0.01f);
            initial_particle_velocity[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(particle_emission_data + 0xC4) * 2.0f) *
                       frame_time) *
                     scale;
            spawn_type_74_particle(moby, first_particle_position, initial_particle_velocity, &D_L11_001621C8, 1);
            initial_particle_velocity[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(particle_emission_data + 0xC4) * 3.0f) *
                       frame_time) *
                     scale;
            spawn_type_74_particle(moby, second_particle_position, initial_particle_velocity, &D_L11_001621D8, 1);
            initial_particle_velocity[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(particle_emission_data + 0xC4) * 4.0f) *
                       frame_time) *
                     scale;
            spawn_type_74_particle(moby, third_particle_position, initial_particle_velocity, &D_L11_001621E8, 1);
            initial_particle_velocity[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(particle_emission_data + 0xC4) * 2.0f) *
                       frame_time) *
                     scale;
            spawn_type_74_particle(moby, first_particle_position, initial_particle_velocity, &D_L11_001621F8, 1);
            initial_particle_velocity[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(particle_emission_data + 0xC4) * 3.0f) *
                       frame_time) *
                     scale;
            spawn_type_74_particle(moby, joint_world_position, initial_particle_velocity, &D_L11_00162208, 1);
        }
    }
}
extern unsigned char D_0015EDB2;
extern float D_L11_001F1490[];
extern short D_L11_001623F0_d __asm__("D_L11_001623F0") __attribute__((sda));
extern long long trail_effect_texture(int) __asm__("FUN_001f44b8");
extern float integer_to_float(int) __asm__("FUN_001fa6c0");
extern void memset(void *, int, int) __asm__("FUN_001153fc");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void draw_packet(void *, int, int) __asm__("FUN_001f7d30");
extern int level_frame __asm__("D_L11_0015F5CC");
extern int D_L11_0015FFD8;
extern short *D_L11_001AC240[];

typedef struct {
    float positions[4][4];
    int colors[4];
    float texture_coordinates[4][2];
    long packet[4];
} TrailQuad;

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

int draw_linked_moby_trails(char *moby) __asm__("FUN_L11_003192c8");

int draw_linked_moby_trails(char *moby) {
    TrailQuad quads[16];
    float offset[2][4];
    short *list, *next_list;
    int i, j, segment, side, slot, more, next_segment, next_side, own_class, other_class, visible, half, corner;
    float base, blend;
    long skip;
    char *other, *trail;
    int never = 0;

    if (*(unsigned char *)(moby + 0x21) == 0xFF) {
        return;
    }
    if (D_0015EDB2 != 0) {
        *(float *)&D_L11_001623F0_d = 16.0f;
    } else {
        *(float *)&D_L11_001623F0_d = 5.0f;
    }

    for (i = 0; i < 16; i++) {
        long texture = trail_effect_texture(0x13);
        TrailQuad *quad = &quads[i];
        quad->packet[1] = texture;
        quad->packet[2] = 0xFF9000000260L;
        quad->packet[0] = 0;
        quad->packet[3] = 0x8000000048L;
        base = integer_to_float((level_frame + 3) & 3) / (*(float *)&D_L11_001623F0_d * 4.0f);
        for (j = 0; j < 4; j++) {
            half = (j >> 1) - 1;
            quads[i].texture_coordinates[j][0] = ((float (*)[2])D_L11_001F1490)[j][0];
            quads[i].texture_coordinates[j][1] = ((float (*)[2])D_L11_001F1490)[j][1];
            blend = integer_to_float(i - half) / *(float *)&D_L11_001623F0_d + base;
            if (1.0f < blend) {
                blend = 1.0f;
            } else if (blend < 0.0f) {
                blend = 0.0f;
            }
            quads[i].colors[j] = fast_tween_color(0x8C28AA28, 0x144646, blend);
        }
    }

    memset(offset, 0, 0x20);
    offset[0][2] = 0.25f;
    offset[1][2] = -0.25f;

    list = D_L11_001AC240[*(unsigned char *)(moby + 0x21)];
    do {
        next_list = list + 1;
        /* Fakematch: the never-taken call keeps more insns through gcse; its bigger expression
           table orders the spill slots of the two hoisted stack addresses as retail. */
        if (never) {
            draw_packet(0, 0, 0);
        }
        skip = 1;
        other = (char *)(((*list & 0x7FFF) << 8) + D_L11_0015FFD8);
        other_class = *(short *)(other + 0xA6);
        own_class = *(short *)(moby + 0xA6);
        visible = *(unsigned char *)(other + 0x31);
        if ((other_class ^ own_class) == 0) {
            skip = 0;
        }
        if (visible == 0) {
            skip = 1;
        }
        if (!skip) {
            trail = *(char **)(other + 0x78);
            segment = 0;
            if (*(int *)(trail + 0x2C0) > 0) {
                do {
                    next_segment = segment + 1;
                    slot = (*(int *)(trail + 0x2C4) - segment + 15) % 16;
                    side = 0;
                    do {
                        next_side = side + 1;
                        for (corner = 0; corner < 4; corner++) {
                            int point = (slot + (corner >> 1)) % 16;
                            add_vector_xyz(quads[segment].positions[corner], &((float (*)[16][4])(trail + 0xC0))[side][point][0],
                                           offset[corner & 1]);
                        }
                        draw_packet(&quads[segment], 0, 0);
                        side = next_side;
                    } while (side < 2);
                    segment = next_segment;
                } while (segment < *(int *)(trail + 0x2C0));
            }
        }
        more = *list >= 0;
        list = next_list;
    } while (more);
}
/* Advances a trail ring buffer and records two offset points each fourth frame. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0031AAE0), where it is exact; names translated to the US level program. */


extern int level_frame __asm__("D_L11_0015F5CC");
extern int draw_linked_moby_trails();
extern short D_L11_001623F0_d __asm__("D_L11_001623F0") __attribute__((sda));
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
void enqueue_callback_list_1(int arg0, int arg1) __asm__("FUN_001f4600");
extern void f49(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L11_003192c8_c(void) __asm__("FUN_L11_003192c8");

void update_moby_trail_points(char *moby) __asm__("FUN_L11_00319670");

void update_moby_trail_points(char *moby) {
    char *trail_data = *(char **)(moby + 0x78);
    float first_trail_offset[4];
    float second_trail_offset[4];
    int current_frame;
    if ((level_frame & 3) == 0) {
        int trail_segment_count;
        float trail_segment_limit = *(float *)&D_L11_001623F0_d;
        trail_segment_count = *(int *)(trail_data + 0x2C0);
        *(int *)(trail_data + 0x2C4) = (*(int *)(trail_data + 0x2C4) + 1) % 16;
        if ((float)trail_segment_count < trail_segment_limit) {
            *(int *)(trail_data + 0x2C0) = trail_segment_count + 1;
        }
    }
    *(OvlQuad *)first_trail_offset = 0;
    first_trail_offset[1] = 0.7f;
    first_trail_offset[0] = -1.2f;
    first_trail_offset[2] = 0.3f;
    transform_vector_by_basis(first_trail_offset, first_trail_offset, moby + 0xC0);
    add_vector_xyz(trail_data + ((*(int *)(trail_data + 0x2C4) << 4) + 0xC0), first_trail_offset, moby + 0x10);
    *(float *)(trail_data + (*(int *)(trail_data + 0x2C4) << 4) + 0xCC) = 1.0f;
    *(OvlQuad *)second_trail_offset = 0;
    second_trail_offset[0] = -1.2f;
    second_trail_offset[1] = -0.7f;
    second_trail_offset[2] = 0.3f;
    transform_vector_by_basis(second_trail_offset, second_trail_offset, moby + 0xC0);
    add_vector_xyz(trail_data + ((*(int *)(trail_data + 0x2C4) << 4) + 0x1C0), second_trail_offset, moby + 0x10);
    current_frame = level_frame;
    *(float *)(trail_data + (*(int *)(trail_data + 0x2C4) << 4) + 0x1CC) = 1.0f;
    if (current_frame != 0) {
        int *last_callback_frame = *(int **)(trail_data + 0x2C8);
        if (current_frame != *last_callback_frame) {
            *last_callback_frame = current_frame;
            f49(FUN_L11_003192c8_c, moby);
        }
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Collectable crate: when hit, updates the totals (count, best time, levels mask, a level-11 bonus) and breaks open. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0031B630), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
#include "rnc/overlay/shared/gameplay/entities/thruster_pack_lock_globals.h"
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern unsigned char D_0013D408[];
extern unsigned char D_00141968[];
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

void update_moby_hit_stats_and_state(char *moby) __asm__("FUN_L11_0031a1c0");

void update_moby_hit_stats_and_state(char *moby) {
    char *moby_vars = *(char **)(moby + 0x78);
    char *info = find_moby_hit(moby, 0x330000, 0);
    struct Hero *player_hero;
    ((unsigned char *)moby)[0xA4] = 0xFF;
    player_hero = &hero;
    if (player_hero->state.current != 0x32) {
        *(unsigned short *)(moby + 0x34) &= 0xEFFF;
    } else {
        *(unsigned short *)(moby + 0x34) |= 0x1000;
    }
    if (info != 0) {
        char *collection_stats_base = (char *)D_00141968;
        int next_moby_state;
        int current_moby_state;
        if (*(unsigned short *)(collection_stats_base + 0x157E8) < 0xFFFF) {
            *(unsigned short *)(collection_stats_base + 0x157E8) += 1;
        }
        if (scale_game_frames(D_0015EEA4) / 600 > *(unsigned short *)(collection_stats_base + 0x157EA)) {
            *(unsigned short *)(collection_stats_base + 0x157EA) = scale_game_frames(D_0015EEA4) / 600;
        }
        *(int *)(collection_stats_base + 0x157EC) = *(int *)(collection_stats_base + 0x157EC) | (1 << current_level_index) | 0x80000000;
        if (current_level_index == 0xB && *(char **)(info + 0x20) != 0 &&
            *(short *)(*(char **)(info + 0x20) + 0xA6) == 0xAC) {
            if (++level_11_class_ac_hit_count >= 3 && D_0013D408[0x12] == 0) {
                D_0013D408[0x12] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        if (*(int *)(moby_vars + 0x88) != 0) {
            next_moby_state = 6;
            current_moby_state = ((unsigned char *)moby)[0x20];
        } else {
            current_moby_state = ((unsigned char *)moby)[0x20];
            next_moby_state = 2;
        }
        if (current_moby_state != next_moby_state) {
            moby[0x20] = next_moby_state;
            if (((unsigned char *)moby)[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
    } else if (((unsigned char *)moby)[0x20] == 5) {
        struct Hero *hero_for_state_check = &hero;
        if (hero_for_state_check->state.current != 0x32) {
            moby[0x20] = 4;
            *(int *)(moby + 0x94) = 0;
            *(unsigned short *)(moby + 0x34) |= 1;
        }
        if (coll_sphere(1.0f, moby + 0x10, 0, moby)) {
            moby[0x20] = 6;
            if (((unsigned char *)moby)[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
    }
}

/* Queues a GIF packet that draws a line strip through the given points, scaled and offset to the screen. */
typedef unsigned int u128_ad0 __attribute__((mode(TI)));
extern int D_L11_001611C0_lui __asm__("D_L11_001611C0");
extern int screen_extent[] __asm__("D_0013E500");
extern float itof_g(int) __asm__("FUN_001fa6c0");
extern int ftoi_g(float) __asm__("FUN_001fa6d0");

extern u128_ad0 D_L11_001608E0;
extern u128_ad0 D_L11_001608F0;
void append_scaled_screen_triangle_strip(short *pts, int point_count, float x0, float y0, unsigned int col, unsigned long long prim, float scale) __asm__("FUN_L11_00310ad0");

void append_scaled_screen_triangle_strip(short *pts, int point_count, float x0, float y0, unsigned int col, unsigned long long prim, float scale) {
    int packet_qword_count = (point_count + 1) / 2 + 3;
    unsigned long long *vertex_payload;
    int x, y;
    int i;
    *(int *)(D_L11_001611C0 + 0) = packet_qword_count | 0x10000000;
    *(int *)(D_L11_001611C0 + 4) = 0;
    *(int *)(D_L11_001611C0 + 8) = 0;
    *(int *)(D_L11_001611C0 + 12) = packet_qword_count | 0x50000000;
    {
        char *dma_header = (char *)D_L11_001611C0;
        D_L11_001611C0 = (int)(dma_header + 0x10);
        qcopy((void *)D_L11_001611C0, &D_L11_001608E0);
        *(short *)(dma_header + 0x10) = -0x7FFF;
    }
    {
        unsigned long long *primitive_payload = (unsigned long long *)((char *)D_L11_001611C0 + 0x10);
        D_L11_001611C0 = (int)primitive_payload;
        primitive_payload[0] = 0x144;
        primitive_payload[1] = prim;
    }
    {
        char *saved_packet_cursor = (char *)D_L11_001611C0;
        D_L11_001611C0 = (int)(saved_packet_cursor + 0x10);
        qcopy((void *)D_L11_001611C0, &D_L11_001608F0);
        *(short *)(saved_packet_cursor + 0x10) = point_count - 0x8000;
    }
    D_L11_001611C0 += 0x10;
    vertex_payload = (unsigned long long *)D_L11_001611C0;
    for (i = 0; i < point_count; i++) {
        x = ftoi_g((itof_g(pts[i * 2]) * scale + x0) * 16.0f);
        y = ftoi_g((itof_g(pts[i * 2 + 1]) * scale + y0) * 16.0f);
        vertex_payload[i] = (unsigned long long)(x + screen_extent[4] - 8) | ((unsigned long long)(y + screen_extent[5] - 8) << 16) | ((unsigned long long)col << 32);
    }
    D_L11_001611C0 += ((point_count + 1) / 2) * 16;
}
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void interpolate_vector_xyz(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern float vector_distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_atan2(float, float) __asm__("FUN_001f9e90");
extern void rotation_matrix_from_euler(void *, void *) __asm__("FUN_001fa050");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");

#include "rnc/gameplay/entities/moby.h"
#define MOBY(p) ((struct Moby *)(p))

void update_moby_path_transform(char *moby) __asm__("FUN_L11_0031a438");

void update_moby_path_transform(char *moby) {
    char *moby_vars = (char *)MOBY(moby)->pvars;
    int i = *(int *)(moby_vars + 0x64);
    char *table = *(char **)(path_table + *(int *)(moby_vars + 0x60) * 4);
    int count;
    float matrix[16];
    float current_path_point[4], next_path_point[4], lookahead_path_point[4];
    float point[4], turn[4], direction[4];
    float angle_a, angle_b, slope_a, slope_b;
    float distance;
    float speed;

    qcopy(current_path_point, table + i * 16 + 0x10);
    count = *(int *)table;
    qcopy(next_path_point, table + (i + 1) % count * 16 + 0x10);
    qcopy(lookahead_path_point, table + (i + 2) % count * 16 + 0x10);
    clear_u64_value(turn);
    angle_a = fast_atan2(current_path_point[0] - next_path_point[0], current_path_point[1] - next_path_point[1]);
    angle_b = fast_atan2(next_path_point[0] - lookahead_path_point[0], next_path_point[1] - lookahead_path_point[1]);
    slope_a = fast_atan2(vector_distance_xy(current_path_point, next_path_point), next_path_point[2] - current_path_point[2]);
    slope_b = fast_atan2(vector_distance_xy(next_path_point, lookahead_path_point), lookahead_path_point[2] - next_path_point[2]);
    turn[1] = fast_add_rotations(fast_subtract_rotations(slope_b, slope_a) * *(float *)(moby_vars + 0x68), slope_a);
    turn[2] = fast_add_rotations(fast_subtract_rotations(angle_b, angle_a) * *(float *)(moby_vars + 0x68), angle_a);
    qcopy(&MOBY(moby)->rot, turn);
    if (*(float *)(moby_vars + 0x6C) > 0.0f) {
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, 3.14159f);
        MOBY(moby)->rot.y = -MOBY(moby)->rot.y;
    }
    interpolate_vector_xyz(point, current_path_point, next_path_point, *(float *)(moby_vars + 0x68));
    rotation_matrix_from_euler(matrix, turn);
    normalize_vector_xyz(direction, matrix + 4, *(float *)(moby_vars + 0x78) * fast_sin(*(float *)(moby_vars + 0x70)));
    add_vector_xyz(point, point, direction);
    normalize_vector_xyz(direction, matrix + 8, *(float *)(moby_vars + 0x78) * fast_cos(*(float *)(moby_vars + 0x70)));
    add_vector_xyz(point, point, direction);
    speed = 0.0f;
    distance = vector_distance_xyz(&MOBY(moby)->pos, point);
    advance_accelerated_scalar(&speed, (float *)(moby_vars + 0x80), distance, frame_time_sq * 10.0f, frame_time_sq * 10.0f, frame_time * 20.0f);
    subtract_vector_xyz(direction, point, &MOBY(moby)->pos);
    normalize_vector_xyz(direction, direction, *(float *)(moby_vars + 0x80));
    add_vector_xyz(&MOBY(moby)->pos, &MOBY(moby)->pos, direction);
}


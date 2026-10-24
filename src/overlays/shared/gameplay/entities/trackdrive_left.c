/* Shared moby classes: trackdrive_left, tresspasser_pad, escalator, swingshot_versatarget, tresspasser. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

extern u8 D_L02_001CB9C0[];
extern s32 scaled_table_header_leading_word __asm__("D_L02_001612A0") __attribute__((sda));
extern s32 D_L02_001612A4;
extern s32 D_L02_001612A8;
extern s32 D_L02_001612AC;

extern float D_L02_001CB500[];
void prepare_scaled_float_table_with_target_byte(char *value_target, int val, float scale) __asm__("FUN_L02_002a4058");

void prepare_scaled_float_table_with_target_byte(char *value_target, int val, float scale) {
    int i;
    float *destination;
    float *source;
    float *base;
    value_target[0x3F] = val;
    *(s32 *)(D_L02_001CB9C0 + 0x0) = scaled_table_header_leading_word;
    *(s32 *)(D_L02_001CB9C0 + 0x4) = D_L02_001612A4;
    *(s32 *)(D_L02_001CB9C0 + 0x8) = D_L02_001612A8;
    *(s32 *)(D_L02_001CB9C0 + 0xC) = D_L02_001612AC;
    destination = (float *)(D_L02_001CB9C0 + 0x10);
    base = D_L02_001CB500;
    source = base + 4;
    for (i = 0xF7; i >= 0; i--) {
        *destination = *source * scale;
        destination++;
        source++;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002A52B0), where it is exact; names translated to the US level program. */

extern void FUN_L02_002a4058_u(char *arg, int val) __asm__("FUN_L02_002a4058");

void prepare_scaled_table_with_byte_128(char *value_target) __asm__("FUN_L02_002a40d0");

void prepare_scaled_table_with_byte_128(char *value_target) {
    FUN_L02_002a4058_u(value_target, 0x80);
}
#include "qcopy.h"

/* Builds one moby's VU1 draw packet: camera matrices and screen setup, the two interpolated vertex tables of
 * its animation frame, then the GS register block for its texture. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002A52D0), where it is exact; names translated to the US level program. */

typedef struct QuadDrawArgs {
    char pad0[0x1C];
    char *dst;
    float f20, f24, f28, f2C;
    unsigned char c30, c31, c32, frames;
    char pad34[8];
    unsigned short first;
    unsigned char count;
    unsigned char alpha;
} QuadDrawArgs;

typedef struct QuadCamera {
    char pad0[0x190];
    float v190[4];
    float v1A0[4];
    char pad1B0[0x60];
    float f210;
    char pad214[4];
    float f218;
    float f21C;
    char pad220[8];
    float f228;
    float f22C;
    int c230, c234, c238;
} QuadCamera;

typedef struct QuadFrame { char pad0[8]; unsigned short a; unsigned short b; char padC[4]; } QuadFrame;

extern void update_view_context(void) __asm__("FUN_001f2d98");
extern void update_camera_view_matrices(void) __asm__("FUN_001f2260");
extern void set_scale_matrix(float *, float) __asm__("FUN_001f9ff8");
extern void setup_offset_vector(void *, void *, float) __asm__("FUN_001f9a68");
extern void vu1_add_data_ref(void *, int) __asm__("FUN_00233830");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void FUN_L02_0024fca0(void *, void *, void *, void *, void *, int, float);
extern char D_L02_0016D040[];
extern int textured_grid_fog_source_mode __asm__("D_L02_0015F458");
extern float textured_grid_saved_fog_near_distance __asm__("D_L02_001612F8");
extern float textured_grid_saved_fog_far_distance __asm__("D_L02_001612FC");
extern float textured_grid_saved_fog_near_intensity __asm__("D_L02_00161300");
extern float textured_grid_saved_fog_far_intensity __asm__("D_L02_00161304");
extern char D_L02_001673C0[];
extern unsigned short font_vu_program_tag[] __asm__("D_0010E800");
extern u8 font_vu_program[] __asm__("D_0010E810");
extern char D_L02_001CB8F0[];
extern QuadFrame D_L02_0016D780[];
extern char *D_L02_0015F420;
#include "rnc/rendering/dma_tag.h"

extern union PacketCursor l02_packet __asm__("D_L02_001611C0") __attribute__((section(".sdata")));
extern struct GsTextureAllocation gs_texture_allocation __asm__("D_0015EE74") __attribute__((section(".sdata")));
#define PACKET_CURSOR (l02_packet.words)
#define TEXTURE_CURSOR (gs_texture_allocation.cursor)
/* Fakematch: the frame counter is read as a one-element array, so its load stays below the packet stores like retail. */
extern int frame_counter_ref[] __asm__("D_L02_0015F5CC") __attribute__((section(".sdata")));
#define FRAME_COUNTER (frame_counter_ref[0])
/* The quad's GS texture register and the camera values it saves (D_L02_001612F8..00161304). */
struct QuadTextureState {
    long tex0;
    float camera_218;
    float camera_21C;
    float camera_228;
    float camera_22C;
};
extern struct QuadTextureState quad_texture_state __asm__("D_L02_001612F0") __attribute__((section(".sdata")));
#define TEXTURE_REGISTER (quad_texture_state.tex0)

void build_interpolated_dma_packet(QuadDrawArgs *draw_parameters) __asm__("FUN_L02_002a40f0");

void build_interpolated_dma_packet(QuadDrawArgs *draw_parameters) {
    float view_matrix[16];
    int *base;
    int *cursor;
    long *words;
    long color;
    long blend_mode;
    unsigned int alpha_limit;

    textured_grid_saved_fog_near_distance = ((QuadCamera *)D_L02_0016D040)->f218;
    textured_grid_saved_fog_far_distance = ((QuadCamera *)D_L02_0016D040)->f21C;
    textured_grid_saved_fog_near_intensity = ((QuadCamera *)D_L02_0016D040)->f228;
    textured_grid_saved_fog_far_intensity = ((QuadCamera *)D_L02_0016D040)->f22C;
    if (textured_grid_fog_source_mode != 1) {
        ((QuadCamera *)D_L02_0016D040)->f218 = draw_parameters->f20;
        ((QuadCamera *)D_L02_0016D040)->f21C = draw_parameters->f24;
        ((QuadCamera *)D_L02_0016D040)->f228 = draw_parameters->f28;
        ((QuadCamera *)D_L02_0016D040)->f22C = draw_parameters->f2C;
    }
    update_view_context();
    update_camera_view_matrices();
    set_scale_matrix(view_matrix, 1024.0f);
    setup_offset_vector(&view_matrix[12], D_L02_001673C0, -1024.0f);
    view_matrix[15] = 1.0f;
    vu1_add_data_ref(font_vu_program, font_vu_program_tag[0]);
    PACKET_CURSOR[0] = 0x10000000;
    PACKET_CURSOR[1] = 0;
    PACKET_CURSOR[2] = 0x11000000;
    PACKET_CURSOR[3] = 0x01000404;
    base = PACKET_CURSOR;
    base[4] = 0;
    base[5] = 0;
    base[6] = 0;
    base[7] = 0x6C0C43A4;
    cursor = base + 8;
    multiply_matrix(cursor, D_L02_001673C0 - 0x100, view_matrix);
    cursor = base + 0x18;
    multiply_matrix(cursor, D_L02_001673C0 - 0x80, view_matrix);
    base[0x28] = 0x8000;
    base[0x29] = 0x303EC000;
    base[0x2A] = 0x412;
    *(float *)(base + 0x2B) = ((QuadCamera *)D_L02_0016D040)->f210;
    cursor = base + 0x2C;
    qcopy(cursor, ((QuadCamera *)D_L02_0016D040)->v190);
    cursor = base + 0x30;
    qcopy(cursor, ((QuadCamera *)D_L02_0016D040)->v1A0);
    *(float *)(base + 0x34) = ((QuadCamera *)D_L02_0016D040)->f22C;
    *(float *)(base + 0x35) = ((QuadCamera *)D_L02_0016D040)->f228;
    base[0x36] = 0;
    base[0x37] = 0;
    base[0x38] = 0x03000000;
    base[0x39] = 0x020001D2;
    base[0x3A] = 0x15000000;
    base[0x3B] = 0;
    cursor = base + 0x3C;
    PACKET_CURSOR[0] |= ((char *)cursor - (char *)PACKET_CURSOR >> 4) - 1;
    PACKET_CURSOR = cursor;
    PACKET_CURSOR[0] = 0x30000003;
    PACKET_CURSOR[1] = (int)D_L02_001CB8F0;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000003;
    PACKET_CURSOR += 4;
    PACKET_CURSOR[0] = 0x10000009;
    PACKET_CURSOR[1] = 0;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000009;
    PACKET_CURSOR += 4;
    {
        int frame;
        int step;
        float fraction;
        char *dst;
        QuadFrame *frame_from;
        QuadFrame *frame_to;
        char *from_a, *from_b, *to_a, *to_b;
        frame = FRAME_COUNTER;
        step = frame / draw_parameters->frames;
        fraction = (float)frame / (float)draw_parameters->frames - (float)step;
        frame_from = &D_L02_0016D780[draw_parameters->first + step % draw_parameters->count];
        frame_to = &D_L02_0016D780[draw_parameters->first + (step + 1) % draw_parameters->count];
        from_a = D_L02_0015F420 + frame_from->a * 16;
        from_b = D_L02_0015F420 + frame_from->b * 16;
        to_b = D_L02_0015F420 + frame_to->b * 16;
        to_a = D_L02_0015F420 + frame_to->a * 16;
        dst = draw_parameters->dst;
        *(long *)dst = 0x0800000000000400;
        *(long *)(draw_parameters->dst + 8) = 0;
        FUN_L02_0024fca0(draw_parameters->dst + 0x10, from_a, from_b, to_a, to_b, 0x1000, fraction);
    }
    TEXTURE_REGISTER = (TEXTURE_CURSOR >> 8) | 0x18100000 | 0x580000000;
    color = draw_parameters->c30 | draw_parameters->c31 << 8 | draw_parameters->c32 << 16;
    if (textured_grid_fog_source_mode == 1) {
        color = ((QuadCamera *)D_L02_0016D040)->c230 | ((QuadCamera *)D_L02_0016D040)->c234 << 8
            | ((QuadCamera *)D_L02_0016D040)->c238 << 16;
    }
    words = (long *)PACKET_CURSOR;
    alpha_limit = 0x60;
    blend_mode = 0x5360A;
    if (!(alpha_limit < draw_parameters->alpha)) {
        blend_mode = 0x5370B;
    }
    words[0] = 0x8000000000008001;
    words[1] = 0xEEEEEEEE;
    words[2] = (long)draw_parameters->alpha << 32 | 0x64;
    words[3] = 0x42;
    words[4] = TEXTURE_REGISTER;
    words[5] = 6;
    words[6] = color;
    words[7] = 0x3D;
    words[8] = (long)(TEXTURE_CURSOR >> 8) << 32 | 0x1000000000000;
    words[9] = 0x50;
    words[10] = 0;
    words[11] = 0x51;
    words[12] = 0x4000000040;
    words[13] = 0x52;
    words[14] = 0;
    words[15] = 0x53;
    words[16] = blend_mode;
    words[17] = 0x47;
    PACKET_CURSOR = (int *)(words + 18);
    PACKET_CURSOR[0] = 0x30000401;
    PACKET_CURSOR[1] = (int)draw_parameters->dst;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000401;
    PACKET_CURSOR += 4;
    PACKET_CURSOR[0] = 0x10000002;
    PACKET_CURSOR[1] = 0;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000002;
    PACKET_CURSOR += 4;
    words = (long *)PACKET_CURSOR;
    words[0] = 0x1000000000008001;
    words[1] = 0xE;
    words[2] = 0;
    words[3] = 0x3F;
    PACKET_CURSOR = (int *)(words + 4);
    TEXTURE_CURSOR += 0x4000;
}
#undef PACKET_CURSOR
#undef FRAME_COUNTER
#undef TEXTURE_CURSOR
#undef TEXTURE_REGISTER
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002A58C0), where it is exact; names translated to the US level program. */

extern int *D_L02_001611C0;
extern void update_camera_view_matrices(void) __asm__("FUN_001f2260");
extern void vu1_add_vif_code(int) __asm__("FUN_00233938");
extern void update_view_context(void) __asm__("FUN_001f2d98");
void aa_blur_pass(void) __asm__("FUN_001fb680");
extern void aa_blur_pass_00233b68(void) __asm__("FUN_00233b68");

void restore_view_after_textured_grid(void) __asm__("FUN_L02_002a46e0");

void restore_view_after_textured_grid(void) {
    char *camera_state;
    long *gs_register_packet;
    int *packet_base;
    vu1_add_vif_code(0x13000000);
    camera_state = D_L02_0016D040;
    *(float *)(camera_state + 0x218) = textured_grid_saved_fog_near_distance;
    *(float *)(camera_state + 0x21C) = textured_grid_saved_fog_far_distance;
    *(float *)(camera_state + 0x228) = textured_grid_saved_fog_near_intensity;
    *(float *)(camera_state + 0x22C) = textured_grid_saved_fog_far_intensity;
    update_view_context();
    update_camera_view_matrices();
    (*(int *volatile *)&D_L02_001611C0)[0] = 0x10000002;
    (*(int *volatile *)&D_L02_001611C0)[1] = 0;
    (*(int *volatile *)&D_L02_001611C0)[2] = 0x11000000;
    (*(int *volatile *)&D_L02_001611C0)[3] = 0x50000002;
    packet_base = *(int *volatile *)&D_L02_001611C0;
    D_L02_001611C0 = packet_base + 4;
    gs_register_packet = (long *)(packet_base + 4);
    gs_register_packet[0] = 0x1000000000008001L;
    gs_register_packet[1] = 0xE;
    gs_register_packet[2] = (long)*(int *)(camera_state + 0x230) | ((long)*(int *)(camera_state + 0x234) << 8) |
           ((long)*(int *)(camera_state + 0x238) << 16);
    gs_register_packet[3] = 0x3D;
    D_L02_001611C0 = packet_base + 12;
    aa_blur_pass_00233b68();
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002CCD18), where it is exact; names translated to the US level program. */

extern unsigned int D_L02_0015FFDC;

void trackdrive_left_update(char *moby) __asm__("FUN_L02_002cb978");
void trackdrive_left_update(char *moby) {
    if ((unsigned int)moby < D_L02_0015FFDC) {
        *(int *)(moby + 0x58) = 0;
        *(unsigned short *)(moby + 0x34) |= 0x40;
    }
}

#include "qcopy.h"

typedef float V2d8910[4] __attribute__((aligned(16)));
typedef struct {
    char pad[0x140];
    float x;
    float y;
} H2d8910;
extern H2d8910 D_L02_00167280_2d8910 __asm__("D_L02_00167280") __attribute__((section(".data")));
extern char *D_L02_001600EC_2d8910 __asm__("D_L02_001600EC");
extern float addrot_2d8910(float, float) __asm__("FUN_001fa580");
extern float diffrot_2d8910(float, float) __asm__("FUN_001fa688");
extern float atan2_2d8910(float, float) __asm__("FUN_001f9e90");
extern float cos_2d8910(float) __asm__("FUN_001f9dc8");
extern float sin_2d8910(float) __asm__("FUN_001f9de0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");

/* Spawn point for the moby: its slot's stored position and rotation, or, with no slot, a point beside it on the side facing the hero. */
void compute_tresspasser_pad_camera_transform(float *pos, float *camera_rotation_output, char *moby) __asm__("FUN_L02_002d8910");

void compute_tresspasser_pad_camera_transform(float *pos, float *camera_rotation_output, char *moby) {
    V2d8910 midpoint_position;
    float left;
    float right;
    float pick;
    float to;
    float dl;
    int camera_transform_index;
    char *base;

    camera_transform_index = *(int *)(*(char **)(moby + 0x78) + 0xE8);
    if (camera_transform_index == -1) {
        left = addrot_2d8910(*(float *)(moby + 0x48), 1.57079637f);
        right = addrot_2d8910(*(float *)(moby + 0x48), -1.57079637f);
        to = atan2_2d8910(D_L02_00167280_2d8910.x - *(float *)(moby + 0x10),
                          D_L02_00167280_2d8910.y - *(float *)(moby + 0x14));
        dl = diffrot_2d8910(to, left);
        pick = dl < diffrot_2d8910(to, right) ? left : right;
        fast_vec_add(midpoint_position, &hero.motion.pos, moby + 0x10);
        fast_vec_scale(midpoint_position, midpoint_position, 0.5f);
        qcopy(pos, midpoint_position);
        pos[0] += cos_2d8910(pick) * 2.0f;
        pos[1] += sin_2d8910(pick) * 2.0f;
        pos[2] += 1.0f;
        camera_rotation_output[0] = 0;
        camera_rotation_output[1] = 0;
        camera_rotation_output[2] = addrot_2d8910(pick, 3.14159274f);
    } else {
        base = D_L02_001600EC_2d8910;
        qcopy(pos, base + camera_transform_index * 128 + 0x30);
        qcopy(camera_rotation_output, base + camera_transform_index * 128 + 0x70);
    }
}

#include "rnc/gameplay/entities/moby.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002D9F08), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2D9F08;

typedef struct Data_2D9F08 {
    char p0[0x10];
    short h10;
    short h12;
    int i14;
    int i18;
    int i1C;
    char p20[0x90];
    int iB0[12];
    int iE0;
    float fE4;
    char pE8[0x4];
    int iEC;
    Vec_2D9F08 vF0;
    Vec_2D9F08 v100;
    float f110;
    float f114;
    char p118[0x4];
    short h11C;
    unsigned short h11E;
} Data_2D9F08;

typedef struct Cam_2D9F08 {
    char p0[0x140];
    Vec_2D9F08 v140;
    Vec_2D9F08 v150;
    char p160[0x20];
    struct Moby *p180;
} Cam_2D9F08;

typedef struct Pad_2D9F08 {
    char p0[0x1A4];
    int i1A4;
    char p1A8[0x18];
    union {
        long l;
        struct {
            int lo;
            int hi;
        } w;
    } b1C0;
} Pad_2D9F08;

typedef struct {
    int i0;
    char p4[0x20];
    int i24;
} Idle_2D9F08;

typedef struct {
    char pad0[0x454];
    unsigned char collected[1];
} Lvl_2D9F08;

extern char D_0013C940[];
extern char D_0013FDE8[];
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float D_0015ED6C __attribute__((section(".sdata")));
extern float D_0015ED70 __attribute__((section(".sdata")));
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int D_0015ED84; /* no foreign declaration */
extern int fast_dec_timer_s16() __asm__("FUN_001f9770");
extern int lerp_vector() __asm__("FUN_001f9a40");
extern int FUN_L00_002323b8();
extern int FUN_L02_0022b728();
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 force_help_message(s32 arg0, s32 arg1) __asm__("FUN_002151d8");
extern void allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
void trooper_anim_finish(s32 id, void *s) __asm__("FUN_L00_0025d538");
void FUN_L00_002ea9d8(char *src);
void FUN_L00_002eaa30(char *src);
void jet_mount(void *pa, void *pb, s32 mode, s32 w, s32 x) __asm__("FUN_L00_002eaaa0");
void FUN_L00_002eac18(int mode);
void request_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L01_002405a0");
void FUN_L02_002d8910_c(float *pos, float *rot, char *moby) __asm__("FUN_L02_002d8910");
void FUN_L02_002d92f0_c(char *moby) __asm__("FUN_L02_002d92f0");
void draw_tresspasser_puzzle(char *moby) __asm__("FUN_L02_002d93e8");
/* Fakematch: the first parameter is an int; as a long the 0 passed after the 64-bit pad test is set again instead of taken from the mask register, like retail. */
extern int func_0022ED80_2D9F08(long, int, Moby *) __asm__("FUN_0022da68");
extern int func_001F9850_2D9F08(int) __asm__("FUN_001f96f8");
extern int func_L02_0022BE40_2D9F08(int, int) __asm__("FUN_L02_0022b728");
extern void func_L00_00232C10_2D9F08(int, int, float) __asm__("FUN_L00_002323b8");
extern void func_L00_002EBF50_2D9F08(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void func_L02_002DA728_2D9F08(Moby *) __asm__("FUN_L02_002d92f0");
extern void func_L02_002D9D48_2D9F08(void *, void *, Moby *) __asm__("FUN_L02_002d8910");
extern float func_00214D88_2D9F08(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void func_001F9C08_2D9F08(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern float func_001FA790_2D9F08(float, float) __asm__("FUN_001fa5c8");
extern float func_001FA748_2D9F08(float, float) __asm__("FUN_001fa580");
extern void func_L00_002EBE88_2D9F08(void *) __asm__("FUN_L00_002ea9d8");
extern void func_L00_002EBEE0_2D9F08(void *) __asm__("FUN_L00_002eaa30");
extern int func_00216028_2D9F08(int, int) __asm__("FUN_002151d8");
extern float func_001FA850_2D9F08(float, float) __asm__("FUN_001fa688");
extern void func_001F49B0_2D9F08(void (*)(void), Moby *) __asm__("FUN_001f4600");
extern void func_L02_002DA820_2D9F08(void) __asm__("FUN_L02_002d93e8");
extern int func_001F9938_2D9F08(void *) __asm__("FUN_001f9770");
extern void func_L00_002EC0C8_2D9F08(int) __asm__("FUN_L00_002eac18");
extern void request_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L01_002405a0");
extern void func_L00_0025E590_2D9F08(Moby *, Data_2D9F08 *) __asm__("FUN_L00_0025d538");
extern float D_L02_001D33B0[4][4];
extern Lvl_2D9F08 D_L02_001BBB40_2D9F08 __asm__("D_L02_001BB830");
extern int D_0015EE84_2D9F08 __asm__("D_0015ED84") __attribute__((section(".sdata")));
extern char D_0014C290_2D9F08[] __asm__("D_0014C190");
extern Vec_2D9F08 D_L02_00167440_2D9F08[] __asm__("D_L02_001673C0");
extern float D_0015EE70_2D9F08 __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern float D_0015EE6C_2D9F08 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern int D_L02_0017EB88_2D9F08 __asm__("D_L02_0017EB08") __attribute__((section(".data")));
extern Cam_2D9F08 D_L02_00167300_2D9F08 __asm__("D_L02_00167280");
extern Idle_2D9F08 D_L02_00179A10;
extern int D_L02_001BADE0_2D9F08[] __asm__("D_L02_001BAAD0");
extern int D_L02_00161B1C_gp __asm__("D_L02_00161B1C") __attribute__((sda));
extern int D_L02_00161B38_2D9F08 __asm__("D_L02_00161AA8") __attribute__((sda));
extern int D_L02_00161B34_2D9F08 __asm__("D_L02_00161AA4") __attribute__((sda));
extern int D_L02_00161B30_2D9F08 __asm__("D_L02_00161AA0") __attribute__((sda));
extern char D_0013E633_2D9F08[] __asm__("D_0013E533");
extern Hero D_0013F450_2D9F08 __asm__("D_0013F350");
extern int D_0013CAE4 __attribute__((section(".data")));
extern char D_0013C940[];

void tresspasser_pad_update(Moby *moby) __asm__("FUN_L02_002d8ad0");
void tresspasser_pad_update(Moby *moby) {
    Vec_2D9F08 camera_pos, camera_rot;
    Data_2D9F08 *data = (struct Data_2D9F08 *)moby->pvars;
    int i;

    if (data == 0) {
        return;
    }
    switch (moby->state) {
    case 0:
        data->h12 = 0;
        moby->state = 1;
        D_L02_001D33B0[2][1] = 17.0f;
        D_L02_001D33B0[0][1] = 17.0f;
        D_L02_001D33B0[1][0] = 17.0f;
        D_L02_001D33B0[1][1] = -17.0f;
        D_L02_001D33B0[2][0] = -17.0f;
        D_L02_001D33B0[3][0] = -17.0f;
        D_L02_001D33B0[3][1] = -17.0f;
        D_L02_001D33B0[0][0] = 17.0f;
        D_L02_00161B1C_gp = data->iEC;
        if (moby->mission != 0xFF) {
            if (D_L02_001BBB40_2D9F08.collected[(short)moby->save_id] != 0
                || (((int *)(D_0014C290_2D9F08 + (D_0015EE84_2D9F08 << 8)))[(short)moby->save_id >> 5] >> (moby->save_id & 0x1F)) & 1) {
                moby->state = 4;
            }
        }
        break;
    case 1:
        if (((Hero *)(D_0013E633_2D9F08 + 0xE1D))->ground_moby == moby && ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->air_frames.s == 0) {
            int holding_tool;

            if (data->h12 == 0) {
                data->h12 = 1;
                func_0022ED80_2D9F08(3, 0, moby);
            }
            if (((Hero *)(D_0013E633_2D9F08 + 0xE1D))->items[0].moby != 0 && ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->items[0].item_id == 0x1A) {
                moby->glow_color = D_L02_00161B30_2D9F08;
                holding_tool = 1;
            } else {
                moby->glow_color = D_L02_00161B34_2D9F08;
                holding_tool = 0;
            }
            if (holding_tool && ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->items[0].moby->command != 0) {
                data->h10 = func_001F9850_2D9F08(0x12C);
                data->h11C = 0;
                data->h11E = 0;
                func_L02_0022BE40_2D9F08(0x72, 0);
                func_L00_00232C10_2D9F08(0x57, 0, func_001F9850_2D9F08(6));
                func_L00_002EBF50_2D9F08(&D_L02_00167440_2D9F08[0], &D_L02_00167440_2D9F08[1], 1, 0, 0);
                moby->state = 2;
                data->h10 = func_001F9850_2D9F08(0x3C);
                data->f110 = 0.0f;
                qcopy(&data->vF0, &D_L02_00167440_2D9F08[0]);
                qcopy(&data->v100, &D_L02_00167440_2D9F08[1]);
            }
        } else {
            data->h12 = 0;
            moby->glow_color = D_L02_00161B38_2D9F08;
        }
        break;
    case 2:
        func_L02_002DA728_2D9F08(moby);
        func_L02_002D9D48_2D9F08(&camera_pos, &camera_rot, moby);
        func_00214D88_2D9F08(&data->f110, &data->f114, 1.0f, D_0015EE70_2D9F08 + D_0015EE70_2D9F08, D_0015EE70_2D9F08 + D_0015EE70_2D9F08, D_0015EE6C_2D9F08 + D_0015EE6C_2D9F08);
        func_001F9C08_2D9F08(&camera_pos, &data->vF0, &camera_pos, data->f110);
        camera_rot.z = func_001FA748_2D9F08(func_001FA790_2D9F08(camera_rot.z, data->v100.z) * data->f110, data->v100.z);
        camera_rot.y = func_001FA748_2D9F08(func_001FA790_2D9F08(camera_rot.y, data->v100.y) * data->f110, data->v100.y);
        func_L00_002EBE88_2D9F08(&camera_pos);
        func_L00_002EBEE0_2D9F08(&camera_rot);
        D_L02_0017EB88_2D9F08 = 1;
        func_00216028_2D9F08(6, 0);
        if (D_L02_00167300_2D9F08.p180 != 0 && D_L02_00167300_2D9F08.p180->unk86 != 5) {
            func_L00_002EBF50_2D9F08(&D_L02_00167300_2D9F08.v140, &D_L02_00167300_2D9F08.v150, 1, 0, 0);
        }
        i = 0;
        if (0.99f <= data->f110 && func_001FA850_2D9F08(D_L02_00167300_2D9F08.v150.z, camera_rot.z) < 0.017453292f) {
            func_001F49B0_2D9F08(func_L02_002DA820_2D9F08, moby);
            for (i = 0; i < 12; i++) {
                if (data->iB0[i] == 1) {
                    break;
                }
            }
            if (0.0f < data->fE4) {
                data->fE4 = func_001FA748_2D9F08(data->fE4, D_0015EE6C_2D9F08 * 6.2831855f);
                if (0.5235988f < data->fE4) {
                    data->fE4 = 0.0f;
                    if (data->iE0 == 1) {
                        data->i14 += 11;
                        data->i14 %= 12;
                    } else if (data->iE0 == 2) {
                        data->i18 += 11;
                        data->i18 %= 12;
                    } else {
                        data->i1C += 11;
                        data->i1C %= 12;
                    }
                }
            } else if (data->fE4 < 0.0f) {
                data->fE4 = func_001FA748_2D9F08(data->fE4, -(D_0015EE6C_2D9F08 * 6.2831855f));
                if (data->fE4 < -0.5235988f) {
                    data->fE4 = 0.0f;
                    if (data->iE0 == 1) {
                        data->i14 += 13;
                        data->i14 %= 12;
                    } else if (data->iE0 == 2) {
                        data->i18 += 13;
                        data->i18 %= 12;
                    } else {
                        data->i1C += 13;
                        data->i1C %= 12;
                    }
                }
            } else if (i == 12) {
                if (func_001F9938_2D9F08(&data->h10)) {
                    moby->state = 3;
                }
            } else if (((Pad_2D9F08 *)D_0013C940)->b1C0.l & 0xA00000000000) {
                data->h11E |= 1;
                func_0022ED80_2D9F08(0, 0, moby);
                if (((Pad_2D9F08 *)D_0013C940)->b1C0.w.hi & 0x8000) {
                    data->fE4 = -(D_0015EE6C_2D9F08 * 6.2831855f);
                } else {
                    data->fE4 = D_0015EE6C_2D9F08 * 6.2831855f;
                }
            } else if (((Pad_2D9F08 *)D_0013C940)->i1A4 & 0x1000) {
                data->h11E |= 2;
                data->iE0 = data->iE0 - 1;
                if (data->iE0 == 0) {
                    data->iE0 = 1;
                } else {
                    func_0022ED80_2D9F08(1, 0, moby);
                }
            } else if (((Pad_2D9F08 *)D_0013C940)->i1A4 & 0x4000) {
                data->h11E |= 2;
                data->iE0 = data->iE0 + 1;
                if (data->iE0 == 4) {
                    data->iE0 = 3;
                } else {
                    func_0022ED80_2D9F08(1, 0, moby);
                }
            }
        } else if (D_0013F450_2D9F08.unkA98 & 2) {
            func_L00_00232C10_2D9F08(0x58, 0, func_001F9850_2D9F08(6));
        }
        if (i != 12) {
            if (((D_0013CAE4 & 0x10) && D_L02_00179A10.i0 == 0 && D_L02_00179A10.i24 == -1)
                || ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->state.current != 0x72 || ((Hero *)(D_0013E633_2D9F08 + 0xE1D))->items[0].item_id != 0x1A) {
                moby->state = 1;
                func_L00_002EC0C8_2D9F08(1);
                request_hero_idle_state_for_vehicle_mode();
                data->h10 = func_001F9850_2D9F08(0x3C);
            }
        }
        break;
    case 3:
        if (D_0013F450_2D9F08.unkA98 & 2) {
            if (moby->mission != 0xFF) {
                ((int *)(D_0014C290_2D9F08 + (D_0015EE84_2D9F08 << 8)))[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                D_L02_001BADE0_2D9F08[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
            }
            func_L00_002EC0C8_2D9F08(1);
            func_L02_0022BE40_2D9F08(0, 1);
            moby->command = 1;
            moby->state = 4;
        }
        break;
    }
    func_L00_0025E590_2D9F08(moby, data);
}
#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
extern void func_L00_00250800_d92f0(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void func_L00_002607A8_d92f0(void *a, float x) __asm__("FUN_L00_0025f730");
extern void func_001F9BD8_d92f0(void *, void *, void *) __asm__("FUN_001f9a10");
extern float func_001FA748_d92f0(float, float) __asm__("FUN_001fa580");
extern float func_001FA790_d92f0(float, float) __asm__("FUN_001fa5c8");
extern float D_0015EE6C_d92f0 __asm__("D_0015ED6C");
void align_hero_to_tresspasser_pad(char *moby) __asm__("FUN_L02_002d92f0");

void align_hero_to_tresspasser_pad(char *moby) {
    float position_delta[4];
    char *hero_position;
    float f;
    float max_yaw_step;
    struct Hero *hero_ptr;
    func_L00_00250800_d92f0(moby, 0, position_delta);
    hero_position = (char *)&hero.motion.pos;
    fast_vec_sub(position_delta, position_delta, hero_position);
    func_L00_002607A8_d92f0(position_delta, D_0015EE6C_d92f0 * 4.0f);
    func_001F9BD8_d92f0(hero_position, hero_position, position_delta);
    f = func_001FA748_d92f0(*(float *)(moby + 0x48), 3.14159f);
    f = func_001FA790_d92f0(f, *(float *)(hero_position + 0x18));
    max_yaw_step = D_0015EE6C_d92f0 * 6.2831855f;
    if (max_yaw_step < f) {
        f = max_yaw_step;
    } else if (f < -max_yaw_step) {
        f = -max_yaw_step;
    }
    hero_ptr = &hero;
    hero_ptr->motion.rot.f[2] = func_001FA748_d92f0(f, hero_ptr->motion.rot.f[2]);
}

/* Draws an elliptical arc from start_angle to end_angle (radii radius_x, radius_y, around the screen centre) as five sprite
 * segments in the given colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002DB810), where it is exact; names translated to the US level program. */

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern int tresspasser_ring_arc_first_vertex_uv __asm__("D_L02_00161B0C") __attribute__((sda));
extern int tresspasser_ring_arc_second_vertex_uv __asm__("D_L02_00161B10") __attribute__((sda));
extern int tresspasser_ring_arc_third_vertex_uv __asm__("D_L02_00161B14") __attribute__((sda));
extern int tresspasser_ring_arc_fourth_vertex_uv __asm__("D_L02_00161B18") __attribute__((sda));
extern void FUN_L02_0020bc88(void *, void *, void *, s64, s32);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void draw_tresspasser_puzzle_ring_arc(int texture, u32 color, float start_angle, float end_angle, float radius_x, float radius_y) __asm__("FUN_L02_002da3d8");

void draw_tresspasser_puzzle_ring_arc(int texture, u32 color, float start_angle, float end_angle, float radius_x, float radius_y) {
    long corners[4];
    u32 colors[4];
    int uvs[4];
    float angle_step = fast_subtract_rotations(end_angle, start_angle) / 5.0f;
    int i;
    uvs[0] = tresspasser_ring_arc_first_vertex_uv;
    uvs[1] = tresspasser_ring_arc_second_vertex_uv;
    uvs[2] = tresspasser_ring_arc_third_vertex_uv;
    uvs[3] = tresspasser_ring_arc_fourth_vertex_uv;
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    for (i = 0; i < 5; i++) {
        float angle0 = fast_add_rotations(start_angle, angle_step * (float)i);
        float angle1 = fast_add_rotations(start_angle, angle_step * (float)(i + 1));
        corners[3] = 0xFFFFF000000000L;
        corners[2] = 0xFFFFF000000000L;
        corners[1] = 0xFFFFF000000000L;
        corners[0] = 0xFFFFF000000000L;
        corners[0] += (u32)(-(func_001FA898_r(fast_cos(angle0) * radius_x * 16.0f) << 16) - (int)0x80000000);
        corners[0] += func_001FA898_r(fast_sin(angle0) * radius_x * 16.0f) + 0x8000;
        corners[1] += (u32)(-(func_001FA898_r(fast_cos(angle0) * radius_y * 16.0f) << 16) - (int)0x80000000);
        corners[1] += func_001FA898_r(fast_sin(angle0) * radius_y * 16.0f) + 0x8000;
        corners[2] += (u32)(-(func_001FA898_r(fast_cos(angle1) * radius_x * 16.0f) << 16) - (int)0x80000000);
        corners[2] += func_001FA898_r(fast_sin(angle1) * radius_x * 16.0f) + 0x8000;
        corners[3] += (u32)(-(func_001FA898_r(fast_cos(angle1) * radius_y * 16.0f) << 16) - (int)0x80000000);
        corners[3] += func_001FA898_r(fast_sin(angle1) * radius_y * 16.0f) + 0x8000;
        FUN_L02_0020bc88(corners, uvs, colors, texture, 1);
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Draws the rotating radar arrow sprite (style sel) at angle a and scale s, twice. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002DBAF8), where it is exact; names translated to the US level program. */

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L02_001D33B0[4][4];
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern int D_L02_00161ABC __attribute__((sda));
extern int D_L02_00161AC0 __attribute__((sda));
extern int D_L02_00161AC4 __attribute__((sda));
extern int D_L02_00161AC8 __attribute__((sda));
extern int D_L02_00161ACC __attribute__((sda));
extern int D_L02_00161AD0 __attribute__((sda));
extern int D_L02_00161AD4 __attribute__((sda));
extern int D_L02_00161AD8 __attribute__((sda));
extern int D_L02_00161ADC __attribute__((sda));
extern int D_L02_00161AE0 __attribute__((sda));
extern int D_L02_00161AE4 __attribute__((sda));
extern int D_L02_00161AE8 __attribute__((sda));
extern int D_L02_00161B1C_gp __asm__("D_L02_00161B1C") __attribute__((sda));
extern int D_L02_00161B9C __attribute__((sda));
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void FUN_L02_0020bc88(void *, void *, void *, s64, s32);
extern void clear_vector(void *);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void draw_tresspasser_puzzle_arrow(int unused_texture, int unused_parameter, int sel, float angle, float radial_distance) __asm__("FUN_L02_002da6c0");

void draw_tresspasser_puzzle_arrow(int unused_texture, int unused_parameter, int sel, float angle, float radial_distance) {
    enum {
        ARROW_STYLE_NEWLY_REACHED = 0,
        ARROW_STYLE_OCCUPIED = 1,
        ARROW_STYLE_LIT = 2
    };
    long xy[4];
    u32 col[4];
    int uv[4];
    float M[4][4];
    float arrow_rotation_angles[4];
    int x, y;
    long packed_arrow_center;
    s64 texture_tex0;
    switch (sel) {
    case ARROW_STYLE_NEWLY_REACHED:
        uv[0] = D_L02_00161ABC;
        uv[1] = D_L02_00161AC0;
        uv[2] = D_L02_00161AC4;
        uv[3] = D_L02_00161AC8;
        break;
    case ARROW_STYLE_OCCUPIED:
        uv[0] = D_L02_00161ACC;
        uv[1] = D_L02_00161AD0;
        uv[2] = D_L02_00161AD4;
        uv[3] = D_L02_00161AD8;
        break;
    case ARROW_STYLE_LIT:
    default:
        uv[0] = D_L02_00161ADC;
        uv[1] = D_L02_00161AE0;
        uv[2] = D_L02_00161AE4;
        uv[3] = D_L02_00161AE8;
        break;
    }
    col[3] = D_L02_00161B9C;
    col[2] = D_L02_00161B9C;
    col[1] = D_L02_00161B9C;
    col[0] = D_L02_00161B9C;
    y = func_001FA898_r(fast_sin(angle) * radial_distance * 16.0f);
    x = func_001FA898_r(fast_cos(angle) * radial_distance * 16.0f);
    packed_arrow_center = (long)y + 0xFFFFF080000000L;
    packed_arrow_center -= x << 16;
    packed_arrow_center += 0x8000;
    xy[3] = packed_arrow_center;
    xy[2] = packed_arrow_center;
    xy[1] = packed_arrow_center;
    xy[0] = packed_arrow_center;
    clear_vector(arrow_rotation_angles);
    arrow_rotation_angles[2] = -angle;
    euler_rotation_matrix(M, arrow_rotation_angles);
    multiply_matrix(M, M, D_L02_001D33B0);
    xy[0] += func_001FA898_r(M[0][0] * 16.0f) << 16;
    xy[0] += func_001FA898_r(M[0][1] * 16.0f);
    xy[1] += func_001FA898_r(M[1][0] * 16.0f) << 16;
    xy[1] += func_001FA898_r(M[1][1] * 16.0f);
    xy[2] += func_001FA898_r(M[2][0] * 16.0f) << 16;
    xy[2] += func_001FA898_r(M[2][1] * 16.0f);
    xy[3] += func_001FA898_r(M[3][0] * 16.0f) << 16;
    xy[3] += func_001FA898_r(M[3][1] * 16.0f);
    texture_tex0 = get_effect_texture(D_L02_00161B1C_gp + 0x29);
    FUN_L02_0020bc88(xy, uv, col, texture_tex0, 1);
    FUN_L02_0020bc88(xy, uv, col, texture_tex0, 1);
}
extern s32 D_L02_0015F5CC;
extern f32 D_L02_00161B60 __attribute__((sda));
extern char *D_L02_00161AEC __attribute__((sda));
extern char *D_L02_00161AF0 __attribute__((sda));
extern char *D_L02_00161AF4 __attribute__((sda));
extern char *D_L02_00161AF8 __attribute__((sda));
extern char *D_L02_00161AFC __attribute__((sda));
extern char *D_L02_00161B00 __attribute__((sda));
extern char *D_L02_00161B04 __attribute__((sda));
extern char *D_L02_00161B08 __attribute__((sda));
extern s32 D_L02_00161B64 __attribute__((sda));

extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 FUN_001f44b8(s32);
extern void FUN_L02_0020bc88_u(void *, void *, void *, s32, s32) __asm__("FUN_L02_0020bc88");

typedef unsigned long long u64_t;

#define NEG20(dst, e)                                                                              \
    {                                                                                              \
        s32 q_ = (e);                                                                              \
        q_ = q_ << 20;                                                                             \
        (dst) += (u64_t)(u32)(-q_ + 0x80000000u);                                                  \
    }
#define LO4(dst, e)                                                                                \
    {                                                                                              \
        s32 q_ = (e);                                                                              \
        q_ = (q_ << 4) + 0x8000;                                                                   \
        (dst) += (u64_t)q_;                                                                        \
    }

void draw_tresspasser_puzzle_radial_strip(void *unused, f32 angle, f32 start_distance, f32 end_distance, s32 color) __asm__("FUN_L02_002da938");

void draw_tresspasser_puzzle_radial_strip(void *unused, f32 angle, f32 start_distance, f32 end_distance, s32 color) {
    u64_t packed_vertices[4];
    s32 vertex_colors[4];
    s32 vertex_uvs[4];
    s32 unwrapped_uv_scroll;
    s32 wrapped_uv_scroll_offset;
    s32 strip_texture_tex0;
    f32 f24, f25, f26, f27, f28, f29, f30, f31;
    f32 trig_value;

    unwrapped_uv_scroll = truncate_float_to_s32((f32)D_L02_0015F5CC * D_L02_00161B60);
    wrapped_uv_scroll_offset = (unwrapped_uv_scroll % 512) << 16;
    vertex_colors[3] = color;
    vertex_colors[2] = color;
    vertex_colors[1] = color;
    vertex_colors[0] = color;
    vertex_uvs[0] = (s32)(D_L02_00161AEC + wrapped_uv_scroll_offset);
    vertex_uvs[1] = (s32)(D_L02_00161AF0 + wrapped_uv_scroll_offset);
    vertex_uvs[2] = (s32)(D_L02_00161AF4 + wrapped_uv_scroll_offset);
    vertex_uvs[3] = (s32)(D_L02_00161AF8 + wrapped_uv_scroll_offset);

    f25 = fast_sin(angle) * start_distance;
    f24 = fast_cos(angle) * start_distance;
    f28 = fast_sin(angle) * end_distance;
    f27 = fast_cos(angle) * end_distance;
    trig_value = fast_cos(angle);
    f29 = f25 + trig_value * 2.0f;
    trig_value = fast_cos(angle);
    f25 = f25 - trig_value * 2.0f;
    trig_value = fast_sin(angle);
    f26 = f24 - trig_value * 2.0f;
    trig_value = fast_sin(angle);
    f24 = f24 + trig_value * 2.0f;
    trig_value = fast_cos(angle);
    f31 = f28 + trig_value * 2.0f;
    trig_value = fast_cos(angle);
    f28 = f28 - trig_value * 2.0f;
    trig_value = fast_sin(angle);
    f30 = f27 - trig_value * 2.0f;
    trig_value = fast_sin(angle);
    f27 = f27 + trig_value * 2.0f;

    packed_vertices[3] = 0xFFFFF000000000ULL;
    packed_vertices[2] = 0xFFFFF000000000ULL;
    packed_vertices[1] = 0xFFFFF000000000ULL;
    packed_vertices[0] = 0xFFFFF000000000ULL;

    NEG20(packed_vertices[0], truncate_float_to_s32(f26));
    LO4(packed_vertices[0], truncate_float_to_s32(f29));
    NEG20(packed_vertices[1], truncate_float_to_s32(f24));
    LO4(packed_vertices[1], truncate_float_to_s32(f25));
    NEG20(packed_vertices[2], truncate_float_to_s32(f30));
    LO4(packed_vertices[2], truncate_float_to_s32(f31));
    NEG20(packed_vertices[3], truncate_float_to_s32(f27));
    LO4(packed_vertices[3], truncate_float_to_s32(f28));

    strip_texture_tex0 = FUN_001f44b8(D_L02_00161B64);
    FUN_L02_0020bc88_u(packed_vertices, vertex_uvs, vertex_colors, strip_texture_tex0, 0);

    vertex_uvs[0] = (s32)D_L02_00161AFC;
    vertex_uvs[1] = (s32)D_L02_00161B00;
    vertex_uvs[2] = (s32)D_L02_00161B04;
    vertex_uvs[3] = (s32)D_L02_00161B08;

    f25 = fast_sin(angle) * start_distance;
    f24 = fast_cos(angle) * start_distance;
    f28 = fast_sin(angle) * end_distance;
    f27 = fast_cos(angle) * end_distance;
    trig_value = fast_cos(angle);
    f29 = f25 + trig_value * 5.0f;
    trig_value = fast_cos(angle);
    f25 = f25 - trig_value * 5.0f;
    trig_value = fast_sin(angle);
    f26 = f24 - trig_value * 5.0f;
    trig_value = fast_sin(angle);
    f24 = f24 + trig_value * 5.0f;
    trig_value = fast_cos(angle);
    f31 = f28 + trig_value * 5.0f;
    trig_value = fast_cos(angle);
    f28 = f28 - trig_value * 5.0f;
    trig_value = fast_sin(angle);
    f30 = f27 - trig_value * 5.0f;
    trig_value = fast_sin(angle);
    f27 = f27 + trig_value * 5.0f;

    packed_vertices[3] = 0xFFFFF000000000ULL;
    packed_vertices[2] = 0xFFFFF000000000ULL;
    packed_vertices[1] = 0xFFFFF000000000ULL;
    packed_vertices[0] = 0xFFFFF000000000ULL;

    NEG20(packed_vertices[0], truncate_float_to_s32(f26));
    LO4(packed_vertices[0], truncate_float_to_s32(f29));
    NEG20(packed_vertices[1], truncate_float_to_s32(f24));
    LO4(packed_vertices[1], truncate_float_to_s32(f25));
    NEG20(packed_vertices[2], truncate_float_to_s32(f30));
    LO4(packed_vertices[2], truncate_float_to_s32(f31));
    NEG20(packed_vertices[3], truncate_float_to_s32(f27));
    LO4(packed_vertices[3], truncate_float_to_s32(f28));

    FUN_L02_0020bc88_u(packed_vertices, vertex_uvs, vertex_colors, strip_texture_tex0, 0);
}

typedef struct { long a, b, c, d; } Q_2dae88;
extern int D_L02_00161B1C_gp __asm__("D_L02_00161B1C") __attribute__((sda));
int tex_2dae88(int) __asm__("FUN_001f44b8");
void f20bc88_2dae88(Q_2dae88 *, int *, int *, int, int) __asm__("FUN_L02_0020bc88");
extern Q_2dae88 D_L02_001D3310[];
extern int tresspasser_textured_quad_color __asm__("D_L02_00161B94") __attribute__((sda));
extern int tresspasser_quad_first_vertex_uv __asm__("D_L02_00161AAC") __attribute__((sda));
extern int tresspasser_quad_second_vertex_uv __asm__("D_L02_00161AB0") __attribute__((sda));
extern int tresspasser_quad_third_vertex_uv __asm__("D_L02_00161AB4") __attribute__((sda));
extern int tresspasser_quad_fourth_vertex_uv __asm__("D_L02_00161AB8") __attribute__((sda));
void draw_tresspasser_puzzle_textured_quads(void) __asm__("FUN_L02_002dae88");

void draw_tresspasser_puzzle_textured_quads(void) {
    Q_2dae88 packed_vertices;
    int vertex_colors[4];
    int vertex_uvs[4];
    Q_2dae88 *vertex_quad_cursor = D_L02_001D3310;
    int i;
    for (i = 3; i >= 0; i--) {
        vertex_uvs[0] = tresspasser_quad_first_vertex_uv;
        vertex_uvs[1] = tresspasser_quad_second_vertex_uv;
        vertex_uvs[2] = tresspasser_quad_third_vertex_uv;
        vertex_uvs[3] = tresspasser_quad_fourth_vertex_uv;
        vertex_colors[0] = vertex_colors[1] = vertex_colors[2] = vertex_colors[3] = tresspasser_textured_quad_color;
        packed_vertices = *vertex_quad_cursor++;
        f20bc88_2dae88(&packed_vertices, vertex_uvs, vertex_colors, tex_2dae88(D_L02_00161B1C_gp + 0x28), 1);
    }
}

extern int escalator_direction_timer_frames __asm__("D_L02_00161C18") __attribute__((sda));
extern float escalator_target_speed __asm__("D_L02_00161C14") __attribute__((sda));
extern float escalator_acceleration __asm__("D_L02_00161C1C") __attribute__((sda));
extern float D_0015ED70_2dc6b0 __asm__("D_0015ED70");
extern void vec_clear_2dc6b0(void *) __asm__("FUN_001f99f8");
extern int scale_game_frames_2dc6b0(int) __asm__("FUN_001f96f8");
extern int count_down_2dc6b0(int *) __asm__("FUN_001f9740");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern float fast_cos_2dc6b0(float) __asm__("FUN_001f9dc8");
extern float fast_sin_2dc6b0(float) __asm__("FUN_001f9de0");
extern void FUN_L00_00260738_2dc6b0(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

void escalator_update(char *moby) __asm__("FUN_L02_002dc6b0");
void escalator_update(char *moby) {
    char *data = *(char **)(moby + 0x78);
    float limit;
    float y;
    int state;

    if (data == 0) return;
    state = *(u8 *)(moby + 0x20);
    if (state == 1) goto state1;
    if (state < 2) {
        if (state == 0) goto state0;
        goto move;
    }
    if (state == 2) goto state2;
    if (state == 3) goto state3;
    goto move;
state0:
        vec_clear_2dc6b0(data + 0x40);
        *(int *)(data + 0xa0) = scale_game_frames_2dc6b0(escalator_direction_timer_frames);
        *(u32 *)(data + 0x9c) |= 4;
        if (*(int *)(data + 0xa4) != 0) *(u8 *)(moby + 0x20) = 3;
        else *(u8 *)(moby + 0x20) = 1;
        goto move;
state1: {
        float speed;

        *(float *)(moby + 0x58) = 1.0f;
        if (count_down_2dc6b0((int *)(data + 0xa0))) {
            speed = fast_vec_length(data + 0x40) - escalator_acceleration * D_0015ED70_2dc6b0;
            if (speed <= 0.0f) {
                *(int *)(data + 0xa0) = scale_game_frames_2dc6b0(escalator_direction_timer_frames);
                *(u8 *)(moby + 0x20) = 2;
            } else {
                *(float *)(data + 0x40) = fast_cos_2dc6b0(*(float *)(moby + 0x48)) * -speed;
                y = fast_sin_2dc6b0(*(float *)(moby + 0x48)) * -speed;
                *(float *)(data + 0x48) = 0.0f;
                *(float *)(data + 0x44) = y;
            }
        } else {
            speed = fast_vec_length(data + 0x40);
            limit = absolute_float(escalator_target_speed * frame_time);
            /* Retail loads frame_time_sq through its absolute EE address here. */
            if (speed < limit) speed += escalator_acceleration * (*(float *)0x0015ED70);
            *(float *)(data + 0x40) = fast_cos_2dc6b0(*(float *)(moby + 0x48)) * -speed;
            *(float *)(data + 0x44) = fast_sin_2dc6b0(*(float *)(moby + 0x48)) * -speed;
            *(int *)(data + 0x48) = 0;
        }
        *(float *)(moby + 0x58) = speed / (escalator_target_speed * frame_time);
        goto move;
}
state2: {
        float speed;

        *(float *)(moby + 0x58) = -1.0f;
        if (count_down_2dc6b0((int *)(data + 0xa0))) {
            speed = fast_vec_length(data + 0x40) - escalator_acceleration * D_0015ED70_2dc6b0;
            if (speed <= 0.0f) {
                *(int *)(data + 0xa0) = scale_game_frames_2dc6b0(escalator_direction_timer_frames);
                *(u8 *)(moby + 0x20) = 1;
            } else {
                *(float *)(data + 0x40) = fast_cos_2dc6b0(*(float *)(moby + 0x48)) * speed;
                *(float *)(data + 0x44) = fast_sin_2dc6b0(*(float *)(moby + 0x48)) * speed;
                *(float *)(data + 0x48) = 0.0f;
            }
        } else {
            speed = fast_vec_length(data + 0x40);
            limit = absolute_float(escalator_target_speed * frame_time);
            if (speed < limit) speed += escalator_acceleration * (*(float *)0x0015ED70);
            *(float *)(data + 0x40) = fast_cos_2dc6b0(*(float *)(moby + 0x48)) * speed;
            *(float *)(data + 0x44) = fast_sin_2dc6b0(*(float *)(moby + 0x48)) * speed;
            *(int *)(data + 0x48) = 0;
        }
        *(float *)(moby + 0x58) = speed / -(escalator_target_speed * frame_time);
        goto move;
}
state3:
        *(float *)(data + 0x40) = fast_cos_2dc6b0(*(float *)(moby + 0x48)) * -(escalator_target_speed * frame_time);
        *(float *)(data + 0x44) = fast_sin_2dc6b0(*(float *)(moby + 0x48)) * -(escalator_target_speed * frame_time);
        *(int *)(data + 0x48) = 0;
move:
    FUN_L00_00260738_2dc6b0(data + 0x60, data + 0x40, moby + 0x40, moby + 0x40);
}

#define NOT_SDA

#define MACRO_ADDR

/* Hideable flower: appears when its slot is free, faces and sparkles at the hero (class 0x323), hides again
 * when its slot is used. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002E1888), where it is exact; names translated to the US level program. */

extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern unsigned char D_0014C050_a[] __asm__("D_0014C050");
extern int D_L02_0015F5CC; /* no foreign declaration */
extern int D_L02_0015FFD8; /* no foreign declaration */
extern int approach_angle() __asm__("FUN_L00_0025be00");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern float versatarget_particle_base_lifetime __asm__("D_L02_00161CB0") __attribute__((sda));
extern int versatarget_particle_color __asm__("D_L02_00161CB4") __attribute__((sda));
extern float versatarget_particle_scale __asm__("D_L02_00161CB8") __attribute__((sda));
extern float D_L02_00161CBC;
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void spawn_moving_hero_relative_alpha_fade_particle(void *, void *, int, int, int, int, float) __asm__("FUN_L00_002730e0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
void initialize_camera_offset_effect_state(void) __asm__("FUN_L02_002fa6d8");
void try_select_camera_offset_effect_moby(char *m) __asm__("FUN_L02_002fa7c8");
extern unsigned char D_0014171B_x[] __asm__("D_0015ED84");
extern void func_L02_002FBB10_c(char *) __asm__("FUN_L02_002fa6d8");
extern void try_select_camera_offset_effect_moby(char *) __asm__("FUN_L02_002fa7c8");
extern float func_L00_0025CE58_f(float *, float, float *, float, float,
                                 float) __asm__("FUN_L00_0025be00");

void swingshot_versatarget_update(char *m) __asm__("FUN_L02_002e0450");
void swingshot_versatarget_update(char *moby) {
    enum {
        VERSATARGET_STATE_INITIALIZE = 0,
        VERSATARGET_STATE_ACTIVE = 1,
        VERSATARGET_STATE_INACTIVE = 2
    };
    char *moby_data = *(char **)(moby + 0x78);
    float particle_position[4];
    float particle_velocity[4];
    char *linked_moby;
    switch (((unsigned char *)moby)[0x20]) {
    case VERSATARGET_STATE_INITIALIZE:
        if (*(int *)(moby_data + 0x30) != -1 ||
            (*(int *)(moby_data + 0x3C) != -1 &&
             (linked_moby = (char *)(D_L02_0015FFD8 + (*(int *)(moby_data + 0x3C) << 8))) != 0 &&
             ((unsigned char *)linked_moby)[0x20] != 0xFE && ((unsigned char *)linked_moby)[0x20] != 0xFD)) {
            ((unsigned char *)moby)[0x30] = 0xFF;
            moby[0x20] = 2;
            *(unsigned short *)(moby + 0x34) |= 0x41;
        } else {
            set_moby_color(moby, 0xC0, 0xC0, 0xC0);
            func_L02_002FBB10_c(moby);
            moby[0x20] = 1;
        }
        break;
    case VERSATARGET_STATE_ACTIVE:
        if (*(short *)(moby + 0xA6) == 0x323) {
            struct Hero *hero_ptr = &hero;
            float *rz = (float *)(moby + 0x48);
            float yaw = fast_arc_tan(hero_ptr->motion.pos.f[0] - *(float *)(moby + 0x10),
                                     hero_ptr->motion.pos.f[1] - *(float *)(moby + 0x14));
            func_L00_0025CE58_f(rz, yaw, (float *)(moby_data + 0x38), frame_time_sq * 6.2831855f,
                                frame_time_sq * 6.2831855f, frame_time * 12.566371f);
            if (D_L02_0015F5CC & 1) {
                if (((unsigned char *)moby)[0x31] != 0) {
                    float direction_sign = 1.0f;
                    int particle_lifetime;
                    if (D_L02_0015F5CC & 4)
                        direction_sign = -1.0f;
                    normalize_vector_xyz(particle_position, moby + 0xD0, direction_sign * D_L02_00161CBC);
                    normalize_vector_xyz(particle_velocity, particle_position, -2.0f / multiply_global_factor_ed64(versatarget_particle_base_lifetime));
                    add_vector_xyz(particle_position, particle_position, moby + 0x10);
                    particle_lifetime = truncate_float_to_s32(multiply_global_factor_ed64(versatarget_particle_base_lifetime)) & 0xFF;
                    spawn_moving_hero_relative_alpha_fade_particle(particle_position, particle_velocity, versatarget_particle_color, particle_lifetime,
                                     random_integer_below(0xFF) & 0xFF, 0,
                                     versatarget_particle_scale);
                }
            }
        }
        {
            struct Hero *hero_reference = &hero;
            if (hero_reference->state.current != 0x2C)
                try_select_camera_offset_effect_moby(moby);
        }
        break;
    case VERSATARGET_STATE_INACTIVE:
        if (D_0014C050_a[*(int *)(moby_data + 0x30) + (current_level_index << 4)] != 0 ||
            (*(int *)(moby_data + 0x3C) != -1 &&
             ((linked_moby = (char *)(D_L02_0015FFD8 + (*(int *)(moby_data + 0x3C) << 8))) == 0 ||
              ((unsigned char *)linked_moby)[0x20] == 0xFE || ((unsigned char *)linked_moby)[0x20] == 0xFD))) {
            *(unsigned short *)(moby + 0x34) &= 0xFFBE;
            *(int *)(moby_data + 0x30) = -1;
            moby[0x20] = 0;
        }
        break;
    }
}
/* Re-arms the matching entry of the moby list when the level mode is 2. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002E2110), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x44];
    short count;
    char pad46[0x132];
    char *mobys[1];
} Level02MobyList;

extern int level_game_mode __asm__("D_L02_0015F5C4");
#include "rnc/gameplay/state/options.h"
extern unsigned char D_L02_0017CA40[];
void attach_manipulator(void *arg0, s32 arg1, void *arg2) __asm__("FUN_0020cb10");

extern Level02MobyList talk_camera __asm__("D_L02_0016CE60");
void attach_big_head_cheat_manipulator_to_matching_moby(char *moby) __asm__("FUN_L02_002e0cd8");

void attach_big_head_cheat_manipulator_to_matching_moby(char *moby) {
    int i;
    if (level_game_mode == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < talk_camera.count; i++) {
            char *candidate_moby = talk_camera.mobys[i];
            if (*(short *)(candidate_moby + 0xA6) == *(short *)(moby + 0xA6)) {
                unsigned char *manipulator = D_L02_0017CA40;
                if (manipulator[1] == 0) {
                    attach_manipulator(candidate_moby, 0, manipulator);
                    *(float *)(manipulator + 0x20) = 2.75f;
                    *(float *)(manipulator + 0x24) = 2.75f;
                    *(float *)(manipulator + 0x28) = 2.75f;
                }
            }
        }
    }
}
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L02_002ea7d0_c(struct Moby *) __asm__("FUN_L02_002ea7d0");
extern void FUN_L02_002ea8b0_c(struct Moby *) __asm__("FUN_L02_002ea8b0");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float ea210_addrot(float, float) __asm__("FUN_001fa580");
extern float ea210_dist(void *, void *) __asm__("FUN_001f9b80");
extern float ea210_abs(float) __asm__("FUN_001f99c0");
extern void ea210_sound(int) __asm__("FUN_L00_00298840");
extern void grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void ea210_play(int, int) __asm__("FUN_L00_00263d40");
extern void clear_active_help_record(void) __asm__("FUN_L00_002039a0");
extern void ea210_timing(int, int) __asm__("FUN_L00_00203908");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void ea210_approach(float, float, float, float, void *, void *) __asm__("FUN_00213f38");
extern void ea210_transform(float, void *, void *, void *) __asm__("FUN_001f9a40");
extern float ea210_interp(float, float) __asm__("FUN_001fa5c8");
extern float ea210_clamp(float, float) __asm__("FUN_001fa580");
extern void ea210_draw(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002ea9d8_c(void *) __asm__("FUN_L00_002ea9d8");
extern void FUN_L00_002eaa30_c(void *) __asm__("FUN_L00_002eaa30");
extern float ea210_fade(float, float, void *) __asm__("FUN_00213ed8");
extern void FUN_L00_002eac18(int);
extern void ea210_audio(int, int) __asm__("FUN_0020b178");
extern void ea210_destroy(struct Moby *) __asm__("FUN_0020c828");
extern int D_L02_0015F5CC;
extern unsigned char D_001413F4 __attribute__((section(".data")));
extern int D_L02_0015F640 __attribute__((sda));
extern unsigned char items_acquired[] __asm__("D_0013D4E8");
extern char D_0013F3D0[];
extern float D_L02_00174468 __attribute__((section(".data")));
extern int D_L02_00167400 __attribute__((section(".data")));
extern float D_0015ED70;
extern float D_0015ED6C;
extern float D_L02_0015F3FC;

void tresspasser_update(struct Moby *moby) __asm__("FUN_L02_002ea210");
void tresspasser_update(struct Moby *moby) {
    char *data;
    int which;
    int frames;
    float angle;
    float ground_offset;
    data = (char *)moby->pvars;
    frames = scale_game_frames(60);
    angle = 3.14159274f / (float)frames;
    which = moby->oclass == 0x3ED ? 0x1A : 0x16;
    switch (moby->state) {
    case 0:
        FUN_L02_002ea7d0_c(moby);
        ground_offset = 0.2f;
        if (items_acquired[which] == 0) {
            probe_ground_height(&moby->pos, 0, 0.5f);
            *(float *)((char *)moby + 0x40) = 1.57079637f;
            *(float *)((char *)moby + 0x44) = 3.14159274f;
            moby->pos.z = D_L02_00174468 + ground_offset;
            moby->state = 1;
            return;
        }
        ea210_destroy(moby);
        return;
    case 1:
        moby->rot.z = ea210_clamp(moby->rot.z, angle);
        FUN_L02_002ea8b0_c(moby);
        if (D_L02_0015F5CC % 10 != 0) return;
        if (!(ea210_dist(&moby->pos, D_0013F3D0) < 1.0f)) return;
        if (ea210_abs(moby->pos.z - *(float *)(D_0013F3D0 + 8)) < 2.0f) {
            moby->state = 3;
            moby->flags |= 1;
            moby->visible = 0;
            moby->collision = 0;
            if (moby->oclass == 0x3ED) {
                ea210_sound(4);
            } else {
                ea210_sound(2);
            }
        }
        return;
    case 3:
        if (level_game_mode == 2) return;
        if (D_001413F4 != 0) {
            if (which == 0x16) {
                grant_inventory_item(0x16, 0);
                D_L02_0015F640 = 0;
            } else {
                grant_inventory_item(which, 0);
            }
        } else {
            grant_inventory_item(which, 1);
        }
        if (moby->oclass == 0x3ED) {
            ea210_play(0x7DF, -1);
            clear_active_help_record();
            ea210_timing(0x7D9, 0x13);
            D_L02_0015F3FC = 1.0f;
            moby->state = 4;
            FUN_L00_00216f90(D_0013F3D0, D_0013F3D0 + 16, 0x72, 1);
            return;
        }
        ea210_audio(0, -1);
        ea210_destroy(moby);
        return;
    case 4: {
        float a[4] __attribute__((aligned(16))), b[4] __attribute__((aligned(16))), c[4] __attribute__((aligned(16))), d[4] __attribute__((aligned(16)));
        float position[4] __attribute__((aligned(16)));
        float output[4];
        /* Retail clears all four lanes before assigning the XYZ components. */
        *(u128 *)a = 0;
        *(u128 *)b = 0;
        *(u128 *)c = 0;
        *(u128 *)d = 0;
        a[0] = 91.2f; a[1] = 270.21f; a[2] = 64.85f;
        b[0] = 106.98f; b[1] = 273.46f; b[2] = 62.65f;
        c[0] = 0.07f; c[1] = 0.33f; c[2] = 0.55f;
        d[0] = 0.07f; d[1] = 0.43f; d[2] = 0.72f;
        ea210_approach(1.0f, D_0015ED70 * 0.15f, D_0015ED70 * 0.15f,
                       D_0015ED6C * 0.15f, data + 0x40, data + 0x44);
        ea210_transform(*(float *)(data + 0x40), position, (void *)a, (void *)b);
        output[2] = ea210_clamp(ea210_interp(d[2], c[2]) * *(float *)(data + 0x40), c[2]);
        output[1] = ea210_clamp(ea210_interp(d[1], c[1]) * *(float *)(data + 0x40), c[1]);
        output[0] = ea210_clamp(ea210_interp(d[0], c[0]) * *(float *)(data + 0x40), c[0]);
        if (D_L02_00167400 && *(short *)(D_L02_00167400 + 0x86) != 5)
            ea210_draw(position, output, 1, 0, 0);
        FUN_L00_002ea9d8_c(position);
        FUN_L00_002eaa30_c(output);
        if (D_L02_00179A10.i0 == 0 && D_L02_00179A10.i24 == -1 &&
            0.2f < *(float *)(data + 0x40)) {
            ea210_fade(1.0f, frame_time * 4.0f, &D_L02_0015F3FC);
        } else {
            ea210_fade(0.0f, D_0015ED6C * 4.0f, &D_L02_0015F3FC);
        }
        if (D_L02_0015F3FC != 1.0f) return;
        FUN_L00_00216f90(D_0013F3D0, D_0013F3D0 + 16, 0, 0);
        FUN_L00_002eac18(0);
        moby->state = 5;
        return;
    }
    case 5:
        ea210_fade(0.0f, D_0015ED6C * 4.0f, &D_L02_0015F3FC);
        if (D_L02_0015F3FC == 0.0f) {
            ea210_audio(0, -1);
            ea210_destroy(moby);
        }
        return;
    }
}
/* Initialises a moby's data block: four random floats, fixed constants, four random ints. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002EBC08), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

void initialize_collectible_sprite_state(char *moby) __asm__("FUN_L02_002ea7d0");

void initialize_collectible_sprite_state(char *moby) {
    float *moby_data = *(float **)(moby + 0x78);
    int *sprite_timers;
    int i;

    for (i = 0; i < 4; i++) {
        moby_data[i] = (float)random_integer_below(0xFF);
    }
    moby_data[4] = -1.0f;
    moby_data[5] = -2.25f;
    moby_data[6] = 1.25f;
    moby_data[7] = 2.5f;
    sprite_timers = (int *)(moby_data + 8);
    for (i = 0; i < 4; i++) {
        sprite_timers[i] = scale_game_frames(0x3F + i * 0x40);
    }
    moby_data[12] = 2.5f;
    moby_data[13] = 3.0f;
    moby_data[15] = 2.5f;
    moby_data[14] = 3.0f;
}
#include "sda.h"
typedef struct {
    float a[4];
    float b[4];
    int c[4];
    float d[4];
} L02SpriteData_ea8b0;
extern void func_001F9BF0_ea8b0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_ea8b0(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001F9908_ea8b0(int *) __asm__("FUN_001f9740");
extern int func_001F9850_ea8b0(int) __asm__("FUN_001f96f8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int func_001FA8A8_2ebce8_ea8b0(int, int, float) __asm__("FUN_001fa6e0");
extern float func_001F9B88_ea8b0(float) __asm__("FUN_001f99c0");
extern unsigned char *func_L00_00273E08_ea8b0(void *, int, unsigned char, int, int, int, int,
                                              float) __asm__("FUN_L00_00272f68");
extern int func_001FA898_2ebce8_ea8b0(float) __asm__("FUN_001fa6d0");
extern char D_L02_00167440_ea8b0[] __asm__("D_L02_001673C0");
void update_collectible_sprite_effect(char *moby) __asm__("FUN_L02_002ea8b0");

void update_collectible_sprite_effect(char *moby) {
    L02SpriteData_ea8b0 *sprite_data = *(L02SpriteData_ea8b0 **)(moby + 0x78);
    float d[4] __attribute__((aligned(16)));
    float e[4] __attribute__((aligned(16)));
    int i, col;
    float timer_elapsed_fraction;

    *(float *)(moby + 0x18) += 0.5f;
    func_001F9BF0_ea8b0(d, D_L02_00167440_ea8b0, moby + 0x10);
    fast_vec_normalize(d, d, -0.3f);
    fast_vec_normalize(e, d, 0.1f);
    func_001F9BD8_ea8b0(d, d, moby + 0x10);
    *(float *)(moby + 0x18) -= 0.5f;
    for (i = 0; i < 4; i++) {
        float unwrapped_sprite_value = sprite_data->a[i] + sprite_data->b[i];
        sprite_data->a[i] = unwrapped_sprite_value;
        if (255.0f <= unwrapped_sprite_value) {
            sprite_data->a[i] = unwrapped_sprite_value - 255.0f;
        } else if (unwrapped_sprite_value <= 0.0f) {
            sprite_data->a[i] = unwrapped_sprite_value + 255.0f;
        }
        if (func_001F9908_ea8b0(&sprite_data->c[i])) {
            sprite_data->c[i] = func_001F9850_ea8b0(0xFF);
        }
        timer_elapsed_fraction = convert_integer_to_float(func_001F9850_ea8b0(0xFF) - sprite_data->c[i]);
        timer_elapsed_fraction = timer_elapsed_fraction / (float)func_001F9850_ea8b0(0xFF);
        col = func_001FA8A8_2ebce8_ea8b0(0x4040FFFF, 0x1040FFFF, func_001F9B88_ea8b0(0.5f - timer_elapsed_fraction));
        func_L00_00273E08_ea8b0(d, col, func_001FA898_2ebce8_ea8b0(sprite_data->a[i]) & 0xFF, 0x35, 1, 2, 0,
                                sprite_data->d[i]);
        func_001F9BD8_ea8b0(d, d, e);
    }
}

s32 D_L03_001CB4C4[1] __attribute__((section(".data"))) = {0};
s32 D_L03_001CB4C8[1] __attribute__((section(".data"))) = {0};
s32 D_L03_001CB4CC[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0017C5E0[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0017C5E4[1] __attribute__((section(".data"))) = {0};
f32 D_L04_001D3010[1] __attribute__((section(".data"))) = {0};
f32 D_L04_001D3020[1] __attribute__((section(".data"))) = {0};
f32 D_L04_001D3030[1] __attribute__((section(".data"))) = {0};
f32 D_L04_001D3040[1] __attribute__((section(".data"))) = {0};
s32 D_L05_001CB844[1] __attribute__((section(".data"))) = {0};
s32 D_L05_001CB848[1] __attribute__((section(".data"))) = {0};
s32 D_L05_001CB84C[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D4AC0[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D4AD0[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D4AE0[1] __attribute__((section(".data"))) = {0};
f32 D_L06_001D4AF0[1] __attribute__((section(".data"))) = {0};
s32 D_L07_001CB444[1] __attribute__((section(".data"))) = {0};
s32 D_L07_001CB448[1] __attribute__((section(".data"))) = {0};
s32 D_L07_001CB44C[1] __attribute__((section(".data"))) = {0};
s32 D_L08_001CBBC4[1] __attribute__((section(".data"))) = {0};
s32 D_L08_001CBBC8[1] __attribute__((section(".data"))) = {0};
s32 D_L08_001CBBCC[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001E93A0[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001E93B0[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001E93C0[1] __attribute__((section(".data"))) = {0};
f32 D_L08_001E93D0[1] __attribute__((section(".data"))) = {0};
s32 D_L09_001CB544[1] __attribute__((section(".data"))) = {0};
s32 D_L09_001CB548[1] __attribute__((section(".data"))) = {0};
s32 D_L09_001CB54C[1] __attribute__((section(".data"))) = {0};
f32 D_L11_001D3750[1] __attribute__((section(".data"))) = {0};
f32 D_L11_001D3760[1] __attribute__((section(".data"))) = {0};
f32 D_L11_001D3770[1] __attribute__((section(".data"))) = {0};
f32 D_L11_001D3780[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001D3780[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001D3790[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001D37A0[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001D37B0[1] __attribute__((section(".data"))) = {0};
f32 D_L14_0017CB60[1] __attribute__((section(".data"))) = {0};
f32 D_L14_0017CB64[1] __attribute__((section(".data"))) = {0};
s32 D_L14_001CBAC4[1] __attribute__((section(".data"))) = {0};
s32 D_L14_001CBAC8[1] __attribute__((section(".data"))) = {0};
s32 D_L14_001CBACC[1] __attribute__((section(".data"))) = {0};
f32 D_L18_001D3770[1] __attribute__((section(".data"))) = {0};
f32 D_L18_001D3780[1] __attribute__((section(".data"))) = {0};
f32 D_L18_001D3790[1] __attribute__((section(".data"))) = {0};
f32 D_L18_001D37A0[1] __attribute__((section(".data"))) = {0};
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002DA820), where it is exact; names translated to the US level program. */

/* One ring of 12 slots: 0 empty, 1 lit, 2 occupied, 3 just reached. */
typedef struct { int slot[12]; } Ring12;

extern s32 fast_tween_color(s32, s32, f32) __asm__("func_001FA6E0");
extern int D_L02_00161B30 __attribute__((sda));
extern int D_L02_00161B34 __attribute__((sda));
extern int D_L02_00161B38 __attribute__((sda));
extern int D_L02_00161B3C __attribute__((sda));
extern int D_L02_00161B40 __attribute__((sda));
extern int D_L02_00161B44 __attribute__((sda));
extern int D_L02_00161B48 __attribute__((sda));
extern int D_L02_00161B4C __attribute__((sda));
extern int D_L02_00161B50 __attribute__((sda));
extern int D_L02_00161B54 __attribute__((sda));
extern int D_L02_00161B58 __attribute__((sda));
extern int D_L02_00161B5C __attribute__((sda));
extern int D_L02_00161BA0 __attribute__((sda));
extern int D_L02_00161BA4 __attribute__((sda));
extern int D_L02_00161BA8 __attribute__((sda));
extern int D_L02_00161BAC __attribute__((sda));
extern int D_L02_00161BB0 __attribute__((sda));
extern int D_L02_00161BB4 __attribute__((sda));
extern int D_L02_00161BBC __attribute__((sda));
extern int D_L02_00161BC0 __attribute__((sda));
extern int D_L02_00161BC4 __attribute__((sda));
extern void draw_tresspasser_puzzle_ring_arc(int, u32, float, float, float, float) __asm__("FUN_L02_002da3d8");
extern void draw_tresspasser_puzzle_arrow(int, int, int, float, float) __asm__("FUN_L02_002da6c0");
extern void draw_tresspasser_puzzle_radial_strip(void *, f32, f32, f32, s32) __asm__("FUN_L02_002da938");
extern void draw_tresspasser_puzzle_textured_quads(void) __asm__("FUN_L02_002dae88");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

#define get_effect_texture ((u64 (*)(int))get_effect_texture)
#define draw_tresspasser_puzzle_ring_arc ((void (*)(u64, float, float, float, float, u32))draw_tresspasser_puzzle_ring_arc)
#define draw_tresspasser_puzzle_arrow ((void (*)(u64, int, int, float, float))draw_tresspasser_puzzle_arrow)
#define draw_tresspasser_puzzle_radial_strip ((void (*)(u64, f32, f32, f32, s32))draw_tresspasser_puzzle_radial_strip)

void draw_tresspasser_puzzle(char *moby) {
    /* Fakematch: the texture sits at retail's stack offset inside this frame. */
    struct { char pad0[0x78]; u64 texture; char pad80[0x10]; } ctx;
    int first_step, second_step;
    char *data = *(char **)(moby + 0x78);
    Ring12 *markers = (Ring12 *)(data + 0xb0);
    Ring12 *first, *second, *third;

    int i, state, third_step;
    float phase;

    vu1_add_g_sregister(0x47, 0x5380b);
    ctx.texture = get_effect_texture(0x10);
    draw_tresspasser_puzzle_textured_quads();
    {
        int marker_reset_countdown, *marker_cursor;
        for (marker_reset_countdown = 11, marker_cursor = markers->slot; marker_reset_countdown >= 0; --marker_reset_countdown, ++marker_cursor) {
            if (*marker_cursor != 0) *marker_cursor = 1;
        }
    }
    first_step = 0; second_step = 0; third_step = 0;
    if (*(float *)(data + 0xe4) > 0.31415927f) {
        state = *(int *)(data + 0xe0);
        if (state == 1) first_step = 1;
        else if (state == 2) second_step = 1;
        else third_step = 1;
    } else if (*(float *)(data + 0xe4) < -0.31415927f) {
        state = *(int *)(data + 0xe0);
        if (state == 1) first_step = -1;
        else if (state == 2) second_step = -1;
        else third_step = -1;
    }

    i = 0;
    first = (Ring12 *)(data + 0x20);
    second = (Ring12 *)(data + 0x50);
    third = (Ring12 *)(data + 0x80);
    {
        int index, other, slot, draw_second, color;
        float angle, distance, phase;
    for (; i < 12; ++i) {
        draw_second = 1;
        phase = -0.31415927f;
        if (*(int *)(data + 0xe0) == 1) phase = fast_add_rotations(phase, *(float *)(data + 0xe4));
        angle = fast_add_rotations(phase, (float)(i * 30) * 0.017453292f);
        index = i + *(int *)(data + 0x14);
        slot = index % 12;
        if (first->slot[slot] == 2) {
            other = i + *(int *)(data + 0x18) + first_step - second_step;
            color = D_L02_00161BAC;
            if (second->slot[other % 12] != 0) distance = (float)D_L02_00161B44;
            else {
                int third_index = i + *(int *)(data + 0x1c) + first_step - third_step;
                if (third->slot[third_index % 12] != 0) distance = (float)D_L02_00161B38;
                else if (third->slot[(third_index + 6) % 12] != 0) distance = (float)-D_L02_00161B38;
                else if (second->slot[(other + 6) % 12] != 0) distance = (float)-D_L02_00161B44;
                else if (first->slot[(index + 6) % 12] != 0) distance = (float)-D_L02_00161B50;
                else {
                    int opposite;
                    distance = (float)-D_L02_00161B5C;
                    draw_second = 0;
                    opposite = first_step + 6;
                    if (markers->slot[(i + opposite) % 12] != 0) {
                        int marker = (i + opposite) % 12;
                        first->slot[slot] = 3;
                        markers->slot[marker] = 2;
                        color = D_L02_00161BB0;
                    }
                }
            }
            draw_tresspasser_puzzle_radial_strip(ctx.texture, fast_add_rotations(angle, 0.31415927f), (float)D_L02_00161B50, distance, color);
            if (draw_second) {
                draw_tresspasser_puzzle_radial_strip(ctx.texture, fast_add_rotations(angle, 0.31415927f), distance, (float)D_L02_00161B50, D_L02_00161BB4);
            }
        }
    }
    }
    {
        int i, index, other, slot, draw_second, color;
        float angle, distance, phase;
    for (i = 0; i < 12; ++i) {
        draw_second = 1;
        phase = -0.31415927f;
        color = D_L02_00161BAC;
        if (*(int *)(data + 0xe0) == 2) phase = fast_add_rotations(phase, *(float *)(data + 0xe4));
        angle = fast_add_rotations(phase, (float)(i * 30) * 0.017453292f);
        index = i + *(int *)(data + 0x18);
        slot = index % 12;
        if (second->slot[slot] == 2) {
            other = i + *(int *)(data + 0x1c) + second_step - third_step;
            if (third->slot[other % 12] != 0) distance = (float)D_L02_00161B38;
            else if (third->slot[(other + 6) % 12] != 0) distance = (float)-D_L02_00161B38;
            else if (second->slot[(index + 6) % 12] != 0) distance = (float)-D_L02_00161B44;
            else if (first->slot[(i + *(int *)(data + 0x14) + second_step - first_step + 6) % 12] != 0) distance = (float)-D_L02_00161B50;
            else {
                int opposite;
                distance = (float)-D_L02_00161B5C;
                draw_second = 0;
                opposite = second_step + 6;
                if (markers->slot[(i + opposite) % 12] != 0) {
                    int marker = (i + opposite) % 12;
                    second->slot[slot] = 3;
                    markers->slot[marker] = 2;
                    color = D_L02_00161BB0;
                }
            }
            draw_tresspasser_puzzle_radial_strip(ctx.texture, fast_add_rotations(angle, 0.31415927f), (float)D_L02_00161B44, distance, color);
            if (draw_second) {
                draw_tresspasser_puzzle_radial_strip(ctx.texture, fast_add_rotations(angle, 0.31415927f), distance, (float)D_L02_00161B44, D_L02_00161BB4);
            }
        }
    }
    }
    {
        int i, index, other, slot, draw_second, color;
        float angle, distance, phase;
    for (i = 0; i < 12; ++i) {
        draw_second = 1;
        phase = -0.31415927f;
        color = D_L02_00161BAC;
        if (*(int *)(data + 0xe0) == 3) phase = fast_add_rotations(phase, *(float *)(data + 0xe4));
        angle = fast_add_rotations(phase, (float)(i * 30) * 0.017453292f);
        index = i + *(int *)(data + 0x1c);
        slot = index % 12;
        if (third->slot[slot] == 2) {
            if (third->slot[(index + 6) % 12] != 0) distance = (float)-D_L02_00161B38;
            else if (second->slot[(i + *(int *)(data + 0x18) + third_step - second_step + 6) % 12] != 0) distance = (float)-D_L02_00161B44;
            else if (first->slot[(i + *(int *)(data + 0x14) + third_step - first_step + 6) % 12] != 0) distance = (float)-D_L02_00161B50;
            else {
                int opposite;
                distance = (float)-D_L02_00161B5C;
                draw_second = 0;
                opposite = third_step + 6;
                if (markers->slot[(i + opposite) % 12] != 0) {
                    int marker = (i + opposite) % 12;
                    third->slot[slot] = 3;
                    markers->slot[marker] = 2;
                    color = D_L02_00161BB0;
                }
            }
            draw_tresspasser_puzzle_radial_strip(ctx.texture, fast_add_rotations(angle, 0.31415927f), (float)D_L02_00161B38, distance, color);
            if (draw_second) {
                draw_tresspasser_puzzle_radial_strip(ctx.texture, fast_add_rotations(angle, 0.31415927f), distance, (float)D_L02_00161B38, D_L02_00161BB4);
            }
        }
    }
    }
    {
        int i, color;
        float angle;
    for (i = 0; i < 12; ++i) {
        angle = -0.2617994f;
        if (*(int *)(data + 0xe0) == 1) {
            color = fast_tween_color(D_L02_00161BA0, D_L02_00161BA0 & 0xffffff,
                                  fast_sin((float)((D_L02_0015F5CC % 60) * 6) * 0.017453292f) * 0.5f + 0.5f);
            if (*(float *)(data + 0xe4) != 0.0f) angle = fast_add_rotations(angle, *(float *)(data + 0xe4));
        } else color = D_L02_00161BA4;
        angle = fast_add_rotations(angle, (float)(i * 30) * 0.017453292f);
        phase = fast_add_rotations(angle, 0.5235988f);
        draw_tresspasser_puzzle_ring_arc(ctx.texture, angle, phase, (float)D_L02_00161B4C, (float)D_L02_00161B48, color);
        if (first->slot[(i + *(int *)(data + 0x14)) % 12] != 0) {
            if (first->slot[(i + *(int *)(data + 0x14)) % 12] >= 3) {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 0, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B50);
                first->slot[(i + *(int *)(data + 0x14)) % 12] = 2;
            } else if (first->slot[(i + *(int *)(data + 0x14)) % 12] == 2) {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 1, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B50);
            } else {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 2, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B50);
            }
        }
    }
    }
    {
        int i, color;
        float angle;
    for (i = 0; i < 12; ++i) {
        angle = -0.2617994f;
        if (*(int *)(data + 0xe0) == 2) {
            color = fast_tween_color(D_L02_00161BA0, D_L02_00161BA0 & 0xffffff,
                                  fast_sin((float)((D_L02_0015F5CC % 60) * 6) * 0.017453292f) * 0.5f + 0.5f);
            if (*(float *)(data + 0xe4) != 0.0f) angle = fast_add_rotations(angle, *(float *)(data + 0xe4));
        } else color = D_L02_00161BA4;
        angle = fast_add_rotations(angle, (float)(i * 30) * 0.017453292f);
        phase = fast_add_rotations(angle, 0.5235988f);
        draw_tresspasser_puzzle_ring_arc(ctx.texture, angle, phase, (float)D_L02_00161B40, (float)D_L02_00161B3C, color);
        if (second->slot[(i + *(int *)(data + 0x18)) % 12] != 0) {
            if (second->slot[(i + *(int *)(data + 0x18)) % 12] >= 3) {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 0, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B44);
                second->slot[(i + *(int *)(data + 0x18)) % 12] = 2;
            } else if (second->slot[(i + *(int *)(data + 0x18)) % 12] == 2) {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 1, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B44);
            } else {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 2, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B44);
            }
        }
    }
    }
    {
        int i, color;
        float angle;
    for (i = 0; i < 12; ++i) {
        angle = -0.2617994f;
        if (*(int *)(data + 0xe0) == 3) {
            color = fast_tween_color(D_L02_00161BA0, D_L02_00161BA0 & 0xffffff,
                                  fast_sin((float)((D_L02_0015F5CC % 60) * 6) * 0.017453292f) * 0.5f + 0.5f);
            if (*(float *)(data + 0xe4) != 0.0f) angle = fast_add_rotations(angle, *(float *)(data + 0xe4));
        } else color = D_L02_00161BA4;
        angle = fast_add_rotations(angle, (float)(i * 30) * 0.017453292f);
        phase = fast_add_rotations(angle, 0.5235988f);
        draw_tresspasser_puzzle_ring_arc(ctx.texture, angle, phase, (float)D_L02_00161B34, (float)D_L02_00161B30, color);
        if (third->slot[(i + *(int *)(data + 0x1c)) % 12] != 0) {
            if (third->slot[(i + *(int *)(data + 0x1c)) % 12] >= 3) {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 0, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B38);
                third->slot[(i + *(int *)(data + 0x1c)) % 12] = 2;
            } else if (third->slot[(i + *(int *)(data + 0x1c)) % 12] == 2) {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 1, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B38);
            } else {
                draw_tresspasser_puzzle_arrow(ctx.texture, D_L02_00161BA8, 2, fast_add_rotations(angle, 0.2617994f), (float)D_L02_00161B38);
            }
        }
    }
    }
    {
        int value, color;
        float angle;
    for (i = 0; i < 12; ++i) {
        angle = fast_add_rotations(-0.08726646f, (float)(i * 30) * 0.017453292f);
        phase = fast_add_rotations(angle, 0.17453292f);
        value = markers->slot[i];
        if (value != 0) {
            if (value == 2) color = D_L02_00161BC4;
            else if (D_L02_0015F5CC & 0x20) color = D_L02_00161BC0;
            else color = D_L02_00161BBC;
            draw_tresspasser_puzzle_ring_arc(ctx.texture, angle, phase, (float)D_L02_00161B58, (float)D_L02_00161B54, color);
        }
    }
    }
    vu1_add_g_sregister(0x47, 0x5360b);
}

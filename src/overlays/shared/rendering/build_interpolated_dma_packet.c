/* Textured grid packets: prepares the scaled float table, builds a moby's interpolated VU1 packet and restores the view after it. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/ui/text/font_state.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/rendering/view/update_camera_view_matrices.h"
#include "rnc/rendering/vu1/vu1_add_data_ref.h"
#include "rnc/rendering/vu1/vu1_tex_flush.h"
#include "rnc/overlay/shared/rendering/build_interpolated_dma_packet_globals.h"
#include "qcopy.h"
#include "rnc/rendering/dma_tag.h"
#include "sda.h"

typedef struct QuadDrawArgs {
    char           pad0[0x1C];
    char          *dst;
    float          f20, f24, f28, f2C;
    unsigned char  c30, c31, c32, frames;
    char           pad34[8];
    unsigned short first;
    unsigned char  count;
    unsigned char  alpha;
} QuadDrawArgs;

typedef struct QuadCamera {
    char  pad0[0x190];
    float v190[4];
    float v1A0[4];
    char  pad1B0[0x60];
    float f210;
    char  pad214[4];
    float f218;
    float f21C;
    char  pad220[8];
    float f228;
    float f22C;
    int   c230, c234, c238;
} QuadCamera;

typedef struct QuadFrame {
    char           pad0[8];
    unsigned short a;
    unsigned short b;
    char           padC[4];
} QuadFrame;

/* The quad's GS texture register and the camera values it saves (D_L02_001612F8..00161304). */
struct QuadTextureState {
    long  tex0;
    float camera_218;
    float camera_21C;
    float camera_228;
    float camera_22C;
};

extern u8 scaled_table_output[] __asm__("D_L02_001CB9C0");

extern s32 scaled_table_header_leading_word_gp __asm__("D_L02_001612A0") __attribute__((sda));

extern float scaled_table_source[] __asm__("D_L02_001CB500");

extern void FUN_L02_002a4058_u(char *arg, int val) __asm__("FUN_L02_002a4058");

extern void      FUN_L02_0024fca0(void *, void *, void *, void *, void *, int, float);
extern char      level02_camera_view_state[] __asm__("D_L02_0016D040");
extern char      D_L02_001673C0[];
extern char      interpolated_draw_gif_preamble[] __asm__("D_L02_001CB8F0");
extern QuadFrame l02_interpolated_frame_source_index_table[] __asm__("D_L02_0016D780");
extern char     *interpolated_frame_source_data __asm__("D_L02_0015F420");

extern union PacketCursor l02_packet __asm__("D_L02_001611C0") MACRO_ADDR;

extern int frame_counter_ref[] __asm__("D_L02_0015F5CC") MACRO_ADDR;

extern struct QuadTextureState quad_texture_state __asm__("D_L02_001612F0") MACRO_ADDR;

extern int *D_L02_001611C0;
extern void vu1_add_vif_code(int) __asm__("FUN_00233938");

void prepare_scaled_float_table_with_target_byte(char *value_target,
                                                 int   val,
                                                 float scale) __asm__("FUN_L02_002a4058");

void prepare_scaled_float_table_with_target_byte(char *value_target, int val, float scale) {
    int    i;
    float *destination;
    float *source;
    float *base;
    value_target[0x3F] = val;
    *(s32 *)(scaled_table_output + 0x0) = scaled_table_header_leading_word_gp;
    *(s32 *)(scaled_table_output + 0x4) = scaled_table_header_second_word;
    *(s32 *)(scaled_table_output + 0x8) = scaled_table_header_third_word;
    *(s32 *)(scaled_table_output + 0xC) = scaled_table_header_final_word;
    destination = (float *)(scaled_table_output + 0x10);
    base = scaled_table_source;
    source = base + 4;
    for (i = 0xF7; i >= 0; i--) {
        *destination = *source * scale;
        destination++;
        source++;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002A52B0), where it is exact; names translated to the US level program. */

void prepare_scaled_table_with_byte_128(char *value_target) __asm__("FUN_L02_002a40d0");

void prepare_scaled_table_with_byte_128(char *value_target) {
    FUN_L02_002a4058_u(value_target, 0x80);
}
#define PACKET_CURSOR    (l02_packet.words)
#define TEXTURE_CURSOR   (gs_texture_allocation.cursor)
#define FRAME_COUNTER    (frame_counter_ref[0])
#define TEXTURE_REGISTER (quad_texture_state.tex0)

/* Builds one moby's VU1 draw packet: camera matrices and screen setup, the two interpolated vertex tables of
 * its animation frame, then the GS register block for its texture. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002A52D0), where it is exact; names translated to the US level program. */

/* Fakematch: the frame counter is read as a one-element array, so its load stays below the packet stores like retail. */

void build_interpolated_dma_packet(QuadDrawArgs *draw_parameters) __asm__("FUN_L02_002a40f0");

void build_interpolated_dma_packet(QuadDrawArgs *draw_parameters) {
    float        view_matrix[16];
    int         *base;
    int         *cursor;
    long        *words;
    long         color;
    long         blend_mode;
    unsigned int alpha_limit;

    textured_grid_saved_fog_near_distance = ((QuadCamera *)level02_camera_view_state)->f218;
    textured_grid_saved_fog_far_distance = ((QuadCamera *)level02_camera_view_state)->f21C;
    textured_grid_saved_fog_near_intensity = ((QuadCamera *)level02_camera_view_state)->f228;
    textured_grid_saved_fog_far_intensity = ((QuadCamera *)level02_camera_view_state)->f22C;
    if (textured_grid_fog_source_mode != 1) {
        ((QuadCamera *)level02_camera_view_state)->f218 = draw_parameters->f20;
        ((QuadCamera *)level02_camera_view_state)->f21C = draw_parameters->f24;
        ((QuadCamera *)level02_camera_view_state)->f228 = draw_parameters->f28;
        ((QuadCamera *)level02_camera_view_state)->f22C = draw_parameters->f2C;
    }
    update_view_context();
    update_camera_view_matrices();
    set_scale_matrix(view_matrix, 1024.0f);
    fast_vec_scale(&view_matrix[12], D_L02_001673C0, -1024.0f);
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
    *(float *)(base + 0x2B) = ((QuadCamera *)level02_camera_view_state)->f210;
    cursor = base + 0x2C;
    qcopy(cursor, ((QuadCamera *)level02_camera_view_state)->v190);
    cursor = base + 0x30;
    qcopy(cursor, ((QuadCamera *)level02_camera_view_state)->v1A0);
    *(float *)(base + 0x34) = ((QuadCamera *)level02_camera_view_state)->f22C;
    *(float *)(base + 0x35) = ((QuadCamera *)level02_camera_view_state)->f228;
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
    PACKET_CURSOR[1] = (int)interpolated_draw_gif_preamble;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000003;
    PACKET_CURSOR += 4;
    PACKET_CURSOR[0] = 0x10000009;
    PACKET_CURSOR[1] = 0;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000009;
    PACKET_CURSOR += 4;
    {
        int        frame;
        int        step;
        float      fraction;
        char      *dst;
        QuadFrame *frame_from;
        QuadFrame *frame_to;
        char      *from_a, *from_b, *to_a, *to_b;
        frame = FRAME_COUNTER;
        step = frame / draw_parameters->frames;
        fraction = (float)frame / (float)draw_parameters->frames - (float)step;
        frame_from = &l02_interpolated_frame_source_index_table[draw_parameters->first +
                                                                step % draw_parameters->count];
        frame_to = &l02_interpolated_frame_source_index_table[draw_parameters->first +
                                                              (step + 1) % draw_parameters->count];
        from_a = interpolated_frame_source_data + frame_from->a * 16;
        from_b = interpolated_frame_source_data + frame_from->b * 16;
        to_b = interpolated_frame_source_data + frame_to->b * 16;
        to_a = interpolated_frame_source_data + frame_to->a * 16;
        dst = draw_parameters->dst;
        *(long *)dst = 0x0800000000000400;
        *(long *)(draw_parameters->dst + 8) = 0;
        FUN_L02_0024fca0(draw_parameters->dst + 0x10, from_a, from_b, to_a, to_b, 0x1000, fraction);
    }
    TEXTURE_REGISTER = (TEXTURE_CURSOR >> 8) | 0x18100000 | 0x580000000;
    color = draw_parameters->c30 | draw_parameters->c31 << 8 | draw_parameters->c32 << 16;
    if (textured_grid_fog_source_mode == 1) {
        color = ((QuadCamera *)level02_camera_view_state)->c230 |
                ((QuadCamera *)level02_camera_view_state)->c234 << 8 |
                ((QuadCamera *)level02_camera_view_state)->c238 << 16;
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

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002A58C0), where it is exact; names translated to the US level program. */

void restore_view_after_textured_grid(void) __asm__("FUN_L02_002a46e0");

void restore_view_after_textured_grid(void) {
    char *camera_state;
    long *gs_register_packet;
    int  *packet_base;
    vu1_add_vif_code(0x13000000);
    camera_state = level02_camera_view_state;
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
    gs_register_packet[2] = (long)*(int *)(camera_state + 0x230) |
                            ((long)*(int *)(camera_state + 0x234) << 8) |
                            ((long)*(int *)(camera_state + 0x238) << 16);
    gs_register_packet[3] = 0x3D;
    D_L02_001611C0 = packet_base + 12;
    vu1_tex_flush();
}

s32 D_L03_001CB4C4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001CB4C8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001CB4CC[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CB844[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CB848[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CB84C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CB444[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CB448[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CB44C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CBBC4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CBBC8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CBBCC[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CB544[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CB548[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CB54C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001CBAC4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001CBAC8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001CBACC[1] __attribute__((section(".data"))) = {0};

s32   D_L03_001612A0 = 0x20000000;
char *D_L03_0015F420 = 0;

int  *D_L03_001611C0 = 0;
int  *D_L04_001611C0 = 0;
s32   D_L05_001612A0 = 0x20000000;
char *D_L05_0015F420 = 0;

int *D_L05_001611C0 = 0;
int *D_L06_001611C0 = 0;
s32  D_L07_001612A0 = 0x20000000;

int  *D_L07_001611C0 = 0;
s32   D_L08_001612A0 = 0x20000000;
char *D_L08_0015F420 = 0;

int  *D_L08_001611C0 = 0;
s32   D_L09_001612A0 = 0x20000000;
s32   D_L14_001612A0 = 0x20000000;
char *D_L14_0015F420 = 0;

int *D_L14_001611C0 = 0;
int *D_L18_001611C0 = 0;

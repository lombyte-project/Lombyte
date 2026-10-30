#ifndef LOMBYTE_RNC_OVERLAY_L08_MESH_BATCHES_H
#define LOMBYTE_RNC_OVERLAY_L08_MESH_BATCHES_H

#include "types.h"
#include "rnc/overlay/mesh_batch.h"

/* Level 08 mesh batches. Each batch i draws counts[i] vertices with
   FUN_L08_00258090(scratch, uvs[i], counts[i], pair) and
   FUN_L00_001fde98(counts[i], positions[i], colours[i], scratch, 1);
   scale_rgba_tables reads colours[i][j] as RGBA words for j < counts[i].
   The batch count is the drawing loop's bound. */
/* pair-offset batch (emit_pair_offset_render_batch, loop i < 8): vertex counts [41, 18, 45, 40, 38, 37, 35, 35]. */
extern f32 D_L08_001DB5C0[41][3];
extern f32 D_L08_001DB7B0[18][3];
extern f32 D_L08_001DB888[45][3];
extern f32 D_L08_001DBAA8[40][3];
extern f32 D_L08_001DBC88[38][3];
extern f32 D_L08_001DBE50[37][3];
extern f32 D_L08_001DC010[35][3];
extern f32 D_L08_001DC1B8[35][3];
extern f32 D_L08_001DC360[41][2];
extern f32 D_L08_001DC4B0[18][2];
extern f32 D_L08_001DC540[45][2];
extern f32 D_L08_001DC6B0[40][2];
extern f32 D_L08_001DC7F0[38][2];
extern f32 D_L08_001DC920[37][2];
extern f32 D_L08_001DCA50[35][2];
extern f32 D_L08_001DCB70[35][2];
extern u32 D_L08_001DDA28[41];
extern u32 D_L08_001DDAD0[18];
extern u32 D_L08_001DDB18[45];
extern u32 D_L08_001DDBD0[40];
extern u32 D_L08_001DDC70[38];
extern u32 D_L08_001DDD08[37];
extern u32 D_L08_001DDDA0[35];
extern u32 D_L08_001DDE30[35];

/* The pair-offset batch tables, also read by emit_pair_offset_render_batch. */
extern s32        pair_offset_batch_vertex_counts[8] __asm__("D_L08_001DB518");
extern MeshPoint *pair_offset_batch_packet_triplet_streams[8] __asm__("D_L08_001DDEC0");
extern MeshUv    *pair_offset_batch_input_pair_streams[8] __asm__("D_L08_001DDEE0");
extern u32       *pair_offset_batch_colour_tables[8] __asm__("D_L08_001DDF20");

/* eleven-entry batch (draw_eleven_entry_scrolled_texture_batch, loop i < 11): vertex counts [90, 8, 7, 62, 95, 89, 13, 95, 33, 7, 5]. */
extern f32 D_L08_001DE020[90][3];
extern f32 D_L08_001DE458[8][3];
extern f32 D_L08_001DE4B8[7][3];
extern f32 D_L08_001DE510[62][3];
extern f32 D_L08_001DE7F8[95][3];
extern f32 D_L08_001DEC70[89][3];
extern f32 D_L08_001DF0A0[13][3];
extern f32 D_L08_001DF140[95][3];
extern f32 D_L08_001DF5B8[33][3];
extern f32 D_L08_001DF748[7][3];
extern f32 D_L08_001DF7A0[5][3];
extern f32 D_L08_001DF7E0[90][2];
extern f32 D_L08_001DFAB0[8][2];
extern f32 D_L08_001DFAF0[7][2];
extern f32 D_L08_001DFB30[62][2];
extern f32 D_L08_001DFD20[95][2];
extern f32 D_L08_001E0020[89][2];
extern f32 D_L08_001E02F0[13][2];
extern f32 D_L08_001E0360[95][2];
extern f32 D_L08_001E0660[33][2];
extern f32 D_L08_001E0770[7][2];
extern f32 D_L08_001E07B0[5][2];
extern u32 D_L08_001E1F98[90];
extern u32 D_L08_001E2100[8];
extern u32 D_L08_001E2120[7];
extern u32 D_L08_001E2140[62];
extern u32 D_L08_001E2238[95];
extern u32 D_L08_001E23B8[89];
extern u32 D_L08_001E2520[13];
extern u32 D_L08_001E2558[95];
extern u32 D_L08_001E26D8[33];
extern u32 D_L08_001E2760[7];
extern u32 D_L08_001E2780[5];

/* scrolled mesh batch (draw_scrolled_mesh_batch, loop i < 13): vertex counts [13, 13, 21, 7, 76, 44, 24, 37, 41, 148, 8, 71, 126]. */
extern f32 D_L08_001E2960[13][3];
extern f32 D_L08_001E2A00[13][3];
extern f32 D_L08_001E2AA0[21][3];
extern f32 D_L08_001E2BA0[7][3];
extern f32 D_L08_001E2BF8[76][3];
extern f32 D_L08_001E2F88[44][3];
extern f32 D_L08_001E3198[24][3];
extern f32 D_L08_001E32B8[37][3];
extern f32 D_L08_001E3478[41][3];
extern f32 D_L08_001E3668[148][3];
extern f32 D_L08_001E3D58[8][3];
extern f32 D_L08_001E3DB8[71][3];
extern f32 D_L08_001E4110[126][3];
extern f32 D_L08_001E4700[13][2];
extern f32 D_L08_001E4770[13][2];
extern f32 D_L08_001E47E0[21][2];
extern f32 D_L08_001E4890[7][2];
extern f32 D_L08_001E48D0[76][2];
extern f32 D_L08_001E4B30[44][2];
extern f32 D_L08_001E4C90[24][2];
extern f32 D_L08_001E4D50[37][2];
extern f32 D_L08_001E4E80[41][2];
extern f32 D_L08_001E4FD0[148][2];
extern f32 D_L08_001E5470[8][2];
extern f32 D_L08_001E54B0[71][2];
extern f32 D_L08_001E56F0[126][2];
extern u32 D_L08_001E7878[13];
extern u32 D_L08_001E78B0[13];
extern u32 D_L08_001E78E8[21];
extern u32 D_L08_001E7940[7];
extern u32 D_L08_001E7960[76];
extern u32 D_L08_001E7A90[44];
extern u32 D_L08_001E7B40[24];
extern u32 D_L08_001E7BA0[37];
extern u32 D_L08_001E7C38[41];
extern u32 D_L08_001E7CE0[148];
extern u32 D_L08_001E7F30[8];
extern u32 D_L08_001E7F50[71];
extern u32 D_L08_001E8070[126];

/* The eleven-entry and scrolled batch tables, also read by draw_scrolled_mesh_batch. */
extern s32        eleven_entry_batch_vertex_counts[11] __asm__("D_L08_001DDF40");
extern MeshPoint *eleven_entry_scrolled_vertex_position_tables[11] __asm__("D_L08_001E2798");
extern MeshUv    *eleven_entry_scrolled_base_uv_tables[11] __asm__("D_L08_001E27C8");
extern u32       *eleven_entry_batch_colour_tables[11] __asm__("D_L08_001E2828");
extern s32        scrolled_mesh_batch_vertex_counts[13] __asm__("D_L08_001E2858");
extern MeshPoint *scrolled_mesh_position_streams[13] __asm__("D_L08_001E8268");
extern MeshUv    *scrolled_mesh_uv_source_streams[13] __asm__("D_L08_001E82A0");
extern u32       *scrolled_mesh_batch_colour_tables[13] __asm__("D_L08_001E8310");

/* batalia six batches (queue_batalia_six_textured_batches, loop i < 6): vertex counts [89, 89, 126, 10, 134, 75]. */
extern f32 D_L08_001F0240[89][3];
extern f32 D_L08_001F0670[89][3];
extern f32 D_L08_001F0AA0[126][3];
extern f32 D_L08_001F1088[10][3];
extern f32 D_L08_001F1100[134][3];
extern f32 D_L08_001F1748[75][3];
extern f32 D_L08_001F1AD0[89][2];
extern f32 D_L08_001F1DA0[89][2];
extern f32 D_L08_001F2070[126][2];
extern f32 D_L08_001F2460[10][2];
extern f32 D_L08_001F24B0[134][2];
extern f32 D_L08_001F28E0[75][2];
extern u32 D_L08_001F43C8[89];
extern u32 D_L08_001F4530[89];
extern u32 D_L08_001F4698[126];
extern u32 D_L08_001F4890[10];
extern u32 D_L08_001F48B8[134];
extern u32 D_L08_001F4AD0[75];

/* batalia eight batches (queue_batalia_eight_textured_batches, loop i < 8): vertex counts [94, 68, 35, 10, 91, 65, 22, 10]. */
extern f32 D_L08_001F4D00[94][3];
extern f32 D_L08_001F5168[68][3];
extern f32 D_L08_001F5498[35][3];
extern f32 D_L08_001F5640[10][3];
extern f32 D_L08_001F56B8[91][3];
extern f32 D_L08_001F5B00[65][3];
extern f32 D_L08_001F5E10[22][3];
extern f32 D_L08_001F5F18[10][3];
extern f32 D_L08_001F5F90[94][2];
extern f32 D_L08_001F6280[68][2];
extern f32 D_L08_001F64A0[35][2];
extern f32 D_L08_001F65C0[10][2];
extern f32 D_L08_001F6610[91][2];
extern f32 D_L08_001F68F0[65][2];
extern f32 D_L08_001F6B00[22][2];
extern f32 D_L08_001F6BB0[10][2];
extern u32 D_L08_001F7E90[94];
extern u32 D_L08_001F8008[68];
extern u32 D_L08_001F8118[35];
extern u32 D_L08_001F81A8[10];
extern u32 D_L08_001F81D0[91];
extern u32 D_L08_001F8340[65];
extern u32 D_L08_001F8448[22];
extern u32 D_L08_001F84A0[10];

/* queue_batalia_offset_textured_batches: two batches of 35 vertices
   (D_L08_001623C8 = {0x23, 0x23}). */
extern f32 D_L08_001EF7D0[105];
extern f32 D_L08_001EF978[105];
extern f32 D_L08_001EFB20[70];
extern f32 D_L08_001EFC40[70];
extern int D_L08_001F00A8[35];
extern int D_L08_001F0138[35];

#endif /* LOMBYTE_RNC_OVERLAY_L08_MESH_BATCHES_H */

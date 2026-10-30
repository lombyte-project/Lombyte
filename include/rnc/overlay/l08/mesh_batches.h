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
extern f32 pair_offset_batch_0_positions[41][3] __asm__("D_L08_001DB5C0");
extern f32 pair_offset_batch_1_positions[18][3] __asm__("D_L08_001DB7B0");
extern f32 pair_offset_batch_2_positions[45][3] __asm__("D_L08_001DB888");
extern f32 pair_offset_batch_3_positions[40][3] __asm__("D_L08_001DBAA8");
extern f32 pair_offset_batch_4_positions[38][3] __asm__("D_L08_001DBC88");
extern f32 pair_offset_batch_5_positions[37][3] __asm__("D_L08_001DBE50");
extern f32 pair_offset_batch_6_positions[35][3] __asm__("D_L08_001DC010");
extern f32 pair_offset_batch_7_positions[35][3] __asm__("D_L08_001DC1B8");
extern f32 pair_offset_batch_0_uvs[41][2] __asm__("D_L08_001DC360");
extern f32 pair_offset_batch_1_uvs[18][2] __asm__("D_L08_001DC4B0");
extern f32 pair_offset_batch_2_uvs[45][2] __asm__("D_L08_001DC540");
extern f32 pair_offset_batch_3_uvs[40][2] __asm__("D_L08_001DC6B0");
extern f32 pair_offset_batch_4_uvs[38][2] __asm__("D_L08_001DC7F0");
extern f32 pair_offset_batch_5_uvs[37][2] __asm__("D_L08_001DC920");
extern f32 pair_offset_batch_6_uvs[35][2] __asm__("D_L08_001DCA50");
extern f32 pair_offset_batch_7_uvs[35][2] __asm__("D_L08_001DCB70");
extern u32 pair_offset_batch_0_colours[41] __asm__("D_L08_001DDA28");
extern u32 pair_offset_batch_1_colours[18] __asm__("D_L08_001DDAD0");
extern u32 pair_offset_batch_2_colours[45] __asm__("D_L08_001DDB18");
extern u32 pair_offset_batch_3_colours[40] __asm__("D_L08_001DDBD0");
extern u32 pair_offset_batch_4_colours[38] __asm__("D_L08_001DDC70");
extern u32 pair_offset_batch_5_colours[37] __asm__("D_L08_001DDD08");
extern u32 pair_offset_batch_6_colours[35] __asm__("D_L08_001DDDA0");
extern u32 pair_offset_batch_7_colours[35] __asm__("D_L08_001DDE30");

/* The pair-offset batch tables, also read by emit_pair_offset_render_batch. */
extern s32        pair_offset_batch_vertex_counts[8] __asm__("D_L08_001DB518");
extern MeshPoint *pair_offset_batch_packet_triplet_streams[8] __asm__("D_L08_001DDEC0");
extern MeshUv    *pair_offset_batch_input_pair_streams[8] __asm__("D_L08_001DDEE0");
extern u32       *pair_offset_batch_colour_tables[8] __asm__("D_L08_001DDF20");

/* eleven-entry batch (draw_eleven_entry_scrolled_texture_batch, loop i < 11): vertex counts [90, 8, 7, 62, 95, 89, 13, 95, 33, 7, 5]. */
extern f32 eleven_entry_batch_0_positions[90][3] __asm__("D_L08_001DE020");
extern f32 eleven_entry_batch_1_positions[8][3] __asm__("D_L08_001DE458");
extern f32 eleven_entry_batch_2_positions[7][3] __asm__("D_L08_001DE4B8");
extern f32 eleven_entry_batch_3_positions[62][3] __asm__("D_L08_001DE510");
extern f32 eleven_entry_batch_4_positions[95][3] __asm__("D_L08_001DE7F8");
extern f32 eleven_entry_batch_5_positions[89][3] __asm__("D_L08_001DEC70");
extern f32 eleven_entry_batch_6_positions[13][3] __asm__("D_L08_001DF0A0");
extern f32 eleven_entry_batch_7_positions[95][3] __asm__("D_L08_001DF140");
extern f32 eleven_entry_batch_8_positions[33][3] __asm__("D_L08_001DF5B8");
extern f32 eleven_entry_batch_9_positions[7][3] __asm__("D_L08_001DF748");
extern f32 eleven_entry_batch_10_positions[5][3] __asm__("D_L08_001DF7A0");
extern f32 eleven_entry_batch_0_uvs[90][2] __asm__("D_L08_001DF7E0");
extern f32 eleven_entry_batch_1_uvs[8][2] __asm__("D_L08_001DFAB0");
extern f32 eleven_entry_batch_2_uvs[7][2] __asm__("D_L08_001DFAF0");
extern f32 eleven_entry_batch_3_uvs[62][2] __asm__("D_L08_001DFB30");
extern f32 eleven_entry_batch_4_uvs[95][2] __asm__("D_L08_001DFD20");
extern f32 eleven_entry_batch_5_uvs[89][2] __asm__("D_L08_001E0020");
extern f32 eleven_entry_batch_6_uvs[13][2] __asm__("D_L08_001E02F0");
extern f32 eleven_entry_batch_7_uvs[95][2] __asm__("D_L08_001E0360");
extern f32 eleven_entry_batch_8_uvs[33][2] __asm__("D_L08_001E0660");
extern f32 eleven_entry_batch_9_uvs[7][2] __asm__("D_L08_001E0770");
extern f32 eleven_entry_batch_10_uvs[5][2] __asm__("D_L08_001E07B0");
extern u32 eleven_entry_batch_0_colours[90] __asm__("D_L08_001E1F98");
extern u32 eleven_entry_batch_1_colours[8] __asm__("D_L08_001E2100");
extern u32 eleven_entry_batch_2_colours[7] __asm__("D_L08_001E2120");
extern u32 eleven_entry_batch_3_colours[62] __asm__("D_L08_001E2140");
extern u32 eleven_entry_batch_4_colours[95] __asm__("D_L08_001E2238");
extern u32 eleven_entry_batch_5_colours[89] __asm__("D_L08_001E23B8");
extern u32 eleven_entry_batch_6_colours[13] __asm__("D_L08_001E2520");
extern u32 eleven_entry_batch_7_colours[95] __asm__("D_L08_001E2558");
extern u32 eleven_entry_batch_8_colours[33] __asm__("D_L08_001E26D8");
extern u32 eleven_entry_batch_9_colours[7] __asm__("D_L08_001E2760");
extern u32 eleven_entry_batch_10_colours[5] __asm__("D_L08_001E2780");

/* scrolled mesh batch (draw_scrolled_mesh_batch, loop i < 13): vertex counts [13, 13, 21, 7, 76, 44, 24, 37, 41, 148, 8, 71, 126]. */
extern f32 scrolled_mesh_batch_0_positions[13][3] __asm__("D_L08_001E2960");
extern f32 scrolled_mesh_batch_1_positions[13][3] __asm__("D_L08_001E2A00");
extern f32 scrolled_mesh_batch_2_positions[21][3] __asm__("D_L08_001E2AA0");
extern f32 scrolled_mesh_batch_3_positions[7][3] __asm__("D_L08_001E2BA0");
extern f32 scrolled_mesh_batch_4_positions[76][3] __asm__("D_L08_001E2BF8");
extern f32 scrolled_mesh_batch_5_positions[44][3] __asm__("D_L08_001E2F88");
extern f32 scrolled_mesh_batch_6_positions[24][3] __asm__("D_L08_001E3198");
extern f32 scrolled_mesh_batch_7_positions[37][3] __asm__("D_L08_001E32B8");
extern f32 scrolled_mesh_batch_8_positions[41][3] __asm__("D_L08_001E3478");
extern f32 scrolled_mesh_batch_9_positions[148][3] __asm__("D_L08_001E3668");
extern f32 scrolled_mesh_batch_10_positions[8][3] __asm__("D_L08_001E3D58");
extern f32 scrolled_mesh_batch_11_positions[71][3] __asm__("D_L08_001E3DB8");
extern f32 scrolled_mesh_batch_12_positions[126][3] __asm__("D_L08_001E4110");
extern f32 scrolled_mesh_batch_0_uvs[13][2] __asm__("D_L08_001E4700");
extern f32 scrolled_mesh_batch_1_uvs[13][2] __asm__("D_L08_001E4770");
extern f32 scrolled_mesh_batch_2_uvs[21][2] __asm__("D_L08_001E47E0");
extern f32 scrolled_mesh_batch_3_uvs[7][2] __asm__("D_L08_001E4890");
extern f32 scrolled_mesh_batch_4_uvs[76][2] __asm__("D_L08_001E48D0");
extern f32 scrolled_mesh_batch_5_uvs[44][2] __asm__("D_L08_001E4B30");
extern f32 scrolled_mesh_batch_6_uvs[24][2] __asm__("D_L08_001E4C90");
extern f32 scrolled_mesh_batch_7_uvs[37][2] __asm__("D_L08_001E4D50");
extern f32 scrolled_mesh_batch_8_uvs[41][2] __asm__("D_L08_001E4E80");
extern f32 scrolled_mesh_batch_9_uvs[148][2] __asm__("D_L08_001E4FD0");
extern f32 scrolled_mesh_batch_10_uvs[8][2] __asm__("D_L08_001E5470");
extern f32 scrolled_mesh_batch_11_uvs[71][2] __asm__("D_L08_001E54B0");
extern f32 scrolled_mesh_batch_12_uvs[126][2] __asm__("D_L08_001E56F0");
extern u32 scrolled_mesh_batch_0_colours[13] __asm__("D_L08_001E7878");
extern u32 scrolled_mesh_batch_1_colours[13] __asm__("D_L08_001E78B0");
extern u32 scrolled_mesh_batch_2_colours[21] __asm__("D_L08_001E78E8");
extern u32 scrolled_mesh_batch_3_colours[7] __asm__("D_L08_001E7940");
extern u32 scrolled_mesh_batch_4_colours[76] __asm__("D_L08_001E7960");
extern u32 scrolled_mesh_batch_5_colours[44] __asm__("D_L08_001E7A90");
extern u32 scrolled_mesh_batch_6_colours[24] __asm__("D_L08_001E7B40");
extern u32 scrolled_mesh_batch_7_colours[37] __asm__("D_L08_001E7BA0");
extern u32 scrolled_mesh_batch_8_colours[41] __asm__("D_L08_001E7C38");
extern u32 scrolled_mesh_batch_9_colours[148] __asm__("D_L08_001E7CE0");
extern u32 scrolled_mesh_batch_10_colours[8] __asm__("D_L08_001E7F30");
extern u32 scrolled_mesh_batch_11_colours[71] __asm__("D_L08_001E7F50");
extern u32 scrolled_mesh_batch_12_colours[126] __asm__("D_L08_001E8070");

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
extern f32 batalia_six_batch_0_positions[89][3] __asm__("D_L08_001F0240");
extern f32 batalia_six_batch_1_positions[89][3] __asm__("D_L08_001F0670");
extern f32 batalia_six_batch_2_positions[126][3] __asm__("D_L08_001F0AA0");
extern f32 batalia_six_batch_3_positions[10][3] __asm__("D_L08_001F1088");
extern f32 batalia_six_batch_4_positions[134][3] __asm__("D_L08_001F1100");
extern f32 batalia_six_batch_5_positions[75][3] __asm__("D_L08_001F1748");
extern f32 batalia_six_batch_0_uvs[89][2] __asm__("D_L08_001F1AD0");
extern f32 batalia_six_batch_1_uvs[89][2] __asm__("D_L08_001F1DA0");
extern f32 batalia_six_batch_2_uvs[126][2] __asm__("D_L08_001F2070");
extern f32 batalia_six_batch_3_uvs[10][2] __asm__("D_L08_001F2460");
extern f32 batalia_six_batch_4_uvs[134][2] __asm__("D_L08_001F24B0");
extern f32 batalia_six_batch_5_uvs[75][2] __asm__("D_L08_001F28E0");
extern u32 batalia_six_batch_0_colours[89] __asm__("D_L08_001F43C8");
extern u32 batalia_six_batch_1_colours[89] __asm__("D_L08_001F4530");
extern u32 batalia_six_batch_2_colours[126] __asm__("D_L08_001F4698");
extern u32 batalia_six_batch_3_colours[10] __asm__("D_L08_001F4890");
extern u32 batalia_six_batch_4_colours[134] __asm__("D_L08_001F48B8");
extern u32 batalia_six_batch_5_colours[75] __asm__("D_L08_001F4AD0");

/* batalia eight batches (queue_batalia_eight_textured_batches, loop i < 8): vertex counts [94, 68, 35, 10, 91, 65, 22, 10]. */
extern f32 batalia_eight_batch_0_positions[94][3] __asm__("D_L08_001F4D00");
extern f32 batalia_eight_batch_1_positions[68][3] __asm__("D_L08_001F5168");
extern f32 batalia_eight_batch_2_positions[35][3] __asm__("D_L08_001F5498");
extern f32 batalia_eight_batch_3_positions[10][3] __asm__("D_L08_001F5640");
extern f32 batalia_eight_batch_4_positions[91][3] __asm__("D_L08_001F56B8");
extern f32 batalia_eight_batch_5_positions[65][3] __asm__("D_L08_001F5B00");
extern f32 batalia_eight_batch_6_positions[22][3] __asm__("D_L08_001F5E10");
extern f32 batalia_eight_batch_7_positions[10][3] __asm__("D_L08_001F5F18");
extern f32 batalia_eight_batch_0_uvs[94][2] __asm__("D_L08_001F5F90");
extern f32 batalia_eight_batch_1_uvs[68][2] __asm__("D_L08_001F6280");
extern f32 batalia_eight_batch_2_uvs[35][2] __asm__("D_L08_001F64A0");
extern f32 batalia_eight_batch_3_uvs[10][2] __asm__("D_L08_001F65C0");
extern f32 batalia_eight_batch_4_uvs[91][2] __asm__("D_L08_001F6610");
extern f32 batalia_eight_batch_5_uvs[65][2] __asm__("D_L08_001F68F0");
extern f32 batalia_eight_batch_6_uvs[22][2] __asm__("D_L08_001F6B00");
extern f32 batalia_eight_batch_7_uvs[10][2] __asm__("D_L08_001F6BB0");
extern u32 batalia_eight_batch_0_colours[94] __asm__("D_L08_001F7E90");
extern u32 batalia_eight_batch_1_colours[68] __asm__("D_L08_001F8008");
extern u32 batalia_eight_batch_2_colours[35] __asm__("D_L08_001F8118");
extern u32 batalia_eight_batch_3_colours[10] __asm__("D_L08_001F81A8");
extern u32 batalia_eight_batch_4_colours[91] __asm__("D_L08_001F81D0");
extern u32 batalia_eight_batch_5_colours[65] __asm__("D_L08_001F8340");
extern u32 batalia_eight_batch_6_colours[22] __asm__("D_L08_001F8448");
extern u32 batalia_eight_batch_7_colours[10] __asm__("D_L08_001F84A0");

/* queue_batalia_offset_textured_batches: two batches of 35 vertices
   (batalia_offset_batch_vertex_counts = {35, 35}). */
extern f32 batalia_offset_batch_0_positions[35][3] __asm__("D_L08_001EF7D0");
extern f32 batalia_offset_batch_1_positions[35][3] __asm__("D_L08_001EF978");
extern f32 batalia_offset_batch_0_uvs[35][2] __asm__("D_L08_001EFB20");
extern f32 batalia_offset_batch_1_uvs[35][2] __asm__("D_L08_001EFC40");
extern int batalia_offset_batch_0_colours[35] __asm__("D_L08_001F00A8");
extern int batalia_offset_batch_1_colours[35] __asm__("D_L08_001F0138");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MESH_BATCHES_H */

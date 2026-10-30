#ifndef LOMBYTE_RNC_RENDERING_MOBYS_RENDER_ENVIRONMENT_MAPPED_OBJECT_GLOBALS_H
#define LOMBYTE_RNC_RENDERING_MOBYS_RENDER_ENVIRONMENT_MAPPED_OBJECT_GLOBALS_H

#include "types.h"
#include "rnc/math/vector.h"

/* Scalar data read by src/rendering/mobys/render_environment_mapped_object.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 environment_mapping_was_active __asm__("D_001604A8");

/* Additional scalar data read by this unit. */
extern s32 environment_mapping_uv_refresh_state __asm__("D_001604A4") __attribute__((sda));

/* One corner of a mesh quad: the vertex it uses (pad is not read here). */
typedef struct {
    s16 vertex_index;
    s16 pad;
} QuadCornerIndex;

typedef struct {
    QuadCornerIndex corners[4];
} IndexedQuad;

/* The four environment-mapped meshes (classes 0x212..0x215). The sdata tables at
   D_00160550 / D_00160560 / D_00160570 point at their normals, positions and quads, and
   D_00160530 / D_00160540 give their vertex counts (0x66, 0x27, 0x31, 0x2A) and quad
   counts (0x4A, 0x18, 0x30, 0x1E). */
extern f32         environment_mesh_0_positions[102][4] __asm__("D_001D9CE0");
extern IndexedQuad environment_mesh_0_quads[74] __asm__("D_001DA340");
extern f32         environment_mesh_0_normals[102][4] __asm__("D_001DA7E0");
extern f32         environment_mesh_1_positions[39][4] __asm__("D_001DAE40");
extern IndexedQuad environment_mesh_1_quads[24] __asm__("D_001DB0B0");
extern f32         environment_mesh_1_normals[39][4] __asm__("D_001DB230");
extern f32         environment_mesh_2_positions[49][4] __asm__("D_001DB4A0");
extern IndexedQuad environment_mesh_2_quads[48] __asm__("D_001DB7B0");
extern f32         environment_mesh_2_normals[49][4] __asm__("D_001DBAB0");
extern f32         environment_mesh_3_positions[42][4] __asm__("D_001DBDC0");
extern IndexedQuad environment_mesh_3_quads[30] __asm__("D_001DC060");
extern f32         environment_mesh_3_normals[42][4] __asm__("D_001DC240");

/* Per-vertex work tables, sized for the largest mesh (0x66 vertices). */
extern Vec4  environment_mapping_transformed_vertex_positions[102] __asm__("D_001DC4E0");
extern float environment_mapping_vertex_uvs[102][2] __asm__("D_001DCB40");
extern float environment_mapping_saved_vertex_uvs[102][2] __asm__("D_001DCE70");

#endif /* LOMBYTE_RNC_RENDERING_MOBYS_RENDER_ENVIRONMENT_MAPPED_OBJECT_GLOBALS_H */

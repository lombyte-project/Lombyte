/* Ported from rac1-decomp (src/game/draw.c, func_001F4C30). */

#include "sda.h"
#include "qcopy.h"
#include "types.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/math/fast_math.h"

#include "rnc/rendering/effects/draw_light_quads.h"

typedef struct {
    FRow pos;
    FRow dir;
} LightRec;

extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");

/* Builds a 4-row matrix per light in D_0018E340 (count D_0015F474):
   each row starts as the light's position and is pushed along the
   corner D_0018CAA0[k], projected off the normalised light direction,
   scaled by the position's w; draw_geometry_quad draws it. The packet header,
   colours and UVs are filled on the stack but never sent. Both copies
   are qcopy (retail's lq/sq stay inside the loop), and the inner loop
   needs its own counter, not the one the setup loop used. */
void draw_light_quads(void) __asm__("FUN_001f4880");

extern LightRec light_quad_records[] __asm__("D_0018E340");

void draw_light_quads(void) {
    FRow          quad_vertices[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    FRow          dir;
    int           i;
    int           j;

    pkt[1] = get_effect_texture(0);
    pkt[2] = 0xFF9000000260;
    pkt[0] = 5;
    pkt[3] = 0x8000000044;
    for (j = 0; j < 4; j++) {
        uv[j][0] = light_quad_vertex_template[j].v[2];
        uv[j][1] = light_quad_vertex_template[j].v[3];
        colors[j] = 0x40808080;
    }
    for (i = 0; i < effect_quad_count; i++) {
        float radius;
        int   k;

        qcopy(&dir, &light_quad_records[i].dir);
        fast_vec_normalize(&dir, &dir, 1.0f);
        radius = light_quad_records[i].pos.v[3];
        for (k = 0; k < 4; k++) {
            float cx = light_quad_vertex_template[k].v[0];
            float vx = dir.v[0];
            float cy = light_quad_vertex_template[k].v[1];
            float corner_normal_dot = cx * vx + cy * dir.v[1];

            qcopy(&quad_vertices[k], &light_quad_records[i].pos);
            quad_vertices[k].v[0] += (cx - vx * corner_normal_dot) * radius;
            quad_vertices[k].v[1] += (cy - dir.v[1] * corner_normal_dot) * radius;
            quad_vertices[k].v[2] -= dir.v[2] * corner_normal_dot * radius;
        }
        draw_geometry_quad(quad_vertices, 0, 0);
    }
}

extern __typeof__(draw_light_quads) func_001F4880 __attribute__((alias("FUN_001f4880")));

FRow light_quad_vertex_template[4] = {
    {{1.0f, -1.0f, 0.0f, 1.0f}},
    {{-1.0f, -1.0f, 0.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f, 0.0f}},
};

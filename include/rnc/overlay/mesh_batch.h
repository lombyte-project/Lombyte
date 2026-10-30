#ifndef LOMBYTE_RNC_OVERLAY_MESH_BATCH_H
#define LOMBYTE_RNC_OVERLAY_MESH_BATCH_H

#include "types.h"

/* Per-vertex streams of the scrolling level mesh batches. Per batch: a vertex
   count, then that many positions, uv pairs and RGBA colour words, each stream
   reached through a table of one pointer per batch.
   FUN_L00_001fde98(count, positions, colours, uvs, 1) copies 3 words per
   position and one word per colour; FUN_L08_00258090(out, uvs, count, scroll)
   adds the scroll pair to each uv pair; scale_rgba_tables (FUN_L08_002f0f68)
   rescales each colour word. */
typedef f32 MeshPoint[3];
typedef f32 MeshUv[2];

#endif /* LOMBYTE_RNC_OVERLAY_MESH_BATCH_H */

#ifndef LOMBYTE_RNC_RENDERING_VIEW_SKY_VIEW_MATRIX_H
#define LOMBYTE_RNC_RENDERING_VIEW_SKY_VIEW_MATRIX_H

#include "types.h"

/* The sky's view matrix: update_camera_view_matrices copies the camera's
   three basis rows and writes the camera position scaled by 1024 as the
   fourth row (w = 1024). Read by the sky and bounding sphere checks. */
extern f32 sky_view_mtx[4][4] __asm__("D_0018CF80");

#endif /* LOMBYTE_RNC_RENDERING_VIEW_SKY_VIEW_MATRIX_H */

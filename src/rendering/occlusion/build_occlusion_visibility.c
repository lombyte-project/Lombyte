#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/rendering/occlusion/parse_occlusion_grid.h"
#include "rnc/math/asm_helpers.h"

#include "rnc/ui/menus/menu_globals.h"

extern f32 *occl_oct __asm__("D_0015F644");
extern s32  occl_invalid_grid __asm__("D_0015F64C");
extern u8  *occl_previous_grid __asm__("D_0015F650");

#include "rnc/camera/update_cam.h"

extern u8  *get_occlusion_grid_from_pair(s32, s32, s32, s32, s32, s32, f32) __asm__("FUN_001f2768");
extern void fast_mem_or16(u8 *, u8 *, u8 *, s32) __asm__("FUN_001f98f8");
extern s32  truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void fill_transfer_words(u8 *, s32, s32) __asm__("func_001F97E8");

void build_occlusion_visibility(void) __asm__("FUN_001f2820");

void build_occlusion_visibility(void) {
    enum {
        OCCLUSION_FALLBACK_PREVIOUS_MASK = 0,
        OCCLUSION_FALLBACK_ALL_VISIBLE = 1,
        OCCLUSION_FALLBACK_OCTANT_MASK = 2
    };
    f32  scale = 0.25f;
    s32  x, y, z;
    u8  *vis;
    u8  *gx, *gy, *gz;
    f32 *octant_mask_data;
    s32  bx, by, bz;

    x = truncate_float_to_s32(preview_camera.position.f[0] * scale);
    y = truncate_float_to_s32(preview_camera.position.f[1] * scale);
    z = truncate_float_to_s32(preview_camera.position.f[2] * scale);
    vis = parse_occlusion_grid(x, y, z);
    if (vis != 0) {
        occl_invalid_grid = 0;
        fast_mem_copy(occl_visibility, vis, 0x80);
        occl_previous_grid = vis;
    } else {
        occl_invalid_grid = 1;
        if (D_0015F648 == 0) {
            gx = get_occlusion_grid_from_pair(x - 1,
                                              y,
                                              z,
                                              x + 1,
                                              y,
                                              z,
                                              preview_camera.position.f[0] * scale -
                                                  convert_integer_to_float(x));
            gy = get_occlusion_grid_from_pair(x,
                                              y - 1,
                                              z,
                                              x,
                                              y + 1,
                                              z,
                                              preview_camera.position.f[1] * scale -
                                                  convert_integer_to_float(y));
            gz = get_occlusion_grid_from_pair(x,
                                              y,
                                              z - 1,
                                              x,
                                              y,
                                              z + 1,
                                              preview_camera.position.f[2] * scale -
                                                  convert_integer_to_float(z));
            if (gx != 0 || gy != 0 || gz != 0) {
                fast_mem_zero16(occl_visibility_merged, 0x80);
                if (gx != 0) {
                    fast_mem_or16(occl_visibility_merged, occl_visibility_merged, gx, 0x80);
                }
                if (gy != 0) {
                    fast_mem_or16(occl_visibility_merged, occl_visibility_merged, gy, 0x80);
                }
                if (gz != 0) {
                    fast_mem_or16(occl_visibility_merged, occl_visibility_merged, gz, 0x80);
                }
                vis = occl_visibility_merged;
                occl_previous_grid = vis;
                fast_mem_copy(occl_visibility, vis, 0x80);
            }
        }
        if (vis == 0) {
            switch (D_0015F648) {
            case OCCLUSION_FALLBACK_PREVIOUS_MASK:
                if (camera_position_publication_suppressed[0] == 0 && occl_previous_grid != 0) {
                    fast_mem_copy(occl_visibility, occl_previous_grid, 0x80);
                } else {
                    fill_transfer_words(occl_visibility, -1, 0x80);
                }
                break;
            case OCCLUSION_FALLBACK_ALL_VISIBLE:
                fill_transfer_words(occl_visibility, -1, 0x80);
                break;
            case OCCLUSION_FALLBACK_OCTANT_MASK:
                octant_mask_data = occl_oct;
                if (octant_mask_data != 0) {
                    bx = 0.0f < preview_camera.position.f[0] - octant_mask_data[0];
                    by = 0.0f < preview_camera.position.f[1] - octant_mask_data[1];
                    bz = 0.0f < preview_camera.position.f[2] - octant_mask_data[2];
                    fast_mem_copy(occl_visibility,
                                  (u8 *)octant_mask_data + ((bz + by * 2 + bx * 4) * 0x80 + 0x10),
                                  0x80);
                } else if (camera_position_publication_suppressed[0] == 0 &&
                           occl_previous_grid != 0) {
                    fast_mem_copy(occl_visibility, occl_previous_grid, 0x80);
                } else {
                    fill_transfer_words(occl_visibility, -1, 0x80);
                }
                break;
            }
        }
    }
    occl_visibility[0x7F] |= 0x80;
}

extern __typeof__(build_occlusion_visibility) func_001F2820 __attribute__((alias("FUN_001f2820")));

u8 occl_visibility[128] = {0};

u8 occl_visibility_merged[128] = {0};

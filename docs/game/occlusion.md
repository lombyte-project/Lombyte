# Occlusion visibility

`src/rendering/occlusion/` groups the four boot helpers that select the current
0x80-byte visibility mask.

| Function | USA symbol | Role |
|---|---|---|
| `update_occlusion` | `FUN_001f2c10` | Fill all-visible output in mode 0, rebuild in mode 2, otherwise leave it alone |
| `build_occlusion_visibility` | `FUN_001f2820` | Sample the camera cell, combine available neighboring masks, or apply the selected fallback |
| `parse_occlusion_grid` | `FUN_001f2690` | Walk the sparse z/y/x lookup tables and return a cell mask address, or zero |
| `get_occlusion_grid_from_pair` | `FUN_001f2768` | Try the nearer of two axis neighbors first, then the other if its lookup is empty |

The builder scales camera coordinates by 0.25 and truncates them to integer cell
coordinates. A valid cell becomes the current and previous mask. In fallback
mode 0, missing cells first try the neighboring pairs on all three axes and OR
any available masks. If none is available, mode 0 reuses the previous mask when
camera publication is not suppressed, or fills all-visible output. Mode 1 always
fills all-visible output. Mode 2 selects an octant mask when that table exists,
otherwise using the same previous-mask/all-visible choice. The builder always
sets bit 0x80 in the final byte of the output.

The lookup helpers have compiler-sensitive return declarations that differ
between units; the pair helper also calls `parse_occlusion_grid()` without arguments.

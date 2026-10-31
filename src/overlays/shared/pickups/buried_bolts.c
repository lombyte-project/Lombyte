/* Shared moby class 0x25D: selects the nearest area moby along its path. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/data_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"

/* pvars of the moby run by FUN_L01_002f2eb8 */
typedef struct {
    Vec4 home; /* position at spawn */
    union {
        struct {
            s32 path;   /* -1, or an index into D_L01_001B0930 */
            s32 volume; /* -1, or a clip volume is_point_inside_clip_volume tests */
        } s;
        s64 both; /* -1 when neither is set */
    } area;
    s32 count; /* spawn budget; the moby is deleted when it runs out */
} AreaVars;

extern int D_0015ED84;

extern s32 point_in_path_xy(void *, void *, s32) __asm__("FUN_L00_00259740");

extern s32 D_L01_00161AF0 __attribute__((sda));

void buried_bolts_update(struct Moby *moby) __asm__("FUN_L01_002f2eb8");

/* Moby class 605 */
void buried_bolts_update(struct Moby *moby) {
    AreaVars *area_vars = (AreaVars *)moby->pvars;
    Vec4      target;
    s32       k;
    s32      *path;
    f32       target_distance;

    if (moby->state == 0) {
        if (area_vars == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->update_dist = 0xFF;
        qcopy(&area_vars->home, &moby->pos);
        moby->flags |= 0x41;
        moby->state = 1;
        if (moby->command == 0 && D_0015ED84 < 20) {
            k = D_0014BF10[D_L01_00161AF0 / 2 + D_0015ED84 * 16];
            if (D_L01_00161AF0 & 1) {
                k &= 0xF;
            } else {
                k = (k >> 4) & 0xF;
            }
            D_L01_00161AF0++;
            moby->command = D_L01_00161AF0;
            k *= 5;
            area_vars->count -= k;
            if (area_vars->count <= 0) {
                mark_moby_for_removal(moby);
                return;
            }
        }
    } else {
        D_L01_00161AF0 = 0;
    }
    if (hero.items[0].moby != 0 && hero.items[0].item_id == 0x1B) {
        qcopy(&target, hero.items[0].moby->pvars + 0x10);
    } else {
        qcopy(&target, &hero.motion.pos);
    }
    if (area_vars->area.s.path != -1) {
        path = ((s32 *)l01_paths[area_vars->area.s.path]);
        k = point_in_path_xy(&target, path + 4, path[0]);
        if (k) {
            goto in_range;
        }
    }
    if (area_vars->area.s.volume != -1 &&
        is_point_inside_clip_volume(&target, area_vars->area.s.volume)) {
        goto in_range;
    }
    if (area_vars->area.both != -1) {
        return;
    }
in_range:
    target_distance = vector_distance_xy(&moby->pos, &target);
    if (target_distance < hero.detector_target_dist) {
        hero.metal_detector_target = moby;
        hero.detector_target_dist = target_distance;
        if (target_distance < 20.0f) {
            hero.nearby_moby_close = 1;
        }
    }
}

s32 D_L02_00161A60 = 0;
s32 D_L03_00161990 = 0;
s32 D_L04_001619E0 = 0;
s32 D_L05_00161A70 = 0;
s32 D_L06_00161960 = 0;
s32 D_L07_00161930 = 0;
s32 D_L08_00161E30 = 0;
s32 D_L09_001619F0 = 0;
s32 D_L10_00161950 = 0;
s32 D_L11_00161AC0 = 0;
s32 D_L12_00161B20 = 0;
s32 D_L13_00161A20 = 0;
s32 D_L14_00161C00 = 0;
s32 D_L15_00161BC0 = 0;
s32 D_L16_00161A70 = 0;
s32 D_L17_00161A50 = 0;
s32 D_L18_00161AB8 = 0;

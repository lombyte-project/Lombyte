/* Shared moby class 0x2E1: a volume that starts a cutscene camera sequence when the hero enters it. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/hero/hero_frame_update.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    u8  pad0[0xA6];
    s16 type;
} Moby;

typedef struct {
    u8    pad0[0x44];
    s16   count;
    u8    pad46[0x178 - 0x46];
    Moby *list[1];
} MobyList;

typedef struct {
    u8  b0;
    u8  active;
    u8  pad2[0x1E];
    f32 x;
    f32 y;
    f32 z;
} Effect;

typedef struct {
    char          pad0[0x454];
    unsigned char collected[1];
} TpLevelState;

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void func_L01_002FC890_m(char *) __asm__("FUN_L01_002fb4b8");
extern int  debug_print(void *, int, int) __asm__("FUN_001e93b0");

extern s32      D_0015ED84;
extern u8       D_0015EDB0;
extern MobyList D_L01_0016CCE0;
extern Effect   D_L01_0017C8C0;
extern char     cutscene_camera_init_position_error_format[] __asm__("D_L01_0020B550");
extern int      D_0015ED84; /* no foreign declaration */
extern int      cutscene_active __asm__("D_L01_0015F404");
extern char     camera_pos[] __asm__("D_L01_00167240");
extern s32      cutscene_active __asm__("D_L01_0015F404");

void attach_cutscene_fixed_scale_manipulator(void) __asm__("FUN_L01_002fb4b8");

void attach_cutscene_fixed_scale_manipulator(void) {
    s32 i;

    if (cutscene_mode == 2 && D_0015EDB0 != 0) {
        for (i = 0; i < D_L01_0016CCE0.count; i++) {
            int moby_type = D_L01_0016CCE0.list[i]->type;

            if ((moby_type == 0x398 || moby_type == 0x1BF) && D_L01_0017C8C0.active == 0) {
                attach_manipulator(D_L01_0016CCE0.list[i], 0, &D_L01_0017C8C0);
                D_L01_0017C8C0.z = D_L01_0017C8C0.y = D_L01_0017C8C0.x = 2.1f;
            }
        }
    }
}

/* Cutscene trigger: unless already seen, starts its camera sequence when the hero enters the volume (or it is
 * triggered), then ends it after its timer and re-arms. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_002FC988), where it is exact; names translated to the US level program. */
void cutscene_trigger_update(char *moby) __asm__("FUN_L01_002fb5b0");

/* Moby class 737 */
void cutscene_trigger_update(char *moby) {
    enum {
        CUTSCENE_TRIGGER_INITIALIZE = 0,
        CUTSCENE_TRIGGER_WAIT_FOR_TRIGGER = 1,
        CUTSCENE_TRIGGER_ACTIVE = 2,
        CUTSCENE_TRIGGER_WAIT_FOR_REARM = 3
    };
    char *trigger_vars = ((char *)MOBY(moby)->pvars);
    float saved_camera_position[4];
    float saved_camera_rotation[4];
    func_L01_002FC890_m(moby);
    switch (((unsigned char *)moby)[0x20]) {
    case CUTSCENE_TRIGGER_INITIALIZE:
        if (*(int *)(trigger_vars + 0x24) == -1) {
            debug_print(cutscene_camera_init_position_error_format,
                        MOBY(moby)->oclass,
                        *(unsigned short *)(moby + 0xA8));
            mark_moby_for_removal(moby);
            return;
        }
        *(int *)(trigger_vars + 0x2C) = scale_game_frames(*(int *)(trigger_vars + 0x2C));
        moby[0x20] = 1;
        {
            int ff = 0xFF;
            MOBY(moby)->draw_dist = ff;
            ((unsigned char *)moby)[0x30] = ff;
        }
        break;
    case CUTSCENE_TRIGGER_WAIT_FOR_TRIGGER: {
        char *hero_position;
        if (l01_save_state.cleared[(short)MOBY(moby)->save_id] != 0) {
            moby[0x20] = 1;
            break;
        }
        if ((*(int *)((((short)MOBY(moby)->save_id >> 5) << 2) + (D_0015ED84 << 8) +
                      (char *)D_0014C190_257218) >>
             (MOBY(moby)->save_id & 0x1F)) &
            1) {
            moby[0x20] = 1;
            break;
        }
        hero_position = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(hero_position, *(int *)(trigger_vars + 0x20)) == 0 &&
            ((unsigned char *)moby)[0xBC] != 1)
            break;
        qcopy(saved_camera_position, camera_pos);
        qcopy(saved_camera_rotation, camera_pos + 0x10);
        if (*(int *)(trigger_vars + 0x28) == -1) {
            set_race_camera(hero_position, hero_position + 0x10, 0x72, 1);
        } else {
            LevelCuboid *o = &l01_cuboids[*(int *)(trigger_vars + 0x28)];
            set_race_camera(&o->pos, &o->rot, 0x72, 1);
        }
        if (*(int *)(trigger_vars + 0x30) != 0) {
            jet_mount(
                saved_camera_position, saved_camera_rotation, 2, *(int *)(trigger_vars + 0x34), 0);
        } else {
            LevelCuboid *o = &l01_cuboids[*(int *)(trigger_vars + 0x24)];
            jet_mount(&o->pos, &o->rot, 0, 0, 0);
        }
        cutscene_active = 1;
        moby[0x20] = 2;
        moby[0xBC] = 1;
        {
            int   o = *(int *)(trigger_vars + 0x24) << 7;
            char *record_table_base = (char *)l01_cuboids;
            qcopy(trigger_vars, (char *)(o + (int)record_table_base) + 0x30);
            qcopy(trigger_vars + 0x10, (char *)(o + (int)record_table_base) + 0x70);
        }
        break;
    }
    case CUTSCENE_TRIGGER_ACTIVE:
        if (fast_dec_timer(trigger_vars + 0x2C) != 0 || ((unsigned char *)moby)[0xBC] != 1) {
            if (*(int *)(trigger_vars + 0x30) != 0) {
                jet_end_mount(2);
            } else {
                jet_end_mount(0);
            }
            request_hero_idle_state_for_vehicle_mode();
            cutscene_active = 0;
            moby[0x20] = 3;
            moby[0xBC] = 0;
        } else {
            set_camera_position(trigger_vars);
            set_camera_rotation(trigger_vars + 0x10);
        }
        break;
    case CUTSCENE_TRIGGER_WAIT_FOR_REARM:
        if (((unsigned char *)moby)[0xBC] == 1)
            moby[0x20] = ((unsigned char *)moby)[0xBC];
        break;
    }
}

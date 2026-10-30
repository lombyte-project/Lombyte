/* Ported from rac1-decomp (src/game/camera.c, func_001EC5B8). */
#include "sda.h"
#include "rnc/camera/camera_modes.h"
#include "rnc/hero/hero.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/clip_volume.h"

typedef struct {
    char unk_00[0x10];
    int  unk10;
    char unk14[8];
    int  unk1C;
} CamRec20;

extern CamRec20 *D_0015EF90 MACRO_ADDR;
extern CamRec20 *D_0015EF40 MACRO_ADDR;

typedef struct {
    char unk_00[4];
    int (*fn_04)(void *, void *);
    char unk_08[0xC];
} CamPrioHook;

extern CamPrioHook D_001E8C00[];
/* Camera_ActivationCheckPriority(cur, other): whether camera `cur` should
   take over from `other`. An inactive camera (+0x7C) never does; the camera
   type's +4 hook (D_001E8C00) may decide first (-1 no, 1 yes); otherwise
   the mode at +0x74 decides: 0 (and 1/2 once +0x7D is set) by priority
   byte, 4 through is_point_inside_clip_volume on the target record, 7 by the level's
   mode and target. Each case ends in `if (x) return 1;` falling out to the
   one shared `return 0;`, which is what lets cross-jumping and reorg give
   retail's branches; case 7's final test shares its `return 1` with the
   g < 0 exit so its `$v0 = 1` is not hoisted above the load. */
int camera_activation_check_priority(void *cur, void *other) __asm__("FUN_001ec210");

int camera_activation_check_priority(void *candidate_camera, void *other) {
    enum {
        CAMERA_COMPARE_ACCEPT = 1
    };
    char          *candidate_camera_bytes = (char *)candidate_camera;
    char          *current_camera_bytes = (char *)other;
    unsigned char *state = (unsigned char *)(candidate_camera_bytes + 0x74);

    if (*(unsigned char *)(candidate_camera_bytes + 0x7C) == 0) {
        return 0;
    }
    {
        int (*fn)(void *, void *) = D_001E8C00[*(short *)(candidate_camera_bytes + 0x8C)].fn_04;
        if (fn != 0) {
            switch (fn(candidate_camera, other)) {
            case -1:
                return 0;
            case CAMERA_COMPARE_ACCEPT:
                return 1;
            }
        }
    }
    switch (*(int *)state) {
    case CAMERA_ACTIVATION_HANDOFF_GATED_PRIORITY:
    case CAMERA_ACTIVATION_REQUIRE_HANDOFF_PRIORITY:
        if (state[9] == 0) {
            return 0;
        }
        /* fallthrough */
    case CAMERA_ACTIVATION_BY_PRIORITY:
        if (current_camera_bytes == 0) {
            return 1;
        }
        if (*(short *)(current_camera_bytes + 0x7E) != 0) {
            return 1;
        }
        if (state[8] > *(unsigned char *)(current_camera_bytes + 0x7C)) {
            return 1;
        }
        break;
    case CAMERA_ACTIVATION_IN_CLIP_VOLUME: {
        char *camera_descriptor_bytes =
            (char *)D_0015EF90[*(short *)(candidate_camera_bytes + 0x84)].unk1C;

        if (current_camera_bytes != 0 && *(short *)(current_camera_bytes + 0x7E) == 0 &&
            !(state[8] > *(unsigned char *)(current_camera_bytes + 0x7C))) {
            return 0;
        }
        if (is_point_inside_clip_volume(D_0013F3D0, *(int *)(camera_descriptor_bytes + 0xC))) {
            return 1;
        }
        break;
    }
    case CAMERA_ACTIVATION_MATCH_LISTENER_GROUP: {
        struct Hero *base = &hero;
        short        e = *(short *)(candidate_camera_bytes + 0x86);

        if (e != base->unk2284) {
            return 0;
        }
        if (*(short *)(current_camera_bytes + 0x7E) == 0 &&
            !(state[8] > *(unsigned char *)(current_camera_bytes + 0x7C))) {
            return 0;
        }
        if (e != 3) {
            return 1;
        }
        {
            CamRec20 *rec = &D_0015EF90[*(short *)(candidate_camera_bytes + 0x84)];
            int       secondary_descriptor_index = *(int *)((char *)rec->unk1C + 0x24);

            if (secondary_descriptor_index < 0 ||
                (base->rail_spline == D_0015EF40[secondary_descriptor_index].unk10 &&
                 base->rail_off_frames == 0)) {
                return 1;
            }
        }
        break;
    }
    }
    return 0;
}

extern __typeof__(camera_activation_check_priority) func_001EC210
    __attribute__((alias("FUN_001ec210")));

/* Defined below their only users, so retail reaches them with lui. */
CamRec20 *D_0015EF40 MACRO_ADDR = 0;

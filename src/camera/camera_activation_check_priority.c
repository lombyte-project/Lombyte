/* Ported from rac1-decomp (src/game/camera.c, func_001EC5B8). */
#include "sda.h"
#include "rnc/camera/camera_modes.h"
#include "rnc/hero/hero.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/camera/update_cam.h"

/* A 0x20-byte row of the level's camera record table: the record's
   pvars (pvar +0x0C: the clip volume of its activation, +0x24: the spline
   row of its rail) sit at +0x1C. */
typedef struct {
    char pad00[0x1C];
    int  pvars;
} CameraRecordRow;

/* A 0x20-byte row of the level's spline table (LevelSpline): path is what
   hero.rail_spline holds while the hero grinds that rail. */
typedef struct {
    char pad00[0x10];
    int  path;
    char pad14[0xC];
} CameraSplineRow;

/* UpdateCam from its activation rule on, as this check reads it: the rule as
   a word, then the priority (UpdateCam.active) and the handoff byte. */
struct CameraActivation {
    s32 rule; /* UpdateCam 0x74 */
    u8  pad_04[0x4];
    u8  priority;      /* UpdateCam 0x7C: 0 = inactive */
    u8  handoff_ready; /* UpdateCam 0x7D */
};

extern CameraRecordRow *D_0015EF90 MACRO_ADDR;
extern CameraSplineRow *D_0015EF40 MACRO_ADDR;

/* Each case ends in `if (x) return 1;` falling out to the one shared
   `return 0;`, which is what lets cross-jumping and reorg give retail's
   branches; the listener group case's final test shares its `return 1`
   with the negative index exit so its `$v0 = 1` is not hoisted above the
   load. */
int camera_activation_check_priority(struct UpdateCam *candidate,
                                     struct UpdateCam *current) __asm__("FUN_001ec210");

int camera_activation_check_priority(struct UpdateCam *candidate, struct UpdateCam *current) {
    enum {
        CAMERA_COMPARE_ACCEPT = 1
    };
    enum {
        CAMERA_TRANSITION_PRIORITY_GATED = 0
    };
    struct CameraActivation *activation = (struct CameraActivation *)&candidate->state;

    if (candidate->active == 0) {
        return 0;
    }
    {
        int (*check_priority)(void *, void *) = camera_types[candidate->type].check_priority;
        if (check_priority != 0) {
            switch (check_priority(candidate, current)) {
            case -1:
                return 0;
            case CAMERA_COMPARE_ACCEPT:
                return 1;
            }
        }
    }
    switch (activation->rule) {
    case CAMERA_ACTIVATION_HANDOFF_GATED_PRIORITY:
    case CAMERA_ACTIVATION_REQUIRE_HANDOFF_PRIORITY:
        if (activation->handoff_ready == 0) {
            return 0;
        }
        /* fallthrough */
    case CAMERA_ACTIVATION_BY_PRIORITY:
        if (current == 0) {
            return 1;
        }
        if (current->transition_state != CAMERA_TRANSITION_PRIORITY_GATED) {
            return 1;
        }
        if (activation->priority > current->active) {
            return 1;
        }
        break;
    case CAMERA_ACTIVATION_IN_CLIP_VOLUME: {
        char *camera_descriptor_bytes = (char *)D_0015EF90[candidate->descriptor_index].pvars;

        if (current != 0 && current->transition_state == CAMERA_TRANSITION_PRIORITY_GATED &&
            !(activation->priority > current->active)) {
            return 0;
        }
        if (is_point_inside_clip_volume(D_0013F3D0, *(int *)(camera_descriptor_bytes + 0xC))) {
            return 1;
        }
        break;
    }
    case CAMERA_ACTIVATION_MATCH_LISTENER_GROUP: {
        struct Hero *hero_state = &hero;
        short        listener_group = candidate->camera_class;

        if (listener_group != hero_state->camera_mode) {
            return 0;
        }
        if (current->transition_state == CAMERA_TRANSITION_PRIORITY_GATED &&
            !(activation->priority > current->active)) {
            return 0;
        }
        if (listener_group != 3) {
            return 1;
        }
        {
            CameraRecordRow *candidate_descriptor_entry = &D_0015EF90[candidate->descriptor_index];
            int              secondary_descriptor_index =
                *(int *)((char *)candidate_descriptor_entry->pvars + 0x24);

            if (secondary_descriptor_index < 0 ||
                (hero_state->rail_spline == D_0015EF40[secondary_descriptor_index].path &&
                 hero_state->rail_off_frames == 0)) {
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
CameraSplineRow *D_0015EF40 MACRO_ADDR = 0;

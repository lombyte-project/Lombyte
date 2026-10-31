/* Level 03 moby class moving_platform_small. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_path.h"

extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern int   trooper_play_sound(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern void  noop_main_callback(void *) __asm__("FUN_001e93e8");
extern void  FUN_L00_0025f730(void *, float);

/* Path platform update: waits, then glides along its path one way and back (ease in/out over 240 frames),
 * or steps node to node towards the path end, pausing between runs and playing its loop sound. */
void moving_platform_small_update(char *moby) __asm__("FUN_L03_00294c08");

/* Moby classes 868, 905, 928 */
void moving_platform_small_update(char *moby) {
    enum {
        PLATFORM_INITIALIZE_AND_WAIT = 0,
        PLATFORM_EASED_ENDPOINT_TRAVEL = 1,
        PLATFORM_PAUSE_BEFORE_EASED_TRAVEL = 2,
        PLATFORM_FOLLOW_PATH_NODES = 3,
        PLATFORM_PAUSE_BEFORE_NODE_TRAVEL = 4
    };
    float          delta[4];
    float          old_rot[4];
    float          step[4];
    float          endpoint_delta_or_position[4];
    char          *platform_pvars;
    char          *pa;
    char          *pb;
    char          *pc;
    char          *initial_path_position;
    int           *path;
    char          *platform_position;
    char          *vel;
    char          *target;
    int            voice_slot_index;
    int            easing_frame_count;
    int            next_path_node_index;
    int            end;
    int            count;
    int            next_path_node_byte_offset;
    int            node_off;
    signed char    path_node_step;
    float          speed;
    float          eased_endpoint_distance;
    float          d;
    unsigned char *voice_slot_entry;

    platform_pvars = ((char *)MOBY(moby)->pvars);
    fast_vec_scale(delta, moby + 0x10, -1.0f);
    qcopy(old_rot, moby + 0x40);
    noop_main_callback(moby);
    switch (((unsigned char *)moby)[0x20]) {
    case PLATFORM_INITIALIZE_AND_WAIT:
        pa = platform_pvars + 0xB0;
        *(char **)(platform_pvars + 0xC0) = ((char *)l03_paths[*(int *)(platform_pvars + 0xE0)]);
        if (*(int *)(platform_pvars + 0xF8) != 0) {
            pa[4] = -1;
        } else {
            pa[4] = 1;
        }
        initial_path_position = *(char **)(platform_pvars + 0xC0) + 0x10;
        if (pa[4] >= 0) {
            initial_path_position += (**(int **)(pa + 0x10) - 1) * 0x10;
        }
        qcopy_nc(moby + 0x10, initial_path_position);
        *(int *)pa = pa[4] < 0 ? 0 : **(int **)(pa + 0x10) - 1;
        MOBY(moby)->scale =
            *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * *(float *)(platform_pvars + 0xFC);
        *(int *)(platform_pvars + 0x104) = -1;
        if (fast_dec_timer(platform_pvars + 0x100)) {
            *(float *)(platform_pvars + 0x110) = fast_vec_dist(
                *(char **)(pa + 0x10) + 0x10, *(char **)(pa + 0x10) + **(int **)(pa + 0x10) * 0x10);
            easing_frame_count = scale_game_frames(240);
            *(float *)(platform_pvars + 0x114) =
                *(float *)(platform_pvars + 0x110) * 4.0f /
                convert_integer_to_float(easing_frame_count * scale_game_frames(240));
            *(int *)(platform_pvars + 0x10C) = 0;
            moby[0x20] = 1;
        }
        break;
    case PLATFORM_EASED_ENDPOINT_TRAVEL:
        pb = platform_pvars + 0xB0;
        speed = *(float *)(platform_pvars + 0x114);
        eased_endpoint_distance = 0.0f;
        if ((MOBY(moby)->oclass == 0x389 || MOBY(moby)->oclass == 0x364) &&
            !voice_is_playing(moby, *(int *)(platform_pvars + 0x104))) {
            *(int *)(platform_pvars + 0x104) = trooper_play_sound(0, 4, (int)moby, 0x389);
        }
        easing_frame_count = ++*(int *)(platform_pvars + 0x10C);
        if (scale_game_frames(240) / 2 < *(int *)(platform_pvars + 0x10C)) {
            speed = -speed;
            easing_frame_count = scale_game_frames(240) - *(int *)(platform_pvars + 0x10C);
            eased_endpoint_distance = *(float *)(platform_pvars + 0x110);
        }
        eased_endpoint_distance +=
            speed * 0.5f * convert_integer_to_float(easing_frame_count * easing_frame_count);
        if (pb[4] < 0) {
            eased_endpoint_distance = *(float *)(platform_pvars + 0x110) - eased_endpoint_distance;
        }
        fast_vec_sub(endpoint_delta_or_position,
                     *(char **)(pb + 0x10) + **(int **)(pb + 0x10) * 0x10,
                     *(char **)(pb + 0x10) + 0x10);
        *(OvlQuad *)step = *(OvlQuad *)endpoint_delta_or_position;
        fast_vec_normalize(step, step, eased_endpoint_distance);
        fast_vec_add(endpoint_delta_or_position, *(char **)(pb + 0x10) + 0x10, step);
        *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)endpoint_delta_or_position;
        if (*(int *)(platform_pvars + 0x10C) >= scale_game_frames(240)) {
            pb[4] = pb[4] * -1;
            voice_slot_index = *(int *)(platform_pvars + 0x104);
            if (voice_slot_index != -1) {
                voice_slot_entry = (unsigned char *)&voice_pool + voice_slot_index * 0x70;
                if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                    release_voice_slot(voice_slot_index);
                }
            }
            *(int *)(platform_pvars + 0x104) = -1;
            moby[0x20] = 2;
            *(int *)(platform_pvars + 0xE8) = scale_game_frames(*(int *)(platform_pvars + 0xF0));
        }
        break;
    case PLATFORM_PAUSE_BEFORE_EASED_TRAVEL:
        if (fast_dec_timer(platform_pvars + 0xE8)) {
            *(int *)(platform_pvars + 0x10C) = 0;
            moby[0x20] = 1;
        }
        break;
    case PLATFORM_FOLLOW_PATH_NODES:
        pc = platform_pvars + 0xB0;
        path = *(int **)(pc + 0x10);
        path_node_step = pc[4];
        count = *path;
        end = path_node_step < 0 ? 0 : count - 1;
        next_path_node_index = (*(int *)(platform_pvars + 0xB0) + count + path_node_step) % count;
        node_off = next_path_node_index * 0x10;
        platform_position = moby + 0x10;
        fast_vec_sub(platform_pvars + 0xA0, (char *)path + (node_off + 0x10), platform_position);
        next_path_node_byte_offset = node_off;
        target = *(char **)(pc + 0x10) + 0x10;
        if (*(int *)(platform_pvars + 0x108) != 0) {
            target += next_path_node_byte_offset;
        } else {
            target += end * 0x10;
            node_off =
                0; /* dead store: unused afterwards, but needed for the retail register allocation */
        }
        if (fast_vec_dist(platform_position, target) < *(float *)(platform_pvars + 0xF4)) {
            approach_value((float *)(platform_pvars + 0xE4),
                           frame_time * 0.5f,
                           *(float *)(platform_pvars + 0xEC) * *(float *)(platform_pvars + 0xEC) /
                               *(float *)(platform_pvars + 0xF4) * frame_time_sq * 0.5f);
        } else {
            approach_value((float *)(platform_pvars + 0xE4),
                           *(float *)(platform_pvars + 0xEC) * frame_time,
                           *(float *)(platform_pvars + 0xEC) * *(float *)(platform_pvars + 0xEC) /
                               *(float *)(platform_pvars + 0xF4) * frame_time_sq * 0.5f);
        }
        FUN_L00_0025f730(platform_pvars + 0xA0, *(float *)(platform_pvars + 0xE4));
        fast_vec_add(moby + 0x10, moby + 0x10, platform_pvars + 0xA0);
        if (fast_vec_dist(moby + 0x10,
                          *(char **)(pc + 0x10) + (next_path_node_byte_offset + 0x10)) <
            *(float *)(platform_pvars + 0xE4) * 2.0f) {
            *(int *)pc = next_path_node_index;
            if ((next_path_node_index == end || *(int *)(platform_pvars + 0x108) != 0) &&
                *(int *)(platform_pvars + 0xF0) > 0) {
                voice_slot_index = *(int *)(platform_pvars + 0x104);
                if (voice_slot_index != -1) {
                    voice_slot_entry = (unsigned char *)&voice_pool + voice_slot_index * 0x70;
                    if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                *(int *)(platform_pvars + 0x104) = -1;
                moby[0x20] = 4;
                *(int *)(platform_pvars + 0xE8) =
                    scale_game_frames(*(int *)(platform_pvars + 0xF0));
            }
        }
        break;
    case PLATFORM_PAUSE_BEFORE_NODE_TRAVEL:
        if (fast_dec_timer(platform_pvars + 0xE8)) {
            moby[0x20] = 3;
        }
        break;
    }
    fast_vec_add(delta, delta, moby + 0x10);
    update_entity_matrix(platform_pvars + 0x60, delta, old_rot, moby + 0x40);
}

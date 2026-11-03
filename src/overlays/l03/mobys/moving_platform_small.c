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
#include "rnc/overlay/shared/math/quad.h"
#include "rnc/overlay/shared/world/level_path.h"

extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern int   trooper_play_sound(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern void  noop_main_callback(void *) __asm__("FUN_001e93e8");
extern void  FUN_L00_0025f730(void *, float);

typedef struct MovingPlatformSmallVars {
    u8    pad_0[0x60];
    f32   transform[16];
    f32   node_direction[4];
    s32   path_cursor;
    u8    pad_B4[0xC];
    char *path;
    u8    pad_C4[0x1C];
    s32   path_index;
    f32   speed;
    s32   wait_timer;
    f32   max_speed;
    s32   wait_frames;
    f32   slowdown_distance;
    s32   reverse_direction;
    f32   scale_factor;
    s32   timer_100;
    s32   voice;
    s32   stop_at_every_node;
    s32   easing_frames;
    f32   path_endpoint_distance;
    f32   glide_speed;
} MovingPlatformSmallVars;

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
    float                    delta[4];
    float                    old_rot[4];
    float                    step[4];
    float                    endpoint_delta_or_position[4];
    MovingPlatformSmallVars *platform_pvars;
    char                    *pa;
    char                    *pb;
    char                    *pc;
    char                    *initial_path_position;
    int                     *path;
    char                    *platform_position;
    char                    *vel;
    char                    *target;
    int                      voice_slot_index;
    int                      easing_frame_count;
    int                      next_path_node_index;
    int                      end;
    int                      path_node_count;
    int                      next_path_node_byte_offset;
    int                      node_off;
    signed char              path_node_step;
    float                    speed;
    float                    eased_endpoint_distance;
    float                    d;
    unsigned char           *voice_slot_entry;

    platform_pvars = ((char *)MOBY(moby)->pvars);
    fast_vec_scale(delta, moby + 0x10, -1.0f);
    qcopy(old_rot, moby + 0x40);
    noop_main_callback(moby);
    switch (((unsigned char *)moby)[0x20]) {
    case PLATFORM_INITIALIZE_AND_WAIT:
        pa = &platform_pvars->path_cursor;
        platform_pvars->path = ((char *)l03_paths[platform_pvars->path_index]);
        if (platform_pvars->reverse_direction != 0) {
            pa[4] = -1;
        } else {
            pa[4] = 1;
        }
        initial_path_position = platform_pvars->path + 0x10;
        if (pa[4] >= 0) {
            initial_path_position += (**(int **)(pa + 0x10) - 1) * 0x10;
        }
        qcopy_nc(moby + 0x10, initial_path_position);
        *(int *)pa = pa[4] < 0 ? 0 : **(int **)(pa + 0x10) - 1;
        MOBY(moby)->scale = MOBY(moby)->pclass->scale * platform_pvars->scale_factor;
        platform_pvars->voice = -1;
        if (fast_dec_timer(&platform_pvars->timer_100)) {
            platform_pvars->path_endpoint_distance = fast_vec_dist(
                *(char **)(pa + 0x10) + 0x10, *(char **)(pa + 0x10) + **(int **)(pa + 0x10) * 0x10);
            easing_frame_count = scale_game_frames(240);
            platform_pvars->glide_speed =
                platform_pvars->path_endpoint_distance * 4.0f /
                convert_integer_to_float(easing_frame_count * scale_game_frames(240));
            platform_pvars->easing_frames = 0;
            MOBY(moby)->state = PLATFORM_EASED_ENDPOINT_TRAVEL;
        }
        break;
    case PLATFORM_EASED_ENDPOINT_TRAVEL:
        pb = &platform_pvars->path_cursor;
        speed = platform_pvars->glide_speed;
        eased_endpoint_distance = 0.0f;
        if ((MOBY(moby)->oclass == 0x389 || MOBY(moby)->oclass == 0x364) &&
            !voice_is_playing(moby, platform_pvars->voice)) {
            platform_pvars->voice = trooper_play_sound(0, 4, (int)moby, 0x389);
        }
        easing_frame_count = ++platform_pvars->easing_frames;
        if (scale_game_frames(240) / 2 < platform_pvars->easing_frames) {
            speed = -speed;
            easing_frame_count = scale_game_frames(240) - platform_pvars->easing_frames;
            eased_endpoint_distance = platform_pvars->path_endpoint_distance;
        }
        eased_endpoint_distance +=
            speed * 0.5f * convert_integer_to_float(easing_frame_count * easing_frame_count);
        if (pb[4] < 0) {
            eased_endpoint_distance =
                platform_pvars->path_endpoint_distance - eased_endpoint_distance;
        }
        fast_vec_sub(endpoint_delta_or_position,
                     *(char **)(pb + 0x10) + **(int **)(pb + 0x10) * 0x10,
                     *(char **)(pb + 0x10) + 0x10);
        *(OvlQuad *)step = *(OvlQuad *)endpoint_delta_or_position;
        fast_vec_normalize(step, step, eased_endpoint_distance);
        fast_vec_add(endpoint_delta_or_position, *(char **)(pb + 0x10) + 0x10, step);
        *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)endpoint_delta_or_position;
        if (platform_pvars->easing_frames >= scale_game_frames(240)) {
            pb[4] = pb[4] * -1;
            voice_slot_index = platform_pvars->voice;
            if (voice_slot_index != -1) {
                voice_slot_entry = (unsigned char *)&voice_pool + voice_slot_index * 0x70;
                if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                    release_voice_slot(voice_slot_index);
                }
            }
            platform_pvars->voice = -1;
            MOBY(moby)->state = PLATFORM_PAUSE_BEFORE_EASED_TRAVEL;
            platform_pvars->wait_timer = scale_game_frames(platform_pvars->wait_frames);
        }
        break;
    case PLATFORM_PAUSE_BEFORE_EASED_TRAVEL:
        if (fast_dec_timer(&platform_pvars->wait_timer)) {
            platform_pvars->easing_frames = 0;
            MOBY(moby)->state = PLATFORM_EASED_ENDPOINT_TRAVEL;
        }
        break;
    case PLATFORM_FOLLOW_PATH_NODES:
        pc = &platform_pvars->path_cursor;
        path = *(int **)(pc + 0x10);
        path_node_step = pc[4];
        count = *path;
        end = path_node_step < 0 ? 0 : count - 1;
        next_path_node_index = (platform_pvars->path_cursor + count + path_node_step) % count;
        node_off = next_path_node_index * 0x10;
        platform_position = moby + 0x10;
        fast_vec_sub(
            platform_pvars->node_direction, (char *)path + (node_off + 0x10), platform_position);
        next_path_node_byte_offset = node_off;
        target = *(char **)(pc + 0x10) + 0x10;
        if (platform_pvars->stop_at_every_node != 0) {
            target += next_path_node_byte_offset;
        } else {
            target += end * 0x10;
            node_off =
                0; /* dead store: unused afterwards, but needed for the retail register allocation */
        }
        if (fast_vec_dist(platform_position, target) < platform_pvars->slowdown_distance) {
            approach_value(&platform_pvars->speed,
                           frame_time * 0.5f,
                           platform_pvars->max_speed * platform_pvars->max_speed /
                               platform_pvars->slowdown_distance * frame_time_sq * 0.5f);
        } else {
            approach_value(&platform_pvars->speed,
                           platform_pvars->max_speed * frame_time,
                           platform_pvars->max_speed * platform_pvars->max_speed /
                               platform_pvars->slowdown_distance * frame_time_sq * 0.5f);
        }
        FUN_L00_0025f730(platform_pvars->node_direction, platform_pvars->speed);
        fast_vec_add(moby + 0x10, moby + 0x10, platform_pvars->node_direction);
        if (fast_vec_dist(moby + 0x10,
                          *(char **)(pc + 0x10) + (next_path_node_byte_offset + 0x10)) <
            platform_pvars->speed * 2.0f) {
            *(int *)pc = next_path_node_index;
            if ((next_path_node_index == end || platform_pvars->stop_at_every_node != 0) &&
                platform_pvars->wait_frames > 0) {
                voice_slot_index = platform_pvars->voice;
                if (voice_slot_index != -1) {
                    voice_slot_entry = (unsigned char *)&voice_pool + voice_slot_index * 0x70;
                    if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                platform_pvars->voice = -1;
                MOBY(moby)->state = PLATFORM_PAUSE_BEFORE_NODE_TRAVEL;
                platform_pvars->wait_timer = scale_game_frames(platform_pvars->wait_frames);
            }
        }
        break;
    case PLATFORM_PAUSE_BEFORE_NODE_TRAVEL:
        if (fast_dec_timer(&platform_pvars->wait_timer)) {
            MOBY(moby)->state = PLATFORM_FOLLOW_PATH_NODES;
        }
        break;
    }
    fast_vec_add(delta, delta, moby + 0x10);
    update_entity_matrix(platform_pvars->transform, delta, old_rot, moby + 0x40);
}

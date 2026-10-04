#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/voices/sound_update/FUN_0022ca50.s", FUN_0022ca50);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "qzero.h"

typedef union {
    u128 q;
    f32 f[4];
} VoiceVector;

typedef struct {
    u8 pad00[0x18];
    u8 source_state;
    u8 attenuation_flags;
    s16 sound_index;
    s32 bank_handle;
} VoiceDefinition;

typedef struct {
    u8 pad00[0x10];
    VoiceVector position;
    u8 state;
    u8 pad21[0x9F];
    VoiceVector rotation;
} VoiceMoby;

typedef struct {
    u32 handle;      /* 0x00 */
    u8 state;        /* 0x04 */
    u8 flags;        /* 0x05 */
    u8 pad06[2];
    VoiceDefinition *definition;   /* 0x08 */
    s32 pad0C;
    s32 volume;      /* 0x10 */
    s32 pitch_bend;       /* 0x14 */
    VoiceMoby *moby; /* 0x18 */
    s32 owner_context;     /* 0x1C */
    VoiceVector position;        /* 0x20 */
    VoiceVector position_offset;      /* 0x30 */
    u32 occlusion_history_position;    /* 0x40 */
    u8 occlusion_history[36];     /* 0x44 */
    u8 pad68[8];
} VoiceSlot;

typedef struct {
    VoiceVector listener_history[4];   /* 0x000 */
    s32 listener_history_position;        /* 0x040 */
    s32 pad44;
    s32 group_0_volume;          /* 0x048 */
    s32 group_1_volume;
    s32 group_2_volume;
    s32 group_3_volume;
    s32 group_4_volume;
    s32 group_5_volume;
    s32 pad60;
    s32 reverb_depth;          /* 0x064 */
    u8 reverb_type;
    u8 reverb_delay;
    u8 reverb_feedback;
    u8 reverb_commands;
    s32 pending_commands;          /* 0x06C */
    VoiceSlot voices[30];   /* 0x070 */
    s32 padD90[2];
    s32 occlusion_frame;         /* 0xD98 */
    s32 padD9C;
    VoiceVector occlusion_samples[6];     /* 0xDA0 */
} VoiceRuntimeState;

extern VoiceRuntimeState D_0013E550;
extern f32 D_0013F640[];
extern f32 D_0015ED6C;
extern s32 D_0015F5E8;
extern s32 D_0015F604;
extern s32 D_0015F60C;
extern VoiceVector D_00187080;
extern u8 D_00187290[];
extern s32 D_001872D4;

extern s32 ComputeByteStringHash(u8 *, s32);
extern int ComputeSectorIndex(int arg0);
extern void FillTransferWords(void *, s32, s32);
extern void ReadGlobalTableEntry(void);
extern s32 snd_flush_sound_commands(void) __asm__("FUN_0012dc80");
extern void snd_set_master_volume(s32, s32) __asm__("FUN_0012e208");
extern void snd_play_sound_vol_pan_pmpb() __asm__("FUN_0012e308");
extern void snd_stop_sound(s32) __asm__("FUN_0012e368");
extern void snd_sound_is_still_playing_cb() __asm__("FUN_0012e448");
extern void snd_set_sound_params_cb() __asm__("FUN_0012e4c0");
extern void snd_reset_state_and_flush_commands() __asm__("FUN_0012eb00");
extern void snd_set_reverb_ex(s32, s32, s32, s32, s32) __asm__("FUN_0012ef68");
extern void snd_auto_reverb(s32, s32, s32, s32) __asm__("FUN_0012efe0");
extern void add_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *out, void *a, f32 scale) __asm__("FUN_001f9a68");
extern f32 dot_vectors_xyz(void *a, void *b) __asm__("FUN_001f9ab0");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void func_001F9CF8(void *out, void *a, void *b) __asm__("FUN_001f9cf8");
extern void func_001FA2D8(void *out, void *a) __asm__("FUN_001fa2d8");
extern f32 ConvertIntegerToFloat(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void music_update() __asm__("FUN_00216290");
extern void clamp_voice_position_to_collision(void *) __asm__("FUN_0022c5a8");
extern u8 test_voice_occlusion(VoiceSlot *, void *) __asm__("FUN_0022c658");
extern s32 calculate_voice_volume(VoiceSlot *, VoiceVector *) __asm__("FUN_0022c7e8");
extern s32 calculate_voice_pan(VoiceSlot *, VoiceVector *, void *) __asm__("FUN_0022c830");
extern void voice_start_callback() __asm__("FUN_0022dd90");
extern void voice_update_callback() __asm__("FUN_0022ddd8");

s32 sound_update(void) __asm__("FUN_0022ca50");

s32 sound_update(void) {
    VoiceVector listener_occlusion_position;
    VoiceVector listener_velocity;
    VoiceVector relative_velocity;
    s32 flags[30];
    s32 voice_volumes[30];
    f32 radial_velocities[30];
    VoiceVector direction;
    VoiceVector listener_matrix[4];
    s32 slot_index;
    s32 underwater;
    s32 *voice_flags;
    s32 *volumes;
    VoiceMoby *moby;
    f32 water_height;
    s32 history_index;
    s32 previous_history_index;
    s32 listener_sample_count;
    s32 group_volume;
    s32 history_offset;
    s32 sample_index;
    s32 repeat_index;
    s32 owner_removed;
    s32 distance_volume;
    s32 occluded_samples;
    s32 parameter_mask;
    s32 pan;
    s32 pitch_modifier;
    s32 pitch_bend;
    s32 handle;
    s32 command_flags;

    water_height = 0.0f;
    underwater = D_001872D4;
    if (underwater != 0) {
        water_height = D_0013F640[0];
    }
    snd_flush_sound_commands();

    if (D_0013E550.reverb_commands & 8) {
        snd_set_reverb_ex(2, 0, 0, 0, 0);
    } else if (D_0013E550.reverb_commands & 0x13) {
        snd_set_reverb_ex(2, D_0013E550.reverb_type, D_0013E550.reverb_depth, D_0013E550.reverb_delay, D_0013E550.reverb_feedback);
    } else if (D_0013E550.reverb_commands & 4) {
        snd_auto_reverb(2, D_0013E550.reverb_depth, 0xC, 3);
    }
    D_0013E550.reverb_commands = 0;

    if ((D_0015F604 == 0 || D_0015F604 == 2) && D_0015F5E8 == 0) {
        clamp_voice_position_to_collision(&listener_occlusion_position);
    }

    qzero(&listener_velocity);
    history_index = (D_0013E550.listener_history_position + 1) % 4;
    D_0013E550.listener_history_position = history_index;
    qcopy(&D_0013E550.listener_history[history_index], &D_00187080);

    voice_flags = flags;
    volumes = voice_volumes;
    listener_sample_count = 0;
    previous_history_index = (history_index + 3) % 4;
    if (previous_history_index != history_index) {
        do {
            subtract_vector_xyz(&relative_velocity, &D_0013E550.listener_history[history_index], &D_0013E550.listener_history[previous_history_index]);
            if (!(vector_length_xyz(&relative_velocity) < D_0015ED6C * 60.0f)) {
                break;
            }
            listener_sample_count++;
            add_vector_xyz(&listener_velocity, &listener_velocity, &relative_velocity);
            history_index = previous_history_index;
            previous_history_index = (history_index + 3) % 4;
        } while (previous_history_index != D_0013E550.listener_history_position);
    }
    if (listener_sample_count >= 2) {
        scale_vector_xyz(&listener_velocity, &listener_velocity, 1.0f / ConvertIntegerToFloat(listener_sample_count));
    }

    if (D_0015F604 == 2) {
        snd_set_master_volume(1, 0);
        snd_set_master_volume(0, D_0013E550.group_0_volume / 2);
        snd_set_master_volume(3, D_0013E550.group_3_volume / 2);
    } else {
        group_volume = D_0013E550.group_1_volume;
        if (underwater) {
            group_volume = group_volume * 3 / 5;
        }
        snd_set_master_volume(1, group_volume);
        snd_set_master_volume(0, D_0013E550.group_0_volume);
        snd_set_master_volume(3, D_0013E550.group_3_volume);
    }
    snd_set_master_volume(2, D_0013E550.group_2_volume);
    snd_set_master_volume(4, D_0013E550.group_4_volume);
    snd_set_master_volume(5, D_0013E550.group_5_volume);

    FillTransferWords(voice_flags, 0, 0x78);
    FillTransferWords(volumes, 0, 0x78);
    FillTransferWords(radial_velocities, 0, 0x78);

    for (slot_index = 0; slot_index < 30; slot_index++) {
        if (D_0013E550.voices[slot_index].state != 7) {
            if (D_0013E550.voices[slot_index].handle == 0) {
                continue;
            }
            if (D_0013E550.voices[slot_index].handle == -1) {
                continue;
            }
        }
        moby = D_0013E550.voices[slot_index].moby;
        owner_removed = 0;
        if (moby != NULL && (moby->state == 0xFE || moby->state == 0xFD)) {
            owner_removed = 1;
        }
        if (owner_removed) {
            D_0013E550.voices[slot_index].moby = NULL;
        }
        if (D_0013E550.voices[slot_index].state == 4 || (D_0013E550.voices[slot_index].state != 6 && owner_removed && D_0013E550.voices[slot_index].definition->source_state != 0)) {
            voice_flags[slot_index] = 0x20;
            continue;
        }
        if (D_0015F604 != 0 && D_0015F604 != 2 && D_0015F604 != 6 && D_0013E550.voices[slot_index].state != 7) {
            voice_flags[slot_index] = 0x10;
            continue;
        }

        if (D_0013E550.voices[slot_index].moby != NULL && !(D_0013E550.voices[slot_index].flags & 8)) {
            if (D_0013E550.voices[slot_index].flags & 0x40) {
                func_001F9CF8(&direction, &D_0013E550.voices[slot_index].position_offset, &moby->rotation);
                add_vector_xyz(&direction, &direction, &moby->position);
                subtract_vector_xyz(&relative_velocity, &direction, &D_0013E550.voices[slot_index].position);
                qcopy(&D_0013E550.voices[slot_index].position, &direction);
            } else {
                D_0013E550.voices[slot_index].position.f[2] -= 1.0f;
                subtract_vector_xyz(&relative_velocity, &D_0013E550.voices[slot_index].moby->position, &D_0013E550.voices[slot_index].position);
                qcopy(&D_0013E550.voices[slot_index].position, &D_0013E550.voices[slot_index].moby->position);
                D_0013E550.voices[slot_index].position.f[2] += 1.0f;
            }
        } else {
            qzero(&relative_velocity);
        }
        subtract_vector_xyz(&relative_velocity, &relative_velocity, &listener_velocity);
        subtract_vector_xyz(&direction, &D_00187080, &D_0013E550.voices[slot_index].position);
        normalize_vector_xyz(&direction, &direction, 1.0f);
        radial_velocities[slot_index] = dot_vectors_xyz(&direction, &relative_velocity);

        if (!(D_0013E550.voices[slot_index].flags & 0x10)) {
            distance_volume = calculate_voice_volume(&D_0013E550.voices[slot_index], &D_0013E550.voices[slot_index].position);
            volumes[slot_index] = distance_volume * D_0013E550.voices[slot_index].volume / 1024;
            if (!(D_0013E550.voices[slot_index].definition->attenuation_flags & 2)) {
                voice_flags[slot_index] |= 8;
            }
        } else {
            distance_volume = 0x400;
            volumes[slot_index] = D_0013E550.voices[slot_index].volume;
        }
        if (underwater && !(D_0013E550.voices[slot_index].definition->attenuation_flags & 4) && water_height < D_0013E550.voices[slot_index].position.f[2]) {
            volumes[slot_index] /= 2;
        }
        voice_flags[slot_index] |= 1;
        if (distance_volume < 0x20 && volumes[slot_index] < 0x20 && (D_0013E550.voices[slot_index].flags & 4)) {
            voice_flags[slot_index] = 0x20;
            continue;
        }
        if (!(D_0013E550.voices[slot_index].flags & 1)) {
            voice_flags[slot_index] |= 2;
            if (!(D_0013E550.voices[slot_index].flags & 0x20)) {
                voice_flags[slot_index] |= 4;
            }
        }
    }

    for (slot_index = 0; slot_index < 30; slot_index++) {
        if (!(voice_flags[slot_index] & 8)) {
            continue;
        }
        if (D_0013E550.voices[slot_index].state == 7) {
            if ((D_0015F604 != 0 && D_0015F604 != 2) || D_0015F5E8 != 0) {
                continue;
            }
            if (D_0015F60C != D_0013E550.occlusion_frame) {
                for (history_offset = 0; history_offset < 6; history_offset++) {
                    clamp_voice_position_to_collision(&D_0013E550.occlusion_samples[history_offset]);
                }
                D_0013E550.occlusion_frame = D_0015F60C;
            }
            occluded_samples = 0;
            for (history_offset = 0, sample_index = 0; history_offset < 36; history_offset += 6, sample_index++) {
                D_0013E550.voices[slot_index].occlusion_history[history_offset] = test_voice_occlusion(&D_0013E550.voices[slot_index], &D_0013E550.occlusion_samples[sample_index]);
                for (repeat_index = 1; repeat_index < 6; repeat_index++) {
                    D_0013E550.voices[slot_index].occlusion_history[history_offset + repeat_index] = D_0013E550.voices[slot_index].occlusion_history[history_offset];
                }
                if (D_0013E550.voices[slot_index].occlusion_history[history_offset]) {
                    occluded_samples += 6;
                }
            }
            if (occluded_samples >= 36) {
                volumes[slot_index] = 0;
            } else if (occluded_samples >= 19) {
                volumes[slot_index] = (36 - occluded_samples) * volumes[slot_index] / 18;
            }
        } else {
            if ((D_0015F604 == 0 || D_0015F604 == 2) && D_0015F5E8 == 0) {
                D_0013E550.voices[slot_index].occlusion_history_position = (D_0013E550.voices[slot_index].occlusion_history_position + 1) % 36;
                /* Active sources test every other frame; other voices every fourth. */
                command_flags = (slot_index ^ D_0015F60C) & 1;
                if (!(D_0013E550.voices[slot_index].flags & 4)) {
                    command_flags = ((D_0015F60C ^ slot_index) & 3) == 0;
                }
                if (command_flags) {
                    D_0013E550.voices[slot_index].occlusion_history[D_0013E550.voices[slot_index].occlusion_history_position] = test_voice_occlusion(&D_0013E550.voices[slot_index], &listener_occlusion_position);
                } else {
                    D_0013E550.voices[slot_index].occlusion_history[D_0013E550.voices[slot_index].occlusion_history_position] = D_0013E550.voices[slot_index].occlusion_history[(D_0013E550.voices[slot_index].occlusion_history_position + 35) % 36];
                }
            }
            occluded_samples = ComputeByteStringHash(D_0013E550.voices[slot_index].occlusion_history, 36);
            if (occluded_samples >= 36) {
                volumes[slot_index] = 0;
            } else if (occluded_samples >= 19) {
                volumes[slot_index] = (36 - occluded_samples) * volumes[slot_index] / 18;
            }
        }
    }

    func_001FA2D8(listener_matrix, D_00187290);
    for (slot_index = 0; slot_index < 30; slot_index++) {
        command_flags = voice_flags[slot_index];
        if (command_flags == 0) {
            continue;
        }
        handle = D_0013E550.voices[slot_index].handle;
        D_0013E550.voices[slot_index].handle = -1;
        if (command_flags & 0x20) {
            if (D_0013E550.voices[slot_index].state == 7) {
                D_0013E550.voices[slot_index].state = 0;
                D_0013E550.voices[slot_index].moby = NULL;
                D_0013E550.voices[slot_index].owner_context = 0;
            } else {
                snd_stop_sound(handle);
                D_0013E550.voices[slot_index].state = 6;
                snd_sound_is_still_playing_cb(handle, voice_update_callback, &D_0013E550.voices[slot_index]);
            }
        } else if (command_flags & 0x10) {
            snd_sound_is_still_playing_cb(handle, voice_update_callback, &D_0013E550.voices[slot_index]);
        } else {
            pitch_bend = D_0013E550.voices[slot_index].pitch_bend;
            parameter_mask = pitch_bend ? 0x11 : 1;
            pan = 0;
            pitch_modifier = 0;
            if (command_flags & 2) {
                pan = calculate_voice_pan(&D_0013E550.voices[slot_index], &D_0013E550.voices[slot_index].position, listener_matrix);
                parameter_mask |= 6;
            }
            if (voice_flags[slot_index] & 4) {
                parameter_mask |= 8;
                pitch_modifier = ComputeSectorIndex(truncate_float_to_s32(radial_velocities[slot_index] * 300.0f));
            }
            if (underwater && !(D_0013E550.voices[slot_index].definition->attenuation_flags & 8)) {
                parameter_mask |= 8;
                pitch_modifier -= 0x5F4;
            }
            if (D_0013E550.voices[slot_index].state != 7) {
                snd_set_sound_params_cb(handle, parameter_mask, volumes[slot_index], pan, pitch_modifier, pitch_bend, voice_update_callback, &D_0013E550.voices[slot_index]);
            } else {
                D_0013E550.voices[slot_index].state = 1;
                snd_play_sound_vol_pan_pmpb(D_0013E550.voices[slot_index].definition->bank_handle, D_0013E550.voices[slot_index].definition->sound_index, volumes[slot_index], pan, pitch_modifier, pitch_bend, voice_start_callback, &D_0013E550.voices[slot_index]);
            }
        }
    }

    music_update();
    snd_reset_state_and_flush_commands();
    snd_flush_sound_commands();
    ReadGlobalTableEntry();
    D_0013E550.pending_commands = 0;
    return 0;
}
#endif /* NON_MATCHING */

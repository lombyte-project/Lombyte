#ifndef LOMBYTE_RNC_AUDIO_VOICE_STATE_H
#define LOMBYTE_RNC_AUDIO_VOICE_STATE_H

/* Game-side request states, not 989SND hardware voice states.
 * sound_update sends the start/stop commands and callbacks finish them.
 * Only the states used by allocation/release are named here. */
#define VOICE_SLOT_FREE           0
#define VOICE_SLOT_STOP_REQUESTED 4
#define VOICE_SLOT_STOP_PENDING   6
#define VOICE_SLOT_START_QUEUED   7

#endif /* LOMBYTE_RNC_AUDIO_VOICE_STATE_H */

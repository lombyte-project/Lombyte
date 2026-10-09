#ifndef LOMBYTE_RNC_AUDIO_SOUND_READ_WORK_H
#define LOMBYTE_RNC_AUDIO_SOUND_READ_WORK_H

#include "types.h"

/* State of the sound system's disc read: set when a read starts, cleared by its callback. */
struct StartSoundWork {
    volatile s32 read_active;
    u8 pad_4[0xC];
    volatile s32 read_error;
};

extern struct StartSoundWork sound_read_work __asm__("D_00137B00");

#endif /* LOMBYTE_RNC_AUDIO_SOUND_READ_WORK_H */

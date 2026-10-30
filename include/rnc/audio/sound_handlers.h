#ifndef LOMBYTE_RNC_AUDIO_SOUND_HANDLERS_H
#define LOMBYTE_RNC_AUDIO_SOUND_HANDLERS_H

#include "types.h"

/* One per sound emitter type a level program defines (its lvl.sndvtbl
   section): resolve_lookup_record finds a record's type and stores its
   handler. A -1 id ends the list. */
struct SoundHandler {
    s32   id;
    void *handler;
};

#endif /* LOMBYTE_RNC_AUDIO_SOUND_HANDLERS_H */

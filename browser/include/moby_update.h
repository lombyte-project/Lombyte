#ifndef RNC_BROWSER_MOBY_UPDATE_H
#define RNC_BROWSER_MOBY_UPDATE_H

/* Portable form of include/moby_update.h's asm call: `if (update) update(moby);`.
   The call is indirect with one pointer argument, so a handler defined with a
   different parameter list traps in WebAssembly instead of ignoring the mismatch. */
#define MOBY_CALL_UPDATE(update, moby)                                 \
    do {                                                               \
        void (*moby_update_)(void *) = (void (*)(void *))(update);     \
        if (moby_update_) {                                            \
            moby_update_((void *)(moby));                              \
        }                                                              \
    } while (0)

#endif

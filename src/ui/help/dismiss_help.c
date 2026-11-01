/* Ported from rac1-decomp (src/game/help.c, func_001FE438). */
#include "rnc/ui/help/window_state.h"

extern char help_window_state[] __asm__("D_001996D0");
/* Advances the help screen's state (help_window_state). Every access goes
   through the global, and cases 6 and 7 are spelled out: the jump table
   has eight entries. */
void dismiss_help(void) __asm__("FUN_001fdc08");

void dismiss_help(void) {
    switch (((HelpWindowState *)help_window_state)->state) {
    case HELP_WINDOW_IDLE:
        ((HelpWindowState *)help_window_state)->pending_message_id = -1;
        break;
    case HELP_WINDOW_OPENING:
    case HELP_WINDOW_PROMPT:
        ((HelpWindowState *)help_window_state)->state = HELP_WINDOW_CLOSING;
        ((HelpWindowState *)help_window_state)->state_timer = 0;
        break;
    case HELP_WINDOW_EXPANDING:
        ((HelpWindowState *)help_window_state)->state = HELP_WINDOW_CLOSING;
        ((HelpWindowState *)help_window_state)->state_timer = 0;
        break;
    case HELP_WINDOW_TEXT_FADE_IN:
        ((HelpWindowState *)help_window_state)->state = HELP_WINDOW_TEXT_FADE_OUT;
        ((HelpWindowState *)help_window_state)->state_timer =
            4 - ((HelpWindowState *)help_window_state)->state_timer;
        break;
    case HELP_WINDOW_TEXT_VISIBLE:
        ((HelpWindowState *)help_window_state)->state = HELP_WINDOW_TEXT_FADE_OUT;
        ((HelpWindowState *)help_window_state)->state_timer = 0;
        break;
    case HELP_WINDOW_TEXT_FADE_OUT:
    case HELP_WINDOW_CLOSING:
        break;
    }
}

extern __typeof__(dismiss_help) func_001FDC08 __attribute__((alias("FUN_001fdc08")));

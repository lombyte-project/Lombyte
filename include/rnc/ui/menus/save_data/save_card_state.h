#ifndef LOMBYTE_RNC_UI_MENUS_SAVE_DATA_SAVE_CARD_STATE_H
#define LOMBYTE_RNC_UI_MENUS_SAVE_DATA_SAVE_CARD_STATE_H

#include "types.h"

/* Values of card_state. The names are the game's own: the levels keep a table of
 * these names right after the handler table that run_state_handler indexes. */
enum CardState {
    CS_INIT = 0,
    CS_GOOD_SAVE = 1,
    CS_WARNING = 2,
    CS_NOCARD = 3,
    CS_WAIT_FOR_CARD = 4,
    CS_UNFORMATTED = 5,
    CS_PROMPT_FORMAT = 6,
    CS_FORMAT_PENDING = 7,
    CS_FORMATTING = 8,
    CS_FORMATTED = 9,
    CS_CHECK_SAVE = 10,
    CS_CHECKING_SAVE = 11,
    CS_NOSAVE = 12,
    CS_PROMPT_CREATE_SAVE = 13,
    CS_CREATE_SAVE_PENDING = 14,
    CS_CREATING_SAVE = 15,
    CS_NEWCARD = 16,
    CS_FORMAT_FAILED = 17,
    CS_CREATE_FAILED = 18,
    CS_NO_ROOM = 19,
    CS_LOAD_FAILED = 20,
    CS_SAVE_FAILED = 21,
    CS_SAVING = 22,
    CS_PROMPT_BEGIN_UNFORMATTED = 23,
    CS_PROMPT_BEGIN_NOSAVE = 24
};

#endif /* LOMBYTE_RNC_UI_MENUS_SAVE_DATA_SAVE_CARD_STATE_H */

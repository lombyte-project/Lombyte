#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/movies/decoder/decoder_globals.h"

void end_display(void) __asm__("ClearStageStateFlag");

void end_display(void) {
    movie_display_active = 0;
}

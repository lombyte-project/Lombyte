#include "types.h"

char *FindSubstring(char *search_string, char *target_substring) {
    s32 i;

    if (*search_string == 0) {
        return *target_substring == 0 ? search_string : 0;
    }
    do {
        i = 0;
        while (1) {
            if (target_substring[i] == 0) {
                return search_string;
            }
            if (target_substring[i] != search_string[i]) {
                break;
            }
            i++;
        }
        search_string++;
    } while (*search_string != 0);
    return 0;
}

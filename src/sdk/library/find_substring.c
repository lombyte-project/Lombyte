#include "types.h"
char *FindSubstring(char *search_string, char *sub) {
    s32 i;

    if (*search_string == 0) {
        return *sub == 0 ? search_string : 0;
    }
    do {
        i = 0;
        while (1) {
            if (sub[i] == 0) {
                return search_string;
            }
            if (sub[i] != search_string[i]) {
                break;
            }
            i++;
        }
        search_string++;
    } while (*search_string != 0);
    return 0;
}

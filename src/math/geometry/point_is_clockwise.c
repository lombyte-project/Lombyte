#include "rnc1_functions.h"

/* Sign of the 2D cross product (a - c) x (t - c): 1 when negative.
   The product is a block of its own (a do/while (0), as a macro would
   expand), which keeps the two leading differences ahead of it, as in
   retail. */
int point_is_clockwise(int a0, int a1, int a2, int a3, int t0, int t1) __asm__("func_00208818");
int point_is_clockwise(int first_point_x, int first_point_y, int reference_x, int reference_y, int second_point_x, int second_point_y) {
    int dx;
    int dy;

    dx = first_point_x - reference_x;
    dy = first_point_y - reference_y;
    /* Fakematch: the block keeps the two differences below dx and dy. */
    do {
        int term1 = (second_point_x - reference_x) * dy;
        int term2 = (second_point_y - reference_y) * dx;

        if (term1 - term2 < 0) {
            return 1;
        }
        return 0;
    } while (0);
}

#include "rnc1_functions.h"

/* Sign of the 2D cross product (a - c) x (t - c): 1 when negative.
   The product is a block of its own (a do/while (0), as a macro would
   expand), which keeps the two leading differences ahead of it, as in
   retail. */
int Func00208818(int a0, int a1, int a2, int a3, int t0, int t1)
{
    int dx;
    int dy;

    dx = a0 - a2;
    dy = a1 - a3;
    do {
        int term1 = (t0 - a2) * dy;
        int term2 = (t1 - a3) * dx;

        if (term1 - term2 < 0) {
            return 1;
        }
        return 0;
    } while (0);
}

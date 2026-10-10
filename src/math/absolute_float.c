/* Return the absolute value of an EE single-precision value. */

float absolute_float(float input) __asm__("func_001F99C0");

float absolute_float(float input) {
    return __builtin_fabsf(input);
}

typedef struct Object Object;

struct Object {
    unsigned char pad0[16];
    int           state;
    int           value;
};

Object *_Balloc(Object *object, int index) __asm__("func_001154D0");

Object *__i2b(Object *unused, Object *object) __asm__("func_00115808");

Object *__i2b(Object *unused, Object *initial_bigint_value) {
    enum {
        BIGINT_SINGLE_WORD_COUNT = 1
    };
    Object *allocated_bigint = _Balloc(unused, 1);
    allocated_bigint->value = (int)initial_bigint_value;
    allocated_bigint->state = BIGINT_SINGLE_WORD_COUNT;
    return allocated_bigint;
}

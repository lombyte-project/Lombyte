typedef struct Resource Resource;

struct Resource {
    unsigned char pad0[14];
    short         index;
    unsigned char pad10[68];
    int           value;
};

int _close_r(int value, int index) __asm__("func_00112468");

int __sclose(Resource *resource) __asm__("func_00116408");

int __sclose(Resource *resource) {
    return _close_r(resource->value, resource->index);
}

typedef struct Resource Resource;
struct Resource {
    unsigned char pad0[14];
    short index;
    unsigned char pad10[68];
    int value;
};

int fun_00112468(int value, int index) __asm__("func_00112468");

int resolve_resource_index(Resource *resource) __asm__("func_00116408");

int resolve_resource_index(Resource *resource) {
    return fun_00112468(resource->value, resource->index);
}

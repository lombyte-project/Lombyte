typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

typedef union ImageDimension {
    struct {
        u16 low_half;
        u16 high_half;
    } halves;
    s32 whole;
} ImageDimension;

typedef struct ImageDescriptorInput {
    u8 reserved[0x20];
    ImageDimension width;
    ImageDimension height;
} ImageDescriptorInput;

typedef struct ImageDescriptorOutput {
    u16 zero0;
    u16 height_half;
    u16 zero4;
    u16 width_half;
    u16 width_shifted;
    u16 height_shifted;
    u16 unusedC;
    u16 unusedE;
    u16 constant10;
    u16 zero12;
} ImageDescriptorOutput;

void PackImageDescriptor(ImageDescriptorOutput *output, ImageDescriptorInput *input) {
    output->zero0 = 0;
    output->height_half = input->height.halves.low_half;
    output->zero4 = 0;
    output->width_half = input->width.halves.low_half;
    output->width_shifted = input->width.whole >> 1;
    output->height_shifted = input->height.whole >> 1;
    output->constant10 = 0x10;
    output->zero12 = 0;
}

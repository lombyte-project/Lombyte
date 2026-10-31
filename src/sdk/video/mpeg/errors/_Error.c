typedef unsigned int u32;

struct ErrorContext {
    unsigned char opaque;
};

#define FIELD(type, object, offset) (*(type *)((unsigned char *)(object) + (offset)))

void _dispatchMpegCallback(u32 state, int *arguments) __asm__("_dispatchMpegCallback");
void print_mpeg_error(int argument) __asm__("JumpToImageSetup");

void _Error(struct ErrorContext *context, int argument) __asm__("_Error");

void _Error(struct ErrorContext *decoder_state, int argument) {
    u32 state;
    int callback_arguments[2];

    state = FIELD(u32, decoder_state, 0x858);
    if (state != 0 && decoder_state != 0 && FIELD(u32, decoder_state, 0x0C) != 0) {
        callback_arguments[1] = argument;
        callback_arguments[0] = 0;
        _dispatchMpegCallback(state, callback_arguments);
    } else {
        print_mpeg_error(argument);
    }
}

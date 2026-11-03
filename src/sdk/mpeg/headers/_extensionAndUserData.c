#include "types.h"
#include "rnc/sdk/mpeg/bitstream.h"

typedef void (*ExtFunc)(void *);

typedef struct {
    ExtFunc f[11];
} ExtTable;

extern void report_unknown_mpeg_extension(void *) __asm__("FUN_0012cb00");
extern void _sequenceExtension(void *);
extern void _sequenceDisplayExtension(void *);
extern void _quantMatrixExtension(void *);
extern void _copyrightExtension(void *);
extern void report_mpeg_sequence_scalable_unsupported(void *) __asm__("FUN_0012caf0");
extern void _pictureDisplayExtension(void *);
extern void _pictureCodingExtension(void *);
extern void report_mpeg_picture_spatial_scalable_unsupported(void *) __asm__("FUN_0012cb10");
extern void report_mpeg_picture_temporal_scalable_unsupported(void *) __asm__("FUN_0012cb20");

/* Parser by extension_start_code_identifier (ISO 13818-2 table 6-2). */
const ExtTable mpeg_extension_dispatch_table __asm__("D_001538B8") = {
    {
     report_unknown_mpeg_extension,                     /* 0: reserved */
        _sequenceExtension,                                /* 1: sequence */
        _sequenceDisplayExtension,                         /* 2: sequence display */
        _quantMatrixExtension,                             /* 3: quant matrix */
        _copyrightExtension,                               /* 4: copyright */
        report_mpeg_sequence_scalable_unsupported,         /* 5: sequence scalable */
        report_unknown_mpeg_extension,                     /* 6: reserved */
        _pictureDisplayExtension,                          /* 7: picture display */
        _pictureCodingExtension,                           /* 8: picture coding */
        report_mpeg_picture_spatial_scalable_unsupported,  /* 9: picture spatial scalable */
        report_mpeg_picture_temporal_scalable_unsupported, /* 10: picture temporal scalable */
    }
};

void _extensionAndUserData(void *decoder_state) {
    ExtTable extension_handlers = mpeg_extension_dispatch_table;
    u32      code;
    u32      id;

    _nextStartCode(decoder_state);
    while ((code = _peepBit(decoder_state, 32)) == 0x1B5 || code == 0x1B2) {
        if (code == 0x1B5) {
            _flushBuf(decoder_state, 32);
            id = _nextBit(decoder_state, 4);
            id = id > 10 ? 0 : id;
            extension_handlers.f[id](decoder_state);
            _nextStartCode(decoder_state);
        } else {
            _flushBuf(decoder_state, 32);
            _nextStartCode(decoder_state);
        }
    }
}

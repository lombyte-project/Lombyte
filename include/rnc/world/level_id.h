#ifndef LOMBYTE_RNC_WORLD_LEVEL_ID_H
#define LOMBYTE_RNC_WORLD_LEVEL_ID_H

/* Values of current_level_index, in the order of the level table
 * (config/us/levels/levels.json). Folders src/overlays/lNN use the same numbers. */
enum LevelId {
    LEVEL_VELDIN1 = 0x0,
    LEVEL_NOVALIS = 0x1,
    LEVEL_ARIDIA = 0x2,
    LEVEL_KERWAN = 0x3,
    LEVEL_EUDORA = 0x4,
    LEVEL_RILGAR = 0x5,
    LEVEL_NEBULA_G34 = 0x6,
    LEVEL_UMBRIS = 0x7,
    LEVEL_BATALIA = 0x8,
    LEVEL_GASPAR = 0x9,
    LEVEL_ORXON = 0xA,
    LEVEL_POKITARU = 0xB,
    LEVEL_HOVEN = 0xC,
    LEVEL_OLTANIS_ORBIT = 0xD,
    LEVEL_OLTANIS = 0xE,
    LEVEL_QUARTU = 0xF,
    LEVEL_KALEBO3 = 0x10,
    LEVEL_VELDIN_ORBIT = 0x11,
    LEVEL_VELDIN2 = 0x12,
    LEVEL_COUNT = 0x13
};

#endif /* LOMBYTE_RNC_WORLD_LEVEL_ID_H */

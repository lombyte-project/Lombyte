#ifndef LOMBYTE_SDA_H
#define LOMBYTE_SDA_H

/*
 * Small-data placement for externals, per declaration.
 *
 * The game addresses some globals through $gp (small data) and the rest with
 * a %hi/%lo pair. Three spellings cover what retail does:
 *
 *   __attribute__((sda))  the variable is small data: loads and stores are
 *                         gp-relative (compiler patches 0044/0045/0056).
 *   NOT_SDA               keeps a variable out of small data whatever its size,
 *                         so it is reached with lui + %lo.
 *   MACRO_ADDR            the compiler emits the unsplit assembler macro
 *                         (`lw $2,D`), which the assembler expands through the
 *                         destination register: retail's one-register
 *                         `lui $2,%hi(D)` / `lw $2,%lo(D)($2)` form. Loads only;
 *                         a store becomes a two-instruction $at macro.
 */
#define NOT_SDA    __attribute__((section(".data")))
#define MACRO_ADDR __attribute__((section(".sdata")))

#endif /* LOMBYTE_SDA_H */
